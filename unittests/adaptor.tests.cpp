#include "pch.h"

#include <boost/geometry.hpp>
#include "../adaptor.boost.h"

#ifdef _MSC_VER
#pragma warning(disable : 4127)
#pragma warning(disable : 5054)
#endif

#include <Eigen/Dense>
#include <Eigen/Geometry>
#include "../adaptor.eigen.h"
#ifdef _MSC_VER
#pragma warning(default : 4127)
#pragma warning(default : 5054)
#endif

#include "../adaptor.xyz.h"

#define UNREAL_DUMMY_TYPES
#define UNREAL_DUMMY_TYPES__SUPPRESS_ASSERTS
#ifndef UNREAL_DUMMY_TYPES
#include "Math/Box.h"
#include "Math/Box2D.h"
#include "Math/Plane.h"
#include "Math/Ray.h"
#include "Math/Vector.h"
#include "Math/Vector2D.h"
#endif // !UNREAL_DUMMY_TYPES
#include "../adaptor.unreal.h"

#include <functional>

using namespace OrthoTree;

using std::array;
using std::vector;

namespace CustomGeometryType
{
  // User-defined geometrical objects

  struct MyPoint2D { float x; float y; };
  using MyBox2D = std::array<MyPoint2D, 2>;
  using MyRay2D = std::array<MyPoint2D, 2>;
  struct MyPlane2D { float OrigoDistance; MyPoint2D Normal; };


  // Adaptor

  struct AdaptorBasicsCustom
  {
    static float GetPointC(MyPoint2D const& pt, OrthoTree::dim_t i)
    {
      switch (i)
      {
      case 0: return pt.x;
      case 1: return pt.y;
      default: assert(false); return pt.x;
      }
    }

    static void SetPointC(MyPoint2D& pt, OrthoTree::dim_t i, float v)
    {
      switch (i)
      {
        case 0:  pt.x = v; break;
        case 1:  pt.y = v; break;
        default: assert(false);
      }
    }

    static void SetBoxMinC(MyBox2D& box, dim_t i, float v) { SetPointC(box[0], i, v); }
    static void SetBoxMaxC(MyBox2D& box, dim_t i, float v) { SetPointC(box[1], i, v); }
    static float GetBoxMinC(MyBox2D const& box, dim_t i) { return GetPointC(box[0], i); }
    static float GetBoxMaxC(MyBox2D const& box, dim_t i) { return GetPointC(box[1], i); }

    static MyPoint2D const& GetRayDirection(MyRay2D const& ray) { return ray[1]; }
    static MyPoint2D const& GetRayOrigin(MyRay2D const& ray) { return ray[0]; }

    static MyPoint2D const& GetPlaneNormal(MyPlane2D const& plane) { return plane.Normal; }
    static float GetPlaneOrigoDistance(MyPlane2D const& plane) { return plane.OrigoDistance; }
  };

  using AdaptorCustom = OrthoTree::AdaptorGeneralBase<
    2, 
    MyPoint2D, 
    MyBox2D, 
    MyRay2D, 
    MyPlane2D, 
    float, 
    AdaptorBasicsCustom>;


  // Tailored Quadtree objects

  using QuadtreePointCustom = OrthoTree::OrthoTreePoint<
    2, 
    MyPoint2D, 
    MyBox2D, 
    MyRay2D, 
    MyPlane2D, 
    float, 
    AdaptorCustom>;

  using QuadtreeBoxCustom = OrthoTree::OrthoTreeBoundingBox<
    2, 
    MyPoint2D, 
    MyBox2D, 
    MyRay2D, 
    MyPlane2D, 
    float, 
    true, 
    AdaptorCustom>;
}



namespace AdaptorTest
{
  using namespace CustomGeometryType;

  static bool AreEqualAlmost(MyBox2D const& l, MyBox2D const& r) noexcept
  {
    for (auto const iMax : { 0, 1 })
      for (dim_t iD = 0; iD < 2; ++iD)
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
      auto const tree = QuadtreePointCustom(vector<MyPoint2D>{}, 2);
      auto const& nodes = tree.GetNodes();
      Assert::IsTrue(nodes.size() == 1);
      Assert::IsTrue(tree.IsNodeEntitiesEmpty(1));

      auto const& box = tree.GetBox();
      Assert::IsTrue(AreEqualAlmost(
        MyBox2D{
          MyPoint2D{box.Min[0], box.Min[1]},
          MyPoint2D{box.Max[0], box.Min[1]}
      },
        MyBox2D{}));
    }

    TEST_METHOD(Insert_NonLeaf_Successful)
    {
      auto constexpr vpt = array{ MyPoint2D{ 0.0, 0.0 }, MyPoint2D{ 1.0, 1.0 }, MyPoint2D{ 2.0, 2.0 }, MyPoint2D{ 3.0, 3.0 } };
      auto tree = QuadtreePointCustom(vpt, 3, std::nullopt, 1);

      Assert::IsTrue(tree.Insert(4, MyPoint2D{ 2.5, 2.5 }, false));

      auto const& nodes = tree.GetNodes();
      Assert::AreEqual<size_t>(7, nodes.size());
      Assert::IsTrue(AreContainersItemsEqual(tree.GetNodeEntities(QuadtreePointCustom::SI::GetHash(2, 15)), vector<size_t>{ 3, 4 }));
    }

    TEST_METHOD(Contains__1__True)
    {
      auto constexpr vpt = array{ MyPoint2D{ 0.0, 0.0 }, MyPoint2D{ 1.0, 1.0 }, MyPoint2D{ 2.0, 2.0 }, MyPoint2D{ 3.0, 3.0 } };
      auto tree = QuadtreePointCustom(vpt, 3, std::nullopt, 1);

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
        using EntityID = Eigen::OctreePoint3d::TEntityID;

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
        auto tree = Eigen::OctreePoint3d(vpt, 3, std::nullopt, 1);
        
        auto entityIDsInBFS = tree.CollectAllEntitiesInBFS(Eigen::OctreePoint3d::SI::GetRootKey());
        auto entityIDsInDFS = tree.CollectAllEntitiesInDFS(Eigen::OctreePoint3d::SI::GetRootKey());

        auto searchBox = Eigen::AlignedBox3d(Eigen::Vector3d(0.0, 0.0, 0.0), Eigen::Vector3d(2.0, 2.0, 2.0));
        auto pointsInSearchBox = tree.RangeSearch(searchBox, vpt);
        
        auto sqrt3Reciproc = 1.0 / sqrt(3.0);
        auto pointsInPlane = tree.PlaneSearch(2.6, Eigen::Vector3d(sqrt3Reciproc, sqrt3Reciproc, sqrt3Reciproc), 0.3, vpt);
        
        auto sqrt2 = sqrt(2.0);
        auto sqrt2Reciproc = 1.0 / sqrt2;
        vector<Eigen::Hyperplane<double, 3>> planes;
        planes.emplace_back(Eigen::Vector3d(sqrt2Reciproc, sqrt2Reciproc, 0.0), -2 * sqrt2);
        planes.emplace_back(Eigen::Vector3d(0.0, sqrt2Reciproc, sqrt2Reciproc), -2 * sqrt2);
        planes.emplace_back(Eigen::Vector3d(sqrt2Reciproc, -sqrt2Reciproc, 0.0), 0);

        auto pointsInFrustum = tree.FrustumCulling(planes, 0.01, vpt);
          
        auto const n = EntityID(vpt.size());
        vpt.push_back(Eigen::Vector3d(1.0, 1.0, 1.5));
        tree.Insert(n, vpt.back());
        tree.Erase<false>(0, vpt[0]);
        auto entityIDsInDFS_AfterErase = tree.CollectAllEntitiesInDFS();

        auto searchPoint = Eigen::Vector3d(1.0, 1.0, 1.0);
        auto entityIDsKNN = tree.GetNearestNeighbors(searchPoint, 3, vpt);
        
        Assert::IsTrue(std::ranges::is_permutation(vector<EntityID>{ 0, 1, 2, 8, 9 }, pointsInSearchBox));
        Assert::IsTrue(std::ranges::is_permutation(vector<EntityID>{ 5, 6, 7, 8, 9 }, pointsInPlane));
        Assert::IsTrue(std::ranges::is_permutation(vector<EntityID>{ 1, 10, 8 }, entityIDsKNN));
        Assert::IsTrue(std::ranges::is_permutation(vector<EntityID>{ 2, 3, 4, 6, 7 }, pointsInFrustum));

        Assert::IsTrue(vector<EntityID>{ 7, 6, 5, 0, 2, 1, 8, 9, 3, 4 } == entityIDsInBFS);
        Assert::IsTrue(vector<EntityID>{ 0, 1, 8, 9, 7, 6, 5, 2, 3, 4 } == entityIDsInDFS);
        Assert::IsTrue(vector<EntityID>{ 1, 8, 10, 9, 7, 6, 5, 2, 3, 4 } == entityIDsInDFS_AfterErase);
      }
 
      TEST_METHOD(BoxGeneral2DC_Example2)
      {
        using EntityID = Eigen::QuadtreeBoxC2d::TEntityID;

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
          , 1            // max element in a node 
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

        // Plane       
        auto sqrt2 = sqrt(2.0);
        auto sqrt2Reciproc = 1.0 / sqrt2;
        auto boxesInFrustum = quadtree.FrustumCulling( // Cross-point of the planes: 2;2;2
          vector{
            Eigen::Hyperplane<double, 2>(Eigen::Vector2d(sqrt2Reciproc, sqrt2Reciproc), -2 * sqrt2),
            Eigen::Hyperplane<double, 2>(Eigen::Vector2d(0, -1.0), -2.0)
        },
          0.01f);

        // Collect all IDs in breadth/depth first order
        auto entityIDsInDFS = quadtree.CollectAllEntitiesInBFS();
        auto entityIDsInBFS = quadtree.CollectAllEntitiesInDFS();

        Assert::IsTrue(std::ranges::is_permutation(
          vector<std::pair<EntityID, EntityID>>{
            { 1, 4 },
            { 2, 4 }
        },
          collidingIDPairs));
        Assert::IsTrue(std::ranges::is_permutation(vector<EntityID>{1, 2, 4}, insideBoxIDs));
        Assert::IsTrue(std::ranges::is_permutation(vector<EntityID>{1, 2, 3, 4}, overlappingBoxIDs));
        Assert::IsTrue(std::ranges::is_permutation(vector<EntityID>{2, 4}, pickedIDs));
        Assert::IsTrue(firstIntersectedBox.has_value());
        Assert::AreEqual(EntityID(4), *firstIntersectedBox);

        Assert::IsTrue(std::ranges::is_permutation(vector<EntityID>{ 1, 2, 4 }, boxesInFrustum));

        Assert::IsTrue(vector<EntityID>{ 4, 2, 3 } == intersectedPoints);
        Assert::IsTrue(vector<EntityID>{ 4, 0, 1, 2, 3 } == entityIDsInDFS);
        Assert::IsTrue(vector<EntityID>{ 4, 0, 1, 2, 3 } == entityIDsInBFS);
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
        using EntityID = XYZ::OctreePoint::TEntityID;

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
        auto tree = XYZ::OctreePoint(vpt, 3, std::nullopt, 1);
        
        auto entityIDsInBFS = tree.CollectAllEntitiesInBFS(OctreePoint::SI::GetRootKey());
        auto entityIDsInDFS = tree.CollectAllEntitiesInDFS(OctreePoint::SI::GetRootKey());

        auto searchBox = BasicTypesXYZ::BoundingBox3D{
          BasicTypesXYZ::Point3D{0.0, 0.0, 0.0},
          BasicTypesXYZ::Point3D{2.0, 2.0, 2.0}
        };
        auto pointsInSearchBox = tree.RangeSearch(searchBox, vpt);
        
        auto sqrt3Reciproc = BasicTypesXYZ::float_t(1.0 / sqrt(3.0));
        auto pointsInPlane = tree.PlaneSearch(2.6f, BasicTypesXYZ::Point3D{ sqrt3Reciproc, sqrt3Reciproc, sqrt3Reciproc }, 0.3f, vpt);

        auto sqrt2 = float_t(sqrt(2.0));
        auto sqrt2Reciproc = float_t(1.0 / sqrt2);
        
        auto pointsInFrustum = tree.FrustumCulling( // Cross-point of the planes: 2;2;2
          vector<BasicTypesXYZ::Plane3D>{
            {2 * sqrt2,  { sqrt2Reciproc, sqrt2Reciproc, 0.0 }},
            {2 * sqrt2,  { 0.0, sqrt2Reciproc, sqrt2Reciproc }},
            {      0.0, { sqrt2Reciproc, -sqrt2Reciproc, 0.0 }}
        },
          0.01f,
          vpt);

        auto const n = EntityID(vpt.size());
        vpt.push_back(BasicTypesXYZ::Point3D(1.0, 1.0, 1.5));
        tree.Insert(n, vpt.back());
        tree.Erase<false>(0, vpt[0]);
        auto entityIDsInDFS_AfterErase = tree.CollectAllEntitiesInDFS();

        auto searchPoint = BasicTypesXYZ::Point3D(1.0, 1.0, 1.0);
        auto entityIDsKNN = tree.GetNearestNeighbors(searchPoint, 3, vpt);
        
        Assert::IsTrue(std::ranges::is_permutation(vector<EntityID>{ 0, 1, 2, 8, 9 }, pointsInSearchBox));
        Assert::IsTrue(std::ranges::is_permutation(vector<EntityID>{ 5, 6, 7, 8, 9 }, pointsInPlane));
        Assert::IsTrue(std::ranges::is_permutation(vector<EntityID>{ 1, 10, 8 }, entityIDsKNN));
        Assert::IsTrue(std::ranges::is_permutation(vector<EntityID>{ 2, 3, 4, 6, 7 }, pointsInFrustum));

        Assert::IsTrue(vector<EntityID>{ 7, 6, 5, 0, 2, 1, 8, 9, 3, 4 } == entityIDsInBFS);
        Assert::IsTrue(vector<EntityID>{ 0, 1, 8, 9, 7, 6, 5, 2, 3, 4 } == entityIDsInDFS);
        Assert::IsTrue(vector<EntityID>{ 1, 8, 10, 9, 7, 6, 5, 2, 3, 4 } == entityIDsInDFS_AfterErase);
      }
 
      TEST_METHOD(BoxGeneral2DC_Example2)
      {
        using EntityID = XYZ::QuadtreeBoxC::TEntityID;

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
          , 1            // max element in a node 
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

        // Plane
        auto sqrt2 = sqrtf(2.0);
        auto sqrt2Reciproc = 1.0f / sqrt2;
        auto boxesInFrustum = quadtree.FrustumCulling( // Cross-point of the planes: 2;2;2
          vector<BasicTypesXYZ::Plane2D>{
            {2 * sqrt2, { sqrt2Reciproc, sqrt2Reciproc }},
            {     2.0f,                  { 0.0f, -1.0f }}
        },
          0.01f);

        // Collect all IDs in breadth/depth first order
        auto entityIDsInDFS = quadtree.CollectAllEntitiesInBFS();
        auto entityIDsInBFS = quadtree.CollectAllEntitiesInDFS();

        Assert::IsTrue(std::ranges::is_permutation(
          vector<std::pair<EntityID, EntityID>>{
            { 1, 4 },
            { 2, 4 }
        },
          collidingIDPairs));
        Assert::IsTrue(std::ranges::is_permutation(vector<EntityID>{1, 2, 4}, insideBoxIDs));
        Assert::IsTrue(std::ranges::is_permutation(vector<EntityID>{1, 2, 3, 4}, overlappingBoxIDs));
        Assert::IsTrue(std::ranges::is_permutation(vector<EntityID>{2, 4}, pickedIDs));
        Assert::IsTrue(firstIntersectedBox.has_value());
        Assert::AreEqual(EntityID(4), *firstIntersectedBox);

        Assert::IsTrue(std::ranges::is_permutation(vector<EntityID>{ 1, 2, 4 }, boxesInFrustum));

        Assert::IsTrue(vector<EntityID>{ 4, 2, 3 } == intersectedPoints);
        Assert::IsTrue(vector<EntityID>{ 4, 0, 1, 2, 3 } == entityIDsInDFS);
        Assert::IsTrue(vector<EntityID>{ 4, 0, 1, 2, 3 } == entityIDsInBFS);
      }
    };
  } // namespace XYZAdaptorTest

  namespace AbstractAdaptorTest
  {
    // User-defined geometrical objects

    struct MyPoint2DBase
    {
      float x;
      float y;

      MyPoint2DBase(float x, float y): x(x), y(y){}
      virtual ~MyPoint2DBase() {}

      virtual float GetDistance() const = 0;
    };
    
    struct MyPoint2DConcrete1 : MyPoint2DBase
    {
      using MyPoint2DBase::MyPoint2DBase;

      virtual float GetDistance() const override { return x + y; };
    };
    
    struct MyPoint2DConcrete2 : MyPoint2DBase
    {
      using MyPoint2DBase::MyPoint2DBase;

      virtual float GetDistance() const override { return std::sqrt(x*x + y*y); };
    };

    struct MyBox2DBase
    {
      std::unique_ptr<MyPoint2DBase> Min, Max;

      MyBox2DBase() = default;
      MyBox2DBase(std::unique_ptr<MyPoint2DBase>&& minPoint, std::unique_ptr<MyPoint2DBase>&& maxPoint)
      : Min(std::move(minPoint))
      , Max(std::move(maxPoint))
      {
      }

      virtual ~MyBox2DBase() {}
      virtual float GetSize() const = 0;
    };

    struct MyBox2DConcrete1 : MyBox2DBase
    {
      using MyBox2DBase::MyBox2DBase;
      virtual float GetSize() const override { return Max->x - Min->x; }
    };

    struct MyBox2DConcrete2 : MyBox2DBase
    {
      using MyBox2DBase::MyBox2DBase;
      virtual float GetSize() const override { return Max->y - Min->y; }
    };
    
    struct MyRay2DBase
    {
      std::unique_ptr<MyPoint2DBase> Origin, Direction;
    };
    struct MyPlane2DBase
    {
      float OrigoDistance;
      std::unique_ptr<MyPoint2DBase> Normal;

      MyPlane2DBase(float origoDistance, std::unique_ptr<MyPoint2DBase> normal)
      : OrigoDistance(origoDistance)
      , Normal(std::move(normal))
      {}
    };
    

    // Adaptor

    struct AdaptorBasicsCustom
    {
      static float GetPointC(MyPoint2DBase const* pt, OrthoTree::dim_t i)
      {
        switch (i)
        {
        case 0: return pt->x;
        case 1: return pt->y;
        default: assert(false); return pt->x;
        }
      }

      static void SetPointC(MyPoint2DBase* pt, OrthoTree::dim_t i, float v)
      {
        switch (i)
        {
        case 0: pt->x = v; break;
        case 1: pt->y = v; break;
        default: assert(false);
        }
      }

      static void SetBoxMinC(MyBox2DBase* box, dim_t i, float v) { SetPointC(box->Min.get(), i, v); }
      static void SetBoxMaxC(MyBox2DBase* box, dim_t i, float v) { SetPointC(box->Max.get(), i, v); }
      static float GetBoxMinC(MyBox2DBase const* box, dim_t i) { return GetPointC(box->Min.get(), i); }
      static float GetBoxMaxC(MyBox2DBase const* box, dim_t i) { return GetPointC(box->Max.get(), i); }

      static MyPoint2DBase* GetRayDirection(MyRay2DBase const& ray) { return ray.Direction.get(); }
      static MyPoint2DBase* GetRayOrigin(MyRay2DBase const& ray) { return ray.Origin.get(); }

      static MyPoint2DBase* GetPlaneNormal(MyPlane2DBase const& plane) { return plane.Normal.get(); }
      static float GetPlaneOrigoDistance(MyPlane2DBase const& plane) { return plane.OrigoDistance; }
    };

    using MyPoint2DPtr = MyPoint2DBase*;
    using MyBox2DPtr = MyBox2DBase*;
    using AdaptorCustom = OrthoTree::AdaptorGeneralBase<2, MyPoint2DBase*, MyBox2DBase*, MyRay2DBase, MyPlane2DBase, float, AdaptorBasicsCustom>;


    // Tailored Quadtree objects

    using QuadtreePointCustom = OrthoTree::OrthoTreePoint<2, MyPoint2DBase*, MyBox2DBase*, MyRay2DBase, MyPlane2DBase, float, AdaptorCustom>;

    using QuadtreeBoxCustom = OrthoTree::OrthoTreeBoundingBox<2, MyPoint2DBase*, MyBox2DBase*, MyRay2DBase, MyPlane2DBase, float, true, AdaptorCustom>;
    using QuadtreeBoxCustomC = OrthoTreeContainerBox<QuadtreeBoxCustom>;

    TEST_CLASS(AbstractAdaptorTest)
    { 
    public: 
      TEST_METHOD(PointGeneral2D)
      { 
        using EntityID = QuadtreePointCustom::TEntityID;

        auto vptOwner = std::vector<std::unique_ptr<MyPoint2DBase>>{};
        vptOwner.emplace_back(std::make_unique<MyPoint2DConcrete1>(0.0f, 0.0f));
        vptOwner.emplace_back(std::make_unique<MyPoint2DConcrete1>(1.0f, 0.0f));
        vptOwner.emplace_back(std::make_unique<MyPoint2DConcrete1>(1.0f, 1.0f));
        vptOwner.emplace_back(std::make_unique<MyPoint2DConcrete1>(2.0f, 2.0f));
        vptOwner.emplace_back(std::make_unique<MyPoint2DConcrete1>(3.0f, 3.0f));
        vptOwner.emplace_back(std::make_unique<MyPoint2DConcrete1>(4.0f, 4.0f));
        vptOwner.emplace_back(std::make_unique<MyPoint2DConcrete2>(0.0f, 1.0f));
        vptOwner.emplace_back(std::make_unique<MyPoint2DConcrete2>(0.0f, 4.0f));
        vptOwner.emplace_back(std::make_unique<MyPoint2DConcrete2>(4.0f, 0.0f));
        vptOwner.emplace_back(std::make_unique<MyPoint2DConcrete2>(1.5f, 1.5f));
        vptOwner.emplace_back(std::make_unique<MyPoint2DConcrete2>(1.0f, 1.5f));

        auto vptView = std::vector<MyPoint2DBase*>{};
        std::ranges::transform(vptOwner, std::back_inserter(vptView), [](auto& ppt) { return ppt.get(); });

        auto tree = QuadtreePointCustom(vptView, 3, std::nullopt, 1);

        auto entityIDsInBFS = tree.CollectAllEntitiesInBFS(QuadtreePointCustom::SI::GetRootKey());
        auto entityIDsInDFS = tree.CollectAllEntitiesInDFS(QuadtreePointCustom::SI::GetRootKey());

        auto searchBox = std::make_unique<MyBox2DConcrete1>();
        searchBox->Min = std::make_unique<MyPoint2DConcrete1>(0.0f, 0.0f);
        searchBox->Max = std::make_unique<MyPoint2DConcrete2>(2.0f, 2.0f);
        auto pointsInSearchBox = tree.RangeSearch(searchBox.get(), vptView);

        auto sqrt2Reciproc = float(1.0 / sqrt(2.0));
        auto planeNormal = std::make_unique<MyPoint2DConcrete2>(sqrt2Reciproc, sqrt2Reciproc);
        auto pointsInPlane = tree.PlaneSearch(2.6f, planeNormal.get(), 0.3f, vptView);

        auto planesForFustrum = std::vector<MyPlane2DBase>{};
        planesForFustrum.push_back({ -2.0f, std::make_unique<MyPoint2DConcrete1>(-1.0f, 0.0f) });
        planesForFustrum.push_back({ -2.0f, std::make_unique<MyPoint2DConcrete1>(0.0f, -1.0f) });
        planesForFustrum.push_back({ -2.0f, std::make_unique<MyPoint2DConcrete1>(+1.0f, 0.0f) });
        auto pointsInFrustum = tree.FrustumCulling(planesForFustrum, 0.01f, vptView);

        auto const n = EntityID(vptView.size());
        vptView.emplace_back(vptOwner.emplace_back(std::make_unique<MyPoint2DConcrete1>(1.0f, 1.1f)).get());
        tree.Insert(n, vptOwner.back().get());
        tree.Erase<false>(0, vptOwner.front().get());
        auto entityIDsInDFS_AfterErase = tree.CollectAllEntitiesInDFS();

        auto searchPoint = std::make_unique<MyPoint2DConcrete1>(1.0f, 1.0f);
        auto entityIDsKNN = tree.GetNearestNeighbors(searchPoint.get(), 3, vptView);

        Assert::IsTrue(std::ranges::is_permutation(std::vector<EntityID>{ 0, 1, 2, 3, 6, 9, 10 }, pointsInSearchBox));
        Assert::IsTrue(std::ranges::is_permutation(std::vector<EntityID>{ 3, 7, 8 }, pointsInPlane));
        Assert::IsTrue(std::ranges::is_permutation(std::vector<EntityID>{ 2, 11, 10 }, entityIDsKNN));
        Assert::IsTrue(std::ranges::is_permutation(std::vector<EntityID>{ 0, 1, 2, 3, 6, 9, 10 }, pointsInFrustum));

        Assert::IsTrue(std::vector<EntityID>{ 8, 7, 0, 1, 6, 3, 2, 10, 9, 4, 5 } == entityIDsInBFS);
        Assert::IsTrue(std::vector<EntityID>{ 0, 1, 6, 2, 10, 9, 8, 7, 3, 4, 5 } == entityIDsInDFS);
        Assert::IsTrue(std::vector<EntityID>{ 1, 6, 2, 11, 10, 9, 8, 7, 3, 4, 5 } == entityIDsInDFS_AfterErase);
      }

      TEST_METHOD(BoxGeneral2DC_Example2)
      {
        using EntityID = QuadtreeBoxCustomC::TEntityID;

        auto boxes = std::vector<std::unique_ptr<MyBox2DBase>>{};
        boxes.emplace_back(
          std::make_unique<MyBox2DConcrete1>(std::make_unique<MyPoint2DConcrete1>(0.0f, 0.0f), std::make_unique<MyPoint2DConcrete2>(1.0f, 1.0f)));
        boxes.emplace_back(
          std::make_unique<MyBox2DConcrete1>(std::make_unique<MyPoint2DConcrete1>(1.0f, 1.0f), std::make_unique<MyPoint2DConcrete2>(2.0f, 2.0f)));
        boxes.emplace_back(
          std::make_unique<MyBox2DConcrete1>(std::make_unique<MyPoint2DConcrete1>(2.0f, 2.0f), std::make_unique<MyPoint2DConcrete2>(3.0f, 3.0f)));
        boxes.emplace_back(
          std::make_unique<MyBox2DConcrete1>(std::make_unique<MyPoint2DConcrete1>(3.0f, 3.0f), std::make_unique<MyPoint2DConcrete2>(4.0f, 4.0f)));
        boxes.emplace_back(
          std::make_unique<MyBox2DConcrete1>(std::make_unique<MyPoint2DConcrete1>(1.2f, 1.2f), std::make_unique<MyPoint2DConcrete2>(2.8f, 2.8f)));

        auto boxesView = std::vector<MyBox2DBase*>{};
        std::ranges::transform(boxes, std::back_inserter(boxesView), [](auto& ppt) { return ppt.get(); });

        auto quadtree = QuadtreeBoxCustomC(
          boxesView,
          3, // max depth
          std::nullopt, // user-provided bounding Box for all
          1, // max element in a node
          false // parallel calculation option
        );

        auto collidingIDPairs = quadtree.CollisionDetection(); //: { {1,4}, {2,4} }

        auto searchBox = MyBox2DConcrete1(
          std::make_unique<MyPoint2DConcrete1>(1.0f, 1.0f),
          std::make_unique<MyPoint2DConcrete1>(3.1f, 3.1f)
        );

        // Boxes within the range
        auto insideBoxIDs = quadtree.RangeSearch(&searchBox); //: { 1, 2, 4 }

        // Overlapping Boxes with the range
        constexpr bool shouldFullyContain = false;                                    // overlap is enough
        auto overlappingBoxIDs = quadtree.RangeSearch<shouldFullyContain>(&searchBox); //: { 1, 2, 3, 4 }

        // Picked boxes
        auto pickPoint = MyPoint2DConcrete1( 2.5f, 2.5f );
        auto pickedIDs = quadtree.PickSearch(&pickPoint); //: { 2, 4 }

        // Ray intersections
        auto rayBasePoint = std::make_unique<MyPoint2DConcrete1>( 1.5f, 2.5f );
        auto rayHeading = MyPoint2DConcrete1( 1.5f, 0.5f );
        auto firstIntersectedBox = quadtree.RayIntersectedFirst(rayBasePoint.get(), &rayHeading, 0.01f); //: 4
        auto intersectedPoints = quadtree.RayIntersectedAll(rayBasePoint.get(), &rayHeading, 0.01f);     //: { 4, 2, 3 } in distance order!

        // Plane
        auto sqrt2 = sqrtf(2.0);
        auto sqrt2Reciproc = 1.0f / sqrt2;

        // Cross-point of the planes: 2;2;2
        auto frustumBoundaryPlanes = std::vector<MyPlane2DBase>();
        frustumBoundaryPlanes.emplace_back(2 * sqrt2, std::make_unique<MyPoint2DConcrete1>(sqrt2Reciproc, sqrt2Reciproc));        
        frustumBoundaryPlanes.emplace_back(2.0f, std::make_unique<MyPoint2DConcrete1>(0.0f, -1.0f));

        auto boxesInFrustum = quadtree.FrustumCulling( 
          frustumBoundaryPlanes,
          0.01f);

        // Collect all IDs in breadth/depth first order
        auto entityIDsInDFS = quadtree.CollectAllEntitiesInBFS();
        auto entityIDsInBFS = quadtree.CollectAllEntitiesInDFS();

        Assert::IsTrue(std::ranges::is_permutation(
          std::vector<std::pair<EntityID, EntityID>>{
            {1, 4},
            {2, 4}
        },
          collidingIDPairs));
        Assert::IsTrue(std::ranges::is_permutation(std::vector<EntityID>{ 1, 2, 4 }, insideBoxIDs));
        Assert::IsTrue(std::ranges::is_permutation(std::vector<EntityID>{ 1, 2, 3, 4 }, overlappingBoxIDs));
        Assert::IsTrue(std::ranges::is_permutation(std::vector<EntityID>{ 2, 4 }, pickedIDs));
        Assert::IsTrue(firstIntersectedBox.has_value());
        Assert::AreEqual(EntityID(4), *firstIntersectedBox);

        Assert::IsTrue(std::ranges::is_permutation(std::vector<EntityID>{ 1, 2, 4 }, boxesInFrustum));

        Assert::IsTrue(std::vector<EntityID>{ 4, 2, 3 } == intersectedPoints);
        Assert::IsTrue(std::vector<EntityID>{ 4, 0, 1, 2, 3 } == entityIDsInDFS);
        Assert::IsTrue(std::vector<EntityID>{ 4, 0, 1, 2, 3 } == entityIDsInBFS);
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
        using EntityID = FOctreePoint::TEntityID;

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
        auto tree = FOctreePoint(vpt, 3, std::nullopt, 1);
        
        auto entityIDsInBFS = tree.CollectAllEntitiesInBFS(FOctreePoint::SI::GetRootKey());
        auto entityIDsInDFS = tree.CollectAllEntitiesInDFS(FOctreePoint::SI::GetRootKey());

        auto searchBox = FBox(
          FVector(0.0, 0.0, 0.0),
          FVector(2.0, 2.0, 2.0)
        );
        auto pointsInSearchBox = tree.RangeSearch(searchBox, vpt);
        
        auto sqrt3Reciproc = 1.0 / sqrt(3.0);
        auto pointsInPlane = tree.PlaneSearch(2.6, FVector( sqrt3Reciproc, sqrt3Reciproc, sqrt3Reciproc ), 0.3, vpt);

        auto const n = EntityID(vpt.size());
        vpt.push_back(FVector(1.0, 1.0, 1.5));
        tree.Insert(n, vpt.back());
        tree.Erase<false>(0, vpt[0]);
        auto entityIDsInDFS_AfterErase = tree.CollectAllEntitiesInDFS();

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
          , 1            // max element in a node 
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

        using Plane = FQuadtreeBox2DC::TPlane;
        auto sqrt2 = sqrtf(2.0);
        auto sqrt2Reciproc = 1.0f / sqrt2;
        auto boxesInFrustum = quadtree.FrustumCulling( // Cross-point of the planes: 2;2;2
          vector{
            Plane{FVector2D{ sqrt2Reciproc, sqrt2Reciproc }, FVector2D{ 2.0f, 2.0f }},
            Plane{                 FVector2D{ 0.0f, -1.0f }, FVector2D{ 0.0f, 2.0f }}
        },
          0.01f);

        // Collect all IDs in breadth/depth first order
        auto entityIDsInDFS = quadtree.CollectAllEntitiesInBFS();
        auto entityIDsInBFS = quadtree.CollectAllEntitiesInDFS();
      }
    };
  }

  namespace BoostAdaptorTest
  {
    using namespace boost::geometry;

    TEST_CLASS(BoostTest)
    {
    public:
      TEST_METHOD(PointGeneral3D)
      {    
        using point_t = model::pointNd_t<3, double>;
        using box_t = model::boxNd_t<3, double>;
        using plane_t = model::planeNd_t<3, double>;
        using EntityID = boost::geometry::octree_point::TEntityID;

        auto vpt = vector{
          point_t(0.0, 0.0, 0.0), 
          point_t(1.0, 1.0, 1.0),
          point_t(2.0, 2.0, 2.0),
          point_t(3.0, 3.0, 3.0),
          point_t(4.0, 4.0, 4.0),
          point_t(0.0, 0.0, 4.0),
          point_t(0.0, 4.0, 0.0),
          point_t(4.0, 0.0, 0.0),
          point_t(1.5, 1.5, 1.0),
          point_t(1.0, 1.5, 1.5),
        };
        auto tree = boost::geometry::octree_point(vpt, 3, std::nullopt, 1);
        
        auto entityIDsInBFS = tree.CollectAllEntitiesInBFS(octree_point::SI::GetRootKey());
        auto entityIDsInDFS = tree.CollectAllEntitiesInDFS(octree_point::SI::GetRootKey());

        auto searchBox = box_t(point_t(0.0, 0.0, 0.0), point_t(2.0, 2.0, 2.0));
        auto pointsInSearchBox = tree.RangeSearch(searchBox, vpt);
        
        auto sqrt3Reciproc = 1.0 / sqrt(3.0);
        auto pointsInPlane = tree.PlaneSearch(2.6, point_t(sqrt3Reciproc, sqrt3Reciproc, sqrt3Reciproc), 0.3, vpt);

        auto sqrt2 = float_t(sqrt(2.0));
        auto sqrt2Reciproc = float_t(1.0 / sqrt2);

        auto pointsInFrustum = tree.FrustumCulling( // Cross-point of the planes: 2;2;2
          vector<plane_t>{
            {2 * sqrt2,  { sqrt2Reciproc, sqrt2Reciproc, 0.0 }},
            {2 * sqrt2,  { 0.0, sqrt2Reciproc, sqrt2Reciproc }},
            {      0.0, { sqrt2Reciproc, -sqrt2Reciproc, 0.0 }}
        },
          0.01f,
          vpt);

        auto const n = EntityID(vpt.size());
        vpt.push_back(point_t(1.0, 1.0, 1.5));
        tree.Insert(n, vpt.back());
        tree.Erase<false>(0, vpt[0]);
        auto entityIDsInDFS_AfterErase = tree.CollectAllEntitiesInDFS();

        auto searchPoint = point_t{ 1.0, 1.0, 1.0 };
        auto entityIDsKNN = tree.GetNearestNeighbors(searchPoint, 3, vpt);

        Assert::IsTrue(std::ranges::is_permutation(vector<EntityID>{ 0, 1, 2, 8, 9 }, pointsInSearchBox));
        Assert::IsTrue(std::ranges::is_permutation(vector<EntityID>{ 5, 6, 7, 8, 9 }, pointsInPlane));
        Assert::IsTrue(std::ranges::is_permutation(vector<EntityID>{ 1, 10, 8 }, entityIDsKNN));
        Assert::IsTrue(std::ranges::is_permutation(vector<EntityID>{ 2, 3, 4, 6, 7 }, pointsInFrustum));

        Assert::IsTrue(vector<EntityID>{ 7, 6, 5, 0, 2, 1, 8, 9, 3, 4 } == entityIDsInBFS);
        Assert::IsTrue(vector<EntityID>{ 0, 1, 8, 9, 7, 6, 5, 2, 3, 4 } == entityIDsInDFS);
        Assert::IsTrue(vector<EntityID>{ 1, 8, 10, 9, 7, 6, 5, 2, 3, 4 } == entityIDsInDFS_AfterErase);
      }
 
      TEST_METHOD(BoxGeneral2DC_Example2)
      {
        using point_t = model::pointNd_t<2, double>;
        using box_t = model::boxNd_t<2, double>;
        using plane_t = model::planeNd_t<2, double>;
        using EntityID = boost::geometry::quadtree_box_c::TEntityID;

        auto boxes = std::vector
        {
          box_t(point_t(0.0, 0.0), point_t(1.0, 1.0)),
          box_t(point_t(1.0, 1.0), point_t(2.0, 2.0)),
          box_t(point_t(2.0, 2.0), point_t(3.0, 3.0)),
          box_t(point_t(3.0, 3.0), point_t(4.0, 4.0)),
          box_t(point_t(1.2, 1.2), point_t(2.8, 2.8))
        };

        auto quadtree = boost::geometry::quadtree_box_c(
          boxes
          , 3            // max depth
          , std::nullopt // user-provided bounding Box for all
          , 1            // max element in a node 
          , false        // parallel calculation option
        );

        auto collidingIDPairs = quadtree.CollisionDetection(); //: { {1,4}, {2,4} }

        auto searchBox = box_t(point_t(1.0, 1.0), point_t(3.1, 3.1));

        // Boxes within the range
        auto insideBoxIDs = quadtree.RangeSearch(searchBox); //: { 1, 2, 4 }

        // Overlapping Boxes with the range
        constexpr bool shouldFullyContain = false; // overlap is enough
        auto overlappingBoxIDs = quadtree.RangeSearch<shouldFullyContain>(searchBox); //: { 1, 2, 3, 4 }

        // Picked boxes
        auto pickPoint = point_t(2.5, 2.5);
        auto pickedIDs = quadtree.PickSearch(pickPoint); //: { 2, 4 }

        // Ray intersections
        auto rayBasePoint = point_t(1.5, 2.5);
        auto rayHeading = point_t(1.5, 0.5);
        auto firstIntersectedBox = quadtree.RayIntersectedFirst(rayBasePoint, rayHeading, 0.01); //: 4
        auto intersectedPoints = quadtree.RayIntersectedAll(rayBasePoint, rayHeading, 0.01); //: { 4, 2, 3 } in distance order!
        auto sqrt2 = sqrtf(2.0);
        auto sqrt2Reciproc = 1.0f / sqrt2;
        auto boxesInFrustum = quadtree.FrustumCulling( // Cross-point of the planes: 2;2;2
          vector{
            plane_t{2.0 * sqrt2, point_t{ sqrt2Reciproc, sqrt2Reciproc }},
            plane_t{        2.0,                  point_t{ 0.0f, -1.0f }}
        },
          0.01);

        // Collect all IDs in breadth/depth first order
        auto entityIDsInDFS = quadtree.CollectAllEntitiesInBFS();
        auto entityIDsInBFS = quadtree.CollectAllEntitiesInDFS();

        Assert::IsTrue(std::ranges::is_permutation(
          vector<std::pair<EntityID, EntityID>>{
            {1, 4},
            {2, 4}
        },
          collidingIDPairs));
        Assert::IsTrue(std::ranges::is_permutation(vector<EntityID>{ 1, 2, 4 }, insideBoxIDs));
        Assert::IsTrue(std::ranges::is_permutation(vector<EntityID>{ 1, 2, 3, 4 }, overlappingBoxIDs));
        Assert::IsTrue(std::ranges::is_permutation(vector<EntityID>{ 2, 4 }, pickedIDs));
        Assert::IsTrue(firstIntersectedBox.has_value());
        Assert::AreEqual(EntityID(4), *firstIntersectedBox);

        Assert::IsTrue(std::ranges::is_permutation(vector<EntityID>{ 1, 2, 4 }, boxesInFrustum));

        Assert::IsTrue(vector<EntityID>{ 4, 2, 3 } == intersectedPoints);
        Assert::IsTrue(vector<EntityID>{ 4, 0, 1, 2, 3 } == entityIDsInDFS);
        Assert::IsTrue(vector<EntityID>{ 4, 0, 1, 2, 3 } == entityIDsInBFS);
      }
    };
  }

}