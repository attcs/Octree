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

namespace OrthoTree::detail
{

  template<dim_t DIMENSION_NO, typename TScalar, typename TVector, typename TBox, typename GA>
  class GridSpaceIndexing
  {
  public:
    template<typename T>
    using DimArray = std::array<T, DIMENSION_NO>;

    using IGM = InternalGeometryModule<DIMENSION_NO, TScalar, TVector, TBox, GA>;
    using IGM_Geometry = typename IGM::Geometry;

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

    constexpr IGM::Vector CalculateGridCellCenter(DimArray<GridID>&& gridID, depth_t&& centerLevel) const noexcept
    {
      using IGM_Vector = typename IGM::Vector;

      auto const halfGrid = IGM_Geometry(detail::pow2(centerLevel)) * IGM_Geometry(0.5);

      IGM_Vector center;
      LOOPIVDEP
      for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
        center[dimensionID] = (IGM_Geometry(gridID[dimensionID]) + halfGrid) / m_rasterizerFactors[dimensionID] + m_boxSpace.Min[dimensionID];

      return center;
    }

    template<bool HANDLE_OUT_OF_TREE_GEOMETRY = false>
    constexpr DimArray<GridID> GetPointGridID(TVector const& point) const noexcept
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

    constexpr std::array<DimArray<GridID>, 2> GetEdgePointGridID(TVector const& point) const noexcept
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

    template<bool DO_POINT_LIKE_CLASSIFICATION = false, typename TBox_ = TBox>
    constexpr std::array<DimArray<GridID>, 2> GetBoxGridID(TBox_ const& box) const noexcept
    {
      std::array<DimArray<GridID>, 2> gridID;
      constexpr IGM_Geometry zero = IGM_Geometry{};
      for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
      {
        IGM_Geometry boxMin, boxMax;
        if constexpr (std::is_same_v<TBox_, TBox>)
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

  private:
    GridID m_maxRasterResolution = {};
    GridID m_maxRasterID = {};

    IGM::Box m_boxSpace = {};
    IGM::Geometry m_volumeOfOverallSpace = {};
    IGM::Vector m_rasterizerFactors = {};
    IGM::Vector m_sizeInDimensions = {};
  };
} // namespace OrthoTree::detail
