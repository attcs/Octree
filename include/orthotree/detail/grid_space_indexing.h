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

#include "common.h"
#include "internal_geometry_module.h"

namespace OrthoTree::detail
{

  template<typename TGeometryAdapter>
  class GridSpaceIndexing
  {
  public:
    using GA = TGeometryAdapter;
    using IGM = InternalGeometryModule<GA>;
    using IGM_Geometry = typename IGM::Geometry;

    static constexpr dim_t DIMENSION_NO = GA::DIMENSION_NO;

    template<typename T>
    using DimArray = std::array<T, DIMENSION_NO>;

    // Grid cell identification type
    using GridPosition = DimArray<GridID>;

    // Min-max grid ID pairs
    using GridRange = std::array<GridPosition, 2>;

  public:
    constexpr GridSpaceIndexing() = default;

    constexpr GridSpaceIndexing(depth_t maxDepthID, IGM::Box const& boxSpace) noexcept
    : m_maxRasterResolution(detail::pow2<depth_t, GridID>(maxDepthID))
    , m_maxRasterID(m_maxRasterResolution - 1)
    , m_boxSpace(boxSpace)
    {
      auto const subDivisionNoFactor = IGM_Geometry(m_maxRasterResolution);
      for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
      {
        m_sizeInDimensions[dimensionID] = m_boxSpace.Max[dimensionID] - m_boxSpace.Min[dimensionID];
        m_derasterizerFactors[dimensionID] = m_sizeInDimensions[dimensionID] / subDivisionNoFactor;
        auto const isFlat = m_sizeInDimensions[dimensionID] == 0;
        m_rasterizerFactors[dimensionID] = isFlat ? IGM_Geometry(1.0) : (subDivisionNoFactor / m_sizeInDimensions[dimensionID]);
      }

      m_volumeOfOverallSpace = IGM::GetVolumeAD(m_boxSpace);
    }

    constexpr IGM::Vector const& GetSizes() const noexcept { return m_sizeInDimensions; }

    constexpr IGM::Geometry GetVolume() const noexcept { return m_volumeOfOverallSpace; }

    constexpr IGM::Box const& GetBoxSpace() const noexcept { return m_boxSpace; }

    constexpr void Move(IGM::Vector const& moveVector) noexcept { IGM::MoveAD(m_boxSpace, moveVector); }

    constexpr GridID GetResolution() const noexcept { return m_maxRasterResolution; }

    // TODO: remove?
    constexpr IGM::Vector CalculateGridCellCenter(DimArray<GridID>&& gridID, depth_t&& centerLevel) const noexcept
    {
      using IGM_Vector = typename IGM::Vector;

      auto const halfGrid = IGM_Geometry(detail::pow2(centerLevel)) * IGM_Geometry(0.5);

      IGM_Vector center;
      ORTHOTREE_LOOPIVDEP
      for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
        center[dimensionID] = (IGM_Geometry(gridID[dimensionID]) + halfGrid) * m_derasterizerFactors[dimensionID] + m_boxSpace.Min[dimensionID];

      return center;
    }

    constexpr IGM::Vector CalculateGridCellMinPoint(DimArray<GridID>&& gridID) const noexcept
    {
      using IGM_Vector = typename IGM::Vector;

      IGM_Vector minPoint;
      detail::static_for<DIMENSION_NO>([&](auto dimensionID) noexcept {
        minPoint[dimensionID] = IGM_Geometry(gridID[dimensionID]) * m_derasterizerFactors[dimensionID] + m_boxSpace.Min[dimensionID];
      });

      return minPoint;
    }

    template<bool HANDLE_OUT_OF_TREE_GEOMETRY = false>
    constexpr DimArray<GridID> GetPointGridID(GA::Vector const& point) const noexcept
    {
      auto gridIDs = DimArray<GridID>{};
      for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
      {
        auto pointComponent = IGM_Geometry(GA::GetPointC(point, dimensionID)) - m_boxSpace.Min[dimensionID];
        if constexpr (HANDLE_OUT_OF_TREE_GEOMETRY)
        {
          if (pointComponent < 0.0)
            pointComponent = 0.0;
        }
        else
        {
          assert(pointComponent >= 0.0);
        }

        auto const rasterID = GridID(pointComponent * m_rasterizerFactors[dimensionID]);
        gridIDs[dimensionID] = std::min<GridID>(m_maxRasterID, rasterID);
      }
      return gridIDs;
    }

    constexpr std::array<DimArray<GridID>, 2> GetEdgePointGridID(GA::Vector const& point) const noexcept
    {
      auto constexpr minRasterID = IGM_Geometry{};
      auto const maxRasterID = static_cast<IGM_Geometry>(m_maxRasterResolution - 1);

      auto pointMinMaxGridID = std::array<DimArray<GridID>, 2>{};
      for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
      {
        auto const rasterID = std::clamp(
          (IGM_Geometry(GA::GetPointC(point, dimensionID)) - m_boxSpace.Min[dimensionID]) * m_rasterizerFactors[dimensionID], minRasterID, maxRasterID);
        pointMinMaxGridID[0][dimensionID] = pointMinMaxGridID[1][dimensionID] = static_cast<GridID>(rasterID);

        if (0 < pointMinMaxGridID[0][dimensionID] && pointMinMaxGridID[0][dimensionID] < m_maxRasterResolution)
          pointMinMaxGridID[0][dimensionID] -= std::floor(rasterID) == rasterID;
      }
      return pointMinMaxGridID;
    }

    template<bool DO_POINT_LIKE_CLASSIFICATION = false, typename TBox_ = GA::Box>
    constexpr std::array<DimArray<GridID>, 2> GetBoxGridID(TBox_ const& box) const noexcept
    {
      std::array<DimArray<GridID>, 2> gridID;
      constexpr IGM_Geometry zero = IGM_Geometry{};
      for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
      {
        IGM_Geometry boxMin, boxMax;
        if constexpr (std::is_same_v<TBox_, typename GA::Box>)
        {
          boxMin = IGM_Geometry(GA::GetBoxMinC(box, dimensionID));
          boxMax = IGM_Geometry(GA::GetBoxMaxC(box, dimensionID));
        }
        else
        {
          boxMin = box.Min[dimensionID];
          boxMax = box.Max[dimensionID];
        }

        assert(boxMin <= boxMax && "Wrong bounding box. Input error.");
        auto const minComponentRasterID = (boxMin - m_boxSpace.Min[dimensionID]) * m_rasterizerFactors[dimensionID];
        auto const maxComponentRasterID = (boxMax - m_boxSpace.Min[dimensionID]) * m_rasterizerFactors[dimensionID];

        if constexpr (DO_POINT_LIKE_CLASSIFICATION)
        {
          gridID[0][dimensionID] = std::min(m_maxRasterID, static_cast<GridID>(minComponentRasterID));
          gridID[1][dimensionID] = std::min(m_maxRasterID, static_cast<GridID>(maxComponentRasterID));
        }
        else
        {
          auto const maxRasterID = IGM_Geometry(m_maxRasterResolution);

          gridID[0][dimensionID] = static_cast<GridID>(std::clamp(minComponentRasterID, zero, maxRasterID));
          gridID[1][dimensionID] = static_cast<GridID>(std::clamp(maxComponentRasterID, zero, maxRasterID));

          if ((gridID[0][dimensionID] != gridID[1][dimensionID] && std::floor(maxComponentRasterID) == maxComponentRasterID) || gridID[1][dimensionID] >= m_maxRasterResolution)
          {
            --gridID[1][dimensionID];
          }
        }

        assert(gridID[0][dimensionID] < m_maxRasterResolution);
        assert(gridID[1][dimensionID] < m_maxRasterResolution);
      }
      return gridID;
    }

    template<double LOOSE_FACTOR, bool ALLOW_OUT_OF_SPACE_INSERTION = false, typename TBox_ = GA::Box>
    constexpr std::pair<GridPosition, depth_t> GetLooseBoxGridData(TBox_ const& box) const noexcept
    {
      if (!IGM::DoesRangeContainBoxAD(m_boxSpace, box))
      {
        if constexpr (ALLOW_OUT_OF_SPACE_INSERTION)
          return { GridPosition{}, 0 };
        else
          return { GridPosition{}, INVALID_DEPTH };
      }

      auto const boxCenter = IGM::GetBoxCenterAD(box);
      auto const boxSize = IGM::GetBoxSizeAD(box);

      GridPosition boxCenterGrid;
      static_for<DIMENSION_NO>([&](auto dimensionID) noexcept {
        // box is withing the space, so center must be also within, no need to clamp
        boxCenterGrid[dimensionID] = GridID((boxCenter[dimensionID] - m_boxSpace.Min[dimensionID]) * m_rasterizerFactors[dimensionID]);
      });

      auto maxRelativeSize = IGM_Geometry{};
      static_for<DIMENSION_NO>(
        [&](auto dimensionID) noexcept { maxRelativeSize = std::max(maxRelativeSize, boxSize[dimensionID] * m_rasterizerFactors[dimensionID]); });

      GridID maxRelativeGridSize = GridID(std::ceil(maxRelativeSize));
      // TODO: enable it:
      // assert(maxRelativeGridSize > 0); // bounding box has no volume
      if (maxRelativeGridSize == 0)
        return { boxCenterGrid, 0 };

      auto levelID = std::bit_width(maxRelativeGridSize - 1);
      // TODO: remove:
      assert(levelID == std::ceil(std::log2(maxRelativeGridSize)));

      // depth calculation
      if constexpr (LOOSE_FACTOR != 2.0)
      {
        while (true)
        {
          typename IGM::Box looseCellBox;

          auto const cellGridSize = GridID(1) << levelID;
          static_for<DIMENSION_NO>([&](auto dimensionID) noexcept {
            auto const cellGridMin = (boxCenterGrid[dimensionID] / cellGridSize) * cellGridSize;

            auto const cellGridCenter = cellGridMin + cellGridSize / GridID(2);
            auto const cellCenterWorld = IGM_Geometry(cellGridCenter) / m_rasterizerFactors[dimensionID];
            auto const halfLooseCellSizeWorld = IGM_Geometry(cellGridSize) * IGM_Geometry(LOOSE_FACTOR * 0.5) / m_rasterizerFactors[dimensionID];

            looseCellBox.Min[dimensionID] = cellCenterWorld - halfLooseCellSizeWorld;
            looseCellBox.Max[dimensionID] = cellCenterWorld + halfLooseCellSizeWorld;
          });

          if (IGM::DoesRangeContainBoxAD(looseCellBox, box))
            break;

          ++levelID;
        }
      }

      return { boxCenterGrid, levelID };
    }

  private:
    GridID m_maxRasterResolution = {};
    GridID m_maxRasterID = {};

    IGM::Box m_boxSpace = {};
    IGM::Geometry m_volumeOfOverallSpace = {};
    IGM::Vector m_rasterizerFactors = {};
    IGM::Vector m_derasterizerFactors = {};
    IGM::Vector m_sizeInDimensions = {};
  };
} // namespace OrthoTree::detail
