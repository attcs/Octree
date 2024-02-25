#include "pch.h"

#include <Eigen/Geometry>
#include "../adaptor.eigen.h"
#include "../adaptor.xyz.h"

#define UNREAL_DUMMY_TYPES
#define UNREAL_DUMMY_TYPES__SUPPRESS_ASSERTS
#include "../adaptor.unreal.h"

using namespace OrthoTree;

using std::array;
using std::vector;

namespace CustomGeometryType
{
  // User-defined geometrical objects

  struct Point2DCustom { float x; float y; };
  using BoundingBox2DCustom = std::array<Point2DCustom, 2>;


  // Adaptor

  struct AdaptorBasicsCustom
  {
    static constexpr float& point_comp(Point2DCustom& pt, OrthoTree::dim_type iDimension)
    {
      switch (iDimension)
      {
        case 0: return pt.x;
        case 1: return pt.y;
        default: return pt.x;
      }
    }

    static constexpr float point_comp_c(Point2DCustom const& pt, OrthoTree::dim_type iDimension)
    {
      switch (iDimension)
      {
        case 0: return pt.x;
        case 1: return pt.y;
        default: assert(false); return pt.x;
      }
    }

    static constexpr Point2DCustom& box_min(BoundingBox2DCustom& Box) { return Box[0]; }
    static constexpr Point2DCustom& box_max(BoundingBox2DCustom& Box) { return Box[1]; }
    static constexpr Point2DCustom const& box_min_c(BoundingBox2DCustom const& Box) { return Box[0]; }
    static constexpr Point2DCustom const& box_max_c(BoundingBox2DCustom const& Box) { return Box[1]; }
  };

  using AdaptorCustom = OrthoTree::AdaptorGeneralBase<2, Point2DCustom, BoundingBox2DCustom, AdaptorBasicsCustom, float>;


  // Tailored Quadtree objects

  using QuadtreePointCustom = OrthoTree::OrthoTreePoint<2, Point2DCustom, BoundingBox2DCustom, AdaptorCustom, float>;
  using QuadtreeBoxCustom = OrthoTree::OrthoTreeBoundingBox<2, Point2DCustom, BoundingBox2DCustom, AdaptorCustom, float>;

}



namespace AdaptorTest
{
  using namespace CustomGeometryType;

  static bool AreEqualAlmost(BoundingBox2DCustom const& l, BoundingBox2DCustom const& r) noexcept
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
      autoc tree = QuadtreePointCustom(vector<Point2DCustom>{}, 2);
      autoc& nodes = tree.GetNodes();
      Assert::IsTrue(nodes.size() == 1);
      Assert::IsTrue(nodes.at(1).Entities.empty());
      Assert::IsTrue(AreEqualAlmost(tree.GetBox(), BoundingBox2DCustom{}));
    }

    TEST_METHOD(Insert_NonLeaf_Successful)
    {
      autoce vpt = array{ Point2DCustom{ 0.0, 0.0 }, Point2DCustom{ 1.0, 1.0 }, Point2DCustom{ 2.0, 2.0 }, Point2DCustom{ 3.0, 3.0 } };
      auto tree = QuadtreePointCustom(vpt, 3, std::nullopt, 2);

      Assert::IsTrue(tree.Insert(4, Point2DCustom{ 2.5, 2.5 }, false));

      autoc& nodes = tree.GetNodes();
      Assert::AreEqual<size_t>(7, nodes.size());
      Assert::IsTrue(nodes.at(tree.GetHash(2, 15)).Entities == vector<size_t>{ 3, 4 });
    }

    TEST_METHOD(Contains__1__True)
    {
      autoce vpt = array{ Point2DCustom{ 0.0, 0.0 }, Point2DCustom{ 1.0, 1.0 }, Point2DCustom{ 2.0, 2.0 }, Point2DCustom{ 3.0, 3.0 } };
      auto tree = QuadtreePointCustom(vpt, 3, std::nullopt, 2);

      Assert::IsTrue(tree.Contains(vpt[1], vpt, 0));
    }
  };

  namespace EigenAdaptorTest
  {
    TEST_CLASS(EigenTest)
    {
    public:
      TEST_METHOD(PointGeneral3D)
      {        
        auto vpt = vector{
          Eigen::Vector3d(0.0, 0.0, 0.0), 
          Eigen::Vector3d(1.0, 1.0, 1.0),
          Eigen::Vector3d(2.0, 2.0, 2.0),
          Eigen::Vector3d(3.0, 3.0, 3.0),
          Eigen::Vector3d(4.0, 4.0, 4.0),
          Eigen::Vector3d(0.0, 0.0, 4.0),
          Eigen::Vector3d(0.0, 4.0, 0.0),
          Eigen::Vector3d(4.0, 0.0, 0.0),
          Eigen::Vector3d(1.5, 1.5, 1.0),
          Eigen::Vector3d(1.0, 1.5, 1.5),
        };
        auto tree = Eigen::OctreePoint3d(vpt, 3, std::nullopt, 2);
        
        auto entityIDsInBFS = tree.CollectAllIdInBFS(tree.GetRootKey());
        auto entityIDsInDFS = tree.CollectAllIdInDFS(tree.GetRootKey());

        auto searchBox = Eigen::AlignedBox3d(Eigen::Vector3d(0.0, 0.0, 0.0), Eigen::Vector3d(2.0, 2.0, 2.0));
        auto pointsInSearchBox = tree.RangeSearch(searchBox, vpt);
        
        auto sqrt3Reciproc = 1.0 / sqrt(3.0);
        auto pointsInPlane = tree.PlaneSearch(2.6, Eigen::Vector3d(sqrt3Reciproc, sqrt3Reciproc, sqrt3Reciproc), 0.3, vpt);

        autoc n = vpt.size();
        vpt.push_back(Eigen::Vector3d(1.0, 1.0, 1.5));
        tree.Insert(n, vpt.back());
        tree.Erase<false>(0, vpt[0]);
        auto entityIDsInDFS_AfterErase = tree.CollectAllIdInDFS();

        auto searchPoint = Eigen::Vector3d(1.0, 1.0, 1.0);
        auto entityIDsKNN = tree.GetNearestNeighbors(searchPoint, 3, vpt);
        
        Assert::IsTrue(std::ranges::is_permutation(vector<entity_id_type>{ 0, 1, 2, 8, 9 }, pointsInSearchBox));
        Assert::IsTrue(std::ranges::is_permutation(vector<entity_id_type>{ 5, 6, 7, 8, 9 }, pointsInPlane));
        Assert::IsTrue(std::ranges::is_permutation(vector<entity_id_type>{ 1, 10, 8 }, entityIDsKNN));

        Assert::IsTrue(vector<entity_id_type>{ 7, 6, 5, 0, 2, 1, 8, 9, 3, 4 } == entityIDsInBFS);
        Assert::IsTrue(vector<entity_id_type>{ 0, 1, 8, 9, 7, 6, 5, 2, 3, 4 } == entityIDsInDFS);
        Assert::IsTrue(vector<entity_id_type>{ 1, 8, 9, 10, 7, 6, 5, 2, 3, 4 } == entityIDsInDFS_AfterErase);
      }
 
      TEST_METHOD(BoxGeneral2DC_Example2)
      {
        auto boxes = std::vector
        {
          Eigen::AlignedBox2d( Eigen::Vector2d( 0.0, 0.0 ), Eigen::Vector2d( 1.0, 1.0 ) ),
          Eigen::AlignedBox2d( Eigen::Vector2d( 1.0, 1.0 ), Eigen::Vector2d( 2.0, 2.0 ) ),
          Eigen::AlignedBox2d( Eigen::Vector2d( 2.0, 2.0 ), Eigen::Vector2d( 3.0, 3.0 ) ),
          Eigen::AlignedBox2d( Eigen::Vector2d( 3.0, 3.0 ), Eigen::Vector2d( 4.0, 4.0 ) ),
          Eigen::AlignedBox2d( Eigen::Vector2d( 1.2, 1.2 ), Eigen::Vector2d( 2.8, 2.8 ) )
        };

        auto quadtree = Eigen::QuadtreeBoxC2d(boxes
          , 3            // max depth
          , std::nullopt // user-provided bounding Box for all
          , 2            // max element in a node 
          , false        // parallel calculation option
        );

        auto collidingIDPairs = quadtree.CollisionDetection(); //: { {1,4}, {2,4} }

        auto searchBox = Eigen::AlignedBox2d( Eigen::Vector2d( 1.0, 1.0 ), Eigen::Vector2d( 3.1, 3.1 ) );

        // Boxes within the range
        auto insideBoxIDs = quadtree.RangeSearch(searchBox); //: { 1, 2, 4 }

        // Overlapping Boxes with the range
        constexpr bool shouldFullyContain = false; // overlap is enough
        auto overlappingBoxIDs = quadtree.RangeSearch<shouldFullyContain>(searchBox); //: { 1, 2, 3, 4 }

        // Picked boxes
        auto pickPoint = Eigen::Vector2d(2.5, 2.5);
        auto pickedIDs = quadtree.PickSearch(pickPoint); //: { 2, 4 }

        // Ray intersections
        auto rayBasePoint = Eigen::Vector2d(1.5, 2.5);
        auto rayHeading = Eigen::Vector2d(1.5, 0.5);
        auto firstIntersectedBox = quadtree.RayIntersectedFirst(rayBasePoint, rayHeading, 0.01); //: 4
        auto intersectedPoints = quadtree.RayIntersectedAll(rayBasePoint, rayHeading, 0.01); //: { 4, 2, 3 } in distance order!

        // Collect all IDs in breadth/depth first order
        auto entityIDsInDFS = quadtree.CollectAllIdInBFS();
        auto entityIDsInBFS = quadtree.CollectAllIdInDFS();

        Assert::IsTrue(std::ranges::is_permutation(vector<std::pair<entity_id_type, entity_id_type>>{ {1, 4}, { 2, 4 } }, collidingIDPairs));
        Assert::IsTrue(std::ranges::is_permutation(vector<entity_id_type>{1, 2, 4}, insideBoxIDs));
        Assert::IsTrue(std::ranges::is_permutation(vector<entity_id_type>{1, 2, 3, 4}, overlappingBoxIDs));
        Assert::IsTrue(std::ranges::is_permutation(vector<entity_id_type>{2, 4}, pickedIDs));
        Assert::IsTrue(firstIntersectedBox.has_value());
        Assert::AreEqual(entity_id_type(4), *firstIntersectedBox);
        Assert::IsTrue(vector<entity_id_type>{ 4, 2, 3 } == intersectedPoints);
        Assert::IsTrue(vector<entity_id_type>{ 4, 0, 1, 2, 3 } == entityIDsInDFS);
        Assert::IsTrue(vector<entity_id_type>{ 4, 0, 1, 2, 3 } == entityIDsInBFS);
      }
    };
  }

  
  namespace XYZAdaptorTest
  {
    using namespace BasicTypesXYZ;

    TEST_CLASS(XYZTest)
    {
    public:
      TEST_METHOD(PointGeneral3D)
      {
        auto vpt = vector{
          BasicTypesXYZ::Point3D(0.0, 0.0, 0.0), 
          BasicTypesXYZ::Point3D(1.0, 1.0, 1.0),
          BasicTypesXYZ::Point3D(2.0, 2.0, 2.0),
          BasicTypesXYZ::Point3D(3.0, 3.0, 3.0),
          BasicTypesXYZ::Point3D(4.0, 4.0, 4.0),
          BasicTypesXYZ::Point3D(0.0, 0.0, 4.0),
          BasicTypesXYZ::Point3D(0.0, 4.0, 0.0),
          BasicTypesXYZ::Point3D(4.0, 0.0, 0.0),
          BasicTypesXYZ::Point3D(1.5, 1.5, 1.0),
          BasicTypesXYZ::Point3D(1.0, 1.5, 1.5),
        };
        auto tree = XYZ::OctreePoint(vpt, 3, std::nullopt, 2);
        
        auto entityIDsInBFS = tree.CollectAllIdInBFS(tree.GetRootKey());
        auto entityIDsInDFS = tree.CollectAllIdInDFS(tree.GetRootKey());

        auto searchBox = BasicTypesXYZ::BoundingBox3D{
          BasicTypesXYZ::Point3D{0.0, 0.0, 0.0},
          BasicTypesXYZ::Point3D{2.0, 2.0, 2.0}
        };
        auto pointsInSearchBox = tree.RangeSearch(searchBox, vpt);
        
        auto sqrt3Reciproc = BasicTypesXYZ::float_t(1.0 / sqrt(3.0));
        auto pointsInPlane = tree.PlaneSearch(2.6f, BasicTypesXYZ::Point3D{ sqrt3Reciproc, sqrt3Reciproc, sqrt3Reciproc }, 0.3f, vpt);

        autoc n = vpt.size();
        vpt.push_back(BasicTypesXYZ::Point3D(1.0, 1.0, 1.5));
        tree.Insert(n, vpt.back());
        tree.Erase<false>(0, vpt[0]);
        auto entityIDsInDFS_AfterErase = tree.CollectAllIdInDFS();

        auto searchPoint = BasicTypesXYZ::Point3D(1.0, 1.0, 1.0);
        auto entityIDsKNN = tree.GetNearestNeighbors(searchPoint, 3, vpt);
        
        Assert::IsTrue(std::ranges::is_permutation(vector<entity_id_type>{ 0, 1, 2, 8, 9 }, pointsInSearchBox));
        Assert::IsTrue(std::ranges::is_permutation(vector<entity_id_type>{ 5, 6, 7, 8, 9 }, pointsInPlane));
        Assert::IsTrue(std::ranges::is_permutation(vector<entity_id_type>{ 1, 10, 8 }, entityIDsKNN));

        Assert::IsTrue(vector<entity_id_type>{ 7, 6, 5, 0, 2, 1, 8, 9, 3, 4 } == entityIDsInBFS);
        Assert::IsTrue(vector<entity_id_type>{ 0, 1, 8, 9, 7, 6, 5, 2, 3, 4 } == entityIDsInDFS);
        Assert::IsTrue(vector<entity_id_type>{ 1, 8, 9, 10, 7, 6, 5, 2, 3, 4 } == entityIDsInDFS_AfterErase);
      }
 
      TEST_METHOD(BoxGeneral2DC_Example2)
      {
        auto boxes = std::vector
        {
          BasicTypesXYZ::BoundingBox2D{ BasicTypesXYZ::Point2D{ 0.0f, 0.0f }, BasicTypesXYZ::Point2D{ 1.0f, 1.0f } },
          BasicTypesXYZ::BoundingBox2D{ BasicTypesXYZ::Point2D{ 1.0f, 1.0f }, BasicTypesXYZ::Point2D{ 2.0f, 2.0f } },
          BasicTypesXYZ::BoundingBox2D{ BasicTypesXYZ::Point2D{ 2.0f, 2.0f }, BasicTypesXYZ::Point2D{ 3.0f, 3.0f } },
          BasicTypesXYZ::BoundingBox2D{ BasicTypesXYZ::Point2D{ 3.0f, 3.0f }, BasicTypesXYZ::Point2D{ 4.0f, 4.0f } },
          BasicTypesXYZ::BoundingBox2D{ BasicTypesXYZ::Point2D{ 1.2f, 1.2f }, BasicTypesXYZ::Point2D{ 2.8f, 2.8f } }
        };

        auto quadtree = XYZ::QuadtreeBoxC(boxes
          , 3            // max depth
          , std::nullopt // user-provided bounding Box for all
          , 2            // max element in a node 
          , false        // parallel calculation option
        );

        auto collidingIDPairs = quadtree.CollisionDetection(); //: { {1,4}, {2,4} }

        auto searchBox = BasicTypesXYZ::BoundingBox2D{
          BasicTypesXYZ::Point2D{1.0f, 1.0f},
          BasicTypesXYZ::Point2D{3.1f, 3.1f}
        };

        // Boxes within the range
        auto insideBoxIDs = quadtree.RangeSearch(searchBox); //: { 1, 2, 4 }

        // Overlapping Boxes with the range
        constexpr bool shouldFullyContain = false; // overlap is enough
        auto overlappingBoxIDs = quadtree.RangeSearch<shouldFullyContain>(searchBox); //: { 1, 2, 3, 4 }

        // Picked boxes
        auto pickPoint = BasicTypesXYZ::Point2D{ 2.5f, 2.5f };
        auto pickedIDs = quadtree.PickSearch(pickPoint); //: { 2, 4 }

        // Ray intersections
        auto rayBasePoint = BasicTypesXYZ::Point2D{ 1.5f, 2.5f };
        auto rayHeading = BasicTypesXYZ::Point2D{ 1.5f, 0.5f };
        auto firstIntersectedBox = quadtree.RayIntersectedFirst(rayBasePoint, rayHeading, 0.01f); //: 4
        auto intersectedPoints = quadtree.RayIntersectedAll(rayBasePoint, rayHeading, 0.01f); //: { 4, 2, 3 } in distance order!

        // Collect all IDs in breadth/depth first order
        auto entityIDsInDFS = quadtree.CollectAllIdInBFS();
        auto entityIDsInBFS = quadtree.CollectAllIdInDFS();

        Assert::IsTrue(std::ranges::is_permutation(vector<std::pair<entity_id_type, entity_id_type>>{ {1, 4}, { 2, 4 } }, collidingIDPairs));
        Assert::IsTrue(std::ranges::is_permutation(vector<entity_id_type>{1, 2, 4}, insideBoxIDs));
        Assert::IsTrue(std::ranges::is_permutation(vector<entity_id_type>{1, 2, 3, 4}, overlappingBoxIDs));
        Assert::IsTrue(std::ranges::is_permutation(vector<entity_id_type>{2, 4}, pickedIDs));
        Assert::IsTrue(firstIntersectedBox.has_value());
        Assert::AreEqual(entity_id_type(4), *firstIntersectedBox);
        Assert::IsTrue(vector<entity_id_type>{ 4, 2, 3 } == intersectedPoints);
        Assert::IsTrue(vector<entity_id_type>{ 4, 0, 1, 2, 3 } == entityIDsInDFS);
        Assert::IsTrue(vector<entity_id_type>{ 4, 0, 1, 2, 3 } == entityIDsInBFS);
      }
    };
  } // namespace XYZAdaptorTest


  namespace UnrealAdaptorTest
  {
    using namespace UnrealDummyTypes;

    TEST_CLASS(UnrealTest)
    {
    public:
      TEST_METHOD(PointGeneral3D)
      {        
        auto vpt = vector{
          FVector(0.0, 0.0, 0.0), 
          FVector(1.0, 1.0, 1.0),
          FVector(2.0, 2.0, 2.0),
          FVector(3.0, 3.0, 3.0),
          FVector(4.0, 4.0, 4.0),
          FVector(0.0, 0.0, 4.0),
          FVector(0.0, 4.0, 0.0),
          FVector(4.0, 0.0, 0.0),
          FVector(1.5, 1.5, 1.0),
          FVector(1.0, 1.5, 1.5),
        };
        auto tree = FOctreePoint(vpt, 3, std::nullopt, 2);
        
        auto entityIDsInBFS = tree.CollectAllIdInBFS(tree.GetRootKey());
        auto entityIDsInDFS = tree.CollectAllIdInDFS(tree.GetRootKey());

        auto searchBox = FBox(
          FVector(0.0, 0.0, 0.0),
          FVector(2.0, 2.0, 2.0)
        );
        auto pointsInSearchBox = tree.RangeSearch(searchBox, vpt);
        
        auto sqrt3Reciproc = 1.0 / sqrt(3.0);
        auto pointsInPlane = tree.PlaneSearch(2.6, FVector( sqrt3Reciproc, sqrt3Reciproc, sqrt3Reciproc ), 0.3, vpt);

        autoc n = vpt.size();
        vpt.push_back(FVector(1.0, 1.0, 1.5));
        tree.Insert(n, vpt.back());
        tree.Erase<false>(0, vpt[0]);
        auto entityIDsInDFS_AfterErase = tree.CollectAllIdInDFS();

        auto searchPoint = FVector{ 1.0, 1.0, 1.0 };
        auto entityIDsKNN = tree.GetNearestNeighbors(searchPoint, 3, vpt);
      }
 
      TEST_METHOD(BoxGeneral2DC_Example2)
      {
        auto boxes = std::vector
        {
          FBox2D(FVector2D(0.0, 0.0), FVector2D(1.0, 1.0)),
          FBox2D(FVector2D(1.0, 1.0), FVector2D(2.0, 2.0)),
          FBox2D(FVector2D(2.0, 2.0), FVector2D(3.0, 3.0)),
          FBox2D(FVector2D(3.0, 3.0), FVector2D(4.0, 4.0)),
          FBox2D(FVector2D(1.2, 1.2), FVector2D(2.8, 2.8))
        };

        auto quadtree = FQuadtreeBox2DC(
          boxes
          , 3            // max depth
          , std::nullopt // user-provided bounding Box for all
          , 2            // max element in a node 
          , false        // parallel calculation option
        );

        auto collidingIDPairs = quadtree.CollisionDetection(); //: { {1,4}, {2,4} }

        auto searchBox = FBox2D(
          FVector2D(1.0, 1.0),
          FVector2D(3.1, 3.1)
        );

        // Boxes within the range
        auto insideBoxIDs = quadtree.RangeSearch(searchBox); //: { 1, 2, 4 }

        // Overlapping Boxes with the range
        constexpr bool shouldFullyContain = false; // overlap is enough
        auto overlappingBoxIDs = quadtree.RangeSearch<shouldFullyContain>(searchBox); //: { 1, 2, 3, 4 }

        // Picked boxes
        auto pickPoint = FVector2D( 2.5, 2.5 );
        auto pickedIDs = quadtree.PickSearch(pickPoint); //: { 2, 4 }

        // Ray intersections
        auto rayBasePoint = FVector2D( 1.5, 2.5 );
        auto rayHeading = FVector2D( 1.5, 0.5 );
        auto firstIntersectedBox = quadtree.RayIntersectedFirst(rayBasePoint, rayHeading, 0.01); //: 4
        auto intersectedPoints = quadtree.RayIntersectedAll(rayBasePoint, rayHeading, 0.01); //: { 4, 2, 3 } in distance order!

        // Collect all IDs in breadth/depth first order
        auto entityIDsInDFS = quadtree.CollectAllIdInBFS();
        auto entityIDsInBFS = quadtree.CollectAllIdInDFS();
      }
    };
  }
}