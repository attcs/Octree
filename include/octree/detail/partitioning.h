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

  template<uint32_t kRadixMaxSize>
  struct BucketPartitionContext
  {
    std::array<uint32_t, kRadixMaxSize> histogram = {};
    std::array<uint32_t, kRadixMaxSize> offsets = {};
    std::array<uint32_t, kRadixMaxSize> inserted = {};
  };

  template<uint32_t kRadixBits>
  constexpr uint32_t BucketPartition(auto beginIt, auto endIt, auto const& accessor, uint32_t bitNumToTest, auto& context, auto& partitions) noexcept
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
} // namespace OrthoTree::Partitioning
