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

    constexpr DimArray<GridID> GetPointGridID(GA::Vector const& point) const noexcept
    {
      auto gridIDs = DimArray<GridID>{};
      for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
      {
        auto pointComponent = IGM_Geometry(GA::GetPointC(point, dimensionID)) - m_boxSpace.Min[dimensionID];
        if (pointComponent < -GA::BASE_TOLERANCE || pointComponent > m_sizeInDimensions[dimensionID] + GA::BASE_TOLERANCE)
        {
          gridIDs[0] = INVALID_GRIDID;
          return gridIDs;
        }

        if (pointComponent <= 0)
          gridIDs[dimensionID] = 0;
        else
        {
          auto const rasterID = GridID(pointComponent * m_rasterizerFactors[dimensionID]);
          gridIDs[dimensionID] = std::min<GridID>(m_maxRasterID, rasterID);
        }
      }
      return gridIDs;
    }

    constexpr std::array<DimArray<GridID>, 2> GetBoxGridID(GA::Box const& box) const noexcept
    {
      std::array<DimArray<GridID>, 2> gridID;

      auto constexpr zero = IGM_Geometry{};
      auto const maxRasterID = IGM_Geometry(m_maxRasterResolution);
      for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
      {
        auto const boxMin = IGM_Geometry(GA::GetBoxMinC(box, dimensionID)) - m_boxSpace.Min[dimensionID];
        auto const boxMax = IGM_Geometry(GA::GetBoxMaxC(box, dimensionID)) - m_boxSpace.Min[dimensionID];

        if (boxMin < GA::BASE_TOLERANCE || boxMax > m_sizeInDimensions[dimensionID] + GA::BASE_TOLERANCE)
        {
          gridID[0][0] = INVALID_GRIDID;
          return gridID;
        }

        assert(boxMin <= boxMax && "Wrong bounding box. Input error.");
        auto const minComponentRasterID = boxMin * m_rasterizerFactors[dimensionID];
        auto const maxComponentRasterID = boxMax * m_rasterizerFactors[dimensionID];

        gridID[0][dimensionID] = static_cast<GridID>(std::clamp(minComponentRasterID, zero, maxRasterID));
        gridID[1][dimensionID] = static_cast<GridID>(std::clamp(maxComponentRasterID, zero, maxRasterID));

        if ((gridID[0][dimensionID] != gridID[1][dimensionID] && std::floor(maxComponentRasterID) == maxComponentRasterID) || gridID[1][dimensionID] >= m_maxRasterResolution)
        {
          --gridID[1][dimensionID];
        }

        assert(gridID[0][dimensionID] < m_maxRasterResolution);
        assert(gridID[1][dimensionID] < m_maxRasterResolution);
      }
      return gridID;
    }

    template<double LOOSE_FACTOR>
    constexpr std::pair<GridPosition, depth_t> GetLooseBoxGridData(GA::Box const& box) const noexcept
    {
      if (!IGM::DoesRangeContainBoxAD(m_boxSpace, box, GA::BASE_TOLERANCE))
        return { GridPosition{ INVALID_GRIDID }, 0 };

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
