/*
MIT License

Copyright (c) 2021 Attila Csikós

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#pragma once

#include <algorithm>
#include <bit>
#include <cassert>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <iterator>
#include <limits>
#include <memory>
#include <span>
#include <type_traits>
#include <utility>
#include <vector>

namespace OrthoTree::detail
{

// ============================================================================
// RawPage
// Typed heap buffer without object-lifetime management.
// Construction / destruction of T is the caller's responsibility, except when
// T is default-constructible and non-trivially-copyable (handled automatically).
// ============================================================================
template<typename T>
struct RawPage
{
  T*          data     = nullptr;
  std::size_t size     = 0;      // logical used size
  std::size_t capacity = 0;      // physical allocation size

  RawPage() = default;
  RawPage(RawPage const&)            = delete;
  RawPage& operator=(RawPage const&) = delete;

  RawPage(RawPage&& o) noexcept : data(o.data), size(o.size), capacity(o.capacity)
  {
    o.data = nullptr;
    o.size = o.capacity = 0;
  }

  RawPage& operator=(RawPage&& o) noexcept
  {
    if (this != &o)
    {
      Free();
      data = o.data; size = o.size; capacity = o.capacity;
      o.data = nullptr; o.size = o.capacity = 0;
    }
    return *this;
  }

  ~RawPage() { Free(); }

  void Allocate(std::size_t cap)
  {
    Free();
    if (cap == 0)
      return;
    data     = std::allocator<T>{}.allocate(cap);
    capacity = cap;
    size     = cap;
    if constexpr (!std::is_trivially_copyable_v<T> && std::is_default_constructible_v<T>)
      std::uninitialized_default_construct_n(data, cap);
  }

  void Free() noexcept
  {
    if (!data)
      return;
    if constexpr (!std::is_trivially_copyable_v<T> && std::is_default_constructible_v<T> && !std::is_trivially_destructible_v<T>)
      std::destroy_n(data, size);
    std::allocator<T>{}.deallocate(data, capacity);
    data = nullptr; size = capacity = 0;
  }

  // Grow-only resize: allocates more if needed, default-constructs new elements.
  void Resize(std::size_t newSize)
  {
    if (newSize > capacity)
    {
      T* nd = std::allocator<T>{}.allocate(newSize);
      if (data && size > 0)
      {
        if constexpr (std::is_trivially_copyable_v<T>)
          std::memcpy(nd, data, size * sizeof(T));
        else
          std::uninitialized_move_n(data, size, nd);
      }
      if constexpr (!std::is_trivially_copyable_v<T> && std::is_default_constructible_v<T>)
        std::uninitialized_default_construct_n(nd + size, newSize - size);
      Free();
      data     = nd;
      capacity = newSize;
    }
    else if constexpr (!std::is_trivially_copyable_v<T> && std::is_default_constructible_v<T>)
    {
      if (newSize > size)
        std::uninitialized_default_construct_n(data + size, newSize - size);
      else if constexpr (!std::is_trivially_destructible_v<T>)
        std::destroy_n(data + newSize, size - newSize);
    }
    size = newSize;
  }
};


// ============================================================================
// FixedBufferAllocator
// Single pre-allocated buffer + address-sorted free list.
//   • TryAllocate  : O(k) first-fit linear scan (k = free-segment count, typically < 32)
//   • TryExtend    : O(log k) binary search
//   • DeallocateRange : O(log k) binary search + O(k) insert/erase (rare)
// Free list is kept sorted by begin address, enabling O(log k) neighbour
// lookup during coalescing (vs. the old capacity-sorted scheme which needed
// O(k) linear scans for FindConnecting/PrecedingFreeSegment).
// ============================================================================
template<typename T>
class FixedBufferAllocator
{
public:
#ifdef ORTHOTREE__LARGE_DATASET
  using Index = std::uint64_t;
#else
  using Index = std::uint32_t;
#endif

  static constexpr std::size_t MIN_SEGMENT_SIZE = 4;

private:
  struct FreeSegment { Index begin = 0; Index capacity = 0; };

  RawPage<T>               m_page;
  std::vector<FreeSegment> m_free; // sorted ascending by begin address

public:
  void Init(std::size_t pageSize)
  {
    m_page.Allocate(pageSize + MIN_SEGMENT_SIZE);
    m_free.reserve(16);
    m_free.push_back({ 0, Index(m_page.size) });
  }

  T*          DataBegin() const noexcept { return m_page.data; }
  std::size_t Capacity()  const noexcept { return m_page.capacity; }

  std::size_t FreeCapacity() const noexcept
  {
    std::size_t s = 0;
    for (auto const& f : m_free) s += f.capacity;
    return s;
  }

  bool Owns(T const* ptr) const noexcept
  {
    return ptr >= m_page.data && ptr < m_page.data + m_page.size;
  }

  // First-fit allocation: O(k) scan. Small leftover fragments (< MIN_SEGMENT_SIZE)
  // are consumed whole to avoid unusable slivers.
  std::span<T> TryAllocate(std::size_t n) noexcept
  {
    for (auto it = m_free.begin(); it != m_free.end(); ++it)
    {
      if (it->capacity < Index(n))
        continue;

      T*         ptr       = m_page.data + it->begin;
      auto const remaining = it->capacity - Index(n);

      if (remaining < MIN_SEGMENT_SIZE)
      {
        m_free.erase(it); // consume whole segment, no tiny leftover
      }
      else
      {
        it->begin    += Index(n); // trim from front; order preserved
        it->capacity  = remaining;
      }
      return { ptr, n };
    }
    return {};
  }

  // O(log k): extend seg into the immediately following free segment.
  bool TryExtend(std::span<T>& seg, std::size_t increase) noexcept
  {
    if (seg.empty())
      return false;
    auto const endIdx    = IndexOf(seg.data()) + Index(seg.size());
    auto const increase_ = Index(increase);

    auto it = LowerBoundBegin(endIdx);
    if (it == m_free.end() || it->begin != endIdx || it->capacity < increase_)
      return false;

    it->begin    += increase_;
    it->capacity -= increase_;
    if (it->capacity == 0)
      m_free.erase(it);
    seg = std::span<T>(seg.data(), seg.size() + increase);
    return true;
  }

  // Return the tail `sizeDecrease` elements back to the free list and shrink seg.
  void Trim(std::span<T>& seg, std::size_t sizeDecrease) noexcept
  {
    assert(seg.size() > sizeDecrease);
    DeallocateRange(std::span<T>(seg.data() + seg.size() - sizeDecrease, sizeDecrease));
    seg = seg.first(seg.size() - sizeDecrease);
  }

  void Deallocate(std::span<T> seg) noexcept
  {
    if (!seg.empty())
      DeallocateRange(seg);
  }

  void Reset() noexcept
  {
    m_page.Free();
    m_free.clear();
  }

  // Copy-compact all non-empty spans into a single contiguous block at the
  // start of this page. Updates each span pointer in-place.
  // Precondition: this allocator is fresh / reset.
  void CompactInto(std::vector<std::span<T>*>& spans, std::size_t totalSize)
  {
    m_page.Resize(totalSize);
    T* dest = m_page.data;
    for (auto* s : spans)
    {
      if (s->empty())
        continue;
      auto const n = s->size();
      if constexpr (std::is_trivially_copyable_v<T>)
        std::memcpy(dest, s->data(), n * sizeof(T));
      else
        std::uninitialized_copy_n(s->data(), n, dest);
      *s = std::span<T>(dest, n);
      dest += n;
    }
    m_free.clear();
    std::size_t remaining = m_page.capacity - totalSize;
    if (remaining > MIN_SEGMENT_SIZE)
      m_free.push_back({ Index(totalSize), Index(remaining) });
  }

private:
  Index IndexOf(T const* ptr) const noexcept { return Index(ptr - m_page.data); }

  // First free segment with begin >= val.
  auto LowerBoundBegin(Index val) noexcept
  {
    return std::lower_bound(m_free.begin(), m_free.end(), val,
      [](FreeSegment const& f, Index v) { return f.begin < v; });
  }

  // O(log k): return freed region to the free list, coalescing with neighbours.
  void DeallocateRange(std::span<T> seg) noexcept
  {
    auto const beginIdx = IndexOf(seg.data());
    auto const segSize  = Index(seg.size());
    auto const endIdx   = beginIdx + segSize;

    // First free segment that starts at or after the freed block's end.
    auto nextIt  = LowerBoundBegin(endIdx);
    bool hasNext = (nextIt != m_free.end()) && (nextIt->begin == endIdx);

    // The potential predecessor is the entry immediately before nextIt.
    auto prevIt  = nextIt;
    bool hasPrev = false;
    if (nextIt != m_free.begin())
    {
      --prevIt;
      hasPrev = (prevIt->begin + prevIt->capacity == beginIdx);
    }

    if (hasPrev && hasNext)
    {
      prevIt->capacity += segSize + nextIt->capacity; // merge all three
      m_free.erase(nextIt);
    }
    else if (hasPrev)
    {
      prevIt->capacity += segSize;
    }
    else if (hasNext)
    {
      nextIt->begin     = beginIdx;  // extend backwards
      nextIt->capacity += segSize;
    }
    else
    {
      m_free.insert(nextIt, FreeSegment{ beginIdx, segSize }); // new isolated block
    }

#ifdef _DEBUG
    if constexpr (std::is_integral_v<T>)
      std::fill(seg.begin(), seg.end(), std::numeric_limits<T>::max());
    assert(CheckFreeList());
#endif
  }

  [[maybe_unused]] bool CheckFreeList() const noexcept
  {
    for (std::size_t i = 1; i < m_free.size(); ++i)
      if (m_free[i - 1].begin + m_free[i - 1].capacity > m_free[i].begin)
        return false;
    return true;
  }

  template<typename T2>
  friend class MemoryResource;
};


// ============================================================================
// HeapFallbackAllocator
// Each allocation is an independent heap allocation freed immediately on
// Deallocate(). The capacity is implicit in the span size — no auxiliary
// bookkeeping map needed, eliminating the unordered_map overhead.
// ============================================================================
template<typename T>
class HeapFallbackAllocator
{
public:
  // Allocate n elements; span.size() == capacity (no separate tracking).
  std::span<T> Allocate(std::size_t n) noexcept
  {
    T* ptr = std::allocator<T>{}.allocate(n);
    if constexpr (!std::is_trivially_copyable_v<T> && std::is_default_constructible_v<T>)
      std::uninitialized_default_construct_n(ptr, n);
    return { ptr, n };
  }

  // Immediately free the allocation; seg.size() is used as the capacity.
  void Deallocate(std::span<T> seg) noexcept
  {
    if (seg.empty())
      return;
    if constexpr (!std::is_trivially_copyable_v<T> && !std::is_trivially_destructible_v<T>)
      std::destroy_n(seg.data(), seg.size());
    std::allocator<T>{}.deallocate(seg.data(), seg.size());
  }

  // Reallocate to newSize, preserving existing data. Old span is freed.
  std::span<T> Reallocate(std::span<T> old, std::size_t newSize) noexcept
  {
    if (newSize == 0) { Deallocate(old); return {}; }
    auto const copy = std::min(old.size(), newSize);
    T* nd = std::allocator<T>{}.allocate(newSize);
    if constexpr (std::is_trivially_copyable_v<T>)
      std::memcpy(nd, old.data(), copy * sizeof(T));
    else
      std::uninitialized_move_n(old.data(), copy, nd);
    if constexpr (!std::is_trivially_copyable_v<T> && std::is_default_constructible_v<T>)
      if (newSize > copy)
        std::uninitialized_default_construct_n(nd + copy, newSize - copy);
    Deallocate(old);
    return { nd, newSize };
  }

  // Ownership is tracked by MemorySegment::pageID, not here.
  bool        Owns(T const*)  const noexcept { return false; }
  std::size_t TotalCapacity() const noexcept { return 0; }

  // No-op: callers must Deallocate() each segment individually.
  // (MemoryResource guarantees all segments are freed before Reset/destruction.)
  void Reset() noexcept {}

  template<typename T2>
  friend class MemoryResource;
};


// ============================================================================
// MemoryResource
// Chains FixedBufferAllocator (primary, cache-friendly pre-allocated buffer)
// and HeapFallbackAllocator (fallback, zero-leak individual heap allocations).
//
// PageID == PRIMARY_PAGEID  → owned by FixedBufferAllocator
// PageID == FALLBACK_PAGEID → owned by HeapFallbackAllocator
// ============================================================================
template<typename T>
class MemoryResource
{
public:
#ifdef ORTHOTREE__LARGE_DATASET
  using Index = std::uint64_t;
#else
  using Index = std::uint32_t;
#endif

  using PageID = std::uint32_t;

  static constexpr PageID PRIMARY_PAGEID  = 0;
  static constexpr PageID FALLBACK_PAGEID = 1;
  static constexpr PageID MAIN_PAGEID     = PRIMARY_PAGEID; // legacy alias for serialization
  static constexpr PageID INVALID_PAGEID  = std::numeric_limits<PageID>::max();

  static constexpr std::size_t MIN_SEGMENT_SIZE  = FixedBufferAllocator<T>::MIN_SEGMENT_SIZE;
  static constexpr std::size_t DEFAULT_PAGE_SIZE = 4096 / sizeof(T);

  using Segment = std::span<T>;

  struct MemorySegment
  {
    PageID  pageID  = PRIMARY_PAGEID;
    Segment segment;
  };

  // ---- Lifecycle -----------------------------------------------------------

  MemoryResource() = default;
  MemoryResource(MemoryResource const&) = delete;
  MemoryResource(MemoryResource&&)      = default;
  MemoryResource& operator=(MemoryResource&&) = default;

  void Init(std::size_t firstPageSize = DEFAULT_PAGE_SIZE) noexcept
  {
    m_primary.Init(firstPageSize);
  }

  void Reset() noexcept
  {
    m_primary.Reset();
    m_fallback.Reset();
  }

  // ---- Capacity queries ----------------------------------------------------

  std::size_t GetCapacity() const noexcept
  {
    return m_primary.Capacity() + m_fallback.TotalCapacity();
  }

  std::size_t GetFreeCapacity() const noexcept
  {
    return m_primary.FreeCapacity();
    // Fallback allocations are always 100 % utilized from the caller's view.
  }

  std::size_t GetSize() const noexcept { return GetCapacity() - GetFreeCapacity(); }

  // ---- Core operations -----------------------------------------------------

  MemorySegment Allocate(std::size_t capacity) noexcept
  {
    ORTHOTREE_CRASH_IF(capacity > std::numeric_limits<Index>::max(),
      "Too many elements. Use ORTHOTREE__LARGE_DATASET!");

    // Hot path: fixed primary buffer (cache-friendly, no heap round-trip)
    if (auto seg = m_primary.TryAllocate(capacity); !seg.empty())
      return { PRIMARY_PAGEID, seg };

    // Cold path: individual heap allocation — freed precisely, no leak
    return { FALLBACK_PAGEID, m_fallback.Allocate(capacity) };
  }

  void Deallocate(MemorySegment const& ms) noexcept
  {
    if (ms.segment.empty())
      return;
    if (ms.pageID == PRIMARY_PAGEID)
      m_primary.Deallocate(ms.segment);
    else
      m_fallback.Deallocate(ms.segment);  // immediate, no-leak
  }

  void IncreaseSegment(MemorySegment& ms, std::size_t sizeIncrease) noexcept
  {
    if (ms.segment.empty()) { ms = Allocate(sizeIncrease); return; }

    if (ms.pageID == PRIMARY_PAGEID)
    {
      // Fast path: extend in-place into adjacent free segment
      if (m_primary.TryExtend(ms.segment, sizeIncrease))
        return;

      // Slow path: move to new allocation (may spill to fallback)
      auto newMs = Allocate(ms.segment.size() + sizeIncrease);
      CopyOrMove(ms.segment, newMs.segment);
      m_primary.Deallocate(ms.segment);
      ms = newMs;
    }
    else
    {
      // Fallback: realloc in-place (OS may extend the allocation without copy)
      ms.segment = m_fallback.Reallocate(ms.segment, ms.segment.size() + sizeIncrease);
    }
  }

  void DecreaseSegment(MemorySegment& ms, std::size_t sizeDecrease)
  {
    if (ms.segment.empty())
      return;
    assert(ms.segment.size() >= sizeDecrease);

    if (ms.segment.size() == sizeDecrease)
    {
      Deallocate(ms);
      ms.segment = {};
      return;
    }

    if (ms.pageID == PRIMARY_PAGEID)
    {
      // Return tail to the free list
      m_primary.Trim(ms.segment, sizeDecrease);
    }
    else
    {
      // Shrink heap allocation — previously this leaked the tail
      ms.segment = m_fallback.Reallocate(ms.segment, ms.segment.size() - sizeDecrease);
    }
  }

  // Copy-compact all segments into a single contiguous block in dst.
  // Updates each MemorySegment pointer to refer to the new location.
  // After this call every segment has pageID == PRIMARY_PAGEID.
  void Clone(MemoryResource& dst, std::vector<MemorySegment*> memorySegments) const noexcept
  {
    std::size_t totalSize = 0;
    for (auto* pms : memorySegments) totalSize += pms->segment.size();

    std::vector<std::span<T>*> spans;
    spans.reserve(memorySegments.size());
    for (auto* pms : memorySegments) spans.push_back(&pms->segment);

    dst.m_primary.CompactInto(spans, totalSize);
    dst.m_fallback.Reset();

    for (auto* pms : memorySegments)
      pms->pageID = PRIMARY_PAGEID;
  }

private:
  static void CopyOrMove(Segment const& src, Segment& dst) noexcept
  {
    if constexpr (std::is_trivially_copyable_v<T>)
      std::memcpy(dst.data(), src.data(), src.size() * sizeof(T));
    else
      std::uninitialized_move_n(src.data(), src.size(), dst.data());
  }

private:
  static constexpr uint32_t SERIALIZED_VERSION_ID = 0;

  template<typename TData, typename TNodeMap>
  friend class MemoryResourceSerializerProxy;

  template<typename TArchive, typename TData, typename TNodes>
  friend void serialize(TArchive& ar, MemoryResource<TData>& memoryResource, TNodes& nodes);

private:
  FixedBufferAllocator<T>  m_primary;
  HeapFallbackAllocator<T> m_fallback;
};

} // namespace OrthoTree::detail
