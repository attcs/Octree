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

/* Settings
* Use the following define-s before the header include

// PMR is used with MSVC only by default. To use PMR anyway
#define ORTHOTREE__USE_PMR

// To disable PMR on all platforms use:
#define ORTHOTREE__DISABLE_PMR

// If the depth is less than 10, 32bit location code is enough (otherwise 64bit will be used)
#define ORTHOTREE__LOCATIONCODE_32

// Contiguous container of geometry data does not have specified index type. Octree lib uses index_t for it, it can specified to int or std::size_t.
ORTHOTREE_INDEX_T__INT / ORTHOTREE_INDEX_T__SIZE_T / ORTHOTREE_INDEX_T__UINT_FAST32_T

*/

#if defined(ORTHOTREE__USE_PMR) || defined(_MSC_VER)
#ifndef ORTHOTREE__DISABLE_PMR
#define ORTHOTREE_IS_PMR_USED
#endif // !ORTHOTREE__DISABLE_PMR
#endif

#include <assert.h>
#include <math.h>

#include <algorithm>
#include <array>
#include <bit>
#include <bitset>
#include <concepts>
#include <cstdint> // TODO: is needed?
#include <cstring>
#include <execution>
#include <functional>
#include <iterator>
#include <map>
#include <memory_resource>
#include <numeric>
#include <optional>
#include <queue>
#include <ranges>
#include <set>
#include <span>
#include <stack>
#include <stdexcept>
#include <thread>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>
#include <version>

#include "detail/bitset_arithmetic.h"
#include "detail/common.h"
#include "detail/configuration.h"
#include "detail/inplace_vector.h"
#include "detail/internal_geometry_module.h"
#include "detail/memory_resource.h"
#include "detail/partitioning.h"
#include "detail/si_mortongrid.h"
#include "detail/utils.h"
#include "detail/zip_view.h"


namespace OrthoTree
{
  namespace detail
  {
    template<std::size_t CHILD_NO, typename NodeID, typename ChildID, typename EntityID, typename Geometry>
    class OrthoTreeNodeData
    {
    private:
      constexpr static auto IS_SPARSE_CHILDREN_CONTAINER = CHILD_NO > 8;

      constexpr static bool IS_BITSET_BASED_FLAGS = CHILD_NO > 64;
      using ChildFlags = std::conditional_t<(CHILD_NO > 32), uint64_t, std::conditional_t<(CHILD_NO > 8), uint32_t, uint8_t>>;
      using ChildIndex = std::conditional_t<IS_BITSET_BASED_FLAGS, std::vector<std::size_t>, ChildFlags>;

    public:
      using ChildContainer = std::conditional_t<IS_SPARSE_CHILDREN_CONTAINER, typename std::vector<NodeID>, detail::inplace_vector<NodeID, CHILD_NO>>;
      using EntityContainer = detail::MemoryResource<EntityID>::MemorySegment;

    private:
      ChildIndex m_childIndex = {};
      ChildContainer m_children = {};
      EntityContainer m_entities = {};

      Geometry m_geometry;

    public:
      explicit constexpr OrthoTreeNodeData() noexcept = default;

      constexpr Geometry const& GetGeometry() const noexcept { return m_geometry; }
      constexpr Geometry& GetGeometry() noexcept { return m_geometry; }

      void Clear() noexcept
      {
        m_entities = {};
        m_children = {};
      }

    public: // Entity handling
      constexpr auto const& GetEntities() const noexcept { return m_entities.segment; }

      constexpr auto& GetEntities() noexcept { return m_entities.segment; }

      constexpr std::size_t GetEntitiesSize() const noexcept { return m_entities.segment.size(); }

      constexpr bool IsEntitiesEmpty() const noexcept { return m_entities.segment.empty(); }

      constexpr bool ContainsEntity(EntityID entityID) const noexcept
      {
        return std::find(m_entities.segment.begin(), m_entities.segment.end(), entityID) != m_entities.segment.end();
      }

      constexpr void ReplaceEntities(std::span<EntityID> entities) noexcept { m_entities.segment = std::move(entities); }

      constexpr bool RemoveEntity(EntityID entityID) noexcept
      {
        auto const endIteratorAfterRemove = std::remove(m_entities.segment.begin(), m_entities.segment.end(), entityID);
        if (endIteratorAfterRemove == m_entities.segment.end())
          return false; // id was not registered previously.

        return true;
      }

      constexpr void DecreaseEntityIDs(EntityID removedEntityID) noexcept
      {
        for (auto& id : m_entities.segment)
          id -= removedEntityID < id;
      }

      EntityContainer& GetEntitySegment() noexcept { return m_entities; }

    public: // Child handling
      constexpr void AddChild(ChildID childID, NodeID nodeID) noexcept
      {
        std::size_t elementID = 0;
        bool shouldOverwrite = false;
        if constexpr (IS_BITSET_BASED_FLAGS)
        {
          auto const it = std::ranges::lower_bound(m_childIndex, childID);
          elementID = std::distance(m_childIndex.begin(), it);
          shouldOverwrite = elementID < m_childIndex.size() && m_childIndex[elementID] == childID;
          if (!shouldOverwrite)
            m_childIndex.insert(it, childID);
        }
        else
        {
          ChildFlags const childFlag = (ChildFlags{ 1 } << childID);
          ChildFlags const childMask = childFlag - ChildFlags{ 1 };
          elementID = std::popcount(ChildFlags(m_childIndex & childMask));
          shouldOverwrite = m_childIndex & childFlag;
          m_childIndex |= childFlag;
        }

        if (shouldOverwrite)
          m_children[elementID] = nodeID;
        else
          m_children.insert(m_children.begin() + elementID, nodeID);
      }

      constexpr NodeID GetChild(ChildID childID) noexcept
      {
        std::size_t elementID = 0;
        if constexpr (IS_BITSET_BASED_FLAGS)
        {
          auto const it = std::ranges::lower_bound(m_childIndex, childID);
          elementID = std::distance(m_childIndex.begin(), it);
        }
        else
        {
          ChildFlags const childFlag = (ChildFlags{ 1 } << childID);
          ChildFlags const childMask = childFlag - ChildFlags{ 1 };
          elementID = std::popcount(ChildFlags(m_childIndex & childMask));
        }

        return m_children[elementID];
      }

      constexpr bool HasChild(ChildID childID) const noexcept
      {
        if constexpr (IS_BITSET_BASED_FLAGS)
        {
          return std::ranges::binary_search(m_childIndex, childID);
        }
        else
        {
          return m_childIndex & (ChildFlags{ 1 } << childID);
        }
      }

      constexpr void RemoveChild(ChildID childID) noexcept
      {
        std::size_t elementID = 0;
        if constexpr (IS_BITSET_BASED_FLAGS)
        {
          auto const it = std::ranges::lower_bound(m_childIndex, childID);
          elementID = std::distance(m_childIndex.begin(), it);
          m_childIndex.erase(it);
        }
        else
        {
          static_assert(std::unsigned_integral<ChildFlags>, "Incompatible type!");

          ChildFlags const childFlag = (ChildFlags{ 1 } << childID);
          ChildFlags const childMask = childFlag - ChildFlags{ 1 };
          elementID = std::popcount(ChildFlags(m_childIndex & childMask));
          m_childIndex &= ~childFlag;
        }

        m_children.erase(m_children.begin() + elementID);
      }

      constexpr bool IsAnyChildExist() const noexcept { return !m_children.empty(); }

      constexpr auto const& GetChildren() const noexcept { return m_children; }
    };
  } // namespace detail


  // TODO: TEntityContainer should be removed from here. It is only required by OctreeContainer to avoid data conversion.
  template<GeometryType GEOMETRY_TYPE_, typename TEntity, typename TEntityID, typename TEntityContainer, typename TEntityContainerView, typename TGeometry, typename TEntityIDHash = std::hash<TEntityID>>
  struct EntityAdapterDefault
  {
  public:
    // OctreeContainer requires the TEntityContainer to eliminate data conversion
    using EntityContainer = TEntityContainer;

    // Generally, EntityContainerView is used in the Core type
    using EntityContainerView = TEntityContainerView;

    // Underlying data, that could be anything
    using Entity = TEntity;

    // TODO: TEntityID is possible not default-constructible, it should be handled
    // Trivially copyable type, that is stored in the nodes
    using EntityID = TEntityID;

    // Geometric representation of the Entity: Point/Box
    using Geometry = TGeometry;

    // Hash to store EntityID in hashmaps
    using Hash = TEntityIDHash;

    static constexpr GeometryType GEOMETRY_TYPE = GEOMETRY_TYPE_;

  private:
    using EntityContainerViewType = std::remove_cvref_t<EntityContainerView>;

  public:
    static constexpr bool REQUIRES_CONTIGUOUS_ENTITY_IDS = std::contiguous_iterator<typename EntityContainerViewType::iterator> &&
                                                           std::is_same_v<typename EntityContainerViewType::value_type, Geometry> &&
                                                           std::is_integral_v<EntityID>;

  public:
    static constexpr EntityID GetEntityID(EntityContainerView entities, Entity const& entity) noexcept
    {
      return detail::getKeyPart(entities, entity);
    }

    static constexpr Geometry const& GetGeometry(EntityContainerView entities, EntityID const& entityID) noexcept
    {
      return detail::at(entities, entityID);
    }

    static constexpr Geometry const& GetGeometry(Entity const& entity) noexcept { return detail::getValuePart(entity); }
    static constexpr void SetGeometry(Entity& entity, Geometry const& geometry) noexcept { return detail::setValuePart(entity, geometry); }

    static constexpr Entity const& GetEntity(EntityContainerView entities, EntityID entityID) noexcept { return detail::get(entities, entityID); }
    static constexpr Entity& GetEntity(EntityContainer& entities, EntityID entityID) noexcept { return detail::get(entities, entityID); }
  };

  template<typename TPoint>
  using PointEntitySpanAdapter = EntityAdapterDefault<GeometryType::Point, TPoint, index_t, std::vector<TPoint>, std::span<TPoint const>, TPoint>;

  template<typename TBox>
  using BoxEntitySpanAdapter = EntityAdapterDefault<GeometryType::Box, TBox, index_t, std::vector<TBox>, std::span<TBox const>, TBox>;

  template<typename TPoint, typename TEntityContainer = std::unordered_map<index_t, TPoint>, typename TEntityContainerView = TEntityContainer const&>
  using PointEntityMapAdapter =
    EntityAdapterDefault<GeometryType::Point, typename TEntityContainer::value_type, typename TEntityContainer::key_type, TEntityContainer, TEntityContainerView, TPoint>;

  template<typename TBox, typename TEntityContainer = std::unordered_map<index_t, TBox>, typename TEntityContainerView = TEntityContainer const&>
  using BoxEntityMapAdapter =
    EntityAdapterDefault<GeometryType::Box, typename TEntityContainer::value_type, typename TEntityContainer::key_type, TEntityContainer, TEntityContainerView, TBox>;

  template<typename TEntityAdapter, typename TGeometryAdapter, typename TConfiguration>
  class OrthoTreeCoreBase
  {
  protected:
    using EA = TEntityAdapter;
    using GA = TGeometryAdapter;
    using CONFIG = TConfiguration;
    using IGM = detail::InternalGeometryModule<GA>;
    using SI = detail::MortonSpaceIndexing<GA::DIMENSION_NO>;

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

    using NodeGeometry = std::conditional_t<CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MinPoint, IGM_Vector, NodeBox>;

  private:
    std::size_t m_maxElementNum = CONFIG::DEFAULT_TARGET_ELEMENT_NUM_IN_NODES;
    depth_t m_maxDepthID = INVALID_DEPTH;

    struct TreeBox
    {
      IGM::Vector minPoint;
      IGM::Vector size;
    };

    std::conditional_t<CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::None, TreeBox, std::monostate> m_nominalTreeBox;
    std::conditional_t<CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::None && CONFIG::ALLOW_OUT_OF_SPACE_INSERTION, TreeBox, std::monostate> m_realTreeBox;
    std::conditional_t<CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MinPoint, std::vector<typename IGM::Vector>, std::monostate> m_nodeSize;

  public:
    using EntityContainerView = EA::EntityContainerView;
    using EntityID = EA::EntityID;

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
        m_nodeSize = TreeBox{ .minPoint = treeMinPoint, .size = treeSize };
        if constexpr (CONFIG::ALLOW_OUT_OF_SPACE_INSERTION)
        {
          m_nodeSize = TreeBox{ .minPoint = treeMinPoint, .size = treeSize };
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
    }

  public:
    constexpr depth_t GetExaminationLevelID(depth_t depthID) const { return m_maxDepthID - depthID; }


    constexpr typename IGM::Vector CalculateNodeSize(depth_t depthID) const noexcept
      requires(CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::None)
    {
      return IGM::Multiply(m_nodeSize.nominalSize, detail::pow2(depthID));
    }

    // TODO: remove?
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

    constexpr typename IGM::Vector const& GetTreeMinPoint() const noexcept
      requires(CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::None)
    {
      return m_nominalTreeBox.minPoint;
    }

    constexpr typename IGM::Vector const& GetRealTreeMinPoint() const noexcept
      requires(CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::None && CONFIG::ALLOW_OUT_OF_SPACE_INSERTION)
    {
      return m_realTreeBox.minPoint;
    }

    constexpr typename IGM::Vector const& GetRealTreeSize() const noexcept
      requires(CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::None && CONFIG::ALLOW_OUT_OF_SPACE_INSERTION)
    {
      return m_realTreeBox.size;
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

#define ORTHOTREE_DEPENDENT_TYPES(Base)                \
  using EA = Base::EA;                                 \
  using GA = Base::GA;                                 \
  using CONFIG = Base::CONFIG;                         \
  using IGM = Base::IGM;                               \
  using SI = Base::SI;                                 \
  using IGM_Geometry = IGM::Geometry;                  \
                                                       \
  using TScalar = typename GA::Scalar;                 \
  using TFloatScalar = typename GA::FloatScalar;       \
  using TVector = typename GA::Vector;                 \
  using TBox = typename GA::Box;                       \
  using TRay = typename GA::Ray;                       \
  using TPlane = typename GA::Plane;                   \
  using EntityContainerView = EA::EntityContainerView; \
  using EntityID = EA::EntityID


  // Non-owning ortho-tree with compact SoA storage for not changing datasets.
  template<typename TEntityAdapter, typename TGeometryAdapter, typename TConfiguration>
  class StaticLinearOrthoTreeCore : public OrthoTreeCoreBase<TEntityAdapter, TGeometryAdapter, TConfiguration>
  {
  public:
    using Base = OrthoTreeCoreBase<TEntityAdapter, TGeometryAdapter, TConfiguration>;
    ORTHOTREE_DEPENDENT_TYPES(Base);

  public:
    using NodeID = uint32_t;
    using NodeIDCR = NodeID;

    using NodeValue = NodeID;
    using NodeValueCP = NodeID;
    using NodeValueCP = NodeID;

  private:
    static_assert(GA::DIMENSION_NO <= 16);
    static_assert(SI::IS_LINEAR_TREE);

  private:
    template<typename TBegin, typename TLength>
    struct Segment
    {
      using Begin = TBegin;
      using Length = TLength;

      TBegin begin;
      TLength length;
    };
    struct NodeStorage256
    {
      using NodeID = uint8_t;
      using ChildNodeSegment = Segment<uint8_t, uint8_t>;
      using EntitySegment = Segment<uint8_t, uint8_t>;

      std::vector<ChildNodeSegment> nodeChildSegments;
      std::vector<NodeID> nodeChildIDs;
      std::vector<EntitySegment> nodeEntities;
    };
    struct NodeStorage65536
    {
      using NodeID = uint16_t;
      using ChildNodeSegment = Segment<uint16_t, uint8_t>;
      using EntitySegment = Segment<uint16_t, uint16_t>;

      std::vector<ChildNodeSegment> nodeChildSegments;
      std::vector<NodeID> nodeChildIDs;
      std::vector<EntitySegment> nodeEntities;
    };

    struct NodeStorageGeneral
    {
      using NodeID = uint32_t;
      using ChildNodeSegment = Segment<uint32_t, uint16_t>;
      using EntitySegment = Segment<uint32_t, uint32_t>;

      std::vector<ChildNodeSegment> nodeChildSegments;
      std::vector<NodeID> nodeChildIDs;
      std::vector<EntitySegment> nodeEntities;
    };
    using NodeGeometry =
      std::conditional_t<CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::None, std::monostate, std::vector<typename Base::NodeGeometry>>;

  private: // Data
    std::variant<NodeStorage256, NodeStorage65536, NodeStorageGeneral> m_nodes;
    NodeGeometry m_nodeGeometry;
    std::vector<uint8_t> m_nodeDepthIDs;
    std::vector<EntityID> m_entityStorage;

  public: // Constructors
    // Default constructor. Requires Create call before usage.
    constexpr StaticLinearOrthoTreeCore() = default;

    constexpr StaticLinearOrthoTreeCore(StaticLinearOrthoTreeCore&&) = default;

    // Initialize the base octree structure with entity collection
    explicit StaticLinearOrthoTreeCore(
      EA::EntityContainerView entities,
      std::optional<depth_t> maxDepthIDIn = std::nullopt,
      std::optional<TBox> boxSpaceOptional = std::nullopt,
      std::size_t maxElementNoInNode = CONFIG::DEFAULT_TARGET_ELEMENT_NUM_IN_NODES,
      bool isParallelExec = false) noexcept
    {
      auto isSuccessfullyInsertedAllElements = false;
      if (isParallelExec)
        isSuccessfullyInsertedAllElements = Create<true>(entities, maxDepthIDIn, std::move(boxSpaceOptional), maxElementNoInNode);
      else
        isSuccessfullyInsertedAllElements = Create<false>(entities, maxDepthIDIn, std::move(boxSpaceOptional), maxElementNoInNode);
      assert(isSuccessfullyInsertedAllElements);
    }

    // Initialize the base octree structure with entity collection and parallel tree-building option
    template<typename EXEC_TAG>
    StaticLinearOrthoTreeCore(
      EXEC_TAG,
      EntityContainerView entities,
      std::optional<depth_t> maxDepthIDIn = std::nullopt,
      std::optional<TBox> boxSpaceOptional = std::nullopt,
      std::size_t maxElementNoInNode = CONFIG::DEFAULT_TARGET_ELEMENT_NUM_IN_NODES) noexcept
    {
      auto isSuccessfullyInsertedAllElements =
        Create<std::is_same_v<EXEC_TAG, ExecutionTags::Parallel>>(entities, maxDepthIDIn, std::move(boxSpaceOptional), maxElementNoInNode);
      assert(isSuccessfullyInsertedAllElements);
    }

  public:
    constexpr TBox GetBox() const noexcept
    {
      auto box = GA::MakeBox();

      auto const& minPoint = [&]() -> decltype(auto) {
        if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MBR)
          return m_nodeGeometry[GetRootNodeID()].minPoint;
        else if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MinPoint)
          return m_nodeGeometry[GetRootNodeID()];
        else if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::None)
        {
          if constexpr (CONFIG::ALLOW_OUT_OF_SPACE_INSERTION)
            return Base::GetRealTreeMinPoint();
          else
            return Base::GetTreeMinPoint();
        }
      }();

      auto const& size = [&]() -> decltype(auto) {
        if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MBR)
          return m_nodeGeometry[GetRootNodeID()].size;
        else if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MinPoint)
          return GetNodeSize(GetRootNodeID());
        else if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::None)
        {
          if constexpr (CONFIG::ALLOW_OUT_OF_SPACE_INSERTION)
            return Base::GetRealTreeSize();
          else
            return Base::GetTreeSize();
        }
      }();

      detail::static_for<GA::DIMENSION_NO>([&](auto dimensionID) {
        GA::SetBoxMinC(box, dimensionID, minPoint[dimensionID]);
        GA::SetBoxMaxC(box, dimensionID, minPoint[dimensionID] + size[dimensionID]);
      });

      return box;
    }

    static NodeID GetRootNodeID() noexcept { return 0; }

    constexpr NodeID GetRootNodeValue() const noexcept { return GetRootNodeID(); }

    constexpr NodeID GetNodeValueP(NodeID nodeID) const noexcept { return nodeID; }

    constexpr std::size_t GetNodeCount() const noexcept { return m_nodeDepthIDs.size(); }

    constexpr std::size_t GetNodeEntityCount(NodeID nodeID) const noexcept
    {
      return std::visit([](auto const& nodes) { return nodes.nodeEntities[nodeID].length; }, m_nodes);
    }

    constexpr std::span<NodeID> GetNodeChildren(NodeID nodeID) const noexcept
    {
      return std::visit(m_nodes, [nodeID](auto const& nodes) {
        auto const& [begin, length] = nodes.nodeChildIDs[nodeID];
        return std::span<NodeID>(&nodes.nodeChildSegments[begin], length);
      });
    }

    constexpr std::span<EntityID const> GetNodeEntities(NodeID nodeID) const noexcept
    {
      return std::visit(m_nodes, [nodeID](auto const& nodes) {
        auto const& [begin, length] = nodes.nodeEntities[nodeID];
        return std::span<EntityID const>(&m_entityStorage[begin], length);
      });
    }

    constexpr bool IsNodeEntitiesEmpty(NodeID nodeID) const noexcept
    {
      return std::visit(m_nodes, [nodeID](auto const& nodes) {
        auto const& [_, length] = nodes.nodeEntities[nodeID];
        return length == 0;
      });
    }

    constexpr decltype(auto) GetNodeMinPoint(NodeID nodeID) const noexcept
    {
      static_assert(CONFIG::NODE_GEOMETRY_STORAGE != NodeGeometryStorage::None, "Real-time min-point calculation is not supported!");

      if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MinPoint)
      {
        return m_nodeGeometry[nodeID];
      }
      else if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MBR)
      {
        return m_nodeGeometry[nodeID].minPoint;
      }
      else
      {
        static_assert(false, "Unsupported node geometry storage!");
      }
    }

    constexpr decltype(auto) GetNodeSize(NodeID nodeID) const noexcept
    {
      if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MinPoint || CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::None)
      {
        return Base::GetNodeSize(m_nodeDepthIDs[nodeID]);
      }
      else if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MBR)
      {
        return m_nodeGeometry[nodeID].size;
      }
      else
      {
        static_assert(false, "Unsupported node geometry storage!");
      }
    }

    constexpr IGM::Box GetNodeBox(NodeID nodeID) const noexcept
    {
      if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MinPoint)
      {
        return Base::GetNodeBox(m_nodeDepthIDs[nodeID], m_nodeGeometry[nodeID]);
      }
      else if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MBR)
      {
        auto const& [minPoint, size] = m_nodeGeometry[nodeID];
        return { .Min = minPoint, .Max = IGM::Add(minPoint, size) };
      }
      else
      {
        static_assert(false, "Unsupported node geometry storage!");
      }
    }

  private: // Build
    constexpr void InitBase(IGM::Vector const& boxSpaceMinPoint, IGM::Vector const& boxSpaceSize, depth_t maxDepthID, std::size_t maxElementNo) noexcept
    {
      Base::InitBase(boxSpaceMinPoint, boxSpaceSize, maxDepthID, maxElementNo);

      ORTHOTREE_CRASH_IF(
        GetNodeCount() > 0, "To build/setup/create the tree, use the Create() [recommended] or Init() function. If an already built tree is wanted to be reset, use the Reset() function before Init().");
    }

    IGM::Box InitializeSubtreeMinimalNodeGeometry(NodeID nodeID, EntityContainerView entities)
    {
      if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MBR)
      {
        auto nodeBox = IGM::BoxInvertedInit();

        // Union with all children
        for (auto const& childNodeID : GetNodeChildren(nodeID))
        {
          auto childNodeBox = InitializeSubtreeMinimalNodeGeometry(childNodeID, entities);
          IGM::UniteInBoxAD(nodeBox, childNodeBox);
        }

        // Union with all entities
        for (auto const& entityID : GetNodeEntities(nodeID))
          IGM::UniteInBoxAD(nodeBox, EA::GetGeometry(entities, entityID));

        m_nodeGeometry[nodeID] = { nodeBox.Min, IGM::Sub(nodeBox.Max, nodeBox.Min) };
        return nodeBox;
      }
      else
      {
        return {};
      }
    }
  private:
    constexpr std::size_t GetMaxPossibleNodeCount(std::size_t entityCount) const noexcept
    {
      // Assuming perfect tree
      // - All entities are in a leaf node as a single entity

      auto const calculateNodeNum = [](depth_t depthID) {
        return (detail::pow2(GA::DIMENSION_NO * depthID) - 1) / (SI::CHILD_NO - 1);
      };

      auto const maxNodeNumByMaxDepth = calculateNodeNum(Base::GetMaxDepthID());
      auto const possiblyFullLevels = std::min(Base::GetMaxDepthID(), std::bit_width(entityCount - 1) / GA::DIMENSION_NO);
      auto const maxNodeNumByEntities = calculateNodeNum(possiblyFullLevels) + (Base::GetMaxDepthID() - possiblyFullLevels) * entityCount;

      return std::min(maxNodeNumByEntities, maxNodeNumByMaxDepth);
    }

    constexpr IGM::Vector CalculateNodeMinPoint(SI::Location location) const noexcept
    {
      // TODO: finish SI::Decode(location...
      return {};
      // Base::m_grid.CalculateGridCellMinPoint(SI::Decode(location.GetLocationID(), Base::m_maxDepthID));
    }

    constexpr void InitNodeGeometry(NodeID nodeID, SI::Location location) noexcept
    {
      if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::None)
      {
        return;
      }
      else if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MinPoint)
      {
        m_nodeGeometry[nodeID] = CalculateNodeMinPoint(location);
      }
      else if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MBR)
      {
        return;
      }
      else
      {
        static_assert(false, "Unsupported node geometry storage!");
      }
    }

    template<typename TNodeStorage>
    static constexpr bool IsFit(std::size_t entityCount, std::size_t maxNodeCount) noexcept
    {
      return maxNodeCount < std::numeric_limits<typename TNodeStorage::NodeID>::max() &&
             entityCount < std::numeric_limits<typename TNodeStorage::EntitySegment::Begin>::max() &&
             SI::CHILD_NO < std::numeric_limits<typename TNodeStorage::ChildNodeSegment::Length>::max();
    }

    constexpr NodeID CreateNode(SI::Location location, uint32_t nodeEntityBeginID, uint32_t nodeEntityCount)
    {
      auto const nodeID = static_cast<NodeID>(m_nodeDepthIDs.size());

      m_nodeDepthIDs.emplace_back(location.GetDepthID());
      std::visit(
        [&](auto& nodes) {
          using NodeStorage = std::decay_t<decltype(nodes)>;
          using EntitySegment = typename NodeStorage::EntitySegment;

          nodes.nodeEntities.emplace_back(
            EntitySegment{ static_cast<EntitySegment::Begin>(nodeEntityBeginID), static_cast<EntitySegment::Length>(nodeEntityCount) });
          nodes.nodeChildIDs.emplace_back(nodeID);
          nodes.nodeChildSegments.emplace_back();
        },
        m_nodes);

      if constexpr (!std::is_same_v<decltype(m_nodeGeometry), std::monostate>)
      {
        m_nodeGeometry.emplace_back();
      }
      InitNodeGeometry(nodeID, location);
      return nodeID;
    }

    // Build the tree in depth-first order
    template<bool ARE_LOCATIONS_SORTED>
    constexpr void Build(auto const& rootBeginLocationIt, auto const& rootEndLocationIt) noexcept
    {
      // Breadth-first build is required to ensure continuous addition of child nodes in nodeChildIDs

      struct NodeProcessingData
      {
        typename SI::Location location = {};
        uint32_t beginID = 0, length = 0;
      };

      auto nodeQueue = std::queue<NodeProcessingData>();
      nodeQueue.push(NodeProcessingData{ {}, 0, detail::size<uint32_t>(rootBeginLocationIt.GetSecond(), rootEndLocationIt.GetSecond()) });

      auto locationIt = rootBeginLocationIt;
      while (!nodeQueue.empty())
      {
        auto [location, beginID, nodeEntityCount] = nodeQueue.front();
        nodeQueue.pop();

        auto depthID = location.GetDepthID();
        auto nodeID = CreateNode(location, beginID, nodeEntityCount);

        if (nodeEntityCount <= Base::GetMaxElementNum() || depthID >= Base::GetMaxDepthID())
        {
          std::visit(
            [&](auto& nodes) {
              using NodeStorage = std::decay_t<decltype(nodes)>;
              using EntitySegment = typename NodeStorage::EntitySegment;

              nodes.nodeEntities[nodeID] = { static_cast<EntitySegment::Begin>(beginID), static_cast<EntitySegment::Length>(nodeEntityCount) };
            },
            m_nodes);

          continue;
        }

        auto beginIt = rootBeginLocationIt + beginID;
        auto endIt = beginIt + nodeEntityCount;

        if constexpr (EA::GEOMETRY_TYPE == GeometryType::Box)
        {
          auto nonRefinableEndIt = beginIt;
          if constexpr (ARE_LOCATIONS_SORTED)
          {
            nonRefinableEndIt = std::partition_point(beginIt, endIt, [depthID](auto const& location) { return location.GetFirst().depthID == depthID; });
          }
          else
          {
            nonRefinableEndIt = std::partition(beginIt, endIt, [depthID](auto const& location) { return location.GetFirst().depthID == depthID; });
          }

          if (beginIt != nonRefinableEndIt)
          {
            auto nonRefinableEntityCount = detail::size<uint32_t>(beginIt.GetSecond(), nonRefinableEndIt.GetSecond());

            std::visit(
              [&](auto& nodes) {
                using NodeStorage = std::decay_t<decltype(nodes)>;
                using EntitySegment = typename NodeStorage::EntitySegment;

                nodes.nodeEntities[nodeID] = { static_cast<EntitySegment::Begin>(beginID), static_cast<EntitySegment::Length>(nonRefinableEntityCount) };
              },
              m_nodes);

            beginID += nonRefinableEntityCount;
            if (beginID == nodeEntityCount)
              continue;

            beginIt = nonRefinableEndIt;
          }
        }

        auto const childNodeSegmentBeginID = nodeID + nodeQueue.size();
        ++depthID;
        auto const examinedLevelID = Base::GetExaminationLevelID(depthID);
        auto const keyGenerator = typename SI::ChildKeyGenerator(location.GetLocationID());

        uint32_t childNodeCount = 0;
        while (beginID < nodeEntityCount)
        {
          auto const& pivot = (*beginIt).GetFirst();
          auto const childChecker = typename SI::ChildCheckerFixedDepth(examinedLevelID, pivot.GetLocationID());
          auto const childID = childChecker.GetChildID(examinedLevelID);

          auto lcah = SI::GetLowestCommonAncestorHelper(pivot);
          auto endChildIt = [&] {
            if constexpr (ARE_LOCATIONS_SORTED)
            {
              // TODO: LCA
              return std::partition_point(beginIt, endIt, [&](auto const& location) { return childChecker.Test(location.GetFirst().GetLocationID()); });
            }
            else
            {
              // TODO: remove
              // return std::partition(beginIt, endIt, [&](auto const& location) { return childChecker.Test(location.GetFirst().locationID); });
              return Partitioning::partition(
                beginIt,
                endIt,
                [&](auto const& location) { return childChecker.Test(location.GetFirst().GetLocationID()); },
                [&](auto const& loc) { lcah.Add(loc.GetFirst()); });
            }
          }();

          auto entityCount = detail::size<uint32_t>(beginIt.GetSecond(), endChildIt.GetSecond());
          nodeQueue.push({ lcah.GetLocation(Base::GetMaxDepthID()), beginID, entityCount });

          ++childNodeCount;
          beginID += entityCount;
          beginIt = endChildIt;
        }

        // Child node registration
        std::visit(
          [&](auto& nodes) {
            using NodeStorage = std::decay_t<decltype(nodes)>;
            using ChildNodeSegment = typename NodeStorage::ChildNodeSegment;

            nodes.nodeChildSegments[nodeID] = { static_cast<ChildNodeSegment::Begin>(childNodeSegmentBeginID),
                                                static_cast<ChildNodeSegment::Length>(childNodeCount) };
          },
          m_nodes);
      }
    }

  public: // Create
    // Create
    template<bool IS_PARALLEL_EXEC = false, bool ARE_ENTITIES_SURELY_IN_MODELSPACE = false>
    bool Create(
      EntityContainerView entities,
      std::optional<depth_t> maxDepthIn = std::nullopt,
      std::optional<TBox> boxSpaceOptional = std::nullopt,
      std::size_t maxElementNumInNode = CONFIG::DEFAULT_TARGET_ELEMENT_NUM_IN_NODES) noexcept
    {
      auto const boxSpace = boxSpaceOptional ? IGM::GetBoxAD(*boxSpaceOptional) : IGM::template GetBoundingBoxAD<EA>(entities);
      auto const entityCount = entities.size();
      auto const maxDepthID = (!maxDepthIn || maxDepthIn == depth_t{})
                              ? detail::EstimateMaxDepth<GA::DIMENSION_NO, SI::MAX_THEORETICAL_DEPTH_ID>(entityCount, maxElementNumInNode)
                              : *maxDepthIn;
      auto const spaceIndexing = detail::MortonGridSpaceIndexing<GA, CONFIG::ALLOW_OUT_OF_SPACE_INSERTION, CONFIG::LOOSE_FACTOR>(maxDepthID, boxSpace);
      InitBase(spaceIndexing.GetMinPoint(), spaceIndexing.GetSize(), maxDepthID, maxElementNumInNode);

      if (entityCount == 0)
        return true;


      auto mortonIDs = std::vector<typename SI::Location>(entityCount);
      m_entityStorage.resize(entityCount);

      auto locationsZip = detail::zip_view(mortonIDs, m_entityStorage);
      using Location = decltype(locationsZip)::iterator::value_type;

      EXEC_POL_DEF(epf); // GCC 11.3
      std::transform(EXEC_POL_ADD(epf) entities.begin(), entities.end(), locationsZip.begin(), [&](auto const& entity) -> Location {
        return { spaceIndexing.GetLocation(EA::GetGeometry(entity)), EA::GetEntityID(entities, entity) };
      });

      auto endIt = locationsZip.end();
      if constexpr (!ARE_ENTITIES_SURELY_IN_MODELSPACE && !CONFIG::ALLOW_OUT_OF_SPACE_INSERTION)
      {
        endIt = std::partition(locationsZip.begin(), endIt, [](auto const& element) { return element.GetFirst().GetDepthID() != INVALID_DEPTH; });
      }

      constexpr bool ARE_LOCATIONS_SORTED = IS_PARALLEL_EXEC;
      if constexpr (ARE_LOCATIONS_SORTED)
      {
        EXEC_POL_DEF(eps); // GCC 11.3
        std::sort(EXEC_POL_ADD(eps) locationsZip.begin(), endIt, [](Location const& l, Location const& r) { return l.first < r.first; });
      }

      auto const maxNodeCount = GetMaxPossibleNodeCount(entityCount);
      if (IsFit<NodeStorage256>(entityCount, maxNodeCount))
        m_nodes = NodeStorage256{};
      else if (IsFit<NodeStorage65536>(entityCount, maxNodeCount))
        m_nodes = NodeStorage65536{};
      else
        m_nodes = NodeStorageGeneral{};

      auto const estimatedNodeNum =
        detail::EstimateNodeNumber<GA::DIMENSION_NO, SI::MAX_THEORETICAL_DEPTH_ID>(entityCount, maxDepthID, maxElementNumInNode);

      std::visit(
        [&](auto& nodes) {
          nodes.nodeChildSegments.reserve(estimatedNodeNum);
          nodes.nodeChildIDs.reserve(estimatedNodeNum);
          nodes.nodeEntities.reserve(estimatedNodeNum);
        },
        m_nodes);

      m_nodeDepthIDs.reserve(estimatedNodeNum);
      if constexpr (!std::is_same_v<decltype(m_nodeGeometry), std::monostate>)
        m_nodeGeometry.reserve(estimatedNodeNum);

      Build<ARE_LOCATIONS_SORTED>(locationsZip.begin(), endIt);
      InitializeSubtreeMinimalNodeGeometry(GetRootNodeID(), entities);
      return true;
    }

  public: // Create
    // Create
    template<bool IS_PARALLEL_EXEC = false>
    static bool Create(
      StaticLinearOrthoTreeCore& tree,
      EntityContainerView entities,
      std::optional<depth_t> maxDepthIn = std::nullopt,
      std::optional<TBox> boxSpaceOptional = std::nullopt,
      std::size_t maxElementNoInNode = CONFIG::DEFAULT_TARGET_ELEMENT_NUM_IN_NODES) noexcept
    {
      return tree.Create<IS_PARALLEL_EXEC>(entities, maxDepthIn, std::move(boxSpaceOptional), maxElementNoInNode);
    }
  };


  // Non-owning ortho-tree with dynamic storage for changing datasets.
  template<typename TEntityAdapter, typename TGeometryAdapter, typename TConfiguration>
  class DynamicHashOrthoTreeCore : public OrthoTreeCoreBase<TEntityAdapter, TGeometryAdapter, TConfiguration>
  {
  public:
    using Base = OrthoTreeCoreBase<TEntityAdapter, TGeometryAdapter, TConfiguration>;
    ORTHOTREE_DEPENDENT_TYPES(Base);

  public:
    using NodeID = typename SI::NodeID;
    using NodeIDCR = typename SI::NodeIDCR;

    using NodeGeometry = std::conditional_t<CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::None, std::monostate, typename Base::NodeGeometry>;
    using Node = detail::OrthoTreeNodeData<SI::CHILD_NO, NodeID, typename SI::ChildID, EntityID, NodeGeometry>;
    using NodeValue = std::pair<NodeID const, Node>;
    using NodeValueCP = std::pair<NodeID const, Node> const*;

#ifdef ORTHOTREE_IS_PMR_USED
    template<typename TData>
    using LinearNodeContainer = std::pmr::unordered_map<NodeID, TData>;

    template<typename TData>
    using NonLinearNodeContainer = std::pmr::map<NodeID, TData, bitset_arithmetic_compare>;
#else
    template<typename TData>
    using LinearNodeContainer = typename CONFIG::LinearNodeContainer<NodeID, TData, std::hash<NodeID>>;

    template<typename TData>
    using NonLinearNodeContainer = std::map<NodeID, TData, bitset_arithmetic_compare>;
#endif // ORTHOTREE_IS_PMR_USED

    template<typename TData>
    using NodeContainer = typename std::conditional_t<SI::IS_LINEAR_TREE, LinearNodeContainer<TData>, NonLinearNodeContainer<TData>>;

  protected: // Member variables
#ifdef ORTHOTREE_IS_PMR_USED
    std::pmr::unsynchronized_pool_resource m_umrNodes;
    NodeContainer<Node> m_nodes = NodeContainer<Node>(&m_umrNodes);
#else
    NodeContainer<Node> m_nodes;
#endif

    using ReverseMapType = typename CONFIG::template ReverseMap<EntityID, NodeID, typename EA::Hash>;
    using ReverseMap = std::conditional_t<CONFIG::USE_REVERSE_MAPPING, ReverseMapType, std::monostate>;
    ReverseMap m_reverseMap;

    detail::MortonGridSpaceIndexing<GA, CONFIG::ALLOW_OUT_OF_SPACE_INSERTION, CONFIG::LOOSE_FACTOR> m_spaceIndexing;
    detail::MemoryResource<EntityID> m_memoryResource;

  public: // Constructors
    // Default constructor. Requires InitBase call before usage.
    constexpr DynamicHashOrthoTreeCore() = default;

    constexpr DynamicHashOrthoTreeCore(DynamicHashOrthoTreeCore&&) = default;

    // Initialize the base octree structure
    constexpr DynamicHashOrthoTreeCore(
      TBox const& boxSpace,
      depth_t maxDepthID,
      std::size_t maxElementNo = CONFIG::DEFAULT_TARGET_ELEMENT_NUM_IN_NODES,
      std::size_t estimatedEntityNo = detail::MemoryResource<EntityID>::DEFAULT_PAGE_SIZE) noexcept
    {
      InitBase(IGM::GetBoxAD(boxSpace), maxDepthID, maxElementNo, estimatedEntityNo);
    }

    // Initialize the base octree structure with entity collection
    explicit DynamicHashOrthoTreeCore(
      EntityContainerView entities,
      std::optional<depth_t> maxDepthIDIn = std::nullopt,
      std::optional<TBox> boxSpaceOptional = std::nullopt,
      std::size_t maxElementNoInNode = CONFIG::DEFAULT_TARGET_ELEMENT_NUM_IN_NODES,
      bool isParallelExec = false) noexcept
    {
      auto isSuccessfullyInsertedAllElements = false;
      if (isParallelExec)
        isSuccessfullyInsertedAllElements = Create<true>(entities, maxDepthIDIn, std::move(boxSpaceOptional), maxElementNoInNode);
      else
        isSuccessfullyInsertedAllElements = Create<false>(entities, maxDepthIDIn, std::move(boxSpaceOptional), maxElementNoInNode);

      assert(isSuccessfullyInsertedAllElements);
    }

    // Initialize the base octree structure with entity collection and parallel tree-building option
    template<typename EXEC_TAG>
    DynamicHashOrthoTreeCore(
      EXEC_TAG,
      EntityContainerView entities,
      std::optional<depth_t> maxDepthIDIn = std::nullopt,
      std::optional<TBox> boxSpaceOptional = std::nullopt,
      std::size_t maxElementNoInNode = CONFIG::DEFAULT_TARGET_ELEMENT_NUM_IN_NODES) noexcept
    {
      auto isSuccessfullyInsertedAllElements =
        Create<std::is_same_v<EXEC_TAG, ExecutionTags::Parallel>>(entities, maxDepthIDIn, std::move(boxSpaceOptional), maxElementNoInNode);
      assert(isSuccessfullyInsertedAllElements);
    }

    DynamicHashOrthoTreeCore(DynamicHashOrthoTreeCore const& other)
    : Base(other)
#ifdef ORTHOTREE_IS_PMR_USED
    , m_umrNodes()
    , m_nodes(&m_umrNodes)
#else
    : m_nodes(other.m_nodes)
    , m_spaceIndexing(other.m_spaceIndexing)
#endif
    {
#ifdef ORTHOTREE_IS_PMR_USED
      m_nodes = other.m_nodes;
#endif

      auto segments = std::vector<typename detail::MemoryResource<EntityID>::MemorySegment*>(m_nodes.size());
      int i = 0;
      for (auto& [key, node] : m_nodes)
      {
        segments[i] = &node.GetEntitySegment();
        ++i;
      }
      other.m_memoryResource.Clone(m_memoryResource, segments);
    }

    DynamicHashOrthoTreeCore& operator=(DynamicHashOrthoTreeCore const& other)
    {
      if (this == &other)
        return *this;

      Base::operator=(other);

      m_nodes = other.m_nodes;

      // using MR = detail::MemoryResource<EntityID>;
      auto segments = std::vector<typename detail::MemoryResource<EntityID>::MemorySegment*>(m_nodes.size());
      int i = 0;
      for (auto& [key, node] : m_nodes)
      {
        segments[i] = &node.GetEntitySegment();
        ++i;
      }
      other.m_memoryResource.Clone(m_memoryResource, segments);
      return *this;
    }

  public:
    constexpr TBox GetBox() const noexcept
    {
      auto box = GA::MakeBox();

      auto const& minPoint = [&]() -> decltype(auto) {
        if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MBR)
          return m_nodes.at(GetRootNodeID()).GetGeometry().minPoint;
        else if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MinPoint)
          return m_nodes.at(GetRootNodeID()).GetGeometry();
        else if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::None)
        {
          if constexpr (CONFIG::ALLOW_OUT_OF_SPACE_INSERTION)
            return Base::GetRealTreeMinPoint();
          else
            return Base::GetTreeMinPoint();
        }
      }();

      auto const& size = [&]() -> decltype(auto) {
        if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MBR)
          return m_nodes.at(GetRootNodeID()).GetGeometry().size;
        else if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MinPoint)
          return m_nodes.at(GetRootNodeID()).GetGeometry();
        else if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::None)
        {
          if constexpr (CONFIG::ALLOW_OUT_OF_SPACE_INSERTION)
            return Base::GetRealTreeSize();
          else
            return Base::GetTreeSize();
        }
      }();

      detail::static_for<GA::DIMENSION_NO>([&](auto dimensionID) {
        GA::SetBoxMinC(box, dimensionID, minPoint[dimensionID]);
        GA::SetBoxMaxC(box, dimensionID, minPoint[dimensionID] + size[dimensionID]);
      });

      return box;
    }

  public: // Node helpers
    constexpr auto const& GetNodes() const noexcept { return m_nodes; }
    constexpr std::size_t GetNodeCount() const noexcept { return m_nodes.size(); }
    bool HasNode(NodeIDCR key) const noexcept { return m_nodes.contains(key); }
    static NodeID GetRootNodeID() noexcept { return SI::GetRootKey(); }
    auto const& GetNode(NodeIDCR key) const noexcept { return m_nodes.at(key); }

    NodeValueCP GetNodeValueP(NodeIDCR nodeID) const noexcept { return &(*m_nodes.find(nodeID)); }
    NodeValueCP GetRootNodeValue() const noexcept { return GetNodeValueP(SI::GetRootKey()); }
    NodeValue* GetNodeValueP(NodeIDCR nodeID) noexcept { return &(*m_nodes.find(nodeID)); }
    NodeValue* GetRootNodeValue() noexcept { return GetNodeValueP(SI::GetRootKey()); }

    // Get EntityIDs of the node
    constexpr auto const& GetNodeEntities(NodeValueCP pNodeValue) const noexcept { return pNodeValue->second.GetEntities(); }

    // Get EntityIDs number of the node
    constexpr std::size_t GetNodeEntityCount(NodeValueCP nodeValue) const noexcept { return nodeValue->second.GetEntitiesSize(); }

    // Get Node min-point
    constexpr decltype(auto) GetNodeMinPoint(NodeValueCP pNodeValue) const noexcept
    {
      if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::None)
      {
        auto const nodeCenter = m_spaceIndexing.CalculateNodeCenter(pNodeValue->first);
        auto const depthID = SI::GetDepthID(pNodeValue->first);
        auto const& nodeHalfSize = Base::GetNodeSize(depthID + 1);

        return IGM::Sub(nodeCenter, nodeHalfSize);
      }
      else if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MinPoint)
        return pNodeValue->second.GetGeometry();
      else if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MBR)
        return pNodeValue->second.GetGeometry().minPoint;
      else
        static_assert(false, "Unsupported node geometry storage!");
    }

    // Get Node size
    constexpr decltype(auto) GetNodeSize(NodeValueCP pNodeValue) const noexcept
    {
      auto const depthID = SI::GetDepthID(pNodeValue->first);
      if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::None || CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MinPoint)
        return Base::GetNodeSize(depthID);
      else if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MBR)
        return pNodeValue->second.GetGeometry().size;
      else
        static_assert(false, "Unsupported node geometry storage!");
    }

    // Get Child Node IDs
    constexpr decltype(auto) GetNodeChildren(NodeValueCP pNodeValue) const noexcept { return pNodeValue->second.GetChildren(); }

    // Get Node box
    constexpr IGM::Box GetNodeBox(NodeValueCP pNodeValue) const noexcept
    {
      auto const& minPoint = GetNodeMinPoint(pNodeValue);
      return { .Min = minPoint, .Max = IGM::Add(minPoint, GetNodeSize(pNodeValue)) };
    }

    // Is the node has any entity
    constexpr bool IsNodeEntitiesEmpty(NodeValueCP nodeValue) const noexcept { return nodeValue->second.IsEntitiesEmpty(); }

    // TODO: remove these
    // Get EntityIDs number of the node
    constexpr std::size_t GetNodeEntityCount(NodeIDCR nodeKey) const noexcept { return GetNodeEntityCount(GetNode(nodeKey)); }

    // Get EntityIDs of the node
    constexpr auto const& GetNodeEntities(Node const& node) const noexcept { return node.GetEntities(); }

    // Get EntityIDs of the node
    constexpr auto const& GetNodeEntities(NodeIDCR nodeKey) const noexcept { return GetNodeEntities(GetNode(nodeKey)); }

    // Get EntityIDs number of the node
    constexpr std::size_t GetNodeEntityCount(Node const& node) const noexcept { return node.GetEntitiesSize(); }

    // Obsolete: Is the node has any entity
    constexpr bool IsNodeEntitiesEmpty(NodeIDCR nodeKey) const noexcept { return IsNodeEntitiesEmpty(GetNodeValueP(nodeKey)); }

    // Obsolete
    constexpr decltype(auto) GetNodeMinPoint(NodeIDCR nodeID) const noexcept { return GetNodeMinPoint(&*m_nodes.find(nodeID)); }

    // Obsolete
    constexpr decltype(auto) GetNodeSize(NodeIDCR nodeID) const noexcept { return GetNodeSize(&*m_nodes.find(nodeID)); }

  protected:
    constexpr void AddNodeEntity(NodeValue* nodeValue, EntityID newEntity) noexcept
    {
      m_memoryResource.IncreaseSegment(nodeValue->second.GetEntitySegment(), 1);
      nodeValue->second.GetEntities().back() = std::move(newEntity);
      UpdateReverseMap(newEntity, nodeValue->first);
    }

    constexpr bool RemoveNodeEntity(Node& node, EntityID entity) noexcept
    {
      auto const isRemoved = node.RemoveEntity(entity);
      if (isRemoved)
        m_memoryResource.DecreaseSegment(node.GetEntitySegment(), 1);

      return isRemoved;
    }

    constexpr void ClearNodeEntities(Node& node) noexcept
    {
      auto& ms = node.GetEntitySegment();
      m_memoryResource.DecreaseSegment(ms, ms.segment.size());
    }

    constexpr void ResizeNodeEntities(Node& node, std::size_t size) noexcept
    {
      auto& ms = node.GetEntitySegment();
      m_memoryResource.DecreaseSegment(ms, ms.segment.size() - size);
    }

    constexpr void InitNodeGeometry(NodeValue* nodeValue) const noexcept
    {
      if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MinPoint)
      {
        auto const nodeCenter = m_spaceIndexing.CalculateNodeCenter(nodeValue->first);
        auto const depthID = SI::GetDepthID(nodeValue->first);
        auto const& nodeHalfSize = Base::GetNodeSize(depthID + 1);

        nodeValue->second.GetGeometry() = IGM::Sub(nodeCenter, nodeHalfSize);
      }
      else if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MBR)
      {
        nodeValue->second.GetGeometry() = NodeGeometry{ .minPoint = { std::numeric_limits<IGM_Geometry>::max() }, .size = {} };
      }
    }

    bool IsNodeGeometryInitialized(NodeGeometry const& nodeGeometry)
    {
      if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MBR)
      {
        auto& [minPoint, _] = nodeGeometry;
        return minPoint[0] != std::numeric_limits<IGM_Geometry>::max();
      }
      else
      {
        return true;
      }
    }
    // TODO: remove?
    constexpr NodeValue CreateNode(NodeIDCR nodeID) const noexcept
    {
      if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::None)
      {
        return NodeValue{ nodeID, Node() };
      }
      else if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MinPoint || CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MBR)
      {
        auto nodeValue = NodeValue{ nodeID, Node() };
        InitNodeGeometry(&nodeValue);
        return nodeValue;
      }
      else
      {
        static_assert(false, "Unsupported node geometry storage!");
      }
    }

  private:
    constexpr void InitBase(IGM::Box const& boxSpace, depth_t maxDepthID, std::size_t maxElementNo, std::size_t estimatedEntityNo) noexcept
    {
      m_spaceIndexing = detail::MortonGridSpaceIndexing<GA, CONFIG::ALLOW_OUT_OF_SPACE_INSERTION, CONFIG::LOOSE_FACTOR>(maxDepthID, boxSpace);
      Base::InitBase(m_spaceIndexing.GetMinPoint(), m_spaceIndexing.GetSize(), maxDepthID, maxElementNo);

      ORTHOTREE_CRASH_IF(
        !m_nodes.empty(), "To build/setup/create the tree, use the Create() [recommended] or Init() function. If an already built tree is wanted to be reset, use the Reset() function before Init().");

      m_nodes.emplace(CreateNode(SI::GetRootKey()));
      m_memoryResource.Init(estimatedEntityNo);
    }

  public: // Main service functions
    // Alternative creation mode (instead of Create), Init then Insert items into leafs one by one. NOT RECOMMENDED.
    constexpr void Init(
      TBox const& box,
      depth_t maxDepthID,
      std::size_t maxElementNo = 11,
      std::size_t estimatedEntityNo = detail::MemoryResource<EntityID>::DEFAULT_PAGE_SIZE) noexcept
    {
      InitBase(IGM::GetBoxAD(box), maxDepthID, maxElementNo, estimatedEntityNo);
    }

  private: // Build
    using LocationIterator = typename detail::zip_view<std::vector<typename SI::Location>, std::span<EntityID>>::iterator;

    struct NodeProcessingData
    {
      std::pair<NodeID, Node> NodeInstance;
      LocationIterator EndLocationIt;
    };
    template<bool ARE_LOCATIONS_SORTED>
    constexpr void ProcessNode(depth_t depthID, LocationIterator& locationIt, NodeProcessingData& nodeProcessingData)
    {
      auto& [node, endLocationIt] = nodeProcessingData;

      auto const subtreeEntityNo = detail::size(locationIt, endLocationIt);
      if (subtreeEntityNo == 0)
        return;

      auto nodeEntityNo = subtreeEntityNo;
      auto stuckedEndLocationIt = endLocationIt;
      if (subtreeEntityNo > this->Base::GetMaxElementNum() && depthID < Base::GetMaxDepthID())
      {
        if constexpr (ARE_LOCATIONS_SORTED)
        {
          stuckedEndLocationIt =
            std::partition_point(locationIt, endLocationIt, [depthID](auto const& location) { return location.GetFirst().depthID == depthID; });
        }
        else
        {
          stuckedEndLocationIt =
            std::partition(locationIt, endLocationIt, [depthID](auto const& location) { return location.GetFirst().depthID == depthID; });
        }

        nodeEntityNo = std::size_t(std::distance(locationIt, stuckedEndLocationIt));
      }

      if (nodeEntityNo == 0)
        return;

      node.second.ReplaceEntities(std::span(locationIt.GetSecond(), stuckedEndLocationIt.GetSecond()));

      if constexpr (CONFIG::USE_REVERSE_MAPPING)
      {
        for (auto it = locationIt.GetSecond(); it != stuckedEndLocationIt.GetSecond(); ++it)
        {
          m_reverseMap.emplace(*it, node.first);
        }
      }

      locationIt += nodeEntityNo;
    }

    template<bool ARE_LOCATIONS_SORTED>
    constexpr void CreateProcessingData(
      depth_t examinedLevelID,
      SI::ChildKeyGenerator const& keyGenerator,
      LocationIterator const& locationIt,
      NodeProcessingData& parentNodeProcessingData,
      NodeProcessingData& nodeProcessingData) const noexcept
    {
      auto const childChecker = typename SI::ChildCheckerFixedDepth(examinedLevelID, (*locationIt).GetFirst().locationID);
      auto const childID = childChecker.GetChildID(examinedLevelID);
      auto childKey = keyGenerator.GetChildNodeKey(childID);

      parentNodeProcessingData.NodeInstance.second.AddChild(childID, childKey);
      if constexpr (ARE_LOCATIONS_SORTED)
      {
        nodeProcessingData.EndLocationIt = std::partition_point(locationIt, parentNodeProcessingData.EndLocationIt, [&](auto const& location) {
          return childChecker.Test(location.GetFirst().locationID);
        });
      }
      else
      {
        nodeProcessingData.EndLocationIt = std::partition(locationIt, parentNodeProcessingData.EndLocationIt, [&](auto const& location) {
          return childChecker.Test(location.GetFirst().locationID);
        });
      }

      nodeProcessingData.NodeInstance = CreateNode(childKey);
    }

    // Build the tree in depth-first order
    template<bool ARE_LOCATIONS_SORTED, typename TResulEntityContainerView>
    constexpr void BuildSubtree(
      LocationIterator const& rootBeginLocationIt,
      LocationIterator const& rootEndLocationIt,
      std::pair<NodeID, Node> const& rootNode,
      TResulEntityContainerView& nodes) noexcept
    {
      auto nodeStack = std::vector<NodeProcessingData>(this->GetDepthNo());
      nodeStack[0] = NodeProcessingData{ rootNode, rootEndLocationIt };

      auto locationIt = rootBeginLocationIt;
      auto constexpr exitDepthID = depth_t(-1);
      for (depth_t depthID = 0; depthID != exitDepthID;)
      {
        if (!nodeStack[depthID].NodeInstance.second.IsAnyChildExist())
        {
          this->template ProcessNode<ARE_LOCATIONS_SORTED>(depthID, locationIt, nodeStack[depthID]);
        }

        auto const isEntitesProcessed = locationIt == nodeStack[depthID].EndLocationIt;

        if (isEntitesProcessed || depthID == Base::GetMaxDepthID())
        {
          detail::emplace(nodes, std::move(nodeStack[depthID].NodeInstance));
          --depthID;
          continue;
        }

        ++depthID;
        auto const examinedLevelID = this->GetExaminationLevelID(depthID);
        auto const keyGenerator = typename SI::ChildKeyGenerator(nodeStack[depthID - 1].NodeInstance.first);
        this->template CreateProcessingData<ARE_LOCATIONS_SORTED>(examinedLevelID, keyGenerator, locationIt, nodeStack[depthID - 1], nodeStack[depthID]);
      }
    }

  public: // Create
    // Create
    template<bool IS_PARALLEL_EXEC = false, bool ARE_ENTITIES_SURELY_IN_MODELSPACE = false>
    bool Create(
      EntityContainerView entities,
      std::optional<depth_t> maxDepthIn = std::nullopt,
      std::optional<TBox> boxSpaceOptional = std::nullopt,
      std::size_t maxElementNoInNode = CONFIG::DEFAULT_TARGET_ELEMENT_NUM_IN_NODES) noexcept
    {
      auto const boxSpace = boxSpaceOptional ? IGM::GetBoxAD(*boxSpaceOptional) : IGM::template GetBoundingBoxAD<EA>(entities);

      auto const entityNo = entities.size();
      auto const maxDepthID = (!maxDepthIn || maxDepthIn == depth_t{})
                              ? detail::EstimateMaxDepth<GA::DIMENSION_NO, SI::MAX_THEORETICAL_DEPTH_ID>(entityNo, maxElementNoInNode)
                              : *maxDepthIn;
      InitBase(boxSpace, maxDepthID, maxElementNoInNode, entityNo);

      if (entityNo == 0)
        return true;

      auto mortonIDs = std::vector<typename SI::Location>(entityNo);
      auto entityIDsView = m_memoryResource.Allocate(entityNo).segment;

      auto locationsZip = detail::zip_view(mortonIDs, entityIDsView);
      using Location = decltype(locationsZip)::iterator::value_type;

      EXEC_POL_DEF(epf); // GCC 11.3
      std::transform(EXEC_POL_ADD(epf) entities.begin(), entities.end(), locationsZip.begin(), [&](auto const& entity) -> Location {
        return { m_spaceIndexing.GetLocation(EA::GetGeometry(entity)), EA::GetEntityID(entities, entity) };
      });

      auto endIt = locationsZip.end();
      if constexpr (!ARE_ENTITIES_SURELY_IN_MODELSPACE && !CONFIG::ALLOW_OUT_OF_SPACE_INSERTION)
      {
        endIt = std::partition(locationsZip.begin(), endIt, [](auto const& element) { return element.GetFirst().GetDepthID() != INVALID_DEPTH; });
      }

      constexpr bool ARE_LOCATIONS_SORTED = IS_PARALLEL_EXEC;
      if constexpr (ARE_LOCATIONS_SORTED)
      {
        EXEC_POL_DEF(eps); // GCC 11.3
        std::sort(EXEC_POL_ADD(eps) locationsZip.begin(), endIt, [](Location const& l, Location const& r) { return l.first < r.first; });
      }

      auto const rootNode = *m_nodes.begin();
      m_nodes.clear();
      detail::reserve(this->m_nodes, detail::EstimateNodeNumber<GA::DIMENSION_NO, SI::MAX_THEORETICAL_DEPTH_ID>(entityNo, maxDepthID, maxElementNoInNode));
      BuildSubtree<ARE_LOCATIONS_SORTED>(locationsZip.begin(), endIt, rootNode, m_nodes);


      if (boxSpaceOptional && !ARE_ENTITIES_SURELY_IN_MODELSPACE)
        InitializeRootNodeBox(entities);
      else
        InitializeSubtreeMinimalNodeGeometry(GetRootNodeValue(), entities);

      return true;
    }

    // Create
    template<bool IS_PARALLEL_EXEC = false>
    static bool Create(
      DynamicHashOrthoTreeCore& tree,
      EntityContainerView entities,
      std::optional<depth_t> maxDepthIn = std::nullopt,
      std::optional<TBox> boxSpaceOptional = std::nullopt,
      std::size_t maxElementNoInNode = CONFIG::DEFAULT_TARGET_ELEMENT_NUM_IN_NODES) noexcept
    {
      return tree.Create<IS_PARALLEL_EXEC>(entities, maxDepthIn, std::move(boxSpaceOptional), maxElementNoInNode);
    }

  public: // BulkInsert // TODO finish
    void BulkInsertV1(EntityContainerView entities, auto EXEC_TAG = SEQ_EXEC) noexcept
    {
      constexpr bool IS_PARALLEL_EXEC = std::is_same_v<std::remove_cvref_t<decltype(EXEC_TAG)>, ExecutionTags::Parallel>;

      auto const entityNo = entities.size();

      auto mortonIDs = std::vector<typename SI::Location>(entityNo);
      auto mainMemorySegment = this->m_memoryResource.Allocate(entityNo);
      auto locationsZip = detail::zip_view(mortonIDs, mainMemorySegment.segment);
      detail::reserve(m_nodes, EstimateNodeNumber(entityNo, Base::GetMaxDepthID(), Base::GetMaxElementNum()));


      using EntityLocation = decltype(locationsZip)::iterator::value_type;
      EXEC_POL_DEF(ept); // GCC 11.3
      std::transform(EXEC_POL_ADD(ept) entities.begin(), entities.end(), locationsZip.begin(), [&](auto const& entity) -> EntityLocation {
        return { this->GetLocation(EA::GetGeometry(entity)), EA::GetEntityID(entities, entity) };
      });

      auto const partitions = Partitioning::Partition<std::min(dim_t(9), GA::DIMENSION_NO * 3), IS_PARALLEL_EXEC>(
        locationsZip.begin(),
        locationsZip.end(),
        [](EntityLocation const& e) -> NodeID { return e.first.locationID; },
        Base::GetMaxElementNum(),
        Base::GetMaxDepthID(),
        GA::DIMENSION_NO);

      auto orphanNodes = std::vector<NodeID>{};
      auto partitionIt = partitions.begin();
      for (auto beginIt = locationsZip.begin(); beginIt != locationsZip.end();)
      {
        /*
        auto const possibleEndIt = beginIt + std::min(detail::size(beginIt, locationsZip.end()), Base::GetMaxElementNum());
        partitionIt =
          std::partition_point(partitionIt, partitions.end(), [possibleEndIt](auto partitionEndIt) { return partitionEndIt <= possibleEndIt; });

        auto endIt = partitionIt == partitions.begin() ? *partitions.begin() : *std::prev(partitionIt);
        if (endIt == beginIt)
          endIt = *partitionIt;
       */
        auto endIt = *partitionIt++;
        // auto const [minIt, maxIt] =
        //   std::minmax_element(beginIt, endIt, [](EntityLocation const& l, EntityLocation const& r) { return SI::Less(l.first, r.first); });
        auto const [minIt, maxIt] = std::minmax_element(beginIt.GetFirst(), endIt.GetFirst());
        auto const minNodeID = m_spaceIndexing.GetNodeID(*minIt);
        auto const maxNodeID = m_spaceIndexing.GetNodeID(*maxIt);

        auto nodeID = SI::GetLowestCommonAncestor(minNodeID, maxNodeID);
        auto [it, isInserted] = this->m_nodes.try_emplace(nodeID);
        if (isInserted)
        {
          orphanNodes.push_back(nodeID);
          InitNodeGeometry(&*it);
        }

        it->second.ReplaceEntities(std::span(beginIt.GetSecond(), endIt.GetSecond()));
        beginIt = endIt;
      }


      for (std::size_t i = 0; i < orphanNodes.size(); ++i)
      {
        auto const orphanNodeID = orphanNodes[i];
        auto& [parentNodeID, parentNode] = *this->GetParentIt(orphanNodeID);
        auto const childID = SI::GetChildID2(parentNodeID, orphanNodeID);

        if (parentNode.HasChild(childID))
        {
          auto const childNodeID = parentNode.GetChild(childID);
          auto const lcaNodeID = SI::GetLowestCommonAncestor(childNodeID, orphanNodeID);
          parentNode.AddChild(childID, lcaNodeID);

          if (orphanNodeID == lcaNodeID)
          {
            auto& orphanNode = this->m_nodes.at(orphanNodeID);
            auto const childIDOfOrphanNode = SI::GetChildID2(orphanNodeID, childNodeID);
            if (orphanNode.HasChild(childIDOfOrphanNode))
              orphanNodes.push_back(orphanNode.GetChild(childIDOfOrphanNode));

            orphanNode.AddChild(childIDOfOrphanNode, childNodeID);
          }
          else
          {
            auto [lcaIt, _] = this->m_nodes.emplace(lcaNodeID, Node{});
            InitNodeGeometry(&*lcaIt);
            auto& lcaNode = lcaIt->second;
            lcaNode.AddChild(SI::GetChildID2(lcaNodeID, childNodeID), childNodeID);
            lcaNode.AddChild(SI::GetChildID2(lcaNodeID, orphanNodeID), orphanNodeID);
          }
        }
        else
        {
          parentNode.AddChild(childID, orphanNodeID);
        }
      }
    }
    void BulkInsert(EntityContainerView entities, auto EXEC_TAG = SEQ_EXEC) noexcept
    {
      constexpr bool IS_PARALLEL_EXEC = std::is_same_v<std::remove_cvref_t<decltype(EXEC_TAG)>, ExecutionTags::Parallel>;

      auto const entityNo = entities.size();

      auto mortonIDs = std::vector<typename SI::Location>(entityNo);
      auto mainMemorySegment = this->m_memoryResource.Allocate(entityNo);
      auto locationsZip = detail::zip_view(mortonIDs, mainMemorySegment.segment);
      detail::reserve(
        m_nodes, detail::EstimateNodeNumber<GA::DIMENSION_NO, SI::MAX_THEORETICAL_DEPTH_ID>(entityNo, Base::GetMaxDepthID(), Base::GetMaxElementNum()));


      using EntityLocation = decltype(locationsZip)::iterator::value_type;
      EXEC_POL_DEF(ept); // GCC 11.3
      std::transform(EXEC_POL_ADD(ept) entities.begin(), entities.end(), locationsZip.begin(), [&](auto const& entity) -> EntityLocation {
        return { this->GetLocation(EA::GetGeometry(entity)), EA::GetEntityID(entities, entity) };
      });

      auto orphanNodes = std::vector<NodeID>{};
      auto const maxElementNo = static_cast<uint32_t>(Base::GetMaxElementNum());
      Partitioning::DepthFirstPartition<GA::DIMENSION_NO, EA::GEOMETRY_TYPE != GeometryType::Point, typename SI::Location>(
        EXEC_TAG,
        locationsZip.begin(),
        locationsZip.end(),
        SI::GetRootLocation(),
        Base::GetMaxDepthID(),
        maxElementNo,
        [&](auto beginIt, auto endIt, auto location, bool isForcedToFinish) {
          auto const elementNum = detail::size(beginIt, endIt);
          if (elementNum == 0)
            return true;

          if (!isForcedToFinish && elementNum > Base::GetMaxElementNum() && location.depthID < Base::GetMaxDepthID())
            return false;

          auto nodeID = m_spaceIndexing.GetNodeID(location);
          auto [it, isInserted] = this->m_nodes.try_emplace(nodeID);
          if (isInserted)
          {
            orphanNodes.push_back(nodeID);
            InitNodeGeometry(&*it);
          }

          it->second.ReplaceEntities(std::span(beginIt.GetSecond(), endIt.GetSecond()));
          return true;
        });

      for (std::size_t i = 0; i < orphanNodes.size(); ++i)
      {
        auto const orphanNodeID = orphanNodes[i];
        auto& [parentNodeID, parentNode] = *this->GetParentIt(orphanNodeID);
        auto const childID = SI::GetChildID2(parentNodeID, orphanNodeID);

        if (parentNode.HasChild(childID))
        {
          auto const childNodeID = parentNode.GetChild(childID);
          auto const lcaNodeID = SI::GetLowestCommonAncestor(childNodeID, orphanNodeID);
          parentNode.AddChild(childID, lcaNodeID);

          if (orphanNodeID == lcaNodeID)
          {
            auto& orphanNode = this->m_nodes.at(orphanNodeID);
            auto const childIDOfOrphanNode = SI::GetChildID2(orphanNodeID, childNodeID);
            if (orphanNode.HasChild(childIDOfOrphanNode))
              orphanNodes.push_back(orphanNode.GetChild(childIDOfOrphanNode));

            orphanNode.AddChild(childIDOfOrphanNode, childNodeID);
          }
          else
          {
            auto [lcaIt, _] = this->m_nodes.emplace(lcaNodeID, Node{});
            InitNodeGeometry(&*lcaIt);

            auto& lcaNode = lcaIt->second;
            lcaNode.AddChild(SI::GetChildID2(lcaNodeID, childNodeID), childNodeID);
            lcaNode.AddChild(SI::GetChildID2(lcaNodeID, orphanNodeID), orphanNodeID);
          }
        }
        else
        {
          parentNode.AddChild(childID, orphanNodeID);
        }
      }
    }


    void BulkInsertWOPart(EntityContainerView entities, auto EXEC_TAG = SEQ_EXEC) noexcept
    {
      constexpr bool IS_PARALLEL_EXEC = std::is_same_v<std::remove_cvref_t<decltype(EXEC_TAG)>, ExecutionTags::Parallel>;

      auto const entityNo = entities.size();

      auto mortonIDs = std::vector<typename SI::Location>(entityNo);
      auto mainMemorySegment = this->m_memoryResource.Allocate(entityNo);
      detail::reserve(
        m_nodes, EstimateNodeNumber<GA::DIMENSION_NO, SI::MAX_THEORETICAL_DEPTH_ID>(entityNo, Base::GetMaxDepthID(), Base::GetMaxElementNum()));

      auto orphanNodes = std::vector<NodeID>{};
      for (auto const& entity : entities)
      {
        auto nodeID = m_spaceIndexing.GetNodeID(EA::GetGeometry(entity));
        auto [it, isInserted] = m_nodes.try_emplace(nodeID);
        if (isInserted)
        {
          orphanNodes.push_back(nodeID);
          InitNodeGeometry(&*it);
        }

        AddNodeEntity(&*it, EA::GetEntityID(entities, entity));
      }

      for (std::size_t i = 0; i < orphanNodes.size(); ++i)
      {
        auto const orphanNodeID = orphanNodes[i];
        auto& [parentNodeID, parentNode] = *this->GetParentIt(orphanNodeID);
        auto const childID = SI::GetChildID2(parentNodeID, orphanNodeID);

        if (!parentNode.HasChild(childID))
        {
          parentNode.AddChild(childID, orphanNodeID);
          continue;
        }

        auto const childNodeID = parentNode.GetChild(childID);
        auto const lcaNodeID = SI::GetLowestCommonAncestor(childNodeID, orphanNodeID);
        parentNode.AddChild(childID, lcaNodeID);

        if (orphanNodeID == lcaNodeID)
        {
          auto& orphanNode = this->m_nodes.at(orphanNodeID);
          auto const childIDOfOrphanNode = SI::GetChildID2(orphanNodeID, childNodeID);
          if (orphanNode.HasChild(childIDOfOrphanNode))
            orphanNodes.push_back(orphanNode.GetChild(childIDOfOrphanNode));

          orphanNode.AddChild(childIDOfOrphanNode, childNodeID);
        }
        else
        {
          auto [lcaIt, _] = this->m_nodes.emplace(lcaNodeID, Node{});
          InitNodeGeometry(&*lcaIt);

          auto& lcaNode = lcaIt->second;
          lcaNode.AddChild(SI::GetChildID2(lcaNodeID, childNodeID), childNodeID);
          lcaNode.AddChild(SI::GetChildID2(lcaNodeID, orphanNodeID), orphanNodeID);
        }
      }
    }


  private:
    auto AddNode(NodeIDCR nodeID) noexcept
    {
      auto [nodeIt, isNew] = m_nodes.emplace(nodeID, Node{});
      assert(isNew);

      InitNodeGeometry(&*nodeIt);

      auto& [parentNodeID, parentNode] = *GetParentIt(nodeID);
      auto const childID = SI::GetChildID2(parentNodeID, nodeID);

      if (!parentNode.HasChild(childID))
      {
        parentNode.AddChild(childID, nodeID);
        // leaf node does not have MBR geometry yet
        return nodeIt;
      }

      auto const childNodeID = parentNode.GetChild(childID);
      auto const lcaNodeID = SI::GetLowestCommonAncestor(childNodeID, nodeID);
      parentNode.AddChild(childID, lcaNodeID);

      if (nodeID == lcaNodeID)
      {
        nodeIt->second.AddChild(SI::GetChildID2(lcaNodeID, childNodeID), childNodeID);
        InitializeMinimalNodeGeometry(&*nodeIt, true);
      }
      else
      {
        auto [lcaIt, _] = m_nodes.emplace(lcaNodeID, Node{});
        InitNodeGeometry(&*lcaIt);

        auto& lcaNode = lcaIt->second;
        lcaNode.AddChild(SI::GetChildID2(lcaNodeID, childNodeID), childNodeID);
        lcaNode.AddChild(SI::GetChildID2(lcaNodeID, nodeID), nodeID);
        InitializeMinimalNodeGeometry(&*lcaIt, true);
      }

      return nodeIt;
    }

    static auto GetParentIt(auto& nodes, NodeID nodeID) noexcept
    {
      auto endIt = nodes.end();
      for (nodeID = SI::GetParentKey(nodeID); SI::IsValidKey(nodeID); nodeID = SI::GetParentKey(nodeID))
      {
        auto nodeIt = nodes.find(nodeID);
        if (nodeIt != endIt)
          return nodeIt;
      }

      return endIt;
    }

    static auto GetNodeIt(auto& nodes, auto const& searchNodeID) noexcept
    {
      auto const endIt = nodes.end();
      if (!SI::IsValidKey(searchNodeID))
        return endIt;

      auto const nodeIt = nodes.find(searchNodeID);
      if (nodeIt == endIt)
        return GetParentIt(nodes, searchNodeID);

      return nodeIt;
    }

    auto GetParentIt(NodeID nodeID) const noexcept { return GetParentIt(m_nodes, nodeID); }
    auto GetParentIt(NodeID nodeID) noexcept { return GetParentIt(m_nodes, nodeID); }
    auto GetNodeIt(auto const& searchGeometry) const noexcept { return GetNodeIt(m_nodes, m_spaceIndexing.GetNodeID(searchGeometry)); }
    auto GetNodeIt(auto const& searchGeometry) { return GetNodeIt(m_nodes, m_spaceIndexing.GetNodeID(searchGeometry)); }


    // TODO: delete?
    constexpr NodeID GetNodeIDByEntity(EntityID entityID) const noexcept
    {
      if constexpr (CONFIG::USE_REVERSE_MAPPING)
      {
        auto const it = this->m_reverseMap.find(entityID);
        return it != this->m_reverseMap.end();
      }
      else
      {
        auto const it = std::find_if(m_nodes.begin(), m_nodes.end(), [&](auto const& keyAndValue) {
          auto const& entities = GetNodeEntities(&keyAndValue);
          return std::ranges::find(entities, entityID) != entities.end();
        });
        return it == m_nodes.end() ? NodeID{} : it->first;
      }
    }

    constexpr void UpdateReverseMap(EntityID entityID, NodeIDCR nodeID) noexcept
    {
      if constexpr (CONFIG::USE_REVERSE_MAPPING)
        m_reverseMap[entityID] = nodeID;
    }

    IGM::Box InitializeSubtreeMinimalNodeGeometry(NodeValue* nodeValue, EntityContainerView entities)
    {
      if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MBR)
      {
        auto nodeBox = IGM::BoxInvertedInit();

        // Union with all children
        for (auto const& childNodeID : GetNodeChildren(nodeValue))
        {
          auto childNodeValue = GetNodeValueP(childNodeID);
          auto childNodeBox = InitializeSubtreeMinimalNodeGeometry(childNodeValue, entities);
          IGM::UniteInBoxAD(nodeBox, childNodeBox);
        }

        // Union with all entities
        for (auto const& entityID : GetNodeEntities(nodeValue))
          IGM::UniteInBoxAD(nodeBox, EA::GetGeometry(entities, entityID));

        nodeValue->second.GetGeometry() = { nodeBox.Min, IGM::Sub(nodeBox.Max, nodeBox.Min) };
        return nodeBox;
      }
      else
      {
        return {};
      }
    }

    // Cascade update of node geometry by a new geometry
    template<bool IS_ROOT_UPDATE = false>
    void UpdateMinimalNodeGeometry(NodeValue* nodeValue, IGM::Box const& box)
    {
      if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MBR)
      {
        auto& nodeGeometry = nodeValue->second.GetGeometry();
        bool const isInitialized = IsNodeGeometryInitialized(nodeGeometry);
        bool isChanged = false;
        auto& [minPoint, size] = nodeGeometry;
        if (!isInitialized)
        {
          isChanged = true;
          for (dim_t d = 0; d < GA::DIMENSION_NO; ++d)
          {
            minPoint[d] = box.Min[d];
            size[d] = box.Max[d] - minPoint[d];
          }
        }
        else
        {
          for (dim_t d = 0; d < GA::DIMENSION_NO; ++d)
          {
            auto maxPoint = minPoint[d] + size[d];

            if (box.Min[d] < minPoint[d])
            {
              minPoint[d] = box.Min[d];
              isChanged = true;
            }

            if (box.Max[d] > maxPoint)
            {
              maxPoint = box.Max[d];
              isChanged = true;
            }

            size[d] = maxPoint - minPoint[d];
          }
        }

        if (isChanged)
        {
          auto parentNodeIt = GetParentIt(nodeValue->first);
          if (parentNodeIt != m_nodes.end())
            UpdateMinimalNodeGeometry(&*parentNodeIt, box);
        }
      }
      else if constexpr (IS_ROOT_UPDATE && CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MinPoint)
      {
        assert(nodeValue == GetRootNodeValue());
        auto nodeBox = GetNodeBox(nodeValue);
        IGM::UniteInBoxAD(nodeBox, box);

        Base::SetTreeSize(IGM::Sub(nodeBox.Max, nodeBox.Min));
        if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MinPoint)
          nodeValue->second.GetGeometry() = nodeBox.Min;
        else
          Base::SetRealTreeMinPoint(nodeBox.Min);
      }
    }

    // Cascade initialization of node geometry
    void InitializeMinimalNodeGeometry(NodeValue* nodeValue, bool isCascadeMode = false, std::optional<typename IGM::Box> entityBox = std::nullopt)
    {
      if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MBR)
      {
        auto nodeBox = IGM::BoxInvertedInit();
        if (entityBox)
          IGM::UniteInBoxAD(nodeBox, *entityBox);

        // Union with all children
        auto const& children = GetNodeChildren(nodeValue);
        for (auto const& childNodeID : children)
        {
          auto const childNodeValue = GetNodeValueP(childNodeID);
          if (IsNodeEntitiesEmpty(childNodeValue))
            continue;

          const auto childBox = GetNodeBox(childNodeValue);
          IGM::UniteInBoxAD(nodeBox, childBox);
        }

        // Initialize geometry
        nodeValue->second.GetGeometry() = { nodeBox.Min, IGM::Sub(nodeBox.Max, nodeBox.Min) };

        if (isCascadeMode)
        {
          auto parentNodeIt = GetParentIt(nodeValue->first);
          if (parentNodeIt != m_nodes.end())
          {
            UpdateMinimalNodeGeometry(&*parentNodeIt, nodeBox);
          }
        }
      }
    }

    // Cascade initialization of node geometry
    void InitializeMinimalNodeGeometry(NodeValue* nodeValue, EntityContainerView entities, bool isCascadeMode = false)
    {
      if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MBR)
      {
        auto nodeBox = IGM::BoxInvertedInit();

        // Union with all entities
        auto const& entityIDs = GetNodeEntities(nodeValue);
        for (auto const& entityID : entityIDs)
          IGM::UniteInBoxAD(nodeBox, EA::GetGeometry(entities, entityID));

        InitializeMinimalNodeGeometry(nodeValue, isCascadeMode, nodeBox);
      }
    }

    void InitializeRootNodeBox(EntityContainerView entities)
    {
      auto rootNodeValue = GetRootNodeValue();
      if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MBR)
      {
        InitializeSubtreeMinimalNodeGeometry(rootNodeValue, entities);
      }
      else if constexpr (CONFIG::ALLOW_OUT_OF_SPACE_INSERTION)
      {
        auto nodeBox = GetNodeBox(GetRootNodeValue());
        if (!IsNodeGeometryInitialized(GetRootNodeValue()->second.GetGeometry()))
          nodeBox = IGM::BoxInvertedInit();

        auto const& entityIDs = GetNodeEntities(rootNodeValue);
        for (auto const& entityID : entityIDs)
          IGM::UniteInBoxAD(nodeBox, EA::GetGeometry(entities, entityID));

        if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::None)
        {
          Base::SetRealTreeMinPoint(nodeBox.Min);
          Base::SetTreeSize(IGM::Sub(nodeBox.Max, nodeBox.Min));
        }
        else if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MinPoint)
        {
          rootNodeValue->second.GetGeometry() = nodeBox.Min;
          Base::SetTreeSize(IGM::Sub(nodeBox.Max, nodeBox.Min));
        }
      }
    }

  public:
    // TODO: delete?
    NodeID Find(EntityID entityID) const noexcept { return GetNodeIDByEntity(entityID); }

    bool IsEveryEntityUnique() const noexcept
    {
      auto ids = std::vector<EntityID>();
      ids.reserve(100);
      std::for_each(m_nodes.begin(), m_nodes.end(), [&](auto& nodeValue) {
        auto const& entities = GetNodeEntities(&nodeValue);
        ids.insert(ids.end(), entities.begin(), entities.end());
      });

      auto const idsSizeBeforeUnique = ids.size();
      detail::sortAndUnique(ids);
      return idsSizeBeforeUnique == ids.size();
    }

  private:
    bool IsGeometryInTree(EA::Geometry const& entityGeometry, TFloatScalar tolerance) const noexcept
    {
      const auto treeBox = GetBox();
      if constexpr (EA::GEOMETRY_TYPE == GeometryType::Point)
      {
        return GA::DoesBoxContainPoint(treeBox, entityGeometry, tolerance);
      }
      else if constexpr (EA::GEOMETRY_TYPE == GeometryType::Box)
      {
        return GA::AreBoxesOverlapped(treeBox, entityGeometry, true, false, tolerance);
      }
      else
      {
        static_assert(false, "Unsupported geometry type!");
      }
    }

    enum class OutOfSpaceInsertionResult
    {
      NotHandled,
      InsertedIntoRoot,
      Denied
    };
    OutOfSpaceInsertionResult HandleOutOfSpaceInsertion(EntityID entityID, EA::Geometry const& entityGeometry)
    {
      if (!IsGeometryInTree(entityGeometry, GA::BASE_TOLERANCE))
      {
        if constexpr (CONFIG::ALLOW_OUT_OF_SPACE_INSERTION)
        {
          auto nodeValue = &*m_nodes.find(SI::GetRootKey());
          AddNodeEntity(nodeValue, entityID);
          UpdateMinimalNodeGeometry<true>(nodeValue, IGM::GetBoxAD(entityGeometry));
          return OutOfSpaceInsertionResult::InsertedIntoRoot;
        }
        else
        {
          return OutOfSpaceInsertionResult::Denied;
        }
      }

      return OutOfSpaceInsertionResult::NotHandled;
    }

  public:
    // Insert entity into the tree. If allowLeafCreation is true: The smallest node will be chosen by the max depth. If allowLeafCreation is false: The smallest existing level on the branch will be chosen.
    bool InsertIntoLeaf(EntityID entityID, EA::Geometry const& entityGeometry, bool allowLeafCreation = false) noexcept
    {
      auto const oosResult = HandleOutOfSpaceInsertion(entityID, entityGeometry);
      if (oosResult != OutOfSpaceInsertionResult::NotHandled)
        return oosResult != OutOfSpaceInsertionResult::Denied;

      auto const entityNodeID = m_spaceIndexing.GetNodeID(entityGeometry);
      auto nodeIt = m_nodes.find(entityNodeID);
      bool isNodeAdded = false;
      if (nodeIt == m_nodes.end())
      {
        if (allowLeafCreation)
        {
          nodeIt = AddNode(entityNodeID);
          isNodeAdded = true;
        }
        else
        {
          nodeIt = GetParentIt(entityNodeID);
          if (nodeIt->second.IsAnyChildExist())
          {
            auto const childID = SI::GetChildID2(nodeIt->first, entityNodeID);
            auto const childGenerator = typename SI::ChildKeyGenerator(nodeIt->first);
            auto const childNodeID = childGenerator.GetChildNodeKey(childID);

            nodeIt = AddNode(childNodeID);
            isNodeAdded = true;
          }
        }
      }

      AddNodeEntity(&*nodeIt, entityID);
      UpdateMinimalNodeGeometry(&*nodeIt, IGM::GetBoxAD(entityGeometry));

      auto constexpr DO_UNIQUENESS_CHECK_TO_INDICIES = true;
      if constexpr (DO_UNIQUENESS_CHECK_TO_INDICIES)
        assert(this->IsEveryEntityUnique()); // Assert means: index is already added. Wrong input!

      return true;
    }

    // Insert entity into the tree with node rebalancing
    bool Insert(EntityID entityID, EA::Geometry const& entityGeometry, EntityContainerView entities) noexcept
    {
      auto const oosResult = HandleOutOfSpaceInsertion(entityID, entityGeometry);
      if (oosResult != OutOfSpaceInsertionResult::NotHandled)
        return oosResult != OutOfSpaceInsertionResult::Denied;


      // Rules:
      // 1. If the node has a child, it is already split
      // 2. Split nodes contain stucked boxes only, In other words: new elements that would be inserted in the node (but not stucked), if the node
      // has any child node, the new element should be inserted into a child.
      // 3. Rebalance push elements downward only once.

      auto const entityNodeID = m_spaceIndexing.GetNodeID(entityGeometry);
      auto nodeIt = m_nodes.find(entityNodeID);
      auto const isNonRefinable = nodeIt != m_nodes.end();
      if (nodeIt == m_nodes.end())
        nodeIt = GetParentIt(entityNodeID);

      auto const nodeValue = &*nodeIt;
      auto const entityCount = GetNodeEntityCount(nodeValue) + 1;
      auto const isOverflowed = entityCount > Base::GetMaxElementNum();
      auto const isPreviouslyOverflowed = nodeValue->second.IsAnyChildExist();
      auto const depthID = SI::GetDepthID(nodeValue->first);

      if (isNonRefinable)
        UpdateMinimalNodeGeometry(nodeValue, IGM::GetBoxAD(entityGeometry));

      // No overflow or non-refinable or max depth reached
      if ((!isPreviouslyOverflowed && !isOverflowed) || (isPreviouslyOverflowed && isNonRefinable) || depthID == Base::GetMaxDepthID())
      {
        AddNodeEntity(nodeValue, entityID);
        return true;
      }

      // Previously overflowed but now non-overflowed. Just a direct child node creation is required.
      if (isPreviouslyOverflowed)
      {
        assert(!isNonRefinable);

        nodeIt = AddNode(SI::GetDirectChildNodeID(nodeValue->first, entityNodeID));
        AddNodeEntity(&*nodeIt, entityID);
        UpdateMinimalNodeGeometry(&*nodeIt, IGM::GetBoxAD(entityGeometry));

        return true;
      }

      // Handling overflow

      assert(isOverflowed);

      auto const& originalEntityIDs = GetNodeEntities(nodeValue);

      auto mortonIDs = std::vector<typename SI::Location>(entityCount);
      auto entityIDs = std::vector<EntityID>{};
      entityIDs.reserve(entityCount);
      entityIDs.assign(originalEntityIDs.begin(), originalEntityIDs.end());
      entityIDs.emplace_back(entityID);

      auto locationsZip = detail::zip_view(mortonIDs, entityIDs);
      using ZipValue = decltype(locationsZip)::iterator::value_type;
      std::transform(entityIDs.begin(), entityIDs.end() - 1, locationsZip.begin(), [&](auto const& storedEntityID) -> ZipValue {
        return { m_spaceIndexing.GetLocation(EA::GetGeometry(entities, storedEntityID)), storedEntityID };
      });

      // new entity is may not added to the geometry container
      auto lastIt = locationsZip.end() - 1;
      *lastIt = { m_spaceIndexing.GetLocation(entityGeometry), entityID };

      auto beginIt = locationsZip.begin();

      // Non-refinable box entities
      if constexpr (EA::GEOMETRY_TYPE == GeometryType::Point)
      {
        ClearNodeEntities(nodeValue->second);
      }
      else if constexpr (EA::GEOMETRY_TYPE == GeometryType::Box)
      {
        beginIt = std::partition(beginIt, locationsZip.end(), [&](auto const& element) { return element.GetFirst().GetDepthID() == depthID; });

        auto const isAllNonRefinable = beginIt == locationsZip.end();

        if (isAllNonRefinable && isNonRefinable)
        {
          AddNodeEntity(nodeValue, entityID);
          return true;
        }

        ClearNodeEntities(nodeValue->second);
        for (auto it = locationsZip.begin().GetSecond(); it != beginIt.GetSecond(); ++it)
          AddNodeEntity(nodeValue, *it);

        // Update parent node geometry once to avoid multiple updates
        UpdateMinimalNodeGeometry(nodeValue, IGM::GetBoxAD(entityGeometry));
      }
      else
      {
        static_assert(false);
      }

      // Child nodes
      auto const childNodeLevelID = Base::GetMaxDepthID() - depthID - 1;
      while (beginIt != locationsZip.end())
      {
        auto const nodeChecker = typename SI::ChildCheckerFixedDepth(childNodeLevelID, beginIt.GetFirst()->GetLocationID());
        auto const nextIt =
          std::partition(beginIt, locationsZip.end(), [&](auto const& element) { return nodeChecker.Test(element.GetFirst().GetLocationID()); });

        // Create child node deeper if possible and worth it
        NodeValue* childNodeValue = nullptr;
        if (detail::size(beginIt, nextIt) >= Base::GetMaxElementNum() / 2)
        {
          auto lcaHelper = SI::GetLowestCommonAncestorHelper(*beginIt.GetFirst());
          for (auto it = beginIt.GetFirst(); it != nextIt.GetFirst(); ++it)
            lcaHelper.Add(*it);

          auto childNodeIt = AddNode(lcaHelper.GetNodeID(Base::GetMaxDepthID()));
          childNodeValue = &*childNodeIt;
        }
        else
        {
          auto const partitionChildNodeID = m_spaceIndexing.GetNodeID(*beginIt.GetFirst());
          auto childNodeIt = AddNode(m_spaceIndexing.GetDirectChildNodeID(nodeValue->first, partitionChildNodeID));
          childNodeValue = &*childNodeIt;
        }

        bool isNewEntityContained = false;

        // Insert entities to the child node
        for (auto it = beginIt.GetSecond(); it != nextIt.GetSecond(); ++it)
        {
          // New entity will be added separately, to avoid non-containment issues in InitializeMinimalNodeGeometry
          if (entityID == *it)
          {
            isNewEntityContained = true;
            continue;
          }

          AddNodeEntity(childNodeValue, *it);
        }

        // Update child node geometry. Parent node geometry is already updated, cascading is not required.
        InitializeMinimalNodeGeometry(childNodeValue, entities, false);
        if (isNewEntityContained)
        {
          AddNodeEntity(childNodeValue, entityID);
          UpdateMinimalNodeGeometry(childNodeValue, IGM::GetBoxAD(entityGeometry));
        }

        beginIt = nextIt;
      }

      auto constexpr DO_UNIQUENESS_CHECK_TO_INDICIES = true;
      if constexpr (DO_UNIQUENESS_CHECK_TO_INDICIES)
        assert(IsEveryEntityUnique()); // Assert means: index is already added. Wrong input!

      return true;
    }


    // Update id by the new bounding box information
    bool Update(EntityID entityID, EA::Geometry const& newEntityGeometry, bool allowLeafCreation = false) noexcept
    {
      if constexpr (!CONFIG::ALLOW_OUT_OF_SPACE_INSERTION)
      {
        if (!IsGeometryInTree(newEntityGeometry, GA::BASE_TOLERANCE))
          return false;
      }

      if (!EraseBase<false>(entityID))
        return false;

      return InsertIntoLeaf(entityID, newEntityGeometry, allowLeafCreation);
    }

    // Update id by the new bounding box information and the erase part is aided by the old bounding box geometry data
    bool Update(EntityID entityID, EA::Geometry const& oldEntityGeometry, EA::Geometry const& newEntityGeometry, bool allowLeafCreation = false) noexcept
    {
      if constexpr (CONFIG::USE_REVERSE_MAPPING)
      {
        return Update(entityID, newEntityGeometry, allowLeafCreation);
      }
      else
      {
        auto const oldNodeIt = GetNodeIt(oldEntityGeometry);
        if (oldNodeIt == m_nodes.end())
          return false;

        auto const newNodeIt = GetNodeIt(newEntityGeometry);
        if (oldNodeIt == newNodeIt)
          return true; // No update is required.

        if (!EraseBase<false>(entityID, oldEntityGeometry))
          return false; // entityID was not registered previously.

        if (newNodeIt == m_nodes.end())
          return false;

        return InsertIntoLeaf(entityID, newEntityGeometry, allowLeafCreation);
      }
    }

    // Update id with rebalancing by the new bounding box information
    bool Update(EntityID entityID, EA::Geometry const& newEntityGeometry, EntityContainerView entities) noexcept
    {
      if (!EraseBase<false>(entityID))
        return false;

      return Insert(entityID, newEntityGeometry, entities);
    }

    // Update id with rebalancing by the new bounding box information and the erase part is aided by the old bounding box geometry data
    bool Update(EntityID entityID, EA::Geometry const& oldEntityGeometry, EA::Geometry const& newEntityGeometry, EntityContainerView const& entities) noexcept
    {
      if constexpr (CONFIG::USE_REVERSE_MAPPING)
      {
        return Update(entityID, newEntityGeometry, entities);
      }
      else
      {
        auto const oldNodeIt = GetNodeIt(oldEntityGeometry);
        if (oldNodeIt == m_nodes.end())
          return false;

        auto const newNodeIt = GetNodeIt(newEntityGeometry);
        if (oldNodeIt == newNodeIt)
          return true; // No update is required.

        if (!EraseBase<false>(entityID, oldEntityGeometry))
          return false; // entityID was not registered previously.

        if (newNodeIt == m_nodes.end())
          return false;

        return Insert(entityID, newEntityGeometry, entities);
      }
    }

    // Update all element which are in the given hash-table.
    template<bool IS_PARALLEL_EXEC = false, bool DO_UNIQUENESS_CHECK_TO_INDICIES = false>
    void UpdateIndexes(std::unordered_map<EntityID, std::optional<EntityID>> updateMap) noexcept
    {
      auto const updateMapEndIterator = updateMap.end();

      auto const UpdateNodes = [&](auto& node) {
        auto& entityIDs = node.second.GetEntities();
        auto entityNo = entityIDs.size();
        for (std::size_t i = 0; i < entityNo; ++i)
        {
          auto it = updateMap.find(entityIDs[i]);
          if (it == updateMapEndIterator)
            continue;

          if (it->second)
            entityIDs[i] = *it->second;
          else
          {
            --entityNo;
            entityIDs[i] = entityIDs[entityNo];
            --i;
          }

          if constexpr (!IS_PARALLEL_EXEC)
            updateMap.erase(it);
        }

        ResizeNodeEntities(node.second, entityNo);
      };

      if constexpr (CONFIG::USE_REVERSE_MAPPING)
      {
        auto nodes = std::unordered_set<NodeID>{};
        for (auto const [oldEntityID, newEntityID] : updateMap)
        {
          nodes.emplace(m_reverseMap.at(oldEntityID));

          if (!newEntityID)
            m_reverseMap.erase(oldEntityID);
        }

        EXEC_POL_DEF(ep);
        std::for_each(EXEC_POL_ADD(ep) nodes.begin(), nodes.end(), UpdateNodes);
      }
      else
      {
        EXEC_POL_DEF(ep);
        std::for_each(EXEC_POL_ADD(ep) m_nodes.begin(), m_nodes.end(), UpdateNodes);
      }

      if constexpr (DO_UNIQUENESS_CHECK_TO_INDICIES)
        assert(IsEveryEntityUnique()); // Assert means: index replacements causes that multiple object has the same id. Wrong input!
    }

    // Reset the tree
    void Reset() noexcept
    {
      m_nodes.clear();
      m_memoryResource.Reset();
      m_spaceIndexing = {};
    }


    // Remove all elements and ids, except Root
    void Clear() noexcept
    {
      std::erase_if(m_nodes, [](auto const& p) { return p.first != SI::GetRootKey(); });
      detail::at(m_nodes, SI::GetRootKey()).Clear();
    }


    // Move the whole tree with a std::vector of the movement
    template<bool IS_PARALLEL_EXEC = false>
    void Move(TVector const& moveVector) noexcept
    {
      if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::None)
      {
        return;
      }
      else if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MinPoint || CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MBR)
      {
        EXEC_POL_DEF(ep); // GCC 11.3
        std::for_each(EXEC_POL_ADD(ep) m_nodes.begin(), m_nodes.end(), [&moveVector](auto& pairKeyNode) {
          if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MinPoint)
          {
            auto& minPoint = pairKeyNode.second.GetGeometry();
            IGM::MoveAD(minPoint, moveVector);
          }
          else if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MBR)
          {
            auto& box = pairKeyNode.second.GetGeometry();
            IGM::MoveAD(box.minPoint, moveVector);
          }
        });
      }
      else
      {
        static_assert(false, "Unsupported NODE_GEOMETRY_STORAGE value!");
      }

      m_spaceIndexing.Move(moveVector);
    }

  private:
    void RemoveNodeIfPossible(auto nodeIt) noexcept
    {
      while (true)
      {
        auto& [nodeID, node] = *nodeIt;
        if (nodeID == SI::GetRootKey())
          return;

        if (node.IsAnyChildExist() || !IsNodeEntitiesEmpty(&*nodeIt))
          return;

        m_memoryResource.Deallocate(node.GetEntitySegment());
        auto parentNodeIt = GetParentIt(nodeID);

        auto& [parentNodeID, parentNode] = *parentNodeIt;
        parentNode.RemoveChild(SI::GetChildID2(parentNodeID, nodeID));
        m_nodes.erase(nodeIt);

        nodeIt = parentNodeIt;
      }
    }

    void DecreaseEntityIDs(EntityID entityID) noexcept
    {
      for (auto& [_, node] : m_nodes)
        node.DecreaseEntityIDs(entityID);

      if constexpr (CONFIG::USE_REVERSE_MAPPING)
      {
        auto reverseMap = std::move(m_reverseMap);
        for (auto it = reverseMap.begin(); it != reverseMap.end();)
        {
          auto node = reverseMap.extract(it++);
          node.key() -= (entityID <= node.key());
          m_reverseMap.insert(std::move(node));
        }
      }
    }

    template<bool DECREASE_ENTITY_IDS = EA::REQUIRES_CONTIGUOUS_ENTITY_IDS>
    constexpr bool EraseBase(EntityID entityID) noexcept
    {
      bool isErased = false;

      if constexpr (CONFIG::USE_REVERSE_MAPPING)
      {
        auto it = m_reverseMap.find(entityID);
        if (it == m_reverseMap.end())
          return false;

        NodeID n = it->second;
        auto nodeIt = m_nodes.find(n);
        if (!RemoveNodeEntity(nodeIt->second, entityID))
          return false;

        m_reverseMap.erase(it);
        RemoveNodeIfPossible(nodeIt);
        isErased = true;
      }
      else
      {
        for (auto nodeIt = m_nodes.begin(); nodeIt != m_nodes.end(); ++nodeIt)
        {
          if (!RemoveNodeEntity(nodeIt->second, entityID))
            continue;

          isErased = true;
          RemoveNodeIfPossible(nodeIt);
          break;
        }
      }

      if (!isErased)
        return false;

      if constexpr (DECREASE_ENTITY_IDS)
      {
        DecreaseEntityIDs(entityID);
      }

      return true;
    }

    template<bool DECREASE_ENTITY_IDS = EA::REQUIRES_CONTIGUOUS_ENTITY_IDS>
    constexpr bool EraseBase(EntityID entitiyID, EA::Geometry const& entityGeometry) noexcept
    {
      auto nodeIt = GetNodeIt(entityGeometry);
      if (nodeIt == m_nodes.end())
        return false; // entity's geometry is not in the handled space

      bool const isEntityRemoved = RemoveNodeEntity(nodeIt->second, entitiyID);
      if (!isEntityRemoved)
        return false;

      RemoveNodeIfPossible(nodeIt);

      if constexpr (DECREASE_ENTITY_IDS)
      {
        DecreaseEntityIDs(entitiyID);
      }

      return true;
    }

  public: // Entity handling
    // Erase entity via reverse mapping or brute force search. Reverse mapping is recommended.
    constexpr bool Erase(EntityID entityID) noexcept { return EraseBase<EA::REQUIRES_CONTIGUOUS_ENTITY_IDS>(entityID); }

    // Erase id, aided with the original geometry. Reverse mapping is not used in this function, consider its usage, with the alternative Erase().
    constexpr bool Erase(EntityID entityID, EA::Geometry const& entityGeometry) noexcept
    {
      return EraseBase<EA::REQUIRES_CONTIGUOUS_ENTITY_IDS>(entityID, entityGeometry);
    }

  public: // Search functions
    bool Contains(EA::Geometry const& geometry, EntityContainerView entities, TFloatScalar tolerance = GA::BASE_TOLERANCE) const noexcept
    {
      auto const nodeIt = GetNodeIt(geometry);
      if (nodeIt == m_nodes.end())
        return false;

      auto const& nodeEntityIDs = GetNodeEntities(&*nodeIt);
      return std::any_of(nodeEntityIDs.begin(), nodeEntityIDs.end(), [&](auto const& entityID) {
        if constexpr (EA::GEOMETRY_TYPE == GeometryType::Point)
        {
          return GA::ArePointsEqual(geometry, EA::GetGeometry(entities, entityID), tolerance);
        }
        else if constexpr (EA::GEOMETRY_TYPE == GeometryType::Point)
        {
          return GA::AreBoxesEqual(geometry, EA::GetGeometry(entities, entityID), tolerance);
        }
        else
        {
          static_assert(false, "Unsupported geometry type!");
        }
        ORTHOTREE_UNREACHABLE();
      });
    }

    bool Contains(EntityID const& entityID, EntityContainerView entities, TFloatScalar tolerance = GA::BASE_TOLERANCE) const noexcept
    {
      if constexpr (CONFIG::USE_REVERSE_MAPPING)
      {
        return m_reverseMap.find(entityID) != m_reverseMap.end();
      }
      else
      {
        if (entities.size() == 0)
          return false;

        auto const& entityGeometry = EA::GetGeometry(entities, entityID);
        return Contains(entityGeometry, entities, tolerance);
      }
    }
  };


  // TraverseControl is the result type of node-visitor functions' procedure.
  enum class TraverseControl
  {
    Terminate,    // Terminates the traverse
    SkipChildren, // Skips children nodes
    Continue      // Continues the traverse
  };

  // OrthoTree: Non-owning Base container which spatially organize data ids in N dimension space into a hash-table by Morton Z order.
  template<typename T>
  concept OrthoTreeCoreView = requires(
    const T ct,
    typename T::NodeIDCR nodeID,
    typename T::NodeValueCP nodeValue,
    typename T::EA::Geometry const& geometry,
    typename T::EA::EntityContainerView entities,
    typename T::EntityID entityID) {
    typename T::NodeID;
    typename T::NodeIDCR;
    typename T::NodeValueCP;
    typename T::EA;
    typename T::GA;
    typename T::CONFIG;
    typename T::IGM;

    { T::GetRootNodeID() } -> std::convertible_to<typename T::NodeID>;
    { ct.GetRootNodeValue() } -> std::convertible_to<typename T::NodeValueCP>;
    { ct.GetNodeValueP(nodeID) } -> std::convertible_to<typename T::NodeValueCP>;

    { ct.GetNodeChildren(nodeValue) };
    { ct.GetNodeEntities(nodeValue) };

    { ct.GetNodeBox(nodeValue) };
    { ct.GetNodeMinPoint(nodeValue) };
    { ct.GetNodeSize(nodeValue) };

    { ct.IsNodeEntitiesEmpty(nodeValue) } -> std::convertible_to<bool>;

    { ct.GetNodeCount() } -> std::convertible_to<std::size_t>;
  };

  template<typename TOrthoTreeCore>
  class OrthoTreeQueryBase : public TOrthoTreeCore
  {
  public:
    using Core = TOrthoTreeCore;
    ORTHOTREE_DEPENDENT_TYPES(Core);


  public:
    static_assert(OrthoTreeCoreView<TOrthoTreeCore>);
    static_assert(GeometryAdapterConcept<GA, GA::DIMENSION_NO, TVector, TBox, TRay, TPlane, TScalar, TFloatScalar>);
    static_assert(0 < GA::DIMENSION_NO && GA::DIMENSION_NO < 64);

    static_assert(std::is_trivially_copyable_v<EntityID>, "Only trivially copyable EntityID types are supported!");
    static_assert(CONFIG::IS_HOMOGENEOUS_GEOMETRY, "Mixed geometry types are not supported yet!");
    static_assert(EA::GEOMETRY_TYPE == GeometryType::Point || EA::GEOMETRY_TYPE == GeometryType::Box, "Entity geometry type is not supported!");
    static_assert(CONFIG::MAX_ALLOWED_DEPTH_ID <= MAX_DEPTH_ID, "MAX_ALLOWED_DEPTH_ID of Configuration is too large.");
    static_assert(CONFIG::LOOSE_FACTOR >= 1.0 && CONFIG::LOOSE_FACTOR <= 2.0, "Wrong loose factor for Loose trees.");

    using TOrthoTreeCore::TOrthoTreeCore;

    using NodeID = typename Core::NodeID;
    using NodeIDCR = typename Core::NodeIDCR;
    using NodeValueCP = Core::NodeValueCP;

  protected: // Aid struct to partitioning and distance ordering
    template<typename T>
    using DimArray = std::array<T, GA::DIMENSION_NO>;

  public:
    // Insert entity into the tree, if there is no entity within the same location by tolerance.
    bool InsertUnique(EA::Geometry const& entityGeometry, EntityContainerView entities, TFloatScalar tolerance = GA::BASE_TOLERANCE, bool doInsertToLeaf = false)
      requires(EA::REQUIRES_CONTIGUOUS_ENTITY_IDS)
    {
      auto const nearestEntityList = GetNearestNeighbors(entityGeometry, 1, 0.0, entities, tolerance);
      if (!nearestEntityList.empty())
        return false;

      auto const entityID = EntityID(entities.size());
      return doInsertToLeaf ? Core::InsertIntoLeaf(entityID, entityGeometry, true) : Core::Insert(entityID, entityGeometry, entities);
    }

    // Insert entity into the tree, if there is no entity within the same location by tolerance.
    bool InsertUnique(
      EntityID entityID,
      EA::Geometry const& entityGeometry,
      EntityContainerView entities,
      TFloatScalar tolerance = GA::BASE_TOLERANCE,
      bool doInsertToLeaf = false)
      requires(!EA::REQUIRES_CONTIGUOUS_ENTITY_IDS)
    {
      auto const nearestEntityList = GetNearestNeighbors(entityGeometry, 1, 0.0, entities, tolerance);
      if (!nearestEntityList.empty())
        return false;

      return doInsertToLeaf ? Core::InsertIntoLeaf(entityID, entityGeometry, true) : Core::Insert(entityID, entityGeometry, entities);
    }


  public: // Getters
    constexpr TBox GetNodeBox(NodeValueCP nodeValue) const noexcept
    {
      auto const box = Core::GetNodeBox(nodeValue);
      TBox outBox = {};
      detail::static_for<GA::DIMENSION_NO>([&](auto dimensionID) {
        GA::SetBoxMinC(outBox, dimensionID, box.Min[dimensionID]);
        GA::SetBoxMaxC(outBox, dimensionID, box.Max[dimensionID]);
      });

      return outBox;
    }

    // Visit nodes in breadth first order
    constexpr void TraverseNodesBreadthFirst(auto&& procedure, NodeValueCP pRootNode) const noexcept
    {
      auto queue = std::queue<NodeValueCP>();
      queue.push(pRootNode);
      while (!queue.empty())
      {
        auto const pNodeValue = queue.front();
        queue.pop();

        auto const traverseControl = procedure(pNodeValue);
        switch (traverseControl)
        {
        case TraverseControl::Terminate: return;
        case TraverseControl::SkipChildren: continue;
        case TraverseControl::Continue:
          for (NodeIDCR childNodeID : Core::GetNodeChildren(pNodeValue))
            queue.push(Core::GetNodeValueP(childNodeID));

          break;
        }
      }
    }

    // Visit nodes in breadth first order
    constexpr void TraverseNodesBreadthFirst(auto&& procedure) const noexcept
    {
      TraverseNodesBreadthFirst(std::forward<decltype(procedure)>(procedure), Core::GetRootNodeValue());
    }

    // Visit nodes in depth first order
    constexpr void TraverseNodesDepthFirst(auto&& procedure, NodeValueCP pRootValue) const noexcept
    {
      auto nodeStack = std::vector<NodeValueCP>{};
      nodeStack.reserve(64);
      nodeStack.push_back(pRootValue);

      while (!nodeStack.empty())
      {
        auto const currentNodeValue = nodeStack.back();
        nodeStack.pop_back();

        auto const control = procedure(currentNodeValue);
        switch (control)
        {
        case TraverseControl::Terminate: return;
        case TraverseControl::SkipChildren: continue;
        case TraverseControl::Continue: {
          for (auto const& childNodeID : Core::GetNodeChildren(currentNodeValue))
          {
            nodeStack.push_back(Core::GetNodeValueP(childNodeID));
          }
          break;
        }
        }
      }
    }

    // Visit nodes in depth first order
    constexpr void TraverseNodesDepthFirst(auto&& procedure) const noexcept
    {
      TraverseNodesDepthFirst(std::forward<decltype(procedure)>(procedure), Core::GetRootNodeValue());
    }

    // Visit nodes in priority order
    constexpr void TraverseNodesByPriority(auto&& procedure, auto&& priorityCalculator, NodeValueCP nodeValue) const noexcept
    {
      using TPriorityResult = std::invoke_result_t<decltype(priorityCalculator), NodeValueCP>;
      using TPriority = std::conditional_t<detail::IsStdOptionalV<TPriorityResult>, typename TPriorityResult::value_type, TPriorityResult>;

      auto constexpr GetValue = [](TPriorityResult const& pr) noexcept -> TPriority {
        if constexpr (detail::IsStdOptionalV<TPriorityResult>)
          return *pr;
        else
          return pr;
      };

      struct PrioritizedNode
      {
        NodeValueCP pNodeValue;
        TPriority priority;

        constexpr auto operator<=>(PrioritizedNode const& other) const noexcept { return priority <=> other.priority; }
      };

      auto nodePriority = priorityCalculator(nodeValue);
      if constexpr (detail::IsStdOptionalV<TPriorityResult>)
      {
        if (!nodePriority)
          return;
      }

      auto nodesToProceed = std::priority_queue<PrioritizedNode, std::vector<PrioritizedNode>, std::greater<PrioritizedNode>>();
      nodesToProceed.push({ nodeValue, GetValue(nodePriority) });
      while (!nodesToProceed.empty())
      {
        auto const [pNodeValue, priority] = nodesToProceed.top();
        nodesToProceed.pop();

        auto const control = procedure(pNodeValue, priority);
        switch (control)
        {
        case TraverseControl::Terminate: return;
        case TraverseControl::SkipChildren: continue;
        case TraverseControl::Continue: break;
        }

        for (NodeIDCR childNodeID : Core::GetNodeChildren(pNodeValue))
        {
          auto const& childNodeValue = Core::GetNodeValueP(childNodeID);
          auto childNodePriority = priorityCalculator(childNodeValue);

          if constexpr (detail::IsStdOptionalV<TPriorityResult>)
          {
            if (!childNodePriority)
              continue;
          }

          nodesToProceed.push({ childNodeValue, GetValue(childNodePriority) });
        }
      }
    }

    // Visit nodes in priority order
    constexpr void TraverseNodesByPriority(auto&& procedure, auto&& priorityCalculator) const noexcept
    {
      TraverseNodesByPriority(
        std::forward<decltype(procedure)>(procedure), std::forward<decltype(priorityCalculator)>(priorityCalculator), Core::GetRootNodeValue());
    }

    // Visit entities in breadth first order
    constexpr void TraverseEntitiesBreadthFirst(auto&& procedure) const noexcept
    {
      TraverseNodesBreadthFirst([&](auto const nodeValue) { return procedure(GetNodeEntities(nodeValue), GetNodeBox(nodeValue)); });
    }

    // Visit entities in depth first order
    constexpr void TraverseEntitiesDepthFirst(auto&& procedure) const noexcept
    {
      TraverseNodesDepthFirst([&](auto const nodeValue) { return procedure(GetNodeEntities(nodeValue), GetNodeBox(nodeValue)); });
    }

    // Visit entities in priority order
    constexpr void TraverseEntitiesByPriority(auto&& procedure, auto&& priorityCalculator) const noexcept
    {
      TraverseNodesByPriority(
        [&](auto const nodeValue, auto const& priority) { return procedure(GetNodeEntities(nodeValue), GetNodeBox(nodeValue), priority); },
        [&](auto const nodeValue) { return priorityCalculator(GetNodeBox(nodeValue)); });
    }

    // Collect all item id, traversing the tree in breadth-first search order
    std::vector<EntityID> GetEntitiesBreadthFirst(NodeIDCR rootKey = Core::GetRootNodeID(), bool shouldSortInsideNodes = false) const noexcept
    {
      auto entityIDs = std::vector<EntityID>();
      TraverseNodesBreadthFirst(
        [&](auto const pNodeValue) {
          auto const& entities = Core::GetNodeEntities(pNodeValue);
          auto const entityIDsSize = entityIDs.size();
          entityIDs.insert(entityIDs.end(), entities.begin(), entities.end());
          if (shouldSortInsideNodes)
            std::sort(entityIDs.begin() + entityIDsSize, entityIDs.end());

          return TraverseControl::Continue;
        },
        Core::GetNodeValueP(rootKey));
      return entityIDs;
    }

  private:
    void CollectAllEntitiesInDFSRecursive(NodeValueCP parentNode, std::vector<EntityID>& foundEntities, bool shouldSortInsideNodes) const noexcept
    {
      auto const& entities = Core::GetNodeEntities(parentNode);
      auto const entityIDsSize = foundEntities.size();
      foundEntities.insert(foundEntities.end(), entities.begin(), entities.end());
      if (shouldSortInsideNodes)
        std::sort(foundEntities.begin() + entityIDsSize, foundEntities.end());

      for (NodeIDCR childKey : Core::GetNodeChildren(parentNode))
        CollectAllEntitiesInDFSRecursive(Core::GetNodeValueP(childKey), foundEntities, shouldSortInsideNodes);
    }

  public:
    // Collect all entity id, traversing the tree in depth-first search pre-order
    std::vector<EntityID> GetEntitiesDepthFirst(NodeIDCR parentKey = Core::GetRootNodeID(), bool shouldSortInsideNodes = false) const noexcept
    {
      auto entityIDs = std::vector<EntityID>{};
      CollectAllEntitiesInDFSRecursive(Core::GetNodeValueP(parentKey), entityIDs, shouldSortInsideNodes);
      return entityIDs;
    }


    template<typename TAgainst, typename TTester>
    constexpr bool TestEntity(TTester const& tester, EntityID entityID, EntityContainerView entities, TAgainst const& against) const noexcept
    {
      if constexpr (std::is_same_v<TTester, std::monostate>)
      {
        return true;
      }
      else if constexpr (std::is_invocable_r_v<bool, TTester, EntityID>)
      {
        return tester(entityID);
      }
      else if constexpr (std::is_invocable_r_v<bool, TTester, typename EA::Entity>)
      {
        return tester(EA::GetEntity(entities, entityID));
      }
      else if constexpr (std::is_invocable_r_v<bool, TTester, EntityID, TAgainst&>)
      {
        return tester(entityID, against);
      }
      else if constexpr (std::is_invocable_r_v<bool, TTester, typename EA::Entity, TAgainst&>)
      {
        return tester(EA::GetEntity(entities, entityID), against);
      }
      else
      {
        static_assert(sizeof(TTester) == 0, "Unsupported tester signature");
      }
    }

  public: // Search functions
    // Pick search
    //
    // Accepted tester signatures:
    // * bool(EntityID) / bool(EntityID, TVector&)
    // * bool(Entity) / bool(Entity, TVector&)
    template<typename TTester = std::monostate>
    std::vector<EntityID> PickSearch(
      TVector const& pickPoint, EntityContainerView entities, TFloatScalar tolerance = GA::BASE_TOLERANCE, TTester tester = {}) const noexcept
    {
      auto foundEntitiyIDs = std::vector<EntityID>();
      if (!IGM::DoesBoxContainPointAD(Core::GetNodeBox(Core::GetRootNodeValue()), pickPoint, tolerance))
        return foundEntitiyIDs;

      foundEntitiyIDs.reserve(100);

      TraverseNodesDepthFirst([&](auto const nodeValue) {
        if (!IGM::DoesBoxContainPointAD(Core::GetNodeMinPoint(nodeValue), Core::GetNodeSize(nodeValue), pickPoint, tolerance))
          return TraverseControl::SkipChildren;

        auto const pointTest = [&](auto const entityID) {
          if constexpr (EA::GEOMETRY_TYPE == GeometryType::Point)
          {
            return GA::ArePointsEqual(EA::GetGeometry(entities, entityID), pickPoint, tolerance);
          }
          else if constexpr (EA::GEOMETRY_TYPE == GeometryType::Box)
          {
            return GA::DoesBoxContainPoint(EA::GetGeometry(entities, entityID), pickPoint, tolerance);
          }
          else
          {
            static_assert(false, "Unsupported geometry type!");
          }
          ORTHOTREE_UNREACHABLE();
        };

        std::ranges::copy_if(Core::GetNodeEntities(nodeValue), std::back_inserter(foundEntitiyIDs), [&](auto const entityID) {
          if (!pointTest(entityID))
            return false;

          return TestEntity(tester, entityID, entities, pickPoint);
        });

        return TraverseControl::Continue;
      });

      return foundEntitiyIDs;
    }

  public:
    // Range search
    //
    // Accepted tester signatures:
    // * bool(EntityID) / bool(EntityID, TVector&)
    // * bool(Entity) / bool(Entity, TVector&)
    template<bool DO_RANGE_MUST_FULLY_CONTAIN = true, typename TTester = std::monostate>
    std::vector<EntityID> RangeSearch(TBox const& range, EntityContainerView entities, TFloatScalar tolerance = GA::BASE_TOLERANCE, TTester tester = {}) const noexcept
    {
      auto foundEntities = std::vector<EntityID>{};

      auto const entityNo = entities.size();
      if (IGM::DoesRangeContainBoxAD(range, Core::GetNodeBox(Core::GetRootNodeValue()), tolerance))
      {
        foundEntities.resize(entityNo);

        if constexpr (EA::REQUIRES_CONTIGUOUS_ENTITY_IDS)
          std::iota(foundEntities.begin(), foundEntities.end(), 0);
        else
          std::transform(entities.begin(), entities.end(), foundEntities.begin(), [&entities](auto const& item) {
            return EA::GetEntityID(entities, item);
          });

        return foundEntities;
      }

      // If the range has zero volume, it could stuck at any node comparison with point/side touch. It is eliminated to work node bounding box independently.
      const auto rangeVolume = IGM::GetVolumeAD(range);
      if (rangeVolume <= 0.0)
      {
        return foundEntities;
      }

      auto const boxTest = [&](auto const entityID) {
        if constexpr (EA::GEOMETRY_TYPE == GeometryType::Point)
        {
          return GA::DoesBoxContainPoint(range, EA::GetGeometry(entities, entityID), tolerance);
        }
        else if constexpr (EA::GEOMETRY_TYPE == GeometryType::Box)
        {
          if constexpr (DO_RANGE_MUST_FULLY_CONTAIN)
            return GA::AreBoxesOverlapped(range, EA::GetGeometry(entities, entityID), DO_RANGE_MUST_FULLY_CONTAIN, false, tolerance);
          else
            return GA::AreBoxesOverlappedStrict(range, EA::GetGeometry(entities, entityID), tolerance);
        }
        else
        {
          static_assert(false, "Unsupported geometry type!");
        }
        ORTHOTREE_UNREACHABLE();
      };

      auto const searchRangeMinPoint = IGM::GetBoxMinPointAD(range);
      auto const searchRangeSize = IGM::GetBoxSizeAD(range);
      TraverseNodesDepthFirst([&](auto const pNodeValue) {
        auto const& nodeMinPoint = Core::GetNodeMinPoint(pNodeValue);
        auto const& nodeSize = Core::GetNodeSize(pNodeValue);
        if (!IGM::AreBoxesOverlappingByMinPoint(searchRangeMinPoint, searchRangeSize, nodeMinPoint, nodeSize, tolerance))
          return TraverseControl::SkipChildren;

        if (IGM::DoesRangeContainBoxAD(range, nodeMinPoint, nodeSize, tolerance))
        {
          TraverseNodesDepthFirst(
            [&](auto const pChildNodeValue) {
              if constexpr (std::is_same_v<TTester, std::monostate>)
              {
                std::ranges::copy(Core::GetNodeEntities(pChildNodeValue), std::back_inserter(foundEntities));
              }
              else
              {
                std::ranges::copy_if(Core::GetNodeEntities(pChildNodeValue), std::back_inserter(foundEntities), [&](auto const entityID) {
                  return TestEntity(tester, entityID, entities, range);
                });
              }
              return TraverseControl::Continue;
            },
            pNodeValue);

          return TraverseControl::SkipChildren;
        }
        else
        {
          std::ranges::copy_if(Core::GetNodeEntities(pNodeValue), std::back_inserter(foundEntities), [&](auto const entityID) {
            if (boxTest(entityID))
              return true;

            return TestEntity(tester, entityID, entities, range);
          });
        }

        return TraverseControl::Continue;
      });

      return foundEntities;
    }

  private:
    static constexpr PlaneRelation GetEntityPlaneRelation(
      EA::Geometry const& entityGeometry, TScalar distanceOfOrigo, TVector const& planeNormal, TFloatScalar tolerance = GA::BASE_TOLERANCE) noexcept
    {
      if constexpr (EA::GEOMETRY_TYPE == GeometryType::Point)
      {
        return GA::GetPointPlaneRelation(entityGeometry, distanceOfOrigo, planeNormal, tolerance);
      }
      else if constexpr (EA::GEOMETRY_TYPE == GeometryType::Box)
      {
        return IGM::GetBoxPlaneRelationAD(IGM::GetBoxMinPointAD(entityGeometry), IGM::GetBoxSizeAD(entityGeometry), distanceOfOrigo, planeNormal, tolerance);
      }
      else
      {
        static_assert(false, "Unsupported geometry type for kNN!");
      }
      ORTHOTREE_UNREACHABLE();
    }

  public:
    // Hyperplane intersection
    std::vector<EntityID> PlaneSearch(
      TScalar distanceOfOrigo, TVector const& planeNormal, EntityContainerView entities, TFloatScalar tolerance = GA::BASE_TOLERANCE) const noexcept
    {
      assert(GA::IsNormalizedVector(planeNormal));

      auto results = std::vector<EntityID>{};
      TraverseNodesDepthFirst([&](auto const pNodeValue) {
        if (IGM::GetBoxPlaneRelationAD(Core::GetNodeMinPoint(pNodeValue), Core::GetNodeSize(pNodeValue), distanceOfOrigo, planeNormal, tolerance) != PlaneRelation::Hit)
          return TraverseControl::SkipChildren;

        for (auto const entityID : Core::GetNodeEntities(pNodeValue))
          if (GetEntityPlaneRelation(EA::GetGeometry(entities, entityID), distanceOfOrigo, planeNormal, tolerance) == PlaneRelation::Hit)
            results.emplace_back(entityID);

        return TraverseControl::Continue;
      });

      return results;
    }

    // Hyperplane intersection using built-in plane
    std::vector<EntityID> PlaneSearch(TPlane const& plane, EntityContainerView entities, TFloatScalar tolerance = GA::BASE_TOLERANCE) const noexcept
    {
      return PlaneSearch(GA::GetPlaneOrigoDistance(plane), GA::GetPlaneNormal(plane), entities, tolerance);
    }

    // Hyperplane intersection using built-in plane (same as PlaneSearch())
    std::vector<EntityID> PlaneIntersection(
      TScalar distanceOfOrigo, TVector const& planeNormal, EntityContainerView entities, TFloatScalar tolerance = GA::BASE_TOLERANCE) const noexcept
    {
      return PlaneSearch(distanceOfOrigo, planeNormal, entities, tolerance);
    }

    // Hyperplane intersection using built-in plane (same as PlaneSearch())
    std::vector<EntityID> PlaneIntersection(TPlane const& plane, EntityContainerView entities, TFloatScalar tolerance = GA::BASE_TOLERANCE) const noexcept
    {
      return PlaneSearch(plane, entities, tolerance);
    }

    // Hyperplane segmentation, get all elements in positive side (Plane equation: dotProduct(planeNormal, point) = distanceOfOrigo)
    std::vector<EntityID> PlanePositiveSegmentation(
      TScalar distanceOfOrigo, TVector const& planeNormal, EntityContainerView entities, TFloatScalar tolerance = GA::BASE_TOLERANCE) const noexcept
    {
      assert(GA::IsNormalizedVector(planeNormal));

      auto results = std::vector<EntityID>{};
      TraverseNodesDepthFirst([&](auto const pNodeValue) {
        auto const nodeRelation =
          IGM::GetBoxPlaneRelationAD(Core::GetNodeMinPoint(pNodeValue), Core::GetNodeSize(pNodeValue), distanceOfOrigo, planeNormal, tolerance);

        switch (nodeRelation)
        {
        case PlaneRelation::Negative: return TraverseControl::SkipChildren;
        case PlaneRelation::Positive:
          TraverseNodesDepthFirst(
            [&](auto const pChildNodeValue) {
              std::ranges::copy(Core::GetNodeEntities(pChildNodeValue), std::back_inserter(results));
              return TraverseControl::Continue;
            },
            pNodeValue);
          return TraverseControl::SkipChildren;

        case PlaneRelation::Hit:
          for (auto const entityID : Core::GetNodeEntities(pNodeValue))
          {
            auto const entityRelation = GetEntityPlaneRelation(EA::GetGeometry(entities, entityID), distanceOfOrigo, planeNormal, tolerance);
            if (entityRelation == PlaneRelation::Negative)
              continue;

            results.emplace_back(entityID);
          }
          return TraverseControl::Continue;
        }

        ORTHOTREE_UNREACHABLE();
      });

      return results;
    }

    // Hyperplane segmentation, get all elements in positive side (Plane equation: dotProduct(planeNormal, point) = distanceOfOrigo)
    std::vector<EntityID> PlanePositiveSegmentation(TPlane const& plane, EntityContainerView entities, TFloatScalar tolerance = GA::BASE_TOLERANCE) const noexcept
    {
      return PlanePositiveSegmentation(GA::GetPlaneOrigoDistance(plane), GA::GetPlaneNormal(plane), entities, tolerance);
    }

    // Get all entities which relation is positive or intersected by the given space boundary planes
    std::vector<EntityID> FrustumCulling(
      std::span<TPlane const> const& boundaryPlanes, EntityContainerView entities, TFloatScalar tolerance = GA::BASE_TOLERANCE) const noexcept
    {
      auto results = std::vector<EntityID>{};
      if (boundaryPlanes.empty())
        return results;

      assert(std::all_of(boundaryPlanes.begin(), boundaryPlanes.end(), [](auto const& plane) -> bool {
        return GA::IsNormalizedVector(GA::GetPlaneNormal(plane));
      }));

      auto const selector = [&](auto const pNodeValue) -> bool {
        auto const& nodeMinPoint = Core::GetNodeMinPoint(pNodeValue);
        auto const& nodeSize = Core::GetNodeSize(pNodeValue);

        for (auto const& plane : boundaryPlanes)
        {
          auto const relation =
            IGM::GetBoxPlaneRelationAD(nodeMinPoint, nodeSize, GA::GetPlaneOrigoDistance(plane), GA::GetPlaneNormal(plane), tolerance);
          if (relation == PlaneRelation::Hit)
            return true;

          if (relation == PlaneRelation::Negative)
            return false;
        }
        return true;
      };

      auto const procedure = [&](auto const pNodeValue) {
        if (!selector(pNodeValue))
          return TraverseControl::SkipChildren;

        for (auto const entityID : Core::GetNodeEntities(pNodeValue))
        {
          auto relation = PlaneRelation::Negative;
          for (auto const& plane : boundaryPlanes)
          {
            relation =
              GetEntityPlaneRelation(EA::GetGeometry(entities, entityID), GA::GetPlaneOrigoDistance(plane), GA::GetPlaneNormal(plane), tolerance);
            if (relation != PlaneRelation::Positive)
              break;
          }

          if (relation == PlaneRelation::Negative)
            continue;

          results.emplace_back(entityID);
        }

        return TraverseControl::Continue;
      };

      TraverseNodesDepthFirst(procedure);

      return results;
    }

  public: // K Nearest Neighbor
    // Get the precise distance between the entity and kNN's search point. Floating-point return value is required.
    using EntityDistanceFn = std::function<TFloatScalar(TVector const&, EntityID)>;

  private: // K Nearest Neighbor helpers
    static constexpr TFloatScalar GetValueWithToleranceUpper(TFloatScalar value, TFloatScalar tolerance = GA::BASE_TOLERANCE) noexcept
    {
      return std::fmax(tolerance, value * (TFloatScalar(1.0) + tolerance));
    }

    static constexpr TFloatScalar GetValueWithToleranceLower(TFloatScalar value, TFloatScalar tolerance = GA::BASE_TOLERANCE) noexcept
    {
      return value == 0 ? -tolerance : (value * (TFloatScalar(1.0) - tolerance));
    }

    // TODO: remove? lower is not used
    struct FarthestDistance
    {
      TFloatScalar lower;
      TFloatScalar upper;
    };

    struct EntityDistance
    {
      EntityID entityID;
      TFloatScalar distance;

      constexpr auto operator<=>(EntityDistance const& other) const noexcept { return distance <=> other.distance; }
      constexpr bool operator==(EntityDistance const& other) const noexcept { return distance == other.distance; }
    };

    struct EntityDistances
    {
      std::vector<EntityDistance> optimistic;
      std::vector<EntityDistance> pessimistic;
    };

    static void AddEntityDistance(
      std::size_t neighborCount,
      TVector const& searchPoint,
      std::optional<EntityDistanceFn> const& entityDistanceFn,
      auto const& nodeEntityIDs,
      EntityContainerView entities,
      TFloatScalar tolerance,
      EntityDistances& neighborEntities,
      FarthestDistance& farthestEntityDistance) noexcept
    {
      auto constexpr pessimisticCmp = [](auto const& lhs, auto const& rhs) {
        return lhs.pessimisticDistance < rhs.pessimisticDistance;
      };

      auto neighborEntitiesPessimisticMaxHeap = std::vector<TFloatScalar>{};
      for (auto const entityID : nodeEntityIDs)
      {
        typename GA::PointBoxMinMaxDistance pbd;
        if constexpr (EA::GEOMETRY_TYPE == GeometryType::Point)
        {
          const auto distance = GA::Distance(searchPoint, EA::GetGeometry(entities, entityID));
          pbd = { distance, distance };
        }
        else if constexpr (EA::GEOMETRY_TYPE == GeometryType::Box)
        {
          pbd = GA::MinMaxDistance(searchPoint, EA::GetGeometry(entities, entityID), tolerance);
        }
        else
        {
          static_assert(false, "Unsupported geometry type for kNN!");
        }

        if (pbd.min >= farthestEntityDistance.upper)
          continue;

        if (entityDistanceFn)
        {
          pbd.min = (*entityDistanceFn)(searchPoint, entityID);
          if (pbd.min >= farthestEntityDistance.upper)
            continue;

          pbd.minMax = pbd.min;
        }

        auto const shouldHeapify = neighborEntities.optimistic.size() == neighborCount - 1;
        neighborEntities.optimistic.push_back({ entityID, pbd.min });
        neighborEntities.pessimistic.push_back({ entityID, pbd.minMax });
        if (neighborEntities.pessimistic.size() < neighborCount)
          continue;

        if (shouldHeapify)
        {
          std::ranges::make_heap(neighborEntities.optimistic);
          std::ranges::make_heap(neighborEntities.pessimistic);

          farthestEntityDistance.upper = GetValueWithToleranceUpper(neighborEntities.pessimistic[0].distance, tolerance);
          if constexpr (EA ::GEOMETRY_TYPE == GeometryType::Point)
            farthestEntityDistance.lower = farthestEntityDistance.upper;
          else
            farthestEntityDistance.lower = GetValueWithToleranceLower(neighborEntities.pessimistic[0].distance, tolerance);
        }
        else
        {
          std::ranges::push_heap(neighborEntities.optimistic);

          std::ranges::push_heap(neighborEntities.pessimistic);
          std::ranges::pop_heap(neighborEntities.pessimistic);
          neighborEntities.pessimistic.pop_back();

          auto nthFarthestEntityDistanceLimit = GetValueWithToleranceUpper(neighborEntities.pessimistic.front().distance, tolerance);

          if (nthFarthestEntityDistanceLimit < farthestEntityDistance.upper)
          {
            farthestEntityDistance.upper = nthFarthestEntityDistanceLimit;
            if constexpr (EA::GEOMETRY_TYPE == GeometryType::Point)
              farthestEntityDistance.lower = farthestEntityDistance.upper;
            else if constexpr (EA::GEOMETRY_TYPE == GeometryType::Box)
              farthestEntityDistance.lower = GetValueWithToleranceLower(neighborEntities.pessimistic.front().distance, tolerance);
            else
              static_assert(false);

            bool pessimisticRemoved = false;
            while (neighborEntities.optimistic.front().distance > farthestEntityDistance.upper)
            {
              auto const removedEntityID = neighborEntities.optimistic.front().entityID;
              auto const it =
                std::ranges::find_if(neighborEntities.pessimistic, [removedEntityID](auto const& ed) { return ed.entityID == removedEntityID; });
              if (it != neighborEntities.pessimistic.end())
              {
                neighborEntities.pessimistic.erase(it);
                pessimisticRemoved = true;
              }
              std::ranges::pop_heap(neighborEntities.optimistic);
              neighborEntities.optimistic.pop_back();
            }

            if (pessimisticRemoved)
              std::ranges::make_heap(neighborEntities.pessimistic);
          }
        }
      }
    }

    template<bool SHOULD_SORT_ENTITIES_BY_DISTANCE = true>
    static constexpr std::vector<EntityID> ConvertEntityDistanceToList(std::vector<EntityDistance>& neighborEntities, std::size_t neighborNo) noexcept
    {
      auto entityIDs = std::vector<EntityID>();
      if (neighborEntities.empty())
        return entityIDs;

      if (neighborEntities.size() < neighborNo)
      {
        if constexpr (SHOULD_SORT_ENTITIES_BY_DISTANCE)
        {
          std::sort(neighborEntities.begin(), neighborEntities.end());
        }
      }
      else
      {
        std::sort_heap(neighborEntities.begin(), neighborEntities.end());
      }

      auto const entityNo = neighborEntities.size();
      entityIDs.resize(entityNo);
      for (std::size_t i = 0; i < entityNo; ++i)
        entityIDs[i] = neighborEntities[i].entityID;

      return entityIDs;
    }

    constexpr IGM::Geometry GetNodeWallDistance(TVector const& searchPoint, NodeValueCP pNodeValue, bool isInsideConsideredAsZero) const noexcept
    {
      return IGM::GetBoxWallDistanceAD(searchPoint, Core::GetNodeMinPoint(pNodeValue), Core::GetNodeSize(pNodeValue), isInsideConsideredAsZero);
    }

  public:
    // Get K Nearest Neighbor sorted by distance (point distance should be less than maxDistanceWithin, it is used as a Tolerance check). It may
    // results more element than neighborCount, if those are in equal distance (point-like) or possible hit (box-like).
    template<bool SHOULD_SORT_ENTITIES_BY_DISTANCE = true>
    std::vector<EntityID> GetNearestNeighbors(
      TVector const& searchPoint,
      std::size_t neighborCount,
      TScalar maxDistanceWithin,
      EntityContainerView entities,
      TFloatScalar tolerance = GA::BASE_TOLERANCE,
      std::optional<EntityDistanceFn> const& entityDistanceFn = std::nullopt) const noexcept
    {
      assert(neighborCount > 0 && "At least one neighbor must be requested!");
      if (neighborCount == 0)
        return {};

      auto neighborEntities = EntityDistances{};
      neighborEntities.optimistic.reserve(neighborCount + 4);
      neighborEntities.pessimistic.reserve(neighborCount);

      // farthestEntityDistance already contains the numerical tolerance
      auto farthestEntityDistance =
        FarthestDistance{ {},
                          maxDistanceWithin == std::numeric_limits<TScalar>::max() ? std::numeric_limits<TScalar>::max()
                                                                                   : GetValueWithToleranceUpper(maxDistanceWithin, tolerance) };
      TraverseNodesByPriority(
        [&](auto const pNodeValue, TFloatScalar nodeDistance) -> TraverseControl {
          if (nodeDistance >= farthestEntityDistance.upper)
            return TraverseControl::Terminate;

          AddEntityDistance(
            neighborCount, searchPoint, entityDistanceFn, Core::GetNodeEntities(pNodeValue), entities, tolerance, neighborEntities, farthestEntityDistance);

          return TraverseControl::Continue;
        },
        [&](auto const pNodeValue) -> std::optional<TFloatScalar> {
          auto wallDistance = GetNodeWallDistance(searchPoint, pNodeValue, true);
          if (wallDistance >= farthestEntityDistance.upper)
            return std::nullopt;

          return wallDistance;
        });

      return ConvertEntityDistanceToList<SHOULD_SORT_ENTITIES_BY_DISTANCE>(neighborEntities.optimistic, neighborCount);
    }

    // Get K Nearest Neighbor sorted by distance
    template<bool SHOULD_SORT_ENTITIES_BY_DISTANCE = true>
    std::vector<EntityID> GetNearestNeighbors(
      TVector const& searchPoint,
      std::size_t neighborNo,
      EntityContainerView entities,
      TFloatScalar tolerance = GA::BASE_TOLERANCE,
      std::optional<EntityDistanceFn> const& entityDistanceFn = std::nullopt) const noexcept
    {
      return this->GetNearestNeighbors<SHOULD_SORT_ENTITIES_BY_DISTANCE>(
        searchPoint, neighborNo, std::numeric_limits<TScalar>::max(), entities, tolerance, entityDistanceFn);
    }


  private:
    struct SweepAndPruneDatabase
    {
      constexpr SweepAndPruneDatabase(EntityContainerView entities, auto const& entityIDs) noexcept
      : m_sortedEntityIDs(entityIDs.begin(), entityIDs.end())
      {
        std::sort(m_sortedEntityIDs.begin(), m_sortedEntityIDs.end(), [&](auto const entityIDL, auto const entityIDR) {
          if constexpr (EA::GEOMETRY_TYPE == GeometryType::Point)
          {
            return GA::GetPointC(EA::GetGeometry(entities, entityIDL), 0) < GA::GetPointC(EA::GetGeometry(entities, entityIDR), 0);
          }
          else if constexpr (EA::GEOMETRY_TYPE == GeometryType::Box)
          {
            return GA::GetBoxMinC(EA::GetGeometry(entities, entityIDL), 0) < GA::GetBoxMinC(EA::GetGeometry(entities, entityIDR), 0);
          }
          else
          {
            static_assert(false, "Unsupported geometry type for collision detection!");
          }
          ORTHOTREE_UNREACHABLE();
        });
      }

      constexpr std::vector<EntityID> const& GetEntities() const noexcept { return m_sortedEntityIDs; }

    private:
      std::vector<EntityID> m_sortedEntityIDs;
    };

  public:
    // Client-defined Collision detector based on indexes. AABB intersection is executed independently from this checker.
    using FCollisionDetector = std::function<bool(EntityID, EntityID)>;

  private:
    // Collision detection: Returns all overlapping entities from the source trees.
    static void CollisionDetection(
      OrthoTreeQueryBase const& leftTree,
      EntityContainerView leftEntities,
      NodeValueCP leftNodeValue,
      OrthoTreeQueryBase const& rightTree,
      EntityContainerView rightEntities,
      NodeValueCP rightNodeValue,
      std::vector<std::pair<EntityID, EntityID>>& collidedEntities,
      TFloatScalar tolerance,
      std::optional<FCollisionDetector> const& collisionDetector) noexcept
    {
      struct NodeValueAndStatus
      {
        NodeValueCP pNodeValue;
        bool IsTraversed;
      };

      enum : bool
      {
        Left,
        Right
      };

      auto const isSameTree = &leftTree == &rightTree;

      auto const trees = std::array{ &leftTree, &rightTree };

      auto entitiesInOrderCache = std::array<std::unordered_map<NodeValueCP, SweepAndPruneDatabase>, 2>{};
      auto const GetOrCreateEntitiesInOrder = [&](bool side, NodeValueCP pNodeValue, EntityContainerView entities) -> std::vector<EntityID> const& {
        if (isSameTree)
          side = Left;

        auto itKeyAndSPD = entitiesInOrderCache[side].find(pNodeValue);
        if (itKeyAndSPD == entitiesInOrderCache[side].end())
        {
          bool isInserted = false;
          std::tie(itKeyAndSPD, isInserted) =
            entitiesInOrderCache[side].emplace(pNodeValue, SweepAndPruneDatabase(entities, trees[side]->GetNodeEntities(pNodeValue)));
        }

        return itKeyAndSPD->second.GetEntities();
      };

      auto nodePairToProceed = std::queue<std::array<NodeValueAndStatus, 2>>{};
      nodePairToProceed.push(
        {
          NodeValueAndStatus{  leftNodeValue, false },
          NodeValueAndStatus{ rightNodeValue, false }
      });
      while (!nodePairToProceed.empty())
      {
        auto const parentNodePair = nodePairToProceed.front();
        nodePairToProceed.pop();

        // Check the current ascendant content

        auto const& leftEntitiesInOrder = GetOrCreateEntitiesInOrder(Left, parentNodePair[Left].pNodeValue, leftEntities);
        auto const& rightEntitiesInOrder = GetOrCreateEntitiesInOrder(Right, parentNodePair[Right].pNodeValue, rightEntities);

        auto const rightEntityNo = rightEntitiesInOrder.size();
        std::size_t iRightEntityBegin = 0;
        for (auto const leftEntityID : leftEntitiesInOrder)
        {
          auto const& leftEntityGeometry = EA::GetGeometry(leftEntities, leftEntityID);
          for (; iRightEntityBegin < rightEntityNo; ++iRightEntityBegin)
          {
            auto const& rightEntityGeometry = EA::GetGeometry(rightEntities, rightEntitiesInOrder[iRightEntityBegin]);
            if constexpr (EA::GEOMETRY_TYPE == GeometryType::Point)
            {
              if (GA::GetPointC(rightEntityGeometry, 0) >= GA::GetPointC(leftEntityGeometry, 0))
                break; // sweep and prune optimization
            }
            else if constexpr (EA::GEOMETRY_TYPE == GeometryType::Box)
            {
              if (GA::GetBoxMaxC(rightEntityGeometry, 0) >= GA::GetBoxMinC(leftEntityGeometry, 0))
                break; // sweep and prune optimization
            }
            else
            {
              static_assert(false, "Unsupported geometry type for collision detection!");
            }
          }

          for (std::size_t iRightEntity = iRightEntityBegin; iRightEntity < rightEntityNo; ++iRightEntity)
          {
            auto const righEntityID = rightEntitiesInOrder[iRightEntity];

            auto const& rightEntityGeometry = EA::GetGeometry(rightEntities, righEntityID);
            if constexpr (EA::GEOMETRY_TYPE == GeometryType::Point)
            {
              if (GA::GetPointC(leftEntityGeometry, 0) < GA::GetPointC(rightEntityGeometry, 0))
                break; // sweep and prune optimization

              if (GA::ArePointEqual(leftEntityGeometry, rightEntityGeometry, tolerance))
                if (!collisionDetector || (*collisionDetector)(leftEntityID, righEntityID))
                  collidedEntities.emplace_back(leftEntityID, righEntityID);
            }
            else if constexpr (EA::GEOMETRY_TYPE == GeometryType::Box)
            {
              if (GA::GetBoxMaxC(leftEntityGeometry, 0) < GA::GetBoxMinC(rightEntityGeometry, 0))
                break; // sweep and prune optimization

              if (GA::AreBoxesOverlapped(leftEntityGeometry, rightEntityGeometry, false, false, tolerance))
                if (!collisionDetector || (*collisionDetector)(leftEntityID, righEntityID))
                  collidedEntities.emplace_back(leftEntityID, righEntityID);
            }
            else
            {
              static_assert(false, "Unsupported geometry type for collision detection!");
            }
          }
        }

        // Collect children
        auto childNodes = std::array<std::vector<NodeValueAndStatus>, 2>{};
        for (auto const sideID : { Left, Right })
        {
          auto const& [pNodeValue, isTraversed] = parentNodePair[sideID];
          if (isTraversed)
            continue;

          auto const& childIDs = trees[sideID]->Core::GetNodeChildren(pNodeValue);
          childNodes[sideID].resize(childIDs.size());
          std::transform(childIDs.begin(), childIDs.end(), childNodes[sideID].begin(), [&](NodeIDCR childNodeID) -> NodeValueAndStatus {
            return { trees[sideID]->Core::GetNodeValueP(childNodeID), false };
          });
        }

        // Stop condition
        if (childNodes[Left].empty() && childNodes[Right].empty())
          continue;

        // Add parent if it has any element
        for (auto const sideID : { Left, Right })
          if (!trees[sideID]->IsNodeEntitiesEmpty(parentNodePair[sideID].pNodeValue))
            childNodes[sideID].push_back({ parentNodePair[sideID].pNodeValue, true });


        // Cartesian product of childNodes left and right
        for (auto const& leftChildNode : childNodes[Left])
          for (auto const& rightChildNode : childNodes[Right])
            if (!(leftChildNode.pNodeValue == parentNodePair[Left].pNodeValue && rightChildNode.pNodeValue == parentNodePair[Right].pNodeValue))
              if (IGM::AreBoxesOverlappingByMinPoint(
                    trees[Left]->Core::GetNodeMinPoint(leftChildNode.pNodeValue),
                    trees[Left]->Core::GetNodeSize(leftChildNode.pNodeValue),
                    trees[Right]->Core::GetNodeMinPoint(rightChildNode.pNodeValue),
                    trees[Right]->Core::GetNodeSize(rightChildNode.pNodeValue),
                    tolerance))
                nodePairToProceed.emplace(std::array{ leftChildNode, rightChildNode });
      }
    }

  public:
    // Collision detection: Returns all overlapping entities from the source trees.
    static std::vector<std::pair<EntityID, EntityID>> CollisionDetection(
      OrthoTreeQueryBase const& leftTree,
      EntityContainerView leftEntities,
      OrthoTreeQueryBase const& rightTree,
      EntityContainerView rightEntities,
      TFloatScalar tolerance = GA::BASE_TOLERANCE,
      std::optional<FCollisionDetector> const& collisionDetector = std::nullopt) noexcept
    {
      std::vector<std::pair<EntityID, EntityID>> collidedEntities;
      collidedEntities.reserve(leftEntities.size() / 10);

      CollisionDetection(
        leftTree,
        leftEntities,
        leftTree.Core::GetNodeValueP(Core::GetRootNodeID()),
        rightTree,
        rightEntities,
        rightTree.Core::GetNodeValueP(Core::GetRootNodeID()),
        collidedEntities,
        tolerance,
        collisionDetector);

      return collidedEntities;
    }


    // Collision detection: Returns all overlapping boxes from the source trees.
    std::vector<std::pair<EntityID, EntityID>> CollisionDetection(
      EntityContainerView const& boxes,
      OrthoTreeQueryBase const& otherTree,
      EntityContainerView const& otherBoxes,
      TFloatScalar tolerance = GA::BASE_TOLERANCE,
      std::optional<FCollisionDetector> const& collisionDetector = std::nullopt) const noexcept
    {
      return CollisionDetection(*this, boxes, otherTree, otherBoxes, tolerance, collisionDetector);
    }

  private:
    struct NodeCollisionContext
    {
      NodeValueCP pNodeValue;
      IGM::Box Box;
      std::vector<EntityID> EntityIDs;
    };

    constexpr void FillNodeCollisionContext(NodeValueCP nodeValue, NodeCollisionContext& nodeContext, auto const& comparator) const noexcept
    {
      auto const& nodeEntities = Core::GetNodeEntities(nodeValue);

      nodeContext.pNodeValue = nodeValue;
      nodeContext.EntityIDs.clear();
      nodeContext.EntityIDs.assign(nodeEntities.begin(), nodeEntities.end());
      std::ranges::sort(nodeContext.EntityIDs, comparator);
      nodeContext.Box = Core::GetNodeBox(nodeValue);
    }

    constexpr void InsertCollidedEntitiesInsideNode(
      EntityContainerView entities,
      NodeCollisionContext const& context,
      std::vector<std::pair<EntityID, EntityID>>& collidedEntities,
      TFloatScalar tolerance,
      std::optional<FCollisionDetector> const& collisionDetector) const noexcept
    {
      auto const& entityIDs = context.EntityIDs;
      auto const entityNo = entityIDs.size();
      for (std::size_t i = 0; i < entityNo; ++i)
      {
        auto const entityIDI = entityIDs[i];
        auto const& entityBoxI = EA::GetGeometry(entities, entityIDI);

        for (std::size_t j = i + 1; j < entityNo; ++j)
        {
          auto const entityIDJ = entityIDs[j];
          auto const& entityBoxJ = EA::GetGeometry(entities, entityIDJ);

          if constexpr (EA::GEOMETRY_TYPE == GeometryType::Point)
          {
            if (GA::GetPointC(entityBoxI, 0) < GA::GetPointC(entityBoxJ, 0))
              break; // sweep and prune optimization

            if (GA::ArePointsEqual(entityBoxI, entityBoxJ, tolerance))
              if (!collisionDetector || (*collisionDetector)(entityIDI, entityIDJ))
                collidedEntities.emplace_back(entityIDI, entityIDJ);
          }
          else if constexpr (EA::GEOMETRY_TYPE == GeometryType::Box)
          {
            if (GA::GetBoxMaxC(entityBoxI, 0) < GA::GetBoxMinC(entityBoxJ, 0))
              break; // sweep and prune optimization

            if (GA::AreBoxesOverlappedStrict(entityBoxI, entityBoxJ, tolerance))
              if (!collisionDetector || (*collisionDetector)(entityIDI, entityIDJ))
                collidedEntities.emplace_back(entityIDI, entityIDJ);
          }
          else
          {
            static_assert(false, "Unsupported geometry type for collision detection!");
          }
        }
      }
    }

    constexpr void InsertCollidedEntitiesWithParents(
      EntityContainerView entities,
      std::vector<NodeCollisionContext> const& nodeContextStack,
      std::vector<std::pair<EntityID, EntityID>>& collidedEntities,
      TFloatScalar tolerance,
      std::optional<FCollisionDetector> const& collisionDetector) const noexcept
    {
      auto const& nodeContext = nodeContextStack.back();
      auto const& nodeMinPoint = nodeContext.Box.Min;
      auto const& nodeSize = Core::GetNodeSize(nodeContext.pNodeValue);
      auto const& entityIDs = nodeContext.EntityIDs;

      auto const entityNo = entityIDs.size();
      auto const depthNo = nodeContextStack.size();
      for (depth_t parentDepthID = 0; parentDepthID < depthNo - 1; ++parentDepthID)
      {
        auto const& [pParentNodeValue, parentBox, parenEntityIDs] = nodeContextStack[parentDepthID];

        auto iEntityBegin = std::size_t{};
        for (auto const parenEntityID : parenEntityIDs)
        {
          auto const& parentEntityGeometry = EA::GetGeometry(entities, parenEntityID);
          if constexpr (EA::GEOMETRY_TYPE == GeometryType::Point)
          {
            if (GA::GetPointC(parentEntityGeometry, 0) > nodeContext.Box.Max[0])
              break;
          }
          else if constexpr (EA::GEOMETRY_TYPE == GeometryType::Box)
          {
            if (GA::GetBoxMinC(parentEntityGeometry, 0) > nodeContext.Box.Max[0])
              break;
          }
          else
          {
            static_assert(false, "Unsupported geometry type for collision detection!");
          }

          auto const parentEntityMinPoint = IGM::GetBoxMinPointAD(parentEntityGeometry);
          auto const parentEntitySize = IGM::GetBoxSizeAD(parentEntityGeometry);
          if (!IGM::AreBoxesOverlappingByMinPoint(nodeMinPoint, nodeSize, parentEntityMinPoint, parentEntitySize, tolerance))
            continue;

          for (; iEntityBegin < entityNo; ++iEntityBegin)
          {
            if constexpr (EA::GEOMETRY_TYPE == GeometryType::Point)
            {
              if (GA::GetPointC(EA::GetGeometry(entities, entityIDs[iEntityBegin]), 0) >= GA::GetPointC(parentEntityGeometry, 0))
                break; // sweep and prune optimization
            }
            else if constexpr (EA::GEOMETRY_TYPE == GeometryType::Box)
            {
              if (GA::GetBoxMaxC(EA::GetGeometry(entities, entityIDs[iEntityBegin]), 0) >= GA::GetBoxMinC(parentEntityGeometry, 0))
                break; // sweep and prune optimization
            }
            else
            {
              static_assert(false, "Unsupported geometry type for collision detection!");
            }
          }

          for (std::size_t iEntity = iEntityBegin; iEntity < entityNo; ++iEntity)
          {
            auto const entityID = entityIDs[iEntity];
            auto const& entityGeometry = EA::GetGeometry(entities, entityID);

            if constexpr (EA::GEOMETRY_TYPE == GeometryType::Point)
            {
              if (GA::GetPointC(parentEntityGeometry, 0) < GA::GetPointC(entityGeometry, 0))
                break; // sweep and prune optimization

              if (GA::ArePointsEqual(parentEntityGeometry, entityGeometry, tolerance))
                if (!collisionDetector || (*collisionDetector)(entityID, parenEntityID))
                  collidedEntities.emplace_back(entityID, parenEntityID);
            }
            else if constexpr (EA::GEOMETRY_TYPE == GeometryType::Box)
            {
              if (GA::GetBoxMaxC(parentEntityGeometry, 0) < GA::GetBoxMinC(entityGeometry, 0))
                break; // sweep and prune optimization

              if (GA::AreBoxesOverlappedStrict(entityGeometry, parentEntityGeometry, tolerance))
                if (!collisionDetector || (*collisionDetector)(entityID, parenEntityID))
                  collidedEntities.emplace_back(entityID, parenEntityID);
            }
            else
            {
              static_assert(false, "Unsupported geometry type for collision detection!");
            }
          }
        }
      }
    }

    void InsertCollidedEntitiesInSubtree(
      EntityContainerView entities,
      auto const& comparator,
      NodeValueCP nodeValue,
      std::vector<NodeCollisionContext>& nodeContextStack,
      std::vector<std::pair<EntityID, EntityID>>& collidedEntities,
      TFloatScalar tolerance,
      std::optional<FCollisionDetector> const& collisionDetector) const noexcept
    {
      auto& nodeContext = nodeContextStack.emplace_back();
      FillNodeCollisionContext(nodeValue, nodeContext, comparator);
      InsertCollidedEntitiesInsideNode(entities, nodeContext, collidedEntities, tolerance, collisionDetector);
      InsertCollidedEntitiesWithParents(entities, nodeContextStack, collidedEntities, tolerance, collisionDetector);

      for (NodeIDCR childKey : Core::GetNodeChildren(nodeValue))
        InsertCollidedEntitiesInSubtree(entities, comparator, Core::GetNodeValueP(childKey), nodeContextStack, collidedEntities, tolerance, collisionDetector);

      // Pairwise sub-tree collision detection for loose octree
      if constexpr (CONFIG::LOOSE_FACTOR > 1.0)
      {
        auto const& childNodeIDs = Core::GetNodeChildren(nodeValue);
        for (auto it1 = childNodeIDs.begin(); it1 != childNodeIDs.end(); ++it1)
        {
          for (auto it2 = it1 + 1; it2 != childNodeIDs.end(); ++it2)
          {
            CollisionDetection(
              *this, entities, Core::GetNodeValueP(*it1), *this, entities, Core::GetNodeValueP(*it2), collidedEntities, tolerance, collisionDetector);
          }
        }
      }

      nodeContextStack.pop_back();
    }

    // Collision detection between the stored entities from bottom to top logic
    template<bool IS_PARALLEL_EXEC = false>
    std::vector<std::pair<EntityID, EntityID>> CollectCollidedEntities(
      EntityContainerView entities,
      TFloatScalar tolerance = EA::GEOMETRY_TYPE == GeometryType::Point ? GA::BASE_TOLERANCE : 0,
      std::optional<FCollisionDetector> const& collisionDetector = std::nullopt) const noexcept
    {
      auto const comparator = [&entities](EntityID entityID1, EntityID entityID2) {
        if constexpr (EA::GEOMETRY_TYPE == GeometryType::Point)
        {
          auto const x1 = GA::GetPointC(EA::GetGeometry(entities, entityID1), 0);
          auto const x2 = GA::GetPointC(EA::GetGeometry(entities, entityID2), 0);
          return x1 < x2 || (x1 == x2 && entityID1 < entityID2);
        }
        else if constexpr (EA::GEOMETRY_TYPE == GeometryType::Box)
        {
          auto const x1 = GA::GetBoxMinC(EA::GetGeometry(entities, entityID1), 0);
          auto const x2 = GA::GetBoxMinC(EA::GetGeometry(entities, entityID2), 0);
          return x1 < x2 || (x1 == x2 && entityID1 < entityID2);
        }
        else
        {
          static_assert(false, "Unsupported geometry type for collision detection!");
        }
      };

      auto const entityNo = entities.size();
      auto collidedEntities = std::vector<std::pair<EntityID, EntityID>>{};
      collidedEntities.reserve(std::max<std::size_t>(100, entityNo / 10));

      // non-parallel execution
      if constexpr (!IS_PARALLEL_EXEC)
      {
        auto nodeContextStack = std::vector<NodeCollisionContext>{};
        nodeContextStack.reserve(this->GetDepthNo());
        InsertCollidedEntitiesInSubtree(
          entities, comparator, Core::GetNodeValueP(Core::GetRootNodeID()), nodeContextStack, collidedEntities, tolerance, collisionDetector);

        return collidedEntities;
      }
      else
      {
        // maybe parallel execution

        auto const threadNum = uint32_t(std::thread::hardware_concurrency());
        auto const isSingleThreadMoreEffective = Core::GetNodeCount() < threadNum * 3;
        if (isSingleThreadMoreEffective)
        {
          auto nodeContextStack = std::vector<NodeCollisionContext>();
          nodeContextStack.reserve(this->GetDepthNo());
          InsertCollidedEntitiesInSubtree(
            entities, comparator, Core::GetNodeValueP(Core::GetRootNodeID()), nodeContextStack, collidedEntities, tolerance, collisionDetector);

          return collidedEntities;
        }

        constexpr uint32_t INVALID_INDEX = std::numeric_limits<uint32_t>::max();
        struct NodeData
        {
          NodeValueCP nodeValue;
          uint32_t parentID = INVALID_INDEX;
          uint32_t contextID = INVALID_INDEX;
        };

        auto nodeQueue = std::vector<NodeData>{};
        nodeQueue.reserve(threadNum * 2);
        nodeQueue.push_back(NodeData{ Core::GetNodeValueP(Core::GetRootNodeID()), INVALID_INDEX, INVALID_INDEX });

        auto nodeContextMap = std::vector<NodeCollisionContext>{};

        uint32_t nodeQueueNum = 1;
        uint32_t childNodeAddedParentNum = 0;
        for (uint32_t i = 0; 0 < nodeQueueNum && nodeQueueNum < threadNum - 2; --nodeQueueNum, ++i)
        {
          ++childNodeAddedParentNum;
          auto& [nodeValue, parentID, contextID] = nodeQueue[i];
          contextID = static_cast<uint32_t>(nodeContextMap.size());
          auto& nodeContext = nodeContextMap.emplace_back();
          FillNodeCollisionContext(nodeValue, nodeContext, comparator);

          for (NodeIDCR childNodeID : Core::GetNodeChildren(nodeValue))
          {
            nodeQueue.push_back(NodeData{ Core::GetNodeValueP(childNodeID), i, INVALID_INDEX });
            ++nodeQueueNum;
          }
        }

        if (nodeQueueNum < threadNum)
        {
          auto nodeContextStack = std::vector<NodeCollisionContext>();
          nodeContextStack.reserve(this->GetDepthNo());
          InsertCollidedEntitiesInSubtree(
            entities, comparator, Core::GetNodeValueP(Core::GetRootNodeID()), nodeContextStack, collidedEntities, tolerance, collisionDetector);

          return collidedEntities;
        }

        // parallel execution

        struct TaskContext
        {
          NodeValueCP nodeValue;
          NodeValueCP nodeValueSecondary;
          uint32_t parentID;
          uint32_t contextID;
          std::vector<std::pair<EntityID, EntityID>> collidedEntities;
        };

        auto const nodeQueueAllNo = nodeQueue.size();
        auto const nodeQueueBegin = nodeQueueAllNo - nodeQueueNum;
        auto taskContexts = std::vector<TaskContext>(nodeQueueNum);
        for (std::size_t taskID = 0; taskID < nodeQueueNum; ++taskID)
        {
          auto const& [nodeValue, parentID, contextID] = nodeQueue[nodeQueueBegin + taskID];
          taskContexts[taskID].nodeValue = nodeValue;
          taskContexts[taskID].parentID = parentID;
          taskContexts[taskID].contextID = contextID;
        }

        EXEC_POL_DEF(epcd); // GCC 11.3
        std::for_each(EXEC_POL_ADD(epcd) taskContexts.begin(), taskContexts.end(), [&](auto& taskContext) {
          auto const& [nodeValue, nvs_, parentID, contextID, ce_] = taskContext;

          auto nodeContextStack = std::vector<NodeCollisionContext>();
          nodeContextStack.reserve(this->GetDepthNo());

          auto parentID_ = parentID;
          while (parentID_ != INVALID_INDEX)
          {
            auto const& parentNodeData = nodeQueue[parentID_];
            nodeContextStack.push_back(nodeContextMap[parentNodeData.contextID]);
            parentID_ = parentNodeData.parentID;
          }

          std::ranges::reverse(nodeContextStack);
          InsertCollidedEntitiesInSubtree(entities, comparator, nodeValue, nodeContextStack, taskContext.collidedEntities, tolerance, collisionDetector);
        });

        if constexpr (CONFIG::LOOSE_FACTOR > 1.0)
        {
          for (std::size_t i = 0; i < childNodeAddedParentNum; ++i)
          {
            auto const& childNodeIDs = Core::GetNodeChildren(nodeQueue[i].nodeValue);
            for (auto it1 = childNodeIDs.begin(); it1 != childNodeIDs.end(); ++it1)
            {
              for (auto it2 = it1 + 1; it2 != childNodeIDs.end(); ++it2)
              {
                taskContexts.push_back({ .nodeValue = Core::GetNodeValueP(*it1), .nodeValueSecondary = Core::GetNodeValueP(*it2) });
              }
            }
          }

          EXEC_POL_DEF(epst); // GCC 11.3
          std::for_each(EXEC_POL_ADD(epst) taskContexts.begin() + nodeQueueNum, taskContexts.end(), [&](auto& taskContext) {
            CollisionDetection(
              *this, entities, taskContext.nodeValue, *this, entities, taskContext.nodeValueSecondary, taskContext.collidedEntities, tolerance, collisionDetector);
          });
        }
        auto collidedEntitiesInParents = std::vector<std::pair<EntityID, EntityID>>{};
        auto nodeContextStack = std::vector<NodeCollisionContext>();
        auto usedContextsStack = std::vector<NodeCollisionContext*>{};
        std::for_each(nodeQueue.begin(), nodeQueue.end() - nodeQueueNum, [&](auto const& nodeData) {
          {
            usedContextsStack.emplace_back(&nodeContextMap[nodeData.contextID]);
            auto parentID = nodeData.parentID;
            while (parentID != INVALID_INDEX)
            {
              auto const& parentData = nodeQueue[parentID];
              usedContextsStack.emplace_back(&nodeContextMap[parentData.contextID]);
              parentID = parentData.parentID;
            }

            for (auto it = usedContextsStack.rbegin(); it != usedContextsStack.rend(); ++it)
              nodeContextStack.emplace_back(std::move(*(*it)));
          }

          InsertCollidedEntitiesInsideNode(entities, nodeContextStack.back(), collidedEntitiesInParents, tolerance, collisionDetector);
          InsertCollidedEntitiesWithParents(entities, nodeContextStack, collidedEntitiesInParents, tolerance, collisionDetector);

          {
            auto i = 0;
            for (auto it = usedContextsStack.rbegin(); it != usedContextsStack.rend(); ++it, ++i)
              *(*it) = std::move(nodeContextStack[i]);
            usedContextsStack.clear();
            nodeContextStack.clear();
          }
        });

        auto const collisionNo =
          std::transform_reduce(taskContexts.begin(), taskContexts.end(), collidedEntitiesInParents.size(), std::plus{}, [](auto const& taskContext) {
            return taskContext.collidedEntities.size();
          });

        collidedEntities.reserve(collisionNo);
        collidedEntities.insert(collidedEntities.end(), collidedEntitiesInParents.begin(), collidedEntitiesInParents.end());
        for (auto const& taskContext : taskContexts)
          collidedEntities.insert(collidedEntities.end(), taskContext.collidedEntities.begin(), taskContext.collidedEntities.end());

        return collidedEntities;
      }
    }

  public:
    // Collision detection between the stored entities from bottom to top logic
    template<bool IS_PARALLEL_EXEC = false>
    std::vector<std::pair<EntityID, EntityID>> CollisionDetection(EntityContainerView entities, TFloatScalar tolerance = GA::BASE_TOLERANCE) const noexcept
    {
      return CollectCollidedEntities<IS_PARALLEL_EXEC>(entities, tolerance, std::nullopt);
    }

    // Collision detection between the stored entities from bottom to top logic
    template<bool IS_PARALLEL_EXEC = false>
    std::vector<std::pair<EntityID, EntityID>> CollisionDetection(
      EntityContainerView entities, FCollisionDetector&& collisionDetector, TFloatScalar tolerance = GA::BASE_TOLERANCE) const noexcept
    {
      return CollectCollidedEntities<IS_PARALLEL_EXEC>(entities, tolerance, collisionDetector);
    }

  public:
    // Get all entities that are intersected by the ray in order
    template<bool SHOULD_SORT_ENTITIES_BY_DISTANCE = true>
    std::vector<EntityID> RayIntersectedAll(
      TVector const& rayBasePoint,
      TVector const& rayHeading,
      EntityContainerView entities,
      TFloatScalar tolerance = GA::BASE_TOLERANCE,
      TFloatScalar toleranceIncrement = {},
      TScalar maxExaminationDistance = std::numeric_limits<TScalar>::max(),
      std::optional<std::function<std::optional<TScalar>(EntityID)>> entityRayHitTester = std::nullopt) const noexcept
    {
      assert(
        toleranceIncrement <= std::max<TFloatScalar>(tolerance * 10, TFloatScalar(0.1)) &&
        "toleranceIncrement seems to be large. Probably maxExaminationDistance is used.");

      const auto rayHitTester = IGM::RayHitTester::Make(rayBasePoint, rayHeading, tolerance, toleranceIncrement);
      if (!rayHitTester)
        return {};

      struct ItemDistance
      {
        IGM_Geometry Distance;
        auto operator<=>(ItemDistance const& rhs) const = default;
      };

      struct EntityDistance : ItemDistance
      {
        EntityID EntityID;
        auto operator<=>(EntityDistance const& rhs) const { return ItemDistance::operator<=>(rhs); }
      };

      using EntityDistanceContainer = std::conditional_t<SHOULD_SORT_ENTITIES_BY_DISTANCE, std::vector<EntityDistance>, std::vector<EntityID>>;

      auto foundEntities = EntityDistanceContainer{};
      foundEntities.reserve(20);

      TraverseNodesDepthFirst([&](auto const pNodeValue) {
        auto const nodeHit = rayHitTester->Hit(Core::GetNodeMinPoint(pNodeValue), Core::GetNodeSize(pNodeValue));
        if (!nodeHit)
          return TraverseControl::SkipChildren;

        for (auto const entityID : Core::GetNodeEntities(pNodeValue))
        {
          auto const entityDistance = rayHitTester->Hit(EA::GetGeometry(entities, entityID));
          if (entityDistance && (maxExaminationDistance == 0 || entityDistance->enterDistance <= maxExaminationDistance))
          {
            auto closestEntityDistance = entityDistance->enterDistance;
            if (entityRayHitTester)
            {
              const auto result = (*entityRayHitTester)(entityID);
              if (!result)
                continue;

              closestEntityDistance = *result;
            }

            if constexpr (SHOULD_SORT_ENTITIES_BY_DISTANCE)
              detail::insert(foundEntities, EntityDistance{ { closestEntityDistance }, entityID });
            else
              detail::insert(foundEntities, entityID);
          }
        }

        return TraverseControl::Continue;
      });

      if constexpr (!SHOULD_SORT_ENTITIES_BY_DISTANCE)
      {
        return foundEntities;
      }
      else
      {
        std::ranges::sort(foundEntities);

        auto foundEntityIDs = std::vector<EntityID>(foundEntities.size());
        std::ranges::transform(foundEntities, foundEntityIDs.begin(), [](auto const& entityDistance) { return entityDistance.EntityID; });
        return foundEntityIDs;
      }
    }

    // Get all box which is intersected by the ray in order
    std::vector<EntityID> RayIntersectedAll(
      TRay const& ray,
      EntityContainerView entities,
      TFloatScalar tolerance = GA::BASE_TOLERANCE,
      TFloatScalar toleranceIncrement = {},
      TScalar maxExaminationDistance = std::numeric_limits<TScalar>::max()) const noexcept
    {
      return RayIntersectedAll(GA::GetRayOrigin(ray), GA::GetRayDirection(ray), entities, tolerance, toleranceIncrement, maxExaminationDistance);
    }

    // Get first entities that hit by the ray
    std::vector<EntityID> RayIntersectedFirst(
      TVector const& rayBasePoint,
      TVector const& rayHeading,
      EntityContainerView entities,
      TFloatScalar tolerance = GA::BASE_TOLERANCE,
      TFloatScalar toleranceIncrement = {},
      TScalar maxDistance = std::numeric_limits<TScalar>::max(),
      std::optional<std::function<std::optional<TFloatScalar>(EntityID)>> entityRayHitTester = std::nullopt) const noexcept
    {
      const auto rayHitTester = IGM::RayHitTester::Make(rayBasePoint, rayHeading, tolerance, toleranceIncrement);
      if (!rayHitTester)
        return {};

      struct Candidate
      {
        EntityID entityID;
        TFloatScalar enterDistance;

        constexpr auto operator<=>(Candidate const& other) const noexcept { return enterDistance <=> other.enterDistance; }
      };

      const auto appliedTolerance = tolerance == 0 ? std::numeric_limits<TFloatScalar>::epsilon() : tolerance;

      // max-heap by enterDistance (largest enterDistance at top)
      auto maxDistanceHeap = std::priority_queue<Candidate, std::vector<Candidate>>{};
      auto maxExaminationDistance = TFloatScalar(maxDistance);
      TraverseNodesByPriority(
        [&, rayHitTester = *rayHitTester](const auto pNodeValue, TFloatScalar nodeEnterDistance) {
          if (nodeEnterDistance > maxExaminationDistance)
            return TraverseControl::Terminate;

          for (auto const entityID : Core::GetNodeEntities(pNodeValue))
          {
            auto pickDomain = rayHitTester.Hit(EA::GetGeometry(entities, entityID));
            if (!pickDomain)
              continue;

            if (pickDomain->enterDistance > maxExaminationDistance)
              continue;

            // furthest possible hit distance
            auto& [closestHitDistance, furthestPossibleHitDistance] = *pickDomain;
            if (entityRayHitTester)
            {
              auto const result = (*entityRayHitTester)(entityID);
              if (!result)
                continue;

              assert(*result <= furthestPossibleHitDistance && "entityRayHitTester returned out of box result.");
              assert(*result >= closestHitDistance && "entityRayHitTester returned out of box result.");
              closestHitDistance = furthestPossibleHitDistance = *result;
            }

            // push candidate
            maxDistanceHeap.push(Candidate{ entityID, closestHitDistance });

            // update bestExitDistance if this entity's exit is closer
            if (furthestPossibleHitDistance >= maxExaminationDistance)
              continue;

            maxExaminationDistance = (TScalar(1) + toleranceIncrement) * furthestPossibleHitDistance + appliedTolerance;

            // drop any candidates whose enterDistance > new maxExaminationDistance
            while (!maxDistanceHeap.empty() && maxDistanceHeap.top().enterDistance > maxExaminationDistance)
              maxDistanceHeap.pop();
          }

          return TraverseControl::Continue;
        },
        [&, rayHitTester = *rayHitTester](auto const pNodeValue) -> std::optional<TFloatScalar> {
          auto result = rayHitTester.Hit(Core::GetNodeMinPoint(pNodeValue), Core::GetNodeSize(pNodeValue));
          if (!result)
            return std::nullopt;

          return result->enterDistance;
        });

      std::vector<EntityID> resulEntityIDs;
      resulEntityIDs.reserve(maxDistanceHeap.size());
      for (; !maxDistanceHeap.empty(); maxDistanceHeap.pop())
        resulEntityIDs.push_back(maxDistanceHeap.top().entityID);

      return resulEntityIDs;
    }

    // Get first entities that hit by the ray
    std::vector<EntityID> RayIntersectedFirst(
      TRay const& ray,
      EntityContainerView entities,
      TFloatScalar tolerance = GA::BASE_TOLERANCE,
      TFloatScalar toleranceIncrement = {},
      TScalar maxDistance = std::numeric_limits<TScalar>::max(),
      std::optional<std::function<std::optional<TScalar>(EntityID)>> entityHitTester = std::nullopt) const noexcept
    {
      return RayIntersectedFirst(GA::GetRayOrigin(ray), GA::GetRayDirection(ray), entities, tolerance, toleranceIncrement, maxDistance, entityHitTester);
    }
  }; // namespace OrthoTree

  template<typename TEntityAdapter, typename TGeometryAdapter, typename TConfiguration>
  using DynamicOrthoTreeBase = OrthoTreeQueryBase<DynamicHashOrthoTreeCore<TEntityAdapter, TGeometryAdapter, TConfiguration>>;

  template<typename TEntityAdapter, typename TGeometryAdapter, typename TConfiguration>
  using StaticOrthoTreeBase = OrthoTreeQueryBase<StaticLinearOrthoTreeCore<TEntityAdapter, TGeometryAdapter, TConfiguration>>;

  template<typename TEntityAdapter, typename TGeometryAdapter, typename TConfiguration>
  using OrthoTreeBase = DynamicOrthoTreeBase<TEntityAdapter, TGeometryAdapter, TConfiguration>;

} // namespace OrthoTree
#include "adapters/general.h"
#include "octree_container.h"

#include "detail/undefs.h"
