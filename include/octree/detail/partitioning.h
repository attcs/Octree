/*
MIT License

Copyright (c) 2026 Attila Csikós

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
#include <array>
#include <cassert>
#include <iterator>
#include <utility>
#include <vector>

#include "common.h"
namespace OrthoTree::Partitioning
{
  template<uint32_t kChunkBitSize>
  class flagset
  {
  private:
    using BucketIndexType = uint64_t;
    static constexpr std::size_t kBucketIndexBitSize = sizeof(BucketIndexType) * 8;
    static constexpr std::size_t kChunkNum = std::max<std::size_t>(1, kChunkBitSize / kBucketIndexBitSize);
    using Container = std::array<BucketIndexType, kChunkNum>;

    static_assert(kChunkBitSize > 0);

  public:
    class Iterator
    {
    public:
      constexpr Iterator(BucketIndexType const* chunks, uint32_t currentChunkIndex, BucketIndexType currentChunkState) noexcept
      : m_chunks(chunks)
      , m_currentChunkIndex(currentChunkIndex)
      , m_currentChunkState(currentChunkState)
      {
        AdvanceToValid();
      }

      constexpr uint32_t operator*() const noexcept
      {
        return m_currentChunkIndex * kBucketIndexBitSize + static_cast<uint32_t>(std::countr_zero(m_currentChunkState));
      }

      constexpr Iterator& operator++() noexcept
      {
        m_currentChunkState &= m_currentChunkState - 1; // clear lowest bit
        AdvanceToValid();
        return *this;
      }

      constexpr bool operator==(std::default_sentinel_t) const noexcept { return m_currentChunkIndex >= kChunkNum; }

    private:
      constexpr void AdvanceToValid() noexcept
      {
        while (!m_currentChunkState && ++m_currentChunkIndex < kChunkNum)
          m_currentChunkState = m_chunks[m_currentChunkIndex];
      }

      BucketIndexType const* m_chunks;
      uint32_t m_currentChunkIndex;
      BucketIndexType m_currentChunkState;
    };

    constexpr flagset() = default;

    constexpr Iterator begin() const noexcept { return Iterator(m_chunks.data(), 0, m_chunks[0]); }

    constexpr std::default_sentinel_t end() const noexcept { return {}; }

    constexpr void set(uint32_t bucketID) noexcept
    {
      auto const indexID = bucketID / kBucketIndexBitSize;
      auto const indexBit = bucketID - indexID * kBucketIndexBitSize;
      m_chunks[indexID] |= BucketIndexType(1) << indexBit;
    }

    constexpr std::size_t size() const noexcept
    {
      std::size_t bucketNum = 0;
      for (const auto bucketIndexChunk : m_chunks)
        bucketNum += std::popcount(bucketIndexChunk);

      return bucketNum;
    }

    constexpr void for_each(auto&& fn) const noexcept
    {
      for (int bucketIndexID = 0; bucketIndexID < kChunkNum; ++bucketIndexID)
      {
        auto bucketIndexChunk = m_chunks[bucketIndexID];
        while (bucketIndexChunk)
        {
          auto const bucketID = bucketIndexID * kBucketIndexBitSize + std::countr_zero(bucketIndexChunk);
          bucketIndexChunk &= bucketIndexChunk - 1;
          fn(bucketID);
        }
      }
    }

  private:
    Container m_chunks = {};
  };


  constexpr auto Quicksort3WayPartitionImpl(auto beginIt, auto endIt, auto const& accessor) noexcept
  {
    assert(detail::size(beginIt, endIt) > 1);

    auto lastIt = std::prev(endIt);
    auto const v = accessor(*lastIt);

    auto i = beginIt;
    auto j = lastIt;

    // equal elements cursors
    auto p = beginIt; // elements equal to pivot from left
    auto q = lastIt;  // elements equal to pivot from right

    for (;;)
    {
      while (accessor(*i) < v)
        ++i;

      while (v < accessor(*(--j)))
        if (j == beginIt)
          break;

      if (i >= j)
        break;

      std::iter_swap(i, j);

      if (accessor(*i) == v)
      {
        std::iter_swap(p, i);
        ++p;
      }

      if (accessor(*j) == v)
      {
        --q;
        std::iter_swap(j, q);
      }

      ++i;
    }

    // Moving equal elements to center
    std::iter_swap(i, lastIt);

    auto left_ptr = i;
    for (auto k = beginIt; k < p; ++k)
    {
      if (left_ptr != beginIt)
      {
        --left_ptr;
        std::iter_swap(k, left_ptr);
      }
    }

    auto right_ptr = i;
    for (auto k = std::prev(lastIt); k >= q; --k)
    {
      ++right_ptr;
      std::iter_swap(k, right_ptr);
    }

    return std::pair{ left_ptr, std::next(right_ptr) };
  }

  // Bentley-McIlroy 3-way quicksort-based partitioning
  void Quicksort3WayPartition(auto beginIt, auto endIt, auto const& accessor, std::size_t maxClusterSize, auto& partititionEnds) noexcept
  {
    auto const elementNum = detail::size(beginIt, endIt);
    if (elementNum <= maxClusterSize)
    {
      partititionEnds.push_back(endIt);
      return;
    }

    auto const [midBeginIt, midEndIt] = Quicksort3WayPartitionImpl(beginIt, endIt, accessor);
    Quicksort3WayPartition(beginIt, midBeginIt, accessor, maxClusterSize, partititionEnds);
    Quicksort3WayPartition(midEndIt, endIt, accessor, maxClusterSize, partititionEnds);
  }

  /*
  template<uint32_t kRadixMaxSize>
  struct BucketPartitionContext
  {
    std::array<uint32_t, kRadixMaxSize> histogram = {};
    std::array<uint32_t, kRadixMaxSize> offsets = {};
    std::array<uint32_t, kRadixMaxSize> inserted = {};
  };
  template<uint32_t kRadixBits>
  constexpr uint32_t BucketPartitionV0(auto beginIt, auto endIt, auto const& accessor, uint32_t bitNumToTest, auto& context, auto& partitions) noexcept
  {
    uint32_t constexpr kRadixMaxSize = 1u << kRadixBits;
    uint32_t const kRadixBitsTest = (kRadixBits > bitNumToTest ? bitNumToTest : kRadixBits);
    uint32_t const kRadixSize = 1u << kRadixBitsTest;
    uint32_t const kRadixMask = kRadixSize - 1;
    uint32_t const shift = bitNumToTest - kRadixBitsTest;

    auto const GetBucketID = [shift, kRadixMask](const auto key) noexcept -> uint32_t {
      return static_cast<uint32_t>((key >> shift) & kRadixMask);
    };

    partitions = {};
    context = {};
    auto& [histogram, offsets, inserted] = context;

    using BucketIndexType = uint64_t;
    auto constexpr kBucketIndexBitSize = sizeof(BucketIndexType) * 8;
    auto bucketIndex = std::array<BucketIndexType, std::max<std::size_t>(1, kRadixMaxSize / kBucketIndexBitSize)>{};

    auto minElement = accessor(*beginIt);
    auto maxElement = minElement;
    for (auto it = beginIt; it != endIt; ++it)
    {
      auto const element = accessor(*it);
      auto bucketID = GetBucketID(element);
      ++histogram[bucketID];

      auto const indexID = bucketID / kBucketIndexBitSize;
      auto const indexBit = bucketID - indexID * kBucketIndexBitSize;
      bucketIndex[indexID] |= BucketIndexType(1) << indexBit;

      minElement = std::min(minElement, element);
      maxElement = std::max(maxElement, element);
    }

    int bucketNum = 0;
    for (const auto bucketIndexChunk : bucketIndex)
      bucketNum += std::popcount(bucketIndexChunk);

    if (bucketNum == 1)
    {
      partitions.emplace_back(beginIt, endIt);
      auto const mask = (1ull << bitNumToTest) - 1;
      auto const remainingBitNum = std::bit_width((minElement ^ maxElement) & mask);
      return remainingBitNum;
    }
    else
    {
      uint32_t sum = 0;
      for (int bucketIndexID = 0, bucketIndexNum = static_cast<int>(bucketIndex.size()); bucketIndexID < bucketIndexNum; ++bucketIndexID)
      {
        auto bucketIndexChunk = bucketIndex[bucketIndexID];
        while (bucketIndexChunk)
        {
          auto const bucketID = bucketIndexID * kBucketIndexBitSize + std::countr_zero(bucketIndexChunk);
          offsets[bucketID] = sum;
          sum += histogram[bucketID];
          auto segmentBeginIt = beginIt + offsets[bucketID];
          partitions.emplace_back(segmentBeginIt, segmentBeginIt + histogram[bucketID]);
          bucketIndexChunk &= bucketIndexChunk - 1;
        }
      }

      uint32_t bucketIndexID = 0;
      uint32_t actualOffsetID = -1;
      uint32_t segmentEnd = 0;
      uint32_t i = 0;
      for (auto it = beginIt; it != endIt;)
      {
        if (i == segmentEnd)
        {
          while (!bucketIndex[bucketIndexID])
            ++bucketIndexID;

          auto& bucketIndexChunk = bucketIndex[bucketIndexID];
          actualOffsetID = bucketIndexID * kBucketIndexBitSize + std::countr_zero(bucketIndexChunk);
          segmentEnd = offsets[actualOffsetID] + histogram[actualOffsetID];
          bucketIndexChunk &= bucketIndexChunk - 1;
        }

        if (inserted[actualOffsetID] > 0 && i == offsets[actualOffsetID])
        {
          i += inserted[actualOffsetID];
          it += inserted[actualOffsetID];
          continue;
        }


        auto bucketID = GetBucketID(accessor(*it));
        auto const destIndex = offsets[bucketID] + inserted[bucketID];
        ++inserted[bucketID];
        if (destIndex == i)
        {
          ++i;
          ++it;
          continue;
        }

        auto destIt = beginIt + destIndex;
        std::iter_swap(it, destIt);
      }
    }

    return shift;
  }
  */


  template<uint32_t kRadixMaxSize>
  struct BucketPartitionContext
  {
    std::array<uint32_t, kRadixMaxSize> histogram = {};
    std::array<uint32_t, kRadixMaxSize> offsets = {};
  };
  template<uint32_t kRadixBits>
  constexpr uint32_t BucketPartition(auto beginIt, auto endIt, auto const& accessor, uint32_t bitNumToTest, auto& context, auto& partitions) noexcept
  {
    using ValueType = typename decltype(beginIt)::value_type;

    uint32_t constexpr kRadixMaxSize = 1u << kRadixBits;
    uint32_t const kRadixBitsTest = (kRadixBits > bitNumToTest ? bitNumToTest : kRadixBits);
    uint32_t const kRadixSize = 1u << kRadixBitsTest;
    uint32_t const kRadixMask = kRadixSize - 1;
    uint32_t const shift = bitNumToTest - kRadixBitsTest;

    auto const GetBucketID = [shift, kRadixMask](const auto key) noexcept -> uint32_t {
      return static_cast<uint32_t>((key >> shift) & kRadixMask);
    };

    partitions = {};
    context = {};
    auto& [histogram, offsets] = context;

    auto activeBuckets = flagset<kRadixMaxSize>{};
    auto minElement = accessor(*beginIt);
    auto maxElement = minElement;
    for (auto it = beginIt; it != endIt; ++it)
    {
      auto const element = accessor(*it);
      auto bucketID = GetBucketID(element);
      ++histogram[bucketID];

      activeBuckets.set(bucketID);

      minElement = std::min(minElement, element);
      maxElement = std::max(maxElement, element);
    }

    if (activeBuckets.size() == 1)
    {
      partitions.emplace_back(beginIt, endIt);
      auto const mask = (1ull << bitNumToTest) - 1;
      auto const remainingBitNum = std::bit_width((minElement ^ maxElement) & mask);
      return remainingBitNum;
    }

    uint32_t sum = 0;
    uint32_t lastBucketID = 0;
    for (auto bucketID : activeBuckets)
    {
      auto segmentBeginIt = beginIt + sum;
      auto segmentEndIt = segmentBeginIt + histogram[bucketID];
      partitions.push_back(segmentBeginIt, segmentEndIt);

      offsets[bucketID] = sum;
      sum += histogram[bucketID];
      lastBucketID = bucketID;
    }

    // in-place permutation (cycle leader)
    for (auto leaderBucketID : activeBuckets)
    {
      while (histogram[leaderBucketID] > 0)
      {
        auto from = beginIt + offsets[leaderBucketID];
        ValueType v = std::move(*from);
        auto bucketID = GetBucketID(accessor(v));

        while (bucketID != leaderBucketID)
        {
          auto to = beginIt + offsets[bucketID];
          ValueType tmp = std::move(*to);
          *to = std::move(v);
          v = std::move(tmp);

          ++offsets[bucketID];
          --histogram[bucketID];
          bucketID = GetBucketID(accessor(v));
        }

        *from = std::move(v);
        ++offsets[leaderBucketID];
        --histogram[leaderBucketID];
      }
    }

    return shift;
  }

  template<uint32_t kRadixBits>
  void PartitionRec(auto beginIt, auto endIt, auto const& accessor, std::size_t maxClusterSize, uint32_t bitNumToTest, auto& context, auto& partititionEnds)
  {
    uint32_t const elementNum = static_cast<uint32_t>(std::distance(beginIt, endIt));

    if (elementNum <= 1024)
    {
      Quicksort3WayPartition(beginIt, endIt, accessor, maxClusterSize, partititionEnds);
      return;
    }

    uint32_t constexpr kRadixMaxSize = 1u << kRadixBits;
    using iterator = decltype(beginIt);
    auto partitions = detail::inplace_vector<std::pair<iterator, iterator>, kRadixMaxSize>{};
    bitNumToTest = BucketPartition<kRadixBits>(beginIt, endIt, accessor, bitNumToTest, context, partitions);

    if (bitNumToTest == 0)
    {
      for (auto const& [_, endIt] : partitions)
        partititionEnds.push_back(endIt);

      return;
    }

    for (auto const& [beginIt, endIt] : partitions)
      PartitionRec<kRadixBits>(beginIt, endIt, accessor, maxClusterSize, bitNumToTest, context, partititionEnds);
  }

  template<typename TIterator>
  struct Subspan
  {
    TIterator begin;
    TIterator end;
    uint32_t bitNumToTest;
  };

  template<uint32_t kRadixBits>
  bool CollectSubSpans(auto beginIt, auto endIt, auto const& accessor, std::size_t maxClusterSize, uint32_t bitNumToTest, std::size_t maxSubspanNum, auto& queue)
  {
    uint32_t constexpr kRadixMaxSize = 1u << kRadixBits;

    using iterator = decltype(beginIt);
    auto const cmp = [](auto const& a, auto const& b) noexcept {
      return detail::size(a.begin, a.end) < detail::size(b.begin, b.end);
    };

    queue.reserve(maxSubspanNum + 4);
    queue.push_back({ beginIt, endIt, bitNumToTest });

    auto context = BucketPartitionContext<kRadixMaxSize>{};
    auto partitions = detail::inplace_vector<std::pair<iterator, iterator>, kRadixMaxSize>{};
    bool isLargestSpanBiggerThanMaxClusterSize = false;
    while (!queue.empty() && queue.size() < maxSubspanNum)
    {
      auto current = queue[0];
      auto const elementNum = static_cast<std::size_t>(std::distance(current.begin, current.end));
      if (elementNum < maxClusterSize)
      {
        isLargestSpanBiggerThanMaxClusterSize = true;
        break;
      }

      std::pop_heap(queue.begin(), queue.end(), cmp);

      auto remainingBitNumToTest = current.bitNumToTest;
      queue.pop_back();
      if (elementNum < 1024)
      {
        const auto [left_ptr, right_ptr] = Quicksort3WayPartitionImpl(current.begin, current.end, accessor);
        assert(current.end == right_ptr || *left_ptr.GetFirst() < *right_ptr.GetFirst());

        if (current.begin != left_ptr)
        {
          queue.push_back({ current.begin, left_ptr, remainingBitNumToTest });
          std::push_heap(queue.begin(), queue.end(), cmp);
        }

        if (current.end != right_ptr)
        {
          queue.push_back({ right_ptr, current.end, remainingBitNumToTest });
          std::push_heap(queue.begin(), queue.end(), cmp);
        }
      }
      else
      {
        remainingBitNumToTest = BucketPartition<kRadixBits>(current.begin, current.end, accessor, current.bitNumToTest, context, partitions);

        for (auto const& [pBegin, pEnd] : partitions)
        {
          queue.push_back({ pBegin, pEnd, remainingBitNumToTest });
          std::push_heap(queue.begin(), queue.end(), cmp);
        }
      }
    }

    std::sort(queue.begin(), queue.end(), [](auto const& lhs, auto const& rhs) { return lhs.begin < rhs.begin; });
    return isLargestSpanBiggerThanMaxClusterSize;
  }

  template<uint32_t kRadixBits>
  auto ParallelPartition(auto beginIt, auto endIt, auto const& accessor, std::size_t maxClusterSize, uint32_t bitNumToTest, auto& partititionEnds)
  {
    using iterator = decltype(beginIt);

    auto subSpansInOrder = std::vector<Subspan<iterator>>{};
    auto const isAlreadyPartitioned =
      CollectSubSpans<kRadixBits>(beginIt, endIt, accessor, maxClusterSize, bitNumToTest, std::thread::hardware_concurrency() * 2, subSpansInOrder);

    if (isAlreadyPartitioned)
    {
      for (auto const& v : subSpansInOrder)
        partititionEnds.emplace_back(v.end);
    }
    else
    {
      auto partititionEndsByThreads = std::vector<std::vector<iterator>>(subSpansInOrder.size());

      bool constexpr IS_PARALLEL_EXEC = true;
      EXEC_POL_DEF(ept); // GCC 11.3
      std::transform(EXEC_POL_ADD(ept) subSpansInOrder.begin(), subSpansInOrder.end(), partititionEndsByThreads.begin(), [&](auto const& span) {
        auto context = BucketPartitionContext<1u << kRadixBits>{};
        auto localPartititionEnds = std::vector<iterator>{};
        PartitionRec<kRadixBits>(span.begin, span.end, accessor, maxClusterSize, span.bitNumToTest, context, localPartititionEnds);
        return localPartititionEnds;
      });

      for (auto const& v : partititionEndsByThreads)
        partititionEnds.insert(partititionEnds.end(), v.begin(), v.end());
    }
  }

  template<uint32_t kRadixBits, bool kIsParalellizable = false>
  auto Partition(auto beginIt, auto endIt, auto const& accessor, std::size_t maxClusterSize, uint32_t bitNumToTest)
  {
    auto const elementNum = detail::size<double>(beginIt, endIt);

    using iterator = decltype(beginIt);
    auto partititionEnds = std::vector<iterator>{};
    partititionEnds.reserve(static_cast<std::size_t>(static_cast<double>(elementNum) / (static_cast<double>(maxClusterSize) * 0.7) + 1));

    uint32_t constexpr kRadixMaxSize = 1u << kRadixBits;
    if constexpr (kIsParalellizable)
    {
      if (elementNum > 200)
      {
        ParallelPartition<kRadixBits>(beginIt, endIt, accessor, maxClusterSize, bitNumToTest, partititionEnds);
      }
      else
      {
        auto context = BucketPartitionContext<kRadixMaxSize>{};
        PartitionRec<kRadixBits>(beginIt, endIt, accessor, maxClusterSize, bitNumToTest, context, partititionEnds);
      }
    }
    else
    {
      auto context = BucketPartitionContext<kRadixMaxSize>{};
      PartitionRec<kRadixBits>(beginIt, endIt, accessor, maxClusterSize, bitNumToTest, context, partititionEnds);
    }
    return partititionEnds;
  }


  template<typename Iterator, typename Location>
  struct PartitionData
  {
    Iterator beginIt, endIt;
    // uint32_t size;
    Location location;
    uint32_t bucketID;
  };

  template<typename TLocationID>
  struct BucketLcaState
  {
    TLocationID reference;
    TLocationID xorMask;
    depth_t depthID = INVALID_DEPTH;
  };

  template<dim_t DIMENSION_NO, typename TLocation>
  TLocation GetLocationFromLCAState(auto const& lcaState, depth_t maxDepthID)
  {
    auto const levelID = (std::bit_width(lcaState.xorMask) + DIMENSION_NO - 1) / DIMENSION_NO;
    auto const depthID = std::min(lcaState.depthID, maxDepthID - levelID);
    auto const shift = decltype(TLocation::locationID)(levelID * DIMENSION_NO);
    return TLocation{ .depthID = depthID, .locationID = (lcaState.reference >> shift) << shift };
  }

  template<uint32_t kRadixBits, dim_t DIMENSION_NO, typename TLocation>
  constexpr uint32_t BucketPartition2(
    auto beginIt, auto endIt, auto const& accessor, auto const& getDepth, uint32_t bitNumToTest, depth_t maxDepthID, auto& context, auto& partitions) noexcept
  {
    using Iterator = decltype(beginIt);
    using ValueType = typename decltype(beginIt)::value_type;
    using LocationID = typename std::remove_reference_t<decltype(accessor(*beginIt))>;

    uint32_t constexpr kRadixMaxSize = 1u << kRadixBits;
    uint32_t const kRadixBitsTest = (kRadixBits > bitNumToTest ? bitNumToTest : kRadixBits);
    uint32_t const kRadixSize = 1u << kRadixBitsTest;
    uint32_t const kRadixMask = kRadixSize - 1;
    uint32_t const shift = bitNumToTest - kRadixBitsTest;

    auto const GetBucketID = [shift, kRadixMask](const auto key) noexcept -> uint32_t {
      return static_cast<uint32_t>((key >> shift) & kRadixMask);
    };

    partitions = {};
    context = {};
    auto& [histogram, offsets] = context;

    std::array<BucketLcaState<LocationID>, kRadixMaxSize> lcaStates = {};

    auto activeBuckets = flagset<kRadixMaxSize>{};
    for (auto it = beginIt; it != endIt; ++it)
    {
      auto const locationID = accessor(*it);
      auto bucketID = GetBucketID(locationID);
      ++histogram[bucketID];

      activeBuckets.set(bucketID);

      auto& lcaState = lcaStates[bucketID];
      if (lcaState.depthID == INVALID_DEPTH)
      {
        lcaState.reference = locationID;
        lcaState.xorMask = 0;
        lcaState.depthID = getDepth(*it);
      }
      else
      {
        lcaState.reference |= locationID;
        lcaState.xorMask |= (locationID ^ lcaState.reference);
        lcaState.depthID = std::min(lcaState.depthID, getDepth(*it));
      }
    }

    auto const lcaMask = (1ull << bitNumToTest) - 1;
    if (activeBuckets.size() == 1)
    {
      auto const bucketID = *activeBuckets.begin();
      auto const remainingBitNum = std::bit_width(lcaStates[bucketID].xorMask & lcaMask);
      auto const location = GetLocationFromLCAState<DIMENSION_NO, TLocation>(lcaStates[bucketID], maxDepthID);

      partitions.push_back(PartitionData<Iterator, TLocation>{ .beginIt = beginIt, .endIt = endIt, .location = location, .bucketID = bucketID });
      return remainingBitNum;
    }

    uint32_t sum = 0;
    uint32_t lastBucketID = 0;
    for (auto bucketID : activeBuckets)
    {
      auto const segmentBeginIt = beginIt + sum;
      auto const segmentEndIt = segmentBeginIt + histogram[bucketID];
      auto const location = GetLocationFromLCAState<DIMENSION_NO, TLocation>(lcaStates[bucketID], maxDepthID);
      partitions.push_back(
        PartitionData<Iterator, TLocation>{ .beginIt = segmentBeginIt, .endIt = segmentEndIt, .location = location, .bucketID = bucketID });

      offsets[bucketID] = sum;
      sum += histogram[bucketID];
      lastBucketID = bucketID;
    }

    // in-place permutation (cycle leader)
    for (auto leaderBucketID : activeBuckets)
    {
      while (histogram[leaderBucketID] > 0)
      {
        auto from = beginIt + offsets[leaderBucketID];
        ValueType v = std::move(*from);
        auto bucketID = GetBucketID(accessor(v));

        while (bucketID != leaderBucketID)
        {
          auto to = beginIt + offsets[bucketID];
          ValueType tmp = std::move(*to);
          *to = std::move(v);
          v = std::move(tmp);

          ++offsets[bucketID];
          --histogram[bucketID];
          bucketID = GetBucketID(accessor(v));
        }

        *from = std::move(v);
        ++offsets[leaderBucketID];
        --histogram[leaderBucketID];
      }
    }

    return shift;
  }

  constexpr auto partition(auto first, auto last, auto&& p, auto&& spectate) noexcept
  {
    auto find_if_not = [&](auto first, auto last, const auto& p) noexcept {
      for (; first != last; ++first)
      {
        if (p(*first))
          return first;

        spectate(*first);
      }

      return last;
    };

    first = find_if_not(first, last, p);
    if (first == last)
      return first;

    for (auto i = std::next(first); i != last; ++i)
    {
      if (!p(*i))
      {
        spectate(*i);
        continue;
      }

      std::iter_swap(i, first);
      ++first;
    }
    return first;
  }


  template<dim_t DIMENSION_NO, bool IS_ELEMENT_DEPTH_SPECIFIC, typename TLocation>
  constexpr void DepthFirstPartition(auto EXEC_TAG, auto beginIt, auto endIt, TLocation locationHint, depth_t maxDepthID, auto const& callback) noexcept
  {
    using Iterator = decltype(beginIt);
    using ValueType = typename decltype(beginIt)::value_type;
    using Reference = typename decltype(beginIt)::reference;

    constexpr uint32_t kRadixBits = DIMENSION_NO;
    constexpr uint32_t kRadixSize = 1u << kRadixBits;

    if constexpr (IS_ELEMENT_DEPTH_SPECIFIC)
    {
      using LocationID = decltype(locationHint.locationID);
      auto lcaState = BucketLcaState<LocationID>{};
      auto middleIt = partition(
        beginIt,
        endIt,
        [depthID = locationHint.depthID](auto const& element) { return element.GetFirst().depthID == depthID; },
        [&](auto const& element) {
          auto const& location = element.GetFirst();
          if (lcaState.depthID == INVALID_DEPTH)
          {
            lcaState.reference = location.locationID;
            lcaState.xorMask = 0;
            lcaState.depthID = location.depthID;
            return;
          }

          lcaState.reference |= location.locationID;
          lcaState.xorMask |= (location.locationID ^ lcaState.reference);
          lcaState.depthID = std::min(lcaState.depthID, location.depthID);
        });

      callback(beginIt, middleIt, locationHint, true);
      beginIt = middleIt;
      locationHint = GetLocationFromLCAState<DIMENSION_NO, TLocation>(lcaState, maxDepthID);
    }

    auto partitions = detail::inplace_vector<PartitionData<Iterator, TLocation>, kRadixSize>{};
    auto context = BucketPartitionContext<kRadixSize>{};
    BucketPartition2<kRadixBits, DIMENSION_NO, TLocation>(
      beginIt,
      endIt,
      [](ValueType const& location) { return location.first.locationID; },
      [](Reference const& location) { return location.GetFirst().depthID; },
      (maxDepthID - locationHint.depthID) * DIMENSION_NO,
      maxDepthID,
      context,
      partitions);

    for (auto partition : partitions)
    {
      if (!callback(partition.beginIt, partition.endIt, partition.location, false))
        continue;

      DepthFirstPartition<DIMENSION_NO, IS_ELEMENT_DEPTH_SPECIFIC, TLocation>(
        EXEC_TAG, partition.beginIt, partition.endIt, partition.location, maxDepthID, callback);
    }
  }

} // namespace OrthoTree::Partitioning
