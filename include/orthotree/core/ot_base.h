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
#include "../detail/si_morton.h"
#include "configuration.h"

#include <climits>
#include <cstddef>
#include <limits>
#include <type_traits>
#include <variant>
#include <vector>


namespace OrthoTree
{

  template<typename TEntityAdapter, typename TGeometryAdapter, typename TConfiguration>
  class OrthoTreeCoreBase
  {
  protected:
    using EA = TEntityAdapter;
    using GA = TGeometryAdapter;
    using CONFIG = TConfiguration;
    using IGM = detail::InternalGeometryModule<GA>;
    using SI = detail::MortonSpaceIndexing<GA::DIMENSION_NO, CONFIG::MAX_ALLOWED_DEPTH_ID>;

    using IGM_Geometry = typename IGM::Geometry;

    using TScalar = typename GA::Scalar;
    using TFloatScalar = typename GA::FloatScalar;
    using TVector = typename GA::Vector;
    using TBox = typename GA::Box;
    using TRay = typename GA::Ray;
    using TPlane = typename GA::Plane;

    using IGM_Vector = typename IGM::Vector;

    struct NodeBox
    {
      IGM_Vector minPoint;
      IGM_Vector size;
    };

    using NodeGeometry = std::conditional_t<
      CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MinPoint,
      IGM_Vector,
      std::conditional_t<CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MBR, NodeBox, std::monostate>>;

  private:
    std::size_t m_maxElementNum = CONFIG::DEFAULT_TARGET_ELEMENT_NUM_IN_NODES;
    depth_t m_maxDepthID = INVALID_DEPTH;

    struct TreeBox
    {
      IGM::Vector minPoint;
      IGM::Vector size;
    };

    std::conditional_t<CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::None || CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MBR, TreeBox, std::monostate>
      m_nominalTreeBox;
    std::conditional_t<CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::None && CONFIG::ALLOW_OUT_OF_SPACE_INSERTION, TreeBox, std::monostate> m_realTreeBox;
    std::conditional_t<CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MinPoint, std::vector<typename IGM::Vector>, std::monostate> m_nodeSize;

  public:
    using EntityContainerView = EA::EntityContainerView;
    using EntityID = EA::EntityID;
    using Entity = EA::Entity;
    using EntityGeometry = EA::Geometry;

  protected:
    constexpr OrthoTreeCoreBase() = default;

  protected:
    constexpr void InitBase(IGM::Vector const& treeMinPoint, IGM::Vector const& treeSize, depth_t maxDepthID, std::size_t maxElementNo) noexcept
    {
      ORTHOTREE_CRASH_IF(maxDepthID < 1, "maxDepthID must be larger than 0!");
      ORTHOTREE_CRASH_IF(maxDepthID > SI::MAX_THEORETICAL_DEPTH_ID, "maxDepthID is larger than the applicable with the current GA::DIMENSION_NO!");
      ORTHOTREE_CRASH_IF(maxDepthID >= std::numeric_limits<uint8_t>::max(), "maxDepthID is too large.");
      ORTHOTREE_CRASH_IF(maxElementNo == 0, "maxElementNo must be larger than 0. It is allowed max entity number for one node.");
      ORTHOTREE_CRASH_IF(CHAR_BIT * sizeof(GridID) < maxDepthID, "GridID and maxDepthID are not compatible.");

      m_maxDepthID = maxDepthID;
      m_maxElementNum = maxElementNo;

      if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::None)
      {
        m_nominalTreeBox = TreeBox{ .minPoint = treeMinPoint, .size = treeSize };
        if constexpr (CONFIG::ALLOW_OUT_OF_SPACE_INSERTION)
        {
          m_realTreeBox = TreeBox{ .minPoint = treeMinPoint, .size = treeSize };
        }
      }
      else if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MinPoint)
      {
        // the 0-based depth size of the tree is m_maxDepthID+1, and a fictive childnode halfsize (+1) could be asked prematurely.
        depth_t constexpr additionalDepth = 2;
        auto const examinedDepthSize = m_maxDepthID + additionalDepth;

        auto constexpr multiplier = IGM_Geometry(0.5);
        auto looseFactor = IGM_Geometry(CONFIG::LOOSE_FACTOR);

        m_nodeSize.resize(examinedDepthSize);
        for (depth_t depthID = 0; depthID < examinedDepthSize; ++depthID, looseFactor *= multiplier)
          detail::static_for<GA::DIMENSION_NO>(
            [&](auto dimensionID) noexcept { m_nodeSize[depthID][dimensionID] = treeSize[dimensionID] * looseFactor; });
      }
      else if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MBR)
      {
        m_nominalTreeBox = TreeBox{ .minPoint = treeMinPoint, .size = treeSize };
      }
      else
      {
        static_assert(false);
      }
    }

    constexpr IGM::Box GetNodeBox(depth_t depthID, auto const& nodeGeometry) const noexcept
    {
      if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MBR)
      {
        return { nodeGeometry.minPoint, IGM::Add(nodeGeometry.minPoint, nodeGeometry.size) };
      }
      else
      {
        auto const& size = GetNodeSize(depthID);
        return { .Min = nodeGeometry, .Max = IGM::Add(nodeGeometry, size) };
      }
    }

  public:
    constexpr depth_t GetExaminationLevelID(depth_t depthID) const { return m_maxDepthID - depthID; }


    constexpr typename IGM::Vector CalculateNodeSize(depth_t depthID) const noexcept
      requires(CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::None)
    {
      return IGM::Multiply(m_nominalTreeBox.size, IGM_Geometry(1) / static_cast<IGM_Geometry>(detail::pow2(depthID)));
    }

  public:
    constexpr auto GetMaxDepthID() const noexcept { return m_maxDepthID; }
    constexpr auto GetMaxElementNum() const noexcept { return m_maxElementNum; }
    constexpr auto GetDepthNo() const noexcept { return m_maxDepthID + 1; }

  protected:
    constexpr decltype(auto) GetNodeSize(depth_t depthID) const
    {
      if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::None)
      {
        return CalculateNodeSize(depthID);
      }
      else
      {
        return m_nodeSize[depthID];
      }
    }

    constexpr typename IGM::Vector const& GetTreeBoxNominalMinPoint() const noexcept
      requires(CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::None || CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MBR)
    {
      return m_nominalTreeBox.minPoint;
    }

    constexpr typename IGM::Vector const& GetTreeBoxNominalSize() const noexcept
      requires(CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::None || CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MBR)
    {
      return m_nominalTreeBox.size;
    }

    constexpr typename IGM::Vector const& GetTreeBoxRealMinPoint() const noexcept
      requires(CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::None && CONFIG::ALLOW_OUT_OF_SPACE_INSERTION)
    {
      return m_realTreeBox.minPoint;
    }

    constexpr typename IGM::Vector const& GetTreeBoxRealSize() const noexcept
      requires(CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::None && CONFIG::ALLOW_OUT_OF_SPACE_INSERTION)
    {
      return m_realTreeBox.size;
    }

    constexpr void SetRealTreeMinPoint(IGM::Vector const& minPoint) noexcept
      requires(CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::None && CONFIG::ALLOW_OUT_OF_SPACE_INSERTION)
    {
      m_realTreeBox.minPoint = minPoint;
    }

    constexpr void SetRealTreeSize(IGM::Vector const& size) noexcept
      requires(CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::None && CONFIG::ALLOW_OUT_OF_SPACE_INSERTION)
    {
      m_realTreeBox.size = size;
    }

    constexpr void SetTreeSize(IGM::Vector const& treeSize) noexcept
      requires(CONFIG::ALLOW_OUT_OF_SPACE_INSERTION)
    {
      if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::None)
      {
        m_realTreeBox.size = treeSize;
      }
      else if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MinPoint)
      {
        m_nodeSize[0] = treeSize;
      }
    }
  };

#define ORTHOTREE_DEPENDENT_TYPES(Base)                         \
  using EA = Base::EA;                                          \
  using GA = Base::GA;                                          \
  using CONFIG = Base::CONFIG;                                  \
  using IGM = Base::IGM;                                        \
  using SI = Base::SI;                                          \
  using IGM_Geometry = IGM::Geometry;                           \
                                                                \
  using TScalar = typename GA::Scalar;                          \
  using TFloatScalar = typename GA::FloatScalar;                \
  using TVector = typename GA::Vector;                          \
  using TBox = typename GA::Box;                                \
  using TRay = typename GA::Ray;                                \
  using TPlane = typename GA::Plane;                            \
                                                                \
  using EntityContainerView = typename EA::EntityContainerView; \
  using EntityID = typename EA::EntityID;                       \
  using Entity = typename EA::Entity;                           \
  using EntityGeometry = typename EA::Geometry
} // namespace OrthoTree
