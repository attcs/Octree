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

Node center is not stored within the nodes. It will be calculated ad-hoc every time when it is required, e.g in search algorithm.
#define ORTHOTREE__DISABLED_NODECENTER

Node size is not stored within the nodes. It will be calculated ad-hoc every time when it is required, e.g in search algorithm.
#define ORTHOTREE__DISABLED_NODESIZE

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
#include <vector>
#include <version>

#include "detail/bitset_arithmetic.h"
#include "detail/common.h"
#include "detail/grid_space_indexing.h"
#include "detail/inplace_vector.h"
#include "detail/internal_geometry_module.h"
#include "detail/memory_resource.h"
#include "detail/morton_space_indexing.h"
#include "detail/partitioning.h"
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

#ifndef ORTHOTREE__DISABLED_NODECENTER
      Geometry m_center;
#endif
    public:
      explicit constexpr OrthoTreeNodeData() noexcept = default;

#ifndef ORTHOTREE__DISABLED_NODECENTER
      constexpr Geometry const& GetCenter() const noexcept { return m_center; }
      constexpr void SetCenter(Geometry&& center) noexcept { m_center = std::move(center); }
#endif // !ORTHOTREE__DISABLED_NODECENTER

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
          shouldOverwrite = elementID < m_childIndex.size() - 1 && m_childIndex[elementID] == childID;
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
          elementID = std::popcount(ChildFlags(m_childIndex & childMask)) + 1;
          m_childIndex &= ~childFlag;
        }

        m_children.erase(m_children.begin() + elementID);
      }

      constexpr bool IsAnyChildExist() const noexcept { return !m_children.empty(); }

      constexpr auto const& GetChildren() const noexcept { return m_children; }
    };
  } // namespace detail

  namespace ExecutionTags
  {
    // Sequential execution tag
    struct Sequential
    {};

    // Parallel execution tag
    struct Parallel
    {};
  } // namespace ExecutionTags

  auto constexpr SEQ_EXEC = ExecutionTags::Sequential{};
  auto constexpr PAR_EXEC = ExecutionTags::Parallel{};

  enum class GeometryType
  {
    Point,
    Box,
    // Mixed // Not supported yet
  };

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

  enum class NodeGeometryStorage
  {
    None,
    Center,
    Box,
    MBR,
  };
  template<bool IS_LOOSE_TREE_ = false, bool USE_MBR_ = false>
  struct Configuration
  {
    // Geometry type cannot be mixed within the same octree.
    static constexpr bool IS_HOMOGENEOUS_GEOMETRY = true;

    // If true, loose octree will be used (nodes are allowed to overlap each other). Otherwise, strict octree will be used.
    static constexpr bool IS_LOOSE_TREE = IS_LOOSE_TREE_;

    // If IS_LOOSE_TREE == true, this looseness factor will be applied.
    static constexpr double LOOSE_FACTOR = 2.0;

    // If true, out-of-handled space element will be stored in the root node. Otherwise, insertion will fail.
    static constexpr bool ALLOW_OUT_OF_SPACE_INSERTION = true;

    // If tree, Reverse mapping (entityID -> nodeID) will be stored to speed up removal and update operations. (Dynamic datasets)
    static constexpr bool USE_REVERSE_MAPPING = false;

    // TODO: rethink Location
    // It determines the internal location data type sizes.
    // In 3D, 21: (maximum allowed depth for 3D), LocationID is an uint64_t, Location's size is over 64bit. Resolution: for 1km model-space is 0.5mm
    // In 3D, 19: Location's size is 57bit. Resolution: for 1km model-space is 20mm
    // In 3D, 10: LocationID is an uint32_t, Location's size is over 32bit. Resolution: for 1km model-space is 1m
    // In 3D, 8: LocationID is an uint32_t, Location's size is 32bit. Resolution: for 1km model-space is 4m
    static constexpr depth_t MAX_ALLOWED_DEPTH_ID = depth_t{ 19 };

    // static constexpr bool USE_PMR = true;

    static constexpr NodeGeometryStorage NODE_GEOMETRY_STORAGE = USE_MBR_ ? NodeGeometryStorage::MBR : NodeGeometryStorage::Center;

    // Target number of elements in nodes
    static constexpr std::size_t DEFAULT_TARGET_ELEMENT_NUM_IN_NODES = 20; // TODO: set to 16

    // Associative container used for node storage (default: std::unordered_map)
    template<typename TKey, typename TValue, typename THash>
    using LinearNodeContainer = std::unordered_map<TKey, TValue, THash>;

    // Associative container used for reverse mapping storage (default: std::unordered_map)
    template<typename TKey, typename TValue, typename THash>
    using ReverseMap = std::unordered_map<TKey, TValue, THash>;
  };

  using PointConfiguration = Configuration<false, false>;

  template<bool IS_LOOSE_TREE>
  using BoxConfiguration = Configuration<IS_LOOSE_TREE, false>;

  // OrthoTree: Non-owning Base container which spatially organize data ids in N dimension space into a hash-table by Morton Z order.
  template<typename TEntityAdapter, typename TGeometryAdapter, typename TConfiguration>
  class OrthoTreeBase
  {
  public:
    using EA = TEntityAdapter;
    using GA = TGeometryAdapter;
    using CONFIG = Configuration<true, false>; // TConfiguration;

    static constexpr dim_t DIMENSION_NO = GA::DIMENSION_NO;
    using TScalar = typename GA::Scalar;
    using TFloatScalar = typename GA::FloatScalar;
    using TVector = typename GA::Vector;
    using TBox = typename GA::Box;
    using TRay = typename GA::Ray;
    using TPlane = typename GA::Plane;

    using EntityContainerView = EA::EntityContainerView;
    using EntityID = EA::EntityID;

    static_assert(GeometryAdapterConcept<GA, DIMENSION_NO, TVector, TBox, TRay, TPlane, TScalar, TFloatScalar>);
    static_assert(0 < DIMENSION_NO && DIMENSION_NO < 64);

    static_assert(std::is_trivially_copyable_v<EntityID>, "Only trivially copyable EntityID types are supported!");
    static_assert(CONFIG::IS_HOMOGENEOUS_GEOMETRY, "Mixed geometry types are not supported yet!");
    static_assert(EA::GEOMETRY_TYPE == GeometryType::Point || EA::GEOMETRY_TYPE == GeometryType::Box, "Entity geometry type is not supported!");
    static_assert(CONFIG::MAX_ALLOWED_DEPTH_ID <= MAX_DEPTH_ID, "MAX_ALLOWED_DEPTH_ID of Configuration is too large.");

    template<typename T>
    using DimArray = std::array<T, DIMENSION_NO>;
    using IGM = typename detail::InternalGeometryModule<GA>;
    using IGM_Geometry = typename IGM::Geometry;

    using SI = detail::MortonSpaceIndexing<DIMENSION_NO>;
    using MortonNodeID = typename SI::NodeID;
    using MortonNodeIDCR = typename SI::NodeIDCR;
    using MortonLocationID = typename SI::LocationID;
    using MortonLocationIDCR = typename SI::LocationIDCR;
    using MortonChildID = typename SI::ChildID;


    using Node = detail::OrthoTreeNodeData<SI::CHILD_NO, MortonNodeID, MortonChildID, EntityID, typename IGM::Vector>;
    using NodeValue = std::pair<MortonNodeID const, Node>;

  protected: // Aid struct to partitioning and distance ordering
    struct ItemDistance
    {
      IGM_Geometry Distance;
      auto operator<=>(ItemDistance const& rhs) const = default;
    };

    struct EntityDistance : ItemDistance
    {
      EntityID EntityID;
      auto operator<=>(EntityDistance const& rhs) const = default;
    };

    struct BoxDistance : ItemDistance
    {
      MortonNodeID NodeKey;
      Node const* NodePtr;
    };

#ifdef ORTHOTREE_IS_PMR_USED
    template<typename TData>
    using LinearNodeContainer = std::pmr::unordered_map<MortonNodeID, TData>;

    template<typename TData>
    using NonLinearNodeContainer = std::pmr::map<MortonNodeID, TData, bitset_arithmetic_compare>;
#else
    template<typename TData>
    using LinearNodeContainer = typename CONFIG::LinearNodeContainer<MortonNodeID, TData, std::hash<MortonNodeID>>;

    template<typename TData>
    using NonLinearNodeContainer = std::map<MortonNodeID, TData, bitset_arithmetic_compare>;
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
    // TODO: add element indexing in nodes
    using ReverseMapType = typename CONFIG::ReverseMap<EntityID, MortonNodeID, typename EA::Hash>;
    using ReverseMap = std::conditional_t<CONFIG::USE_REVERSE_MAPPING, ReverseMapType, std::monostate>;
    ReverseMap m_reverseMap;

    detail::MemoryResource<EntityID> m_memoryResource;

    std::size_t m_maxElementNo = CONFIG::DEFAULT_TARGET_ELEMENT_NUM_IN_NODES;
    depth_t m_maxDepthID = {};

    std::vector<typename IGM::Vector> m_nodeLooseSizes;
    std::vector<typename IGM::Vector> m_nodeNominalSizes;

    detail::GridSpaceIndexing<GA> m_grid;

  public: // Constructors
    // Default constructor. Requires InitBase call before usage.
    constexpr OrthoTreeBase() = default;

    OrthoTreeBase(OrthoTreeBase&&) = default;

    OrthoTreeBase(OrthoTreeBase const& other)
#ifdef ORTHOTREE_IS_PMR_USED
    : m_umrNodes()
    , m_nodes(&m_umrNodes)
#else
    : m_nodes(other.m_nodes)
#endif
    , m_maxElementNo(other.m_maxElementNo)
    , m_maxDepthID(other.m_maxDepthID)
    , m_nodeNominalSizes(other.m_nodeNominalSizes)
    , m_nodeLooseSizes(other.m_nodeLooseSizes)
    , m_grid(other.m_grid)
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

    OrthoTreeBase& operator=(OrthoTreeBase const& other)
    {
      m_maxElementNo = other.m_maxElementNo;
      m_maxDepthID = other.m_maxDepthID;
      m_nodeNominalSizes = other.m_nodeNominalSizes;
      m_nodeLooseSizes = other.m_nodeLooseSizes;
      m_grid = other.m_grid;
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

    // Initialize the base octree structure
    constexpr OrthoTreeBase(
      TBox const& boxSpace,
      depth_t maxDepthID,
      std::size_t maxElementNo = CONFIG::DEFAULT_TARGET_ELEMENT_NUM_IN_NODES,
      std::size_t estimatedEntityNo = detail::MemoryResource<EntityID>::DEFAULT_PAGE_SIZE) noexcept
    {
      this->InitBase(IGM::GetBoxAD(boxSpace), maxDepthID, maxElementNo, estimatedEntityNo);
    }

    // Initialize the base octree structure with entity collection
    explicit OrthoTreeBase(
      EntityContainerView entities,
      std::optional<depth_t> maxDepthIDIn = std::nullopt,
      std::optional<TBox> boxSpaceOptional = std::nullopt,
      std::size_t maxElementNoInNode = CONFIG::DEFAULT_TARGET_ELEMENT_NUM_IN_NODES,
      bool isParallelExec = false) noexcept
    {
      auto isSuccessfullyInsertedAllElements = false;
      if (isParallelExec)
        isSuccessfullyInsertedAllElements = this->template Create<true>(entities, maxDepthIDIn, std::move(boxSpaceOptional), maxElementNoInNode);
      else
        isSuccessfullyInsertedAllElements = this->template Create<false>(entities, maxDepthIDIn, std::move(boxSpaceOptional), maxElementNoInNode);

      assert(isSuccessfullyInsertedAllElements);
    }

    // Initialize the base octree structure with entity collection and parallel tree-building option
    template<typename EXEC_TAG>
    OrthoTreeBase(
      EXEC_TAG,
      EntityContainerView entities,
      std::optional<depth_t> maxDepthIDIn = std::nullopt,
      std::optional<TBox> boxSpaceOptional = std::nullopt,
      std::size_t maxElementNoInNode = CONFIG::DEFAULT_TARGET_ELEMENT_NUM_IN_NODES) noexcept
    {
      auto isSuccessfullyInsertedAllElements =
        this->template Create<std::is_same_v<EXEC_TAG, ExecutionTags::Parallel>>(entities, maxDepthIDIn, std::move(boxSpaceOptional), maxElementNoInNode);
      assert(isSuccessfullyInsertedAllElements);
    }

  private:
    using LocationIterator = typename detail::zip_view<std::vector<typename SI::Location>, std::span<EntityID>>::iterator;

    struct NodeProcessingData
    {
      std::pair<MortonNodeID, Node> NodeInstance;
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
      if (subtreeEntityNo > this->m_maxElementNo && depthID < this->m_maxDepthID)
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

      nodeProcessingData.NodeInstance.second = this->CreateChild(parentNodeProcessingData.NodeInstance.second, childKey);
      nodeProcessingData.NodeInstance.first = std::move(childKey);
    }

    // Build the tree in depth-first order
    template<bool ARE_LOCATIONS_SORTED, typename TResulEntityContainerView>
    constexpr void BuildSubtree(
      LocationIterator const& rootBeginLocationIt,
      LocationIterator const& rootEndLocationIt,
      std::pair<MortonNodeID, Node> const& rootNode,
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

        if (isEntitesProcessed || depthID == this->m_maxDepthID)
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
      typename IGM::Box boxSpace;
      if (boxSpaceOptional)
        boxSpace = IGM::GetBoxAD(*boxSpaceOptional);
      else
      {
        if constexpr (EA::GEOMETRY_TYPE == GeometryType::Point)
        {
          boxSpace = IGM::GetBoxOfPointsAD(entities);
        }
        else if constexpr (EA::GEOMETRY_TYPE == GeometryType::Box)
        {
          boxSpace = IGM::GetBoxOfBoxesAD(entities);
        }
        else
        {
          static_assert(false, "Unsupported geometry type!");
        }
      }

      auto const entityNo = entities.size();
      auto const maxDepthID = (!maxDepthIn || maxDepthIn == depth_t{}) ? EstimateMaxDepth(entityNo, maxElementNoInNode) : *maxDepthIn;
      this->InitBase(boxSpace, maxDepthID, maxElementNoInNode, entityNo);

      if (entityNo == 0)
        return true;

      auto mortonIDs = std::vector<typename SI::Location>(entityNo);
      auto entityIDsView = this->m_memoryResource.Allocate(entityNo).segment;

      auto locationsZip = detail::zip_view(mortonIDs, entityIDsView);
      using Location = decltype(locationsZip)::iterator::value_type;

      EXEC_POL_DEF(epf); // GCC 11.3
      std::transform(EXEC_POL_ADD(epf) entities.begin(), entities.end(), locationsZip.begin(), [&](auto const& entity) -> Location {
        return { this->template GetLocation<ARE_ENTITIES_SURELY_IN_MODELSPACE>(EA::GetGeometry(entity)), EA::GetEntityID(entities, entity) };
      });

      // TODO: out-of-space handling //!ATT

      constexpr bool ARE_LOCATIONS_SORTED = IS_PARALLEL_EXEC;
      if constexpr (ARE_LOCATIONS_SORTED)
      {
        EXEC_POL_DEF(eps); // GCC 11.3
        std::sort(EXEC_POL_ADD(eps) locationsZip.begin(), locationsZip.end(), [](Location const& l, Location const& r) {
          return SI::IsLess(l.first, r.first);
        });
      }

      auto const rootNode = *this->m_nodes.begin();
      this->m_nodes.clear();
      detail::reserve(this->m_nodes, EstimateNodeNumber(entityNo, maxDepthID, maxElementNoInNode));
      this->template BuildSubtree<ARE_LOCATIONS_SORTED>(locationsZip.begin(), locationsZip.end(), rootNode, this->m_nodes);
      return true;
    }

    // Create
    template<bool IS_PARALLEL_EXEC = false>
    static bool Create(
      OrthoTreeBase& tree,
      EntityContainerView entities,
      std::optional<depth_t> maxDepthIn = std::nullopt,
      std::optional<TBox> boxSpaceOptional = std::nullopt,
      std::size_t maxElementNoInNode = CONFIG::DEFAULT_TARGET_ELEMENT_NUM_IN_NODES) noexcept
    {
      return tree.Create<IS_PARALLEL_EXEC>(entities, maxDepthIn, std::move(boxSpaceOptional), maxElementNoInNode);
    }

  public: // Node helpers
          // Get EntityIDs of the node
    constexpr auto const& GetNodeEntities(Node const& node) const noexcept { return node.GetEntities(); }

    // Get EntityIDs of the node
    constexpr auto const& GetNodeEntities(MortonNodeIDCR nodeKey) const noexcept { return GetNodeEntities(GetNode(nodeKey)); }

    // Get EntityIDs number of the node
    constexpr std::size_t GetNodeEntitiesSize(Node const& node) const noexcept { return node.GetEntitiesSize(); }

    // Get EntityIDs number of the node
    constexpr std::size_t GetNodeEntitiesSize(MortonNodeIDCR nodeKey) const noexcept { return GetNodeEntitiesSize(GetNode(nodeKey)); }

    // Is the node has any entity
    constexpr bool IsNodeEntitiesEmpty(Node const& node) const noexcept { return node.IsEntitiesEmpty(); }

    // Is the node has any entity
    constexpr bool IsNodeEntitiesEmpty(MortonNodeIDCR nodeKey) const noexcept { return IsNodeEntitiesEmpty(GetNode(nodeKey)); }

    // Calculate extent by box of the tree and the nodeID of the node
    constexpr IGM::Vector CalculateNodeCenter(MortonNodeIDCR key) const noexcept
    {
      return m_grid.CalculateGridCellCenter(SI::Decode(key, m_maxDepthID), m_maxDepthID - SI::GetDepthID(key));
    }

    constexpr decltype(auto) GetNodeCenter(NodeValue const& nodeValue) const noexcept
    {
#ifdef ORTHOTREE__DISABLED_NODECENTER
      return CalculateNodeCenter(nodeValue.first);
#else
      return nodeValue.second.GetCenter();
#endif // ORTHOTREE__DISABLED_NODECENTER
    }

    // Obsolete
    constexpr decltype(auto) GetNodeCenter(MortonNodeIDCR nodeKey) const noexcept
    {
#ifdef ORTHOTREE__DISABLED_NODECENTER
      return CalculateNodeCenter(nodeKey);
#else
      return GetNode(nodeKey).GetCenter();
#endif // ORTHOTREE__DISABLED_NODECENTER
    }

    constexpr decltype(auto) GetNodeSize(depth_t depthID) const noexcept
    {
#ifdef ORTHOTREE__DISABLED_NODESIZE
      return CalculateNodeSize(depthID);
#else
      return m_nodeLooseSizes[depthID];
#endif // ORTHOTREE__DISABLED_NODESIZE
    }

    constexpr decltype(auto) GetNodeSizeByKey(MortonNodeIDCR key) const noexcept { return this->GetNodeSize(SI::GetDepthID(key)); }

    constexpr IGM::Box GetNodeBox(depth_t depthID, IGM::Vector const& center) const noexcept
    {
      auto const& halfSize = GetNodeSize(depthID + 1); // +1: half size will be required
      typename IGM::Box box{ .Min = center, .Max = center };

      ORTHOTREE_LOOPIVDEP
      for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
      {
        box.Min[dimensionID] -= halfSize[dimensionID];
        box.Max[dimensionID] += halfSize[dimensionID];
      }

      return box;
    }

    constexpr IGM::Box GetNodeBox(MortonNodeIDCR key) const noexcept { return this->GetNodeBox(SI::GetDepthID(key), this->GetNodeCenter(key)); }

  protected:
    constexpr void AddNodeEntity(Node& node, EntityID newEntity) noexcept
    {
      m_memoryResource.IncreaseSegment(node.GetEntitySegment(), 1);
      node.GetEntities().back() = std::move(newEntity);
    }

    constexpr bool RemoveNodeEntity(Node& node, EntityID entity) noexcept
    {
      auto const isRemoved = node.RemoveEntity(entity);
      if (isRemoved)
        m_memoryResource.DecreaseSegment(node.GetEntitySegment(), 1);

      return isRemoved;
    }

    constexpr void ResizeNodeEntities(Node& node, std::size_t size) noexcept
    {
      auto& ms = node.GetEntitySegment();
      m_memoryResource.DecreaseSegment(ms, ms.segment.size() - size);
    }

    constexpr Node CreateChild([[maybe_unused]] Node const& parentNode, MortonNodeIDCR childKey) const noexcept
    {
#ifdef ORTHOTREE__DISABLED_NODECENTER
      return Node();
#else
      auto nodeChild = Node();

      auto const depthID = SI::GetDepthID(childKey);
      auto const& halfSizes = m_nodeNominalSizes[depthID + 1];
      auto const& parentCenter = parentNode.GetCenter();

      typename IGM::Vector childCenter;
      ORTHOTREE_LOOPIVDEP
      for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
      {
        auto const isGreater = SI::IsChildInGreaterSegment(childKey, dimensionID);
        auto const sign = IGM_Geometry(isGreater * 2 - 1);
        childCenter[dimensionID] = parentCenter[dimensionID] + sign * halfSizes[dimensionID];
      }

      nodeChild.SetCenter(std::move(childCenter));
      return nodeChild;
#endif // ORTHOTREE__DISABLED_NODECENTER
    }

    template<bool HANDLE_OUT_OF_TREE_GEOMETRY = false>
    constexpr SI::Location GetLocation(TVector const& point) const noexcept
    {
      // TODO: handle out-of-tree geometry
      return SI::GetLocation(SI::Encode(this->m_grid.template GetPointGridID<HANDLE_OUT_OF_TREE_GEOMETRY>(point)), this->m_maxDepthID);
    }

    template<bool HANDLE_OUT_OF_TREE_GEOMETRY = false>
    constexpr SI::Location GetLocation(TBox const& box) const noexcept
    {
      // TODO: handle out-of-tree geometry

      if constexpr (CONFIG::IS_LOOSE_TREE)
      {
        auto const [centerGridID, levelID] = this->m_grid.template GetLooseBoxGridData<CONFIG::LOOSE_FACTOR, HANDLE_OUT_OF_TREE_GEOMETRY>(box);
        return SI::GetLocation(SI::Encode(centerGridID), m_maxDepthID - levelID);
      }
      else
      {
        return SI::GetRangeLocation(this->m_grid.template GetBoxGridID<HANDLE_OUT_OF_TREE_GEOMETRY>(box), this->m_maxDepthID);
      }
    }

  public:
    template<bool HANDLE_OUT_OF_TREE_GEOMETRY = false>
    MortonNodeID GetNodeID(TVector const& point) const noexcept
    {
      auto locationID = SI::Encode(this->m_grid.template GetPointGridID<HANDLE_OUT_OF_TREE_GEOMETRY>(point));
      return SI::GetNodeID(locationID, m_maxDepthID);
    }

    template<bool HANDLE_OUT_OF_TREE_GEOMETRY = false>
    MortonNodeID GetNodeID(TBox const& box) const noexcept
    {
      return SI::GetNodeID(this->GetLocation<HANDLE_OUT_OF_TREE_GEOMETRY>(box), m_maxDepthID);
    }

    constexpr depth_t GetExaminationLevelID(depth_t depth) const { return m_maxDepthID - depth; }

  private:
    auto FindParentNodeIt(MortonNodeIDCR nodeID)
    {

      for (auto parentNodeID = SI::GetParentKey(nodeID); SI::IsValidKey(parentNodeID); parentNodeID = SI::GetParentKey(parentNodeID))
      {
        auto parentNodeIt = m_nodes.find(parentNodeID);
        if (parentNodeIt == m_nodes.end())
          continue;

        return parentNodeIt;
      }

      assert(false);
      return m_nodes.find(SI::GetRootKey());
    }

    void RemoveNodeIfPossible(auto nodeIt) noexcept
    {
      while (true)
      {
        auto& [nodeID, node] = *nodeIt;
        if (nodeID == SI::GetRootKey())
          return;

        if (node.IsAnyChildExist() || !IsNodeEntitiesEmpty(node))
          return;

        m_memoryResource.Deallocate(node.GetEntitySegment());
        auto parentNodeIt = FindParentNodeIt(nodeID);

        auto& [parentNodeID, parentNode] = *parentNodeIt;
        parentNode.RemoveChild(SI::GetChildID2(parentNodeID, nodeID));
        m_nodes.erase(nodeIt);

        nodeIt = parentNodeIt;
      }
    }

  protected:
    bool IsGeometryInTree(EA::Geometry const& entityGeometry) const noexcept
    {
      if constexpr (EA::GEOMETRY_TYPE == GeometryType::Point)
      {
        if (!IGM::DoesBoxContainPointAD(m_grid.GetBoxSpace(), entityGeometry))
          return false;
      }
      else if constexpr (EA::GEOMETRY_TYPE == GeometryType::Box)
      {
        if (!IGM::DoesRangeContainBoxAD(m_grid.GetBoxSpace(), entityGeometry))
          return false;
      }
      else
      {
        static_assert(false, "Unsupported geometry type!");
      }

      return false;
    }

  public:
    void BulkInsertV1(EntityContainerView entities, auto EXEC_TAG = SEQ_EXEC) noexcept
    {
      constexpr bool IS_PARALLEL_EXEC = std::is_same_v<std::remove_cvref_t<decltype(EXEC_TAG)>, ExecutionTags::Parallel>;

      auto const entityNo = entities.size();

      auto mortonIDs = std::vector<typename SI::Location>(entityNo);
      auto mainMemorySegment = this->m_memoryResource.Allocate(entityNo);
      auto locationsZip = detail::zip_view(mortonIDs, mainMemorySegment.segment);
      detail::reserve(m_nodes, EstimateNodeNumber(entityNo, m_maxDepthID, m_maxElementNo));


      using EntityLocation = decltype(locationsZip)::iterator::value_type;
      EXEC_POL_DEF(ept); // GCC 11.3
      std::transform(EXEC_POL_ADD(ept) entities.begin(), entities.end(), locationsZip.begin(), [&](auto const& entity) -> EntityLocation {
        return { this->GetLocation(EA::GetGeometry(entity)), EA::GetEntityID(entities, entity) };
      });

      auto const partitions = Partitioning::Partition<std::min(dim_t(9), DIMENSION_NO * 3), IS_PARALLEL_EXEC>(
        locationsZip.begin(),
        locationsZip.end(),
        [](EntityLocation const& e) -> MortonNodeID { return e.first.locationID; },
        m_maxElementNo,
        m_maxDepthID* DIMENSION_NO);

      auto orphanNodes = std::vector<MortonNodeID>{};
      auto partitionIt = partitions.begin();
      for (auto beginIt = locationsZip.begin(); beginIt != locationsZip.end();)
      {
        /*
        auto const possibleEndIt = beginIt + std::min(detail::size(beginIt, locationsZip.end()), m_maxElementNo);
        partitionIt =
          std::partition_point(partitionIt, partitions.end(), [possibleEndIt](auto partitionEndIt) { return partitionEndIt <= possibleEndIt; });

        auto endIt = partitionIt == partitions.begin() ? *partitions.begin() : *std::prev(partitionIt);
        if (endIt == beginIt)
          endIt = *partitionIt;
       */
        auto endIt = *partitionIt++;
        // auto const [minIt, maxIt] =
        //   std::minmax_element(beginIt, endIt, [](EntityLocation const& l, EntityLocation const& r) { return SI::Less(l.first, r.first); });
        auto const [minIt, maxIt] = std::minmax_element(beginIt.GetFirst(), endIt.GetFirst(), SI::IsLess);
        auto const minNodeID = SI::GetNodeID(*minIt, m_maxDepthID);
        auto const maxNodeID = SI::GetNodeID(*maxIt, m_maxDepthID);

        auto nodeID = SI::GetLowestCommonAncestor(minNodeID, maxNodeID);
        auto [it, isInserted] = this->m_nodes.try_emplace(nodeID);
        if (isInserted)
        {
          orphanNodes.push_back(nodeID);
          it->second.SetCenter(this->CalculateNodeCenter(nodeID));
        }

        it->second.ReplaceEntities(std::span(beginIt.GetSecond(), endIt.GetSecond()));
        beginIt = endIt;
      }


      for (std::size_t i = 0; i < orphanNodes.size(); ++i)
      {
        auto const orphanNodeID = orphanNodes[i];
        auto& [parentNodeID, parentNode] = *this->GetParentNode(orphanNodeID);
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
            auto& lcaNode = lcaIt->second;
            lcaNode.SetCenter(this->CalculateNodeCenter(lcaNodeID));
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
      detail::reserve(m_nodes, EstimateNodeNumber(entityNo, m_maxDepthID, m_maxElementNo));


      using EntityLocation = decltype(locationsZip)::iterator::value_type;
      EXEC_POL_DEF(ept); // GCC 11.3
      std::transform(EXEC_POL_ADD(ept) entities.begin(), entities.end(), locationsZip.begin(), [&](auto const& entity) -> EntityLocation {
        return { this->GetLocation(EA::GetGeometry(entity)), EA::GetEntityID(entities, entity) };
      });

      auto orphanNodes = std::vector<MortonNodeID>{};
      auto const maxElementNo = static_cast<uint32_t>(m_maxElementNo);
      Partitioning::DepthFirstPartition<DIMENSION_NO, EA::GEOMETRY_TYPE != GeometryType::Point, typename SI::Location>(
        EXEC_TAG,
        locationsZip.begin(),
        locationsZip.end(),
        SI::GetRootLocation(),
        m_maxDepthID,
        maxElementNo,
        [&](auto beginIt, auto endIt, auto location, bool isForcedToFinish) {
          auto const elementNum = detail::size(beginIt, endIt);
          if (elementNum == 0)
            return true;

          if (!isForcedToFinish && elementNum > m_maxElementNo && location.depthID < m_maxDepthID)
            return false;

          auto nodeID = SI::GetNodeID(location, m_maxDepthID);
          auto [it, isInserted] = this->m_nodes.try_emplace(nodeID);
          if (isInserted)
          {
            orphanNodes.push_back(nodeID);
            it->second.SetCenter(this->CalculateNodeCenter(nodeID));
          }

          it->second.ReplaceEntities(std::span(beginIt.GetSecond(), endIt.GetSecond()));
          return true;
        });

      for (std::size_t i = 0; i < orphanNodes.size(); ++i)
      {
        auto const orphanNodeID = orphanNodes[i];
        auto& [parentNodeID, parentNode] = *this->GetParentNode(orphanNodeID);
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
            auto& lcaNode = lcaIt->second;
            lcaNode.SetCenter(this->CalculateNodeCenter(lcaNodeID));
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
      detail::reserve(m_nodes, EstimateNodeNumber(entityNo, m_maxDepthID, m_maxElementNo));

      auto orphanNodes = std::vector<MortonNodeID>{};
      for (auto const& entity : entities)
      {
        auto nodeID = this->GetNodeID(EA::GetGeometry(entity));
        auto [it, isInserted] = this->m_nodes.try_emplace(nodeID);
        if (isInserted)
        {
          orphanNodes.push_back(nodeID);
          it->second.SetCenter(this->CalculateNodeCenter(nodeID));
        }

        this->AddNodeEntity(it->second, EA::GetEntityID(entities, entity));
      }

      for (std::size_t i = 0; i < orphanNodes.size(); ++i)
      {
        auto const orphanNodeID = orphanNodes[i];
        auto& [parentNodeID, parentNode] = *this->GetParentNode(orphanNodeID);
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
          auto& lcaNode = lcaIt->second;
          lcaNode.SetCenter(this->CalculateNodeCenter(lcaNodeID));
          lcaNode.AddChild(SI::GetChildID2(lcaNodeID, childNodeID), childNodeID);
          lcaNode.AddChild(SI::GetChildID2(lcaNodeID, orphanNodeID), orphanNodeID);
        }
      }
    }

    // Erase id, aided with the original geometry
    bool Erase(EntityID entitiyID, EA::Geometry const& entityGeometry) noexcept
    {
      auto nodeID = this->FindSmallestNode(entityGeometry);
      if (!SI::IsValidKey(nodeID))
        return false; // old box is not in the handled space domain

      for (; SI::IsValidKey(nodeID); nodeID = SI::GetParentKey(nodeID))
      {
        auto nodeIt = this->m_nodes.find(nodeID);
        if (nodeIt == this->m_nodes.end())
          continue;

        bool const isEntityRemoved = this->RemoveNodeEntity(nodeIt->second, entitiyID);
        if (isEntityRemoved)
        {
          this->RemoveNodeIfPossible(nodeIt);
          break;
        }
      }

      if (!SI::IsValidKey(nodeID))
        return false; // id was not registered previously.

      if constexpr (EA::REQUIRES_CONTIGUOUS_ENTITY_IDS)
      {
        for (auto& [key, node] : this->m_nodes)
          node.DecreaseEntityIDs(entitiyID);
      }


      return true;
    }

    bool IsEveryEntityUnique() const noexcept
    {
      auto ids = std::vector<EntityID>();
      ids.reserve(100);
      std::for_each(m_nodes.begin(), m_nodes.end(), [&](auto& node) {
        auto const& entities = this->GetNodeEntities(node.second);
        ids.insert(ids.end(), entities.begin(), entities.end());
      });

      auto const idsSizeBeforeUnique = ids.size();
      detail::sortAndUnique(ids);
      return idsSizeBeforeUnique == ids.size();
    }

    template<bool DO_UNIQUENESS_CHECK_TO_INDICIES>
    bool InsertWithRebalancingBase(
      MortonNodeIDCR parentNodeKey, depth_t parentDepthID, SI::Location const& newEntityLocation, EntityID newEntityID, EntityContainerView entities) noexcept
    {
      // TODO: handle out-of-tree geometry
      // TODO: reverse mapping

      enum class ControlFlow
      {
        InsertInParentNode,
        ShouldCreateOnlyOneChild,
        FullRebalancing,
      };


      // If newEntityNodeKey is not the equal to the parentNodeKey, it is not exists.
      auto const newEntityNodeKey = SI::GetNodeID(newEntityLocation, this->m_maxDepthID);
      auto const shouldInsertInParentNode = newEntityNodeKey == parentNodeKey;

      auto& parentNode = this->m_nodes.at(parentNodeKey);
      auto const cf = [&] {
        if (parentDepthID == this->m_maxDepthID)
          return ControlFlow::InsertInParentNode;
        else if (parentNode.IsAnyChildExist() && !shouldInsertInParentNode) // !shouldInsertInParentNode means the entity's node not exist
          return ControlFlow::ShouldCreateOnlyOneChild; // If possible, entity should be placed in a leaf node, therefore if the parent node is not a leaf, a new child should be created.
        else if (this->GetNodeEntitiesSize(parentNode) + 1 >= this->m_maxElementNo)
          return ControlFlow::FullRebalancing;
        else
          return ControlFlow::InsertInParentNode;
      }();

      switch (cf)
      {
      case ControlFlow::ShouldCreateOnlyOneChild: {
        auto const childGenerator = typename SI::ChildKeyGenerator(parentNodeKey);
        auto const childID = SI::GetChildID(newEntityLocation.locationID, m_maxDepthID - parentDepthID);
        assert(childID < SI::CHILD_NO);
        auto const childNodeKey = childGenerator.GetChildNodeKey(childID);

        // TODO: FINISH
        assert(false);
        parentNode.AddChild(childID, childNodeKey);
        auto [childNode, _] = this->m_nodes.emplace(childNodeKey, this->CreateChild(parentNode, childNodeKey));
        this->AddNodeEntity(childNode->second, newEntityID);

        break;
      }

      case ControlFlow::FullRebalancing: {
        auto const childGenerator = typename SI::ChildKeyGenerator(parentNodeKey);
        this->AddNodeEntity(parentNode, newEntityID);
        auto& parentEntities = this->GetNodeEntities(parentNode); // Box entities could be stuck in the parent node.
        auto parentEntitiesNo = parentEntities.size();
        for (std::size_t i = 0; i < parentEntitiesNo; ++i)
        {
          auto const entityID = parentEntities[i];
          auto const entityLocation = this->GetLocation(EA::GetGeometry(entities, entityID));
          if (entityLocation.depthID <= parentDepthID) // entity is stucked
          {
            continue;
          }
          else // entity should be moved to a child node
          {
            auto const childID = SI::GetChildID(entityLocation.locationID, this->GetExaminationLevelID(parentDepthID));
            assert(childID < SI::CHILD_NO);
            if (parentNode.HasChild(childID))
            {
              // ShouldCreateOnlyOneChild supposes if newEntityNodeKey == parentNodeKey then parentNodeKey does not exist, therefore we need to find
              // the smallest smallestChildNodeKey which may not have the relevant child.
              auto const entityNodeKey = SI::GetNodeID(entityLocation, this->m_maxDepthID);
              auto const& [smallestChildNodeKey, smallestChildDepthID] = this->FindSmallestNodeKeyWithDepth(entityNodeKey);

              this->template InsertWithRebalancingBase<false>(smallestChildNodeKey, smallestChildDepthID, entityLocation, entityID, entities);
            }
            else
            {
              auto const childNodeKey = childGenerator.GetChildNodeKey(childID);
              // TODO: finish!
              assert(false);
              parentNode.AddChild(childID, -1);
              auto [childNode, _] = this->m_nodes.emplace(childNodeKey, this->CreateChild(parentNode, childNodeKey));
              this->AddNodeEntity(childNode->second, entityID);
            }
          }

          --parentEntitiesNo;
          parentEntities[i] = std::move(parentEntities[parentEntitiesNo]);
          --i;
        }

        this->ResizeNodeEntities(parentNode, parentEntitiesNo);
        break;
      }

      case ControlFlow::InsertInParentNode: {
        this->AddNodeEntity(parentNode, newEntityID);
        break;
      }
      }

      if constexpr (DO_UNIQUENESS_CHECK_TO_INDICIES)
        assert(this->IsEveryEntityUnique()); // Assert means: index is already added. Wrong input!

      return true;
    }

    template<bool DO_UNIQUENESS_CHECK_TO_INDICIES>
    bool InsertWithoutRebalancingBase(MortonNodeIDCR existingParentNodeKey, MortonNodeIDCR entityNodeKey, EntityID entityID, bool doInsertToLeaf) noexcept
    {
      // TODO: handle out-of-tree geometry
      // TODO: reverse mapping

      if (entityNodeKey == existingParentNodeKey)
      {
        this->AddNodeEntity(detail::at(this->m_nodes, entityNodeKey), entityID);
        if constexpr (DO_UNIQUENESS_CHECK_TO_INDICIES)
          assert(this->IsEveryEntityUnique()); // Assert means: index is already added. Wrong input!
        return true;
      }

      if (doInsertToLeaf)
      {
        auto nonExistingNodeStack = std::stack<MortonNodeID, std::vector<MortonNodeID>>{};
        auto parentNodeKey = entityNodeKey;
        for (; parentNodeKey != existingParentNodeKey; parentNodeKey = SI::GetParentKey(nonExistingNodeStack.top()))
        {
          if (this->m_nodes.contains(parentNodeKey))
            break;

          nonExistingNodeStack.push(parentNodeKey);
        }

        auto parentNodeIt = this->m_nodes.find(parentNodeKey);
        for (; !nonExistingNodeStack.empty(); nonExistingNodeStack.pop())
        {
          MortonNodeIDCR newParentNodeKey = nonExistingNodeStack.top();

          [[maybe_unused]] bool isSuccessful = false;
          parentNodeIt->second.AddChild(SI::GetChildID(newParentNodeKey), newParentNodeKey);
          std::tie(parentNodeIt, isSuccessful) = this->m_nodes.emplace(newParentNodeKey, this->CreateChild(parentNodeIt->second, newParentNodeKey));
          assert(isSuccessful);
        }
        this->AddNodeEntity(parentNodeIt->second, entityID);
      }
      else
      {
        auto& parentNode = detail::at(this->m_nodes, existingParentNodeKey);
        if (parentNode.IsAnyChildExist())
        {
          auto const parentDepth = SI::GetDepthID(existingParentNodeKey);
          auto const childID = SI::GetChildIDByDepth(parentDepth, SI::GetDepthID(entityNodeKey), entityNodeKey);
          auto const childGenerator = typename SI::ChildKeyGenerator(existingParentNodeKey);
          auto const childNodeKey = childGenerator.GetChildNodeKey(childID);

          parentNode.AddChild(childID, entityNodeKey);
          auto [childNode, _] = this->m_nodes.emplace(childNodeKey, this->CreateChild(parentNode, childNodeKey));
          this->AddNodeEntity(childNode->second, entityID);
        }
        else
          this->AddNodeEntity(parentNode, entityID);
      }

      if constexpr (DO_UNIQUENESS_CHECK_TO_INDICIES)
        assert(this->IsEveryEntityUnique()); // Assert means: index is already added. Wrong input!

      return true;
    }

    // Insert entity into the tree. If doInsertToLeaf is true: The smallest node will be chosen by the max depth. If doInsertToLeaf is false: The smallest existing level on the branch will be chosen.
    bool Insert(EntityID entityID, EA::Geometry const& entityGeometry, bool doInsertToLeaf = false) noexcept
    {
      // TODO: reverse mapping

      if (!IsGeometryInTree(entityGeometry))
      {
        if constexpr (CONFIG::ALLOW_OUT_OF_SPACE_INSERTION)
        {
          AddNodeEntity(m_nodes.at(SI::GetRootKey()), entityID);
          return true;
        }
        else
        {
          return false;
        }
      }

      auto const entityNodeKey = GetNodeID(entityGeometry);
      auto const smallestNodeKey = FindSmallestNodeKey(entityNodeKey);
      if (!SI::IsValidKey(smallestNodeKey))
        return false;

      return InsertWithoutRebalancingBase<true>(smallestNodeKey, entityNodeKey, entityID, doInsertToLeaf);
    }

    // Insert entity into the tree with node rebalancing
    bool InsertWithRebalancing(EntityID entityID, EA::Geometry const& entityGeometry, EntityContainerView entities) noexcept
    {
      if (!IsGeometryInTree(entityGeometry))
      {
        if constexpr (CONFIG::ALLOW_OUT_OF_SPACE_INSERTION)
        {
          AddNodeEntity(m_nodes.at(SI::GetRootKey()), entityID);
          return true;
        }
        else
        {
          return false;
        }
      }

      auto const entityLocation = this->GetLocation(entityGeometry);
      auto const entityNodeKey = SI::GetNodeID(entityLocation, m_maxDepthID);
      auto const [parentNodeNodeID, parentDepthID] = this->FindSmallestNodeKeyWithDepth(entityNodeKey);
      if (!SI::IsValidKey(parentNodeNodeID))
        return false;

      return this->template InsertWithRebalancingBase<true>(parentNodeNodeID, parentDepthID, entityLocation, entityID, entities);
    }

    // Insert entity into the tree, if there is no entity within the same location by tolerance.
    bool InsertUnique(
      EntityID entityID,
      EA::Geometry const& entityGeometry,
      EntityContainerView entities,
      TFloatScalar tolerance = GA::BASE_TOLERANCE,
      bool doInsertToLeaf = false)
    {
      // TODO: reverse mapping
      if (!IsGeometryInTree(entityGeometry))
      {
        if constexpr (CONFIG::ALLOW_OUT_OF_SPACE_INSERTION)
        {
          auto const nearestEntityList = GetNearestNeighbors(entityGeometry, 1, 0.0, entities, tolerance);
          if (!nearestEntityList.empty())
            return false;

          AddNodeEntity(m_nodes.at(SI::GetRootKey()), entityID);
          return true;
        }
        else
        {
          return false;
        }
      }

      auto const location = GetLocation(entityGeometry);
      auto const nodeID = SI::GetNodeID(location, m_maxDepthID);
      auto const [parentNodeKey, parentDepthID] = FindSmallestNodeKeyWithDepth(nodeID);
      if (!SI::IsValidKey(parentNodeKey))
        return false;

      auto const nearestEntityList = GetNearestNeighbors(entityGeometry, 1, 0.0, entities, tolerance);
      if (!nearestEntityList.empty())
        return false;

      if (doInsertToLeaf)
        return InsertWithoutRebalancingBase<true>(parentNodeKey, nodeID, entityID, true);
      else
        return InsertWithRebalancingBase<true>(parentNodeKey, parentDepthID, location, entityID, entities);
    }


    // Update id by the new bounding box information
    bool Update(EntityID entityID, EA::Geometry const& newEntityGeometry, bool doInsertToLeaf = false) noexcept
    {
      if (!EraseEntity(entityID))
        return false;

      return Insert(entityID, newEntityGeometry, doInsertToLeaf);
    }

    // Update id by the new bounding box information and the erase part is aided by the old bounding box geometry data
    bool Update(EntityID entityID, EA::Geometry const& oldEntityGeometry, EA::Geometry const& newEntityGeometry, bool doInsertToLeaf = false) noexcept
    {
      if constexpr (CONFIG::USE_REVERSE_MAPPING)
        return Update(entityID, newEntityGeometry, doInsertToLeaf);

      auto oldEntityNodeID = SI::GetRootKey();
      if (!IsGeometryInTree(oldEntityGeometry))
      {
        if constexpr (!CONFIG::ALLOW_OUT_OF_SPACE_INSERTION)
          return false;
      }
      else
      {
        oldEntityNodeID = FindSmallestNode(oldEntityGeometry);
      }

      auto const isNewGeometryInTree = IsGeometryInTree(newEntityGeometry);
      if (isNewGeometryInTree)
      {
        if (oldEntityNodeID == FindSmallestNode(newEntityGeometry))
          return true; // No update is required.
      }

      if (!Erase(entityID, oldEntityGeometry))
        return false; // entityID was not registered previously.

      if (!isNewGeometryInTree)
      {
        if constexpr (!CONFIG::ALLOW_OUT_OF_SPACE_INSERTION)
          return false;
      }

      return Insert(entityID, newEntityGeometry, doInsertToLeaf);
    }


    // Update id with rebalancing by the new bounding box information
    bool Update(EntityID entityID, EA::Geometry const& newEntityGeometry, EntityContainerView entities) noexcept
    {
      if (!EraseEntity(entityID))
        return false;

      return InsertWithRebalancing(entityID, newEntityGeometry, entities);
    }

    // Update id with rebalancing by the new bounding box information and the erase part is aided by the old bounding box geometry data
    bool Update(EntityID entityID, EA::Geometry const& oldEntityGeometry, EA::Geometry const& newEntityGeometry, EntityContainerView const& entities) noexcept
    {
      if constexpr (CONFIG::USE_REVERSE_MAPPING)
        return Update(entityID, newEntityGeometry, entities);

      auto oldEntityNodeID = SI::GetRootKey();
      if (IsGeometryInTree(oldEntityGeometry))
        oldEntityNodeID = FindSmallestNode(oldEntityGeometry);
      else
      {
        if constexpr (!CONFIG::ALLOW_OUT_OF_SPACE_INSERTION)
          return false;
      }

      auto const isNewGeometryInTree = IsGeometryInTree(newEntityGeometry);
      if (isNewGeometryInTree)
      {
        if (oldEntityNodeID == FindSmallestNode(newEntityGeometry))
          return true; // No update is required.
      }

      if (!Erase(entityID, oldEntityGeometry))
        return false; // entityID was not registered previously.

      if (!isNewGeometryInTree)
      {
        if constexpr (!CONFIG::ALLOW_OUT_OF_SPACE_INSERTION)
          return false;
      }

      return InsertWithRebalancing(entityID, newEntityGeometry, entities);
    }

  public: // Static aid functions
    static constexpr std::size_t EstimateNodeNumber(std::size_t elementNo, depth_t maxDepthID, std::size_t maxElementNo) noexcept
    {
      assert(maxElementNo > 0);
      assert(maxDepthID > 0);

      if (elementNo < 10)
        return 10;

      auto constexpr rMult = 1.5;
      constexpr depth_t bitSize = sizeof(std::size_t) * CHAR_BIT;
      if ((maxDepthID + 1) * DIMENSION_NO < bitSize)
      {
        auto const nMaxChild = detail::pow2(maxDepthID * DIMENSION_NO);
        auto const nElementInNode = elementNo / nMaxChild;
        if (nElementInNode > maxElementNo / 2)
          return nMaxChild;
      }

      auto const nElementInNodeAvg = static_cast<float>(elementNo) / static_cast<float>(maxElementNo);
      auto const nDepthEstimated = std::min(maxDepthID, static_cast<depth_t>(ceil((log2f(nElementInNodeAvg) + 1.0) / static_cast<float>(DIMENSION_NO))));
      if (nDepthEstimated * DIMENSION_NO < 64)
        return static_cast<std::size_t>(1.05 * detail::pow2(nDepthEstimated * std::min<depth_t>(6, DIMENSION_NO)));

      return static_cast<std::size_t>(rMult * nElementInNodeAvg);
    }

    static depth_t EstimateMaxDepth(std::size_t elementNo, std::size_t maxElementNo) noexcept
    {
      if (elementNo <= maxElementNo)
        return 2;

      auto const nLeaf = elementNo / maxElementNo;
      // nLeaf = (2^nDepth)^DIMENSION_NO
      return std::clamp(static_cast<depth_t>(std::log2(nLeaf) / static_cast<double>(DIMENSION_NO)), depth_t(2), SI::MAX_THEORETICAL_DEPTH_ID);
    }


  public: // Getters
    constexpr auto const& GetNodes() const noexcept { return m_nodes; }
    bool HasNode(MortonNodeIDCR key) const noexcept { return m_nodes.contains(key); }
    auto const& GetNode(MortonNodeIDCR key) const noexcept { return m_nodes.at(key); }
    constexpr auto const& GetBox() const noexcept { return m_grid.GetBoxSpace(); }
    constexpr auto GetMaxDepthID() const noexcept { return m_maxDepthID; }
    constexpr auto GetDepthNo() const noexcept { return m_maxDepthID + 1; }
    constexpr auto GetResolutionMax() const noexcept { return m_grid.GetResolution(); }
    constexpr auto GetNodeIDByEntity(EntityID entityID) const noexcept
    {
      auto const it = std::find_if(m_nodes.begin(), m_nodes.end(), [&](auto const& keyAndValue) { return keyAndValue.second.ContainsEntity(entityID); });

      return it == m_nodes.end() ? MortonNodeID{} : it->first;
    }

  protected:
    // Alternative creation mode (instead of Create), Init then Insert items into leafs one by one. NOT RECOMMENDED.
    constexpr void InitBase(IGM::Box const& boxSpace, depth_t maxDepthID, std::size_t maxElementNo, std::size_t estimatedEntityNo) noexcept
    {
      ORTHOTREE_CRASH_IF(
        !this->m_nodes.empty(), "To build/setup/create the tree, use the Create() [recommended] or Init() function. If an already built tree is wanted to be reset, use the Reset() function before Init().");
      ORTHOTREE_CRASH_IF(maxDepthID < 1, "maxDepthID must be largar than 0!");
      ORTHOTREE_CRASH_IF(maxDepthID > SI::MAX_THEORETICAL_DEPTH_ID, "maxDepthID is larger than the applicable with the current DIMENSION_NO!");
      ORTHOTREE_CRASH_IF(maxDepthID >= std::numeric_limits<uint8_t>::max(), "maxDepthID is too large.");
      ORTHOTREE_CRASH_IF(maxElementNo == 0, "maxElementNo must be larger than 0. It is allowed max entity number for one node.");
      ORTHOTREE_CRASH_IF(CHAR_BIT * sizeof(GridID) < maxDepthID, "GridID and maxDepthID are not compatible.");

      m_grid = detail::GridSpaceIndexing<GA>(maxDepthID, boxSpace);
      m_maxDepthID = maxDepthID;
      m_maxElementNo = maxElementNo;

#ifndef ORTHOTREE__DISABLED_NODECENTER
      auto& nodeRoot = m_nodes[SI::GetRootKey()];
      nodeRoot.SetCenter(IGM::GetBoxCenter(boxSpace));
#endif // !ORTHOTREE__DISABLED_NODECENTER

      // the 0-based depth size of the tree is m_maxDepthID+1, and a fictive childnode halfsize (+2) could be asked prematurely.
      depth_t constexpr additionalDepth = 3;
      auto const examinedDepthSize = this->m_maxDepthID + additionalDepth;
      m_nodeNominalSizes.resize(examinedDepthSize);
      m_nodeLooseSizes.resize(examinedDepthSize);
      m_nodeNominalSizes[0] = m_nodeLooseSizes[0] = m_grid.GetSizes();

      auto constexpr multiplier = IGM_Geometry(0.5);
      auto nominalfactor = IGM_Geometry(1.0);
      auto looseFactor = IGM_Geometry(CONFIG::LOOSE_FACTOR);

      for (depth_t depthID = 0; depthID < examinedDepthSize; ++depthID, nominalfactor *= multiplier, looseFactor *= multiplier)
        for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
        {
          m_nodeNominalSizes[depthID][dimensionID] = m_nodeNominalSizes[0][dimensionID] * nominalfactor;
          m_nodeLooseSizes[depthID][dimensionID] = m_nodeNominalSizes[0][dimensionID] * looseFactor;
        }
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
      this->InitBase(IGM::GetBoxAD(box), maxDepthID, maxElementNo, estimatedEntityNo);
    }

    using FProcedure = std::function<void(MortonNodeIDCR, Node const&)>;
    using FProcedureUnconditional = std::function<void(MortonNodeIDCR, Node const&, bool)>;
    using FSelector = std::function<bool(MortonNodeIDCR, Node const&)>;
    using FSelectorUnconditional = std::function<bool(MortonNodeIDCR, Node const&)>;

    // Visit nodes with special selection and procedure in breadth-first search order
    void VisitNodes(MortonNodeIDCR rootKey, FProcedure const& procedure, FSelector const& selector) const noexcept
    {
      auto nodeIDsToProceed = std::queue<MortonNodeID>();
      for (nodeIDsToProceed.push(rootKey); !nodeIDsToProceed.empty(); nodeIDsToProceed.pop())
      {
        auto const& key = nodeIDsToProceed.front();
        auto const& node = GetNode(key);
        if (!selector(key, node))
          continue;

        procedure(key, node);

        for (auto childKey : node.GetChildren())
          nodeIDsToProceed.push(childKey);
      }
    }


    // Visit nodes with special selection and procedure in breadth-first search order
    void VisitNodes(MortonNodeIDCR rootKey, FProcedure const& procedure) const noexcept
    {
      VisitNodes(rootKey, procedure, [](MortonNodeIDCR, Node const&) { return true; });
    }

    // TraverseControl is the result type of node-visitor functions' procedure.
    enum class TraverseControl
    {
      Terminate,    // Terminates the traverse
      SkipChildren, // Skips children nodes
      Continue      // Continues the traverse
    };

    // Visit nodes in depth first order
    constexpr void VisitNodesInDepthFirst(auto&& procedure, NodeValue const& rootValue) const noexcept
    {
      auto nodeStack = std::vector<NodeValue const*>{};
      nodeStack.reserve(64);
      nodeStack.push_back(&rootValue);

      while (!nodeStack.empty())
      {
        auto const* currentNodeValue = nodeStack.back();
        nodeStack.pop_back();

        auto const control = procedure(*currentNodeValue);
        switch (control)
        {
        case TraverseControl::Terminate: return;
        case TraverseControl::SkipChildren: continue;
        case TraverseControl::Continue: {
          auto const& [nodeID, node] = *currentNodeValue;
          for (auto const& childNodeID : node.GetChildren())
          {
            auto const childNodeValueIt = m_nodes.find(childNodeID);
            nodeStack.push_back(&*childNodeValueIt);
          }
          break;
        }
        }
      }
    }

    // Visit nodes in depth first order
    constexpr void VisitNodesInDepthFirst(auto&& procedure) const noexcept
    {
      VisitNodesInDepthFirst(std::forward<decltype(procedure)>(procedure), *m_nodes.find(SI::GetRootKey()));
    }

    constexpr void VisitNodesInPriorityOrder(auto&& procedure, auto&& priorityCalculator, NodeValue const& nodeValue) const noexcept
    {
      using TPriorityResult = std::invoke_result_t<decltype(priorityCalculator), Node>;
      using TPriority = std::conditional_t<detail::IsStdOptionalV<TPriorityResult>, typename TPriorityResult::value_type, TPriorityResult>;

      auto constexpr GetValue = [](TPriorityResult const& pr) noexcept -> TPriority {
        if constexpr (detail::IsStdOptionalV<TPriorityResult>)
          return *pr;
        else
          return pr;
      };

      struct PrioritizedNode
      {
        NodeValue const* pNodeValue;
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
      nodesToProceed.push({ &nodeValue, GetValue(nodePriority) });
      while (!nodesToProceed.empty())
      {
        auto const [pNodeValue, priority] = nodesToProceed.top();
        nodesToProceed.pop();

        auto const control = procedure(*pNodeValue, priority);
        switch (control)
        {
        case TraverseControl::Terminate: return;
        case TraverseControl::SkipChildren: continue;
        case TraverseControl::Continue: break;
        }

        auto const& node = pNodeValue->second;
        for (MortonNodeIDCR childNodeID : node.GetChildren())
        {
          auto childNodeIt = m_nodes.find(childNodeID);
          auto childNodePriority = priorityCalculator(*childNodeIt);

          if constexpr (detail::IsStdOptionalV<TPriorityResult>)
          {
            if (!childNodePriority)
              continue;
          }

          nodesToProceed.push({ &*childNodeIt, GetValue(childNodePriority) });
        }
      }
    }

    constexpr void VisitNodesInPriorityOrder(auto&& procedure, auto&& priorityCalculator) const noexcept
    {
      VisitNodesInPriorityOrder(
        std::forward<decltype(procedure)>(procedure), std::forward<decltype(priorityCalculator)>(priorityCalculator), *m_nodes.find(SI::GetRootKey()));
    }

    // Collect all item id, traversing the tree in breadth-first search order
    std::vector<EntityID> CollectAllEntitiesInBFS(MortonNodeIDCR rootKey = SI::GetRootKey(), bool shouldSortInsideNodes = false) const noexcept
    {
      auto entityIDs = std::vector<EntityID>();
      entityIDs.reserve(m_nodes.size() * std::max<std::size_t>(2, m_maxElementNo / 2));

      VisitNodes(rootKey, [&](MortonNodeIDCR, auto const& node) {
        auto const& entities = this->GetNodeEntities(node);
        auto const entityIDsSize = entityIDs.size();
        entityIDs.insert(entityIDs.end(), entities.begin(), entities.end());
        if (shouldSortInsideNodes)
          std::sort(entityIDs.begin() + entityIDsSize, entityIDs.end());
      });
      return entityIDs;
    }

  private:
    void CollectAllEntitiesInDFSRecursive(Node const& parentNode, std::vector<EntityID>& foundEntities, bool shouldSortInsideNodes) const noexcept
    {
      auto const& entities = this->GetNodeEntities(parentNode);
      auto const entityIDsSize = foundEntities.size();
      foundEntities.insert(foundEntities.end(), entities.begin(), entities.end());
      if (shouldSortInsideNodes)
        std::sort(foundEntities.begin() + entityIDsSize, foundEntities.end());

      for (MortonNodeIDCR childKey : parentNode.GetChildren())
        CollectAllEntitiesInDFSRecursive(this->GetNode(childKey), foundEntities, shouldSortInsideNodes);
    }

  public:
    // Collect all entity id, traversing the tree in depth-first search pre-order
    std::vector<EntityID> CollectAllEntitiesInDFS(MortonNodeIDCR parentKey = SI::GetRootKey(), bool shouldSortInsideNodes = false) const noexcept
    {
      auto entityIDs = std::vector<EntityID>{};
      CollectAllEntitiesInDFSRecursive(GetNode(parentKey), entityIDs, shouldSortInsideNodes);
      return entityIDs;
    }

    // Update all element which are in the given hash-table.
    template<bool IS_PARALLEL_EXEC = false, bool DO_UNIQUENESS_CHECK_TO_INDICIES = false>
    void UpdateIndexes(std::unordered_map<EntityID, std::optional<EntityID>> const& updateMap) noexcept
    {
      auto const updateMapEndIterator = updateMap.end();

      EXEC_POL_DEF(ep);
      std::for_each(EXEC_POL_ADD(ep) m_nodes.begin(), m_nodes.end(), [&](auto& node) {
        auto& entityIDs = node.second.GetEntities();
        auto entityNo = entityIDs.size();
        for (std::size_t i = 0; i < entityNo; ++i)
        {
          auto const it = updateMap.find(entityIDs[i]);
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
        }
        ResizeNodeEntities(node.second, entityNo);
      });

      if constexpr (DO_UNIQUENESS_CHECK_TO_INDICIES)
        assert(IsEveryEntityUnique()); // Assert means: index replacements causes that multiple object has the same id. Wrong input!
    }

    // Reset the tree
    void Reset() noexcept
    {
      m_nodes.clear();
      m_grid = {};
      m_memoryResource.Reset();
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
#ifndef ORTHOTREE__DISABLED_NODECENTER
      EXEC_POL_DEF(ep); // GCC 11.3
      std::for_each(EXEC_POL_ADD(ep) m_nodes.begin(), m_nodes.end(), [&moveVector](auto& pairKeyNode) {
        auto center = pairKeyNode.second.GetCenter();
        IGM::MoveAD(center, moveVector);
        pairKeyNode.second.SetCenter(std::move(center));
      });
#endif // !ORTHOTREE__DISABLED_NODECENTER
      m_grid.Move(moveVector);
    }

    std::tuple<MortonNodeID, depth_t> FindSmallestNodeKeyWithDepth(MortonNodeID searchKey) const noexcept
    {
      for (depth_t depthID = SI::GetDepthID(searchKey); SI::IsValidKey(searchKey); searchKey = SI::GetParentKey(searchKey), --depthID)
        if (this->m_nodes.contains(searchKey))
          return { searchKey, depthID };

      return {}; // Not found
    }

    auto GetParentNode(MortonNodeID nodeID)
    {
      auto const endIt = this->m_nodes.end();
      for (nodeID = SI::GetParentKey(nodeID); SI::IsValidKey(nodeID); nodeID = SI::GetParentKey(nodeID))
      {
        auto nodeIt = this->m_nodes.find(nodeID);
        if (nodeIt != endIt)
          return nodeIt;
      }

      return this->m_nodes.find(SI::GetRootKey());
    }

    MortonNodeID FindSmallestNodeKey(MortonNodeID searchKey) const noexcept
    {
      for (; SI::IsValidKey(searchKey); searchKey = SI::GetParentKey(searchKey))
        if (this->m_nodes.contains(searchKey))
          return searchKey;

      return MortonNodeID{}; // Not found
    }

    // Find smallest node which contains the box
    template<bool HANDLE_OUT_OF_TREE_GEOMETRY = false>
    MortonNodeID FindSmallestNode(TVector const& searchPoint) const noexcept
    {
      if constexpr (!HANDLE_OUT_OF_TREE_GEOMETRY)
      {
        if (!IGM::DoesBoxContainPointAD(this->m_grid.GetBoxSpace(), searchPoint))
          return MortonNodeID{};
      }
      return this->FindSmallestNodeKey(this->GetNodeID<HANDLE_OUT_OF_TREE_GEOMETRY>(searchPoint));
    }

    // Find smallest node which contains the box
    MortonNodeID FindSmallestNode(TBox const& box) const noexcept
    {
      if (!IGM::DoesRangeContainBoxAD(this->m_grid.GetBoxSpace(), box))
        return MortonNodeID{};

      return FindSmallestNodeKey(this->GetNodeID(box));
    }

    MortonNodeID Find(EntityID entityID) const noexcept { return GetNodeIDByEntity(entityID); }

    constexpr bool EraseEntity(EntityID entityID) noexcept
    {
      bool isErased = false;

      if constexpr (CONFIG::USE_REVERSE_MAPPING)
      {
        auto it = m_reverseMap.find(entityID);
        if (it == m_reverseMap.end())
          return false;

        auto& nodeIt = m_nodes.find(it->second);
        if (!RemoveNodeEntity(nodeIt->second, entityID))
          return false;

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

      if constexpr (EA::REQUIRES_CONTIGUOUS_ENTITY_IDS)
      {
        for (auto& [_, node] : this->m_nodes)
          node.DecreaseEntityIDs(entityID);
      }

      return true;
    }

  public: // Search functions
    // Pick search
    std::vector<EntityID> PickSearch(TVector const& pickPoint, EntityContainerView entities, TFloatScalar tolerance = GA::BASE_TOLERANCE) const noexcept
    {
      auto foundEntitiyIDs = std::vector<EntityID>();
      if (!IGM::DoesBoxContainPointAD(this->m_grid.GetBoxSpace(), pickPoint))
        return foundEntitiyIDs;

      foundEntitiyIDs.reserve(100);

      VisitNodesInDepthFirst([&](auto const& nodeValue) {
        auto const& [nodeID, node] = nodeValue;
        auto const& halfSize = GetNodeSize(SI::GetDepthID(nodeID) + 1);
        // TODO: use tolerance here also ?
        if (!IGM::DoesBoxContainPointAD(GetNodeCenter(nodeValue), halfSize, pickPoint))
          return TraverseControl::SkipChildren;

        auto const& nodeEntityIDs = this->GetNodeEntities(node);
        std::ranges::copy_if(nodeEntityIDs, std::back_inserter(foundEntitiyIDs), [&](auto const entityID) {
          if constexpr (EA::GEOMETRY_TYPE == GeometryType::Point)
          {
            return GA::ArePointsEqual(EA::GetGeometry(entities, entityID), pickPoint, tolerance);
          }
          else if constexpr (EA::GEOMETRY_TYPE == GeometryType::Box)
          {
            return GA::DoesBoxContainPoint(EA::GetGeometry(entities, entityID), pickPoint);
          }
          else
          {
            static_assert(false, "Unsupported geometry type!");
          }
          ORTHOTREE_UNREACHABLE();
        });

        return TraverseControl::Continue;
      });

      return foundEntitiyIDs;
    }

  public:
    template<bool DO_RANGE_MUST_FULLY_CONTAIN = false>
    std::vector<EntityID> RangeSearch(TBox const& range, EntityContainerView entities) const noexcept
    {
      auto foundEntities = std::vector<EntityID>{};

      auto const entityNo = entities.size();
      if (IGM::DoesRangeContainBoxAD(range, this->m_grid.GetBoxSpace()))
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

      auto const entityFilter = [&](auto const entityID) {
        if constexpr (EA::GEOMETRY_TYPE == GeometryType::Point)
        {
          return GA::DoesBoxContainPoint(range, EA::GetGeometry(entities, entityID));
        }
        else if constexpr (EA::GEOMETRY_TYPE == GeometryType::Box)
        {
          if constexpr (DO_RANGE_MUST_FULLY_CONTAIN)
            return GA::AreBoxesOverlapped(range, EA::GetGeometry(entities, entityID), DO_RANGE_MUST_FULLY_CONTAIN);
          else
            return GA::AreBoxesOverlappedStrict(range, EA::GetGeometry(entities, entityID));
        }
        else
        {
          static_assert(false, "Unsupported geometry type!");
        }
        ORTHOTREE_UNREACHABLE();
      };

      auto const searchRangeCenter = IGM::GetBoxCenterAD(range);
      auto const searchRangeHalfSize = IGM::GetBoxHalfSizeAD(range);
      VisitNodesInDepthFirst([&](auto const& nodeValue) {
        auto const& [nodeID, node] = nodeValue;
        auto const depthID = SI::GetDepthID(nodeID);
        auto const& nodeCenter = GetNodeCenter(nodeValue);
        auto const& nodeHalfSize = GetNodeSize(depthID);
        if (!IGM::AreBoxesOverlappingByCenter(searchRangeCenter, nodeCenter, searchRangeHalfSize, nodeHalfSize))
          return TraverseControl::SkipChildren;

        if (IGM::DoesRangeContainBoxAD(range, nodeCenter, nodeHalfSize))
        {
          VisitNodesInDepthFirst(
            [&](auto const& childNodeValue) {
              auto const& [_, childNode] = childNodeValue;
              std::ranges::copy(GetNodeEntities(childNode), std::back_inserter(foundEntities));
              return TraverseControl::Continue;
            },
            nodeValue);

          return TraverseControl::SkipChildren;
        }

        std::ranges::copy_if(GetNodeEntities(node), std::back_inserter(foundEntities), entityFilter);

        return TraverseControl::Continue;
      });

      return foundEntities;
    }

    static constexpr PlaneRelation GetEntityPlaneRelation(
      EA::Geometry const& entityGeometry, TScalar distanceOfOrigo, TVector const& planeNormal, TFloatScalar tolerance = GA::BASE_TOLERANCE) noexcept
    {
      if constexpr (EA::GEOMETRY_TYPE == GeometryType::Point)
      {
        return GA::GetPointPlaneRelation(entityGeometry, distanceOfOrigo, planeNormal, tolerance);
      }
      else if constexpr (EA::GEOMETRY_TYPE == GeometryType::Box)
      {
        return IGM::GetBoxPlaneRelationAD(IGM::GetBoxCenterAD(entityGeometry), IGM::GetBoxHalfSizeAD(entityGeometry), distanceOfOrigo, planeNormal, tolerance);
      }
      else
      {
        static_assert(false, "Unsupported geometry type for kNN!");
      }
      ORTHOTREE_UNREACHABLE();
    }

    // Hyperplane intersection
    std::vector<EntityID> PlaneSearch(
      TScalar distanceOfOrigo, TVector const& planeNormal, EntityContainerView entities, TFloatScalar tolerance = GA::BASE_TOLERANCE) const noexcept
    {
      assert(GA::IsNormalizedVector(planeNormal));

      auto results = std::vector<EntityID>{};
      VisitNodesInDepthFirst([&](auto const& nodeValue) {
        auto const& [nodeID, node] = nodeValue;
        auto const& halfSize = GetNodeSize(SI::GetDepthID(nodeID) + 1);
        if (IGM::GetBoxPlaneRelationAD(GetNodeCenter(nodeValue), halfSize, distanceOfOrigo, planeNormal, tolerance) != PlaneRelation::Hit)
          return TraverseControl::SkipChildren;

        for (auto const entityID : GetNodeEntities(node))
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
      VisitNodesInDepthFirst([&](auto const& nodeValue) {
        auto const& [nodeID, node] = nodeValue;
        auto const& halfSize = GetNodeSize(SI::GetDepthID(nodeID) + 1);
        auto const nodeRelation = IGM::GetBoxPlaneRelationAD(GetNodeCenter(nodeValue), halfSize, distanceOfOrigo, planeNormal, tolerance);

        switch (nodeRelation)
        {
        case PlaneRelation::Negative: return TraverseControl::SkipChildren;
        case PlaneRelation::Positive:
          VisitNodesInDepthFirst(
            [&](auto const& childNodeValue) {
              auto const& [_, childNode] = childNodeValue;
              std::ranges::copy(GetNodeEntities(childNode), std::back_inserter(results));
              return TraverseControl::Continue;
            },
            nodeValue);
          return TraverseControl::SkipChildren;

        case PlaneRelation::Hit:
          for (auto const entityID : GetNodeEntities(node))
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

      auto const selector = [&](auto const& nodeValue) -> bool {
        auto const& [nodeID, node] = nodeValue;
        auto const& halfSize = this->GetNodeSize(SI::GetDepthID(nodeID) + 1);
        auto const& center = this->GetNodeCenter(nodeValue);

        for (auto const& plane : boundaryPlanes)
        {
          auto const relation = IGM::GetBoxPlaneRelationAD(center, halfSize, GA::GetPlaneOrigoDistance(plane), GA::GetPlaneNormal(plane), tolerance);
          if (relation == PlaneRelation::Hit)
            return true;

          if (relation == PlaneRelation::Negative)
            return false;
        }
        return true;
      };

      auto const procedure = [&](auto const& nodeValue) {
        if (!selector(nodeValue))
          return TraverseControl::SkipChildren;

        auto const& [nodeID, node] = nodeValue;
        for (auto const entityID : this->GetNodeEntities(node))
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

      VisitNodesInDepthFirst(procedure);

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

    struct MinEntityDistance
    {
      EntityID entityID;
      TFloatScalar optimisticDistance;
      TFloatScalar pessimisticDistance;

      constexpr auto operator<=>(MinEntityDistance const& other) const noexcept { return optimisticDistance <=> other.optimisticDistance; }
    };

    static bool AreOverflownNearestNeighborsDroppable(
      std::size_t neighborNo, std::vector<MinEntityDistance> const& neighborEntities, TScalar farthestEntityDistance) noexcept
    {
      std::size_t tieCountOverK = neighborNo >= neighborEntities.size() ? 0 : (neighborEntities.size() - neighborNo + 1);
      if (tieCountOverK == 0)
        return false;

      if (farthestEntityDistance > neighborEntities[0].optimisticDistance)
        return false;

      std::size_t tieCount = 1;
      std::size_t nonTieCount = 0; // if the whole level is non-tie, all elements above can be excluded.
      std::size_t levelEnd = 3;
      std::size_t elementNoInLevel = 2;
      for (std::size_t index = 1; index < neighborEntities.size(); ++index)
      {
        auto const& e = neighborEntities[index];
        if (e.optimisticDistance >= farthestEntityDistance)
          ++tieCount;
        else
          ++nonTieCount;

        if (tieCount >= tieCountOverK)
          return false;

        if (index == levelEnd)
        {
          if (nonTieCount == elementNoInLevel)
            return true;

          nonTieCount = 0;
          elementNoInLevel = levelEnd + 1;
          levelEnd += elementNoInLevel;
        }
      }

      return true;
    }

    struct FarthestDistance
    {
      TFloatScalar lower;
      TFloatScalar upper;
    };

    static void AddEntityDistance(
      std::size_t neighborNo,
      TVector const& searchPoint,
      std::optional<EntityDistanceFn> const& entityDistanceFn,
      auto const& nodeEntityIDs,
      EntityContainerView entities,
      TFloatScalar tolerance,
      std::vector<MinEntityDistance>& neighborEntities,
      FarthestDistance& farthestEntityDistance) noexcept
    {
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

        auto const shouldHeapify = neighborEntities.size() == neighborNo - 1;
        neighborEntities.push_back({ entityID, pbd.min, pbd.minMax });
        if (neighborEntities.size() < neighborNo)
          continue;

        if (shouldHeapify)
        {
          std::make_heap(neighborEntities.begin(), neighborEntities.end());
        }
        else
        {
          std::push_heap(neighborEntities.begin(), neighborEntities.end());

          if (AreOverflownNearestNeighborsDroppable(neighborNo, neighborEntities, farthestEntityDistance.lower))
          {
            auto endIt = neighborEntities.end();
            while (neighborEntities.front().optimisticDistance >= farthestEntityDistance.lower)
            {
              std::pop_heap(neighborEntities.begin(), endIt);
              --endIt;
            }

            neighborEntities.resize(neighborNo);
          }
        }

        TFloatScalar maxMinMax;
        if constexpr (EA::GEOMETRY_TYPE == GeometryType::Box)
        {
          maxMinMax = std::max_element(
                        neighborEntities.begin(),
                        neighborEntities.begin() + std::min(neighborNo, neighborEntities.size()),
                        [](auto const& lhs, auto const& rhs) { return lhs.pessimisticDistance < rhs.pessimisticDistance; })
                        ->pessimisticDistance;
        }
        else
        {
          maxMinMax = neighborEntities.front().optimisticDistance;
        }

        auto const newFarthestEntityDistanceUpper = GetValueWithToleranceUpper(maxMinMax, tolerance);
        if (newFarthestEntityDistanceUpper < farthestEntityDistance.upper)
        {
          farthestEntityDistance.lower = GetValueWithToleranceLower(maxMinMax, tolerance);
          farthestEntityDistance.upper = newFarthestEntityDistanceUpper;
        }
      }
    }

    template<bool SHOULD_SORT_ENTITIES_BY_DISTANCE = true>
    static constexpr std::vector<EntityID> ConvertEntityDistanceToList(std::vector<MinEntityDistance>& neighborEntities, std::size_t neighborNo) noexcept
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

    constexpr IGM::Geometry GetNodeWallDistance(TVector const& searchPoint, NodeValue const& nodeValue, bool isInsideConsideredAsZero) const noexcept
    {
      auto const& centerPoint = this->GetNodeCenter(nodeValue);
      
      auto const& [key, _] = nodeValue;
      auto const depthID = SI::GetDepthID(key);
      auto const& halfSize = this->GetNodeSize(depthID + 1);
      return IGM::GetBoxWallDistanceAD(searchPoint, centerPoint, halfSize, isInsideConsideredAsZero);
    }

  public:
    // Get K Nearest Neighbor sorted by distance (point distance should be less than maxDistanceWithin, it is used as a Tolerance check). It may
    // results more element than neighborNo, if those are in equal distance (point-like) or possible hit (box-like).
    template<bool SHOULD_SORT_ENTITIES_BY_DISTANCE = true>
    std::vector<EntityID> GetNearestNeighbors(
      TVector const& searchPoint,
      std::size_t neighborNo,
      TScalar maxDistanceWithin,
      EntityContainerView entities,
      TFloatScalar tolerance = GA::BASE_TOLERANCE,
      std::optional<EntityDistanceFn> const& entityDistanceFn = std::nullopt) const noexcept
    {
      assert(neighborNo > 0 && "At least one neighbor must be requested!");

      auto neighborEntities = std::vector<MinEntityDistance>();
      neighborEntities.reserve(neighborNo + 4);

      auto smallestNodeKey = FindSmallestNodeKey(GetNodeID<true>(searchPoint));
      if (!SI::IsValidKey(smallestNodeKey))
        smallestNodeKey = SI::GetRootKey();

      // farthestEntityDistance already contains the numerical tolerance
      auto farthestEntityDistance =
        FarthestDistance{ {},
                          maxDistanceWithin == std::numeric_limits<TScalar>::max() ? std::numeric_limits<TScalar>::max()
                                                                                   : GetValueWithToleranceUpper(maxDistanceWithin, tolerance) };

      // Parent checks (in a usual case parents do not have entities)
      for (auto nodeKey = smallestNodeKey; SI::IsValidKey(nodeKey); nodeKey = SI::GetParentKey(nodeKey))
        AddEntityDistance(neighborNo, searchPoint, entityDistanceFn, GetNodeEntities(nodeKey), entities, tolerance, neighborEntities, farthestEntityDistance);

      VisitNodesInPriorityOrder(
        [&](auto const& nodeValue, TFloatScalar nodeDistance) -> TraverseControl {
          if (nodeDistance >= farthestEntityDistance.upper)
            return TraverseControl::Terminate;

          auto const& [nodeID, node] = nodeValue;

          // Skip already visited parent nodes
          if (smallestNodeKey == nodeID || SI::IsParentKey(smallestNodeKey, nodeID))
            return TraverseControl::Continue;

          AddEntityDistance(neighborNo, searchPoint, entityDistanceFn, GetNodeEntities(node), entities, tolerance, neighborEntities, farthestEntityDistance);

          return TraverseControl::Continue;
        },
        [&](auto const& nodeValue) -> std::optional<TFloatScalar> {
          auto wallDistance = GetNodeWallDistance(searchPoint, nodeValue, true);
          if (wallDistance >= farthestEntityDistance.upper)
            return std::nullopt;

          return wallDistance;
        });

      return ConvertEntityDistanceToList<SHOULD_SORT_ENTITIES_BY_DISTANCE>(neighborEntities, neighborNo);
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

    // Collision detection: Returns all overlapping entities from the source trees.
    static std::vector<std::pair<EntityID, EntityID>> CollisionDetection(
      OrthoTreeBase const& leftTree,
      EntityContainerView leftEntities,
      OrthoTreeBase const& rightTree,
      EntityContainerView rightEntities,
      TFloatScalar tolerance = GA::BASE_TOLERANCE,
      std::optional<FCollisionDetector> const& collisionDetector = std::nullopt) noexcept
    {
      using NodeIterator = typename NodeContainer<Node>::const_iterator;
      struct NodeIteratorAndStatus
      {
        NodeIterator Iterator;
        bool IsTraversed;
      };
      using ParentIteratorArray = std::array<NodeIteratorAndStatus, 2>;

      enum : bool
      {
        Left,
        Right
      };

      auto results = std::vector<std::pair<EntityID, EntityID>>{};
      results.reserve(leftEntities.size() / 10);

      auto constexpr rootKey = SI::GetRootKey();
      auto const trees = std::array{ &leftTree, &rightTree };

      auto entitiesInOrderCache = std::array<std::unordered_map<MortonNodeID, SweepAndPruneDatabase>, 2>{};
      auto const getOrCreateEntitiesInOrder = [&](bool side, NodeIterator const& it, EntityContainerView entities) -> std::vector<EntityID> const& {
        auto itKeyAndSPD = entitiesInOrderCache[side].find(it->first);
        if (itKeyAndSPD == entitiesInOrderCache[side].end())
        {
          bool isInserted = false;
          std::tie(itKeyAndSPD, isInserted) =
            entitiesInOrderCache[side].emplace(it->first, SweepAndPruneDatabase(entities, trees[side]->GetNodeEntities(it->second)));
        }

        return itKeyAndSPD->second.GetEntities();
      };

      [[maybe_unused]] auto const pLeftTree = &leftTree;
      [[maybe_unused]] auto const pRightTree = &rightTree;
      auto nodePairToProceed = std::queue<ParentIteratorArray>{};
      nodePairToProceed.push(
        {
          NodeIteratorAndStatus{  leftTree.m_nodes.find(rootKey), false },
          NodeIteratorAndStatus{ rightTree.m_nodes.find(rootKey), false }
      });
      for (; !nodePairToProceed.empty(); nodePairToProceed.pop())
      {
        auto const& parentNodePair = nodePairToProceed.front();

        // Check the current ascendant content

        auto const& leftEntitiesInOrder = getOrCreateEntitiesInOrder(Left, parentNodePair[Left].Iterator, leftEntities);
        auto const& rightEntitiesInOrder = getOrCreateEntitiesInOrder(Right, parentNodePair[Right].Iterator, rightEntities);

        auto const rightEntityNo = rightEntitiesInOrder.size();
        std::size_t iRightEntityBegin = 0;
        for (auto const lefEntityID : leftEntitiesInOrder)
        {
          auto const& leftEntityGeometry = EA::GetGeometry(leftEntities, lefEntityID);
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
                if (!collisionDetector || (*collisionDetector)(lefEntityID, righEntityID))
                  results.emplace_back(lefEntityID, righEntityID);
            }
            else if constexpr (EA::GEOMETRY_TYPE == GeometryType::Box)
            {
              if (GA::GetBoxMaxC(leftEntityGeometry, 0) < GA::GetBoxMinC(rightEntityGeometry, 0))
                break; // sweep and prune optimization

              if (GA::AreBoxesOverlapped(leftEntityGeometry, rightEntityGeometry, false, tolerance))
                if (!collisionDetector || (*collisionDetector)(lefEntityID, righEntityID))
                  results.emplace_back(lefEntityID, righEntityID);
            }
            else
            {
              static_assert(false, "Unsupported geometry type for collision detection!");
            }
          }
        }

        // Collect children
        auto childNodes = std::array<std::vector<NodeIteratorAndStatus>, 2>{};
        for (auto const sideID : { Left, Right })
        {
          auto const& [nodeIterator, fTraversed] = parentNodePair[sideID];
          if (fTraversed)
            continue;

          auto const& childIDs = nodeIterator->second.GetChildren();
          childNodes[sideID].resize(childIDs.size());
          std::transform(childIDs.begin(), childIDs.end(), childNodes[sideID].begin(), [&](MortonNodeIDCR childKey) -> NodeIteratorAndStatus {
            return { trees[sideID]->m_nodes.find(childKey), false };
          });
        }

        // Stop condition
        if (childNodes[Left].empty() && childNodes[Right].empty())
          continue;

        // Add parent if it has any element
        for (auto const sideID : { Left, Right })
          if (!trees[sideID]->IsNodeEntitiesEmpty(parentNodePair[sideID].Iterator->second))
            childNodes[sideID].push_back({ parentNodePair[sideID].Iterator, true });


        // Cartesian product of childNodes left and right
        for (auto const& leftChildNode : childNodes[Left])
          for (auto const& rightChildNode : childNodes[Right])
            if (!(leftChildNode.Iterator == parentNodePair[Left].Iterator && rightChildNode.Iterator == parentNodePair[Right].Iterator))
              if (IGM::AreBoxesOverlappingByCenter(
                    pLeftTree->GetNodeCenter(*leftChildNode.Iterator),
                    pRightTree->GetNodeCenter(*rightChildNode.Iterator),
                    leftTree.GetNodeSizeByKey(leftChildNode.Iterator->first),
                    rightTree.GetNodeSizeByKey(rightChildNode.Iterator->first)))
                nodePairToProceed.emplace(std::array{ leftChildNode, rightChildNode });
      }

      return results;
    }


    // Collision detection: Returns all overlapping boxes from the source trees.
    std::vector<std::pair<EntityID, EntityID>> CollisionDetection(
      EntityContainerView const& boxes,
      OrthoTreeBase const& otherTree,
      EntityContainerView const& otherBoxes,
      TFloatScalar tolerance = GA::BASE_TOLERANCE,
      std::optional<FCollisionDetector> const& collisionDetector = std::nullopt) const noexcept
    {
      return CollisionDetection(*this, boxes, otherTree, otherBoxes, tolerance, collisionDetector);
    }

  private:
    struct NodeCollisionContext
    {
      IGM::Vector Center;
      IGM::Box Box;
      std::vector<EntityID> EntityIDs;
    };

    constexpr void FillNodeCollisionContext(
      NodeValue const& nodeValue, depth_t depthID, NodeCollisionContext& nodeContext) const noexcept
    {
      auto const& [nodeID, node] = nodeValue;
      auto const& nodeEntities = this->GetNodeEntities(node);

      nodeContext.EntityIDs.clear();
      nodeContext.EntityIDs.assign(nodeEntities.begin(), nodeEntities.end());
      nodeContext.Center = this->GetNodeCenter(nodeValue);
      nodeContext.Box = this->GetNodeBox(depthID, nodeContext.Center);
    }

    constexpr void PrepareNodeCollisionContext(auto const& comparator, NodeCollisionContext& nodeContext) const noexcept
    {
      auto& entityIDs = nodeContext.EntityIDs;
      std::sort(entityIDs.begin(), entityIDs.end(), comparator);
    }

    constexpr void InsertCollidedEntitiesInsideNode(
      EntityContainerView entities,
      NodeCollisionContext const& context,
      std::vector<std::pair<EntityID, EntityID>>& collidedEntityPairs,
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
                collidedEntityPairs.emplace_back(entityIDI, entityIDJ);
          }
          else if constexpr (EA::GEOMETRY_TYPE == GeometryType::Box)
          {
            if (GA::GetBoxMaxC(entityBoxI, 0) < GA::GetBoxMinC(entityBoxJ, 0))
              break; // sweep and prune optimization

            if (GA::AreBoxesOverlappedStrict(entityBoxI, entityBoxJ))
              if (!collisionDetector || (*collisionDetector)(entityIDI, entityIDJ))
                collidedEntityPairs.emplace_back(entityIDI, entityIDJ);
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
      depth_t depthID,
      std::vector<NodeCollisionContext> const& nodeContextStack,
      std::vector<std::pair<EntityID, EntityID>>& collidedEntityPairs,
      TFloatScalar tolerance,
      std::optional<FCollisionDetector> const& collisionDetector) const noexcept
    {
      auto const& nodeContext = nodeContextStack[depthID];
      auto const& nodeCenter = nodeContext.Center;
      auto const& nodeSizes = this->GetNodeSize(depthID);
      auto const& entityIDs = nodeContext.EntityIDs;

      auto const entityNo = entityIDs.size();

      for (depth_t parentDepthID = 0; parentDepthID < depthID; ++parentDepthID)
      {
        auto const& [parentCenter, parentBox, parenEntityIDs] = nodeContextStack[parentDepthID];

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

          auto const parentEntityCenter = IGM::GetBoxCenterAD(parentEntityGeometry);
          auto const parentEntitySizes = IGM::GetBoxSizeAD(parentEntityGeometry);
          if (!IGM::AreBoxesOverlappingByCenter(nodeCenter, parentEntityCenter, nodeSizes, parentEntitySizes))
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
                  collidedEntityPairs.emplace_back(entityID, parenEntityID);
            }
            else if constexpr (EA::GEOMETRY_TYPE == GeometryType::Box)
            {
              if (GA::GetBoxMaxC(parentEntityGeometry, 0) < GA::GetBoxMinC(entityGeometry, 0))
                break; // sweep and prune optimization

              // TODO: Add tolerance-based box-box overlap check
              if (GA::AreBoxesOverlappedStrict(entityGeometry, parentEntityGeometry))
                if (!collisionDetector || (*collisionDetector)(entityID, parenEntityID))
                  collidedEntityPairs.emplace_back(entityID, parenEntityID);
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
      depth_t depthID,
      MortonNodeIDCR nodeKey,
      std::vector<NodeCollisionContext>& nodeContextStack,
      std::vector<std::pair<EntityID, EntityID>>& collidedEntityPairs,
      TFloatScalar tolerance,
      std::optional<FCollisionDetector> const& collisionDetector) const noexcept
    {
      auto const& nodeValue = *m_nodes.find(nodeKey);

      FillNodeCollisionContext(nodeValue, depthID, nodeContextStack[depthID]);
      PrepareNodeCollisionContext(comparator, nodeContextStack[depthID]);

      auto const childDepthID = depthID + 1;
      for (MortonLocationIDCR childKey : nodeValue.second.GetChildren())
        InsertCollidedEntitiesInSubtree(entities, comparator, childDepthID, childKey, nodeContextStack, collidedEntityPairs, tolerance, collisionDetector);

      InsertCollidedEntitiesInsideNode(entities, nodeContextStack[depthID], collidedEntityPairs, tolerance, collisionDetector);
      InsertCollidedEntitiesWithParents(entities, depthID, nodeContextStack, collidedEntityPairs, tolerance, collisionDetector);
    }

    // Collision detection between the stored entities from bottom to top logic
    template<bool IS_PARALLEL_EXEC = false>
    std::vector<std::pair<EntityID, EntityID>> CollectCollidedEntities(
      EntityContainerView entities,
      TFloatScalar tolerance = GA::BASE_TOLERANCE,
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
      auto collidedEntityPairs = std::vector<std::pair<EntityID, EntityID>>{};
      collidedEntityPairs.reserve(std::max<std::size_t>(100, entityNo / 10));
      if constexpr (!IS_PARALLEL_EXEC)
      {
        auto nodeContextStack = std::vector<NodeCollisionContext>(this->GetDepthNo());
        this->InsertCollidedEntitiesInSubtree(entities, comparator, 0, SI::GetRootKey(), nodeContextStack, collidedEntityPairs, tolerance, collisionDetector);
      }
      else
      {
        auto const nodeNo = this->m_nodes.size();
        auto const threadNo = std::size_t(std::thread::hardware_concurrency());
        auto const isSingleThreadMoreEffective = nodeNo < threadNo * 3;
        if (isSingleThreadMoreEffective)
        {
          auto nodeContextStack = std::vector<NodeCollisionContext>(this->GetDepthNo());
          this->InsertCollidedEntitiesInSubtree(entities, comparator, 0, SI::GetRootKey(), nodeContextStack, collidedEntityPairs, tolerance, collisionDetector);
        }
        else
        {
          using NodeIterator = typename NodeContainer<Node>::const_iterator;

          auto nodeQueue = std::vector<NodeIterator>{};
          nodeQueue.reserve(threadNo * 2);
          nodeQueue.emplace_back(this->m_nodes.find(SI::GetRootKey()));

          auto nodeContextMap = std::unordered_map<MortonNodeID, NodeCollisionContext>{};

          std::size_t nodeQueueNo = 1;
          for (std::size_t i = 0; 0 < nodeQueueNo && nodeQueueNo < threadNo - 2; --nodeQueueNo, ++i)
          {
            for (MortonLocationIDCR childKey : nodeQueue[i]->second.GetChildren())
            {
              nodeQueue.emplace_back(this->m_nodes.find(childKey));
              ++nodeQueueNo;
            }

            MortonNodeIDCR nodeKey = nodeQueue[i]->first;
            auto const depthID = SI::GetDepthID(nodeKey);
            auto& nodeContext = nodeContextMap[nodeKey];
            FillNodeCollisionContext(*nodeQueue[i], depthID, nodeContext);
            PrepareNodeCollisionContext(comparator, nodeContext);
          }

          if (nodeQueueNo == 0)
          {
            auto nodeContextStack = std::vector<NodeCollisionContext>(this->GetDepthNo());
            InsertCollidedEntitiesInSubtree(entities, comparator, 0, SI::GetRootKey(), nodeContextStack, collidedEntityPairs, tolerance, collisionDetector);
          }
          else
          {
            struct TaskContext
            {
              NodeIterator NodeIt;
              std::vector<std::pair<EntityID, EntityID>> CollidedEntityPairs;
            };

            auto const nodeQueueAllNo = nodeQueue.size();
            auto const nodeQueueBegin = nodeQueueAllNo - nodeQueueNo;
            auto taskContexts = std::vector<TaskContext>(nodeQueueNo);
            for (std::size_t taskID = 0; taskID < nodeQueueNo; ++taskID)
              taskContexts[taskID].NodeIt = nodeQueue[nodeQueueBegin + taskID];

            EXEC_POL_DEF(epcd); // GCC 11.3
            std::for_each(EXEC_POL_ADD(epcd) taskContexts.begin(), taskContexts.end(), [&](auto& taskContext) {
              auto const depthID = SI::GetDepthID(taskContext.NodeIt->first);
              auto parentDepthID = depthID;

              auto nodeContextStack = std::vector<NodeCollisionContext>(this->GetDepthNo());
              for (auto parentKey = SI::GetParentKey(taskContext.NodeIt->first); SI::IsValidKey(parentKey); parentKey = SI::GetParentKey(parentKey))
                nodeContextStack[--parentDepthID] = nodeContextMap.at(parentKey);

              InsertCollidedEntitiesInSubtree(
                entities, comparator, depthID, taskContext.NodeIt->first, nodeContextStack, taskContext.CollidedEntityPairs, tolerance, collisionDetector);
            });

            auto collidedEntityPairsInParents = std::vector<std::pair<EntityID, EntityID>>{};
            auto nodeContextStack = std::vector<NodeCollisionContext>();
            auto usedContextsStack = std::vector<NodeCollisionContext*>{};
            std::for_each(nodeQueue.begin(), nodeQueue.end() - nodeQueueNo, [&](auto& nodeIt) {
              {
                usedContextsStack.emplace_back(&nodeContextMap.at(nodeIt->first));
                for (auto parentKey = SI::GetParentKey(nodeIt->first); SI::IsValidKey(parentKey); parentKey = SI::GetParentKey(parentKey))
                  usedContextsStack.emplace_back(&nodeContextMap.at(parentKey));

                for (auto it = usedContextsStack.rbegin(); it != usedContextsStack.rend(); ++it)
                  nodeContextStack.emplace_back(std::move(*(*it)));
              }

              auto const depthID = depth_t(usedContextsStack.size()) - 1;
              InsertCollidedEntitiesInsideNode(entities, nodeContextStack[depthID], collidedEntityPairsInParents, tolerance, collisionDetector);
              InsertCollidedEntitiesWithParents(entities, depthID, nodeContextStack, collidedEntityPairsInParents, tolerance, collisionDetector);

              {
                auto i = 0;
                for (auto it = usedContextsStack.rbegin(); it != usedContextsStack.rend(); ++it, ++i)
                  *(*it) = std::move(nodeContextStack[i]);
                usedContextsStack.clear();
                nodeContextStack.clear();
              }
            });

            auto const collisionNo =
              std::transform_reduce(taskContexts.begin(), taskContexts.end(), collidedEntityPairsInParents.size(), std::plus{}, [](auto const& taskContext) {
                return taskContext.CollidedEntityPairs.size();
              });

            collidedEntityPairs.reserve(collisionNo);
            collidedEntityPairs.insert(collidedEntityPairs.end(), collidedEntityPairsInParents.begin(), collidedEntityPairsInParents.end());
            for (auto const& taskContext : taskContexts)
              collidedEntityPairs.insert(collidedEntityPairs.end(), taskContext.CollidedEntityPairs.begin(), taskContext.CollidedEntityPairs.end());
          }
        }
      }

      return collidedEntityPairs;
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
      assert(toleranceIncrement < tolerance * 10);
      const auto rayHitTester = IGM::RayHitTester::Make(rayBasePoint, rayHeading, tolerance, toleranceIncrement);
      if (!rayHitTester)
        return {};

      using EntityDistanceContainer = std::conditional_t<SHOULD_SORT_ENTITIES_BY_DISTANCE, std::vector<EntityDistance>, std::vector<EntityID>>;

      auto foundEntities = EntityDistanceContainer{};
      foundEntities.reserve(20);

      VisitNodesInDepthFirst([&](auto const& nodeValue) {
        auto const& [nodeID, node] = nodeValue;
        depth_t const depthID = SI::GetDepthID(nodeID);

        auto const nodeHit = rayHitTester->Hit(GetNodeCenter(nodeValue), GetNodeSize(depthID + 1));
        if (!nodeHit)
          return TraverseControl::SkipChildren;

        for (auto const entityID : GetNodeEntities(node))
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
            using ValueType = typename std::decay_t<decltype(foundEntities)>::value_type;
            if constexpr (std::is_same_v<ValueType, EntityID>)
              detail::insert(foundEntities, entityID);
            else
              detail::insert(foundEntities, EntityDistance{ { closestEntityDistance }, entityID });
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
      VisitNodesInPriorityOrder(
        [&, rayHitTester = *rayHitTester](const auto& nodeValue, TFloatScalar nodeEnterDistance) {
          if (nodeEnterDistance > maxExaminationDistance)
            return TraverseControl::Terminate;

          auto const& [nodeID, node] = nodeValue;
          for (auto const entityID : GetNodeEntities(node))
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
        [&, rayHitTester = *rayHitTester](auto const& nodeValue) -> std::optional<TFloatScalar> {
          auto const& [nodeID, node] = nodeValue;
          auto result = rayHitTester.Hit(GetNodeCenter(nodeValue), GetNodeSize(SI::GetDepthID(nodeID) + 1));
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

  public: // Search functions
    bool Contains(EA::Geometry const& geometry, EntityContainerView entities, TFloatScalar tolerance = GA::BASE_TOLERANCE) const noexcept
    {
      auto const smallestNodeKey = this->FindSmallestNode(geometry);
      if (!SI::IsValidKey(smallestNodeKey))
        return false;

      auto const& nodeEntityIDs = this->GetNodeEntities(smallestNodeKey);
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
        auto const it = this->m_reverseMap.find(entityID);
        return it != this->m_reverseMap.end();
      }
      else
      {
        auto const& entityGeometry = EA::GetGeometry(entities, entityID);
        return Contains(entityGeometry, entities, tolerance);
      }
    }
  };
} // namespace OrthoTree


#include "adapters/general.h"
#include "octree_container.h"

#include "detail/undefs.h"
