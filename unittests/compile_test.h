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

  auto const key = OT::SI::GetHash(2, 3);
  auto const noNode = OT::EstimateNodeNumber(100, 10, 3);
  auto const aidGrid = OT::SI::Decode(key, 3);
  auto const idGrid = OT::SI::Encode(aidGrid);
  auto const fValid = OT::SI::IsValidKey(key);
  auto const idChild = OT::SI::RemoveSentinelBit(key);

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
    auto const center = tree.GetNodeCenter(keyRoot);
    auto const size = tree.GetNodeSize(0);

    auto const allidBFS = tree.CollectAllEntitiesInBFS(keyRoot);
    auto const allidDFS = tree.CollectAllEntitiesInDFS(keyRoot);

    auto const nodeSmallest = tree.FindSmallestNode(vpt.back());

    auto const keySmallest = tree.FindSmallestNodeKey(keyRoot);
    auto const boxAll = tree.GetBox();
    auto const nDepth = OT::SI::GetDepthID(keyRoot);
    auto const nDepthMax = tree.GetDepthMax();
    auto const nodeRoot = tree.GetNode(keyRoot);
    auto const nodes = tree.GetNodes();
    auto const grid = tree.GetResolutionMax();

    auto const aidBoxesInRange = tree.RangeSearch(boxes[0], vpt);
    auto const aidBoxesInRangeF = tree.template RangeSearch<false>(boxes[0], vpt);
    auto const aidBoxesInRangeT = tree.template RangeSearch<true>(boxes[0], vpt);
    auto const aidPtsInPlane = tree.PlaneSearch(0.0, Point{ 1.0 }, 0.01, vpt);
    auto const idPlaneIntersected = tree.PlaneSearch(1.0, { 1.0, 0.0 }, 0.0, vpt);
    auto const idPlaneIntersectedP = tree.PlaneSearch({1.0, { 1.0, 0.0 }}, 0.0, vpt);
    auto const idPlanePosSeg = tree.PlanePositiveSegmentation(1.0, { 1.0, 0.0 }, 0.0, vpt);
    auto const idPlanePosSegP = tree.PlanePositiveSegmentation({1.0, { 1.0, 0.0 }}, 0.0, vpt);
    auto const idFrustum = tree.FrustumCulling(
      std::vector{
        Plane{1.0, { 1.0, 0.0 }}
      },
      0.0,
      vpt);
    auto const kNN = tree.GetNearestNeighbors({}, 2, vpt);

    auto vListIsAnyChild = std::vector<bool>{};
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

  auto const key = OT::SI::GetHash(2, 3);
  auto const noNode = OT::EstimateNodeNumber(100, 10, 3);
  auto const aidGrid = OT::SI::Decode(key, 3);
  auto const idGrid = OT::SI::Encode(aidGrid);
  auto const fValid = OT::SI::IsValidKey(key);
  auto const idChild = OT::SI::RemoveSentinelBit(key);

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
    auto const keyRoot = OT::SI::GetRootKey();
    auto const center = tree.GetNodeCenter(keyRoot);
    auto const size = tree.GetNodeSize(0);

    auto const allidBFS = tree.CollectAllEntitiesInBFS(keyRoot);
    auto const allidDFS = tree.CollectAllEntitiesInDFS(keyRoot);

    auto const nodeSmallest = tree.FindSmallestNode(vpt.at(11));

    auto const keySmallest = tree.FindSmallestNodeKey(keyRoot);
    auto const boxAll = tree.GetBox();
    auto const nDepth = OT::SI::GetDepthID(keyRoot);
    auto const nDepthMax = tree.GetDepthMax();
    auto const nodeRoot = tree.GetNode(keyRoot);
    auto const nodes = tree.GetNodes();
    auto const grid = tree.GetResolutionMax();

    auto const aidBoxesInRange = tree.RangeSearch(boxes[0], vpt);
    auto const aidBoxesInRangeF = tree.template RangeSearch<false>(boxes[0], vpt);
    auto const aidBoxesInRangeT = tree.template RangeSearch<true>(boxes[0], vpt);
    auto const aidPtsInPlane = tree.PlaneSearch(0.0, Point{ 1.0 }, 0.01, vpt);
    auto const idPlaneIntersected = tree.PlaneSearch(1.0, { 1.0, 0.0 }, 0.0, vpt);
    auto const idPlaneIntersectedP = tree.PlaneSearch(
      {
        1.0, {1.0, 0.0}
    },
      0.0,
      vpt);
    auto const idPlanePosSeg = tree.PlanePositiveSegmentation(1.0, { 1.0, 0.0 }, 0.0, vpt);
    auto const idPlanePosSegP = tree.PlanePositiveSegmentation(
      {
        1.0, {1.0, 0.0}
    },
      0.0,
      vpt);
    auto const idFrustum = tree.FrustumCulling(
      std::vector{
        Plane{1.0, { 1.0, 0.0 }}
    },
      0.0,
      vpt);
    auto const kNN = tree.GetNearestNeighbors({}, 2, vpt);

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

template<OrthoTree::dim_t N, bool IS_PARALLEL_EXEC, uint32_t nSplitStrategyAdditionalDepth = 2>
void testCompileBox()
{
  using Vector = OrthoTree::VectorND<N>;
  using BoundingBox = OrthoTree::BoundingBoxND<N>;
  using Plane = OrthoTree::PlaneND<N>;

  using OT = OrthoTree::TreeBoxND<N, nSplitStrategyAdditionalDepth>;

  auto const key = OT::SI::GetHash(2, 3);
  auto const noNode = OT::EstimateNodeNumber(100, 10, 3);
  auto const aidGrid = OT::SI::Decode(key, 3);
  auto const idGrid = OT::SI::Encode(aidGrid);
  auto const fValid = OT::SI::IsValidKey(key);
  auto const idChild = OT::SI::RemoveSentinelBit(key);

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
    auto const keyRoot = OT::SI::GetRootKey();
    auto const center = tree.GetNodeCenter(keyRoot);
    auto const size = tree.GetNodeSize(0);

    auto const allidBFS = tree.CollectAllEntitiesInBFS(keyRoot);
    auto const allidDFS = tree.CollectAllEntitiesInDFS(keyRoot);

    auto const nodeSmallest = tree.FindSmallestNode(boxes.back());

    auto const keySmallest = tree.FindSmallestNodeKey(keyRoot);
    auto const boxAll = tree.GetBox();
    auto const nDepth = OT::SI::GetDepthID(keyRoot);
    auto const nDepthMax = tree.GetDepthMax();
    auto const nodeRoot = tree.GetNode(keyRoot);
    auto const nodes = tree.GetNodes();
    auto const grid = tree.GetResolutionMax();

    auto const vidCollision = tree.template CollisionDetection<IS_PARALLEL_EXEC>(boxes);
    auto const vidCollisionTree = tree.CollisionDetection(boxes, tree, boxes);

    auto const aidPick = tree.PickSearch({}, boxes);
    auto const aidBoxesInRange = tree.RangeSearch(boxes[0], boxes);
    auto const aidBoxesInRangeF = tree.template RangeSearch<false>(boxes[0], boxes);
    auto const aidBoxesInRangeT = tree.template RangeSearch<true>(boxes[0], boxes);

    auto const idBoxesIntersectedAll = tree.RayIntersectedAll({}, { 1.0, 1.0 }, boxes, 0);
    auto const idBoxesIntersectedFirst = tree.RayIntersectedFirst({}, { 1.0, 1.0 }, boxes, 0);

    auto const idPlaneIntersected = tree.PlaneIntersection(1.0, { 1.0, 0.0 }, 0.0, boxes);
    auto const idPlaneIntersectedP = tree.PlaneIntersection({1.0, { 1.0, 0.0 }}, 0.0, boxes);
    auto const idPlanePosSeg = tree.PlanePositiveSegmentation(1.0, { 1.0, 0.0 }, 0.0, boxes);
    auto const idPlanePosSegP = tree.PlanePositiveSegmentation({1.0, { 1.0, 0.0 }}, 0.0, boxes);
    auto const idFrustum = tree.FrustumCulling(
      std::vector{
        Plane{1.0, Vector{ 1.0, 0.0 }}
      },
      0.0,
      boxes);

    auto vListIsAnyChild = std::vector<bool>{};
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

template<OrthoTree::dim_t N, bool IS_PARALLEL_EXEC, uint32_t nSplitStrategyAdditionalDepth = 2>
void testCompileBoxMap()
{
  using Vector = OrthoTree::VectorND<N>;
  using BoundingBox = OrthoTree::BoundingBoxND<N>;
  using Plane = OrthoTree::PlaneND<N>;

  using Map = std::unordered_map<int, BoundingBox>;
  using OT = OrthoTree::TreeBoxND<N, nSplitStrategyAdditionalDepth, OrthoTree::BaseGeometryType, Map>;

  auto const key = OT::SI::GetHash(2, 3);
  auto const noNode = OT::EstimateNodeNumber(100, 10, 3);
  auto const aidGrid = OT::SI::Decode(key, 3);
  auto const idGrid = OT::SI::Encode(aidGrid);
  auto const fValid = OT::SI::IsValidKey(key);
  auto const idChild = OT::SI::RemoveSentinelBit(key);

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
    auto const keyRoot = OT::SI::GetRootKey();
    auto const center = tree.GetNodeCenter(keyRoot);
    auto const size = tree.GetNodeSize(0);

    auto const allidBFS = tree.CollectAllEntitiesInBFS(keyRoot);
    auto const allidDFS = tree.CollectAllEntitiesInDFS(keyRoot);

    auto const nodeSmallest = tree.FindSmallestNode(boxes.at(11));

    auto const keySmallest = tree.FindSmallestNodeKey(keyRoot);
    auto const boxAll = tree.GetBox();
    auto const nDepth = OT::SI::GetDepthID(keyRoot);
    auto const nDepthMax = tree.GetDepthMax();
    auto const nodeRoot = tree.GetNode(keyRoot);
    auto const nodes = tree.GetNodes();
    auto const grid = tree.GetResolutionMax();

    auto const vidCollision = tree.template CollisionDetection<IS_PARALLEL_EXEC>(boxes);
    auto const vidCollisionTree = tree.CollisionDetection(boxes, tree, boxes);

    auto const aidPick = tree.PickSearch({}, boxes);
    auto const aidBoxesInRange = tree.RangeSearch(boxes.at(10), boxes);
    auto const aidBoxesInRangeF = tree.template RangeSearch<false>(boxes.at(10), boxes);
    auto const aidBoxesInRangeT = tree.template RangeSearch<true>(boxes.at(10), boxes);

    auto const idBoxesIntersectedAll = tree.RayIntersectedAll({}, { 1.0, 1.0 }, boxes, 0);
    auto const idBoxesIntersectedFirst = tree.RayIntersectedFirst({}, { 1.0, 1.0 }, boxes, 0);

    auto const idPlaneIntersected = tree.PlaneIntersection(1.0, { 1.0, 0.0 }, 0.0, boxes);
    auto const idPlaneIntersectedP = tree.PlaneIntersection(
      {
        1.0, {1.0, 0.0}
    },
      0.0,
      boxes);
    auto const idPlanePosSeg = tree.PlanePositiveSegmentation(1.0, { 1.0, 0.0 }, 0.0, boxes);
    auto const idPlanePosSegP = tree.PlanePositiveSegmentation(
      {
        1.0, {1.0, 0.0}
    },
      0.0,
      boxes);
    auto const idFrustum = tree.FrustumCulling(
      std::vector{
        Plane{1.0, Vector{ 1.0, 0.0 }}
    },
      0.0,
      boxes);

    auto vListIsAnyChild = std::vector<bool>{};
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
    auto const& treeCore = tree.GetCore();
    auto const& data = tree.GetData();

    auto const aidBoxesInRange = tree.RangeSearch(boxes[0]);
    auto const aidBoxesInRangeF = tree.template RangeSearch<false>(boxes[0]);
    auto const aidBoxesInRangeT = tree.template RangeSearch<true>(boxes[0]);

    auto const kNN = tree.GetNearestNeighbors({}, 2);

    auto const idPlaneIntersected = tree.PlaneSearch(1.0, { 1.0, 0.0 }, 0.0);
    auto const idPlaneIntersectedP = tree.PlaneSearch({1.0, { 1.0, 0.0 }}, 0.0);
    auto const idPlanePosSeg = tree.PlanePositiveSegmentation(1.0, { 1.0, 0.0 }, 0.0);
    auto const idPlanePosSegP = tree.PlanePositiveSegmentation({1.0, { 1.0, 0.0 }}, 0.0);
    auto const idFrustum = tree.FrustumCulling(
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
    auto const& treeCore = tree.GetCore();
    auto const& data = tree.GetData();

    auto const aidBoxesInRange = tree.RangeSearch(boxes[0]);
    auto const aidBoxesInRangeF = tree.template RangeSearch<false>(boxes[0]);
    auto const aidBoxesInRangeT = tree.template RangeSearch<true>(boxes[0]);

    auto const kNN = tree.GetNearestNeighbors({}, 2);

    auto const idPlaneIntersected = tree.PlaneSearch(1.0, { 1.0, 0.0 }, 0.0);
    auto const idPlaneIntersectedP = tree.PlaneSearch(
      {
        1.0, {1.0, 0.0}
    },
      0.0);
    auto const idPlanePosSeg = tree.PlanePositiveSegmentation(1.0, { 1.0, 0.0 }, 0.0);
    auto const idPlanePosSegP = tree.PlanePositiveSegmentation(
      {
        1.0, {1.0, 0.0}
    },
      0.0);
    auto const idFrustum = tree.FrustumCulling(
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


template<OrthoTree::dim_t N, bool IS_PARALLEL_EXEC, uint32_t nSplitStrategyAdditionalDepth = 2>
void testCompileBoxC()
{
  using BoundingBox = OrthoTree::BoundingBoxND<N>;
  using Plane = OrthoTree::PlaneND<N>;
  using OT = OrthoTree::TreeBoxContainerND<N, nSplitStrategyAdditionalDepth>;

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
    auto const& treeCore = tree.GetCore();
    auto const& data = tree.GetData();

    auto const aidPick = tree.PickSearch({});
    auto const aidBoxesInRange = tree.RangeSearch(boxes[0]);
    auto const aidBoxesInRangeF = tree.template RangeSearch<false>(boxes[0]);
    auto const aidBoxesInRangeT = tree.template RangeSearch<true>(boxes[0]);

    auto const vidCollision = tree.template CollisionDetection<IS_PARALLEL_EXEC>();
    auto const vidCollisionTree = tree.CollisionDetection(tree);
   
    auto const idBoxesIntersectedAll = tree.RayIntersectedAll({}, { 1.0, 1.0 }, 0);
    auto const idBoxesIntersectedFirst = tree.RayIntersectedFirst({}, { 1.0, 1.0 }, 0);

    auto const idPlaneIntersected = tree.PlaneIntersection(1.0, { 1.0, 0.0 }, 0.0);
    auto const idPlaneIntersectedP = tree.PlaneIntersection({1.0, { 1.0, 0.0 }}, 0.0);
    auto const idPlanePosSeg = tree.PlanePositiveSegmentation(1.0, { 1.0, 0.0 }, 0.0);
    auto const idPlanePosSegP = tree.PlanePositiveSegmentation({1.0, { 1.0, 0.0 }}, 0.0);
    auto const idFrustum = tree.FrustumCulling(
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

template<OrthoTree::dim_t N, bool IS_PARALLEL_EXEC, uint32_t nSplitStrategyAdditionalDepth = 2>
void testCompileBoxMapC()
{
  using BoundingBox = OrthoTree::BoundingBoxND<N>;
  using Plane = OrthoTree::PlaneND<N>;
  using Map = std::unordered_map<int, BoundingBox>;
  using OT = OrthoTree::TreeBoxContainerND<N, nSplitStrategyAdditionalDepth, OrthoTree::BaseGeometryType, Map>;

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
    auto const& treeCore = tree.GetCore();
    auto const& data = tree.GetData();

    auto const aidPick = tree.PickSearch({});
    auto const aidBoxesInRange = tree.RangeSearch(boxes.at(30));
    auto const aidBoxesInRangeF = tree.template RangeSearch<false>(boxes.at(30));
    auto const aidBoxesInRangeT = tree.template RangeSearch<true>(boxes.at(30));

    auto const vidCollision = tree.template CollisionDetection<IS_PARALLEL_EXEC>();
    auto const vidCollisionTree = tree.CollisionDetection(tree);

    auto const idBoxesIntersectedAll = tree.RayIntersectedAll({}, { 1.0, 1.0 }, 0);
    auto const idBoxesIntersectedFirst = tree.RayIntersectedFirst({}, { 1.0, 1.0 }, 0);

    auto const idPlaneIntersected = tree.PlaneIntersection(1.0, { 1.0, 0.0 }, 0.0);
    auto const idPlaneIntersectedP = tree.PlaneIntersection(
      {
        1.0, {1.0, 0.0}
    },
      0.0);
    auto const idPlanePosSeg = tree.PlanePositiveSegmentation(1.0, { 1.0, 0.0 }, 0.0);
    auto const idPlanePosSegP = tree.PlanePositiveSegmentation(
      {
        1.0, {1.0, 0.0}
    },
      0.0);
    auto const idFrustum = tree.FrustumCulling(
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


template<bool IS_PARALLEL_EXEC, uint32_t nSplitStrategyAdditionalDepth = 2>
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

    testCompileBox<2, IS_PARALLEL_EXEC, nSplitStrategyAdditionalDepth>();
    testCompileBox<3, IS_PARALLEL_EXEC, nSplitStrategyAdditionalDepth>();
    testCompileBox<4, IS_PARALLEL_EXEC, nSplitStrategyAdditionalDepth>();
    testCompileBox<5, IS_PARALLEL_EXEC, nSplitStrategyAdditionalDepth>();
    testCompileBox<6, IS_PARALLEL_EXEC, nSplitStrategyAdditionalDepth>();
    testCompileBox<7, IS_PARALLEL_EXEC, nSplitStrategyAdditionalDepth>();
    testCompileBox<8, IS_PARALLEL_EXEC, nSplitStrategyAdditionalDepth>();
    testCompileBox<12, IS_PARALLEL_EXEC, nSplitStrategyAdditionalDepth>();
    testCompileBox<16, IS_PARALLEL_EXEC, nSplitStrategyAdditionalDepth>();
    testCompileBox<31, IS_PARALLEL_EXEC, nSplitStrategyAdditionalDepth>();
    if constexpr (isPlatform64)
    {
      testCompileBox<32, IS_PARALLEL_EXEC, nSplitStrategyAdditionalDepth>();
      testCompileBox<63, IS_PARALLEL_EXEC, nSplitStrategyAdditionalDepth>();
    }

    testCompileBoxMap<2, IS_PARALLEL_EXEC, nSplitStrategyAdditionalDepth>();
    testCompileBoxMap<3, IS_PARALLEL_EXEC, nSplitStrategyAdditionalDepth>();
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

    testCompileBoxC<2, IS_PARALLEL_EXEC, nSplitStrategyAdditionalDepth>();
    testCompileBoxC<3, IS_PARALLEL_EXEC, nSplitStrategyAdditionalDepth>();
    testCompileBoxC<4, IS_PARALLEL_EXEC, nSplitStrategyAdditionalDepth>();
    testCompileBoxC<5, IS_PARALLEL_EXEC, nSplitStrategyAdditionalDepth>();
    testCompileBoxC<6, IS_PARALLEL_EXEC, nSplitStrategyAdditionalDepth>();
    testCompileBoxC<7, IS_PARALLEL_EXEC, nSplitStrategyAdditionalDepth>();
    testCompileBoxC<8, IS_PARALLEL_EXEC, nSplitStrategyAdditionalDepth>();
    testCompileBoxC<12, IS_PARALLEL_EXEC, nSplitStrategyAdditionalDepth>();
    testCompileBoxC<16, IS_PARALLEL_EXEC, nSplitStrategyAdditionalDepth>();
    testCompileBoxC<31, IS_PARALLEL_EXEC, nSplitStrategyAdditionalDepth>();

    if constexpr (isPlatform64)
    {
      testCompileBoxC<32, IS_PARALLEL_EXEC, nSplitStrategyAdditionalDepth>();
      testCompileBoxC<63, IS_PARALLEL_EXEC, nSplitStrategyAdditionalDepth>();
    }

    testCompileBoxMapC<2, IS_PARALLEL_EXEC, nSplitStrategyAdditionalDepth>();
    testCompileBoxMapC<3, IS_PARALLEL_EXEC, nSplitStrategyAdditionalDepth>();
  }
}


template<uint32_t nSplitStrategyAdditionalDepth = 2>
void testCompileBoxBatchExPol()
{
#ifdef __cpp_lib_execution
  testCompileBoxBatchDimension<false, nSplitStrategyAdditionalDepth>();
  testCompileBoxBatchDimension<true, nSplitStrategyAdditionalDepth>();
#else
  testCompileBoxBatchDimension<false, nSplitStrategyAdditionalDepth>();
#endif
}


inline void testCompileBoxBatchSplitStrategy()
{
  testCompileBoxBatchExPol<0>();
  testCompileBoxBatchExPol<2>();
  testCompileBoxBatchExPol<4>();
}

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#ifdef __GNUC__
#pragma GCC diagnostic pop
#pragma GCC diagnostic pop
#endif

#endif // ORTHOTREE_COMPILE_TEST_GUARD
