#include "pch.h"


namespace CustomGeometryType
{
  // User-defined geometrical objects

  struct Point2DCustom { float x; float y; };
  using BoundingBox2DCustom = std::array<Point2DCustom, 2>;


  // Adaptor

  struct AdaptorBasicsCustom
  {
    static constexpr float& point_comp(Point2DCustom& pt, NTree::dim_type iDimension)
    {
      switch (iDimension)
      {
        case 0: return pt.x;
        case 1: return pt.y;
        default: return pt.x;
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



namespace AdaptorTest
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

    TEST_METHOD(Contains__1__True)
    {
      autoce vpt = array{ Point2DCustom{ 0.0, 0.0 }, Point2DCustom{ 1.0, 1.0 }, Point2DCustom{ 2.0, 2.0 }, Point2DCustom{ 3.0, 3.0 } };
      auto tree = QuadtreePointCustom::Create(vpt, 3, std::nullopt, 2);

      Assert::IsTrue(tree.Contains(vpt[1], vpt, 0));
    }
  };
}