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

#include "../detail/bitset_arithmetic.h"
#include "../detail/common.h"
#include "../detail/inplace_vector.h"
#include "../detail/internal_geometry_module.h"
#include "../detail/memory_resource.h"
#include "../detail/partitioning.h"
#include "../detail/si_morton.h"
#include "../detail/si_mortongrid.h"
#include "../detail/zip_view.h"

#include "configuration.h"
#include "entity_adapter.h"
#include "ot_base.h"
#include "types.h"


#include <algorithm>
#include <bit>
#include <cassert>
#include <compare>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <iterator>
#include <limits>
#include <map>
#include <memory>
#include <numeric>
#include <optional>
#include <ranges>
#include <set>
#include <span>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <variant>
#include <vector>


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

    private:
      static constexpr uint32_t SERIALIZED_VERSION_ID = 0;

      template<typename TArchive, std::size_t CHILD_NO, typename NodeID, typename ChildID, typename EntityID, typename Geometry>
      friend void serialize(TArchive& ar, OrthoTreeNodeData<CHILD_NO, NodeID, ChildID, EntityID, Geometry>& node, const uint32_t version);

      template<typename TArchive, typename T, typename TNodes>
      friend void serialize(TArchive& ar, MemoryResource<T>& memoryResource, TNodes& nodes, const uint32_t version);

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

        // segment size decrease is handled by the memory resource
        return true;
      }

      constexpr void DecreaseEntityIDs([[maybe_unused]] EntityID removedEntityID) noexcept
      {
        if constexpr (std::is_integral_v<EntityID>)
        {
          for (auto& id : m_entities.segment)
            id -= removedEntityID < id;
        }
      }

      EntityContainer& GetEntitySegment() noexcept { return m_entities; }
      EntityContainer const& GetEntitySegment() const noexcept { return m_entities; }

    public: // Child handling
      constexpr void LinkChild(ChildID childID, NodeID nodeID) noexcept
      {
        std::size_t elementID = 0;
        bool shouldOverwrite = false;
        if constexpr (IS_BITSET_BASED_FLAGS)
        {
          auto const it = std::ranges::lower_bound(m_childIndex, static_cast<std::size_t>(childID));
          elementID = std::distance(m_childIndex.begin(), it);
          shouldOverwrite = elementID < m_childIndex.size() && m_childIndex[elementID] == childID;
          if (!shouldOverwrite)
            m_childIndex.insert(it, static_cast<std::size_t>(childID));
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
          auto const it = std::ranges::lower_bound(m_childIndex, static_cast<std::size_t>(childID));
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
          return std::ranges::binary_search(m_childIndex, static_cast<std::size_t>(childID));
        }
        else
        {
          return m_childIndex & (ChildFlags{ 1 } << childID);
        }
      }

      constexpr void UnlinkChild(ChildID childID) noexcept
      {
        std::size_t elementID = 0;
        if constexpr (IS_BITSET_BASED_FLAGS)
        {
          auto const it = std::ranges::lower_bound(m_childIndex, static_cast<std::size_t>(childID));
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


  // Non-owning ortho-tree with dynamic storage for changing datasets.
  template<typename TEntityAdapter, typename TGeometryAdapter, typename TConfiguration>
  class DynamicHashOrthoTreeCore : public OrthoTreeCoreBase<TEntityAdapter, TGeometryAdapter, TConfiguration>
  {
  public:
    using Base = OrthoTreeCoreBase<TEntityAdapter, TGeometryAdapter, TConfiguration>;
    ORTHOTREE_DEPENDENT_TYPES(Base);

  public:
    using SI = typename Base::SI;

    using NodeID = typename SI::NodeID;
    using NodeIDCR = typename SI::NodeIDCR;

  private:
    static_assert(std::is_trivially_copyable_v<EntityID>, "EntityID must be trivially copyable!");

    using NodeGeometry = std::conditional_t<CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::None, std::monostate, typename Base::NodeGeometry>;

    using Node = detail::OrthoTreeNodeData<SI::CHILD_NO, NodeID, typename SI::ChildID, EntityID, NodeGeometry>;

    template<typename TData>
    using LinearNodeContainer = typename CONFIG::template UMapNodeContainer<NodeID, TData, std::hash<NodeID>>;

    template<typename TData>
    using NonLinearNodeContainer = typename CONFIG::template MapNodeContainer<NodeID, TData, bitset_arithmetic_compare>;

    template<typename TData>
    using NodeContainer = typename std::conditional_t<SI::IS_LINEAR_TREE, LinearNodeContainer<TData>, NonLinearNodeContainer<TData>>;

    using ReverseMapType = std::conditional_t<
      EA::ENTITY_ID_STRATEGY == EntityIdStrategy::EntityKeyed,
      typename CONFIG::template ReverseMapKeyed<EntityID, NodeID, typename EA::Hash>,
      typename CONFIG::template ReverseMapIndexed<NodeID>>;
    using ReverseMap = std::conditional_t<CONFIG::USE_REVERSE_MAPPING, ReverseMapType, std::monostate>;

    using NodeValueType = std::pair<NodeID, Node>;
    using MutableNodeValue = std::decay_t<typename NodeContainer<Node>::value_type>*;

  public:
    using NodeValue = std::decay_t<typename NodeContainer<Node>::value_type> const*;

  private: // Member variables
    NodeContainer<Node> m_nodes;
    ReverseMap m_reverseMap;

    detail::MortonGridSpaceIndexing<GA, CONFIG::ALLOW_OUT_OF_SPACE_INSERTION, CONFIG::LOOSE_FACTOR, CONFIG::MAX_ALLOWED_DEPTH_ID> m_spaceIndexing;
    detail::MemoryResource<EntityID> m_memoryResource;

  private: // Serialization
    static constexpr uint32_t SERIALIZED_VERSION_ID = 0;

    template<typename TArchive, typename TEntityAdapter_, typename TGeometryAdapter_, typename TConfiguration_>
    friend void serialize(TArchive& ar, DynamicHashOrthoTreeCore<TEntityAdapter_, TGeometryAdapter_, TConfiguration_>& core, const uint32_t version);

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
    template<typename TExecMode = SeqExec>
    explicit DynamicHashOrthoTreeCore(
      EntityContainerView entities,
      std::optional<depth_t> maxDepthIDIn = std::nullopt,
      std::optional<TBox> boxSpaceOptional = std::nullopt,
      std::size_t maxElementNoInNode = CONFIG::DEFAULT_TARGET_ELEMENT_NUM_IN_NODES,
      TExecMode execMode = {}) noexcept
    {
      [[maybe_unused]] auto isSuccessfullyInsertedAllElements =
        Create(entities, maxDepthIDIn, std::move(boxSpaceOptional), maxElementNoInNode, execMode);
      assert(isSuccessfullyInsertedAllElements);
    }

    // Initialize the base octree structure with entity collection and parallel tree-building option
    template<typename TExecMode>
    DynamicHashOrthoTreeCore(
      TExecMode execMode,
      EntityContainerView entities,
      std::optional<depth_t> maxDepthIDIn = std::nullopt,
      std::optional<TBox> boxSpaceOptional = std::nullopt,
      std::size_t maxElementNoInNode = CONFIG::DEFAULT_TARGET_ELEMENT_NUM_IN_NODES) noexcept
    {
      [[maybe_unused]] auto isSuccessfullyInsertedAllElements =
        Create(entities, maxDepthIDIn, std::move(boxSpaceOptional), maxElementNoInNode, execMode);
      assert(isSuccessfullyInsertedAllElements);
    }

    DynamicHashOrthoTreeCore(DynamicHashOrthoTreeCore const& other)
    : Base(other)
    , m_nodes(other.m_nodes)
    , m_reverseMap(other.m_reverseMap)
    , m_spaceIndexing(other.m_spaceIndexing)
    {
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
      m_reverseMap = other.m_reverseMap;
      m_spaceIndexing = other.m_spaceIndexing;

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
          return Base::GetTreeBoxNominalMinPoint();
        else if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MinPoint)
          return m_nodes.at(GetRootNodeID()).GetGeometry();
        else if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::None)
        {
          if constexpr (CONFIG::ALLOW_OUT_OF_SPACE_INSERTION)
            return Base::GetTreeBoxRealMinPoint();
          else
            return Base::GetTreeBoxNominalMinPoint();
        }
      }();

      auto const& size = [&]() -> decltype(auto) {
        if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MBR)
          return Base::GetTreeBoxNominalSize();
        else if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MinPoint)
          return Base::GetNodeSize(0);
        else if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::None)
        {
          if constexpr (CONFIG::ALLOW_OUT_OF_SPACE_INSERTION)
            return Base::GetTreeBoxRealSize();
          else
            return Base::GetTreeBoxNominalSize();
        }
      }();

      detail::static_for<GA::DIMENSION_NO>([&](auto dimensionID) {
        GA::SetBoxMinC(box, dimensionID, minPoint[dimensionID]);
        GA::SetBoxMaxC(box, dimensionID, minPoint[dimensionID] + size[dimensionID]);
      });

      return box;
    }

  private:
    MutableNodeValue GetMutableNodeValue(NodeIDCR nodeID) noexcept { return &(*m_nodes.find(nodeID)); }
    MutableNodeValue GetMutableRootNodeValue() noexcept { return GetMutableNodeValue(GetRootNodeID()); }

    // Node interface
#ifdef ORTHOTREE__PUBLIC_NODE_INTERFACE
  public:
#else
  protected:
#endif

    static NodeID GetRootNodeID() noexcept { return SI::GetRootKey(); }
    static NodeID GetNoneNodeID() noexcept { return SI::GetNoneKey(); }

    constexpr auto const& GetNodes() const noexcept { return m_nodes; }
    constexpr std::size_t GetNodeCount() const noexcept { return m_nodes.size(); }
    bool HasNode(NodeIDCR key) const noexcept { return m_nodes.contains(key); }
    auto const& GetNode(NodeIDCR key) const noexcept { return m_nodes.at(key); }

    NodeValue GetNodeValue(NodeIDCR nodeID) const noexcept { return &(*m_nodes.find(nodeID)); }
    NodeValue GetRootNodeValue() const noexcept
    {
      assert(
        m_nodes.contains(GetRootNodeID()) &&
        "Root node does not exist! Non-default Constructor, Init() or Create() must be called first to create root node.");
      return GetNodeValue(GetRootNodeID());
    }


    // Get EntityIDs of the node
    constexpr auto const& GetNodeEntities(NodeValue nodeValue) const noexcept { return nodeValue->second.GetEntities(); }

    // Get EntityIDs number of the node
    constexpr std::size_t GetNodeEntityCount(NodeValue nodeValue) const noexcept { return nodeValue->second.GetEntitiesSize(); }

    // Get Node min-point
    constexpr decltype(auto) GetNodeMinPoint(NodeValue nodeValue) const noexcept
    {
      if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::None)
      {
        auto const nodeCenter = m_spaceIndexing.CalculateNodeCenter(nodeValue->first);
        auto const depthID = SI::GetDepthID(nodeValue->first);
        auto const& nodeHalfSize = Base::GetNodeSize(depthID + 1);

        return IGM::Sub(nodeCenter, nodeHalfSize);
      }
      else if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MinPoint)
        return nodeValue->second.GetGeometry();
      else if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MBR)
        return nodeValue->second.GetGeometry().minPoint;
      else
        static_assert(false, "Unsupported node geometry storage!");
    }

    // Get Node size
    constexpr decltype(auto) GetNodeSize(NodeValue nodeValue) const noexcept
    {
      auto const depthID = SI::GetDepthID(nodeValue->first);
      if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::None || CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MinPoint)
        return Base::GetNodeSize(depthID);
      else if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MBR)
        return nodeValue->second.GetGeometry().size;
      else
        static_assert(false, "Unsupported node geometry storage!");
    }

    // Get Child Node IDs
    constexpr decltype(auto) GetNodeChildren(NodeValue nodeValue) const noexcept { return nodeValue->second.GetChildren(); }

    // Get Node box
    constexpr IGM::Box GetNodeBox(NodeValue nodeValue) const noexcept
    {
      auto const& minPoint = GetNodeMinPoint(nodeValue);
      return { .Min = minPoint, .Max = IGM::Add(minPoint, GetNodeSize(nodeValue)) };
    }

    // Is the node has any entity
    constexpr bool IsNodeEntitiesEmpty(NodeValue nodeValue) const noexcept { return nodeValue->second.IsEntitiesEmpty(); }

  protected:
    constexpr void AddNodeEntity(MutableNodeValue nodeValue, EntityID newEntity) noexcept
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

    constexpr void InitNodeGeometry([[maybe_unused]] auto* nodeValue) const noexcept
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

    constexpr NodeValueType CreateNode(NodeIDCR nodeID) const noexcept
    {
      if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::None)
      {
        return NodeValueType{ nodeID, Node() };
      }
      else if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MinPoint || CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MBR)
      {
        auto nodeValue = NodeValueType{ nodeID, Node() };
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
      m_spaceIndexing =
        detail::MortonGridSpaceIndexing<GA, CONFIG::ALLOW_OUT_OF_SPACE_INSERTION, CONFIG::LOOSE_FACTOR, CONFIG::MAX_ALLOWED_DEPTH_ID>(maxDepthID, boxSpace);
      Base::InitBase(m_spaceIndexing.GetMinPoint(), m_spaceIndexing.GetSize(), maxDepthID, maxElementNo);

      ORTHOTREE_CRASH_IF(
        !m_nodes.empty(), "To build/setup/create the tree, use the Create() [recommended] or Init() function. If an already built tree is wanted to be reset, use the Reset() function before Init().");

      m_nodes.emplace(CreateNode(GetRootNodeID()));
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
      NodeValueType NodeInstance;
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
      if (subtreeEntityNo > Base::GetMaxElementNum() && depthID < Base::GetMaxDepthID())
      {
        auto const isNonRefinable = [depthID](auto const& location) {
          auto const d = location.GetFirst().GetDepthID();
          return d == depthID;
        };

        if constexpr (ARE_LOCATIONS_SORTED)
          stuckedEndLocationIt = std::partition_point(locationIt, endLocationIt, isNonRefinable);
        else
          stuckedEndLocationIt = std::partition(locationIt, endLocationIt, isNonRefinable);

        nodeEntityNo = std::size_t(std::distance(locationIt, stuckedEndLocationIt));
      }

      if (nodeEntityNo == 0)
        return;

      node.second.ReplaceEntities(std::span(locationIt.GetSecond(), stuckedEndLocationIt.GetSecond()));

      if constexpr (CONFIG::USE_REVERSE_MAPPING)
      {
        for (auto it = locationIt.GetSecond(); it != stuckedEndLocationIt.GetSecond(); ++it)
          detail::set(m_reverseMap, *it, node.first);
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
      auto const childChecker = typename SI::ChildCheckerFixedDepth(examinedLevelID, (*locationIt).GetFirst().GetLocationID());
      auto const childID = childChecker.GetChildID(examinedLevelID);
      auto childKey = keyGenerator.GetChildNodeKey(childID);

      parentNodeProcessingData.NodeInstance.second.LinkChild(childID, childKey);
      if constexpr (ARE_LOCATIONS_SORTED)
      {
        nodeProcessingData.EndLocationIt = std::partition_point(locationIt, parentNodeProcessingData.EndLocationIt, [&](auto const& location) {
          return childChecker.Test(location.GetFirst().GetLocationID());
        });
      }
      else
      {
        nodeProcessingData.EndLocationIt = std::partition(locationIt, parentNodeProcessingData.EndLocationIt, [&](auto const& location) {
          return childChecker.Test(location.GetFirst().GetLocationID());
        });
      }

      nodeProcessingData.NodeInstance = CreateNode(childKey);
    }

    // Build the tree in depth-first order
    template<bool ARE_LOCATIONS_SORTED, typename TResulEntityContainerView>
    constexpr void BuildSubtree(
      LocationIterator const& rootBeginLocationIt, LocationIterator const& rootEndLocationIt, NodeValueType&& rootNode, TResulEntityContainerView& nodes) noexcept
    {
      auto nodeStack = std::vector<NodeProcessingData>(Base::GetDepthNo());
      nodeStack[0] = NodeProcessingData{ std::move(rootNode), rootEndLocationIt };

      auto locationIt = rootBeginLocationIt;
      auto constexpr exitDepthID = depth_t(-1);
      for (depth_t depthID = 0; depthID != exitDepthID;)
      {
        if (!nodeStack[depthID].NodeInstance.second.IsAnyChildExist())
        {
          ProcessNode<ARE_LOCATIONS_SORTED>(depthID, locationIt, nodeStack[depthID]);
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
        CreateProcessingData<ARE_LOCATIONS_SORTED>(examinedLevelID, keyGenerator, locationIt, nodeStack[depthID - 1], nodeStack[depthID]);
      }
    }

  public: // Create
    // Create
    template<typename TExecMode = SeqExec, bool ARE_ENTITIES_SURELY_IN_MODELSPACE = false>
    bool Create(
      EntityContainerView entities,
      std::optional<depth_t> maxDepthIn = std::nullopt,
      std::optional<TBox> boxSpaceOptional = std::nullopt,
      std::size_t maxElementNoInNode = CONFIG::DEFAULT_TARGET_ELEMENT_NUM_IN_NODES,
      TExecMode execMode = {}) noexcept
    {
      auto const boxSpace = boxSpaceOptional ? IGM::GetBoxAD(*boxSpaceOptional) : IGM::template GetBoundingBoxAD<EA>(entities);

      auto const entityNo = entities.size();
      auto const maxDepthID = (!maxDepthIn || maxDepthIn == depth_t{})
                              ? detail::EstimateMaxDepth<GA::DIMENSION_NO, SI::MAX_THEORETICAL_DEPTH_ID>(entityNo, maxElementNoInNode)
                              : *maxDepthIn;
      InitBase(boxSpace, maxDepthID, maxElementNoInNode, entityNo);

      if (entityNo == 0)
        return true;


      if constexpr (CONFIG::USE_REVERSE_MAPPING)
        detail::resize(m_reverseMap, entityNo);

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

      constexpr bool ARE_LOCATIONS_SORTED = std::is_same_v<TExecMode, ExecutionTags::Parallel>;
      if constexpr (ARE_LOCATIONS_SORTED)
      {
        EXEC_POL_DEF(eps); // GCC 11.3
        std::sort(EXEC_POL_ADD(eps) locationsZip.begin(), endIt, [](Location const& l, Location const& r) { return l.first < r.first; });
      }

      auto rootNode = *m_nodes.begin();
      m_nodes.clear();
      detail::reserve(m_nodes, detail::EstimateNodeNumber<GA::DIMENSION_NO, SI::MAX_THEORETICAL_DEPTH_ID>(entityNo, maxDepthID, maxElementNoInNode));
      BuildSubtree<ARE_LOCATIONS_SORTED>(locationsZip.begin(), endIt, std::move(rootNode), m_nodes);

      if (boxSpaceOptional && !ARE_ENTITIES_SURELY_IN_MODELSPACE)
        InitializeRootNodeBox(entities);
      else
        InitializeSubtreeMinimalNodeGeometry(GetMutableRootNodeValue(), entities);

      return true;
    }

    // Create
    template<typename TExecMode = SeqExec, bool ARE_ENTITIES_SURELY_IN_MODELSPACE = false>
    static bool Create(
      DynamicHashOrthoTreeCore& tree,
      EntityContainerView entities,
      std::optional<depth_t> maxDepthIn = std::nullopt,
      std::optional<TBox> boxSpaceOptional = std::nullopt,
      std::size_t maxElementNoInNode = CONFIG::DEFAULT_TARGET_ELEMENT_NUM_IN_NODES,
      TExecMode execMode = {}) noexcept
    {
      return tree.Create<TExecMode, ARE_ENTITIES_SURELY_IN_MODELSPACE>(entities, maxDepthIn, std::move(boxSpaceOptional), maxElementNoInNode, execMode);
    }


  private:
    void LinkOrphanNodes(std::vector<NodeID>&& orphanNodes) noexcept
    {
      for (std::size_t i = 0; i < orphanNodes.size(); ++i)
      {
        auto const orphanNodeID = orphanNodes[i];
        auto& [parentNodeID, parentNode] = *GetParentIt(orphanNodeID);
        auto const childID = SI::GetNonDirectChildID(parentNodeID, orphanNodeID);

        if (parentNode.HasChild(childID))
        {
          auto const childNodeID = parentNode.GetChild(childID);
          auto const lcaNodeID = SI::GetLowestCommonAncestor(childNodeID, orphanNodeID);
          parentNode.LinkChild(childID, lcaNodeID);

          if (orphanNodeID == lcaNodeID)
          {
            auto& orphanNode = m_nodes.at(orphanNodeID);
            auto const childIDOfOrphanNode = SI::GetNonDirectChildID(orphanNodeID, childNodeID);
            if (orphanNode.HasChild(childIDOfOrphanNode))
              orphanNodes.push_back(orphanNode.GetChild(childIDOfOrphanNode));

            orphanNode.LinkChild(childIDOfOrphanNode, childNodeID);
          }
          else
          {
            auto [lcaIt, _] = m_nodes.try_emplace(lcaNodeID);
            InitNodeGeometry(&*lcaIt);
            auto& lcaNode = lcaIt->second;
            lcaNode.LinkChild(SI::GetNonDirectChildID(lcaNodeID, childNodeID), childNodeID);
            lcaNode.LinkChild(SI::GetNonDirectChildID(lcaNodeID, orphanNodeID), orphanNodeID);
          }
        }
        else
        {
          parentNode.LinkChild(childID, orphanNodeID);
        }
      }
    }

    template<typename TNewEntities, typename TExecMode = SeqExec>
    constexpr void UpdateNodeGeometry(
      [[maybe_unused]] std::vector<std::pair<typename SI::Location, std::size_t>> changedNodes,
      [[maybe_unused]] TNewEntities const& newEntities,
      [[maybe_unused]] std::size_t existingEntityNum,
      [[maybe_unused]] TExecMode execMode = {}) noexcept
    {
      if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MBR)
      {
        if (changedNodes.empty())
          return;

        std::sort(changedNodes.begin(), changedNodes.end(), [](auto const& lhs, auto const& rhs) {
          return SI::Location::template IsLess<true>(lhs.first, rhs.first);
        });
        auto const& changedNodesInDFS = changedNodes;

        auto entityGeometries =
          std::conditional_t<EA::ENTITY_ID_STRATEGY == EntityIdStrategy::EntityKeyed, std::unordered_map<EntityID, EntityGeometry>, std::vector<EntityGeometry>>{};
        detail::reserve(entityGeometries, detail::size(newEntities));

        EXEC_POL_DEF(epu);
        if constexpr (EA::ENTITY_ID_STRATEGY == EntityIdStrategy::EntityKeyed)
        {
          std::for_each(newEntities.begin(), newEntities.end(), [&](auto const& item) {
            using T = std::remove_cvref_t<decltype(item)>;
            if constexpr (std::is_convertible_v<T, typename EA::Entity>)
            {
              entityGeometries.try_emplace(EA::GetEntityID(item), EA::GetGeometry(item));
            }
            else if constexpr (requires(T v) {
                                 []<typename U>(U&& u) {
                                   auto [a, b] = u;
                                 }(v);
                               })
            {
              auto const& [entityID, entityGeometry] = item;
              entityGeometries.try_emplace(static_cast<EntityID>(entityID), entityGeometry);
            }
            else
            {
              static_assert(false, "EntityID cannot be determined for non-keyed entities in a non-contiguous container.");
            }
          });
        }
        else
        {
          entityGeometries.resize(detail::size(newEntities));
          std::for_each(EXEC_POL_ADD(epu) newEntities.begin(), newEntities.end(), [&](auto const& item) {
            using T = std::remove_cvref_t<decltype(item)>;

            if constexpr (std::is_convertible_v<T, typename EA::Geometry>)
            {
              entityGeometries[detail::getKeyPart(newEntities, item)] = item;
            }
            else if constexpr (requires(T v) {
                                 []<typename U>(U&& u) {
                                   auto [a, b] = u;
                                 }(v);
                               })
            {
              auto const& [entityID, entityGeometry] = item;
              assert(entityID >= existingEntityNum);
              entityGeometries[entityID - existingEntityNum] = entityGeometry;
            }
            else
            {
              static_assert(false, "EntityID cannot be determined for non-keyed entities in a non-contiguous container.");
            }
          });
        }

        auto getNewEntityGeometry = [&](EntityID id) -> EntityGeometry const& {
          if constexpr (EA::ENTITY_ID_STRATEGY == EntityIdStrategy::EntityKeyed)
            return entityGeometries.at(id);
          else
            return entityGeometries[id - existingEntityNum];
        };

        // Reverse DFS order
        int changedNodesInDFSIndex = int(changedNodesInDFS.size() - 1);
        for (int i = changedNodesInDFSIndex; i >= 0; --i)
        {
          auto const& [location, beginID] = changedNodesInDFS[i];
          auto const nodeID = m_spaceIndexing.GetNodeID(location);
          auto lcaNodeID = i == 0 ? GetRootNodeID() : SI::GetLowestCommonAncestor(changedNodesInDFS[i - 1].first, location, Base::GetMaxDepthID());

          auto nodeIt = m_nodes.find(nodeID);

          auto const& entityIDs = GetNodeEntities(&*nodeIt);
          auto& nodeGeometry = nodeIt->second.GetGeometry();
          bool const isInitialized = IsNodeGeometryInitialized(nodeGeometry);

          bool nodeChanged = false;
          typename IGM::Box nodeBox;
          if (isInitialized)
          {
            auto const& [minPoint, size] = nodeGeometry;
            nodeBox = typename IGM::Box{ minPoint, IGM::Add(minPoint, size) };
          }
          else
          {
            nodeBox = IGM::BoxInvertedInit();
            nodeChanged = true;
          }

          auto const nodeBoxOriginal = nodeBox;
          for (auto it = std::next(entityIDs.begin(), beginID); it != entityIDs.end(); ++it)
            IGM::UniteInBoxAD(nodeBox, getNewEntityGeometry(*it));

          // it is possible that it is a new Parent node (LCA), it should update by all children
          for (auto const& childNodeID : GetNodeChildren(&*nodeIt))
            IGM::UniteInBoxAD(nodeBox, GetNodeBox(GetNodeValue(childNodeID)));

          nodeChanged = nodeChanged || (nodeBoxOriginal.Min != nodeBox.Min || nodeBoxOriginal.Max != nodeBox.Max);
          if (nodeChanged)
            nodeIt->second.GetGeometry() = { nodeBox.Min, IGM::Sub(nodeBox.Max, nodeBox.Min) };

          NodeID workingNodeID = nodeID;
          typename IGM::Box workingBox = nodeBox;
          while (nodeChanged && workingNodeID != GetRootNodeID())
          {
            auto parentNodeIt = GetParentIt(m_nodes, workingNodeID);
            auto const parentNodeID = parentNodeIt->first;
            auto parentNodeBox = GetNodeBox(&*parentNodeIt);
            auto const parentNodeBoxOriginal = parentNodeBox;

            IGM::UniteInBoxAD(parentNodeBox, workingBox);
            nodeChanged = (parentNodeBoxOriginal.Min != parentNodeBox.Min || parentNodeBoxOriginal.Max != parentNodeBox.Max);

            if (nodeChanged)
              parentNodeIt->second.GetGeometry() = { parentNodeBox.Min, IGM::Sub(parentNodeBox.Max, parentNodeBox.Min) };

            if (parentNodeID == lcaNodeID || SI::GetDepthID(parentNodeID) < SI::GetDepthID(lcaNodeID))
              break;

            workingNodeID = parentNodeID;
            workingBox = parentNodeBox;
          }
        }
      }
    }

  public:
    // Insert entities into the octree. Returns true if all entities were inserted successfully, false otherwise.
    // Input: Accepted newEntities containers: std::span<EA::Geometry>, TContainer<[EA::EntityID, EA::Geometry]>, TContainer<EA:Entity>
    // Output: failedEntities contains the IDs of entities that were not inserted if there were any. (Only if CONFIG::ALLOW_OUT_OF_SPACE_INSERTION == false)
    template<typename TExecMode = SeqExec>
    bool Insert(auto&& newEntities, EA::EntityContainerView existingEntities, TExecMode execMode = {}, std::unordered_set<EntityID>* failedEntities = nullptr) noexcept
      requires std::ranges::input_range<decltype(newEntities)> &&
               (std::convertible_to<std::ranges::range_value_t<decltype(newEntities)>, typename EA::Geometry> ||
                std::convertible_to<std::ranges::range_value_t<decltype(newEntities)>, typename EA::Entity> ||
                requires(std::ranges::range_value_t<decltype(newEntities)> v) { v.first; v.second; } ||
                requires(std::ranges::range_value_t<decltype(newEntities)> v) {
                  // Structured binding check that is less likely to trigger hard errors on arrays
                  []<typename U>(U&& u) { [[maybe_unused]]auto [a, b] = u; }(v);
                })
    {
      constexpr bool IS_ELEMENT_DEPTH_SPECIFIC = (EA::GEOMETRY_TYPE != GeometryType::Point);
      constexpr dim_t DIMENSION_NO = GA::DIMENSION_NO;
      constexpr uint32_t kSortThreshold = 1024;

      using Location = typename SI::Location;
      using LowestCommonAncestorCalculator = typename SI::template LowestCommonAncestorCalculator<IS_ELEMENT_DEPTH_SPECIFIC>;

      struct EntityData
      {
        Location location;
        EntityID id;
      };

      struct WorkItem
      {
        uint32_t begin;
        uint32_t end;
        uint32_t bitsRemaining;
        Location location;
      };

      auto newEntityCount = newEntities.size();
      if (newEntityCount == 0)
        return true;

      auto const maxDepthID = Base::GetMaxDepthID();
      auto const maxElementNo = Base::GetMaxElementNum();
      auto const existingEntityNum = EA::GetEntityCount(existingEntities);

      // Morton code creation
      auto buffer = std::vector<EntityData>(newEntityCount);
      EXEC_POL_DEF(ept); // GCC 11.3
      std::transform(EXEC_POL_ADD(ept) newEntities.begin(), newEntities.end(), buffer.begin(), [&](auto const& item) -> EntityData {
        using T = std::remove_cvref_t<decltype(item)>;
        if constexpr (EA::ENTITY_ID_STRATEGY == EntityIdStrategy::EntityKeyed && std::is_convertible_v<T, typename EA::Entity>)
        {
          return { m_spaceIndexing.GetLocation(EA::GetGeometry(item)), EA::GetEntityID(item) };
        }
        else if constexpr (std::is_convertible_v<T, typename EA::Geometry> && std::ranges::contiguous_range<decltype(newEntities)>)
        {
          auto const entityID = static_cast<EntityID>(existingEntityNum + detail::getID(newEntities, item));
          return { m_spaceIndexing.GetLocation(item), entityID };
        }
        else if constexpr (requires(T v) {
                             []<typename U>(U&& u) {
                               auto [a, b] = u;
                             }(v);
                           })
        {
          auto const& [entityID, entityGeometry] = item;
          return { m_spaceIndexing.GetLocation(entityGeometry), static_cast<EntityID>(entityID) };
        }
        else
        {
          static_assert(false, "EntityID cannot be determined for non-keyed entities in a non-contiguous container.");
        }
      });

      // Check if all entities are within the tree bounds
      bool isAllEntitiesInserted = true;
      if constexpr (!CONFIG::ALLOW_OUT_OF_SPACE_INSERTION)
      {
        auto const endIt =
          std::partition(buffer.begin(), buffer.end(), [](auto const& element) { return element.location.GetDepthID() != INVALID_DEPTH; });
        newEntityCount = std::distance(buffer.begin(), endIt);
        isAllEntitiesInserted = endIt == buffer.end();

        if (!isAllEntitiesInserted && failedEntities)
        {
          failedEntities->reserve(std::distance(endIt, buffer.end()));
          std::transform(endIt, buffer.end(), std::back_inserter(*failedEntities), [](auto const& element) { return element.id; });
        }

        if constexpr (EA::ENTITY_ID_STRATEGY != EntityIdStrategy::EntityKeyed)
        {
          if (!isAllEntitiesInserted)
            return false;
        }
      }

      if constexpr (CONFIG::USE_REVERSE_MAPPING)
        detail::resize(m_reverseMap, existingEntityNum + newEntityCount);

      // Configurable digit width
      // Considering 32kB L1 cache with 64B width cache lines: 512 cache lines total is the limit.
      // 3D: 6 bits/64 buckets with 2 cache lines per bucket: 128 cache lines. Other stack variabbles (e.g. histogram / offsets) are also cached.
      constexpr uint32_t kRadixBits = DIMENSION_NO > 8 ? std::min<uint32_t>(12, DIMENSION_NO) : (8 / DIMENSION_NO) * DIMENSION_NO;
      constexpr uint32_t kRadixMaxSize = 1u << kRadixBits;

      auto const totalBits = static_cast<uint32_t>(maxDepthID * DIMENSION_NO);

      // In-place partition of buffer[begin..end) using bits [shift, shift+numBits).
      auto bucketSort = [&](auto& activeBuckets, auto& histogram, auto& offsets, auto& buffer, auto const& GetBucketID) {
        auto leaderBucketIt = activeBuckets.begin();
        uint32_t leaderBucketID = 0;
        auto leaderBucketEndIdx = 0;
        int fromIdx = 0;
        for (;;)
        {
          if (fromIdx == leaderBucketEndIdx)
          {
            leaderBucketID = *leaderBucketIt;
            ++leaderBucketIt;
            if (leaderBucketIt == activeBuckets.end())
              break;

            if (histogram[leaderBucketID] == 0)
              continue;

            fromIdx = offsets[leaderBucketID];
            leaderBucketEndIdx = fromIdx + histogram[leaderBucketID];
            histogram[leaderBucketID] = 0;
          }

          auto bucketID = GetBucketID(buffer[fromIdx].location.GetLocationID());
          if (leaderBucketID == bucketID)
          {
            ++fromIdx;
          }
          else
          {
            std::swap(buffer[fromIdx], buffer[offsets[bucketID]]);

            ++offsets[bucketID];
            --histogram[bucketID];
          }
        }
      };

      // Computes per-bucket LCA fold during histogram pass (zero extra scan).
      auto const radixPartition = [&](uint32_t begin, uint32_t end, uint32_t bitsToTest, auto& workStack) {
        uint32_t const numBits = std::min(kRadixBits, bitsToTest);
        uint32_t const radixShift = bitsToTest - numBits;
        uint32_t const radixMask = (1u << numBits) - 1;

        auto const GetBucketID = [radixShift, radixMask](SI::LocationIDCR key) noexcept -> uint32_t {
          return static_cast<uint32_t>(key >> radixShift) & radixMask;
        };

        // Histogram + active bucket tracking + per-bucket LCA fold
        auto histogram = std::array<uint32_t, kRadixMaxSize>{};
        auto activeBuckets = Partitioning::flagset<kRadixMaxSize>{};
        auto bucketFolds = std::array<LowestCommonAncestorCalculator, kRadixMaxSize>{};

        for (uint32_t i = begin; i < end; ++i)
        {
          auto const& loc = buffer[i].location;
          auto const b = GetBucketID(loc.GetLocationID());
          if (histogram[b] == 0)
            bucketFolds[b] = LowestCommonAncestorCalculator(loc);
          else
            bucketFolds[b].Add(loc);

          ++histogram[b];
          activeBuckets.set(b);
        }

        // Prefix sum (only active buckets)
        auto offsets = std::array<uint32_t, kRadixMaxSize>{};
        uint32_t sum = begin;
        for (auto b : activeBuckets)
        {
          offsets[b] = sum;
          sum += histogram[b];

          auto location = bucketFolds[b].GetLocation(Base::GetMaxDepthID());

          auto const levelID = Base::GetMaxDepthID() - location.GetDepthID();
          workStack.push_back({ offsets[b], sum, std::min(levelID * DIMENSION_NO, radixShift), location });
        }

        if (activeBuckets.size() == 1)
          return;

        bucketSort(activeBuckets, histogram, offsets, buffer, GetBucketID);
      };

      // Emit range as a single node using precomputed LCA fold. Zero scan.
      auto orphanNodes = std::vector<NodeID>{};
      auto changedNodes = std::vector<std::pair<Location, std::size_t>>{};

      auto const emitCluster = [&](uint32_t begin, uint32_t end, Location const& location) {
        auto const elementNum = end - begin;
        if (elementNum == 0)
          return;

        auto nodeID = m_spaceIndexing.GetNodeID(location);
        auto [it, isInserted] = m_nodes.try_emplace(nodeID);
        auto& entitySegment = it->second.GetEntitySegment();
        auto existingElementNum = entitySegment.segment.size();
        changedNodes.emplace_back(location, existingElementNum);

        if (isInserted)
        {
          orphanNodes.push_back(nodeID);
          InitNodeGeometry(&*it);
          entitySegment = m_memoryResource.Allocate(elementNum);
        }
        else
          m_memoryResource.IncreaseSegment(entitySegment, elementNum);

        for (uint32_t i = 0; i < elementNum; ++i)
          entitySegment.segment[existingElementNum + i] = buffer[begin + i].id;

        // Update reverse map
        if constexpr (CONFIG::USE_REVERSE_MAPPING)
        {
          for (uint32_t i = 0; i < elementNum; ++i)
            UpdateReverseMap(buffer[begin + i].id, nodeID);
        }
      };

      // Linear walk over pre-sorted buffer[begin..end), emits tree nodes.
      auto const emitSortedRange = [&](uint32_t begin, uint32_t end) {
        uint32_t clusterBegin = begin;
        while (clusterBegin < end)
        {
          auto lcah = LowestCommonAncestorCalculator(buffer[clusterBegin].location);

          uint32_t clusterEnd = clusterBegin + 1;
          while (clusterEnd < end)
          {
            auto newLcah = lcah;
            newLcah.Add(buffer[clusterEnd].location);

            auto const newDepthID = newLcah.GetLocation(maxDepthID).GetDepthID();
            auto const clusterSize = clusterEnd - clusterBegin + 1;

            if (clusterSize > maxElementNo && newDepthID < maxDepthID)
            {
              auto const curDepthID = lcah.GetLocation(maxDepthID).GetDepthID();
              if (newDepthID < curDepthID)
                break;
            }

            lcah = newLcah;
            ++clusterEnd;
          }

          emitCluster(clusterBegin, clusterEnd, lcah.GetLocation(maxDepthID));
          clusterBegin = clusterEnd;
        }
      };


      auto workStack = std::vector<WorkItem>{};
      workStack.reserve(64);
      workStack.push_back({ 0, static_cast<uint32_t>(newEntityCount), totalBits, SI::GetRootLocation() });

      while (!workStack.empty())
      {
        auto const w = workStack.back();
        workStack.pop_back();

        auto const elementNum = w.end - w.begin;
        if (elementNum == 0)
          continue;

        // Terminal: fits in one node → emit directly using precomputed fold (zero scan)
        if (elementNum <= maxElementNo || w.bitsRemaining == 0)
        {
          emitCluster(w.begin, w.end, w.location);
          continue;
        }

        // Size-based strategy: sort+walk if fits cache, else radix partition
        if (elementNum <= kSortThreshold)
        {
          std::sort(buffer.begin() + w.begin, buffer.begin() + w.end, [](EntityData const& a, EntityData const& b) {
            return Location::template IsLess<IS_ELEMENT_DEPTH_SPECIFIC>(a.location, b.location);
          });
          emitSortedRange(w.begin, w.end);
        }
        else
        {
          radixPartition(w.begin, w.end, w.bitsRemaining, workStack);
        }
      }

      LinkOrphanNodes(std::move(orphanNodes));
      UpdateNodeGeometry(changedNodes, newEntities, existingEntityNum, execMode);

      return isAllEntitiesInserted;
    }


  private:
    auto AddNode(NodeIDCR nodeID) noexcept
    {
      auto [nodeIt, isNew] = m_nodes.try_emplace(nodeID);
      assert(isNew);

      InitNodeGeometry(&*nodeIt);

      auto& [parentNodeID, parentNode] = *GetParentIt(nodeID);
      auto const childID = SI::GetNonDirectChildID(parentNodeID, nodeID);

      if (!parentNode.HasChild(childID))
      {
        parentNode.LinkChild(childID, nodeID);
        // leaf node does not have MBR geometry yet
        return nodeIt;
      }

      auto const childNodeID = parentNode.GetChild(childID);
      auto const lcaNodeID = SI::GetLowestCommonAncestor(childNodeID, nodeID);
      parentNode.LinkChild(childID, lcaNodeID);

      if (nodeID == lcaNodeID)
      {
        nodeIt->second.LinkChild(SI::GetNonDirectChildID(lcaNodeID, childNodeID), childNodeID);
        return nodeIt;
      }
      else
      {
        auto [lcaIt, _] = m_nodes.emplace(lcaNodeID, Node{});
        InitNodeGeometry(&*lcaIt);

        auto& lcaNode = lcaIt->second;
        lcaNode.LinkChild(SI::GetNonDirectChildID(lcaNodeID, childNodeID), childNodeID);
        lcaNode.LinkChild(SI::GetNonDirectChildID(lcaNodeID, nodeID), nodeID);

        if constexpr (detail::is_reference_stable_v<decltype(m_nodes)>)
          return nodeIt;
        else
          return m_nodes.find(nodeID);
      }
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

    constexpr void UpdateReverseMap(EntityID entityID, NodeIDCR nodeID) noexcept
    {
      if constexpr (CONFIG::USE_REVERSE_MAPPING)
      {
        if constexpr (EA::ENTITY_ID_STRATEGY != EntityIdStrategy::EntityKeyed)
        {
          auto size = detail::size(m_reverseMap);
          if (entityID >= size)
            detail::resize(m_reverseMap, std::size_t(entityID + 1));
        }

        detail::set(m_reverseMap, entityID, nodeID);
      }
    }

    IGM::Box InitializeSubtreeMinimalNodeGeometry(MutableNodeValue nodeValue, EntityContainerView entities)
    {
      if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MBR)
      {
        auto nodeBox = IGM::BoxInvertedInit();

        // Union with all children
        for (auto const& childNodeID : GetNodeChildren(nodeValue))
        {
          auto childNodeValue = GetMutableNodeValue(childNodeID);
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
    void UpdateMinimalNodeGeometry(MutableNodeValue nodeValue, IGM::Box const& newGeometryBox)
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

          auto nodeBox = IGM::BoxInvertedInit();
          IGM::UniteInBoxAD(nodeBox, newGeometryBox);

          // it is possible that it is a new Parent node (LCA), it should update by all children
          for (auto const& childNodeID : GetNodeChildren(nodeValue))
            IGM::UniteInBoxAD(nodeBox, GetNodeBox(GetNodeValue(childNodeID)));

          minPoint = nodeBox.Min;
          size = IGM::Sub(nodeBox.Max, nodeBox.Min);
        }
        else
        {
          for (dim_t d = 0; d < GA::DIMENSION_NO; ++d)
          {
            auto maxPoint = minPoint[d] + size[d];

            if (newGeometryBox.Min[d] < minPoint[d])
            {
              minPoint[d] = newGeometryBox.Min[d];
              isChanged = true;
            }

            if (newGeometryBox.Max[d] > maxPoint)
            {
              maxPoint = newGeometryBox.Max[d];
              isChanged = true;
            }

            size[d] = maxPoint - minPoint[d];
          }
        }

        if (isChanged)
        {
          auto parentNodeIt = GetParentIt(nodeValue->first);
          if (parentNodeIt != m_nodes.end())
            UpdateMinimalNodeGeometry(&*parentNodeIt, newGeometryBox);
        }
      }
      else if constexpr (IS_ROOT_UPDATE && (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MinPoint || CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::None))
      {
        assert(nodeValue == GetMutableRootNodeValue());
        auto nodeBox = GetNodeBox(nodeValue);
        IGM::UniteInBoxAD(nodeBox, newGeometryBox);

        Base::SetTreeSize(IGM::Sub(nodeBox.Max, nodeBox.Min));
        if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MinPoint)
          nodeValue->second.GetGeometry() = nodeBox.Min;
        else
          Base::SetRealTreeMinPoint(nodeBox.Min);
      }
    }

    // Cascade initialization of node geometry
    void InitializeMinimalNodeGeometry(MutableNodeValue nodeValue, bool isCascadeMode = false, std::optional<typename IGM::Box> entityBox = std::nullopt)
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
          auto const childNodeValue = GetNodeValue(childNodeID);
          if (!IsNodeGeometryInitialized(childNodeValue->second.GetGeometry()))
          {
            assert(false);
            continue;
          }

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
    void InitializeMinimalNodeGeometry(MutableNodeValue nodeValue, EntityContainerView entities, bool isCascadeMode = false)
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
      auto rootNodeValue = GetMutableRootNodeValue();
      if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MBR)
      {
        InitializeSubtreeMinimalNodeGeometry(rootNodeValue, entities);
      }
      else if constexpr (CONFIG::ALLOW_OUT_OF_SPACE_INSERTION)
      {
        auto nodeBox = GetNodeBox(rootNodeValue);
        if (!IsNodeGeometryInitialized(rootNodeValue->second.GetGeometry()))
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

#ifdef ORTHOTREE__PUBLIC_NODE_INTERFACE
  public:
#else
  private:
#endif // ORTHOTREE__PUBLIC_NODE_INTERFACE

    constexpr NodeID Find(EntityID entityID) const noexcept
    {
      if constexpr (CONFIG::USE_REVERSE_MAPPING)
      {
        auto const pNodeID = detail::get_if(m_reverseMap, entityID);
        if (!pNodeID)
          return GetNoneNodeID();

        return *pNodeID;
      }
      else
      {
        auto const it = std::find_if(m_nodes.begin(), m_nodes.end(), [&](auto const& keyAndValue) {
          auto const& entities = GetNodeEntities(&keyAndValue);
          return std::ranges::find(entities, entityID) != entities.end();
        });
        return it == m_nodes.end() ? GetNoneNodeID() : it->first;
      }
    }

    bool IsEveryEntityUnique() const noexcept
    {
#ifdef ORTHOTREE__DEBUG_CHECKS
      auto ids = std::vector<EntityID>();
      ids.reserve(100);
      std::for_each(m_nodes.begin(), m_nodes.end(), [&](auto& nodeValue) {
        auto const& entities = GetNodeEntities(&nodeValue);
        ids.insert(ids.end(), entities.begin(), entities.end());
      });

      auto const idsSizeBeforeUnique = ids.size();
      detail::sortAndUnique(ids);
      return idsSizeBeforeUnique == ids.size();
#else
      return true;
#endif
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
          auto nodeValue = GetMutableNodeValue(GetRootNodeID());
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
    bool InsertIntoLeaf(EntityID entityID, EA::Geometry const& entityGeometry, InsertionMode insertionMode = InsertionMode::ExistingLeaf) noexcept
    {
      assert([insertionMode] {
        switch (insertionMode)
        {
        case InsertionMode::Balanced: return false;
        case InsertionMode::LowestLeaf:
        case InsertionMode::ExistingLeaf: return true;
        }
        return false;
      }());

      auto const oosResult = HandleOutOfSpaceInsertion(entityID, entityGeometry);
      if (oosResult != OutOfSpaceInsertionResult::NotHandled)
        return oosResult != OutOfSpaceInsertionResult::Denied;

      auto const entityNodeID = m_spaceIndexing.GetNodeID(entityGeometry);
      auto nodeIt = m_nodes.find(entityNodeID);
      if (nodeIt == m_nodes.end())
      {
        if (insertionMode == InsertionMode::LowestLeaf)
        {
          nodeIt = AddNode(entityNodeID);
        }
        else
        {
          nodeIt = GetParentIt(entityNodeID);
          if (nodeIt->second.IsAnyChildExist())
          {
            auto const childID = SI::GetNonDirectChildID(nodeIt->first, entityNodeID);
            auto const childGenerator = typename SI::ChildKeyGenerator(nodeIt->first);
            auto const childNodeID = childGenerator.GetChildNodeKey(childID);

            nodeIt = AddNode(childNodeID);
          }
        }
      }

      AddNodeEntity(&*nodeIt, entityID);
      UpdateMinimalNodeGeometry(&*nodeIt, IGM::GetBoxAD(entityGeometry));

#ifdef ORTHOTREE__DEBUG_CHECKS
      assert(IsEveryEntityUnique()); // Assert means: index is already added. Wrong input!
#endif

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

      constexpr bool IS_ELEMENT_DEPTH_SPECIFIC = (EA::GEOMETRY_TYPE != GeometryType::Point);
      using LCAC = typename SI::template LowestCommonAncestorCalculator<IS_ELEMENT_DEPTH_SPECIFIC>;

      // Child nodes
      auto const childNodeLevelID = Base::GetMaxDepthID() - depthID - 1;
      while (beginIt != locationsZip.end())
      {
        auto const nodeChecker = typename SI::ChildCheckerFixedDepth(childNodeLevelID, beginIt.GetFirst()->GetLocationID());
        auto const nextIt =
          std::partition(beginIt, locationsZip.end(), [&](auto const& element) { return nodeChecker.Test(element.GetFirst().GetLocationID()); });

        // Create child node deeper if possible
        auto lcaHelper = LCAC(*beginIt.GetFirst());
        for (auto it = std::next(beginIt.GetFirst()); it != nextIt.GetFirst(); ++it)
          lcaHelper.Add(*it);

        auto lcaNodeID = lcaHelper.GetNodeID(Base::GetMaxDepthID());
        assert(
          lcaHelper.GetLocation(Base::GetMaxDepthID()).GetDepthID() >= depthID &&
          "Existing entities are out-of-sync with the tree. Client code may have modified the geometry of the existing entities without updating "
          "the tree.");

        auto childNodeIt = AddNode(lcaNodeID);
        auto childNodeValue = &*childNodeIt;

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

#ifdef ORTHOTREE__DEBUG_CHECKS
      assert(IsEveryEntityUnique()); // Assert means: index is already added. Wrong input!
#endif

      return true;
    }


    // Update id by the new bounding box information
    bool Update(EntityID entityID, EA::Geometry const& newEntityGeometry, InsertionMode insertionMode = InsertionMode::ExistingLeaf) noexcept
    {
      if constexpr (!CONFIG::ALLOW_OUT_OF_SPACE_INSERTION)
      {
        if (!IsGeometryInTree(newEntityGeometry, GA::BASE_TOLERANCE))
          return false;
      }

      if (!EraseBase<true>(entityID))
        return false;

      bool isInserted = InsertIntoLeaf(entityID, newEntityGeometry, insertionMode);
      if (!isInserted)
        ErasePostUpdate<false>(entityID); // Finish the erase if the update is failed due to out-of-space insertion.

      return isInserted;
    }

    // Update id by the new bounding box information and the erase part is aided by the old bounding box geometry data
    bool Update(
      EntityID entityID,
      EA::Geometry const& oldEntityGeometry,
      EA::Geometry const& newEntityGeometry,
      InsertionMode insertionMode = InsertionMode::ExistingLeaf) noexcept
    {
      if constexpr (CONFIG::USE_REVERSE_MAPPING)
      {
        return Update(entityID, newEntityGeometry, insertionMode);
      }
      else
      {
        if (!EraseBase<true>(entityID, oldEntityGeometry))
          return false; // entityID was not registered previously.

        bool isInserted = InsertIntoLeaf(entityID, newEntityGeometry, insertionMode);
        if (!isInserted)
          ErasePostUpdate<false>(entityID); // Finish the erase if the update is failed due to out-of-space insertion.

        return isInserted;
      }
    }

    // Update id with rebalancing by the new bounding box information
    bool Update(EntityID entityID, EA::Geometry const& newEntityGeometry, EntityContainerView entities) noexcept
    {
      if (!EraseBase<true>(entityID))
        return false;

      bool isInserted = Insert(entityID, newEntityGeometry, entities);
      if (!isInserted)
        ErasePostUpdate<false>(entityID); // Finish the erase if the update is failed due to out-of-space insertion.

      return isInserted;
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
        if (!EraseBase<true>(entityID, oldEntityGeometry))
          return false; // entityID was not registered previously.

        auto isInserted = Insert(entityID, newEntityGeometry, entities);
        if (!isInserted)
          ErasePostUpdate<false>(entityID); // Finish the erase if the update is failed due to out-of-space insertion.

        return isInserted;
      }
    }

    // Update all element which are in the given hash-table. Use with std::move
    template<typename TExecMode = SeqExec>
    void UpdateIndexes(
      std::unordered_map<EntityID, std::conditional_t<EA::ENTITY_ID_STRATEGY == EntityIdStrategy::ContiguousIndex, EntityID, std::optional<EntityID>>> updateMap,
      TExecMode execMode = {}) noexcept
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

          if constexpr (EA::ENTITY_ID_STRATEGY == EntityIdStrategy::ContiguousIndex)
          {
            entityIDs[i] = it->second;
          }
          else
          {
            if (it->second)
              entityIDs[i] = *it->second;
            else
            {
              --entityNo;
              entityIDs[i] = entityIDs[entityNo];
              --i;
            }
          }
          if constexpr (std::is_same_v<TExecMode, SeqExec>)
            updateMap.erase(it);
        }

        ResizeNodeEntities(node.second, entityNo);
      };

      if constexpr (CONFIG::USE_REVERSE_MAPPING)
      {
        auto nodes = std::unordered_set<NodeID>{};
        for (auto const [oldEntityID, newEntityID] : updateMap)
        {
          nodes.emplace(detail::at(m_reverseMap, oldEntityID));

          if (!newEntityID && EA::ENTITY_ID_STRATEGY == EntityIdStrategy::EntityKeyed)
            detail::erase(m_reverseMap, oldEntityID);
        }

        EXEC_POL_DEF(ep);
        std::for_each(EXEC_POL_ADD(ep) nodes.begin(), nodes.end(), [&](auto nodeID) { UpdateNodes(*m_nodes.find(nodeID)); });
      }
      else
      {
        EXEC_POL_DEF(ep);
        std::for_each(EXEC_POL_ADD(ep) m_nodes.begin(), m_nodes.end(), UpdateNodes);
      }

#ifdef ORTHOTREE__DEBUG_CHECKS
      assert(IsEveryEntityUnique()); // Assert means: index replacements causes that multiple object has the same id. Wrong input!
#endif
    }

    // Reset the tree
    void Reset() noexcept
    {
      m_nodes.clear();
      if constexpr (CONFIG::USE_REVERSE_MAPPING)
      {
        detail::clear(m_reverseMap);
      }
      m_memoryResource.Reset();
      m_spaceIndexing = {};
    }


    // Remove all elements and ids, except Root
    void Clear() noexcept
    {
      auto& rootNode = m_nodes.at(GetRootNodeID());
      rootNode.Clear();
      auto rootNodeCopy = rootNode;

      m_nodes.clear();
      m_nodes.emplace(GetRootNodeID(), std::move(rootNodeCopy));
    }


    // Move the whole tree with a std::vector of the movement
    template<typename TExecMode = SeqExec>
    void Move(TVector const& moveVector, TExecMode execMode = {}) noexcept
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
        m_spaceIndexing.Move(moveVector);
      }
      else
      {
        static_assert(false, "Unsupported NODE_GEOMETRY_STORAGE value!");
      }
    }

  private:
    void RemoveNodeIfPossible(auto nodeIt) noexcept
    {
      while (true)
      {
        auto& [nodeID, node] = *nodeIt;
        if (nodeID == GetRootNodeID())
          return;

        if (node.IsAnyChildExist() || !IsNodeEntitiesEmpty(&*nodeIt))
          return;

        m_memoryResource.Deallocate(node.GetEntitySegment());
        auto parentNodeIt = GetParentIt(nodeID);

        auto& [parentNodeID, parentNode] = *parentNodeIt;
        parentNode.UnlinkChild(SI::GetNonDirectChildID(parentNodeID, nodeID));
        m_nodes.erase(nodeIt);

        nodeIt = parentNodeIt;
      }
    }

    void DecreaseEntityIDs([[maybe_unused]] EntityID entityID) noexcept
    {
      if constexpr (EA::ENTITY_ID_STRATEGY == EntityIdStrategy::ContiguousIndex)
      {
        for (auto& [_, node] : m_nodes)
          node.DecreaseEntityIDs(entityID);

        if constexpr (CONFIG::USE_REVERSE_MAPPING && EA::ENTITY_ID_STRATEGY == EntityIdStrategy::ContiguousIndex)
          detail::decrementKeys(m_reverseMap, entityID);
      }
    }

    template<bool IS_PART_OF_UPDATE = false>
    constexpr void ErasePostUpdate(EntityID entityID) noexcept
    {
      if constexpr (IS_PART_OF_UPDATE)
      {
        if constexpr (CONFIG::USE_REVERSE_MAPPING)
        {
          if constexpr (EA::ENTITY_ID_STRATEGY == EntityIdStrategy::EntityKeyed)
            detail::erase(m_reverseMap, entityID);
        }
      }
      else
      {
        if constexpr (EA::ENTITY_ID_STRATEGY == EntityIdStrategy::ContiguousIndex)
          DecreaseEntityIDs(entityID);
        else if constexpr (CONFIG::USE_REVERSE_MAPPING && EA::ENTITY_ID_STRATEGY == EntityIdStrategy::EntityKeyed)
          detail::erase(m_reverseMap, entityID);
      }
    }

    template<bool IS_PART_OF_UPDATE = false>
    constexpr bool EraseBase(EntityID entityID) noexcept
    {
      bool isErased = false;

      if constexpr (CONFIG::USE_REVERSE_MAPPING)
      {
        auto pNodeID = detail::get_if(m_reverseMap, entityID);
        if (!pNodeID)
          return false;

        auto nodeIt = m_nodes.find(*pNodeID);
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

      ErasePostUpdate<IS_PART_OF_UPDATE>(entityID);
      return true;
    }

    template<bool IS_PART_OF_UPDATE = false>
    constexpr bool EraseBase(EntityID entitiyID, EA::Geometry const& entityGeometry) noexcept
    {
      auto nodeIt = GetNodeIt(entityGeometry);
      if (nodeIt == m_nodes.end())
        return false; // entity's geometry is not in the handled space

      // entityID may be in a parent node, so we need to check all the way up to the root
      while (!RemoveNodeEntity(nodeIt->second, entitiyID))
      {
        nodeIt = GetParentIt(nodeIt->first);
        if (nodeIt == m_nodes.end())
          return false; // entity is not in the tree
      }

      RemoveNodeIfPossible(nodeIt);
      ErasePostUpdate<IS_PART_OF_UPDATE>(entitiyID);
      return true;
    }

  public: // Entity handling
    // Erase entity via reverse mapping or brute force search. Reverse mapping is recommended.
    constexpr bool Erase(EntityID entityID) noexcept { return EraseBase(entityID); }

    // Erase id, aided with the original geometry. Reverse mapping is not used in this function, consider its usage, with the alternative Erase().
    constexpr bool Erase(EntityID entityID, EA::Geometry const& entityGeometry) noexcept { return EraseBase(entityID, entityGeometry); }

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
        else if constexpr (EA::GEOMETRY_TYPE == GeometryType::Box)
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
        return detail::get_if(m_reverseMap, entityID) != nullptr;
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
} // namespace OrthoTree
