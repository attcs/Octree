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
  static bool AreEqualAlmost(BoundingBox<nDimension, geometry_type> const& l, BoundingBox<nDimension, geometry_type> const& r)
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
    }
  }
}
namespace GeneralTest
{
  TEST_CLASS(MortonTest)
  {
    //static std::wstring ToString(uint16_t const& q) { RETURN_WIDE_STRING(q); }

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
        , [](autoc key, autoc& node) { return true; }
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
        , [](autoc key, autoc& node) { return true; }
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
        , [](autoc key, autoc& node) { return true; }
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
        , [](autoc key, autoc& node) { return true; }
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
      tree.UpdateIndexes({ { 2, std::numeric_limits<NTree::entity_id_type>::max() } });

      auto ids = vector<size_t>();
      tree.VisitNodes(tree.GetRootKey()
        , [](autoc key, autoc& node) { return true; }
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
        , [](autoc key, autoc& node) { return true; }
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
        , [](autoc key, autoc& node) { return true; }
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
          { 1, std::numeric_limits<NTree::entity_id_type>::max() },
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

      autoc ret = DualtreeBox::CollisionDetection(treeL, vBoxL, treeR, vBoxR);

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
      Assert::AreEqual<size_t>(ret.size(), aExpected.size());
      Assert::IsTrue(std::ranges::is_permutation(aExpected, ret, [](autoc& p1, autoc& p2) { return p1.first == p2.first && p1.second == p2.second; }));
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


namespace QuadtreePointTest
{
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
      autoce nDimension = 27;
      using PointXD = NTree::Point<nDimension>;
      autoce vpt = array{ PointXD{ 0.0 }, PointXD{ 1.0 }, PointXD{ 2.0 }, PointXD{ 3.0 } };

      auto nt = NTreePointXD<nDimension>::Create(vpt, 4);
      
    }
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