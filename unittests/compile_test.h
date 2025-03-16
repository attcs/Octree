#ifndef ORTHOTREE_COMPILE_TEST_GUARD
#define ORTHOTREE_COMPILE_TEST_GUARD

#include <array>
#include <vector>

#include "../octree.h"

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

template<OrthoTree::dim_t N, bool IS_PARALLEL_EXEC>
void testCompilePoint()
{
  using Point = OrthoTree::PointND<N>;
  using BoundingBox = OrthoTree::BoundingBoxND<N>;
  using Plane = OrthoTree::PlaneND<N>;

  using OT = OrthoTree::TreePointND<N>;

  [[maybe_unused]] auto const key = OT::SI::GetHash(2, 3);
  [[maybe_unused]] auto const noNode = OT::EstimateNodeNumber(100, 10, 3);
  [[maybe_unused]] auto const aidGrid = OT::SI::Decode(key, 3);
  [[maybe_unused]] auto const idGrid = OT::SI::Encode(aidGrid);
  [[maybe_unused]] auto const fValid = OT::SI::IsValidKey(key);
  [[maybe_unused]] auto const idChild = OT::SI::RemoveSentinelBit(key);

  auto constexpr vpt = std::array{ Point{ 0.0 }, Point{ 1.0 }, Point{ 2.0 }, Point{ 3.0 }, Point{ 4.0 } };

  auto constexpr boxes = std::array
  {
    BoundingBox{ { 0.0, 0.0 }, { 1.0, 1.0 } },
    BoundingBox{ { 1.0, 1.0 }, { 2.0, 2.0 } },
    BoundingBox{ { 2.0, 2.0 }, { 3.0, 3.0 } },
    BoundingBox{ { 3.0, 3.0 }, { 4.0, 4.0 } },
    BoundingBox{ { 1.2, 1.2 }, { 2.8, 2.8 } }
  };

  auto tree = OT(vpt, 3, std::nullopt, 2);

  // const member functions
  {
    auto const keyRoot = OT::SI::GetRootKey();
    [[maybe_unused]] auto const center = tree.GetNodeCenter(keyRoot);
    [[maybe_unused]] auto const size = tree.GetNodeSize(0);

    [[maybe_unused]] auto const allidBFS = tree.CollectAllEntitiesInBFS(keyRoot);
    [[maybe_unused]] auto const allidDFS = tree.CollectAllEntitiesInDFS(keyRoot);

    [[maybe_unused]] auto const nodeSmallest = tree.FindSmallestNode(vpt.back());

    [[maybe_unused]] auto const keySmallest = tree.FindSmallestNodeKey(keyRoot);
    [[maybe_unused]] auto const boxAll = tree.GetBox();
    [[maybe_unused]] auto const nDepth = OT::SI::GetDepthID(keyRoot);
    [[maybe_unused]] auto const nDepthMax = tree.GetDepthNo();
    [[maybe_unused]] auto const nodeRoot = tree.GetNode(keyRoot);
    [[maybe_unused]] auto const nodes = tree.GetNodes();
    [[maybe_unused]] auto const grid = tree.GetResolutionMax();

    [[maybe_unused]] auto const aidBoxesInRange = tree.RangeSearch(boxes[0], vpt);
    [[maybe_unused]] auto const aidBoxesInRangeF = tree.template RangeSearch<false>(boxes[0], vpt);
    [[maybe_unused]] auto const aidBoxesInRangeT = tree.template RangeSearch<true>(boxes[0], vpt);
    [[maybe_unused]] auto const aidPtsInPlane = tree.PlaneSearch(0.0, Point{ 1.0 }, 0.01, vpt);
    [[maybe_unused]] auto const idPlaneIntersected = tree.PlaneSearch(1.0, { 1.0, 0.0 }, 0.0, vpt);
    [[maybe_unused]] auto const idPlaneIntersectedP = tree.PlaneSearch({1.0, { 1.0, 0.0 }}, 0.0, vpt);
    [[maybe_unused]] auto const idPlanePosSeg = tree.PlanePositiveSegmentation(1.0, { 1.0, 0.0 }, 0.0, vpt);
    [[maybe_unused]] auto const idPlanePosSegP = tree.PlanePositiveSegmentation({1.0, { 1.0, 0.0 }}, 0.0, vpt);
    [[maybe_unused]] auto const idFrustum = tree.FrustumCulling(
      std::vector{
        Plane{1.0, { 1.0, 0.0 }}
      },
      0.0,
      vpt);
    [[maybe_unused]] auto const kNN = tree.GetNearestNeighbors({}, 2, vpt);

    [[maybe_unused]] auto vListIsAnyChild = std::vector<bool>{};
    tree.VisitNodes(keyRoot
      , [&vListIsAnyChild](auto const&, auto const& node) { vListIsAnyChild.emplace_back(node.IsAnyChildExist()); }
      , [](auto const&, auto const&) -> bool { return true; }
    );
  }

  // non-const member functions
  {
    tree.template EraseEntity<false>(4);
    tree.template Erase<false>(3, vpt[3]);
    tree.Insert(3, vpt[3]);
    tree.Update(2, vpt[2], vpt[3]);
    tree.Update(3, vpt[4]);
    tree.UpdateIndexes({ {1, std::nullopt }, {3, 4} });
    tree.template Move<IS_PARALLEL_EXEC>({ 1.0, 1.0 });
    tree.Clear();
    tree.Reset();

    tree.Init(boxes[0], 3, 12);
    tree.Reset();

    OT::template Create<IS_PARALLEL_EXEC>(tree, vpt);
  }
}


template<OrthoTree::dim_t N, bool IS_PARALLEL_EXEC>
void testCompilePointMap()
{
  using Point = OrthoTree::PointND<N>;
  using BoundingBox = OrthoTree::BoundingBoxND<N>;
  using Plane = OrthoTree::PlaneND<N>;
  using Map = std::unordered_map<int, Point>;

  using OT = OrthoTree::TreePointND<N, OrthoTree::BaseGeometryType, Map>;

  [[maybe_unused]] auto const key = OT::SI::GetHash(2, 3);
  [[maybe_unused]] auto const noNode = OT::EstimateNodeNumber(100, 10, 3);
  [[maybe_unused]] auto const aidGrid = OT::SI::Decode(key, 3);
  [[maybe_unused]] auto const idGrid = OT::SI::Encode(aidGrid);
  [[maybe_unused]] auto const fValid = OT::SI::IsValidKey(key);
  [[maybe_unused]] auto const idChild = OT::SI::RemoveSentinelBit(key);

  auto const vpt = Map{
    { 10, Point{ 0.0 } },
    { 20, Point{ 1.0 } },
    { 30, Point{ 2.0 } },
    { 11, Point{ 3.0 } },
    { 12, Point{ 4.0 } }
  };

  auto const boxes = std::array{
    BoundingBox{{ 0.0, 0.0 }, { 1.0, 1.0 }},
    BoundingBox{{ 1.0, 1.0 }, { 2.0, 2.0 }},
    BoundingBox{{ 2.0, 2.0 }, { 3.0, 3.0 }},
    BoundingBox{{ 3.0, 3.0 }, { 4.0, 4.0 }},
    BoundingBox{{ 1.2, 1.2 }, { 2.8, 2.8 }}
  };

  auto tree = OT(vpt, 3, std::nullopt, 2);

  // const member functions
  {
    [[maybe_unused]] auto const keyRoot = OT::SI::GetRootKey();
    [[maybe_unused]] auto const center = tree.GetNodeCenter(keyRoot);
    [[maybe_unused]] auto const size = tree.GetNodeSize(0);

    [[maybe_unused]] auto const allidBFS = tree.CollectAllEntitiesInBFS(keyRoot);
    [[maybe_unused]] auto const allidDFS = tree.CollectAllEntitiesInDFS(keyRoot);

    [[maybe_unused]] auto const nodeSmallest = tree.FindSmallestNode(vpt.at(11));

    [[maybe_unused]] auto const keySmallest = tree.FindSmallestNodeKey(keyRoot);
    [[maybe_unused]] auto const boxAll = tree.GetBox();
    [[maybe_unused]] auto const nDepth = OT::SI::GetDepthID(keyRoot);
    [[maybe_unused]] auto const nDepthMax = tree.GetDepthNo();
    [[maybe_unused]] auto const nodeRoot = tree.GetNode(keyRoot);
    [[maybe_unused]] auto const nodes = tree.GetNodes();
    [[maybe_unused]] auto const grid = tree.GetResolutionMax();

    [[maybe_unused]] auto const aidBoxesInRange = tree.RangeSearch(boxes[0], vpt);
    [[maybe_unused]] auto const aidBoxesInRangeF = tree.template RangeSearch<false>(boxes[0], vpt);
    [[maybe_unused]] auto const aidBoxesInRangeT = tree.template RangeSearch<true>(boxes[0], vpt);
    [[maybe_unused]] auto const aidPtsInPlane = tree.PlaneSearch(0.0, Point{ 1.0 }, 0.01, vpt);
    [[maybe_unused]] auto const idPlaneIntersected = tree.PlaneSearch(1.0, { 1.0, 0.0 }, 0.0, vpt);
    [[maybe_unused]] auto const idPlaneIntersectedP = tree.PlaneSearch(
      {
        1.0, {1.0, 0.0}
    },
      0.0,
      vpt);
    [[maybe_unused]] auto const idPlanePosSeg = tree.PlanePositiveSegmentation(1.0, { 1.0, 0.0 }, 0.0, vpt);
    [[maybe_unused]] auto const idPlanePosSegP = tree.PlanePositiveSegmentation(
      {
        1.0, {1.0, 0.0}
    },
      0.0,
      vpt);
    [[maybe_unused]] auto const idFrustum = tree.FrustumCulling(
      std::vector{
        Plane{1.0, { 1.0, 0.0 }}
    },
      0.0,
      vpt);
    [[maybe_unused]] auto const kNN = tree.GetNearestNeighbors({}, 2, vpt);

    auto vListIsAnyChild = std::vector<bool>{};
    tree.VisitNodes(
      keyRoot,
      [&vListIsAnyChild](auto const&, auto const& node) { vListIsAnyChild.emplace_back(node.IsAnyChildExist()); },
      [](auto const&, auto const&) -> bool { return true; });
  }

  // non-const member functions
  {
    tree.template EraseEntity<false>(20);
    tree.template Erase<false>(30, vpt.at(30));
    tree.Insert(3, vpt.at(11));
    tree.Update(11, vpt.at(11), vpt.at(12));
    tree.Update(3, vpt.at(12));
    tree.UpdateIndexes({
      {11, std::nullopt},
      {3,            4}
    });
    tree.template Move<IS_PARALLEL_EXEC>({ 1.0, 1.0 });
    tree.Clear();
    tree.Reset();

    tree.Init(boxes[0], 3, 12);
    tree.Reset();

    OT::template Create<IS_PARALLEL_EXEC>(tree, vpt);
  }
}

template<OrthoTree::dim_t N, bool IS_PARALLEL_EXEC, bool DO_SPLIT_PARENT_ENTITIES = true>
void testCompileBox()
{
  using Vector = OrthoTree::VectorND<N>;
  using BoundingBox = OrthoTree::BoundingBoxND<N>;
  using Plane = OrthoTree::PlaneND<N>;

  using OT = OrthoTree::TreeBoxND<N, DO_SPLIT_PARENT_ENTITIES>;

  [[maybe_unused]] auto const key = OT::SI::GetHash(2, 3);
  [[maybe_unused]] auto const noNode = OT::EstimateNodeNumber(100, 10, 3);
  [[maybe_unused]] auto const aidGrid = OT::SI::Decode(key, 3);
  [[maybe_unused]] auto const idGrid = OT::SI::Encode(aidGrid);
  [[maybe_unused]] auto const fValid = OT::SI::IsValidKey(key);
  [[maybe_unused]] auto const idChild = OT::SI::RemoveSentinelBit(key);

  auto constexpr boxes = std::array
  {
    BoundingBox{ { 0.0, 0.0 }, { 1.0, 1.0 } },
    BoundingBox{ { 1.0, 1.0 }, { 2.0, 2.0 } },
    BoundingBox{ { 2.0, 2.0 }, { 3.0, 3.0 } },
    BoundingBox{ { 3.0, 3.0 }, { 4.0, 4.0 } },
    BoundingBox{ { 1.2, 1.2 }, { 2.8, 2.8 } }
  };

  auto tree = OT(boxes, 3, std::nullopt, 2);

  // const member functions
  {
    [[maybe_unused]] auto const keyRoot = OT::SI::GetRootKey();
    [[maybe_unused]] auto const center = tree.GetNodeCenter(keyRoot);
    [[maybe_unused]] auto const size = tree.GetNodeSize(0);

    [[maybe_unused]] auto const allidBFS = tree.CollectAllEntitiesInBFS(keyRoot);
    [[maybe_unused]] auto const allidDFS = tree.CollectAllEntitiesInDFS(keyRoot);

    [[maybe_unused]] auto const nodeSmallest = tree.FindSmallestNode(boxes.back());

    [[maybe_unused]] auto const keySmallest = tree.FindSmallestNodeKey(keyRoot);
    [[maybe_unused]] auto const boxAll = tree.GetBox();
    [[maybe_unused]] auto const nDepth = OT::SI::GetDepthID(keyRoot);
    [[maybe_unused]] auto const nDepthMax = tree.GetDepthNo();
    [[maybe_unused]] auto const nodeRoot = tree.GetNode(keyRoot);
    [[maybe_unused]] auto const nodes = tree.GetNodes();
    [[maybe_unused]] auto const grid = tree.GetResolutionMax();

    [[maybe_unused]] auto const vidCollision = tree.template CollisionDetection<IS_PARALLEL_EXEC>(boxes);
    [[maybe_unused]] auto const vidCollisionTree = tree.CollisionDetection(boxes, tree, boxes);

    [[maybe_unused]] auto const aidPick = tree.PickSearch({}, boxes);
    [[maybe_unused]] auto const aidBoxesInRange = tree.RangeSearch(boxes[0], boxes);
    [[maybe_unused]] auto const aidBoxesInRangeF = tree.template RangeSearch<false>(boxes[0], boxes);
    [[maybe_unused]] auto const aidBoxesInRangeT = tree.template RangeSearch<true>(boxes[0], boxes);

    [[maybe_unused]] auto const idBoxesIntersectedAll = tree.RayIntersectedAll({}, { 1.0, 1.0 }, boxes, 0);
    [[maybe_unused]] auto const idBoxesIntersectedFirst = tree.RayIntersectedFirst({}, { 1.0, 1.0 }, boxes, 0);

    [[maybe_unused]] auto const idPlaneIntersected = tree.PlaneIntersection(1.0, { 1.0, 0.0 }, 0.0, boxes);
    [[maybe_unused]] auto const idPlaneIntersectedP = tree.PlaneIntersection({1.0, { 1.0, 0.0 }}, 0.0, boxes);
    [[maybe_unused]] auto const idPlanePosSeg = tree.PlanePositiveSegmentation(1.0, { 1.0, 0.0 }, 0.0, boxes);
    [[maybe_unused]] auto const idPlanePosSegP = tree.PlanePositiveSegmentation({1.0, { 1.0, 0.0 }}, 0.0, boxes);
    [[maybe_unused]] auto const idFrustum = tree.FrustumCulling(
      std::vector{
        Plane{1.0, Vector{ 1.0, 0.0 }}
      },
      0.0,
      boxes);

    [[maybe_unused]] auto vListIsAnyChild = std::vector<bool>{};
    tree.VisitNodes(keyRoot
      , [&vListIsAnyChild](auto const&, auto const& node) { vListIsAnyChild.emplace_back(node.IsAnyChildExist()); }
      , [](auto const&, auto const&) -> bool { return true; }
    );
  }

  // non-const member functions
  {
    tree.template EraseEntity<false>(4);
    tree.template Erase<false>(3, boxes[3]);
    tree.Insert(3, boxes[3]);
    tree.Update(2, boxes[2], boxes[3]);
    tree.Update(3, boxes[4]);
    tree.UpdateIndexes({ {1, std::nullopt }, {3, 4} });
    tree.template Move<IS_PARALLEL_EXEC>({ 1.0, 1.0 });
    tree.Clear();
    tree.Reset();

    tree.Init(boxes[0], 3, 12);
    tree.Reset();

    OT::template Create<IS_PARALLEL_EXEC>(tree, boxes);
  }
}

template<OrthoTree::dim_t N, bool IS_PARALLEL_EXEC, bool DO_SPLIT_PARENT_ENTITIES = true>
void testCompileBoxMap()
{
  using Vector = OrthoTree::VectorND<N>;
  using BoundingBox = OrthoTree::BoundingBoxND<N>;
  using Plane = OrthoTree::PlaneND<N>;

  using Map = std::unordered_map<int, BoundingBox>;
  using OT = OrthoTree::TreeBoxND<N, DO_SPLIT_PARENT_ENTITIES, OrthoTree::BaseGeometryType, Map>;

  [[maybe_unused]] auto const key = OT::SI::GetHash(2, 3);
  [[maybe_unused]] auto const noNode = OT::EstimateNodeNumber(100, 10, 3);
  [[maybe_unused]] auto const aidGrid = OT::SI::Decode(key, 3);
  [[maybe_unused]] auto const idGrid = OT::SI::Encode(aidGrid);
  [[maybe_unused]] auto const fValid = OT::SI::IsValidKey(key);
  [[maybe_unused]] auto const idChild = OT::SI::RemoveSentinelBit(key);

  auto const boxes = Map{
    { 10, BoundingBox{{ 0.0, 0.0 }, { 1.0, 1.0 }}},
    { 12, BoundingBox{{ 1.0, 1.0 }, { 2.0, 2.0 }}},
    { 17, BoundingBox{{ 2.0, 2.0 }, { 3.0, 3.0 }}},
    { 13, BoundingBox{{ 3.0, 3.0 }, { 4.0, 4.0 }}},
    { 11, BoundingBox{{ 1.2, 1.2 }, { 2.8, 2.8 }}}
  };

  auto tree = OT(boxes, 3, std::nullopt, 2);

  // const member functions
  {
    [[maybe_unused]] auto const keyRoot = OT::SI::GetRootKey();
    [[maybe_unused]] auto const center = tree.GetNodeCenter(keyRoot);
    [[maybe_unused]] auto const size = tree.GetNodeSize(0);

    [[maybe_unused]] auto const allidBFS = tree.CollectAllEntitiesInBFS(keyRoot);
    [[maybe_unused]] auto const allidDFS = tree.CollectAllEntitiesInDFS(keyRoot);

    [[maybe_unused]] auto const nodeSmallest = tree.FindSmallestNode(boxes.at(11));

    [[maybe_unused]] auto const keySmallest = tree.FindSmallestNodeKey(keyRoot);
    [[maybe_unused]] auto const boxAll = tree.GetBox();
    [[maybe_unused]] auto const nDepth = OT::SI::GetDepthID(keyRoot);
    [[maybe_unused]] auto const nDepthMax = tree.GetDepthNo();
    [[maybe_unused]] auto const nodeRoot = tree.GetNode(keyRoot);
    [[maybe_unused]] auto const nodes = tree.GetNodes();
    [[maybe_unused]] auto const grid = tree.GetResolutionMax();

    [[maybe_unused]] auto const vidCollision = tree.template CollisionDetection<IS_PARALLEL_EXEC>(boxes);
    [[maybe_unused]] auto const vidCollisionTree = tree.CollisionDetection(boxes, tree, boxes);

    [[maybe_unused]] auto const aidPick = tree.PickSearch({}, boxes);
    [[maybe_unused]] auto const aidBoxesInRange = tree.RangeSearch(boxes.at(10), boxes);
    [[maybe_unused]] auto const aidBoxesInRangeF = tree.template RangeSearch<false>(boxes.at(10), boxes);
    [[maybe_unused]] auto const aidBoxesInRangeT = tree.template RangeSearch<true>(boxes.at(10), boxes);

    [[maybe_unused]] auto const idBoxesIntersectedAll = tree.RayIntersectedAll({}, { 1.0, 1.0 }, boxes, 0);
    [[maybe_unused]] auto const idBoxesIntersectedFirst = tree.RayIntersectedFirst({}, { 1.0, 1.0 }, boxes, 0);

    [[maybe_unused]] auto const idPlaneIntersected = tree.PlaneIntersection(1.0, { 1.0, 0.0 }, 0.0, boxes);
    [[maybe_unused]] auto const idPlaneIntersectedP = tree.PlaneIntersection(
      {
        1.0, {1.0, 0.0}
    },
      0.0,
      boxes);
    [[maybe_unused]] auto const idPlanePosSeg = tree.PlanePositiveSegmentation(1.0, { 1.0, 0.0 }, 0.0, boxes);
    [[maybe_unused]] auto const idPlanePosSegP = tree.PlanePositiveSegmentation(
      {
        1.0, {1.0, 0.0}
    },
      0.0,
      boxes);
    [[maybe_unused]] auto const idFrustum = tree.FrustumCulling(
      std::vector{
        Plane{1.0, Vector{ 1.0, 0.0 }}
    },
      0.0,
      boxes);

    [[maybe_unused]] auto vListIsAnyChild = std::vector<bool>{};
    tree.VisitNodes(
      keyRoot,
      [&vListIsAnyChild](auto const&, auto const& node) { vListIsAnyChild.emplace_back(node.IsAnyChildExist()); },
      [](auto const&, auto const&) -> bool { return true; });
  }

  // non-const member functions
  {
    tree.template EraseEntity<false>(12);
    tree.template Erase<false>(10, boxes.at(10));
    tree.Insert(23, boxes.at(17));
    tree.Update(11, boxes.at(11), boxes.at(17));
    tree.Update(23, boxes.at(13));
    tree.UpdateIndexes({
      {17, std::nullopt},
      {23,            22}
    });
    tree.template Move<IS_PARALLEL_EXEC>({ 1.0, 1.0 });
    tree.Clear();
    tree.Reset();

    tree.Init(boxes.at(13), 3, 12);
    tree.Reset();

    OT::template Create<IS_PARALLEL_EXEC>(tree, boxes);
  }
}


template<OrthoTree::dim_t N, bool IS_PARALLEL_EXEC>
void testCompilePointC()
{
  using Point = OrthoTree::PointND<N>;
  using BoundingBox = OrthoTree::BoundingBoxND<N>;
  using Plane = OrthoTree::PlaneND<N>;
  using OT = OrthoTree::TreePointContainerND<N>;

  auto constexpr vpt = std::array{ Point{ 0.0 }, Point{ 1.0 }, Point{ 2.0 }, Point{ 3.0 }, Point{ 4.0 } };

  auto constexpr boxes = std::array
  {
    BoundingBox{ { 0.0, 0.0 }, { 1.0, 1.0 } },
    BoundingBox{ { 1.0, 1.0 }, { 2.0, 2.0 } },
    BoundingBox{ { 2.0, 2.0 }, { 3.0, 3.0 } },
    BoundingBox{ { 3.0, 3.0 }, { 4.0, 4.0 } },
    BoundingBox{ { 1.2, 1.2 }, { 2.8, 2.8 } }
  };

  auto tree = OT(vpt, 3, std::nullopt, 2, false);
#ifdef __cpp_lib_execution
  [[maybe_unused]] auto treePar = OT(vpt, 3, std::nullopt, 2, true);
#endif
    
  // const member functions
  {
    [[maybe_unused]] auto const& treeCore = tree.GetCore();
    [[maybe_unused]] auto const& data = tree.GetData();

    [[maybe_unused]] auto const aidBoxesInRange = tree.RangeSearch(boxes[0]);
    [[maybe_unused]] auto const aidBoxesInRangeF = tree.template RangeSearch<false>(boxes[0]);
    [[maybe_unused]] auto const aidBoxesInRangeT = tree.template RangeSearch<true>(boxes[0]);

    [[maybe_unused]] auto const kNN = tree.GetNearestNeighbors({}, 2);

    [[maybe_unused]] auto const idPlaneIntersected = tree.PlaneSearch(1.0, { 1.0, 0.0 }, 0.0);
    [[maybe_unused]] auto const idPlaneIntersectedP = tree.PlaneSearch({1.0, { 1.0, 0.0 }}, 0.0);
    [[maybe_unused]] auto const idPlanePosSeg = tree.PlanePositiveSegmentation(1.0, { 1.0, 0.0 }, 0.0);
    [[maybe_unused]] auto const idPlanePosSegP = tree.PlanePositiveSegmentation({1.0, { 1.0, 0.0 }}, 0.0);
    [[maybe_unused]] auto const idFrustum = tree.FrustumCulling(
      std::vector{
        Plane{1.0, { 1.0, 0.0 }},
        Plane{1.0, { 0.0, 1.0 }}
      },
      0.0);
  }

  // non-const member functions
  {
    tree.Add(vpt[2]);
    tree.Erase(2);
    tree.Update(3, vpt[4]);
    tree.template Move<IS_PARALLEL_EXEC>({ 1.0, 1.0 });
    tree.Clear();
    tree.Reset();

    tree.Init(boxes[0], 3, 12);
    tree.Reset();

    tree = OT::template Create<IS_PARALLEL_EXEC>(vpt);
    tree = OT::template Create<IS_PARALLEL_EXEC>(std::vector{ Point{ 0.0 }, Point{ 1.0 }, Point{ 2.0 }, Point{ 3.0 }, Point{ 4.0 } });
  }
}

template<OrthoTree::dim_t N, bool IS_PARALLEL_EXEC>
void testCompilePointMapC()
{
  using Point = OrthoTree::PointND<N>;
  using BoundingBox = OrthoTree::BoundingBoxND<N>;
  using Plane = OrthoTree::PlaneND<N>;
  using Map = std::unordered_map<int, Point>;
  using OT = OrthoTree::TreePointContainerND<N, OrthoTree::BaseGeometryType, Map>;

  auto const vpt = Map{
    { 10, Point{ 0.0 } },
    { 20, Point{ 1.0 } },
    { 30, Point{ 2.0 } },
    { 40, Point{ 3.0 } },
    { 50, Point{ 4.0 } }
  };

  auto constexpr boxes = std::array{
    BoundingBox{{ 0.0, 0.0 }, { 1.0, 1.0 }},
    BoundingBox{{ 1.0, 1.0 }, { 2.0, 2.0 }},
    BoundingBox{{ 2.0, 2.0 }, { 3.0, 3.0 }},
    BoundingBox{{ 3.0, 3.0 }, { 4.0, 4.0 }},
    BoundingBox{{ 1.2, 1.2 }, { 2.8, 2.8 }}
  };

  auto tree = OT(vpt, 3, std::nullopt, 2, false);
#ifdef __cpp_lib_execution
  [[maybe_unused]] auto treePar = OT(vpt, 3, std::nullopt, 2, true);
#endif
  // const member functions
  {
    [[maybe_unused]] auto const& treeCore = tree.GetCore();
    [[maybe_unused]] auto const& data = tree.GetData();

    [[maybe_unused]] auto const aidBoxesInRange = tree.RangeSearch(boxes[0]);
    [[maybe_unused]] auto const aidBoxesInRangeF = tree.template RangeSearch<false>(boxes[0]);
    [[maybe_unused]] auto const aidBoxesInRangeT = tree.template RangeSearch<true>(boxes[0]);

    [[maybe_unused]] auto const kNN = tree.GetNearestNeighbors({}, 2);

    [[maybe_unused]] auto const idPlaneIntersected = tree.PlaneSearch(1.0, { 1.0, 0.0 }, 0.0);
    [[maybe_unused]] auto const idPlaneIntersectedP = tree.PlaneSearch(
      {
        1.0, {1.0, 0.0}
    },
      0.0);
    [[maybe_unused]] auto const idPlanePosSeg = tree.PlanePositiveSegmentation(1.0, { 1.0, 0.0 }, 0.0);
    [[maybe_unused]] auto const idPlanePosSegP = tree.PlanePositiveSegmentation(
      {
        1.0, {1.0, 0.0}
    },
      0.0);
    [[maybe_unused]] auto const idFrustum = tree.FrustumCulling(
      std::vector{
        Plane{1.0, { 1.0, 0.0 }},
        Plane{1.0, { 0.0, 1.0 }}
    },
      0.0);
  }

  // non-const member functions
  {
    tree.Add(60, vpt.at(20));
    tree.Erase(20);
    tree.Update(30, vpt.at(40));
    tree.template Move<IS_PARALLEL_EXEC>({ 1.0, 1.0 });
    tree.Clear();
    tree.Reset();

    tree.Init(boxes[0], 3, 12);
    tree.Reset();

    tree = OT::template Create<IS_PARALLEL_EXEC>(vpt);
    tree = OT::template Create<IS_PARALLEL_EXEC>(
      Map{
        { 11, Point{ 0.0 } },
        { 21, Point{ 1.0 } },
        { 31, Point{ 2.0 } },
        { 41, Point{ 3.0 } },
        { 51, Point{ 4.0 } }
      },
      4);
  }
}


template<OrthoTree::dim_t N, bool IS_PARALLEL_EXEC, bool DO_SPLIT_PARENT_ENTITIES = true>
void testCompileBoxC()
{
  using BoundingBox = OrthoTree::BoundingBoxND<N>;
  using Plane = OrthoTree::PlaneND<N>;
  using OT = OrthoTree::TreeBoxContainerND<N, DO_SPLIT_PARENT_ENTITIES>;

  auto constexpr boxes = std::array
  {
    BoundingBox{ { 0.0, 0.0 }, { 1.0, 1.0 } },
    BoundingBox{ { 1.0, 1.0 }, { 2.0, 2.0 } },
    BoundingBox{ { 2.0, 2.0 }, { 3.0, 3.0 } },
    BoundingBox{ { 3.0, 3.0 }, { 4.0, 4.0 } },
    BoundingBox{ { 1.2, 1.2 }, { 2.8, 2.8 } }
  };

  auto tree = OT(boxes, 3, std::nullopt, 2, false);
#ifdef __cpp_lib_execution
  [[maybe_unused]] auto treePar = OT(boxes, 3, std::nullopt, 2, true);
#endif
    
  // const member functions
  {
    [[maybe_unused]] auto const& treeCore = tree.GetCore();
    [[maybe_unused]] auto const& data = tree.GetData();

    [[maybe_unused]] auto const aidPick = tree.PickSearch({});
    [[maybe_unused]] auto const aidBoxesInRange = tree.RangeSearch(boxes[0]);
    [[maybe_unused]] auto const aidBoxesInRangeF = tree.template RangeSearch<false>(boxes[0]);
    [[maybe_unused]] auto const aidBoxesInRangeT = tree.template RangeSearch<true>(boxes[0]);

    [[maybe_unused]] auto const vidCollision = tree.template CollisionDetection<IS_PARALLEL_EXEC>();
    [[maybe_unused]] auto const vidCollisionTree = tree.CollisionDetection(tree);
   
    [[maybe_unused]] auto const idBoxesIntersectedAll = tree.RayIntersectedAll({}, { 1.0, 1.0 }, 0);
    [[maybe_unused]] auto const idBoxesIntersectedFirst = tree.RayIntersectedFirst({}, { 1.0, 1.0 }, 0);

    [[maybe_unused]] auto const idPlaneIntersected = tree.PlaneIntersection(1.0, { 1.0, 0.0 }, 0.0);
    [[maybe_unused]] auto const idPlaneIntersectedP = tree.PlaneIntersection({1.0, { 1.0, 0.0 }}, 0.0);
    [[maybe_unused]] auto const idPlanePosSeg = tree.PlanePositiveSegmentation(1.0, { 1.0, 0.0 }, 0.0);
    [[maybe_unused]] auto const idPlanePosSegP = tree.PlanePositiveSegmentation({1.0, { 1.0, 0.0 }}, 0.0);
    [[maybe_unused]] auto const idFrustum = tree.FrustumCulling(
      std::vector{
        Plane{1.0, { 1.0, 0.0 }},
        Plane{1.0, { 0.0, 1.0 }}
      },
      0.0);

  }

  // non-const member functions
  {
    tree.Add(boxes[0]);
    tree.Erase(2);
    tree.Update(3, boxes[4]);
    tree.template Move<IS_PARALLEL_EXEC>({ 1.0, 1.0 });
    tree.Clear();
    tree.Reset();

    tree.Init(boxes[0], 3, 12);
    tree.Reset();

    tree = OT::template Create<IS_PARALLEL_EXEC>(boxes);
    tree = OT::template Create<IS_PARALLEL_EXEC>(
      std::vector
      {
        BoundingBox{ { 0.0, 0.0 }, { 1.0, 1.0 } },
        BoundingBox{ { 1.0, 1.0 }, { 2.0, 2.0 } },
        BoundingBox{ { 2.0, 2.0 }, { 3.0, 3.0 } },
        BoundingBox{ { 3.0, 3.0 }, { 4.0, 4.0 } },
        BoundingBox{ { 1.2, 1.2 }, { 2.8, 2.8 } }
      }
    );

  }

}

template<OrthoTree::dim_t N, bool IS_PARALLEL_EXEC, bool DO_SPLIT_PARENT_ENTITIES = true>
void testCompileBoxMapC()
{
  using BoundingBox = OrthoTree::BoundingBoxND<N>;
  using Plane = OrthoTree::PlaneND<N>;
  using Map = std::unordered_map<int, BoundingBox>;
  using OT = OrthoTree::TreeBoxContainerND<N, DO_SPLIT_PARENT_ENTITIES, OrthoTree::BaseGeometryType, Map>;

  auto const boxes = Map{
    { 5, BoundingBox{{ 0.0, 0.0 }, { 1.0, 1.0 }}},
    { 10, BoundingBox{{ 1.0, 1.0 }, { 2.0, 2.0 }}},
    { 20, BoundingBox{{ 2.0, 2.0 }, { 3.0, 3.0 }}},
    { 40, BoundingBox{{ 3.0, 3.0 }, { 4.0, 4.0 }}},
    { 30, BoundingBox{{ 1.2, 1.2 }, { 2.8, 2.8 }}}
  };

  auto tree = OT(boxes, std::nullopt, std::nullopt, 2, false);

#ifdef __cpp_lib_execution
  [[maybe_unused]] auto treePar = OT(boxes, std::nullopt, std::nullopt, 2, true);
#endif
    
  // const member functions
  {
    [[maybe_unused]] auto const& treeCore = tree.GetCore();
    [[maybe_unused]] auto const& data = tree.GetData();

    [[maybe_unused]] auto const aidPick = tree.PickSearch({});
    [[maybe_unused]] auto const aidBoxesInRange = tree.RangeSearch(boxes.at(30));
    [[maybe_unused]] auto const aidBoxesInRangeF = tree.template RangeSearch<false>(boxes.at(30));
    [[maybe_unused]] auto const aidBoxesInRangeT = tree.template RangeSearch<true>(boxes.at(30));

    [[maybe_unused]] auto const vidCollision = tree.template CollisionDetection<IS_PARALLEL_EXEC>();
    [[maybe_unused]] auto const vidCollisionTree = tree.CollisionDetection(tree);

    [[maybe_unused]] auto const idBoxesIntersectedAll = tree.RayIntersectedAll({}, { 1.0, 1.0 }, 0);
    [[maybe_unused]] auto const idBoxesIntersectedFirst = tree.RayIntersectedFirst({}, { 1.0, 1.0 }, 0);

    [[maybe_unused]] auto const idPlaneIntersected = tree.PlaneIntersection(1.0, { 1.0, 0.0 }, 0.0);
    [[maybe_unused]] auto const idPlaneIntersectedP = tree.PlaneIntersection(
      {
        1.0, {1.0, 0.0}
    },
      0.0);
    [[maybe_unused]] auto const idPlanePosSeg = tree.PlanePositiveSegmentation(1.0, { 1.0, 0.0 }, 0.0);
    [[maybe_unused]] auto const idPlanePosSegP = tree.PlanePositiveSegmentation(
      {
        1.0, {1.0, 0.0}
    },
      0.0);
    [[maybe_unused]] auto const idFrustum = tree.FrustumCulling(
      std::vector{
        Plane{1.0, { 1.0, 0.0 }},
        Plane{1.0, { 0.0, 1.0 }}
    },
      0.0);
  }

  // non-const member functions
  {
    tree.Add(11, boxes.at(10));
    tree.Erase(20);
    tree.Update(40, boxes.at(30));
    tree.template Move<IS_PARALLEL_EXEC>({ 1.0, 1.0 });
    tree.Clear();
    tree.Reset();

    tree.Init(boxes.at(5), 3, 12);
    tree.Reset();

    tree = OT::template Create<IS_PARALLEL_EXEC>(boxes);
    tree = OT::template Create<IS_PARALLEL_EXEC>(
      Map{
        { 10, BoundingBox{{ 0.0, 0.0 }, { 1.0, 1.0 }}},
        { 15, BoundingBox{{ 1.0, 1.0 }, { 2.0, 2.0 }}},
        { 16, BoundingBox{{ 2.0, 2.0 }, { 3.0, 3.0 }}},
        { 12, BoundingBox{{ 3.0, 3.0 }, { 4.0, 4.0 }}},
        { 11, BoundingBox{{ 1.2, 1.2 }, { 2.8, 2.8 }}}
      },
      4);
  }
}


template<bool IS_PARALLEL_EXEC, bool DO_SPLIT_PARENT_ENTITIES = true>
void testCompileBoxBatchDimension()
{
  auto constexpr isPlatform64 = sizeof(std::size_t) == 8;

  // Core types
  {
    testCompilePoint<2, IS_PARALLEL_EXEC>();
    testCompilePoint<3, IS_PARALLEL_EXEC>();
    testCompilePoint<4, IS_PARALLEL_EXEC>();
    testCompilePoint<5, IS_PARALLEL_EXEC>();
    testCompilePoint<6, IS_PARALLEL_EXEC>();
    testCompilePoint<7, IS_PARALLEL_EXEC>();
    testCompilePoint<8, IS_PARALLEL_EXEC>();
    testCompilePoint<12, IS_PARALLEL_EXEC>();
    testCompilePoint<16, IS_PARALLEL_EXEC>();
    testCompilePoint<31, IS_PARALLEL_EXEC>();
    if constexpr (isPlatform64)
    {
      testCompilePoint<32, IS_PARALLEL_EXEC>();
      testCompilePoint<63, IS_PARALLEL_EXEC>();
    }
    testCompilePointMap<2, IS_PARALLEL_EXEC>();
    testCompilePointMap<3, IS_PARALLEL_EXEC>();

    testCompileBox<2, IS_PARALLEL_EXEC, DO_SPLIT_PARENT_ENTITIES>();
    testCompileBox<3, IS_PARALLEL_EXEC, DO_SPLIT_PARENT_ENTITIES>();
    testCompileBox<4, IS_PARALLEL_EXEC, DO_SPLIT_PARENT_ENTITIES>();
    testCompileBox<5, IS_PARALLEL_EXEC, DO_SPLIT_PARENT_ENTITIES>();
    testCompileBox<6, IS_PARALLEL_EXEC, DO_SPLIT_PARENT_ENTITIES>();
    testCompileBox<7, IS_PARALLEL_EXEC, DO_SPLIT_PARENT_ENTITIES>();
    testCompileBox<8, IS_PARALLEL_EXEC, DO_SPLIT_PARENT_ENTITIES>();
    testCompileBox<12, IS_PARALLEL_EXEC, DO_SPLIT_PARENT_ENTITIES>();
    testCompileBox<16, IS_PARALLEL_EXEC, DO_SPLIT_PARENT_ENTITIES>();
    testCompileBox<31, IS_PARALLEL_EXEC, DO_SPLIT_PARENT_ENTITIES>();
    if constexpr (isPlatform64)
    {
      testCompileBox<32, IS_PARALLEL_EXEC, DO_SPLIT_PARENT_ENTITIES>();
      testCompileBox<63, IS_PARALLEL_EXEC, DO_SPLIT_PARENT_ENTITIES>();
    }

    testCompileBoxMap<2, IS_PARALLEL_EXEC, DO_SPLIT_PARENT_ENTITIES>();
    testCompileBoxMap<3, IS_PARALLEL_EXEC, DO_SPLIT_PARENT_ENTITIES>();
  }
  
  // Container types
  {
    testCompilePointC<2, IS_PARALLEL_EXEC>();
    testCompilePointC<3, IS_PARALLEL_EXEC>();
    testCompilePointC<4, IS_PARALLEL_EXEC>();
    testCompilePointC<5, IS_PARALLEL_EXEC>();
    testCompilePointC<6, IS_PARALLEL_EXEC>();
    testCompilePointC<7, IS_PARALLEL_EXEC>();
    testCompilePointC<8, IS_PARALLEL_EXEC>();
    testCompilePointC<12, IS_PARALLEL_EXEC>();
    testCompilePointC<16, IS_PARALLEL_EXEC>();
    testCompilePointC<31, IS_PARALLEL_EXEC>();
    if constexpr (isPlatform64)
    {
      testCompilePointC<32, IS_PARALLEL_EXEC>();
      testCompilePointC<63, IS_PARALLEL_EXEC>();
    }
    testCompilePointMapC<2, IS_PARALLEL_EXEC>();
    testCompilePointMapC<3, IS_PARALLEL_EXEC>();

    testCompileBoxC<2, IS_PARALLEL_EXEC, DO_SPLIT_PARENT_ENTITIES>();
    testCompileBoxC<3, IS_PARALLEL_EXEC, DO_SPLIT_PARENT_ENTITIES>();
    testCompileBoxC<4, IS_PARALLEL_EXEC, DO_SPLIT_PARENT_ENTITIES>();
    testCompileBoxC<5, IS_PARALLEL_EXEC, DO_SPLIT_PARENT_ENTITIES>();
    testCompileBoxC<6, IS_PARALLEL_EXEC, DO_SPLIT_PARENT_ENTITIES>();
    testCompileBoxC<7, IS_PARALLEL_EXEC, DO_SPLIT_PARENT_ENTITIES>();
    testCompileBoxC<8, IS_PARALLEL_EXEC, DO_SPLIT_PARENT_ENTITIES>();
    testCompileBoxC<12, IS_PARALLEL_EXEC, DO_SPLIT_PARENT_ENTITIES>();
    testCompileBoxC<16, IS_PARALLEL_EXEC, DO_SPLIT_PARENT_ENTITIES>();
    testCompileBoxC<31, IS_PARALLEL_EXEC, DO_SPLIT_PARENT_ENTITIES>();

    if constexpr (isPlatform64)
    {
      testCompileBoxC<32, IS_PARALLEL_EXEC, DO_SPLIT_PARENT_ENTITIES>();
      testCompileBoxC<63, IS_PARALLEL_EXEC, DO_SPLIT_PARENT_ENTITIES>();
    }

    testCompileBoxMapC<2, IS_PARALLEL_EXEC, DO_SPLIT_PARENT_ENTITIES>();
    testCompileBoxMapC<3, IS_PARALLEL_EXEC, DO_SPLIT_PARENT_ENTITIES>();
  }
}


template<bool DO_SPLIT_PARENT_ENTITIES = true>
void testCompileBoxBatchExPol()
{
#ifdef __cpp_lib_execution
  testCompileBoxBatchDimension<false, DO_SPLIT_PARENT_ENTITIES>();
  testCompileBoxBatchDimension<true, DO_SPLIT_PARENT_ENTITIES>();
#else
  testCompileBoxBatchDimension<false, DO_SPLIT_PARENT_ENTITIES>();
#endif
}


inline void testCompileBoxBatchSplitStrategy()
{
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
