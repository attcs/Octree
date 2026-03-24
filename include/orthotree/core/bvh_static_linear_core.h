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
  template<int CHILD_NUM_ = 8>
  struct BVHConfiguration
  {
    static constexpr int CHILD_NUM = CHILD_NUM_;
    static constexpr std::size_t DEFAULT_TARGET_ELEMENT_NUM_IN_NODES = 16;

    // SAH bin count. Increasing this value will improve the quality of the BVH but will also increase the creation time.
    static constexpr int SAH_BIN_COUNT = 16;

    // Required by OrthoTreeQueryBase
    static constexpr bool IS_HOMOGENEOUS_GEOMETRY = true;
    static constexpr bool USE_REVERSE_MAPPING = false;
    static constexpr depth_t MAX_ALLOWED_DEPTH_ID = depth_t{ 32 };

    template<typename TKey, typename TValue, typename THash>
    using UMapNodeContainer = std::unordered_map<TKey, TValue, THash>;
    template<typename TKey, typename TValue, typename TComp>
    using MapNodeContainer = std::map<TKey, TValue, TComp>;
  };

  template<typename TEntityAdapter, typename TGeometryAdapter, typename TConfiguration>
  class StaticBVHLinearCore
  {
  public: // Public type aliases
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

    using TScalar = typename GA::Scalar;
    using TFloatScalar = typename GA::FloatScalar;
    using TVector = typename GA::Vector;
    using TBox = typename GA::Box;
    using TRay = typename GA::Ray;
    using TPlane = typename GA::Plane;

  private: // Internal type aliases
    using NodeStorage256 = detail::NodeStorage256;
    using NodeStorage65536 = detail::NodeStorage65536;
    using NodeStorageGeneral = detail::NodeStorageGeneral;
    using NodeGeometryData = detail::NodeGeometryData<typename IGM::Vector>;

  private: // Data members
    std::variant<NodeStorage256, NodeStorage65536, NodeStorageGeneral> m_nodes;
    std::vector<EntityID> m_entityStorage;
    std::vector<NodeGeometryData> m_nodeGeometry;
    depth_t m_maxDepthNo = 0;
    std::size_t m_maxElementNum = CONFIG::DEFAULT_TARGET_ELEMENT_NUM_IN_NODES;

  private: // Serialization
    static constexpr uint32_t SERIALIZED_VERSION_ID = 0;

    template<typename TArchive, typename TEntityAdapter_, typename TGeometryAdapter_, typename TConfiguration_>
    friend void serialize(TArchive& ar, StaticBVHLinearCore<TEntityAdapter_, TGeometryAdapter_, TConfiguration_>& core);

  public:
    // Default constructor. Requires Create call before usage.
    constexpr StaticBVHLinearCore() = default;

    // Initialize the base octree structure with entity collection
    template<typename TExecMode = SeqExec>
    explicit StaticBVHLinearCore(
      EA::EntityContainerView entities, std::size_t maxElementNoInNode = CONFIG::DEFAULT_TARGET_ELEMENT_NUM_IN_NODES, TExecMode execMode = {}) noexcept
    {
      [[maybe_unused]] auto isSuccessfullyInsertedAllElements = Create(entities, maxElementNoInNode, execMode);
      assert(isSuccessfullyInsertedAllElements);
    }

    template<typename TExecMode = SeqExec>
    explicit StaticBVHLinearCore(
      TExecMode execMode, EA::EntityContainerView entities, std::size_t maxElementNoInNode = CONFIG::DEFAULT_TARGET_ELEMENT_NUM_IN_NODES) noexcept
    {
      [[maybe_unused]] auto isSuccessfullyInsertedAllElements = Create(entities, maxElementNoInNode, execMode);
      assert(isSuccessfullyInsertedAllElements);
    }

  public:
    constexpr depth_t GetMaxDepthID() const { return m_maxDepthNo - 1; }
    constexpr depth_t GetDepthNo() const { return m_maxDepthNo; }
    constexpr std::size_t GetMaxElementNum() const noexcept { return m_maxElementNum; }

    constexpr TBox GetBox() const noexcept
    {
      if (GetNodeCount() == 0)
        return GA::MakeBox();

      auto const rootBox = GetNodeBox(GetRootNodeID());
      auto box = GA::MakeBox();
      detail::static_for<GA::DIMENSION_NO>([&](auto d) {
        GA::SetBoxMinC(box, d, rootBox.Min[d]);
        GA::SetBoxMaxC(box, d, rootBox.Max[d]);
      });
      return box;
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

    constexpr std::size_t GetNodeCount() const noexcept
    {
      return std::visit([&](auto const& nodes) -> std::size_t { return nodes.nodeEntitySegment.size(); }, m_nodes);
    }

    constexpr std::size_t GetNodeEntityCount(NodeValue nodeID) const noexcept
    {
      return std::visit([&](auto const& nodes) -> std::size_t { return static_cast<std::size_t>(nodes.nodeEntitySegment[nodeID].length); }, m_nodes);
    }

    constexpr SequenceView<NodeID> GetNodeChildren(NodeValue nodeID) const noexcept
    {
      return std::visit(
        [nodeID](auto const& nodes) {
          if (nodeID >= NodeValue(nodes.nodeChildSegmentBegins.size()) - 1)
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

  private:
    static constexpr int CHILD_NUM = CONFIG::CHILD_NUM;

    struct EntityBuildData
    {
      EntityID entityID;
      IGM::Vector center;
      typename IGM::Box box;
    };

    // Evaluate SAH cost for splitting along a given axis at a given position
    // Returns the cost = leftArea * leftCount + rightArea * rightCount
    struct SplitCandidate
    {
      typename IGM::Geometry cost = std::numeric_limits<typename IGM::Geometry>::max();
      typename IGM::Geometry splitPos = {};
      dim_t axis = 0;
    };

    template<typename TExecMode = SeqExec>
    static SplitCandidate FindBestSplitAxis(std::span<EntityBuildData const> entities, [[maybe_unused]] TExecMode execMode = {}) noexcept
    {
      using Geometry = typename IGM::Geometry;

      struct Bin
      {
        uint32_t count = 0;
        typename IGM::Box bounds;
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
        std::array<Bin, CONFIG::SAH_BIN_COUNT> bins;
        for (auto& b : bins)
        {
          b.count = 0;
          b.bounds = IGM::BoxInvertedInit();
        }

        Geometry const scale = Geometry(CONFIG::SAH_BIN_COUNT) / (boundsMax - boundsMin);
        for (auto const& ed : entities)
        {
          int binIdx = std::min(CONFIG::SAH_BIN_COUNT - 1, static_cast<int>((ed.center[dimID] - boundsMin) * scale));
          ++bins[binIdx].count;
          IGM::UniteInBoxAD(bins[binIdx].bounds, ed.box);
        }

        // Sweep from left and right to compute prefix areas and counts
        std::array<Geometry, CONFIG::SAH_BIN_COUNT - 1> leftArea, rightArea;
        std::array<uint32_t, CONFIG::SAH_BIN_COUNT - 1> leftCount, rightCount;

        {
          typename IGM::Box leftBox = IGM::BoxInvertedInit();
          uint32_t leftSum = 0;
          for (int i = 0; i < CONFIG::SAH_BIN_COUNT - 1; ++i)
          {
            leftSum += bins[i].count;
            leftCount[i] = leftSum;
            if (bins[i].count > 0)
              IGM::UniteInBoxAD(leftBox, bins[i].bounds);
            leftArea[i] = IGM::GetSurfaceAreaAD(leftBox);
          }
        }
        {
          typename IGM::Box rightBox = IGM::BoxInvertedInit();
          uint32_t rightSum = 0;
          for (int i = CONFIG::SAH_BIN_COUNT - 1; i > 0; --i)
          {
            rightSum += bins[i].count;
            rightCount[i - 1] = rightSum;
            if (bins[i].count > 0)
              IGM::UniteInBoxAD(rightBox, bins[i].bounds);
            rightArea[i - 1] = IGM::GetSurfaceAreaAD(rightBox);
          }
        }

        // Evaluate SAH cost for each split plane
        Geometry const binWidth = (boundsMax - boundsMin) / Geometry(CONFIG::SAH_BIN_COUNT);
        for (int i = 0; i < CONFIG::SAH_BIN_COUNT - 1; ++i)
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
      auto const nodeID = static_cast<NodeID>(GetNodeCount());

      m_maxDepthNo = std::max<depth_t>(m_maxDepthNo, depthID + 1);

      std::visit(
        [&](auto& nodes) {
          using NodeStorage = std::decay_t<decltype(nodes)>;
          using EntitySegment = typename NodeStorage::EntitySegment;

          nodes.nodeEntitySegment.emplace_back(
            static_cast<typename EntitySegment::Begin>(nodeEntityBeginID), static_cast<typename EntitySegment::Length>(nodeEntityCount));

          // If there are no child nodes, we can skip storing child segment begin index for this node.
          if (childNodeCount == 0)
            return;

          nodes.nodeChildSegmentBegins.resize(nodeID + 2, nodes.nodeChildSegmentBegins.back());
          nodes.nodeChildSegmentBegins.back() += static_cast<typename NodeStorage::NodeSegmentIndex>(childNodeCount);
        },
        m_nodes);

      m_nodeGeometry.push_back({});

      return nodeID;
    }


    template<typename TExecMode = SeqExec>
    void BuildBVH(
      std::vector<EntityBuildData>& buildData,
      [[maybe_unused]] EA::EntityContainerView entities,
      uint8_t maxDepthID,
      std::size_t maxElementNoInNode,
      std::vector<uint8_t>& nodeDepthIDs,
      TExecMode execMode = {}) noexcept
    {
      struct NodeProcessingData
      {
        uint32_t beginID;
        uint32_t length;
        uint8_t depthID;
      };

      if constexpr (std::is_same_v<TExecMode, SeqExec>)
      {
        std::queue<NodeProcessingData> q;
        q.push({ 0, static_cast<uint32_t>(buildData.size()), 0 });

        while (!q.empty())
        {
          auto const r = q.front();
          q.pop();

          if (r.length <= maxElementNoInNode || r.depthID >= maxDepthID)
          {
            nodeDepthIDs.push_back(r.depthID);
            CreateBVHNode(r.depthID, r.beginID, r.length, 0);
            continue;
          }

          struct SubRange
          {
            uint32_t beginID;
            uint32_t length;
          };
          std::array<SubRange, CHILD_NUM> subRanges;
          subRanges[0] = { r.beginID, r.length };
          int rangeCount = 1;

          while (rangeCount < CHILD_NUM)
          {
            int bestIdx = 0;
            for (int k = 1; k < rangeCount; ++k)
              if (subRanges[k].length > subRanges[bestIdx].length)
                bestIdx = k;

            auto& sub = subRanges[bestIdx];
            if (sub.length <= 1)
              break;

            auto subSpan = std::span<EntityBuildData>(buildData.data() + sub.beginID, sub.length);
            auto split = FindBestSplitAxis(subSpan, SeqExec{});
            if (split.cost >= std::numeric_limits<typename IGM::Geometry>::max())
              break;

            auto partIt = std::partition(subSpan.begin(), subSpan.end(), [&](auto const& ed) { return ed.center[split.axis] < split.splitPos; });
            uint32_t leftCount = static_cast<uint32_t>(partIt - subSpan.begin());
            leftCount = std::clamp(leftCount, uint32_t(1), sub.length - 1);

            uint32_t origBegin = sub.beginID;
            uint32_t origLength = sub.length;
            sub = { origBegin, leftCount };
            subRanges[rangeCount] = { origBegin + leftCount, origLength - leftCount };
            ++rangeCount;
          }

          if (rangeCount == 1)
          {
            nodeDepthIDs.push_back(r.depthID);
            CreateBVHNode(r.depthID, r.beginID, r.length, 0);
          }
          else
          {
            std::sort(subRanges.begin(), subRanges.begin() + rangeCount, [](auto const& a, auto const& b) { return a.beginID < b.beginID; });
            nodeDepthIDs.push_back(r.depthID);
            CreateBVHNode(r.depthID, r.beginID, 0, static_cast<uint32_t>(rangeCount));
            for (int i = 0; i < rangeCount; ++i)
              q.push({ subRanges[i].beginID, subRanges[i].length, static_cast<uint8_t>(r.depthID + 1) });
          }
        }
      }
      else
      {
        std::vector<NodeProcessingData> currentLevel;
        currentLevel.push_back({ 0, static_cast<uint32_t>(buildData.size()), 0 });

        while (!currentLevel.empty())
        {
          struct SubRange
          {
            uint32_t beginID;
            uint32_t length;
          };
          struct SplitResult
          {
            std::array<SubRange, CHILD_NUM> subRanges;
            int rangeCount = 1;
            bool isLeaf = false;
          };

          std::vector<SplitResult> results(currentLevel.size());

          EXEC_POL_DEF(epl);
          auto const indices = SequenceView<std::size_t>(0, currentLevel.size());
          std::for_each(EXEC_POL_ADD(epl) indices.begin(), indices.end(), [&](std::size_t i) {
            auto const& r = currentLevel[i];
            auto& res = results[i];

            if (r.length <= maxElementNoInNode || r.depthID >= maxDepthID)
            {
              res.isLeaf = true;
              return;
            }

            res.subRanges[0] = { r.beginID, r.length };
            while (res.rangeCount < CHILD_NUM)
            {
              int bestIdx = 0;
              for (int k = 1; k < res.rangeCount; ++k)
                if (res.subRanges[k].length > res.subRanges[bestIdx].length)
                  bestIdx = k;

              auto& sub = res.subRanges[bestIdx];
              if (sub.length <= 1)
                break;

              auto subSpan = std::span<EntityBuildData>(buildData.data() + sub.beginID, sub.length);
              auto split = FindBestSplitAxis(subSpan, execMode);
              if (split.cost >= std::numeric_limits<typename IGM::Geometry>::max())
                break;

              auto partIt = std::partition(subSpan.begin(), subSpan.end(), [&](auto const& ed) { return ed.center[split.axis] < split.splitPos; });
              uint32_t leftCount = static_cast<uint32_t>(partIt - subSpan.begin());
              leftCount = std::clamp(leftCount, uint32_t(1), sub.length - 1);

              uint32_t origBegin = sub.beginID;
              uint32_t origLength = sub.length;
              sub = { origBegin, leftCount };
              res.subRanges[res.rangeCount] = { origBegin + leftCount, origLength - leftCount };
              ++res.rangeCount;
            }

            if (res.rangeCount == 1)
            {
              res.isLeaf = true;
            }
            else
            {
              std::sort(res.subRanges.begin(), res.subRanges.begin() + res.rangeCount, [](auto const& a, auto const& b) {
                return a.beginID < b.beginID;
              });
            }
          });

          std::vector<NodeProcessingData> nextLevel;
          for (std::size_t i = 0; i < currentLevel.size(); ++i)
          {
            auto const& r = currentLevel[i];
            auto const& res = results[i];
            if (res.isLeaf)
            {
              nodeDepthIDs.push_back(r.depthID);
              CreateBVHNode(r.depthID, r.beginID, r.length, 0);
            }
            else
            {
              nodeDepthIDs.push_back(r.depthID);
              CreateBVHNode(r.depthID, r.beginID, 0, static_cast<uint32_t>(res.rangeCount));
              for (int j = 0; j < res.rangeCount; ++j)
                nextLevel.push_back({ res.subRanges[j].beginID, res.subRanges[j].length, static_cast<uint8_t>(r.depthID + 1) });
            }
          }
          currentLevel = std::move(nextLevel);
        }
      }
    }


    template<typename TExecMode = SeqExec>
    void InitializeNodeGeometryBVH(EA::EntityContainerView entities, std::vector<uint8_t>& nodeDepthIDs, TExecMode execMode = {}) noexcept
    {
      auto const nodeCount = static_cast<uint32_t>(GetNodeCount());
      if (nodeCount == 0)
        return;

      // Group nodes by depth for safe parallel/iterative bottom-up processing
      auto const maxDepth = GetMaxDepthID();
      std::vector<std::vector<uint32_t>> nodesAtDepth(maxDepth + 1);
      for (uint32_t i = 0; i < nodeCount; ++i)
        nodesAtDepth[nodeDepthIDs[i]].push_back(i);

      auto const processNode = [&](uint32_t nodeID) {
        auto nodeBox = IGM::BoxInvertedInit();

        std::visit(
          [&](auto const& nodes) {
            auto const [entityBeginID, entityLength] = nodes.nodeEntitySegment[nodeID];
            for (uint32_t j = entityBeginID, endID = entityBeginID + entityLength; j < endID; ++j)
              IGM::UniteInBoxAD(nodeBox, EA::GetGeometry(entities, m_entityStorage[j]));

            if (nodes.nodeChildSegmentBegins.size() <= static_cast<size_t>(nodeID) + 1)
              return;

            auto const childBeginNodeID = nodes.nodeChildSegmentBegins[nodeID];
            auto const childEndNodeID = nodes.nodeChildSegmentBegins[nodeID + 1];
            for (auto childID = childBeginNodeID; childID < childEndNodeID; ++childID)
              IGM::UniteInBoxAD(nodeBox, GetNodeBox(childID));
          },
          m_nodes);

        m_nodeGeometry[nodeID] = { nodeBox.Min, IGM::Sub(nodeBox.Max, nodeBox.Min) };
      };

      // Process depths bottom-up
      for (int d = static_cast<int>(maxDepth); d >= 0; --d)
      {
        auto const& currentLevelNodes = nodesAtDepth[d];
        if (currentLevelNodes.empty())
          continue;

        EXEC_POL_DEF(ep);
        std::for_each(EXEC_POL_ADD(ep) currentLevelNodes.begin(), currentLevelNodes.end(), processNode);
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

      m_maxElementNum = maxElementNoInNode;
      m_maxDepthNo = 0;

      // Estimate max depth: log_{CHILD_NUM}(entityCount / maxElementNoInNode)
      auto const maxDepthID = [&]() -> uint8_t {
        if (entityCount <= maxElementNoInNode)
          return 1;
        auto const nLeaf = static_cast<double>(entityCount) / static_cast<double>(maxElementNoInNode);
        auto const logBase = std::log2(nLeaf) / std::log2(static_cast<double>(CHILD_NUM));
        return static_cast<uint8_t>(std::clamp(static_cast<int>(std::ceil(logBase)), 2, static_cast<int>(CONFIG::MAX_ALLOWED_DEPTH_ID)));
      }();

      // Build entity center data and populate entity storage
      auto buildData = std::vector<EntityBuildData>(entityCount);
      m_entityStorage.resize(entityCount);

      EXEC_POL_DEF(epb);
      detail::zip_view buildDataAndStorageZip(buildData, m_entityStorage);
      std::transform(EXEC_POL_ADD(epb) entities.begin(), entities.end(), buildDataAndStorageZip.begin(), [&](auto const& entity) {
        auto const entityID = EA::GetEntityID(entities, entity);
        auto const& geometry = EA::GetGeometry(entity);

        typename IGM::Vector center;
        auto const entityBox = IGM::GetBoxAD(geometry);

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

        return std::make_pair(EntityBuildData{ entityID, center, entityBox }, entityID);
      });

      // Initialize node storage variant
      auto const estimatedNodeCount = std::max<std::size_t>(16, entityCount / std::max<std::size_t>(1, maxElementNoInNode) * 4);
      if (IsFit<NodeStorage256>(entityCount, estimatedNodeCount))
        m_nodes = NodeStorage256{ .nodeChildSegmentBegins = { 1 }, .nodeEntitySegment = {} };
      else if (IsFit<NodeStorage65536>(entityCount, estimatedNodeCount))
        m_nodes = NodeStorage65536{ .nodeChildSegmentBegins = { 1 }, .nodeEntitySegment = {} };
      else
        m_nodes = NodeStorageGeneral{ .nodeChildSegmentBegins = { 1 }, .nodeEntitySegment = {} };

      // Reserve space to avoid reallocations and appease compiler warnings
      std::vector<uint8_t> nodeDepthIDs;
      nodeDepthIDs.reserve(estimatedNodeCount);
      m_nodeGeometry.reserve(estimatedNodeCount);

      std::visit(
        [&](auto& nodes) {
          nodes.nodeEntitySegment.reserve(estimatedNodeCount);
          nodes.nodeChildSegmentBegins.reserve(estimatedNodeCount + 1);
        },
        m_nodes);

      // Build the BVH
      BuildBVH(buildData, entities, maxDepthID, maxElementNoInNode, nodeDepthIDs, execMode);

      // Write back entity IDs in the final sorted order
      EXEC_POL_DEF(epw);
      std::transform(EXEC_POL_ADD(epw) buildData.begin(), buildData.end(), m_entityStorage.begin(), [](auto const& ed) { return ed.entityID; });

      // Initialize node geometry (MBR) bottom-up
      InitializeNodeGeometryBVH(entities, nodeDepthIDs, execMode);

      return true;
    }

    template<typename TExecMode = SeqExec>
    static bool Create(
      StaticBVHLinearCore& tree,
      EA::EntityContainerView entities,
      std::size_t maxElementNoInNode = CONFIG::DEFAULT_TARGET_ELEMENT_NUM_IN_NODES,
      TExecMode execMode = {}) noexcept
    {
      return tree.Create<TExecMode>(entities, maxElementNoInNode, execMode);
    }


    static constexpr bool AreChildNodesOverlapping() noexcept { return true; }
  };

  template<typename TEntityAdapter, typename TGeometryAdapter, typename TConfiguration>
  using StaticBVHLinearBase = OrthoTreeQueryBase<StaticBVHLinearCore<TEntityAdapter, TGeometryAdapter, TConfiguration>>;
} // namespace OrthoTree
