#include "pch.h"

#include <filesystem>
#include <charconv>
#include <fstream>

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-braces"
#endif

using namespace OrthoTree;

using std::array;
using std::vector;

namespace Microsoft {
  namespace VisualStudio {
    namespace CppUnitTestFramework
    {
      template<> inline std::wstring ToString<std::bitset<65>>(const std::bitset<65>& t) { RETURN_WIDE_STRING(t); }

    }
  }
}


namespace
{
  autoce BB1_INV = BoundingBox1D{ std::numeric_limits<double>::max(), std::numeric_limits<double>::lowest() };

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

    return array
    {
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

}


namespace GeneralTest
{
  TEST_CLASS(MortonTest)
  {
  public:
    TEST_METHOD(M1D_0_0)
    {
      autoce arr = array<GridID, 1> { 0 };
      Assert::AreEqual(DualtreePoint::MortonGridID{ 0 }, DualtreePoint::MortonEncode(arr));
    }

    TEST_METHOD(M1D_4_4)
    {
      autoce arr = array<GridID, 1>{ 4 };
      Assert::AreEqual(DualtreePoint::MortonGridID{ 4 }, DualtreePoint::MortonEncode(arr));
    }


    TEST_METHOD(M2D_00_0)
    {
      autoce arr = array<GridID, 2>{ 0, 0 };
      Assert::AreEqual(QuadtreePoint::MortonGridID{ 0 }, QuadtreePoint::MortonEncode(arr));
    }

    TEST_METHOD(M2D_20_4)
    {
      autoce arr = array<GridID, 2>{ 2, 0 };
      Assert::AreEqual(QuadtreePoint::MortonGridID{ 4 }, QuadtreePoint::MortonEncode(arr));
    }

    TEST_METHOD(M2D_02_8)
    {
      autoce arr = array<GridID, 2>{ 0, 2 };
      Assert::AreEqual(QuadtreePoint::MortonGridID{ 8 }, QuadtreePoint::MortonEncode(arr));
    }

    TEST_METHOD(M2D_22_12)
    {
      autoce arr = array<GridID, 2>{ 2, 2 };
      Assert::AreEqual(QuadtreePoint::MortonGridID{ 12 }, QuadtreePoint::MortonEncode(arr));
    }

    TEST_METHOD(M2D_13_11)
    {
      autoce arr = array<GridID, 2>{ 1, 3 };
      Assert::AreEqual(QuadtreePoint::MortonGridID{ 11 }, QuadtreePoint::MortonEncode(arr));
    }

    TEST_METHOD(M3D_000_0)
    {
      autoce arr = array<GridID, 3>{ 0, 0, 0 };
      Assert::AreEqual(OctreePoint::MortonGridID{ 0 }, OctreePoint::MortonEncode(arr));
    }

    TEST_METHOD(M3D_100_1)
    {
      autoce arr = array<GridID, 3>{ 1, 0, 0 };
      Assert::AreEqual(OctreePoint::MortonGridID{ 1 }, OctreePoint::MortonEncode(arr));
    }

    TEST_METHOD(M3D_001_4)
    {
      autoce arr = array<GridID, 3>{ 0, 0, 1 };
      Assert::AreEqual(OctreePoint::MortonGridID{ 4 }, OctreePoint::MortonEncode(arr));
    }

    TEST_METHOD(M3D_111_7)
    {
      autoce arr = array<GridID, 3>{ 1, 1, 1 };
      Assert::AreEqual(OctreePoint::MortonGridID{ 7 }, OctreePoint::MortonEncode(arr));
    }

    TEST_METHOD(M4D_1111_15)
    {
      autoce arr = array<GridID, 4>{ 1, 1, 1, 1 };
      Assert::AreEqual(HexatreePoint::MortonGridID{ 15 }, HexatreePoint::MortonEncode(arr));
    }

    TEST_METHOD(M4D_2111_30)
    {
      autoce arr = array<GridID, 4>{ 2, 1, 1, 1 };
      Assert::AreEqual(HexatreePoint::MortonGridID{ 30 }, HexatreePoint::MortonEncode(arr));
    }
  };

  TEST_CLASS(NodeTest)
  {
  private:
    template<dim_t N>
    static void _complex_ND_Only1()
    {
      using child_id_type_ = typename TreeBoxND<N>::ChildID;
      using MortonNodeID = typename TreeBoxND<N>::MortonNodeID;
      auto node = typename TreeBoxND<N>::Node();
      Assert::IsFalse(node.IsAnyChildExist());

      autoce nChild = 1 << N;
      for (child_id_type_ idChild = 0; idChild < nChild; ++idChild)
      {
        autoc kChild = MortonNodeID(idChild);
        node.AddChild(kChild);
        Assert::IsTrue(node.HasChild(kChild));
        Assert::IsTrue(node.IsAnyChildExist());

        autoc vChild = node.GetChildren();
        Assert::AreEqual<size_t>(1, vChild.size());
        Assert::AreEqual(kChild, vChild[0]);

        node.RemoveChild(kChild);
        autoc vChild2 = node.GetChildren();
        Assert::AreEqual<size_t>(0, vChild2.size());
      }
    }

  public:
    TEST_METHOD(Complex_1D_Only1) { _complex_ND_Only1<1>(); }
    TEST_METHOD(Complex_2D_Only1) { _complex_ND_Only1<2>(); }
    TEST_METHOD(Complex_3D_Only1) { _complex_ND_Only1<3>(); }
    TEST_METHOD(Complex_4D_Only1) { _complex_ND_Only1<4>(); }

    TEST_METHOD(Complex_16D_Only1) { _complex_ND_Only1<16>(); }
    //TEST_METHOD(Complex_24D_Only1) { Complex_ND_Only1<24>(); }


  private:
    template<dim_t N>
    static void _complex_All_ND()
    {
      using ChildID = typename TreeBoxND<N>::ChildID;
      using MortonNodeID = typename TreeBoxND<N>::MortonNodeID;
      auto node = typename TreeBoxND<N>::Node();

      ChildID constexpr nChild = 1 << N;
      for (ChildID idChild = 0; idChild < nChild; ++idChild)
      {
        autoc kChild = MortonNodeID(idChild);
        node.AddChild(kChild);
        Assert::IsTrue(node.HasChild(kChild));
        Assert::IsTrue(node.IsAnyChildExist());

        autoc vChild = node.GetChildren();
        Assert::AreEqual<size_t>(static_cast<size_t>(idChild) + 1, vChild.size());
      }

      for (ChildID idChild = 0; idChild < nChild; ++idChild)
      {
        autoc kChild = MortonNodeID(idChild);

        node.RemoveChild(kChild);
        autoc vChildActual = node.GetChildren();
        auto vChildExpected = vector<ChildID>(static_cast<size_t>(nChild - idChild) - 1);
        std::iota(begin(vChildExpected), end(vChildExpected), idChild + 1);
        Assert::IsTrue(std::ranges::is_permutation(vChildExpected, vChildActual));
      }

      Assert::IsFalse(node.IsAnyChildExist());
    }

  public:
    TEST_METHOD(Complex_1D_All) { _complex_All_ND<1>(); }
    TEST_METHOD(Complex_2D_All) { _complex_All_ND<2>(); }
    TEST_METHOD(Complex_3D_All) { _complex_All_ND<3>(); }
    TEST_METHOD(Complex_4D_All) { _complex_All_ND<4>(); }

    TEST_METHOD(Complex_8D_All) { _complex_All_ND<8>(); }
    //TEST_METHOD(Complex_24D_All) { Complex_All_ND<24>(); }
  };


  TEST_CLASS(BasicFunctionsTest)
  {
    using MortonNodeID = DualtreePoint::MortonNodeID;

    TEST_METHOD(Ctor_Point_SameAsCreate__True)
    {
      autoce N = 16;
      autoce vPoint = getPointSetNo1<N>();
      autoc treeExpected = TreePointND<N>(vPoint, 3);
      autoc treeActual = TreePointND<N>(vPoint, 3);

      autoc& nodesE = treeExpected.GetNodes();
      autoc& nodesA = treeActual.GetNodes();
      Assert::AreEqual(nodesE.size(), nodesA.size());

      autoc vidE = treeExpected.CollectAllIdInBFS();
      autoc vidA = treeActual.CollectAllIdInBFS();
      Assert::IsTrue(vidE == vidA);
    }

    TEST_METHOD(Ctor_Box_SameAsCreate__True)
    {
      autoce N = 16;
      using BoundingBoxXD = BoundingBoxND<N>;
      autoce vBox = array
      {
        BoundingBoxXD{ 0.0, 4.0 },
        BoundingBoxXD{ 0.0, 2.0 }, BoundingBoxXD{ 2.0, 4.0 },
        BoundingBoxXD{ 0.0, 1.0 }, BoundingBoxXD{ 1.0, 2.0 }, BoundingBoxXD{ 2.0, 3.0 }, BoundingBoxXD{ 3.0, 4.0 }
      };

      autoc treeExpected = TreeBoxND<N>(vBox, 3);
      autoc treeActual = TreeBoxND<N>(vBox, 3);

      autoc& nodesE = treeExpected.GetNodes();
      autoc& nodesA = treeActual.GetNodes();
      Assert::AreEqual(nodesE.size(), nodesA.size());

      autoc vidE = treeExpected.CollectAllIdInBFS();
      autoc vidA = treeActual.CollectAllIdInBFS();
      Assert::IsTrue(vidE == vidA);
    }

    TEST_METHOD(EstimateNodeNumber__0)
    {
      //!
    }

    TEST_METHOD(EstimateNodeNumber__10)
    {
      //!
    }

    TEST_METHOD(GetHash__00_1)
    {
      Assert::AreEqual(DualtreePoint::MortonNodeID{ 1 }, DualtreePoint::GetHash(0, 0));
    }

    TEST_METHOD(GetHash__11_3)
    {
      Assert::AreEqual(DualtreePoint::MortonNodeID{ 3 }, DualtreePoint::GetHash(1, 1));
    }
    TEST_METHOD(GetHash__22_4)
    {
      Assert::AreEqual(DualtreePoint::MortonNodeID{ 6 }, DualtreePoint::GetHash(2, 2));
    }

    TEST_METHOD(GetHash__37_15)
    {
      Assert::AreEqual(DualtreePoint::MortonNodeID{ 15 }, DualtreePoint::GetHash(3, 7));
    }

    TEST_METHOD(GetDepth__37_15__3)
    {
      autoc lc = DualtreePoint::GetDepthID(DualtreePoint::GetHash(3, 7));
      Assert::AreEqual(depth_t{ 3 }, lc);
    }

    TEST_METHOD(RemoveSentinelBit__37_15__7)
    {
      autoc lc = DualtreePoint::RemoveSentinelBit(DualtreePoint::GetHash(3, 7));
      Assert::AreEqual(MortonNodeID{ 7 }, lc);
    }

    TEST_METHOD(Init)
    {
      auto tree = DualtreePoint{};
      autoc bb = BoundingBox1D{ -1, +1 };
      tree.Init(bb, 3, 10);
      Assert::IsTrue(AreEqualAlmost(bb, tree.GetBox()));

      autoc& nodes = tree.GetNodes();
      Assert::AreEqual<size_t>(1, nodes.size());
      Assert::AreEqual<depth_t>(3, tree.GetDepthMax());
      Assert::AreEqual<GridID>(8, tree.GetResolutionMax());
    }

    TEST_METHOD(InitThenInsert)
    {
      auto tree = DualtreeBoxC{};
      autoc handledSpaceDomain = BoundingBox1D{ -2, +2 };
      tree.Init(handledSpaceDomain, 3, 10);

      // Trying to add into the lead nodes
      {
        autoc boxes = array
        {
          BoundingBox1D{ -2.0, -1.0 },   // Fit in the leaf node
          BoundingBox1D{ -1.0,  0.0 },    // Fit in the leaf node
          BoundingBox1D{  0.0,  1.0 },    // Fit in the leaf node
          BoundingBox1D{  1.0,  2.0 },    // Fit in the leaf node
          BoundingBox1D{ -1.5,  1.5 } // Only fit in a parent node
        };

        for (autoc Box : boxes)
        {
          autoc isInsertedSuccessfully = tree.Add(Box, true /* Insert into leaf */);
          Assert::IsTrue(isInsertedSuccessfully);
        }
      }

      // Adding nodes in the current structure
      {
        autoc boxes = array
        {
          BoundingBox1D{ -1.5, -1.2 },    // Fit in the leaf node
          BoundingBox1D{ -1.2,  0.2 },    // Not fit in the leaf node
          BoundingBox1D{  0.0,  1.0 },    // Fit in the leaf node
          BoundingBox1D{ -1.1,  1.2 }     // Only fit in the root
        };

        for (autoc Box : boxes)
        {
          autoc isInsertedSuccessfully = tree.Add(Box, false /* Insert into the previously defined nodes */);
          Assert::IsTrue(isInsertedSuccessfully);
        }
      }


      // Outside of the handled domain
      {
        autoc boxIsNotInTheHandledSpace = BoundingBox1D{ 1, 3 }; // Min point inside, max point outside
        autoc isInsertedSuccessfully = tree.Add(boxIsNotInTheHandledSpace);
        Assert::IsFalse(isInsertedSuccessfully);
      }

      autoc& nodes = tree.GetCore().GetNodes();
      Assert::AreEqual<size_t>(7, nodes.size());

      autoc idsActual = tree.RangeSearch<false /*overlap instead of fully contained*/>(BoundingBox1D{ -1.1, 0.9 });
      autoc idsExpected = vector<size_t>{ /* 1. phase */ 0, 1, 2, 4, /* 2. phase */ 6, 7, 8 };
      Assert::IsTrue(std::ranges::is_permutation(idsActual, idsExpected));
    }


    TEST_METHOD(EraseFromEmpty)
    {
      autoc handledSpaceDomain = BoundingBox1D{ -2, +2 };

      auto tree = DualtreeBoxC{};
      tree.Reset();
      tree.Init(handledSpaceDomain, 10, 4);
      autoc isErased = tree.Erase(0);
      Assert::IsFalse(isErased);
    }

    TEST_METHOD(VisitNodes__points__0123)
    {
      autoce vpt = array{ Point1D{ 0.0 }, Point1D{ 1.0 }, Point1D{ 2.0 }, Point1D{ 3.0 } };
      autoc tree = DualtreePoint(vpt, 2, std::nullopt, 2);

      auto ids = vector<size_t>();
      tree.VisitNodes(tree.GetRootKey()
        , [&ids](autoc, autoc& node) { ids.insert(end(ids), begin(node.Entities), end(node.Entities)); }
      );

      Assert::IsTrue(ids == vector<size_t>{0, 1, 2, 3 });
    }

    TEST_METHOD(VisitNodes__boxes__0123)
    {
      autoce vBox = array
      {
        BoundingBox1D{ 0.0, 1.0 }, BoundingBox1D{ 1.0, 2.0 }, BoundingBox1D{ 2.0, 3.0 }, BoundingBox1D{ 3.0, 4.0 },
        BoundingBox1D{ 0.0, 2.0 }, BoundingBox1D{ 2.0, 4.0 },
        BoundingBox1D{ 0.0, 4.0 }
      };
      autoc tree = DualtreeBox(vBox, 3, std::nullopt, 2);

      auto ids = vector<size_t>();
      tree.VisitNodes(tree.GetRootKey()
        , [&ids](autoc, autoc& node) { ids.insert(end(ids), begin(node.Entities), end(node.Entities)); }
      );

      Assert::IsTrue(ids == vector<size_t>{6, 4, 5, 0, 1, 2, 3 });
    }


    TEST_METHOD(EraseId__2__EmptyNode11)
    {
      autoce vpt = array{ Point1D{ 0.0 }, Point1D{ 1.0 }, Point1D{ 2.0 }, Point1D{ 3.0 } };
      auto tree = DualtreePoint(vpt, 2, std::nullopt, 2);

      autoc kNode = tree.GetHash(2, 2);
      autoc& node = tree.GetNode(kNode);
      Assert::AreEqual<size_t>(node.Entities.size(), 1);
      tree.EraseId(2);
      Assert::IsTrue(node.Entities.empty());

      autoc kNode3 = tree.GetHash(2, 3);
      autoc& node3 = tree.GetNode(kNode3);
      Assert::AreEqual<std::size_t>(node3.Entities[0], 2);
    }


    TEST_METHOD(UpdateIndexes__None__NothingsChange)
    {
      autoce vBox = array
      {
        BoundingBox1D{ 0.0, 1.0 }, BoundingBox1D{ 1.0, 2.0 }, BoundingBox1D{ 2.0, 3.0 }, BoundingBox1D{ 3.0, 4.0 },
        BoundingBox1D{ 0.0, 2.0 }, BoundingBox1D{ 2.0, 4.0 },
        BoundingBox1D{ 0.0, 4.0 }
      };
      auto tree = DualtreeBox(vBox, 3, std::nullopt, 2);
      tree.UpdateIndexes({});

      auto ids = vector<size_t>();
      tree.VisitNodes(tree.GetRootKey()
        , [&ids](autoc, autoc& node) { ids.insert(end(ids), begin(node.Entities), end(node.Entities)); }
      );

      Assert::IsTrue(ids == vector<size_t>{6, 4, 5, 0, 1, 2, 3 });
    }


    TEST_METHOD(UpdateIndexes__2to7__6450173)
    {
      autoce vBox = array
      {
        BoundingBox1D{ 0.0, 1.0 }, BoundingBox1D{ 1.0, 2.0 }, BoundingBox1D{ 2.0, 3.0 }, BoundingBox1D{ 3.0, 4.0 },
        BoundingBox1D{ 0.0, 2.0 }, BoundingBox1D{ 2.0, 4.0 },
        BoundingBox1D{ 0.0, 4.0 }
      };
      auto tree = DualtreeBox(vBox, 3, std::nullopt, 2);
      tree.UpdateIndexes({ { 2, 7 } });

      auto ids = vector<size_t>();
      tree.VisitNodes(tree.GetRootKey()
        , [&ids](autoc, autoc& node) { ids.insert(end(ids), begin(node.Entities), end(node.Entities)); }
      );

      Assert::IsTrue(ids == vector<size_t>{6, 4, 5, 0, 1, 7, 3 });
    }

    TEST_METHOD(UpdateIndexes__2toMax__2Removed)
    {
      autoce vBox = array
      {
        BoundingBox1D{ 0.0, 1.0 }, BoundingBox1D{ 1.0, 2.0 }, BoundingBox1D{ 2.0, 3.0 }, BoundingBox1D{ 3.0, 4.0 },
        BoundingBox1D{ 0.0, 2.0 }, BoundingBox1D{ 2.0, 4.0 },
        BoundingBox1D{ 0.0, 4.0 }
      };
      auto tree = DualtreeBox(vBox, 3, std::nullopt, 2);
      tree.UpdateIndexes({ { 2, DualtreeBox::UpdateID::ERASE } });

      auto ids = vector<size_t>();
      tree.VisitNodes(tree.GetRootKey()
        , [&ids](autoc, autoc& node) { ids.insert(end(ids), begin(node.Entities), end(node.Entities)); }
      );

      Assert::IsTrue(ids == vector<size_t>{6, 4, 5, 0, 1, 3 });
    }

    TEST_METHOD(UpdateIndexes__Swap3And6)
    {
      autoce vBox = array
      {
        BoundingBox1D{ 0.0, 1.0 }, BoundingBox1D{ 1.0, 2.0 }, BoundingBox1D{ 2.0, 3.0 }, BoundingBox1D{ 3.0, 4.0 },
        BoundingBox1D{ 0.0, 2.0 }, BoundingBox1D{ 2.0, 4.0 },
        BoundingBox1D{ 0.0, 4.0 }
      };
      auto tree = DualtreeBox(vBox, 3, std::nullopt, 2);
      tree.UpdateIndexes(
        {
          { 3, 6 },
          { 6, 3 },
        }
      );


      auto ids = vector<size_t>();
      tree.VisitNodes(tree.GetRootKey()
        , [&ids](autoc, autoc& node) { ids.insert(end(ids), begin(node.Entities), end(node.Entities)); }
      );

      Assert::IsTrue(ids == vector<size_t>{3, 4, 5, 0, 1, 2, 6 });
    }

    TEST_METHOD(UpdateIndexes__Swap0And1)
    {
      autoce vBox = array
      {
        BoundingBox1D{ 0.0, 1.0 }, BoundingBox1D{ 1.0, 2.0 }, BoundingBox1D{ 2.0, 3.0 }, BoundingBox1D{ 3.0, 4.0 },
        BoundingBox1D{ 0.0, 2.0 }, BoundingBox1D{ 2.0, 4.0 },
        BoundingBox1D{ 0.0, 4.0 }
      };
      auto tree = DualtreeBox(vBox, 3, std::nullopt, 2);
      tree.UpdateIndexes(
        {
          { 1, 0 },
          { 0, 1 },
        }
      );


      auto ids = vector<size_t>();
      tree.VisitNodes(tree.GetRootKey()
        , [&ids](autoc, autoc& node) { ids.insert(end(ids), begin(node.Entities), end(node.Entities)); }
      );

      Assert::IsTrue(ids == vector<size_t>{6, 4, 5, 1, 0, 2, 3 });
    }


    TEST_METHOD(UpdateIndexes__3to4_4to5_5to6_6Removed)
    {
      autoce vBox = array
      {
        BoundingBox1D{ 0.0, 1.0 }, BoundingBox1D{ 1.0, 2.0 }, BoundingBox1D{ 2.0, 3.0 }, BoundingBox1D{ 3.0, 4.0 },
        BoundingBox1D{ 0.0, 2.0 }, BoundingBox1D{ 2.0, 4.0 },
        BoundingBox1D{ 0.0, 4.0 }
      };
      auto tree = DualtreeBox(vBox, 3, std::nullopt, 2);
      tree.UpdateIndexes(
        {
          { 6, 3 },
          { 3, 6 },
          { 4, 5 },
          { 5, 1 },
          { 1, DualtreeBox::UpdateID::ERASE },
        }
      );

      autoc ids = tree.CollectAllIdInBFS();

      Assert::IsTrue(ids == vector<size_t>{3, 5, 1, 0, 2, 6 }); // instead of { 6, 4, 5, 0, 1, 2, 3 }
    }

    TEST_METHOD(Clear__EmptyRootRemains)
    {
      autoce vBox = array
      {
        BoundingBox1D{ 0.0, 4.0 },
        BoundingBox1D{ 0.0, 2.0 }, BoundingBox1D{ 2.0, 4.0 },
        BoundingBox1D{ 0.0, 1.0 }, BoundingBox1D{ 1.0, 2.0 }, BoundingBox1D{ 2.0, 3.0 }, BoundingBox1D{ 3.0, 4.0 }
      };
      auto tree = DualtreeBox(vBox, 3, std::nullopt, 2);

      autoc& nodes = tree.GetNodes();
      Assert::AreEqual<size_t>(7, nodes.size());

      tree.Clear();
      Assert::AreEqual<size_t>(1, nodes.size());
      autoc node = nodes.at(tree.GetHash(0, 0));
      Assert::AreEqual<size_t>(1, node.Entities.empty());
    }


    TEST_METHOD(GetExtent_)
    {
      //!
    }

    TEST_METHOD(Contains_EmptyTree__False)
    {
      autoc tree = DualtreePoint({}, 3, std::nullopt, 2);
      autoc isPointContained = tree.Contains({}, {}, 1.0);
      Assert::IsFalse(isPointContained);
    }

  private:
    template<dim_t N>
    bool _isOnePointTreeContains()
    {
      using PointXD = PointND<N>;
      autoce vPoint = std::array<PointXD, 1>{ PointXD{ 1.1 } };
      autoc tree = TreePointND<N>(vPoint, 3);
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
      autoce vPoint = getPointSetNo1<N>();
      autoc tree = TreePointND<N>(vPoint, 3);
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
      autoce N = 16;
      autoce vPoint = getPointSetNo1<N>();
      autoc tree = TreePointND<N>(vPoint, 3);
      autoc fContain = tree.Contains(PointND<N>{-1.0, -1.0 }, vPoint, 0.0);

      Assert::IsFalse(fContain);
    }

    TEST_METHOD(Contains_PointSetNo1_3D__False)
    {
      autoce N = 3;
      autoce vPoint = getPointSetNo1<N>();
      autoc tree = TreePointND<N>(vPoint, 3);
      autoc fContain = tree.Contains(PointND<N>{7.0, 9.0 }, vPoint, 0.0);

      Assert::IsFalse(fContain);
    }


    TEST_METHOD(Move__Empty__NotCrash)
    {
      auto tree = DualtreePoint();
      tree.Move({});
    }


  private:
    template<typename tree_type, size_t N>
    bool _isMoveOfTwoTreeProper(tree_type const& tPre, tree_type const& tAfter, PointND<N> const& vMoveExpected)
    {
      using AD = AdaptorGeneral<N, VectorND<N>, BoundingBoxND<N>, RayND<N>, PlaneND<N>>;
      autoce rAcc = std::numeric_limits<double>::min();

      autoc nodesPre = tPre.GetNodes();
      autoc nodesAfter = tAfter.GetNodes();

      autoc nNode = nodesPre.size();
      auto vMatch = vector<bool>(nNode);
      std::transform(std::begin(nodesPre), std::end(nodesPre), std::begin(nodesAfter), begin(vMatch), [&](autoc& pairPre, autoc& pairAfter)
      {
        // same order, same box sizes

        if (pairPre.first != pairAfter.first)
          return false;

        autoc& nodePre = pairPre.second;
        autoc& nodeAfter = pairAfter.second;

        auto vMoveActualMin = PointND<N>{};
        auto vMoveActualMax = PointND<N>{};
        for (dim_t dimensionID = 0; dimensionID < N; ++dimensionID)
        {
          AD::SetPointC(vMoveActualMin, dimensionID, AD::GetBoxMinC(nodeAfter.Box, dimensionID) - AD::GetBoxMinC(nodePre.Box, dimensionID));
          AD::SetPointC(vMoveActualMax, dimensionID, AD::GetBoxMaxC(nodeAfter.Box, dimensionID) - AD::GetBoxMaxC(nodePre.Box, dimensionID));
        }

        autoc bMin = AD::ArePointsEqual(vMoveActualMin, vMoveExpected, rAcc);
        autoc bMax = AD::ArePointsEqual(vMoveActualMax, vMoveExpected, rAcc);
        return bMin && bMax;
      });

      return std::ranges::all_of(vMatch, [](autoc bMatch) { return bMatch; });
    }


  public:
    TEST_METHOD(MoveP__P0__Same)
    {
      autoce vpt = array{ Point1D{ 0.0 }, Point1D{ 1.0 }, Point1D{ 2.0 }, Point1D{ 3.0 } };
      autoc treePre = DualtreePoint(vpt, 3, std::nullopt, 2);

      auto treeAfter = treePre;
      autoce vMove = Point1D{ 0.0 };
      treeAfter.Move(vMove);

      Assert::IsTrue(_isMoveOfTwoTreeProper(treePre, treeAfter, vMove));
    }


    TEST_METHOD(MoveP__P1__Moved)
    {
      autoce vpt = array{ Point1D{ 0.0 }, Point1D{ 1.0 }, Point1D{ 2.0 }, Point1D{ 3.0 } };
      autoc treePre = DualtreePoint(vpt, 3, std::nullopt, 2);

      auto treeAfter = treePre;
      autoce vMove = Point1D{ 1.0 };
      treeAfter.Move(vMove);

      Assert::IsTrue(_isMoveOfTwoTreeProper(treePre, treeAfter, vMove));
    }


    TEST_METHOD(MoveB__M20__Moved)
    {
      autoce vBox = array
      {
        BoundingBox1D{ 0.0, 1.0 }, BoundingBox1D{ 1.0, 2.0 }, BoundingBox1D{ 2.0, 3.0 }, BoundingBox1D{ 3.0, 4.0 },
        BoundingBox1D{ 0.0, 2.0 }, BoundingBox1D{ 2.0, 4.0 },
        BoundingBox1D{ 0.0, 4.0 }
      };
      auto treePre = DualtreeBox(vBox, 3, std::nullopt, 2);

      auto treeAfter = treePre;
      autoce vMove = Point1D{ -20.0 };
      treeAfter.Move(vMove);

      Assert::IsTrue(_isMoveOfTwoTreeProper(treePre, treeAfter, vMove));
    }


    TEST_METHOD(RayIntersectedFirst__XRay__None)
    {
      autoce rayBase = Point2D{ 0.0, -1.0 };
      autoce rayHeading = Point2D{ 1.0, 0.0 };
      autoce boxes = array
      {
        BoundingBox2D{ { 0.0, 0.0 }, { 1.0, 1.0 } },
        BoundingBox2D{ { 1.0, 1.0 }, { 2.0, 2.0 } },
        BoundingBox2D{ { 2.0, 2.0 }, { 3.0, 3.0 } },
        BoundingBox2D{ { 3.0, 3.0 }, { 4.0, 4.0 } },
      };
      autoc qt = QuadtreeBox(boxes, 3, std::nullopt, 2);
      autoc oid = qt.RayIntersectedFirst(rayBase, rayHeading, boxes, 0);
      Assert::IsFalse(oid.has_value());
    }


    TEST_METHOD(RayIntersectedFirst__YRay__None)
    {
      autoce rayBase = Point2D{ 5.0, 0.0 };
      autoce rayHeading = Point2D{ 0.0, 1.0 };
      autoce boxes = array
      {
        BoundingBox2D{ { 0.0, 0.0 }, { 1.0, 1.0 } },
        BoundingBox2D{ { 1.0, 1.0 }, { 2.0, 2.0 } },
        BoundingBox2D{ { 2.0, 2.0 }, { 3.0, 3.0 } },
        BoundingBox2D{ { 3.0, 3.0 }, { 4.0, 4.0 } },
      };
      autoc qt = QuadtreeBox(boxes, 3, std::nullopt, 2);
      autoc oid = qt.RayIntersectedFirst(rayBase, rayHeading, boxes, 0);
      Assert::IsFalse(oid.has_value());
    }


    TEST_METHOD(RayIntersectedFirst__YRay2__None)
    {
      autoce rayBase = Point2D{ 4.2, 4.4 };
      autoce rayHeading = Point2D{ 1.0, 1.0 };
      autoce boxes = array
      {
        BoundingBox2D{ { 0.0, 0.0 }, { 1.0, 1.0 } },
        BoundingBox2D{ { 1.0, 1.0 }, { 2.0, 2.0 } },
        BoundingBox2D{ { 2.0, 2.0 }, { 3.0, 3.0 } },
        BoundingBox2D{ { 3.0, 3.0 }, { 4.0, 4.0 } },
      };
      autoc qt = QuadtreeBox(boxes, 3, std::nullopt, 2);
      autoc oid = qt.RayIntersectedFirst(rayBase, rayHeading, boxes, 0);
      Assert::IsFalse(oid.has_value());
    }


    TEST_METHOD(RayIntersectedFirst_InsideTheTree_Neg_None)
    {
      autoce rayBase = Point2D{ 1.5, 2.6 };
      autoce rayHeading = Point2D{ -1.0, -1.0 };
      autoce boxes = array
      {
        BoundingBox2D{ { 0.0, 0.0 }, { 1.0, 1.0 } },
        BoundingBox2D{ { 1.0, 1.0 }, { 2.0, 2.0 } },
        BoundingBox2D{ { 2.0, 2.0 }, { 3.0, 3.0 } },
        BoundingBox2D{ { 3.0, 3.0 }, { 4.0, 4.0 } },
      };
      autoc qt = QuadtreeBox(boxes, 3, std::nullopt, 2);
      autoc oid = qt.RayIntersectedFirst(rayBase, rayHeading, boxes, 0);
      Assert::IsFalse(oid.has_value());
    }


    TEST_METHOD(RayIntersectedFirst_OutsideTheTree_Neg_None)
    {
      autoce rayBase = Point2D{ 7.0, 2.0 };
      autoce rayHeading = Point2D{ -1.0, -1.0 };
      autoce boxes = array
      {
        BoundingBox2D{ { 0.0, 0.0 }, { 1.0, 1.0 } },
        BoundingBox2D{ { 1.0, 1.0 }, { 2.0, 2.0 } },
        BoundingBox2D{ { 2.0, 2.0 }, { 3.0, 3.0 } },
        BoundingBox2D{ { 3.0, 3.0 }, { 4.0, 4.0 } },
      };
      autoc qt = QuadtreeBox(boxes, 3, std::nullopt, 2);
      autoc oid = qt.RayIntersectedFirst(rayBase, rayHeading, boxes, 0);
      Assert::IsFalse(oid.has_value());
    }

    TEST_METHOD(RayIntersectedFirst__XRay__0)
    {
      autoce rayBase = Point2D{ 0.0, 0.5 };
      autoce rayHeading = Point2D{ 1.0, 0.0 };
      autoce boxes = array
      {
        BoundingBox2D{ { 0.0, 0.0 }, { 1.0, 1.0 } },
        BoundingBox2D{ { 1.0, 1.0 }, { 2.0, 2.0 } },
        BoundingBox2D{ { 2.0, 2.0 }, { 3.0, 3.0 } },
        BoundingBox2D{ { 3.0, 3.0 }, { 4.0, 4.0 } },
      };
      autoc qt = QuadtreeBox(boxes, 3, std::nullopt, 2);
      autoc oid = qt.RayIntersectedFirst(rayBase, rayHeading, boxes, 0);
      Assert::AreEqual<std::size_t>(0, oid.value());
    }


    TEST_METHOD(RayIntersectedFirst__XRay__1)
    {
      autoce rayBase = Point2D{ 0.0, 2.0 };
      autoce rayHeading = Point2D{ 1.0, 0.0 };
      autoce boxes = array
      {
        BoundingBox2D{ { 0.0, 0.0 }, { 1.0, 1.0 } },
        BoundingBox2D{ { 1.0, 1.0 }, { 2.0, 2.0 } },
        BoundingBox2D{ { 2.0, 2.0 }, { 3.0, 3.0 } },
        BoundingBox2D{ { 3.0, 3.0 }, { 4.0, 4.0 } },
      };
      autoc qt = QuadtreeBox(boxes, 3, std::nullopt, 2);
      autoc oid = qt.RayIntersectedFirst(rayBase, rayHeading, boxes, 0);
      Assert::AreEqual<std::size_t>(1, oid.value());
    }


    TEST_METHOD(RayIntersectedFirst__XRayInsideTheTreePos__1)
    {
      autoce rayBase = Point2D{ 1.5, 1.0 };
      autoce rayHeading = Point2D{ 1.0, 0.0 };
      autoce boxes = array
      {
        BoundingBox2D{ { 0.0, 0.0 }, { 1.0, 1.0 } },
        BoundingBox2D{ { 1.0, 1.0 }, { 2.0, 2.0 } },
        BoundingBox2D{ { 2.0, 2.0 }, { 3.0, 3.0 } },
        BoundingBox2D{ { 3.0, 3.0 }, { 4.0, 4.0 } },
      };
      autoc qt = QuadtreeBox(boxes, 3, std::nullopt, 2);
      autoc oid = qt.RayIntersectedFirst(rayBase, rayHeading, boxes, 0);
      Assert::AreEqual<std::size_t>(1, oid.value());
    }


    TEST_METHOD(RayIntersectedFirst__XRayInsideTheTreeNeg__1)
    {
      autoce rayBase = Point2D{ 1.5, 1.0 };
      autoce rayHeading = Point2D{ -1.0, 0.0 };
      autoce boxes = array
      {
        BoundingBox2D{ { 0.0, 0.0 }, { 1.0, 1.0 } },
        BoundingBox2D{ { 1.0, 1.0 }, { 2.0, 2.0 } },
        BoundingBox2D{ { 2.0, 2.0 }, { 3.0, 3.0 } },
        BoundingBox2D{ { 3.0, 3.0 }, { 4.0, 4.0 } },
      };
      autoc qt = QuadtreeBox(boxes, 3, std::nullopt, 2);
      autoc oid = qt.RayIntersectedFirst(rayBase, rayHeading, boxes, 0);
      Assert::AreEqual<std::size_t>(1, oid.value());
    }


    TEST_METHOD(RayIntersectedFirst__XRayInsideTheTreeNeg__3)
    {
      autoce rayBase = Point2D{ 3.5, 3.5 };
      autoce rayHeading = Point2D{ -0.5, -1.0 };
      autoce boxes = array
      {
        BoundingBox2D{ { 0.0, 0.0 }, { 1.0, 1.0 } },
        BoundingBox2D{ { 1.0, 1.0 }, { 2.0, 2.0 } },
        BoundingBox2D{ { 2.0, 2.0 }, { 3.0, 3.0 } },
        BoundingBox2D{ { 3.0, 3.0 }, { 4.0, 4.0 } },
      };
      autoc qt = QuadtreeBox(boxes, 3, std::nullopt, 2);
      autoc oid = qt.RayIntersectedFirst(rayBase, rayHeading, boxes, 0);
      Assert::AreEqual<std::size_t>(3, oid.value());
    }


    TEST_METHOD(RayIntersectedAll_2D_General_234)
    {
      autoce rayBase = Point2D{ 2.7, 2.6 };
      autoce rayHeading = Point2D{ 1.0, 1.0 };

      autoce boxes = array
      {
        BoundingBox2D{ { 2.0, 0.0 }, { 3.0, 1.0 } },
        BoundingBox2D{ { 3.0, 1.0 }, { 4.0, 2.0 } },
        BoundingBox2D{ { 3.0, 2.0 }, { 4.0, 3.0 } },
        BoundingBox2D{ { 2.0, 3.0 }, { 4.0, 4.0 } },
        BoundingBox2D{ { 2.5, 2.5 }, { 3.5, 3.5 } },
      };

      autoc qt = QuadtreeBox(boxes, 3, std::nullopt, 2);
      autoc Entities = qt.RayIntersectedAll(rayBase, rayHeading, boxes, 0);
      Assert::IsTrue(vector<std::size_t>{ 4, 2, 3 } == Entities);
    }

    TEST_METHOD(RayIntersectedAll_2D_General_34)
    {
      autoce rayBase = Point2D{ 2.6, 2.8 };
      autoce rayHeading = Point2D{ 1.0, 1.0 };

      autoce boxes = array
      {
        BoundingBox2D{ { 2.0, 0.0 }, { 3.0, 1.0 } },
        BoundingBox2D{ { 3.0, 1.0 }, { 4.0, 2.0 } },
        BoundingBox2D{ { 3.0, 2.0 }, { 4.0, 3.0 } },
        BoundingBox2D{ { 2.0, 3.0 }, { 4.0, 4.0 } },
        BoundingBox2D{ { 2.5, 2.5 }, { 3.5, 3.5 } },
      };

      autoc qt = QuadtreeBox(boxes, 3, std::nullopt, 2);
      autoc Entities = qt.RayIntersectedAll(rayBase, rayHeading, boxes, 0);
      Assert::IsTrue(std::ranges::is_permutation(vector{ 3, 4 }, Entities));
    }

    TEST_METHOD(RayIntersectedAll_3D_General_34)
    {
      autoce rayBase = Point3D{ 2.6, 2.8, 1.0 };
      autoce rayHeading = Point3D{ 1.0, 1.0, 1.0 };

      autoce boxes = array
      {
        BoundingBox3D{ { 2.0, 0.0, 1.0 }, { 3.0, 1.0, 2.0 } },
        BoundingBox3D{ { 3.0, 1.0, 1.0 }, { 4.0, 2.0, 2.0 } },
        BoundingBox3D{ { 3.0, 2.0, 1.0 }, { 4.0, 3.0, 2.0 } },
        BoundingBox3D{ { 2.0, 3.0, 1.5 }, { 4.0, 4.0, 2.0 } },
        BoundingBox3D{ { 2.5, 2.5, 1.5 }, { 3.5, 3.5, 2.0 } },
      };

      autoc qt = OctreeBox(boxes, 3, std::nullopt, 2);
      autoc Entities = qt.RayIntersectedAll(rayBase, rayHeading, boxes, 0);
      Assert::IsTrue(std::ranges::is_permutation(vector{ 3, 4 }, Entities));
    }

    TEST_METHOD(RayIntersectedAll_5D_General_34)
    {
      autoce N = 5;
      autoce rayBase = PointND<N>{ 2.6, 2.8, 1.0, 0.0, 0.0 };
      autoce rayHeading = PointND<N>{ 1.0, 1.0, 1.0, 1.0, 1.0 };

      autoce boxes = array
      {
        BoundingBoxND<N>{ { 2.0, 0.0, 1.0, 0.0, 0.0 }, { 3.0, 1.0, 2.0, 2.0, 2.0 } },
        BoundingBoxND<N>{ { 3.0, 1.0, 1.0, 0.0, 0.0 }, { 4.0, 2.0, 2.0, 2.0, 2.0 } },
        BoundingBoxND<N>{ { 3.0, 2.0, 1.0, 0.0, 0.0 }, { 4.0, 3.0, 2.0, 2.0, 2.0 } },
        BoundingBoxND<N>{ { 2.0, 3.0, 1.5, 0.0, 0.0 }, { 4.0, 4.0, 2.0, 2.0, 2.0 } },
        BoundingBoxND<N>{ { 2.5, 2.5, 1.5, 0.0, 0.0 }, { 3.5, 3.5, 2.0, 2.0, 2.0 } },
      };

      autoc qt = TreeBoxND<N>(boxes, 3, std::nullopt, 2);
      autoc Entities = qt.RayIntersectedAll(rayBase, rayHeading, boxes, 0);
      Assert::IsTrue(std::ranges::is_permutation(vector{ 3, 4 }, Entities));
    }


  };
}

namespace Tree1DTest
{
	TEST_CLASS(Point_CreateTest)
	{
	public:
		
		TEST_METHOD(Empty)
		{
			autoc tree = DualtreePoint({}, 2);
			autoc& nodes = tree.GetNodes();
			Assert::IsTrue(nodes.size() == 1);
      Assert::IsTrue(nodes.at(1).Entities.empty());
      Assert::IsTrue(AreEqualAlmost(tree.GetBox(), BB1_INV));
		}

    // ext    //!
    TEST_METHOD(NoPt1)
    {
      autoc tree = DualtreePoint(vector<Point1D>{ { 1.0 } }, 2);
      autoc& nodes = tree.GetNodes();
      Assert::IsTrue(nodes.size() == 1);
      Assert::IsFalse(nodes.at(1).Entities.empty());
      Assert::IsTrue(AreEqualAlmost(tree.GetBox(), BoundingBox1D{ Point1D{1.0}, Point1D{1.0} }));
    }

    // ext     //!
    TEST_METHOD(NoPt2)
    {
      autoc tree = DualtreePoint({}, 2);
      autoc& nodes = tree.GetNodes();
      Assert::IsTrue(nodes.size() == 1);
      Assert::IsTrue(nodes.at(1).Entities.empty());
    }

    TEST_METHOD(NoPt4)
    {
      autoce vpt = array{ Point1D{ 0.0 }, Point1D{ 1.0 }, Point1D{ 2.0 }, Point1D{ 3.0 } };
      autoc tree = DualtreePoint(vpt, 2, std::nullopt, 2);

      Assert::AreEqual<size_t>(tree.GetNodes().size(), 7);
      Assert::IsTrue(AreEqualAlmost(tree.GetBox(), BoundingBox1D{ Point1D{0.0}, Point1D{3.0} }));
    }

    //!
    TEST_METHOD(Init)
    {
      auto tree = DualtreePoint{};
      tree.Init(BoundingBox1D{ { -1.0 }, { +1.0 } }, 2);
      
      autoc& nodes = tree.GetNodes();
      Assert::IsTrue(nodes.size() == 1);
      Assert::IsTrue(nodes.at(1).Entities.empty());
    }
	};


  TEST_CLASS(Point_SearchTest)
  {
    TEST_METHOD(FindSmallestNode_At30)
    {
      using MortonNodeID = DualtreePoint::MortonNodeID;

      autoce vpt = array{ Point1D{ 0.0 }, Point1D{ 1.0 }, Point1D{ 2.0 }, Point1D{ 3.0 } };
      autoc tree = DualtreePoint(vpt, 2, std::nullopt, 2);
      autoc kLast = tree.FindSmallestNode(vpt.back());
      Assert::AreEqual<MortonNodeID>(7, kLast);
    }

    TEST_METHOD(FindSmallestNode_At29)
    {
      using MortonNodeID = DualtreePoint::MortonNodeID;

      autoce vpt = array{ Point1D{ 0.0 }, Point1D{ 1.0 }, Point1D{ 2.0 }, Point1D{ 3.0 } };
      autoc tree = DualtreePoint(vpt, 2, std::nullopt, 2);
      autoc kLast = tree.FindSmallestNode(Point1D{ 2.9 });
      Assert::AreEqual<MortonNodeID>(7, kLast);
    }


    TEST_METHOD(FindSmallestNode_At15)
    {
      using MortonNodeID = DualtreePoint::MortonNodeID;

      autoce vpt = array{ Point1D{ 0.0 }, Point1D{ 1.0 }, Point1D{ 2.0 }, Point1D{ 3.0 } };
      autoc tree = DualtreePoint(vpt, 2, std::nullopt, 2);
      autoc kLast = tree.FindSmallestNode(Point1D{ 1.5 });
      Assert::AreEqual<MortonNodeID>(6, kLast);
    }

    TEST_METHOD(RangeSearch__EqualAtBorder__12)
    {
      autoce vpt = array{ Point1D{ 0.0 }, Point1D{ 1.0 }, Point1D{ 2.0 }, Point1D{ 3.0 } };
      autoc tree = DualtreePoint(vpt, 2, std::nullopt, 2);
      autoc ids = tree.RangeSearch({ 1.0, 2.0 }, vpt);

      Assert::IsTrue(std::ranges::is_permutation(vector{1, 2}, ids));
    }

    TEST_METHOD(RangeSearch__Usual__23)
    {
      autoce vpt = array{ Point1D{ 0.0 }, Point1D{ 1.0 }, Point1D{ 2.0 }, Point1D{ 3.0 } };
      autoc tree = DualtreePoint(vpt, 2, std::nullopt, 2);
      autoc ids = tree.RangeSearch({ 1.1, 3.1 }, vpt);

      Assert::IsTrue(std::ranges::is_permutation(vector{ 2, 3 }, ids));
    }

    TEST_METHOD(RangeSearch__AtNodeBorder__2)
    {
      autoce vpt = array{ Point1D{ 0.0 }, Point1D{ 1.0 }, Point1D{ 2.0 }, Point1D{ 3.0 } };
      autoc tree = DualtreePoint(vpt, 2, std::nullopt, 2);
      autoc ids = tree.RangeSearch({ 1.5, 2.25 }, vpt);

      Assert::IsTrue(std::ranges::is_permutation(vector{ 2 }, ids));
    }

    TEST_METHOD(RangeSearch__ZeroBox__Empty)
    {
      autoce vpt = array{ Point1D{ 0.0 }, Point1D{ 1.0 }, Point1D{ 2.0 }, Point1D{ 3.0 } };
      autoc tree = DualtreePoint(vpt, 2, std::nullopt, 2);
      autoc ids = tree.RangeSearch({}, vpt);

      Assert::IsTrue(ids.empty());
    }

    TEST_METHOD(RangeSearch__Empty)
    {
      autoce vpt = array{ Point1D{ 0.0 }, Point1D{ 1.0 }, Point1D{ 2.0 }, Point1D{ 3.0 } };
      autoc tree = DualtreePoint(vpt, 2, std::nullopt, 2);
      autoc ids = tree.RangeSearch({ 1.5, 1.75 }, vpt);

      Assert::IsTrue(ids.empty());
    }
  };


  TEST_CLASS(Point_EditTest)
  {
    TEST_METHOD(Insert__NonLeaf__Successful)
    {
      autoce vpt = array{ Point1D{ 0.0 }, Point1D{ 1.0 }, Point1D{ 2.0 }, Point1D{ 3.0 } };
      auto tree = DualtreePoint(vpt, 3, std::nullopt, 2);

      Assert::IsTrue(tree.Insert(4, Point1D{ 2.5 }, false));

      autoc& nodes = tree.GetNodes();
      Assert::AreEqual<size_t>(7, nodes.size());
      Assert::IsTrue(nodes.at(7).Entities == vector<size_t>{ 3, 4 });
    }

    TEST_METHOD(Insert__Leaf__Successful)
    {
      autoce vpt = array{ Point1D{ 0.0 }, Point1D{ 1.0 }, Point1D{ 2.0 }, Point1D{ 3.0 } };
      auto tree = DualtreePoint(vpt, 3, std::nullopt, 2);

      Assert::IsTrue(tree.Insert(4, Point1D{ 2.5 }, true));

      autoc& nodes = tree.GetNodes();
      Assert::AreEqual<size_t>(nodes.size(), 8);
      Assert::IsTrue(nodes.at(14).Entities == vector<size_t>{ 4 });
    }

    TEST_METHOD(Insert__OutOfSpace__ReturnsFalse)
    {
      autoce vpt = array{ Point1D{ 0.0 }, Point1D{ 1.0 }, Point1D{ 2.0 }, Point1D{ 3.0 } };
      auto tree = DualtreePoint(vpt, 3, std::nullopt, 2);
      Assert::IsFalse(tree.Insert(4, Point1D{ 4.0 }, true));

      autoc& nodes = tree.GetNodes();
      Assert::AreEqual<size_t>(nodes.size(), 7);
      Assert::IsTrue(nodes.at(7).Entities == vector<size_t>{ 3 });
    }


    TEST_METHOD(Update3p_)
    {
      //!
    }

    TEST_METHOD(Update2p__Rewrite3__Successful)
    {
      auto vpt = vector{ Point1D{ 0.0 }, Point1D{ 1.0 }, Point1D{ 2.0 }, Point1D{ 3.0 } };

      auto tree = DualtreePoint(vpt, 3, std::nullopt, 2);
      
      Assert::IsTrue(tree.Update(3, { 1.1 }));
      autoc ids = tree.CollectAllIdInBFS();
      Assert::IsTrue(ids == vector<std::size_t>{ 0, 1, 3, 2 });
    }

    TEST_METHOD(Update2p__Rewrite2__Successful)
    {
      auto vpt = vector{ Point1D{ 0.0 }, Point1D{ 1.0 }, Point1D{ 2.0 }, Point1D{ 3.0 } };

      auto tree = DualtreePoint(vpt, 3, std::nullopt, 2);

      Assert::IsTrue(tree.Update(2, { 1.1 }));
      autoc ids = tree.CollectAllIdInBFS();
      Assert::IsTrue(ids == vector<std::size_t>{ 0, 1, 2, 3 });
    }


    TEST_METHOD(Update2p__Rewrite3OutOfBox__ReturnsFalse)
    {
      auto vpt = vector{ Point1D{ 0.0 }, Point1D{ 1.0 }, Point1D{ 2.0 }, Point1D{ 3.0 } };

      auto tree = DualtreePoint(vpt, 3, std::nullopt, 2);

      autoc idsPre = tree.CollectAllIdInBFS();
      Assert::IsFalse(tree.Update(2, { 3.1 }));
      autoc idsPost = tree.CollectAllIdInBFS();

      Assert::IsTrue(idsPre == idsPost);
    }


    TEST_METHOD(Erase__3__Removed)
    {
      autoce vpt = array{ Point1D{ 0.0 }, Point1D{ 1.0 }, Point1D{ 2.0 }, Point1D{ 3.0 } };
      auto tree = DualtreePoint(vpt, 3, std::nullopt, 2);

      Assert::IsTrue(tree.Erase(3, vpt.back()));
      autoc ids = tree.CollectAllIdInBFS();
      Assert::IsTrue(std::ranges::find(ids, 3) == end(ids));
    }

    TEST_METHOD(Erase__Nonexist__ReturnsFalse)
    {
      autoce vpt = array{ Point1D{ 0.0 }, Point1D{ 1.0 }, Point1D{ 2.0 }, Point1D{ 3.0 } };
      auto tree = DualtreePoint(vpt, 3, std::nullopt, 2);

      autoc idsPre = tree.CollectAllIdInBFS();
      Assert::IsFalse(tree.Erase(4, vpt.back()));
      autoc idsPost = tree.CollectAllIdInBFS();

      Assert::IsTrue(idsPre == idsPost);
    }

    TEST_METHOD(Erase__WrongPointWasGiven__ReturnsFalse)
    {
      autoce vpt = array{ Point1D{ 0.0 }, Point1D{ 1.0 }, Point1D{ 2.0 }, Point1D{ 3.0 } };
      auto tree = DualtreePoint(vpt, 3, std::nullopt, 2);

      autoc idsPre = tree.CollectAllIdInBFS();
      Assert::IsFalse(tree.Erase(3, vpt.front()));
      autoc idsPost = tree.CollectAllIdInBFS();

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

    TEST_METHOD(SplitStrategy_Seq)
    {
      autoc boxes = array
      {
          BoundingBox2D{ { 0.0, 0.0 }, { 1.0, 1.0 } },
          BoundingBox2D{ { 1.0, 1.0 }, { 2.0, 2.0 } },
          BoundingBox2D{ { 2.0, 2.0 }, { 3.0, 3.0 } },
          BoundingBox2D{ { 3.0, 3.0 }, { 4.0, 4.0 } },
          BoundingBox2D{ { 1.2, 1.2 }, { 2.8, 2.8 } },
          BoundingBox2D{ { 1.2, 1.2 }, { 2.8, 1.8 } }, // 5; 19
          BoundingBox2D{ { 0.5, 0.0 }, { 1.5, 0.5 } }  // 16; 17

      };

      auto quadtreebox = QuadtreeBox(boxes, 2
        , std::nullopt // user-provided bounding Box for all
        , 2            // max element in a node 
      );


      autoc& nodes = quadtreebox.GetNodes();
      Assert::AreEqual<size_t>(9, nodes.size());

      Assert::IsTrue(std::ranges::is_permutation(vector<std::size_t>{ 4 }, quadtreebox.GetNode(1).Entities));
      Assert::IsTrue(quadtreebox.GetNode(4).Entities.empty());
      Assert::IsTrue(std::ranges::is_permutation(vector<std::size_t>{ 5 }, quadtreebox.GetNode(5).Entities));

      Assert::IsTrue(std::ranges::is_permutation(vector<std::size_t>{ 0, 6 }, quadtreebox.GetNode(16).Entities));
      Assert::IsTrue(std::ranges::is_permutation(vector<std::size_t>{ 6 }, quadtreebox.GetNode(17).Entities));
      Assert::IsTrue(std::ranges::is_permutation(vector<std::size_t>{ 1, 5 }, quadtreebox.GetNode(19).Entities));
      Assert::IsTrue(std::ranges::is_permutation(vector<std::size_t>{ 2 }, quadtreebox.GetNode(28).Entities));
      Assert::IsTrue(std::ranges::is_permutation(vector<std::size_t>{ 3 }, quadtreebox.GetNode(31).Entities));
    }


    TEST_METHOD(SplitStrategy_Par)
    {
      autoc boxes = array
      {
          BoundingBox2D{ { 0.0, 0.0 }, { 1.0, 1.0 } },
          BoundingBox2D{ { 1.0, 1.0 }, { 2.0, 2.0 } },
          BoundingBox2D{ { 2.0, 2.0 }, { 3.0, 3.0 } },
          BoundingBox2D{ { 3.0, 3.0 }, { 4.0, 4.0 } },
          BoundingBox2D{ { 1.2, 1.2 }, { 2.8, 2.8 } },
          BoundingBox2D{ { 1.2, 1.2 }, { 2.8, 1.8 } }, // 5; 19
          BoundingBox2D{ { 0.5, 0.0 }, { 1.5, 0.5 } }  // 16; 17

      };

      auto quadtreebox = QuadtreeBox{};
      QuadtreeBox::Create<std::execution::parallel_unsequenced_policy>(quadtreebox, boxes, 2
        , std::nullopt // user-provided bounding Box for all
        , 2            // max element in a node 
      );


      autoc& nodes = quadtreebox.GetNodes();
      Assert::AreEqual<size_t>(9, nodes.size());

      Assert::IsTrue(std::ranges::is_permutation(vector<std::size_t>{ 4 }, quadtreebox.GetNode(1).Entities));
      Assert::IsTrue(quadtreebox.GetNode(4).Entities.empty());
      Assert::IsTrue(std::ranges::is_permutation(vector<std::size_t>{ 5 }, quadtreebox.GetNode(5).Entities));

      Assert::IsTrue(std::ranges::is_permutation(vector<std::size_t>{ 0, 6 }, quadtreebox.GetNode(16).Entities));
      Assert::IsTrue(std::ranges::is_permutation(vector<std::size_t>{ 6 }, quadtreebox.GetNode(17).Entities));
      Assert::IsTrue(std::ranges::is_permutation(vector<std::size_t>{ 1, 5 }, quadtreebox.GetNode(19).Entities));
      Assert::IsTrue(std::ranges::is_permutation(vector<std::size_t>{ 2 }, quadtreebox.GetNode(28).Entities));
      Assert::IsTrue(std::ranges::is_permutation(vector<std::size_t>{ 3 }, quadtreebox.GetNode(31).Entities));
    }




    TEST_METHOD(CollistionDetection__0040_3565__P30)
    {
      autoce vBoxL = array{ BoundingBox1D{ 0.0, 1.0 }, BoundingBox1D{ 1.0, 2.0 }, BoundingBox1D{ 2.0, 3.0 }, BoundingBox1D{ 3.0, 4.0 } };
      autoc treeL = DualtreeBox(vBoxL, 3, std::nullopt, 2);

      autoce vBoxR = array{ BoundingBox1D{ 3.5, 4.5 },  BoundingBox1D{ 4.5, 5.5 },  BoundingBox1D{ 5.5, 6.5 } };
      autoc treeR = DualtreeBox(vBoxR, 3, std::nullopt, 2);

      autoc ret = DualtreeBox::CollisionDetection(treeL, vBoxL, treeR, vBoxR);

      autoce aExpected = std::pair{ 3, 0 };
      Assert::AreEqual<size_t>(ret.size(), 1);
      Assert::AreEqual<size_t>(ret[0].first, aExpected.first);
      Assert::AreEqual<size_t>(ret[0].second, aExpected.second);
    }


    TEST_METHOD(CollistionDetection__Complex1)
    {
      autoce vBoxL = array
      { 
        BoundingBox1D{ 0.0, 4.0 }, 
        BoundingBox1D{ 0.0, 2.0 }, BoundingBox1D{ 2.0, 4.0 }, 
        BoundingBox1D{ 0.0, 1.0 }, BoundingBox1D{ 1.0, 2.0 }, BoundingBox1D{ 2.0, 3.0 }, BoundingBox1D{ 3.0, 4.0 }
      };
      autoc treeL = DualtreeBox(vBoxL, 3, std::nullopt, 2);

      autoce vBoxR = array
      { 
        BoundingBox1D{ 2.0, 6.0 },
        BoundingBox1D{ 2.0, 4.0 }, BoundingBox1D{ 4.0, 6.0 }, 
        BoundingBox1D{ 2.0, 3.0 }, BoundingBox1D{ 3.0, 4.0 }, BoundingBox1D{ 4.0, 5.0 }, BoundingBox1D{ 5.0, 6.0 }
      };
      autoc treeR = DualtreeBox(vBoxR, 3, std::nullopt, 2);

      autoc vActual = DualtreeBox::CollisionDetection(treeL, vBoxL, treeR, vBoxR);

      autoce aExpected = array
      {
        std::pair{ 0, 0 }, // Level 0
        std::pair{ 0, 1 },
        std::pair{ 2, 0 }, // Level 1
        std::pair{ 2, 1 },
        std::pair{ 0, 3 }, // Level 2
        std::pair{ 2, 3 },
        std::pair{ 5, 0 },
        std::pair{ 5, 1 },
        std::pair{ 5, 3 }, // ==
        std::pair{ 0, 4 },
        std::pair{ 2, 4 },
        std::pair{ 6, 0 },
        std::pair{ 6, 1 },
        std::pair{ 6, 4 }, // ==
      };
      Assert::AreEqual<size_t>(aExpected.size(), vActual.size());
      Assert::IsTrue(std::ranges::is_permutation(aExpected, vActual, [](autoc& p1, autoc& p2) { return p1.first == p2.first && p1.second == p2.second; }));
    }

  };

  TEST_CLASS(Box_EditTest)
  {
    TEST_METHOD(Insert_IntoRoot_Successful)
    {
      autoce vBoxL = array{ BoundingBox1D{ 0.0, 1.0 }, BoundingBox1D{ 1.0, 2.0 }, BoundingBox1D{ 2.0, 3.0 }, BoundingBox1D{ 3.0, 4.0 } };
      auto tree = DualtreeBox(vBoxL, 3, std::nullopt, 2);

      Assert::IsTrue(tree.Insert(4, BoundingBox1D{ 0, 4 }, false));

      autoc& nodes = tree.GetNodes();
      Assert::AreEqual<size_t>(nodes.size(), 7);
      Assert::IsTrue(nodes.at(tree.GetRootKey()).Entities == vector<size_t>{ 4 });
    }


    TEST_METHOD(Insert_NonLeaf_Successful)
    {
      autoce vBoxL = array{ BoundingBox1D{ 0.0, 1.0 }, BoundingBox1D{ 1.0, 2.0 }, BoundingBox1D{ 2.0, 3.0 }, BoundingBox1D{ 3.0, 4.0 } };
      auto tree = DualtreeBox(vBoxL, 3, std::nullopt, 2);

      Assert::IsTrue(tree.Insert(4, BoundingBox1D{ 3.5, 3.7 }, false));

      autoc& nodes = tree.GetNodes();
      Assert::AreEqual<size_t>(nodes.size(), 7);
      Assert::IsTrue(nodes.at(7).Entities == vector<size_t>{ 3, 4 });
    }
    
    TEST_METHOD(Insert_Leaf_Successful)
    {
      autoce vBoxL = array{ BoundingBox1D{ 0.0, 1.0 }, BoundingBox1D{ 1.0, 2.0 }, BoundingBox1D{ 2.0, 3.0 }, BoundingBox1D{ 3.0, 4.0 } };
      auto tree = DualtreeBox(vBoxL, 3, std::nullopt, 2);

      Assert::IsTrue(tree.Insert(4, BoundingBox1D{ 3.5, 3.7 }, true));

      autoc& nodes = tree.GetNodes();
      Assert::AreEqual<size_t>(nodes.size(), 8);
      Assert::IsTrue(nodes.at(15).Entities == vector<size_t>{ 4 });
    }


    TEST_METHOD(Insert_LeafBut1_Successful)
    {
      autoce vBoxL = array{ BoundingBox1D{ 0.0, 1.0 }, BoundingBox1D{ 1.0, 2.0 }, BoundingBox1D{ 2.0, 3.0 }, BoundingBox1D{ 3.0, 4.0 } };
      auto tree = DualtreeBox(vBoxL, 3, std::nullopt, 2);

      Assert::IsTrue(tree.Insert(4, BoundingBox1D{ 1.0, 3.0 }, true));

      autoc& nodes = tree.GetNodes();
      Assert::AreEqual<size_t>(nodes.size(), 7);
      Assert::IsTrue(nodes.at(1).Entities == vector<size_t>{ 4 });
    }


    TEST_METHOD(Insert_LeafBut2_Successful)
    {
      autoce vBoxL = array{ BoundingBox1D{ 0.0, 1.0 }, BoundingBox1D{ 1.0, 2.0 }, BoundingBox1D{ 2.0, 3.0 }, BoundingBox1D{ 3.0, 4.0 } };
      auto tree = DualtreeBox(vBoxL, 3, std::nullopt, 2);

      Assert::IsTrue(tree.Insert(4, BoundingBox1D{ 0.0, 2.0 }, true));

      autoc& nodes = tree.GetNodes();
      Assert::AreEqual<size_t>(nodes.size(), 7);
      Assert::IsTrue(nodes.at(2).Entities == vector<size_t>{ 4 });
    }


    TEST_METHOD(Complex)
    {
      autoce vBoxL = array{ BoundingBox1D{ 0.0, 1.0 }, BoundingBox1D{ 1.0, 2.0 }, BoundingBox1D{ 2.0, 3.0 }, BoundingBox1D{ 3.0, 4.0 } };
      auto tree = DualtreeBox(vBoxL, 3, std::nullopt, 2);
      tree.Insert(4, BoundingBox1D{ 3.5, 3.7 }, false);
      tree.Insert(5, BoundingBox1D{ 3.5, 3.7 }, true);
      tree.EraseId(3);
      tree.Update(4, BoundingBox1D{ 3.8, 3.9 });
      tree.Update(0, vBoxL.front(), BoundingBox1D{ 3.8, 3.9 });
      tree.Update(1, vBoxL[1]);
      tree.FindSmallestNode(BoundingBox1D{ 3.5, 3.7 });
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
}


namespace Tree2DTest
{

  TEST_CLASS(Point_General)
  {
    TEST_METHOD(Create_SetNo1)
    {
      autoce N = 2;
      autoce points = getPointSetNo1<N>();
      autoc nt = TreePointND<N>(points, 3, std::nullopt, 3);
      autoc& nodes = nt.GetNodes();
      Assert::AreEqual<size_t>(22, nodes.size());
    }

    TEST_METHOD(PlaneSearch_3D_XY_dP0)
    {
      autoc vpt = vector
      {
        Point3D{ 2.0, -1.0, 0.0 }, 
        Point3D{ 3.0, -2.0, 1.0 }, 
        Point3D{ 4.0, -3.0, 0.02 },
        Point3D{ 4.0, -2.0, 0.00 },
        Point3D{ 5.0,  0.0, 0.00999 }
      };

      autoc tree = OctreePoint(vpt, 3, std::nullopt, 2);

      autoc ids = tree.PlaneSearch(0.0, Point3D{ 0.0, 0.0, 1.0 }, 0.01, vpt);
      Assert::IsTrue(std::ranges::is_permutation(vector<size_t>{ 0, 3, 4 }, ids));
    }


    TEST_METHOD(PlaneSearch_3D_XY_dP1)
    {
      autoc vpt = vector
      {
        Point3D{ 2.0, -1.0, 0.0 },
        Point3D{ 3.0, -2.0, 1.0 },
        Point3D{ 4.0, -3.0, 0.02 },
        Point3D{ 4.0, -2.0, 0.00 },
        Point3D{ 5.0,  0.0, 1.00999 }
      };

      autoc tree = OctreePoint(vpt, 3, std::nullopt, 2);

      autoc ids = tree.PlaneSearch(1.0, Point3D{ 0.0, 0.0, 1.0 }, 0.01, vpt);
      Assert::IsTrue(std::ranges::is_permutation(vector<size_t>{ 1, 4 }, ids));
    }


    TEST_METHOD(PlaneSearch_3D_XZ_dP4)
    {
      autoc vpt = vector
      {
        Point3D{ 2.0, -1.0, 0.0 },
        Point3D{ 3.0, -2.0, 1.0 },
        Point3D{ 4.0, -3.0, 0.02 },
        Point3D{ 4.0, -2.0, 0.00 },
        Point3D{ 5.0,  0.0, 1.00999 }
      };

      autoc tree = OctreePoint(vpt, 3, std::nullopt, 2);

      autoc ids = tree.PlaneSearch(4.0, Point3D{ 1.0, 0.0, 0.0 }, 0.01, vpt);
      Assert::IsTrue(std::ranges::is_permutation(vector<size_t>{ 2, 3 }, ids));
    }


    TEST_METHOD(PlaneSearchC_3D_XZ_dP4)
    {
      autoc vpt = vector
      {
        Point3D{ 2.0, -1.0, 0.0 },
        Point3D{ 3.0, -2.0, 1.0 },
        Point3D{ 4.0, -3.0, 0.02 },
        Point3D{ 4.0, -2.0, 0.00 },
        Point3D{ 5.0,  0.0, 1.00999 }
      };

      autoc tree = OctreePointC(vpt, 3, std::nullopt, 2);

      autoc ids = tree.PlaneSearch(4.0, Point3D{ 1.0, 0.0, 0.0 }, 0.01);
      Assert::IsTrue(std::ranges::is_permutation(vector<size_t>{ 2, 3 }, ids));
    }
  };

  TEST_CLASS(Point_kNNTest)
  {
    TEST_METHOD(N103_k2_RemainInSmallestNode__17_18)
    {
      autoce N = 2;
      autoce points = getPointSetNo1<N>();
      autoc tree = TreePointND<N>(points, 3, std::nullopt, 3);

      autoce pt = PointND<N>{ 3.5, 5.5 };
      autoc vnn = tree.GetNearestNeighbors(pt, 2, points);
      Assert::IsTrue(std::ranges::is_permutation(vector<size_t>{17, 18}, vnn));
    }

    TEST_METHOD(N103_k3__14_17_18)
    {
      autoce N = 2;
      autoce points = getPointSetNo1<N>();
      autoc tree = TreePointND<N>(points, 3, std::nullopt, 3);

      autoce pt = PointND<N>{ 3.5, 5.5 };
      autoc vnn = tree.GetNearestNeighbors(pt, 3, points);
      Assert::IsTrue(std::ranges::is_permutation(vector<size_t>{17, 18, 14}, vnn));
    }

    TEST_METHOD(N103_k4__14_16_17_18)
    {
      autoce N = 2;
      autoce points = getPointSetNo1<N>();
      autoc tree = TreePointND<N>(points, 3, std::nullopt, 3);

      autoce pt = PointND<N>{ 3.5, 5.5 };
      autoc vnn = tree.GetNearestNeighbors(pt, 4, points);
      Assert::IsTrue(std::ranges::is_permutation(vector<size_t>{17, 18, 14, 16}, vnn));
    }

    TEST_METHOD(N103_k100_OverTheContainingElements__All)
    {
      autoce N = 2;
      autoce points = getPointSetNo1<N>();
      autoc tree = TreePointND<N>(points, 3, std::nullopt, 3);

      autoce pt = PointND<N>{ 3.5, 5.5 };
      autoc vnn = tree.GetNearestNeighbors(pt, 100, points);
      Assert::AreEqual(points.size(), vnn.size());

      auto Entities = vector<size_t>(points.size());
      iota(begin(Entities), end(Entities), 0);
      Assert::IsTrue(std::ranges::is_permutation(Entities, vnn));
    }

    TEST_METHOD(N4_k1__1)
    {
      autoce N = 2;
      autoce points = getPointSetNo1<N>();
      autoc tree = TreePointND<N>(points, 3, std::nullopt, 3);

      autoce pt = PointND<N>{ 2.0, 2.0 };
      autoc vnn = tree.GetNearestNeighbors(pt, 1, points);
      Assert::IsTrue(std::ranges::is_permutation(vector<size_t>{1}, vnn));
    }

    TEST_METHOD(N4_k2__1_11_12)
    {
      autoce N = 2;
      autoce points = getPointSetNo1<N>();
      autoc tree = TreePointND<N>(points, 3, std::nullopt, 3);

      autoce pt = PointND<N>{ 2.0, 2.0 };
      autoc vnn = tree.GetNearestNeighbors(pt, 2, points);
      Assert::IsTrue(std::ranges::is_permutation(vector<size_t>{1, 11}, vnn));
    }

    TEST_METHOD(N4_k3__1_11_12)
    {
      autoce N = 2;
      autoce points = getPointSetNo1<N>();
      autoc tree = TreePointND<N>(points, 3, std::nullopt, 3);

      autoce pt = PointND<N>{ 2.0, 2.0 };
      autoc vnn = tree.GetNearestNeighbors(pt, 3, points);
      Assert::IsTrue(std::ranges::is_permutation(vector<size_t>{1, 11, 12}, vnn));
    }

    // outside

    TEST_METHOD(OutSide_k1__0)
    {
      autoce N = 2;
      autoce points = getPointSetNo1<N>();
      autoc tree = TreePointND<N>(points, 3, std::nullopt, 3);

      autoce pt = PointND<N>{ -1.0, -1.0 };
      autoc vnn = tree.GetNearestNeighbors(pt, 1, points);
      Assert::IsTrue(std::ranges::is_permutation(vector<size_t>{0}, vnn));
    }

    TEST_METHOD(OutSide_k5__0_1_5_6_11)
    {
      autoce N = 2;
      autoce points = getPointSetNo1<N>();
      autoc tree = TreePointND<N>(points, 3, std::nullopt, 3);

      autoce pt = PointND<N>{ -1.0, -1.0 };
      autoc vnn = tree.GetNearestNeighbors(pt, 5, points);
      Assert::IsTrue(std::ranges::is_permutation(vector<size_t>{0, 1, 5, 6, 11}, vnn));
    }



    TEST_METHOD(OutSide_k5_16D__0_1_5_6_11)
    {
      autoce N = 23;
      autoce points = getPointSetNo1<N>();
      autoc tree = TreePointND<N>(points, 3, std::nullopt, 3);

      autoce pt = PointND<N>{ -1.0, -1.0 };
      autoc vnn = tree.GetNearestNeighbors(pt, 5, points);
      Assert::IsTrue(std::ranges::is_permutation(vector<size_t>{0, 1, 5, 6, 11}, vnn));
    }


    TEST_METHOD(Issue9_2D)
    {
      autoc poses = vector<array<double, 2>>
      {
        { 78.2619, 77.843 },
        { 90.3005, 90.5172 },
        { 69.8652, 12.2467 },
        { 48.4675, 48.4948 },
        { 36.3226, 68.4619 },
        { 98.8799, 42.7149 },
        { 31.412, 38.6866 },
        { 63.2748, 77.0524 },
        { 62.8844, 17.0536 },
        { 80.8931, 39.8099 },
        { 77.426, 64.9844 },
        { 81.9552, 25.009 },
        { 87.6088, 51.319 },
        { 78.5609, 80.4623 },
        { 51.3967, 39.5269 },
        { 32.2042, 81.8779 },
        { 79.1739, 81.5467 },
        { 95.2619, 40.4742 },
        { 86.437, 92.4406 },
        { 3.95388, 60.2327 },
        { 31.1283, 44.4917 },
        { 35.6778, 79.8545 },
        { 50.9926, 66.1373 },
        { 3.16271, 65.2519 },
        { 56.3665, 45.3819 }
      };


      autoce search_point = array<double, 2>{ 43.6406, 57.5691 };
      using AD = OrthoTree::AdaptorGeneral<2, array<double, 2>, OrthoTree::BoundingBox2D, OrthoTree::Ray2D, OrthoTree::Plane2D>;
      autoc itMin = std::ranges::min_element(poses, [&search_point](autoc& lhs, autoc& rhs) { return AD::Distance2(lhs, search_point) < AD::Distance2(rhs, search_point); });

      std::array<double, 2> inspection_space_min = { 0.0, 0.0 };
      std::array<double, 2> inspection_space_max = { 100.0, 100.0 };
      OrthoTree::BoundingBox2D inspection_space;
      inspection_space.Min = inspection_space_min;
      inspection_space.Max = inspection_space_max;
      //Standard Tree
      auto tree = QuadtreePointC(poses, 9, inspection_space);

      auto neighbors = tree.GetNearestNeighbors(search_point, 1);
      Assert::AreEqual<size_t>(std::distance(poses.begin(), itMin), neighbors[0]);
    }


    TEST_METHOD(Issue9_6D)
    {
      using VectorType = array<double, 6>;

      autoc poses = vector<VectorType>
      {
        { 50.2232, 0.276687, 37.7662, 41.2776, 26.3818, 74.0284 },
        { 35.8946, 83.7503, 97.1127, 47.2895, 40.9232, 83.7666 },
        { 33.3541, 63.0669, 3.86075, 47.7923, 19.8039, 87.5608 },
        { 89.0684, 67.3278, 50.867, 49.8193, 72.6692, 54.0271 },
        { 75.7099, 53.1241, 39.624, 40.4669, 13.6433, 88.6247 },
        { 43.7641, 93.7985, 68.8286, 9.71882, 2.16644, 12.1925 },
        { 82.5195, 30.6964, 71.0556, 48.4744, 99.2295, 70.4137 },
        { 28.4568, 37.8366, 74.8597, 27.897, 60.6816, 0.247821 },
        { 58.3617, 17.0165, 15.1021, 70.9832, 22.5325, 34.8085 },
        { 33.1004, 72.6729, 35.7043, 35.2888, 94.9917, 17.652 },
        { 80.9693, 7.41406, 2.18394, 40.2606, 1.63274, 65.8996 },
        { 88.4851, 73.8366, 55.0264, 77.6467, 61.6751, 33.7444 },
        { 48.1777, 90.1285, 75.9069, 49.1867, 39.7186, 74.2862 },
        { 68.0826, 73.0622, 7.85933, 60.8879, 41.3229, 16.6928 },
        { 19.1693, 49.1172, 91.055, 52.5548, 16.2326, 0.610075 },
        { 19.3085, 81.9363, 55.8924, 1.77161, 29.4124, 72.3873 },
        { 57.8801, 28.3918, 45.7062, 92.1774, 0.742745, 29.0499 },
        { 74.2952, 84.6691, 46.3357, 76.7894, 68.8394, 24.1707 },
        { 0.199418, 93.2845, 94.9628, 38.2295, 82.1998, 59.2499 },
        { 68.4689, 0.261607, 75.5001, 58.427, 55.4243, 18.7392 },
        { 53.9164, 95.4966, 59.657, 71.0292, 82.4362, 53.9452 },
      };

      autoc search_point = VectorType{ 78.8658, 64.0361, 18.7755, 61.4618, 14.3312, 40.0196 };
      using AD = OrthoTree::AdaptorGeneral<6, VectorType, OrthoTree::BoundingBoxND<6>, OrthoTree::RayND<6>, OrthoTree::PlaneND<6>>;
      autoc itMinExpected = std::ranges::min_element(poses, [&search_point](autoc& lhs, autoc& rhs) { return AD::Distance2(lhs, search_point) < AD::Distance2(rhs, search_point); });

      autoc inspection_space = OrthoTree::BoundingBoxND<6>
      {
        { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 },
        { 100.0, 100.0, 100.0, 100.0, 100.0, 100.0 }
      };

      //Standard Tree
      auto tree = TreePointND<6>();
      tree.Create(tree, poses, 10, inspection_space);

      autoc neighbors = tree.GetNearestNeighbors(search_point, 1, poses);
      autoc idMinExpected = std::distance(poses.begin(), itMinExpected);
      Assert::AreEqual<size_t>(idMinExpected, neighbors[0]);
    }
  };

  TEST_CLASS(Box_General)
  {
    TEST_METHOD(Create_d3__Node7)
    {
      autoce boxes = array
      {
        BoundingBox2D{ { 0.0, 0.0 }, { 1.0, 1.0 } },
        BoundingBox2D{ { 1.0, 1.0 }, { 2.0, 2.0 } },
        BoundingBox2D{ { 2.0, 2.0 }, { 3.0, 3.0 } },
        BoundingBox2D{ { 3.0, 3.0 }, { 4.0, 4.0 } },
        BoundingBox2D{ { 1.2, 1.2 }, { 2.8, 2.8 } }
      };

      autoc quadtreebox = QuadtreeBox(boxes, 3, std::nullopt, 2);
      autoc nodes = quadtreebox.GetNodes();
      Assert::AreEqual<size_t>(7, nodes.size());
      
      Assert::IsTrue(nodes.contains(1));
      Assert::IsTrue(nodes.at(1).Entities == vector<std::size_t>{ 4 });

      Assert::IsTrue(nodes.contains(4));
      Assert::IsTrue(nodes.at(4).Entities.empty());

      Assert::IsTrue(nodes.contains(7));
      Assert::IsTrue(nodes.at(7).Entities.empty());

      Assert::IsTrue(nodes.contains(16));
      Assert::IsTrue(nodes.at(16).Entities == vector<std::size_t>{ 0 });

      Assert::IsTrue(nodes.contains(19));
      Assert::IsTrue(nodes.at(19).Entities == vector<std::size_t>{ 1 });

      Assert::IsTrue(nodes.contains(28));
      Assert::IsTrue(nodes.at(28).Entities == vector<std::size_t>{ 2 });

      Assert::IsTrue(nodes.contains(31));
      Assert::IsTrue(nodes.at(31).Entities == vector<std::size_t>{ 3 });
    }


    TEST_METHOD(Create_d3__Node8)
    {
      autoce boxes = array
      {
        BoundingBox2D{ { 0.0, 0.0 }, { 1.0, 1.0 } },
        BoundingBox2D{ { 1.0, 1.0 }, { 2.0, 2.0 } },
        BoundingBox2D{ { 2.0, 2.0 }, { 3.0, 3.0 } },
        BoundingBox2D{ { 3.0, 3.0 }, { 4.0, 4.0 } },
        BoundingBox2D{ { 1.2, 1.2 }, { 2.8, 2.8 } },
        BoundingBox2D{ { 1.5, 1.5 }, { 2.0, 2.0 } },
      };

      autoc quadtreebox = QuadtreeBox(boxes, 3, std::nullopt, 2);
      autoc nodes = quadtreebox.GetNodes();
      Assert::AreEqual<size_t>(8, nodes.size());

      Assert::IsTrue(nodes.contains(1));
      Assert::IsTrue(nodes.at(1).Entities == vector<std::size_t>{ 4 });

      Assert::IsTrue(nodes.contains(4));
      Assert::IsTrue(nodes.at(4).Entities.empty());

      Assert::IsTrue(nodes.contains(7));
      Assert::IsTrue(nodes.at(7).Entities.empty());

      Assert::IsTrue(nodes.contains(16));
      Assert::IsTrue(nodes.at(16).Entities == vector<std::size_t>{ 0 });

      Assert::IsTrue(nodes.contains(19));
      Assert::IsTrue(nodes.at(19).Entities == vector<std::size_t>{ 1 });

      Assert::IsTrue(nodes.contains(28));
      Assert::IsTrue(nodes.at(28).Entities == vector<std::size_t>{ 2 });

      Assert::IsTrue(nodes.contains(31));
      Assert::IsTrue(nodes.at(31).Entities == vector<std::size_t>{ 3 });

      Assert::IsTrue(nodes.contains(31));
      Assert::IsTrue(nodes.at(31).Entities == vector<std::size_t>{ 3 });

      Assert::IsTrue(nodes.contains(79));
      Assert::IsTrue(nodes.at(79).Entities == vector<std::size_t>{ 5 });

    }

  };

  TEST_CLASS(Box_SearchTest)
  {
    TEST_METHOD(RangeSearch__Inside__124)
    {
      autoce boxes = array
      {
        BoundingBox2D{ { 0.0, 0.0 }, { 1.0, 1.0 } },
        BoundingBox2D{ { 1.0, 1.0 }, { 2.0, 2.0 } },
        BoundingBox2D{ { 2.0, 2.0 }, { 3.0, 3.0 } },
        BoundingBox2D{ { 3.0, 3.0 }, { 4.0, 4.0 } },
        BoundingBox2D{ { 1.2, 1.2 }, { 2.8, 2.8 } }
      };

      autoc quadtreebox = QuadtreeBox(boxes, 3, std::nullopt, 2);

      autoc search_box = BoundingBox2D{ { 1.0, 1.0 }, { 3.1, 3.1 } };
      autoc ids = quadtreebox.RangeSearch(search_box, boxes); // { 1, 2, 4 }

      Assert::IsTrue(std::ranges::is_permutation(vector<size_t>{1, 2, 4}, ids));
    }

    TEST_METHOD(RangeSearch__Overlap__1234)
    {
      autoce boxes = array
      {
        BoundingBox2D{ { 0.0, 0.0 }, { 1.0, 1.0 } },
        BoundingBox2D{ { 1.0, 1.0 }, { 2.0, 2.0 } },
        BoundingBox2D{ { 2.0, 2.0 }, { 3.0, 3.0 } },
        BoundingBox2D{ { 3.0, 3.0 }, { 4.0, 4.0 } },
        BoundingBox2D{ { 1.2, 1.2 }, { 2.8, 2.8 } }
      };

      autoc quadtreebox = QuadtreeBox(boxes, 3, std::nullopt, 2);

      autoc search_box = BoundingBox2D{ { 1.0, 1.0 }, { 3.1, 3.1 } };
      autoc ids = quadtreebox.RangeSearch<false>(search_box, boxes); // { 1, 2, 3, 4 }

      Assert::IsTrue(std::ranges::is_permutation(vector<size_t>{1, 2, 3, 4}, ids));
    }

    TEST_METHOD(PickSearch__24)
    {
      autoce boxes = array
      {
        BoundingBox2D{ { 0.0, 0.0 }, { 1.0, 1.0 } },
        BoundingBox2D{ { 1.0, 1.0 }, { 2.0, 2.0 } },
        BoundingBox2D{ { 2.0, 2.0 }, { 3.0, 3.0 } },
        BoundingBox2D{ { 3.0, 3.0 }, { 4.0, 4.0 } },
        BoundingBox2D{ { 1.2, 1.2 }, { 2.8, 2.8 } }
      };

      autoc quadtreebox = QuadtreeBox(boxes, 3, std::nullopt, 2);

      autoc ptPick = Point2D{ 2.5, 2.5 };
      auto ids_picked = quadtreebox.PickSearch(ptPick, boxes); // { 2, 4 }

      Assert::IsTrue(std::ranges::is_permutation(vector<size_t>{ 2, 4 }, ids_picked));
    }


    TEST_METHOD(PickSearch__124)
    {
      autoce boxes = array
      {
        BoundingBox2D{ { 0.0, 0.0 }, { 1.0, 1.0 } },
        BoundingBox2D{ { 1.0, 1.0 }, { 2.0, 2.0 } },
        BoundingBox2D{ { 2.0, 2.0 }, { 3.0, 3.0 } },
        BoundingBox2D{ { 3.0, 3.0 }, { 4.0, 4.0 } },
        BoundingBox2D{ { 1.2, 1.2 }, { 2.8, 2.8 } }
      };

      autoc quadtreebox = QuadtreeBox(boxes, 3, std::nullopt, 2);

      autoc ptPick = Point2D{ 2.0, 2.0 };
      auto ids_picked = quadtreebox.PickSearch(ptPick, boxes); // { 1, 2, 4 }

      Assert::IsTrue(std::ranges::is_permutation(vector<size_t>{ 1, 2, 4 }, ids_picked));
    }


    TEST_METHOD(PlaneIntersection_3D_XY_d0)
    {
      autoce boxes = array
      {
        BoundingBox3D{ { 0.0, 0.0, 0.0 }, { 1.0, 1.0, 0.0 } },
        BoundingBox3D{ { 1.0, 1.0, -1.0 }, { 2.0, 2.0, 0.0 } },
        BoundingBox3D{ { 2.0, 2.0, -1.0 }, { 3.0, 3.0, -0.2 } },
        BoundingBox3D{ { 3.0, 3.0, 1.0 }, { 4.0, 4.0, 0.2 } },
        BoundingBox3D{ { 1.2, 1.2, -1.0 }, { 2.8, 2.8, 1.0 } }
      };

      autoc octreebox = OctreeBox(boxes, 3, std::nullopt, 2);
      autoc intersectedBoxes = octreebox.PlaneIntersection(0.0, Point3D{ 0.0, 0.0, 1.0 }, 0.01, boxes);

      Assert::IsTrue(std::ranges::is_permutation(vector<size_t>{ 0, 1, 4 }, intersectedBoxes));
    }


    TEST_METHOD(PlaneIntersection_3D_XY_dP1)
    {
      autoce boxes = array
      {
        BoundingBox3D{ { 0.0, 0.0, 0.0 }, { 1.0, 1.0, 0.0 } },
        BoundingBox3D{ { 1.0, 1.0, -1.0 }, { 2.0, 2.0, 0.0 } },
        BoundingBox3D{ { 2.0, 2.0, -1.0 }, { 3.0, 3.0, -0.2 } },
        BoundingBox3D{ { 3.0, 3.0, 1.0 }, { 4.0, 4.0, 0.2 } },
        BoundingBox3D{ { 1.2, 1.2, -1.0 }, { 2.8, 2.8, 1.1 } }
      };

      autoc octreebox = OctreeBox(boxes, 3, std::nullopt, 2);
      autoc intersectedBoxes = octreebox.PlaneIntersection(1.0, Point3D{ 0.0, 0.0, 1.0 }, 0.01, boxes);

      Assert::IsTrue(std::ranges::is_permutation(vector<size_t>{ 4 }, intersectedBoxes));
    }

    TEST_METHOD(PlaneIntersection_3D_XY_dN1)
    {
      autoce boxes = array
      {
        BoundingBox3D{ { 0.0, 0.0, 0.0 }, { 1.0, 1.0, 0.0 } },
        BoundingBox3D{ { 1.0, 1.0, -1.0 }, { 2.0, 2.0, 0.0 } },
        BoundingBox3D{ { 2.0, 2.0, -1.0 }, { 3.0, 3.0, -0.2 } },
        BoundingBox3D{ { 3.0, 3.0, 1.0 }, { 4.0, 4.0, 0.2 } },
        BoundingBox3D{ { 1.2, 1.2, -1.1 }, { 2.8, 2.8, 1.1 } }
      };

      autoc octreebox = OctreeBox(boxes, 3, std::nullopt, 2);
      autoc intersectedBoxes = octreebox.PlaneIntersection(-1.0, Point3D{ 0.0, 0.0, 1.0 }, 0.01, boxes);

      Assert::IsTrue(std::ranges::is_permutation(vector<size_t>{ 1, 2, 4 }, intersectedBoxes));
    }


    TEST_METHOD(PlaneIntersection_3D_YZ_dP1)
    {
      autoce boxes = array
      {
        BoundingBox3D{ { 0.0, 0.0, 0.0 }, { 1.0, 1.0, 0.0 } },
        BoundingBox3D{ { 1.0, 1.0, -1.0 }, { 2.0, 2.0, 0.0 } },
        BoundingBox3D{ { 2.0, 2.0, -1.0 }, { 3.0, 3.0, -0.2 } },
        BoundingBox3D{ { 3.0, 3.0, 1.0 }, { 4.0, 4.0, 0.2 } },
        BoundingBox3D{ { 1.2, 1.2, -1.1 }, { 2.8, 2.8, 1.1 } }
      };

      autoc octreebox = OctreeBox(boxes, 3, std::nullopt, 2);
      autoc intersectedBoxes = octreebox.PlaneIntersection(1.0, Point3D{ 1.0, 0.0, 0.0 }, 0.01, boxes);

      Assert::IsTrue(std::ranges::is_permutation(vector<size_t>{ 0, 1 }, intersectedBoxes));
    }

    TEST_METHOD(PlaneIntersection_3D_YZ_dN1)
    {
      autoce boxes = array
      {
        BoundingBox3D{ { 0.0, 0.0, 0.0 }, { 1.0, 1.0, 0.0 } },
        BoundingBox3D{ { 1.0, 1.0, -1.0 }, { 2.0, 2.0, 0.0 } },
        BoundingBox3D{ { 2.0, 2.0, -1.0 }, { 3.0, 3.0, -0.2 } },
        BoundingBox3D{ { 3.0, 3.0, 1.0 }, { 4.0, 4.0, 0.2 } },
        BoundingBox3D{ { 1.2, 1.2, -1.1 }, { 2.8, 2.8, 1.1 } }
      };

      autoc octreebox = OctreeBox(boxes, 3, std::nullopt, 2);
      autoc intersectedBoxes = octreebox.PlaneIntersection(-1.0, Point3D{ 1.0, 0.0, 0.0 }, 0.01, boxes);

      Assert::IsTrue(std::ranges::is_permutation(vector<size_t>{}, intersectedBoxes));
    }

    TEST_METHOD(PlaneIntersectionC_3D_YZ_dN1)
    {
      autoce boxes = array
      {
        BoundingBox3D{ { 0.0, 0.0, 0.0 }, { 1.0, 1.0, 0.0 } },
        BoundingBox3D{ { 1.0, 1.0, -1.0 }, { 2.0, 2.0, 0.0 } },
        BoundingBox3D{ { 2.0, 2.0, -1.0 }, { 3.0, 3.0, -0.2 } },
        BoundingBox3D{ { 3.0, 3.0, 1.0 }, { 4.0, 4.0, 0.2 } },
        BoundingBox3D{ { 1.2, 1.2, -1.1 }, { 2.8, 2.8, 1.1 } }
      };

      autoc octreebox = OctreeBoxC(boxes, 3, std::nullopt, 2);
      autoc intersectedBoxes = octreebox.PlaneIntersection(-1.0, Point3D{ 1.0, 0.0, 0.0 }, 0.01);

      Assert::IsTrue(std::ranges::is_permutation(vector<size_t>{}, intersectedBoxes));
    }
    TEST_METHOD(PlaneIntersection_3D_YZA_dPSQRT2)
    {
      autoce boxes = array
      {
        BoundingBox3D{ { 0.0, 0.0, 0.0 }, { 1.0, 1.0, 0.0 } },
        BoundingBox3D{ { 1.0, 1.0, -1.0 }, { 2.0, 2.0, 0.0 } },
        BoundingBox3D{ { 2.0, 2.0, -1.0 }, { 3.0, 3.0, -0.2 } },
        BoundingBox3D{ { 3.0, 3.0, 1.0 }, { 4.0, 4.0, 0.2 } },
        BoundingBox3D{ { 1.2, 1.2, -1.1 }, { 2.8, 2.8, 1.1 } }
      };

      autoc octreebox = OctreeBox(boxes, 3, std::nullopt, 2);
      autoc sqrt2__2 = std::sqrt(2.0) * 0.5;
      autoc intersectedBoxes = octreebox.PlaneIntersection(std::sqrt(2.0), Point3D{ 0.0, sqrt2__2, sqrt2__2 }, 0.01, boxes);

      Assert::IsTrue(std::ranges::is_permutation(vector<size_t>{1, 2, 4}, intersectedBoxes));
    }


    TEST_METHOD(CollistionDetection__33_24_34)
    {
      autoce boxesL = array
      {
        BoundingBox2D{ { 0.0, 0.0 }, { 1.0, 1.0 } },
        BoundingBox2D{ { 1.0, 1.0 }, { 2.0, 2.0 } },
        BoundingBox2D{ { 2.0, 2.0 }, { 3.0, 3.0 } },
        BoundingBox2D{ { 3.0, 3.0 }, { 4.0, 4.0 } },
      };

      autoce boxesR = array
      {
        BoundingBox2D{ { 2.0, 0.0 }, { 3.0, 1.0 } },
        BoundingBox2D{ { 3.0, 1.0 }, { 4.0, 2.0 } },
        BoundingBox2D{ { 3.0, 2.0 }, { 4.0, 3.0 } },
        BoundingBox2D{ { 2.0, 3.0 }, { 4.0, 4.0 } },
        BoundingBox2D{ { 2.5, 2.5 }, { 3.5, 3.5 } },
      };

      autoc qtL = QuadtreeBox(boxesL, 3, std::nullopt, 2);
      autoc qtR = QuadtreeBox(boxesR, 3, std::nullopt, 2);

      autoc aActualPair = QuadtreeBox::CollisionDetection(qtL, boxesL, qtR, boxesR); // { {3, 3}, {2, 4}, {3, 4} }

      autoce aExpectedPair = array
      {
        std::pair{ 3, 3 }, // Level 0
        std::pair{ 2, 4 },
        std::pair{ 3, 4 }, // Level 1
      };
      Assert::AreEqual<size_t>(aExpectedPair.size(), aActualPair.size());
      Assert::IsTrue(std::ranges::is_permutation(aExpectedPair, aActualPair, [](autoc& p1, autoc& p2) { return p1.first == p2.first && p1.second == p2.second; }));
    }
  };
}


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
      auto pointNo = points.size();

      OctreePoint tree(
        points,
        8,
        BoundingBox3D{
          {-4, -4, -4},
          {+4, +4, +4}
      },
        2);
      tree.UpdateIndexes<false>({});

      autoc isOutsiderInserted = tree.Insert(pointNo, Point3D{ +5.0, +4.0, +4.0}, false);
      Assert::IsFalse(isOutsiderInserted);

      points.emplace_back(Point3D{ +3.0, +3.0, +3.0 });
      tree.Insert(pointNo, points.back(), false);
      autoc nodeID_6 = tree.GetNodeIDByEntity(pointNo);
      Assert::AreEqual<OctreePoint::MortonNodeID>(nodeID_6, 1023); // 1023: 5,6
      ++pointNo;

      points.emplace_back(Point3D{ +2.0, +2.0, +2.0 });
      tree.Insert(pointNo, points.back(), true);
      autoc nodeID_7 = tree.GetNodeIDByEntity(pointNo);
      Assert::AreEqual<OctreePoint::MortonNodeID>(nodeID_7, 33292288); // It should place in the leaf node
      ++pointNo;

      points.emplace_back(Point3D{ +3.25, +3.25, +3.25});
      tree.Insert(pointNo, points.back(), false);
      autoc nodeID_8 = tree.GetNodeIDByEntity(pointNo);
      Assert::AreEqual<OctreePoint::MortonNodeID>(nodeID_8, 1023); // Parent has a child, another child should be added.
      ++pointNo;

      // InsertWithRebalancing
      {
        points.emplace_back(Point3D{ +3.15, +3.15, +3.15 });
        tree.Insert(pointNo, points.back(), false);
        autoc nodeID_9 = tree.GetNodeIDByEntity(pointNo);
        Assert::AreEqual<OctreePoint::MortonNodeID>(nodeID_9, 1023); // It should stuck on a parent level
        ++pointNo;

        points.emplace_back(Point3D{ +3.0, +3.0, +3.5 });
        tree.Insert(pointNo, points.back(), false);
        autoc nodeID_10 = tree.GetNodeIDByEntity(pointNo);
        Assert::AreEqual<OctreePoint::MortonNodeID>(nodeID_10, 1023); // It should stuck on a parent level
        ++pointNo;

        points.emplace_back(Point3D{ +3.0, +3.0, +3.15 });
        tree.Insert(pointNo, points.back(), false);
        autoc nodeID_11 = tree.GetNodeIDByEntity(pointNo);
        Assert::AreEqual<OctreePoint::MortonNodeID>(nodeID_11, 1023); // It should stuck on a parent level
        ++pointNo;

        points.emplace_back(Point3D{ +3.75, +3.75, +3.75});
        tree.InsertWithRebalancing(pointNo, points.back(), points);
        autoc nodeID_12 = tree.GetNodeIDByEntity(pointNo);
        Assert::AreEqual<OctreePoint::MortonNodeID>(nodeID_12, 8191); // It should reoder the elements
        ++pointNo;

        autoc nodeID_9_u = tree.GetNodeIDByEntity(9);
        autoc nodeID_10_u = tree.GetNodeIDByEntity(10);
        autoc nodeID_11_u = tree.GetNodeIDByEntity(11);
        Assert::AreEqual<OctreePoint::MortonNodeID>(nodeID_9_u, 523783);
        Assert::AreEqual<OctreePoint::MortonNodeID>(nodeID_10_u, 8188);
        Assert::AreEqual<OctreePoint::MortonNodeID>(nodeID_11_u, 523780);

        points.emplace_back(Point3D{ -2.0, -2.0, -2.0 });
        tree.InsertWithRebalancing(pointNo, points.back(), points);
        autoc nodeID_13 = tree.GetNodeIDByEntity(pointNo);
        Assert::AreEqual<OctreePoint::MortonNodeID>(nodeID_13, 18612224); // It should reoder to the bottom because of [0] is the same
        Assert::AreEqual<depth_t>(8, tree.GetDepthID(nodeID_13));
        ++pointNo;
      }

      tree.Update(2, Point3D{ +2.0, +2.2, +2.0 });
      autoc nodeID_2_u1 = tree.GetNodeIDByEntity(2);

      points[2] = { +2.0, +2.0, +2.6 };
      tree.Update(2, points[2]);
      autoc nodeID_2_u2 = tree.GetNodeIDByEntity(2);
      Assert::AreEqual<OctreePoint::MortonNodeID>(nodeID_2_u1, 520194);
      Assert::AreEqual<OctreePoint::MortonNodeID>(nodeID_2_u2, 8132);  
      Assert::IsFalse(tree.HasNode(520194));

      points[6] = { -2.0, -2.0, -2.0 };
      tree.Update(6, points[6]);
      autoc nodeID_6_u1 = tree.GetNodeIDByEntity(6);

      autoc oldbox2 = points[2];
      points[2] = { -2.0, -2.5, -2.0};
      tree.Update(2, oldbox2, points[2]);
      autoc nodeID_2_u3 = tree.GetNodeIDByEntity(2);

      autoc oldbox4 = points[4];
      points[4] = { +3.50, +3.50, +3.50 };
      tree.Update(4, oldbox4, points[4], points); // It should move and erase 1016
      autoc nodeID_4_u1 = tree.GetNodeIDByEntity(4);
      autoc nodeID_9_u1 = tree.GetNodeIDByEntity(9);

      points[8] = { +3.0, +3.0, +3.75 };
      tree.Update(8, points[8], points);
      autoc nodeID_8_u1 = tree.GetNodeIDByEntity(8);

      autoc entitiesInBFS = tree.CollectAllIdInBFS();

      Assert::AreEqual<OctreePoint::MortonNodeID>(nodeID_6_u1, 18612224);    
      Assert::AreEqual<OctreePoint::MortonNodeID>(nodeID_2_u3, 69);   
      Assert::AreEqual<OctreePoint::MortonNodeID>(nodeID_4_u1, 65528);
      Assert::AreEqual<OctreePoint::MortonNodeID>(nodeID_9_u1, 523783);
      Assert::AreEqual<OctreePoint::MortonNodeID>(nodeID_8_u1, 65508);

      Assert::IsTrue(entitiesInBFS == std::vector<std::size_t>{ 1, 2, 3, 10, 8, 4, 12, 11, 9, 13, 0, 6, 7, 5 });
    }
  };

  TEST_CLASS(Box_InsertTest)
  {
    TEST_METHOD(CreateWithData)
    {
      // This gives a tree with 9 nodes.
      std::vector<BoundingBox3D> boxes = {
      { { -2.00375, -0.20375, +0.19625 }, { -1.52125, -0.19625, +0.20375 } },
      { { -0.88692, -1.05210, -0.80026 }, { +0.88692, +0.72175, +0.97359 } },
      { { -0.78692, -1.05210, -0.80026 }, { +0.98692, +0.72175, +0.97359 } },
      { { -0.68692, -1.05210, -0.80026 }, { +1.08692, +0.72175, +0.97359 } },
      { { -0.58692, -1.05210, -0.80026 }, { +1.18692, +0.72175, +0.97359 } },
      };

      OctreeBox tree(boxes, 8, BoundingBox3D{ { -10, -10, -10 }, { +10, +10, +10 } }, 2);
      tree.UpdateIndexes<false>({});
    }


    TEST_METHOD(AddToEmptyTree)
    {
      std::vector<BoundingBox3D> boxes0;
      OctreeBox tree(
        boxes0,
        8,
        BoundingBox3D{
          {-10, -10, -10},
          {+10, +10, +10}
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

      for (unsigned i = 0; i < boxes.size(); ++i) {
        Assert::IsTrue(tree.Insert(i, boxes[i], true));
        boxes0.emplace_back(boxes[i]);

      }
      tree.UpdateIndexes<false>({});
    }

    TEST_METHOD(CreateWithDataThenInsert)
    {
      // This gives a tree with 9 nodes.
      std::vector<BoundingBox3D> boxes = {
        {{ -2.0, -2.0, -2.0 }, { -1.0, -1.0, +2.0 }}, // split to 8 and 12
        {{ +2.0, +2.0, +2.0 }, { +4.0, +1.0, +1.0 }}, // 127
        {{ +2.0, +2.0, +2.0 }, { +3.1, +3.1, +3.1 }}, // 127
      };
      auto boxNo = boxes.size();

      OctreeBox tree(
        boxes,
        8,
        BoundingBox3D{
          {-4, -4, -4},
          {+4, +4, +4}
        },
        2);
      tree.UpdateIndexes<false>({});

      autoc isOutsiderInserted = tree.Insert(
        boxNo,
        BoundingBox3D{
          {+3.0, +3.0, +3.0},
          {+5.0, +4.0, +4.0}
      },
        false);

      Assert::IsFalse(isOutsiderInserted);

      boxes.emplace_back(BoundingBox3D{
        {+3.0, +3.0, +3.0},
        {+4.0, +4.0, +4.0}
      });
      tree.Insert(boxNo, boxes.back(), false);
      autoc nodeID127 = tree.GetNodeIDByEntity(boxNo);
      Assert::AreEqual<OctreeBox::MortonNodeID>(nodeID127, 127); // It should stuck on a parent level
      ++boxNo;

      boxes.emplace_back(BoundingBox3D{
        {+2.0, +2.0, +2.0},
        {+3.0, +3.0, +3.0}
      });
      tree.Insert(boxNo, boxes.back(), true);
      autoc nodeID1016 = tree.GetNodeIDByEntity(boxNo);
      Assert::AreEqual<OctreeBox::MortonNodeID>(nodeID1016, 1016); // It should place in the leaf node
      ++boxNo;

      boxes.emplace_back(BoundingBox3D{
        {+3.0, +3.0, +3.0},
        {+4.0, +4.0, +4.0}
      });
      tree.Insert(boxNo, boxes.back(), false);
      autoc nodeID1023 = tree.GetNodeIDByEntity(boxNo);
      Assert::AreEqual<OctreeBox::MortonNodeID>(nodeID1023, 1023); // Parent has a child, another child should be added.
      ++boxNo;

      // InsertWithRebalancing
      {
        boxes.emplace_back(BoundingBox3D{
          {+3.0, +3.0, +3.0},
          {+3.5, +3.5, +3.5}
        });
        tree.Insert(boxNo, boxes.back(), false); // 6
        autoc nodeID1023_1 = tree.GetNodeIDByEntity(boxNo);
        Assert::AreEqual<OctreeBox::MortonNodeID>(nodeID1023_1, 1023); // It should stuck on a parent level
        ++boxNo;

        boxes.emplace_back(BoundingBox3D{
          {+3.0, +3.0, +3.5},
          {+3.5, +3.5, +4.0}
        });
        tree.Insert(boxNo, boxes.back(), false); // 7
        autoc nodeID1023_2 = tree.GetNodeIDByEntity(boxNo);
        Assert::AreEqual<OctreeBox::MortonNodeID>(nodeID1023_2, 1023); // It should stuck on a parent level
        ++boxNo;

        boxes.emplace_back(BoundingBox3D{
          {+3.0, +3.0, +3.1},
          {+3.5, +3.5, +3.2}
        });
        tree.Insert(boxNo, boxes.back(), false); // 8
        autoc nodeID1023_3 = tree.GetNodeIDByEntity(boxNo);
        Assert::AreEqual<OctreeBox::MortonNodeID>(nodeID1023_3, 1023); // It should stuck on a parent level
        ++boxNo;

        boxes.emplace_back(BoundingBox3D{
          {+3.75, +3.75, +3.75},
          {+4.0, +4.0, +4.0}
        });
        tree.InsertWithRebalancing(boxNo, boxes.back(), boxes); // 9
        autoc nodeID8191 = tree.GetNodeIDByEntity(boxNo);
        Assert::AreEqual<OctreeBox::MortonNodeID>(nodeID8191, 8191); // It should reoder the elements
        ++boxNo;

        // It should reoder the elements
        autoc nodeID8184_6 = tree.GetNodeIDByEntity(6);
        Assert::AreEqual<OctreeBox::MortonNodeID>(nodeID8184_6, 8184); 
        autoc nodeID8188_7 = tree.GetNodeIDByEntity(7);
        Assert::AreEqual<OctreeBox::MortonNodeID>(nodeID8188_7, 8188);
        autoc nodeID8184_8 = tree.GetNodeIDByEntity(8);
        Assert::AreEqual<OctreeBox::MortonNodeID>(nodeID8184_8, 8184); 

        boxes.emplace_back(BoundingBox3D{
          { -2.0, -2.0, -2.0},
          { -1.0, -1.0, -1.0}
        });
        tree.InsertWithRebalancing(boxNo, boxes.back(), boxes); // 10
        autoc nodeID_10 = tree.GetNodeIDByEntity(boxNo);
        Assert::AreEqual<OctreeBox::MortonNodeID>(nodeID_10, 71); // It should reoder the elements
        ++boxNo;

        // Splitted box is not reordered
        autoc nodeID_0 = tree.GetNodeIDByEntity(0);
        Assert::AreEqual<OctreeBox::MortonNodeID>(nodeID_0, 8);
      }

      tree.Update(
        2,
        {
          {+2.0, +2.0, +2.0},
          {+3.2, +3.2, +3.1}
      });
      autoc nodeID_2_u1 = tree.GetNodeIDByEntity(2);
      Assert::AreEqual<OctreeBox::MortonNodeID>(nodeID_2_u1, 127); // It should remain

      boxes[2] = {
        {+2.0, +2.0, +2.0},
        {+2.6, +2.6, +2.2}
      };
      tree.Update(2, boxes[2]); // 1016
      autoc nodeID_2_u2 = tree.GetNodeIDByEntity(2);
      Assert::AreEqual<OctreeBox::MortonNodeID>(nodeID_2_u2, 1016); // It should move

      boxes[6] = {
        {-2.0, -2.0, -2.0},
        {+2.0, +2.0, +2.0}
      };
      tree.Update(6, boxes[6]); // 1
      autoc nodeID_6_u1 = tree.GetNodeIDByEntity(6);
      Assert::AreEqual<OctreeBox::MortonNodeID>(nodeID_6_u1, 1); // It should move

      autoc oldbox2 = boxes[2];
      boxes[2] = {
        {-2.0, -2.0, -2.0},
        {+0.0, +0.0, +0.0}
      };
      tree.Update(2, oldbox2, boxes[2]);
      autoc nodeID_2_u3 = tree.GetNodeIDByEntity(2);
      Assert::AreEqual<OctreeBox::MortonNodeID>(nodeID_2_u3, 71); // It should move

      autoc oldbox4 = boxes[4];
      boxes[4] = {
        {+3.50, +3.50, +3.50},
        {+3.75, +3.75, +3.75}
      };
      tree.Update(4, oldbox4, boxes[4], boxes); // It should move and erase 1016
      autoc nodeID_4_u1 = tree.GetNodeIDByEntity(4);
      Assert::AreEqual<OctreeBox::MortonNodeID>(nodeID_4_u1, 65528); // It should move 4 and 9 down with a level
      autoc nodeID_9_u1 = tree.GetNodeIDByEntity(9);
      Assert::AreEqual<OctreeBox::MortonNodeID>(nodeID_9_u1, 65535); 

      Assert::IsFalse(tree.HasNode(1016));

      boxes[8] = {
        {+3.0, +3.0, +3.75},
        {+3.25, +3.25, +4.0}
      };
      tree.Update(8, boxes[8], boxes);
      autoc nodeID_8_u1 = tree.GetNodeIDByEntity(8);
      Assert::AreEqual<OctreeBox::MortonNodeID>(nodeID_8_u1, 65508); // It should move

      autoc entitiesInBFS = tree.CollectAllIdInBFS();
      Assert::IsTrue(entitiesInBFS == std::vector<std::size_t>{ 6, 0, 0, 1, 10, 2, 3, 5, 7, 8, 4, 9, }); // [0] should be repetead, because it is splitted. 
    }


  };

}


namespace LongIntAdaptor
{
  using namespace OrthoTree;

  using GeometryType = long int;
  template<size_t N> using CustomVectorTypeND = std::array<GeometryType, N>;
  template<size_t N> using CustomBoundingBoxND = std::array<CustomVectorTypeND<N>, 2>;
  template<size_t N> using CustomRayND = std::array<CustomVectorTypeND<N>, 2>;
  template<size_t N> using CustomPlaneND = std::tuple<CustomVectorTypeND<N>, GeometryType>;

  template <size_t N>
  struct AdaptorBasicsCustom
  {
    static constexpr GeometryType GetPointC(CustomVectorTypeND<N> const& pt, OrthoTree::dim_t iDimension) { return pt[iDimension]; }

    static constexpr void SetPointC(CustomVectorTypeND<N>& pt, OrthoTree::dim_t iDimension, GeometryType value)
    {
      pt[iDimension] = value;
    }

    static constexpr void SetBoxMinC(CustomBoundingBoxND<N>& box, dim_t dimensionID, GeometryType value) { SetPointC(box[0], dimensionID, value); }
    static constexpr void SetBoxMaxC(CustomBoundingBoxND<N>& box, dim_t dimensionID, GeometryType value) { SetPointC(box[1], dimensionID, value); }
    static constexpr GeometryType GetBoxMinC(CustomBoundingBoxND<N> const& box, dim_t dimensionID) { return GetPointC(box[0], dimensionID); }
    static constexpr GeometryType GetBoxMaxC(CustomBoundingBoxND<N> const& box, dim_t dimensionID) { return GetPointC(box[1], dimensionID); }
    
    static constexpr CustomVectorTypeND<N> const& GetRayDirection(CustomRayND<N> const& ray) noexcept { ray[1]; }
    static constexpr CustomVectorTypeND<N> const& GetRayOrigin(CustomRayND<N> const& ray) noexcept { ray[0]; }

    static constexpr CustomVectorTypeND<N> const& GetPlaneNormal(CustomPlaneND<N> const& plane) noexcept { return std::get<0>(plane); }
    static constexpr GeometryType GetPlaneOrigoDistance(CustomPlaneND<N> const& plane) noexcept { return std::get<1>(plane); }

  };

  template <size_t N> using AdaptorCustom = AdaptorGeneralBase<N, CustomVectorTypeND<N>, CustomBoundingBoxND<N>, CustomRayND<N>, CustomPlaneND<N>, GeometryType, AdaptorBasicsCustom<N>>;
  template <size_t N> using OrthoTreePointCustom = OrthoTreePoint<N, CustomVectorTypeND<N>, CustomBoundingBoxND<N>, CustomRayND<N>, CustomPlaneND<N>, GeometryType, AdaptorCustom<N>>;
  template <size_t N> using OrthoTreePointContainerCustom = OrthoTree::OrthoTreeContainerPoint<OrthoTreePointCustom<N>, CustomVectorTypeND<N>>;
  template <size_t N, depth_t nSplit = 2> using OrthoTreeBoxCustom = OrthoTreeBoundingBox<N, CustomVectorTypeND<N>, CustomBoundingBoxND<N>, CustomRayND<N>, CustomPlaneND<N>, GeometryType, nSplit, AdaptorCustom<N>>;
  template <size_t N> using OrthoTreeBoxContainerCustom = OrthoTree::OrthoTreeContainerBox<OrthoTreeBoxCustom<N>, CustomBoundingBoxND<N>>;


  TEST_CLASS(LongIntTest)
  {
    TEST_METHOD(RangeSearchPointAtTheBorder)
    {
      autoce nDim = 1;
      using Vector = CustomVectorTypeND<nDim>;
      using Box = CustomBoundingBoxND<nDim>;

      autoce points = array
      {
        Vector{ 0 },
        Vector{ 4 },
        Vector{ 5 },
        Vector{ 8 }
      };

      autoc tree = OrthoTreePointContainerCustom<nDim>(points, 3, std::nullopt, 2);

      {
        autoc vidActual = tree.RangeSearch(Box{ points[1], points[2] });
        autoce vidExpected = array{ 1, 2 };
        Assert::IsTrue(std::ranges::is_permutation(vidActual, vidExpected));
      }

      {
        autoc vidActual = tree.RangeSearch(Box{ points[2], Vector{ 6 } });
        autoce vidExpected = array{ 2 };
        Assert::IsTrue(std::ranges::is_permutation(vidActual, vidExpected));
      }
    }

    TEST_METHOD(RangeSearchBoxAtTheBorder)
    {
      autoce nDim = 1;
      using Box = CustomBoundingBoxND<nDim>;

      autoc boxes = array<Box, 5>
      {
        Box{ 0, 1 },
        Box{ 3, 4 },
        Box{ 4, 5 },
        Box{ 5, 6 },
        Box{ 7, 8 }
      };

      autoc tree = OrthoTreeBoxContainerCustom<nDim>(boxes, 3, std::nullopt, 2, false);
      {
        autoc vidActual = tree.RangeSearch(boxes[1]);
        autoce vidExpected = array{ 1 };
        Assert::IsTrue(std::ranges::is_permutation(vidActual, vidExpected));
      }

      {
        autoc vidActual = tree.RangeSearch(boxes[2]);
        autoce vidExpected = array{ 2 };
        Assert::IsTrue(std::ranges::is_permutation(vidActual, vidExpected));
      }

      {
        autoc vidActual = tree.RangeSearch(CustomBoundingBoxND<nDim>{ 4, 8 });
        autoce vidExpected = array{ 2, 3, 4 };
        Assert::IsTrue(std::ranges::is_permutation(vidActual, vidExpected));
      }
    }

    TEST_METHOD(PickSearchAtTheBorder)
    {
      autoce nDim = 1;
      using Vector = CustomVectorTypeND<nDim>;
      using Box = CustomBoundingBoxND<nDim>;

      autoc boxes = array<Box, 5>
      {
        Box{ 0, 1 },
        Box{ 3, 4 },
        Box{ 4, 5 },
        Box{ 5, 6 },
        Box{ 7, 8 }
      };

      autoc tree = OrthoTreeBoxContainerCustom<nDim>(boxes, 3, std::nullopt, 2, false);
      {
        autoc vidActual = tree.PickSearch(Vector{4});
        autoce vidExpected = array{ 1, 2 };
        Assert::IsTrue(std::ranges::is_permutation(vidActual, vidExpected));
      }
    }

    TEST_METHOD(PickSearchAtTheBorder3LevelPos)
    {
      autoce nDim = 1;
      using Vector = CustomVectorTypeND<nDim>;
      using Box = CustomBoundingBoxND<nDim>;

      autoc boxes = array
      {
        Box{ 0, 16 },

        Box{ 0, 1 },
        Box{ 3, 4 },
        Box{ 4, 5 },
        Box{ 5, 6 },

        Box{ 2, 4 },
        Box{ 4, 6 },
        Box{ 2, 6 },
      };

      autoc tree = OrthoTreeBoxContainerCustom<nDim>(boxes, 4, std::nullopt, 2, false);
      {
        autoc vidActual = tree.PickSearch(Vector{ 4 });
        autoce vidExpected = array{ 0, 2, 3, 5, 6, 7 };
        Assert::IsTrue(std::ranges::is_permutation(vidActual, vidExpected));
      }
    }

    TEST_METHOD(PickSearchAtTheBorder3LevelPosNeg)
    {
      autoce nDim = 1;
      using Vector = CustomVectorTypeND<nDim>;
      using Box = CustomBoundingBoxND<nDim>;

      autoc boxes = array
      {
        Box{ -8, 8 },

        Box{  0,  1 },
        Box{ -4, -3 },
        Box{  4,  5 },
        Box{ -6, -5 },

        Box{ -4,  2 },
        Box{  4,  6 },
        Box{ -6,  2 },
      };

      autoc tree = OrthoTreeBoxContainerCustom<nDim>(boxes, 4, std::nullopt, 2, false);
      {
        autoc vidActual = tree.PickSearch(Vector{ -4 });
        autoce vidExpected = array{ 0, 2, 5, 7 };
        Assert::IsTrue(std::ranges::is_permutation(vidActual, vidExpected));
      }
    }

    TEST_METHOD(PickSearchAtTheBorder3LevelNeg)
    {
      autoce nDim = 1;
      using Vector = CustomVectorTypeND<nDim>;
      using Box = CustomBoundingBoxND<nDim>;

      autoc boxes = array
      {
        Box{ -17,  -1 },

        Box{  -2,  -1 },
        Box{  -4, -3 },
        Box{  -5, -4 },
        Box{  -6, -5 },

        Box{  -4, -2 },
        Box{  -6, -4 },
        Box{  -6, -2 },
      };

      autoc tree = OrthoTreeBoxContainerCustom<nDim>(boxes, 4, std::nullopt, 2, false);
      {
        autoc vidActual = tree.PickSearch(Vector{ -4 });
        autoce vidExpected = array{ 0, 2, 3, 5, 6, 7 };
        Assert::IsTrue(std::ranges::is_permutation(vidActual, vidExpected));
      }
    }


    TEST_METHOD(PickSearch_Issue8)
    {
      autoce nDim = 3;

      using Vector = CustomVectorTypeND<nDim>;
      using Box = CustomBoundingBoxND<nDim>;

      auto mGridCells = array<Box, 144>{};
      mGridCells[0] = Box{ Vector{-1024, 0, 1024}, Vector{-961, 0, 1087} };
      mGridCells[1] = Box{ Vector{-1024, 0, 1088}, Vector {-961, 0, 1151} };
      mGridCells[2] = Box{ Vector{-1024, 0, 1152}, Vector {-961, 0, 1215} };
      mGridCells[3] = Box{ Vector{-1024, 0, 1216}, Vector {-961, 0, 1279} };
      mGridCells[4] = Box{ Vector{-1024, 0, 1280}, Vector {-961, 0, 1343} };
      mGridCells[5] = Box{ Vector{-1024, 0, 1344}, Vector {-961, 0, 1407} };
      mGridCells[6] = Box{ Vector{-1024, 0, 1408}, Vector {-961, 0, 1471} };
      mGridCells[7] = Box{ Vector{-1024, 0, 1472}, Vector {-961, 0, 1535} };
      mGridCells[8] = Box{ Vector{-960, 0, 1024}, Vector{-897, 0, 1087} };
      mGridCells[9] = Box{ Vector{-960, 0, 1088}, Vector{-897, 0, 1151} };
      mGridCells[10] = Box{ Vector{-960, 0, 1152}, Vector{-897, 0, 1215} };
      mGridCells[11] = Box{ Vector{-960, 0, 1216}, Vector{-897, 0, 1279} };
      mGridCells[12] = Box{ Vector{-960, 0, 1280}, Vector{-897, 0, 1343} };
      mGridCells[13] = Box{ Vector{-960, 0, 1344}, Vector{-897, 0, 1407} };
      mGridCells[14] = Box{ Vector{-960, 0, 1408}, Vector{-897, 0, 1471} };
      mGridCells[15] = Box{ Vector{-960, 0, 1472}, Vector{-897, 0, 1535} };
      mGridCells[16] = Box{ Vector{-896, 0, 1024}, Vector{-833, 0, 1087} };
      mGridCells[17] = Box{ Vector{-896, 0, 1088}, Vector{-833, 0, 1151} };
      mGridCells[18] = Box{ Vector{-896, 0, 1152}, Vector{-833, 0, 1215} };
      mGridCells[19] = Box{ Vector{-896, 0, 1216}, Vector{-833, 0, 1279} };
      mGridCells[20] = Box{ Vector{-896, 0, 1280}, Vector{-833, 0, 1343} };
      mGridCells[21] = Box{ Vector{-896, 0, 1344}, Vector{-833, 0, 1407} };
      mGridCells[22] = Box{ Vector{-896, 0, 1408}, Vector{-833, 0, 1471} };
      mGridCells[23] = Box{ Vector{-896, 0, 1472}, Vector{-833, 0, 1535} };
      mGridCells[24] = Box{ Vector{-832, 0, 1024}, Vector{-769, 0, 1087} };
      mGridCells[25] = Box{ Vector{-832, 0, 1088}, Vector{-769, 0, 1151} };
      mGridCells[26] = Box{ Vector{-832, 0, 1152}, Vector{-769, 0, 1215} };
      mGridCells[27] = Box{ Vector{-832, 0, 1216}, Vector{-769, 0, 1279} };
      mGridCells[28] = Box{ Vector{-832, 0, 1280}, Vector{-769, 0, 1343} };
      mGridCells[29] = Box{ Vector{-832, 0, 1344}, Vector{-769, 0, 1407} };
      mGridCells[30] = Box{ Vector{-832, 0, 1408}, Vector{-769, 0, 1471} };
      mGridCells[31] = Box{ Vector{-832, 0, 1472}, Vector{-769, 0, 1535} };
      mGridCells[32] = Box{ Vector{-768, 0, 1024}, Vector{-705, 0, 1087} };
      mGridCells[33] = Box{ Vector{-768, 0, 1088}, Vector{-705, 0, 1151} };
      mGridCells[34] = Box{ Vector{-768, 0, 1152}, Vector{-705, 0, 1215} };
      mGridCells[35] = Box{ Vector{-768, 0, 1216}, Vector{-705, 0, 1279} };
      mGridCells[36] = Box{ Vector{-768, 0, 1280}, Vector{-705, 0, 1343} };
      mGridCells[37] = Box{ Vector{-768, 0, 1344}, Vector{-705, 0, 1407} };
      mGridCells[38] = Box{ Vector{-768, 0, 1408}, Vector{-705, 0, 1471} };
      mGridCells[39] = Box{ Vector{-768, 0, 1472}, Vector{-705, 0, 1535} };
      mGridCells[40] = Box{ Vector{-704, 0, 1024}, Vector{-641, 0, 1087} };
      mGridCells[41] = Box{ Vector{-704, 0, 1088}, Vector{-641, 0, 1151} };
      mGridCells[42] = Box{ Vector{-704, 0, 1152}, Vector{-641, 0, 1215} };
      mGridCells[43] = Box{ Vector{-704, 0, 1216}, Vector{-641, 0, 1279} };
      mGridCells[44] = Box{ Vector{-704, 0, 1280}, Vector{-641, 0, 1343} };
      mGridCells[45] = Box{ Vector{-704, 0, 1344}, Vector{-641, 0, 1407} };
      mGridCells[46] = Box{ Vector{-704, 0, 1408}, Vector{-641, 0, 1471} };
      mGridCells[47] = Box{ Vector{-704, 0, 1472}, Vector{-641, 0, 1535} };
      mGridCells[48] = Box{ Vector{-640, 0, 1024}, Vector{-577, 0, 1087} };
      mGridCells[49] = Box{ Vector{-640, 0, 1088}, Vector{-577, 0, 1151} };
      mGridCells[50] = Box{ Vector{-640, 0, 1152}, Vector{-577, 0, 1215} };
      mGridCells[51] = Box{ Vector{-640, 0, 1216}, Vector{-577, 0, 1279} };
      mGridCells[52] = Box{ Vector{-640, 0, 1280}, Vector{-577, 0, 1343} };
      mGridCells[53] = Box{ Vector{-640, 0, 1344}, Vector{-577, 0, 1407} };
      mGridCells[54] = Box{ Vector{-640, 0, 1408}, Vector{-577, 0, 1471} };
      mGridCells[55] = Box{ Vector{-640, 0, 1472}, Vector{-577, 0, 1535} };
      mGridCells[56] = Box{ Vector{-576, 0, 1024}, Vector{-513, 0, 1087} };
      mGridCells[57] = Box{ Vector{-576, 0, 1088}, Vector{-513, 0, 1151} };
      mGridCells[58] = Box{ Vector{-576, 0, 1152}, Vector{-513, 0, 1215} };
      mGridCells[59] = Box{ Vector{-576, 0, 1216}, Vector{-513, 0, 1279} };
      mGridCells[60] = Box{ Vector{-576, 0, 1280}, Vector{-513, 0, 1343} };
      mGridCells[61] = Box{ Vector{-576, 0, 1344}, Vector{-513, 0, 1407} };
      mGridCells[62] = Box{ Vector{-576, 0, 1408}, Vector{-513, 0, 1471} };
      mGridCells[63] = Box{ Vector{-576, 0, 1472}, Vector{-513, 0, 1535} };
      mGridCells[64] = Box{ Vector{-1152, 0, 896}, Vector{-1089, 0, 959} };
      mGridCells[65] = Box{ Vector{-1152, 0, 960}, Vector{-1089, 0, 1023} };
      mGridCells[66] = Box{ Vector{-1152, 0, 1024}, Vector{-1089, 0, 1087} };
      mGridCells[67] = Box{ Vector{-1152, 0, 1088}, Vector{-1089, 0, 1151} };
      mGridCells[68] = Box{ Vector{-1152, 0, 1152}, Vector{-1089, 0, 1215} };
      mGridCells[69] = Box{ Vector{-1152, 0, 1216}, Vector{-1089, 0, 1279} };
      mGridCells[70] = Box{ Vector{-1152, 0, 1280}, Vector{-1089, 0, 1343} };
      mGridCells[71] = Box{ Vector{-1152, 0, 1344}, Vector{-1089, 0, 1407} };
      mGridCells[72] = Box{ Vector{-1152, 0, 1408}, Vector{-1089, 0, 1471} };
      mGridCells[73] = Box{ Vector{-1152, 0, 1472}, Vector{-1089, 0, 1535} };
      mGridCells[74] = Box{ Vector{-1152, 0, 1536}, Vector{-1089, 0, 1599} };
      mGridCells[75] = Box{ Vector{-1152, 0, 1600}, Vector{-1089, 0, 1663} };
      mGridCells[76] = Box{ Vector{-1088, 0, 896}, Vector{-1025, 0, 959} };
      mGridCells[77] = Box{ Vector{-1088, 0, 960}, Vector{-1025, 0, 1023} };
      mGridCells[78] = Box{ Vector{-1088, 0, 1024}, Vector{-1025, 0, 1087} };
      mGridCells[79] = Box{ Vector{-1088, 0, 1088}, Vector{-1025, 0, 1151} };
      mGridCells[80] = Box{ Vector{-1088, 0, 1152}, Vector{-1025, 0, 1215} };
      mGridCells[81] = Box{ Vector{-1088, 0, 1216}, Vector{-1025, 0, 1279} };
      mGridCells[82] = Box{ Vector{-1088, 0, 1280}, Vector{-1025, 0, 1343} };
      mGridCells[83] = Box{ Vector{-1088, 0, 1344}, Vector{-1025, 0, 1407} };
      mGridCells[84] = Box{ Vector{-1088, 0, 1408}, Vector{-1025, 0, 1471} };
      mGridCells[85] = Box{ Vector{-1088, 0, 1472}, Vector{-1025, 0, 1535} };
      mGridCells[86] = Box{ Vector{-1088, 0, 1536}, Vector{-1025, 0, 1599} };
      mGridCells[87] = Box{ Vector{-1088, 0, 1600}, Vector{-1025, 0, 1663} };
      mGridCells[88] = Box{ Vector{-512, 0, 896}, Vector{-449, 0, 959} };
      mGridCells[89] = Box{ Vector{-512, 0, 960}, Vector{-449, 0, 1023} };
      mGridCells[90] = Box{ Vector{-512, 0, 1024}, Vector{-449, 0, 1087} };
      mGridCells[91] = Box{ Vector{-512, 0, 1088}, Vector{-449, 0, 1151} };
      mGridCells[92] = Box{ Vector{-512, 0, 1152}, Vector{-449, 0, 1215} };
      mGridCells[93] = Box{ Vector{-512, 0, 1216}, Vector{-449, 0, 1279} };
      mGridCells[94] = Box{ Vector{-512, 0, 1280}, Vector{-449, 0, 1343} };
      mGridCells[95] = Box{ Vector{-512, 0, 1344}, Vector{-449, 0, 1407} };
      mGridCells[96] = Box{ Vector{-512, 0, 1408}, Vector{-449, 0, 1471} };
      mGridCells[97] = Box{ Vector{-512, 0, 1472}, Vector{-449, 0, 1535} };
      mGridCells[98] = Box{ Vector{-512, 0, 1536}, Vector{-449, 0, 1599} };
      mGridCells[99] = Box{ Vector{-512, 0, 1600}, Vector{-449, 0, 1663} };
      mGridCells[100] = Box{ Vector{-448, 0, 896}, Vector{-385, 0, 959} };
      mGridCells[101] = Box{ Vector{-448, 0, 960}, Vector{-385, 0, 1023} };
      mGridCells[102] = Box{ Vector{-448, 0, 1024}, Vector{-385, 0, 1087} };
      mGridCells[103] = Box{ Vector{-448, 0, 1088}, Vector{-385, 0, 1151} };
      mGridCells[104] = Box{ Vector{-448, 0, 1152}, Vector{-385, 0, 1215} };
      mGridCells[105] = Box{ Vector{-448, 0, 1216}, Vector{-385, 0, 1279} };
      mGridCells[106] = Box{ Vector{-448, 0, 1280}, Vector{-385, 0, 1343} };
      mGridCells[107] = Box{ Vector{-448, 0, 1344}, Vector{-385, 0, 1407} };
      mGridCells[108] = Box{ Vector{-448, 0, 1408}, Vector{-385, 0, 1471} };
      mGridCells[109] = Box{ Vector{-448, 0, 1472}, Vector{-385, 0, 1535} };
      mGridCells[110] = Box{ Vector{-448, 0, 1536}, Vector{-385, 0, 1599} };
      mGridCells[111] = Box{ Vector{-448, 0, 1600}, Vector{-385, 0, 1663} };
      mGridCells[112] = Box{ Vector{-1024, 0, 896}, Vector{-961, 0, 959} };
      mGridCells[113] = Box{ Vector{-1024, 0, 960}, Vector{-961, 0, 1023} };
      mGridCells[114] = Box{ Vector{-960, 0, 896}, Vector{-897, 0, 959} };
      mGridCells[115] = Box{ Vector{-960, 0, 960}, Vector{-897, 0, 1023} };
      mGridCells[116] = Box{ Vector{-896, 0, 896}, Vector{-833, 0, 959} };
      mGridCells[117] = Box{ Vector{-896, 0, 960}, Vector{-833, 0, 1023} };
      mGridCells[118] = Box{ Vector{-832, 0, 896}, Vector{-769, 0, 959} };
      mGridCells[119] = Box{ Vector{-832, 0, 960}, Vector{-769, 0, 1023} };
      mGridCells[120] = Box{ Vector{-768, 0, 896}, Vector{-705, 0, 959} };
      mGridCells[121] = Box{ Vector{-768, 0, 960}, Vector{-705, 0, 1023} };
      mGridCells[122] = Box{ Vector{-704, 0, 896}, Vector{-641, 0, 959} };
      mGridCells[123] = Box{ Vector{-704, 0, 960}, Vector{-641, 0, 1023} };
      mGridCells[124] = Box{ Vector{-640, 0, 896}, Vector{-577, 0, 959} };
      mGridCells[125] = Box{ Vector{-640, 0, 960}, Vector{-577, 0, 1023} };
      mGridCells[126] = Box{ Vector{-576, 0, 896}, Vector{-513, 0, 959} };
      mGridCells[127] = Box{ Vector{-576, 0, 960}, Vector{-513, 0, 1023} };
      mGridCells[128] = Box{ Vector{-1024, 0, 1536}, Vector{-961, 0, 1599} };
      mGridCells[129] = Box{ Vector{-1024, 0, 1600}, Vector{-961, 0, 1663} };
      mGridCells[130] = Box{ Vector{-960, 0, 1536}, Vector{-897, 0, 1599} };
      mGridCells[131] = Box{ Vector{-960, 0, 1600}, Vector{-897, 0, 1663} };
      mGridCells[132] = Box{ Vector{-896, 0, 1536}, Vector{-833, 0, 1599} };
      mGridCells[133] = Box{ Vector{-896, 0, 1600}, Vector{-833, 0, 1663} };
      mGridCells[134] = Box{ Vector{-832, 0, 1536}, Vector{-769, 0, 1599} };
      mGridCells[135] = Box{ Vector{-832, 0, 1600}, Vector{-769, 0, 1663} };
      mGridCells[136] = Box{ Vector{-768, 0, 1536}, Vector{-705, 0, 1599} };
      mGridCells[137] = Box{ Vector{-768, 0, 1600}, Vector{-705, 0, 1663} };
      mGridCells[138] = Box{ Vector{-704, 0, 1536}, Vector{-641, 0, 1599} };
      mGridCells[139] = Box{ Vector{-704, 0, 1600}, Vector{-641, 0, 1663} };
      mGridCells[140] = Box{ Vector{-640, 0, 1536}, Vector{-577, 0, 1599} };
      mGridCells[141] = Box{ Vector{-640, 0, 1600}, Vector{-577, 0, 1663} };
      mGridCells[142] = Box{ Vector{-576, 0, 1536}, Vector{-513, 0, 1599} };
      mGridCells[143] = Box{ Vector{-576, 0, 1600}, Vector{-513, 0, 1663} };

      autoc tree = OrthoTreeBoxContainerCustom<nDim>(mGridCells, 3, std::nullopt, 21, true);
      autoc vidActual = tree.PickSearch(Vector{ -864, 0, 1471 });
      autoce vidExpected = array{ 22 };
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
          autoc[ptr, ec] = std::from_chars(sw.data(), sw.data() + sw.length(), point[iDim]);
          if (ec != std::errc{})
            return points;

          sw.remove_prefix(ptr - sw.data());
          if (!sw.empty())
            sw.remove_prefix(1); // space
        }
      }

      return points;
    }


    template<std::size_t nDim>
    vector<std::size_t> brute_force_search(vector<CustomVectorTypeND<nDim>> const& points, CustomBoundingBoxND<nDim> const& searchbox)
    {
      auto Entities = vector<std::size_t>{};
      autoc nid = points.size();
      for (std::size_t id = 0; id < nid; ++id)
        if (AdaptorCustom<nDim>::DoesBoxContainPoint(searchbox, points[id]))
          Entities.emplace_back(id);

      return Entities;
    }


    TEST_METHOD(BruteForceRangeSearch_UsingPredefinedData_IfAvailable)
    {
      autoce nDim = 3;
      using Vector = CustomVectorTypeND<nDim>;
      using Box = CustomBoundingBoxND<nDim>;
      using Tree = OrthoTreePointContainerCustom<nDim>;

      autoc points = readPointCloud<nDim>("../../../octree_data.txt");
      if (points.empty())
        return;

      autoc searchbox = Box{ Vector{39, 43, 72}, Vector{49, 53, 76} };

      autoc tree = Tree(points, 3, std::nullopt, 2);
      auto vidActual = tree.RangeSearch(searchbox);
      auto vidExpected = brute_force_search(points, searchbox);
      
      // To investigate
      std::ranges::sort(vidActual);
      std::ranges::sort(vidExpected);
      auto missing_ids = vector<std::size_t>{};
      std::ranges::set_difference(vidExpected, vidActual, std::back_inserter(missing_ids));
      auto missing_nodes = vector<OrthoTreePointCustom<nDim>::MortonNodeID>(missing_ids.size());
      std::ranges::transform(missing_ids, missing_nodes.begin(), [&](autoc id) { return tree.GetCore().Find(id); });
      
      Assert::IsTrue(std::ranges::is_permutation(vidActual, vidExpected));
    }

    template<int nDim, typename TGeometry>
    vector<BoundingBoxND<nDim, TGeometry>> readBoxCloud(std::filesystem::path const& path)
    {
      auto boxes = vector<BoundingBoxND<nDim, TGeometry>>{};
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
          autoc[ptr, ec] = std::from_chars(sw.data(), sw.data() + sw.length(), box.Min[iDim]);
          if (ec != std::errc{})
            return boxes;

          sw.remove_prefix(ptr - sw.data());
          if (!sw.empty())
            sw.remove_prefix(2); // space
        }

        sw.remove_prefix(9);
        for (int iDim = 0; iDim < nDim; ++iDim)
        {
          autoc[ptr, ec] = std::from_chars(sw.data(), sw.data() + sw.length(), box.Max[iDim]);
          if (ec != std::errc{})
            return boxes;

          sw.remove_prefix(ptr - sw.data());
          if (!sw.empty())
            sw.remove_prefix(2); // space
        }
      }

      return boxes;
    }


    TEST_METHOD(Issue26)
    {
      const auto boxes = readBoxCloud<3, float>("../../../bbox_output.txt");
      if (boxes.empty())
        return;

      using Tree = TreeBoxContainerND<3, 2, float>;

      autoc tree = Tree(boxes, 4, std::nullopt, 21, false);

      srand(0);
      const auto boxOfTree = tree.GetCore().GetBox();
      const float rayIntersectTolerance = 0.1f;
      for (int iTry = 0; iTry < 100; ++iTry)
      {
        constexpr int mod = 1000;
        constexpr auto modr = 0.0011f; // it ensures a little bit bigger space, to make possible out of tree test

        const float searchSizeY = float(rand() % mod) * modr * (boxOfTree.Max[1] - boxOfTree.Min[1]);
        const auto x = float(rand() % mod) * modr * (boxOfTree.Max[0] - boxOfTree.Min[0]);
        const auto y = float(rand() % mod) * modr * (boxOfTree.Max[1] - boxOfTree.Min[1]);
        const auto z = float(rand() % mod) * modr * (boxOfTree.Max[2] - boxOfTree.Min[2]);
        const auto pos = Tree::TVector{ x, y, z };
        const auto searchBox = Tree::TBox{
          {pos[0] - rayIntersectTolerance, pos[1] - searchSizeY, pos[2] - rayIntersectTolerance},
          {pos[0] + rayIntersectTolerance,               pos[1], pos[2] + rayIntersectTolerance}
        };

        
        auto resultOfBruteForce = vector<std::size_t>{};
        autoc boxNo = boxes.size();
        for (std::size_t id = 0; id < boxNo; ++id)
        {
          if (Tree::AD::AreBoxesOverlapped(searchBox, boxes[id], false, false))
            resultOfBruteForce.emplace_back(id);
        }

        auto resultOfSearchBox = tree.RangeSearch<false>(searchBox);
        std::ranges::sort(resultOfSearchBox);

        std::vector<OctreeBox::MortonNodeID> nodeIDs;
        for (auto entityID : resultOfBruteForce)
          nodeIDs.emplace_back(tree.GetCore().GetNodeIDByEntity(entityID));

        auto resultOfRay = tree.RayIntersectedAll(pos, { 0.0, -1.0, 0.0 }, rayIntersectTolerance, searchSizeY);
        std::ranges::sort(resultOfRay);

        Assert::IsTrue(resultOfBruteForce == resultOfSearchBox);
        Assert::IsTrue(resultOfBruteForce == resultOfRay);
      }
    }

  };
}

namespace CompileTest
{
  TEST_CLASS(CompileTest)
  {
    TEST_METHOD(AllFunc)
    {
      testCompileBoxBatchSplitStrategy();
    }

  };
}
