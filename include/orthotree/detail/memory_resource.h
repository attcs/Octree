/*
MIT License

Copyright (c) 2021 Attila Csik�s

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

  // MemoryResource is paged-vector style memory handler which allows to make segment deallocation independently from allocation (e.g. middle
  // allocated segment's middle part can be deallocated). Main page is prioritized to reduce heap allocations and being cache efficient.
  // Note: This class manages raw memory only; it does not construct or destruct objects. The caller is responsible for object lifetime.
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

    static constexpr Index INVALID_PAGEID = std::numeric_limits<Index>::max();
    static constexpr Index MAIN_PAGEID = 0;
    static constexpr std::size_t MIN_SEGMENT_SIZE = 4;
    static constexpr std::size_t DEFAULT_PAGE_SIZE = 4096 / sizeof(T);

    using Segment = std::span<T>;
    struct MemorySegment
    {
      PageID pageID = MAIN_PAGEID;
      Segment segment;
    };

  private:
    struct IndexedSegment
    {
      Index begin = 0;
      Index capacity = 0;
    };

    // Page structure that manages raw memory without requiring default-constructible T
    struct Page
    {
      T* data = nullptr;
      std::size_t size = 0;         // Total allocated size (capacity for main page, used for arena pages)
      std::size_t capacity = 0;     // Allocated capacity
      std::size_t segmentCount = 0; // Number of active segments (for arena pages)

      Page() = default;
      Page(Page const&) = delete;
      Page& operator=(Page const&) = delete;

      Page(Page&& other) noexcept
      : data(other.data)
      , size(other.size)
      , capacity(other.capacity)
      , segmentCount(other.segmentCount)
      {
        other.data = nullptr;
        other.size = 0;
        other.capacity = 0;
        other.segmentCount = 0;
      }

      Page& operator=(Page&& other) noexcept
      {
        if (this != &other)
        {
          deallocate();
          data = other.data;
          size = other.size;
          capacity = other.capacity;
          segmentCount = other.segmentCount;
          other.data = nullptr;
          other.size = 0;
          other.capacity = 0;
          other.segmentCount = 0;
        }
        return *this;
      }

      ~Page() { deallocate(); }

      void allocate(std::size_t cap)
      {
        deallocate();
        if (cap > 0)
        {
          data = std::allocator<T>{}.allocate(cap);
          capacity = cap;
          size = cap;
          // Trivially copyable: no initialization needed (assignment is just memcpy)
          // Default-constructible but not trivially copyable: must construct for valid assignment
          // Non-default-constructible: leave uninitialized (client uses placement new)
          if constexpr (!std::is_trivially_copyable_v<T> && std::is_default_constructible_v<T>)
          {
            std::uninitialized_default_construct_n(data, cap);
          }
        }
      }

      void deallocate()
      {
        if (data)
        {
          // Only destroy if we constructed them (non-trivially-copyable but default-constructible)
          if constexpr (!std::is_trivially_copyable_v<T> && std::is_default_constructible_v<T> && !std::is_trivially_destructible_v<T>)
          {
            std::destroy_n(data, size);
          }
          std::allocator<T>{}.deallocate(data, capacity);
          data = nullptr;
          size = 0;
          capacity = 0;
        }
      }

      void resize(std::size_t newSize)
      {
        if (newSize > capacity)
        {
          T* newData = std::allocator<T>{}.allocate(newSize);
          if (data && size > 0)
          {
            if constexpr (std::is_trivially_copyable_v<T>)
              std::memcpy(newData, data, size * sizeof(T));
            else
              std::uninitialized_move_n(data, size, newData);
          }
          // Construct new elements only if needed
          if constexpr (!std::is_trivially_copyable_v<T> && std::is_default_constructible_v<T>)
          {
            std::uninitialized_default_construct_n(newData + size, newSize - size);
          }
          deallocate();
          data = newData;
          capacity = newSize;
        }
        else if constexpr (!std::is_trivially_copyable_v<T> && std::is_default_constructible_v<T>)
        {
          if (newSize > size)
          {
            std::uninitialized_default_construct_n(data + size, newSize - size);
          }
          else if constexpr (!std::is_trivially_destructible_v<T>)
          {
            std::destroy_n(data + newSize, size - newSize);
          }
        }
        size = newSize;
      }

      T* begin() const noexcept { return data; }
      T* end() const noexcept { return data + size; }
      T& operator[](std::size_t i) const noexcept { return data[i]; }
      bool empty() const noexcept { return size == 0; }
    };

  public:
    MemoryResource() = default;
    MemoryResource(MemoryResource const&) = delete;
    MemoryResource(MemoryResource&&) = default;

  public:
    void Init(std::size_t firstPageSize = DEFAULT_PAGE_SIZE) noexcept
    {
      m_pages.reserve(10);
      m_pages.emplace_back();
      m_pages.back().allocate(firstPageSize + MIN_SEGMENT_SIZE);
      m_freeMainSegments.reserve(10);
      m_freeMainSegments.emplace_back(IndexedSegment{ 0, Index(m_pages[0].size) });
    }

    void Clone(MemoryResource& resource, std::vector<MemorySegment*> memorySegments) const noexcept
    {
      auto sumsize = std::size_t{};
      for (auto pms : memorySegments)
        sumsize += pms->segment.size();

      resource.m_pages.resize(1);
      auto& page = resource.m_pages[0];
      page.resize(sumsize);

      auto destIt = page.data;
      for (auto pms : memorySegments)
      {
        auto const size = pms->segment.size();
        if (size == 0)
          continue;

        if constexpr (std::is_trivially_copyable_v<T>)
          std::memcpy(destIt, pms->segment.data(), size * sizeof(T));
        else
          std::uninitialized_copy_n(pms->segment.data(), size, destIt);

        pms->pageID = MAIN_PAGEID;
        pms->segment = std::span(destIt, size);

        destIt += size;
      }
    }

    void Reset() noexcept
    {
      m_pages.clear();
      m_freeMainSegments.clear();
      m_freedPages.clear();
    }

    void IncreaseSegment(MemorySegment& ms, std::size_t sizeIncrease) noexcept
    {
      if (ms.segment.empty())
      {
        ms = Allocate(sizeIncrease);
        return;
      }

      if (ms.pageID == MAIN_PAGEID)
      {
        // Main page: try to extend into adjacent free segment
        auto const sizeIncrease_ = Index(sizeIncrease);
        auto freeSegmentIt = GetConnectingFreeSegment(ms.segment);
        if (freeSegmentIt != m_freeMainSegments.end() && freeSegmentIt->capacity >= sizeIncrease_)
        {
          auto const begin = GetMainPageIndexOfBegin(ms.segment);
          auto& page = m_pages[MAIN_PAGEID];
          ms.segment = std::span(page.data + begin, ms.segment.size() + sizeIncrease);
          HandleFreeSegmentChange(freeSegmentIt, freeSegmentIt->begin + sizeIncrease_, freeSegmentIt->capacity - sizeIncrease_);
          return;
        }
      }
      else
      {
        // Arena page: check if this segment is at the end and can extend
        auto& page = m_pages[ms.pageID];
        T* segmentEnd = ms.segment.data() + ms.segment.size();
        T* pageUsedEnd = page.data + page.size;

        if (segmentEnd == pageUsedEnd)
        {
          // Segment is at the end of used portion - can we extend within capacity?
          std::size_t remaining = page.capacity - page.size;
          if (remaining >= sizeIncrease)
          {
            page.size += sizeIncrease;
            ms.segment = std::span(ms.segment.data(), ms.segment.size() + sizeIncrease);

            // Construct new elements if needed
            if constexpr (!std::is_trivially_copyable_v<T> && std::is_default_constructible_v<T>)
            {
              std::uninitialized_default_construct_n(segmentEnd, sizeIncrease);
            }
            return;
          }
        }
      }

      // Fallback: allocate new segment and copy
      auto newPool = Allocate(ms.segment.size() + sizeIncrease);
      if constexpr (std::is_trivially_copyable_v<T>)
        std::memcpy(newPool.segment.data(), ms.segment.data(), ms.segment.size() * sizeof(T));
      else
        std::uninitialized_move_n(ms.segment.data(), ms.segment.size(), newPool.segment.data());

      Deallocate(ms);
      ms = newPool;
    }

    void DecreaseSegment(MemorySegment& ms, std::size_t sizeDecrease)
    {
      if (ms.segment.empty())
        return;

      assert(ms.segment.size() >= sizeDecrease);
      if (ms.pageID == MAIN_PAGEID)
      {
        auto freeMemorySegment = ms;
        freeMemorySegment.segment = ms.segment.last(sizeDecrease);
        Deallocate(freeMemorySegment);
      }
      else
      {
        if (ms.segment.size() == sizeDecrease)
        {
          Deallocate(ms);
        }
        else
        {
          auto& page = m_pages[ms.pageID];
          T* segmentEnd = ms.segment.data() + ms.segment.size();
          T* pageUsedEnd = page.data + page.size;

          if (segmentEnd == pageUsedEnd)
            page.resize(page.size - sizeDecrease);
        }
      }

      ms.segment = ms.segment.first(ms.segment.size() - sizeDecrease);
    }

    MemorySegment Allocate(std::size_t capacity) noexcept
    {
      ORTHOTREE_CRASH_IF(capacity > std::numeric_limits<Index>::max(), "Too many elements. Use ORTHOTREE__LARGE_DATASET!");

      auto const capacity_ = Index(capacity);

      auto freeSegmentIt = GetFreeSegmentByCapacity(capacity);
      assert(freeSegmentIt == m_freeMainSegments.end() || freeSegmentIt->capacity >= capacity_);

      auto ms = MemorySegment{};
      if (freeSegmentIt != m_freeMainSegments.end())
      {
        // Allocate from main page (PAGE 0) - best-fit from free segments
        ms.pageID = MAIN_PAGEID;
        ms.segment = std::span(m_pages[0].data + freeSegmentIt->begin, capacity);
        HandleFreeSegmentChange(freeSegmentIt, freeSegmentIt->begin + capacity_, freeSegmentIt->capacity - capacity_);
      }
      else
      {
        // Arena-style allocation from overflow pages
        ms = AllocateFromArena(capacity);
      }

      return ms;
    }

  private:
    MemorySegment AllocateFromArena(std::size_t capacity) noexcept
    {
      auto ms = MemorySegment{};

      // Try to append to current arena page (last non-main page)
      if (m_pages.size() > 1)
      {
        auto& currentArena = m_pages.back();
        std::size_t remaining = currentArena.capacity - currentArena.size;
        if (remaining >= capacity)
        {
          // Append to current arena
          ms.pageID = Index(m_pages.size() - 1);
          ms.segment = std::span(currentArena.data + currentArena.size, capacity);
          currentArena.size += capacity;
          currentArena.segmentCount++;

          // Construct elements if needed
          if constexpr (!std::is_trivially_copyable_v<T> && std::is_default_constructible_v<T>)
          {
            std::uninitialized_default_construct_n(ms.segment.data(), capacity);
          }
          return ms;
        }
      }

      // Need new arena page - size it based on expected usage pattern
      // Use at least DEFAULT_PAGE_SIZE or 2x requested capacity
      std::size_t arenaSize = std::max(DEFAULT_PAGE_SIZE, capacity * 2);

      // Check if we can reuse a freed page
      if (!m_freedPages.empty())
      {
        ms.pageID = m_freedPages.back();
        m_freedPages.pop_back();
        m_pages[ms.pageID].allocate(arenaSize);
      }
      else
      {
        ms.pageID = Index(m_pages.size());
        m_pages.emplace_back();
        m_pages.back().allocate(arenaSize);
      }

      auto& newArena = m_pages[ms.pageID];
      newArena.size = capacity; // Used portion
      newArena.segmentCount = 1;
      ms.segment = std::span(newArena.data, capacity);

      return ms;
    }

  public:
    void Deallocate(MemorySegment const& ms) noexcept
    {
      if (ms.segment.empty())
        return;

      if (ms.pageID == MAIN_PAGEID)
      {
        auto nextSegmentIt = GetConnectingFreeSegment(ms.segment);
        auto prevSegmentIt = GetPreviousFreeSegment(ms.segment);
        auto const segmentSize = Index(ms.segment.size());
        if (prevSegmentIt != m_freeMainSegments.end() && nextSegmentIt != m_freeMainSegments.end())
        {
          auto const begin = prevSegmentIt->begin;
          auto const capacity = prevSegmentIt->capacity + segmentSize + nextSegmentIt->capacity;

          // one of the segments will be erased that cause iterator invalidation to the next elements
          if (prevSegmentIt < nextSegmentIt)
          {
            HandleFreeSegmentChange(nextSegmentIt, 0, 0);
            HandleFreeSegmentChange(prevSegmentIt, begin, capacity);
          }
          else
          {
            HandleFreeSegmentChange(prevSegmentIt, 0, 0);
            HandleFreeSegmentChange(nextSegmentIt, begin, capacity);
          }
        }
        else if (prevSegmentIt != m_freeMainSegments.end())
        {
          HandleFreeSegmentChange(prevSegmentIt, prevSegmentIt->begin, prevSegmentIt->capacity + segmentSize);
        }
        else if (nextSegmentIt != m_freeMainSegments.end())
        {
          HandleFreeSegmentChange(nextSegmentIt, nextSegmentIt->begin - segmentSize, nextSegmentIt->capacity + segmentSize);
        }
        else
        {
          if (m_freeMainSegments.empty())
          {
            m_freeMainSegments.push_back({ GetMainPageIndexOfBegin(ms.segment), segmentSize });
#ifdef _DEBUG
            FillWithPattern(m_freeMainSegments.back());
#endif
          }
          else
          {
            // Insert new free segment for the freed range. The capacity of the
            // newly freed segment is the segmentSize, not the last free segment's
            // capacity. Use emplace_back and then handle the inserted element.
            auto const begin = GetMainPageIndexOfBegin(ms.segment);
            m_freeMainSegments.emplace_back(IndexedSegment{ begin, segmentSize });
            HandleFreeSegmentChange(m_freeMainSegments.end() - 1, begin, segmentSize);
          }
        }
      }
      else
      {
        // Arena page deallocation - decrement segment count
        auto& arenaPage = m_pages[ms.pageID];
        assert(arenaPage.segmentCount > 0);
        arenaPage.segmentCount--;

        // If arena page is now empty, we can reclaim it
        if (arenaPage.segmentCount == 0)
        {
          if (ms.pageID == Index(m_pages.size() - 1))
          {
            // Last page - just pop it
            m_pages.pop_back();
            // Also pop any trailing empty pages
            while (m_pages.size() > 1 && m_pages.back().segmentCount == 0 && m_pages.back().data == nullptr)
            {
              m_pages.pop_back();
            }
          }
          else
          {
            // Middle page - mark as reusable
            arenaPage.deallocate();
            m_freedPages.emplace_back(ms.pageID);
          }
        }
        // else: Arena page still has other segments, memory stays allocated (no fragmentation tracking)
      }
    }

  private:
    inline auto GetMainPageIndexOfBegin(Segment const& pool) const noexcept { return Index(pool.data() - m_pages[MAIN_PAGEID].data); }

    inline auto GetMainPageIndexOfEnd(Segment const& pool) const noexcept { return Index(pool.data() - m_pages[MAIN_PAGEID].data + pool.size()); }

    void HandleFreeSegmentChange(auto freeSegmentIt, Index newBegin, Index newCapacity) noexcept
    {
      assert(newCapacity != Index(-1));

      if (newCapacity == 0)
      {
        m_freeMainSegments.erase(freeSegmentIt);
        return;
      }

      auto freeSegmentResultIt = freeSegmentIt;

      // this loops defer the new value assignment, until it finds the final place
      if (newCapacity < freeSegmentIt->capacity)
      {
        for (; freeSegmentIt != m_freeMainSegments.begin() && (freeSegmentIt - 1)->capacity > newCapacity; --freeSegmentIt)
        {
          --freeSegmentResultIt;
          *freeSegmentIt = std::move(*freeSegmentResultIt);
        }
      }
      else
      {
        for (auto const lastFreeSegmentIt = m_freeMainSegments.end() - 1;
             freeSegmentIt != lastFreeSegmentIt && (freeSegmentIt + 1)->capacity < newCapacity;
             ++freeSegmentIt)
        {
          ++freeSegmentResultIt;
          *freeSegmentIt = std::move(*freeSegmentResultIt);
        }
      }

      freeSegmentResultIt->begin = newBegin;
      freeSegmentResultIt->capacity = newCapacity;
#ifdef _DEBUG
      FillWithPattern(*freeSegmentResultIt);
#endif
      assert(CheckFreeSegments());
    }

    constexpr bool CheckFreeSegments() const noexcept
    {
#ifdef _DEBUG
      if (m_freeMainSegments.size() <= 1)
        return true;

      auto segments = m_freeMainSegments;

      std::sort(segments.begin(), segments.end(), [](const auto& a, const auto& b) { return a.begin < b.begin; });

      for (size_t i = 1; i < segments.size(); ++i)
      {
        const auto& prev = segments[i - 1];
        const auto& curr = segments[i];
        if (prev.begin + prev.capacity > curr.begin)
        {
          return false;
        }
      }
#endif
      return true;
    }

    constexpr void FillWithPattern(IndexedSegment const& segment) noexcept
    {
      if constexpr (std::is_integral_v<T> && std::is_default_constructible_v<T>)
      {
        std::fill(m_pages[0].data + segment.begin, m_pages[0].data + segment.begin + segment.capacity, std::numeric_limits<T>::max());
      }
    }

    constexpr auto GetFreeSegmentByCapacity(std::size_t capacity) noexcept
    {
      if (m_freeMainSegments.empty() || std::size_t(m_freeMainSegments.back().capacity) < capacity + MIN_SEGMENT_SIZE)
        return m_freeMainSegments.end();

      auto const requiredCapacity = Index(capacity + MIN_SEGMENT_SIZE);
      return std::partition_point(m_freeMainSegments.begin(), m_freeMainSegments.end(), [&](auto const& freeSection) {
        return freeSection.capacity < requiredCapacity;
      });
    }

    auto GetConnectingFreeSegment(Segment const& allocatedSegment) noexcept
    {
      if (allocatedSegment.empty())
        return m_freeMainSegments.end();

      auto const begin = GetMainPageIndexOfEnd(allocatedSegment);
      if (begin == m_pages[MAIN_PAGEID].size)
        return m_freeMainSegments.end();

      return std::find_if(m_freeMainSegments.begin(), m_freeMainSegments.end(), [begin](auto const& freeSegment) { return freeSegment.begin == begin; });
    }

    auto GetPreviousFreeSegment(Segment const& allocatedSegment) noexcept
    {
      if (allocatedSegment.empty())
        return m_freeMainSegments.end();

      auto const begin = GetMainPageIndexOfBegin(allocatedSegment);
      if (begin == 0)
        return m_freeMainSegments.end();

      return std::find_if(m_freeMainSegments.begin(), m_freeMainSegments.end(), [begin](auto const& freeSegment) {
        return freeSegment.begin + freeSegment.capacity == begin;
      });
    }

  private:
    // stores the data
    std::vector<Page> m_pages;

    // stores the freed sections of memory in ascending order
    std::vector<IndexedSegment> m_freeMainSegments;

    // stores the freed pages
    std::vector<Index> m_freedPages;
  };
} // namespace OrthoTree::detail
