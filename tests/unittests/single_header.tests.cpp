#include "CppUnitTest.h"
#include "../../single_include/orthotree.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace OrthoTree;

namespace SingleHeaderTest
{
  TEST_CLASS(SingleHeaderTests)
  {
  public:
    TEST_METHOD(TestOctreeFromSingleHeader)
    {
      auto constexpr points = std::array{
        Point3D{ 0.0, 0.0, 0.0 },
        Point3D{ 1.0, 1.0, 1.0 },
        Point3D{ 2.0, 2.0, 2.0 }
      };
      
      auto const octree = OctreePointM(points, 3);
      auto const searchBox = BoundingBox3D{ { 0.5, 0.5, 0.5 }, { 2.5, 2.5, 2.5 } };
      auto pointIDs = octree.RangeSearch(searchBox);
      
      Assert::AreEqual(size_t(2), pointIDs.size());
      Assert::AreEqual(OrthoTree::index_t(1), pointIDs[0]);
      Assert::AreEqual(OrthoTree::index_t(2), pointIDs[1]);
    }

    TEST_METHOD(TestBVHFromSingleHeader)
    {
      auto boxes = std::vector{
        BoundingBox3D{ { 0.0, 0.0, 0.0 }, { 1.0, 1.0, 1.0 } },
        BoundingBox3D{ { 2.0, 2.0, 2.0 }, { 3.0, 3.0, 3.0 } }
      };
      
      auto bvh = StaticBVHBox3DM(boxes, 3);
      auto searchBox = BoundingBox3D{ { 0.5, 0.5, 0.5 }, { 2.5, 2.5, 2.5 } };
      auto overlaps = bvh.RangeSearch(searchBox, RangeSearchMode::Overlap);
      
      Assert::AreEqual(size_t(2), overlaps.size());
    }
  };
}
