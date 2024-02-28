#include "pch.h"

using namespace OrthoTree;

using std::array;
using std::vector;

namespace Example
{
  TEST_CLASS(ExampleTests)
  {
    TEST_METHOD(Example1)
    {
      auto constexpr points = array{ Point3D{0,0,0}, Point3D{1,1,1}, Point3D{2,2,2} };
      auto const octree = OctreePoint(points, 3 /*max depth*/);

      auto const searchBox = BoundingBox3D{ {0.5, 0.5, 0.5}, {2.5, 2.5, 2.5} };
      auto pointIDsByRange = octree.RangeSearch(searchBox, points); //: { 1, 2 }
      auto pointIDsByKNN = octree.GetNearestNeighbors(Point3D{ 1.1,1.1,1.1 }
        , 2 // neighborNo
        , points
      ); //: { 1, 2 }

      Assert::IsTrue(std::ranges::is_permutation(vector<std::size_t>{1, 2}, pointIDsByRange));
      Assert::IsTrue(std::ranges::is_permutation(vector<std::size_t>{1, 2}, pointIDsByKNN));
    }


    TEST_METHOD(Example2)
    {
      auto boxes = vector
      {
        BoundingBox2D{ { 0.0, 0.0 }, { 1.0, 1.0 } },
        BoundingBox2D{ { 1.0, 1.0 }, { 2.0, 2.0 } },
        BoundingBox2D{ { 2.0, 2.0 }, { 3.0, 3.0 } },
        BoundingBox2D{ { 3.0, 3.0 }, { 4.0, 4.0 } },
        BoundingBox2D{ { 1.2, 1.2 }, { 2.8, 2.8 } }
      };

      auto qt = QuadtreeBox(boxes
        , 3            // max depth
        , std::nullopt // user-provided bounding Box for all
        , 2            // max element in a node 
      );

      auto collidingIDPairs = qt.CollisionDetection(boxes); //: { {1,4}, {2,4} }

      auto searchBox = BoundingBox2D{ { 1.0, 1.0 }, { 3.1, 3.1 } };

      // Boxes within the range
      auto insideBoxIDs = qt.RangeSearch(searchBox, boxes); //: { 1, 2, 4 }

      // Overlapping Boxes with the range
      constexpr bool shouldFullyContain = false;
      auto overlappingBoxIDs = qt.RangeSearch<shouldFullyContain>(searchBox, boxes); //: { 1, 2, 3, 4 }

      // Picked boxes
      auto pickPoint = Point2D{ 2.5, 2.5 };
      auto pickedBoxIDs = qt.PickSearch(pickPoint, boxes); //: { 2, 4 }


      Assert::IsTrue(std::ranges::is_permutation(vector<std::pair<std::size_t, std::size_t>>{ {1, 4}, { 2, 4 } }, collidingIDPairs));
      Assert::IsTrue(std::ranges::is_permutation(vector<std::size_t>{1, 2, 4}, insideBoxIDs));
      Assert::IsTrue(std::ranges::is_permutation(vector<std::size_t>{1, 2, 3, 4}, overlappingBoxIDs));
      Assert::IsTrue(std::ranges::is_permutation(vector<std::size_t>{2, 4}, pickedBoxIDs));
    }


    TEST_METHOD(Example3)
    {
      auto boxes = vector{ BoundingBox3D{ { 0.0, 0.0, 0.0 }, { 1.0, 1.0, 1.0 } } /* and more... */ };
      auto octreebox = OctreeBox{};
      OctreeBox::Create<std::execution::parallel_unsequenced_policy>(octreebox, boxes, 3);
      // or
      // TreeBoxND<3>::template Create<std::execution::parallel_unsequenced_policy>(boxes, 3);
    }


    TEST_METHOD(Example1_Container)
    {
      auto constexpr points = array{ Point3D{0,0,0}, Point3D{1,1,1}, Point3D{2,2,2} };
      auto const octree = OctreePointC(points, 3 /*max depth*/);

      auto const searchBox = BoundingBox3D{ {0.5, 0.5, 0.5}, {2.5, 2.5, 2.5} };
      auto const pointIDs = octree.RangeSearch(searchBox); //: { 1, 2 }

      auto neighborNo = 2;
      auto pointIDsByKNN = octree.GetNearestNeighbors(Point3D{ 1.1, 1.1, 1.1 }
        , neighborNo
      ); //: { 1, 2 }

      Assert::IsTrue(std::ranges::is_permutation(vector<std::size_t>{1, 2}, pointIDs));
      Assert::IsTrue(std::ranges::is_permutation(vector<std::size_t>{1, 2}, pointIDsByKNN));
    }


    TEST_METHOD(Example2_Container)
    {
      auto boxes = vector
      {
        BoundingBox2D{ { 0.0, 0.0 }, { 1.0, 1.0 } },
        BoundingBox2D{ { 1.0, 1.0 }, { 2.0, 2.0 } },
        BoundingBox2D{ { 2.0, 2.0 }, { 3.0, 3.0 } },
        BoundingBox2D{ { 3.0, 3.0 }, { 4.0, 4.0 } },
        BoundingBox2D{ { 1.2, 1.2 }, { 2.8, 2.8 } }
      };

      auto quadtree = QuadtreeBoxC(boxes
        , 3            // max depth
        , std::nullopt // user-provided bounding Box for all
        , 2            // max element in a node 
        , false        // parallel calculation option
      );

      auto collidingIDPairs = quadtree.CollisionDetection(); //: { {1,4}, {2,4} }

      auto searchBox = BoundingBox2D{ { 1.0, 1.0 }, { 3.1, 3.1 } };

      // Boxes within the range
      auto insideBoxIDs = quadtree.RangeSearch(searchBox); //: { 1, 2, 4 }

      // Overlapping Boxes with the range
      constexpr bool shouldFullyContain = false; // overlap is enough
      auto overlappingBoxIDs = quadtree.RangeSearch<shouldFullyContain>(searchBox); //: { 1, 2, 3, 4 }

      // Picked boxes
      auto pickPoint = Point2D{ 2.5, 2.5 };
      auto pickedIDs = quadtree.PickSearch(pickPoint); //: { 2, 4 }

      Assert::IsTrue(std::ranges::is_permutation(vector<std::pair<std::size_t, std::size_t>>{ {1, 4}, { 2, 4 } }, collidingIDPairs));
      Assert::IsTrue(std::ranges::is_permutation(vector<std::size_t>{1, 2, 4}, insideBoxIDs));
      Assert::IsTrue(std::ranges::is_permutation(vector<std::size_t>{1, 2, 3, 4}, overlappingBoxIDs));
      Assert::IsTrue(std::ranges::is_permutation(vector<std::size_t>{2, 4}, pickedIDs));
    }


    TEST_METHOD(Example3_Container)
    {
      auto boxes = vector{ BoundingBox3D{ { 0.0, 0.0, 0.0 }, { 1.0, 1.0, 1.0 } } /* and more... */ };
      // Using ctor
      {
        auto octreeBox = OctreeBoxC(boxes, 3, std::nullopt, OctreeBox::DEFAULT_MAX_ELEMENT
          , true // Set std::execution::parallel_unsequenced_policy
        );
      }
      // Using Create
      {
        auto octreeBox = OctreeBoxC::Create<std::execution::parallel_unsequenced_policy>(boxes, 3);
      }
      // or
      // TreeBoxND<3>::template Create<std::execution::parallel_unsequenced_policy>(boxes, 3);
    }
  };
}