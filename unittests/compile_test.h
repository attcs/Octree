#ifndef ORTHOTREE_COMPILE_TEST_GUARD
#define ORTHOTREE_COMPILE_TEST_GUARD

#include <array>
#include <vector>

#ifndef autoc
#define autoc_remove
#define autoc auto const
#define autoce auto constexpr
#endif


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

template<OrthoTree::dim_t N, typename execution_policy_type>
void testCompilePoint()
{
  using Point = OrthoTree::PointND<N>;
  using BoundingBox = OrthoTree::BoundingBoxND<N>;
  using Plane = OrthoTree::PlaneND<N>;

  using OT = OrthoTree::TreePointND<N>;

  autoc key = OT::GetHash(2, 3);
  autoc noNode = OT::EstimateNodeNumber(100, 10, 3);
  autoc aidGrid = OT::MortonDecode(key, 3);
  autoc idGrid = OT::MortonEncode(aidGrid);
  autoc fValid = OT::IsValidKey(key);
  autoc idChild = OT::RemoveSentinelBit(key);

  autoce vpt = std::array{ Point{ 0.0 }, Point{ 1.0 }, Point{ 2.0 }, Point{ 3.0 }, Point{ 4.0 } };

  autoce boxes = std::array
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
    autoc keyRoot = tree.GetRootKey();
    autoc extent = tree.CalculateExtent(keyRoot);

    autoc allidBFS = tree.CollectAllIdInBFS(keyRoot);
    autoc allidDFS = tree.CollectAllIdInDFS(keyRoot);

    autoc nodeSmallest = tree.FindSmallestNode(vpt.back());

    autoc keySmallest = tree.FindSmallestNodeKey(keyRoot);
    autoc boxAll = tree.GetBox();
    autoc nDepth = tree.GetDepthID(keyRoot);
    autoc nDepthMax = tree.GetDepthMax();
    autoc nodeRoot = tree.GetNode(keyRoot);
    autoc nodes = tree.GetNodes();
    autoc grid = tree.GetResolutionMax();

    autoc aidBoxesInRange = tree.RangeSearch(boxes[0], vpt);
    autoc aidBoxesInRangeF = tree.template RangeSearch<false>(boxes[0], vpt);
    autoc aidBoxesInRangeT = tree.template RangeSearch<true>(boxes[0], vpt);
    autoc aidPtsInPlane = tree.PlaneSearch(0.0, Point{ 1.0 }, 0.01, vpt);
    autoc idPlaneIntersected = tree.PlaneSearch(1.0, { 1.0, 0.0 }, 0.0, vpt);
    autoc idPlaneIntersectedP = tree.PlaneSearch({1.0, { 1.0, 0.0 }}, 0.0, vpt);
    autoc idPlanePosSeg = tree.PlanePositiveSegmentation(1.0, { 1.0, 0.0 }, 0.0, vpt);
    autoc idPlanePosSegP = tree.PlanePositiveSegmentation({1.0, { 1.0, 0.0 }}, 0.0, vpt);
    autoc idFrustum = tree.FrustumCulling(
      std::vector{
        Plane{1.0, { 1.0, 0.0 }}
      },
      0.0,
      vpt);
    autoc kNN = tree.GetNearestNeighbors({}, 2, vpt);

    auto vListIsAnyChild = std::vector<bool>{};
    tree.VisitNodes(keyRoot
      , [&vListIsAnyChild](autoc&, autoc& node) { vListIsAnyChild.emplace_back(node.IsAnyChildExist()); }
      , [](autoc&, autoc&) -> bool { return true; }
    );
  }

  // non-const member functions
  {
    tree.template EraseId<false>(4);
    tree.template Erase<false>(3, vpt[3]);
    tree.Insert(3, vpt[3]);
    tree.Update(2, vpt[2], vpt[3]);
    tree.Update(3, vpt[4]);
    tree.UpdateIndexes({ {1, OT::UpdateID::ERASE }, {3, 4} });
    tree.template Move<execution_policy_type>({ 1.0, 1.0 });
    tree.Clear();
    tree.Reset();

    tree.Init(boxes[0], 3, 12);
    tree.Reset();

    OT::template Create<execution_policy_type>(tree, vpt, 4);
  }
}


template<OrthoTree::dim_t N, typename execution_policy_type, uint32_t nSplitStrategyAdditionalDepth = 2>
void testCompileBox()
{
  using Vector = OrthoTree::VectorND<N>;
  using BoundingBox = OrthoTree::BoundingBoxND<N>;
  using Plane = OrthoTree::PlaneND<N>;

  using OT = OrthoTree::TreeBoxND<N, nSplitStrategyAdditionalDepth>;

  autoc key = OT::GetHash(2, 3);
  autoc noNode = OT::EstimateNodeNumber(100, 10, 3);
  autoc aidGrid = OT::MortonDecode(key, 3);
  autoc idGrid = OT::MortonEncode(aidGrid);
  autoc fValid = OT::IsValidKey(key);
  autoc idChild = OT::RemoveSentinelBit(key);

  autoce boxes = std::array
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
    autoc keyRoot = tree.GetRootKey();
    autoc extent = tree.CalculateExtent(keyRoot);

    autoc allidBFS = tree.CollectAllIdInBFS(keyRoot);
    autoc allidDFS = tree.CollectAllIdInDFS(keyRoot);

    autoc nodeSmallest = tree.FindSmallestNode(boxes.back());

    autoc keySmallest = tree.FindSmallestNodeKey(keyRoot);
    autoc boxAll = tree.GetBox();
    autoc nDepth = tree.GetDepthID(keyRoot);
    autoc nDepthMax = tree.GetDepthMax();
    autoc nodeRoot = tree.GetNode(keyRoot);
    autoc nodes = tree.GetNodes();
    autoc grid = tree.GetResolutionMax();

    autoc vidCollision = tree.template CollisionDetection<execution_policy_type>(boxes);
    autoc vidCollisionTree = tree.CollisionDetection(boxes, tree, boxes);

    autoc aidPick = tree.PickSearch({}, boxes);
    autoc aidBoxesInRange = tree.RangeSearch(boxes[0], boxes);
    autoc aidBoxesInRangeF = tree.template RangeSearch<false>(boxes[0], boxes);
    autoc aidBoxesInRangeT = tree.template RangeSearch<true>(boxes[0], boxes);

    autoc idBoxesIntersectedAll = tree.RayIntersectedAll({}, { 1.0, 1.0 }, boxes, 0);
    autoc idBoxesIntersectedFirst = tree.RayIntersectedFirst({}, { 1.0, 1.0 }, boxes, 0);

    autoc idPlaneIntersected = tree.PlaneIntersection(1.0, { 1.0, 0.0 }, 0.0, boxes);
    autoc idPlaneIntersectedP = tree.PlaneIntersection({1.0, { 1.0, 0.0 }}, 0.0, boxes);
    autoc idPlanePosSeg = tree.PlanePositiveSegmentation(1.0, { 1.0, 0.0 }, 0.0, boxes);
    autoc idPlanePosSegP = tree.PlanePositiveSegmentation({1.0, { 1.0, 0.0 }}, 0.0, boxes);
    autoc idFrustum = tree.FrustumCulling(
      std::vector{
        Plane{1.0, Vector{ 1.0, 0.0 }}
      },
      0.0,
      boxes);

    auto vListIsAnyChild = std::vector<bool>{};
    tree.VisitNodes(keyRoot
      , [&vListIsAnyChild](autoc&, autoc& node) { vListIsAnyChild.emplace_back(node.IsAnyChildExist()); }
      , [](autoc&, autoc&) -> bool { return true; }
    );
  }

  // non-const member functions
  {
    tree.template EraseId<false>(4);
    tree.template Erase<false>(3, boxes[3]);
    tree.Insert(3, boxes[3]);
    tree.Update(2, boxes[2], boxes[3]);
    tree.Update(3, boxes[4]);
    tree.UpdateIndexes({ {1, OT::UpdateID::ERASE }, {3, 4} });
    tree.template Move<execution_policy_type>({ 1.0, 1.0 });
    tree.Clear();
    tree.Reset();

    tree.Init(boxes[0], 3, 12);
    tree.Reset();

    OT::template Create<execution_policy_type>(tree, boxes, 4);
  }
}


template<OrthoTree::dim_t N, typename execution_policy_type>
void testCompilePointC()
{
  using Point = OrthoTree::PointND<N>;
  using BoundingBox = OrthoTree::BoundingBoxND<N>;
  using Plane = OrthoTree::PlaneND<N>;
  using OT = OrthoTree::TreePointContainerND<N>;

  autoce vpt = std::array{ Point{ 0.0 }, Point{ 1.0 }, Point{ 2.0 }, Point{ 3.0 }, Point{ 4.0 } };

  autoce boxes = std::array
  {
    BoundingBox{ { 0.0, 0.0 }, { 1.0, 1.0 } },
    BoundingBox{ { 1.0, 1.0 }, { 2.0, 2.0 } },
    BoundingBox{ { 2.0, 2.0 }, { 3.0, 3.0 } },
    BoundingBox{ { 3.0, 3.0 }, { 4.0, 4.0 } },
    BoundingBox{ { 1.2, 1.2 }, { 2.8, 2.8 } }
  };

  auto tree = OT(vpt, 3, std::nullopt, 2, false);
  auto treePar = OT(vpt, 3, std::nullopt, 2, true);

  // const member functions
  {
    autoc& treeCore = tree.GetCore();
    autoc& data = tree.GetData();

    autoc aidBoxesInRange = tree.RangeSearch(boxes[0]);
    autoc aidBoxesInRangeF = tree.template RangeSearch<false>(boxes[0]);
    autoc aidBoxesInRangeT = tree.template RangeSearch<true>(boxes[0]);

    autoc kNN = tree.GetNearestNeighbors({}, 2);

    autoc idPlaneIntersected = tree.PlaneSearch(1.0, { 1.0, 0.0 }, 0.0);
    autoc idPlaneIntersectedP = tree.PlaneSearch({1.0, { 1.0, 0.0 }}, 0.0);
    autoc idPlanePosSeg = tree.PlanePositiveSegmentation(1.0, { 1.0, 0.0 }, 0.0);
    autoc idPlanePosSegP = tree.PlanePositiveSegmentation({1.0, { 1.0, 0.0 }}, 0.0);
    autoc idFrustum = tree.FrustumCulling(
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
    tree.template Move<execution_policy_type>({ 1.0, 1.0 });
    tree.Clear();
    tree.Reset();

    tree.Init(boxes[0], 3, 12);
    tree.Reset();

    tree = OT::template Create<execution_policy_type>(vpt, 4);
    tree = OT::template Create<execution_policy_type>(std::vector{ Point{ 0.0 }, Point{ 1.0 }, Point{ 2.0 }, Point{ 3.0 }, Point{ 4.0 } }, 4);
  }

}


template<OrthoTree::dim_t N, typename execution_policy_type, uint32_t nSplitStrategyAdditionalDepth = 2>
void testCompileBoxC()
{
  using BoundingBox = OrthoTree::BoundingBoxND<N>;
  using Plane = OrthoTree::PlaneND<N>;
  using OT = OrthoTree::TreeBoxContainerND<N, nSplitStrategyAdditionalDepth>;

  autoce boxes = std::array
  {
    BoundingBox{ { 0.0, 0.0 }, { 1.0, 1.0 } },
    BoundingBox{ { 1.0, 1.0 }, { 2.0, 2.0 } },
    BoundingBox{ { 2.0, 2.0 }, { 3.0, 3.0 } },
    BoundingBox{ { 3.0, 3.0 }, { 4.0, 4.0 } },
    BoundingBox{ { 1.2, 1.2 }, { 2.8, 2.8 } }
  };

  auto tree = OT(boxes, 3, std::nullopt, 2, false);
  auto treePar = OT(boxes, 3, std::nullopt, 2, true);

  // const member functions
  {
    autoc& treeCore = tree.GetCore();
    autoc& data = tree.GetData();

    autoc aidPick = tree.PickSearch({});
    autoc aidBoxesInRange = tree.RangeSearch(boxes[0]);
    autoc aidBoxesInRangeF = tree.template RangeSearch<false>(boxes[0]);
    autoc aidBoxesInRangeT = tree.template RangeSearch<true>(boxes[0]);

    autoc vidCollision = tree.template CollisionDetection<execution_policy_type>();
    autoc vidCollisionTree = tree.CollisionDetection(tree);
   
    autoc idBoxesIntersectedAll = tree.RayIntersectedAll({}, { 1.0, 1.0 }, 0);
    autoc idBoxesIntersectedFirst = tree.RayIntersectedFirst({}, { 1.0, 1.0 }, 0);

    autoc idPlaneIntersected = tree.PlaneIntersection(1.0, { 1.0, 0.0 }, 0.0);
    autoc idPlaneIntersectedP = tree.PlaneIntersection({1.0, { 1.0, 0.0 }}, 0.0);
    autoc idPlanePosSeg = tree.PlanePositiveSegmentation(1.0, { 1.0, 0.0 }, 0.0);
    autoc idPlanePosSegP = tree.PlanePositiveSegmentation({1.0, { 1.0, 0.0 }}, 0.0);
    autoc idFrustum = tree.FrustumCulling(
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
    tree.template Move<execution_policy_type>({ 1.0, 1.0 });
    tree.Clear();
    tree.Reset();

    tree.Init(boxes[0], 3, 12);
    tree.Reset();

    tree = OT::template Create<execution_policy_type>(boxes, 4);
    tree = OT::template Create<execution_policy_type>(
      std::vector
      {
        BoundingBox{ { 0.0, 0.0 }, { 1.0, 1.0 } },
        BoundingBox{ { 1.0, 1.0 }, { 2.0, 2.0 } },
        BoundingBox{ { 2.0, 2.0 }, { 3.0, 3.0 } },
        BoundingBox{ { 3.0, 3.0 }, { 4.0, 4.0 } },
        BoundingBox{ { 1.2, 1.2 }, { 2.8, 2.8 } }
      }
    , 4);

  }

}


template<typename execution_policy_type, uint32_t nSplitStrategyAdditionalDepth = 2>
void testCompileBoxBatchDimension()
{
  autoce isPlatform64 = sizeof(std::size_t) == 8;

  // Core types
  {
    testCompilePoint<2, execution_policy_type>();
    testCompilePoint<3, execution_policy_type>();
    testCompilePoint<4, execution_policy_type>();
    testCompilePoint<5, execution_policy_type>();
    testCompilePoint<6, execution_policy_type>();
    testCompilePoint<7, execution_policy_type>();
    testCompilePoint<8, execution_policy_type>();
    testCompilePoint<12, execution_policy_type>();
    testCompilePoint<16, execution_policy_type>();
    testCompilePoint<31, execution_policy_type>();
    if constexpr (isPlatform64)
    {
      testCompilePoint<32, execution_policy_type>();
      testCompilePoint<63, execution_policy_type>();
    }

    testCompileBox<2, execution_policy_type, nSplitStrategyAdditionalDepth>();
    testCompileBox<3, execution_policy_type, nSplitStrategyAdditionalDepth>();
    testCompileBox<4, execution_policy_type, nSplitStrategyAdditionalDepth>();
    testCompileBox<5, execution_policy_type, nSplitStrategyAdditionalDepth>();
    testCompileBox<6, execution_policy_type, nSplitStrategyAdditionalDepth>();
    testCompileBox<7, execution_policy_type, nSplitStrategyAdditionalDepth>();
    testCompileBox<8, execution_policy_type, nSplitStrategyAdditionalDepth>();
    testCompileBox<12, execution_policy_type, nSplitStrategyAdditionalDepth>();
    testCompileBox<16, execution_policy_type, nSplitStrategyAdditionalDepth>();
    testCompileBox<31, execution_policy_type, nSplitStrategyAdditionalDepth>();
    if constexpr (isPlatform64)
    {
      testCompileBox<32, execution_policy_type, nSplitStrategyAdditionalDepth>();
      testCompileBox<63, execution_policy_type, nSplitStrategyAdditionalDepth>();
    }
  }
  
  // Container types
  {
    testCompilePointC<2, execution_policy_type>();
    testCompilePointC<3, execution_policy_type>();
    testCompilePointC<4, execution_policy_type>();
    testCompilePointC<5, execution_policy_type>();
    testCompilePointC<6, execution_policy_type>();
    testCompilePointC<7, execution_policy_type>();
    testCompilePointC<8, execution_policy_type>();
    testCompilePointC<12, execution_policy_type>();
    testCompilePointC<16, execution_policy_type>();
    testCompilePointC<31, execution_policy_type>();
    if constexpr (isPlatform64)
    {
      testCompilePointC<32, execution_policy_type>();
      testCompilePointC<63, execution_policy_type>();
    }

    testCompileBoxC<2, execution_policy_type, nSplitStrategyAdditionalDepth>();
    testCompileBoxC<3, execution_policy_type, nSplitStrategyAdditionalDepth>();
    testCompileBoxC<4, execution_policy_type, nSplitStrategyAdditionalDepth>();
    testCompileBoxC<5, execution_policy_type, nSplitStrategyAdditionalDepth>();
    testCompileBoxC<6, execution_policy_type, nSplitStrategyAdditionalDepth>();
    testCompileBoxC<7, execution_policy_type, nSplitStrategyAdditionalDepth>();
    testCompileBoxC<8, execution_policy_type, nSplitStrategyAdditionalDepth>();
    testCompileBoxC<12, execution_policy_type, nSplitStrategyAdditionalDepth>();
    testCompileBoxC<16, execution_policy_type, nSplitStrategyAdditionalDepth>();
    testCompileBoxC<31, execution_policy_type, nSplitStrategyAdditionalDepth>();

    if constexpr (isPlatform64)
    {
      testCompileBoxC<32, execution_policy_type, nSplitStrategyAdditionalDepth>();
      testCompileBoxC<63, execution_policy_type, nSplitStrategyAdditionalDepth>();
    }
  }
}


template<uint32_t nSplitStrategyAdditionalDepth = 2>
void testCompileBoxBatchExPol()
{
  testCompileBoxBatchDimension<std::execution::sequenced_policy, nSplitStrategyAdditionalDepth>();
  testCompileBoxBatchDimension<std::execution::unsequenced_policy, nSplitStrategyAdditionalDepth>();
  testCompileBoxBatchDimension<std::execution::parallel_policy, nSplitStrategyAdditionalDepth>();
  testCompileBoxBatchDimension<std::execution::parallel_unsequenced_policy, nSplitStrategyAdditionalDepth>();
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

#ifdef autoc_remove
#undef autoc_remove
#undef autoc
#undef autoce
#endif

#endif // ORTHOTREE_COMPILE_TEST_GUARD
