#include "pch.h"

// clang-format off
#include "../octree.h"
#include "../octree_container.h"

// Boost
#include <boost/geometry.hpp>
#include "../adaptor.boost.h"

// CGAL
#include <CGAL/Bbox_2.h>
#include <CGAL/Bbox_3.h>
#include <CGAL/Cartesian.h>
#include <CGAL/Origin.h>
#include <CGAL/Plane_3.h>
#include <CGAL/Point_2.h>
#include <CGAL/Point_3.h>
#include <CGAL/Ray_2.h>
#include <CGAL/Ray_3.h>
#include <CGAL/basic.h>
#include <CGAL/cartesian.h>
#include "../adaptor.cgal.h"

// Eigen
#include <Eigen/Dense>
#include <Eigen/Geometry>
#include "../adaptor.eigen.h"

// glm
#include <glm/glm.hpp>
#include "../adaptor.glm.h"

// XYZ
#include "../adaptor.xyz.h"

// Unreal Engine
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

// clang-format on


#define EXPECT_EQ_UNORDERED(val1, val2) EXPECT_TRUE(std::ranges::is_permutation(val1, val2))
namespace
{
  // TODO: corePointTest2D
  // TODO: corePointTest4D
  // TODO: coreBoxTest2D
  // TODO: coreBoxTest3D

  // TODO: containerPointTest2D
  // TODO: containerPointTest3D
  // TODO: containerBoxTest3D


  template<int DIMENSION_NO, typename TVector, typename TOrthoTreeA>
  void vectorConv(TVector const& pointO, typename TOrthoTreeA::TVector& pointA)
  {
    for (int dimID = 0; dimID < DIMENSION_NO; ++dimID)
      TOrthoTreeA::AD::SetPointC(pointA, dimID, pointO[dimID]);
  }

  template<int DIMENSION_NO, typename TBox, typename TOrthoTreeA>
  void boxConv(TBox const& boxO, typename TOrthoTreeA::TBox& boxA)
  {
    for (int dimID = 0; dimID < DIMENSION_NO; ++dimID)
    {
      TOrthoTreeA::AD::SetBoxMinC(boxA, dimID, boxO.Min[dimID]);
      TOrthoTreeA::AD::SetBoxMaxC(boxA, dimID, boxO.Max[dimID]);
    }
  }


  // Boost

  template<int DIMENSION_NO, typename TRay, typename TVector, typename TOrthoTreeA>
  void rayConv(TRay const& rayO, typename boost::geometry::model::rayNd_t<DIMENSION_NO, typename TOrthoTreeA::TGeometry>& rayA)
  {
    vectorConv<DIMENSION_NO, TVector, TOrthoTreeA>(rayO.Origin, rayA.origin);
    vectorConv<DIMENSION_NO, TVector, TOrthoTreeA>(rayO.Direction, rayA.direction);
  }

  template<int DIMENSION_NO, typename TPlane, typename TVector, typename TOrthoTreeA>
  void planeConv(TPlane const& planeO, typename boost::geometry::model::planeNd_t<DIMENSION_NO, typename TOrthoTreeA::TGeometry>& planeA)
  {
    vectorConv<DIMENSION_NO, TVector, TOrthoTreeA>(planeO.Normal, planeA.normal);
    planeA.origo_distance = typename TOrthoTreeA::TGeometry(planeO.OrigoDistance);
  }

  
  // CGAL

  template<int DIMENSION_NO, typename TRay, typename TVector, typename TOrthoTreeA>
  void rayConv(TRay const& rayO, CGAL::Ray_2<CGAL::Cartesian<double>>& rayA)
  {
    using namespace CGAL;

    auto const s = Point_2<Cartesian<double>>(rayO.Origin[0], rayO.Origin[1]);
    auto const v = Vector_2<Cartesian<double>>(rayO.Direction[0], rayO.Direction[1]);
    rayA = Ray_2<Cartesian<double>>(s, v);
  }

  template<int DIMENSION_NO, typename TPlane, typename TVector, typename TOrthoTreeA>
  void planeConv(TPlane const& planeO, CGAL::Plane_2& planeA)
  {
    vectorConv<2, TVector, TOrthoTreeA>(planeO.Normal, planeA.normal);
    planeA.offset = -typename TOrthoTreeA::TGeometry(planeO.OrigoDistance);
  }

  template<int DIMENSION_NO, typename TRay, typename TVector, typename TOrthoTreeA>
  void rayConv(TRay const& rayO, CGAL::Ray_3<CGAL::Cartesian<double>>& rayA)
  {
    using namespace CGAL;

    auto const s = Point_3<Cartesian<double>>(rayO.origin[0], rayO.origin[1], rayO.origin[2]);
    auto const v = Vector_3<Cartesian<double>>(rayO.direction[0], rayO.direction[1], rayO.direction[2]);
    rayA = Ray_2<Cartesian<double>>(s, v);
  }

  template<int DIMENSION_NO, typename TPlane, typename TVector, typename TOrthoTreeA>
  void planeConv(TPlane const& planeO, CGAL::Plane_3<CGAL::Cartesian<double>>& planeA)
  {
    planeA = CGAL::Plane_3<CGAL::Cartesian<double>>(planeO.Normal[0], planeO.Normal[1], planeO.Normal[2], -planeO.OrigoDistance);
  }

  // Eigen

  template<int DIMENSION_NO, typename TRay, typename TVector, typename TOrthoTreeA>
  void rayConv(TRay const& rayO, Eigen::ParametrizedLine<typename TOrthoTreeA::TGeometry, DIMENSION_NO>& rayA)
  {
    vectorConv<DIMENSION_NO, TVector, TOrthoTreeA>(rayO.Origin, rayA.origin());
    vectorConv<DIMENSION_NO, TVector, TOrthoTreeA>(rayO.Direction, rayA.direction());
  }

  template<int DIMENSION_NO, typename TPlane, typename TVector, typename TOrthoTreeA>
  void planeConv(TPlane const& planeO, typename Eigen::Hyperplane<typename TOrthoTreeA::TGeometry, DIMENSION_NO, 0>& planeA)
  {
    typename TOrthoTreeA::TVector normalA;
    vectorConv<DIMENSION_NO, TVector, TOrthoTreeA>(planeO.Normal, normalA);
    planeA = Eigen::Hyperplane<typename TOrthoTreeA::TGeometry, DIMENSION_NO>(normalA, typename TOrthoTreeA::TGeometry(-planeO.OrigoDistance));
  }


  // glm

  template<int DIMENSION_NO, typename TRay, typename TVector, typename TOrthoTreeA>
  void rayConv(TRay const& rayO, typename glm::rayNd_t<DIMENSION_NO, typename TOrthoTreeA::TGeometry>& rayA)
  {
    vectorConv<DIMENSION_NO, TVector, TOrthoTreeA>(rayO.Origin, rayA.origin);
    vectorConv<DIMENSION_NO, TVector, TOrthoTreeA>(rayO.Direction, rayA.direction);
  }

  template<int DIMENSION_NO, typename TPlane, typename TVector, typename TOrthoTreeA>
  void planeConv(TPlane const& planeO, typename glm::planeNd_t<DIMENSION_NO, typename TOrthoTreeA::TGeometry>& planeA)
  {
    vectorConv<DIMENSION_NO, TVector, TOrthoTreeA>(planeO.Normal, planeA.normal);
    planeA.origo_distance = typename TOrthoTreeA::TGeometry(planeO.OrigoDistance);
  }


  // Unreal

  template<int DIMENSION_NO, typename TRay, typename TVector, typename TOrthoTreeA>
  void rayConv(TRay const& rayO, FQuadtreePoint::TRay& rayA)
  {
    vectorConv<DIMENSION_NO, TVector, TOrthoTreeA>(rayO.Origin, rayA.Origin);
    vectorConv<DIMENSION_NO, TVector, TOrthoTreeA>(rayO.Direction, rayA.Direction);
  }

  template<int DIMENSION_NO, typename TPlane, typename TVector, typename TOrthoTreeA>
  void planeConv(TPlane const& planeO, FQuadtreePoint::TPlane& planeA)
  {
    vectorConv<DIMENSION_NO, TVector, TOrthoTreeA>(planeO.Normal, planeA.Direction);
    planeA.Origin = {};
  }


  template<int DIMENSION_NO, typename TRay, typename TVector, typename TOrthoTreeA>
  void rayConv(TRay const& rayO, FRay& rayA)
  {
    vectorConv<DIMENSION_NO, TVector, TOrthoTreeA>(rayO.Origin, rayA.Origin);
    vectorConv<DIMENSION_NO, TVector, TOrthoTreeA>(rayO.Direction, rayA.Direction);
  }

  template<int DIMENSION_NO, typename TPlane, typename TVector, typename TOrthoTreeA>
  void planeConv(TPlane const& planeO, FPlane& planeA)
  {
    vectorConv<DIMENSION_NO, TVector, TOrthoTreeA>(planeO.Normal, planeA);
    planeA.W = float(planeO.OrigoDistance);
  }


  // XYZ

  template<int DIMENSION_NO, typename TRay, typename TVector, typename TOrthoTreeA>
  void rayConv(TRay const& rayO, BasicTypesXYZ::Ray2D& rayA)
  {
    vectorConv<DIMENSION_NO, TVector, TOrthoTreeA>(rayO.Origin, rayA.BasePoint);
    vectorConv<DIMENSION_NO, TVector, TOrthoTreeA>(rayO.Direction, rayA.Heading);
  }

  template<int DIMENSION_NO, typename TPlane, typename TVector, typename TOrthoTreeA>
  void planeConv(TPlane const& planeO, BasicTypesXYZ::Plane2D& planeA)
  {
    vectorConv<DIMENSION_NO, TVector, TOrthoTreeA>(planeO.Normal, planeA.Normal);
    planeA.OrigoDistance = typename TOrthoTreeA::TGeometry(planeO.OrigoDistance);
  }

  template<int DIMENSION_NO, typename TRay, typename TVector, typename TOrthoTreeA>
  void rayConv(TRay const& rayO, BasicTypesXYZ::Ray3D& rayA)
  {
    vectorConv<DIMENSION_NO, TVector, TOrthoTreeA>(rayO.Origin, rayA.BasePoint);
    vectorConv<DIMENSION_NO, TVector, TOrthoTreeA>(rayO.Direction, rayA.Heading);
  }

  template<int DIMENSION_NO, typename TPlane, typename TVector, typename TOrthoTreeA>
  void planeConv(TPlane const& planeO, BasicTypesXYZ::Plane3D& planeA)
  {
    vectorConv<DIMENSION_NO, TVector, TOrthoTreeA>(planeO.Normal, planeA.Normal);
    planeA.OrigoDistance = typename TOrthoTreeA::TGeometry(planeO.OrigoDistance);
  }


  template<typename TOrthoTreeA>
  struct PointTestInputT
  {
    using TGeometry = typename TOrthoTreeA::TGeometry;
    using TVector = typename TOrthoTreeA::TVector;
    using TBox = typename TOrthoTreeA::TBox;
    using TRay = typename TOrthoTreeA::TRay;
    using TPlane = typename TOrthoTreeA::TPlane;

    std::vector<TVector> points;
    TVector pointOfkNN;
    TBox searchBox;
    TPlane searchPlane;
    std::vector<TPlane> frustumPlanes;
  };

  template<typename TPointTestInput, typename TOrthoTreeA>
  void corePointTest3D_Input(TPointTestInput& testdata)
  {
    using namespace OrthoTree;
    auto constexpr DIMENSION_NO = 3;

    using GeometryA = typename TOrthoTreeA::TGeometry;

    using Vector = VectorND<DIMENSION_NO, GeometryA>;
    using Box = BoundingBoxND<DIMENSION_NO, GeometryA>;
    using Plane = PlaneND<DIMENSION_NO, GeometryA>;

    auto const sqrt3Reciproc = GeometryA(1.0 / std::sqrt(3.0));
    auto const sqrt2 = GeometryA(sqrt(2.0));
    auto const sqrt2Reciproc = GeometryA(1.0 / sqrt2);

    auto const points = std::vector{
      Vector{0.0, 0.0, 0.0},
      Vector{1.0, 1.0, 1.0},
      Vector{2.0, 2.0, 2.0},
      Vector{3.0, 3.0, 3.0},
      Vector{4.0, 4.0, 4.0},
      Vector{0.0, 0.0, 4.0},
      Vector{0.0, 4.0, 0.0},
      Vector{4.0, 0.0, 0.0},
      Vector{1.5, 1.5, 1.0},
      Vector{1.0, 1.5, 1.5},
      Vector{1.0, 1.0, 1.5}
    };

    auto constexpr searchBox = Box{
      Vector{0.0, 0.0, 0.0},
      Vector{2.0, 2.0, 2.0}
    };

    auto const searchPlane = Plane{
      GeometryA(2.6), Vector{sqrt3Reciproc, sqrt3Reciproc, sqrt3Reciproc}
    };
    auto const pointOfkNN = Vector{ 1.0, 1.0, 1.0 };

    // Cross-point of the planes: 2;2;2
    auto const frustumPlanes = std::vector<Plane>{

      {2 * sqrt2,  { sqrt2Reciproc, sqrt2Reciproc, 0.0 }},
      {2 * sqrt2,  { 0.0, sqrt2Reciproc, sqrt2Reciproc }},
      {      0.0, { sqrt2Reciproc, -sqrt2Reciproc, 0.0 }}
    };

    std::size_t pointNo = points.size();
    testdata.points.resize(pointNo);
    for (size_t pointID = 0; pointID < pointNo; ++pointID)
      vectorConv<DIMENSION_NO, Vector, TOrthoTreeA>(points[pointID], testdata.points[pointID]);

    boxConv<DIMENSION_NO, Box, TOrthoTreeA>(searchBox, testdata.searchBox);
    vectorConv<DIMENSION_NO, Vector, TOrthoTreeA>(pointOfkNN, testdata.pointOfkNN);
    planeConv<DIMENSION_NO, Plane, Vector, TOrthoTreeA>(searchPlane, testdata.searchPlane);

    testdata.frustumPlanes.resize(frustumPlanes.size());
    for (size_t planeID = 0; planeID < frustumPlanes.size(); ++planeID)
      planeConv<DIMENSION_NO, Plane, Vector, TOrthoTreeA>(frustumPlanes[planeID], testdata.frustumPlanes[planeID]);
  }

  template<typename TPointTestInput, typename TOrthoTreeA>
  void corePointTest3D_Check(TPointTestInput& testdata, bool doCheck)
  {
    using AD = typename TOrthoTreeA::AD;
    using GeometryA = typename TOrthoTreeA::TGeometry;

    auto const& [points, pointOfkNN, searchBox, searchPlane, frustumPlanes] = testdata;

    auto const pointNo = points.size();
    auto const points_span = std::span(points.begin(), points.end() - 1);

    auto tree = TOrthoTreeA(points_span, 3, std::nullopt, 2);

    auto const entityIDsInBFSActual = tree.CollectAllIdInBFS(tree.GetRootKey());
    auto const entityIDsInBFSExpected = std::vector<std::size_t>{ 7, 6, 5, 0, 2, 1, 8, 9, 3, 4 };

    auto const entityIDsInDFSActual = tree.CollectAllIdInDFS(tree.GetRootKey());
    auto const entityIDsInDFSExpected = std::vector<std::size_t>{ 0, 1, 8, 9, 7, 6, 5, 2, 3, 4 };

    auto const pointsInSearchBoxActual = tree.RangeSearch(searchBox, points);
    auto const pointsInSearchBoxExpected = std::vector<std::size_t>{ 0, 1, 2, 8, 9 };

    auto const pointsInPlaneActual = tree.PlaneSearch(AD::GetPlaneOrigoDistance(searchPlane), AD::GetPlaneNormal(searchPlane), GeometryA(0.3), points);
    auto const pointsInPlaneExpected = std::vector<std::size_t>{ 5, 6, 7, 8, 9 };

    auto const pointsInFrustumActual = tree.FrustumCulling(frustumPlanes, GeometryA(0.01), points);
    auto const pointsInFrustumExpected = std::vector<std::size_t>{ 2, 3, 4, 6, 7 };

    tree.Insert(pointNo - 1, points.back());
    tree.template Erase<false>(0, points[0]);
    auto const entityIDsInDFS_AfterErase_Actual = tree.CollectAllIdInDFS();
    auto const entityIDsInDFS_AfterErase_Expected = std::vector<std::size_t>{ 1, 8, 10, 9, 7, 6, 5, 2, 3, 4 };

    auto const entityIDsKNNActual = tree.GetNearestNeighbors(pointOfkNN, 3, points);
    auto const entityIDsKNNExpected = std::vector<std::size_t>{ 1, 10, 8 };

    if (!doCheck)
      return;

    EXPECT_EQ(entityIDsInBFSExpected, entityIDsInBFSActual);
    EXPECT_EQ(entityIDsInDFSExpected, entityIDsInDFSActual);
    EXPECT_EQ_UNORDERED(pointsInSearchBoxExpected, pointsInSearchBoxActual);
    EXPECT_EQ_UNORDERED(pointsInPlaneExpected, pointsInPlaneActual);
    EXPECT_EQ_UNORDERED(pointsInFrustumExpected, pointsInFrustumActual);
    EXPECT_EQ(entityIDsInDFS_AfterErase_Expected, entityIDsInDFS_AfterErase_Actual);
    EXPECT_EQ_UNORDERED(entityIDsKNNExpected, entityIDsKNNActual);
  }

  template<typename TOrthoTreeA>
  static void corePointTest3D(bool doCheck = true) noexcept
  {
    using PointTestInput = PointTestInputT<TOrthoTreeA>;
    PointTestInput testdata;
    corePointTest3D_Input<PointTestInput, TOrthoTreeA>(testdata);
    corePointTest3D_Check<PointTestInput, TOrthoTreeA>(testdata, doCheck);
  }


  template<typename TOrthoTreeA>
  struct BoxTestInputT
  {
    using TGeometry = typename TOrthoTreeA::TGeometry;
    using TVector = typename TOrthoTreeA::TVector;
    using TBox = typename TOrthoTreeA::TBox;
    using TRay = typename TOrthoTreeA::TRay;
    using TPlane = typename TOrthoTreeA::TPlane;

    std::vector<TBox> boxes;
    TVector pickPoint;
    TBox searchBox;
    TRay ray;
    TPlane searchPlane;
    std::vector<TPlane> frustumPlanes;
  };

  template<typename TBoxTestInput, typename TOrthoTreeA>
  void coreBoxTest2D_Input(TBoxTestInput& testdata)
  {
    using namespace OrthoTree;
    auto constexpr DIMENSION_NO = 2;

    using GeometryA = typename TOrthoTreeA::TGeometry;

    using Vector = VectorND<DIMENSION_NO, GeometryA>;
    using Box = BoundingBoxND<DIMENSION_NO, GeometryA>;
    using Ray = RayND<DIMENSION_NO, GeometryA>;
    using Plane = PlaneND<DIMENSION_NO, GeometryA>;

    auto const sqrt2 = GeometryA(sqrt(2.0));
    auto const sqrt2Reciproc = GeometryA(1.0 / sqrt2);

    auto const boxes = std::vector{
      Box{Vector{ GeometryA{ 0.0 }, GeometryA{ 0.0 } }, Vector{ GeometryA{ 1.0 }, GeometryA{ 1.0 } }},
      Box{Vector{ GeometryA{ 1.0 }, GeometryA{ 1.0 } }, Vector{ GeometryA{ 2.0 }, GeometryA{ 2.0 } }},
      Box{Vector{ GeometryA{ 2.0 }, GeometryA{ 2.0 } }, Vector{ GeometryA{ 3.0 }, GeometryA{ 3.0 } }},
      Box{Vector{ GeometryA{ 3.0 }, GeometryA{ 3.0 } }, Vector{ GeometryA{ 4.0 }, GeometryA{ 4.0 } }},
      Box{Vector{ GeometryA{ 1.2 }, GeometryA{ 1.2 } }, Vector{ GeometryA{ 2.8 }, GeometryA{ 2.8 } }}
    };

    auto const pickPoint = Vector{ 2.5, 2.5 };
    auto const searchBox = Box{
      Vector{GeometryA{ 1.0 }, GeometryA{ 1.0 }},
      Vector{GeometryA{ 3.1 }, GeometryA{ 3.1 }}
    };
    auto const ray = Ray{
      Vector{GeometryA{ 1.5 }, GeometryA{ 2.5 }},
      Vector{GeometryA{ 1.5 }, GeometryA{ 0.5 }}
    };
    auto const frustumPlanes = std::vector{
      Plane{GeometryA(2.0 * sqrt2),    Vector{ sqrt2Reciproc, sqrt2Reciproc }},
      Plane{        GeometryA(2.0), Vector{ GeometryA(0.0), GeometryA(-1.0) }}
    };

    std::size_t boxNo = boxes.size();
    testdata.boxes.resize(boxNo);
    for (size_t boxID = 0; boxID < boxNo; ++boxID)
      boxConv<DIMENSION_NO, Box, TOrthoTreeA>(boxes[boxID], testdata.boxes[boxID]);

    boxConv<DIMENSION_NO, Box, TOrthoTreeA>(searchBox, testdata.searchBox);
    vectorConv<DIMENSION_NO, Vector, TOrthoTreeA>(pickPoint, testdata.pickPoint);
    rayConv<DIMENSION_NO, Ray, Vector, TOrthoTreeA>(ray, testdata.ray);

    testdata.frustumPlanes.resize(frustumPlanes.size());
    for (size_t planeID = 0; planeID < frustumPlanes.size(); ++planeID)
      planeConv<DIMENSION_NO, Plane, Vector, TOrthoTreeA>(frustumPlanes[planeID], testdata.frustumPlanes[planeID]);
  }

  template<typename TBoxTestInput, typename TOrthoTreeA>
  void containerBoxTest2D_Check(TBoxTestInput& testdata, bool doCheck)
  {
    using AD = typename TOrthoTreeA::AD;
    using GeometryA = typename TOrthoTreeA::TGeometry;

    auto const& [boxes, pickPoint, searchBox, ray, searchPlane, frustumPlanes] = testdata;

    auto quadtree = TOrthoTreeA(
      boxes,
      3,            // max depth
      std::nullopt, // user-provided bounding Box for all
      2,            // max element in a node
      false         // parallel calculation option
    );

    // Collision detection
    auto const collidingIDPairsActual = quadtree.CollisionDetection(); //: { {1,4}, {2,4} }
    auto const collidingIDPairsExpected = std::vector<std::pair<std::size_t, std::size_t>>{
      {1, 4},
      {2, 4}
    };

    // Boxes within the range
    auto const insideBoxIDsActual = quadtree.RangeSearch(searchBox); //: { 1, 2, 4 }
    auto const insideBoxIDsExpected = std::vector<std::size_t>{ 1, 2, 4 };

    // Overlapping Boxes with the range
    constexpr bool shouldFullyContain = false;                                                // overlap is enough
    auto const overlappingBoxIDsActual = quadtree.template RangeSearch<shouldFullyContain>(searchBox); //: { 1, 2, 3, 4 }
    auto const overlappingBoxIDsExpected = std::vector<std::size_t>{ 1, 2, 3, 4 };

    auto const pickedIDsActual = quadtree.PickSearch(pickPoint); //: { 2, 4 }
    auto const pickedIDsExpected = std::vector<std::size_t>{ 2, 4 };

    auto const firstIntersectedBox = quadtree.RayIntersectedFirst(AD::GetRayOrigin(ray), AD::GetRayDirection(ray), GeometryA(0.01)); //: 4

    auto const intersectedPointsActual =
      quadtree.RayIntersectedAll(AD::GetRayOrigin(ray), AD::GetRayDirection(ray), GeometryA(0.01)); //: { 4, 2, 3 } in distance order!
    auto const intersectedPointsExpected = std::vector<std::size_t>{ 4, 2, 3 };

    auto const boxesInFrustumActual = quadtree.FrustumCulling(frustumPlanes, GeometryA(0.01));
    auto const boxesInFrustumExpected = std::vector<std::size_t>{ 1, 2, 4 };

    auto const entityIDsInDFSActual = quadtree.CollectAllIdInBFS();
    auto const entityIDsInDFSExpected = std::vector<std::size_t>{ 4, 0, 1, 2, 3 };

    auto const entityIDsInBFSActual = quadtree.CollectAllIdInDFS();
    auto const entityIDsInBFSExpected = std::vector<std::size_t>{ 4, 0, 1, 2, 3 };

    if (!doCheck)
      return;

    EXPECT_EQ_UNORDERED(collidingIDPairsExpected, collidingIDPairsActual);
    EXPECT_EQ_UNORDERED(insideBoxIDsExpected, insideBoxIDsActual);
    EXPECT_EQ_UNORDERED(overlappingBoxIDsExpected, overlappingBoxIDsActual);
    EXPECT_EQ_UNORDERED(pickedIDsExpected, pickedIDsActual);
    ASSERT_TRUE(firstIntersectedBox.has_value());
    EXPECT_EQ(std::size_t(4), *firstIntersectedBox);
    EXPECT_EQ(intersectedPointsExpected, intersectedPointsActual);
    EXPECT_EQ_UNORDERED(boxesInFrustumExpected, boxesInFrustumActual);
    EXPECT_EQ(entityIDsInDFSExpected, entityIDsInDFSActual);
    EXPECT_EQ(entityIDsInBFSExpected, entityIDsInBFSActual);

    // TODO: Plane intersection, Add, Erase
  }

  template<typename TOrthoTreeA>
  static void containerBoxTest2D(bool doCheck = true) noexcept
  {
    using BoxTestInput = BoxTestInputT<TOrthoTreeA>;
    BoxTestInput testdata;
    coreBoxTest2D_Input<BoxTestInput, TOrthoTreeA>(testdata);
    containerBoxTest2D_Check<BoxTestInput, TOrthoTreeA>(testdata, doCheck);
  }

} // namespace


namespace XYZAdapter
{
  TEST(XYZ_CoreTest, Point3D)
  {
    corePointTest3D<XYZ::OctreePoint>();
  }

  TEST(XYZ_ContainerTest, Box2D)
  {
    containerBoxTest2D<XYZ::QuadtreeBoxC>();
  }
} // namespace XYZAdapter

namespace BoostAdapter
{
  TEST(Boost_CoreTest, Point3D)
  {
    corePointTest3D<boost::geometry::octree_point>();
  }

  TEST(Boost_ContainerTest, Box2D)
  {
    containerBoxTest2D<boost::geometry::quadtree_box_c>();
  }
} // namespace BoostAdapter

namespace CGALAdapter
{
  TEST(CGAL_CoreTest, Point3D)
  {
    corePointTest3D<CGAL::OctreePoint>();
  }

  TEST(CGAL_ContainerTest, Box2D)
  {
    containerBoxTest2D<CGAL::QuadtreeBoxC>();
  }
} // namespace CGALAdapter

namespace EigenAdapter
{
  TEST(Eigen_CoreTest, Point3D)
  {
    corePointTest3D<Eigen::OctreePoint3d>();
  }

  TEST(Eigen_ContainerTest, Box2D)
  {
    containerBoxTest2D<Eigen::QuadtreeBoxC2d>();
  }
} // namespace EigenAdapter


namespace glmAdapter
{
  TEST(glm_CoreTest, Point3D)
  {
    corePointTest3D<glm::octree_point>();
  }

  TEST(glm_ContainerTest, Box2D)
  {
    containerBoxTest2D<glm::quadtree_box_c>();
  }
} // namespace glmAdapter

namespace UnrealAdapter
{
  TEST(Unreal_CoreTest, Point3D)
  {
    corePointTest3D<FOctreePoint>(false);
  }

  TEST(Unreal_ContainerTest, Box2D)
  {
    containerBoxTest2D<FQuadtreeBoxC>(false);
  }
} // namespace UnrealAdapter
