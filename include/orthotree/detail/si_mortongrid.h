
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

#include "common.h"
#include "si_grid.h"
#include "si_morton.h"

namespace OrthoTree::detail
{
  template<typename TGeometryAdapter, bool ALLOW_OUT_OF_SPACE_INSERTION, double LOOSE_FACTOR, depth_t MAX_ALLOWED_DEPTH_ID>
  class MortonGridSpaceIndexing : public MortonSpaceIndexing<TGeometryAdapter::DIMENSION_NO, MAX_ALLOWED_DEPTH_ID>
  {
    static constexpr dim_t DIMENSION_NO = TGeometryAdapter::DIMENSION_NO;
    using SI = MortonSpaceIndexing<DIMENSION_NO, MAX_ALLOWED_DEPTH_ID>;

  public:
    using TVector = typename TGeometryAdapter::Vector;
    using TBox = typename TGeometryAdapter::Box;
    using IGM = InternalGeometryModule<TGeometryAdapter>;

  public:
    constexpr MortonGridSpaceIndexing() = default;

    template<typename TArchive, typename TGA, bool ALLOW_OUT_SPACE, double LOOSE, depth_t MAX_DP>
    friend void serialize(TArchive& ar, MortonGridSpaceIndexing<TGA, ALLOW_OUT_SPACE, LOOSE, MAX_DP>& si, const uint32_t version);

    constexpr MortonGridSpaceIndexing(depth_t maxDepthID, typename IGM::Box const& boxSpace) noexcept
    : m_grid(GridSpaceIndexing<TGeometryAdapter>(maxDepthID, boxSpace))
    , m_maxDepthID(maxDepthID)
    {}

  public:
    constexpr typename SI::Location GetInvalidLocation() const noexcept
    {
      if constexpr (ALLOW_OUT_OF_SPACE_INSERTION)
      {
        return typename SI::Location(typename SI::LocationID(), 0);
      }
      else
      {
        return typename SI::Location(typename SI::LocationID(), INVALID_DEPTH);
      }
    }

    constexpr typename SI::Location GetLocation(TVector const& point) const noexcept
    {
      auto gridID = m_grid.GetPointGridID(point);
      if (gridID[0] == INVALID_GRIDID) [[unlikely]]
        return GetInvalidLocation();

      return typename SI::Location(SI::Encode(gridID), m_maxDepthID);
    }

    constexpr typename SI::Location GetLocation(TBox const& box) const noexcept
    {
      if constexpr (LOOSE_FACTOR > 1.0)
      {
        auto [centerGridID, levelID] = m_grid.template GetLooseBoxGridData<LOOSE_FACTOR>(box);
        if (centerGridID[0] == INVALID_GRIDID) [[unlikely]]
          return GetInvalidLocation();

        auto const shift = levelID * DIMENSION_NO;
        return typename SI::Location(SI::Encode(centerGridID) >> shift << shift, m_maxDepthID - levelID);
      }
      else
      {
        auto boxGrid = m_grid.GetBoxGridID(box);
        if (boxGrid[0][0] == INVALID_GRIDID) [[unlikely]]
          return GetInvalidLocation();

        return SI::GetRangeLocation(boxGrid, m_maxDepthID);
      }
    }

    constexpr typename SI::NodeID GetNodeID(TVector const& point) const noexcept
    {
      auto gridID = m_grid.GetPointGridID(point);
      if (gridID[0] == INVALID_GRIDID) [[unlikely]]
        return {};

      return SI::GetNodeID(SI::Encode(gridID), m_maxDepthID);
    }

    constexpr typename SI::NodeID GetNodeID(TBox const& box) const noexcept
    {
      auto location = GetLocation(box);
      if (location.GetDepthID() == INVALID_DEPTH) [[unlikely]]
        return {};

      return SI::GetNodeID(location, m_maxDepthID);
    }

    constexpr typename SI::NodeID GetNodeID(SI::Location const& location) const noexcept { return SI::GetNodeID(location, m_maxDepthID); }

    constexpr decltype(auto) GetMinPoint() const noexcept { return m_grid.GetMinPoint(); }

    constexpr decltype(auto) GetSize() const noexcept { return m_grid.GetSize(); }

    constexpr IGM::Vector CalculateNodeCenter(SI::NodeIDCR nodeID) const noexcept
    {
      return m_grid.CalculateGridCellCenter(SI::Decode(nodeID, m_maxDepthID), m_maxDepthID - SI::GetDepthID(nodeID));
    }

    constexpr void Move(TVector const& move) noexcept { m_grid.Move(move); }

  private:
    GridSpaceIndexing<TGeometryAdapter> m_grid;
    depth_t m_maxDepthID;
  };

} // namespace OrthoTree::detail
