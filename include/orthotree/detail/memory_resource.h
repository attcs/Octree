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
  using PageID = std::uint32_t;
  static constexpr PageID PRIMARY_PAGEID = 0;
  static constexpr PageID MAIN_PAGEID = PRIMARY_PAGEID; // legacy alias for serialization
  static constexpr PageID INVALID_PAGEID = std::numeric_limits<PageID>::max();

  template<typename T>
  using MemoryBlock = std::span<T>;

  template<typename T>
  struct MemorySegment
  {
    PageID pageID = PRIMARY_PAGEID;
    MemoryBlock<T> segment;
  };

  // RawPage: std::vector<T> like container without initialization
  template<typename T, typename Allocator = std::allocator<T>>
  class RawPage
  {
  public:
    constexpr RawPage() noexcept = default;
    constexpr RawPage(RawPage const&) = delete;
    constexpr RawPage& operator=(RawPage const&) = delete;

    constexpr RawPage(RawPage&& o) noexcept
    : m_data(o.m_data)
    , m_size(o.m_size)
    , m_capacity(o.m_capacity)
    {
      o.m_data = nullptr;
      o.m_size = o.m_capacity = 0;
    }

    constexpr RawPage& operator=(RawPage&& o) noexcept
    {
      if (this != &o)
      {
        Free();
        m_data = o.m_data;
        m_size = o.m_size;
        m_capacity = o.m_capacity;
        o.m_data = nullptr;
        o.m_size = o.m_capacity = 0;
      }
      return *this;
    }

    constexpr ~RawPage() noexcept { Free(); }

    constexpr void Allocate(std::size_t size, std::size_t capacity = -1) noexcept
    {
      Free();
      if (size == 0)
        return;

      m_capacity = capacity == -1 ? CalcCapacity(size) : capacity;
      m_data = Allocator{}.allocate(m_capacity);
      m_size = size;
      if constexpr (!std::is_trivially_copyable_v<T> && std::is_default_constructible_v<T>)
        std::uninitialized_default_construct_n(m_data, size);
    }

    constexpr void Free() noexcept
    {
      if (!m_data)
        return;

      if constexpr (!std::is_trivially_copyable_v<T> && std::is_default_constructible_v<T> && !std::is_trivially_destructible_v<T>)
        std::destroy_n(m_data, m_size);

      Allocator{}.deallocate(m_data, m_capacity);
      m_data = nullptr;
      m_size = m_capacity = 0;
    }

    // Resize: allocates more if needed (doubling capacity), default-constructs new elements.
    constexpr void Resize(std::size_t newSize) noexcept
    {
      if (newSize > m_capacity)
      {
        std::size_t newCapacity = CalcCapacity(newSize);
        T* nd = Allocator{}.allocate(newCapacity);
        if (m_data && m_size > 0)
        {
          if constexpr (std::is_trivially_copyable_v<T>)
            std::memcpy(nd, m_data, m_size * sizeof(T));
          else
            std::uninitialized_move_n(m_data, m_size, nd);
        }
        if constexpr (!std::is_trivially_copyable_v<T> && std::is_default_constructible_v<T>)
          std::uninitialized_default_construct_n(nd + m_size, newSize - m_size);
        Free();
        m_data = nd;
        m_capacity = newCapacity;
      }
      else
      {
        if (newSize > m_size)
        {
          if constexpr (!std::is_trivially_copyable_v<T> && std::is_default_constructible_v<T>)
            std::uninitialized_default_construct_n(m_data + m_size, newSize - m_size);
        }
        else if (newSize < m_size)
        {
          if constexpr (!std::is_trivially_destructible_v<T>)
            std::destroy_n(m_data + newSize, m_size - newSize);
        }
      }
      m_size = newSize;
    }

    constexpr T* Data() const noexcept { return m_data; }
    constexpr std::size_t Size() const noexcept { return m_size; }
    constexpr std::size_t Capacity() const noexcept { return m_capacity; }

  private:
    static constexpr std::size_t CalcCapacity(std::size_t size) noexcept
    {
      return std::clamp(std::bit_ceil(size), static_cast<std::size_t>(1.10 * size), static_cast<std::size_t>(1.30 * size));
    }

  private:
    T* m_data = nullptr;
    std::size_t m_size = 0;     // logical used size
    std::size_t m_capacity = 0; // physical allocation size
  };


  template<typename T>
  class AllocatorInterface
  {
  public:
    virtual ~AllocatorInterface() = default;
    virtual MemorySegment<T> Allocate(std::size_t capacity) noexcept = 0;
    virtual void Deallocate(MemorySegment<T>& segment) noexcept = 0;
    virtual bool TryToExtend(PageID pageID, MemoryBlock<T>& segment, std::size_t sizeIncrease) noexcept = 0;
    virtual bool TryToShrink(PageID pageID, MemoryBlock<T>& segment, std::size_t sizeDecrease) noexcept = 0;
    virtual void Reset() noexcept = 0;
  };


  template<typename T, std::size_t BUCKET_SIZE>
  class BucketAllocator : public AllocatorInterface<T>
  {
  public:
    static constexpr std::size_t POOL_SIZE = std::max<std::size_t>(64, 16 * 1024 / sizeof(T) / BUCKET_SIZE);
    struct Page
    {
    private:
      alignas(T) std::byte m_data[POOL_SIZE * BUCKET_SIZE * sizeof(T)];

    public:
      std::uint16_t pageOccupancy = 0;
      constexpr T* data() noexcept { return reinterpret_cast<T*>(m_data); }
    };

  public:
    constexpr explicit BucketAllocator() noexcept = default;
    constexpr explicit BucketAllocator(BucketAllocator&&) noexcept = default;
    constexpr BucketAllocator& operator=(BucketAllocator&&) noexcept = default;

    constexpr MemorySegment<T> Allocate(std::size_t size) noexcept override
    {
      assert(size <= BUCKET_SIZE);

      T* bucketPtr = nullptr;
      PageID pageID = 0;

      if (m_freeList.empty())
      {
        if (m_pages.empty() || m_cursor == POOL_SIZE * BUCKET_SIZE)
        {
          m_pages.emplace_back(new Page);
          m_cursor = 0;
        }

        pageID = PageID(m_pages.size() - 1);
        bucketPtr = m_pages.back()->data() + m_cursor;
        m_cursor += BUCKET_SIZE;
      }
      else
      {
        auto item = m_freeList.back();
        m_freeList.pop_back();
        pageID = item.first;
        bucketPtr = m_pages[pageID]->data() + item.second;
      }

      ++m_pages[pageID]->pageOccupancy;

      if constexpr (!std::is_trivially_copyable_v<T> && std::is_default_constructible_v<T>)
        std::uninitialized_default_construct_n(bucketPtr, size);

      return { pageID, std::span<T>(bucketPtr, size) };
    }

    constexpr void Deallocate(MemorySegment<T>& segment) noexcept override
    {
      assert(segment.pageID < m_pages.size());
      if (segment.pageID >= m_pages.size())
        return;

      if constexpr (!std::is_trivially_copyable_v<T> && std::is_default_constructible_v<T> && !std::is_trivially_destructible_v<T>)
        std::destroy_n(segment.segment.data(), segment.segment.size());

      assert(m_pages[segment.pageID]);
      assert(m_pages[segment.pageID]->pageOccupancy > 0);
      --m_pages[segment.pageID]->pageOccupancy;

      // If it matches the current cursor, just move cursor back
      if (segment.pageID == m_pages.size() - 1 && segment.segment.data() == (m_pages.back()->data() + (m_cursor - BUCKET_SIZE)))
      {
        m_cursor -= BUCKET_SIZE;
      }
      else
      {
        m_freeList.push_back({ segment.pageID, static_cast<std::uint16_t>(segment.segment.data() - m_pages[segment.pageID]->data()) });
      }

      // Recursive reclamation of terminal empty pages
      while (!m_pages.empty() && m_pages.back()->pageOccupancy == 0)
      {
        PageID const lastPageID = PageID(m_pages.size() - 1);
        std::erase_if(m_freeList, [lastPageID](auto const& p) { return p.first == lastPageID; });
        m_pages.pop_back();
        m_cursor = POOL_SIZE * BUCKET_SIZE;
      }
    }

    constexpr bool TryToExtend(PageID pageID, MemoryBlock<T>& segment, std::size_t sizeIncrease) noexcept override
    {
      assert(pageID < m_pages.size());
      if (pageID >= m_pages.size())
        return false;

      assert(m_pages[pageID]);
      auto const requestedSize = segment.size() + sizeIncrease;
      if (requestedSize > BUCKET_SIZE)
        return false;

      if constexpr (!std::is_trivially_copyable_v<T> && std::is_default_constructible_v<T>)
        std::uninitialized_default_construct_n(segment.data() + segment.size(), sizeIncrease);

      segment = std::span<T>(segment.data(), requestedSize);
      return true;
    }

    constexpr bool TryToShrink(PageID pageID, MemoryBlock<T>& segment, std::size_t sizeDecrease) noexcept override
    {
      assert(pageID < m_pages.size());
      if (pageID >= m_pages.size())
        return false;

      assert(m_pages[pageID]);
      auto const requestedSize = segment.size() - sizeDecrease;
      if (requestedSize < 1)
        return false;

      if constexpr (!std::is_trivially_destructible_v<T>)
        std::destroy_n(segment.data() + requestedSize, sizeDecrease);

      segment = std::span<T>(segment.data(), requestedSize);
      return true;
    }

    constexpr void Reset() noexcept override
    {
      m_pages.clear();
      m_freeList.clear();
      m_cursor = 0;
    }

  private:
    std::vector<std::unique_ptr<Page>> m_pages;
    std::vector<std::pair<PageID, std::uint16_t>> m_freeList;
    std::uint16_t m_cursor = 0;

    static_assert(std::numeric_limits<decltype(m_cursor)>::max() > POOL_SIZE * BUCKET_SIZE);
  };


  template<typename T>
  class PagedHeapAllocator : public AllocatorInterface<T>
  {
  public:
    constexpr explicit PagedHeapAllocator() noexcept = default;

    constexpr MemorySegment<T> Allocate(std::size_t capacity) noexcept override
    {
      if (capacity == 0)
        return { INVALID_PAGEID, {} };

      PageID id = 0;
      if (!m_freeList.empty())
      {
        id = m_freeList.back();
        m_freeList.pop_back();
      }
      else
      {
        id = static_cast<PageID>(m_pages.size());
        m_pages.emplace_back();
      }

      m_pages[id].Allocate(capacity);
      return { id, std::span<T>(m_pages[id].Data(), capacity) };
    }

    constexpr void Deallocate(MemorySegment<T>& segment) noexcept override
    {
      if (segment.segment.empty() || segment.pageID == INVALID_PAGEID)
        return;

      PageID id = segment.pageID;
      assert(id < m_pages.size());
      m_pages[id].Free();

      if (id < m_pages.size() - 1)
      {
        m_freeList.push_back(id);
      }
      else
      {
        m_pages.pop_back();
        /*
        // Fully reclaim any trailing free pages
        while (!m_pages.empty())
        {
          auto lastId = static_cast<PageID>(m_pages.size() - 1);
          auto it = std::find(m_freeList.begin(), m_freeList.end(), lastId);
          if (it == m_freeList.end())
            break;

          m_freeList.erase(it);
          m_pages.pop_back();
        }
        */
      }
    }

    constexpr bool TryToExtend(PageID pageID, MemoryBlock<T>& segment, std::size_t sizeIncrease) noexcept override
    {
      assert(pageID < m_pages.size());
      auto const requestedSize = segment.size() + sizeIncrease;

      m_pages[pageID].Resize(requestedSize);
      segment = std::span<T>(m_pages[pageID].Data(), requestedSize);
      return true;
    }

    constexpr bool TryToShrink(PageID pageID, MemoryBlock<T>& segment, std::size_t sizeDecrease) noexcept override
    {
      assert(pageID < m_pages.size());
      auto const requestedSize = segment.size() - sizeDecrease;
      if (requestedSize < 1)
        return false;

      m_pages[pageID].Resize(requestedSize);
      segment = std::span<T>(m_pages[pageID].Data(), requestedSize);
      return true;
    }

    constexpr void Reset() noexcept override
    {
      m_pages.clear();
      m_freeList.clear();
    }

  private:
    std::vector<RawPage<T>> m_pages;
    std::vector<PageID> m_freeList;
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
  class FixedBufferAllocator : public AllocatorInterface<T>
  {
  public:
#ifdef ORTHOTREE__LARGE_DATASET
    using Index = std::uint64_t;
#else
    using Index = std::uint32_t;
#endif

    static constexpr std::size_t MIN_SEGMENT_SIZE = 4;

  private:
    struct FreeSegment
    {
      Index begin = 0;
      Index capacity = 0;
    };

  public:
    constexpr void Init(std::size_t pageSize) noexcept
    {
      m_page.Allocate(pageSize + MIN_SEGMENT_SIZE, pageSize + MIN_SEGMENT_SIZE);
      m_free.clear();
      m_free.reserve(16);
      m_free.push_back({ 0, Index(m_page.Size()) });
    }

    // First-fit allocation: O(k) scan. Small leftover fragments (< MIN_SEGMENT_SIZE)
    // are consumed whole to avoid unusable slivers.
    constexpr MemorySegment<T> Allocate(std::size_t n) noexcept override
    {
      auto const requiredSize = Index(n);
      for (auto freeIt = m_free.begin(); freeIt != m_free.end(); ++freeIt)
      {
        if (freeIt->capacity < requiredSize)
          continue;

        T* ptr = m_page.Data() + freeIt->begin;
        auto const remaining = freeIt->capacity - requiredSize;

        if (remaining < MIN_SEGMENT_SIZE)
        {
          m_free.erase(freeIt);
        }
        else
        {
          freeIt->begin += requiredSize;
          freeIt->capacity = remaining;
        }
        return {
          PRIMARY_PAGEID, { ptr, n }
        };
      }
      return { PRIMARY_PAGEID, {} };
    }

    // O(log k): extend seg into the immediately following free segment.
    constexpr bool TryToExtend(PageID /*pageID*/, MemoryBlock<T>& seg, std::size_t increase) noexcept override
    {
      if (seg.empty())
        return false;
      auto const endIdx = IndexOf(seg.data()) + Index(seg.size());
      auto const increase_ = Index(increase);

      auto it = LowerBoundBegin(endIdx);
      if (it == m_free.end() || it->begin != endIdx || it->capacity < increase_)
        return false;

      it->begin += increase_;
      it->capacity -= increase_;
      if (it->capacity == 0)
        m_free.erase(it);
      seg = std::span<T>(seg.data(), seg.size() + increase);
      return true;
    }

    // Return the tail `sizeDecrease` elements back to the free list and shrink seg.
    constexpr bool TryToShrink(PageID /*pageID*/, MemoryBlock<T>& seg, std::size_t sizeDecrease) noexcept override
    {
      if (seg.size() <= sizeDecrease)
        return false;
      DeallocateRange(std::span<T>(seg.data() + seg.size() - sizeDecrease, sizeDecrease));
      seg = seg.first(seg.size() - sizeDecrease);
      return true;
    }

    constexpr void Deallocate(MemorySegment<T>& seg) noexcept override
    {
      if (!seg.segment.empty())
        DeallocateRange(seg.segment);
    }

    constexpr void Reset() noexcept override
    {
      m_page.Free();
      m_free.clear();
    }

    // Copy-compact all non-empty spans into a single contiguous block at the
    // start of this page. Updates each span pointer in-place.
    // Precondition: this allocator is fresh / reset.
    constexpr void CompactInto(std::vector<std::span<T>*>& spans, std::size_t totalSize)
    {
      m_page.Resize(totalSize);
      T* dest = m_page.Data();
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
      std::size_t remaining = m_page.Size() - totalSize;
      if (remaining > MIN_SEGMENT_SIZE)
        m_free.push_back({ Index(totalSize), Index(remaining) });
    }

  private:
    constexpr Index IndexOf(T const* ptr) const noexcept { return Index(ptr - m_page.Data()); }

    // First free segment with begin >= val.
    constexpr auto LowerBoundBegin(Index val) noexcept
    {
      return std::lower_bound(m_free.begin(), m_free.end(), val, [](FreeSegment const& f, Index v) { return f.begin < v; });
    }

    // O(log k): return freed region to the free list, coalescing with neighbours.
    constexpr void DeallocateRange(std::span<T> seg) noexcept
    {
      auto const beginIdx = IndexOf(seg.data());
      auto const segSize = Index(seg.size());
      auto const endIdx = beginIdx + segSize;

      // First free segment that starts at or after the freed block's end.
      auto nextIt = LowerBoundBegin(endIdx);
      bool hasNext = (nextIt != m_free.end()) && (nextIt->begin == endIdx);

      // The potential predecessor is the entry immediately before nextIt.
      auto prevIt = nextIt;
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
        nextIt->begin = beginIdx; // extend backwards
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

    [[maybe_unused]] constexpr bool CheckFreeList() const noexcept
    {
      for (std::size_t i = 1; i < m_free.size(); ++i)
        if (m_free[i - 1].begin + m_free[i - 1].capacity > m_free[i].begin)
          return false;
      return true;
    }

    template<typename T2>
    friend class MemoryResource;

  private:
    RawPage<T> m_page;
    std::vector<FreeSegment> m_free; // sorted ascending by begin address
  };

  template<typename T>
  class MemoryResource
  {
  public:
#ifdef ORTHOTREE__LARGE_DATASET
    using Index = std::uint64_t;
#else
    using Index = std::uint32_t;
#endif

    using MemorySegment = detail::MemorySegment<T>;

    static constexpr std::size_t MIN_SEGMENT_SIZE = FixedBufferAllocator<T>::MIN_SEGMENT_SIZE;
    static constexpr std::size_t DEFAULT_PAGE_SIZE = 4096 / sizeof(T);

  public:
    MemoryResource() = default;
    MemoryResource(MemoryResource const&) = delete;
    MemoryResource(MemoryResource&&) = default;
    MemoryResource& operator=(MemoryResource&&) = default;

    void Init(std::size_t firstPageSize = DEFAULT_PAGE_SIZE) noexcept { m_primary.Init(firstPageSize); }

    void Reset() noexcept
    {
      m_primary.Reset();
      m_fallback.Reset();
    }


    MemorySegment Allocate(std::size_t capacity) noexcept
    {
      ORTHOTREE_CRASH_IF(capacity > std::numeric_limits<Index>::max(), "Too many elements. Use ORTHOTREE__LARGE_DATASET!");
      assert(capacity > 0);

      // Hot path: fixed primary buffer (cache-friendly, no heap round-trip)
      if (capacity > 32)
      {
        if (auto ms = m_primary.Allocate(capacity); !ms.segment.empty())
          return ms;
      }

      // Bucket allocators for small, frequent allocations
      auto bucketIndex = (capacity - 1) / 4;
      switch (bucketIndex)
      {
      case 0: {
        auto ms = m_bucket4.Allocate(capacity);
        ms.pageID = GetGlobalPageID(ms.pageID, 0);
        return ms;
      }
      case 1: {
        auto ms = m_bucket8.Allocate(capacity);
        ms.pageID = GetGlobalPageID(ms.pageID, 1);
        return ms;
      }
      case 2:
      case 3: {
        auto ms = m_bucket16.Allocate(capacity);
        ms.pageID = GetGlobalPageID(ms.pageID, 2);
        return ms;
      }
      case 4:
      case 5: {
        auto ms = m_bucket24.Allocate(capacity);
        ms.pageID = GetGlobalPageID(ms.pageID, 3);
        return ms;
      }
      case 6:
      case 7: {
        auto ms = m_bucket32.Allocate(capacity);
        ms.pageID = GetGlobalPageID(ms.pageID, 4);
        return ms;
      }
      default: {
        auto ms = m_fallback.Allocate(capacity);
        ++ms.pageID;
        return ms;
      }
      }
    }

    void Deallocate(MemorySegment const& ms) noexcept
    {
      if (ms.segment.empty())
        return;

      auto localMs = ms;

      if (ms.pageID == PRIMARY_PAGEID)
      {
        m_primary.Deallocate(localMs);
      }
      else if (IsBucketPageID(ms.pageID))
      {
        localMs.pageID = GetLocalPageID(ms.pageID);
        switch (GetBucketIndex(ms.pageID))
        {
        case 0: m_bucket4.Deallocate(localMs); break;
        case 1: m_bucket8.Deallocate(localMs); break;
        case 2: m_bucket16.Deallocate(localMs); break;
        case 3: m_bucket24.Deallocate(localMs); break;
        case 4: m_bucket32.Deallocate(localMs); break;
        default: break;
        }
      }
      else
      {
        localMs.pageID = GetLocalPageID(ms.pageID);
        m_fallback.Deallocate(localMs);
      }
    }

    AllocatorInterface<T>& GetAllocator(PageID pageID) noexcept
    {
      if (pageID == PRIMARY_PAGEID)
      {
        return m_primary;
      }
      else if (IsBucketPageID(pageID))
      {
        switch (GetBucketIndex(pageID))
        {
        case 0: return m_bucket4;
        case 1: return m_bucket8;
        case 2: return m_bucket16;
        case 3: return m_bucket24;
        case 4: return m_bucket32;
        default: return m_fallback;
        }
      }
      else
      {
        return m_fallback;
      }
    }

    void IncreaseSegment(MemorySegment& ms, std::size_t sizeIncrease) noexcept
    {
      if (ms.segment.empty())
      {
        ms = Allocate(sizeIncrease);
        return;
      }

      auto& allocator = GetAllocator(ms.pageID);
      if (allocator.TryToExtend(GetLocalPageID(ms.pageID), ms.segment, sizeIncrease))
        return;

      auto newMs = Allocate(ms.segment.size() + sizeIncrease);
      CopyOrMove(ms.segment, newMs.segment);
      Deallocate(ms);
      ms = newMs;
    }

    void DecreaseSegment(MemorySegment& ms, std::size_t sizeDecrease)
    {
      if (ms.segment.empty())
        return;
      assert(ms.segment.size() >= sizeDecrease);

      auto& allocator = GetAllocator(ms.pageID);
      if (allocator.TryToShrink(GetLocalPageID(ms.pageID), ms.segment, sizeDecrease))
        return;

      if (sizeDecrease < ms.segment.size())
      {
        auto newMs = Allocate(ms.segment.size() - sizeDecrease);
        CopyOrMove(ms.segment.first(ms.segment.size() - sizeDecrease), newMs.segment);
        Deallocate(ms);
        ms = newMs;
      }
      else
      {
        Deallocate(ms);
        ms = {};
      }
    }

    // Copy-compact all segments into a single contiguous block in dst.
    // Updates each MemorySegment pointer to refer to the new location.
    // After this call every segment has pageID == PRIMARY_PAGEID.
    void Clone(MemoryResource& dst, std::vector<MemorySegment*> memorySegments) const noexcept
    {
      std::size_t totalSize = 0;
      for (auto* pms : memorySegments)
        totalSize += pms->segment.size();

      std::vector<std::span<T>*> spans;
      spans.reserve(memorySegments.size());
      for (auto* pms : memorySegments)
        spans.push_back(&pms->segment);

      dst.m_primary.CompactInto(spans, totalSize);
      dst.m_bucket4.Reset();
      dst.m_bucket8.Reset();
      dst.m_bucket16.Reset();
      dst.m_bucket24.Reset();
      dst.m_bucket32.Reset();
      dst.m_fallback.Reset();

      for (auto* pms : memorySegments)
        pms->pageID = PRIMARY_PAGEID;
    }

  private:
    static constexpr bool IsBucketPageID(PageID pageID) noexcept { return pageID >= (1u << 31); }
    static constexpr int GetBucketIndex(PageID pageID) noexcept { return static_cast<int>((pageID >> 28) & 0x7); }

    static constexpr PageID GetLocalPageID(PageID pageID) noexcept
    {
      if (IsBucketPageID(pageID))
        return pageID & 0x0FFFFFFF;

      if (pageID == PRIMARY_PAGEID)
        return PRIMARY_PAGEID;

      return pageID - 1;
    }

    static constexpr PageID GetGlobalPageID(PageID localPageID, int bucketIndex) noexcept
    {
      return (1u << 31) | (static_cast<uint32_t>(bucketIndex) << 28) | localPageID;
    }

    static void CopyOrMove(MemoryBlock<T> const& src, MemoryBlock<T>& dst) noexcept
    {
      if constexpr (std::is_trivially_copyable_v<T>)
        std::memcpy(dst.data(), src.data(), src.size() * sizeof(T));
      else
        std::uninitialized_move_n(src.data(), src.size(), dst.data());
    }

  private:
    /*
      Version 0 -> 1: m_memorySize from std::size_t to serialized_size_t
    */
    static constexpr uint32_t SERIALIZED_VERSION_ID = 1;

    template<typename TData, typename TNodeMap>
    friend class MemoryResourceSerializerProxy;

    template<typename TArchive, typename TData, typename TNodes>
    friend void serialize(TArchive& ar, MemoryResource<TData>& memoryResource, TNodes& nodes);

  private:
    FixedBufferAllocator<T> m_primary;
    BucketAllocator<T, 4> m_bucket4;
    BucketAllocator<T, 8> m_bucket8;
    BucketAllocator<T, 16> m_bucket16;
    BucketAllocator<T, 24> m_bucket24;
    BucketAllocator<T, 32> m_bucket32;
    PagedHeapAllocator<T> m_fallback;
  };
} // namespace OrthoTree::detail
