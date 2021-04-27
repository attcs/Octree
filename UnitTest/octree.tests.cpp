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


namespace GeneralTest
{
  TEST_CLASS(MortonTest)
  {
  public:
    TEST_METHOD(M1D_0_0)
    {
      autoce arr = array<location_id_type, 1>{ 0 };
      Assert::AreEqual(Morton(arr), location_id_type(0));
    }

    TEST_METHOD(M1D_4_4)
    {
      autoce arr = array<location_id_type, 1>{ 4 };
      Assert::AreEqual(Morton(arr), location_id_type(4));
    }


    TEST_METHOD(M2D_00_0)
    {
      autoce arr = array<location_id_type, 2>{ 0, 0 };
      Assert::AreEqual(Morton(arr), location_id_type(0));
    }

    TEST_METHOD(M2D_20_4)
    {
      autoce arr = array<location_id_type, 2>{ 2, 0 };
      Assert::AreEqual(Morton(arr), location_id_type(4));
    }

    TEST_METHOD(M2D_02_8)
    {
      autoce arr = array<location_id_type, 2>{ 0, 2 };
      Assert::AreEqual(Morton(arr), location_id_type(8));
    }

    TEST_METHOD(M2D_22_12)
    {
      autoce arr = array<location_id_type, 2>{ 2, 2 };
      Assert::AreEqual(Morton(arr), location_id_type(12));
    }

    TEST_METHOD(M2D_13_11)
    {
      autoce arr = array<location_id_type, 2>{ 1, 3 };
      Assert::AreEqual(Morton(arr), location_id_type(11));
    }

    TEST_METHOD(M3D_000_0)
    {
      autoce arr = array<location_id_type, 3>{ 0, 0, 0 };
      Assert::AreEqual(Morton(arr), location_id_type(0));
    }

    TEST_METHOD(M3D_100_1)
    {
      autoce arr = array<location_id_type, 3>{ 1, 0, 0 };
      Assert::AreEqual(Morton(arr), location_id_type(1));
    }

    TEST_METHOD(M3D_001_4)
    {
      autoce arr = array<location_id_type, 3>{ 0, 0, 1 };
      Assert::AreEqual(Morton(arr), location_id_type(4));
    }

    TEST_METHOD(M3D_111_7)
    {
      autoce arr = array<location_id_type, 3>{ 1, 1, 1 };
      Assert::AreEqual(Morton(arr), location_id_type(7));
    }

    TEST_METHOD(M4D_1111_15)
    {
      autoce arr = array<location_id_type, 4>{ 1, 1, 1, 1 };
      Assert::AreEqual(Morton(arr), location_id_type(15));
    }

    TEST_METHOD(M4D_2111_30)
    {
      autoce arr = array<location_id_type, 4>{ 2, 1, 1, 1 };
      Assert::AreEqual(Morton(arr), location_id_type(30));
    }
  };

  using tree_id_type = DualtreePoint::tree_id_type;
  TEST_CLASS(NTreeLinearTest)
  {
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
      Assert::AreEqual(DualtreePoint::GetHash(0,0), tree_id_type(1));
    }

    TEST_METHOD(GetHash__11_3)
    {
      Assert::AreEqual(DualtreePoint::GetHash(1, 1), tree_id_type(3));
    }
    TEST_METHOD(GetHash__22_4)
    {
      Assert::AreEqual(DualtreePoint::GetHash(2, 2), tree_id_type(6));
    }

    TEST_METHOD(GetHash__37_15)
    {
      Assert::AreEqual(DualtreePoint::GetHash(3, 7), tree_id_type(15));
    }

    TEST_METHOD(GetDepth__37_15__3)
    {
      autoc lc = DualtreePoint::GetDepth(DualtreePoint::GetHash(3, 7));
      Assert::AreEqual(lc, bucket_id_type(3));
    }

    TEST_METHOD(RemoveSentinelBit__37_15__7)
    {
      autoc lc = DualtreePoint::RemoveSentinelBit(DualtreePoint::GetHash(3, 7));
      Assert::AreEqual(lc, tree_id_type(7));
    }

    TEST_METHOD(Init)
    {
      auto tree = DualtreePoint{};
      autoc bb = BoundingBox1D{ -1, +1 };
      tree.Init(bb, 3, 10);
      Assert::IsTrue(AreEqualAlmost(bb, tree.GetBox()));
      Assert::AreEqual<size_t>(tree.GetSize(), 1);
      Assert::AreEqual<size_t>(tree.GetDepthMax(), 3);
      Assert::AreEqual<size_t>(tree.GetResolutionMax(), 8);
    }
    
    TEST_METHOD(VisitNodes_)
    {
      //!
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

    TEST_METHOD(Update)
    {

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

      Assert::AreEqual<size_t>(tree.GetSize(), 7);
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
    TEST_METHOD(FindSmallestNode_)
    {
      //!
    }

    TEST_METHOD(RangeSearch_)
    {
      //!
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
      Assert::AreEqual<size_t>(tree.GetSize(), 7);
      Assert::IsTrue(nodes.at(7).vid == vector<size_t>{ 3, 4 });
    }

    TEST_METHOD(Insert__Leaf__Successful)
    {
      autoce vpt = array{ Point1D{ 0.0 }, Point1D{ 1.0 }, Point1D{ 2.0 }, Point1D{ 3.0 } };
      auto tree = DualtreePoint::Create(vpt, 3, std::nullopt, 2);

      Assert::IsTrue(tree.Insert(4, Point1D{ 2.5 }, true));

      autoc& nodes = tree.Get();
      Assert::AreEqual<size_t>(tree.GetSize(), 8);
      Assert::IsTrue(nodes.at(14).vid == vector<size_t>{ 4 });
    }

    TEST_METHOD(Insert__OutOfSpace__ReturnsFalse)
    {
      autoce vpt = array{ Point1D{ 0.0 }, Point1D{ 1.0 }, Point1D{ 2.0 }, Point1D{ 3.0 } };
      auto tree = DualtreePoint::Create(vpt, 3, std::nullopt, 2);
      Assert::IsFalse(tree.Insert(4, Point1D{ 4.0 }, true));

      autoc& nodes = tree.Get();
      Assert::AreEqual<size_t>(tree.GetSize(), 7);
      Assert::IsTrue(nodes.at(7).vid == vector<size_t>{ 3 });
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
    TEST_METHOD(Insert_NonLeaf_Successful)
    {
      //!
    }

    TEST_METHOD(Insert_Leaf_Successful)
    {
      //!
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
}


namespace OctreePointTest
{
}


namespace OctreeBoxTest
{
}


namespace CustomGeometryType
{
  struct Point2DCustom
  {
    float x;
    float y;
  };

  using BoundingBox2DCustom = std::array<Point2DCustom, 2>;

  struct AdaptorBasicsCustom
  {
    static constexpr float& point_comp(Point2DCustom& pt, dim_type iDimension) {
      switch (iDimension)
      {
        case 0: return pt.x;
        case 1: return pt.y;
        default: assert(false); return pt.x;
      }
    }

    static constexpr float const& point_comp_c(Point2DCustom const& pt, dim_type iDimension) {
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

  using QuadtreePointCustom = NTree::NTreePoint<2, Point2DCustom, BoundingBox2DCustom, NTree::AdaptorGeneralBase<2, Point2DCustom, BoundingBox2DCustom, AdaptorBasicsCustom, float>, float>;
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
      Assert::AreEqual<size_t>(tree.GetSize(), 7);
      Assert::IsTrue(nodes.at(tree.GetHash(2, 15)).vid == vector<size_t>{ 3, 4 });
    }

  };
}