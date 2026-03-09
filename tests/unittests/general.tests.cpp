#include "pch.h"

#include <charconv>
#include <filesystem>
#include <fstream>
#include <numbers>
#include <random>

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-braces"
#endif

using namespace OrthoTree;
using std::array;
using std::vector;

namespace Microsoft
{
  namespace VisualStudio
  {
    namespace CppUnitTestFramework
    {
      template<>
      inline std::wstring ToString<std::bitset<65>>(const std::bitset<65>& t)
      {
        RETURN_WIDE_STRING(t);
      }

    } // namespace CppUnitTestFramework
  } // namespace VisualStudio
} // namespace Microsoft

namespace
{
  using SI1D = OrthoTree::detail::MortonSpaceIndexing<1, 20>;
  using SI2D = OrthoTree::detail::MortonSpaceIndexing<2, 20>;
  using SI3D = OrthoTree::detail::MortonSpaceIndexing<3, 20>;
  using SI4D = OrthoTree::detail::MortonSpaceIndexing<4, 20>;

  using EntityID = OrthoTree::index_t;

  auto constexpr BB1_INV = BoundingBox1D{ std::numeric_limits<double>::max(), std::numeric_limits<double>::lowest() };

  static bool AreEqualAlmost(double l, double r) noexcept
  {
    return l == r || abs(l - r) < std::numeric_limits<double>::min() * 10; // l and r could be inf.
  }

  template<dim_t nDimension, typename geometry_type = double>
  static bool AreEqualAlmost(BoundingBoxND<nDimension, geometry_type> const& l, BoundingBoxND<nDimension, geometry_type> const& r)
  {
    for (dim_t iD = 0; iD < nDimension; ++iD)
    {
      if (!AreEqualAlmost(l.Min[iD], r.Min[iD]))
        return false;

      if (!AreEqualAlmost(l.Max[iD], r.Max[iD]))
        return false;
    }

    return true;
  }


  template<dim_t N>
  static constexpr auto getPointSetNo1()
  {
    using PointXD = PointND<N>;

    return array{
      // N:1/4
      PointXD{ 0.0, 0.0 }, // N:1/4
      PointXD{ 3.0, 3.0 }, // N:1/4

      // N:1/5
      PointXD{ 5.0, 3.0 }, // N:1/5/22
      PointXD{ 5.2, 3.0 }, // N:1/5/22

      PointXD{ 7.0, 1.0 }, // N:1/5/21

      // N:1/6
      PointXD{ 0.0, 5.0 }, // N:1/6/24
      PointXD{ 1.0, 5.0 }, // N:1/6/24

      PointXD{ 0.0, 7.0 }, // N:1/6/26
      PointXD{ 1.0, 7.0 }, // N:1/6/26

      PointXD{ 3.0, 7.0 }, // N:1/6/26
      PointXD{ 3.0, 7.0 }, // N:1/6/26

      PointXD{ 2.2, 4.5 }, // N:1/6/25/100
      PointXD{ 2.5, 4.5 }, // N:1/6/25/100

      PointXD{ 3.4, 4.5 }, // N:1/6/25/101
      PointXD{ 3.6, 4.6 }, // N:1/6/25/101

      PointXD{ 2.4, 5.5 }, // N:1/6/25/102
      PointXD{ 2.5, 5.5 }, // N:1/6/25/102

      PointXD{ 3.5, 5.5 }, // N:1/6/25/103
      PointXD{ 3.5, 5.5 }, // N:1/6/25/103

      // N:1/7
      PointXD{ 5.0, 5.0 }, // N:1/7/28
      PointXD{ 5.0, 5.0 }, // N:1/7/28

      PointXD{ 7.0, 5.0 }, // N:1/7/30
      PointXD{ 7.0, 5.0 }, // N:1/7/30

      PointXD{ 6.5, 6.5 }, // N:1/7/31/124
      PointXD{ 6.5, 6.5 }, // N:1/7/31/124

      PointXD{ 7.5, 6.5 }, // N:1/7/31/125
      PointXD{ 7.5, 6.5 }, // N:1/7/31/125

      PointXD{ 6.5, 7.5 }, // N:1/7/31/126
      PointXD{ 6.5, 7.5 }, // N:1/7/31/126

      PointXD{ 7.5, 7.5 }, // N:1/7/31/127
      PointXD{ 7.5, 7.5 }, // N:1/7/31/127
      PointXD{ 8.0, 8.0 }, // N:1/7/31/127
    };
  }

} // namespace


namespace GeneralTest
{
  TEST_CLASS(MortonTest)
  {
  public:
    TEST_METHOD(M1D_0_0)
    {
      auto constexpr arr = array<GridID, 1>{ 0 };
      Assert::AreEqual(SI1D::LocationID{ 0 }, SI1D::Encode(arr));
    } // namespace GeneralTest

    TEST_METHOD(M1D_4_4)
    {
      auto constexpr arr = array<GridID, 1>{ 4 };
      Assert::AreEqual(SI1D::LocationID{ 4 }, SI1D::Encode(arr));
    }


    TEST_METHOD(M2D_00_0)
    {
      auto constexpr arr = array<GridID, 2>{ 0, 0 };
      Assert::AreEqual(SI2D::LocationID{ 0 }, SI2D::Encode(arr));
    }

    TEST_METHOD(M2D_20_4)
    {
      auto constexpr arr = array<GridID, 2>{ 2, 0 };
      Assert::AreEqual(SI2D::LocationID{ 4 }, SI2D::Encode(arr));
    }

    TEST_METHOD(M2D_02_8)
    {
      auto constexpr arr = array<GridID, 2>{ 0, 2 };
      Assert::AreEqual(SI2D::LocationID{ 8 }, SI2D::Encode(arr));
    }

    TEST_METHOD(M2D_22_12)
    {
      auto constexpr arr = array<GridID, 2>{ 2, 2 };
      Assert::AreEqual(SI2D::LocationID{ 12 }, SI2D::Encode(arr));
    }

    TEST_METHOD(M2D_13_11)
    {
      auto constexpr arr = array<GridID, 2>{ 1, 3 };
      Assert::AreEqual(SI2D::LocationID{ 11 }, SI2D::Encode(arr));
    }

    TEST_METHOD(M3D_000_0)
    {
      auto constexpr arr = array<GridID, 3>{ 0, 0, 0 };
      Assert::AreEqual(SI3D::LocationID{ 0 }, SI3D::Encode(arr));
    }

    TEST_METHOD(M3D_100_1)
    {
      auto constexpr arr = array<GridID, 3>{ 1, 0, 0 };
      Assert::AreEqual(SI3D::LocationID{ 1 }, SI3D::Encode(arr));
    }

    TEST_METHOD(M3D_001_4)
    {
      auto constexpr arr = array<GridID, 3>{ 0, 0, 1 };
      Assert::AreEqual(SI3D::LocationID{ 4 }, SI3D::Encode(arr));
    }

    TEST_METHOD(M3D_111_7)
    {
      auto constexpr arr = array<GridID, 3>{ 1, 1, 1 };
      Assert::AreEqual(SI3D::LocationID{ 7 }, SI3D::Encode(arr));
    }

    TEST_METHOD(M4D_1111_15)
    {
      auto constexpr arr = array<GridID, 4>{ 1, 1, 1, 1 };
      Assert::AreEqual(SI4D::LocationID{ 15 }, SI4D::Encode(arr));
    }

    TEST_METHOD(M4D_2111_30)
    {
      auto constexpr arr = array<GridID, 4>{ 2, 1, 1, 1 };
      Assert::AreEqual(SI4D::LocationID{ 30 }, SI4D::Encode(arr));
    }
  };

  TEST_CLASS(NodeTest)
  {
  private:
    template<dim_t N>
    static void _complex_ND_Only1()
    {
      using child_id_type_ = typename detail::MortonSpaceIndexing<N, 20>::ChildID;
      using NodeID = typename OrthoTreeBoxND<N>::NodeID;

      auto node = detail::OrthoTreeNodeData<2 << N, NodeID, child_id_type_, typename OrthoTreeBoxND<N>::EntityID, std::monostate>();
      Assert::IsFalse(node.IsAnyChildExist());

      auto constexpr nChild = 1 << N;
      for (child_id_type_ iChild = 0; iChild < nChild; ++iChild)
      {
        auto idChild = nChild - iChild - 1;
        node.LinkChild(idChild, NodeID(idChild));
        Assert::IsTrue(node.HasChild(idChild));
        Assert::IsTrue(node.IsAnyChildExist());

        auto const vChild = node.GetChildren();
        Assert::AreEqual<std::size_t>(1, vChild.size());
        Assert::AreEqual(NodeID(idChild), *vChild.begin());

        node.UnlinkChild(idChild);
        auto const vChild2 = node.GetChildren();
        Assert::AreEqual<std::size_t>(0, vChild2.size());
      }
    }

  public:
    TEST_METHOD(Complex_1D_Only1)
    {
      _complex_ND_Only1<1>();
    }
    TEST_METHOD(Complex_2D_Only1)
    {
      _complex_ND_Only1<2>();
    }
    TEST_METHOD(Complex_3D_Only1)
    {
      _complex_ND_Only1<3>();
    }
    TEST_METHOD(Complex_4D_Only1)
    {
      _complex_ND_Only1<4>();
    }

    TEST_METHOD(Complex_16D_Only1)
    {
      _complex_ND_Only1<16>();
    }
    // TEST_METHOD(Complex_24D_Only1) { Complex_ND_Only1<24>(); }


  private:
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
        Assert::IsTrue(node.HasChild(kChild));
        Assert::IsTrue(node.IsAnyChildExist());

        auto const vChild = node.GetChildren();
        Assert::AreEqual(static_cast<std::size_t>(idChild) + 1, vChild.size());
      }

      for (ChildID idChild = 0; idChild < nChild; ++idChild)
      {
        auto const kChild = NodeID(idChild);

        node.UnlinkChild(kChild);
        auto const vChildActual = node.GetChildren();
        auto vChildExpected = vector<NodeID>(std::size_t(nChild - idChild - 1));
        std::iota(begin(vChildExpected), end(vChildExpected), NodeID(idChild + 1));
        auto const isPerm = std::is_permutation(vChildActual.begin(), vChildActual.end(), vChildExpected.begin());
        Assert::IsTrue(isPerm);
      }

      Assert::IsFalse(node.IsAnyChildExist());
    }

  public:
    TEST_METHOD(Complex_1D_All)
    {
      _complex_All_ND<1>();
    }
    TEST_METHOD(Complex_2D_All)
    {
      _complex_All_ND<2>();
    }
    TEST_METHOD(Complex_3D_All)
    {
      _complex_All_ND<3>();
    }
    TEST_METHOD(Complex_4D_All)
    {
      _complex_All_ND<4>();
    }

    TEST_METHOD(Complex_8D_All)
    {
      _complex_All_ND<8>();
    }
    // TEST_METHOD(Complex_24D_All) { Complex_All_ND<24>(); }
  };


  TEST_CLASS(BasicFunctionsTest)
  {
    using NodeID = DualtreePoint::NodeID;

    TEST_METHOD(Ctor_Point_SameAsCreate__True)
    {
      auto constexpr N = 16;
      auto constexpr vPoint = getPointSetNo1<N>();
      auto const treeExpected = OrthoTreePointND<N>(vPoint, 3);
      auto const treeActual = OrthoTreePointND<N>(vPoint, 3);

      auto const& nodesE = treeExpected.GetNodes();
      auto const& nodesA = treeActual.GetNodes();
      Assert::AreEqual(nodesE.size(), nodesA.size());

      auto const vidE = treeExpected.GetEntitiesBreadthFirst();
      auto const vidA = treeActual.GetEntitiesBreadthFirst();
      Assert::IsTrue(vidE == vidA);
    }

    TEST_METHOD(Ctor_Box_SameAsCreate__True)
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
      Assert::AreEqual(nodesE.size(), nodesA.size());

      auto const vidE = treeExpected.GetEntitiesBreadthFirst();
      auto const vidA = treeActual.GetEntitiesBreadthFirst();
      Assert::IsTrue(vidE == vidA);
    }

    TEST_METHOD(Ctor_Box_Move)
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
      Assert::AreEqual(nodesE.size(), nodesA.size());

      auto const vidE = treeExpected.GetEntitiesBreadthFirst();
      auto const vidA = treeActual.GetEntitiesBreadthFirst();
      Assert::IsTrue(vidE == vidA);
    }

    TEST_METHOD(Ctor_Box_Copy)
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
      Assert::AreEqual(nodesE.size(), nodesA.size());

      auto const vidE = treeExpected.GetEntitiesBreadthFirst();
      auto const vidA = treeActual.GetEntitiesBreadthFirst();
      Assert::IsTrue(vidE == vidA);
    }

    TEST_METHOD(Ctor_Box_CopyAssignment)
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
      Assert::AreEqual(nodesE.size(), nodesA.size());

      auto const vidE = treeExpected.GetEntitiesBreadthFirst();
      auto const vidA = treeActual.GetEntitiesBreadthFirst();
      Assert::IsTrue(vidE == vidA);
    }

    TEST_METHOD(GetNodeID__00_1)
    {
      Assert::AreEqual(DualtreePoint::NodeID{ 1 }, SI1D::GetNodeID(0, 0));
    }

    TEST_METHOD(GetNodeIDh__11_3)
    {
      Assert::AreEqual(DualtreePoint::NodeID{ 3 }, SI1D::GetNodeID(1, 1));
    }
    TEST_METHOD(GetNodeID__22_4)
    {
      Assert::AreEqual(DualtreePoint::NodeID{ 6 }, SI1D::GetNodeID(2, 2));
    }

    TEST_METHOD(GetNodeIDh__37_15)
    {
      Assert::AreEqual(DualtreePoint::NodeID{ 15 }, SI1D::GetNodeID(7, 3));
    }

    TEST_METHOD(GetDepth__37_15__3)
    {
      auto const lc = SI1D::GetDepthID(SI1D::GetNodeID(7, 3));
      Assert::AreEqual(depth_t{ 3 }, lc);
    }

    TEST_METHOD(RemoveSentinelBit__37_15__7)
    {
      auto const lc = SI1D::RemoveSentinelBit(SI1D::GetNodeID(7, 3));
      Assert::AreEqual(NodeID{ 7 }, lc);
    }

    TEST_METHOD(Init)
    {
      auto tree = DualtreePoint{};
      auto const bb = BoundingBox1D{ -1, +1 };
      tree.Init(bb, 3, 10);
      auto const& box = tree.GetBox();

      Assert::IsTrue(AreEqualAlmost(bb, BoundingBox1D{ .Min = box.Min, .Max = box.Max }));

      auto const& nodes = tree.GetNodes();
      Assert::AreEqual<std::size_t>(1, nodes.size());
      Assert::AreEqual<depth_t>(3, tree.GetMaxDepthID());
    }

    TEST_METHOD(InitThenInsert)
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
          Assert::IsTrue(entityIDIfInserted.has_value());
        }

        auto const& nodes = tree.GetCore().GetNodes();
        auto const entitiesInBFS = tree.GetCore().GetEntitiesBreadthFirst();
        auto const entitiesInDFS = tree.GetCore().GetEntitiesDepthFirst();

        Assert::AreEqual<std::size_t>(7, nodes.size());
        Assert::IsTrue(AreContainersItemsEqual(std::vector<EntityID>{ 4, 0, 1, 2, 3 }, entitiesInBFS));
        Assert::IsTrue(AreContainersItemsEqual(std::vector<EntityID>{ 4, 0, 1, 2, 3 }, entitiesInDFS));
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
          Assert::IsTrue(entityIDIfInserted.has_value());
        }
      }


      // Outside of the handled domain
      {
        auto const boxIsNotInTheHandledSpace = BoundingBox1D{ 1, 3 }; // Min point inside, max point outside, it should adjust the root
        auto const entityIDIfInserted = tree.Add(boxIsNotInTheHandledSpace, InsertionMode::Balanced);
        Assert::IsTrue(entityIDIfInserted.has_value());
      }

      auto const& nodes = tree.GetCore().GetNodes();
      auto const entitiesInBFS = tree.GetCore().GetEntitiesBreadthFirst();
      auto const entitiesInDFS = tree.GetCore().GetEntitiesDepthFirst();

      Assert::AreEqual<std::size_t>(7, nodes.size());
      Assert::IsTrue(AreContainersItemsEqual(std::vector<EntityID>{ 4, 8, 9, 6, 0, 5, 1, 2, 7, 3 }, entitiesInBFS));
      Assert::IsTrue(AreContainersItemsEqual(std::vector<EntityID>{ 4, 8, 9, 6, 0, 5, 1, 2, 7, 3 }, entitiesInDFS));

      auto const idsActual = tree.RangeSearch(BoundingBox1D{ -1.1, 0.9 }, RangeSearchMode::Overlap);
      auto const idsExpected = vector<EntityID>{ /* 1. phase */ 0, 1, 2, 4, /* 2. phase */ 6, 7, 8 };
      Assert::IsTrue(std::ranges::is_permutation(idsActual, idsExpected));
    }

    TEST_METHOD(InitThenInsertToLeaf)
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
      Assert::AreEqual<std::size_t>(2, tree.GetNodeCount());
    }
    TEST_METHOD(InsertWithRebalancing_EmptyTree_SingleEntity)
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

      Assert::AreEqual<std::size_t>(1, tree.GetNodeCount());
      Assert::AreEqual<std::size_t>(1, tree.GetNodeEntityCount(tree.GetRootNodeValue()));
    }

    TEST_METHOD(InsertWithRebalancing_ExactCapacity_NoSplit)
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

      Assert::AreEqual<std::size_t>(1, tree.GetNodeCount(), L"Should remain single node at capacity");
      Assert::AreEqual<std::size_t>(3, tree.GetNodeEntityCount(tree.GetRootNodeValue()));
    }

    TEST_METHOD(InsertWithRebalancing_OverCapacity_TriggersSplit)
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

      Assert::IsTrue(tree.GetNodeCount() > 1, L"Should split after exceeding capacity");
    }

    TEST_METHOD(InsertWithRebalancing_LargeEntity_StaysInRoot)
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
      Assert::IsTrue(allEntities == std::vector<EntityID>{ 0 });
    }

    TEST_METHOD(InsertWithRebalancing_MultipleOctants_CorrectDistribution)
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

      Assert::IsTrue(allEntities == std::vector<EntityID>{ 0, 1, 2, 3 });
      Assert::AreEqual<size_t>(5, nodeCount);
    }

    TEST_METHOD(InsertWithRebalancing_MaxDepth_StopsSubdivision)
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

      Assert::IsTrue(maxDepthID <= 2, L"Should not exceed max depth");
    }

    TEST_METHOD(InsertWithRebalancing_TinyEntities_ProperPlacement)
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

      Assert::IsTrue(tree.GetNodeCount() > 1, L"Tiny entities should trigger subdivision");
    }

    TEST_METHOD(InsertWithRebalancing_IdenticalLocations_HandledCorrectly)
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

      Assert::IsTrue(maxDepthID == 5, L"Should add into a deepest level.");
    }

    TEST_METHOD(InsertWithRebalancing_MixedSizes_OptimalDistribution)
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

      Assert::IsTrue(allEntities == std::vector<EntityID>{ 2, 0, 1 });
      Assert::AreEqual<size_t>(3, nodeCount);
    }

    TEST_METHOD(InitThenInsertWithRebalancingLooseOctree)
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

      Assert::AreEqual<std::size_t>(1, tree.GetNodeCount(), L"Tree should have 1 node (Root) before split");
      Assert::AreEqual<std::size_t>(2, tree.GetNodeEntityCount(tree.GetRootNodeValue()), L"Root should have 2 entities");

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

      Assert::IsTrue(idsInBFS == std::vector<EntityID>{ 3, 0, 1, 2 });
      Assert::IsTrue(idsInDFS == std::vector<EntityID>{ 3, 0, 1, 2 });
    }

    TEST_METHOD(EraseFromEmpty)
    {
      auto const handledSpaceDomain = BoundingBox1D{ -2, +2 };

      auto tree = DualtreeBoxM{};
      tree.Reset();
      tree.Init(handledSpaceDomain, 10, 4);
      auto const isErased = tree.Erase(0);
      Assert::IsFalse(isErased);
    }

    TEST_METHOD(VisitNodes__points__0123)
    {
      auto constexpr vpt = array{ Point1D{ 0.0 }, Point1D{ 1.0 }, Point1D{ 2.0 }, Point1D{ 3.0 } };
      auto const tree = DualtreePoint(vpt, 2, std::nullopt, 2);

      auto const ids = tree.GetEntitiesBreadthFirst();
      Assert::IsTrue(ids == vector<EntityID>{ 0, 1, 2, 3 });
    }

    TEST_METHOD(VisitNodes__boxes__0123)
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
      Assert::IsTrue(ids == vector<EntityID>{ 6, 4, 5, 0, 1, 2, 3 });
    }


    TEST_METHOD(EraseId__2__EmptyNode11)
    {
      auto constexpr vpt = array{ Point1D{ 0.0 }, Point1D{ 1.0 }, Point1D{ 2.0 }, Point1D{ 3.0 } };
      auto tree = DualtreePoint(vpt, 2, std::nullopt, 1);

      auto const kNode = SI1D::GetNodeID(2, 2);
      Assert::AreEqual<std::size_t>(tree.GetNodeEntityCount(tree.GetNodeValue(kNode)), 1);
      tree.Erase(2);
      auto const& nodes = tree.GetNodes();
      Assert::IsTrue(nodes.find(kNode) == nodes.end());

      auto const kNode3 = SI1D::GetNodeID(3, 2);
      Assert::AreEqual<EntityID>(*tree.GetNodeEntities(tree.GetNodeValue(kNode3)).begin(), 2);
    }


    TEST_METHOD(UpdateIndexes__None__NothingsChange)
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
      Assert::IsTrue(ids == vector<EntityID>{ 6, 4, 5, 0, 1, 2, 3 });
    }


    TEST_METHOD(UpdateIndexes__2to7__6450173)
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
      Assert::IsTrue(ids == vector<EntityID>{ 6, 4, 5, 0, 1, 7, 3 });
    }

    TEST_METHOD(UpdateIndexes__2toMax__2Removed)
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
      Assert::IsTrue(ids == vector<EntityID>{ 6, 4, 5, 0, 2, 1, 3 });
    }

    TEST_METHOD(UpdateIndexes__Swap3And6)
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
      Assert::IsTrue(ids == vector<EntityID>{ 3, 4, 5, 0, 1, 2, 6 });
    }

    TEST_METHOD(UpdateIndexes__Swap0And1)
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
      Assert::IsTrue(ids == vector<EntityID>{ 6, 4, 5, 1, 0, 2, 3 });
    }


    TEST_METHOD(UpdateIndexes__3to4_4to5_5to6_6Removed)
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

      Assert::IsTrue(ids == vector<EntityID>{ 3, 5, 1, 0, 4, 2, 6 }); // instead of { 6, 4, 5, 0, 1, 2, 3 }
    }

    TEST_METHOD(Clear__EmptyRootRemains)
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
      Assert::AreEqual<std::size_t>(7, nodes.size());

      tree.Clear();
      Assert::AreEqual<std::size_t>(1, nodes.size());
      Assert::IsTrue(tree.IsNodeEntitiesEmpty(tree.GetNodeValue(DualtreeBox::SI::GetNodeID(0, 0))));
    }

    TEST_METHOD(Contains_EmptyTree__False)
    {
      auto const tree = DualtreePoint({}, 3, std::nullopt, 2);
      auto const isPointContained = tree.Contains({}, {}, 1.0);
      Assert::IsFalse(isPointContained);
    }

  private:
    template<dim_t N>
    bool _isOnePointTreeContains()
    {
      using PointXD = PointND<N>;
      auto constexpr vPoint = std::array<PointXD, 1>{ PointXD{ 1.1 } };
      auto const tree = OrthoTreePointND<N>(vPoint, 3);
      return tree.Contains(vPoint[0], vPoint, 0.01);
    }

  public:
    TEST_METHOD(Contains_OnePointTree_1D__True)
    {
      Assert::IsTrue(_isOnePointTreeContains<1>());
    }

    TEST_METHOD(Contains_OnePointTree_2D__True)
    {
      Assert::IsTrue(_isOnePointTreeContains<2>());
    }

    TEST_METHOD(Contains_OnePointTree_3D__True)
    {
      Assert::IsTrue(_isOnePointTreeContains<3>());
    }

    TEST_METHOD(Contains_OnePointTree_4D__True)
    {
      Assert::IsTrue(_isOnePointTreeContains<4>());
    }

    TEST_METHOD(Contains_OnePointTree_16D__True)
    {
      Assert::IsTrue(_isOnePointTreeContains<16>());
    }

  private:
    template<dim_t N>
    bool _isTreeContainsPointSetNo1()
    {
      auto constexpr vPoint = getPointSetNo1<N>();
      auto const tree = OrthoTreePointND<N>(vPoint, 3);
      return tree.Contains(vPoint[4], vPoint, 0.0);
    }

  public:
    TEST_METHOD(Contains_PointSetNo1_2D__True)
    {
      Assert::IsTrue(_isTreeContainsPointSetNo1<2>());
    }

    TEST_METHOD(Contains_PointSetNo1_3D__True)
    {
      Assert::IsTrue(_isTreeContainsPointSetNo1<3>());
    }

    TEST_METHOD(Contains_PointSetNo1_16D__True)
    {
      Assert::IsTrue(_isTreeContainsPointSetNo1<16>());
    }

    TEST_METHOD(Contains_PointSetNo1_16D__False)
    {
      auto constexpr N = 16;
      auto constexpr vPoint = getPointSetNo1<N>();
      auto const tree = OrthoTreePointND<N>(vPoint, 3);
      auto const fContain = tree.Contains(PointND<N>{ -1.0, -1.0 }, vPoint, 0.0);

      Assert::IsFalse(fContain);
    }

    TEST_METHOD(Contains_PointSetNo1_3D__False)
    {
      auto constexpr N = 3;
      auto constexpr vPoint = getPointSetNo1<N>();
      auto const tree = OrthoTreePointND<N>(vPoint, 3);
      auto const fContain = tree.Contains(PointND<N>{ 7.0, 9.0 }, vPoint, 0.0);

      Assert::IsFalse(fContain);
    }


    TEST_METHOD(Move__Empty__NotCrash)
    {
      auto tree = DualtreePoint();
      tree.Move({});
    }


  private:
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


  public:
    TEST_METHOD(MoveP__P0__Same)
    {
      auto constexpr vpt = array{ Point1D{ 0.0 }, Point1D{ 1.0 }, Point1D{ 2.0 }, Point1D{ 3.0 } };
      auto const treePre = DualtreePoint(vpt, 3, std::nullopt, 2);

      auto treeAfter = treePre;
      auto constexpr vMove = Point1D{ 0.0 };
      treeAfter.Move(vMove);

      Assert::IsTrue(_isMoveOfTwoTreeProper(treePre, treeAfter, vMove));
    }


    TEST_METHOD(MoveP__P1__Moved)
    {
      auto constexpr vpt = array{ Point1D{ 0.0 }, Point1D{ 1.0 }, Point1D{ 2.0 }, Point1D{ 3.0 } };
      auto const treePre = DualtreePoint(vpt, 3, std::nullopt, 2);

      auto treeAfter = treePre;
      auto constexpr vMove = Point1D{ 1.0 };
      treeAfter.Move(vMove);

      Assert::IsTrue(_isMoveOfTwoTreeProper(treePre, treeAfter, vMove));
    }


    TEST_METHOD(MoveB__M20__Moved)
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

      Assert::IsTrue(_isMoveOfTwoTreeProper(treePre, treeAfter, vMove));
    }


    TEST_METHOD(RayIntersectedFirst__XRay__None)
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
      Assert::IsTrue(oid.empty());
    }


    TEST_METHOD(RayIntersectedFirst__YRay__None)
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
      Assert::IsTrue(oid.empty());
    }


    TEST_METHOD(RayIntersectedFirst__YRay2__None)
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
      Assert::IsTrue(oid.empty());
    }


    TEST_METHOD(RayIntersectedFirst_InsideTheTree_Neg_None)
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
      Assert::IsTrue(oid.empty());
    }


    TEST_METHOD(RayIntersectedFirst_OutsideTheTree_Neg_None)
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
      Assert::IsTrue(oid.empty());
    }


    TEST_METHOD(RayIntersectedFirst__XRay__0)
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
      Assert::AreEqual<size_t>(1, oid.size());
      Assert::AreEqual<EntityID>(0, oid[0]);
    }


    TEST_METHOD(RayIntersectedFirst__XRay__1)
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
      Assert::AreEqual<size_t>(2, oid.size());
      Assert::AreEqual<EntityID>(2, oid[0]);
      Assert::AreEqual<EntityID>(1, oid[1]);
    }


    TEST_METHOD(RayIntersectedFirst__XRayInsideTheTreePos__1)
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
      Assert::AreEqual<size_t>(1, oid.size());
      Assert::AreEqual<EntityID>(1, oid[0]);
    }


    TEST_METHOD(RayIntersectedFirst__XRayInsideTheTreeNeg__1)
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
      Assert::AreEqual<size_t>(2, oid.size());
      Assert::AreEqual<EntityID>(0, oid[0]);
      Assert::AreEqual<EntityID>(1, oid[1]);
    }


    TEST_METHOD(RayIntersectedFirst__XRayInsideTheTreeNeg__3)
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
      Assert::AreEqual<size_t>(1, oid.size());
      Assert::AreEqual<EntityID>(3, oid[0]);
    }

    TEST_METHOD(RayIntersectedFirst_MultipleHits_ReturnsReverseSorted)
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

      Assert::AreEqual<size_t>(3, result.size());
      Assert::AreEqual((size_t)2, (size_t)result[0]);
      Assert::AreEqual((size_t)1, (size_t)result[1]);
      Assert::AreEqual((size_t)0, (size_t)result[2]);
    }

    TEST_METHOD(RayIntersectedFirst_Tolerance_AllowsGrazingHit)
    {
      auto constexpr rayBase = Point2D{ 0.0, 1.001 };
      auto constexpr rayHeading = Point2D{ 1.0, 0.0 };

      auto constexpr boxes = std::array{
        BoundingBox2D{ { 1.0, 0.0 }, { 2.0, 1.0 } }
      };

      auto const qt = QuadtreeBox(boxes, 1, std::nullopt, 1);

      auto const result = qt.RayIntersectedFirst(rayBase, rayHeading, boxes, 0.01);

      Assert::AreEqual<size_t>(1, result.size());
      Assert::AreEqual((size_t)0, (size_t)result[0]);
    }

    TEST_METHOD(RayIntersectedFirst_ToleranceIncrement_PrunesLaterHits)
    {
      auto constexpr rayBase = Point2D{ -1.0, 0.5 };
      auto constexpr rayHeading = Point2D{ 1.0, 0.0 };

      auto constexpr boxes = std::array{
        BoundingBox2D{  { 0.0, 0.0 },  { 1.0, 1.0 } }, // hitDistance ~1
        BoundingBox2D{ { 10.0, 0.0 }, { 11.0, 1.0 } }, // hitDistance ~11 -> prune
      };

      auto const qt = QuadtreeBox(boxes, 3, std::nullopt, 2);

      auto const result = qt.RayIntersectedFirst(rayBase, rayHeading, boxes, 0.0, 0.5);

      Assert::AreEqual<size_t>(1, result.size());
      Assert::AreEqual((size_t)0, (size_t)result[0]);
    }

    TEST_METHOD(RayIntersectedFirst_MaxDistance_CutsOff)
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

      Assert::AreEqual<size_t>(2, result.size());
      Assert::AreEqual((size_t)1, (size_t)result[0]);
      Assert::AreEqual((size_t)0, (size_t)result[1]);
    }

    TEST_METHOD(RayIntersectedFirst_UserHitTest_FiltersEntity)
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

      Assert::AreEqual<size_t>(1, result.size());
      Assert::AreEqual((size_t)1, (size_t)result[0]);
    }

    TEST_METHOD(RayIntersectedAll_2D_General_234)
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
      Assert::IsTrue(vector<EntityID>{ 4, 2, 3 } == entities);
    }

    TEST_METHOD(RayIntersectedAll_2D_General_34)
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
      Assert::IsTrue(std::ranges::is_permutation(vector<EntityID>{ 3, 4 }, entities));
    }

    TEST_METHOD(RayIntersectedAll_3D_General_34)
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
      Assert::IsTrue(std::ranges::is_permutation(vector<EntityID>{ 3, 4 }, entities));
    }

    TEST_METHOD(RayIntersectedAll_5D_General_34)
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
      Assert::IsTrue(std::ranges::is_permutation(vector<EntityID>{ 3, 4 }, entities));
    }

    TEST_METHOD(RayIntersectedAll_ToleranceTest)
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

      Assert::IsTrue(std::ranges::is_permutation(vector<EntityID>{ 1, 4, 2 }, raySearchWithTolerance__0_000));
      Assert::IsTrue(std::ranges::is_permutation(vector<EntityID>{ 4, 2 }, raySearchWithTolerance__0_001));
      Assert::IsTrue(std::ranges::is_permutation(vector<EntityID>{ 2, 4 }, raySearchWithTolerance__0_001_2));
      Assert::IsTrue(std::ranges::is_permutation(vector<EntityID>{ 1, 4, 2 }, raySearchWithTolerance__0_899));
      Assert::IsTrue(std::ranges::is_permutation(vector<EntityID>{ 1, 4, 2 }, raySearchWithTolerance__0_900));
      Assert::IsTrue(std::ranges::is_permutation(vector<EntityID>{ 0, 1, 4, 2 }, raySearchWithTolerance__0_901));
    }

    TEST_METHOD(RayIntersectedAll_ToleranceIncrement_AllowsHit)
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
        Assert::IsTrue(result.empty());
      }

      // Positive tolerance increment -> hit expected
      {
        auto const result = qt.RayIntersectedAll(
          rayBase,
          rayHeading,
          boxes,
          /*tolerance=*/0.0,
          /*toleranceIncrement=*/0.01);
        Assert::AreEqual<size_t>(1, result.size());
        Assert::AreEqual<EntityID>(0, result[0]);
      }
    }

    TEST_METHOD(RayIntersectedAll_MaxExaminationDistance_CutsResults)
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
        Assert::AreEqual<size_t>(1, result.size());
        Assert::AreEqual<EntityID>(0, result[0]);
      }

      // Distance limit large -> both hits expected
      {
        auto const result = qt.RayIntersectedAll(rayBase, rayHeading, boxes, 0.0, 0.0, 100.0);
        Assert::AreEqual<size_t>(2, result.size());
        Assert::IsTrue(result[0] == 0 && result[1] == 1);
      }
    }
    TEST_METHOD(RayIntersectedAll_EntityRayHitTester_FiltersIDs)
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

      Assert::AreEqual<size_t>(1, result.size());
      Assert::AreEqual<EntityID>(1, result[0]);
    }

    TEST_METHOD(RayIntersectedAll_Split_NoSort)
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

      Assert::AreEqual<size_t>(3, result.size());
      Assert::IsTrue(std::ranges::is_permutation(vector<EntityID>{ 0, 1, 2 }, result));
    }

    TEST_METHOD(RayIntersectedAll_NoSplit_NoSort)
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

      Assert::AreEqual<size_t>(3, result.size());
      Assert::IsTrue(std::ranges::is_permutation(vector<EntityID>{ 0, 1, 2 }, result));
    }
  };
} // namespace GeneralTest

namespace Tree1DTest
{
  TEST_CLASS(Point_CreateTest)
  {
  public:
    TEST_METHOD(Empty)
    {
      auto const tree = DualtreePoint({}, 2);
      Assert::IsTrue(tree.GetNodeCount() == 1);
      Assert::IsTrue(tree.IsNodeEntitiesEmpty(tree.GetNodeValue(1)));

      auto const& box = tree.GetBox();
      Assert::IsTrue(AreEqualAlmost(BoundingBox1D{ .Min = box.Min, .Max = box.Max }, {}));
    } // namespace Tree1DTest

    // ext    //!
    TEST_METHOD(NoPt1)
    {
      auto const tree = DualtreePoint(vector<Point1D>{ { 1.0 } }, 2);
      Assert::IsTrue(tree.GetNodeCount() == 1);
      Assert::IsFalse(tree.IsNodeEntitiesEmpty(tree.GetNodeValue(1)));

      auto const& box = tree.GetBox();
      Assert::IsTrue(AreEqualAlmost(BoundingBox1D{ .Min = box.Min, .Max = box.Max }, BoundingBox1D{ Point1D{ 1.0 }, Point1D{ 1.0 } }));
    }

    // ext     //!
    TEST_METHOD(NoPt2)
    {
      auto const tree = DualtreePoint({}, 2);
      Assert::IsTrue(tree.GetNodeCount() == 1);
      Assert::IsTrue(tree.IsNodeEntitiesEmpty(tree.GetNodeValue(1)));
    }

    TEST_METHOD(NoPt4)
    {
      auto constexpr vpt = array{ Point1D{ 0.0 }, Point1D{ 1.0 }, Point1D{ 2.0 }, Point1D{ 3.0 } };
      auto const tree = DualtreePoint(vpt, 2, std::nullopt, 1);

      Assert::AreEqual<std::size_t>(tree.GetNodeCount(), 7);

      auto const& box = tree.GetBox();
      Assert::IsTrue(AreEqualAlmost(BoundingBox1D{ .Min = box.Min, .Max = box.Max }, BoundingBox1D{ Point1D{ 0.0 }, Point1D{ 3.0 } }));
    }

    //!
    TEST_METHOD(Init)
    {
      auto tree = DualtreePoint{};
      tree.Init(BoundingBox1D{ { -1.0 }, { +1.0 } }, 2);

      Assert::IsTrue(tree.GetNodeCount() == 1);
      Assert::IsTrue(tree.IsNodeEntitiesEmpty(tree.GetRootNodeValue()));
    }
  };


  TEST_CLASS(Point_SearchTest)
  {
    TEST_METHOD(RangeSearch__EqualAtBorder__12)
    {
      auto constexpr vpt = array{ Point1D{ 0.0 }, Point1D{ 1.0 }, Point1D{ 2.0 }, Point1D{ 3.0 } };
      auto const tree = DualtreePoint(vpt, 2, std::nullopt, 1);
      auto const ids = tree.RangeSearch({ 1.0, 2.0 }, vpt);

      Assert::IsTrue(std::ranges::is_permutation(vector<EntityID>{ 1, 2 }, ids));
    }

    TEST_METHOD(RangeSearch__Usual__23)
    {
      auto constexpr vpt = array{ Point1D{ 0.0 }, Point1D{ 1.0 }, Point1D{ 2.0 }, Point1D{ 3.0 } };
      auto const tree = DualtreePoint(vpt, 2, std::nullopt, 1);
      auto const ids = tree.RangeSearch({ 1.1, 3.1 }, vpt);

      Assert::IsTrue(std::ranges::is_permutation(vector<EntityID>{ 2, 3 }, ids));
    }

    TEST_METHOD(RangeSearch__AtNodeBorder__2)
    {
      auto constexpr vpt = array{ Point1D{ 0.0 }, Point1D{ 1.0 }, Point1D{ 2.0 }, Point1D{ 3.0 } };
      auto const tree = DualtreePoint(vpt, 2, std::nullopt, 1);
      auto const ids = tree.RangeSearch({ 1.5, 2.25 }, vpt);

      Assert::IsTrue(std::ranges::is_permutation(vector<EntityID>{ 2 }, ids));
    }

    TEST_METHOD(RangeSearch__ZeroBox__Empty)
    {
      auto constexpr vpt = array{ Point1D{ 0.0 }, Point1D{ 1.0 }, Point1D{ 2.0 }, Point1D{ 3.0 } };
      auto const tree = DualtreePoint(vpt, 2, std::nullopt, 1);
      auto const ids = tree.RangeSearch({}, vpt);

      Assert::IsTrue(ids.empty());
    }

    TEST_METHOD(RangeSearch__Empty)
    {
      auto constexpr vpt = array{ Point1D{ 0.0 }, Point1D{ 1.0 }, Point1D{ 2.0 }, Point1D{ 3.0 } };
      auto const tree = DualtreePoint(vpt, 2, std::nullopt, 1);
      auto const ids = tree.RangeSearch({ 1.5, 1.75 }, vpt);

      Assert::IsTrue(ids.empty());
    }


    TEST_METHOD(Query)
    {
      auto constexpr vpt = array{
        Point2D{ 0.0, 0.0 },
        Point2D{ 8.0, 8.0 },
        Point2D{ 2.0, 1.0 },
        Point2D{ 3.5, 3.0 }, // within box, outside of others
        Point2D{ 4.5, 3.0 }, // outside of the frustum
        Point2D{ 5.5, 4.0 }, // easy hit
        Point2D{ 6.5, 5.5 }, // within box and frustum, but fails plane intersection
        Point2D{ 4.5, 5.5 },
        Point2D{ 4.0, 4.0 }  // edge of everything
      };
      auto const tree = QuadtreePoint(vpt, 3, std::nullopt, 1);

      auto const conditions = std::array<QuadtreePoint::QueryCondition, 4>{
        QuadtreePoint::FrustumCondition{
                                        .boundaryPlanes = { { Plane2D{ .OrigoDistance = std::numbers::sqrt2 * 4.0, .Normal = { std::numbers::sqrt2 * 0.5, std::numbers::sqrt2 * 0.5 } },
                                true } } },
        QuadtreePoint::RangeCondition{ .range{ .Min = { 3.0, 2.0 }, .Max = { 7.0, 6.0 } }, .rangeSearchMode = RangeSearchMode::Inside },
        QuadtreePoint::PlaneIntersectionCondition{ .plane = Plane2D{ .OrigoDistance = 5.0, .Normal = { 1.0, 0.0 } }, .tolerance = 1.0 },
        QuadtreePoint::EntityIDCondition([](QuadtreePoint::EntityID id) { return id > 5; }
        )
      };

      auto const idsAnd = tree.Query(conditions, vpt);
      Assert::IsTrue(std::ranges::is_permutation(vector<EntityID>{ 7, 8 }, idsAnd));

      auto const idsOr = tree.Query<LogicalOperator::Or>(conditions, vpt);
      Assert::IsTrue(std::ranges::is_permutation(vector<EntityID>{ 1, 3, 4, 5, 6, 7, 8 }, idsOr));

      auto const treeC = QuadtreePointM(vpt, 3, std::nullopt, 1);
      auto const idsAndC = treeC.Query(conditions);
      Assert::IsTrue(std::ranges::is_permutation(vector<EntityID>{ 7, 8 }, idsAndC));
    }
  };

  struct UserDefinedConfig : Configuration<>
  {
    static constexpr bool ALLOW_OUT_OF_SPACE_INSERTION = false;
    static constexpr std::size_t DEFAULT_TARGET_ELEMENT_NUM_IN_NODES = 10;
  };
  using DualtreePointStrictBound = OrthoTreeBase<PointEntitySpanAdapter<Point1D>, GeneralGeometryAdapterND<1>, UserDefinedConfig>;

  TEST_CLASS(Point_EditTest)
  {
    TEST_METHOD(Insert__NonLeaf__Successful)
    {
      auto constexpr vpt = array{ Point1D{ 0.0 }, Point1D{ 1.0 }, Point1D{ 2.0 }, Point1D{ 3.0 } };
      auto tree = DualtreePoint(vpt, 3, std::nullopt, 1);

      Assert::IsTrue(tree.InsertIntoLeaf(4, Point1D{ 2.5 }, InsertionMode::ExistingLeaf));

      Assert::AreEqual<std::size_t>(7, tree.GetNodeCount());
      Assert::IsTrue(AreContainersItemsEqual(tree.GetNodeEntities(tree.GetNodeValue(7)), vector<EntityID>{ 3, 4 }));
    }

    TEST_METHOD(Insert__Leaf__Successful)
    {
      auto constexpr vpt = array{ Point1D{ 0.0 }, Point1D{ 1.0 }, Point1D{ 2.0 }, Point1D{ 3.0 } };
      auto tree = DualtreePoint(vpt, 3, std::nullopt, 1);

      Assert::IsTrue(tree.InsertIntoLeaf(4, Point1D{ 2.5 }, InsertionMode::LowestLeaf));

      auto const& nodes = tree.GetNodes();
      Assert::AreEqual<std::size_t>(nodes.size(), 8);
      Assert::IsTrue(AreContainersItemsEqual(tree.GetNodeEntities(tree.GetNodeValue(14)), vector<EntityID>{ 4 }));
    }

    TEST_METHOD(Insert__OutOfSpace__ReturnsFalse)
    {
      auto constexpr vpt = array{ Point1D{ 0.0 }, Point1D{ 1.0 }, Point1D{ 2.0 }, Point1D{ 3.0 } };
      auto tree = DualtreePointStrictBound(vpt, 3, std::nullopt, 1);
      Assert::IsFalse(tree.InsertIntoLeaf(4, Point1D{ 4.0 }, InsertionMode::LowestLeaf));

      auto const& nodes = tree.GetNodes();
      Assert::AreEqual<std::size_t>(nodes.size(), 7);
      Assert::IsTrue(AreContainersItemsEqual(tree.GetNodeEntities(tree.GetNodeValue(7)), vector<EntityID>{ 3 }));
    }


    TEST_METHOD(Update3p_)
    {
      //!
    }

    TEST_METHOD(Update2p__Update3__Successful)
    {
      auto vpt = vector{ Point1D{ 0.0 }, Point1D{ 1.0 }, Point1D{ 2.0 }, Point1D{ 3.0 } };

      auto tree = DualtreePoint(vpt, 3, std::nullopt, 1);

      Assert::IsTrue(tree.Update(3, { 1.1 }));
      auto const ids = tree.GetEntitiesBreadthFirst();
      Assert::IsTrue(ids == vector<EntityID>{ 0, 1, 3, 2 });
    }

    TEST_METHOD(Update2p__Update2__Successful)
    {
      auto vpt = vector{ Point1D{ 0.0 }, Point1D{ 1.0 }, Point1D{ 2.0 }, Point1D{ 3.0 } };

      auto tree = DualtreePoint(vpt, 3, std::nullopt, 1);

      Assert::IsTrue(tree.Update(2, { 1.1 }));
      auto const ids = tree.GetEntitiesBreadthFirst();
      Assert::IsTrue(ids == vector<EntityID>{ 0, 1, 2, 3 });
    }

    TEST_METHOD(Update2p__Rewrite3OutOfBox__ReturnsFalse)
    {
      auto vpt = vector{ Point1D{ 0.0 }, Point1D{ 1.0 }, Point1D{ 2.0 }, Point1D{ 3.0 } };

      auto tree = DualtreePointStrictBound(vpt, 3, std::nullopt, 1);

      auto const idsPre = tree.GetEntitiesBreadthFirst();
      Assert::IsFalse(tree.Update(2, { 3.1 }));
      auto const idsPost = tree.GetEntitiesBreadthFirst();

      Assert::IsTrue(idsPre == idsPost);
    }


    TEST_METHOD(Erase__3__Removed)
    {
      auto constexpr vpt = array{ Point1D{ 0.0 }, Point1D{ 1.0 }, Point1D{ 2.0 }, Point1D{ 3.0 } };
      auto tree = DualtreePoint(vpt, 3, std::nullopt, 1);

      Assert::IsTrue(tree.Erase(3, vpt.back()));
      auto const ids = tree.GetEntitiesBreadthFirst();
      Assert::IsTrue(std::ranges::find(ids, EntityID(3)) == end(ids));
    }

    TEST_METHOD(Erase__Nonexist__ReturnsFalse)
    {
      auto constexpr vpt = array{ Point1D{ 0.0 }, Point1D{ 1.0 }, Point1D{ 2.0 }, Point1D{ 3.0 } };
      auto tree = DualtreePoint(vpt, 3, std::nullopt, 1);

      auto const idsPre = tree.GetEntitiesBreadthFirst();
      Assert::IsFalse(tree.Erase(4, vpt.back()));
      auto const idsPost = tree.GetEntitiesBreadthFirst();

      Assert::IsTrue(idsPre == idsPost);
    }

    TEST_METHOD(Erase__WrongPointWasGiven__ReturnsFalse)
    {
      auto constexpr vpt = array{ Point1D{ 0.0 }, Point1D{ 1.0 }, Point1D{ 2.0 }, Point1D{ 3.0 } };
      auto tree = DualtreePoint(vpt, 3, std::nullopt, 1);

      auto const idsPre = tree.GetEntitiesBreadthFirst();
      Assert::IsFalse(tree.Erase(3, vpt.front()));
      auto const idsPost = tree.GetEntitiesBreadthFirst();

      Assert::IsTrue(idsPre == idsPost);
    }
  };


  TEST_CLASS(Box_SearchTest)
  {
    TEST_METHOD(FindSmallestNode_)
    {
      //!
    }

    TEST_METHOD(PickSearch_)
    {
      //!
    }


    TEST_METHOD(RangeSearch_)
    {
      //!
    }

    TEST_METHOD(LooseStrategy_Seq)
    {
      auto const boxes = array{
        BoundingBox2D{ { 0.0, 0.0 }, { 1.0, 1.0 } },
        BoundingBox2D{ { 1.0, 1.0 }, { 2.0, 2.0 } },
        BoundingBox2D{ { 2.0, 2.0 }, { 3.0, 3.0 } },
        BoundingBox2D{ { 3.0, 3.0 }, { 4.0, 4.0 } },
        BoundingBox2D{ { 1.2, 1.2 }, { 2.8, 2.8 } },
        BoundingBox2D{ { 1.2, 1.2 }, { 2.8, 1.8 } }, // 5; 19
        BoundingBox2D{ { 0.5, 0.0 }, { 1.5, 0.5 } }  // 16; 17
      };

      auto quadtreebox = QuadtreeBox(
        boxes,
        2,
        std::nullopt // user-provided bounding Box for all
        ,
        1 // max element in a node
      );


      auto const& nodes = quadtreebox.GetNodes();
      auto const entitiesInBFS = quadtreebox.GetEntitiesBreadthFirst();
      auto const entitiesInDFS = quadtreebox.GetEntitiesDepthFirst();

      Assert::AreEqual<std::size_t>(nodes.size(), 9);
      Assert::IsTrue(AreContainersItemsEqual(entitiesInBFS, vector<EntityID>{ 5, 4, 0, 6, 1, 2, 3 }));
      Assert::IsTrue(AreContainersItemsEqual(entitiesInDFS, vector<EntityID>{ 0, 6, 1, 5, 4, 2, 3 }));
    }


    TEST_METHOD(LooseStrategy_Par)
    {
      auto const boxes = array{
        BoundingBox2D{ { 0.0, 0.0 }, { 1.0, 1.0 } },
        BoundingBox2D{ { 1.0, 1.0 }, { 2.0, 2.0 } },
        BoundingBox2D{ { 2.0, 2.0 }, { 3.0, 3.0 } },
        BoundingBox2D{ { 3.0, 3.0 }, { 4.0, 4.0 } },
        BoundingBox2D{ { 1.2, 1.2 }, { 2.8, 2.8 } },
        BoundingBox2D{ { 1.2, 1.2 }, { 2.8, 1.8 } }, // 5; 19
        BoundingBox2D{ { 0.5, 0.0 }, { 1.5, 0.5 } }  // 16; 17
      };

      auto quadtreebox = QuadtreeBox{};
      QuadtreeBox::Create<ExecutionTags::Parallel>(
        quadtreebox,
        boxes,
        2,
        std::nullopt // user-provided bounding Box for all
        ,
        1 // max element in a node
      );


      auto const& nodes = quadtreebox.GetNodes();
      auto const entitiesInBFS = quadtreebox.GetEntitiesBreadthFirst();
      auto const entitiesInDFS = quadtreebox.GetEntitiesDepthFirst();

      Assert::AreEqual<std::size_t>(nodes.size(), 9);
      Assert::IsTrue(AreContainersItemsEqual(entitiesInBFS, vector<EntityID>{ 5, 4, 0, 6, 1, 2, 3 }));
      Assert::IsTrue(AreContainersItemsEqual(entitiesInDFS, vector<EntityID>{ 0, 6, 1, 5, 4, 2, 3 }));
    }


    TEST_METHOD(CollistionDetection__0040_3565__P30)
    {
      auto constexpr vBoxL = array{
        BoundingBox1D{ 0.0, 1.0 },
        BoundingBox1D{ 1.0, 2.0 },
        BoundingBox1D{ 2.0, 3.0 },
        BoundingBox1D{ 3.0, 4.0 }
      };
      auto const treeL = DualtreeBox(vBoxL, 3, std::nullopt, 1);

      auto constexpr vBoxR = array{
        BoundingBox1D{ 3.5, 4.5 },
        BoundingBox1D{ 4.5, 5.5 },
        BoundingBox1D{ 5.5, 6.5 }
      };
      auto const treeR = DualtreeBox(vBoxR, 3, std::nullopt, 1);

      auto const ret = DualtreeBox::CollisionDetection(treeL, vBoxL, treeR, vBoxR);

      auto constexpr aExpected = std::pair{ 3, 0 };
      Assert::AreEqual<std::size_t>(ret.size(), 1);
      Assert::AreEqual<EntityID>(ret[0].first, aExpected.first);
      Assert::AreEqual<EntityID>(ret[0].second, aExpected.second);
    }


    TEST_METHOD(CollistionDetection__Complex1)
    {
      auto constexpr vBoxL = array{
        BoundingBox1D{ 0.0, 4.0 },
        BoundingBox1D{ 0.0, 2.0 },
        BoundingBox1D{ 2.0, 4.0 },
        BoundingBox1D{ 0.0, 1.0 },
        BoundingBox1D{ 1.0, 2.0 },
        BoundingBox1D{ 2.0, 3.0 },
        BoundingBox1D{ 3.0, 4.0 }
      };
      auto const treeL = DualtreeBox(vBoxL, 3, std::nullopt, 1);

      auto constexpr vBoxR = array{
        BoundingBox1D{ 2.0, 6.0 },
        BoundingBox1D{ 2.0, 4.0 },
        BoundingBox1D{ 4.0, 6.0 },
        BoundingBox1D{ 2.0, 3.0 },
        BoundingBox1D{ 3.0, 4.0 },
        BoundingBox1D{ 4.0, 5.0 },
        BoundingBox1D{ 5.0, 6.0 }
      };
      auto const treeR = DualtreeBox(vBoxR, 3, std::nullopt, 1);

      auto const vActual = DualtreeBox::CollisionDetection(treeL, vBoxL, treeR, vBoxR);

      using EntityPair = std::pair<EntityID, EntityID>;
      auto constexpr aExpected = std::array{
        EntityPair{ 0, 0 }, // Level 0
        EntityPair{ 0, 1 },
        EntityPair{ 2, 0 }, // Level 1
        EntityPair{ 2, 1 },
        EntityPair{ 0, 3 }, // Level 2
        EntityPair{ 2, 3 },
        EntityPair{ 5, 0 },
        EntityPair{ 5, 1 },
        EntityPair{ 5, 3 }, // ==
        EntityPair{ 0, 4 },
        EntityPair{ 2, 4 },
        EntityPair{ 6, 0 },
        EntityPair{ 6, 1 },
        EntityPair{ 6, 4 }, // ==
      };
      Assert::AreEqual(aExpected.size(), vActual.size());
      Assert::IsTrue(
        std::ranges::is_permutation(aExpected, vActual, [](auto const& p1, auto const& p2) { return p1.first == p2.first && p1.second == p2.second; }));
    }
  };

  TEST_CLASS(Box_EditTest)
  {
    TEST_METHOD(Insert_IntoRoot_Successful)
    {
      auto constexpr vBoxL = array{
        BoundingBox1D{ 0.0, 1.0 },
        BoundingBox1D{ 1.0, 2.0 },
        BoundingBox1D{ 2.0, 3.0 },
        BoundingBox1D{ 3.0, 4.0 }
      };
      auto tree = DualtreeBox(vBoxL, 3, std::nullopt, 1);

      Assert::IsTrue(tree.InsertIntoLeaf(4, BoundingBox1D{ 0, 4 }, InsertionMode::ExistingLeaf));

      auto const& nodes = tree.GetNodes();
      Assert::AreEqual<std::size_t>(nodes.size(), 7);
      Assert::IsTrue(AreContainersItemsEqual(tree.GetNodeEntities(tree.GetRootNodeValue()), vector<EntityID>{ 4 }));
    }


    TEST_METHOD(Insert_NonLeaf_Successful)
    {
      auto constexpr vBoxL = array{
        BoundingBox1D{ 0.0, 1.0 },
        BoundingBox1D{ 1.0, 2.0 },
        BoundingBox1D{ 2.0, 3.0 },
        BoundingBox1D{ 3.0, 4.0 }
      };
      auto tree = DualtreeBox(vBoxL, 3, std::nullopt, 1);

      Assert::IsTrue(tree.InsertIntoLeaf(4, BoundingBox1D{ 3.5, 3.7 }, InsertionMode::ExistingLeaf));

      auto const& nodes = tree.GetNodes();
      Assert::AreEqual<std::size_t>(nodes.size(), 7);
      Assert::IsTrue(AreContainersItemsEqual(tree.GetNodeEntities(tree.GetNodeValue(7)), vector<EntityID>{ 3, 4 }));
    }

    TEST_METHOD(Insert_Leaf_Successful)
    {
      auto constexpr vBoxL = array{
        BoundingBox1D{ 0.0, 1.0 },
        BoundingBox1D{ 1.0, 2.0 },
        BoundingBox1D{ 2.0, 3.0 },
        BoundingBox1D{ 3.0, 4.0 }
      };
      auto tree = DualtreeBox(vBoxL, 3, std::nullopt, 1);

      Assert::IsTrue(tree.InsertIntoLeaf(4, BoundingBox1D{ 3.5, 3.7 }, InsertionMode::LowestLeaf));

      auto const& nodes = tree.GetNodes();
      Assert::AreEqual<std::size_t>(nodes.size(), 8);
      Assert::IsTrue(AreContainersItemsEqual(tree.GetNodeEntities(tree.GetNodeValue(15)), vector<EntityID>{ 4 }));
    }


    TEST_METHOD(Insert_LeafBut1_Successful)
    {
      auto constexpr vBoxL = array{
        BoundingBox1D{ 0.0, 1.0 },
        BoundingBox1D{ 1.0, 2.0 },
        BoundingBox1D{ 2.0, 3.0 },
        BoundingBox1D{ 3.0, 4.0 }
      };
      auto tree = DualtreeBox(vBoxL, 3, std::nullopt, 1);

      Assert::IsTrue(tree.InsertIntoLeaf(4, BoundingBox1D{ 1.0, 3.0 }, InsertionMode::LowestLeaf));

      auto const& nodes = tree.GetNodes();
      auto const entitiesInBFS = tree.GetEntitiesBreadthFirst();
      auto const entitiesInDFS = tree.GetEntitiesDepthFirst();

      Assert::AreEqual<std::size_t>(nodes.size(), 7);
      Assert::IsTrue(AreContainersItemsEqual(entitiesInBFS, vector<EntityID>{ 4, 0, 1, 2, 3 }));
      Assert::IsTrue(AreContainersItemsEqual(entitiesInDFS, vector<EntityID>{ 0, 1, 4, 2, 3 }));
    }


    TEST_METHOD(Insert_LeafBut2_Successful)
    {
      auto constexpr vBoxL = array{
        BoundingBox1D{ 0.0, 1.0 },
        BoundingBox1D{ 1.0, 2.0 },
        BoundingBox1D{ 2.0, 3.0 },
        BoundingBox1D{ 3.0, 4.0 }
      };
      auto tree = DualtreeBox(vBoxL, 3, std::nullopt, 1);

      Assert::IsTrue(tree.InsertIntoLeaf(4, BoundingBox1D{ 0.0, 2.0 }, InsertionMode::LowestLeaf));

      auto const& nodes = tree.GetNodes();
      auto const entitiesInBFS = tree.GetEntitiesBreadthFirst();
      auto const entitiesInDFS = tree.GetEntitiesDepthFirst();

      Assert::AreEqual<std::size_t>(nodes.size(), 7);
      Assert::IsTrue(AreContainersItemsEqual(entitiesInBFS, vector<EntityID>{ 4, 0, 1, 2, 3 }));
      Assert::IsTrue(AreContainersItemsEqual(entitiesInDFS, vector<EntityID>{ 4, 0, 1, 2, 3 }));
    }


    TEST_METHOD(Complex)
    {
      auto constexpr vBoxL = array{
        BoundingBox1D{ 0.0, 1.0 },
        BoundingBox1D{ 1.0, 2.0 },
        BoundingBox1D{ 2.0, 3.0 },
        BoundingBox1D{ 3.0, 4.0 }
      };
      auto tree = DualtreeBox(vBoxL, 3, std::nullopt, 1);
      tree.InsertIntoLeaf(4, BoundingBox1D{ 3.5, 3.7 }, InsertionMode::ExistingLeaf);
      tree.InsertIntoLeaf(5, BoundingBox1D{ 3.5, 3.7 }, InsertionMode::LowestLeaf);
      tree.Erase(3);
      tree.Update(4, BoundingBox1D{ 3.8, 3.9 });
      tree.Update(0, vBoxL.front(), BoundingBox1D{ 3.8, 3.9 });
      tree.Update(1, vBoxL[1]);
    }

    TEST_METHOD(Update2p_)
    {
      //!
    }

    TEST_METHOD(Update3p_)
    {
      //!
    }
    TEST_METHOD(Erase_)
    {
      //!
    }
  };
} // namespace Tree1DTest


namespace Tree2DTest
{

  TEST_CLASS(Point_General)
  {
    TEST_METHOD(Create_SetNo1)
    {
      auto constexpr N = 2;
      auto constexpr points = getPointSetNo1<N>();
      auto const nt = OrthoTreePointND<N>(points, 3, std::nullopt, 2);
      auto const& nodes = nt.GetNodes();
      Assert::AreEqual<std::size_t>(22, nodes.size());
    } // namespace Tree2DTest

    TEST_METHOD(PlaneSearch_3D_XY_dP0)
    {
      auto const vpt = vector{
        Point3D{ 2.0, -1.0,     0.0 },
        Point3D{ 3.0, -2.0,     1.0 },
        Point3D{ 4.0, -3.0,    0.02 },
        Point3D{ 4.0, -2.0,    0.00 },
        Point3D{ 5.0,  0.0, 0.00999 }
      };

      auto const tree = OctreePoint(vpt, 3, std::nullopt, 2);

      auto const ids = tree.PlaneSearch(0.0, Point3D{ 0.0, 0.0, 1.0 }, vpt, 0.01);
      Assert::IsTrue(std::ranges::is_permutation(vector<EntityID>{ 0, 3, 4 }, ids));
    }


    TEST_METHOD(PlaneSearch_3D_XY_dP1)
    {
      auto const vpt = vector{
        Point3D{ 2.0, -1.0,     0.0 },
        Point3D{ 3.0, -2.0,     1.0 },
        Point3D{ 4.0, -3.0,    0.02 },
        Point3D{ 4.0, -2.0,    0.00 },
        Point3D{ 5.0,  0.0, 1.00999 }
      };

      auto const tree = OctreePoint(vpt, 3, std::nullopt, 2);

      auto const ids = tree.PlaneSearch(1.0, Point3D{ 0.0, 0.0, 1.0 }, vpt, 0.01);
      Assert::IsTrue(std::ranges::is_permutation(vector<EntityID>{ 1, 4 }, ids));
    }


    TEST_METHOD(PlaneSearch_3D_XZ_dP4)
    {
      auto const vpt = vector{
        Point3D{ 2.0, -1.0,     0.0 },
        Point3D{ 3.0, -2.0,     1.0 },
        Point3D{ 4.0, -3.0,    0.02 },
        Point3D{ 4.0, -2.0,    0.00 },
        Point3D{ 5.0,  0.0, 1.00999 }
      };

      auto const tree = OctreePoint(vpt, 3, std::nullopt, 2);

      auto const ids = tree.PlaneSearch(4.0, Point3D{ 1.0, 0.0, 0.0 }, vpt, 0.01);
      Assert::IsTrue(std::ranges::is_permutation(vector<EntityID>{ 2, 3 }, ids));
    }


    TEST_METHOD(PlaneSearchC_3D_XZ_dP4)
    {
      auto const vpt = vector{
        Point3D{ 2.0, -1.0,     0.0 },
        Point3D{ 3.0, -2.0,     1.0 },
        Point3D{ 4.0, -3.0,    0.02 },
        Point3D{ 4.0, -2.0,    0.00 },
        Point3D{ 5.0,  0.0, 1.00999 }
      };

      auto const tree = OctreePointM(vpt, 3, std::nullopt, 2);

      auto const ids = tree.PlaneSearch(4.0, Point3D{ 1.0, 0.0, 0.0 }, 0.01);
      Assert::IsTrue(std::ranges::is_permutation(vector<EntityID>{ 2, 3 }, ids));
    }
  };

  TEST_CLASS(Point_kNNTest)
  {
    TEST_METHOD(N103_k2_RemainInSmallestNode__17_18)
    {
      auto constexpr N = 2;
      auto constexpr points = getPointSetNo1<N>();
      auto const tree = OrthoTreePointND<N>(points, 3, std::nullopt, 3);

      auto constexpr pt = PointND<N>{ 3.5, 5.5 };
      auto const vnn = tree.GetNearestNeighbors(pt, 2, points, 0.0);
      Assert::IsTrue(std::ranges::is_permutation(vector<EntityID>{ 17, 18 }, vnn));
    }

    TEST_METHOD(N103_k3__14_17_18)
    {
      auto constexpr N = 2;
      auto constexpr points = getPointSetNo1<N>();
      auto const tree = OrthoTreePointND<N>(points, 3, std::nullopt, 3);

      auto constexpr pt = PointND<N>{ 3.5, 5.5 };
      auto const vnn = tree.GetNearestNeighbors(pt, 3, points);
      Assert::IsTrue(std::ranges::is_permutation(vector<EntityID>{ 17, 18, 14 }, vnn));
    }

    TEST_METHOD(N103_k4__14_16_17_18)
    {
      auto constexpr N = 2;
      auto constexpr points = getPointSetNo1<N>();
      auto const tree = OrthoTreePointND<N>(points, 3, std::nullopt, 3);

      auto constexpr pt = PointND<N>{ 3.5, 5.5 };
      auto const vnn = tree.GetNearestNeighbors(pt, 4, points);
      Assert::IsTrue(std::ranges::is_permutation(vector<EntityID>{ 17, 18, 14, 16 }, vnn));
    }

    TEST_METHOD(N103_k4_within0__17_18)
    {
      auto constexpr N = 2;
      auto constexpr points = getPointSetNo1<N>();
      auto const tree = OrthoTreePointND<N>(points, 3, std::nullopt, 3);

      auto constexpr pt = PointND<N>{ 3.5, 5.5 };
      auto const vnn = tree.GetNearestNeighbors(pt, 4, 0, points);
      Assert::IsTrue(std::ranges::is_permutation(vector<EntityID>{ 17, 18 }, vnn));
    }

    TEST_METHOD(N103_k5_within1__14_17_18_16)
    {
      auto constexpr N = 2;
      auto constexpr points = getPointSetNo1<N>();
      auto const tree = OrthoTreePointND<N>(points, 3, std::nullopt, 3);

      auto constexpr pt = PointND<N>{ 3.5, 5.5 };
      auto const vnn = tree.GetNearestNeighbors(pt, 5, 1.0, points);
      Assert::IsTrue(std::ranges::is_permutation(vector<EntityID>{ 17, 18, 14, 16 }, vnn));
    }

    TEST_METHOD(N103_k100_OverTheContainingElements__All)
    {
      auto constexpr N = 2;
      auto constexpr points = getPointSetNo1<N>();
      auto const tree = OrthoTreePointND<N>(points, 3, std::nullopt, 3);

      auto constexpr pt = PointND<N>{ 3.5, 5.5 };
      auto const vnn = tree.GetNearestNeighbors(pt, 100, points);
      Assert::AreEqual(points.size(), vnn.size());

      auto Entities = vector<EntityID>(points.size());
      iota(begin(Entities), end(Entities), 0);
      Assert::IsTrue(std::ranges::is_permutation(Entities, vnn));
    }

    TEST_METHOD(N4_k1__1)
    {
      auto constexpr N = 2;
      auto constexpr points = getPointSetNo1<N>();
      auto const tree = OrthoTreePointND<N>(points, 3, std::nullopt, 3);

      auto constexpr pt = PointND<N>{ 2.0, 2.0 };
      auto const vnn = tree.GetNearestNeighbors(pt, 1, points);
      Assert::IsTrue(std::ranges::is_permutation(vector<EntityID>{ 1 }, vnn));
    }

    TEST_METHOD(N4_k2__1_11_12)
    {
      auto constexpr N = 2;
      auto constexpr points = getPointSetNo1<N>();
      auto const tree = OrthoTreePointND<N>(points, 3, std::nullopt, 3);

      auto constexpr pt = PointND<N>{ 2.0, 2.0 };
      auto const vnn = tree.GetNearestNeighbors(pt, 2, points);
      Assert::IsTrue(std::ranges::is_permutation(vector<EntityID>{ 1, 11 }, vnn));
    }

    TEST_METHOD(N4_k3__1_11_12)
    {
      auto constexpr N = 2;
      auto constexpr points = getPointSetNo1<N>();
      auto const tree = OrthoTreePointND<N>(points, 3, std::nullopt, 3);

      auto constexpr pt = PointND<N>{ 2.0, 2.0 };
      auto const vnn = tree.GetNearestNeighbors(pt, 3, points);
      Assert::IsTrue(std::ranges::is_permutation(vector<EntityID>{ 1, 11, 12 }, vnn));
    }

    // outside

    TEST_METHOD(OutSide_k1__0)
    {
      auto constexpr N = 2;
      auto constexpr points = getPointSetNo1<N>();
      auto const tree = OrthoTreePointND<N>(points, 3, std::nullopt, 3);

      auto constexpr pt = PointND<N>{ -1.0, -1.0 };
      auto const vnn = tree.GetNearestNeighbors(pt, 1, points);
      Assert::IsTrue(std::ranges::is_permutation(vector<EntityID>{ 0 }, vnn));
    }

    TEST_METHOD(OutSide_k5__0_1_5_6_11)
    {
      auto constexpr N = 2;
      auto constexpr points = getPointSetNo1<N>();
      auto const tree = OrthoTreePointND<N>(points, 3, std::nullopt, 3);

      auto constexpr pt = PointND<N>{ -1.0, -1.0 };
      auto const vnn = tree.GetNearestNeighbors(pt, 5, points);
      Assert::IsTrue(std::ranges::is_permutation(vector<EntityID>{ 0, 1, 5, 6, 11 }, vnn));
    }

    TEST_METHOD(OutSide_k5_16D__0_1_5_6_11)
    {
      auto constexpr N = 23;
      auto constexpr points = getPointSetNo1<N>();
      auto const tree = OrthoTreePointND<N>(points, 3, std::nullopt, 3);

      auto constexpr pt = PointND<N>{ -1.0, -1.0 };
      auto const vnn = tree.GetNearestNeighbors(pt, 5, points);
      Assert::IsTrue(std::ranges::is_permutation(vector<EntityID>{ 0, 1, 5, 6, 11 }, vnn));
    }


    TEST_METHOD(GetNearestNeighbors_EntityDistanceFn)
    {
      using Box = BoundingBox2D;
      using Pt = Point2D;
      constexpr array<Box, 3> boxes = {
        Box{ { 0.0, 0.0 }, { 2.0, 2.0 } }, // id=0
        Box{ { 2.0, 2.0 }, { 4.0, 4.0 } }, // id=1
        Box{ { 5.0, 5.0 }, { 6.0, 6.0 } }  // id=2
      };
      const auto tree = QuadtreeBoxs<false>(boxes, 3, std::nullopt, 2);

      const Pt searchPt{ 2.5, 2.5 };

      auto realEntityDistance = [&](const Pt&, EntityID id) -> double {
        switch (id)
        {
        case 0: return 1.0;
        case 1: return 1.25;
        case 2: return 4.0;
        }

        return double(id * 10);
      };

      auto const result =
        tree.GetNearestNeighbors(searchPt, 1, std::numeric_limits<double>::max(), boxes, std::numeric_limits<double>::epsilon(), realEntityDistance);

      vector<EntityID> expected{ 0 };
      Assert::AreEqual<size_t>(1, result.size());
      Assert::IsTrue(std::ranges::is_permutation(result, expected));
    }

    TEST_METHOD(GetNearestNeighbors_MinMaxDistance)
    {
      using Box = BoundingBox2D;
      using Pt = Point2D;

      // od: optimistic distance
      // pd: pessimistic distance
      constexpr auto boxes = std::array{
        Box{ { +0.20, 0.00 }, { +2.31, 2.31 } }, // od: 0.20, pd: 2.31 // winner in tie
        Box{ { +1.00, 0.00 }, { +1.01, 1.41 } }, // od: 1.00, pd: 1.41 // closed out by [2]
        Box{ { +0.40, 0.00 }, { +0.89, 0.89 } }, // od: 0.40, pd: 0.89 // possible winner by pessimistic distance
        Box{ { +2.10, 0.00 }, { +3.66, 3.66 } }, // od: 2.10, pd: 3.66 // obviously ruled out
        Box{ { -2.21, 0.00 }, { -0.20, 2.21 } }, // od: 0.20, pd: 2.21 // winner in tie
        Box{ { -2.50, 0.00 }, { -0.90, 2.47 } }, // od: 0.90, pd: 2.47 // ruled out by [2]
      };
      const auto tree = QuadtreeBoxs<false>(boxes, 3, std::nullopt, 2);

      const Pt searchPt{ 0.0, 0.0 };

      auto const result = tree.GetNearestNeighbors(searchPt, 1, std::numeric_limits<double>::max(), boxes, std::numeric_limits<double>::epsilon());

      vector<EntityID> expected{ 0, 2, 4 };
      Assert::AreEqual<size_t>(expected.size(), result.size());
      Assert::IsTrue(std::ranges::is_permutation(result, expected));
    }

    TEST_METHOD(Issue9_2D)
    {
      auto const poses = vector<array<double, 2>>{
        { 78.2619,  77.843 },
        { 90.3005, 90.5172 },
        { 69.8652, 12.2467 },
        { 48.4675, 48.4948 },
        { 36.3226, 68.4619 },
        { 98.8799, 42.7149 },
        {  31.412, 38.6866 },
        { 63.2748, 77.0524 },
        { 62.8844, 17.0536 },
        { 80.8931, 39.8099 },
        {  77.426, 64.9844 },
        { 81.9552,  25.009 },
        { 87.6088,  51.319 },
        { 78.5609, 80.4623 },
        { 51.3967, 39.5269 },
        { 32.2042, 81.8779 },
        { 79.1739, 81.5467 },
        { 95.2619, 40.4742 },
        {  86.437, 92.4406 },
        { 3.95388, 60.2327 },
        { 31.1283, 44.4917 },
        { 35.6778, 79.8545 },
        { 50.9926, 66.1373 },
        { 3.16271, 65.2519 },
        { 56.3665, 45.3819 }
      };


      auto constexpr search_point = array<double, 2>{ 43.6406, 57.5691 };
      using GA = OrthoTree::GeneralGeometryAdapterTemplate<2, array<double, 2>, OrthoTree::BoundingBox2D, OrthoTree::Ray2D, OrthoTree::Plane2D>;
      auto const itMin = std::ranges::min_element(poses, [&search_point](auto const& lhs, auto const& rhs) {
        return GA::Distance2(lhs, search_point) < GA::Distance2(rhs, search_point);
      });

      std::array<double, 2> inspection_space_min = { 0.0, 0.0 };
      std::array<double, 2> inspection_space_max = { 100.0, 100.0 };
      OrthoTree::BoundingBox2D inspection_space;
      inspection_space.Min = inspection_space_min;
      inspection_space.Max = inspection_space_max;

      auto tree = QuadtreePointM(poses, 9, inspection_space);

      auto neighbors = tree.GetNearestNeighbors(search_point, 1);
      Assert::AreEqual<EntityID>(EntityID(std::distance(poses.begin(), itMin)), neighbors[0]);
    }


    TEST_METHOD(Issue9_6D)
    {
      using VectorType = array<double, 6>;

      auto const poses = vector<VectorType>{
        {  50.2232, 0.276687, 37.7662, 41.2776,  26.3818,  74.0284 },
        {  35.8946,  83.7503, 97.1127, 47.2895,  40.9232,  83.7666 },
        {  33.3541,  63.0669, 3.86075, 47.7923,  19.8039,  87.5608 },
        {  89.0684,  67.3278,  50.867, 49.8193,  72.6692,  54.0271 },
        {  75.7099,  53.1241,  39.624, 40.4669,  13.6433,  88.6247 },
        {  43.7641,  93.7985, 68.8286, 9.71882,  2.16644,  12.1925 },
        {  82.5195,  30.6964, 71.0556, 48.4744,  99.2295,  70.4137 },
        {  28.4568,  37.8366, 74.8597,  27.897,  60.6816, 0.247821 },
        {  58.3617,  17.0165, 15.1021, 70.9832,  22.5325,  34.8085 },
        {  33.1004,  72.6729, 35.7043, 35.2888,  94.9917,   17.652 },
        {  80.9693,  7.41406, 2.18394, 40.2606,  1.63274,  65.8996 },
        {  88.4851,  73.8366, 55.0264, 77.6467,  61.6751,  33.7444 },
        {  48.1777,  90.1285, 75.9069, 49.1867,  39.7186,  74.2862 },
        {  68.0826,  73.0622, 7.85933, 60.8879,  41.3229,  16.6928 },
        {  19.1693,  49.1172,  91.055, 52.5548,  16.2326, 0.610075 },
        {  19.3085,  81.9363, 55.8924, 1.77161,  29.4124,  72.3873 },
        {  57.8801,  28.3918, 45.7062, 92.1774, 0.742745,  29.0499 },
        {  74.2952,  84.6691, 46.3357, 76.7894,  68.8394,  24.1707 },
        { 0.199418,  93.2845, 94.9628, 38.2295,  82.1998,  59.2499 },
        {  68.4689, 0.261607, 75.5001,  58.427,  55.4243,  18.7392 },
        {  53.9164,  95.4966,  59.657, 71.0292,  82.4362,  53.9452 },
      };

      auto const search_point = VectorType{ 78.8658, 64.0361, 18.7755, 61.4618, 14.3312, 40.0196 };
      using GA = OrthoTree::GeneralGeometryAdapterTemplate<6, VectorType, OrthoTree::BoundingBoxND<6>, OrthoTree::RayND<6>, OrthoTree::PlaneND<6>>;
      auto const itMinExpected = std::ranges::min_element(poses, [&search_point](auto const& lhs, auto const& rhs) {
        return GA::Distance2(lhs, search_point) < GA::Distance2(rhs, search_point);
      });

      auto const inspection_space = OrthoTree::BoundingBoxND<6>{
        {   0.0,   0.0,   0.0,   0.0,   0.0,   0.0 },
        { 100.0, 100.0, 100.0, 100.0, 100.0, 100.0 }
      };

      // Standard Tree
      auto tree = OrthoTreePointND<6>();
      tree.Create(tree, poses, 9, inspection_space);

      auto const neighbors = tree.GetNearestNeighbors(search_point, 1, poses);
      auto const idMinExpected = EntityID(std::distance(poses.begin(), itMinExpected));
      Assert::AreEqual<EntityID>(idMinExpected, neighbors[0]);
    }

    TEST_METHOD(BoxNN_k1__InsideSingleBox)
    {
      auto constexpr boxes = array{
        BoundingBox2D{ { 0.0, 0.0 }, { 1.0, 1.0 } }, // 0
        BoundingBox2D{ { 1.0, 1.0 }, { 3.0, 3.0 } }, // 1
        BoundingBox2D{ { 4.0, 4.0 }, { 5.0, 5.0 } }  // 2
      };

      auto const tree = QuadtreeBoxs<false>(boxes, 3, std::nullopt, 2);

      auto constexpr pt = Point2D{ 2.0, 2.0 }; // inside box 1
      auto const vnn = tree.GetNearestNeighbors(pt, 1, boxes);

      Assert::IsTrue(std::ranges::is_permutation(vector<EntityID>{ 1 }, vnn));
    }

    TEST_METHOD(BoxNN_k1__OutsideSingleBox)
    {
      auto constexpr boxes = array{
        BoundingBox2D{ { 0.0, 0.0 }, { 1.0, 1.0 } }, // 0
        BoundingBox2D{ { 1.0, 1.0 }, { 3.0, 3.0 } }, // 1
        BoundingBox2D{ { 4.0, 4.0 }, { 5.0, 5.0 } }  // 2
      };

      auto const tree = QuadtreeBoxs<false>(boxes, 3, std::nullopt, 2);

      auto constexpr pt = Point2D{ -1.0, 1.0 }; // outside of the box, further distance rank lower the [1].
      auto const vnn = tree.GetNearestNeighbors(pt, 1, boxes);

      Assert::IsTrue(std::ranges::is_permutation(vector<EntityID>{ 0 }, vnn));
    }
  };

  TEST_CLASS(Box_General)
  {
    TEST_METHOD(Create_d3__Node7)
    {
      auto constexpr boxes = array{
        BoundingBox2D{ { 0.0, 0.0 }, { 1.0, 1.0 } },
        BoundingBox2D{ { 1.0, 1.0 }, { 2.0, 2.0 } },
        BoundingBox2D{ { 2.0, 2.0 }, { 3.0, 3.0 } },
        BoundingBox2D{ { 3.0, 3.0 }, { 4.0, 4.0 } },
        BoundingBox2D{ { 1.2, 1.2 }, { 2.8, 2.8 } }
      };

      auto const quadtreebox = QuadtreeBox(boxes, 3, std::nullopt, 1);
      auto const nodes = quadtreebox.GetNodes();
      Assert::AreEqual<std::size_t>(7, nodes.size());
      Assert::IsTrue(nodes.contains(1));
      Assert::IsTrue(quadtreebox.IsNodeEntitiesEmpty(quadtreebox.GetRootNodeValue()));

      Assert::IsTrue(nodes.contains(4));
      Assert::IsTrue(quadtreebox.IsNodeEntitiesEmpty(quadtreebox.GetRootNodeValue()));

      Assert::IsTrue(nodes.contains(7));
      Assert::IsTrue(AreContainersItemsEqual(quadtreebox.GetNodeEntities(quadtreebox.GetNodeValue(7)), vector<EntityID>{ 4 }));

      Assert::IsTrue(nodes.contains(16));
      Assert::IsTrue(AreContainersItemsEqual(quadtreebox.GetNodeEntities(quadtreebox.GetNodeValue(16)), vector<EntityID>{ 0 }));

      Assert::IsTrue(nodes.contains(19));
      Assert::IsTrue(AreContainersItemsEqual(quadtreebox.GetNodeEntities(quadtreebox.GetNodeValue(19)), vector<EntityID>{ 1 }));

      Assert::IsTrue(nodes.contains(28));
      Assert::IsTrue(AreContainersItemsEqual(quadtreebox.GetNodeEntities(quadtreebox.GetNodeValue(28)), vector<EntityID>{ 2 }));

      Assert::IsTrue(nodes.contains(31));
      Assert::IsTrue(AreContainersItemsEqual(quadtreebox.GetNodeEntities(quadtreebox.GetNodeValue(31)), vector<EntityID>{ 3 }));
    }


    TEST_METHOD(Create_d3__Node8)
    {
      auto constexpr boxes = array{
        BoundingBox2D{ { 0.0, 0.0 }, { 1.0, 1.0 } },
        BoundingBox2D{ { 1.0, 1.0 }, { 2.0, 2.0 } },
        BoundingBox2D{ { 2.0, 2.0 }, { 3.0, 3.0 } },
        BoundingBox2D{ { 3.0, 3.0 }, { 4.0, 4.0 } },
        BoundingBox2D{ { 1.2, 1.2 }, { 2.8, 2.8 } },
        BoundingBox2D{ { 1.5, 1.5 }, { 2.0, 2.0 } },
      };

      auto const quadtreebox = QuadtreeBox(boxes, 3, std::nullopt, 1);
      auto const nodes = quadtreebox.GetNodes();
      Assert::AreEqual<std::size_t>(8, nodes.size());

      Assert::IsTrue(nodes.contains(1));
      Assert::IsTrue(quadtreebox.IsNodeEntitiesEmpty(quadtreebox.GetNodeValue(1)));

      Assert::IsTrue(nodes.contains(4));
      Assert::IsTrue(quadtreebox.IsNodeEntitiesEmpty(quadtreebox.GetNodeValue(4)));

      Assert::IsTrue(nodes.contains(7));
      Assert::IsTrue(AreContainersItemsEqual(quadtreebox.GetNodeEntities(quadtreebox.GetNodeValue(7)), vector<EntityID>{ 4 }));

      Assert::IsTrue(nodes.contains(16));
      Assert::IsTrue(AreContainersItemsEqual(quadtreebox.GetNodeEntities(quadtreebox.GetNodeValue(16)), vector<EntityID>{ 0 }));

      Assert::IsTrue(nodes.contains(19));
      Assert::IsTrue(AreContainersItemsEqual(quadtreebox.GetNodeEntities(quadtreebox.GetNodeValue(19)), vector<EntityID>{ 1 }));

      Assert::IsTrue(nodes.contains(28));
      Assert::IsTrue(AreContainersItemsEqual(quadtreebox.GetNodeEntities(quadtreebox.GetNodeValue(28)), vector<EntityID>{ 2 }));

      Assert::IsTrue(nodes.contains(31));
      Assert::IsTrue(AreContainersItemsEqual(quadtreebox.GetNodeEntities(quadtreebox.GetNodeValue(31)), vector<EntityID>{ 3 }));

      Assert::IsTrue(nodes.contains(79));
      Assert::IsTrue(AreContainersItemsEqual(quadtreebox.GetNodeEntities(quadtreebox.GetNodeValue(79)), vector<EntityID>{ 5 }));
    }
  };

  TEST_CLASS(Box_SearchTest)
  {
    TEST_METHOD(RangeSearch__Inside__124)
    {
      auto constexpr boxes = array{
        BoundingBox2D{ { 0.0, 0.0 }, { 1.0, 1.0 } },
        BoundingBox2D{ { 1.0, 1.0 }, { 2.0, 2.0 } },
        BoundingBox2D{ { 2.0, 2.0 }, { 3.0, 3.0 } },
        BoundingBox2D{ { 3.0, 3.0 }, { 4.0, 4.0 } },
        BoundingBox2D{ { 1.2, 1.2 }, { 2.8, 2.8 } }
      };

      auto const quadtreebox = QuadtreeBox(boxes, 3, std::nullopt, 2);

      auto const search_box = BoundingBox2D{
        { 1.0, 1.0 },
        { 3.1, 3.1 }
      };
      auto const ids = quadtreebox.RangeSearch(search_box, boxes); // { 1, 2, 4 }

      Assert::IsTrue(std::ranges::is_permutation(vector<EntityID>{ 1, 2, 4 }, ids));
    }

    TEST_METHOD(RangeSearch__Overlap__1234)
    {
      auto constexpr boxes = array{
        BoundingBox2D{ { 0.0, 0.0 }, { 1.0, 1.0 } },
        BoundingBox2D{ { 1.0, 1.0 }, { 2.0, 2.0 } },
        BoundingBox2D{ { 2.0, 2.0 }, { 3.0, 3.0 } },
        BoundingBox2D{ { 3.0, 3.0 }, { 4.0, 4.0 } },
        BoundingBox2D{ { 1.2, 1.2 }, { 2.8, 2.8 } }
      };

      auto const quadtreebox = QuadtreeBox(boxes, 3, std::nullopt, 2);

      auto const search_box = BoundingBox2D{
        { 1.0, 1.0 },
        { 3.1, 3.1 }
      };
      auto const ids = quadtreebox.RangeSearch(search_box, boxes, RangeSearchMode::Overlap); // { 1, 2, 3, 4 }

      Assert::IsTrue(std::ranges::is_permutation(vector<EntityID>{ 1, 2, 3, 4 }, ids));
    }

    TEST_METHOD(PickSearch__24)
    {
      auto constexpr boxes = array{
        BoundingBox2D{ { 0.0, 0.0 }, { 1.0, 1.0 } },
        BoundingBox2D{ { 1.0, 1.0 }, { 2.0, 2.0 } },
        BoundingBox2D{ { 2.0, 2.0 }, { 3.0, 3.0 } },
        BoundingBox2D{ { 3.0, 3.0 }, { 4.0, 4.0 } },
        BoundingBox2D{ { 1.2, 1.2 }, { 2.8, 2.8 } }
      };

      auto const quadtreebox = QuadtreeBox(boxes, 3, std::nullopt, 2);

      auto const ptPick = Point2D{ 2.5, 2.5 };
      auto ids_picked = quadtreebox.PickSearch(ptPick, boxes); // { 2, 4 }

      Assert::IsTrue(std::ranges::is_permutation(vector<EntityID>{ 2, 4 }, ids_picked));
    }


    TEST_METHOD(PickSearch__124)
    {
      auto constexpr boxes = array{
        BoundingBox2D{ { 0.0, 0.0 }, { 1.0, 1.0 } },
        BoundingBox2D{ { 1.0, 1.0 }, { 2.0, 2.0 } },
        BoundingBox2D{ { 2.0, 2.0 }, { 3.0, 3.0 } },
        BoundingBox2D{ { 3.0, 3.0 }, { 4.0, 4.0 } },
        BoundingBox2D{ { 1.2, 1.2 }, { 2.8, 2.8 } }
      };

      auto const quadtreebox = QuadtreeBox(boxes, 3, std::nullopt, 2);

      auto const ptPick = Point2D{ 2.0, 2.0 };
      auto ids_picked = quadtreebox.PickSearch(ptPick, boxes); // { 1, 2, 4 }

      Assert::IsTrue(std::ranges::is_permutation(vector<EntityID>{ 1, 2, 4 }, ids_picked));
    }


    TEST_METHOD(PlaneIntersection_3D_XY_d0)
    {
      auto constexpr boxes = array{
        BoundingBox3D{  { 0.0, 0.0, 0.0 },  { 1.0, 1.0, 0.0 } },
        BoundingBox3D{ { 1.0, 1.0, -1.0 },  { 2.0, 2.0, 0.0 } },
        BoundingBox3D{ { 2.0, 2.0, -1.0 }, { 3.0, 3.0, -0.2 } },
        BoundingBox3D{  { 3.0, 3.0, 1.0 },  { 4.0, 4.0, 1.2 } },
        BoundingBox3D{ { 1.2, 1.2, -1.0 },  { 2.8, 2.8, 1.0 } }
      };

      auto const octreebox = OctreeBox(boxes, 3, std::nullopt, 2);
      auto const intersectedBoxes = octreebox.PlaneIntersection(0.0, Point3D{ 0.0, 0.0, 1.0 }, boxes, 0.01);

      Assert::IsTrue(std::ranges::is_permutation(vector<EntityID>{ 0, 1, 4 }, intersectedBoxes));
    }


    TEST_METHOD(PlaneIntersection_3D_XY_dP1)
    {
      auto constexpr boxes = array{
        BoundingBox3D{  { 0.0, 0.0, 0.0 },  { 1.0, 1.0, 0.0 } },
        BoundingBox3D{ { 1.0, 1.0, -1.0 },  { 2.0, 2.0, 0.0 } },
        BoundingBox3D{ { 2.0, 2.0, -1.0 }, { 3.0, 3.0, -0.2 } },
        BoundingBox3D{  { 3.0, 3.0, 0.8 },  { 4.0, 4.0, 0.9 } },
        BoundingBox3D{ { 1.2, 1.2, -1.0 },  { 2.8, 2.8, 1.1 } }
      };

      auto const octreebox = OctreeBox(boxes, 3, std::nullopt, 2);
      auto const intersectedBoxes = octreebox.PlaneIntersection(1.0, Point3D{ 0.0, 0.0, 1.0 }, boxes, 0.01);

      Assert::IsTrue(std::ranges::is_permutation(vector<EntityID>{ 4 }, intersectedBoxes));
    }

    TEST_METHOD(PlaneIntersection_3D_XY_dN1)
    {
      auto constexpr boxes = array{
        BoundingBox3D{  { 0.0, 0.0, 0.0 },  { 1.0, 1.0, 0.0 } },
        BoundingBox3D{ { 1.0, 1.0, -1.0 },  { 2.0, 2.0, 0.0 } },
        BoundingBox3D{ { 2.0, 2.0, -1.0 }, { 3.0, 3.0, -0.2 } },
        BoundingBox3D{  { 3.0, 3.0, 1.0 },  { 4.0, 4.0, 1.2 } },
        BoundingBox3D{ { 1.2, 1.2, -1.1 },  { 2.8, 2.8, 1.1 } }
      };

      auto const octreebox = OctreeBox(boxes, 3, std::nullopt, 2);
      auto const intersectedBoxes = octreebox.PlaneIntersection(-1.0, Point3D{ 0.0, 0.0, 1.0 }, boxes, 0.01);

      Assert::IsTrue(std::ranges::is_permutation(vector<EntityID>{ 1, 2, 4 }, intersectedBoxes));
    }


    TEST_METHOD(PlaneIntersection_3D_YZ_dP1)
    {
      auto constexpr boxes = array{
        BoundingBox3D{  { 0.0, 0.0, 0.0 },  { 1.0, 1.0, 0.0 } },
        BoundingBox3D{ { 1.0, 1.0, -1.0 },  { 2.0, 2.0, 0.0 } },
        BoundingBox3D{ { 2.0, 2.0, -1.0 }, { 3.0, 3.0, -0.2 } },
        BoundingBox3D{  { 3.0, 3.0, 1.0 },  { 4.0, 4.0, 1.2 } },
        BoundingBox3D{ { 1.2, 1.2, -1.1 },  { 2.8, 2.8, 1.1 } }
      };

      auto const octreebox = OctreeBox(boxes, 3, std::nullopt, 2);
      auto const intersectedBoxes = octreebox.PlaneIntersection(1.0, Point3D{ 1.0, 0.0, 0.0 }, boxes, 0.01);

      Assert::IsTrue(std::ranges::is_permutation(vector<EntityID>{ 0, 1 }, intersectedBoxes));
    }

    TEST_METHOD(PlaneIntersection_3D_YZ_dN1)
    {
      auto constexpr boxes = array{
        BoundingBox3D{  { 0.0, 0.0, 0.0 },  { 1.0, 1.0, 0.0 } },
        BoundingBox3D{ { 1.0, 1.0, -1.0 },  { 2.0, 2.0, 0.0 } },
        BoundingBox3D{ { 2.0, 2.0, -1.0 }, { 3.0, 3.0, -0.2 } },
        BoundingBox3D{  { 3.0, 3.0, 1.0 },  { 4.0, 4.0, 1.2 } },
        BoundingBox3D{ { 1.2, 1.2, -1.1 },  { 2.8, 2.8, 1.1 } }
      };

      auto const octreebox = OctreeBox(boxes, 3, std::nullopt, 2);
      auto const intersectedBoxes = octreebox.PlaneIntersection(-1.0, Point3D{ 1.0, 0.0, 0.0 }, boxes, 0.01);

      Assert::IsTrue(std::ranges::is_permutation(vector<EntityID>{}, intersectedBoxes));
    }

    TEST_METHOD(PlaneIntersectionC_3D_YZ_dN1)
    {
      auto constexpr boxes = array{
        BoundingBox3D{  { 0.0, 0.0, 0.0 },  { 1.0, 1.0, 0.0 } },
        BoundingBox3D{ { 1.0, 1.0, -1.0 },  { 2.0, 2.0, 0.0 } },
        BoundingBox3D{ { 2.0, 2.0, -1.0 }, { 3.0, 3.0, -0.2 } },
        BoundingBox3D{  { 3.0, 3.0, 1.0 },  { 4.0, 4.0, 1.2 } },
        BoundingBox3D{ { 1.2, 1.2, -1.1 },  { 2.8, 2.8, 1.1 } }
      };

      auto const octreebox = OctreeBoxM(boxes, 3, std::nullopt, 2);
      auto const intersectedBoxes = octreebox.PlaneIntersection(-1.0, Point3D{ 1.0, 0.0, 0.0 }, 0.01);

      Assert::IsTrue(std::ranges::is_permutation(vector<EntityID>{}, intersectedBoxes));
    }
    TEST_METHOD(PlaneIntersection_3D_YZA_dPSQRT2)
    {
      auto constexpr boxes = array{
        BoundingBox3D{  { 0.0, 0.0, 0.0 },  { 1.0, 1.0, 0.0 } },
        BoundingBox3D{ { 1.0, 1.0, -1.0 },  { 2.0, 2.0, 0.0 } },
        BoundingBox3D{ { 2.0, 2.0, -1.0 }, { 3.0, 3.0, -0.2 } },
        BoundingBox3D{  { 3.0, 3.0, 1.0 },  { 4.0, 4.0, 1.2 } },
        BoundingBox3D{ { 1.2, 1.2, -1.1 },  { 2.8, 2.8, 1.1 } }
      };

      auto const octreebox = OctreeBox(boxes, 3, std::nullopt, 2);
      auto const sqrt2__2 = std::sqrt(2.0) * 0.5;
      auto const intersectedBoxes = octreebox.PlaneIntersection(std::sqrt(2.0), Point3D{ 0.0, sqrt2__2, sqrt2__2 }, boxes, 0.01);

      Assert::IsTrue(std::ranges::is_permutation(vector<EntityID>{ 1, 2, 4 }, intersectedBoxes));
    }


    TEST_METHOD(CollistionDetection__33_24_34)
    {
      auto constexpr boxesL = array{
        BoundingBox2D{ { 0.0, 0.0 }, { 1.0, 1.0 } },
        BoundingBox2D{ { 1.0, 1.0 }, { 2.0, 2.0 } },
        BoundingBox2D{ { 2.0, 2.0 }, { 3.0, 3.0 } },
        BoundingBox2D{ { 3.0, 3.0 }, { 4.0, 4.0 } },
      };

      auto constexpr boxesR = array{
        BoundingBox2D{ { 2.0, 0.0 }, { 3.0, 1.0 } },
        BoundingBox2D{ { 3.0, 1.0 }, { 4.0, 2.0 } },
        BoundingBox2D{ { 3.0, 2.0 }, { 4.0, 3.0 } },
        BoundingBox2D{ { 2.0, 3.0 }, { 4.0, 4.0 } },
        BoundingBox2D{ { 2.5, 2.5 }, { 3.5, 3.5 } },
      };

      auto const qtL = QuadtreeBox(boxesL, 3, std::nullopt, 2);
      auto const qtR = QuadtreeBox(boxesR, 3, std::nullopt, 2);

      auto const aActualPair = QuadtreeBox::CollisionDetection(qtL, boxesL, qtR, boxesR); // { {3, 3}, {2, 4}, {3, 4} }

      auto constexpr aExpectedPair = array{
        std::pair<EntityID, EntityID>{ 3, 3 }, // Level 0
        std::pair<EntityID, EntityID>{ 2, 4 },
        std::pair<EntityID, EntityID>{ 3, 4 }, // Level 1
      };
      Assert::AreEqual(aExpectedPair.size(), aActualPair.size());
      Assert::IsTrue(std::ranges::is_permutation(aExpectedPair, aActualPair, [](auto const& p1, auto const& p2) {
        return p1.first == p2.first && p1.second == p2.second;
      }));
    }
  };
} // namespace Tree2DTest


namespace Tree3DTest
{
  TEST_CLASS(Point_Insert)
  {
    TEST_METHOD(CreateWithDataThenInsert)
    {
      auto points = std::vector<Point3D>{
        { -2.0, -2.0, -2.0 },
        { -1.0, -1.0, +2.0 },
        { +2.0, +2.0, +2.0 },
        { +4.0, +1.0, +1.0 },
        { +2.5, +2.5, +2.5 },
        { +3.3, +3.3, +3.3 },
      };
      auto pointNo = EntityID(points.size());

      OctreePoint tree(
        points,
        8,
        BoundingBox3D{
          { -4, -4, -4 },
          { +4, +4, +4 }
      },
        1);
      tree.UpdateIndexes({}, SEQ_EXEC);

      points.emplace_back(Point3D{ +5.0, +4.0, +4.0 }); // Keep points[entityID] consistent (outsider is out-of-space, not out-of-container)
      auto const isOutsiderInserted = tree.InsertIntoLeaf(pointNo, points.back(), InsertionMode::ExistingLeaf);
      Assert::IsTrue(isOutsiderInserted);
      ++pointNo;

      points.emplace_back(Point3D{ +3.0, +3.0, +3.0 });
      tree.InsertIntoLeaf(pointNo, points.back(), InsertionMode::ExistingLeaf);
      auto const nodeID_6 = tree.Find(pointNo);
      Assert::AreEqual<OctreePoint::NodeID>(nodeID_6, 1023); // 1023: 5,6
      ++pointNo;

      points.emplace_back(Point3D{ +2.0, +2.0, +2.0 });
      tree.InsertIntoLeaf(pointNo, points.back(), InsertionMode::LowestLeaf);
      auto const nodeID_7 = tree.Find(pointNo);
      Assert::AreEqual<OctreePoint::NodeID>(nodeID_7, 33292288); // It should place in the leaf node
      ++pointNo;

      points.emplace_back(Point3D{ +3.25, +3.25, +3.25 });
      tree.InsertIntoLeaf(pointNo, points.back(), InsertionMode::ExistingLeaf);
      auto const nodeID_8 = tree.Find(pointNo);
      Assert::AreEqual<OctreePoint::NodeID>(nodeID_8, 1023); // Parent has a child, another child should be added.
      ++pointNo;

      // InsertIntoLeaf
      {
        points.emplace_back(Point3D{ +3.15, +3.15, +3.15 });
        tree.InsertIntoLeaf(pointNo, points.back(), InsertionMode::ExistingLeaf);
        auto const nodeID_9 = tree.Find(pointNo);
        Assert::AreEqual<OctreePoint::NodeID>(nodeID_9, 1023); // It should stuck on a parent level
        ++pointNo;

        points.emplace_back(Point3D{ +3.0, +3.0, +3.5 });
        tree.InsertIntoLeaf(pointNo, points.back(), InsertionMode::ExistingLeaf);
        auto const nodeID_10 = tree.Find(pointNo);
        Assert::AreEqual<OctreePoint::NodeID>(nodeID_10, 1023); // It should stuck on a parent level
        ++pointNo;

        {
          points.emplace_back(Point3D{ +3.0, +3.0, +3.15 });
          tree.InsertIntoLeaf(pointNo, points.back(), InsertionMode::ExistingLeaf);
          auto const nodeID_11 = tree.Find(pointNo);
          Assert::AreEqual<OctreePoint::NodeID>(nodeID_11, 1023); // It should stuck on a parent level
          ++pointNo;

          auto const& nodes = tree.GetNodes();
          auto const entitiesInBFS = tree.GetEntitiesBreadthFirst();
          auto const entitiesInDFS = tree.GetEntitiesDepthFirst();

          Assert::AreEqual<std::size_t>(11, nodes.size());
          Assert::IsTrue(entitiesInBFS == std::vector<EntityID>{ 6, 0, 1, 3, 5, 7, 9, 10, 11, 12, 2, 4, 8 });
          Assert::IsTrue(entitiesInDFS == std::vector<EntityID>{ 6, 0, 1, 3, 2, 8, 4, 5, 7, 9, 10, 11, 12 });
        }

        {
          points.emplace_back(Point3D{ +3.75, +3.75, +3.75 });
          tree.Insert(pointNo, points.back(), points);
          auto const nodeID_12 = tree.Find(pointNo);
          Assert::AreEqual<OctreePoint::NodeID>(nodeID_12, 33553920);
          ++pointNo;

          auto const nodeID_10_u = tree.Find(10);
          auto const nodeID_11_u = tree.Find(11);
          auto const nodeID_12_u = tree.Find(12);
          auto const& nodes = tree.GetNodes();
          auto const entitiesInBFS = tree.GetEntitiesBreadthFirst();
          auto const entitiesInDFS = tree.GetEntitiesDepthFirst();

          Assert::AreEqual<OctreePoint::NodeID>(nodeID_10_u, 8184);
          Assert::AreEqual<OctreePoint::NodeID>(nodeID_11_u, 33538048);
          Assert::AreEqual<OctreePoint::NodeID>(nodeID_12_u, 8184);
          Assert::AreEqual<std::size_t>(14, nodes.size());
          Assert::IsTrue(entitiesInBFS == std::vector<EntityID>{ 6, 0, 1, 3, 2, 4, 5, 7, 9, 10, 12, 11, 13, 8 });
          Assert::IsTrue(entitiesInDFS == std::vector<EntityID>{ 6, 0, 1, 3, 2, 8, 4, 5, 7, 9, 10, 12, 11, 13 });
        }

        points.emplace_back(Point3D{ -2.0, -2.0, -2.0 });
        tree.Insert(pointNo, points.back(), points);
        auto const nodeID_13 = tree.Find(pointNo);
        Assert::AreEqual<OctreePoint::NodeID>(nodeID_13, 18612224);
        Assert::AreEqual<depth_t>(8, SI3D::GetDepthID(nodeID_13));
        ++pointNo;
      }

      // InsertUnique
      {
        auto const p1 = Point3D{ +3.75, +3.75, +3.76 };
        auto const isP1Inserted = tree.InsertUnique(p1, points, 0.01);
        Assert::IsFalse(isP1Inserted);

        auto const p2 = Point3D{ +3.75, +3.75, +3.75 };
        auto const isP2Inserted = tree.InsertUnique(p2, points, 0.0);
        Assert::IsTrue(isP2Inserted);
        if (isP2Inserted)
        {
          points.emplace_back(p2);
          ++pointNo;
        }

        auto const p3 = Point3D{ +3.75, +3.75, +3.77 };
        auto const isP3Inserted = tree.InsertUnique(p3, points, 0.001);
        Assert::IsTrue(isP3Inserted);
        if (isP3Inserted)
        {
          points.emplace_back(p3);
          ++pointNo;
        }
      }

      tree.Update(2, Point3D{ +2.0, +2.2, +2.0 });
      auto const nodeID_2_u1 = tree.Find(2);

      points[2] = { +2.0, +2.0, +2.6 };
      tree.Update(2, points[2]);
      auto const nodeID_2_u2 = tree.Find(2);
      Assert::AreEqual<OctreePoint::NodeID>(nodeID_2_u1, 65024);
      Assert::AreEqual<OctreePoint::NodeID>(nodeID_2_u2, 8132);
      Assert::IsFalse(tree.HasNode(520194));

      points[6] = { -2.0, -2.0, -2.0 };
      tree.Update(6, points[6]);
      auto const nodeID_6_u1 = tree.Find(6);

      auto const oldbox2 = points[2];
      points[2] = { -2.0, -2.5, -2.0 };
      tree.Update(2, oldbox2, points[2]);
      auto const nodeID_2_u3 = tree.Find(2);

      auto const oldbox4 = points[4];
      points[4] = { +3.50, +3.50, +3.50 };
      tree.Update(4, oldbox4, points[4], points); // It should move and erase 1016
      auto const nodeID_4_u1 = tree.Find(4);
      auto const nodeID_9_u1 = tree.Find(9);
      {
        points[8] = { +3.0, +3.0, +3.75 };
        tree.Update(8, points[8], points);
        auto const nodeID_8_u1 = tree.Find(8);

        auto const& nodes = tree.GetNodes();
        auto const entitiesInBFS = tree.GetEntitiesBreadthFirst();
        auto const entitiesInDFS = tree.GetEntitiesDepthFirst();

        Assert::AreEqual<OctreePoint::NodeID>(nodeID_6_u1, 18612224);
        Assert::AreEqual<OctreePoint::NodeID>(nodeID_2_u3, 69);
        Assert::AreEqual<OctreePoint::NodeID>(nodeID_4_u1, 8191);
        Assert::AreEqual<OctreePoint::NodeID>(nodeID_9_u1, 8184);
        Assert::AreEqual<OctreePoint::NodeID>(nodeID_8_u1, 8188);

        Assert::AreEqual<std::size_t>(14, nodes.size());
        Assert::IsTrue(entitiesInBFS == std::vector<EntityID>{ 1, 2, 0, 14, 6, 3, 5, 7, 9, 10, 12, 8, 4, 11, 13, 15, 16 });
        Assert::IsTrue(entitiesInDFS == std::vector<EntityID>{ 2, 0, 14, 6, 1, 3, 5, 7, 9, 10, 12, 8, 11, 4, 13, 15, 16 });
      }
    } // namespace Tree3DTest
  };

  TEST_CLASS(Box_InsertTest)
  {
    TEST_METHOD(CreateWithData)
    { // This gives a tree with 9 nodes.
      std::vector<BoundingBox3D> boxes = {
        { { -2.00375, -0.20375, +0.19625 }, { -1.52125, -0.19625, +0.20375 } },
        { { -0.88692, -1.05210, -0.80026 }, { +0.88692, +0.72175, +0.97359 } },
        { { -0.78692, -1.05210, -0.80026 }, { +0.98692, +0.72175, +0.97359 } },
        { { -0.68692, -1.05210, -0.80026 }, { +1.08692, +0.72175, +0.97359 } },
        { { -0.58692, -1.05210, -0.80026 }, { +1.18692, +0.72175, +0.97359 } },
      };

      OctreeBox tree(
        boxes,
        8,
        BoundingBox3D{
          { -10, -10, -10 },
          { +10, +10, +10 }
      },
        2);
      tree.UpdateIndexes({});
    }


    TEST_METHOD(AddToEmptyTree)
    {
      std::vector<BoundingBox3D> boxes0;
      OctreeBox tree(
        boxes0,
        21,
        BoundingBox3D{
          { -10, -10, -10 },
          { +10, +10, +10 }
      },
        2);

      // This gives a tree with 10 nodes.
      std::vector<BoundingBox3D> boxes = {
        { { -2.00375, -0.20375, +0.19625 }, { -1.52125, -0.19625, +0.20375 } },
        { { -0.88692, -1.05210, -0.80026 }, { +0.88692, +0.72175, +0.97359 } },
        { { -0.78692, -1.05210, -0.80026 }, { +0.98692, +0.72175, +0.97359 } },
        { { -0.68692, -1.05210, -0.80026 }, { +1.08692, +0.72175, +0.97359 } },
        { { -0.58692, -1.05210, -0.80026 }, { +1.18692, +0.72175, +0.97359 } },
      };

      for (EntityID i = 0; i < boxes.size(); ++i)
      {
        Assert::IsTrue(tree.InsertIntoLeaf(i, boxes[i], InsertionMode::LowestLeaf));
        boxes0.emplace_back(boxes[i]);
      }
      tree.UpdateIndexes({});
    }

    TEST_METHOD(CreateWithDataThenInsert)
    {
      // This gives a tree with 9 nodes.
      std::vector<BoundingBox3D> boxes = {
        { { -2.0, -2.0, -2.0 }, { -1.0, -1.0, +2.0 } }, // split to 8 and 12
        { { +1.0, +1.0, +1.0 }, { +4.0, +3.1, +3.4 } }, // 15
        { { +2.0, +2.0, +2.0 }, { +3.1, +3.1, +3.1 } }, // 127
      };
      auto boxNo = EntityID(boxes.size());

      OctreeBox tree(
        boxes,
        8,
        BoundingBox3D{
          { -4, -4, -4 },
          { +4, +4, +4 }
      },
        1);

      {
        auto const& nodes = tree.GetNodes();
        auto const entitiesInBFS = tree.GetEntitiesBreadthFirst();
        auto const entitiesInDFS = tree.GetEntitiesDepthFirst();

        Assert::AreEqual<std::size_t>(4, nodes.size());
        Assert::IsTrue(AreContainersItemsEqual(std::vector<EntityID>{ 0, 1, 2 }, entitiesInBFS));
        Assert::IsTrue(AreContainersItemsEqual(std::vector<EntityID>{ 0, 1, 2 }, entitiesInDFS));
      }

      tree.UpdateIndexes({});

      boxes.emplace_back(
        BoundingBox3D{
          { +3.0, +3.0, +3.0 },
          { +5.0, +4.0, +4.0 }
      });
      auto const isOutsiderInserted = tree.InsertIntoLeaf(boxNo, boxes.back(), InsertionMode::ExistingLeaf);
      ++boxNo;

      Assert::IsTrue(isOutsiderInserted);

      {
        boxes.emplace_back(
          BoundingBox3D{
            { +3.0, +3.0, +3.0 },
            { +4.0, +4.0, +4.0 }
        });
        tree.InsertIntoLeaf(boxNo, boxes.back(), InsertionMode::ExistingLeaf);
        ++boxNo;

        auto const& nodes = tree.GetNodes();
        auto const entitiesInBFS = tree.GetEntitiesBreadthFirst();
        auto const entitiesInDFS = tree.GetEntitiesDepthFirst();

        Assert::AreEqual<std::size_t>(4, nodes.size());
        Assert::IsTrue(AreContainersItemsEqual(std::vector<EntityID>{ 3, 0, 1, 2, 4 }, entitiesInBFS));
        Assert::IsTrue(AreContainersItemsEqual(std::vector<EntityID>{ 3, 0, 1, 2, 4 }, entitiesInDFS));
      }

      {
        boxes.emplace_back(
          BoundingBox3D{
            { +2.0, +2.0, +2.0 },
            { +3.0, +3.0, +3.0 }
        });
        tree.InsertIntoLeaf(boxNo, boxes.back(), InsertionMode::LowestLeaf);

        auto const& nodes = tree.GetNodes();
        auto const entitiesInBFS = tree.GetEntitiesBreadthFirst();
        auto const entitiesInDFS = tree.GetEntitiesDepthFirst();

        Assert::AreEqual<std::size_t>(5, nodes.size());
        Assert::IsTrue(AreContainersItemsEqual(std::vector<EntityID>{ 3, 0, 1, 2, 4, 5 }, entitiesInBFS));
        Assert::IsTrue(AreContainersItemsEqual(std::vector<EntityID>{ 3, 0, 1, 2, 4, 5 }, entitiesInDFS));

        ++boxNo;
      }

      boxes.emplace_back(
        BoundingBox3D{
          { +3.0, +3.0, +3.0 },
          { +4.0, +4.0, +4.0 }
      });
      tree.InsertIntoLeaf(boxNo, boxes.back(), InsertionMode::ExistingLeaf);
      ++boxNo;

      // InsertWithRebalancing
      {
        boxes.emplace_back(
          BoundingBox3D{
            { +3.0, +3.0, +3.0 },
            { +3.5, +3.5, +3.5 }
        });
        tree.InsertIntoLeaf(boxNo, boxes.back(), InsertionMode::ExistingLeaf); // 6
        ++boxNo;

        boxes.emplace_back(
          BoundingBox3D{
            { +3.0, +3.0, +3.5 },
            { +3.5, +3.5, +4.0 }
        });
        tree.InsertIntoLeaf(boxNo, boxes.back(), InsertionMode::ExistingLeaf); // 7
        ++boxNo;
        {
          boxes.emplace_back(
            BoundingBox3D{
              { +3.0, +3.0, +3.1 },
              { +3.5, +3.5, +3.2 }
          });
          tree.InsertIntoLeaf(boxNo, boxes.back(), InsertionMode::ExistingLeaf); // 8
          ++boxNo;

          auto const& nodes = tree.GetNodes();
          auto const entitiesInBFS = tree.GetEntitiesBreadthFirst();
          auto const entitiesInDFS = tree.GetEntitiesDepthFirst();

          Assert::AreEqual<std::size_t>(6, nodes.size());
          Assert::IsTrue(AreContainersItemsEqual(std::vector<EntityID>{ 3, 0, 1, 2, 4, 5, 6, 7, 8, 9 }, entitiesInBFS));
          Assert::IsTrue(AreContainersItemsEqual(std::vector<EntityID>{ 3, 0, 1, 2, 4, 5, 6, 7, 8, 9 }, entitiesInDFS));
        }

        {
          boxes.emplace_back(
            BoundingBox3D{
              { +3.75, +3.75, +3.75 },
              {  +4.0,  +4.0,  +4.0 }
          }); // [65535] -> newly creates 8191
          tree.Insert(boxNo, boxes.back(), boxes);              // 9
          ++boxNo;

          auto const& nodes = tree.GetNodes();
          auto const entitiesInBFS = tree.GetEntitiesBreadthFirst();
          auto const entitiesInDFS = tree.GetEntitiesDepthFirst();

          Assert::AreEqual<std::size_t>(9, nodes.size());
          Assert::IsTrue(AreContainersItemsEqual(std::vector<EntityID>{ 3, 0, 1, 2, 4, 5, 6, 7, 9, 8, 10 }, entitiesInBFS));
          Assert::IsTrue(AreContainersItemsEqual(std::vector<EntityID>{ 3, 0, 1, 2, 4, 5, 6, 7, 9, 8, 10 }, entitiesInDFS));
        }

        {
          boxes.emplace_back(
            BoundingBox3D{
              { -2.0, -2.0, -2.0 },
              { -1.0, -1.0, -1.0 }
          });
          tree.Insert(boxNo, boxes.back(), boxes); // 10
          ++boxNo;

          auto const& nodes = tree.GetNodes();
          auto const entitiesInBFS = tree.GetEntitiesBreadthFirst();
          auto const entitiesInDFS = tree.GetEntitiesDepthFirst();

          Assert::AreEqual<std::size_t>(10, nodes.size());
          Assert::IsTrue(AreContainersItemsEqual(std::vector<EntityID>{ 3, 11, 0, 1, 2, 4, 5, 6, 7, 9, 8, 10 }, entitiesInBFS));
          Assert::IsTrue(AreContainersItemsEqual(std::vector<EntityID>{ 3, 11, 0, 1, 2, 4, 5, 6, 7, 9, 8, 10 }, entitiesInDFS));
        }
      }

      tree.Update(
        2,
        {
          { +2.0, +2.0, +2.0 },
          { +3.2, +3.2, +3.1 }
      });

      boxes[2] = {
        { +2.0, +2.0, +2.0 },
        { +2.6, +2.6, +2.2 }
      };
      tree.Update(2, boxes[2]); // 1016

      boxes[6] = {
        { -2.0, -2.0, -2.0 },
        { +2.0, +2.0, +2.0 }
      };
      tree.Update(6, boxes[6]); // 1

      auto const oldbox2 = boxes[2];
      boxes[2] = {
        { -2.0, -2.0, -2.0 },
        { +0.0, +0.0, +0.0 }
      };
      tree.Update(2, oldbox2, boxes[2]);

      auto const oldbox4 = boxes[4];
      boxes[4] = {
        { +3.50, +3.50, +3.50 },
        { +3.75, +3.75, +3.75 }
      };
      tree.Update(4, oldbox4, boxes[4], boxes); // It should move and erase 1016

      boxes[8] = {
        {  +3.0,  +3.0, +3.75 },
        { +3.25, +3.25,  +4.0 }
      };
      tree.Update(8, boxes[8], boxes);

      auto const& nodes = tree.GetNodes();
      auto const entitiesInBFS = tree.GetEntitiesBreadthFirst();
      auto const entitiesInDFS = tree.GetEntitiesDepthFirst();

      Assert::AreEqual<std::size_t>(11, nodes.size());
      Assert::IsTrue(entitiesInBFS == std::vector<EntityID>{ 3, 11, 2, 0, 1, 6, 5, 7, 9, 8, 4, 10 });
      Assert::IsTrue(entitiesInDFS == std::vector<EntityID>{ 3, 11, 2, 0, 1, 6, 5, 7, 9, 8, 4, 10 });
    }
  };
} // namespace Tree3DTest


namespace LongIntAdaptor
{
  using namespace OrthoTree;

  using GeometryType = long int;
  template<dim_t N>
  using CustomVectorTypeND = std::array<GeometryType, N>;
  template<dim_t N>
  using CustomBoundingBoxND = std::array<CustomVectorTypeND<N>, 2>;
  template<dim_t N>
  using CustomRayND = std::array<CustomVectorTypeND<N>, 2>;
  template<dim_t N>
  using CustomPlaneND = std::tuple<CustomVectorTypeND<N>, GeometryType>;

  template<dim_t N>
  struct CustomBaseGeometryAdapter
  {
    using Scalar = GeometryType;
    using FloatScalar = float;
    using Vector = CustomVectorTypeND<N>;
    using Box = CustomBoundingBoxND<N>;
    using Ray = CustomRayND<N>;
    using Plane = CustomPlaneND<N>;

    static constexpr dim_t DIMENSION_NO = N;
    static constexpr FloatScalar BASE_TOLERANCE = std::numeric_limits<FloatScalar>::epsilon() * FloatScalar(10);

    static constexpr Scalar GetPointC(Vector const& pt, OrthoTree::dim_t iDimension) { return pt[iDimension]; }

    static constexpr void SetPointC(Vector& pt, OrthoTree::dim_t iDimension, Scalar value) { pt[iDimension] = value; }

    static constexpr void SetBoxMinC(Box& box, dim_t dimensionID, Scalar value) { SetPointC(box[0], dimensionID, value); }
    static constexpr void SetBoxMaxC(Box& box, dim_t dimensionID, Scalar value) { SetPointC(box[1], dimensionID, value); }
    static constexpr Scalar GetBoxMinC(Box const& box, dim_t dimensionID) { return GetPointC(box[0], dimensionID); }
    static constexpr Scalar GetBoxMaxC(Box const& box, dim_t dimensionID) { return GetPointC(box[1], dimensionID); }

    static constexpr Vector const& GetRayDirection(Ray const& ray) noexcept { ray[1]; }
    static constexpr Vector const& GetRayOrigin(Ray const& ray) noexcept { ray[0]; }

    static constexpr Vector const& GetPlaneNormal(Plane const& plane) noexcept { return std::get<0>(plane); }
    static constexpr Scalar GetPlaneOrigoDistance(Plane const& plane) noexcept { return std::get<1>(plane); }
  };

  template<dim_t N>
  using CustomGeometryAdapter = GeneralGeometryAdapter<CustomBaseGeometryAdapter<N>>;
  template<dim_t N>
  using OrthoTreePointCustom = OrthoTreeBase<PointEntitySpanAdapter<CustomVectorTypeND<N>>, CustomGeometryAdapter<N>, PointConfiguration<>>;
  template<dim_t N>
  using OrthoTreePointContainerCustom = OrthoTree::OrthoTreeManaged<OrthoTreePointCustom<N>>;
  template<dim_t N, bool IS_LOOSE_TREE = true>
  using OrthoTreeBoxCustom = OrthoTreeBase<BoxEntitySpanAdapter<CustomBoundingBoxND<N>>, CustomGeometryAdapter<N>, BoxConfiguration<IS_LOOSE_TREE>>;
  template<dim_t N>
  using OrthoTreeBoxContainerCustom = OrthoTree::OrthoTreeManaged<OrthoTreeBoxCustom<N>>;


  TEST_CLASS(LongIntTest)
  {
    TEST_METHOD(RangeSearchPointAtTheBorder)
    {
      auto constexpr nDim = 1;
      using Vector = CustomVectorTypeND<nDim>;
      using Box = CustomBoundingBoxND<nDim>;

      auto constexpr points = std::array{ Vector{ 0 }, Vector{ 4 }, Vector{ 5 }, Vector{ 8 } };

      auto const tree = OrthoTreePointContainerCustom<nDim>(points, 3, std::nullopt, 2);

      {
        auto const vidActual = tree.RangeSearch(Box{ points[1], points[2] }, RangeSearchMode::Inside, 0.0f);
        auto constexpr vidExpected = std::array<EntityID, 2>{ 1, 2 };
        Assert::IsTrue(std::ranges::is_permutation(vidActual, vidExpected));
      }

      {
        auto const vidActual = tree.RangeSearch(Box{ points[2], Vector{ 6 } }, RangeSearchMode::Inside, 0.0f);
        auto constexpr vidExpected = std::array<EntityID, 1>{ 2 };
        Assert::IsTrue(std::ranges::is_permutation(vidActual, vidExpected));
      }
    } // namespace LongIntAdaptor

    TEST_METHOD(RangeSearchBoxAtTheBorder)
    {
      auto constexpr nDim = 1;
      using Box = CustomBoundingBoxND<nDim>;

      auto const boxes = array<Box, 5>{
        Box{ 0, 1 },
        Box{ 3, 4 },
        Box{ 4, 5 },
        Box{ 5, 6 },
        Box{ 7, 8 }
      };

      auto const tree = OrthoTreeBoxContainerCustom<nDim>(boxes, 3, std::nullopt, 2, false);
      {
        auto const vidActual = tree.RangeSearch(boxes[1], RangeSearchMode::Inside, 0.0f);
        auto constexpr vidExpected = array<EntityID, 1>{ 1 };
        Assert::IsTrue(std::ranges::is_permutation(vidActual, vidExpected));
      }

      {
        auto const vidActual = tree.RangeSearch(boxes[2], RangeSearchMode::Inside, 0.0f);
        auto constexpr vidExpected = array<EntityID, 1>{ 2 };
        Assert::IsTrue(std::ranges::is_permutation(vidActual, vidExpected));
      }

      {
        auto const vidActual = tree.RangeSearch(CustomBoundingBoxND<nDim>{ 4, 8 }, RangeSearchMode::Inside, 0.0f);
        auto constexpr vidExpected = array<EntityID, 3>{ 2, 3, 4 };
        Assert::IsTrue(std::ranges::is_permutation(vidActual, vidExpected));
      }
    }

    TEST_METHOD(PickSearchAtTheBorder)
    {
      auto constexpr nDim = 1;
      using Vector = CustomVectorTypeND<nDim>;
      using Box = CustomBoundingBoxND<nDim>;

      auto const boxes = array<Box, 5>{
        Box{ 0, 1 },
        Box{ 3, 4 },
        Box{ 4, 5 },
        Box{ 5, 6 },
        Box{ 7, 8 }
      };

      auto const tree = OrthoTreeBoxContainerCustom<nDim>(boxes, 3, std::nullopt, 2, false);
      {
        auto const vidActual = tree.PickSearch(Vector{ 4 });
        auto constexpr vidExpected = array<EntityID, 2>{ 1, 2 };
        Assert::IsTrue(std::ranges::is_permutation(vidActual, vidExpected));
      }
    }

    TEST_METHOD(PickSearchAtTheBorder3LevelPos)
    {
      auto constexpr nDim = 1;
      using Vector = CustomVectorTypeND<nDim>;
      using Box = CustomBoundingBoxND<nDim>;

      auto const boxes = array{
        Box{ 0, 16 },

        Box{ 0,  1 },
        Box{ 3,  4 },
        Box{ 4,  5 },
        Box{ 5,  6 },

        Box{ 2,  4 },
        Box{ 4,  6 },
        Box{ 2,  6 },
      };

      auto const tree = OrthoTreeBoxContainerCustom<nDim>(boxes, 4, std::nullopt, 2, false);
      {
        auto const vidActual = tree.PickSearch(Vector{ 4 });
        auto constexpr vidExpected = array<EntityID, 6>{ 0, 2, 3, 5, 6, 7 };
        Assert::IsTrue(std::ranges::is_permutation(vidActual, vidExpected));
      }
    }

    TEST_METHOD(PickSearchAtTheBorder3LevelPosNeg)
    {
      auto constexpr nDim = 1;
      using Vector = CustomVectorTypeND<nDim>;
      using Box = CustomBoundingBoxND<nDim>;

      auto const boxes = array{
        Box{ -8,  8 },

        Box{  0,  1 },
        Box{ -4, -3 },
        Box{  4,  5 },
        Box{ -6, -5 },

        Box{ -4,  2 },
        Box{  4,  6 },
        Box{ -6,  2 },
      };

      auto const tree = OrthoTreeBoxContainerCustom<nDim>(boxes, 4, std::nullopt, 2, false);
      {
        auto const vidActual = tree.PickSearch(Vector{ -4 });
        auto constexpr vidExpected = array<EntityID, 4>{ 0, 2, 5, 7 };
        Assert::IsTrue(std::ranges::is_permutation(vidActual, vidExpected));
      }
    }

    TEST_METHOD(PickSearchAtTheBorder3LevelNeg)
    {
      auto constexpr nDim = 1;
      using Vector = CustomVectorTypeND<nDim>;
      using Box = CustomBoundingBoxND<nDim>;

      auto const boxes = array{
        Box{ -17, -1 },

        Box{  -2, -1 },
        Box{  -4, -3 },
        Box{  -5, -4 },
        Box{  -6, -5 },

        Box{  -4, -2 },
        Box{  -6, -4 },
        Box{  -6, -2 },
      };

      auto const tree = OrthoTreeBoxContainerCustom<nDim>(boxes, 4, std::nullopt, 2, false);
      {
        auto const vidActual = tree.PickSearch(Vector{ -4 });
        auto constexpr vidExpected = array<EntityID, 6>{ 0, 2, 3, 5, 6, 7 };
        Assert::IsTrue(std::ranges::is_permutation(vidActual, vidExpected));
      }
    }


    TEST_METHOD(PickSearch_Issue8)
    {
      auto constexpr nDim = 3;

      using Vector = CustomVectorTypeND<nDim>;
      using Box = CustomBoundingBoxND<nDim>;

      auto mGridCells = array<Box, 144>{};
      mGridCells[0] = Box{
        Vector{ -1024, 0, 1024 },
        Vector{  -961, 0, 1087 }
      };
      mGridCells[1] = Box{
        Vector{ -1024, 0, 1088 },
        Vector{  -961, 0, 1151 }
      };
      mGridCells[2] = Box{
        Vector{ -1024, 0, 1152 },
        Vector{  -961, 0, 1215 }
      };
      mGridCells[3] = Box{
        Vector{ -1024, 0, 1216 },
        Vector{  -961, 0, 1279 }
      };
      mGridCells[4] = Box{
        Vector{ -1024, 0, 1280 },
        Vector{  -961, 0, 1343 }
      };
      mGridCells[5] = Box{
        Vector{ -1024, 0, 1344 },
        Vector{  -961, 0, 1407 }
      };
      mGridCells[6] = Box{
        Vector{ -1024, 0, 1408 },
        Vector{  -961, 0, 1471 }
      };
      mGridCells[7] = Box{
        Vector{ -1024, 0, 1472 },
        Vector{  -961, 0, 1535 }
      };
      mGridCells[8] = Box{
        Vector{ -960, 0, 1024 },
        Vector{ -897, 0, 1087 }
      };
      mGridCells[9] = Box{
        Vector{ -960, 0, 1088 },
        Vector{ -897, 0, 1151 }
      };
      mGridCells[10] = Box{
        Vector{ -960, 0, 1152 },
        Vector{ -897, 0, 1215 }
      };
      mGridCells[11] = Box{
        Vector{ -960, 0, 1216 },
        Vector{ -897, 0, 1279 }
      };
      mGridCells[12] = Box{
        Vector{ -960, 0, 1280 },
        Vector{ -897, 0, 1343 }
      };
      mGridCells[13] = Box{
        Vector{ -960, 0, 1344 },
        Vector{ -897, 0, 1407 }
      };
      mGridCells[14] = Box{
        Vector{ -960, 0, 1408 },
        Vector{ -897, 0, 1471 }
      };
      mGridCells[15] = Box{
        Vector{ -960, 0, 1472 },
        Vector{ -897, 0, 1535 }
      };
      mGridCells[16] = Box{
        Vector{ -896, 0, 1024 },
        Vector{ -833, 0, 1087 }
      };
      mGridCells[17] = Box{
        Vector{ -896, 0, 1088 },
        Vector{ -833, 0, 1151 }
      };
      mGridCells[18] = Box{
        Vector{ -896, 0, 1152 },
        Vector{ -833, 0, 1215 }
      };
      mGridCells[19] = Box{
        Vector{ -896, 0, 1216 },
        Vector{ -833, 0, 1279 }
      };
      mGridCells[20] = Box{
        Vector{ -896, 0, 1280 },
        Vector{ -833, 0, 1343 }
      };
      mGridCells[21] = Box{
        Vector{ -896, 0, 1344 },
        Vector{ -833, 0, 1407 }
      };
      mGridCells[22] = Box{
        Vector{ -896, 0, 1408 },
        Vector{ -833, 0, 1471 }
      };
      mGridCells[23] = Box{
        Vector{ -896, 0, 1472 },
        Vector{ -833, 0, 1535 }
      };
      mGridCells[24] = Box{
        Vector{ -832, 0, 1024 },
        Vector{ -769, 0, 1087 }
      };
      mGridCells[25] = Box{
        Vector{ -832, 0, 1088 },
        Vector{ -769, 0, 1151 }
      };
      mGridCells[26] = Box{
        Vector{ -832, 0, 1152 },
        Vector{ -769, 0, 1215 }
      };
      mGridCells[27] = Box{
        Vector{ -832, 0, 1216 },
        Vector{ -769, 0, 1279 }
      };
      mGridCells[28] = Box{
        Vector{ -832, 0, 1280 },
        Vector{ -769, 0, 1343 }
      };
      mGridCells[29] = Box{
        Vector{ -832, 0, 1344 },
        Vector{ -769, 0, 1407 }
      };
      mGridCells[30] = Box{
        Vector{ -832, 0, 1408 },
        Vector{ -769, 0, 1471 }
      };
      mGridCells[31] = Box{
        Vector{ -832, 0, 1472 },
        Vector{ -769, 0, 1535 }
      };
      mGridCells[32] = Box{
        Vector{ -768, 0, 1024 },
        Vector{ -705, 0, 1087 }
      };
      mGridCells[33] = Box{
        Vector{ -768, 0, 1088 },
        Vector{ -705, 0, 1151 }
      };
      mGridCells[34] = Box{
        Vector{ -768, 0, 1152 },
        Vector{ -705, 0, 1215 }
      };
      mGridCells[35] = Box{
        Vector{ -768, 0, 1216 },
        Vector{ -705, 0, 1279 }
      };
      mGridCells[36] = Box{
        Vector{ -768, 0, 1280 },
        Vector{ -705, 0, 1343 }
      };
      mGridCells[37] = Box{
        Vector{ -768, 0, 1344 },
        Vector{ -705, 0, 1407 }
      };
      mGridCells[38] = Box{
        Vector{ -768, 0, 1408 },
        Vector{ -705, 0, 1471 }
      };
      mGridCells[39] = Box{
        Vector{ -768, 0, 1472 },
        Vector{ -705, 0, 1535 }
      };
      mGridCells[40] = Box{
        Vector{ -704, 0, 1024 },
        Vector{ -641, 0, 1087 }
      };
      mGridCells[41] = Box{
        Vector{ -704, 0, 1088 },
        Vector{ -641, 0, 1151 }
      };
      mGridCells[42] = Box{
        Vector{ -704, 0, 1152 },
        Vector{ -641, 0, 1215 }
      };
      mGridCells[43] = Box{
        Vector{ -704, 0, 1216 },
        Vector{ -641, 0, 1279 }
      };
      mGridCells[44] = Box{
        Vector{ -704, 0, 1280 },
        Vector{ -641, 0, 1343 }
      };
      mGridCells[45] = Box{
        Vector{ -704, 0, 1344 },
        Vector{ -641, 0, 1407 }
      };
      mGridCells[46] = Box{
        Vector{ -704, 0, 1408 },
        Vector{ -641, 0, 1471 }
      };
      mGridCells[47] = Box{
        Vector{ -704, 0, 1472 },
        Vector{ -641, 0, 1535 }
      };
      mGridCells[48] = Box{
        Vector{ -640, 0, 1024 },
        Vector{ -577, 0, 1087 }
      };
      mGridCells[49] = Box{
        Vector{ -640, 0, 1088 },
        Vector{ -577, 0, 1151 }
      };
      mGridCells[50] = Box{
        Vector{ -640, 0, 1152 },
        Vector{ -577, 0, 1215 }
      };
      mGridCells[51] = Box{
        Vector{ -640, 0, 1216 },
        Vector{ -577, 0, 1279 }
      };
      mGridCells[52] = Box{
        Vector{ -640, 0, 1280 },
        Vector{ -577, 0, 1343 }
      };
      mGridCells[53] = Box{
        Vector{ -640, 0, 1344 },
        Vector{ -577, 0, 1407 }
      };
      mGridCells[54] = Box{
        Vector{ -640, 0, 1408 },
        Vector{ -577, 0, 1471 }
      };
      mGridCells[55] = Box{
        Vector{ -640, 0, 1472 },
        Vector{ -577, 0, 1535 }
      };
      mGridCells[56] = Box{
        Vector{ -576, 0, 1024 },
        Vector{ -513, 0, 1087 }
      };
      mGridCells[57] = Box{
        Vector{ -576, 0, 1088 },
        Vector{ -513, 0, 1151 }
      };
      mGridCells[58] = Box{
        Vector{ -576, 0, 1152 },
        Vector{ -513, 0, 1215 }
      };
      mGridCells[59] = Box{
        Vector{ -576, 0, 1216 },
        Vector{ -513, 0, 1279 }
      };
      mGridCells[60] = Box{
        Vector{ -576, 0, 1280 },
        Vector{ -513, 0, 1343 }
      };
      mGridCells[61] = Box{
        Vector{ -576, 0, 1344 },
        Vector{ -513, 0, 1407 }
      };
      mGridCells[62] = Box{
        Vector{ -576, 0, 1408 },
        Vector{ -513, 0, 1471 }
      };
      mGridCells[63] = Box{
        Vector{ -576, 0, 1472 },
        Vector{ -513, 0, 1535 }
      };
      mGridCells[64] = Box{
        Vector{ -1152, 0, 896 },
        Vector{ -1089, 0, 959 }
      };
      mGridCells[65] = Box{
        Vector{ -1152, 0,  960 },
        Vector{ -1089, 0, 1023 }
      };
      mGridCells[66] = Box{
        Vector{ -1152, 0, 1024 },
        Vector{ -1089, 0, 1087 }
      };
      mGridCells[67] = Box{
        Vector{ -1152, 0, 1088 },
        Vector{ -1089, 0, 1151 }
      };
      mGridCells[68] = Box{
        Vector{ -1152, 0, 1152 },
        Vector{ -1089, 0, 1215 }
      };
      mGridCells[69] = Box{
        Vector{ -1152, 0, 1216 },
        Vector{ -1089, 0, 1279 }
      };
      mGridCells[70] = Box{
        Vector{ -1152, 0, 1280 },
        Vector{ -1089, 0, 1343 }
      };
      mGridCells[71] = Box{
        Vector{ -1152, 0, 1344 },
        Vector{ -1089, 0, 1407 }
      };
      mGridCells[72] = Box{
        Vector{ -1152, 0, 1408 },
        Vector{ -1089, 0, 1471 }
      };
      mGridCells[73] = Box{
        Vector{ -1152, 0, 1472 },
        Vector{ -1089, 0, 1535 }
      };
      mGridCells[74] = Box{
        Vector{ -1152, 0, 1536 },
        Vector{ -1089, 0, 1599 }
      };
      mGridCells[75] = Box{
        Vector{ -1152, 0, 1600 },
        Vector{ -1089, 0, 1663 }
      };
      mGridCells[76] = Box{
        Vector{ -1088, 0, 896 },
        Vector{ -1025, 0, 959 }
      };
      mGridCells[77] = Box{
        Vector{ -1088, 0,  960 },
        Vector{ -1025, 0, 1023 }
      };
      mGridCells[78] = Box{
        Vector{ -1088, 0, 1024 },
        Vector{ -1025, 0, 1087 }
      };
      mGridCells[79] = Box{
        Vector{ -1088, 0, 1088 },
        Vector{ -1025, 0, 1151 }
      };
      mGridCells[80] = Box{
        Vector{ -1088, 0, 1152 },
        Vector{ -1025, 0, 1215 }
      };
      mGridCells[81] = Box{
        Vector{ -1088, 0, 1216 },
        Vector{ -1025, 0, 1279 }
      };
      mGridCells[82] = Box{
        Vector{ -1088, 0, 1280 },
        Vector{ -1025, 0, 1343 }
      };
      mGridCells[83] = Box{
        Vector{ -1088, 0, 1344 },
        Vector{ -1025, 0, 1407 }
      };
      mGridCells[84] = Box{
        Vector{ -1088, 0, 1408 },
        Vector{ -1025, 0, 1471 }
      };
      mGridCells[85] = Box{
        Vector{ -1088, 0, 1472 },
        Vector{ -1025, 0, 1535 }
      };
      mGridCells[86] = Box{
        Vector{ -1088, 0, 1536 },
        Vector{ -1025, 0, 1599 }
      };
      mGridCells[87] = Box{
        Vector{ -1088, 0, 1600 },
        Vector{ -1025, 0, 1663 }
      };
      mGridCells[88] = Box{
        Vector{ -512, 0, 896 },
        Vector{ -449, 0, 959 }
      };
      mGridCells[89] = Box{
        Vector{ -512, 0,  960 },
        Vector{ -449, 0, 1023 }
      };
      mGridCells[90] = Box{
        Vector{ -512, 0, 1024 },
        Vector{ -449, 0, 1087 }
      };
      mGridCells[91] = Box{
        Vector{ -512, 0, 1088 },
        Vector{ -449, 0, 1151 }
      };
      mGridCells[92] = Box{
        Vector{ -512, 0, 1152 },
        Vector{ -449, 0, 1215 }
      };
      mGridCells[93] = Box{
        Vector{ -512, 0, 1216 },
        Vector{ -449, 0, 1279 }
      };
      mGridCells[94] = Box{
        Vector{ -512, 0, 1280 },
        Vector{ -449, 0, 1343 }
      };
      mGridCells[95] = Box{
        Vector{ -512, 0, 1344 },
        Vector{ -449, 0, 1407 }
      };
      mGridCells[96] = Box{
        Vector{ -512, 0, 1408 },
        Vector{ -449, 0, 1471 }
      };
      mGridCells[97] = Box{
        Vector{ -512, 0, 1472 },
        Vector{ -449, 0, 1535 }
      };
      mGridCells[98] = Box{
        Vector{ -512, 0, 1536 },
        Vector{ -449, 0, 1599 }
      };
      mGridCells[99] = Box{
        Vector{ -512, 0, 1600 },
        Vector{ -449, 0, 1663 }
      };
      mGridCells[100] = Box{
        Vector{ -448, 0, 896 },
        Vector{ -385, 0, 959 }
      };
      mGridCells[101] = Box{
        Vector{ -448, 0,  960 },
        Vector{ -385, 0, 1023 }
      };
      mGridCells[102] = Box{
        Vector{ -448, 0, 1024 },
        Vector{ -385, 0, 1087 }
      };
      mGridCells[103] = Box{
        Vector{ -448, 0, 1088 },
        Vector{ -385, 0, 1151 }
      };
      mGridCells[104] = Box{
        Vector{ -448, 0, 1152 },
        Vector{ -385, 0, 1215 }
      };
      mGridCells[105] = Box{
        Vector{ -448, 0, 1216 },
        Vector{ -385, 0, 1279 }
      };
      mGridCells[106] = Box{
        Vector{ -448, 0, 1280 },
        Vector{ -385, 0, 1343 }
      };
      mGridCells[107] = Box{
        Vector{ -448, 0, 1344 },
        Vector{ -385, 0, 1407 }
      };
      mGridCells[108] = Box{
        Vector{ -448, 0, 1408 },
        Vector{ -385, 0, 1471 }
      };
      mGridCells[109] = Box{
        Vector{ -448, 0, 1472 },
        Vector{ -385, 0, 1535 }
      };
      mGridCells[110] = Box{
        Vector{ -448, 0, 1536 },
        Vector{ -385, 0, 1599 }
      };
      mGridCells[111] = Box{
        Vector{ -448, 0, 1600 },
        Vector{ -385, 0, 1663 }
      };
      mGridCells[112] = Box{
        Vector{ -1024, 0, 896 },
        Vector{  -961, 0, 959 }
      };
      mGridCells[113] = Box{
        Vector{ -1024, 0,  960 },
        Vector{  -961, 0, 1023 }
      };
      mGridCells[114] = Box{
        Vector{ -960, 0, 896 },
        Vector{ -897, 0, 959 }
      };
      mGridCells[115] = Box{
        Vector{ -960, 0,  960 },
        Vector{ -897, 0, 1023 }
      };
      mGridCells[116] = Box{
        Vector{ -896, 0, 896 },
        Vector{ -833, 0, 959 }
      };
      mGridCells[117] = Box{
        Vector{ -896, 0,  960 },
        Vector{ -833, 0, 1023 }
      };
      mGridCells[118] = Box{
        Vector{ -832, 0, 896 },
        Vector{ -769, 0, 959 }
      };
      mGridCells[119] = Box{
        Vector{ -832, 0,  960 },
        Vector{ -769, 0, 1023 }
      };
      mGridCells[120] = Box{
        Vector{ -768, 0, 896 },
        Vector{ -705, 0, 959 }
      };
      mGridCells[121] = Box{
        Vector{ -768, 0,  960 },
        Vector{ -705, 0, 1023 }
      };
      mGridCells[122] = Box{
        Vector{ -704, 0, 896 },
        Vector{ -641, 0, 959 }
      };
      mGridCells[123] = Box{
        Vector{ -704, 0,  960 },
        Vector{ -641, 0, 1023 }
      };
      mGridCells[124] = Box{
        Vector{ -640, 0, 896 },
        Vector{ -577, 0, 959 }
      };
      mGridCells[125] = Box{
        Vector{ -640, 0,  960 },
        Vector{ -577, 0, 1023 }
      };
      mGridCells[126] = Box{
        Vector{ -576, 0, 896 },
        Vector{ -513, 0, 959 }
      };
      mGridCells[127] = Box{
        Vector{ -576, 0,  960 },
        Vector{ -513, 0, 1023 }
      };
      mGridCells[128] = Box{
        Vector{ -1024, 0, 1536 },
        Vector{  -961, 0, 1599 }
      };
      mGridCells[129] = Box{
        Vector{ -1024, 0, 1600 },
        Vector{  -961, 0, 1663 }
      };
      mGridCells[130] = Box{
        Vector{ -960, 0, 1536 },
        Vector{ -897, 0, 1599 }
      };
      mGridCells[131] = Box{
        Vector{ -960, 0, 1600 },
        Vector{ -897, 0, 1663 }
      };
      mGridCells[132] = Box{
        Vector{ -896, 0, 1536 },
        Vector{ -833, 0, 1599 }
      };
      mGridCells[133] = Box{
        Vector{ -896, 0, 1600 },
        Vector{ -833, 0, 1663 }
      };
      mGridCells[134] = Box{
        Vector{ -832, 0, 1536 },
        Vector{ -769, 0, 1599 }
      };
      mGridCells[135] = Box{
        Vector{ -832, 0, 1600 },
        Vector{ -769, 0, 1663 }
      };
      mGridCells[136] = Box{
        Vector{ -768, 0, 1536 },
        Vector{ -705, 0, 1599 }
      };
      mGridCells[137] = Box{
        Vector{ -768, 0, 1600 },
        Vector{ -705, 0, 1663 }
      };
      mGridCells[138] = Box{
        Vector{ -704, 0, 1536 },
        Vector{ -641, 0, 1599 }
      };
      mGridCells[139] = Box{
        Vector{ -704, 0, 1600 },
        Vector{ -641, 0, 1663 }
      };
      mGridCells[140] = Box{
        Vector{ -640, 0, 1536 },
        Vector{ -577, 0, 1599 }
      };
      mGridCells[141] = Box{
        Vector{ -640, 0, 1600 },
        Vector{ -577, 0, 1663 }
      };
      mGridCells[142] = Box{
        Vector{ -576, 0, 1536 },
        Vector{ -513, 0, 1599 }
      };
      mGridCells[143] = Box{
        Vector{ -576, 0, 1600 },
        Vector{ -513, 0, 1663 }
      };

      auto const tree = OrthoTreeBoxContainerCustom<nDim>(mGridCells, 3, std::nullopt, 21, true);
      auto const vidActual = tree.PickSearch(Vector{ -864, 0, 1471 }, 0.0);
      auto constexpr vidExpected = array<EntityID, 1>{ 22 };
      Assert::IsTrue(std::ranges::is_permutation(vidActual, vidExpected));
    }


    template<int nDim>
    vector<CustomVectorTypeND<nDim>> readPointCloud(std::filesystem::path const& path)
    {
      auto points = vector<CustomVectorTypeND<nDim>>{};
      auto file = std::ifstream(path, std::ios::in);
      if (file.fail())
        return points;

      auto line = std::string{};
      while (std::getline(file, line))
      {
        if (file.fail())
          return points;

        auto& point = points.emplace_back();
        auto sw = std::string_view(line);
        for (int iDim = 0; iDim < nDim; ++iDim)
        {
          auto const [ptr, ec] = std::from_chars(sw.data(), sw.data() + sw.length(), point[iDim]);
          if (ec != std::errc{})
            return points;

          sw.remove_prefix(ptr - sw.data());
          if (!sw.empty())
            sw.remove_prefix(1); // space
        }
      }

      return points;
    }


    template<EntityID nDim>
    vector<EntityID> BruteForceRangeSearch(vector<CustomVectorTypeND<nDim>> const& points, CustomBoundingBoxND<nDim> const& searchbox)
    {
      auto Entities = vector<EntityID>{};
      auto const nid = points.size();
      for (EntityID id = 0; id < nid; ++id)
        if (CustomGeometryAdapter<nDim>::DoesBoxContainPoint(searchbox, points[id], CustomGeometryAdapter<nDim>::BASE_TOLERANCE))
          Entities.emplace_back(id);

      return Entities;
    }


    TEST_METHOD(BruteForceRangeSearch_UsingPredefinedData_IfAvailable)
    {
      auto constexpr nDim = 3;
      using Vector = CustomVectorTypeND<nDim>;
      using Box = CustomBoundingBoxND<nDim>;
      using Tree = OrthoTreePointContainerCustom<nDim>;

      auto const points = readPointCloud<nDim>("../../../octree_data.txt");
      if (points.empty())
        return;

      auto const searchbox = Box{
        Vector{ 39, 43, 72 },
        Vector{ 49, 53, 76 }
      };

      auto const tree = Tree(points, 3, std::nullopt, 2);
      auto vidActual = tree.RangeSearch(searchbox);
      auto vidExpected = BruteForceRangeSearch<nDim>(points, searchbox);

      // To investigate
      std::ranges::sort(vidActual);
      std::ranges::sort(vidExpected);
      auto missing_ids = vector<EntityID>{};
      std::ranges::set_difference(vidExpected, vidActual, std::back_inserter(missing_ids));
      auto missing_nodes = vector<OrthoTreePointCustom<nDim>::NodeID>(missing_ids.size());
      std::ranges::transform(missing_ids, missing_nodes.begin(), [&](auto const id) { return tree.GetCore().Find(id); });

      Assert::IsTrue(std::ranges::is_permutation(vidActual, vidExpected));
    }

    template<int nDim, typename TScalar>
    vector<BoundingBoxND<nDim, TScalar>> readBoxCloud(std::filesystem::path const& path)
    {
      auto boxes = vector<BoundingBoxND<nDim, TScalar>>{};
      auto file = std::ifstream(path, std::ios::in);
      if (file.fail())
        return boxes;

      auto line = std::string{};
      while (std::getline(file, line))
      {
        if (file.fail())
          return boxes;

        //  LOG__DEBUG   min: { -446.75, 60.0322, -182.431 }, max: { -442.374, 60.0322, -180.318 }
        auto& box = boxes.emplace_back();
        auto sw = std::string_view(line).substr(22);
        for (int iDim = 0; iDim < nDim; ++iDim)
        {
          auto const [ptr, ec] = std::from_chars(sw.data(), sw.data() + sw.length(), box.Min[iDim]);
          if (ec != std::errc{})
            return boxes;

          sw.remove_prefix(ptr - sw.data());
          if (!sw.empty())
            sw.remove_prefix(2); // space
        }

        sw.remove_prefix(9);
        for (int iDim = 0; iDim < nDim; ++iDim)
        {
          auto const [ptr, ec] = std::from_chars(sw.data(), sw.data() + sw.length(), box.Max[iDim]);
          if (ec != std::errc{})
            return boxes;

          sw.remove_prefix(ptr - sw.data());
          if (!sw.empty())
            sw.remove_prefix(2); // space
        }
      }

      return boxes;
    }


    template<int nDim, typename TScalar>
    vector<PointND<nDim, TScalar>> readPointCloud(std::filesystem::path const& path)
    {
      auto points = vector<PointND<nDim, TScalar>>{};
      auto file = std::ifstream(path, std::ios::in);
      if (file.fail())
        return points;

      auto line = std::string{};
      while (std::getline(file, line))
      {
        if (file.fail())
          return points;

        auto& point = points.emplace_back();
        auto sw = std::string_view(line).substr(18);
        for (int iDim = 0; iDim < nDim; ++iDim)
        {
          auto const [ptr, ec] = std::from_chars(sw.data(), sw.data() + sw.length(), point[iDim]);
          if (ec != std::errc{})
            return points;

          sw.remove_prefix(ptr - sw.data());
          if (iDim < nDim - 1)
            sw.remove_prefix(3); // space
        }
      }

      return points;
    }

    TEST_METHOD(Issue26)
    {
      const auto boxes = readBoxCloud<3, float>("../../../bbox_output.txt");
      if (boxes.empty())
        return;

      using Tree = OrthoTreeBoxManagedND<3, true, float>;
      auto const tree = Tree(boxes, 4, std::nullopt, 21, false);

      const auto boxOfTree = tree.GetCore().GetBox();
      const float rayIntersectTolerance = 0.1f;

      auto points = readPointCloud<3, float>("../../../pos_input.txt");
      auto searchSizeYs = vector(points.size(), 100.0f);
      if (points.empty())
      {
        srand(0);
        for (int iTry = 0; iTry < 100; ++iTry)
        {
          constexpr int mod = 1000;
          constexpr auto modr = 0.0011f; // it ensures a little bit bigger space, to make possible out of tree test
          const float searchSizeY = float(rand() % mod) * modr * (boxOfTree.Max[1] - boxOfTree.Min[1]);
          searchSizeYs.emplace_back(searchSizeY);

          const auto x = float(rand() % mod) * modr * (boxOfTree.Max[0] - boxOfTree.Min[0]);
          const auto y = float(rand() % mod) * modr * (boxOfTree.Max[1] - boxOfTree.Min[1]);
          const auto z = float(rand() % mod) * modr * (boxOfTree.Max[2] - boxOfTree.Min[2]);
          points.emplace_back(Tree::TVector{ x, y, z });
        }
      }


      for (std::size_t pointID = 0; pointID < points.size(); ++pointID)
      {
        const auto searchBox = Tree::TBox{
          { points[pointID][0] - rayIntersectTolerance, points[pointID][1] - searchSizeYs[pointID], points[pointID][2] - rayIntersectTolerance },
          { points[pointID][0] + rayIntersectTolerance, points[pointID][1] + rayIntersectTolerance, points[pointID][2] + rayIntersectTolerance }
        };

        auto resultOfBruteForce = vector<EntityID>{};
        auto const boxNo = boxes.size();
        for (EntityID id = 0; id < boxNo; ++id)
        {
          if (Tree::GA::AreBoxesOverlapped(searchBox, boxes[id], false, false))
            resultOfBruteForce.emplace_back(id);
        }

        auto resultOfSearchBox = tree.RangeSearch(searchBox, RangeSearchMode::Overlap);
        std::ranges::sort(resultOfSearchBox);

        std::vector<OctreeBox::NodeID> nodeIDs;
        for (auto entityID : resultOfBruteForce)
          nodeIDs.emplace_back(tree.GetCore().Find(entityID));

        auto resultOfRay = tree.RayIntersectedAll(points[pointID], { 0.0, -1.0, 0.0 }, rayIntersectTolerance, searchSizeYs[pointID]);
        std::ranges::sort(resultOfRay);
        std::vector<OctreeBox::NodeID> nodeIDRs;
        for (auto entityID : resultOfRay)
          nodeIDRs.emplace_back(tree.GetCore().Find(entityID));

        Assert::IsTrue(resultOfBruteForce == resultOfSearchBox);
        Assert::IsTrue(resultOfBruteForce == resultOfRay);
      }
    }

    template<int nDim, typename TScalar>
    [[maybe_unused]] std::vector<PointND<nDim, TScalar>> readPointCloud__DXF_CSV(std::filesystem::path const& path)
    {
      auto points = std::vector<PointND<nDim, TScalar>>{};
      auto file = std::ifstream(path, std::ios::in);
      if (file.fail())
        return points;

      auto line = std::string{};
      while (std::getline(file, line))
      {
        if (file.fail())
          return points;

        if (line.find("AcDbEntity:AcDbPoint") == std::string::npos)
          continue;

        // 4010.19212715855,6499.81094676128,0,"0",,AcDbEntity:AcDbPoint,,D21C,
        auto& point = points.emplace_back();
        auto sw = std::string_view(line);
        for (int iDim = 0; iDim < nDim; ++iDim)
        {
          auto const [ptr, ec] = std::from_chars(sw.data(), sw.data() + sw.length(), point[iDim]);
          if (ec != std::errc{})
            return points;

          sw.remove_prefix(ptr - sw.data());
          if (iDim < nDim - 1)
            sw.remove_prefix(1); // comma
        }
      }

      return points;
    }

    std::vector<EntityID> kNNSearchBruteForce(std::vector<Point2D> const& points, Point2D const& point, size_t k)
    {

      std::vector<EntityID> ids(points.size());
      std::iota(ids.begin(), ids.end(), 0);
      std::partial_sort(ids.begin(), ids.begin() + std::min(k + 10, points.size()), ids.end(), [&](auto const& i1, auto const& i2) {
        return std::hypot(point[0] - points[i1][0], point[1] - points[i1][1]) < std::hypot(point[0] - points[i2][0], point[1] - points[i2][1]);
      });

      auto result = std::vector<EntityID>(ids.begin(), ids.begin() + k);
      auto distLast = std::hypot(point[0] - points[ids[k - 1]][0], point[1] - points[ids[k - 1]][1]);
      auto limit = std::max(std::numeric_limits<double>::epsilon(), distLast * (1.0 + std::numeric_limits<double>::epsilon()));
      for (size_t i = k; i < ids.size(); ++i)
      {
        auto dist = std::hypot(point[0] - points[ids[i]][0], point[1] - points[ids[i]][1]);
        if (dist < limit)
          result.push_back(ids[i]);
        else
          return result;
      }

      // Test is not set correctly.
      Assert::IsTrue(false);
      return result;
    }

    TEST_METHOD(Issue36_kNN_Random)
    {
      constexpr dim_t N = 2;

      using GA = GeneralGeometryAdapterTemplate<N, VectorND<N>, BoundingBoxND<N>, RayND<N>, PlaneND<N>>;

      auto pointsNo = 1000;
      auto points = std::vector<Point2D>(pointsNo);
      auto rng = std::mt19937(0);

      for (std::size_t c = 0; c < 100; ++c)
      {
        for (int i = 0; i < pointsNo; ++i)
          for (dim_t d = 0; d < N; ++d)
            points[i][d] = double(rng() % 100000) / 1000.0;

        auto searchPoints = std::vector<Point2D>(pointsNo);
        for (int i = 0; i < pointsNo; ++i)
          for (dim_t d = 0; d < N; ++d)
            searchPoints[i][d] = double(rng() % 100000) / 1000.0;

        auto const tree = OrthoTreePointND<N>(points, 10, std::nullopt, 5, false);
        auto const k = 4;
        for (auto const& searchPoint : searchPoints)
        {
          auto expected = kNNSearchBruteForce(points, searchPoint, k);
          auto actual = tree.GetNearestNeighbors(searchPoint, k, points);
          Assert::IsTrue(expected.size() == actual.size());

          auto const areResultsEqual = expected == actual;

          if (areResultsEqual)
            continue;

          for (std::size_t i = 0; i < expected.size(); ++i)
          {
            if (expected[i] == actual[i])
              continue;

            auto const expectedDistance = GA::Distance2(searchPoint, points[expected[i]]);
            auto const actualDistance = GA::Distance2(searchPoint, points[actual[i]]);
            Assert::IsTrue(std::abs(actualDistance - expectedDistance) < std::numeric_limits<double>::epsilon() * 10.0);
          }
        }
      }
    }


    template<dim_t N>
    vector<std::pair<EntityID, EntityID>> BruteForceCollisionDetection(vector<BoundingBoxND<N>> const& boxes)
    {
      auto collidedEntities = vector<std::pair<EntityID, EntityID>>{};
      auto const nid = EntityID(boxes.size());
      for (EntityID i = 0; i < nid; ++i)
        for (EntityID j = i + 1; j < nid; ++j)
          if (GeneralGeometryAdapterTemplate<N, PointND<N>, BoundingBoxND<N>, RayND<N>, PlaneND<N>>::AreBoxesOverlapped(boxes[i], boxes[j], false))
            collidedEntities.emplace_back(i, j);

      return collidedEntities;
    }

    bool IsAllElementInsideNodes(auto const& tree, auto const& boxes)
    {
      using GA = typename std::remove_cvref_t<decltype(tree)>::GA;
      bool isOK = true;
      tree.TraverseNodesDepthFirst([&](auto const nodeValue) {
        auto const& entityIDs = tree.GetNodeEntities(nodeValue);
        auto const& nodeBox = tree.GetNodeBox(nodeValue);
        for (auto const entityID : entityIDs)
        {
          if (!GA::AreBoxesOverlapped(nodeBox, boxes[entityID], true))
          {
            isOK = false;
            return TraverseControl::Terminate;
          }
        }
        return TraverseControl::Continue;
      });

      return isOK;
    }

    TEST_METHOD(Issue38_ParallelCollisionDetection_Random)
    {
      constexpr dim_t N = 3;
      constexpr bool IS_LOOSE_TREE = true;
      constexpr auto EXEC_MODE = SEQ_EXEC;
      auto execTag = PAR_EXEC;

      constexpr std::size_t boxNo = 1000;

      auto const spaceMax = 8.0;
      auto const sizeMax = 2.0;
      auto rng = std::mt19937(0);
      for (std::size_t c = 0; c < 10; ++c)
      {
        // Generate boxes

        auto boxes = vector<BoundingBoxND<N>>(boxNo);
        boxes[0].Min.fill(0.0);
        boxes[0].Max.fill(sizeMax);
        boxes[1].Min.fill(spaceMax - sizeMax);
        boxes[1].Max.fill(spaceMax);

        for (std::size_t i = 2; i < boxNo; ++i)
        {
          for (dim_t d = 0; d < N; ++d)
          {
            boxes[i].Min[d] = std::min(double(rng() % 10000) / 10000.0 * spaceMax, spaceMax - 0.1);
            boxes[i].Max[d] = std::min(boxes[i].Min[d] + double(rng() % 10000) / 10000.0 * sizeMax, spaceMax);
          }
        }

        auto const tree = OrthoTreeBoxND<N, IS_LOOSE_TREE>(EXEC_MODE, boxes, 10, std::nullopt, 5);
        IsAllElementInsideNodes(tree, boxes);
        auto const expectedResult = BruteForceCollisionDetection(boxes);
        auto actualResult = tree.template CollisionDetection<decltype(execTag)>(boxes);
        for (auto& [a, b] : actualResult)
          if (a > b)
            std::swap(a, b);

        std::ranges::sort(actualResult);
        Assert::IsTrue(actualResult == expectedResult);
      }
    }
  };
} // namespace LongIntAdaptor

namespace StaticTree
{
  TEST_CLASS(StaticTree)
  {
  public:
    TEST_METHOD(OctreeMap_Create)
    {
      auto const points = std::unordered_map<index_t, Point3D>{
        { 1,   Point3D{ 0.0, 0.0, 0.0 } },
        { 2,   Point3D{ 1.0, 0.0, 0.0 } },
        { 3,   Point3D{ 2.0, 0.0, 0.0 } },
        { 4,   Point3D{ 3.0, 0.0, 0.0 } },
        { 5,   Point3D{ 4.0, 4.0, 4.0 } },

        { 6,   Point3D{ 0.5, 0.0, 0.0 } },
        { 7,  Point3D{ 0.25, 0.0, 0.0 } },
        { 8, Point3D{ 0.25, 0.25, 0.0 } },
        { 9, Point3D{ 0.125, 0.0, 0.0 } },
      };

      auto const octree = StaticOctreePointMap(points, std::nullopt, std::nullopt, 1);
    }
  };
} // namespace StaticTree

namespace EntityAdapterFlexibilityTest
{
  using namespace OrthoTree;

  // ============================================================
  // Test 1: Move-only entity with map-based container
  // Entity that wraps a geometry + payload, but is NOT copyable
  // ============================================================

  struct MoveOnlyEntity
  {
    Point3D position;
    std::unique_ptr<int> payload; // makes it move-only

    MoveOnlyEntity() = default;
    MoveOnlyEntity(Point3D pos, int val)
    : position(pos)
    , payload(std::make_unique<int>(val))
    {}

    MoveOnlyEntity(MoveOnlyEntity&&) noexcept = default;
    MoveOnlyEntity& operator=(MoveOnlyEntity&&) noexcept = default;
    MoveOnlyEntity(MoveOnlyEntity const&) = delete;
    MoveOnlyEntity& operator=(MoveOnlyEntity const&) = delete;
  };

  // Custom adapter for move-only entity in a map
  struct MoveOnlyEntityAdapter : public PointEntityMapAdapter<Point3D, std::unordered_map<index_t, MoveOnlyEntity>>
  {
    using Base = PointEntityMapAdapter<Point3D, std::unordered_map<index_t, MoveOnlyEntity>>;

    static Point3D const& GetGeometry(MoveOnlyEntity const& entity) { return entity.position; }
    static Point3D const& GetGeometry(Base::Entity const& entity) { return entity.second.position; }
    static Point3D const& GetGeometry(Base::EntityContainerView entities, Base::EntityID entityID) { return entities.at(entityID).position; }
  };

  using MoveOnlyOctreeCore =
    OrthoTreeBase<MoveOnlyEntityAdapter, GeneralGeometryAdapterTemplate<3, Point3D, BoundingBox3D, Ray3D, Plane3D>, PointConfiguration<>>;

  using MoveOnlyOctreeContainer = OrthoTreeManaged<MoveOnlyOctreeCore>;


  // ============================================================
  // Test 2: Non-default-constructible EntityID (trivially copyable)
  // ============================================================

  struct StrongEntityID
  {
    uint32_t value;

    // No default constructor! But trivially copyable.
    explicit constexpr StrongEntityID(uint32_t v) noexcept
    : value(v)
    {}

    // Required for hash map key
    constexpr bool operator==(StrongEntityID const&) const noexcept = default;
    constexpr auto operator<=>(StrongEntityID const&) const noexcept = default;
  };
  static_assert(std::is_trivially_copyable_v<StrongEntityID>);
  static_assert(!std::is_default_constructible_v<StrongEntityID>);

  struct StrongEntityIDHash
  {
    std::size_t operator()(StrongEntityID const& id) const noexcept { return std::hash<uint32_t>{}(id.value); }
  };

  // Adapter for StrongEntityID
  struct StrongIDEntityAdapter
  : public PointEntityMapAdapter<Point3D, std::unordered_map<StrongEntityID, Point3D, StrongEntityIDHash>, std::unordered_map<StrongEntityID, Point3D, StrongEntityIDHash> const&, StrongEntityIDHash>
  {};

  using StrongIDOctreeCore =
    OrthoTreeBase<StrongIDEntityAdapter, GeneralGeometryAdapterTemplate<3, Point3D, BoundingBox3D, Ray3D, Plane3D>, PointConfiguration<>>;

  using StrongIDOctreeContainer = OrthoTreeManaged<StrongIDOctreeCore>;


  TEST_CLASS(EntityAdapterFlexibilityTests)
  {
  public:
    // ----------------------------------------------------------
    // Move-only entity: Core tree usage with external container
    // ----------------------------------------------------------
    TEST_METHOD(MoveOnlyEntity_CoreCreate)
    {
      auto entities = MoveOnlyEntityAdapter::EntityContainer{};
      entities.emplace(
        0,
        MoveOnlyEntity{
          Point3D{ 0.0, 0.0, 0.0 },
          100
      });
      entities.emplace(
        1,
        MoveOnlyEntity{
          Point3D{ 1.0, 1.0, 1.0 },
          200
      });
      entities.emplace(
        2,
        MoveOnlyEntity{
          Point3D{ 2.0, 2.0, 2.0 },
          300
      });
      entities.emplace(
        3,
        MoveOnlyEntity{
          Point3D{ 3.0, 3.0, 3.0 },
          400
      });
      entities.emplace(
        4,
        MoveOnlyEntity{
          Point3D{ 4.0, 4.0, 4.0 },
          500
      });

      MoveOnlyOctreeCore tree;
      tree.Create(entities, 3, std::nullopt, 2);

      auto const box = tree.GetBox();
      Assert::IsTrue(true); // If we got here, move-only entities work with Core
    }

    TEST_METHOD(MoveOnlyEntity_CoreRangeSearch)
    {
      auto entities = MoveOnlyEntityAdapter::EntityContainer{};
      entities.emplace(
        0,
        MoveOnlyEntity{
          Point3D{ 0.0, 0.0, 0.0 },
          10
      });
      entities.emplace(
        1,
        MoveOnlyEntity{
          Point3D{ 1.0, 1.0, 1.0 },
          20
      });
      entities.emplace(
        2,
        MoveOnlyEntity{
          Point3D{ 2.0, 2.0, 2.0 },
          30
      });
      entities.emplace(
        3,
        MoveOnlyEntity{
          Point3D{ 3.0, 3.0, 3.0 },
          40
      });
      entities.emplace(
        4,
        MoveOnlyEntity{
          Point3D{ 4.0, 4.0, 4.0 },
          50
      });

      auto tree = MoveOnlyOctreeCore();
      MoveOnlyOctreeCore::Create(tree, entities, 3, std::nullopt, 2);

      auto const searchBox = BoundingBox3D{
        Point3D{ 0.5, 0.5, 0.5 },
        Point3D{ 2.5, 2.5, 2.5 }
      };
      auto results = tree.RangeSearch(searchBox, entities);

      // Should find entities 1 and 2
      Assert::AreEqual(std::size_t(2), results.size());
      std::ranges::sort(results);
      Assert::AreEqual(index_t(1), results[0]);
      Assert::AreEqual(index_t(2), results[1]);
    }

    TEST_METHOD(MoveOnlyEntity_CoreInsert)
    {
      auto entities = MoveOnlyEntityAdapter::EntityContainer{};
      entities.emplace(
        0,
        MoveOnlyEntity{
          Point3D{ 0.0, 0.0, 0.0 },
          10
      });
      entities.emplace(
        1,
        MoveOnlyEntity{
          Point3D{ 4.0, 4.0, 4.0 },
          20
      });

      auto tree = MoveOnlyOctreeCore();
      MoveOnlyOctreeCore::Create(tree, entities, 3, std::nullopt, 2);

      // Insert a new entity
      entities.emplace(
        2,
        MoveOnlyEntity{
          Point3D{ 2.0, 2.0, 2.0 },
          30
      });
      bool inserted = tree.Insert(index_t(2), Point3D{ 2.0, 2.0, 2.0 }, entities);
      Assert::IsTrue(inserted);

      auto const searchBox = BoundingBox3D{
        Point3D{ 1.5, 1.5, 1.5 },
        Point3D{ 2.5, 2.5, 2.5 }
      };
      auto results = tree.RangeSearch(searchBox, entities);
      Assert::AreEqual(std::size_t(1), results.size());
      Assert::AreEqual(index_t(2), results[0]);
    }

    // ----------------------------------------------------------
    // Move-only entity: OrthoTreeManaged usage
    // This is expected to FAIL or require container changes
    // because OrthoTreeManaged copies/moves the container
    // ----------------------------------------------------------
    TEST_METHOD(MoveOnlyEntity_ContainerCreate)
    {
      auto entities = MoveOnlyEntityAdapter::EntityContainer{};
      entities.emplace(
        0,
        MoveOnlyEntity{
          Point3D{ 0.0, 0.0, 0.0 },
          100
      });
      entities.emplace(
        1,
        MoveOnlyEntity{
          Point3D{ 1.0, 1.0, 1.0 },
          200
      });
      entities.emplace(
        2,
        MoveOnlyEntity{
          Point3D{ 2.0, 2.0, 2.0 },
          300
      });
      entities.emplace(
        3,
        MoveOnlyEntity{
          Point3D{ 3.0, 3.0, 3.0 },
          400
      });
      entities.emplace(
        4,
        MoveOnlyEntity{
          Point3D{ 4.0, 4.0, 4.0 },
          500
      });

      // This should work via the move constructor
      auto tree = MoveOnlyOctreeContainer(std::move(entities), 3, std::nullopt, 2);

      auto const& data = tree.GetData();
      Assert::AreEqual(std::size_t(5), data.size());
    }

    TEST_METHOD(MoveOnlyEntity_ContainerRangeSearch)
    {
      auto entities = MoveOnlyEntityAdapter::EntityContainer{};
      entities.emplace(
        0,
        MoveOnlyEntity{
          Point3D{ 0.0, 0.0, 0.0 },
          10
      });
      entities.emplace(
        1,
        MoveOnlyEntity{
          Point3D{ 1.0, 1.0, 1.0 },
          20
      });
      entities.emplace(
        2,
        MoveOnlyEntity{
          Point3D{ 2.0, 2.0, 2.0 },
          30
      });
      entities.emplace(
        3,
        MoveOnlyEntity{
          Point3D{ 3.0, 3.0, 3.0 },
          40
      });
      entities.emplace(
        4,
        MoveOnlyEntity{
          Point3D{ 4.0, 4.0, 4.0 },
          50
      });

      auto tree = MoveOnlyOctreeContainer(std::move(entities), 3, std::nullopt, 2);

      auto const searchBox = BoundingBox3D{
        Point3D{ 0.5, 0.5, 0.5 },
        Point3D{ 2.5, 2.5, 2.5 }
      };
      auto results = tree.RangeSearch(searchBox);

      Assert::AreEqual(std::size_t(2), results.size());
      std::ranges::sort(results);
      Assert::AreEqual(index_t(1), results[0]);
      Assert::AreEqual(index_t(2), results[1]);
    }


    // ----------------------------------------------------------
    // Non-default-constructible EntityID: Core tree usage
    // ----------------------------------------------------------
    TEST_METHOD(StrongEntityID_CoreCreate)
    {
      auto entities = StrongIDEntityAdapter::EntityContainer{};
      entities.emplace(StrongEntityID{ 0 }, Point3D{ 0.0, 0.0, 0.0 });
      entities.emplace(StrongEntityID{ 1 }, Point3D{ 1.0, 1.0, 1.0 });
      entities.emplace(StrongEntityID{ 2 }, Point3D{ 2.0, 2.0, 2.0 });
      entities.emplace(StrongEntityID{ 3 }, Point3D{ 3.0, 3.0, 3.0 });
      entities.emplace(StrongEntityID{ 4 }, Point3D{ 4.0, 4.0, 4.0 });

      auto tree = StrongIDOctreeCore();
      StrongIDOctreeCore::Create(tree, entities, 3, std::nullopt, 2);

      auto const box = tree.GetBox();
      Assert::IsTrue(true); // If we got here, non-default-constructible IDs work
    }

    TEST_METHOD(StrongEntityID_CoreRangeSearch)
    {
      auto entities = StrongIDEntityAdapter::EntityContainer{};
      entities.emplace(StrongEntityID{ 0 }, Point3D{ 0.0, 0.0, 0.0 });
      entities.emplace(StrongEntityID{ 1 }, Point3D{ 1.0, 1.0, 1.0 });
      entities.emplace(StrongEntityID{ 2 }, Point3D{ 2.0, 2.0, 2.0 });
      entities.emplace(StrongEntityID{ 3 }, Point3D{ 3.0, 3.0, 3.0 });
      entities.emplace(StrongEntityID{ 4 }, Point3D{ 4.0, 4.0, 4.0 });

      auto tree = StrongIDOctreeCore();
      StrongIDOctreeCore::Create(tree, entities, 3, std::nullopt, 2);

      auto const searchBox = BoundingBox3D{
        Point3D{ 0.5, 0.5, 0.5 },
        Point3D{ 2.5, 2.5, 2.5 }
      };
      auto results = tree.RangeSearch(searchBox, entities);

      Assert::AreEqual(std::size_t(2), results.size());
      std::ranges::sort(results);
      Assert::IsTrue(StrongEntityID{ 1 } == results[0]);
      Assert::IsTrue(StrongEntityID{ 2 } == results[1]);
    }

    TEST_METHOD(StrongEntityID_CoreInsert)
    {
      auto entities = StrongIDEntityAdapter::EntityContainer{};
      entities.emplace(StrongEntityID{ 0 }, Point3D{ 0.0, 0.0, 0.0 });
      entities.emplace(StrongEntityID{ 1 }, Point3D{ 4.0, 4.0, 4.0 });

      auto tree = StrongIDOctreeCore();
      StrongIDOctreeCore::Create(tree, entities, 3, std::nullopt, 2);

      entities.emplace(StrongEntityID{ 2 }, Point3D{ 2.0, 2.0, 2.0 });
      bool inserted = tree.Insert(StrongEntityID{ 2 }, Point3D{ 2.0, 2.0, 2.0 }, entities);
      Assert::IsTrue(inserted);

      auto const searchBox = BoundingBox3D{
        Point3D{ 1.5, 1.5, 1.5 },
        Point3D{ 2.5, 2.5, 2.5 }
      };
      auto results = tree.RangeSearch(searchBox, entities);
      Assert::AreEqual(std::size_t(1), results.size());
      Assert::IsTrue(StrongEntityID{ 2 } == results[0]);
    }

    TEST_METHOD(StrongEntityID_CoreErase)
    {
      auto entities = StrongIDEntityAdapter::EntityContainer{};
      entities.emplace(StrongEntityID{ 0 }, Point3D{ 0.0, 0.0, 0.0 });
      entities.emplace(StrongEntityID{ 1 }, Point3D{ 1.0, 1.0, 1.0 });
      entities.emplace(StrongEntityID{ 2 }, Point3D{ 2.0, 2.0, 2.0 });
      entities.emplace(StrongEntityID{ 3 }, Point3D{ 4.0, 4.0, 4.0 });

      auto tree = StrongIDOctreeCore();
      StrongIDOctreeCore::Create(tree, entities, 3, std::nullopt, 2);

      bool erased = tree.Erase(StrongEntityID{ 1 }, Point3D{ 1.0, 1.0, 1.0 });
      Assert::IsTrue(erased);

      auto const searchBox = BoundingBox3D{
        Point3D{ 0.5, 0.5, 0.5 },
        Point3D{ 1.5, 1.5, 1.5 }
      };
      auto results = tree.RangeSearch(searchBox, entities);
      Assert::AreEqual(std::size_t(0), results.size());
    }

    // ----------------------------------------------------------
    // Non-default-constructible EntityID: OrthoTreeManaged usage
    // This exercises the container's Add/Erase/Update paths
    // ----------------------------------------------------------
    TEST_METHOD(StrongEntityID_ContainerCreate)
    {
      auto entities = StrongIDEntityAdapter::EntityContainer{};
      entities.emplace(StrongEntityID{ 0 }, Point3D{ 0.0, 0.0, 0.0 });
      entities.emplace(StrongEntityID{ 1 }, Point3D{ 1.0, 1.0, 1.0 });
      entities.emplace(StrongEntityID{ 2 }, Point3D{ 2.0, 2.0, 2.0 });
      entities.emplace(StrongEntityID{ 3 }, Point3D{ 3.0, 3.0, 3.0 });
      entities.emplace(StrongEntityID{ 4 }, Point3D{ 4.0, 4.0, 4.0 });

      auto tree = StrongIDOctreeContainer(entities, 3, std::nullopt, 2);

      auto const& data = tree.GetData();
      Assert::AreEqual(std::size_t(5), data.size());
    }

    TEST_METHOD(StrongEntityID_ContainerAdd)
    {
      auto entities = StrongIDEntityAdapter::EntityContainer{};
      entities.emplace(StrongEntityID{ 0 }, Point3D{ 0.0, 0.0, 0.0 });
      entities.emplace(StrongEntityID{ 1 }, Point3D{ 4.0, 4.0, 4.0 });

      auto tree = StrongIDOctreeContainer(entities, 3, std::nullopt, 2);

      auto added = tree.Add(
        StrongIDEntityAdapter::Entity{
          StrongEntityID{ 2 },
          Point3D{ 2.0, 2.0, 2.0 }
      });
      Assert::IsTrue(added.has_value());

      auto const searchBox = BoundingBox3D{
        Point3D{ 1.5, 1.5, 1.5 },
        Point3D{ 2.5, 2.5, 2.5 }
      };
      auto results = tree.RangeSearch(searchBox);
      Assert::AreEqual(std::size_t(1), results.size());
      Assert::IsTrue(StrongEntityID{ 2 } == results[0]);
    }

    TEST_METHOD(StrongEntityID_ContainerErase)
    {
      auto entities = StrongIDEntityAdapter::EntityContainer{};
      entities.emplace(StrongEntityID{ 0 }, Point3D{ 0.0, 0.0, 0.0 });
      entities.emplace(StrongEntityID{ 1 }, Point3D{ 1.0, 1.0, 1.0 });
      entities.emplace(StrongEntityID{ 2 }, Point3D{ 2.0, 2.0, 2.0 });
      entities.emplace(StrongEntityID{ 3 }, Point3D{ 4.0, 4.0, 4.0 });

      auto tree = StrongIDOctreeContainer(entities, 3, std::nullopt, 2);

      bool erased = tree.Erase(StrongEntityID{ 1 });
      Assert::IsTrue(erased);

      auto const searchBox = BoundingBox3D{
        Point3D{ 0.5, 0.5, 0.5 },
        Point3D{ 1.5, 1.5, 1.5 }
      };
      auto results = tree.RangeSearch(searchBox);
      Assert::AreEqual(std::size_t(0), results.size());
    }

    TEST_METHOD(StrongEntityID_ContainerUpdate)
    {
      auto entities = StrongIDEntityAdapter::EntityContainer{};
      entities.emplace(StrongEntityID{ 0 }, Point3D{ 0.0, 0.0, 0.0 });
      entities.emplace(StrongEntityID{ 1 }, Point3D{ 1.0, 1.0, 1.0 });
      entities.emplace(StrongEntityID{ 2 }, Point3D{ 4.0, 4.0, 4.0 });

      auto tree = StrongIDOctreeContainer(entities, 3, std::nullopt, 2);

      // Move entity 1 to a new position
      bool updated = tree.Update(
        {
          StrongEntityID{ 1 },
          Point3D{ 3.0, 3.0, 3.0 }
      });
      Assert::IsTrue(updated);

      // Should no longer be found at old position
      auto const oldSearchBox = BoundingBox3D{
        Point3D{ 0.5, 0.5, 0.5 },
        Point3D{ 1.5, 1.5, 1.5 }
      };
      auto oldResults = tree.RangeSearch(oldSearchBox);
      Assert::AreEqual(std::size_t(0), oldResults.size());

      // Should be found at new position
      auto const newSearchBox = BoundingBox3D{
        Point3D{ 2.5, 2.5, 2.5 },
        Point3D{ 3.5, 3.5, 3.5 }
      };
      auto newResults = tree.RangeSearch(newSearchBox);
      Assert::AreEqual(std::size_t(1), newResults.size());
      Assert::IsTrue(StrongEntityID{ 1 } == newResults[0]);
    }

    TEST_METHOD(StrongEntityID_ContainerPickSearch)
    {
      auto entities = StrongIDEntityAdapter::EntityContainer{};
      entities.emplace(StrongEntityID{ 10 }, Point3D{ 1.0, 1.0, 1.0 });
      entities.emplace(StrongEntityID{ 20 }, Point3D{ 2.0, 2.0, 2.0 });
      entities.emplace(StrongEntityID{ 30 }, Point3D{ 3.0, 3.0, 3.0 });

      auto tree = StrongIDOctreeContainer(entities, 3, std::nullopt, 2);

      auto results = tree.PickSearch(Point3D{ 2.0, 2.0, 2.0 }, 0.1);
      Assert::AreEqual(std::size_t(1), results.size());
      Assert::IsTrue(StrongEntityID{ 20 } == results[0]);
    }
  };

} // namespace EntityAdapterFlexibilityTest


namespace GeneralTest
{
  TEST_CLASS(DynamicVsStaticTest)
  {
  private:
    template<dim_t DIM, bool IS_POINT, bool IS_LOOSE>
    void _compare()
    {
      using PointD = PointND<DIM>;
      using BoxD = BoundingBoxND<DIM>;
      using EntityGeometry = std::conditional_t<IS_POINT, PointD, BoxD>;

      auto const domain = BoxD{
        {  0.0,  0.0,  0.0 },
        { 10.0, 10.0, 10.0 }
      };
      auto const maxDepth = depth_t{ 10 };
      auto const maxElement = std::size_t{ 2 };

      auto entities = vector<EntityGeometry>();
      if constexpr (IS_POINT)
      {
        entities = {
          PointD{ 1.0, 1.0, 1.0 },
          PointD{ 1.1, 1.1, 1.1 },
          PointD{ 5.0, 5.0, 5.0 },
          PointD{ 9.0, 9.0, 9.0 },
          PointD{ 9.1, 9.1, 9.1 }
        };
      }
      else
      {
        entities = {
          BoxD{ { 1.0, 1.0, 1.0 }, { 2.0, 2.0, 2.0 } },
          BoxD{ { 1.1, 1.1, 1.1 }, { 2.1, 2.1, 2.1 } },
          BoxD{ { 4.5, 4.5, 4.5 }, { 5.5, 5.5, 5.5 } },
          BoxD{ { 8.0, 8.0, 8.0 }, { 9.0, 9.0, 9.0 } },
          BoxD{ { 8.1, 8.1, 8.1 }, { 9.1, 9.1, 9.1 } }
        };
      }

      using TreeDynamic = std::conditional_t<IS_POINT, OrthoTreePointND<DIM>, OrthoTreeBoxND<DIM, IS_LOOSE>>;

      using TreeStatic = std::conditional_t<IS_POINT, StaticOrthoTreePointND<DIM>, StaticOrthoTreeBoxND<DIM, IS_LOOSE>>;


      auto treeDynamic = TreeDynamic(entities, maxDepth, domain, maxElement);
      auto treeStatic = TreeStatic(entities, maxDepth, domain, maxElement);

      Assert::AreEqual(treeDynamic.GetNodeCount(), treeStatic.GetNodeCount());
      Assert::IsTrue(treeDynamic.IsEqualTo(treeStatic));
      Assert::IsTrue(treeStatic.IsEqualTo(treeDynamic));

      // Test inequality after modification
      if (entities.size() > 0)
      {
        auto entitiesModified = entities;
        if constexpr (IS_POINT)
          entitiesModified[0][0] += 2.0; // Move within domain (1.0 -> 3.0), not outside (-0.5 would be out-of-domain)
        else
          entitiesModified[0].Min[0] -= 1.5;

        auto treeStaticModified = TreeStatic(entitiesModified, maxDepth, domain, maxElement);
        Assert::IsFalse(treeDynamic.IsEqualTo(treeStaticModified));
      }
    }

  public:
    TEST_METHOD(ComparisonTest_MBR_NotLoose)
    {
      _compare<3, false, false>();
    }

    // Dynamic does not have LCA calculation
    // TEST_METHOD(ComparisonTest_Point_NotLoose)
    // {
    //   _compare<3, true, false>();
    // }

    TEST_METHOD(ComparisonTest_MBR_Loose)
    {
      _compare<3, false, true>();
    }

    // Dynamic does not have LCA calculation
    // TEST_METHOD(ComparisonTest_Point_Loose)
    // {
    //   _compare<3, true, true>();
    // }
  };

  TEST_CLASS(DynamicHashCoreInsertTest)
  {
  public:
    TEST_METHOD(CoreInsert_SpanOfGeometry_2D)
    {
      using Tree = OrthoTreeBoxND<2>;
      using Box = BoundingBoxND<2>;

      auto core = Tree();
      core.Init(
        Box{
          {  0.0,  0.0 },
          { 10.0, 10.0 }
      },
        5,
        2);

      auto const entities = std::vector<Box>{
        { { 1.0, 1.0 }, { 2.0, 2.0 } },
        { { 3.0, 3.0 }, { 4.0, 4.0 } }
      };

      auto const existing = std::vector<Box>{};
      bool success = core.Insert(std::span(entities), std::span(existing));
      Assert::IsTrue(success);
      Assert::AreEqual(size_t(2), core.GetEntitiesBreadthFirst().size());
    }

    TEST_METHOD(CoreInsert_VectorOfPair_3D)
    {
      using Tree = OrthoTreeBoxND<3>;
      using Box = BoundingBoxND<3>;
      using EntityID = Tree::EntityID;

      auto core = Tree();
      core.Init(
        Box{
          {  0.0,  0.0,  0.0 },
          { 10.0, 10.0, 10.0 }
      },
        5,
        2);

      auto const newEntities = std::vector<std::pair<EntityID, Box>>{
        { 1, { { 1.0, 1.0, 1.0 }, { 2.0, 2.0, 2.0 } } },
        { 0, { { 3.0, 3.0, 3.0 }, { 4.0, 4.0, 4.0 } } }
      };

      auto const existing = std::vector<Box>{};
      bool success = core.Insert(newEntities, std::span(existing));
      Assert::IsTrue(success);
      Assert::AreEqual(size_t(2), core.GetEntitiesBreadthFirst().size());
    }

    TEST_METHOD(CoreInsert_VectorOfEntityStruct_6D)
    {
      using Tree = OrthoTreePointND<6>;
      using Point = PointND<6>;
      using EntityID = Tree::EntityID;

      auto core = Tree();
      core.Init(
        BoundingBoxND<6>{
          {  0.0,  0.0,  0.0,  0.0,  0.0,  0.0 },
          { 10.0, 10.0, 10.0, 10.0, 10.0, 10.0 }
      },
        5,
        2);

      struct MyEntity
      {
        EntityID id;
        Point pt;
      };

      auto const newEntities = std::vector<MyEntity>{
        { 5, { 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 } },
        { 6, { 3.0, 3.0, 3.0, 3.0, 3.0, 3.0 } }
      };

      auto const existing = std::vector<Point>{};
      bool success = core.Insert(newEntities, std::span(existing));
      Assert::IsTrue(success);
      Assert::AreEqual(size_t(2), core.GetEntitiesBreadthFirst().size());
    }

    TEST_METHOD(CoreInsert_SpanOfGeometry_12D)
    {
      using Tree = OrthoTreePointND<12>;
      using Point = PointND<12>;

      auto core = Tree();
      core.Init(
        BoundingBoxND<12>{
          {  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0 },
          { 10.0, 10.0, 10.0, 10.0, 10.0, 10.0, 10.0, 10.0, 10.0, 10.0, 10.0, 10.0 }
      },
        5,
        2);

      auto const entities = std::vector<Point>{
        { 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 },
        { 3.0, 3.0, 3.0, 3.0, 3.0, 3.0, 3.0, 3.0, 3.0, 3.0, 3.0, 3.0 }
      };

      auto const existing = std::vector<Point>{};
      bool success = core.Insert(std::span(entities), std::span(existing));
      Assert::IsTrue(success);
      Assert::AreEqual(size_t(2), core.GetEntitiesBreadthFirst().size());
    }
  };

  TEST_CLASS(OrthoTreeManagedAddTest)
  {
  public:
    TEST_METHOD(ContainerAdd_SpanOfGeometry_2D)
    {
      using Container = OrthoTreeBoxManagedND<2>;
      using Box = BoundingBoxND<2>;

      auto container = Container();
      container.Init(
        Box{
          {  0.0,  0.0 },
          { 10.0, 10.0 }
      },
        5,
        2);

      auto const entities = std::vector<Box>{
        { { 1.0, 1.0 }, { 2.0, 2.0 } },
        { { 3.0, 3.0 }, { 4.0, 4.0 } }
      };

      container.Add(std::span(entities), SeqExec{});
      Assert::AreEqual(size_t(2), container.GetEntitiesBreadthFirst().size());
    }

    TEST_METHOD(ContainerAdd_VectorOfGeometry_3D)
    {
      using Container = OrthoTreeBoxManagedND<3>;
      using Box = BoundingBoxND<3>;

      auto container = Container();
      container.Init(
        Box{
          {  0.0,  0.0,  0.0 },
          { 10.0, 10.0, 10.0 }
      },
        5,
        2);

      auto const entities = std::vector<Box>{
        { { 1.0, 1.0, 1.0 }, { 2.0, 2.0, 2.0 } },
        { { 3.0, 3.0, 3.0 }, { 4.0, 4.0, 4.0 } }
      };

      container.Add(entities, SeqExec{});
      Assert::AreEqual(size_t(2), container.GetEntitiesBreadthFirst().size());
    }

    TEST_METHOD(ContainerAdd_VectorOfGeometry_6D)
    {
      using Container = OrthoTreePointManagedND<6>;
      using Point = PointND<6>;

      auto container = Container();
      container.Init(
        BoundingBoxND<6>{
          {  0.0,  0.0,  0.0,  0.0,  0.0,  0.0 },
          { 10.0, 10.0, 10.0, 10.0, 10.0, 10.0 }
      },
        5,
        2);

      auto const entities = std::vector<Point>{
        { 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 },
        { 3.0, 3.0, 3.0, 3.0, 3.0, 3.0 }
      };

      container.Add(entities, SeqExec{});
      Assert::AreEqual(size_t(2), container.GetEntitiesBreadthFirst().size());
    }

    TEST_METHOD(ContainerAdd_SpanOfGeometry_12D)
    {
      using Container = OrthoTreePointManagedND<12>;
      using Point = PointND<12>;

      auto container = Container();
      container.Init(
        BoundingBoxND<12>{
          {  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0 },
          { 10.0, 10.0, 10.0, 10.0, 10.0, 10.0, 10.0, 10.0, 10.0, 10.0, 10.0, 10.0 }
      },
        5,
        2);

      auto const entities = std::vector<Point>{
        { 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 },
        { 3.0, 3.0, 3.0, 3.0, 3.0, 3.0, 3.0, 3.0, 3.0, 3.0, 3.0, 3.0 }
      };

      container.Add(std::span(entities), SeqExec{});
      Assert::AreEqual(size_t(2), container.GetEntitiesBreadthFirst().size());
    }
  };

} // namespace GeneralTest
