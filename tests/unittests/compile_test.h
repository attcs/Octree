#ifndef ORTHOTREE_COMPILE_TEST_GUARD
#define ORTHOTREE_COMPILE_TEST_GUARD

#include <array>
#include <vector>

#include "../../include/orthotree/octree.h"

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-variable"
#endif

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

// Enforce the compilation of all template function

template<OrthoTree::dim_t N, typename TExecMode, OrthoTree::NodeGeometryStorage NODE_GEOMETRY_STORAGE>
void testCompilePoint()
{
  using Point = OrthoTree::PointND<N>;
  using BoundingBox = OrthoTree::BoundingBoxND<N>;
  using Plane = OrthoTree::PlaneND<N>;

  using OT = OrthoTree::OrthoTreePointND<N, OrthoTree::BaseGeometryType, true, NODE_GEOMETRY_STORAGE>;

  auto constexpr vpt = std::array{ Point{ 0.0 }, Point{ 1.0 }, Point{ 2.0 }, Point{ 3.0 }, Point{ 4.0 } };

  auto constexpr boxes = std::array{
    BoundingBox{ { 0.0, 0.0 }, { 1.0, 1.0 } },
    BoundingBox{ { 1.0, 1.0 }, { 2.0, 2.0 } },
    BoundingBox{ { 2.0, 2.0 }, { 3.0, 3.0 } },
    BoundingBox{ { 3.0, 3.0 }, { 4.0, 4.0 } },
    BoundingBox{ { 1.2, 1.2 }, { 2.8, 2.8 } }
  };

  auto tree = OT(TExecMode{}, vpt, 3, std::nullopt, 2);

  // const member functions
  {
    [[maybe_unused]] auto const boxAll = tree.GetBox();
    [[maybe_unused]] auto const nDepthMax = tree.GetDepthNo();
    [[maybe_unused]] auto const idBoxesIntersectedAll = tree.RayIntersectedAll({}, { 3.0 / 5.0, 4.0 / 5.0 }, vpt, 0);
    [[maybe_unused]] auto const idBoxesIntersectedFirst = tree.RayIntersectedFirst({}, { 3.0 / 5.0, 4.0 / 5.0 }, vpt, 0);

    [[maybe_unused]] auto const aidBoxesInRange = tree.RangeSearch(boxes[0], vpt);
    [[maybe_unused]] auto const aidBoxesInRangeF = tree.RangeSearch(boxes[0], vpt, OrthoTree::RangeSearchMode::Overlap);
    [[maybe_unused]] auto const aidBoxesInRangeT = tree.RangeSearch(boxes[0], vpt, OrthoTree::RangeSearchMode::Inside);
    [[maybe_unused]] auto const aidPtsInPlane = tree.PlaneSearch(0.0, Point{ 1.0 }, vpt, 0.01);
    [[maybe_unused]] auto const idPlaneIntersected = tree.PlaneSearch(1.0, { 1.0, 0.0 }, vpt, 0.0);
    [[maybe_unused]] auto const idPlaneIntersectedP = tree.PlaneSearch(
      {
        1.0, { 1.0, 0.0 }
    },
      vpt,
      0.0);
    [[maybe_unused]] auto const idPlanePosSeg = tree.PlanePositiveSegmentation(1.0, { 1.0, 0.0 }, vpt, 0.0);
    [[maybe_unused]] auto const idPlanePosSegP = tree.PlanePositiveSegmentation(
      {
        1.0, { 1.0, 0.0 }
    },
      vpt,
      0.0);
    [[maybe_unused]] auto const idFrustum = tree.FrustumCulling(
      std::vector{
        Plane{ 1.0, { 1.0, 0.0 } }
    },
      vpt,
      0.0);
    [[maybe_unused]] auto const kNN = tree.GetNearestNeighbors({}, 2, vpt);
  }

  // non-const member functions
  {
    tree.Erase(4);
    tree.Erase(3, vpt[3]);
    tree.InsertIntoLeaf(3, vpt[3]);
    tree.Update(2, vpt[2], vpt[3]);
    tree.Update(3, vpt[3], vpt[4]);
    tree.UpdateIndexes(
      {
        { 1, 3 },
        { 3, 1 },
    });
    tree.Move({ 1.0, 1.0 }, TExecMode{});
    tree.Clear();
    tree.Reset();

    tree.Init(boxes[0], 3, 12);
    tree.Reset();

    OT::Create(tree, vpt, 0, std::nullopt, 2, TExecMode{});
  }
}

template<OrthoTree::dim_t N, typename TExecMode, OrthoTree::NodeGeometryStorage NODE_GEOMETRY_STORAGE>
void testCompilePointMap()
{
  using Point = OrthoTree::PointND<N>;
  using BoundingBox = OrthoTree::BoundingBoxND<N>;
  using Plane = OrthoTree::PlaneND<N>;
  using Map = std::unordered_map<int, Point>;

  using OT = OrthoTree::OrthoTreePointNDUD<N, OrthoTree::BaseGeometryType, Map, NODE_GEOMETRY_STORAGE>;

  auto const vpt = Map{
    { 10, Point{ 0.0 } },
    { 20, Point{ 1.0 } },
    { 30, Point{ 2.0 } },
    { 11, Point{ 3.0 } },
    { 12, Point{ 4.0 } }
  };

  auto const boxes = std::array{
    BoundingBox{ { 0.0, 0.0 }, { 1.0, 1.0 } },
    BoundingBox{ { 1.0, 1.0 }, { 2.0, 2.0 } },
    BoundingBox{ { 2.0, 2.0 }, { 3.0, 3.0 } },
    BoundingBox{ { 3.0, 3.0 }, { 4.0, 4.0 } },
    BoundingBox{ { 1.2, 1.2 }, { 2.8, 2.8 } }
  };

  auto tree = OT(TExecMode{}, vpt, 3, std::nullopt, 2);

  // const member functions
  {
    [[maybe_unused]] auto const boxAll = tree.GetBox();
    [[maybe_unused]] auto const nDepthMax = tree.GetDepthNo();
    [[maybe_unused]] auto const idBoxesIntersectedAll = tree.RayIntersectedAll({}, { 3.0 / 5.0, 4.0 / 5.0 }, vpt, 0);
    [[maybe_unused]] auto const idBoxesIntersectedFirst = tree.RayIntersectedFirst({}, { 3.0 / 5.0, 4.0 / 5.0 }, vpt, 0);
    [[maybe_unused]] auto const aidBoxesInRange = tree.RangeSearch(boxes[0], vpt);
    [[maybe_unused]] auto const aidBoxesInRangeF = tree.RangeSearch(boxes[0], vpt, OrthoTree::RangeSearchMode::Overlap);
    [[maybe_unused]] auto const aidBoxesInRangeT = tree.RangeSearch(boxes[0], vpt, OrthoTree::RangeSearchMode::Inside);
    [[maybe_unused]] auto const aidPtsInPlane = tree.PlaneSearch(0.0, Point{ 1.0 }, vpt, 0.01);
    [[maybe_unused]] auto const idPlaneIntersected = tree.PlaneSearch(1.0, { 1.0, 0.0 }, vpt, 0.0);
    [[maybe_unused]] auto const idPlaneIntersectedP = tree.PlaneSearch(
      {
        1.0, { 1.0, 0.0 }
    },
      vpt,
      0.0);
    [[maybe_unused]] auto const idPlanePosSeg = tree.PlanePositiveSegmentation(1.0, { 1.0, 0.0 }, vpt, 0.0);
    [[maybe_unused]] auto const idPlanePosSegP = tree.PlanePositiveSegmentation(
      {
        1.0, { 1.0, 0.0 }
    },
      vpt,
      0.0);
    [[maybe_unused]] auto const idFrustum = tree.FrustumCulling(
      std::vector{
        Plane{ 1.0, { 1.0, 0.0 } }
    },
      vpt,
      0.0);
    [[maybe_unused]] auto const kNN = tree.GetNearestNeighbors({}, 2, vpt);
  }

  // non-const member functions
  {
    tree.Erase(20);
    tree.Erase(30, vpt.at(30));
    tree.InsertIntoLeaf(3, vpt.at(11));
    tree.Update(11, vpt.at(11), vpt.at(12));
    tree.Update(3, vpt.at(11), vpt.at(12));
    tree.UpdateIndexes(
      {
        { 11, std::nullopt },
        {  3,            4 }
    });
    tree.Move({ 1.0, 1.0 }, TExecMode{});
    tree.Clear();
    tree.Reset();

    tree.Init(boxes[0], 3, 12);
    tree.Reset();

    OT::Create(tree, vpt, 0, std::nullopt, 2, TExecMode{});
  }
}

template<OrthoTree::dim_t N, typename TExecMode, bool IS_LOOSE_TREE = true, OrthoTree::NodeGeometryStorage NODE_GEOMETRY_STORAGE = OrthoTree::NodeGeometryStorage::MBR>
void testCompileBox()
{
  using Vector = OrthoTree::VectorND<N>;
  using BoundingBox = OrthoTree::BoundingBoxND<N>;
  using Plane = OrthoTree::PlaneND<N>;
  using OT = OrthoTree::OrthoTreeBoxND<N, IS_LOOSE_TREE, OrthoTree::BaseGeometryType, true, NODE_GEOMETRY_STORAGE>;

  auto constexpr boxes = std::array{
    BoundingBox{ { 0.0, 0.0 }, { 1.0, 1.0 } },
    BoundingBox{ { 1.0, 1.0 }, { 2.0, 2.0 } },
    BoundingBox{ { 2.0, 2.0 }, { 3.0, 3.0 } },
    BoundingBox{ { 3.0, 3.0 }, { 4.0, 4.0 } },
    BoundingBox{ { 1.2, 1.2 }, { 2.8, 2.8 } }
  };

  auto tree = OT(TExecMode{}, boxes, 3, std::nullopt, 2);

  // const member functions
  {
    [[maybe_unused]] auto const boxAll = tree.GetBox();
    [[maybe_unused]] auto const nDepthMax = tree.GetDepthNo();
    [[maybe_unused]] auto const vidCollision = tree.CollisionDetection(boxes);
    [[maybe_unused]] auto const vidCollisionTree = tree.CollisionDetection(boxes, tree, boxes);

    [[maybe_unused]] auto const aidPick = tree.PickSearch({}, boxes);
    [[maybe_unused]] auto const aidBoxesInRange = tree.RangeSearch(boxes[0], boxes);
    [[maybe_unused]] auto const aidBoxesInRangeF = tree.RangeSearch(boxes[0], boxes, OrthoTree::RangeSearchMode::Overlap);
    [[maybe_unused]] auto const aidBoxesInRangeT = tree.RangeSearch(boxes[0], boxes, OrthoTree::RangeSearchMode::Inside);

    [[maybe_unused]] auto const idBoxesIntersectedAll = tree.RayIntersectedAll({}, { 3.0 / 5.0, 4.0 / 5.0 }, boxes, 0);
    [[maybe_unused]] auto const idBoxesIntersectedFirst = tree.RayIntersectedFirst({}, { 3.0 / 5.0, 4.0 / 5.0 }, boxes, 0);

    [[maybe_unused]] auto const idPlaneIntersected = tree.PlaneSearch(1.0, { 1.0, 0.0 }, boxes, 0.0);
    [[maybe_unused]] auto const idPlaneIntersectedP = tree.PlaneSearch(
      {
        1.0, { 1.0, 0.0 }
    },
      boxes,
      0.0);
    [[maybe_unused]] auto const idPlanePosSeg = tree.PlanePositiveSegmentation(1.0, { 1.0, 0.0 }, boxes, 0.0);
    [[maybe_unused]] auto const idPlanePosSegP = tree.PlanePositiveSegmentation(
      {
        1.0, { 1.0, 0.0 }
    },
      boxes,
      0.0);
    [[maybe_unused]] auto const idFrustum = tree.FrustumCulling(
      std::vector{
        Plane{ 1.0, Vector{ 1.0, 0.0 } }
    },
      boxes,
      0.0);
  }

  // non-const member functions
  {
    tree.Erase(4);
    tree.Erase(3, boxes[3]);
    tree.InsertIntoLeaf(3, boxes[3]);
    tree.Update(2, boxes[2], boxes[3]);
    tree.Update(3, boxes[3], boxes[4]);
    tree.UpdateIndexes(
      {
        { 1, 3 },
        { 3, 1 },
    });
    tree.Move({ 1.0, 1.0 }, TExecMode{});
    tree.Clear();
    tree.Reset();

    tree.Init(boxes[0], 3, 12);
    tree.Reset();

    OT::Create(tree, boxes, 0, std::nullopt, 2, TExecMode{});
  }
}

template<OrthoTree::dim_t N, typename TExecMode, bool IS_LOOSE_TREE = true, OrthoTree::NodeGeometryStorage NODE_GEOMETRY_STORAGE = OrthoTree::NodeGeometryStorage::MBR>
void testCompileBoxMap()
{
  using Vector = OrthoTree::VectorND<N>;
  using BoundingBox = OrthoTree::BoundingBoxND<N>;
  using Plane = OrthoTree::PlaneND<N>;

  using Map = std::unordered_map<int, BoundingBox>;
  using OT = OrthoTree::OrthoTreeBoxNDUD<N, IS_LOOSE_TREE, OrthoTree::BaseGeometryType, Map, NODE_GEOMETRY_STORAGE>;

  auto const boxes = Map{
    { 10, BoundingBox{ { 0.0, 0.0 }, { 1.0, 1.0 } } },
    { 12, BoundingBox{ { 1.0, 1.0 }, { 2.0, 2.0 } } },
    { 17, BoundingBox{ { 2.0, 2.0 }, { 3.0, 3.0 } } },
    { 13, BoundingBox{ { 3.0, 3.0 }, { 4.0, 4.0 } } },
    { 11, BoundingBox{ { 1.2, 1.2 }, { 2.8, 2.8 } } }
  };

  auto tree = OT(TExecMode{}, boxes, 3, std::nullopt, 2);

  // const member functions
  {
    [[maybe_unused]] auto const boxAll = tree.GetBox();
    [[maybe_unused]] auto const nDepthMax = tree.GetDepthNo();
    [[maybe_unused]] auto const vidCollision = tree.CollisionDetection(boxes, 0.01);
    [[maybe_unused]] auto const vidCollisionTree = tree.CollisionDetection(boxes, tree, boxes, 0.01);
    [[maybe_unused]] auto const vidCollisionET = tree.CollisionDetection(boxes, [](OT::EntityID, OT::EntityID) { return true; }, 0.01);

    [[maybe_unused]] auto const aidPick = tree.PickSearch({}, boxes);
    [[maybe_unused]] auto const aidBoxesInRange = tree.RangeSearch(boxes.at(10), boxes);
    [[maybe_unused]] auto const aidBoxesInRangeF = tree.RangeSearch(boxes.at(10), boxes, OrthoTree::RangeSearchMode::Overlap);
    [[maybe_unused]] auto const aidBoxesInRangeT = tree.RangeSearch(boxes.at(10), boxes, OrthoTree::RangeSearchMode::Inside);

    [[maybe_unused]] auto const idBoxesIntersectedAll = tree.RayIntersectedAll({}, { 3.0 / 5.0, 4.0 / 5.0 }, boxes, 0);
    [[maybe_unused]] auto const idBoxesIntersectedFirst = tree.RayIntersectedFirst({}, { 3.0 / 5.0, 4.0 / 5.0 }, boxes, 0);

    [[maybe_unused]] auto const idPlaneIntersected = tree.PlaneSearch(1.0, { 1.0, 0.0 }, boxes, 0.0);
    [[maybe_unused]] auto const idPlaneIntersectedP = tree.PlaneSearch(
      {
        1.0, { 1.0, 0.0 }
    },
      boxes,
      0.0);
    [[maybe_unused]] auto const idPlanePosSeg = tree.PlanePositiveSegmentation(1.0, { 1.0, 0.0 }, boxes, 0.0);
    [[maybe_unused]] auto const idPlanePosSegP = tree.PlanePositiveSegmentation(
      {
        1.0, { 1.0, 0.0 }
    },
      boxes,
      0.0);
    [[maybe_unused]] auto const idFrustum = tree.FrustumCulling(
      std::vector{
        Plane{ 1.0, Vector{ 1.0, 0.0 } }
    },
      boxes,
      0.0);

    // Query tests
    [[maybe_unused]] auto const queryEntities = tree.Query(
      std::array{
        OT::ByWithin(boxes.at(10)),
        OT::ByIntersecting(Plane{ 1.0, Vector{ 1.0, 0.0 } }
        ),
        OT::BySatisfies([](OT::Entity const&) { return true; }
        ),
        OT::ByInFrustum({ { Plane{ 1.0, Vector{ 1.0, 0.0 } }, true } }
        ),
        OT::ByOverlaps(boxes.at(10)),
        OT::BySatisfies([](OT::EntityID) { return true; }
        )
    },
      boxes);

    // Traversal tests
    tree.TraverseEntitiesBreadthFirst([](auto const& /*nodeEntities*/, auto const& /*nodeBox*/) { return OrthoTree::TraverseControl::Continue; });
    tree.TraverseEntitiesDepthFirst([](auto const& /*nodeEntities*/, auto const& /*nodeBox*/) { return OrthoTree::TraverseControl::Continue; });
    tree.TraverseEntitiesByPriority(
      [](auto const& /*nodeEntities*/, auto const& /*nodeBox*/, auto const& /*priority*/) { return OrthoTree::TraverseControl::Continue; },
      [](auto const& /*nodeBox*/) { return 0.0; });

    // Ray intersection signatures
    Vector vHeading{};
    vHeading[0] = 1.0;

    [[maybe_unused]] auto const rayIntersectedAll = tree.RayIntersectedAll({}, vHeading, boxes, 0);
    [[maybe_unused]] auto const rayIntersectedFirst = tree.RayIntersectedFirst({}, vHeading, boxes, 0);

    using TFloatScalar = typename OT::GA::FloatScalar;
    // Ray Hit Testers
    [[maybe_unused]] auto const r1 = tree.RayIntersectedAll({}, vHeading, boxes, 0, 0, 1000.0, [](OT::EntityID) { return true; });
    [[maybe_unused]] auto const r2 = tree.RayIntersectedAll({}, vHeading, boxes, 0, 0, 1000.0, [](OT::EntityID, Vector, Vector) { return true; });
    [[maybe_unused]] auto const r3 = tree.RayIntersectedAll({}, vHeading, boxes, 0, 0, 1000.0, [](auto const&) { return true; });
    [[maybe_unused]] auto const r4 = tree.RayIntersectedAll({}, vHeading, boxes, 0, 0, 1000.0, [](auto const&, Vector, Vector) { return true; });

    [[maybe_unused]] auto const rd1 =
      tree.RayIntersectedAll({}, vHeading, boxes, 0, 0, 1000.0, [](OT::EntityID) -> std::optional<TFloatScalar> { return 0.0; });
    [[maybe_unused]] auto const rd2 =
      tree.RayIntersectedAll({}, vHeading, boxes, 0, 0, 1000.0, [](OT::EntityID, Vector, Vector) -> std::optional<TFloatScalar> { return 0.0; });
    [[maybe_unused]] auto const rd3 =
      tree.RayIntersectedAll({}, vHeading, boxes, 0, 0, 1000.0, [](auto const&) -> std::optional<TFloatScalar> { return 0.0; });
    [[maybe_unused]] auto const rd4 =
      tree.RayIntersectedAll({}, vHeading, boxes, 0, 0, 1000.0, [](auto const&, Vector, Vector) -> std::optional<TFloatScalar> { return 0.0; });
  }

  // non-const member functions
  {
    tree.Erase(12);
    tree.Erase(10, boxes.at(10));
    tree.InsertIntoLeaf(23, boxes.at(17));
    tree.Update(11, boxes.at(11), boxes.at(17));
    tree.Update(23, boxes.at(17), boxes.at(13));
    tree.UpdateIndexes(
      {
        { 17, std::nullopt },
        { 23,           22 }
    });
    tree.Move({ 1.0, 1.0 }, TExecMode{});
    tree.Clear();
    tree.Reset();

    tree.Init(boxes.at(13), 3, 12);
    tree.Reset();

    OT::Create(tree, boxes, std::nullopt, std::nullopt, 2, TExecMode{});
  }
}

template<OrthoTree::dim_t N, typename TExecMode, OrthoTree::NodeGeometryStorage NODE_GEOMETRY_STORAGE>
void testCompilePointM()
{
  using Point = OrthoTree::PointND<N>;
  using BoundingBox = OrthoTree::BoundingBoxND<N>;
  using Plane = OrthoTree::PlaneND<N>;
  using OT = OrthoTree::OrthoTreeManaged<OrthoTree::OrthoTreePointND<N, OrthoTree::BaseGeometryType, true, NODE_GEOMETRY_STORAGE>>;

  auto constexpr vpt = std::array{ Point{ 0.0 }, Point{ 1.0 }, Point{ 2.0 }, Point{ 3.0 }, Point{ 4.0 } };

  auto constexpr boxes = std::array{
    BoundingBox{ { 0.0, 0.0 }, { 1.0, 1.0 } },
    BoundingBox{ { 1.0, 1.0 }, { 2.0, 2.0 } },
    BoundingBox{ { 2.0, 2.0 }, { 3.0, 3.0 } },
    BoundingBox{ { 3.0, 3.0 }, { 4.0, 4.0 } },
    BoundingBox{ { 1.2, 1.2 }, { 2.8, 2.8 } }
  };

  auto tree = OT(TExecMode{}, vpt, 3, std::nullopt, 2);

  // const member functions
  {
    [[maybe_unused]] auto const& treeCore = tree.GetCore();
    [[maybe_unused]] auto const& data = tree.GetData();

    [[maybe_unused]] auto const idBoxesIntersectedAll = tree.RayIntersectedAll({}, { 3.0 / 5.0, 4.0 / 5.0 }, OT::GA::BASE_TOLERANCE);
    [[maybe_unused]] auto const idBoxesIntersectedFirst = tree.RayIntersectedFirst({}, { 3.0 / 5.0, 4.0 / 5.0 }, 0);
    [[maybe_unused]] auto const aidBoxesInRange = tree.RangeSearch(boxes[0]);
    [[maybe_unused]] auto const aidBoxesInRangeF = tree.RangeSearch(boxes[0], OrthoTree::RangeSearchMode::Overlap);
    [[maybe_unused]] auto const aidBoxesInRangeT = tree.RangeSearch(boxes[0], OrthoTree::RangeSearchMode::Inside);

    [[maybe_unused]] auto const kNN = tree.GetNearestNeighbors({}, 2);

    [[maybe_unused]] auto const idPlaneIntersected = tree.PlaneSearch(1.0, { 1.0, 0.0 }, 0.0);
    [[maybe_unused]] auto const idPlaneIntersectedP = tree.PlaneSearch(
      {
        1.0, { 1.0, 0.0 }
    },
      0.0);
    [[maybe_unused]] auto const idPlanePosSeg = tree.PlanePositiveSegmentation(1.0, { 1.0, 0.0 }, 0.0);
    [[maybe_unused]] auto const idPlanePosSegP = tree.PlanePositiveSegmentation(
      {
        1.0, { 1.0, 0.0 }
    },
      0.0);
    [[maybe_unused]] auto const idFrustum = tree.FrustumCulling(
      std::vector{
        Plane{ 1.0, { 1.0, 0.0 } },
        Plane{ 1.0, { 0.0, 1.0 } }
    },
      0.0);
  }

  // non-const member functions
  {
    auto const arr = std::array{ Point{} };

    tree.Erase(2);
    tree.Update(3, vpt[4]);
    tree.Move({ 1.0, 1.0 }, TExecMode{});
    tree.Clear();
    tree.Reset();

    tree.Init(boxes[0], 3, 12);
    tree.Reset();

    tree = OT::Create(vpt, 0, std::nullopt, 2, TExecMode{});
  }
}

template<OrthoTree::dim_t N, typename TExecMode, OrthoTree::NodeGeometryStorage NODE_GEOMETRY_STORAGE>
void testCompilePointMapM()
{
  using Point = OrthoTree::PointND<N>;
  using BoundingBox = OrthoTree::BoundingBoxND<N>;
  using Plane = OrthoTree::PlaneND<N>;
  using Map = std::unordered_map<int, Point>;
  using OT = OrthoTree::OrthoTreeManaged<OrthoTree::OrthoTreePointNDUD<N, OrthoTree::BaseGeometryType, Map, NODE_GEOMETRY_STORAGE>>;

  auto const vpt = Map{
    { 10, Point{ 0.0 } },
    { 20, Point{ 1.0 } },
    { 30, Point{ 2.0 } },
    { 40, Point{ 3.0 } },
    { 50, Point{ 4.0 } }
  };

  auto constexpr boxes = std::array{
    BoundingBox{ { 0.0, 0.0 }, { 1.0, 1.0 } },
    BoundingBox{ { 1.0, 1.0 }, { 2.0, 2.0 } },
    BoundingBox{ { 2.0, 2.0 }, { 3.0, 3.0 } },
    BoundingBox{ { 3.0, 3.0 }, { 4.0, 4.0 } },
    BoundingBox{ { 1.2, 1.2 }, { 2.8, 2.8 } }
  };

  auto tree = OT(TExecMode{}, vpt, 3, std::nullopt, 2);
  // const member functions
  {
    [[maybe_unused]] auto const& treeCore = tree.GetCore();
    [[maybe_unused]] auto const& data = tree.GetData();

    [[maybe_unused]] auto const idBoxesIntersectedAll = tree.RayIntersectedAll({}, { 3.0 / 5.0, 4.0 / 5.0 }, 0);
    [[maybe_unused]] auto const idBoxesIntersectedFirst = tree.RayIntersectedFirst({}, { 3.0 / 5.0, 4.0 / 5.0 }, 0);
    [[maybe_unused]] auto const aidBoxesInRange = tree.RangeSearch(boxes[0]);
    [[maybe_unused]] auto const aidBoxesInRangeF = tree.RangeSearch(boxes[0], OrthoTree::RangeSearchMode::Overlap);
    [[maybe_unused]] auto const aidBoxesInRangeT = tree.RangeSearch(boxes[0], OrthoTree::RangeSearchMode::Inside);

    [[maybe_unused]] auto const kNN = tree.GetNearestNeighbors({}, 2);

    [[maybe_unused]] auto const idPlaneIntersected = tree.PlaneSearch(1.0, { 1.0, 0.0 }, 0.0);
    [[maybe_unused]] auto const idPlaneIntersectedP = tree.PlaneSearch(
      {
        1.0, { 1.0, 0.0 }
    },
      0.0);
    [[maybe_unused]] auto const idPlanePosSeg = tree.PlanePositiveSegmentation(1.0, { 1.0, 0.0 }, 0.0);
    [[maybe_unused]] auto const idPlanePosSegP = tree.PlanePositiveSegmentation(
      {
        1.0, { 1.0, 0.0 }
    },
      0.0);
    [[maybe_unused]] auto const idFrustum = tree.FrustumCulling(
      std::vector{
        Plane{ 1.0, { 1.0, 0.0 } },
        Plane{ 1.0, { 0.0, 1.0 } }
    },
      0.0);
  }

  // non-const member functions

  tree.Update({ 30, vpt.at(40) });
  tree.Move({ 1.0, 1.0 }, TExecMode{});
  tree.Clear();
  tree.Reset();

  tree.Init(boxes[0], 3, 12);
  tree.Reset();

  tree = OT::Create(vpt, std::nullopt, std::nullopt, 2, TExecMode{});
}

template<OrthoTree::dim_t N, typename TExecMode, bool IS_LOOSE_TREE = true, OrthoTree::NodeGeometryStorage NODE_GEOMETRY_STORAGE = OrthoTree::NodeGeometryStorage::MBR>
void testCompileBoxM()
{
  using BoundingBox = OrthoTree::BoundingBoxND<N>;
  using Plane = OrthoTree::PlaneND<N>;
  using OT = OrthoTree::OrthoTreeManaged<OrthoTree::OrthoTreeBoxND<N, IS_LOOSE_TREE, OrthoTree::BaseGeometryType, true, NODE_GEOMETRY_STORAGE>>;

  auto constexpr boxes = std::array{
    BoundingBox{ { 0.0, 0.0 }, { 1.0, 1.0 } },
    BoundingBox{ { 1.0, 1.0 }, { 2.0, 2.0 } },
    BoundingBox{ { 2.0, 2.0 }, { 3.0, 3.0 } },
    BoundingBox{ { 3.0, 3.0 }, { 4.0, 4.0 } },
    BoundingBox{ { 1.2, 1.2 }, { 2.8, 2.8 } }
  };

  auto tree = OT(TExecMode{}, boxes, 3, std::nullopt, 2);

  // const member functions
  {
    [[maybe_unused]] auto const& treeCore = tree.GetCore();
    [[maybe_unused]] auto const& data = tree.GetData();

    [[maybe_unused]] auto const aidPick = tree.PickSearch({});

    [[maybe_unused]] auto const idBoxesIntersectedAll = tree.RayIntersectedAll({}, { 3.0 / 5.0, 4.0 / 5.0 }, 0);
    [[maybe_unused]] auto const idBoxesIntersectedFirst = tree.RayIntersectedFirst({}, { 3.0 / 5.0, 4.0 / 5.0 }, 0);
    [[maybe_unused]] auto const aidBoxesInRange = tree.RangeSearch(boxes[0]);
    [[maybe_unused]] auto const aidBoxesInRangeF = tree.RangeSearch(boxes[0], OrthoTree::RangeSearchMode::Overlap);
    [[maybe_unused]] auto const aidBoxesInRangeT = tree.RangeSearch(boxes[0], OrthoTree::RangeSearchMode::Inside);

    [[maybe_unused]] auto const vidCollision = tree.template CollisionDetection<TExecMode>();
    [[maybe_unused]] auto const vidCollisionTree = tree.CollisionDetection(tree);

    [[maybe_unused]] auto const idPlaneIntersected = tree.PlaneIntersection(1.0, { 1.0, 0.0 }, 0.0);
    [[maybe_unused]] auto const idPlaneIntersectedP = tree.PlaneIntersection(
      {
        1.0, { 1.0, 0.0 }
    },
      0.0);
    [[maybe_unused]] auto const idPlanePosSeg = tree.PlanePositiveSegmentation(1.0, { 1.0, 0.0 }, 0.0);
    [[maybe_unused]] auto const idPlanePosSegP = tree.PlanePositiveSegmentation(
      {
        1.0, { 1.0, 0.0 }
    },
      0.0);
    [[maybe_unused]] auto const idFrustum = tree.FrustumCulling(
      std::vector{
        Plane{ 1.0, { 1.0, 0.0 } },
        Plane{ 1.0, { 0.0, 1.0 } }
    },
      0.0);
  }

  // non-const member functions
  {
    auto const arr = std::array{ BoundingBox{} };

    tree.Erase(2);
    tree.Update(3, boxes[4]);
    tree.Move({ 1.0, 1.0 }, TExecMode{});
    tree.Clear();
    tree.Reset();

    tree.Init(boxes[0], 3, 12);
    tree.Reset();

    tree = OT::Create(boxes, 3, std::nullopt, 2, TExecMode{});
  }
}

template<OrthoTree::dim_t N, typename TExecMode, bool IS_LOOSE_TREE = true, OrthoTree::NodeGeometryStorage NODE_GEOMETRY_STORAGE = OrthoTree::NodeGeometryStorage::MBR>
void testCompileBoxMapM()
{
  using BoundingBox = OrthoTree::BoundingBoxND<N>;
  using Plane = OrthoTree::PlaneND<N>;
  using Map = std::unordered_map<int, BoundingBox>;
  using OT = OrthoTree::OrthoTreeManaged<OrthoTree::OrthoTreeBoxNDUD<N, IS_LOOSE_TREE, OrthoTree::BaseGeometryType, Map, NODE_GEOMETRY_STORAGE>>;

  auto const boxes = Map{
    {  5, BoundingBox{ { 0.0, 0.0 }, { 1.0, 1.0 } } },
    { 10, BoundingBox{ { 1.0, 1.0 }, { 2.0, 2.0 } } },
    { 20, BoundingBox{ { 2.0, 2.0 }, { 3.0, 3.0 } } },
    { 40, BoundingBox{ { 3.0, 3.0 }, { 4.0, 4.0 } } },
    { 30, BoundingBox{ { 1.2, 1.2 }, { 2.8, 2.8 } } }
  };

  auto tree = OT(TExecMode{}, boxes, std::nullopt, std::nullopt, 2);

  // const member functions
  {
    [[maybe_unused]] auto const& treeCore = tree.GetCore();
    [[maybe_unused]] auto const& data = tree.GetData();

    [[maybe_unused]] auto const aidPick = tree.PickSearch({});
    [[maybe_unused]] auto const aidBoxesInRange = tree.RangeSearch(boxes.at(30));
    [[maybe_unused]] auto const aidBoxesInRangeF = tree.RangeSearch(boxes.at(30), OrthoTree::RangeSearchMode::Overlap);
    [[maybe_unused]] auto const aidBoxesInRangeT = tree.RangeSearch(boxes.at(30), OrthoTree::RangeSearchMode::Inside);

    [[maybe_unused]] auto const vidCollision = tree.template CollisionDetection<TExecMode>();
    [[maybe_unused]] auto const vidCollisionTree = tree.CollisionDetection(tree);

    [[maybe_unused]] auto const idBoxesIntersectedAll = tree.RayIntersectedAll({}, { 3.0 / 5.0, 4.0 / 5.0 }, 0);
    [[maybe_unused]] auto const idBoxesIntersectedFirst = tree.RayIntersectedFirst({}, { 3.0 / 5.0, 4.0 / 5.0 }, 0);

    [[maybe_unused]] auto const idPlaneIntersected = tree.PlaneIntersection(1.0, { 1.0, 0.0 }, 0.0);
    [[maybe_unused]] auto const idPlaneIntersectedP = tree.PlaneIntersection(
      {
        1.0, { 1.0, 0.0 }
    },
      0.0);
    [[maybe_unused]] auto const idPlanePosSeg = tree.PlanePositiveSegmentation(1.0, { 1.0, 0.0 }, 0.0);
    [[maybe_unused]] auto const idPlanePosSegP = tree.PlanePositiveSegmentation(
      {
        1.0, { 1.0, 0.0 }
    },
      0.0);
    [[maybe_unused]] auto const idFrustum = tree.FrustumCulling(
      std::vector{
        Plane{ 1.0, { 1.0, 0.0 } },
        Plane{ 1.0, { 0.0, 1.0 } }
    },
      0.0);
  }

  // non-const member functions
  {
    tree.Erase(20);
    tree.Update({ 40, boxes.at(30) });
    tree.Move({ 1.0, 1.0 }, TExecMode{});
    tree.Clear();
    tree.Reset();

    tree.Init(boxes.at(5), 3, 12);
    tree.Reset();

    tree = OT::Create(boxes, 0, std::nullopt, 2, TExecMode{});
    tree = OT::Create(
      Map{
        { 10, BoundingBox{ { 0.0, 0.0 }, { 1.0, 1.0 } } },
        { 15, BoundingBox{ { 1.0, 1.0 }, { 2.0, 2.0 } } },
        { 16, BoundingBox{ { 2.0, 2.0 }, { 3.0, 3.0 } } },
        { 12, BoundingBox{ { 3.0, 3.0 }, { 4.0, 4.0 } } },
        { 11, BoundingBox{ { 1.2, 1.2 }, { 2.8, 2.8 } } }
    },
      4);
  }
}

template<typename TExecMode, bool IS_LOOSE_TREE, OrthoTree::NodeGeometryStorage NODE_GEOMETRY_STORAGE>
void testCompileBoxBatchDimension()
{
  auto constexpr isPlatform64 = sizeof(std::size_t) == 8;

  // Core types
  {
    testCompilePoint<2, TExecMode, NODE_GEOMETRY_STORAGE>();
    testCompilePoint<3, TExecMode, NODE_GEOMETRY_STORAGE>();
    testCompilePoint<4, TExecMode, NODE_GEOMETRY_STORAGE>();
    // testCompilePoint<5, TExecMode, NODE_GEOMETRY_STORAGE>();
    testCompilePoint<6, TExecMode, NODE_GEOMETRY_STORAGE>();
    // testCompilePoint<7, TExecMode, NODE_GEOMETRY_STORAGE>();
    testCompilePoint<8, TExecMode, NODE_GEOMETRY_STORAGE>();
    // testCompilePoint<12, TExecMode, NODE_GEOMETRY_STORAGE>();
    testCompilePoint<16, TExecMode, NODE_GEOMETRY_STORAGE>();
    testCompilePoint<31, TExecMode, NODE_GEOMETRY_STORAGE>();
    if constexpr (isPlatform64)
    {
      testCompilePoint<32, TExecMode, NODE_GEOMETRY_STORAGE>();
      testCompilePoint<63, TExecMode, NODE_GEOMETRY_STORAGE>();
    }
    testCompilePointMap<2, TExecMode, NODE_GEOMETRY_STORAGE>();
    testCompilePointMap<3, TExecMode, NODE_GEOMETRY_STORAGE>();

    testCompileBox<2, TExecMode, IS_LOOSE_TREE, NODE_GEOMETRY_STORAGE>();
    testCompileBox<3, TExecMode, IS_LOOSE_TREE, NODE_GEOMETRY_STORAGE>();
    testCompileBox<4, TExecMode, IS_LOOSE_TREE, NODE_GEOMETRY_STORAGE>();
    // testCompileBox<5, TExecMode, IS_LOOSE_TREE, NODE_GEOMETRY_STORAGE>();
    testCompileBox<6, TExecMode, IS_LOOSE_TREE, NODE_GEOMETRY_STORAGE>();
    // testCompileBox<7, TExecMode, IS_LOOSE_TREE, NODE_GEOMETRY_STORAGE>();
    testCompileBox<8, TExecMode, IS_LOOSE_TREE, NODE_GEOMETRY_STORAGE>();
    // testCompileBox<12, TExecMode, IS_LOOSE_TREE, NODE_GEOMETRY_STORAGE>();
    testCompileBox<16, TExecMode, IS_LOOSE_TREE, NODE_GEOMETRY_STORAGE>();
    testCompileBox<31, TExecMode, IS_LOOSE_TREE, NODE_GEOMETRY_STORAGE>();
    if constexpr (isPlatform64)
    {
      testCompileBox<32, TExecMode, IS_LOOSE_TREE, NODE_GEOMETRY_STORAGE>();
      testCompileBox<63, TExecMode, IS_LOOSE_TREE, NODE_GEOMETRY_STORAGE>();
    }

    testCompileBoxMap<2, TExecMode, IS_LOOSE_TREE, NODE_GEOMETRY_STORAGE>();
    testCompileBoxMap<3, TExecMode, IS_LOOSE_TREE, NODE_GEOMETRY_STORAGE>();
  }

  // Container types
  {
    testCompilePointM<2, TExecMode, NODE_GEOMETRY_STORAGE>();
    testCompilePointM<3, TExecMode, NODE_GEOMETRY_STORAGE>();
    // testCompilePointM<4, TExecMode, NODE_GEOMETRY_STORAGE>();
    // testCompilePointM<5, TExecMode, NODE_GEOMETRY_STORAGE>();
    testCompilePointM<6, TExecMode, NODE_GEOMETRY_STORAGE>();
    // testCompilePointM<7, TExecMode, NODE_GEOMETRY_STORAGE>();
    // testCompilePointM<8, TExecMode, NODE_GEOMETRY_STORAGE>();
    // testCompilePointM<12, TExecMode, NODE_GEOMETRY_STORAGE>();
    // testCompilePointM<16, TExecMode, NODE_GEOMETRY_STORAGE>();
    testCompilePointM<31, TExecMode, NODE_GEOMETRY_STORAGE>();
    if constexpr (isPlatform64)
    {
      testCompilePointM<32, TExecMode, NODE_GEOMETRY_STORAGE>();
      testCompilePointM<63, TExecMode, NODE_GEOMETRY_STORAGE>();
    }
    testCompilePointMapM<2, TExecMode, NODE_GEOMETRY_STORAGE>();
    testCompilePointMapM<3, TExecMode, NODE_GEOMETRY_STORAGE>();

    testCompileBoxM<2, TExecMode, IS_LOOSE_TREE, NODE_GEOMETRY_STORAGE>();
    testCompileBoxM<3, TExecMode, IS_LOOSE_TREE, NODE_GEOMETRY_STORAGE>();
    // testCompileBoxM<4, TExecMode, IS_LOOSE_TREE, NODE_GEOMETRY_STORAGE>();
    // testCompileBoxM<5, TExecMode, IS_LOOSE_TREE, NODE_GEOMETRY_STORAGE>();
    testCompileBoxM<6, TExecMode, IS_LOOSE_TREE, NODE_GEOMETRY_STORAGE>();
    // testCompileBoxM<7, TExecMode, IS_LOOSE_TREE, NODE_GEOMETRY_STORAGE>();
    // testCompileBoxM<8, TExecMode, IS_LOOSE_TREE, NODE_GEOMETRY_STORAGE>();
    // testCompileBoxM<12, TExecMode, IS_LOOSE_TREE, NODE_GEOMETRY_STORAGE>();
    // testCompileBoxM<16, TExecMode, IS_LOOSE_TREE, NODE_GEOMETRY_STORAGE>();
    testCompileBoxM<31, TExecMode, IS_LOOSE_TREE, NODE_GEOMETRY_STORAGE>();

    if constexpr (isPlatform64)
    {
      testCompileBoxM<32, TExecMode, IS_LOOSE_TREE, NODE_GEOMETRY_STORAGE>();
      testCompileBoxM<63, TExecMode, IS_LOOSE_TREE, NODE_GEOMETRY_STORAGE>();
    }

    testCompileBoxMapM<2, TExecMode, IS_LOOSE_TREE, NODE_GEOMETRY_STORAGE>();
    testCompileBoxMapM<3, TExecMode, IS_LOOSE_TREE, NODE_GEOMETRY_STORAGE>();
  }
}

template<bool IS_LOOSE_TREE = true>
void testCompileBoxBatchExPol()
{
#ifdef __cpp_lib_execution
  testCompileBoxBatchDimension<OrthoTree::SeqExec, IS_LOOSE_TREE, OrthoTree::NodeGeometryStorage::None>();
  // testCompileBoxBatchDimension<OrthoTree::ParExec, IS_LOOSE_TREE, OrthoTree::NodeGeometryStorage::None>();
  testCompileBoxBatchDimension<OrthoTree::SeqExec, IS_LOOSE_TREE, OrthoTree::NodeGeometryStorage::MinPoint>();
  // testCompileBoxBatchDimension<OrthoTree::ParExec, IS_LOOSE_TREE, OrthoTree::NodeGeometryStorage::MinPoint>();
  testCompileBoxBatchDimension<OrthoTree::SeqExec, IS_LOOSE_TREE, OrthoTree::NodeGeometryStorage::MBR>();
  testCompileBoxBatchDimension<OrthoTree::ParExec, IS_LOOSE_TREE, OrthoTree::NodeGeometryStorage::MBR>();
#else
  testCompileBoxBatchDimension<OrthoTree::SeqExec, IS_LOOSE_TREE, OrthoTree::NodeGeometryStorage::None>();
  testCompileBoxBatchDimension<OrthoTree::SeqExec, IS_LOOSE_TREE, OrthoTree::NodeGeometryStorage::MinPoint>();
  testCompileBoxBatchDimension<OrthoTree::SeqExec, IS_LOOSE_TREE, OrthoTree::NodeGeometryStorage::MBR>();
#endif
}

inline void testCompileSpaceIndexing()
{
  [[maybe_unused]] auto const key = OrthoTree::detail::MortonSpaceIndexing<3, 21>::GetNodeID(3, 2);
  [[maybe_unused]] auto const aidGrid = OrthoTree::detail::MortonSpaceIndexing<3, 21>::Decode(key, 3);
  [[maybe_unused]] auto const idGrid = OrthoTree::detail::MortonSpaceIndexing<3, 21>::Encode(aidGrid);
  [[maybe_unused]] auto const fValid = OrthoTree::detail::MortonSpaceIndexing<3, 21>::IsValidKey(key);
  [[maybe_unused]] auto const idChild = OrthoTree::detail::MortonSpaceIndexing<3, 21>::RemoveSentinelBit(key);
  [[maybe_unused]] auto const keyRoot = OrthoTree::detail::MortonSpaceIndexing<3, 21>::GetRootKey();
  [[maybe_unused]] auto const nDepth = OrthoTree::detail::MortonSpaceIndexing<3, 21>::GetDepthID(keyRoot);
}

template<OrthoTree::dim_t N>
void testCompileStaticTree()
{
  {
    using BoundingBox = OrthoTree::BoundingBoxND<N>;
    using OT = OrthoTree::StaticOrthoTreeBoxND<N, true, OrthoTree::BaseGeometryType, true, OrthoTree::NodeGeometryStorage::MBR>;

    auto constexpr boxes = std::array{
      BoundingBox{ { 0.0, 0.0 }, { 1.0, 1.0 } },
      BoundingBox{ { 1.0, 1.0 }, { 2.0, 2.0 } }
    };
    auto tree = OT(boxes, 3, std::nullopt, 2);

    {
      [[maybe_unused]] auto const boxAll = tree.GetBox();
      [[maybe_unused]] auto const nDepthMax = tree.GetDepthNo();
      [[maybe_unused]] auto const aidBoxesInRange = tree.RangeSearch(boxes[0], boxes);
      [[maybe_unused]] auto const aidPick = tree.PickSearch({}, boxes);
      [[maybe_unused]] auto const vidCollision = tree.CollisionDetection(boxes);
      [[maybe_unused]] auto const rayFirst = tree.RayIntersectedFirst({}, { 3.0 / 5.0, 4.0 / 5.0 }, boxes, 0);
    }
  }

  {
    using Point = OrthoTree::PointND<N>;
    using OT = OrthoTree::StaticOrthoTreePointND<N, OrthoTree::BaseGeometryType, true, OrthoTree::NodeGeometryStorage::MinPoint>;

    auto constexpr pts = std::array{ Point{ 0.0 }, Point{ 1.0 } };
    auto tree = OT(pts, 3, std::nullopt, 2);

    {
      [[maybe_unused]] auto const boxAll = tree.GetBox();
      [[maybe_unused]] auto const nDepthMax = tree.GetDepthNo();
      [[maybe_unused]] auto const aidPtsInRange = tree.RangeSearch(OrthoTree::BoundingBoxND<N>{ pts[0], pts[0] }, pts);
      [[maybe_unused]] auto const aidPick = tree.PickSearch({}, pts);
      [[maybe_unused]] auto const kNN = tree.GetNearestNeighbors({}, 2, pts);
      [[maybe_unused]] auto const rayFirst = tree.RayIntersectedFirst({}, { 3.0 / 5.0, 4.0 / 5.0 }, pts, 0);
    }
  }
}

inline void testCompileBoxBatchSplitStrategy()
{
  testCompileSpaceIndexing();
  testCompileStaticTree<2>();
  testCompileStaticTree<3>();
  testCompileBoxBatchExPol<false>();
  testCompileBoxBatchExPol<true>();
}

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#ifdef __GNUC__
#pragma GCC diagnostic pop
#pragma GCC diagnostic pop
#endif

#endif // ORTHOTREE_COMPILE_TEST_GUARD
