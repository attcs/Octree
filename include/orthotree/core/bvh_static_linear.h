#pragma once

#include "../adapters/general.h"
#include "../detail/freelist_vector.h"
#include "../detail/inplace_vector.h"

#include <array>
#include <stack>

namespace SpatialIndexing
{

  using Index = uint32_t;
  using Length = uint32_t;

  template<typename Geometry>
  struct BVHNode
  {
    Geometry mbr;
    union
    {
      Index elementBlockID = 0;
      struct
      {
        Index leftChildID = 0;
        bool isLeaf = false;
      } children;
    } data;
  };

  struct DefaultBoundingVolumeHearchyConfiguration
  {
    static constexpr Length kElementMaxNum = 8;
  };

  template<typename GeometryAdapter, typename BoundingVolumeHearchyConfiguration = DefaultBoundingVolumeHearchyConfiguration>
  class BoundingVolumeHearchy
  {
  private:
    using GA = GeometryAdapter;
    using C = BoundingVolumeHearchyConfiguration;

  private:
    using Node = BVHNode<GA::Box>;
    using NodeContainer = details::freelist_vector<Node>;
    using NodeElementBlock = details::inplace_vector<ValueType, C::kElementMaxNum>;
    using ElementContainer = details::freelist_vector<NodeElementBlock>;

    using NodeID = NodeContainer::TokenID;


  private:
    NodeContainer m_nodes;
    ElementContainer m_elements;

  public:
    explicit BoundingVolumeHearchy() = default;
    explicit BoundingVolumeHearchy(std::vector<ValueType> elements) { Insert({}, std::move(elements)); }

    struct Split
    {
      float cost = 1e30f;
      float pos = 0.0f;
      uint8_t axis = 0;
    };
    template<int BucketNum>
    Split FindBestSplitPlane(std::span<ValueType> elements)
    {
      struct Bin
      {
        int elementNum = 0;
        Box bounds;
      };

      float bestCost = 1e30f;
      for (int dimID = 0; dimID < GA::DimensionNum; dimID++)
      {
        float boundsMin = 1e30f, boundsMax = -1e30f;
        for (const auto element : elements)
        {
          const auto center = GA::GetCenter(element.box, dimID);
          boundsMin = std::min(boundsMin, center);
          boundsMax = std::max(boundsMax, center);
        }

        if (boundsMin == boundsMax)
        {
          continue;
        }

        // populate the bins
        std::array<Bin, BucketNum> bin = {};
        float scale = BINS / (boundsMax - boundsMin);
        for (const auto element : elements)
        {
          const auto center = GA::GetCenter(element.box, dimID);

          int binIdx = std::min(BINS - 1, static_cast<int>((center - boundsMin) * scale));
          ++bin[binIdx].elementNum;
          GA::Combine(bin[binIdx].bounds, element.box);
        }

        float leftArea[BINS - 1], rightArea[BINS - 1];
        int leftCount[BINS - 1], rightCount[BINS - 1];
        GA::TBox leftBox, rightBox;
        int leftSum = 0, rightSum = 0;
        for (int i = 0; i < BINS - 1; i++)
        {
          leftSum += bin[i].elementNum;
          leftCount[i] = leftSum;
          GA::Enlarge(leftBox, bin[i].bounds);
          leftArea[i] = leftBox.area();
          rightSum += bin[BINS - 1 - i].elementNum;
          rightCount[BINS - 2 - i] = rightSum;
          GA::Enlarge(rightBox, bin[BINS - 1 - i].bounds);
          rightArea[BINS - 2 - i] = rightBox.area();
        }
        // calculate SAH cost for the 7 planes
        scale = (boundsMax - boundsMin) / BINS;
        for (int i = 0; i < BINS - 1; i++)
        {
          float planeCost = leftCount[i] * leftArea[i] + rightCount[i] * rightArea[i];
          if (planeCost < bestCost)
          {
            axis = dimID, splitPos = boundsMin + scale * (i + 1), bestCost = planeCost;
          }
        }
      }
      return bestCost;
    }

    float CalculateNodeCost(const Node& node)
    {
      const auto area = GA::CalculateMargin(node.mbr);
      return area * node.data.elements.second;
    }

    NodeID AddChildren(NodeID parentNodeID, const GA::Box& leftMBR, const GA::Box& rightMBR)
    {
      auto leftChildNode = m_nodes.template insert<2>();

      auto& parentNode = m_nodes.at(parentNodeID);
      parentNode.data.children.leftChildID = leftChildID;
      parentNode.data.children.isLeaf = false;

      m_nodes.at(leftChildID).mbr = leftMBR;
      m_nodes.at(leftChildID + 1).mbr = rightMBR;

      return leftChildID;
    }

    void Subdivide(NodeID overflowingNodeID, std::span<ValueType> newElements)
    {
      struct Context
      {
        NodeID nodeID;
        std::array<std::span<ValueType>, 2> elements;
      };

      std::stack<NodeID> nodeStack;
      auto& overFlowingNode = m_nodes.at(overflowingNodeID);
      auto elementIdx = overFlowingNode.data.elementBlockID;
      nodeStack.push(
        {
          .nodeID = overflowingNodeID, .elements = std::array{ std::span(overFlowingNode.elements), newElements }
      });
      for (; !nodeStack.empty();)
      {
        const auto& ctx = nodeStack.top();

        auto& currentNode = m_nodes.at(currentNodeID);
        auto elementNum = ctx.elements[0].size() + ctx.elements[1].size();

        if (elementNum <= kElementMaxNum)
        {
          bool isNewElementPlace = nodeStack.size() > 1;
          if (isNewElementPlace)
          {
            auto elementID = m_elements.insert();
            currentNode.elementID = elementID;
            auto& element = m_elements.at(elementID);
            element.insert(ctx.elements[0]);
            element.insert(ctx.elements[1]);
          }
          else
          {
            currentNode.elementID = elementIdx;
            auto& element = m_elements.at(elementIdx);
            element.resize(ctx.elements[0].size());
            element.insert(ctx.elements[1]);
          }
          nodeStack.pop();
          continue;
        }

        auto split = FindBestSplitPlane(ConcatView(ctx.elements[0], ctx.elements[1]));
        // float nosplitCost = currentNode.CalculateNodeCost();
        // if (splitCost >= nosplitCost) continue;
        const auto& splitPartition = [split](const ValueType& element) {
          return GA::GetCenter(element.box, split.axis) < split.pos
        };

        auto nodeElementsTail = std::ranges::partition(ctx.elements[0], splitPartition);
        auto newElementsTail = std::ranges::partition(ctx.elements[1], splitPartition);

        auto leftElements =
          std::array{ std::span(nodeElements.begin(), nodeElementsTail.begin()), std::span(newElements.begin(), newElementsTail.begin()) };
        auto rightElements =
          std::array{ std::span(nodeElementsTail.begin(), nodeElements.end()), std::span(newElementsTail.begin(), newElements.end()) };

        GA::Box leftMBR = nodeElements.begin(), rightMBR = nodeElementsTail.begin();
        for (const auto& elements : leftElements)
        {
          UpdateMBR(leftMBR, elements);
        }

        for (const auto& elements : leftElements)
        {
          UpdateMBR(rightMBR, elements);
        }

        const auto leftChildID = AddChildren(currentNode, leftMBR, rightMBR);
        nodeStack.push({ .nodeID = leftChildID, .elements = leftElements });
        nodeStack.push({ .nodeID = leftChildID + 1, .elements = rightElements });
      }
    }

    void Insert(std::vector<ValueType> elements, std::vector<ValueType> newElements)
    {
      struct Context
      {
        NodeID nodeID;
        std::span<ValueType> elements;
      };
      std::stack<Context> stack;

      stack.push({ 0, std::span(newElements) });
      for (; !stack.empty();)
      {
        Context ctx = stack.top();
        stack.pop();

        auto& node = m_nodes.at(ctx.nodeID);
        for (auto& element : ctx.elements)
        {
          //! j� lenne GA::Combine az els� elem�t m�dis�tan� ink�bb, k�l�nben m�solatot csin�l
          GA::Enlarge(node.mbr, GA::GetBox(element));
        }

        if (node.data.isLeaf)
        {
          //! Subdivide leaf node if needed
          if (node.elements.elementNum + ctx.elements.size() > kElementMaxNum)
          {
                      Subdivide(node, ConcatView(std::span(m_elements.begin() +  node.data.elements.firstElementID,  node.data.elements.elementNum), ctx.elements);
          }
          else
          {
            // Append elements to leaf
            Index firstElementID = node.data.elements.firstElementID + node.data.elements.elementNum;
            for (Index i = 0; i < ctx.elements.size(); ++i)
            {
              m_elements[firstElementID + i] = ctx.elements[i];
            }
            node.data.elements.elementNum += static_cast<Length>(ctx.elements.size());
          }

          continue;
        }


        auto& leftNode = m_nodes.at(ctx.leftChildID);
        auto& rightNode = m_nodes.at(ctx.leftChildID + 1);

        std::array<ValueType, 2> nodeMBRs = { leftNode.mbr, rightNode.mbr };
        auto split = FindBestSplitPlane(ConcatView(ctx.elements, nodeMBRs));
        const auto tail =
          std::ranges::partition(ctx.elements, [split](const ValueType& element) { return GA::GetCenter(element.box, split.axis) < split.pos; });

        stack.push({ node.leftChildID, std::span(ctx.elements.begin(), tail.begin()) });
        stack.push({ node.leftChildID + 1, std::span(tail.begin(), ctx.elements.end()) });
      }
    }
    void Remove(std::span<ValueType> elements, std::pair<Index, Length> elements) {};
  };


  // using BVH3D = BoundingVolumeHearchy<AdaptorGeneral3Dd>;

  class Configuration
  {
    static constexpr int CHILD_NUM = 8;
    static constexpr std::size_t DEFAULT_TARGET_ELEMENT_NUM_IN_NODES = 16;
  };

  template<typename TEntityAdapter, typename TGeometryAdapter, typename TConfiguration>
  class BVHStaticLinearCore
  {
    using IGM = TGeometryAdapter;
    using EA = TEntityAdapter;
    using GA = TGeometryAdapter;
    using CONFIG = TConfiguration;
    
    using NodeID = int;
    using NodeValue = NodeID;
    using NodeGeometry = IGM::Box;

    using MGSI = typename detail::MortonGridSpaceIndexing<GA, CONFIG::ALLOW_OUT_OF_SPACE_INSERTION, CONFIG::LOOSE_FACTOR, CONFIG::MAX_ALLOWED_DEPTH_ID>;

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

    using NodeGeometry =
      std::conditional_t<CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::None, std::monostate, std::vector<typename Base::NodeGeometry>>;

  private:
    std::variant<NodeStorage256, NodeStorage65536, NodeStorageGeneral> m_nodes;
    std::vector<uint8_t> m_nodeDepthIDs;
    std::vector<EntityID> m_entityStorage;
    NodeGeometry m_nodeGeometry;


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
      return std::visit([&](auto const& nodes) { return nodes.nodeEntitySegment[nodeID].length; }, m_nodes);
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

    constexpr IGM::Box GetNodeBox(NodeValue nodeID) const noexcept
    {
      auto const& [minPoint, size] = m_nodeGeometry[nodeID];
      return { .Min = minPoint, .Max = IGM::Add(minPoint, size) };
    }


  public:
    template<typename TExecMode = SeqExec>
    bool Create(EA::EntityContainerView entities, std::size_t maxElementNoInNode = CONFIG::DEFAULT_TARGET_ELEMENT_NUM_IN_NODES, TExecMode execMode = {}) noexcept
    {}
  };
}; // namespace SpatialIndexing
