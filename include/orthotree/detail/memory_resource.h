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

namespace OrthoTree::detail
{

  // MemoryResource is paged-vector style memory handler which allows to make segment deallocation independently from allocation (e.g. middle
  // allocated segment's middle part can be deallocated). Main page is prioritized to reduce heap allocations and being cache efficient.
  template<typename T>
  class MemoryResource
  {
  public:
    using Index = std::uint32_t;
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

  public:
    MemoryResource() = default;
    MemoryResource(MemoryResource const&) = delete;
    MemoryResource(MemoryResource&&) = default;

  public:
    constexpr void Init(std::size_t firstPageSize = DEFAULT_PAGE_SIZE) noexcept
    {
      m_pages.reserve(10);
      m_pages.emplace_back(firstPageSize + MIN_SEGMENT_SIZE);
      m_freeMainSegments.reserve(10);
      m_freeMainSegments.emplace_back(IndexedSegment{ 0, Index(m_pages[0].size()) });
    }

    constexpr void Clone(MemoryResource& resource, std::vector<MemorySegment*> memorySegments) const noexcept
    {
      auto sumsize = std::size_t{};
      for (auto pms : memorySegments)
        sumsize += pms->segment.size();

      resource.m_pages.resize(1);
      auto& page = resource.m_pages[0];
      page.resize(sumsize);

      auto destIt = page.begin();
      for (auto pms : memorySegments)
      {
        auto const size = pms->segment.size();
        if (size == 0)
          continue;

        if constexpr (std::is_trivially_copyable_v<T>)
          std::memcpy(&*destIt, pms->segment.data(), size * sizeof(T));
        else
          std::copy(pms->segment.begin(), pms->segment.end(), destIt);

        pms->pageID = MAIN_PAGEID;
        pms->segment = std::span(destIt, size);

        destIt += size;
      }
    }

    constexpr void Reset() noexcept
    {
      m_pages.clear();
      m_freeMainSegments.clear();
      m_freedPages.clear();
    }

    constexpr void IncreaseSegment(MemorySegment& ms, std::size_t sizeIncrease) noexcept
    {
      if (ms.segment.empty())
      {
        ms = Allocate(sizeIncrease);
        return;
      }

      auto& page = m_pages[ms.pageID];
      if (ms.pageID != MAIN_PAGEID)
      {
        page.resize(page.size() + sizeIncrease);
        ms.segment = std::span(page);
        return;
      }

      auto const sizeIncrease_ = Index(sizeIncrease);
      auto freeSegmentIt = GetConnectingFreeSegment(ms.segment);
      if (freeSegmentIt == m_freeMainSegments.end() || freeSegmentIt->capacity < sizeIncrease_)
      {
        auto newPool = Allocate(ms.segment.size() + sizeIncrease);
        if constexpr (std::is_trivially_copyable_v<T>)
          std::memcpy(newPool.segment.data(), ms.segment.data(), ms.segment.size() * sizeof(T));
        else
          std::copy(ms.segment.begin(), ms.segment.end(), newPool.segment.begin());

        Deallocate(ms);
        ms = newPool;
      }
      else
      {
        auto const begin = GetMainPageIndexOfBegin(ms.segment);
        ms.segment = std::span(page.begin() + begin, ms.segment.size() + 1);
        HandleFreeSegmentChange(freeSegmentIt, freeSegmentIt->begin + sizeIncrease_, freeSegmentIt->capacity - sizeIncrease_);
      }
    }

    constexpr void DecreaseSegment(MemorySegment& ms, std::size_t sizeDecrease)
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
        auto& page = m_pages[ms.pageID];
        page.resize(page.size() - sizeDecrease);
      }

      ms.segment = ms.segment.first(ms.segment.size() - sizeDecrease);
    }

    constexpr MemorySegment Allocate(std::size_t capacity) noexcept
    {
      auto const capacity_ = Index(capacity);

      auto freeSegmentIt = GetFreeSegmentByCapacity(capacity);
      assert(freeSegmentIt == m_freeMainSegments.end() || freeSegmentIt->capacity >= capacity_);

      auto ms = MemorySegment{};
      if (freeSegmentIt != m_freeMainSegments.end())
      {
        ms.pageID = MAIN_PAGEID;
        ms.segment = std::span(m_pages[0].begin() + freeSegmentIt->begin, capacity);
        HandleFreeSegmentChange(freeSegmentIt, freeSegmentIt->begin + capacity_, freeSegmentIt->capacity - capacity_);
      }
      else // new page is required
      {
        if (m_freedPages.empty())
        {
          ms.pageID = Index(m_pages.size());
          m_pages.emplace_back(capacity);
        }
        else
        {
          ms.pageID = Index(m_freedPages.back());
          m_pages[ms.pageID].resize(capacity);
        }
        ms.segment = std::span(m_pages[ms.pageID]);
      }

      return ms;
    }

    constexpr void Deallocate(MemorySegment const& ms) noexcept
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
            auto const& last = m_freeMainSegments.back();
            auto const& newSegment = m_freeMainSegments.emplace_back(IndexedSegment{ GetMainPageIndexOfBegin(ms.segment), last.capacity });
            HandleFreeSegmentChange(m_freeMainSegments.end() - 1, newSegment.begin, segmentSize);
          }
        }
      }
      else
      {
        if (ms.pageID == Index(m_pages.size() - 1))
          m_pages.resize(m_pages.size() - 1);
        else
        {
          m_pages[ms.pageID] = {};
          m_freedPages.emplace_back(ms.pageID);
        }
      }
    }

  private:
    inline constexpr auto GetMainPageIndexOfBegin(Segment const& pool) const noexcept { return Index(&(*pool.begin()) - &m_pages[MAIN_PAGEID][0]); }

    inline constexpr auto GetMainPageIndexOfEnd(Segment const& pool) const noexcept
    {
      return Index(&(*pool.begin()) - &m_pages[MAIN_PAGEID][0] + pool.size());
    }

    constexpr void HandleFreeSegmentChange(auto freeSegmentIt, Index newBegin, Index newCapacity) noexcept
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
      if constexpr (!std::is_integral_v<T>)
        return;

      std::fill(m_pages[0].begin() + segment.begin, m_pages[0].begin() + segment.begin + segment.capacity, std::numeric_limits<T>::max());
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

    constexpr auto GetConnectingFreeSegment(Segment const& allocatedSegment) noexcept
    {
      if (allocatedSegment.empty())
        return m_freeMainSegments.end();

      auto const begin = GetMainPageIndexOfEnd(allocatedSegment);
      if (begin == m_pages[MAIN_PAGEID].size())
        return m_freeMainSegments.end();

      return std::find_if(m_freeMainSegments.begin(), m_freeMainSegments.end(), [begin](auto const& freeSegment) { return freeSegment.begin == begin; });
    }

    constexpr auto GetPreviousFreeSegment(Segment const& allocatedSegment) noexcept
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
    std::vector<std::vector<T>> m_pages;

    // stores the freed sections of memory in ascending order
    std::vector<IndexedSegment> m_freeMainSegments;

    // stores the freed pages
    std::vector<Index> m_freedPages;
  };
} // namespace OrthoTree::detail