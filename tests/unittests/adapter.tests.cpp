#include "pch.h"

#define ORTHOTREE__PUBLIC_NODE_INTERFACE
#define ORTHOTREE__DEBUG_CHECKS
#include "orthotree/bvh.h"
#include "orthotree/octree.h"

#include "orthotree/adapters/boost.h"
#include <boost/geometry.hpp>

#ifdef _MSC_VER
#pragma warning(disable : 4127)
#pragma warning(disable : 5054)
#endif

#include "orthotree/adapters/eigen.h"
#include <Eigen/Dense>
#include <Eigen/Geometry>
#ifdef _MSC_VER
#pragma warning(default : 4127)
#pragma warning(default : 5054)
#endif

#include "orthotree/adapters/xyz.h"

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
#include "orthotree/adapters/unreal.h"

#include <functional>

using namespace OrthoTree;

using std::array;
using std::vector;

namespace CustomGeometryType
{
  // User-defined geometrical objects

  struct MyPoint2D
  {
    float x;
    float y;
  };
  using MyBox2D = std::array<MyPoint2D, 2>;
  using MyRay2D = std::array<MyPoint2D, 2>;
  struct MyPlane2D
  {
    float OrigoDistance;
    MyPoint2D Normal;
  };


  // Adapter

  struct AdaptorBasicsCustom
  {
    using Scalar = float;
    using FloatScalar = float;
    using Vector = MyPoint2D;
    using Box = MyBox2D;
    using Ray = MyBox2D;
    using Plane = MyPlane2D;

    static constexpr dim_t DIMENSION_NO = 2;
    static constexpr FloatScalar BASE_TOLERANCE = std::numeric_limits<FloatScalar>::epsilon() * FloatScalar(10);

    static constexpr MyPoint2D MakePoint() noexcept { return {}; };
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
      case 0: pt.x = v; break;
      case 1: pt.y = v; break;
      default: assert(false);
      }
    }

    static constexpr MyBox2D MakeBox() noexcept { return {}; };
    static void SetBoxMinC(MyBox2D& box, dim_t i, float v) { SetPointC(box[0], i, v); }
    static void SetBoxMaxC(MyBox2D& box, dim_t i, float v) { SetPointC(box[1], i, v); }
    static float GetBoxMinC(MyBox2D const& box, dim_t i) { return GetPointC(box[0], i); }
    static float GetBoxMaxC(MyBox2D const& box, dim_t i) { return GetPointC(box[1], i); }

    static MyPoint2D const& GetRayDirection(MyRay2D const& ray) { return ray[1]; }
    static MyPoint2D const& GetRayOrigin(MyRay2D const& ray) { return ray[0]; }

    static MyPoint2D const& GetPlaneNormal(MyPlane2D const& plane) { return plane.Normal; }
    static float GetPlaneOrigoDistance(MyPlane2D const& plane) { return plane.OrigoDistance; }
  };

  using CustomGeometryAdapter = OrthoTree::GeneralGeometryAdapter<AdaptorBasicsCustom>;


  // Tailored Quadtree objects

  using QuadtreePointCustom = OrthoTree::OrthoTreeBase<PointEntitySpanAdapter<MyPoint2D>, CustomGeometryAdapter, PointConfiguration<>>;

  using QuadtreeBoxCustom = OrthoTree::OrthoTreeBase<BoxEntitySpanAdapter<MyBox2D>, CustomGeometryAdapter, BoxConfiguration<true>>;
} // namespace CustomGeometryType


namespace AdaptorTest
{
  using namespace CustomGeometryType;

  static bool AreEqualAlmost(MyBox2D const& l, MyBox2D const& r) noexcept
  {
    for (auto const iMax : { 0, 1 })
    {
      if (std::abs(l[iMax].x - r[iMax].x) > 1e-5f)
        return false;
      if (std::abs(l[iMax].y - r[iMax].y) > 1e-5f)
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
      Assert::IsTrue(tree.IsNodeEntitiesEmpty(tree.GetRootNodeValue()));

      auto const& box = tree.GetBox();
      Assert::IsTrue(AreEqualAlmost(box, MyBox2D{}));
    } // namespace AdaptorTest

    TEST_METHOD(Insert_NonLeaf_Successful)
    {
      auto constexpr vpt = array{
        MyPoint2D{ 0.0, 0.0 },
        MyPoint2D{ 1.0, 1.0 },
        MyPoint2D{ 2.0, 2.0 },
        MyPoint2D{ 3.0, 3.0 }
      };
      auto tree = QuadtreePointCustom(vpt, 3, std::nullopt, 1);

      Assert::IsTrue(tree.InsertIntoLeaf(4, MyPoint2D{ 2.5, 2.5 }, InsertionMode::ExistingLeaf));

      auto const& nodes = tree.GetNodes();
      Assert::AreEqual<size_t>(7, nodes.size());
      Assert::IsTrue(
        AreContainersItemsEqual(tree.GetNodeEntities(tree.GetNodeValue(detail::MortonSpaceIndexing<2, 20>::GetNodeID(15, 2))), vector<size_t>{ 3, 4 }));
    }

    TEST_METHOD(Contains__1__True)
    {
      auto constexpr vpt = array{
        MyPoint2D{ 0.0, 0.0 },
        MyPoint2D{ 1.0, 1.0 },
        MyPoint2D{ 2.0, 2.0 },
        MyPoint2D{ 3.0, 3.0 }
      };
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
        using EntityID = Eigen::OctreePoint3d::EntityID;

        auto vpt = vector{
          Eigen::Vector3d(0.0, 0.0, 0.0), Eigen::Vector3d(1.0, 1.0, 1.0), Eigen::Vector3d(2.0, 2.0, 2.0), Eigen::Vector3d(3.0, 3.0, 3.0),
          Eigen::Vector3d(4.0, 4.0, 4.0), Eigen::Vector3d(0.0, 0.0, 4.0), Eigen::Vector3d(0.0, 4.0, 0.0), Eigen::Vector3d(4.0, 0.0, 0.0),
          Eigen::Vector3d(1.5, 1.5, 1.0), Eigen::Vector3d(1.0, 1.5, 1.5),
        };
        auto tree = Eigen::OctreePoint3d(vpt, 3, std::nullopt, 1);

        auto entityIDsInBFS = tree.GetEntitiesBreadthFirst();
        auto entityIDsInDFS = tree.GetEntitiesDepthFirst();

        auto searchBox = Eigen::AlignedBox3d(Eigen::Vector3d(0.0, 0.0, 0.0), Eigen::Vector3d(2.0, 2.0, 2.0));
        auto pointsInSearchBox = tree.RangeSearch(searchBox, vpt);

        auto sqrt3Reciproc = 1.0 / sqrt(3.0);
        auto pointsInPlane = tree.PlaneSearch(2.6, Eigen::Vector3d(sqrt3Reciproc, sqrt3Reciproc, sqrt3Reciproc), vpt, 0.3);

        auto sqrt2 = sqrt(2.0);
        auto sqrt2Reciproc = 1.0 / sqrt2;
        vector<Eigen::Hyperplane<double, 3>> planes;
        planes.emplace_back(Eigen::Vector3d(sqrt2Reciproc, sqrt2Reciproc, 0.0), -2 * sqrt2);
        planes.emplace_back(Eigen::Vector3d(0.0, sqrt2Reciproc, sqrt2Reciproc), -2 * sqrt2);
        planes.emplace_back(Eigen::Vector3d(sqrt2Reciproc, -sqrt2Reciproc, 0.0), 0);

        auto pointsInFrustum = tree.FrustumCulling(planes, vpt, 0.01);

        auto const n = EntityID(vpt.size());
        vpt.push_back(Eigen::Vector3d(1.0, 1.0, 1.5));
        tree.InsertIntoLeaf(n, vpt.back());
        tree.Erase(0, vpt[0]);
        vpt.erase(vpt.begin());
        auto entityIDsInDFS_AfterErase = tree.GetEntitiesDepthFirst();

        auto searchPoint = Eigen::Vector3d(1.0, 1.0, 1.0);
        auto entityIDsKNN = tree.GetNearestNeighbors(searchPoint, 3, vpt);

        Assert::IsTrue(std::ranges::is_permutation(vector<EntityID>{ 0, 1, 2, 8, 9 }, pointsInSearchBox));
        Assert::IsTrue(std::ranges::is_permutation(vector<EntityID>{ 5, 6, 7, 8, 9 }, pointsInPlane));
        Assert::IsTrue(std::ranges::is_permutation(vector<EntityID>{ 0, 9, 7, 8 }, entityIDsKNN));
        Assert::IsTrue(std::ranges::is_permutation(vector<EntityID>{ 2, 3, 4 }, pointsInFrustum));

        Assert::IsTrue(vector<EntityID>{ 7, 6, 5, 0, 2, 1, 8, 9, 3, 4 } == entityIDsInBFS);
        Assert::IsTrue(vector<EntityID>{ 0, 1, 8, 9, 7, 6, 5, 2, 3, 4 } == entityIDsInDFS);
        Assert::IsTrue(vector<EntityID>{ 0, 7, 9, 8, 6, 5, 4, 1, 2, 3 } == entityIDsInDFS_AfterErase);
      } // namespace EigenAdaptorTest

      TEST_METHOD(BoxGeneral2DC_Example2)
      {
        using EntityID = Eigen::QuadtreeBoxM2d::EntityID;

        auto boxes = std::vector{ Eigen::AlignedBox2d(Eigen::Vector2d(0.0, 0.0), Eigen::Vector2d(1.0, 1.0)),
                                  Eigen::AlignedBox2d(Eigen::Vector2d(1.0, 1.0), Eigen::Vector2d(2.0, 2.0)),
                                  Eigen::AlignedBox2d(Eigen::Vector2d(2.0, 2.0), Eigen::Vector2d(3.0, 3.0)),
                                  Eigen::AlignedBox2d(Eigen::Vector2d(3.0, 3.0), Eigen::Vector2d(4.0, 4.0)),
                                  Eigen::AlignedBox2d(Eigen::Vector2d(1.2, 1.2), Eigen::Vector2d(2.8, 2.8)) };

        auto quadtree = Eigen::QuadtreeBoxM2d(
          boxes,
          3 // max depth
          ,
          std::nullopt // user-provided bounding Box for all
          ,
          1 // max element in a node
          ,
          false // parallel calculation option
        );

        auto collidingIDPairs = quadtree.CollisionDetection(); //: { {1,4}, {2,4} }

        auto searchBox = Eigen::AlignedBox2d(Eigen::Vector2d(1.0, 1.0), Eigen::Vector2d(3.1, 3.1));

        // Boxes within the range
        auto insideBoxIDs = quadtree.RangeSearch(searchBox); //: { 1, 2, 4 }

        // Overlapping Boxes with the range
        auto overlappingBoxIDs = quadtree.RangeSearch(searchBox, RangeSearchMode::Overlap); //: { 1, 2, 3, 4 }

        // Picked boxes
        auto pickPoint = Eigen::Vector2d(2.5, 2.5);
        auto pickedIDs = quadtree.PickSearch(pickPoint); //: { 2, 4 }

        // Ray intersections
        auto rayBasePoint = Eigen::Vector2d(1.5, 2.5);
        auto rayHeading = Eigen::Vector2d(3.0 / std::sqrt(10.0), 1.0 / std::sqrt(10.0));
        auto firstIntersectedBoxes = quadtree.RayIntersectedFirst(rayBasePoint, rayHeading, 0.01); //: {2, 4}
        auto intersectedPoints = quadtree.RayIntersectedAll(rayBasePoint, rayHeading, 0.01);       //: { 4, 2, 3 } in distance order!

        // Plane
        auto sqrt2 = sqrt(2.0);
        auto sqrt2Reciproc = 1.0 / sqrt2;
        auto boxesInFrustum = quadtree.FrustumCulling( // Cross-point of the planes: 2;2;2
          vector{
            Eigen::Hyperplane<double, 2>(Eigen::Vector2d(sqrt2Reciproc, sqrt2Reciproc), -2 * sqrt2),
            Eigen::Hyperplane<double, 2>(Eigen::Vector2d(0, -1.0), 2.0)
        },
          0.01f);

        // Collect all IDs in breadth/depth first order
        auto entityIDsInDFS = quadtree.GetEntitiesBreadthFirst();
        auto entityIDsInBFS = quadtree.GetEntitiesDepthFirst();

        Assert::IsTrue(
          std::ranges::is_permutation(
            vector<std::pair<EntityID, EntityID>>{
              { 1, 4 },
              { 2, 4 }
        },
            collidingIDPairs));
        Assert::IsTrue(std::ranges::is_permutation(vector<EntityID>{ 1, 2, 4 }, insideBoxIDs));
        Assert::IsTrue(std::ranges::is_permutation(vector<EntityID>{ 1, 2, 3, 4 }, overlappingBoxIDs));
        Assert::IsTrue(std::ranges::is_permutation(vector<EntityID>{ 2, 4 }, pickedIDs));
        Assert::AreEqual<size_t>(2, firstIntersectedBoxes.size());
        Assert::AreEqual(EntityID(2), firstIntersectedBoxes[0]);
        Assert::AreEqual(EntityID(4), firstIntersectedBoxes[1]);

        Assert::IsTrue(std::ranges::is_permutation(vector<EntityID>{ 1, 2, 4 }, boxesInFrustum));

        Assert::IsTrue(vector<EntityID>{ 4, 2, 3 } == intersectedPoints);
        Assert::IsTrue(vector<EntityID>{ 4, 0, 1, 2, 3 } == entityIDsInDFS);
        Assert::IsTrue(vector<EntityID>{ 0, 1, 4, 2, 3 } == entityIDsInBFS);
      }
    };
  } // namespace EigenAdaptorTest


  namespace XYZAdaptorTest
  {
    using namespace BasicTypesXYZ;

    TEST_CLASS(XYZTest)
    {
    public:
      TEST_METHOD(PointGeneral3D)
      {
        using EntityID = XYZ::OctreePoint::EntityID;

        auto vpt = vector{
          BasicTypesXYZ::Point3D(0.0, 0.0, 0.0), BasicTypesXYZ::Point3D(1.0, 1.0, 1.0), BasicTypesXYZ::Point3D(2.0, 2.0, 2.0),
          BasicTypesXYZ::Point3D(3.0, 3.0, 3.0), BasicTypesXYZ::Point3D(4.0, 4.0, 4.0), BasicTypesXYZ::Point3D(0.0, 0.0, 4.0),
          BasicTypesXYZ::Point3D(0.0, 4.0, 0.0), BasicTypesXYZ::Point3D(4.0, 0.0, 0.0), BasicTypesXYZ::Point3D(1.5, 1.5, 1.0),
          BasicTypesXYZ::Point3D(1.0, 1.5, 1.5),
        };
        auto tree = XYZ::OctreePoint(vpt, 3, std::nullopt, 1);

        auto entityIDsInBFS = tree.GetEntitiesBreadthFirst();
        auto entityIDsInDFS = tree.GetEntitiesDepthFirst();

        auto searchBox = BasicTypesXYZ::BoundingBox3D{
          BasicTypesXYZ::Point3D{ 0.0, 0.0, 0.0 },
          BasicTypesXYZ::Point3D{ 2.0, 2.0, 2.0 }
        };
        auto pointsInSearchBox = tree.RangeSearch(searchBox, vpt);

        auto sqrt3Reciproc = BasicTypesXYZ::float_t(1.0 / sqrt(3.0));
        auto pointsInPlane = tree.PlaneSearch(2.6f, BasicTypesXYZ::Point3D{ sqrt3Reciproc, sqrt3Reciproc, sqrt3Reciproc }, vpt, 0.3f);

        auto sqrt2 = float_t(sqrt(2.0));
        auto sqrt2Reciproc = float_t(1.0 / sqrt2);

        auto pointsInFrustum = tree.FrustumCulling( // Cross-point of the planes: 2;2;2
          vector<BasicTypesXYZ::Plane3D>{
            {2 * sqrt2,  { sqrt2Reciproc, sqrt2Reciproc, 0.0 }},
            {2 * sqrt2,  { 0.0, sqrt2Reciproc, sqrt2Reciproc }},
            {      0.0, { sqrt2Reciproc, -sqrt2Reciproc, 0.0 }}
        },
          vpt,
          0.01f);

        auto const n = EntityID(vpt.size());
        vpt.push_back(BasicTypesXYZ::Point3D(1.0, 1.0, 1.5));
        tree.InsertIntoLeaf(n, vpt.back());
        tree.Erase(0, vpt[0]);
        vpt.erase(vpt.begin());
        auto entityIDsInDFS_AfterErase = tree.GetEntitiesDepthFirst();

        auto searchPoint = BasicTypesXYZ::Point3D(1.0, 1.0, 1.0);
        auto entityIDsKNN = tree.GetNearestNeighbors(searchPoint, 3, vpt);

        Assert::IsTrue(std::ranges::is_permutation(vector<EntityID>{ 0, 1, 2, 8, 9 }, pointsInSearchBox));
        Assert::IsTrue(std::ranges::is_permutation(vector<EntityID>{ 5, 6, 7, 8, 9 }, pointsInPlane));
        Assert::IsTrue(std::ranges::is_permutation(vector<EntityID>{ 0, 9, 7, 8 }, entityIDsKNN));
        Assert::IsTrue(std::ranges::is_permutation(vector<EntityID>{ 2, 3, 4 }, pointsInFrustum));

        Assert::IsTrue(vector<EntityID>{ 7, 6, 5, 0, 2, 1, 8, 9, 3, 4 } == entityIDsInBFS);
        Assert::IsTrue(vector<EntityID>{ 0, 1, 8, 9, 7, 6, 5, 2, 3, 4 } == entityIDsInDFS);
        Assert::IsTrue(vector<EntityID>{ 0, 7, 9, 8, 6, 5, 4, 1, 2, 3 } == entityIDsInDFS_AfterErase);
      } // namespace XYZAdaptorTest

      TEST_METHOD(BoxGeneral2DC_Example2)
      {
        using EntityID = XYZ::QuadtreeBoxM::EntityID;

        auto boxes = std::vector{
          BasicTypesXYZ::BoundingBox2D{ BasicTypesXYZ::Point2D{ 0.0f, 0.0f }, BasicTypesXYZ::Point2D{ 1.0f, 1.0f } },
          BasicTypesXYZ::BoundingBox2D{ BasicTypesXYZ::Point2D{ 1.0f, 1.0f }, BasicTypesXYZ::Point2D{ 2.0f, 2.0f } },
          BasicTypesXYZ::BoundingBox2D{ BasicTypesXYZ::Point2D{ 2.0f, 2.0f }, BasicTypesXYZ::Point2D{ 3.0f, 3.0f } },
          BasicTypesXYZ::BoundingBox2D{ BasicTypesXYZ::Point2D{ 3.0f, 3.0f }, BasicTypesXYZ::Point2D{ 4.0f, 4.0f } },
          BasicTypesXYZ::BoundingBox2D{ BasicTypesXYZ::Point2D{ 1.2f, 1.2f }, BasicTypesXYZ::Point2D{ 2.8f, 2.8f } }
        };

        auto quadtree = XYZ::QuadtreeBoxM(
          boxes,
          3 // max depth
          ,
          std::nullopt // user-provided bounding Box for all
          ,
          1 // max element in a node
          ,
          false // parallel calculation option
        );

        auto collidingIDPairs = quadtree.CollisionDetection(); //: { {1,4}, {2,4} }

        auto searchBox = BasicTypesXYZ::BoundingBox2D{
          BasicTypesXYZ::Point2D{ 1.0f, 1.0f },
          BasicTypesXYZ::Point2D{ 3.1f, 3.1f }
        };

        // Boxes within the range
        auto insideBoxIDs = quadtree.RangeSearch(searchBox); //: { 1, 2, 4 }

        // Overlapping Boxes with the range

        auto overlappingBoxIDs = quadtree.RangeSearch(searchBox, RangeSearchMode::Overlap); //: { 1, 2, 3, 4 }

        // Picked boxes
        auto pickPoint = BasicTypesXYZ::Point2D{ 2.5f, 2.5f };
        auto pickedIDs = quadtree.PickSearch(pickPoint); //: { 2, 4 }

        // Ray intersections
        auto rayBasePoint = BasicTypesXYZ::Point2D{ 1.5f, 2.5f };
        auto rayHeading = BasicTypesXYZ::Point2D{ 3.0f / std::sqrtf(10.0f), 1.0f / std::sqrt(10.0f) };
        auto firstIntersectedBoxes = quadtree.RayIntersectedFirst(rayBasePoint, rayHeading, 0.01f); //: { 2, 4 }
        auto intersectedPoints = quadtree.RayIntersectedAll(rayBasePoint, rayHeading, 0.01f);       //: { 2, 3, 4 } in distance order!

        // Plane
        auto sqrt2 = sqrtf(2.0);
        auto sqrt2Reciproc = 1.0f / sqrt2;
        auto boxesInFrustum = quadtree.FrustumCulling( // Cross-point of the planes: 2;2;2
          vector<BasicTypesXYZ::Plane2D>{
            {2 * sqrt2, { sqrt2Reciproc, sqrt2Reciproc }},
            {     -2.0f,                  { 0.0f, -1.0f }}
        },
          0.01f);

        // Collect all IDs in breadth/depth first order
        auto entityIDsInBFS = quadtree.GetEntitiesBreadthFirst();
        auto entityIDsInDFS = quadtree.GetEntitiesDepthFirst();

        Assert::IsTrue(
          std::ranges::is_permutation(
            vector<std::pair<EntityID, EntityID>>{
              { 1, 4 },
              { 2, 4 }
        },
            collidingIDPairs));
        Assert::IsTrue(std::ranges::is_permutation(vector<EntityID>{ 1, 2, 4 }, insideBoxIDs));
        Assert::IsTrue(std::ranges::is_permutation(vector<EntityID>{ 1, 2, 3, 4 }, overlappingBoxIDs));
        Assert::IsTrue(std::ranges::is_permutation(vector<EntityID>{ 2, 4 }, pickedIDs));
        Assert::AreEqual<size_t>(2, firstIntersectedBoxes.size());
        Assert::AreEqual(EntityID(2), firstIntersectedBoxes[0]);
        Assert::AreEqual(EntityID(4), firstIntersectedBoxes[1]);

        Assert::IsTrue(std::ranges::is_permutation(vector<EntityID>{ 1, 2, 4 }, boxesInFrustum));

        Assert::IsTrue(vector<EntityID>{ 4, 2, 3 } == intersectedPoints);
        Assert::IsTrue(vector<EntityID>{ 4, 0, 1, 2, 3 } == entityIDsInBFS);
        Assert::IsTrue(vector<EntityID>{ 0, 1, 4, 2, 3 } == entityIDsInDFS);
      }
    };
  } // namespace XYZAdaptorTest


  namespace GameEngineAdaptorTest
  {
    // 1. Non-default constructible geometry
    struct RigidPoint
    {
      float x, y;
      RigidPoint() = delete;
      constexpr RigidPoint(float x, float y)
      : x(x)
      , y(y)
      {}
    };

    struct RigidBox
    {
      RigidPoint Min, Max;
      RigidBox() = delete;
      constexpr RigidBox(RigidPoint min, RigidPoint max)
      : Min(min)
      , Max(max)
      {}
    };

    // 2. Abstract Move-Only Entity
    struct GameObjectID
    {
      int teamId;
      int unitId;
      auto operator<=>(GameObjectID const&) const = default;
    };

    struct GameObjectIDHash
    {
      std::size_t operator()(GameObjectID const& id) const noexcept { return std::hash<int>()(id.teamId) ^ (std::hash<int>()(id.unitId) << 1); }
    };

    class IGameObject
    {
    public:
      virtual ~IGameObject() = default;
      virtual RigidBox const& GetBounds() const = 0;
    };

    class Enemy : public IGameObject
    {
      RigidBox bounds;
      // +visibility, etc

    public:
      Enemy(RigidBox b)
      : bounds(std::move(b))
      {}
      Enemy(Enemy const&) = delete;
      Enemy& operator=(Enemy const&) = delete;
      Enemy(Enemy&&) = default;
      Enemy& operator=(Enemy&&) = default;

      virtual RigidBox const& GetBounds() const override { return bounds; }
    };

    // 3. Adapter for the geometry
    struct CustomRigidGeometryAdapter
    {
      using Scalar = float;
      using FloatScalar = float;
      using Vector = RigidPoint;
      using Box = RigidBox;
      struct Ray
      {
        RigidPoint Origin;
        RigidPoint Direction;
      };
      struct Plane
      {
        float OrigoDistance;
        RigidPoint Normal;
      };

      static constexpr OrthoTree::dim_t DIMENSION_NO = 2;
      static constexpr FloatScalar BASE_TOLERANCE = std::numeric_limits<FloatScalar>::epsilon() * FloatScalar(10);

      static constexpr Vector MakePoint() noexcept { return RigidPoint(0.0f, 0.0f); };
      static constexpr Box MakeBox() noexcept { return RigidBox(RigidPoint(0.0f, 0.0f), RigidPoint(0.0f, 0.0f)); };

      static constexpr float GetPointC(RigidPoint const& pt, OrthoTree::dim_t i) noexcept { return i == 0 ? pt.x : pt.y; }
      static constexpr void SetPointC(RigidPoint& pt, OrthoTree::dim_t i, float v) noexcept
      {
        if (i == 0)
          pt.x = v;
        else
          pt.y = v;
      }

      static constexpr void SetBoxMinC(RigidBox& box, OrthoTree::dim_t i, float v) noexcept { SetPointC(box.Min, i, v); }
      static constexpr void SetBoxMaxC(RigidBox& box, OrthoTree::dim_t i, float v) noexcept { SetPointC(box.Max, i, v); }
      static constexpr float GetBoxMinC(RigidBox const& box, OrthoTree::dim_t i) noexcept { return GetPointC(box.Min, i); }
      static constexpr float GetBoxMaxC(RigidBox const& box, OrthoTree::dim_t i) noexcept { return GetPointC(box.Max, i); }

      static constexpr Vector const& GetRayDirection(Ray const& ray) noexcept { return ray.Direction; }
      static constexpr Vector const& GetRayOrigin(Ray const& ray) noexcept { return ray.Origin; }

      static constexpr Vector const& GetPlaneNormal(Plane const& plane) noexcept { return plane.Normal; }
      static constexpr Scalar GetPlaneOrigoDistance(Plane const& plane) noexcept { return plane.OrigoDistance; }
    };

    using CustomGeometryAdapter = OrthoTree::GeneralGeometryAdapter<CustomRigidGeometryAdapter>;

    // 4. EntityAdapter for std::unique_ptr<IGameObject>
    using GameObjectContainer = std::unordered_map<GameObjectID, std::unique_ptr<IGameObject>, GameObjectIDHash>;
    using GameObjectContainerView = GameObjectContainer const&;

    struct GameObjectEntityAdapter
    {
      using EntityContainer = GameObjectContainer;
      using EntityContainerView = GameObjectContainerView;
      using Entity = std::pair<GameObjectID const, std::unique_ptr<IGameObject>>;
      using EntityID = GameObjectID;
      using Geometry = RigidBox;
      using Hash = GameObjectIDHash;

      static constexpr OrthoTree::GeometryType GEOMETRY_TYPE = OrthoTree::GeometryType::Box;
      static constexpr OrthoTree::EntityIdStrategy ENTITY_ID_STRATEGY = OrthoTree::EntityIdStrategy::EntityKeyed;

      static Entity const& GetEntity(EntityContainerView entities, EntityID const& entityID) noexcept { return *entities.find(entityID); }
      static constexpr EntityID GetEntityID(Entity const& entity) noexcept { return entity.first; }
      static constexpr EntityID GetEntityID(EntityContainerView /*entities*/, Entity const& entity) noexcept { return entity.first; }

      static Geometry const& GetGeometry(EntityContainerView entities, EntityID const& entityID) noexcept
      {
        return entities.at(entityID)->GetBounds();
      }
      static constexpr Geometry const& GetGeometry(Entity const& entity) noexcept { return entity.second->GetBounds(); }

      static std::size_t GetEntityCount(EntityContainerView entities) noexcept { return entities.size(); }

      static EntityID Insert(EntityContainer& entities, Entity&& entity) noexcept
      {
        auto [it, _] = entities.emplace(std::move(entity));
        return it->first;
      }

      static void Erase(EntityContainer& entities, EntityID entityID) noexcept { entities.erase(entityID); }

      static auto Exchange(EntityContainer& entities, EntityID entityID, Entity&& entity) noexcept
      {
        auto it = entities.find(entityID);
        return Entity{ entityID, std::exchange(it->second, std::forward<decltype(entity.second)>(entity.second)) };
      }

      static void Clear(EntityContainer& entities) noexcept { entities.clear(); }

      static bool Contains(EntityContainer& entities, EntityID entityID) noexcept { return entities.contains(entityID); }
    };

    // 5. Quadtree
    using QuadtreeGameObjects = OrthoTree::OrthoTreeBase<GameObjectEntityAdapter, CustomGeometryAdapter, OrthoTree::BoxConfiguration<true>>;
    using QuadtreeGameObjectsM = OrthoTreeManaged<QuadtreeGameObjects>;

    TEST_CLASS(GameEngineAdaptorTest)
    {
    public:
      TEST_METHOD(EnemyObjects)
      {
        auto objects = GameObjectContainer();
        objects.emplace(
          GameObjectID{
            1, 1
        },
          std::make_unique<Enemy>(RigidBox{ RigidPoint{ 0.0f, 0.0f }, RigidPoint{ 1.0f, 1.0f } }));
        objects.emplace(
          GameObjectID{
            1, 2
        },
          std::make_unique<Enemy>(RigidBox{ RigidPoint{ 1.0f, 1.0f }, RigidPoint{ 2.0f, 2.0f } }));
        objects.emplace(
          GameObjectID{
            2, 1
        },
          std::make_unique<Enemy>(RigidBox{ RigidPoint{ 2.0f, 2.0f }, RigidPoint{ 3.0f, 3.0f } }));
        objects.emplace(
          GameObjectID{
            2, 2
        },
          std::make_unique<Enemy>(RigidBox{ RigidPoint{ 3.0f, 3.0f }, RigidPoint{ 4.0f, 4.0f } }));
        objects.emplace(
          GameObjectID{
            3, 1
        },
          std::make_unique<Enemy>(RigidBox{ RigidPoint{ 1.2f, 1.2f }, RigidPoint{ 2.8f, 2.8f } }));

        auto const& view = objects;
        auto tree = QuadtreeGameObjects(view, 3, std::nullopt, 1, false);

        auto searchBox = RigidBox(RigidPoint(1.0f, 1.0f), RigidPoint(3.1f, 3.1f));
        auto insideBoxIDs = tree.RangeSearch(searchBox, view);

        Assert::AreEqual<size_t>(3, insideBoxIDs.size());

        // OrthoTreeManaged with move semantics!
        auto treeManaged = QuadtreeGameObjectsM(std::move(objects), 3, std::nullopt, 1, false);
        Assert::AreEqual<size_t>(0, objects.size()); // should be moved!

        treeManaged.Add(
          {
            GameObjectID{                        4,                        1 },
            std::make_unique<Enemy>(RigidBox{ RigidPoint{ 1.5f, 1.5f }, RigidPoint{ 1.6f, 1.6f } }
            )
        },
          OrthoTree::InsertionMode::ExistingLeaf);

        auto overlappingIDs = treeManaged.RangeSearch(searchBox, OrthoTree::RangeSearchMode::Overlap);
        auto expectedIDs = std::vector<GameObjectID>{
          { 1, 2 },
          { 2, 1 },
          { 2, 2 },
          { 3, 1 },
          { 4, 1 }
        };
        Assert::IsTrue(std::ranges::is_permutation(expectedIDs, overlappingIDs));
      }
    };
  } // namespace GameEngineAdaptorTest

  namespace UnrealAdaptorTest
  {
    using namespace UnrealDummyTypes;

    TEST_CLASS(UnrealTest)
    {
    public:
      TEST_METHOD(PointGeneral3D)
      {
        using EntityID = FOctreePoint::EntityID;

        auto vpt = vector{
          FVector(0.0, 0.0, 0.0), FVector(1.0, 1.0, 1.0), FVector(2.0, 2.0, 2.0), FVector(3.0, 3.0, 3.0), FVector(4.0, 4.0, 4.0),
          FVector(0.0, 0.0, 4.0), FVector(0.0, 4.0, 0.0), FVector(4.0, 0.0, 0.0), FVector(1.5, 1.5, 1.0), FVector(1.0, 1.5, 1.5),
        };
        auto tree = FOctreePoint(vpt, 3, std::nullopt, 1);

        auto entityIDsInBFS = tree.GetEntitiesBreadthFirst();
        auto entityIDsInDFS = tree.GetEntitiesDepthFirst();

        auto searchBox = FBox(FVector(0.0, 0.0, 0.0), FVector(2.0, 2.0, 2.0));
        auto pointsInSearchBox = tree.RangeSearch(searchBox, vpt);

        auto sqrt3Reciproc = 1.0 / sqrt(3.0);
        auto pointsInPlane = tree.PlaneSearch(2.6, FVector(sqrt3Reciproc, sqrt3Reciproc, sqrt3Reciproc), vpt, 0.3);

        auto const n = EntityID(vpt.size());
        vpt.push_back(FVector(1.0, 1.0, 1.5));
        tree.InsertIntoLeaf(n, vpt.back());
        tree.Erase(0, vpt[0]);
        auto entityIDsInDFS_AfterErase = tree.GetEntitiesDepthFirst();

        auto searchPoint = FVector{ 1.0, 1.0, 1.0 };
        auto entityIDsKNN = tree.GetNearestNeighbors(searchPoint, 3, vpt);
      } // namespace UnrealAdaptorTest

      TEST_METHOD(BoxGeneral2DC_Example2)
      {
        auto boxes = std::vector{ FBox2D(FVector2D(0.0, 0.0), FVector2D(1.0, 1.0)),
                                  FBox2D(FVector2D(1.0, 1.0), FVector2D(2.0, 2.0)),
                                  FBox2D(FVector2D(2.0, 2.0), FVector2D(3.0, 3.0)),
                                  FBox2D(FVector2D(3.0, 3.0), FVector2D(4.0, 4.0)),
                                  FBox2D(FVector2D(1.2, 1.2), FVector2D(2.8, 2.8)) };

        auto quadtree = FQuadtreeBox2DC(
          boxes,
          3 // max depth
          ,
          std::nullopt // user-provided bounding Box for all
          ,
          1 // max element in a node
          ,
          false // parallel calculation option
        );

        auto collidingIDPairs = quadtree.CollisionDetection(); //: { {1,4}, {2,4} }

        auto searchBox = FBox2D(FVector2D(1.0, 1.0), FVector2D(3.1, 3.1));

        // Boxes within the range
        auto insideBoxIDs = quadtree.RangeSearch(searchBox); //: { 1, 2, 4 }

        // Overlapping Boxes with the range

        auto overlappingBoxIDs = quadtree.RangeSearch(searchBox, RangeSearchMode::Overlap); //: { 1, 2, 3, 4 }

        // Picked boxes
        auto pickPoint = FVector2D(2.5, 2.5);
        auto pickedIDs = quadtree.PickSearch(pickPoint); //: { 2, 4 }

        // Ray intersections
        auto rayBasePoint = FVector2D(1.5, 2.5);
        auto rayHeading = FVector2D(3.0 / std::sqrt(10.0), 1.0 / std::sqrt(10.0));
        auto firstIntersectedBox = quadtree.RayIntersectedFirst(rayBasePoint, rayHeading, 0.01); //: 4
        auto intersectedPoints = quadtree.RayIntersectedAll(rayBasePoint, rayHeading, 0.01);     //: { 4, 2, 3 } in distance order!

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
        auto entityIDsInDFS = quadtree.GetEntitiesBreadthFirst();
        auto entityIDsInBFS = quadtree.GetEntitiesDepthFirst();
      }
    };
  } // namespace UnrealAdaptorTest

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
        using EntityID = boost::geometry::octree_point::EntityID;

        auto vpt = vector{
          point_t(0.0, 0.0, 0.0), point_t(1.0, 1.0, 1.0), point_t(2.0, 2.0, 2.0), point_t(3.0, 3.0, 3.0), point_t(4.0, 4.0, 4.0),
          point_t(0.0, 0.0, 4.0), point_t(0.0, 4.0, 0.0), point_t(4.0, 0.0, 0.0), point_t(1.5, 1.5, 1.0), point_t(1.0, 1.5, 1.5),
        };
        auto tree = boost::geometry::octree_point(vpt, 3, std::nullopt, 1);

        auto entityIDsInBFS = tree.GetEntitiesBreadthFirst();
        auto entityIDsInDFS = tree.GetEntitiesDepthFirst();

        auto searchBox = box_t(point_t(0.0, 0.0, 0.0), point_t(2.0, 2.0, 2.0));
        auto pointsInSearchBox = tree.RangeSearch(searchBox, vpt);

        auto sqrt3Reciproc = 1.0 / sqrt(3.0);
        auto pointsInPlane = tree.PlaneSearch(2.6, point_t(sqrt3Reciproc, sqrt3Reciproc, sqrt3Reciproc), vpt, 0.3);

        auto sqrt2 = float_t(sqrt(2.0));
        auto sqrt2Reciproc = float_t(1.0 / sqrt2);

        auto pointsInFrustum = tree.FrustumCulling( // Cross-point of the planes: 2;2;2
          vector<plane_t>{
            {2 * sqrt2,  { sqrt2Reciproc, sqrt2Reciproc, 0.0 }},
            {2 * sqrt2,  { 0.0, sqrt2Reciproc, sqrt2Reciproc }},
            {      0.0, { sqrt2Reciproc, -sqrt2Reciproc, 0.0 }}
        },
          vpt,
          0.01f);

        auto const n = EntityID(vpt.size());
        vpt.push_back(point_t(1.0, 1.0, 1.5));
        tree.InsertIntoLeaf(n, vpt.back());
        tree.Erase(0, vpt[0]);
        vpt.erase(vpt.begin());
        auto entityIDsInDFS_AfterErase = tree.GetEntitiesDepthFirst();

        auto searchPoint = point_t{ 1.0, 1.0, 1.0 };
        auto entityIDsKNN = tree.GetNearestNeighbors(searchPoint, 3, vpt);

        Assert::IsTrue(std::ranges::is_permutation(vector<EntityID>{ 0, 1, 2, 8, 9 }, pointsInSearchBox));
        Assert::IsTrue(std::ranges::is_permutation(vector<EntityID>{ 5, 6, 7, 8, 9 }, pointsInPlane));
        Assert::IsTrue(std::ranges::is_permutation(vector<EntityID>{ 0, 9, 7, 8 }, entityIDsKNN));
        Assert::IsTrue(std::ranges::is_permutation(vector<EntityID>{ 2, 3, 4 }, pointsInFrustum));

        Assert::IsTrue(vector<EntityID>{ 7, 6, 5, 0, 2, 1, 8, 9, 3, 4 } == entityIDsInBFS);
        Assert::IsTrue(vector<EntityID>{ 0, 1, 8, 9, 7, 6, 5, 2, 3, 4 } == entityIDsInDFS);
        Assert::IsTrue(vector<EntityID>{ 0, 7, 9, 8, 6, 5, 4, 1, 2, 3 } == entityIDsInDFS_AfterErase);
      } // namespace BoostAdaptorTest

      TEST_METHOD(BoxGeneral2DC_Example2)
      {
        using point_t = model::pointNd_t<2, double>;
        using box_t = model::boxNd_t<2, double>;
        using plane_t = model::planeNd_t<2, double>;
        using EntityID = boost::geometry::quadtree_box_m::EntityID;

        auto boxes = std::vector{ box_t(point_t(0.0, 0.0), point_t(1.0, 1.0)),
                                  box_t(point_t(1.0, 1.0), point_t(2.0, 2.0)),
                                  box_t(point_t(2.0, 2.0), point_t(3.0, 3.0)),
                                  box_t(point_t(3.0, 3.0), point_t(4.0, 4.0)),
                                  box_t(point_t(1.2, 1.2), point_t(2.8, 2.8)) };

        auto quadtree = boost::geometry::quadtree_box_m(
          boxes,
          3 // max depth
          ,
          std::nullopt // user-provided bounding Box for all
          ,
          1 // max element in a node
          ,
          false // parallel calculation option
        );

        auto collidingIDPairs = quadtree.CollisionDetection(); //: { {1,4}, {2,4} }

        auto searchBox = box_t(point_t(1.0, 1.0), point_t(3.1, 3.1));

        // Boxes within the range
        auto insideBoxIDs = quadtree.RangeSearch(searchBox); //: { 1, 2, 4 }

        // Overlapping Boxes with the range

        auto overlappingBoxIDs = quadtree.RangeSearch(searchBox, RangeSearchMode::Overlap); //: { 1, 2, 3, 4 }

        // Picked boxes
        auto pickPoint = point_t(2.5, 2.5);
        auto pickedIDs = quadtree.PickSearch(pickPoint); //: { 2, 4 }

        // Ray intersections
        auto rayBasePoint = point_t(1.5, 2.5);
        auto rayHeading = point_t(3.0 / std::sqrt(10.0), 1.0 / std::sqrt(10.0));
        auto firstIntersectedBoxes = quadtree.RayIntersectedFirst(rayBasePoint, rayHeading, 0.01); //: {2, 4}
        auto intersectedPoints = quadtree.RayIntersectedAll(rayBasePoint, rayHeading, 0.01);       //: { 2, 3, 4 } in distance order!
        auto sqrt2 = sqrtf(2.0);
        auto sqrt2Reciproc = 1.0f / sqrt2;
        auto boxesInFrustum = quadtree.FrustumCulling( // Cross-point of the planes: 2;2;2
          vector{
            plane_t{2.0 * sqrt2, point_t{ sqrt2Reciproc, sqrt2Reciproc }},
            plane_t{        -2.0,                  point_t{ 0.0f, -1.0f }}
        },
          0.01);

        // Collect all IDs in breadth/depth first order
        auto entityIDsInDFS = quadtree.GetEntitiesBreadthFirst();
        auto entityIDsInBFS = quadtree.GetEntitiesDepthFirst();

        Assert::IsTrue(
          std::ranges::is_permutation(
            vector<std::pair<EntityID, EntityID>>{
              { 1, 4 },
              { 2, 4 }
        },
            collidingIDPairs));
        Assert::IsTrue(std::ranges::is_permutation(vector<EntityID>{ 1, 2, 4 }, insideBoxIDs));
        Assert::IsTrue(std::ranges::is_permutation(vector<EntityID>{ 1, 2, 3, 4 }, overlappingBoxIDs));
        Assert::IsTrue(std::ranges::is_permutation(vector<EntityID>{ 2, 4 }, pickedIDs));
        Assert::AreEqual<size_t>(2, firstIntersectedBoxes.size());
        Assert::AreEqual(EntityID(2), firstIntersectedBoxes[0]);
        Assert::AreEqual(EntityID(4), firstIntersectedBoxes[1]);

        Assert::IsTrue(std::ranges::is_permutation(vector<EntityID>{ 1, 2, 4 }, boxesInFrustum));

        Assert::IsTrue(vector<EntityID>{ 4, 2, 3 } == intersectedPoints);
        Assert::IsTrue(vector<EntityID>{ 4, 0, 1, 2, 3 } == entityIDsInDFS);
        Assert::IsTrue(vector<EntityID>{ 0, 1, 4, 2, 3 } == entityIDsInBFS);
      }
    };
  } // namespace BoostAdaptorTest
} // namespace AdaptorTest
