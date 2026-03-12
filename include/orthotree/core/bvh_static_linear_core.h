#pragma once

#include "../adapters/general.h"
#include "../core/entity_adapter.h"
#include "../core/ot_query.h"
#include "../detail/internal_geometry_module.h"
#include "../detail/sequence_view.h"
#include "types.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <limits>
#include <queue>
#include <span>
#include <stack>
#include <variant>
#include <vector>

namespace OrthoTree
{
  template<int CHILD_NUM_ = 2>
  struct BVHConfiguration
  {
    static constexpr int CHILD_NUM = CHILD_NUM_;
    static constexpr std::size_t DEFAULT_TARGET_ELEMENT_NUM_IN_NODES = 16;

    // Required by OrthoTreeQueryBase
    static constexpr bool IS_HOMOGENEOUS_GEOMETRY = true;
    static constexpr double LOOSE_FACTOR = 1.0;
    static constexpr bool ALLOW_OUT_OF_SPACE_INSERTION = true;
    static constexpr bool USE_REVERSE_MAPPING = false;
    static constexpr depth_t MAX_ALLOWED_DEPTH_ID = depth_t{ 19 };

    template<typename TKey, typename TValue, typename THash>
    using UMapNodeContainer = std::unordered_map<TKey, TValue, THash>;
    template<typename TKey, typename TValue, typename TComp>
    using MapNodeContainer = std::map<TKey, TValue, TComp>;
  };

  template<typename TEntityAdapter, typename TGeometryAdapter, typename TConfiguration>
  class BVHStaticLinearCore
  {
  public:
    using EA = TEntityAdapter;
    using GA = TGeometryAdapter;
    using CONFIG = TConfiguration;
    using IGM = detail::InternalGeometryModule<TGeometryAdapter>;

    using NodeID = int;
    using NodeIDCR = NodeID;
    using NodeValue = NodeID;
    using ChildID = NodeID;

    using EntityID = EA::EntityID;
    using EntityGeometry = EA::Geometry;

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
      using NodeSegmentIndex = uint8_t;
      using EntitySegment = Segment<uint8_t, uint8_t>;

      std::vector<NodeSegmentIndex> nodeChildSegmentBegins;
      std::vector<EntitySegment> nodeEntitySegment;
    };

    struct NodeStorage65536
    {
      using NodeSegmentIndex = uint16_t;
      using EntitySegment = Segment<uint16_t, uint16_t>;

      std::vector<NodeSegmentIndex> nodeChildSegmentBegins;
      std::vector<EntitySegment> nodeEntitySegment;
    };

    struct NodeStorageGeneral
    {
      using NodeSegmentIndex = uint32_t;
#ifdef ORTHOTREE__LARGE_DATASET
      using EntitySegment = Segment<uint64_t, uint64_t>;
#else
      using EntitySegment = Segment<uint32_t, uint32_t>;
#endif

      std::vector<NodeSegmentIndex> nodeChildSegmentBegins;
      std::vector<EntitySegment> nodeEntitySegment;
    };

    struct NodeGeometryData
    {
      typename IGM::Vector minPoint;
      typename IGM::Vector size;
    };

  private:
    std::variant<NodeStorage256, NodeStorage65536, NodeStorageGeneral> m_nodes;
    std::vector<uint8_t> m_nodeDepthIDs;
    std::vector<EntityID> m_entityStorage;
    std::vector<NodeGeometryData> m_nodeGeometry;
    depth_t m_maxDepthNo;

  public:
    // Default constructor. Requires Create call before usage.
    constexpr BVHStaticLinearCore() = default;

    // Initialize the base octree structure with entity collection
    template<typename TExecMode = SeqExec>
    explicit BVHStaticLinearCore(
      EA::EntityContainerView entities, std::size_t maxElementNoInNode = CONFIG::DEFAULT_TARGET_ELEMENT_NUM_IN_NODES, TExecMode execMode = {}) noexcept
    {
      [[maybe_unused]] auto isSuccessfullyInsertedAllElements = Create(entities, maxElementNoInNode, execMode);
      assert(isSuccessfullyInsertedAllElements);
    }

    template<typename TExecMode = SeqExec>
    explicit BVHStaticLinearCore(
      TExecMode execMode, EA::EntityContainerView entities, std::size_t maxElementNoInNode = CONFIG::DEFAULT_TARGET_ELEMENT_NUM_IN_NODES) noexcept
    {
      [[maybe_unused]] auto isSuccessfullyInsertedAllElements = Create(entities, maxElementNoInNode, execMode);
      assert(isSuccessfullyInsertedAllElements);
    }


    // Node API
#ifdef ORTHOTREE__PUBLIC_NODE_INTERFACE
  public:
#else
  protected:
#endif
    static NodeID GetRootNodeID() noexcept { return 0; }

    constexpr NodeID GetRootNodeValue() const noexcept { return GetRootNodeID(); }

    constexpr NodeID GetNodeValue(NodeID nodeID) const noexcept { return nodeID; }

    constexpr std::size_t GetNodeCount() const noexcept { return m_nodeDepthIDs.size(); }

    constexpr std::size_t GetNodeEntityCount(NodeValue nodeID) const noexcept
    {
      return std::visit([&](auto const& nodes) -> std::size_t { return static_cast<std::size_t>(nodes.nodeEntitySegment[nodeID].length); }, m_nodes);
    }

    constexpr SequenceView<NodeID> GetNodeChildren(NodeValue nodeID) const noexcept
    {
      return std::visit(
        [nodeID](auto const& nodes) {
          if (nodeID >= nodes.nodeChildSegmentBegins.size() - 1)
            return SequenceView<NodeID>(static_cast<NodeID>(0), static_cast<NodeID>(0));

          return SequenceView<NodeID>(
            static_cast<NodeID>(nodes.nodeChildSegmentBegins[nodeID]), static_cast<NodeID>(nodes.nodeChildSegmentBegins[nodeID + 1]));
        },
        m_nodes);
    }

    constexpr std::span<EntityID const> GetNodeEntities(NodeValue nodeID) const noexcept
    {
      return std::visit(
        [this, nodeID](auto const& nodes) {
          auto const [beginID, length] = nodes.nodeEntitySegment[nodeID];
          return std::span<EntityID const>(&m_entityStorage[beginID], length);
        },
        m_nodes);
    }

    constexpr bool IsNodeEntitiesEmpty(NodeValue nodeID) const noexcept
    {
      return std::visit([nodeID](auto const& nodes) { return nodes.nodeEntitySegment[nodeID].length == 0; }, m_nodes);
    }

    constexpr decltype(auto) GetNodeMinPoint(NodeValue nodeID) const noexcept { return m_nodeGeometry[nodeID].minPoint; }

    constexpr decltype(auto) GetNodeSize(NodeValue nodeID) const noexcept { return m_nodeGeometry[nodeID].size; }

    constexpr typename IGM::Box GetNodeBox(NodeValue nodeID) const noexcept
    {
      auto const& [minPoint, size] = m_nodeGeometry[nodeID];
      return { .Min = minPoint, .Max = IGM::Add(minPoint, size) };
    }

    constexpr depth_t GetMaxDepthID() const { return m_maxDepthNo - 1; }

    constexpr depth_t GetDepthNo() const { return m_maxDepthNo; }

  private:
    static constexpr int CHILD_NUM = CONFIG::CHILD_NUM;
    static constexpr int SAH_BIN_COUNT = 16;

    struct EntityBuildData
    {
      EntityID entityID;
      IGM::Vector center;
    };

    // Evaluate SAH cost for splitting along a given axis at a given position
    // Returns the cost = leftArea * leftCount + rightArea * rightCount
    struct SplitCandidate
    {
      typename IGM::Geometry cost = std::numeric_limits<typename IGM::Geometry>::max();
      typename IGM::Geometry splitPos = {};
      dim_t axis = 0;
    };

    static SplitCandidate FindBestSplitAxis(std::span<EntityBuildData> entities, EA::EntityContainerView sourceEntities) noexcept
    {
      using Geometry = typename IGM::Geometry;

      struct Bin
      {
        uint32_t count = 0;
        typename IGM::Box bounds;
        bool initialized = false;
      };

      SplitCandidate best;

      for (dim_t dimID = 0; dimID < GA::DIMENSION_NO; ++dimID)
      {
        // Find bounds of centers along this axis
        Geometry boundsMin = std::numeric_limits<Geometry>::max();
        Geometry boundsMax = std::numeric_limits<Geometry>::lowest();
        for (auto const& ed : entities)
        {
          boundsMin = std::min(boundsMin, ed.center[dimID]);
          boundsMax = std::max(boundsMax, ed.center[dimID]);
        }

        if (boundsMax - boundsMin < std::numeric_limits<Geometry>::epsilon())
          continue;

        // Populate bins
        std::array<Bin, SAH_BIN_COUNT> bins = {};
        Geometry const scale = Geometry(SAH_BIN_COUNT) / (boundsMax - boundsMin);
        for (auto const& ed : entities)
        {
          int binIdx = std::min(SAH_BIN_COUNT - 1, static_cast<int>((ed.center[dimID] - boundsMin) * scale));
          ++bins[binIdx].count;
          auto entityBox = IGM::GetBoxAD(EA::GetGeometry(sourceEntities, ed.entityID));
          if (!bins[binIdx].initialized)
          {
            bins[binIdx].bounds = entityBox;
            bins[binIdx].initialized = true;
          }
          else
          {
            IGM::UniteInBoxAD(bins[binIdx].bounds, entityBox);
          }
        }

        // Sweep from left and right to compute prefix areas and counts
        std::array<Geometry, SAH_BIN_COUNT - 1> leftArea, rightArea;
        std::array<uint32_t, SAH_BIN_COUNT - 1> leftCount, rightCount;

        {
          typename IGM::Box leftBox = IGM::BoxInvertedInit();
          uint32_t leftSum = 0;
          for (int i = 0; i < SAH_BIN_COUNT - 1; ++i)
          {
            leftSum += bins[i].count;
            leftCount[i] = leftSum;
            if (bins[i].initialized)
              IGM::UniteInBoxAD(leftBox, bins[i].bounds);
            leftArea[i] = IGM::GetVolumeAD(leftBox);
          }
        }
        {
          typename IGM::Box rightBox = IGM::BoxInvertedInit();
          uint32_t rightSum = 0;
          for (int i = SAH_BIN_COUNT - 1; i > 0; --i)
          {
            rightSum += bins[i].count;
            rightCount[i - 1] = rightSum;
            if (bins[i].initialized)
              IGM::UniteInBoxAD(rightBox, bins[i].bounds);
            rightArea[i - 1] = IGM::GetVolumeAD(rightBox);
          }
        }

        // Evaluate SAH cost for each split plane
        Geometry const binWidth = (boundsMax - boundsMin) / Geometry(SAH_BIN_COUNT);
        for (int i = 0; i < SAH_BIN_COUNT - 1; ++i)
        {
          if (leftCount[i] == 0 || rightCount[i] == 0)
            continue;

          Geometry cost = leftCount[i] * leftArea[i] + rightCount[i] * rightArea[i];
          if (cost < best.cost)
          {
            best.cost = cost;
            best.splitPos = boundsMin + binWidth * Geometry(i + 1);
            best.axis = dimID;
          }
        }
      }

      return best;
    }


    constexpr NodeID CreateBVHNode(uint8_t depthID, uint32_t nodeEntityBeginID, uint32_t nodeEntityCount, uint32_t childNodeCount) noexcept
    {
      auto const nodeID = static_cast<NodeID>(m_nodeDepthIDs.size());

      m_nodeDepthIDs.push_back(depthID);
      m_maxDepthNo = std::max<depth_t>(m_maxDepthNo, depthID + 1);

      std::visit(
        [&](auto& nodes) {
          using NodeStorage = std::decay_t<decltype(nodes)>;
          using EntitySegment = typename NodeStorage::EntitySegment;
          using NodeSegmentIndex = typename NodeStorage::NodeSegmentIndex;

          nodes.nodeEntitySegment.push_back(
            EntitySegment{ static_cast<typename EntitySegment::Begin>(nodeEntityBeginID), static_cast<typename EntitySegment::Length>(nodeEntityCount) });

          if (childNodeCount > 0)
          {
            nodes.nodeChildSegmentBegins.resize(nodeID + 2, nodes.nodeChildSegmentBegins.back());
            nodes.nodeChildSegmentBegins.back() += static_cast<NodeSegmentIndex>(childNodeCount);
          }
        },
        m_nodes);

      if constexpr (!std::is_same_v<decltype(m_nodeGeometry), std::monostate>)
      {
        m_nodeGeometry.push_back({});
      }

      return nodeID;
    }


    void BuildBVH(std::vector<EntityBuildData>& buildData, EA::EntityContainerView entities, uint8_t maxDepthID, std::size_t maxElementNoInNode) noexcept
    {
      struct NodeProcessingData
      {
        uint32_t beginID;
        uint32_t length;
        uint8_t depthID;
      };

      auto nodeQueue = std::queue<NodeProcessingData>();
      nodeQueue.push({ 0, static_cast<uint32_t>(buildData.size()), 0 });

      while (!nodeQueue.empty())
      {
        auto [beginID, length, depthID] = nodeQueue.front();
        nodeQueue.pop();

        // Leaf condition: few enough entities or max depth reached
        if (length <= maxElementNoInNode || depthID >= maxDepthID)
        {
          CreateBVHNode(depthID, beginID, length, 0);
          continue;
        }

        // Find best split axis using SAH (quick check: is any split possible at all?)
        auto fullSpan = std::span<EntityBuildData>(buildData.data() + beginID, length);
        {
          auto quickCheck = FindBestSplitAxis(fullSpan, entities);
          if (quickCheck.cost >= std::numeric_limits<typename IGM::Geometry>::max())
          {
            CreateBVHNode(depthID, beginID, length, 0);
            continue;
          }
        }

        // Greedy recursive binary SAH splitting for multi-way BVH.
        // Iteratively pick the sub-range with the most entities and binary-split it
        // using a fresh SAH evaluation, until we have CHILD_NUM sub-ranges.
        struct SubRange
        {
          uint32_t beginID;
          uint32_t length;
        };

        std::array<SubRange, CHILD_NUM> subRanges;
        int rangeCount = 1;
        subRanges[0] = { beginID, length };

        while (rangeCount < CHILD_NUM)
        {
          // Find the sub-range with the most entities
          int bestIdx = 0;
          for (int i = 1; i < rangeCount; ++i)
            if (subRanges[i].length > subRanges[bestIdx].length)
              bestIdx = i;

          auto& r = subRanges[bestIdx];
          if (r.length <= 1)
            break;

          auto subSpan = std::span<EntityBuildData>(buildData.data() + r.beginID, r.length);
          auto subSplit = FindBestSplitAxis(subSpan, entities);

          if (subSplit.cost >= std::numeric_limits<typename IGM::Geometry>::max())
            break;

          // Sort this sub-range along the chosen axis
          std::sort(subSpan.begin(), subSpan.end(), [axis = subSplit.axis](auto const& a, auto const& b) { return a.center[axis] < b.center[axis]; });

          // Find partition point using the SAH split position
          auto partIt =
            std::partition_point(subSpan.begin(), subSpan.end(), [&](auto const& ed) { return ed.center[subSplit.axis] < subSplit.splitPos; });
          auto leftCount = static_cast<uint32_t>(partIt - subSpan.begin());

          // Ensure at least 1 entity on each side
          leftCount = std::clamp(leftCount, uint32_t(1), r.length - 1);

          // Replace current range with left half, append right half
          uint32_t origBegin = r.beginID;
          uint32_t origLength = r.length;
          r = { origBegin, leftCount };
          subRanges[rangeCount] = { origBegin + leftCount, origLength - leftCount };
          ++rangeCount;
        }

        // Sort sub-ranges by beginID to maintain contiguous order for BFS
        std::sort(subRanges.begin(), subRanges.begin() + rangeCount, [](auto const& a, auto const& b) { return a.beginID < b.beginID; });

        // Enqueue children
        for (int i = 0; i < rangeCount; ++i)
          nodeQueue.push({ subRanges[i].beginID, subRanges[i].length, static_cast<uint8_t>(depthID + 1) });

        // Create the internal node (entities stored in this node = 0, all pushed to children)
        CreateBVHNode(depthID, beginID, 0, static_cast<uint32_t>(rangeCount));
      }
    }


    void InitializeNodeGeometryBVH(EA::EntityContainerView entities) noexcept
    {
      if constexpr (std::is_same_v<decltype(m_nodeGeometry), std::monostate>)
        return;
      else
      {
        auto const nodeCount = static_cast<uint32_t>(m_nodeDepthIDs.size());
        if (nodeCount == 0)
          return;

        // Find max depth
        uint8_t maxDepth = 0;
        for (uint32_t i = 0; i < nodeCount; ++i)
          maxDepth = std::max(maxDepth, m_nodeDepthIDs[i]);

        // Group nodes by depth
        std::vector<std::vector<uint32_t>> nodesAtDepth(maxDepth + 1);
        for (uint32_t i = 0; i < nodeCount; ++i)
          nodesAtDepth[m_nodeDepthIDs[i]].push_back(i);

        // Process bottom-up
        for (int d = static_cast<int>(maxDepth); d >= 0; --d)
        {
          for (auto nodeID : nodesAtDepth[d])
          {
            auto nodeBox = IGM::BoxInvertedInit();

            std::visit(
              [&](auto const& nodes) {
                auto const [entityBeginID, entityLength] = nodes.nodeEntitySegment[nodeID];
                for (uint32_t j = entityBeginID, endID = entityBeginID + entityLength; j < endID; ++j)
                  IGM::UniteInBoxAD(nodeBox, EA::GetGeometry(entities, m_entityStorage[j]));

                if (nodes.nodeChildSegmentBegins.size() <= nodeID + 1)
                  return;

                auto const childBeginNodeID = nodes.nodeChildSegmentBegins[nodeID];
                auto const childEndNodeID = nodes.nodeChildSegmentBegins[nodeID + 1];
                for (auto childID = childBeginNodeID; childID < childEndNodeID; ++childID)
                  IGM::UniteInBoxAD(nodeBox, GetNodeBox(childID));
              },
              m_nodes);

            m_nodeGeometry[nodeID] = { nodeBox.Min, IGM::Sub(nodeBox.Max, nodeBox.Min) };
          }
        }
      }
    }

    template<typename TNodeStorage>
    static constexpr bool IsFit(std::size_t entityCount, std::size_t maxNodeCount) noexcept
    {
      return maxNodeCount < std::numeric_limits<typename TNodeStorage::NodeSegmentIndex>::max() &&
             entityCount < std::numeric_limits<typename TNodeStorage::EntitySegment::Begin>::max() &&
             entityCount < std::numeric_limits<typename TNodeStorage::EntitySegment::Length>::max();
    }

  public:
    template<typename TExecMode = SeqExec>
    bool Create(EA::EntityContainerView entities, std::size_t maxElementNoInNode = CONFIG::DEFAULT_TARGET_ELEMENT_NUM_IN_NODES, TExecMode execMode = {}) noexcept
    {
      auto const entityCount = entities.size();
      if (entityCount == 0)
        return true;

      // Estimate max depth: log_{CHILD_NUM}(entityCount / maxElementNoInNode)
      auto const maxDepthID = [&]() -> uint8_t {
        if (entityCount <= maxElementNoInNode)
          return 1;
        auto const nLeaf = static_cast<double>(entityCount) / static_cast<double>(maxElementNoInNode);
        auto const logBase = std::log2(nLeaf) / std::log2(static_cast<double>(CHILD_NUM));
        return static_cast<uint8_t>(std::clamp(static_cast<int>(std::ceil(logBase)), 2, 20));
      }();

      // Build entity center data and populate entity storage
      auto buildData = std::vector<EntityBuildData>(entityCount);
      m_entityStorage.resize(entityCount);

      for (std::size_t i = 0; i < entityCount; ++i)
      {
        auto const& entity = *(entities.begin() + i);
        auto const entityID = EA::GetEntityID(entities, entity);
        auto const& geometry = EA::GetGeometry(entity);

        typename IGM::Vector center;
        if constexpr (EA::GEOMETRY_TYPE == GeometryType::Box)
        {
          for (dim_t d = 0; d < GA::DIMENSION_NO; ++d)
            center[d] = (typename IGM::Geometry(GA::GetBoxMinC(geometry, d)) + typename IGM::Geometry(GA::GetBoxMaxC(geometry, d))) *
                        typename IGM::Geometry(0.5);
        }
        else // Point
        {
          for (dim_t d = 0; d < GA::DIMENSION_NO; ++d)
            center[d] = typename IGM::Geometry(GA::GetPointC(geometry, d));
        }

        buildData[i] = { entityID, center };
        m_entityStorage[i] = entityID;
      }

      // Initialize node storage variant
      auto const estimatedNodeCount = std::max<std::size_t>(16, entityCount / std::max<std::size_t>(1, maxElementNoInNode) * 4);
      if (IsFit<NodeStorage256>(entityCount, estimatedNodeCount))
        m_nodes = NodeStorage256{ .nodeChildSegmentBegins = { 1 }, .nodeEntitySegment = {} };
      else if (IsFit<NodeStorage65536>(entityCount, estimatedNodeCount))
        m_nodes = NodeStorage65536{ .nodeChildSegmentBegins = { 1 }, .nodeEntitySegment = {} };
      else
        m_nodes = NodeStorageGeneral{ .nodeChildSegmentBegins = { 1 }, .nodeEntitySegment = {} };

      std::visit(
        [&](auto& nodes) {
          nodes.nodeChildSegmentBegins.reserve(estimatedNodeCount);
          nodes.nodeEntitySegment.reserve(estimatedNodeCount);
        },
        m_nodes);

      m_nodeDepthIDs.reserve(estimatedNodeCount);
      if constexpr (!std::is_same_v<decltype(m_nodeGeometry), std::monostate>)
        m_nodeGeometry.reserve(estimatedNodeCount);

      // Build the BVH
      BuildBVH(buildData, entities, maxDepthID, maxElementNoInNode);

      // Write back entity IDs in the final sorted order
      for (std::size_t i = 0; i < entityCount; ++i)
        m_entityStorage[i] = buildData[i].entityID;

      // Initialize node geometry (MBR) bottom-up
      InitializeNodeGeometryBVH(entities);

      return true;
    }
  };

  template<typename TEntityAdapter, typename TGeometryAdapter, typename TConfiguration>
  using BVHStaticLinear = OrthoTreeQueryBase<BVHStaticLinearCore<TEntityAdapter, TGeometryAdapter, TConfiguration>>;


  // BVH aliases

  template<dim_t DIMENSION_NO, typename TScalar = BaseGeometryType, bool IS_CONTIOGUOS_CONTAINER = true, int CHILD_NUM = 2>
  using StaticBVHPointND = BVHStaticLinear<
    std::conditional_t<IS_CONTIOGUOS_CONTAINER, PointEntitySpanAdapter<PointND<DIMENSION_NO, TScalar>>, PointEntityMapAdapter<PointND<DIMENSION_NO, TScalar>>>,
    GeneralGeometryAdapterND<DIMENSION_NO, TScalar>,
    BVHConfiguration<CHILD_NUM>>;

  template<dim_t DIMENSION_NO, typename TScalar = BaseGeometryType, bool IS_CONTIOGUOS_CONTAINER = true, int CHILD_NUM = 2>
  using StaticBVHBoxND = BVHStaticLinear<
    std::conditional_t<IS_CONTIOGUOS_CONTAINER, BoxEntitySpanAdapter<BoundingBoxND<DIMENSION_NO, TScalar>>, BoxEntityMapAdapter<BoundingBoxND<DIMENSION_NO, TScalar>>>,
    GeneralGeometryAdapterND<DIMENSION_NO, TScalar>,
    BVHConfiguration<CHILD_NUM>>;

  // BVH for points
  using StaticBVHPoint1D = StaticBVHPointND<1, BaseGeometryType>;
  using StaticBVHPoint2D = StaticBVHPointND<2, BaseGeometryType>;
  using StaticBVHPoint3D = StaticBVHPointND<3, BaseGeometryType>;
  using StaticBVHPoint4D = StaticBVHPointND<4, BaseGeometryType>;

  // BVH for bounding boxes
  using StaticBVHBox1D = StaticBVHBoxND<1, BaseGeometryType>;
  using StaticBVHBox2D = StaticBVHBoxND<2, BaseGeometryType>;
  using StaticBVHBox3D = StaticBVHBoxND<3, BaseGeometryType>;
  using StaticBVHBox4D = StaticBVHBoxND<4, BaseGeometryType>;

  // BVH with unordered_map entities
  using StaticBVHPointMap1D = StaticBVHPointND<1, BaseGeometryType, false>;
  using StaticBVHPointMap2D = StaticBVHPointND<2, BaseGeometryType, false>;
  using StaticBVHPointMap3D = StaticBVHPointND<3, BaseGeometryType, false>;
  using StaticBVHPointMap4D = StaticBVHPointND<4, BaseGeometryType, false>;
  using StaticBVHBoxMap1D = StaticBVHBoxND<1, BaseGeometryType, false>;
  using StaticBVHBoxMap2D = StaticBVHBoxND<2, BaseGeometryType, false>;
  using StaticBVHBoxMap3D = StaticBVHBoxND<3, BaseGeometryType, false>;
  using StaticBVHBoxMap4D = StaticBVHBoxND<4, BaseGeometryType, false>;

  template<dim_t DIMENSION_NO, int CHILD_NUM = 8>
  using StaticBVHNPointND = StaticBVHPointND<DIMENSION_NO, BaseGeometryType, true, CHILD_NUM>;
  template<dim_t DIMENSION_NO, int CHILD_NUM = 8>
  using StaticBVHNBoxND = StaticBVHBoxND<DIMENSION_NO, BaseGeometryType, true, CHILD_NUM>;

  template<dim_t DIMENSION_NO, int CHILD_NUM = 8>
  using StaticBVHNPointMapND = StaticBVHPointND<DIMENSION_NO, BaseGeometryType, false, CHILD_NUM>;
  template<dim_t DIMENSION_NO, int CHILD_NUM = 8>
  using StaticBVHNBoxMapND = StaticBVHBoxND<DIMENSION_NO, BaseGeometryType, false, CHILD_NUM>;
} // namespace OrthoTree
