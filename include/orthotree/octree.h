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
#include <vector>
#include <version>

#include "detail/bitset_arithmetic.h"
#include "detail/common.h"
#include "detail/configuration.h"
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

      Geometry m_geometry;

    public:
      explicit constexpr OrthoTreeNodeData() noexcept = default;

      constexpr Geometry const& GetGeometry() const noexcept { return m_geometry; }
      constexpr void SetGeometry(Geometry&& geometry) noexcept { m_geometry = std::move(geometry); }
      constexpr void SetGeometry(Geometry const& geometry) noexcept { m_geometry = geometry; }

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


  // TraverseControl is the result type of node-visitor functions' procedure.
  enum class TraverseControl
  {
    Terminate,    // Terminates the traverse
    SkipChildren, // Skips children nodes
    Continue      // Continues the traverse
  };

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

  protected:
    std::size_t m_maxElementNum = CONFIG::DEFAULT_TARGET_ELEMENT_NUM_IN_NODES;
    depth_t m_maxDepthID = {};

    std::vector<typename IGM::Vector> m_nodeLooseSizes;
    std::vector<typename IGM::Vector> m_nodeNominalSizes;

    detail::GridSpaceIndexing<GA> m_grid;

  public:
    using EntityContainerView = EA::EntityContainerView;
    using EntityID = EA::EntityID;

  protected:
    constexpr OrthoTreeCoreBase() = default;

  protected:
    constexpr void InitBase(IGM::Box const& boxSpace, depth_t maxDepthID, std::size_t maxElementNo) noexcept
    {
      ORTHOTREE_CRASH_IF(maxDepthID < 1, "maxDepthID must be largar than 0!");
      ORTHOTREE_CRASH_IF(maxDepthID > SI::MAX_THEORETICAL_DEPTH_ID, "maxDepthID is larger than the applicable with the current GA::DIMENSION_NO!");
      ORTHOTREE_CRASH_IF(maxDepthID >= std::numeric_limits<uint8_t>::max(), "maxDepthID is too large.");
      ORTHOTREE_CRASH_IF(maxElementNo == 0, "maxElementNo must be larger than 0. It is allowed max entity number for one node.");
      ORTHOTREE_CRASH_IF(CHAR_BIT * sizeof(GridID) < maxDepthID, "GridID and maxDepthID are not compatible.");

      m_grid = detail::GridSpaceIndexing<GA>(maxDepthID, boxSpace);
      m_maxDepthID = maxDepthID;
      m_maxElementNum = maxElementNo;

      // the 0-based depth size of the tree is m_maxDepthID+1, and a fictive childnode halfsize (+2) could be asked prematurely.
      depth_t constexpr additionalDepth = 3;
      auto const examinedDepthSize = m_maxDepthID + additionalDepth;
      m_nodeNominalSizes.resize(examinedDepthSize);
      m_nodeLooseSizes.resize(examinedDepthSize);
      m_nodeNominalSizes[0] = m_nodeLooseSizes[0] = m_grid.GetSizes();

      auto constexpr multiplier = IGM_Geometry(0.5);
      auto nominalfactor = IGM_Geometry(1.0);
      auto looseFactor = IGM_Geometry(CONFIG::LOOSE_FACTOR);

      for (depth_t depthID = 0; depthID < examinedDepthSize; ++depthID, nominalfactor *= multiplier, looseFactor *= multiplier)
        for (dim_t dimensionID = 0; dimensionID < GA::DIMENSION_NO; ++dimensionID)
        {
          m_nodeNominalSizes[depthID][dimensionID] = m_nodeNominalSizes[0][dimensionID] * nominalfactor;
          m_nodeLooseSizes[depthID][dimensionID] = m_nodeNominalSizes[0][dimensionID] * looseFactor;
        }
    }

  protected:
    template<bool HANDLE_OUT_OF_TREE_GEOMETRY = false>
    constexpr SI::Location GetLocation(TVector const& point) const noexcept
    {
      // TODO: handle out-of-tree geometry
      return SI::GetLocation(SI::Encode(m_grid.template GetPointGridID<HANDLE_OUT_OF_TREE_GEOMETRY>(point)), m_maxDepthID);
    }

    template<bool HANDLE_OUT_OF_TREE_GEOMETRY = false>
    constexpr SI::Location GetLocation(TBox const& box) const noexcept
    {
      // TODO: handle out-of-tree geometry

      if constexpr (CONFIG::LOOSE_FACTOR > 1.0)
      {
        auto const [centerGridID, levelID] = this->m_grid.template GetLooseBoxGridData<CONFIG::LOOSE_FACTOR, HANDLE_OUT_OF_TREE_GEOMETRY>(box);
        return SI::GetLocation(SI::Encode(centerGridID), m_maxDepthID - levelID);
      }
      else
      {
        return SI::GetRangeLocation(m_grid.template GetBoxGridID<HANDLE_OUT_OF_TREE_GEOMETRY>(box), m_maxDepthID);
      }
    }

  public:
    template<bool HANDLE_OUT_OF_TREE_GEOMETRY = false>
    SI::NodeID GetNodeID(TVector const& point) const noexcept
    {
      auto locationID = SI::Encode(m_grid.template GetPointGridID<HANDLE_OUT_OF_TREE_GEOMETRY>(point));
      return SI::GetNodeID(locationID, m_maxDepthID);
    }

    template<bool HANDLE_OUT_OF_TREE_GEOMETRY = false>
    SI::NodeID GetNodeID(TBox const& box) const noexcept
    {
      return SI::GetNodeID(GetLocation<HANDLE_OUT_OF_TREE_GEOMETRY>(box), m_maxDepthID);
    }

    constexpr depth_t GetExaminationLevelID(depth_t depthID) const { return m_maxDepthID - depthID; }


    constexpr IGM::Vector CalculateNodeSize(depth_t depthID) const noexcept { return IGM::Multiply(m_grid.GetSizes(), detail::pow2(depthID)); }

    // TODO: remove?
    constexpr IGM::Box GetNodeBox(depth_t depthID, auto const& geometry) const noexcept
    {
      if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MBR)
      {
        return { geometry.minPoint, IGM::Add(geometry.minPoint, geometry.size) };
      }
      else
      {
        typename IGM::Vector size;
        if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::None)
          size = CalculateNodeSize(depthID);
        else if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MinPoint)
          size = m_nodeLooseSizes[depthID];
        else
          static_assert(false, "Unsupported NodeGeometryStorage type!");

        return { .Min = geometry, .Max = IGM::Add(geometry, geometry.size) };
      }
    }

  public:
    constexpr auto GetMaxDepthID() const noexcept { return m_maxDepthID; }
    constexpr auto GetDepthNo() const noexcept { return m_maxDepthID + 1; }
    constexpr auto GetResolutionMax() const noexcept { return m_grid.GetResolution(); }
    constexpr auto const& GetBox() const noexcept { return m_grid.GetBoxSpace(); }

  protected:
    bool IsGeometryInTree(EA::Geometry const& entityGeometry) const noexcept
    {
      if constexpr (EA::GEOMETRY_TYPE == GeometryType::Point)
      {
        return IGM::DoesBoxContainPointAD(m_grid.GetBoxSpace(), entityGeometry);
      }
      else if constexpr (EA::GEOMETRY_TYPE == GeometryType::Box)
      {
        return IGM::DoesRangeContainBoxAD(m_grid.GetBoxSpace(), entityGeometry);
      }
      else
      {
        static_assert(false, "Unsupported geometry type!");
      }
    }
  };

#define ORTHOTREE_DEPENDENT_TYPES(Base)                \
  using EA = Base::EA;                                 \
  using GA = Base::GA;                                 \
  using CONFIG = typename Base::CONFIG;                \
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

    using ChildNodeSegment = std::conditional_t<GA::DIMENSION_NO <= 6, uint32_t, uint64_t>;
    using EntitySegment = std::pair<uint32_t, uint32_t>; // begin, end

  private:
    std::conditional_t<CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::None, std::monostate, std::vector<typename Base::NodeGeometry>> m_nodeGeometry;
    std::vector<uint8_t> m_nodeDepthIDs;
    std::vector<ChildNodeSegment> m_nodeChildSegments;
    std::vector<NodeID> m_nodeChildIDs;
    std::vector<EntitySegment> m_nodeEntities;
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
    static NodeID GetRootNodeID() noexcept { return 0; }

    constexpr std::size_t GetNodeCount() const noexcept { return m_nodeDepthIDs.size(); }
    constexpr std::size_t GetNodeEntityCount(NodeID nodeID) const noexcept { return m_nodeEntities[nodeID].second; }

    constexpr std::span<NodeID> GetNodeChildren(NodeID nodeID) noexcept
    {
      constexpr ChildNodeSegment shift = GA::DIMENSION_NO <= 6 ? 26 : (64 - GA::DIMENSION_NO);
      constexpr ChildNodeSegment beginMask = GA::DIMENSION_NO <= 6 ? ChildNodeSegment(0x02FFFFFF) : ChildNodeSegment((1ull << shift) - 1ull);

      uint32_t begin = m_nodeChildIDs[nodeID] & beginMask;
      uint32_t length = m_nodeChildIDs[nodeID] >> shift;
      return std::span<NodeID>(&m_nodeChildSegments[begin], length);
    }

    constexpr std::span<EntityID const> GetNodeEntities(NodeID nodeID) noexcept
    {
      auto const& segment = m_nodeEntities[nodeID];
      return { &m_entityStorage[segment.first], segment.second - segment.first };
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
        return Base::m_nodeLooseSizes[m_nodeDepthIDs[nodeID]];
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
    constexpr void InitBase(IGM::Box const& boxSpace, depth_t maxDepthID, std::size_t maxElementNo, std::size_t estimatedEntityNo) noexcept
    {
      Base::InitBase(boxSpace, maxDepthID, maxElementNo);

      ORTHOTREE_CRASH_IF(
        !m_nodeDepthIDs.empty(), "To build/setup/create the tree, use the Create() [recommended] or Init() function. If an already built tree is wanted to be reset, use the Reset() function before Init().");

      SetNodeGeometry(0, { 0, 0 });
      m_nodeDepthIDs.emplace_back();
      m_nodeChildSegments.emplace_back({});
      m_nodeEntities.emplace_back();
    }
    /*
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

      auto const subtreeEntityNum = detail::size(locationIt, endLocationIt);
      if (subtreeEntityNum == 0)
        return;

      auto nodeEntityNum = subtreeEntityNum;
      auto stuckedEndLocationIt = endLocationIt;
      if (subtreeEntityNum > Base::m_maxElementNum && depthID < Base::m_maxDepthID)
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

        nodeEntityNum = std::size_t(std::distance(locationIt, stuckedEndLocationIt));
      }

      if (nodeEntityNum == 0)
        return;

      node.second.ReplaceEntities(std::span(locationIt.GetSecond(), stuckedEndLocationIt.GetSecond()));
      locationIt += nodeEntityNum;
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
    */

  private:
    constexpr IGM::Vector CalculateNodeMinPoint(SI::Location location) const noexcept
    {
      // TODO: finish SI::Decode(location...
      return Base::m_grid.CalculateGridCellMinPoint(SI::Decode(location.GetLocationID(), Base::m_maxDepthID));
    }

    constexpr void SetNodeGeometry(NodeID nodeID, SI::Location location) const noexcept
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
        // TODO: inside-out box?
        return;
      }
      else
      {
        static_assert(false, "Unsupported node geometry storage!");
      }
    }

    static constexpr void IncreaseChildSegmentLength(NodeID nodeID) noexcept
    {
      constexpr ChildNodeSegment shift = GA::DIMENSION_NO <= 6 ? 26 : (64 - GA::DIMENSION_NO);
      constexpr ChildNodeSegment beginMask = GA::DIMENSION_NO <= 6 ? ChildNodeSegment(0x02FFFFFF) : ChildNodeSegment((1ull << shift) - 1ull);

      uint32_t begin = m_nodeChildSegments[nodeID] & beginMask;
      m_nodeChildSegments[nodeID] = (++(m_nodeChildSegments[nodeID] >> shift)) << shift | begin;
    }

    template<bool IS_ROOT>
    constexpr NodeID AttachNode(SI::Location location, uint32_t beginID, uint32_t length, NodeID parentNodeID)
    {
      auto const nodeID = static_cast<NodeID>(m_nodeChildSegments.size());
      m_nodeDepthIDs.emplace_back(location.GetDepthID());
      m_nodeEntities.emplace_back(beginID, length);
      m_nodeGeometry.emplace_back();
      SetNodeGeometry(nodeID, location);
      return nodeID;
    }

    // Build the tree in depth-first order
    template<bool ARE_LOCATIONS_SORTED, typename TResulEntityContainerView>
    constexpr void BuildSubtree(auto const& rootBeginLocationIt, auto const& rootEndLocationIt, NodeID rootNodeID) noexcept
    { /*
      using LocationIterator = typename detail::zip_view<std::vector<typename SI::Location>, std::span<EntityID>>::iterator;

      struct NodeProcessingData
      {
        SI::Location location;
        NodeID parentNodeID;
        uint32_t beginID = 0, length = 0;
      };

      uint32_t nonLeafNodeCount = 0;
      std::array<std::vector<NodeID>, SI::MAX_THEORETICAL_DEPTH> childNodeIDOnLevels;
      std::vector<uint8_t> childSegmentLengths;

      constexpr NodeID kInvalidNodeID = -1;
      auto nodeQueue = std::vector<NodeProcessingData>(64);
      nodeQueue[0] = NodeProcessingData{, kInvalidNodeID, 0 };

      auto locationIt = rootBeginLocationIt;
      while (!nodeQueue.empty())
      {
        auto& [location, parentNodeID, beginID, nodeEntityCount] = nodeQueue.back();
        nodeQueue.pop_back();

        auto const depthID = location.GetDepthID();
        if (nodeEntityCount <= Base::m_maxElementNum || depthID < Base::m_maxDepthID)
        {
          auto nodeID = AttachNode(location, beginID, nodeEntityCount);
          childNodeIDOnLevels[m_nodeDepthIDs[parentNodeID]].push_back(nodeID);
          ++childSegmentLengths[parentNodeID];
          // TODO: CalculateGeometry(location)
          continue;
        }

        auto nodeEntityNum = subtreeEntityNum;
        auto stuckedEndLocationIt = endLocationIt;
        if constexpr (EA::GEOMETRY_TYPE == GeometryType::Box)
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

          nodeEntityNum = detail::size<uint32_t>(locationIt, stuckedEndLocationIt));

          if (nodeEntityNum > 0)
          {
            // TODO: create node
            m_nodeEntities.push_back({ detail::size<uint32_t>(rootBeginLocationIt.GetSecond(), locationIt.GetSecond()), nodeEntityNum });
          }
        }

        if (nodeEntityNum == 0)
          return;

        node.second.ReplaceEntities(std::span(locationIt.GetSecond(), stuckedEndLocationIt.GetSecond()));
        locationIt += nodeEntityNum;

        auto const isEntitesProcessed = locationIt == nodeStack[depthID].EndLocationIt;

        if (isEntitesProcessed || depthID == Base::m_maxDepthID)
        {
          detail::emplace(nodes, std::move(nodeStack[depthID].NodeInstance));
          --depthID;
          continue;
        }

        ++depthID;
        auto const examinedLevelID = GetExaminationLevelID(depthID);
        auto const keyGenerator = typename SI::ChildKeyGenerator(nodeStack[depthID - 1].NodeInstance.first);
        auto pivotLocationIt = locationIt;
        while ()
        {
          auto const childChecker = typename SI::ChildCheckerFixedDepth(examinedLevelID, (*locationIt).GetFirst().locationID);
          auto const childID = childChecker.GetChildID(examinedLevelID);
          auto childKey = keyGenerator.GetChildNodeKey(childID);

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

          nodeProcessingData.NodeInstance.second = CreateNode(parentNodeProcessingData.NodeInstance.second, childKey);
          nodeProcessingData.NodeInstance.first = std::move(childKey);
        }
      }

      // flattening child segment lengths into m_nodeChildIDs
      m_nodeChildSegments.resize(nonLeafNodeCount);
      m_nodeChildIDs.resize(0);
      for (auto depthID = depth_t{ 0 }; depthID < childNodeIDOnLevels.size(); ++depthID)
      {
        auto& childNodeIDs = childNodeIDOnLevels[depthID];
        auto it = childNodeIDOnLevels[depthID].begin();

        std::copy()
        for (auto const childNodeID : childNodeIDs)
        {
          auto const parentNodeID = m_nodeDepthIDs[childNodeID] - 1;
          IncreaseChildSegmentLength(parentNodeID);
        }
      }
      */
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

      auto const entityNum = entities.size();
      auto const maxDepthID = (!maxDepthIn || maxDepthIn == depth_t{})
                              ? detail::EstimateMaxDepth<GA::DIMENSION_NO, SI::MAX_THEORETICAL_DEPTH_ID>(entityNum, maxElementNumInNode)
                              : *maxDepthIn;
      InitBase(boxSpace, maxDepthID, maxElementNumInNode, entityNum);

      if (entityNum == 0)
        return true;

      auto mortonIDs = std::vector<typename SI::Location>(entityNum);
      m_entityStorage.resize(entityNum);

      auto locationsZip = detail::zip_view(mortonIDs, m_entityStorage);
      using Location = decltype(locationsZip)::iterator::value_type;

      EXEC_POL_DEF(epf); // GCC 11.3
      std::transform(EXEC_POL_ADD(epf) entities.begin(), entities.end(), locationsZip.begin(), [&](auto const& entity) -> Location {
        return { GetLocation<ARE_ENTITIES_SURELY_IN_MODELSPACE>(EA::GetGeometry(entity)), EA::GetEntityID(entities, entity) };
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

      auto const estimatedNodeNum = detail::EstimateNodeNumber<GA::DIMENSION_NO>(entityNum, maxDepthID, maxElementNumInNode);
      m_nodeChildSegments.reserve(estimatedNodeNum);
      m_nodeChildIDs.reserve(estimatedNodeNum);
      m_nodeDepthIDs.reserve(estimatedNodeNum);
      m_nodeGeometry.reserve(estimatedNodeNum);
      m_nodeEntities.reserve(estimatedNodeNum);

      BuildSubtree<ARE_LOCATIONS_SORTED>(locationsZip.begin(), locationsZip.end(), GetRootNodeID());
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

    // TODO: remove
    // using MortonLocationID = typename SI::LocationID;
    // using MortonLocationIDCR = typename SI::LocationIDCR;
    // using MortonChildID = typename SI::ChildID;

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

    // TODO: add element indexing in nodes
    // TODO: using ReverseMapType = typename CONFIG::ReverseMap<EntityID, NodeID, typename EA::Hash>;
    using ReverseMapType = typename std::unordered_map<EntityID, NodeID, typename EA::Hash>;
    using ReverseMap = std::conditional_t<CONFIG::USE_REVERSE_MAPPING, ReverseMapType, std::monostate>;
    ReverseMap m_reverseMap;

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


  public: // Node helpers
    constexpr auto const& GetNodes() const noexcept { return m_nodes; }
    constexpr std::size_t GetNodeCount() const noexcept { return m_nodes.size(); }
    bool HasNode(NodeIDCR key) const noexcept { return m_nodes.contains(key); }
    static NodeID GetRootNodeID() noexcept { return SI::GetRootKey(); }
    auto const& GetNode(NodeIDCR key) const noexcept { return m_nodes.at(key); }

    NodeValueCP GetNodeValueP(NodeIDCR nodeID) const noexcept { return &(*m_nodes.find(nodeID)); }
    NodeValueCP GetRootNodeValue() const noexcept { return GetNodeValueP(SI::GetRootKey()); }

    // Get EntityIDs of the node
    constexpr auto const& GetNodeEntities(NodeValueCP pNodeValue) const noexcept { return pNodeValue->second.GetEntities(); }

    // Get EntityIDs number of the node
    constexpr std::size_t GetNodeEntityCount(NodeValueCP nodeValue) const noexcept { return nodeValue->second.GetEntitiesSize(); }

    // Get Node min-point
    constexpr decltype(auto) GetNodeMinPoint(NodeValueCP pNodeValue) const noexcept
    {
      if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::None)
        return CalculateNodeMinPoint(pNodeValue->first);
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
      if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::None)
        return CalculateNodeSize(depthID);
      else if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MinPoint)
        return Base::m_nodeLooseSizes[depthID];
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

    // Calculate Node min-point
    constexpr IGM::Vector CalculateNodeMinPoint(NodeIDCR nodeID) const noexcept
    {
      return Base::m_grid.CalculateGridCellMinPoint(SI::Decode(nodeID, Base::m_maxDepthID));
    }

    // TODO: remove these
    // Get EntityIDs number of the node
    constexpr std::size_t GetNodeEntityCount(NodeIDCR nodeKey) const noexcept { return GetNodeEntityCount(GetNode(nodeKey)); }

    // Is the node has any entity
    constexpr bool IsNodeEntitiesEmpty(Node const& node) const noexcept { return node.IsEntitiesEmpty(); }

    // Get EntityIDs of the node
    constexpr auto const& GetNodeEntities(Node const& node) const noexcept { return node.GetEntities(); }

    // Get EntityIDs of the node
    constexpr auto const& GetNodeEntities(NodeIDCR nodeKey) const noexcept { return GetNodeEntities(GetNode(nodeKey)); }

    // Get EntityIDs number of the node
    constexpr std::size_t GetNodeEntityCount(Node const& node) const noexcept { return node.GetEntitiesSize(); }

    // Obsolete: Is the node has any entity
    constexpr bool IsNodeEntitiesEmpty(NodeIDCR nodeKey) const noexcept { return IsNodeEntitiesEmpty(GetNode(nodeKey)); }

    // Obsolete: Calculate extent by box of the tree and the nodeID of the node
    constexpr IGM::Vector CalculateNodeCenter(NodeIDCR nodeID) const noexcept
    {
      return Base::m_grid.CalculateGridCellCenter(SI::Decode(nodeID, Base::m_maxDepthID), Base::m_maxDepthID - SI::GetDepthID(nodeID));
    }

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

    constexpr void SetNodeGeometry(NodeValue* nodeValue) const noexcept
    {
      if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MinPoint)
      {
        auto const nodeCenter = CalculateNodeCenter(nodeValue->first);
        auto const depthID = SI::GetDepthID(nodeValue->first);
        auto const& nodeHalfSize = Base::m_nodeLooseSizes[depthID + 1];

        nodeValue->second.SetGeometry(IGM::Sub(nodeCenter, nodeHalfSize));
      }
      else if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MBR)
      {
        nodeValue->second.SetGeometry({ .minPoint = { std::numeric_limits<TScalar>::max() }, .size = {} });
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
        SetNodeGeometry(&nodeValue);
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
      Base::InitBase(boxSpace, maxDepthID, maxElementNo);

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
      if (subtreeEntityNo > this->Base::m_maxElementNum && depthID < Base::m_maxDepthID)
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

        if (isEntitesProcessed || depthID == Base::m_maxDepthID)
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
      auto const maxDepthID = (!maxDepthIn || maxDepthIn == depth_t{})
                              ? detail::EstimateMaxDepth<GA::DIMENSION_NO, SI::MAX_THEORETICAL_DEPTH_ID>(entityNo, maxElementNoInNode)
                              : *maxDepthIn;
      InitBase(boxSpace, maxDepthID, maxElementNoInNode, entityNo);

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
      detail::reserve(this->m_nodes, detail::EstimateNodeNumber<GA::DIMENSION_NO, SI::MAX_THEORETICAL_DEPTH_ID>(entityNo, maxDepthID, maxElementNoInNode));
      BuildSubtree<ARE_LOCATIONS_SORTED>(locationsZip.begin(), locationsZip.end(), rootNode, this->m_nodes);
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
      detail::reserve(m_nodes, EstimateNodeNumber(entityNo, Base::m_maxDepthID, Base::m_maxElementNum));


      using EntityLocation = decltype(locationsZip)::iterator::value_type;
      EXEC_POL_DEF(ept); // GCC 11.3
      std::transform(EXEC_POL_ADD(ept) entities.begin(), entities.end(), locationsZip.begin(), [&](auto const& entity) -> EntityLocation {
        return { this->GetLocation(EA::GetGeometry(entity)), EA::GetEntityID(entities, entity) };
      });

      auto const partitions = Partitioning::Partition<std::min(dim_t(9), GA::DIMENSION_NO * 3), IS_PARALLEL_EXEC>(
        locationsZip.begin(),
        locationsZip.end(),
        [](EntityLocation const& e) -> NodeID { return e.first.locationID; },
        Base::m_maxElementNum,
        Base::m_maxDepthID* GA::DIMENSION_NO);

      auto orphanNodes = std::vector<NodeID>{};
      auto partitionIt = partitions.begin();
      for (auto beginIt = locationsZip.begin(); beginIt != locationsZip.end();)
      {
        /*
        auto const possibleEndIt = beginIt + std::min(detail::size(beginIt, locationsZip.end()), Base::m_maxElementNum);
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
        auto const minNodeID = SI::GetNodeID(*minIt, Base::m_maxDepthID);
        auto const maxNodeID = SI::GetNodeID(*maxIt, Base::m_maxDepthID);

        auto nodeID = SI::GetLowestCommonAncestor(minNodeID, maxNodeID);
        auto [it, isInserted] = this->m_nodes.try_emplace(nodeID);
        if (isInserted)
        {
          orphanNodes.push_back(nodeID);
          SetNodeGeometry(&*it);
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
            SetNodeGeometry(&*lcaIt);
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
      detail::reserve(m_nodes, EstimateNodeNumber(entityNo, Base::m_maxDepthID, Base::m_maxElementNum));


      using EntityLocation = decltype(locationsZip)::iterator::value_type;
      EXEC_POL_DEF(ept); // GCC 11.3
      std::transform(EXEC_POL_ADD(ept) entities.begin(), entities.end(), locationsZip.begin(), [&](auto const& entity) -> EntityLocation {
        return { this->GetLocation(EA::GetGeometry(entity)), EA::GetEntityID(entities, entity) };
      });

      auto orphanNodes = std::vector<NodeID>{};
      auto const maxElementNo = static_cast<uint32_t>(Base::m_maxElementNum);
      Partitioning::DepthFirstPartition<GA::DIMENSION_NO, EA::GEOMETRY_TYPE != GeometryType::Point, typename SI::Location>(
        EXEC_TAG,
        locationsZip.begin(),
        locationsZip.end(),
        SI::GetRootLocation(),
        Base::m_maxDepthID,
        maxElementNo,
        [&](auto beginIt, auto endIt, auto location, bool isForcedToFinish) {
          auto const elementNum = detail::size(beginIt, endIt);
          if (elementNum == 0)
            return true;

          if (!isForcedToFinish && elementNum > Base::m_maxElementNum && location.depthID < Base::m_maxDepthID)
            return false;

          auto nodeID = SI::GetNodeID(location, Base::m_maxDepthID);
          auto [it, isInserted] = this->m_nodes.try_emplace(nodeID);
          if (isInserted)
          {
            orphanNodes.push_back(nodeID);
            SetNodeGeometry(&*it);
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
            SetNodeGeometry(&*lcaIt);

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
      detail::reserve(m_nodes, EstimateNodeNumber(entityNo, Base::m_maxDepthID, Base::m_maxElementNum));

      auto orphanNodes = std::vector<NodeID>{};
      for (auto const& entity : entities)
      {
        auto nodeID = this->GetNodeID(EA::GetGeometry(entity));
        auto [it, isInserted] = this->m_nodes.try_emplace(nodeID);
        if (isInserted)
        {
          orphanNodes.push_back(nodeID);
          SetNodeGeometry(&*it);
        }

        AddNodeEntity(&*it, EA::GetEntityID(entities, entity));
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
          SetNodeGeometry(&*lcaIt);

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

      SetNodeGeometry(&*nodeIt);

      auto& [parentNodeID, parentNode] = *GetParentNode(nodeID);
      auto const childID = SI::GetChildID2(parentNodeID, nodeID);

      if (!parentNode.HasChild(childID))
      {
        parentNode.AddChild(childID, nodeID);
        return nodeIt;
      }

      auto const childNodeID = parentNode.GetChild(childID);
      auto const lcaNodeID = SI::GetLowestCommonAncestor(childNodeID, nodeID);
      parentNode.AddChild(childID, lcaNodeID);

      if (nodeID == lcaNodeID)
        nodeIt->second.AddChild(SI::GetChildID2(lcaNodeID, childNodeID), childNodeID);
      else
      {
        auto [lcaIt, _] = m_nodes.emplace(lcaNodeID, Node{});
        SetNodeGeometry(&*lcaIt);

        auto& lcaNode = lcaIt->second;
        lcaNode.AddChild(SI::GetChildID2(lcaNodeID, childNodeID), childNodeID);
        lcaNode.AddChild(SI::GetChildID2(lcaNodeID, nodeID), nodeID);
      }

      return nodeIt;
    }

    auto GetParentNode(NodeID nodeID)
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

    NodeID FindSmallestNodeKey(NodeID searchKey) const noexcept
    {
      for (; SI::IsValidKey(searchKey); searchKey = SI::GetParentKey(searchKey))
        if (this->m_nodes.contains(searchKey))
          return searchKey;

      return NodeID{}; // Not found
    }

    // Find smallest node which contains the box
    template<bool HANDLE_OUT_OF_TREE_GEOMETRY = false>
    NodeID FindSmallestNode(TVector const& searchPoint) const noexcept
    {
      if constexpr (!HANDLE_OUT_OF_TREE_GEOMETRY)
      {
        if (!IGM::DoesBoxContainPointAD(this->m_grid.GetBoxSpace(), searchPoint))
          return NodeID{};
      }
      return this->FindSmallestNodeKey(this->GetNodeID<HANDLE_OUT_OF_TREE_GEOMETRY>(searchPoint));
    }

    // Find smallest node which contains the box
    NodeID FindSmallestNode(TBox const& box) const noexcept
    {
      if (!IGM::DoesRangeContainBoxAD(this->m_grid.GetBoxSpace(), box))
        return NodeID{};

      return FindSmallestNodeKey(this->GetNodeID(box));
    }

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

    // Cascade update of node geometry by a new geometry
    void UpdateMinimalNodeGeometry(NodeValue* nodeValue, IGM::Box const& box)
    {
      if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MBR)
      {
        auto& [minPoint, size] = nodeValue->second.GetGeometry();

        bool isChanged = false;
        if (minPoint == std::numeric_limits<TScalar>::max())
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
          auto parentNodeIt = GetParentNode(nodeValue->first);
          if (parentNodeIt != m_nodes.end())
            UpdateMinimalNodeGeometry(&*parentNodeIt, box);
        }
      }
    }

    // Cascade  of node geometry
    void InitializeMinimalNodeGeometry(NodeValue* nodeValue, EntityContainerView entities, bool isCascadeMode = false)
    {
      if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MBR)
      {
        auto nodeBox = IGM::BoxInvertedInit();

        // Union with all entities
        auto const& entityIDs = GetNodeEntities(nodeValue);
        for (auto const& entityID : entityIDs)
          IGM::UniteInBoxAD(nodeBox, EA::GetGeometry(entities, entityID));

        // Union with all children
        auto const& children = GetNodeChildren(nodeValue);
        for (auto const& childNodeID : children)
        {
          const auto childBox = GetNodeBox(GetNodeValue(childNodeID));
          IGM::UniteInBoxAD(nodeBox, childBox);
        }

        // Initialize geometry
        nodeValue->second.GetGeometry() = { nodeBox.Min, IGM::Sub(nodeBox.Max, nodeBox.Min) };

        if (isCascadeMode)
        {
          auto parentNodeIt = GetParentNode(nodeValue->first);
          if (parentNodeIt != m_nodes.end())
          {
            UpdateMinimalNodeGeometry(&*parentNodeIt, nodeBox);
          }
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


    template<bool DO_UNIQUENESS_CHECK_TO_INDICIES>
    bool InsertWithRebalancingBase(NodeID entityNodeID, EntityID newEntityID, EntityContainerView entities) noexcept
    {
      // TODO: handle out-of-tree geometry

      // Rules:
      // 1. If the node has a child, it is already split
      // 2. Split nodes contain stucked boxes only, In other words: new elements that would be inserted in the node (but not stucked), if the node has
      // any child node, the new element should be inserted into a child.
      // 3. Rebalance push elements downward only once.

      auto nodeIt = m_nodes.find(entityNodeID);
      auto const isNonRefinable = nodeIt != m_nodes.end();
      if (nodeIt == m_nodes.end())
        nodeIt = GetParentNode(entityNodeID);

      auto const nodeValue = &*nodeIt;
      auto const entityCount = GetNodeEntityCount(nodeValue) + 1;
      auto const isOverflowed = entityCount > Base::m_maxElementNum;
      auto const isPreviouslyOverflowed = nodeValue->second.IsAnyChildExist();
      auto const depthID = SI::GetDepthID(nodeValue->first);

      if (isNonRefinable)
        UpdateMinimalNodeGeometry(nodeValue, IGM::GetBoxAD(EA::GetGeometry(entities, newEntityID)));

      // No overflow or non-refinable or max depth reached
      if ((!isPreviouslyOverflowed && !isOverflowed) || (isPreviouslyOverflowed && isNonRefinable) || depthID == Base::m_maxDepthID)
      {
        AddNodeEntity(nodeValue, newEntityID);
        return true;
      }

      // Previously overflowed but now non-overflowed. Just a direct child node creation is required.
      if (isPreviouslyOverflowed)
      {
        assert(!isNonRefinable);

        nodeIt = AddNode(SI::GetDirectChildNodeID(nodeValue->first, entityNodeID));
        AddNodeEntity(&*nodeIt, newEntityID);
        UpdateMinimalNodeGeometry(&*nodeIt, IGM::GetBoxAD(EA::GetGeometry(entities, newEntityID)));

        return true;
      }

      // Handling overflow

      assert(isOverflowed);

      auto const& originalEntityIDs = GetNodeEntities(nodeValue);

      auto mortonIDs = std::vector<typename SI::Location>(entityCount);
      auto entityIDs = std::vector<EntityID>{};
      entityIDs.reserve(entityCount);
      entityIDs.assign(originalEntityIDs.begin(), originalEntityIDs.end());
      entityIDs.emplace_back(newEntityID);

      auto locationsZip = detail::zip_view(mortonIDs, entityIDs);
      using ZipValue = decltype(locationsZip)::iterator::value_type;
      std::transform(entityIDs.begin(), entityIDs.end(), locationsZip.begin(), [&](auto const& entityID) -> ZipValue {
        return { Base::GetLocation(EA::GetGeometry(entities, entityID)), entityID };
      });

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
          AddNodeEntity(nodeValue, newEntityID);
          return true;
        }

        ClearNodeEntities(nodeValue->second);
        for (auto it = locationsZip.begin().GetSecond(); it != beginIt.GetSecond(); ++it)
          AddNodeEntity(nodeValue, *it);

        // Update parent node geometry once to avoid multiple updates
        UpdateMinimalNodeGeometry(nodeValue, IGM::GetBoxAD(EA::GetGeometry(entities, newEntityID)));
      }
      else
      {
        static_assert(false);
      }

      // Child nodes
      auto const childNodeLevelID = Base::m_maxDepthID - depthID - 1;
      while (beginIt != locationsZip.end())
      {
        auto const nodeChecker = typename SI::ChildCheckerFixedDepth(childNodeLevelID, beginIt.GetFirst()->GetLocationID());
        auto const nextIt =
          std::partition(beginIt, locationsZip.end(), [&](auto const& element) { return nodeChecker.Test(element.GetFirst().GetLocationID()); });

        // Create child node deeper if possible and worth it
        NodeValue* childNodeValue = nullptr;
        if (detail::size(beginIt, nextIt) >= Base::m_maxElementNum / 2)
        {
          auto lcaHelper = SI::GetLowestCommonAncestorHelper(*beginIt.GetFirst());
          for (auto it = beginIt.GetFirst(); it != nextIt.GetFirst(); ++it)
            lcaHelper.Add(*it);

          auto childNodeIt = AddNode(lcaHelper.GetNodeID(Base::m_maxDepthID));
          childNodeValue = &*childNodeIt;
        }
        else
        {
          auto childNodeIt = AddNode(SI::GetDirectChildNodeID(nodeValue->first, entityNodeID));
          childNodeValue = &*childNodeIt;
        }

        // Insert entities to the child node
        for (auto it = beginIt.GetSecond(); it != nextIt.GetSecond(); ++it)
        {
          AddNodeEntity(childNodeValue, *it);
          UpdateReverseMap(*it, childNodeValue->first);
        }

        // Update child node geometry. Parent node geometry is already updated, cascading is not required.
        InitializeMinimalNodeGeometry(childNodeValue, entities, false);

        beginIt = nextIt;
      }

      if constexpr (DO_UNIQUENESS_CHECK_TO_INDICIES)
        assert(IsEveryEntityUnique()); // Assert means: index is already added. Wrong input!

      return true;
    }


    template<bool DO_UNIQUENESS_CHECK_TO_INDICIES>
    bool InsertWithoutRebalancingBase(NodeIDCR entityNodeID, EntityID entityID, bool doInsertToLeaf) noexcept
    {
      auto nodeIt = m_nodes.find(entityNodeID);
      if (nodeIt == m_nodes.end())
      {
        if (doInsertToLeaf)
        {
          nodeIt = AddNode(entityNodeID);
        }
        else
        {
          nodeIt = GetParentNode(entityNodeID);
          if (nodeIt->second.IsAnyChildExist())
          {
            auto const childID = SI::GetChildID2(nodeIt->first, entityNodeID);
            auto const childGenerator = typename SI::ChildKeyGenerator(nodeIt->first);
            auto const childNodeID = childGenerator.GetChildNodeKey(childID);

            nodeIt = AddNode(childNodeID);
          }
        }
      }

      AddNodeEntity(&*nodeIt, entityID);

      if constexpr (DO_UNIQUENESS_CHECK_TO_INDICIES)
        assert(this->IsEveryEntityUnique()); // Assert means: index is already added. Wrong input!

      return true;
    }

    // Insert entity into the tree. If doInsertToLeaf is true: The smallest node will be chosen by the max depth. If doInsertToLeaf is false: The smallest existing level on the branch will be chosen.
    bool Insert(EntityID entityID, EA::Geometry const& entityGeometry, bool doInsertToLeaf = false) noexcept
    {
      // TODO: handle out-of-tree geometry

      if (!Base::IsGeometryInTree(entityGeometry))
      {
        if constexpr (CONFIG::ALLOW_OUT_OF_SPACE_INSERTION)
        {
          AddNodeEntity(&*m_nodes.find(SI::GetRootKey()), entityID);
          return true;
        }
        else
        {
          return false;
        }
      }

      return InsertWithoutRebalancingBase<true>(Base::GetNodeID(entityGeometry), entityID, doInsertToLeaf);
    }

    // Insert entity into the tree with node rebalancing
    bool InsertWithRebalancing(EntityID entityID, EA::Geometry const& entityGeometry, EntityContainerView entities) noexcept
    {
      if (!Base::IsGeometryInTree(entityGeometry))
      {
        if constexpr (CONFIG::ALLOW_OUT_OF_SPACE_INSERTION)
        {
          AddNodeEntity(&*m_nodes.find(SI::GetRootKey()), entityID);
          return true;
        }
        else
        {
          return false;
        }
      }

      auto const entityLocation = Base::GetLocation(entityGeometry);
      auto const entityNodeKey = SI::GetNodeID(entityLocation, Base::m_maxDepthID);
      return this->template InsertWithRebalancingBase<true>(entityNodeKey, entityID, entities);
    }


    // Update id by the new bounding box information
    bool Update(EntityID entityID, EA::Geometry const& newEntityGeometry, bool doInsertToLeaf = false) noexcept
    {
      if constexpr (!CONFIG::ALLOW_OUT_OF_SPACE_INSERTION)
      {
        if (!Base::IsGeometryInTree(newEntityGeometry))
        {
          return false;
        }
      }

      if (!EraseBase<false>(entityID))
        return false;

      return Insert(entityID, newEntityGeometry, doInsertToLeaf);
    }

    // Update id by the new bounding box information and the erase part is aided by the old bounding box geometry data
    bool Update(EntityID entityID, EA::Geometry const& oldEntityGeometry, EA::Geometry const& newEntityGeometry, bool doInsertToLeaf = false) noexcept
    {
      if constexpr (CONFIG::USE_REVERSE_MAPPING)
      {
        return Update(entityID, newEntityGeometry, doInsertToLeaf);
      }
      else
      {
        auto oldEntityNodeID = SI::GetRootKey();
        if (!Base::IsGeometryInTree(oldEntityGeometry))
        {
          if constexpr (!CONFIG::ALLOW_OUT_OF_SPACE_INSERTION)
            return false;
        }
        else
        {
          oldEntityNodeID = FindSmallestNode(oldEntityGeometry);
        }

        auto const isNewGeometryInTree = Base::IsGeometryInTree(newEntityGeometry);
        if (isNewGeometryInTree)
        {
          if (oldEntityNodeID == FindSmallestNode(newEntityGeometry))
            return true; // No update is required.
        }

        if (!EraseBase<false>(entityID, oldEntityGeometry))
          return false; // entityID was not registered previously.

        if (!isNewGeometryInTree)
        {
          if constexpr (!CONFIG::ALLOW_OUT_OF_SPACE_INSERTION)
            return false;
        }

        return Insert(entityID, newEntityGeometry, doInsertToLeaf);
      }
    }

    // Update id with rebalancing by the new bounding box information
    bool Update(EntityID entityID, EA::Geometry const& newEntityGeometry, EntityContainerView entities) noexcept
    {
      if (!EraseBase<false>(entityID))
        return false;

      return InsertWithRebalancing(entityID, newEntityGeometry, entities);
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
        auto oldEntityNodeID = SI::GetRootKey();
        if (Base::IsGeometryInTree(oldEntityGeometry))
          oldEntityNodeID = FindSmallestNode(oldEntityGeometry);
        else
        {
          if constexpr (!CONFIG::ALLOW_OUT_OF_SPACE_INSERTION)
            return false;
        }

        auto const isNewGeometryInTree = Base::IsGeometryInTree(newEntityGeometry);
        if (isNewGeometryInTree)
        {
          if (oldEntityNodeID == FindSmallestNode(newEntityGeometry))
            return true; // No update is required.
        }

        if (!EraseBase<false>(entityID, oldEntityGeometry))
          return false; // entityID was not registered previously.

        if (!isNewGeometryInTree)
        {
          if constexpr (!CONFIG::ALLOW_OUT_OF_SPACE_INSERTION)
            return false;
        }

        return InsertWithRebalancing(entityID, newEntityGeometry, entities);
      }
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
      m_memoryResource.Reset();
      Base::m_grid = {};
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
            auto minPoint = pairKeyNode.second.GetGeometry();
            IGM::MoveAD(minPoint, moveVector);
            pairKeyNode.second.SetGeometry(std::move(minPoint));
          }
          else if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MBR)
          {
            auto box = pairKeyNode.second.GetGeometry();
            IGM::MoveAD(box.minPoint, moveVector);
            pairKeyNode.second.SetGeometry(std::move(box));
          }
        });
      }
      else
      {
        static_assert(false, "Unsupported NODE_GEOMETRY_STORAGE value!");
      }

      Base::m_grid.Move(moveVector);
    }

  private:
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
        auto parentNodeIt = GetParentNode(nodeID);

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
      auto nodeID = FindSmallestNode(entityGeometry);
      if (!SI::IsValidKey(nodeID))
        return false; // old box is not in the handled space domain

      for (; SI::IsValidKey(nodeID); nodeID = SI::GetParentKey(nodeID))
      {
        auto nodeIt = m_nodes.find(nodeID);
        if (nodeIt == m_nodes.end())
          continue;

        bool const isEntityRemoved = RemoveNodeEntity(nodeIt->second, entitiyID);
        if (isEntityRemoved)
        {
          RemoveNodeIfPossible(nodeIt);
          break;
        }
      }

      if (!SI::IsValidKey(nodeID))
        return false; // id was not registered previously.

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
      auto const smallestNodeKey = this->FindSmallestNode(geometry);
      if (!SI::IsValidKey(smallestNodeKey))
        return false;

      auto const& nodeEntityIDs = GetNodeEntities(smallestNodeKey);
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
        if (entities.size() == 0)
          return false;

        auto const& entityGeometry = EA::GetGeometry(entities, entityID);
        return Contains(entityGeometry, entities, tolerance);
      }
    }
  };

  // OrthoTree: Non-owning Base container which spatially organize data ids in N dimension space into a hash-table by Morton Z order.
  template<typename TOrthoTreeCore>
  class OrthoTreeQueryBase : public TOrthoTreeCore
  {
  public:
    using Core = TOrthoTreeCore;
    ORTHOTREE_DEPENDENT_TYPES(Core);


  public:
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
    bool InsertUnique(
      EntityID entityID,
      EA::Geometry const& entityGeometry,
      EntityContainerView entities,
      TFloatScalar tolerance = GA::BASE_TOLERANCE,
      bool doInsertToLeaf = false)
    // requires(std::is_same_v<TOrthoTreeCore, DynamicHashOrthoTreeCore<Core::EA, Core::GA, Core::CONFIG>>)
    {
      auto const nearestEntityList = GetNearestNeighbors(entityGeometry, 1, 0.0, entities, tolerance);
      if (!nearestEntityList.empty())
        return false;

      return doInsertToLeaf ? Core::Insert(entityID, entityGeometry, true) : Core::InsertWithRebalancing(entityID, entityGeometry, entities);
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
    std::vector<EntityID> GetEntitiesDepthFirst(NodeIDCR parentKey = SI::GetRootKey(), bool shouldSortInsideNodes = false) const noexcept
    {
      auto entityIDs = std::vector<EntityID>{};
      CollectAllEntitiesInDFSRecursive(Core::GetNodeValueP(parentKey), entityIDs, shouldSortInsideNodes);
      return entityIDs;
    }

  public: // Search functions
    // Pick search
    std::vector<EntityID> PickSearch(TVector const& pickPoint, EntityContainerView entities, TFloatScalar tolerance = GA::BASE_TOLERANCE) const noexcept
    {
      // TODO: entity tester
      auto foundEntitiyIDs = std::vector<EntityID>();
      if (!IGM::DoesBoxContainPointAD(Core::m_grid.GetBoxSpace(), pickPoint))
        return foundEntitiyIDs;

      foundEntitiyIDs.reserve(100);

      TraverseNodesDepthFirst([&](auto const nodeValue) {
        // TODO: use tolerance here also ?
        if (!IGM::DoesBoxContainPointAD(Core::GetNodeMinPoint(nodeValue), Core::GetNodeSize(nodeValue), pickPoint))
          return TraverseControl::SkipChildren;

        auto const& nodeEntityIDs = Core::GetNodeEntities(nodeValue);
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
    template<bool DO_RANGE_MUST_FULLY_CONTAIN = true>
    std::vector<EntityID> RangeSearch(TBox const& range, EntityContainerView entities) const noexcept
    {
      // TODO: entity tester
      // TODO: tolerance?

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

      auto const searchRangeMinPoint = IGM::GetBoxMinPointAD(range);
      auto const searchRangeSize = IGM::GetBoxSizeAD(range);
      TraverseNodesDepthFirst([&](auto const pNodeValue) {
        auto const& nodeMinPoint = Core::GetNodeMinPoint(pNodeValue);
        auto const& nodeSize = Core::GetNodeSize(pNodeValue);
        if (!IGM::AreBoxesOverlappingByMinPoint(searchRangeMinPoint, searchRangeSize, nodeMinPoint, nodeSize))
          return TraverseControl::SkipChildren;

        if (IGM::DoesRangeContainBoxAD(range, nodeMinPoint, nodeSize))
        {
          TraverseNodesDepthFirst(
            [&](auto const pChildNodeValue) {
              std::ranges::copy(Core::GetNodeEntities(pChildNodeValue), std::back_inserter(foundEntities));
              return TraverseControl::Continue;
            },
            pNodeValue);

          return TraverseControl::SkipChildren;
        }

        std::ranges::copy_if(Core::GetNodeEntities(pNodeValue), std::back_inserter(foundEntities), entityFilter);

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
        return IGM::GetBoxPlaneRelationAD(IGM::GetBoxMinPointAD(entityGeometry), IGM::GetBoxSizeAD(entityGeometry), distanceOfOrigo, planeNormal, tolerance);
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
          if (!trees[sideID]->IsNodeEntitiesEmpty(parentNodePair[sideID].pNodeValue->second))
            childNodes[sideID].push_back({ parentNodePair[sideID].pNodeValue, true });


        // Cartesian product of childNodes left and right
        for (auto const& leftChildNode : childNodes[Left])
          for (auto const& rightChildNode : childNodes[Right])
            if (!(leftChildNode.pNodeValue == parentNodePair[Left].pNodeValue && rightChildNode.pNodeValue == parentNodePair[Right].pNodeValue))
              if (IGM::AreBoxesOverlappingByMinPoint(
                    trees[Left]->Core::GetNodeMinPoint(leftChildNode.pNodeValue),
                    trees[Left]->Core::GetNodeSize(leftChildNode.pNodeValue),
                    trees[Right]->Core::GetNodeMinPoint(rightChildNode.pNodeValue),
                    trees[Right]->Core::GetNodeSize(rightChildNode.pNodeValue)))
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
        leftTree.Core::GetNodeValueP(SI::GetRootKey()),
        rightTree,
        rightEntities,
        rightTree.Core::GetNodeValueP(SI::GetRootKey()),
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

            if (GA::AreBoxesOverlappedStrict(entityBoxI, entityBoxJ))
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
          if (!IGM::AreBoxesOverlappingByMinPoint(nodeMinPoint, nodeSize, parentEntityMinPoint, parentEntitySize))
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

              // TODO: Add tolerance-based box-box overlap check
              if (GA::AreBoxesOverlappedStrict(entityGeometry, parentEntityGeometry))
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
          entities, comparator, Core::GetNodeValueP(SI::GetRootKey()), nodeContextStack, collidedEntities, tolerance, collisionDetector);

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
            entities, comparator, Core::GetNodeValueP(SI::GetRootKey()), nodeContextStack, collidedEntities, tolerance, collisionDetector);

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
        nodeQueue.push_back(NodeData{ Core::GetNodeValueP(SI::GetRootKey()), INVALID_INDEX, INVALID_INDEX });

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
            entities, comparator, Core::GetNodeValueP(SI::GetRootKey()), nodeContextStack, collidedEntities, tolerance, collisionDetector);

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
