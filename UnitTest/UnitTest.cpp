#include "pch.h"
#include "CppUnitTest.h"
#include "../octree.h"

#define autoc auto const
#define autoce auto constexpr


using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace n_ary_tree;


namespace
{
  static bool AreEqualAlmost(double l, double r)
  {
    return abs(l - r) < numeric_limits<double>::min() * 10;
  }

  template<dim_type nDimension, typename geometry_type = double>
  static bool AreEqualAlmost(BoundingBox<nDimension, geometry_type> const& l, BoundingBox<nDimension, geometry_type> const& r)
  {
    for (dim_type iD = 0; iD < nDimension; ++iD)
    {
      if (!AreEqualAlmost(l.Min[iD], l.Min[iD]))
        return false;

      if (!AreEqualAlmost(l.Max[iD], l.Max[iD]))
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
			//ext
		}

    // ext    //!
    TEST_METHOD(NoPt1)
    {
      autoc tree = DualtreePoint::Create(vector<Point1D>{ { 1.0 } }, 2);
      autoc& nodes = tree.Get();
      Assert::IsTrue(nodes.size() == 2);
      Assert::IsTrue(nodes.at(1).vid.empty());
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

    TEST_METHOD(Insert_NonLeaf_Successful)
    {
      autoce vpt = array{ Point1D{ 0.0 }, Point1D{ 1.0 }, Point1D{ 2.0 }, Point1D{ 3.0 } };
      auto tree = DualtreePoint::Create(vpt, 3, std::nullopt, 2);

      Assert::IsTrue(tree.Insert(4, Point1D{ 2.5 }, false));

      autoc& nodes = tree.Get();
      Assert::AreEqual<size_t>(tree.GetSize(), 7);
      Assert::IsTrue(nodes.at(7).vid == vector<size_t>{ 3, 4 });
    }

    TEST_METHOD(Insert_Leaf_Successful)
    {
      autoce vpt = array{ Point1D{ 0.0 }, Point1D{ 1.0 }, Point1D{ 2.0 }, Point1D{ 3.0 } };
      auto tree = DualtreePoint::Create(vpt, 3, std::nullopt, 2);

      Assert::IsTrue(tree.Insert(4, Point1D{ 2.5 }, true));

      autoc& nodes = tree.Get();
      Assert::AreEqual<size_t>(tree.GetSize(), 8);
      Assert::IsTrue(nodes.at(14).vid == vector<size_t>{ 4 });
    }


	};
}
