#pragma once

#include "ot_base.h"

namespace OrthoTree
{

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

  private:
    static_assert(GA::DIMENSION_NO <= 16);
    static_assert(SI::IS_LINEAR_TREE);

  private:
    using MGSI = typename detail::MortonGridSpaceIndexing<GA, CONFIG::ALLOW_OUT_OF_SPACE_INSERTION, CONFIG::LOOSE_FACTOR>;

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
      using EntitySegmentIndex = uint8_t;

      std::vector<NodeSegmentIndex> nodeChildSegmentBegins;
      std::vector<EntitySegmentIndex> nodeEntityBegins;
    };

    struct NodeStorage65536
    {
      using NodeSegmentIndex = uint16_t;
      using EntitySegmentIndex = uint16_t;

      std::vector<NodeSegmentIndex> nodeChildSegmentBegins;
      std::vector<EntitySegmentIndex> nodeEntityBegins;
    };

    struct NodeStorageGeneral
    {
      using NodeSegmentIndex = uint32_t;
      using EntitySegmentIndex = uint32_t;

      std::vector<NodeSegmentIndex> nodeChildSegmentBegins;
      std::vector<EntitySegmentIndex> nodeEntityBegins;
    };

    using NodeGeometry =
      std::conditional_t<CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::None, std::monostate, std::vector<typename Base::NodeGeometry>>;

  private: // Data
    std::variant<NodeStorage256, NodeStorage65536, NodeStorageGeneral> m_nodes;
    std::vector<uint8_t> m_nodeDepthIDs;
    std::vector<EntityID> m_entityStorage;
    NodeGeometry m_nodeGeometry;

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
          return Base::GetNodeSize(0);
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


    // Node API
#ifdef ORTHOTREE_PUBLIC_NODE_INTERFACE
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
      return std::visit([&](auto const& nodes) { return nodes.nodeEntities[nodeID].length; }, m_nodes);
    }

    constexpr SequenceView<NodeID> GetNodeChildren(NodeValue nodeID) const noexcept
    {
      return std::visit(
        [this, nodeID](auto const& nodes) {
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
          return std::span<EntityID const>(
            &m_entityStorage[nodes.nodeEntityBegins[nodeID]], nodes.nodeEntityBegins[nodeID + 1] - nodes.nodeEntityBegins[nodeID]);
        },
        m_nodes);
    }

    constexpr bool IsNodeEntitiesEmpty(NodeValue nodeID) const noexcept
    {
      return std::visit(
        [nodeID](auto const& nodes) {
          nodes.nodeEntityBegins[nodeID];
          return nodes.nodeEntityBegins[nodeID] == nodes.nodeEntityBegins[nodeID + 1];
        },
        m_nodes);
    }

    constexpr decltype(auto) GetNodeMinPoint(NodeValue nodeID) const noexcept
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

    constexpr decltype(auto) GetNodeSize(NodeValue nodeID) const noexcept
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

    constexpr IGM::Box GetNodeBox(NodeValue nodeID) const noexcept
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

    constexpr void InitNodeGeometry(NodeID nodeID, SI::Location location, MGSI const& spaceIndexing) noexcept
    {
      if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::None)
      {
        return;
      }
      else if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MinPoint)
      {
        auto const mortonNodeID = spaceIndexing.GetNodeID(location);
        auto const nodeCenter = spaceIndexing.CalculateNodeCenter(mortonNodeID);
        auto const& nodeHalfSize = Base::GetNodeSize(location.GetDepthID() + 1);

        m_nodeGeometry[nodeID] = IGM::Sub(nodeCenter, nodeHalfSize);
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
      return maxNodeCount < std::numeric_limits<typename TNodeStorage::NodeSegmentIndex>::max() &&
             entityCount < std::numeric_limits<typename TNodeStorage::EntitySegmentIndex>::max();
    }

    constexpr NodeID CreateNode(SI::Location location, uint32_t nodeEntityCount, uint32_t childNodeCount, MGSI const& spaceIndexing)
    {
      auto const nodeID = static_cast<NodeID>(m_nodeDepthIDs.size());

      m_nodeDepthIDs.emplace_back(static_cast<uint8_t>(location.GetDepthID()));
      std::visit(
        [&](auto& nodes) {
          using NodeStorage = std::decay_t<decltype(nodes)>;
          using EntitySegmentIndex = typename NodeStorage::EntitySegmentIndex;
          using NodeSegmentIndex = typename NodeStorage::NodeSegmentIndex;

          nodes.nodeEntityBegins.emplace_back(static_cast<EntitySegmentIndex>(nodes.nodeEntityBegins.back() + nodeEntityCount));

          // Child node segment is filled if there are child nodes. Leaf nodes' segments are not created to save space, and GetNodeChildren() returns empty view for them.
          if (childNodeCount > 0)
          {
            nodes.nodeChildSegmentBegins.resize(nodeID + 2, nodes.nodeChildSegmentBegins.back());
            nodes.nodeChildSegmentBegins.back() += static_cast<NodeSegmentIndex>(childNodeCount);
          }
        },
        m_nodes);

      if constexpr (!std::is_same_v<decltype(m_nodeGeometry), std::monostate>)
      {
        m_nodeGeometry.emplace_back();
      }
      InitNodeGeometry(nodeID, location, spaceIndexing);
      return nodeID;
    }

    // Build the tree in depth-first order
    template<bool ARE_LOCATIONS_SORTED>
    constexpr void Build(auto const& rootBeginLocationIt, auto const& rootEndLocationIt, MGSI const& spaceIndexing) noexcept
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

        if (nodeEntityCount <= Base::GetMaxElementNum() || depthID >= Base::GetMaxDepthID())
        {
          CreateNode(location, nodeEntityCount, 0, spaceIndexing);
          continue;
        }

        auto beginIt = rootBeginLocationIt + beginID;
        auto endIt = beginIt + nodeEntityCount;
        auto const endID = beginID + nodeEntityCount;

        uint32_t nonRefinableEntityCount = 0;
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
            nonRefinableEntityCount = detail::size<uint32_t>(beginIt.GetSecond(), nonRefinableEndIt.GetSecond());
            if (nonRefinableEntityCount == nodeEntityCount)
            {
              CreateNode(location, nodeEntityCount, 0, spaceIndexing);
              continue;
            }

            beginIt = nonRefinableEndIt;
          }
        }

        beginID += nonRefinableEntityCount;

        ++depthID;
        auto const examinedLevelID = Base::GetExaminationLevelID(depthID);
        auto const keyGenerator = typename SI::ChildKeyGenerator(location.GetLocationID());

        uint32_t childNodeCount = 0;
        while (beginID < endID)
        {
          auto const& pivot = (*beginIt).GetFirst();
          auto const childChecker = typename SI::ChildCheckerFixedDepth(examinedLevelID, pivot.GetLocationID());

          auto lcah = SI::GetLowestCommonAncestorHelper(pivot);
          auto childEndIt = [&] {
            if constexpr (ARE_LOCATIONS_SORTED)
            {
              auto childEndIt =
                std::partition_point(beginIt, endIt, [&](auto const& location) { return childChecker.Test(location.GetFirst().GetLocationID()); });
              for (auto it = beginIt.GetFirst(); it != childEndIt.GetFirst(); ++it)
                lcah.Add(*it);

              return childEndIt;
            }
            else
            {
              return Partitioning::partition(
                beginIt,
                endIt,
                [&](auto const& location) { return childChecker.Test(location.GetFirst().GetLocationID()); },
                [&](auto const& loc) { lcah.Add(loc.GetFirst()); });
            }
          }();

          auto childEntityCount = detail::size<uint32_t>(beginIt.GetSecond(), childEndIt.GetSecond());
          nodeQueue.push({ lcah.GetLocation(Base::GetMaxDepthID()), beginID, childEntityCount });

          ++childNodeCount;
          beginID += childEntityCount;
          beginIt = childEndIt;
        }

        CreateNode(location, nonRefinableEntityCount, childNodeCount, spaceIndexing);
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
      auto const spaceIndexing = MGSI(maxDepthID, boxSpace);
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
        m_nodes = NodeStorage256{ .nodeChildSegmentBegins = { 1 }, .nodeEntityBegins = { 0 } };
      else if (IsFit<NodeStorage65536>(entityCount, maxNodeCount))
        m_nodes = NodeStorage65536{ .nodeChildSegmentBegins = { 1 }, .nodeEntityBegins = { 0 } };
      else
        m_nodes = NodeStorageGeneral{ .nodeChildSegmentBegins = { 1 }, .nodeEntityBegins = { 0 } };

      auto const estimatedNodeNum =
        detail::EstimateNodeNumber<GA::DIMENSION_NO, SI::MAX_THEORETICAL_DEPTH_ID>(entityCount, maxDepthID, maxElementNumInNode);

      std::visit(
        [&](auto& nodes) {
          nodes.nodeChildSegmentBegins.reserve(estimatedNodeNum);
          nodes.nodeEntityBegins.reserve(estimatedNodeNum);
        },
        m_nodes);

      m_nodeDepthIDs.reserve(estimatedNodeNum);
      if constexpr (!std::is_same_v<decltype(m_nodeGeometry), std::monostate>)
        m_nodeGeometry.reserve(estimatedNodeNum);

      Build<ARE_LOCATIONS_SORTED>(locationsZip.begin(), endIt, spaceIndexing);
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
} // namespace OrthoTree
