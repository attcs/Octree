#include "pch.h"

using namespace OrthoTree;

namespace Example
{
  TEST_CLASS(ExampleTests)
  {
    TEST_METHOD(Example1)
    {
      auto constexpr points = array{ Point3D{0,0,0}, Point3D{1,1,1}, Point3D{2,2,2} };
      auto const octree = OctreePoint::Create(points, 3 /*max depth*/);

      auto const search_box = BoundingBox3D{ {0.5, 0.5, 0.5}, {2.5, 2.5, 2.5} };
      auto ids = octree.RangeSearch(search_box, points); // -> { 1, 2 }
      auto knn_ids = octree.GetNearestNeighbors(Point3D{ 1.1,1.1,1.1 }, 2 /*k*/, points); // -> { 1, 2 }


      Assert::IsTrue(std::ranges::is_permutation(vector<entity_id_type>{1, 2}, ids));
      Assert::IsTrue(std::ranges::is_permutation(vector<entity_id_type>{1, 2}, knn_ids));
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

      auto quadtreebox = QuadtreeBox::Create(boxes, 3
        , std::nullopt // user-provided bounding box for all
        , 2            // max element in a node 
      );

      auto search_box = BoundingBox2D{ { 1.0, 1.0 }, { 3.1, 3.1 } };

      // Boxes within the range
      auto ids_inside = quadtreebox.RangeSearch(search_box, boxes); // -> { 1, 2, 4 }

      // Overlapping Boxes with the range
      auto ids_overlaping = quadtreebox.RangeSearch<false /*overlap is enough*/>(search_box, boxes); // -> { 1, 2, 3, 4 }

      // Picked boxes
      auto ptPick = Point2D{ 2.5, 2.5 };
      auto ids_picked = quadtreebox.PickSearch(ptPick, boxes); // -> { 2, 4 }


      Assert::IsTrue(std::ranges::is_permutation(vector<entity_id_type>{1, 2, 4}, ids_inside));
      Assert::IsTrue(std::ranges::is_permutation(vector<entity_id_type>{1, 2, 3, 4}, ids_overlaping));
      Assert::IsTrue(std::ranges::is_permutation(vector<entity_id_type>{2, 4}, ids_picked));
    }


    TEST_METHOD(Example3)
    {
      auto boxes = vector{ BoundingBox3D{ { 0.0, 0.0, 0.0 }, { 1.0, 1.0, 1.0 } } /* and more... */ };
      auto octreebox = OctreeBox::Create<std::execution::parallel_unsequenced_policy>(boxes, 3);
      // or
      // TreeBoxND<3>::template Create<std::execution::parallel_unsequenced_policy>(boxes, 3);
    }

  };
}