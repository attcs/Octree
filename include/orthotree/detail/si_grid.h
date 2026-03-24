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

#include "../detail/common.h"
#include "../detail/internal_geometry_module.h"
#include "../serialization/nvp.h"

#include <algorithm>
#include <array>
#include <bit>
#include <cassert>
#include <cmath>
#include <type_traits>
#include <variant>


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

    constexpr GridSpaceIndexing(depth_t maxDepthID, typename IGM::Box const& boxSpace) noexcept
    : m_maxRasterResolution(detail::pow2<depth_t, GridID>(maxDepthID))
    , m_maxRasterID(m_maxRasterResolution - 1)
    , m_boxSpace(boxSpace)
    {
      if constexpr (std::is_same_v<IGM_Geometry, float>)
      {
        if (maxDepthID >= 16)
          m_rasterization = ForcedDoubleRasterizationFactors{};
        else
          m_rasterization = NativeRasterizationFactors{};
      }
      else
      {
        m_rasterization = NativeRasterizationFactors{};
      }

      std::visit(
        [&](auto& rasterization) {
          using RasterizationType = std::decay_t<decltype(rasterization)>;
          using RasterizationFloat = typename RasterizationType::Float;

          auto& [sizeInDimensions, rasterizerFactors, derasterizerFactors] = rasterization;

          auto const subDivisionNoFactor = RasterizationFloat(m_maxRasterResolution);
          for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
          {
            m_sizeInDimensions[dimensionID] = m_boxSpace.Max[dimensionID] - m_boxSpace.Min[dimensionID];
            sizeInDimensions[dimensionID] = RasterizationFloat(m_sizeInDimensions[dimensionID]);

            derasterizerFactors[dimensionID] = sizeInDimensions[dimensionID] / subDivisionNoFactor;
            auto const isFlat = sizeInDimensions[dimensionID] == 0;
            rasterizerFactors[dimensionID] = isFlat ? RasterizationFloat(1) : (subDivisionNoFactor / sizeInDimensions[dimensionID]);
          }
        },
        m_rasterization);

      m_volumeOfOverallSpace = IGM::GetVolumeAD(m_boxSpace);
    }

  private:
    static constexpr uint32_t SERIALIZED_VERSION_ID = 0;

    template<typename TArchive, typename TGA>
    friend void serialize(TArchive& ar, GridSpaceIndexing<TGA>& grid);

  public:
    constexpr IGM::Vector const& GetMinPoint() const noexcept { return m_boxSpace.Min; }

    constexpr IGM::Vector const& GetSize() const noexcept { return m_sizeInDimensions; }

    constexpr IGM::Geometry GetVolume() const noexcept { return m_volumeOfOverallSpace; }

    constexpr IGM::Box const& GetBoxSpace() const noexcept { return m_boxSpace; }

    constexpr void Move(IGM::Vector const& moveVector) noexcept { IGM::MoveAD(m_boxSpace, moveVector); }

    constexpr GridID GetResolution() const noexcept { return m_maxRasterResolution; }

    constexpr IGM::Vector CalculateGridCellCenter(DimArray<GridID>&& gridID, depth_t&& centerLevel) const noexcept
    {
      return std::visit(
        [&](auto& rasterization) {
          using RasterizationType = std::decay_t<decltype(rasterization)>;
          using RasterizationFloat = typename RasterizationType::Float;
          auto& [sizeInDimensions, rasterizerFactors, derasterizerFactors] = rasterization;

          auto const halfGrid = RasterizationFloat(detail::pow2(centerLevel)) * RasterizationFloat(0.5);

          typename IGM::Vector center;
          detail::static_for<GA::DIMENSION_NO>([&](auto dimensionID) {
            center[dimensionID] =
              IGM_Geometry((RasterizationFloat(gridID[dimensionID]) + halfGrid) * derasterizerFactors[dimensionID]) + m_boxSpace.Min[dimensionID];
          });

          return center;
        },
        m_rasterization);
    }

    constexpr DimArray<GridID> GetPointGridID(GA::Vector const& point) const noexcept
    {
      return std::visit(
        [&](auto& rasterization) {
          using RasterizationType = std::decay_t<decltype(rasterization)>;
          using RasterizationFloat = typename RasterizationType::Float;

          auto& [sizeInDimensions, rasterizerFactors, derasterizerFactors] = rasterization;

          auto gridIDs = DimArray<GridID>{};
          for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
          {
            auto pointComponent = IGM_Geometry(GA::GetPointC(point, dimensionID)) - m_boxSpace.Min[dimensionID];
            if (pointComponent < -GA::BASE_TOLERANCE || pointComponent > sizeInDimensions[dimensionID] + GA::BASE_TOLERANCE)
            {
              gridIDs[0] = INVALID_GRIDID;
              return gridIDs;
            }

            if (pointComponent <= 0)
              gridIDs[dimensionID] = 0;
            else
            {
              auto const rasterID = GridID(RasterizationFloat(pointComponent) * rasterizerFactors[dimensionID]);
              gridIDs[dimensionID] = std::min<GridID>(m_maxRasterID, rasterID);
            }
          }
          return gridIDs;
        },
        m_rasterization);
    }

    constexpr std::array<DimArray<GridID>, 2> GetBoxGridID(GA::Box const& box) const noexcept
    {
      return std::visit(
        [&](auto& rasterization) {
          using RasterizationType = std::decay_t<decltype(rasterization)>;
          using RasterizationFloat = typename RasterizationType::Float;

          auto constexpr zero = RasterizationFloat{};
          auto const maxRasterID = RasterizationFloat(m_maxRasterResolution);

          auto& [sizeInDimensions, rasterizerFactors, derasterizerFactors] = rasterization;

          std::array<DimArray<GridID>, 2> gridID;
          for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
          {
            auto const boxMin = IGM_Geometry(GA::GetBoxMinC(box, dimensionID)) - m_boxSpace.Min[dimensionID];
            auto const boxMax = IGM_Geometry(GA::GetBoxMaxC(box, dimensionID)) - m_boxSpace.Min[dimensionID];

            if (boxMin < -GA::BASE_TOLERANCE || boxMax > m_sizeInDimensions[dimensionID] + GA::BASE_TOLERANCE)
            {
              gridID[0][0] = INVALID_GRIDID;
              return gridID;
            }

            assert(boxMin <= boxMax && "Wrong bounding box. Input error.");
            auto const minComponentRasterID = RasterizationFloat(boxMin) * rasterizerFactors[dimensionID];
            auto const maxComponentRasterID = RasterizationFloat(boxMax) * rasterizerFactors[dimensionID];

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
        },
        m_rasterization);
    }

    template<double LOOSE_FACTOR>
    constexpr std::pair<GridPosition, depth_t> GetLooseBoxGridData(GA::Box const& box) const noexcept
    {
      if (!IGM::DoesRangeContainBoxAD(m_boxSpace, box, GA::BASE_TOLERANCE))
        return { GridPosition{ INVALID_GRIDID }, 0 };

      auto const boxCenter = IGM::GetBoxCenterAD(box);
      auto const boxSize = IGM::GetBoxSizeAD(box);
      return std::visit(
        [&](auto& rasterization) -> std::pair<GridPosition, depth_t> {
          using RasterizationType = std::decay_t<decltype(rasterization)>;
          using RasterizationFloat = typename RasterizationType::Float;

          auto& [sizeInDimensions, rasterizerFactors, derasterizerFactors] = rasterization;

          GridPosition boxCenterGrid;
          static_for<DIMENSION_NO>([&](auto dimensionID) noexcept {
            // box is withing the space, so center must be also within, no need to clamp
            boxCenterGrid[dimensionID] =
              GridID(RasterizationFloat(boxCenter[dimensionID] - m_boxSpace.Min[dimensionID]) * rasterizerFactors[dimensionID]);
          });

          auto maxRelativeSize = RasterizationFloat{};
          static_for<DIMENSION_NO>([&](auto dimensionID) noexcept {
            maxRelativeSize = std::max(maxRelativeSize, RasterizationFloat(boxSize[dimensionID]) * rasterizerFactors[dimensionID]);
          });

          GridID maxRelativeGridSize = GridID(std::ceil(maxRelativeSize));
          if (maxRelativeGridSize == 0)
            return { boxCenterGrid, 0 };

          auto levelID = std::bit_width(maxRelativeGridSize - 1);
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
                auto const cellCenterWorld = RasterizationFloat(cellGridCenter) / rasterizerFactors[dimensionID];
                auto const halfLooseCellSizeWorld =
                  RasterizationFloat(cellGridSize) * RasterizationFloat(LOOSE_FACTOR * 0.5) / rasterizerFactors[dimensionID];

                looseCellBox.Min[dimensionID] = IGM_Geometry(cellCenterWorld - halfLooseCellSizeWorld);
                looseCellBox.Max[dimensionID] = IGM_Geometry(cellCenterWorld + halfLooseCellSizeWorld);
              });

              if (IGM::DoesRangeContainBoxAD(looseCellBox, box))
                break;

              ++levelID;
            }
          }

          return { boxCenterGrid, levelID };
        },
        m_rasterization);
    }

  private:
    struct NativeRasterizationFactors
    {
      using Float = IGM::Geometry;

      IGM::Vector sizeInDimensions = {};
      IGM::Vector rasterizerFactors = {};
      IGM::Vector derasterizerFactors = {};

      template<typename TArchive>
      void serialize(TArchive& ar)
      {
        ar& ORTHOTREE_NVP_INT(sizeInDimensions);
        ar& ORTHOTREE_NVP_INT(rasterizerFactors);
        ar& ORTHOTREE_NVP_INT(derasterizerFactors);
      }
    };

    // float32_t with large maxDepthID would lead to precision issues, increase double precision is required.
    struct ForcedDoubleRasterizationFactors
    {
      using Float = double;

      std::array<double, GA::DIMENSION_NO> sizeInDimensions = {};
      std::array<double, GA::DIMENSION_NO> rasterizerFactors = {};
      std::array<double, GA::DIMENSION_NO> derasterizerFactors = {};

      template<typename TArchive>
      void serialize(TArchive& ar)
      {
        ar& ORTHOTREE_NVP_INT(sizeInDimensions);
        ar& ORTHOTREE_NVP_INT(rasterizerFactors);
        ar& ORTHOTREE_NVP_INT(derasterizerFactors);
      }
    };

  private:
    GridID m_maxRasterResolution = {};
    GridID m_maxRasterID = {};

    IGM::Geometry m_volumeOfOverallSpace = {};
    IGM::Box m_boxSpace = {};
    IGM::Vector m_sizeInDimensions = {};

    std::variant<NativeRasterizationFactors, ForcedDoubleRasterizationFactors> m_rasterization = {};
  };
} // namespace OrthoTree::detail
