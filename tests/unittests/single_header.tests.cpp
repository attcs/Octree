#include <gtest/gtest.h>
#include "../../single_include/orthotree.h"


using namespace OrthoTree;

namespace SingleHeaderTest
{
  namespace SingleHeaderTests

  {
  // public:
    TEST(SingleHeaderTests, TestOctreeFromSingleHeader)
    {
      auto constexpr points = std::array{
        Point3D{ 0.0, 0.0, 0.0 },
        Point3D{ 1.0, 1.0, 1.0 },
        Point3D{ 2.0, 2.0, 2.0 }
      };
      
      auto const octree = OctreePointM(points, 3);
      auto const searchBox = BoundingBox3D{ { 0.5, 0.5, 0.5 }, { 2.5, 2.5, 2.5 } };
      auto pointIDs = octree.RangeSearch(searchBox);
      
      EXPECT_EQ(size_t(2), pointIDs.size());
      EXPECT_EQ(OrthoTree::index_t(1), pointIDs[0]);
      EXPECT_EQ(OrthoTree::index_t(2), pointIDs[1]);
    }

    TEST(SingleHeaderTests, TestBVHFromSingleHeader)
    {
      auto boxes = std::vector{
        BoundingBox3D{ { 0.0, 0.0, 0.0 }, { 1.0, 1.0, 1.0 } },
        BoundingBox3D{ { 2.0, 2.0, 2.0 }, { 3.0, 3.0, 3.0 } }
      };
      
      auto bvh = StaticBVHBox3DM(boxes, 3);
      auto searchBox = BoundingBox3D{ { 0.5, 0.5, 0.5 }, { 2.5, 2.5, 2.5 } };
      auto overlaps = bvh.RangeSearch(searchBox, RangeSearchMode::Overlap);
      
      EXPECT_EQ(size_t(2), overlaps.size());
    }
  }
}
