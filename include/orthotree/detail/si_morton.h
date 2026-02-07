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

#include <array>
#include <bit>
#include <cassert>

#include "bitset_arithmetic.h"
#include "common.h"

namespace OrthoTree::detail
{

  template<dim_t DIMENSION_NO>
  struct MortonSpaceIndexing
  {
#ifdef ORTHOTREE__LOCATIONCODE_32
    static auto constexpr IS_32BIT_LOCATION = DIMENSION_NO <= 3;
#else
    static auto constexpr IS_32BIT_LOCATION = DIMENSION_NO <= 2;
#endif
    static auto constexpr IS_64BIT_LOCATION = !IS_32BIT_LOCATION && DIMENSION_NO < 15;

    // Indexing can be solved with integral types (above this, internal container will be changed to std::map)
    static auto constexpr IS_LINEAR_TREE = IS_32BIT_LOCATION || IS_64BIT_LOCATION;

    static auto constexpr MAX_NONLINEAR_DEPTH_ID = depth_t{ 4 };

    using UnderlyingInt = std::conditional_t<IS_32BIT_LOCATION, uint32_t, uint64_t>;
    using ChildID = UnderlyingInt;

    // Max number of children
    static auto constexpr CHILD_NO = detail::pow2_ce<DIMENSION_NO, ChildID>();

    // Mask for child bit part
    static auto constexpr CHILD_MASK = detail::pow2_ce<DIMENSION_NO, ChildID>() - 1;

    // Max value: 2 ^ nDepth ^ DIMENSION_NO * 2 (signal bit)
    using LinearLocationID = UnderlyingInt;
    using NonLinearLocationID = bitset_arithmetic<DIMENSION_NO * MAX_NONLINEAR_DEPTH_ID + 1>;
    using LocationID = typename std::conditional_t<IS_LINEAR_TREE, LinearLocationID, NonLinearLocationID>;
    using NodeID = LocationID; // same as the LocationID, but depth is signed by a sentinel bit.
    using LocationIDCR = typename std::conditional_t<IS_LINEAR_TREE, LocationID const, LocationID const&>;
    using NodeIDCR = LocationIDCR;
    template<typename T>
    using DimArray = std::array<T, DIMENSION_NO>;

    // Type system determined maximal depth id due to the resolution.
    static auto constexpr MAX_THEORETICAL_DEPTH_ID =
      IS_LINEAR_TREE ? static_cast<depth_t>((CHAR_BIT * sizeof(NodeID) - 1 /*sentinel bit*/)) / DIMENSION_NO : MAX_NONLINEAR_DEPTH_ID;

    struct Location
    {
      // TODO: make them private and packed
      depth_t depthID;
      LocationID locationID;

      constexpr depth_t GetDepthID() const noexcept { return depthID; }
      constexpr LocationID GetLocationID() const noexcept { return locationID; }

      constexpr bool operator<(Location const& rightLocation) const noexcept
      {
        auto const leftLocationID = GetLocationID();
        auto const rightLocationID = rightLocation.GetLocationID();
        return (leftLocationID < rightLocationID) || ((leftLocationID == rightLocationID) && (GetDepthID() < rightLocation.GetDepthID()));
      }
    };

    static constexpr Location GetRootLocation() noexcept { return Location{ 0, LocationID{} }; }

    class ChildCheckerFixedDepth
    {
    public:
      constexpr ChildCheckerFixedDepth(depth_t examinedLevel, LocationIDCR locationID) noexcept
      : m_mask((LocationID(CHILD_MASK)) << (examinedLevel * DIMENSION_NO))
      , m_childFlag(locationID & m_mask)
      {}

      ChildID GetChildID(depth_t examinedLevel) const noexcept { return CastMortonIDToChildID(m_childFlag >> (examinedLevel * DIMENSION_NO)); }

      constexpr bool Test(LocationIDCR locationID) const noexcept { return (locationID & m_mask) == m_childFlag; }

    private:
      LocationID m_mask;
      LocationID m_childFlag;
    };

    class ChildKeyGenerator
    {
    public:
      constexpr ChildKeyGenerator() noexcept = default;
      explicit constexpr ChildKeyGenerator(NodeIDCR parentNodeKey) noexcept
      : m_parentFlag(parentNodeKey << DIMENSION_NO)
      {}

      constexpr ChildKeyGenerator(ChildKeyGenerator const&) noexcept = default;
      constexpr ChildKeyGenerator(ChildKeyGenerator&&) noexcept = default;

      constexpr NodeID GetChildNodeKey(ChildID childID) const noexcept { return m_parentFlag | NodeID(childID); }

    private:
      NodeID m_parentFlag = {};
    };
    /*
    static constexpr NodeID GetHashAtDepth(auto&& location, depth_t maxDepthID) noexcept
    {
      return (NodeID{ 1 } << (location.depthID * DIMENSION_NO)) | (location.LocID >> ((maxDepthID - location.depthID) * DIMENSION_NO));
    }

    static constexpr NodeID GetHashAtDepth(LocationIDCR locationID, depth_t depthID, depth_t maxDepthID) noexcept
    {
      return (NodeID{ 1 } << (depthID * DIMENSION_NO)) | (locationID >> ((maxDepthID - depthID) * DIMENSION_NO));
    }

    static constexpr NodeID GetHash(depth_t depth, LocationIDCR locationID) noexcept
    {
      assert(locationID < (NodeID(1) << (depth * DIMENSION_NO)));
      return (NodeID{ 1 } << (depth * DIMENSION_NO)) | locationID;
    }
    */
    /*
    static constexpr NodeID GetNodeID(depth_t depthID, LocationIDCR locationID) noexcept
    {
      assert(locationID < (NodeID(1) << (depthID * DIMENSION_NO)));
      return (NodeID{ 1 } << (depthID * DIMENSION_NO)) | locationID;
    }
    */

    static constexpr NodeID GetNodeID(LocationIDCR locationIDOnDepth, depth_t depthID) noexcept
    {
      assert(locationIDOnDepth < (NodeID(1) << (depthID * DIMENSION_NO)));
      return (NodeID{ 1 } << (depthID * DIMENSION_NO)) | locationIDOnDepth;
    }

    template<typename T>
    static constexpr NodeID GetNodeID(T&& location, depth_t maxDepthID) noexcept
      requires requires(T t) {
        t.depthID;
        t.locationID;
      }
    {
      return (NodeID{ 1 } << (location.depthID * DIMENSION_NO)) | (location.locationID >> ((maxDepthID - location.depthID) * DIMENSION_NO));
    }


    static constexpr NodeID GetNodeID(LocationIDCR locationID, depth_t depthID, depth_t maxDepthID) noexcept
    {
      assert((locationID >> (maxDepthID * DIMENSION_NO)) == 0);
      return (NodeID{ 1 } << (depthID * DIMENSION_NO)) | (locationID >> ((maxDepthID - depthID) * DIMENSION_NO));
    }

    static constexpr NodeID GetRootKey() noexcept { return NodeID{ 1 }; }

    static constexpr NodeID GetNoneKey() noexcept { return NodeID{ 0 }; }

    static constexpr bool IsValidKey(LinearLocationID key) noexcept { return key > 0; }

    static bool IsValidKey(NonLinearLocationID const& key) noexcept { return key.any(); }

    static constexpr NodeID GetParentKey(NodeIDCR key) noexcept { return key >> DIMENSION_NO; }
    static constexpr bool IsParentKey(NodeIDCR childKey, NodeIDCR possibleAncestorKey) noexcept
    {
      auto const childDepth = GetDepthID(childKey);
      auto const ancestorDepth = GetDepthID(possibleAncestorKey);

      if (ancestorDepth >= childDepth)
        return false;

      const auto shift = (childDepth - ancestorDepth) * DIMENSION_NO;

      return (childKey >> shift) == possibleAncestorKey;
    }

    // TODO: remove
    static constexpr LocationID GetParentGridID(LocationIDCR locationID) noexcept { return locationID >> DIMENSION_NO; }

    static constexpr depth_t GetDepthID(NodeID key) noexcept
    {
      if constexpr (IS_LINEAR_TREE)
      {
        depth_t const usedBitNo = std::bit_width(key) - 1;
        return usedBitNo / DIMENSION_NO;
      }
      else
      {
        for (depth_t d = 0; IsValidKey(key); ++d, key = GetParentKey(key))
          if (key == 1) // If only sentinel bit remains, exit with node depth
            return d;

        CRASH("Bad key! Internal error!"); // Bad key
      }
    }

    static constexpr NodeID RemoveSentinelBit(NodeIDCR key, std::optional<depth_t> depthIDOptional = std::nullopt) noexcept
    {
      if constexpr (IS_LINEAR_TREE)
      {
        auto const sentinelBitPosition = std::bit_width(key) - 1;
        return key - (NodeID{ 1 } << sentinelBitPosition);
      }
      else
      {
        auto const depthID = depthIDOptional.has_value() ? depthIDOptional.value() : GetDepthID(key);
        auto const sentinelBitPosition = depthID * DIMENSION_NO;
        return key - (NodeID{ 1 } << sentinelBitPosition);
      }
    }

    static constexpr LocationID GetLocationIDOnExaminedLevel(LocationIDCR locationID, depth_t examinationLevel) noexcept
    {
      return locationID >> (examinationLevel * DIMENSION_NO);
    }

    static constexpr bool IsAllChildTouched(std::array<LocationID, 2> const& locationIDRange, depth_t examinationLevel) noexcept
    {
      return IsValidKey((locationIDRange[1] - locationIDRange[0]) >> (examinationLevel * DIMENSION_NO - 1));
    }


    static constexpr bool IsAllChildTouched(LocationIDCR locationDifference, depth_t examinationLevel) noexcept
    {
      return (CastMortonIDToChildID(locationDifference >> ((examinationLevel - 1) * DIMENSION_NO)) ^ CHILD_MASK) == 0;
    }

    static constexpr bool IsAllChildTouched(ChildID touchedDimensionsFlag) noexcept { return touchedDimensionsFlag == CHILD_MASK; }

    static constexpr NodeID GetLowestCommonAncestor(NodeID nodeID1, NodeID nodeID2) noexcept
    {
      if (nodeID1 == nodeID2)
        return nodeID1;

      auto const depthID1 = GetDepthID(nodeID1);
      auto const depthID2 = GetDepthID(nodeID2);
      if (depthID1 > depthID2)
        nodeID1 >>= (depthID1 - depthID2) * DIMENSION_NO;
      else if (depthID2 > depthID1)
        nodeID2 >>= (depthID2 - depthID1) * DIMENSION_NO;

      NodeID const differentNodeBits = nodeID1 ^ nodeID2;
      auto const differentBitWidth = detail::bit_width(differentNodeBits);

      // ceil
      auto const differentLevelNum = (differentBitWidth + DIMENSION_NO - 1) / DIMENSION_NO;
      return nodeID1 >> (differentLevelNum * DIMENSION_NO);
    }

    class GetLowestCommonAncestorHelper
    {
    public:
      constexpr explicit GetLowestCommonAncestorHelper(Location location) noexcept
      : m_minDepthID(location.depthID)
      , m_base(location.locationID)
      {}

      constexpr void Add(Location location) noexcept
      {
        m_minDepthID = std::min(m_minDepthID, location.GetDepthID());
        m_diff |= (m_base ^ location.GetLocationID());
      }

      constexpr NodeID GetNodeID(depth_t maxDepthID) const noexcept
      {
        auto const levelIDDiff = (detail::bit_width(m_diff) + DIMENSION_NO - 1) / DIMENSION_NO;
        auto const depthID = std::min(m_minDepthID, maxDepthID - levelIDDiff);
        assert(levelIDDiff <= maxDepthID);
        assert(depthID <= m_minDepthID);

        return (NodeID{ 1 } << (depthID * DIMENSION_NO)) | (m_base >> ((maxDepthID - depthID) * DIMENSION_NO));
      }

    private:
      depth_t m_minDepthID = std::numeric_limits<depth_t>::max();
      LocationID m_base = {};
      LocationID m_diff = {};
    };

  private: // Morton aid functions
    // Separates low 16/32 bits of input by 1 bit
    static constexpr LocationID Part1By1(GridID gridID) noexcept
    {
      static_assert(sizeof(GridID) == 4);

      auto locationID = LocationID{ gridID };
      if constexpr (sizeof(LocationID) == 4)
      {
        // 15 bits can be used
        // n = ----------------fedcba9876543210 : Bits initially
        // n = --------fedcba98--------76543210 : After (1)
        // n = ----fedc----ba98----7654----3210 : After (2)
        // n = --fe--dc--ba--98--76--54--32--10 : After (3)
        // n = -f-e-d-c-b-a-9-8-7-6-5-4-3-2-1-0 : After (4)
        locationID = (locationID ^ (locationID << 8)) & LocationID{ 0x00ff00ff }; // (1)
        locationID = (locationID ^ (locationID << 4)) & LocationID{ 0x0f0f0f0f }; // (2)
        locationID = (locationID ^ (locationID << 2)) & LocationID{ 0x33333333 }; // (3)
        locationID = (locationID ^ (locationID << 1)) & LocationID{ 0x55555555 }; // (4)
      }
      else if constexpr (sizeof(LocationID) == 8)
      {
        // 31 bits can be used
        // n = --------------------------------xytsrqponmlkjihgfedcba9876543210 : Bits initially
        // n = ----------------xytsrqponmlkjihg----------------fedcba9876543210 : After (1)
        // n = ----xyts----rqpo----nmlk----jihg----fedc----ba98----7654----3210 : After (2)
        // n = --xy--ts--rq--po--nm--lk--ji--hg--fe--dc--ba--98--76--54--32--10 : After (3)
        // n = -x-y-t-s-r-q-p-o-n-m-l-k-j-i-h-g-f-e-d-c-b-a-9-8-7-6-5-4-3-2-1-0 : After (4)
        locationID = (locationID ^ (locationID << 16)) & LocationID{ 0x0000ffff0000ffff }; // (1)
        locationID = (locationID ^ (locationID << 8)) & LocationID{ 0x00ff00ff00ff00ff };  // (2)
        locationID = (locationID ^ (locationID << 4)) & LocationID{ 0x0f0f0f0f0f0f0f0f };  // (3)
        locationID = (locationID ^ (locationID << 2)) & LocationID{ 0x3333333333333333 };  // (4)
        locationID = (locationID ^ (locationID << 1)) & LocationID{ 0x5555555555555555 };  // (5)
      }
      else
      {
        static_assert(sizeof(LocationID) == 4 || sizeof(LocationID) == 8, "Unsupported LocationID size");
      }

      return locationID;
    }

    // Separates low 16/32 bits of input by 2 bit
    static constexpr LocationID Part1By2(GridID gridID) noexcept
    {
      static_assert(sizeof(GridID) == 4);

      auto locationID = LocationID{ gridID };
      if constexpr (sizeof(LocationID) == 4)
      {
        // 10 bits can be used
        // n = ----------------------9876543210 : Bits initially
        // n = ------98----------------76543210 : After (1)
        // n = ------98--------7654--------3210 : After (2)
        // n = ------98----76----54----32----10 : After (3)
        // n = ----9--8--7--6--5--4--3--2--1--0 : After (4)
        locationID = (locationID ^ (locationID << 16)) & LocationID{ 0xff0000ff }; // (1)
        locationID = (locationID ^ (locationID << 8)) & LocationID{ 0x0300f00f };  // (2)
        locationID = (locationID ^ (locationID << 4)) & LocationID{ 0x030c30c3 };  // (3)
        locationID = (locationID ^ (locationID << 2)) & LocationID{ 0x09249249 };  // (4)
      }
      else if constexpr (sizeof(LocationID) == 8)
      {
        // 21 bits can be used
        // n = -------------------------------------------lkjhgfedcba9876543210 : Bits initially
        // n = -----------lkjhg--------------------------------fedcba9876543210 : After (1)
        // n = -----------lkjhg----------------fedcba98----------------76543210 : After (2)
        // n = ---l--------kjhg--------fedc--------ba98--------7654--------3210 : After (3)
        // n = ---l----kj----hg----fe----dc----ba----98----76----54----32----10 : After (4)
        // n = ---l--k--j--i--h--g--f--e--d--c--b--a--9--7--6--5--4--3--2--1--0 : After (5)
        locationID = (locationID ^ (locationID << 32)) & LocationID{ 0xffff00000000ffff }; // (1)
        locationID = (locationID ^ (locationID << 16)) & LocationID{ 0x00ff0000ff0000ff }; // (2)
        locationID = (locationID ^ (locationID << 8)) & LocationID{ 0xf00f00f00f00f00f };  // (3)
        locationID = (locationID ^ (locationID << 4)) & LocationID{ 0x30c30c30c30c30c3 };  // (4)
        locationID = (locationID ^ (locationID << 2)) & LocationID{ 0x9249249249249249 };  // (5)
      }
      else
      {
        static_assert(sizeof(LocationID) == 4 || sizeof(LocationID) == 8, "Unsupported LocationID size");
      }

      return locationID;
    }

    static consteval LocationID GetBitPattern()
    {
      constexpr auto size = sizeof(LocationID) * CHAR_BIT;
      constexpr auto maxDepth = (size - 1) / DIMENSION_NO;

      auto bitPattern = LocationID{ 0 };
      auto shift = LocationID{ 0 };
      for (dim_t depthID = 0; depthID < maxDepth; ++depthID, shift += DIMENSION_NO)
        bitPattern |= LocationID{ 1 } << shift;

      return bitPattern;
    }

    static consteval std::array<LocationID, DIMENSION_NO> GetBitPatterns()
    {
      constexpr auto bitPattern = GetBitPattern();

      std::array<LocationID, DIMENSION_NO> bitPatterns;
      for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
        bitPatterns[dimensionID] = bitPattern << dimensionID;

      return bitPatterns;
    }

  public:
    static LocationID Encode(DimArray<GridID> const& gridID) noexcept
    {
      if constexpr (DIMENSION_NO == 1)
      {
        return LocationID(gridID[0]);
      }
      else if constexpr (DIMENSION_NO == 2)
      {
#ifdef ORTHOTREE_BMI2_PDEP_AVAILABLE
        return _pdep_u32(gridID[1], 0b10101010'10101010'10101010'10101010) | _pdep_u32(gridID[0], 0b01010101'01010101'01010101'01010101);
#else
        return (Part1By1(gridID[1]) << 1) + Part1By1(gridID[0]);
#endif
      }
      else if constexpr (DIMENSION_NO == 3)
      {
#ifdef ORTHOTREE_BMI2_PDEP_AVAILABLE
        return _pdep_u32(gridID[2], 0b00100100'10010010'01001001'00100100) | _pdep_u32(gridID[1], 0b10010010'01001001'00100100'10010010) |
               _pdep_u32(gridID[0], 0b01001001'00100100'10010010'01001001);
#else
        return (Part1By2(gridID[2]) << 2) + (Part1By2(gridID[1]) << 1) + Part1By2(gridID[0]);
#endif
      }
#ifdef ORTHOTREE_BMI2_PDEP_AVAILABLE
      else if constexpr (IS_64BIT_LOCATION)
      {
        static constexpr auto bitPatterns = GetBitPatterns();

        auto locationID = LocationID{};
        for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
          locationID |= _pdep_u64(gridID[dimensionID], bitPatterns[dimensionID]);

        return locationID;
      }
#endif
      else
      {
        auto msb = gridID[0];
        for (dim_t dimensionID = 1; dimensionID < DIMENSION_NO; ++dimensionID)
          msb |= gridID[dimensionID];

        LocationID locationID = 0;
        GridID mask = 1;
        for (dim_t i = 0; msb; mask <<= 1, msb >>= 1, ++i)
        {
          ORTHOTREE_LOOPIVDEP
          for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
          {
            auto const shift = dimensionID + i * DIMENSION_NO;
            if constexpr (IS_LINEAR_TREE)
            {
              locationID |= static_cast<LocationID>(gridID[dimensionID] & mask) << (shift - i);
            }
            else
            {
              locationID.set(shift, gridID[dimensionID] & mask);
            }
          }
        }
        return locationID;
      }
    }

    static DimArray<GridID> Decode(NodeIDCR nodeKey, depth_t maxDepthID) noexcept
    {
      auto const depthID = GetDepthID(nodeKey);
      auto gridID = DimArray<GridID>{};
      if constexpr (DIMENSION_NO == 1)
      {
        auto const examinationLevelID = maxDepthID - depthID;
        gridID[0] = GridID(RemoveSentinelBit(nodeKey) << examinationLevelID);
      }
#ifdef ORTHOTREE_BMI2_PDEP_AVAILABLE
      else if constexpr (IS_LINEAR_TREE)
      {
        static constexpr auto bitPatterns = GetBitPatterns();

        auto const examinationLevelID = maxDepthID - depthID;
        const auto locationID = RemoveSentinelBit(nodeKey) << examinationLevelID * DIMENSION_NO;
        for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
        {
          if constexpr (IS_32BIT_LOCATION)
          {
            gridID[dimensionID] = GridID(_pext_u32(locationID, bitPatterns[dimensionID]));
          }
          else
          {
            gridID[dimensionID] = GridID(_pext_u64(locationID, bitPatterns[dimensionID]));
          }
        }
      }
#endif
      else
      {
        auto constexpr mask = LocationID{ 1 };
        for (depth_t examinationLevelID = maxDepthID - depthID, shift = 0; examinationLevelID < maxDepthID; ++examinationLevelID)
          for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID, ++shift)
          {
            if constexpr (IS_LINEAR_TREE)
            {
              gridID[dimensionID] |= ((nodeKey >> shift) & mask) << examinationLevelID;
            }
            else
            {
              gridID[dimensionID] |= GridID{ nodeKey[shift] } << examinationLevelID;
            }
          }
      }
      return gridID;
    }

    static ChildID CastMortonIDToChildID(NonLinearLocationID const& bs) noexcept
    {
      assert(bs <= NonLinearLocationID(std::numeric_limits<ChildID>::max()));
      return bs.to_ullong();
    }

    static constexpr ChildID CastMortonIDToChildID(LinearLocationID morton) noexcept { return morton; }

    // TODO: remove?
    static ChildID GetChildID(NodeIDCR key) noexcept
    {
      if constexpr (IS_LINEAR_TREE)
      {
        auto constexpr childMask = LocationID(CHILD_MASK);
        return CastMortonIDToChildID(key & childMask);
      }
      else
      {
        auto childID = NodeID{};
        for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
        {
          childID.set(dimensionID, key[dimensionID]);
        }

        return CastMortonIDToChildID(childID);
      }
    }

    // TODO: rename
    static ChildID GetChildID2(NodeIDCR parentNodeID, NodeID childNodeID) noexcept
    {
      auto const parentDepthID = GetDepthID(parentNodeID);
      auto const childDepthID = GetDepthID(childNodeID);
      assert(childDepthID > parentDepthID);
      childNodeID >>= (childDepthID - parentDepthID - 1) * DIMENSION_NO;
      return GetChildID(childNodeID);
    }

    static constexpr ChildID GetChildID(LocationIDCR childNodeKey, depth_t examinationLevelID)
    {
      return GetChildID(childNodeKey >> (DIMENSION_NO * (examinationLevelID - 1)));
    }

    static constexpr NodeID GetDirectChildNodeID(NodeID parentNodeID, NodeID childNodeID) noexcept
    {
      auto const parentDepthID = GetDepthID(parentNodeID);
      auto const childDepthID = GetDepthID(childNodeID);
      assert(parentDepthID < childDepthID);
      auto const depthDiff = childDepthID - parentDepthID;
      if (depthDiff == 1)
        return childNodeID;
      else
        return childNodeID >> (DIMENSION_NO * (depthDiff - 1));
    }

    static constexpr ChildID GetChildIDByDepth(depth_t parentDepth, depth_t childDepth, LocationIDCR childNodeKey)
    {
      auto const depthDifference = childDepth - parentDepth;
      assert(depthDifference > 0);
      return GetChildID(childNodeKey, depthDifference);
    }

    static constexpr bool IsChildInGreaterSegment(ChildID childID, dim_t dimensionID) noexcept { return (ChildID{ 1 } << dimensionID) & childID; }

    static constexpr bool IsChildInGreaterSegment(NonLinearLocationID const& locationID, dim_t dimensionID) noexcept
    {
      return locationID[dimensionID];
    }

    static constexpr std::array<LocationID, 2> GetRangeLocationID(std::array<DimArray<GridID>, 2> const& gridIDRange) noexcept
    {
      return { Encode(gridIDRange[0]), Encode(gridIDRange[1]) };
    }

    static constexpr Location GetLocation(auto&& locationID, depth_t maxDepthID) noexcept { return Location{ maxDepthID, locationID }; }

    static constexpr Location GetRangeLocation(auto&& gridIDRange, depth_t maxDepthID) noexcept
    {
      if (gridIDRange[0] == gridIDRange[1])
        return Location{ maxDepthID, Encode(gridIDRange[0]) };

      auto locationIDRange = std::array<LocationID, 2>{ Encode(gridIDRange[0]), Encode(gridIDRange[1]) };
      auto const locationDifference = locationIDRange[0] ^ locationIDRange[1];

      depth_t levelID = 0;
      if constexpr (IS_LINEAR_TREE)
      {
        auto const differentBitNo = std::bit_width(locationDifference);
        levelID = (differentBitNo + DIMENSION_NO - 1) / DIMENSION_NO;
      }
      else
      {
        for (auto diffLocationFlag = locationDifference; diffLocationFlag != 0; diffLocationFlag >>= DIMENSION_NO)
          ++levelID;
      }

      assert(0 < levelID && levelID <= maxDepthID);

      auto const shift = levelID * DIMENSION_NO;
      return Location{ maxDepthID - levelID, (locationIDRange[0] >> shift) << shift };
    }
  };
} // namespace OrthoTree::detail
