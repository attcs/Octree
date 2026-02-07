
#pragma once

#include "common.h"
#include "si_grid.h"
#include "si_morton.h"

namespace OrthoTree::detail
{
  template<typename TGeometryAdapter, bool ALLOW_OUT_OF_SPACE_INSERTION, double LOOSE_FACTOR>
  class MortonGridSpaceIndexing : public MortonSpaceIndexing<TGeometryAdapter::DIMENSION_NO>
  {
    static constexpr dim_t DIMENSION_NO = TGeometryAdapter::DIMENSION_NO;
    using SI = MortonSpaceIndexing<DIMENSION_NO>;

  public:
    using TVector = typename TGeometryAdapter::Vector;
    using TBox = typename TGeometryAdapter::Box;
    using IGM = typename InternalGeometryModule<TGeometryAdapter>;

  public:
    constexpr MortonGridSpaceIndexing() = default;

    constexpr MortonGridSpaceIndexing(depth_t maxDepthID, typename IGM::Box const& boxSpace) noexcept
    : m_grid(GridSpaceIndexing<TGeometryAdapter>(maxDepthID, boxSpace))
    , m_maxDepthID(maxDepthID)
    {}

  public:
    constexpr typename SI::Location GetInvalidLocation() const noexcept
    {
      if constexpr (ALLOW_OUT_OF_SPACE_INSERTION)
      {
        return SI::GetLocation(SI::LocationID(), 0);
      }
      else
      {
        return SI::GetLocation(SI::LocationID(), INVALID_DEPTH);
      }
    }

    constexpr typename SI::Location GetLocation(TVector const& point) const noexcept
    {
      auto gridID = m_grid.GetPointGridID(point);
      if (gridID[0] == INVALID_GRIDID) [[unlikely]]
        return GetInvalidLocation();

      return SI::GetLocation(SI::Encode(gridID), m_maxDepthID);
    }

    constexpr typename SI::Location GetLocation(TBox const& box) const noexcept
    {
      if constexpr (LOOSE_FACTOR > 1.0)
      {
        auto [centerGridID, levelID] = m_grid.template GetLooseBoxGridData<LOOSE_FACTOR>(box);
        if (centerGridID[0] == INVALID_GRIDID) [[unlikely]]
          return GetInvalidLocation();

        return SI::GetLocation(SI::Encode(centerGridID), m_maxDepthID - levelID);
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
