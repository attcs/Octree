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
#include "inplace_vector.h"
#include "utils.h"
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

  constexpr auto partition(auto first, auto last, auto&& p, auto&& spectate) noexcept
  {
    while (true)
    {
      while (first != last && p(*first))
      {
        spectate(*first);
        ++first;
      }

      if (first == last)
        return first;

      while (true)
      {
        if (first == --last)
          return first;

        if (p(*last))
          break;
      }

      spectate(*last);
      std::iter_swap(first, last);
      ++first;
    }
  }


  constexpr auto partition(auto first, auto last, auto&& p, auto&& spectateTrue, auto&& spectateFalse) noexcept
  {
    while (true)
    {
      while (first != last && p(*first))
      {
        spectateTrue(*first);
        ++first;
      }

      if (first == last)
        return first;

      while (true)
      {
        if (first == --last)
        {
          spectateFalse(*first);
          return first;
        }

        if (p(*last))
          break;

        spectateFalse(*last);
      }

      spectateTrue(*last);
      spectateFalse(*first);
      std::iter_swap(first, last);
      ++first;
    }
  }
} // namespace OrthoTree::Partitioning
