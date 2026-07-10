#include "pch.h"

#include <charconv>
#include <filesystem>
#include <fstream>
#include <numbers>
#include <random>

#define ORTHOTREE__PUBLIC_NODE_INTERFACE
#define ORTHOTREE__DEBUG_CHECKS
#include "orthotree/bvh.h"
#include "orthotree/octree.h"

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-braces"
#endif

using namespace OrthoTree;
using std::array;
using std::vector;


namespace GeneralTest
{
  namespace MortonTest
  {
    TEST(MortonTest, M1D_0_0)
    {
      auto constexpr arr = array<GridID, 1>{ 0 };
      EXPECT_EQ(SI1D::LocationID{ 0 }, SI1D::Encode(arr));
    } // namespace GeneralTest

    TEST(MortonTest, M1D_4_4)
    {
      auto constexpr arr = array<GridID, 1>{ 4 };
      EXPECT_EQ(SI1D::LocationID{ 4 }, SI1D::Encode(arr));
    }


    TEST(MortonTest, M2D_00_0)
    {
      auto constexpr arr = array<GridID, 2>{ 0, 0 };
      EXPECT_EQ(SI2D::LocationID{ 0 }, SI2D::Encode(arr));
    }

    TEST(MortonTest, M2D_20_4)
    {
      auto constexpr arr = array<GridID, 2>{ 2, 0 };
      EXPECT_EQ(SI2D::LocationID{ 4 }, SI2D::Encode(arr));
    }

    TEST(MortonTest, M2D_02_8)
    {
      auto constexpr arr = array<GridID, 2>{ 0, 2 };
      EXPECT_EQ(SI2D::LocationID{ 8 }, SI2D::Encode(arr));
    }

    TEST(MortonTest, M2D_22_12)
    {
      auto constexpr arr = array<GridID, 2>{ 2, 2 };
      EXPECT_EQ(SI2D::LocationID{ 12 }, SI2D::Encode(arr));
    }

    TEST(MortonTest, M2D_13_11)
    {
      auto constexpr arr = array<GridID, 2>{ 1, 3 };
      EXPECT_EQ(SI2D::LocationID{ 11 }, SI2D::Encode(arr));
    }

    TEST(MortonTest, M3D_000_0)
    {
      auto constexpr arr = array<GridID, 3>{ 0, 0, 0 };
      EXPECT_EQ(SI3D::LocationID{ 0 }, SI3D::Encode(arr));
    }

    TEST(MortonTest, M3D_100_1)
    {
      auto constexpr arr = array<GridID, 3>{ 1, 0, 0 };
      EXPECT_EQ(SI3D::LocationID{ 1 }, SI3D::Encode(arr));
    }

    TEST(MortonTest, M3D_001_4)
    {
      auto constexpr arr = array<GridID, 3>{ 0, 0, 1 };
      EXPECT_EQ(SI3D::LocationID{ 4 }, SI3D::Encode(arr));
    }

    TEST(MortonTest, M3D_111_7)
    {
      auto constexpr arr = array<GridID, 3>{ 1, 1, 1 };
      EXPECT_EQ(SI3D::LocationID{ 7 }, SI3D::Encode(arr));
    }

    TEST(MortonTest, M4D_1111_15)
    {
      auto constexpr arr = array<GridID, 4>{ 1, 1, 1, 1 };
      EXPECT_EQ(SI4D::LocationID{ 15 }, SI4D::Encode(arr));
    }

    TEST(MortonTest, M4D_2111_30)
    {
      auto constexpr arr = array<GridID, 4>{ 2, 1, 1, 1 };
      EXPECT_EQ(SI4D::LocationID{ 30 }, SI4D::Encode(arr));
    }
  }

  namespace NodeTest

  {
  // private:
    template<dim_t N>
    static void _complex_ND_Only1()
    {
      using child_id_type_ = typename detail::MortonSpaceIndexing<N, 20>::ChildID;
      using NodeID = typename OrthoTreeBoxND<N>::NodeID;

      auto node = detail::OrthoTreeNodeData<2 << N, NodeID, child_id_type_, typename OrthoTreeBoxND<N>::EntityID, std::monostate>();
      EXPECT_FALSE((node.IsAnyChildExist()));

      auto constexpr nChild = 1 << N;
      for (child_id_type_ iChild = 0; iChild < nChild; ++iChild)
      {
        auto idChild = nChild - iChild - 1;
        node.LinkChild(idChild, NodeID(idChild));
        EXPECT_TRUE((node.HasChild(idChild)));
        EXPECT_TRUE((node.IsAnyChildExist()));

        auto const vChild = node.GetChildren();
        EXPECT_EQ(1, vChild.size());
        EXPECT_EQ(NodeID(idChild), *vChild.begin());

        node.UnlinkChild(idChild);
        auto const vChild2 = node.GetChildren();
        EXPECT_EQ(0, vChild2.size());
      }
    }

  // public:
    TEST(NodeTest, Complex_1D_Only1)
    {
      _complex_ND_Only1<1>();
    }
    TEST(NodeTest, Complex_2D_Only1)
    {
      _complex_ND_Only1<2>();
    }
    TEST(NodeTest, Complex_3D_Only1)
    {
      _complex_ND_Only1<3>();
    }
    TEST(NodeTest, Complex_4D_Only1)
    {
      _complex_ND_Only1<4>();
    }

    TEST(NodeTest, Complex_16D_Only1)
    {
      _complex_ND_Only1<16>();
    }
    // TEST(NodeTest, Complex_24D_Only1) { Complex_ND_Only1<24>(); }


  // private:
    template<dim_t N>
    static void _complex_All_ND()
    {
      using ChildID = typename detail::MortonSpaceIndexing<N, 20>::ChildID;
      using NodeID = typename OrthoTreeBoxND<N>::NodeID;
      auto node = detail::OrthoTreeNodeData<2 << N, NodeID, ChildID, typename OrthoTreeBoxND<N>::EntityID, std::monostate>();

      ChildID constexpr nChild = 1 << N;
      for (ChildID idChild = 0; idChild < nChild; ++idChild)
      {
        auto const kChild = NodeID(idChild);
        node.LinkChild(idChild, kChild);
        EXPECT_TRUE((node.HasChild(kChild)));
        EXPECT_TRUE((node.IsAnyChildExist()));

        auto const vChild = node.GetChildren();
        EXPECT_EQ(static_cast<std::size_t>(idChild) + 1, vChild.size());
      }

      for (ChildID idChild = 0; idChild < nChild; ++idChild)
      {
        auto const kChild = NodeID(idChild);

        node.UnlinkChild(kChild);
        auto const vChildActual = node.GetChildren();
        auto vChildExpected = vector<NodeID>(std::size_t(nChild - idChild - 1));
        std::iota(begin(vChildExpected), end(vChildExpected), NodeID(idChild + 1));
        auto const isPerm = std::is_permutation(vChildActual.begin(), vChildActual.end(), vChildExpected.begin());
        EXPECT_TRUE((isPerm));
      }

      EXPECT_FALSE((node.IsAnyChildExist()));
    }

  // public:
    TEST(NodeTest, Complex_1D_All)
    {
      _complex_All_ND<1>();
    }
    TEST(NodeTest, Complex_2D_All)
    {
      _complex_All_ND<2>();
    }
    TEST(NodeTest, Complex_3D_All)
    {
      _complex_All_ND<3>();
    }
    TEST(NodeTest, Complex_4D_All)
    {
      _complex_All_ND<4>();
    }

    TEST(NodeTest, Complex_8D_All)
    {
      _complex_All_ND<8>();
    }
    // TEST(NodeTest, Complex_24D_All) { Complex_All_ND<24>(); }
  }


  namespace BasicFunctionsTest

  {
    using NodeID = DualtreePoint::NodeID;

    TEST(BasicFunctionsTest, Ctor_Point_SameAsCreate__True)
    {
      auto constexpr N = 16;
      auto constexpr vPoint = getPointSetNo1<N>();
      auto const treeExpected = OrthoTreePointND<N>(vPoint, 3);
      auto const treeActual = OrthoTreePointND<N>(vPoint, 3);

      auto const& nodesE = treeExpected.GetNodes();
      auto const& nodesA = treeActual.GetNodes();
      EXPECT_EQ(nodesE.size(), nodesA.size());

      auto const vidE = treeExpected.GetEntitiesBreadthFirst();
      auto const vidA = treeActual.GetEntitiesBreadthFirst();
      EXPECT_TRUE((vidE == vidA));
    }

    TEST(BasicFunctionsTest, Ctor_Box_SameAsCreate__True)
    {
      auto constexpr N = 16;
      using BoundingBoxXD = BoundingBoxND<N>;
      auto constexpr vBox = array{
        BoundingBoxXD{ { 0.0 }, { 4.0 } },
        BoundingBoxXD{ { 0.0 }, { 2.0 } },
        BoundingBoxXD{ { 2.0 }, { 4.0 } },
        BoundingBoxXD{ { 0.0 }, { 1.0 } },
        BoundingBoxXD{ { 1.0 }, { 2.0 } },
        BoundingBoxXD{ { 2.0 }, { 3.0 } },
        BoundingBoxXD{ { 3.0 }, { 4.0 } }
      };

      auto const treeExpected = OrthoTreeBoxND<N>(vBox, 3);
      auto const treeActual = OrthoTreeBoxND<N>(vBox, 3);

      auto const& nodesE = treeExpected.GetNodes();
      auto const& nodesA = treeActual.GetNodes();
      EXPECT_EQ(nodesE.size(), nodesA.size());

      auto const vidE = treeExpected.GetEntitiesBreadthFirst();
      auto const vidA = treeActual.GetEntitiesBreadthFirst();
      EXPECT_TRUE((vidE == vidA));
    }

    TEST(BasicFunctionsTest, Ctor_Box_Move)
    {
      auto constexpr N = 3;
      using BoundingBoxXD = BoundingBoxND<N>;
      auto constexpr vBox = array{
        BoundingBoxXD{ { 0.0 }, { 4.0 } },
        BoundingBoxXD{ { 0.0 }, { 2.0 } },
        BoundingBoxXD{ { 2.0 }, { 4.0 } },
        BoundingBoxXD{ { 0.0 }, { 1.0 } },
        BoundingBoxXD{ { 1.0 }, { 2.0 } },
        BoundingBoxXD{ { 2.0 }, { 3.0 } },
        BoundingBoxXD{ { 3.0 }, { 4.0 } }
      };

      auto const treeExpected = OrthoTreeBoxND<N>(vBox, 3, std::nullopt, 1);
      auto treeForMove = OrthoTreeBoxND<N>(vBox, 3, std::nullopt, 1);

      auto treeActual(std::move(treeForMove));

      auto const& nodesE = treeExpected.GetNodes();
      auto const& nodesA = treeActual.GetNodes();
      EXPECT_EQ(nodesE.size(), nodesA.size());

      auto const vidE = treeExpected.GetEntitiesBreadthFirst();
      auto const vidA = treeActual.GetEntitiesBreadthFirst();
      EXPECT_TRUE((vidE == vidA));
    }

    TEST(BasicFunctionsTest, Ctor_Box_Copy)
    {
      auto constexpr N = 3;
      using BoundingBoxXD = BoundingBoxND<N>;
      auto constexpr vBox = array{
        BoundingBoxXD{ { 0.0 }, { 4.0 } },
        BoundingBoxXD{ { 0.0 }, { 2.0 } },
        BoundingBoxXD{ { 2.0 }, { 4.0 } },
        BoundingBoxXD{ { 0.0 }, { 1.0 } },
        BoundingBoxXD{ { 1.0 }, { 2.0 } },
        BoundingBoxXD{ { 2.0 }, { 3.0 } },
        BoundingBoxXD{ { 3.0 }, { 4.0 } }
      };

      auto const treeExpected = OrthoTreeBoxND<N>(vBox, 3, std::nullopt, 1);
      auto treeForCopy = OrthoTreeBoxND<N>(vBox, 3, std::nullopt, 1);

      auto treeActual(treeForCopy);

      auto const& nodesE = treeExpected.GetNodes();
      auto const& nodesA = treeActual.GetNodes();
      EXPECT_EQ(nodesE.size(), nodesA.size());

      auto const vidE = treeExpected.GetEntitiesBreadthFirst();
      auto const vidA = treeActual.GetEntitiesBreadthFirst();
      EXPECT_TRUE((vidE == vidA));
    }

    TEST(BasicFunctionsTest, Ctor_Box_CopyAssignment)
    {
      auto constexpr N = 3;
      using BoundingBoxXD = BoundingBoxND<N>;
      auto constexpr vBox = array{
        BoundingBoxXD{ { 0.0 }, { 4.0 } },
        BoundingBoxXD{ { 0.0 }, { 2.0 } },
        BoundingBoxXD{ { 2.0 }, { 4.0 } },
        BoundingBoxXD{ { 0.0 }, { 1.0 } },
        BoundingBoxXD{ { 1.0 }, { 2.0 } },
        BoundingBoxXD{ { 2.0 }, { 3.0 } },
        BoundingBoxXD{ { 3.0 }, { 4.0 } }
      };

      auto const treeExpected = OrthoTreeBoxND<N>(vBox, 3, std::nullopt, 1);
      auto treeForCopy = OrthoTreeBoxND<N>(vBox, 3, std::nullopt, 1);

      auto treeActual = treeForCopy;

      auto const& nodesE = treeExpected.GetNodes();
      auto const& nodesA = treeActual.GetNodes();
      EXPECT_EQ(nodesE.size(), nodesA.size());

      auto const vidE = treeExpected.GetEntitiesBreadthFirst();
      auto const vidA = treeActual.GetEntitiesBreadthFirst();
      EXPECT_TRUE((vidE == vidA));
    }

    TEST(BasicFunctionsTest, GetNodeID__00_1)
    {
      EXPECT_EQ(DualtreePoint::NodeID{ 1 }, SI1D::GetNodeID(0, 0));
    }

    TEST(BasicFunctionsTest, GetNodeIDh__11_3)
    {
      EXPECT_EQ(DualtreePoint::NodeID{ 3 }, SI1D::GetNodeID(1, 1));
    }
    TEST(BasicFunctionsTest, GetNodeID__22_4)
    {
      EXPECT_EQ(DualtreePoint::NodeID{ 6 }, SI1D::GetNodeID(2, 2));
    }

    TEST(BasicFunctionsTest, GetNodeIDh__37_15)
    {
      EXPECT_EQ(DualtreePoint::NodeID{ 15 }, SI1D::GetNodeID(7, 3));
    }

    TEST(BasicFunctionsTest, GetDepth__37_15__3)
    {
      auto const lc = SI1D::GetDepthID(SI1D::GetNodeID(7, 3));
      EXPECT_EQ(depth_t{ 3 }, lc);
    }

    TEST(BasicFunctionsTest, RemoveSentinelBit__37_15__7)
    {
      auto const lc = SI1D::RemoveSentinelBit(SI1D::GetNodeID(7, 3));
      EXPECT_EQ(NodeID{ 7 }, lc);
    }

    TEST(BasicFunctionsTest, Init)
    {
      auto tree = DualtreePoint{};
      auto const bb = BoundingBox1D{ -1, +1 };
      tree.Init(bb, 3, 10);
      auto const& box = tree.GetBox();

      EXPECT_TRUE((AreEqualAlmost(bb, BoundingBox1D{ .Min = box.Min, .Max = box.Max })));

      auto const& nodes = tree.GetNodes();
      EXPECT_EQ(1, nodes.size());
      EXPECT_EQ(3, tree.GetMaxDepthID());
    }

    TEST(BasicFunctionsTest, InitThenInsert)
    {
      auto tree = DualtreeBoxM{};
      auto const handledSpaceDomain = BoundingBox1D{ -2, +2 };
      tree.Init(handledSpaceDomain, 3, 1);

      // Trying to add into the leaf nodes
      {
        auto const boxes = array{
          BoundingBox1D{ -2.0, -1.0 }, // 0, [4]
          BoundingBox1D{ -1.0,  0.0 }, // 1, [5]
          BoundingBox1D{  0.0,  1.0 }, // 2, [6]
          BoundingBox1D{  1.0,  2.0 }, // 3, [7]
          BoundingBox1D{ -1.5,  1.5 }, // 4, [1]
        };

        for (auto const& box : boxes)
        {
          auto const entityIDIfInserted = tree.Add(box, InsertionMode::LowestLeaf);
          EXPECT_TRUE((entityIDIfInserted.has_value()));
        }

        auto const& nodes = tree.GetCore().GetNodes();
        auto const entitiesInBFS = tree.GetCore().GetEntitiesBreadthFirst();
        auto const entitiesInDFS = tree.GetCore().GetEntitiesDepthFirst();

        EXPECT_EQ(7, nodes.size());
        EXPECT_TRUE((AreContainersItemsEqual(std::vector<EntityID>{ 4, 0, 1, 2, 3 }, entitiesInBFS)));
        EXPECT_TRUE((AreContainersItemsEqual(std::vector<EntityID>{ 4, 0, 1, 2, 3 }, entitiesInDFS)));
      }

      // Adding nodes in the current structure
      {
        auto const boxes = array{
          BoundingBox1D{ -1.5, -1.2 }, // 5, [4]
          BoundingBox1D{ -1.2,  0.2 }, // 6, [2]
          BoundingBox1D{  0.0,  1.0 }, // 7, [6]
          BoundingBox1D{ -1.1,  1.2 }  // 8, [1]
        };

        for (auto const box : boxes)
        {
          auto const entityIDIfInserted = tree.Add(box, InsertionMode::ExistingLeaf);
          EXPECT_TRUE((entityIDIfInserted.has_value()));
        }
      }


      // Outside of the handled domain
      {
        auto const boxIsNotInTheHandledSpace = BoundingBox1D{ 1, 3 }; // Min point inside, max point outside, it should adjust the root
        auto const entityIDIfInserted = tree.Add(boxIsNotInTheHandledSpace, InsertionMode::Balanced);
        EXPECT_TRUE((entityIDIfInserted.has_value()));
      }

      auto const& nodes = tree.GetCore().GetNodes();
      auto const entitiesInBFS = tree.GetCore().GetEntitiesBreadthFirst();
      auto const entitiesInDFS = tree.GetCore().GetEntitiesDepthFirst();

      EXPECT_EQ(7, nodes.size());
      EXPECT_TRUE((AreContainersItemsEqual(std::vector<EntityID>{ 4, 8, 9, 6, 0, 5, 1, 2, 7, 3 }, entitiesInBFS)));
      EXPECT_TRUE((AreContainersItemsEqual(std::vector<EntityID>{ 4, 8, 9, 6, 0, 5, 1, 2, 7, 3 }, entitiesInDFS)));

      auto const idsActual = tree.RangeSearch(BoundingBox1D{ -1.1, 0.9 }, RangeSearchMode::Overlap);
      auto const idsExpected = vector<EntityID>{ /* 1. phase */ 0, 1, 2, 4, /* 2. phase */ 6, 7, 8 };
      EXPECT_TRUE((std::ranges::is_permutation(idsActual, idsExpected)));
    }

    TEST(BasicFunctionsTest, InitThenInsertToLeaf)
    {
      auto tree = OrthoTreeBoxND<3, false>();
      tree.Init(
        BoundingBox3D{
          { 0.0, 0.0, 0.0 },
          { 8.0, 8.0, 8.0 }
      },
        5);
      auto const entity = BoundingBox3D{
        { 3.15, 5.95, 6.79 },
        { 3.95, 6.75, 7.59 }
      };

      tree.InsertIntoLeaf(0, entity, InsertionMode::LowestLeaf);
      EXPECT_EQ(2, tree.GetNodeCount());
    }
    TEST(BasicFunctionsTest, InsertWithRebalancing_EmptyTree_SingleEntity)
    {
      using TreeBox = OrthoTreeBoxND<3, true>;

      auto tree = TreeBox();
      tree.Init(
        BoundingBox3D{
          { 0.0, 0.0, 0.0 },
          { 8.0, 8.0, 8.0 }
      },
        5,
        2);

      auto const entities = std::vector<BoundingBox3D>{
        { { 1.0, 1.0, 1.0 }, { 1.5, 1.5, 1.5 } }
      };

      tree.Insert(0, entities[0], entities);

      EXPECT_EQ(1, tree.GetNodeCount());
      EXPECT_EQ(1, tree.GetNodeEntityCount(tree.GetRootNodeValue()));
    }

    TEST(BasicFunctionsTest, InsertWithRebalancing_ExactCapacity_NoSplit)
    {
      using TreeBox = OrthoTreeBoxND<3, true>;

      auto tree = TreeBox();
      tree.Init(
        BoundingBox3D{
          { 0.0, 0.0, 0.0 },
          { 8.0, 8.0, 8.0 }
      },
        5,
        3);

      auto const entities = std::vector<BoundingBox3D>{
        { { 1.0, 1.0, 1.0 }, { 1.5, 1.5, 1.5 } },
        { { 1.1, 1.1, 1.1 }, { 1.6, 1.6, 1.6 } },
        { { 1.2, 1.2, 1.2 }, { 1.7, 1.7, 1.7 } }
      };

      for (size_t i = 0; i < entities.size(); ++i)
        tree.Insert(EntityID(i), entities[i], entities);

      EXPECT_EQ(1, tree.GetNodeCount()) << "Should remain single node at capacity";
      EXPECT_EQ(3, tree.GetNodeEntityCount(tree.GetRootNodeValue()));
    }

    TEST(BasicFunctionsTest, InsertWithRebalancing_OverCapacity_TriggersSplit)
    {
      using TreeBox = OrthoTreeBoxND<3, true>;

      auto tree = TreeBox();
      tree.Init(
        BoundingBox3D{
          { 0.0, 0.0, 0.0 },
          { 8.0, 8.0, 8.0 }
      },
        5,
        2);

      auto const entities = std::vector<BoundingBox3D>{
        { { 1.0, 1.0, 1.0 }, { 1.5, 1.5, 1.5 } },
        { { 1.1, 1.1, 1.1 }, { 1.6, 1.6, 1.6 } },
        { { 1.2, 1.2, 1.2 }, { 1.7, 1.7, 1.7 } }
      };

      tree.Insert(0, entities[0], entities);
      tree.Insert(1, entities[1], entities);
      tree.Insert(2, entities[2], entities);

      EXPECT_TRUE((tree.GetNodeCount() > 1)) << "Should split after exceeding capacity";
    }

    TEST(BasicFunctionsTest, InsertWithRebalancing_LargeEntity_StaysInRoot)
    {
      using TreeBox = OrthoTreeBoxND<3, true>;

      auto tree = TreeBox();
      tree.Init(
        BoundingBox3D{
          { 0.0, 0.0, 0.0 },
          { 8.0, 8.0, 8.0 }
      },
        5,
        1);

      auto const entities = std::vector<BoundingBox3D>{
        { { 2.0, 2.0, 2.0 }, { 6.0, 6.0, 6.0 } }  // Large entity spanning multiple octants
      };

      tree.Insert(0, entities[0], entities);

      auto const allEntities = tree.GetEntitiesBreadthFirst();
      EXPECT_TRUE((allEntities == std::vector<EntityID>{ 0 }));
    }

    TEST(BasicFunctionsTest, InsertWithRebalancing_MultipleOctants_CorrectDistribution)
    {
      using TreeBox = OrthoTreeBoxND<3, true>;

      auto tree = TreeBox();
      tree.Init(
        BoundingBox3D{
          { 0.0, 0.0, 0.0 },
          { 8.0, 8.0, 8.0 }
      },
        5,
        1);

      auto const entities = std::vector<BoundingBox3D>{
        { { 0.5, 0.5, 0.5 }, { 1.0, 1.0, 1.0 } }, // Octant 0
        { { 5.0, 0.5, 0.5 }, { 6.0, 1.0, 1.0 } }, // Octant 1
        { { 0.5, 5.0, 0.5 }, { 1.0, 6.0, 1.0 } }, // Octant 2
        { { 5.0, 5.0, 5.0 }, { 6.0, 6.0, 6.0 } }  // Octant 7
      };

      for (size_t i = 0; i < entities.size(); ++i)
        tree.Insert(EntityID(i), entities[i], entities);

      auto const allEntities = tree.GetEntitiesBreadthFirst();
      auto const nodeCount = tree.GetNodeCount();

      EXPECT_TRUE((allEntities == std::vector<EntityID>{ 0, 1, 2, 3 }));
      EXPECT_EQ(5, nodeCount);
    }

    TEST(BasicFunctionsTest, InsertWithRebalancing_MaxDepth_StopsSubdivision)
    {
      using TreeBox = OrthoTreeBoxND<3, true>;

      auto tree = TreeBox();
      tree.Init(
        BoundingBox3D{
          { 0.0, 0.0, 0.0 },
          { 8.0, 8.0, 8.0 }
      },
        2,
        1); // MaxDepth = 2

      auto const entities = std::vector<BoundingBox3D>{
        { { 1.0, 1.0, 1.0 }, { 1.1, 1.1, 1.1 } },
        { { 1.2, 1.2, 1.2 }, { 1.3, 1.3, 1.3 } },
        { { 1.4, 1.4, 1.4 }, { 1.5, 1.5, 1.5 } },
        { { 1.6, 1.6, 1.6 }, { 1.7, 1.7, 1.7 } }
      };

      for (size_t i = 0; i < entities.size(); ++i)
        tree.Insert(EntityID(i), entities[i], entities);

      auto maxDepthID = 0;
      tree.TraverseNodesDepthFirst([&](auto const& nodeValue) {
        auto depthID = TreeBox::SI::GetDepthID(nodeValue->first);
        maxDepthID = std::max(maxDepthID, static_cast<int>(depthID));
        return TraverseControl::Continue;
      });

      EXPECT_TRUE((maxDepthID <= 2)) << "Should not exceed max depth";
    }

    TEST(BasicFunctionsTest, InsertWithRebalancing_TinyEntities_ProperPlacement)
    {
      using TreeBox = OrthoTreeBoxND<3, true>;

      auto tree = TreeBox();
      tree.Init(
        BoundingBox3D{
          { 0.0, 0.0, 0.0 },
          { 8.0, 8.0, 8.0 }
      },
        5,
        1);

      auto const entities = std::vector<BoundingBox3D>{
        {    { 1.0, 1.0, 1.0 }, { 1.01, 1.01, 1.01 } },
        { { 1.02, 1.02, 1.02 }, { 1.03, 1.03, 1.03 } }
      };

      tree.Insert(0, entities[0], entities);
      tree.Insert(1, entities[1], entities);

      EXPECT_TRUE((tree.GetNodeCount() > 1)) << "Tiny entities should trigger subdivision";
    }

    TEST(BasicFunctionsTest, InsertWithRebalancing_IdenticalLocations_HandledCorrectly)
    {
      using TreeBox = OrthoTreeBoxND<3, true>;

      auto tree = TreeBox();
      tree.Init(
        BoundingBox3D{
          { 0.0, 0.0, 0.0 },
          { 8.0, 8.0, 8.0 }
      },
        5,
        1);

      auto const entities = std::vector<BoundingBox3D>{
        { { 1.0, 1.0, 1.0 }, { 1.1, 1.1, 1.1 } },
        { { 1.0, 1.0, 1.0 }, { 1.1, 1.1, 1.1 } }
      };

      tree.Insert(0, entities[0], entities);
      tree.Insert(1, entities[1], entities);

      auto maxDepthID = 0;
      tree.TraverseNodesDepthFirst([&](auto const& nodeValue) {
        auto depthID = TreeBox::SI::GetDepthID(nodeValue->first);
        maxDepthID = std::max(maxDepthID, static_cast<int>(depthID));
        return TraverseControl::Continue;
      });

      EXPECT_TRUE((maxDepthID == 5)) << "Should add into a deepest level.";
    }

    TEST(BasicFunctionsTest, InsertWithRebalancing_MixedSizes_OptimalDistribution)
    {
      using TreeBox = OrthoTreeBoxND<3, true>;

      auto tree = TreeBox();
      tree.Init(
        BoundingBox3D{
          { 0.0, 0.0, 0.0 },
          { 8.0, 8.0, 8.0 }
      },
        5,
        2);

      auto const entities = std::vector<BoundingBox3D>{
        { { 1.0, 1.0, 1.0 }, { 1.1, 1.1, 1.1 } }, // Tiny
        { { 4.0, 4.0, 4.0 }, { 5.0, 5.0, 5.0 } }, // Medium
        { { 0.5, 0.5, 0.5 }, { 7.5, 7.5, 7.5 } }  // Large (spans most of space)
      };

      for (size_t i = 0; i < entities.size(); ++i)
        tree.Insert(EntityID(i), entities[i], entities);

      auto const allEntities = tree.GetEntitiesDepthFirst();
      auto const nodeCount = tree.GetNodeCount();

      EXPECT_TRUE((allEntities == std::vector<EntityID>{ 2, 0, 1 }));
      EXPECT_EQ(3, nodeCount);
    }

    TEST(BasicFunctionsTest, InitThenInsertWithRebalancingLooseOctree)
    {
      using TreeBox = OrthoTreeBoxND<3, true>;

      // Scenario:
      // 1. Initialize a small tree (8x8x8).
      // 2. Insert items to trigger rebalancing (split).
      // 3. Verify that items are moved to children.
      // 4. Insert an item that CANNOT be moved to children (stuck in root).

      auto tree = TreeBox();
      tree.Init(
        BoundingBox3D{
          { 0.0, 0.0, 0.0 },
          { 8.0, 8.0, 8.0 }
      },
        5, // MaxDepth
        2  // MaxElementNo (Small capacity to force splits)
      );

      auto const entities = std::vector<BoundingBox3D>{
        { { 1.0, 1.0, 1.0 }, { 1.5, 1.5, 1.5 } }, // 0: Small, fits in Child-0
        { { 1.1, 1.1, 1.1 }, { 1.6, 1.6, 1.6 } }, // 1: Small, fits in Child-0
        { { 1.2, 1.2, 1.2 }, { 1.7, 1.7, 1.7 } }, // 2: Small, fits in Child-0, Triggers Split
        { { 1.0, 1.0, 1.0 }, { 7.0, 7.0, 7.0 } }  // 3: Large, center at 4.0, stays in Root
      };

      // 1. Insert first two elements (Under capacity)
      tree.Insert(0, entities[0], entities);
      tree.Insert(1, entities[1], entities);

      EXPECT_EQ(1, tree.GetNodeCount()) << "Tree should have 1 node (Root) before split";
      EXPECT_EQ(2, tree.GetNodeEntityCount(tree.GetRootNodeValue())) << "Root should have 2 entities";

      // 2. Insert third element (Triggers split)
      // All 3 items are small and belong to the same octant.
      tree.Insert(2, entities[2], entities);

      // 3. Insert large element (Stays in Root)
      tree.Insert(3, entities[3], entities);

      // Verification via BFS & DFS Order implies structure
      // Expected:
      // - Root: Contains entity 3 (Large, stuck)
      // - Child: Contains entities 0, 1, 2 (Small, moved down)

      auto const idsInBFS = tree.GetEntitiesBreadthFirst(TreeBox::GetRootNodeID(), true);
      auto const idsInDFS = tree.GetEntitiesDepthFirst(TreeBox::SI::GetRootKey(), true);

      EXPECT_TRUE((idsInBFS == std::vector<EntityID>{ 3, 0, 1, 2 }));
      EXPECT_TRUE((idsInDFS == std::vector<EntityID>{ 3, 0, 1, 2 }));
    }

    TEST(BasicFunctionsTest, EraseFromEmpty)
    {
      auto const handledSpaceDomain = BoundingBox1D{ -2, +2 };

      auto tree = DualtreeBoxM{};
      tree.Reset();
      tree.Init(handledSpaceDomain, 10, 4);
      auto const isErased = tree.Erase(0);
      EXPECT_FALSE((isErased));
    }

    TEST(BasicFunctionsTest, VisitNodes__points__0123)
    {
      auto constexpr vpt = array{ Point1D{ 0.0 }, Point1D{ 1.0 }, Point1D{ 2.0 }, Point1D{ 3.0 } };
      auto const tree = DualtreePoint(vpt, 2, std::nullopt, 2);

      auto const ids = tree.GetEntitiesBreadthFirst();
      EXPECT_TRUE((ids == vector<EntityID>{ 0, 1, 2, 3 }));
    }

    TEST(BasicFunctionsTest, VisitNodes__boxes__0123)
    {
      auto constexpr vBox = array{
        BoundingBox1D{ 0.0, 1.0 },
        BoundingBox1D{ 1.0, 2.0 },
        BoundingBox1D{ 2.0, 3.0 },
        BoundingBox1D{ 3.0, 4.0 },
        BoundingBox1D{ 0.0, 2.0 },
        BoundingBox1D{ 2.0, 4.0 },
        BoundingBox1D{ 0.0, 4.0 }
      };
      auto const tree = DualtreeBox(vBox, 3, std::nullopt, 1);

      auto const ids = tree.GetEntitiesBreadthFirst();
      EXPECT_TRUE((ids == vector<EntityID>{ 6, 4, 5, 0, 1, 2, 3 }));
    }


    TEST(BasicFunctionsTest, EraseId__2__EmptyNode11)
    {
      auto constexpr vpt = array{ Point1D{ 0.0 }, Point1D{ 1.0 }, Point1D{ 2.0 }, Point1D{ 3.0 } };
      auto tree = DualtreePoint(vpt, 2, std::nullopt, 1);

      auto const kNode = SI1D::GetNodeID(2, 2);
      EXPECT_EQ(tree.GetNodeEntityCount(tree.GetNodeValue(kNode)), 1);
      tree.Erase(2);
      auto const& nodes = tree.GetNodes();
      EXPECT_TRUE((nodes.find(kNode) == nodes.end()));

      auto const kNode3 = SI1D::GetNodeID(3, 2);
      EXPECT_EQ(*tree.GetNodeEntities(tree.GetNodeValue(kNode3)).begin(), 2);
    }


    TEST(BasicFunctionsTest, UpdateIndexes__None__NothingsChange)
    {
      auto constexpr vBox = array{
        BoundingBox1D{ 0.0, 1.0 },
        BoundingBox1D{ 1.0, 2.0 },
        BoundingBox1D{ 2.0, 3.0 },
        BoundingBox1D{ 3.0, 4.0 },
        BoundingBox1D{ 0.0, 2.0 },
        BoundingBox1D{ 2.0, 4.0 },
        BoundingBox1D{ 0.0, 4.0 }
      };
      auto tree = DualtreeBox(vBox, 3, std::nullopt, 1);
      tree.UpdateIndexes({});

      auto const ids = tree.GetEntitiesBreadthFirst();
      EXPECT_TRUE((ids == vector<EntityID>{ 6, 4, 5, 0, 1, 2, 3 }));
    }


    TEST(BasicFunctionsTest, UpdateIndexes__2to7__6450173)
    {
      auto constexpr vBox = array{
        BoundingBox1D{ 0.0, 1.0 },
        BoundingBox1D{ 1.0, 2.0 },
        BoundingBox1D{ 2.0, 3.0 },
        BoundingBox1D{ 3.0, 4.0 },
        BoundingBox1D{ 0.0, 2.0 },
        BoundingBox1D{ 2.0, 4.0 },
        BoundingBox1D{ 0.0, 4.0 }
      };
      auto tree = DualtreeBox(vBox, 3, std::nullopt, 1);
      tree.UpdateIndexes(
        {
          { 2, 7 }
      });

      auto const ids = tree.GetEntitiesBreadthFirst();
      EXPECT_TRUE((ids == vector<EntityID>{ 6, 4, 5, 0, 1, 7, 3 }));
    }

    TEST(BasicFunctionsTest, UpdateIndexes__2toMax__2Removed)
    {
      auto constexpr vBox = array{
        BoundingBox1D{ 0.0, 1.0 },
        BoundingBox1D{ 1.0, 2.0 },
        BoundingBox1D{ 2.0, 3.0 },
        BoundingBox1D{ 3.0, 4.0 },
        BoundingBox1D{ 0.0, 2.0 },
        BoundingBox1D{ 2.0, 4.0 },
        BoundingBox1D{ 0.0, 4.0 }
      };
      auto tree = DualtreeBox(vBox, 3, std::nullopt, 1);
      tree.UpdateIndexes(
        {
          { 2, 1 },
          { 1, 2 }
      });

      auto const ids = tree.GetEntitiesBreadthFirst();
      EXPECT_TRUE((ids == vector<EntityID>{ 6, 4, 5, 0, 2, 1, 3 }));
    }

    TEST(BasicFunctionsTest, UpdateIndexes__Swap3And6)
    {
      auto constexpr vBox = array{
        BoundingBox1D{ 0.0, 1.0 },
        BoundingBox1D{ 1.0, 2.0 },
        BoundingBox1D{ 2.0, 3.0 },
        BoundingBox1D{ 3.0, 4.0 },
        BoundingBox1D{ 0.0, 2.0 },
        BoundingBox1D{ 2.0, 4.0 },
        BoundingBox1D{ 0.0, 4.0 }
      };
      auto tree = DualtreeBox(vBox, 3, std::nullopt, 1);
      tree.UpdateIndexes(
        {
          { 3, 6 },
          { 6, 3 },
      });


      auto const ids = tree.GetEntitiesBreadthFirst();
      EXPECT_TRUE((ids == vector<EntityID>{ 3, 4, 5, 0, 1, 2, 6 }));
    }

    TEST(BasicFunctionsTest, UpdateIndexes__Swap0And1)
    {
      auto constexpr vBox = array{
        BoundingBox1D{ 0.0, 1.0 },
        BoundingBox1D{ 1.0, 2.0 },
        BoundingBox1D{ 2.0, 3.0 },
        BoundingBox1D{ 3.0, 4.0 },
        BoundingBox1D{ 0.0, 2.0 },
        BoundingBox1D{ 2.0, 4.0 },
        BoundingBox1D{ 0.0, 4.0 }
      };
      auto tree = DualtreeBox(vBox, 3, std::nullopt, 1);
      tree.UpdateIndexes(
        {
          { 1, 0 },
          { 0, 1 },
      });


      auto const ids = tree.GetEntitiesBreadthFirst();
      EXPECT_TRUE((ids == vector<EntityID>{ 6, 4, 5, 1, 0, 2, 3 }));
    }


    TEST(BasicFunctionsTest, UpdateIndexes__3to4_4to5_5to6_6Removed)
    {
      auto constexpr vBox = array{
        BoundingBox1D{ 0.0, 1.0 },
        BoundingBox1D{ 1.0, 2.0 },
        BoundingBox1D{ 2.0, 3.0 },
        BoundingBox1D{ 3.0, 4.0 },
        BoundingBox1D{ 0.0, 2.0 },
        BoundingBox1D{ 2.0, 4.0 },
        BoundingBox1D{ 0.0, 4.0 }
      };
      auto tree = DualtreeBox(vBox, 3, std::nullopt, 1);
      tree.UpdateIndexes(
        {
          { 6, 3 },
          { 3, 6 },
          { 4, 5 },
          { 5, 1 },
          { 1, 4 },
      });

      auto const ids = tree.GetEntitiesBreadthFirst();

      EXPECT_TRUE((ids == vector<EntityID>{ 3, 5, 1, 0, 4, 2, 6 })); // instead of { 6, 4, 5, 0, 1, 2, 3 }
    }

    TEST(BasicFunctionsTest, Clear__EmptyRootRemains)
    {
      auto constexpr vBox = array{
        BoundingBox1D{ 0.0, 4.0 },
        BoundingBox1D{ 0.0, 2.0 },
        BoundingBox1D{ 2.0, 4.0 },
        BoundingBox1D{ 0.0, 1.0 },
        BoundingBox1D{ 1.0, 2.0 },
        BoundingBox1D{ 2.0, 3.0 },
        BoundingBox1D{ 3.0, 4.0 }
      };
      auto tree = DualtreeBox(vBox, 3, std::nullopt, 1);

      auto const& nodes = tree.GetNodes();
      EXPECT_EQ(7, nodes.size());

      tree.Clear();
      EXPECT_EQ(1, nodes.size());
      EXPECT_TRUE((tree.IsNodeEntitiesEmpty(tree.GetNodeValue(DualtreeBox::SI::GetNodeID(0, 0)))));
    }

    TEST(BasicFunctionsTest, Contains_EmptyTree__False)
    {
      auto const tree = DualtreePoint({}, 3, std::nullopt, 2);
      auto const isPointContained = tree.Contains({}, {}, 1.0);
      EXPECT_FALSE((isPointContained));
    }

  // private:
    template<dim_t N>
    bool _isOnePointTreeContains()
    {
      using PointXD = PointND<N>;
      auto constexpr vPoint = std::array<PointXD, 1>{ PointXD{ 1.1 } };
      auto const tree = OrthoTreePointND<N>(vPoint, 3);
      return tree.Contains(vPoint[0], vPoint, 0.01);
    }

  // public:
    TEST(BasicFunctionsTest, Contains_OnePointTree_1D__True)
    {
      EXPECT_TRUE((_isOnePointTreeContains<1>()));
    }

    TEST(BasicFunctionsTest, Contains_OnePointTree_2D__True)
    {
      EXPECT_TRUE((_isOnePointTreeContains<2>()));
    }

    TEST(BasicFunctionsTest, Contains_OnePointTree_3D__True)
    {
      EXPECT_TRUE((_isOnePointTreeContains<3>()));
    }

    TEST(BasicFunctionsTest, Contains_OnePointTree_4D__True)
    {
      EXPECT_TRUE((_isOnePointTreeContains<4>()));
    }

    TEST(BasicFunctionsTest, Contains_OnePointTree_16D__True)
    {
      EXPECT_TRUE((_isOnePointTreeContains<16>()));
    }

  // private:
    template<dim_t N>
    bool _isTreeContainsPointSetNo1()
    {
      auto constexpr vPoint = getPointSetNo1<N>();
      auto const tree = OrthoTreePointND<N>(vPoint, 3);
      return tree.Contains(vPoint[4], vPoint, 0.0);
    }

  // public:
    TEST(BasicFunctionsTest, Contains_PointSetNo1_2D__True)
    {
      EXPECT_TRUE((_isTreeContainsPointSetNo1<2>()));
    }

    TEST(BasicFunctionsTest, Contains_PointSetNo1_3D__True)
    {
      EXPECT_TRUE((_isTreeContainsPointSetNo1<3>()));
    }

    TEST(BasicFunctionsTest, Contains_PointSetNo1_16D__True)
    {
      EXPECT_TRUE((_isTreeContainsPointSetNo1<16>()));
    }

    TEST(BasicFunctionsTest, Contains_PointSetNo1_16D__False)
    {
      auto constexpr N = 16;
      auto constexpr vPoint = getPointSetNo1<N>();
      auto const tree = OrthoTreePointND<N>(vPoint, 3);
      auto const fContain = tree.Contains(PointND<N>{ -1.0, -1.0 }, vPoint, 0.0);

      EXPECT_FALSE((fContain));
    }

    TEST(BasicFunctionsTest, Contains_PointSetNo1_3D__False)
    {
      auto constexpr N = 3;
      auto constexpr vPoint = getPointSetNo1<N>();
      auto const tree = OrthoTreePointND<N>(vPoint, 3);
      auto const fContain = tree.Contains(PointND<N>{ 7.0, 9.0 }, vPoint, 0.0);

      EXPECT_FALSE((fContain));
    }


    TEST(BasicFunctionsTest, Move__Empty__NotCrash)
    {
      auto tree = DualtreePoint();
      tree.Move({});
    }


  // private:
    template<typename tree_type, std::size_t N>
    bool _isMoveOfTwoTreeProper(tree_type const& tPre, tree_type const& tAfter, PointND<N> const& vMoveExpected)
    {
      using GA = GeneralGeometryAdapterTemplate<N, VectorND<N>, BoundingBoxND<N>, RayND<N>, PlaneND<N>>;
      auto constexpr rAcc = std::numeric_limits<double>::min();

      [[maybe_unused]] auto const ptPre = &tPre;
      [[maybe_unused]] auto const ptAfter = &tAfter;
      auto const nodesPre = tPre.GetNodes();
      auto const nodesAfter = tAfter.GetNodes();

      auto const nNode = nodesPre.size();
      auto vMatch = vector<bool>(nNode);
      std::transform(std::begin(nodesPre), std::end(nodesPre), std::begin(nodesAfter), begin(vMatch), [&](auto const& pairPre, auto const& pairAfter) {
        // same order, same box sizes

        if (pairPre.first != pairAfter.first)
          return false;

        auto vMoveActual = PointND<N>{};
        auto const& centerPre = ptPre->GetNodeMinPoint(&pairPre);
        auto const& centerAfter = ptAfter->GetNodeMinPoint(&pairAfter);
        for (dim_t dimensionID = 0; dimensionID < N; ++dimensionID)
          GA::SetPointC(vMoveActual, dimensionID, centerAfter[dimensionID] - centerPre[dimensionID]);

        return GA::ArePointsEqual(vMoveActual, vMoveExpected, rAcc);
      });

      return std::ranges::all_of(vMatch, [](auto const bMatch) { return bMatch; });
    }


  // public:
    TEST(BasicFunctionsTest, MoveP__P0__Same)
    {
      auto constexpr vpt = array{ Point1D{ 0.0 }, Point1D{ 1.0 }, Point1D{ 2.0 }, Point1D{ 3.0 } };
      auto const treePre = DualtreePoint(vpt, 3, std::nullopt, 2);

      auto treeAfter = treePre;
      auto constexpr vMove = Point1D{ 0.0 };
      treeAfter.Move(vMove);

      EXPECT_TRUE((_isMoveOfTwoTreeProper(treePre, treeAfter, vMove)));
    }


    TEST(BasicFunctionsTest, MoveP__P1__Moved)
    {
      auto constexpr vpt = array{ Point1D{ 0.0 }, Point1D{ 1.0 }, Point1D{ 2.0 }, Point1D{ 3.0 } };
      auto const treePre = DualtreePoint(vpt, 3, std::nullopt, 2);

      auto treeAfter = treePre;
      auto constexpr vMove = Point1D{ 1.0 };
      treeAfter.Move(vMove);

      EXPECT_TRUE((_isMoveOfTwoTreeProper(treePre, treeAfter, vMove)));
    }


    TEST(BasicFunctionsTest, MoveB__M20__Moved)
    {
      auto constexpr vBox = array{
        BoundingBox1D{ 0.0, 1.0 },
        BoundingBox1D{ 1.0, 2.0 },
        BoundingBox1D{ 2.0, 3.0 },
        BoundingBox1D{ 3.0, 4.0 },
        BoundingBox1D{ 0.0, 2.0 },
        BoundingBox1D{ 2.0, 4.0 },
        BoundingBox1D{ 0.0, 4.0 }
      };
      auto treePre = DualtreeBox(vBox, 3, std::nullopt, 1);

      auto treeAfter = treePre;
      auto constexpr vMove = Point1D{ -20.0 };
      treeAfter.Move(vMove);

      EXPECT_TRUE((_isMoveOfTwoTreeProper(treePre, treeAfter, vMove)));
    }


    TEST(BasicFunctionsTest, RayIntersectedFirst__XRay__None)
    {
      auto constexpr rayBase = Point2D{ 0.0, -1.0 };
      auto constexpr rayHeading = Point2D{ 1.0, 0.0 };
      auto constexpr boxes = array{
        BoundingBox2D{ { 0.0, 0.0 }, { 1.0, 1.0 } },
        BoundingBox2D{ { 1.0, 1.0 }, { 2.0, 2.0 } },
        BoundingBox2D{ { 2.0, 2.0 }, { 3.0, 3.0 } },
        BoundingBox2D{ { 3.0, 3.0 }, { 4.0, 4.0 } },
      };
      auto const qt = QuadtreeBox(boxes, 3, std::nullopt, 2);
      auto const oid = qt.RayIntersectedFirst(rayBase, rayHeading, boxes, 0);
      EXPECT_TRUE((oid.empty()));
    }


    TEST(BasicFunctionsTest, RayIntersectedFirst__YRay__None)
    {
      auto constexpr rayBase = Point2D{ 5.0, 0.0 };
      auto constexpr rayHeading = Point2D{ 0.0, 1.0 };
      auto constexpr boxes = array{
        BoundingBox2D{ { 0.0, 0.0 }, { 1.0, 1.0 } },
        BoundingBox2D{ { 1.0, 1.0 }, { 2.0, 2.0 } },
        BoundingBox2D{ { 2.0, 2.0 }, { 3.0, 3.0 } },
        BoundingBox2D{ { 3.0, 3.0 }, { 4.0, 4.0 } },
      };
      auto const qt = QuadtreeBox(boxes, 3, std::nullopt, 2);
      auto const oid = qt.RayIntersectedFirst(rayBase, rayHeading, boxes, 0);
      EXPECT_TRUE((oid.empty()));
    }


    TEST(BasicFunctionsTest, RayIntersectedFirst__YRay2__None)
    {
      auto constexpr rayBase = Point2D{ 4.2, 4.4 };
      auto constexpr rayHeading = Point2D{ std::numbers::sqrt2 * 0.5, std::numbers::sqrt2 * 0.5 };
      auto constexpr boxes = array{
        BoundingBox2D{ { 0.0, 0.0 }, { 1.0, 1.0 } },
        BoundingBox2D{ { 1.0, 1.0 }, { 2.0, 2.0 } },
        BoundingBox2D{ { 2.0, 2.0 }, { 3.0, 3.0 } },
        BoundingBox2D{ { 3.0, 3.0 }, { 4.0, 4.0 } },
      };
      auto const qt = QuadtreeBox(boxes, 3, std::nullopt, 2);
      auto const oid = qt.RayIntersectedFirst(rayBase, rayHeading, boxes, 0);
      EXPECT_TRUE((oid.empty()));
    }


    TEST(BasicFunctionsTest, RayIntersectedFirst_InsideTheTree_Neg_None)
    {
      auto constexpr rayBase = Point2D{ 1.5, 2.6 };
      auto constexpr rayHeading = Point2D{ std::numbers::sqrt2 * -0.5, std::numbers::sqrt2 * -0.5 };
      auto constexpr boxes = array{
        BoundingBox2D{ { 0.0, 0.0 }, { 1.0, 1.0 } },
        BoundingBox2D{ { 1.0, 1.0 }, { 2.0, 2.0 } },
        BoundingBox2D{ { 2.0, 2.0 }, { 3.0, 3.0 } },
        BoundingBox2D{ { 3.0, 3.0 }, { 4.0, 4.0 } },
      };
      auto const qt = QuadtreeBox(boxes, 3, std::nullopt, 2);
      auto const oid = qt.RayIntersectedFirst(rayBase, rayHeading, boxes, 0);
      EXPECT_TRUE((oid.empty()));
    }


    TEST(BasicFunctionsTest, RayIntersectedFirst_OutsideTheTree_Neg_None)
    {
      auto constexpr rayBase = Point2D{ 7.0, 2.0 };
      auto constexpr rayHeading = Point2D{ std::numbers::sqrt2 * -0.5, std::numbers::sqrt2 * -0.5 };
      auto constexpr boxes = array{
        BoundingBox2D{ { 0.0, 0.0 }, { 1.0, 1.0 } },
        BoundingBox2D{ { 1.0, 1.0 }, { 2.0, 2.0 } },
        BoundingBox2D{ { 2.0, 2.0 }, { 3.0, 3.0 } },
        BoundingBox2D{ { 3.0, 3.0 }, { 4.0, 4.0 } },
      };
      auto const qt = QuadtreeBox(boxes, 3, std::nullopt, 2);
      auto const oid = qt.RayIntersectedFirst(rayBase, rayHeading, boxes, 0);
      EXPECT_TRUE((oid.empty()));
    }


    TEST(BasicFunctionsTest, RayIntersectedFirst__XRay__0)
    {
      auto constexpr rayBase = Point2D{ 0.0, 0.5 };
      auto constexpr rayHeading = Point2D{ 1.0, 0.0 };
      auto constexpr boxes = array{
        BoundingBox2D{ { 0.0, 0.0 }, { 1.0, 1.0 } },
        BoundingBox2D{ { 1.0, 1.0 }, { 2.0, 2.0 } },
        BoundingBox2D{ { 2.0, 2.0 }, { 3.0, 3.0 } },
        BoundingBox2D{ { 3.0, 3.0 }, { 4.0, 4.0 } },
      };
      auto const qt = QuadtreeBox(boxes, 3, std::nullopt, 2);
      auto const oid = qt.RayIntersectedFirst(rayBase, rayHeading, boxes, 0);
      EXPECT_EQ(1, oid.size());
      EXPECT_EQ(0, oid[0]);
    }


    TEST(BasicFunctionsTest, RayIntersectedFirst__XRay__1)
    {
      auto constexpr rayBase = Point2D{ 0.0, 2.0 };
      auto constexpr rayHeading = Point2D{ 1.0, 0.0 };
      auto constexpr boxes = array{
        BoundingBox2D{ { 0.0, 0.0 }, { 1.0, 1.0 } },
        BoundingBox2D{ { 1.0, 1.0 }, { 2.0, 2.0 } },
        BoundingBox2D{ { 2.0, 2.0 }, { 3.0, 3.0 } },
        BoundingBox2D{ { 3.0, 3.0 }, { 4.0, 4.0 } },
      };
      auto const qt = QuadtreeBox(boxes, 3, std::nullopt, 2);
      auto const oid = qt.RayIntersectedFirst(rayBase, rayHeading, boxes, 0);
      EXPECT_EQ(2, oid.size());
      EXPECT_EQ(2, oid[0]);
      EXPECT_EQ(1, oid[1]);
    }


    TEST(BasicFunctionsTest, RayIntersectedFirst__XRayInsideTheTreePos__1)
    {
      auto constexpr rayBase = Point2D{ 1.5, 1.0 };
      auto constexpr rayHeading = Point2D{ 1.0, 0.0 };
      auto constexpr boxes = array{
        BoundingBox2D{ { 0.0, 0.0 }, { 1.0, 1.0 } },
        BoundingBox2D{ { 1.0, 1.0 }, { 2.0, 2.0 } },
        BoundingBox2D{ { 2.0, 2.0 }, { 3.0, 3.0 } },
        BoundingBox2D{ { 3.0, 3.0 }, { 4.0, 4.0 } },
      };
      auto const qt = QuadtreeBox(boxes, 3, std::nullopt, 2);
      auto const oid = qt.RayIntersectedFirst(rayBase, rayHeading, boxes, 0);
      EXPECT_EQ(1, oid.size());
      EXPECT_EQ(1, oid[0]);
    }


    TEST(BasicFunctionsTest, RayIntersectedFirst__XRayInsideTheTreeNeg__1)
    {
      auto constexpr rayBase = Point2D{ 1.5, 1.0 };
      auto constexpr rayHeading = Point2D{ -1.0, 0.0 };
      auto constexpr boxes = array{
        BoundingBox2D{ { 0.0, 0.0 }, { 1.0, 1.0 } },
        BoundingBox2D{ { 1.0, 1.0 }, { 2.0, 2.0 } },
        BoundingBox2D{ { 2.0, 2.0 }, { 3.0, 3.0 } },
        BoundingBox2D{ { 3.0, 3.0 }, { 4.0, 4.0 } },
      };
      auto const qt = QuadtreeBox(boxes, 3, std::nullopt, 2);
      auto const oid = qt.RayIntersectedFirst(rayBase, rayHeading, boxes, 0);
      EXPECT_EQ(2, oid.size());
      EXPECT_EQ(0, oid[0]);
      EXPECT_EQ(1, oid[1]);
    }


    TEST(BasicFunctionsTest, RayIntersectedFirst__XRayInsideTheTreeNeg__3)
    {
      auto const rayBase = Point2D{ 3.5, 3.5 };
      auto const rayHeading = Point2D{ -1.0 / std::sqrt(5.0), -2.0 / std::sqrt(5.0) };
      auto constexpr boxes = array{
        BoundingBox2D{ { 0.0, 0.0 }, { 1.0, 1.0 } },
        BoundingBox2D{ { 1.0, 1.0 }, { 2.0, 2.0 } },
        BoundingBox2D{ { 2.0, 2.0 }, { 3.0, 3.0 } },
        BoundingBox2D{ { 3.0, 3.0 }, { 4.0, 4.0 } },
      };
      auto const qt = QuadtreeBox(boxes, 3, std::nullopt, 2);
      auto const oid = qt.RayIntersectedFirst(rayBase, rayHeading, boxes, 0);
      EXPECT_EQ(1, oid.size());
      EXPECT_EQ(3, oid[0]);
    }

    TEST(BasicFunctionsTest, RayIntersectedFirst_MultipleHits_ReturnsReverseSorted)
    {
      auto constexpr rayBase = Point2D{ -1.0, 0.5 };
      auto constexpr rayHeading = Point2D{ 1.0, 0.0 };
      auto constexpr boxes = std::array{
        BoundingBox2D{ { 0.0, 0.0 }, { 5.0, 1.0 } }, // enter  ~1.0
        BoundingBox2D{ { 1.0, 0.0 }, { 3.0, 1.0 } }, // enter  ~3.0
        BoundingBox2D{ { 2.0, 0.0 }, { 4.0, 1.0 } }, // enter  ~5.0
      };

      auto const qt = QuadtreeBox(boxes, 3, std::nullopt, 2);
      auto const result = qt.RayIntersectedFirst(rayBase, rayHeading, boxes, 0.0);

      EXPECT_EQ(3, result.size());
      EXPECT_EQ((size_t)2, (size_t)result[0]);
      EXPECT_EQ((size_t)1, (size_t)result[1]);
      EXPECT_EQ((size_t)0, (size_t)result[2]);
    }

    TEST(BasicFunctionsTest, RayIntersectedFirst_Tolerance_AllowsGrazingHit)
    {
      auto constexpr rayBase = Point2D{ 0.0, 1.001 };
      auto constexpr rayHeading = Point2D{ 1.0, 0.0 };

      auto constexpr boxes = std::array{
        BoundingBox2D{ { 1.0, 0.0 }, { 2.0, 1.0 } }
      };

      auto const qt = QuadtreeBox(boxes, 1, std::nullopt, 1);

      auto const result = qt.RayIntersectedFirst(rayBase, rayHeading, boxes, 0.01);

      EXPECT_EQ(1, result.size());
      EXPECT_EQ((size_t)0, (size_t)result[0]);
    }

    TEST(BasicFunctionsTest, RayIntersectedFirst_ToleranceIncrement_PrunesLaterHits)
    {
      auto constexpr rayBase = Point2D{ -1.0, 0.5 };
      auto constexpr rayHeading = Point2D{ 1.0, 0.0 };

      auto constexpr boxes = std::array{
        BoundingBox2D{  { 0.0, 0.0 },  { 1.0, 1.0 } }, // hitDistance ~1
        BoundingBox2D{ { 10.0, 0.0 }, { 11.0, 1.0 } }, // hitDistance ~11 -> prune
      };

      auto const qt = QuadtreeBox(boxes, 3, std::nullopt, 2);

      auto const result = qt.RayIntersectedFirst(rayBase, rayHeading, boxes, 0.0, 0.5);

      EXPECT_EQ(1, result.size());
      EXPECT_EQ((size_t)0, (size_t)result[0]);
    }

    TEST(BasicFunctionsTest, RayIntersectedFirst_MaxDistance_CutsOff)
    {
      auto constexpr rayBase = Point2D{ -1.0, 0.5 };
      auto constexpr rayHeading = Point2D{ 1.0, 0.0 };

      auto constexpr boxes = std::array{
        BoundingBox2D{  { 0.0, 0.0 },  { 3.0, 1.0 } }, // enter ~1, exit 4
        BoundingBox2D{  { 1.0, 0.0 },  { 2.0, 1.0 } }, // enter ~2, exit 3
        BoundingBox2D{ { 10.0, 0.0 }, { 11.0, 1.0 } }, // enter ~11
      };

      auto const qt = QuadtreeBox(boxes, 3, std::nullopt, 2);

      auto const result = qt.RayIntersectedFirst(rayBase, rayHeading, boxes, 0.0, 0.0, 5.0);

      EXPECT_EQ(2, result.size());
      EXPECT_EQ((size_t)1, (size_t)result[0]);
      EXPECT_EQ((size_t)0, (size_t)result[1]);
    }

    TEST(BasicFunctionsTest, RayIntersectedFirst_UserHitTest_FiltersEntity)
    {
      auto constexpr rayBase = Point2D{ -1.0, 0.5 };
      auto constexpr rayHeading = Point2D{ 1.0, 0.0 };

      auto constexpr boxes = std::array{
        BoundingBox2D{ { 0.0, 0.0 }, { 1.0, 1.0 } },
        BoundingBox2D{ { 2.0, 0.0 }, { 3.0, 1.0 } },
      };

      auto const qt = QuadtreeBox(boxes, 3, std::nullopt, 2);

      auto hitTest = [&](size_t id) -> std::optional<double> {
        if (id == 0)
          return std::nullopt; // [0] excluded

        return 3.0; // [1] distance
      };

      auto const result = qt.RayIntersectedFirst(rayBase, rayHeading, boxes, 0.0, 0.0, std::numeric_limits<double>::max(), hitTest);

      EXPECT_EQ(1, result.size());
      EXPECT_EQ((size_t)1, (size_t)result[0]);
    }

    TEST(BasicFunctionsTest, RayIntersectedAll_2D_General_234)
    {
      auto constexpr rayBase = Point2D{ 2.7, 2.6 };
      auto constexpr rayHeading = Point2D{ std::numbers::sqrt2 * 0.5, std::numbers::sqrt2 * 0.5 };

      auto constexpr boxes = array{
        BoundingBox2D{ { 2.0, 0.0 }, { 3.0, 1.0 } }, // 0, [4] -> [4] (only one)
        BoundingBox2D{ { 3.0, 1.0 }, { 4.0, 2.0 } }, // 1, [5] -> [5] (only one)
        BoundingBox2D{ { 3.0, 2.0 }, { 4.0, 3.0 } }, // 2, [7] -> [28|29]
        BoundingBox2D{ { 2.0, 3.0 }, { 4.0, 4.0 } }, // 3, [1] -> [6|7]
        BoundingBox2D{ { 2.5, 2.5 }, { 3.5, 3.5 } }, // 4, [1] -> [6|7]
      };

      auto const qt = QuadtreeBox(boxes, 3, std::nullopt, 2);
      auto const entities = qt.RayIntersectedAll(rayBase, rayHeading, boxes, 0);
      EXPECT_TRUE((vector<EntityID>{ 4, 2, 3 } == entities));
    }

    TEST(BasicFunctionsTest, RayIntersectedAll_2D_General_34)
    {
      auto constexpr rayBase = Point2D{ 2.6, 2.8 };
      auto constexpr rayHeading = Point2D{ std::numbers::sqrt2 * 0.5, std::numbers::sqrt2 * 0.5 };

      auto constexpr boxes = array{
        BoundingBox2D{ { 2.0, 0.0 }, { 3.0, 1.0 } },
        BoundingBox2D{ { 3.0, 1.0 }, { 4.0, 2.0 } },
        BoundingBox2D{ { 3.0, 2.0 }, { 4.0, 3.0 } },
        BoundingBox2D{ { 2.0, 3.0 }, { 4.0, 4.0 } },
        BoundingBox2D{ { 2.5, 2.5 }, { 3.5, 3.5 } },
      };

      auto const qt = QuadtreeBox(boxes, 3, std::nullopt, 2);
      auto const entities = qt.RayIntersectedAll(rayBase, rayHeading, boxes, 0);
      EXPECT_TRUE((std::ranges::is_permutation(vector<EntityID>{ 3, 4 }, entities)));
    }

    TEST(BasicFunctionsTest, RayIntersectedAll_3D_General_34)
    {
      auto constexpr rayBase = Point3D{ 2.6, 2.8, 1.0 };
      auto constexpr rayHeading = Point3D{ std::numbers::inv_sqrt3, std::numbers::inv_sqrt3, std::numbers::inv_sqrt3 };

      auto constexpr boxes = array{
        BoundingBox3D{ { 2.0, 0.0, 1.0 }, { 3.0, 1.0, 2.0 } },
        BoundingBox3D{ { 3.0, 1.0, 1.0 }, { 4.0, 2.0, 2.0 } },
        BoundingBox3D{ { 3.0, 2.0, 1.0 }, { 4.0, 3.0, 2.0 } },
        BoundingBox3D{ { 2.0, 3.0, 1.5 }, { 4.0, 4.0, 2.0 } },
        BoundingBox3D{ { 2.5, 2.5, 1.5 }, { 3.5, 3.5, 2.0 } },
      };

      auto const qt = OctreeBox(boxes, 3, std::nullopt, 2);
      auto const entities = qt.RayIntersectedAll(rayBase, rayHeading, boxes, 0);
      EXPECT_TRUE((std::ranges::is_permutation(vector<EntityID>{ 3, 4 }, entities)));
    }

    TEST(BasicFunctionsTest, RayIntersectedAll_5D_General_34)
    {
      auto constexpr N = 5;
      auto constexpr rayBase = PointND<N>{ 2.6, 2.8, 1.0, 0.0, 0.0 };
      auto const rayHeading = PointND<N>{ 1.0 / std::sqrt(5.0), 1.0 / std::sqrt(5.0), 1.0 / std::sqrt(5.0), 1.0 / std::sqrt(5.0), 1.0 / std::sqrt(5.0) };

      auto constexpr boxes = array{
        BoundingBoxND<N>{ { 2.0, 0.0, 1.0, 0.0, 0.0 }, { 3.0, 1.0, 2.0, 2.0, 2.0 } },
        BoundingBoxND<N>{ { 3.0, 1.0, 1.0, 0.0, 0.0 }, { 4.0, 2.0, 2.0, 2.0, 2.0 } },
        BoundingBoxND<N>{ { 3.0, 2.0, 1.0, 0.0, 0.0 }, { 4.0, 3.0, 2.0, 2.0, 2.0 } },
        BoundingBoxND<N>{ { 2.0, 3.0, 1.5, 0.0, 0.0 }, { 4.0, 4.0, 2.0, 2.0, 2.0 } },
        BoundingBoxND<N>{ { 2.5, 2.5, 1.5, 0.0, 0.0 }, { 3.5, 3.5, 2.0, 2.0, 2.0 } },
      };

      auto const qt = OrthoTreeBoxND<N>(boxes, 3, std::nullopt, 2);
      auto const entities = qt.RayIntersectedAll(rayBase, rayHeading, boxes, 0);
      EXPECT_TRUE((std::ranges::is_permutation(vector<EntityID>{ 3, 4 }, entities)));
    }

    TEST(BasicFunctionsTest, RayIntersectedAll_ToleranceTest)
    {
      auto const boxes = vector{
        BoundingBox2D{ { 0.0, 0.0 }, { 1.0, 1.0 } },
        BoundingBox2D{ { 1.0, 1.0 }, { 2.0, 2.0 } },
        BoundingBox2D{ { 2.0, 2.0 }, { 3.0, 3.0 } },
        BoundingBox2D{ { 3.0, 3.0 }, { 4.0, 4.0 } },
        BoundingBox2D{ { 1.2, 1.2 }, { 2.8, 2.8 } }
      };

      auto const qt = QuadtreeBox(
        boxes,
        3 // max depth
        ,
        std::nullopt // user-provided bounding Box for all
        ,
        2 // max element in a node
      );

      // Horizontal

      auto const raySearchWithTolerance__0_000 = qt.RayIntersectedAll(
        { 1.1, 2.0 }, // origin
        { 1.0, 0.0 }, // dir
        boxes,
        0.0); // it is on the edge of 1, inside 4, and hit 2

      auto const raySearchWithTolerance__0_001 = qt.RayIntersectedAll(
        { 0.000, 2.001 }, // origin
        { 1.0, 0.0 },     // dir
        boxes,
        0.001 - std::numeric_limits<double>::epsilon()); // ray hits 2,4

      auto const raySearchWithTolerance__0_001_2 = qt.RayIntersectedAll(
        { 2.0005, 2.0 }, // origin
        { 1.0, 0.0 },    // dir
        boxes,
        0.001);

      auto const raySearchWithTolerance__0_899 = qt.RayIntersectedAll(
        { 1.000, 1.9 }, // origin
        { 1.0, 0.0 },   // dir
        boxes,
        0.899);

      auto const raySearchWithTolerance__0_900 = qt.RayIntersectedAll(
        { 1.000, 1.9 }, // origin
        { 1.0, 0.0 },   // dir
        boxes,
        0.9 - std::numeric_limits<double>::epsilon());

      auto const raySearchWithTolerance__0_901 = qt.RayIntersectedAll(
        { 1.000, 1.9 }, // origin
        { 1.0, 0.0 },   // dir
        boxes,
        0.901);

      EXPECT_TRUE((std::ranges::is_permutation(vector<EntityID>{ 1, 4, 2 }, raySearchWithTolerance__0_000)));
      EXPECT_TRUE((std::ranges::is_permutation(vector<EntityID>{ 4, 2 }, raySearchWithTolerance__0_001)));
      EXPECT_TRUE((std::ranges::is_permutation(vector<EntityID>{ 2, 4 }, raySearchWithTolerance__0_001_2)));
      EXPECT_TRUE((std::ranges::is_permutation(vector<EntityID>{ 1, 4, 2 }, raySearchWithTolerance__0_899)));
      EXPECT_TRUE((std::ranges::is_permutation(vector<EntityID>{ 1, 4, 2 }, raySearchWithTolerance__0_900)));
      EXPECT_TRUE((std::ranges::is_permutation(vector<EntityID>{ 0, 1, 4, 2 }, raySearchWithTolerance__0_901)));
    }

    TEST(BasicFunctionsTest, RayIntersectedAll_ToleranceIncrement_AllowsHit)
    {
      auto constexpr rayBase = Point2D{ 0.0, 0.0 };
      auto constexpr rayHeading = Point2D{ 1.0, 0.0 };

      // The ray is epsilon-close to the box but missing without tolerance increment.
      auto constexpr boxes = std::array{
        BoundingBox2D{ Point2D{ 1.0, 0.01 }, Point2D{ 2.0, 0.5 } }  // ID 0
      };

      auto const qt = QuadtreeBox(boxes, 3, std::nullopt, 2);

      // Zero tolerance -> no hit expected
      {
        auto const result = qt.RayIntersectedAll(
          rayBase,
          rayHeading,
          boxes,
          /*tolerance=*/0.0,
          /*toleranceIncrement=*/0.0);
        EXPECT_TRUE((result.empty()));
      }

      // Positive tolerance increment -> hit expected
      {
        auto const result = qt.RayIntersectedAll(
          rayBase,
          rayHeading,
          boxes,
          /*tolerance=*/0.0,
          /*toleranceIncrement=*/0.01);
        EXPECT_EQ(1, result.size());
        EXPECT_EQ(0, result[0]);
      }
    }

    TEST(BasicFunctionsTest, RayIntersectedAll_MaxExaminationDistance_CutsResults)
    {
      auto constexpr rayBase = Point2D{ 0.0, 0.0 };
      auto constexpr rayHeading = Point2D{ 1.0, 0.0 };

      auto constexpr boxes = std::array{
        BoundingBox2D{  Point2D{ 2.0, -1.0 },  Point2D{ 3.0, 1.0 } }, // ID 0, hit distance ~2
        BoundingBox2D{ Point2D{ 10.0, -1.0 }, Point2D{ 11.0, 1.0 } }  // ID 1, hit distance ~10
      };

      auto const qt = QuadtreeBox(boxes, 3, std::nullopt, 2);

      // Distance limit < 10 -> only first hit expected
      {
        auto const result = qt.RayIntersectedAll(
          rayBase,
          rayHeading,
          boxes,
          /*tolerance=*/0.0,
          /*toleranceIncrement=*/0.0,
          /*maxExaminationDistance=*/5.0);
        EXPECT_EQ(1, result.size());
        EXPECT_EQ(0, result[0]);
      }

      // Distance limit large -> both hits expected
      {
        auto const result = qt.RayIntersectedAll(rayBase, rayHeading, boxes, 0.0, 0.0, 100.0);
        EXPECT_EQ(2, result.size());
        EXPECT_TRUE((result[0] == 0 && result[1] == 1));
      }
    }
    TEST(BasicFunctionsTest, RayIntersectedAll_EntityRayHitTester_FiltersIDs)
    {
      auto constexpr rayBase = Point2D{ 0.0, 0.0 };
      auto constexpr rayHeading = Point2D{ 1.0, 0.0 };

      auto constexpr boxes = std::array{
        BoundingBox2D{ { 1.0, -1.0 }, { 2.0, 1.0 } }, // ID 0
        BoundingBox2D{ { 3.0, -1.0 }, { 4.0, 1.0 } }, // ID 1
        BoundingBox2D{ { 5.0, -1.0 }, { 6.0, 1.0 } }, // ID 2
      };

      auto const qt = QuadtreeBox(boxes, 3, std::nullopt, 2);

      auto tester = [&](EntityID id) -> std::optional<double> {
        if (id == 1)
          return 1.1;
        else
          return std::nullopt; // reject other IDs
      };

      auto const result = qt.RayIntersectedAll(
        rayBase,
        rayHeading,
        boxes,
        /*tolerance=*/0.0,
        /*toleranceIncrement=*/0.0,
        /*maxDistance=*/100.0,
        tester);

      EXPECT_EQ(1, result.size());
      EXPECT_EQ(1, result[0]);
    }

    TEST(BasicFunctionsTest, RayIntersectedAll_Split_NoSort)
    {
      auto constexpr rayBase = Point2D{ 0.0, 0.0 };
      auto constexpr rayHeading = Point2D{ 1.0, 0.0 };

      auto constexpr boxes = std::array{
        BoundingBox2D{ { 5.0, -1.0 }, { 6.0, 1.0 } }, // ID 0, far
        BoundingBox2D{ { 2.0, -1.0 }, { 3.0, 1.0 } }, // ID 1, near
        BoundingBox2D{ { 3.5, -1.0 }, { 4.0, 1.0 } }, // ID 2, mid
        BoundingBox2D{  { 5.0, 2.0 }, { 6.0, 3.0 } }, // ID 2
      };

      auto const qt = QuadtreeBoxs<true>(boxes, 3, std::nullopt, 2);

      auto const result = qt.RayIntersectedAll<
        /*SHOULD_SORT_ENTITY_BY_DISTANCE=*/false>(
        rayBase,
        rayHeading,
        boxes,
        /*tolerance=*/0.0,
        /*toleranceIncrement=*/0.0,
        /*maxDistance=*/100.0);

      EXPECT_EQ(3, result.size());
      EXPECT_TRUE((std::ranges::is_permutation(vector<EntityID>{ 0, 1, 2 }, result)));
    }

    TEST(BasicFunctionsTest, RayIntersectedAll_NoSplit_NoSort)
    {
      auto constexpr rayBase = Point2D{ 0.0, 0.0 };
      auto constexpr rayHeading = Point2D{ 1.0, 0.0 };

      auto constexpr boxes = std::array{
        BoundingBox2D{ { 5.0, -1.0 }, { 6.0, 1.0 } }, // ID 0, far
        BoundingBox2D{ { 2.0, -1.0 }, { 3.0, 1.0 } }, // ID 1, near
        BoundingBox2D{ { 3.5, -1.0 }, { 4.0, 1.0 } }, // ID 2, mid
        BoundingBox2D{  { 5.0, 2.0 }, { 6.0, 3.0 } }, // ID 2
      };

      auto const qt = QuadtreeBoxs<false>(boxes, 3, std::nullopt, 2);

      auto const result = qt.RayIntersectedAll<
        /*SHOULD_SORT_ENTITY_BY_DISTANCE=*/false>(
        rayBase,
        rayHeading,
        boxes,
        /*tolerance=*/0.0,
        /*toleranceIncrement=*/0.0,
        /*maxDistance=*/100.0);

      EXPECT_EQ(3, result.size());
      EXPECT_TRUE((std::ranges::is_permutation(vector<EntityID>{ 0, 1, 2 }, result)));
    }
  }
} // namespace GeneralTest
