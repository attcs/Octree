#include "pch.h"
#include "CppUnitTest.h"
#include "../octree.h"

#define autoc auto const
#define autoce auto constexpr


using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace NTree;


namespace
{
  autoce BB1_INV = BoundingBox1D{ numeric_limits<double>::infinity(), -numeric_limits<double>::infinity() };

  static bool AreEqualAlmost(double l, double r)
  {
    return l == r || abs(l - r) < numeric_limits<double>::min() * 10; // l and r could be inf.
  }

  template<dim_type nDimension, typename geometry_type = double>
  static bool AreEqualAlmost(BoundingBoxND<nDimension, geometry_type> const& l, BoundingBoxND<nDimension, geometry_type> const& r)
  {
    for (dim_type iD = 0; iD < nDimension; ++iD)
    {
      if (!AreEqualAlmost(l.Min[iD], r.Min[iD]))
        return false;

      if (!AreEqualAlmost(l.Max[iD], r.Max[iD]))
        return false;
    }

    return true;
  }
}
namespace Microsoft {
  namespace VisualStudio {
    namespace CppUnitTestFramework
    {
      template<> inline std::wstring ToString<uint16_t>(const uint16_t& t) { RETURN_WIDE_STRING(t); }
      template<> inline std::wstring ToString<bitset<9>>(const bitset<9>& t) { RETURN_WIDE_STRING(t.to_ullong()); }
      template<> inline std::wstring ToString<bitset<13>>(const bitset<13>& t) { RETURN_WIDE_STRING(t.to_ullong()); }
      template<> inline std::wstring ToString<bitset<17>>(const bitset<17>& t) { RETURN_WIDE_STRING(t.to_ullong()); }
      template<> inline std::wstring ToString<bitset<18>>(const bitset<18>& t) { RETURN_WIDE_STRING(t.to_ullong()); }
      template<> inline std::wstring ToString<bitset<22>>(const bitset<22>& t) { RETURN_WIDE_STRING(t.to_ullong()); }
      template<> inline std::wstring ToString<bitset<26>>(const bitset<26>& t) { RETURN_WIDE_STRING(t.to_ullong()); }

    }
  }
}
namespace GeneralTest
{
  TEST_CLASS(MortonTest)
  {
  public:
    TEST_METHOD(M1D_0_0)
    {
      autoce arr = array<grid_id_type, 1>{ 0 };
      Assert::AreEqual(DualtreePoint::Morton(arr), DualtreePoint::morton_grid_id_type(0));
    }

    TEST_METHOD(M1D_4_4)
    {
      autoce arr = array<grid_id_type, 1>{ 4 };
      Assert::AreEqual(DualtreePoint::Morton(arr), DualtreePoint::morton_grid_id_type(4));
    }


    TEST_METHOD(M2D_00_0)
    {
      autoce arr = array<grid_id_type, 2>{ 0, 0 };
      Assert::AreEqual(QuadtreePoint::Morton(arr), QuadtreePoint::morton_grid_id_type(0));
    }

    TEST_METHOD(M2D_20_4)
    {
      autoce arr = array<grid_id_type, 2>{ 2, 0 };
      Assert::AreEqual(QuadtreePoint::Morton(arr), QuadtreePoint::morton_grid_id_type(4));
    }

    TEST_METHOD(M2D_02_8)
    {
      autoce arr = array<grid_id_type, 2>{ 0, 2 };
      Assert::AreEqual(QuadtreePoint::Morton(arr), QuadtreePoint::morton_grid_id_type(8));
    }

    TEST_METHOD(M2D_22_12)
    {
      autoce arr = array<grid_id_type, 2>{ 2, 2 };
      Assert::AreEqual(QuadtreePoint::Morton(arr), QuadtreePoint::morton_grid_id_type(12));
    }

    TEST_METHOD(M2D_13_11)
    {
      autoce arr = array<grid_id_type, 2>{ 1, 3 };
      Assert::AreEqual(QuadtreePoint::Morton(arr), QuadtreePoint::morton_grid_id_type(11));
    }

    TEST_METHOD(M3D_000_0)
    {
      autoce arr = array<grid_id_type, 3>{ 0, 0, 0 };
      Assert::AreEqual(OctreePoint::Morton(arr), OctreePoint::morton_grid_id_type(0));
    }

    TEST_METHOD(M3D_100_1)
    {
      autoce arr = array<grid_id_type, 3>{ 1, 0, 0 };
      Assert::AreEqual(OctreePoint::Morton(arr), OctreePoint::morton_grid_id_type(1));
    }

    TEST_METHOD(M3D_001_4)
    {
      autoce arr = array<grid_id_type, 3>{ 0, 0, 1 };
      Assert::AreEqual(OctreePoint::Morton(arr), OctreePoint::morton_grid_id_type(4));
    }

    TEST_METHOD(M3D_111_7)
    {
      autoce arr = array<grid_id_type, 3>{ 1, 1, 1 };
      Assert::AreEqual(OctreePoint::Morton(arr), OctreePoint::morton_grid_id_type(7));
    }

    TEST_METHOD(M4D_1111_15)
    {
      autoce arr = array<grid_id_type, 4>{ 1, 1, 1, 1 };
      Assert::AreEqual(HexatreePoint::Morton(arr), HexatreePoint::morton_grid_id_type(15));
    }

    TEST_METHOD(M4D_2111_30)
    {
      autoce arr = array<grid_id_type, 4>{ 2, 1, 1, 1 };
      Assert::AreEqual(HexatreePoint::Morton(arr), HexatreePoint::morton_grid_id_type(30));
    }
  };

  TEST_CLASS(NodeTest)
  {
    template<dim_type N>
    static void Complex_ND_Only1()
    {
      using child_id_type_ = TreeBoxND<N>::child_id_type;
      auto node = TreeBoxND<N>::Node();
      Assert::IsFalse(node.IsAnyChildExist());

      autoce nChild = 1 << N;
      for (child_id_type_ idChild = 0; idChild < nChild; ++idChild)
      {
        node.EnableChild(idChild);
        Assert::IsTrue(node.HasChild(idChild));
        Assert::IsTrue(node.IsAnyChildExist());

        autoc vChild = node.GetChildren();
        Assert::AreEqual<size_t>(1, vChild.size());
        Assert::AreEqual(idChild, vChild[0]);

        node.DisableChild(idChild);
        autoc vChild2 = node.GetChildren();
        Assert::AreEqual<size_t>(0, vChild2.size());
      }
    }


    TEST_METHOD(Complex_1D_Only1) { Complex_ND_Only1<1>(); }
    TEST_METHOD(Complex_2D_Only1) { Complex_ND_Only1<2>(); }
    TEST_METHOD(Complex_3D_Only1) { Complex_ND_Only1<3>(); }
    TEST_METHOD(Complex_4D_Only1) { Complex_ND_Only1<4>(); }

    TEST_METHOD(Complex_16D_Only1) { Complex_ND_Only1<16>(); }
    //TEST_METHOD(Complex_24D_Only1) { Complex_ND_Only1<24>(); }


    template<dim_type N>
    static void Complex_All_ND()
    {
      using child_id_type_ = TreeBoxND<N>::child_id_type;
      auto node = TreeBoxND<N>::Node();

      autoce nChild = 1 << N;
      for (child_id_type_ idChild = 0; idChild < nChild; ++idChild)
      {
        node.EnableChild(idChild);
        Assert::IsTrue(node.HasChild(idChild));
        Assert::IsTrue(node.IsAnyChildExist());

        autoc vChild = node.GetChildren();
        Assert::AreEqual<size_t>(idChild + 1, vChild.size());
      }

      for (child_id_type_ idChild = 0; idChild < nChild; ++idChild)
      {
        node.DisableChild(idChild);
        autoc vChildActual = node.GetChildren();
        auto vChildExpected = vector<child_id_type_>(nChild - idChild - 1);
        std::iota(begin(vChildExpected), end(vChildExpected), idChild + 1);
        Assert::IsTrue(std::ranges::is_permutation(vChildExpected, vChildActual));
      }

      Assert::IsFalse(node.IsAnyChildExist());
    }

    TEST_METHOD(Complex_1D_All) { Complex_All_ND<1>(); }
    TEST_METHOD(Complex_2D_All) { Complex_All_ND<2>(); }
    TEST_METHOD(Complex_3D_All) { Complex_All_ND<3>(); }
    TEST_METHOD(Complex_4D_All) { Complex_All_ND<4>(); }

    TEST_METHOD(Complex_16D_All) { Complex_All_ND<16>(); }
    //TEST_METHOD(Complex_24D_All) { Complex_All_ND<24>(); }
  };

  TEST_CLASS(NTreeLinearTest)
  {
    using morton_node_id_type = DualtreePoint::morton_node_id_type;
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
      Assert::AreEqual(DualtreePoint::GetHash(0,0), DualtreePoint::morton_node_id_type(1));
    }

    TEST_METHOD(GetHash__11_3)
    {
      Assert::AreEqual(DualtreePoint::GetHash(1, 1), DualtreePoint::morton_node_id_type(3));
    }
    TEST_METHOD(GetHash__22_4)
    {
      Assert::AreEqual(DualtreePoint::GetHash(2, 2), DualtreePoint::morton_node_id_type(6));
    }

    TEST_METHOD(GetHash__37_15)
    {
      Assert::AreEqual(DualtreePoint::GetHash(3, 7), DualtreePoint::morton_node_id_type(15));
    }

    TEST_METHOD(GetDepth__37_15__3)
    {
      autoc lc = DualtreePoint::GetDepth(DualtreePoint::GetHash(3, 7));
      Assert::AreEqual(lc, DualtreePoint::depth_type(3));
    }

    TEST_METHOD(RemoveSentinelBit__37_15__7)
    {
      autoc lc = DualtreePoint::RemoveSentinelBit(DualtreePoint::GetHash(3, 7));
      Assert::AreEqual(lc, morton_node_id_type(7));
    }

    TEST_METHOD(Init)
    {
      auto tree = DualtreePoint{};
      autoc bb = BoundingBox1D{ -1, +1 };
      tree.Init(bb, 3, 10);
      Assert::IsTrue(AreEqualAlmost(bb, tree.GetBox()));
      Assert::AreEqual<size_t>(tree.GetNodeSize(), 1);
      Assert::AreEqual<size_t>(tree.GetDepthMax(), 3);
      Assert::AreEqual<size_t>(tree.GetResolutionMax(), 8);
    }
    
    TEST_METHOD(VisitNodes__points__0123)
    {
      autoce vpt = array{ Point1D{ 0.0 }, Point1D{ 1.0 }, Point1D{ 2.0 }, Point1D{ 3.0 } };
      autoc tree = DualtreePoint::Create(vpt, 2, std::nullopt, 2);

      auto ids = vector<size_t>();
      tree.VisitNodes(tree.GetRootKey()
        , [&ids](autoc key, autoc& node) { ids.insert(end(ids), begin(node.vid), end(node.vid)); }
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
      autoc tree = DualtreeBox::Create(vBox, 3, std::nullopt, 2);

      auto ids = vector<size_t>();
      tree.VisitNodes(tree.GetRootKey()
        , [&ids](autoc key, autoc& node) { ids.insert(end(ids), begin(node.vid), end(node.vid)); }
      );

      Assert::IsTrue(ids == vector<size_t>{6,  4, 5,  0, 1, 2, 3 });
    }


    TEST_METHOD(EraseId__2__EmptyNode11)
    {
      autoce vpt = array{ Point1D{ 0.0 }, Point1D{ 1.0 }, Point1D{ 2.0 }, Point1D{ 3.0 } };
      auto tree = DualtreePoint::Create(vpt, 2, std::nullopt, 2);

      autoc kNode = tree.GetHash(2, 2);
      autoc& node = tree.Get(kNode);
      Assert::AreEqual<size_t>(node.vid.size(), 1);
      tree.EraseId(2);
      Assert::IsTrue(node.vid.empty());
    }

    TEST_METHOD(UpdateIndexes__None__NothingsChange)
    {
      autoce vBox = array
      {
        BoundingBox1D{ 0.0, 1.0 }, BoundingBox1D{ 1.0, 2.0 }, BoundingBox1D{ 2.0, 3.0 }, BoundingBox1D{ 3.0, 4.0 },
        BoundingBox1D{ 0.0, 2.0 }, BoundingBox1D{ 2.0, 4.0 },
        BoundingBox1D{ 0.0, 4.0 }
      };
      auto tree = DualtreeBox::Create(vBox, 3, std::nullopt, 2);
      tree.UpdateIndexes({});

      auto ids = vector<size_t>();
      tree.VisitNodes(tree.GetRootKey()
        , [&ids](autoc key, autoc& node) { ids.insert(end(ids), begin(node.vid), end(node.vid)); }
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
      auto tree = DualtreeBox::Create(vBox, 3, std::nullopt, 2);
      tree.UpdateIndexes({ { 2, 7 } });

      auto ids = vector<size_t>();
      tree.VisitNodes(tree.GetRootKey()
        , [&ids](autoc key, autoc& node) { ids.insert(end(ids), begin(node.vid), end(node.vid)); }
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
      auto tree = DualtreeBox::Create(vBox, 3, std::nullopt, 2);
      tree.UpdateIndexes({ { 2, DualtreeBox::ERASE } });

      auto ids = vector<size_t>();
      tree.VisitNodes(tree.GetRootKey()
        , [&ids](autoc key, autoc& node) { ids.insert(end(ids), begin(node.vid), end(node.vid)); }
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
      auto tree = DualtreeBox::Create(vBox, 3, std::nullopt, 2);
      tree.UpdateIndexes(
        {
          { 3, 6 },
          { 6, 3 },
        }
      );


      auto ids = vector<size_t>();
      tree.VisitNodes(tree.GetRootKey()
        , [&ids](autoc key, autoc& node) { ids.insert(end(ids), begin(node.vid), end(node.vid)); }
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
      auto tree = DualtreeBox::Create(vBox, 3, std::nullopt, 2);
      tree.UpdateIndexes(
        {
          { 1, 0 },
          { 0, 1 },
        }
      );


      auto ids = vector<size_t>();
      tree.VisitNodes(tree.GetRootKey()
        , [&ids](autoc key, autoc& node) { ids.insert(end(ids), begin(node.vid), end(node.vid)); }
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
      auto tree = DualtreeBox::Create(vBox, 3, std::nullopt, 2);
      tree.UpdateIndexes(
        { 
          { 6, 3 },
          { 3, 6 },
          { 4, 5 },
          { 5, 1 },
          { 1, DualtreeBox::ERASE },
        }
      );
     
      autoc ids = tree.CollectAllIdInBFS();
      // Assert::IsTrue(ids == vector<size_t>{6, 4, 5, 0, 1, 2, 3 });

      Assert::IsTrue(ids == vector<size_t>{3, 5, 1, 0, 2, 6 });
    }

    TEST_METHOD(Clear__EmptyRootRemains)
    {
      autoce vBox = array
      {
        BoundingBox1D{ 0.0, 4.0 },
        BoundingBox1D{ 0.0, 2.0 }, BoundingBox1D{ 2.0, 4.0 },
        BoundingBox1D{ 0.0, 1.0 }, BoundingBox1D{ 1.0, 2.0 }, BoundingBox1D{ 2.0, 3.0 }, BoundingBox1D{ 3.0, 4.0 }
      };
      auto tree = DualtreeBox::Create(vBox, 3, std::nullopt, 2);

      autoc& nodes = tree.Get();
      Assert::AreEqual<size_t>(nodes.size(), 7);
      tree.Clear();
      Assert::AreEqual<size_t>(nodes.size(), 1);
      autoc node = nodes.at(tree.GetHash(0, 0));
      Assert::AreEqual<size_t>(node.vid.empty(), 1);
    }


    TEST_METHOD(GetExtent_)
    {
      //!
    }
  };
}

namespace DualtreePointTest
{
	TEST_CLASS(CreateTest)
	{
	public:
		
		TEST_METHOD(Empty)
		{
			autoc tree = DualtreePoint::Create({}, 2);
			autoc& nodes = tree.Get();
			Assert::IsTrue(nodes.size() == 1);
      Assert::IsTrue(nodes.at(1).vid.empty());
      Assert::IsTrue(AreEqualAlmost(tree.GetBox(), BB1_INV));
		}

    // ext    //!
    TEST_METHOD(NoPt1)
    {
      autoc tree = DualtreePoint::Create(vector<Point1D>{ { 1.0 } }, 2);
      autoc& nodes = tree.Get();
      Assert::IsTrue(nodes.size() == 2);
      Assert::IsTrue(nodes.at(1).vid.empty());
      Assert::IsTrue(AreEqualAlmost(tree.GetBox(), BoundingBox1D{ Point1D{1.0}, Point1D{1.0} }));
    }

    // ext     //!
    TEST_METHOD(NoPt2)
    {
      autoc tree = DualtreePoint::Create({}, 2);
      autoc& nodes = tree.Get();
      Assert::IsTrue(nodes.size() == 1);
      Assert::IsTrue(nodes.at(1).vid.empty());
    }

    TEST_METHOD(NoPt4)
    {
      autoce vpt = array{ Point1D{ 0.0 }, Point1D{ 1.0 }, Point1D{ 2.0 }, Point1D{ 3.0 } };
      autoc tree = DualtreePoint::Create(vpt, 2, std::nullopt, 2);

      Assert::AreEqual<size_t>(tree.GetNodeSize(), 7);
      Assert::IsTrue(AreEqualAlmost(tree.GetBox(), BoundingBox1D{ Point1D{0.0}, Point1D{3.0} }));
    }

    //!
    TEST_METHOD(Init)
    {
      auto tree = DualtreePoint{};
      tree.Init(BoundingBox1D{ { -1.0 }, { +1.0 } }, 2);
      
      autoc& nodes = tree.Get();
      Assert::IsTrue(nodes.size() == 1);
      Assert::IsTrue(nodes.at(1).vid.empty());
    }
	};


  TEST_CLASS(SearchTest)
  {
    TEST_METHOD(FindSmallestNode_At30)
    {
      using morton_node_id_type = DualtreePoint::morton_node_id_type;

      autoce vpt = array{ Point1D{ 0.0 }, Point1D{ 1.0 }, Point1D{ 2.0 }, Point1D{ 3.0 } };
      autoc tree = DualtreePoint::Create(vpt, 2, std::nullopt, 2);
      autoc kLast = tree.FindSmallestNode(vpt.back());
      Assert::AreEqual<morton_node_id_type>(7, kLast);
    }

    TEST_METHOD(FindSmallestNode_At29)
    {
      using morton_node_id_type = DualtreePoint::morton_node_id_type;

      autoce vpt = array{ Point1D{ 0.0 }, Point1D{ 1.0 }, Point1D{ 2.0 }, Point1D{ 3.0 } };
      autoc tree = DualtreePoint::Create(vpt, 2, std::nullopt, 2);
      autoc kLast = tree.FindSmallestNode({2.9});
      Assert::AreEqual<morton_node_id_type>(7, kLast);
    }


    TEST_METHOD(FindSmallestNode_At15)
    {
      using morton_node_id_type = DualtreePoint::morton_node_id_type;

      autoce vpt = array{ Point1D{ 0.0 }, Point1D{ 1.0 }, Point1D{ 2.0 }, Point1D{ 3.0 } };
      autoc tree = DualtreePoint::Create(vpt, 2, std::nullopt, 2);
      autoc kLast = tree.FindSmallestNode({ 1.5 });
      Assert::AreEqual<morton_node_id_type>(6, kLast);
    }

    TEST_METHOD(RangeSearch__EqualAtBorder__12)
    {
      autoce vpt = array{ Point1D{ 0.0 }, Point1D{ 1.0 }, Point1D{ 2.0 }, Point1D{ 3.0 } };
      autoc tree = DualtreePoint::Create(vpt, 2, std::nullopt, 2);
      autoc ids = tree.RangeSearch({ 1.0, 2.0 }, vpt);

      Assert::IsTrue(std::ranges::is_permutation(vector{1, 2}, ids));
    }

    TEST_METHOD(RangeSearch__Usual__23)
    {
      autoce vpt = array{ Point1D{ 0.0 }, Point1D{ 1.0 }, Point1D{ 2.0 }, Point1D{ 3.0 } };
      autoc tree = DualtreePoint::Create(vpt, 2, std::nullopt, 2);
      autoc ids = tree.RangeSearch({ 1.1, 3.1 }, vpt);

      Assert::IsTrue(std::ranges::is_permutation(vector{ 2, 3 }, ids));
    }

    TEST_METHOD(RangeSearch__AtNodeBorder__2)
    {
      autoce vpt = array{ Point1D{ 0.0 }, Point1D{ 1.0 }, Point1D{ 2.0 }, Point1D{ 3.0 } };
      autoc tree = DualtreePoint::Create(vpt, 2, std::nullopt, 2);
      autoc ids = tree.RangeSearch({ 1.5, 2.25 }, vpt);

      Assert::IsTrue(std::ranges::is_permutation(vector{ 2 }, ids));
    }

    TEST_METHOD(RangeSearch__ZeroBox__Empty)
    {
      autoce vpt = array{ Point1D{ 0.0 }, Point1D{ 1.0 }, Point1D{ 2.0 }, Point1D{ 3.0 } };
      autoc tree = DualtreePoint::Create(vpt, 2, std::nullopt, 2);
      autoc ids = tree.RangeSearch({}, vpt);

      Assert::IsTrue(ids.empty());
    }

    TEST_METHOD(RangeSearch__Empty)
    {
      autoce vpt = array{ Point1D{ 0.0 }, Point1D{ 1.0 }, Point1D{ 2.0 }, Point1D{ 3.0 } };
      autoc tree = DualtreePoint::Create(vpt, 2, std::nullopt, 2);
      autoc ids = tree.RangeSearch({ 1.5, 1.75 }, vpt);

      Assert::IsTrue(ids.empty());
    }
  };


  TEST_CLASS(EditTest)
  {
    TEST_METHOD(Insert__NonLeaf__Successful)
    {
      autoce vpt = array{ Point1D{ 0.0 }, Point1D{ 1.0 }, Point1D{ 2.0 }, Point1D{ 3.0 } };
      auto tree = DualtreePoint::Create(vpt, 3, std::nullopt, 2);

      Assert::IsTrue(tree.Insert(4, Point1D{ 2.5 }, false));

      autoc& nodes = tree.Get();
      Assert::AreEqual<size_t>(tree.GetNodeSize(), 7);
      Assert::IsTrue(nodes.at(7).vid == vector<size_t>{ 3, 4 });
    }

    TEST_METHOD(Insert__Leaf__Successful)
    {
      autoce vpt = array{ Point1D{ 0.0 }, Point1D{ 1.0 }, Point1D{ 2.0 }, Point1D{ 3.0 } };
      auto tree = DualtreePoint::Create(vpt, 3, std::nullopt, 2);

      Assert::IsTrue(tree.Insert(4, Point1D{ 2.5 }, true));

      autoc& nodes = tree.Get();
      Assert::AreEqual<size_t>(tree.GetNodeSize(), 8);
      Assert::IsTrue(nodes.at(14).vid == vector<size_t>{ 4 });
    }

    TEST_METHOD(Insert__OutOfSpace__ReturnsFalse)
    {
      autoce vpt = array{ Point1D{ 0.0 }, Point1D{ 1.0 }, Point1D{ 2.0 }, Point1D{ 3.0 } };
      auto tree = DualtreePoint::Create(vpt, 3, std::nullopt, 2);
      Assert::IsFalse(tree.Insert(4, Point1D{ 4.0 }, true));

      autoc& nodes = tree.Get();
      Assert::AreEqual<size_t>(tree.GetNodeSize(), 7);
      Assert::IsTrue(nodes.at(7).vid == vector<size_t>{ 3 });
    }


    TEST_METHOD(Update3p_)
    {
      //!
    }

    TEST_METHOD(Update2p__Rewrite3__Successful)
    {
      auto vpt = vector{ Point1D{ 0.0 }, Point1D{ 1.0 }, Point1D{ 2.0 }, Point1D{ 3.0 } };

      auto tree = DualtreePoint::Create(vpt, 3, std::nullopt, 2);
      
      Assert::IsTrue(tree.Update(3, { 1.1 }));
      autoc ids = tree.CollectAllIdInBFS();
      Assert::IsTrue(ids == vector<NTree::entity_id_type>{ 0, 1, 3, 2 });
    }

    TEST_METHOD(Update2p__Rewrite2__Successful)
    {
      auto vpt = vector{ Point1D{ 0.0 }, Point1D{ 1.0 }, Point1D{ 2.0 }, Point1D{ 3.0 } };

      auto tree = DualtreePoint::Create(vpt, 3, std::nullopt, 2);

      Assert::IsTrue(tree.Update(2, { 1.1 }));
      autoc ids = tree.CollectAllIdInBFS();
      Assert::IsTrue(ids == vector<NTree::entity_id_type>{ 0, 1, 2, 3 });
    }


    TEST_METHOD(Update2p__Rewrite3OutOfBox__ReturnsFalse)
    {
      auto vpt = vector{ Point1D{ 0.0 }, Point1D{ 1.0 }, Point1D{ 2.0 }, Point1D{ 3.0 } };

      auto tree = DualtreePoint::Create(vpt, 3, std::nullopt, 2);

      autoc idsPre = tree.CollectAllIdInBFS();
      Assert::IsFalse(tree.Update(2, { 3.1 }));
      autoc idsPost = tree.CollectAllIdInBFS();

      Assert::IsTrue(idsPre == idsPost);
    }


    TEST_METHOD(Erase__3__Removed)
    {
      autoce vpt = array{ Point1D{ 0.0 }, Point1D{ 1.0 }, Point1D{ 2.0 }, Point1D{ 3.0 } };
      auto tree = DualtreePoint::Create(vpt, 3, std::nullopt, 2);

      Assert::IsTrue(tree.Erase(3, vpt.back()));
      autoc ids = tree.CollectAllIdInBFS();
      Assert::IsTrue(std::ranges::find(ids, 3) == end(ids));
    }

    TEST_METHOD(Erase__Nonexist__ReturnsFalse)
    {
      autoce vpt = array{ Point1D{ 0.0 }, Point1D{ 1.0 }, Point1D{ 2.0 }, Point1D{ 3.0 } };
      auto tree = DualtreePoint::Create(vpt, 3, std::nullopt, 2);

      autoc idsPre = tree.CollectAllIdInBFS();
      Assert::IsFalse(tree.Erase(4, vpt.back()));
      autoc idsPost = tree.CollectAllIdInBFS();

      Assert::IsTrue(idsPre == idsPost);
    }

    TEST_METHOD(Erase__WrongPointWasGiven__ReturnsFalse)
    {
      autoce vpt = array{ Point1D{ 0.0 }, Point1D{ 1.0 }, Point1D{ 2.0 }, Point1D{ 3.0 } };
      auto tree = DualtreePoint::Create(vpt, 3, std::nullopt, 2);

      autoc idsPre = tree.CollectAllIdInBFS();
      Assert::IsFalse(tree.Erase(3, vpt.front()));
      autoc idsPost = tree.CollectAllIdInBFS();

      Assert::IsTrue(idsPre == idsPost);
    }

  };
}


namespace DualtreeBoxTest
{

  TEST_CLASS(SearchTest)
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

    TEST_METHOD(CollistionDetection__0040_3565__P30)
    {
      autoce vBoxL = array{ BoundingBox1D{ 0.0, 1.0 }, BoundingBox1D{ 1.0, 2.0 }, BoundingBox1D{ 2.0, 3.0 }, BoundingBox1D{ 3.0, 4.0 } };
      autoc treeL = DualtreeBox::Create(vBoxL, 3, std::nullopt, 2);

      autoce vBoxR = array{ BoundingBox1D{ 3.5, 4.5 },  BoundingBox1D{ 4.5, 5.5 },  BoundingBox1D{ 5.5, 6.5 } };
      autoc treeR = DualtreeBox::Create(vBoxR, 3, std::nullopt, 2);

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
      autoc treeL = DualtreeBox::Create(vBoxL, 3, std::nullopt, 2);

      autoce vBoxR = array
      { 
        BoundingBox1D{ 2.0, 6.0 },
        BoundingBox1D{ 2.0, 4.0 }, BoundingBox1D{ 4.0, 6.0 }, 
        BoundingBox1D{ 2.0, 3.0 }, BoundingBox1D{ 3.0, 4.0 }, BoundingBox1D{ 4.0, 5.0 }, BoundingBox1D{ 5.0, 6.0 }
      };
      autoc treeR = DualtreeBox::Create(vBoxR, 3, std::nullopt, 2);

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

  TEST_CLASS(EditTest)
  {
    TEST_METHOD(Insert_IntoRoot_Successful)
    {
      autoce vBoxL = array{ BoundingBox1D{ 0.0, 1.0 }, BoundingBox1D{ 1.0, 2.0 }, BoundingBox1D{ 2.0, 3.0 }, BoundingBox1D{ 3.0, 4.0 } };
      auto tree = DualtreeBox::Create(vBoxL, 3, std::nullopt, 2);

      Assert::IsTrue(tree.Insert(4, BoundingBox1D{ 0, 4 }, false));

      autoc& nodes = tree.Get();
      Assert::AreEqual<size_t>(tree.GetNodeSize(), 7);
      Assert::IsTrue(nodes.at(tree.GetRootKey()).vid == vector<size_t>{ 4 });
    }


    TEST_METHOD(Insert_NonLeaf_Successful)
    {
      autoce vBoxL = array{ BoundingBox1D{ 0.0, 1.0 }, BoundingBox1D{ 1.0, 2.0 }, BoundingBox1D{ 2.0, 3.0 }, BoundingBox1D{ 3.0, 4.0 } };
      auto tree = DualtreeBox::Create(vBoxL, 3, std::nullopt, 2);

      Assert::IsTrue(tree.Insert(4, BoundingBox1D{ 3.5, 3.7 }, false));

      autoc& nodes = tree.Get();
      Assert::AreEqual<size_t>(tree.GetNodeSize(), 7);
      Assert::IsTrue(nodes.at(7).vid == vector<size_t>{ 3, 4 });
    }
    
    TEST_METHOD(Insert_Leaf_Successful)
    {
      autoce vBoxL = array{ BoundingBox1D{ 0.0, 1.0 }, BoundingBox1D{ 1.0, 2.0 }, BoundingBox1D{ 2.0, 3.0 }, BoundingBox1D{ 3.0, 4.0 } };
      auto tree = DualtreeBox::Create(vBoxL, 3, std::nullopt, 2);

      Assert::IsTrue(tree.Insert(4, BoundingBox1D{ 3.5, 3.7 }, true));

      autoc& nodes = tree.Get();
      Assert::AreEqual<size_t>(tree.GetNodeSize(), 8);
      Assert::IsTrue(nodes.at(15).vid == vector<size_t>{ 4 });
    }


    TEST_METHOD(Insert_LeafBut1_Successful)
    {
      autoce vBoxL = array{ BoundingBox1D{ 0.0, 1.0 }, BoundingBox1D{ 1.0, 2.0 }, BoundingBox1D{ 2.0, 3.0 }, BoundingBox1D{ 3.0, 4.0 } };
      auto tree = DualtreeBox::Create(vBoxL, 3, std::nullopt, 2);

      Assert::IsTrue(tree.Insert(4, BoundingBox1D{ 1.0, 3.0 }, true));

      autoc& nodes = tree.Get();
      Assert::AreEqual<size_t>(tree.GetNodeSize(), 7);
      Assert::IsTrue(nodes.at(1).vid == vector<size_t>{ 4 });
    }


    TEST_METHOD(Insert_LeafBut2_Successful)
    {
      autoce vBoxL = array{ BoundingBox1D{ 0.0, 1.0 }, BoundingBox1D{ 1.0, 2.0 }, BoundingBox1D{ 2.0, 3.0 }, BoundingBox1D{ 3.0, 4.0 } };
      auto tree = DualtreeBox::Create(vBoxL, 3, std::nullopt, 2);

      Assert::IsTrue(tree.Insert(4, BoundingBox1D{ 0.0, 2.0 }, true));

      autoc& nodes = tree.Get();
      Assert::AreEqual<size_t>(tree.GetNodeSize(), 7);
      Assert::IsTrue(nodes.at(2).vid == vector<size_t>{ 4 });
    }


    TEST_METHOD(Complex)
    {
      autoce vBoxL = array{ BoundingBox1D{ 0.0, 1.0 }, BoundingBox1D{ 1.0, 2.0 }, BoundingBox1D{ 2.0, 3.0 }, BoundingBox1D{ 3.0, 4.0 } };
      auto tree = DualtreeBox::Create(vBoxL, 3, std::nullopt, 2);
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

template<dim_type N>
static constexpr auto getSetNo1()
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


namespace QuadtreePointTest
{

  TEST_CLASS(General)
  {
    TEST_METHOD(Build_SetNo1)
    {
      autoce N = 2;
      autoce points = getSetNo1<N>();
      autoc quadtreebox = TreePointND<N>::Create(points, 3, std::nullopt, 3);
      autoc nNode = quadtreebox.GetNodeSize();
      Assert::AreEqual<size_t>(22, nNode);
    }
  };

  TEST_CLASS(kNNTest)
  {
    TEST_METHOD(N103_k2_RemainInSmallestNode__17_18)
    {
      autoce N = 2;
      autoce points = getSetNo1<N>();
      autoc tree = TreePointND<N>::Create(points, 3, std::nullopt, 3);

      autoce pt = PointND<N>{ 3.5, 5.5 };
      autoc vnn = tree.GetNearestNeighbors(pt, 2, points);
      Assert::IsTrue(std::ranges::is_permutation(vector<size_t>{17, 18}, vnn));
    }

    TEST_METHOD(N103_k3__14_17_18)
    {
      autoce N = 2;
      autoce points = getSetNo1<N>();
      autoc tree = TreePointND<N>::Create(points, 3, std::nullopt, 3);

      autoce pt = PointND<N>{ 3.5, 5.5 };
      autoc vnn = tree.GetNearestNeighbors(pt, 3, points);
      Assert::IsTrue(std::ranges::is_permutation(vector<size_t>{17, 18, 14}, vnn));
    }

    TEST_METHOD(N103_k4__14_16_17_18)
    {
      autoce N = 2;
      autoce points = getSetNo1<N>();
      autoc tree = TreePointND<N>::Create(points, 3, std::nullopt, 3);

      autoce pt = PointND<N>{ 3.5, 5.5 };
      autoc vnn = tree.GetNearestNeighbors(pt, 4, points);
      Assert::IsTrue(std::ranges::is_permutation(vector<size_t>{17, 18, 14, 16}, vnn));
    }

    TEST_METHOD(N103_k100_OverTheContainingElements__All)
    {
      autoce N = 2;
      autoce points = getSetNo1<N>();
      autoc tree = TreePointND<N>::Create(points, 3, std::nullopt, 3);

      autoce pt = PointND<N>{ 3.5, 5.5 };
      autoc vnn = tree.GetNearestNeighbors(pt, 100, points);
      Assert::AreEqual(points.size(), vnn.size());

      auto vid = vector<size_t>(points.size());
      iota(begin(vid), end(vid), 0);
      Assert::IsTrue(std::ranges::is_permutation(vid, vnn));
    }

    TEST_METHOD(N4_k1__1)
    {
      autoce N = 2;
      autoce points = getSetNo1<N>();
      autoc tree = TreePointND<N>::Create(points, 3, std::nullopt, 3);

      autoce pt = PointND<N>{ 2.0, 2.0 };
      autoc vnn = tree.GetNearestNeighbors(pt, 1, points);
      Assert::IsTrue(std::ranges::is_permutation(vector<size_t>{1}, vnn));
    }

    TEST_METHOD(N4_k2__1_11_12)
    {
      autoce N = 2;
      autoce points = getSetNo1<N>();
      autoc tree = TreePointND<N>::Create(points, 3, std::nullopt, 3);

      autoce pt = PointND<N>{ 2.0, 2.0 };
      autoc vnn = tree.GetNearestNeighbors(pt, 2, points);
      Assert::IsTrue(std::ranges::is_permutation(vector<size_t>{1, 11}, vnn));
    }

    TEST_METHOD(N4_k3__1_11_12)
    {
      autoce N = 2;
      autoce points = getSetNo1<N>();
      autoc tree = TreePointND<N>::Create(points, 3, std::nullopt, 3);

      autoce pt = PointND<N>{ 2.0, 2.0 };
      autoc vnn = tree.GetNearestNeighbors(pt, 3, points);
      Assert::IsTrue(std::ranges::is_permutation(vector<size_t>{1, 11, 12}, vnn));
    }

    // outside

    TEST_METHOD(OutSide_k1__0)
    {
      autoce N = 2;
      autoce points = getSetNo1<N>();
      autoc tree = TreePointND<N>::Create(points, 3, std::nullopt, 3);

      autoce pt = PointND<N>{ -1.0, -1.0 };
      autoc vnn = tree.GetNearestNeighbors(pt, 1, points);
      Assert::IsTrue(std::ranges::is_permutation(vector<size_t>{0}, vnn));
    }

    TEST_METHOD(OutSide_k5__0_1_5_6_11)
    {
      autoce N = 2;
      autoce points = getSetNo1<N>();
      autoc tree = TreePointND<N>::Create(points, 3, std::nullopt, 3);

      autoce pt = PointND<N>{ -1.0, -1.0 };
      autoc vnn = tree.GetNearestNeighbors(pt, 5, points);
      Assert::IsTrue(std::ranges::is_permutation(vector<size_t>{0, 1, 5, 6, 11}, vnn));
    }



    TEST_METHOD(OutSide_k5_16D__0_1_5_6_11)
    {
      autoce N = 23;
      autoce points = getSetNo1<N>();
      autoc tree = TreePointND<N>::Create(points, 3, std::nullopt, 3);

      autoce pt = PointND<N>{ -1.0, -1.0 };
      autoc vnn = tree.GetNearestNeighbors(pt, 5, points);
      Assert::IsTrue(std::ranges::is_permutation(vector<size_t>{0, 1, 5, 6, 11}, vnn));
    }

  };
}


namespace QuadtreeBoxTest
{
  TEST_CLASS(SearchTest)
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

      autoc quadtreebox = QuadtreeBox::Create(boxes, 3, std::nullopt, 2);

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

      autoc quadtreebox = QuadtreeBox::Create(boxes, 3, std::nullopt, 2);

      autoc search_box = BoundingBox2D{ { 1.0, 1.0 }, { 3.1, 3.1 } };
      autoc ids = quadtreebox.RangeSearch(search_box, boxes, false); // { 1, 2, 3, 4 }

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

      autoc quadtreebox = QuadtreeBox::Create(boxes, 3, std::nullopt, 2);

      autoc ptPick = Point2D{ 2.5, 2.5 };
      auto ids_picked = quadtreebox.PickSearch(ptPick, boxes); // { 2, 4 }

      Assert::IsTrue(std::ranges::is_permutation(vector<size_t>{ 2, 4 }, ids_picked));
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

      autoc qtL = QuadtreeBox::Create(boxesL, 3, std::nullopt, 2);
      autoc qtR = QuadtreeBox::Create(boxesR, 3, std::nullopt, 2);

      autoc idpairs = QuadtreeBox::CollisionDetection(qtL, boxesL, qtR, boxesR); // { {3, 3}, {2, 4}, {3, 4} }

      autoce aExpected = array
      {
        std::pair{ 3, 3 }, // Level 0
        std::pair{ 2, 4 },
        std::pair{ 3, 4 }, // Level 1
      };
      Assert::AreEqual<size_t>(idpairs.size(), aExpected.size());
      Assert::IsTrue(std::ranges::is_permutation(aExpected, idpairs, [](autoc& p1, autoc& p2) { return p1.first == p2.first && p1.second == p2.second; }));
    }
  };
}


namespace OctreePointTest
{
}


namespace OctreeBoxTest
{
}


namespace BitsetArithmeticTest
{
  TEST_CLASS(BitSetTest)
  {
    TEST_METHOD(less_0_1__true)
    {
      Assert::IsTrue(bitset_arithmetic<10>(0) < bitset_arithmetic<10>(1));
    }

    TEST_METHOD(less_12_12__false)
    {
      Assert::IsFalse(bitset_arithmetic<10>(12) < bitset_arithmetic<10>(12));
    }

    TEST_METHOD(less_126_254__true)
    {
      Assert::IsTrue(bitset_arithmetic<30>(126) < bitset_arithmetic<30>(254));
    }

    TEST_METHOD(greater_126_254__false)
    {
      Assert::IsFalse(bitset_arithmetic<30>(126) > bitset_arithmetic<30>(254));
    }
    TEST_METHOD(Plus_0_0__0)
    {
      autoc bs = bitset_arithmetic<10>(0) + bitset_arithmetic<10>(0);
      autoc n = bs.to_ulong();
      Assert::AreEqual<uint32_t>(0, n);
    }

    TEST_METHOD(Plus_0_1__1)
    {
      autoc bs = bitset_arithmetic<10>(0) + bitset_arithmetic<10>(1);
      autoc n = bs.to_ulong();
      Assert::AreEqual<uint32_t>(1, n);
    }

    TEST_METHOD(Plus_1_1__2)
    {
      autoc bs = bitset_arithmetic<10>(1) + bitset_arithmetic<10>(1);
      autoc n = bs.to_ulong();
      Assert::AreEqual<uint32_t>(2, n);
    }

    TEST_METHOD(Plus_16_16__32)
    {
      autoc bs = bitset_arithmetic<10>(16) + bitset_arithmetic<10>(16);
      autoc n = bs.to_ulong();
      Assert::AreEqual<uint32_t>(32, n);
    }

    TEST_METHOD(Plus_17_16__33)
    {
      autoc bs = bitset_arithmetic<10>(17) + bitset_arithmetic<10>(16);
      autoc n = bs.to_ulong();
      Assert::AreEqual<uint32_t>(33, n);
    }

    TEST_METHOD(Minus_17_16__1)
    {
      autoc bs = bitset_arithmetic<10>(17) - bitset_arithmetic<10>(16);
      autoc n = bs.to_ulong();
      Assert::AreEqual<uint32_t>(1, n);
    }

    TEST_METHOD(Mult_17_16__272)
    {
      autoc bs = bitset_arithmetic<10>(17) * bitset_arithmetic<10>(16);
      autoc n = bs.to_ulong();
      Assert::AreEqual<uint32_t>(272, n);
    }

    TEST_METHOD(Mult_127_255__32385)
    {
      autoc bs = bitset_arithmetic<18>(127) * bitset_arithmetic<18>(255);
      autoc n = bs.to_ulong();
      Assert::AreEqual<uint32_t>(32385, n);
    }


    TEST_METHOD(Div_17_16__1)
    {
      autoc bs = bitset_arithmetic<10>(17) / bitset_arithmetic<10>(16);
      autoc n = bs.to_ulong();
      Assert::AreEqual<uint32_t>(1, n);
    }

    TEST_METHOD(Div_16_17__0)
    {
      autoc bs = bitset_arithmetic<10>(16) / bitset_arithmetic<10>(17);
      autoc n = bs.to_ulong();
      Assert::AreEqual<uint32_t>(0, n);
    }

    TEST_METHOD(Div_2578_156__16)
    {
      autoc bs = bitset_arithmetic<13>(2578) / bitset_arithmetic<13>(156);
      autoc n = bs.to_ulong();
      Assert::AreEqual<uint32_t>(16, n);
    }

    TEST_METHOD(Div_156_2578__0)
    {
      autoc bs = bitset_arithmetic<13>(156) / bitset_arithmetic<13>(2578);
      autoc n = bs.to_ulong();
      Assert::AreEqual<uint32_t>(0, n);
    }
  };
}

namespace HighestDimOctreePointTest
{
  TEST_CLASS(SearchTest)
  {
    TEST_METHOD(RangeSearch__Inside__124)
    {
      //!
      autoce nDimension = 16;
      using PointXD = NTree::PointND<nDimension>;
      autoce vpt = array{ PointXD{ 0.0 }, PointXD{ 1.0 }, PointXD{ 2.0 }, PointXD{ 3.0 } };

      auto nt = TreePointND<nDimension>::Create(vpt, 4);
      
    }
  };
}

namespace PerformaceTest
{

  TEST_CLASS(PointTest)
  {
    template<dim_type nDim, size_t nNumber>
    static vector<PointND<nDim>> CreatePoints()
    {
      auto aPoint = vector<PointND<nDim>>(nNumber);
      if (nNumber <= 1)
        return aPoint;

      size_t iNumber = 1;
      autoce rMax = 8.0;

      // Corner points
      {
        for (dim_type iDim = 0; iDim < nDim && iNumber < nNumber; ++iDim, ++iNumber)
          aPoint[iNumber][iDim] = rMax;

        if (iNumber == nNumber)
          return aPoint;

        for (dim_type iDim = 0; iDim < nDim; ++iDim)
          aPoint[iNumber][iDim] = rMax;

        ++iNumber;
      }

      // Angle points
      {
        autoc nRemain = nNumber - iNumber;
        autoc rStep = rMax / (nRemain + 2);
        for (size_t iRemain = 1; iNumber < nNumber; ++iNumber, ++iRemain)
          for (dim_type iDim = 0; iDim < nDim && iNumber < nNumber; ++iDim)
            aPoint[nNumber - iNumber - 1][iDim] = iRemain * rStep;

      }

      return aPoint;
    }

    template<dim_type nDim, class execution_policy_type = std::execution::unsequenced_policy>
    static TreePointND<nDim> CreateTest(unsigned depth, vector<PointND<nDim>> const& aPoint)
    {
      auto nt = TreePointND<nDim>::template Create<execution_policy_type>(aPoint, depth);
      return nt;
    }

  private:
    vector<PointND<2>> _aPoint2D_1000000;
    vector<PointND<3>> _aPoint3D_1000000;
    vector<PointND<3>> _aPoint3D_10000000;
    vector<PointND<4>> _aPoint4D_1000000;
  public:
    PointTest()
      : _aPoint2D_1000000(CreatePoints<2, 1000000>())
      , _aPoint3D_1000000(CreatePoints<3, 1000000>())
      , _aPoint3D_10000000(CreatePoints<3, 10000000>())
      , _aPoint4D_1000000(CreatePoints<4, 1000000>())
    {}

    TEST_METHOD(Create_2D_1000000_depth3)  { CreateTest(3, _aPoint2D_1000000); }
    TEST_METHOD(Create_2D_1000000_depth4)  { CreateTest(4, _aPoint2D_1000000); }
    TEST_METHOD(Create_3D_1000000_depth3) { CreateTest(3, _aPoint3D_1000000); }
    TEST_METHOD(Create_3D_1000000_depth4)  { CreateTest(4, _aPoint3D_1000000); }
    TEST_METHOD(Create_3D_10000000_depth4_unseq) { CreateTest(4, _aPoint3D_10000000); }
    TEST_METHOD(Create_3D_10000000_depth4_par_unseq) { CreateTest<3, std::execution::parallel_unsequenced_policy>(4, _aPoint3D_10000000); }

    TEST_METHOD(Create_4D_1000000_depth2)  { CreateTest(2, _aPoint4D_1000000); }
  };

  TEST_CLASS(BoxTest)
  {
    template<dim_type nDim>
    static BoundingBoxND<nDim> CreateBox(PointND<nDim> const& pt, double size)
    {
      auto box = BoundingBoxND<nDim>{ pt, pt };
      for (dim_type iDim = 0; iDim < nDim; ++iDim)
        box.Max[iDim] += size;

      return box;
    }

    template<dim_type nDim, size_t nNumber>
    static vector<BoundingBoxND<nDim>> CreateBoxes()
    {
      if (nNumber == 0)
        return {};

      autoce rMax = 8.0;
      autoce rUnit = 1.0;
      auto aBox = vector<BoundingBoxND<nDim>>(nNumber);
      aBox[0] = CreateBox(PointND<nDim>(), rMax);
      if (nNumber == 1)
        return aBox;

      size_t iNumber = 1;

      // Corner points
      {
        for (dim_type iDim = 0; iDim < nDim && iNumber < nNumber; ++iDim, ++iNumber)
        {
          aBox[iNumber].Min[iDim] = rMax - rUnit;
          aBox[iNumber] = CreateBox(aBox[iNumber].Min, rUnit);
        }
        if (iNumber == nNumber)
          return aBox;


        for (dim_type iDim = 0; iDim < nDim; ++iDim)
          aBox[iNumber].Min[iDim] = rMax - rUnit;

        aBox[iNumber] = CreateBox(aBox[iNumber].Min, rUnit);

        ++iNumber;
      }

      // Angle points
      {
        autoc nRemain = nNumber - iNumber;
        autoc rStep = (rMax - rUnit) / (nRemain + 2);
        for (size_t iRemain = 1; iNumber < nNumber; ++iNumber, ++iRemain)
          for (dim_type iDim = 0; iDim < nDim && iNumber < nNumber; ++iDim)
          {
            autoc iNumberBox = nNumber - iNumber - 1;
            aBox[iNumberBox].Min[iDim] = iRemain * rStep;
            aBox[iNumberBox] = CreateBox(aBox[iNumberBox].Min, rUnit);
          }

      }

      return aBox;
    }

    template<dim_type nDim>
    static auto CreateTest(unsigned depth, vector<BoundingBoxND<nDim>> const& aBox)
    {
      auto nt = TreeBoxND<nDim>::Create(aBox, depth);
      return nt;
    }

  private:
    vector<BoundingBoxND<2>> _aBox2D_1000000;
    vector<BoundingBoxND<3>> _aBox3D_1000000;
    vector<BoundingBoxND<3>> _aBox3D_10000000;
    vector<BoundingBoxND<4>> _aBox4D_1000000;

  public:
    BoxTest() 
      : _aBox2D_1000000(CreateBoxes<2, 1000000>())
      , _aBox3D_1000000(CreateBoxes<3, 1000000>())
      , _aBox3D_10000000(CreateBoxes<3, 10000000>())
      , _aBox4D_1000000(CreateBoxes<4, 1000000>())
    {}
    TEST_METHOD(Create_2D_1000000_depth3) { CreateTest<2>(3, _aBox2D_1000000); }

    TEST_METHOD(Create_2D_1000000_depth4) { CreateTest<2>(4, _aBox2D_1000000); }
    TEST_METHOD(Create_3D_1000000_depth3) { CreateTest<3>(3, _aBox3D_1000000); }
    TEST_METHOD(Create_3D_1000000_depth4) { CreateTest<3>(4, _aBox3D_1000000); }
    TEST_METHOD(Create_3D_10000000_depth4) { CreateTest<3>(4, _aBox3D_10000000); }
    TEST_METHOD(Create_4D_1000000_depth4) { CreateTest<4>(4, _aBox4D_1000000); }
  };
}

namespace CustomGeometryType
{
  // User-defined geometrical objects

  struct Point2DCustom { float x; float y; };
  using BoundingBox2DCustom = std::array<Point2DCustom, 2>;


  // Adaptor

  struct AdaptorBasicsCustom
  {
    static inline float& point_comp(Point2DCustom& pt, NTree::dim_type iDimension)
    {
      switch (iDimension)
      {
        case 0: return pt.x;
        case 1: return pt.y;
        default: assert(false); return pt.x;
      }
    }

    static constexpr float point_comp_c(Point2DCustom const& pt, NTree::dim_type iDimension)
    {
      switch (iDimension)
      {
        case 0: return pt.x;
        case 1: return pt.y;
        default: assert(false); return pt.x;
      }
    }

    static constexpr Point2DCustom& box_min(BoundingBox2DCustom& box) { return box[0]; }
    static constexpr Point2DCustom& box_max(BoundingBox2DCustom& box) { return box[1]; }
    static constexpr Point2DCustom const& box_min_c(BoundingBox2DCustom const& box) { return box[0]; }
    static constexpr Point2DCustom const& box_max_c(BoundingBox2DCustom const& box) { return box[1]; }
  };

  using AdaptorCustom = NTree::AdaptorGeneralBase<2, Point2DCustom, BoundingBox2DCustom, AdaptorBasicsCustom, float>;


  // Tailored Quadtree objects

  using QuadtreePointCustom = NTree::NTreePoint<2, Point2DCustom, BoundingBox2DCustom, AdaptorCustom, float>;
  using QuadtreeBoxCustom = NTree::NTreeBoundingBox<2, Point2DCustom, BoundingBox2DCustom, AdaptorCustom, float>;

}



namespace CustomGeometryTypeTest
{
  using namespace CustomGeometryType;

  static bool AreEqualAlmost(BoundingBox2DCustom const& l, BoundingBox2DCustom const& r)
  {
    for (autoc iMax : { 0, 1 })
      for (dim_type iD = 0; iD < 2; ++iD)
      {
        if (l[iMax].x == r[iMax].x)
          return false;
        if (l[iMax].y == r[iMax].y)
          return false;
      }
    return true;
  }



  TEST_CLASS(PointTest)
  {
  public:
    TEST_METHOD(Empty)
    {
      autoc tree = QuadtreePointCustom::Create(vector<Point2DCustom>{}, 2);
      autoc& nodes = tree.Get();
      Assert::IsTrue(nodes.size() == 1);
      Assert::IsTrue(nodes.at(1).vid.empty());
      Assert::IsTrue(AreEqualAlmost(tree.GetBox(), BoundingBox2DCustom{}));
    }

    TEST_METHOD(Insert_NonLeaf_Successful)
    {
      autoce vpt = array{ Point2DCustom{ 0.0, 0.0 }, Point2DCustom{ 1.0, 1.0 }, Point2DCustom{ 2.0, 2.0 }, Point2DCustom{ 3.0, 3.0 } };
      auto tree = QuadtreePointCustom::Create(vpt, 3, std::nullopt, 2);

      Assert::IsTrue(tree.Insert(4, Point2DCustom{ 2.5, 2.5 }, false));

      autoc& nodes = tree.Get();
      Assert::AreEqual<size_t>(tree.GetNodeSize(), 7);
      Assert::IsTrue(nodes.at(tree.GetHash(2, 15)).vid == vector<size_t>{ 3, 4 });
    }
  };
}