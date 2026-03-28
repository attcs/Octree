#include <bit>
#include <iostream>
#include <random>
#include <type_traits>

#include "external/benchmark/include/benchmark/benchmark.h"

#include "../manual/generators.h"
#include "orthotree/bvh.h"
#include "orthotree/octree.h"

#define CONCAT_IMPL(a, b) a##b
#define CONCAT(a, b) CONCAT_IMPL(a, b)

using namespace OrthoTree;

static void SetIterationNo(benchmark::State& state, std::size_t no)
{
  state.SetComplexityN(no);
  state.SetItemsProcessed(state.iterations() * no);
}


template<dim_t DIMENSION_NO>
static std::optional<PointND<DIMENSION_NO>> GetNormalized(PointND<DIMENSION_NO> const& direction)
{
  auto length2 = 0.0;
  for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
  {
    length2 += direction[dimensionID] * direction[dimensionID];
  }

  if (length2 == 0)
  {
    return std::nullopt;
  }

  auto normalized = direction;
  auto const length = std::sqrt(length2);
  for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
  {
    normalized[dimensionID] = direction[dimensionID] / length;
  }

  return normalized;
}

namespace Benchmarks
{
  namespace Base
  {
    void GetNodeID(benchmark::State& state)
    {
      constexpr dim_t DIMENSION_NO = 3;
      constexpr depth_t depth = 5;

      using GA = GeneralGeometryAdapterND<DIMENSION_NO>;

      size_t entityNo = state.range();

      auto const points = GeneratePointsRandom<DIMENSION_NO>(entityNo);
      auto const boxSpace = CreateSearcBox<DIMENSION_NO>(0, rMax);
      detail::MortonGridSpaceIndexing<GA, true, 1.0, 19> modelSpaceGrid(depth, detail::InternalGeometryModule<GA>::GetBoxAD(boxSpace));

      using NodeID = typename OrthoTreePointND<DIMENSION_NO>::NodeID;

      std::vector<NodeID> entityIDs(entityNo);
      for (auto _ : state)
      {
        for (int i = 0; i < entityNo; ++i)
        {
          entityIDs[i] = modelSpaceGrid.GetNodeID(points[i]);
        }
      }

      SetIterationNo(state, entityNo);
    }

    static void GetDepthID(benchmark::State& state)
    {
      constexpr dim_t DIMENSION_NO = 3;
      constexpr depth_t depth = 5;
      using GA = GeneralGeometryAdapterND<DIMENSION_NO>;

      size_t entityNo = state.range();

      auto const points = GeneratePointsRandom<DIMENSION_NO>(entityNo);
      auto const boxSpace = CreateSearcBox<DIMENSION_NO>(0, rMax);
      detail::MortonGridSpaceIndexing<GA, true, 1.0, 19> modelSpaceGrid(depth, detail::InternalGeometryModule<GA>::GetBoxAD(boxSpace));

      using NodeID = typename OrthoTreePointND<DIMENSION_NO>::NodeID;
      auto entityIDs = std::vector<NodeID>{};
      for (auto const& point : points)
        entityIDs.emplace_back(modelSpaceGrid.GetNodeID(point));

      std::vector<depth_t> depthIDs(entityNo);
      for (auto _ : state)
      {
        for (int i = 0; i < entityNo; ++i)
        {
          depthIDs[i] = detail::MortonSpaceIndexing<DIMENSION_NO, 19>::GetDepthID(entityIDs[i]);
        }
      }
      SetIterationNo(state, entityNo);
    }
    /*
    static void GetNodeEntities(benchmark::State& state)
    {
      constexpr dim_t DIMENSION_NO = 3;
      constexpr depth_t depth = 5;

      using Node = typename OrthoTreePointND<DIMENSION_NO>::Node;

      size_t entityNo = state.range();

      auto const points = GeneratePointsRandom<DIMENSION_NO>(entityNo);
      auto const tree = OrthoTreePointND<DIMENSION_NO>(points, depth);

      auto const& nodes = tree.GetNodes();

      auto nodePtrs = std::vector<Node const*>();
      for (auto const& [key, node] : nodes)
        nodePtrs.emplace_back(&node);

      for (auto _ : state)
      {
        for (auto const* nodePtr : nodePtrs)
          benchmark::DoNotOptimize(tree.GetNodeEntities(*nodePtr));
      }
      SetIterationNo(state, entityNo);
    }
    */
    static void GridSpaceIndexing_GetPointGridID(benchmark::State& state)
    {
      constexpr dim_t DIMENSION_NO = 3;
      constexpr depth_t depth = 5;
      using GridSpaceIndexing = detail::GridSpaceIndexing<GeneralGeometryAdapterND<DIMENSION_NO>>;

      size_t entityNo = state.range();
      auto const points = GeneratePointsRandom<DIMENSION_NO>(entityNo);
      auto const boxSpace = CreateSearcBox<DIMENSION_NO>(0, rMax);
      auto const gsi = GridSpaceIndexing(depth, GridSpaceIndexing::IGM::GetBoxAD(boxSpace));

      auto gridIDs = std::vector<std::array<GridID, DIMENSION_NO>>(entityNo);
      for (auto _ : state)
      {
        for (std::size_t i = 0; i < entityNo; ++i)
        {
          gridIDs[i] = gsi.GetPointGridID(points[i]);
        }
      }

      SetIterationNo(state, entityNo);
    }

    static void GridSpaceIndexing_GetBoxGridID(benchmark::State& state)
    {
      constexpr dim_t DIMENSION_NO = 3;
      constexpr depth_t depth = 5;
      using GridSpaceIndexing = detail::GridSpaceIndexing<GeneralGeometryAdapterND<DIMENSION_NO>>;

      size_t entityNo = state.range();
      auto const boxes = GenerateBoxesRandom<DIMENSION_NO>(entityNo);
      auto const boxSpace = CreateSearcBox<DIMENSION_NO>(0, rMax);
      auto const gsi = GridSpaceIndexing(depth, GridSpaceIndexing::IGM::GetBoxAD(boxSpace));

      auto gridIDs = std::vector<std::array<std::array<GridID, DIMENSION_NO>, 2>>(entityNo);
      for (auto _ : state)
      {
        for (std::size_t i = 0; i < entityNo; ++i)
        {
          gridIDs[i] = gsi.GetBoxGridID(boxes[i]);
        }
      }

      SetIterationNo(state, entityNo);
    }

  } // namespace Base

  static constexpr depth_t depthCreationForSmallND = 19;
  static constexpr depth_t depthCreationForLargeND = 10;
  static constexpr depth_t depthEdit = 10;

  consteval static depth_t GetCreationDepth(dim_t dimensionNo)
  {
    return dimensionNo <= 3 ? 19 : 10;
  }

  enum class TreeType
  {
    DynamicOrtho,
    StaticOrtho,
    StaticBVH
  };


  template<TreeType TREE_TYPE, OrthoTree::NodeGeometryStorage NODE_GEOMETRY_STORAGE = OrthoTree::NodeGeometryStorage::MinPoint, bool USE_REVERSE_MAPPING = false, bool IS_CONTIOGUOS_CONTAINER = true>
  struct PointBenchmarks
  {
    using Config = Configuration<1.0, NODE_GEOMETRY_STORAGE, USE_REVERSE_MAPPING>;

    template<dim_t DIMENSION_NO>
    using DynamicOrthoTree = typename OrthoTree::OrthoTreeBase<
      std::conditional_t<IS_CONTIOGUOS_CONTAINER, PointEntitySpanAdapter<PointND<DIMENSION_NO>>, PointEntityMapAdapter<PointND<DIMENSION_NO>>>,
      GeneralGeometryAdapterND<DIMENSION_NO>,
      Config>;

    template<dim_t DIMENSION_NO>
    using StaticOrthoTree = typename OrthoTree::StaticOrthoTreeBase<
      std::conditional_t<IS_CONTIOGUOS_CONTAINER, PointEntitySpanAdapter<PointND<DIMENSION_NO>>, PointEntityMapAdapter<PointND<DIMENSION_NO>>>,
      GeneralGeometryAdapterND<DIMENSION_NO>,
      Config>;

    template<dim_t DIMENSION_NO>
    using Tree = std::conditional_t<TREE_TYPE == TreeType::StaticOrtho, StaticOrthoTree<DIMENSION_NO>, DynamicOrthoTree<DIMENSION_NO>>;

    template<dim_t DIMENSION_NO, typename TExecMode>
    static void Create(benchmark::State& state)
    {
      constexpr depth_t depth = GetCreationDepth(DIMENSION_NO);

      size_t entityNo = state.range();
      auto const boxSpace = CreateSearcBox<DIMENSION_NO>(0, rMax);
      auto const points = GeneratePointsRandom<DIMENSION_NO>(entityNo);
      for (auto _ : state)
      {
        auto tree = Tree<DIMENSION_NO>(TExecMode{}, points, depth, boxSpace);
      }
    }

    template<dim_t DIMENSION_NO, typename TExecMode>
    static void Create__OversizedModelspace(benchmark::State& state)
    {
      constexpr depth_t depth = GetCreationDepth(DIMENSION_NO);

      size_t entityNo = state.range();
      auto const boxSpace = CreateSearcBox<DIMENSION_NO>(-rMax * 127.5, rMax * 256.0);
      auto const points = GeneratePointsRandom<DIMENSION_NO>(entityNo);
      for (auto _ : state)
      {
        auto tree = Tree<DIMENSION_NO>(TExecMode{}, points, depth, boxSpace);
      }
    }

    template<dim_t DIMENSION_NO, typename TExecMode>
    static void Insert__Bulk(benchmark::State& state)
    {
      constexpr depth_t depth = GetCreationDepth(DIMENSION_NO);

      size_t entityNo = state.range();
      auto const boxSpace = CreateSearcBox<DIMENSION_NO>(0, rMax);
      auto const points = GeneratePointsRandom<DIMENSION_NO>(entityNo);
      for (auto _ : state)
      {
        auto tree = Tree<DIMENSION_NO>(TExecMode{}, {}, depth, boxSpace);
        tree.Insert(points, std::vector<PointND<DIMENSION_NO>>{}, TExecMode{});
      }
    }

    template<dim_t DIMENSION_NO, typename TExecMode>
    static void Insert__BulkOversizedModelspace(benchmark::State& state)
    {
      constexpr depth_t depth = GetCreationDepth(DIMENSION_NO);

      size_t entityNo = state.range();
      auto const boxSpace = CreateSearcBox<DIMENSION_NO>(-rMax * 127.5, rMax * 256.0);
      auto const points = GeneratePointsRandom<DIMENSION_NO>(entityNo);
      for (auto _ : state)
      {
        auto tree = Tree<DIMENSION_NO>(TExecMode{}, {}, depth, boxSpace);
        tree.Insert(points, std::vector<PointND<DIMENSION_NO>>{}, TExecMode{});
      }
    }

    static void InsertIntoLeaf(benchmark::State& state)
    {
      constexpr dim_t DIMENSION_NO = 3;
      constexpr depth_t depth = depthEdit;

      size_t entityNo = state.range();

      auto const points = GeneratePointsRandom<DIMENSION_NO>(entityNo);
      auto const boxSpace = CreateSearcBox<DIMENSION_NO>(0, rMax);

      for (auto _ : state)
      {
        auto tree = Tree<DIMENSION_NO>();
        tree.Init(boxSpace, depth);

        auto entityID = OrthoTreePointND<DIMENSION_NO>::EntityID(0);
        for (auto const& point : points)
        {
          tree.InsertIntoLeaf(entityID, point, InsertionMode::LowestLeaf);
          ++entityID;
        }
      }
    }


    static void Insert__RebalanceMode(benchmark::State& state)
    {
      constexpr dim_t DIMENSION_NO = 3;
      constexpr depth_t depth = depthEdit;

      size_t entityNo = state.range();

      auto const points = GeneratePointsRandom<DIMENSION_NO>(entityNo);
      auto const boxSpace = CreateSearcBox<DIMENSION_NO>(0, rMax);

      for (auto _ : state)
      {
        auto tree = Tree<DIMENSION_NO>();
        tree.Init(boxSpace, depth);

        auto entityID = OrthoTreePointND<DIMENSION_NO>::EntityID(0);
        for (auto const& point : points)
        {
          tree.Insert(entityID, point, points);
          ++entityID;
        }
      }
    }


    static void InsertUnique(benchmark::State& state)
    {
      constexpr dim_t DIMENSION_NO = 3;
      constexpr depth_t depth = depthEdit;

      size_t entityNo = state.range();

      auto const possiblePoints = GeneratePointsRandom<DIMENSION_NO>(entityNo);
      auto const boxSpace = CreateSearcBox<DIMENSION_NO>(0, rMax);

      for (auto _ : state)
      {
        auto tree = Tree<DIMENSION_NO>();
        tree.Init(boxSpace, depth);

        std::remove_cvref_t<decltype(possiblePoints)> storedPoints;
        for (auto const& point : possiblePoints)
        {
          if (tree.InsertUnique(point, storedPoints, rMax / 100.0))
            storedPoints.emplace_back(point);
        }
      }
    }


    static void Update(benchmark::State& state)
    {
      constexpr dim_t DIMENSION_NO = 3;
      constexpr depth_t depth = depthEdit;

      size_t entityNo = state.range();

      auto const points = GeneratePointsRandom<DIMENSION_NO>(entityNo * 100, 0);
      auto const updatePoints = GeneratePointsRandom<DIMENSION_NO>(entityNo, 1);
      auto const updatePointsNo = OrthoTreePointND<DIMENSION_NO>::EntityID(updatePoints.size());
      auto tree = Tree<DIMENSION_NO>(points, depth);
      for (auto _ : state)
      {
        auto workingPoints = points;
        for (OrthoTreePointND<DIMENSION_NO>::EntityID entityID = 0; entityID < updatePointsNo; ++entityID)
        {
          if (tree.Update(entityID, workingPoints[entityID], updatePoints[entityID], workingPoints))
            workingPoints[entityID] = updatePoints[entityID];
        }
      }
      SetIterationNo(state, entityNo);
    }


    static void Contains(benchmark::State& state)
    {
      constexpr dim_t DIMENSION_NO = 3;
      constexpr depth_t depth = depthEdit;

      size_t entityNo = state.range();

      auto const points = GeneratePointsRandom<DIMENSION_NO>(entityNo);
      auto const tree = Tree<DIMENSION_NO>(points, depth);

      size_t entityID = 0;
      for (auto _ : state)
      {
        tree.Contains(points[entityID % entityNo], points, rMax / 1000.0);
        ++entityID;
      }
    }


    static void RangeSearch(benchmark::State& state)
    {
      constexpr dim_t DIMENSION_NO = 3;
      constexpr depth_t depth = depthEdit;

      size_t entityNo = state.range();

      auto const points = GeneratePointsRandom<DIMENSION_NO>(entityNo);
      auto const tree = Tree<DIMENSION_NO>(points, depth);
      constexpr size_t boxNo = 100;
      auto const searchBoxes = GenerateBoxesRandom<DIMENSION_NO>(boxNo, 1, 0.05);

      size_t entityID = 0;
      for (auto _ : state)
      {
        tree.RangeSearch(searchBoxes[entityID % boxNo], points);
        ++entityID;
      }
    }

    template<dim_t DIMENSION_NO = 3>
    static void GetNearestNeighbors(benchmark::State& state)
    {
      constexpr auto depth = std::min<depth_t>(depthEdit, OrthoTreePointND<DIMENSION_NO>::SI::MAX_THEORETICAL_DEPTH_ID);

      size_t entityNo = state.range();

      auto const points = GeneratePointsRandom<DIMENSION_NO>(entityNo, 0);
      auto const tree = Tree<DIMENSION_NO>(points, depth);

      auto const searchPoints = GeneratePointsRandom<DIMENSION_NO>(std::max<std::size_t>(20, entityNo / 100), 1);
      auto const pointNo = searchPoints.size();
      size_t pointID = 0;
      for (auto _ : state)
      {
        auto const& searchPoint = searchPoints[pointID % pointNo];
        tree.GetNearestNeighbors(searchPoint, std::max(2, static_cast<int>(std::floor(searchPoint[0]))), points);
        ++pointID;
      }
    }


    static void FrustumCulling(benchmark::State& state)
    {
      constexpr dim_t DIMENSION_NO = 3;
      constexpr depth_t depth = depthEdit;

      size_t entityNo = state.range();

      auto const points = GeneratePointsRandom<DIMENSION_NO>(entityNo);
      auto const tree = Tree<DIMENSION_NO>(points, depth);

      auto const planes = std::vector{
        PlaneND<DIMENSION_NO>{ .OrigoDistance = rMax * 0.9, .Normal = { 1.0, 0.0, 0.0 } },
        PlaneND<DIMENSION_NO>{ .OrigoDistance = rMax * 0.9, .Normal = { 0.0, 1.0, 0.0 } },
        PlaneND<DIMENSION_NO>{ .OrigoDistance = rMax * 0.9, .Normal = { 0.0, 0.0, 1.0 } }
      };
      for (auto _ : state)
      {
        tree.FrustumCulling(planes, points, 0.1);
      }
    }
  }; // struct PointBenchmarks

  enum class LooseMode
  {
    Loose,
    Regular
  };

  template<
    TreeType TREE_TYPE,
    LooseMode LOOSE_MODE = LooseMode::Loose,
    OrthoTree::NodeGeometryStorage NODE_GEOMETRY_STORAGE = OrthoTree::NodeGeometryStorage::MinPoint,
    bool USE_REVERSE_MAPPING = false,
    bool IS_CONTIOGUOS_CONTAINER = true>
  struct BoxBenchmarks
  {
    using Config = Configuration<LOOSE_MODE == LooseMode::Loose ? 2.0 : 1.0, NODE_GEOMETRY_STORAGE, USE_REVERSE_MAPPING>;

    template<dim_t DIMENSION_NO>
    using DynamicOrthoTree = typename OrthoTree::OrthoTreeBase<
      std::conditional_t<IS_CONTIOGUOS_CONTAINER, BoxEntitySpanAdapter<BoundingBoxND<DIMENSION_NO>>, BoxEntityMapAdapter<BoundingBoxND<DIMENSION_NO>>>,
      GeneralGeometryAdapterND<DIMENSION_NO>,
      Config>;

    template<dim_t DIMENSION_NO>
    using StaticOrthoTree = typename OrthoTree::StaticOrthoTreeBase<
      std::conditional_t<IS_CONTIOGUOS_CONTAINER, BoxEntitySpanAdapter<BoundingBoxND<DIMENSION_NO>>, BoxEntityMapAdapter<BoundingBoxND<DIMENSION_NO>>>,
      GeneralGeometryAdapterND<DIMENSION_NO>,
      Config>;

    template<dim_t DIMENSION_NO>
    using Tree = std::conditional_t<
      TREE_TYPE == TreeType::StaticBVH,
      StaticBVHBoxND<DIMENSION_NO>,
      std::conditional_t<TREE_TYPE == TreeType::StaticOrtho, StaticOrthoTree<DIMENSION_NO>, DynamicOrthoTree<DIMENSION_NO>>>;

    template<dim_t DIM_NO, typename TExecMode, typename TEntities>
    static auto InitTree(TExecMode const& execMode, TEntities const& entities, depth_t depth, BoundingBoxND<DIM_NO> const& boxSpace)
    {
      if constexpr (TREE_TYPE == TreeType::StaticBVH)
        return StaticBVHBoxND<DIM_NO>(execMode, entities);
      else if constexpr (TREE_TYPE == TreeType::StaticOrtho)
        return StaticOrthoTree<DIM_NO>(execMode, entities, depth, boxSpace);
      else
        return DynamicOrthoTree<DIM_NO>(execMode, entities, depth, boxSpace);
    }

    template<dim_t DIM_NO, typename TEntities>
    static auto InitTree(TEntities const& entities, depth_t depth, BoundingBoxND<DIM_NO> const& boxSpace)
    {
      if constexpr (TREE_TYPE == TreeType::StaticBVH)
        return StaticBVHBoxND<DIM_NO>(entities);
      else if constexpr (TREE_TYPE == TreeType::StaticOrtho)
        return StaticOrthoTree<DIM_NO>(entities, depth, boxSpace);
      else
        return DynamicOrthoTree<DIM_NO>(entities, depth, boxSpace);
    }


    template<dim_t DIMENSION_NO, typename TExecMode>
    static void Create(benchmark::State& state)
    {
      constexpr depth_t depth = GetCreationDepth(DIMENSION_NO);

      size_t entityNo = state.range();
      auto const entities = GenerateBoxesRandom<DIMENSION_NO>(entityNo);
      auto const boxSpace = CreateSearcBox<DIMENSION_NO>(0, rMax);
      for (auto _ : state)
      {
        auto const tree = InitTree<DIMENSION_NO>(TExecMode{}, entities, depth, boxSpace);
      }
    }

    template<dim_t DIMENSION_NO, typename TExecMode>
    static void Create__OversizedModelspace(benchmark::State& state)
    {
      constexpr depth_t depth = GetCreationDepth(DIMENSION_NO);
      auto const boxSpace = CreateSearcBox<DIMENSION_NO>(-rMax * 127.5, rMax * 256.0);

      size_t entityNo = state.range();
      auto const entities = GenerateBoxesRandom<DIMENSION_NO>(entityNo);
      for (auto _ : state)
      {
        auto const tree = InitTree<DIMENSION_NO>(TExecMode{}, entities, depth, boxSpace);
      }
    }

    template<dim_t DIMENSION_NO, typename TExecMode>
    static void Insert__Bulk(benchmark::State& state)
    {
      constexpr depth_t depth = GetCreationDepth(DIMENSION_NO);

      auto const boxSpace = CreateSearcBox<DIMENSION_NO>(0, rMax);

      size_t entityNo = state.range();
      auto const entities = GenerateBoxesRandom<DIMENSION_NO>(entityNo);
      for (auto _ : state)
      {
        auto tree = InitTree<DIMENSION_NO>(TExecMode{}, std::vector<BoundingBoxND<DIMENSION_NO>>{}, depth, boxSpace);
        tree.Insert(entities, std::vector<BoundingBoxND<DIMENSION_NO>>{}, TExecMode{});
      }
    }

    static void InsertToLeaf(benchmark::State& state)
    {
      constexpr dim_t DIMENSION_NO = 3;
      constexpr depth_t depth = depthEdit;

      using EntityID = typename Tree<DIMENSION_NO>::EntityID;

      size_t entityNo = state.range();

      auto const entities = GenerateBoxesRandom<DIMENSION_NO>(entityNo);
      auto const boxSpace = CreateSearcBox<DIMENSION_NO>(0, rMax);

      for (auto _ : state)
      {
        auto tree = Tree<DIMENSION_NO>();
        tree.Init(boxSpace, depth);

        EntityID entityID = 0;
        for (auto const& entity : entities)
        {
          assert(tree.Insert(entityID, entity, entities));
          ++entityID;
        }
      }
    }

    static void Insert__RebalanceMode(benchmark::State& state)
    {
      constexpr dim_t DIMENSION_NO = 3;
      constexpr depth_t depth = depthEdit;

      using EntityID = typename Tree<DIMENSION_NO>::EntityID;

      size_t entityNo = state.range();

      auto const entities = GenerateBoxesRandom<DIMENSION_NO>(entityNo);
      auto const boxSpace = CreateSearcBox<DIMENSION_NO>(0, rMax);

      for (auto _ : state)
      {
        auto tree = Tree<DIMENSION_NO>();
        tree.Init(boxSpace, depth);

        EntityID entityID = 0;
        for (auto const& entity : entities)
        {
          tree.Insert(entityID, entity, entities);
          ++entityID;
        }
      }
    }


    static void Update(benchmark::State& state)
    {
      constexpr dim_t DIMENSION_NO = 3;
      constexpr depth_t depth = depthEdit;

      using EntityID = typename Tree<DIMENSION_NO>::EntityID;

      size_t entityNo = state.range();

      auto const boxSpace = CreateSearcBox<DIMENSION_NO>(0, rMax);
      auto entities = GenerateBoxesRandom<DIMENSION_NO>(entityNo * 10);
      auto const updateEntities = GenerateBoxesRandom<DIMENSION_NO>(entityNo, 1);
      auto const updateEntitiesNo = EntityID(updateEntities.size());

      auto tree = InitTree<DIMENSION_NO>(entities, depth, boxSpace);
      for (auto _ : state)
      {
        for (EntityID entityID = 0; entityID < updateEntitiesNo; ++entityID)
        {
          auto oldEntity = entities[entityID];
          entities[entityID] = updateEntities[entityID];
          tree.Update(entityID, oldEntity, updateEntities[entityID], entities);
        }
      }

      SetIterationNo(state, entityNo);
    }


    static void PickSearch(benchmark::State& state)
    {
      constexpr dim_t DIMENSION_NO = 3;
      constexpr depth_t depth = depthEdit;
      using EntityID = typename Tree<DIMENSION_NO>::EntityID;

      size_t entityNo = state.range();

      auto const entities = GenerateBoxesRandom<DIMENSION_NO>(entityNo, 0, 0.02);
      auto const boxSpace = CreateSearcBox<DIMENSION_NO>(0, rMax);
      auto const tree = InitTree<DIMENSION_NO>(entities, depth, boxSpace);

      constexpr size_t pointNo = 100;
      auto const searchPoints = GeneratePointsRandom<DIMENSION_NO>(pointNo, 1);

      EntityID entityID = 0;
      for (auto _ : state)
      {
        tree.PickSearch(searchPoints[entityID % pointNo], entities);
        ++entityID;
      }
    }


    static void RangeSearch(benchmark::State& state)
    {
      constexpr dim_t DIMENSION_NO = 3;
      constexpr depth_t depth = depthEdit;

      size_t entityNo = state.range();

      auto const entities = GenerateBoxesRandom<DIMENSION_NO>(entityNo, 0, 0.02);
      auto const boxSpace = CreateSearcBox<DIMENSION_NO>(0, rMax);
      auto const tree = InitTree<DIMENSION_NO>(entities, depth, boxSpace);

      constexpr size_t boxNo = 100;
      auto const searchBoxes = GenerateBoxesRandom<DIMENSION_NO>(boxNo, 1, 0.02);

      size_t entityID = 0;
      for (auto _ : state)
      {
        tree.RangeSearch(searchBoxes[entityID % boxNo], entities);
        ++entityID;
      }
    }


    static void FrustumCulling(benchmark::State& state)
    {
      constexpr dim_t DIMENSION_NO = 3;
      constexpr depth_t depth = depthEdit;

      size_t entityNo = state.range();

      auto const entities = GenerateBoxesRandom<DIMENSION_NO>(entityNo, 0, 0.05);
      auto const boxSpace = CreateSearcBox<DIMENSION_NO>(0, rMax);
      auto const tree = InitTree<DIMENSION_NO>(entities, depth, boxSpace);

      auto const planes = std::vector{
        PlaneND<DIMENSION_NO>{ .OrigoDistance = rMax * 0.9, .Normal = { 1.0, 0.0, 0.0 } },
        PlaneND<DIMENSION_NO>{ .OrigoDistance = rMax * 0.9, .Normal = { 0.0, 1.0, 0.0 } },
        PlaneND<DIMENSION_NO>{ .OrigoDistance = rMax * 0.9, .Normal = { 0.0, 0.0, 1.0 } }
      };
      for (auto _ : state)
      {
        tree.FrustumCulling(planes, entities, 0.1);
      }
    }


    template<typename TExecMode>
    static void CollisionDetection(benchmark::State& state)
    {
      constexpr dim_t DIMENSION_NO = 3;
      constexpr depth_t depth = depthEdit;

      size_t entityNo = state.range();
      auto const entities = GenerateBoxesRandom<DIMENSION_NO>(entityNo, 0, 0.05);
      auto const boxSpace = CreateSearcBox<DIMENSION_NO>(0, rMax);
      auto const tree = InitTree<DIMENSION_NO>(entities, depth, boxSpace);

      for (auto _ : state)
      {
        tree.template CollisionDetection<TExecMode>(entities);
      }
    }


    static void CollisionDetection__WithOtherTree(benchmark::State& state)
    {
      constexpr dim_t DIMENSION_NO = 3;
      constexpr depth_t depth = depthEdit;

      size_t entityNo = state.range();
      auto const entities0 = GenerateBoxesRandom<DIMENSION_NO>(entityNo, 0, 0.05);
      auto const entities1 = GenerateBoxesRandom<DIMENSION_NO>(entityNo, 1, 0.05);
      auto const boxSpace = CreateSearcBox<DIMENSION_NO>(0, rMax);
      auto const tree0 = InitTree<DIMENSION_NO>(entities0, depth, boxSpace);
      auto const tree1 = InitTree<DIMENSION_NO>(entities1, depth, boxSpace);

      for (auto _ : state)
      {
        Tree<DIMENSION_NO>::CollisionDetection(tree0, entities0, tree1, entities1);
      }
    }


    template<bool IS_FIRST>
    static void RayIntersectedGeneral(benchmark::State& state)
    {
      constexpr dim_t DIMENSION_NO = 3;
      constexpr depth_t depth = depthEdit;

      size_t entityNo = state.range();

      auto const entities = GenerateBoxesRandom<DIMENSION_NO>(entityNo, 2, 0.02);
      auto const boxSpace = CreateSearcBox<DIMENSION_NO>(0, rMax);
      auto const tree = InitTree<DIMENSION_NO>(entities, depth, boxSpace);

      size_t constexpr rayNo = 100;
      auto const rayOrigins = GeneratePointsRandom<DIMENSION_NO>(rayNo, 3);
      auto const rayDirections = GeneratePointsRandom<DIMENSION_NO>(rayNo, 4);

      auto rays = std::vector<RayND<DIMENSION_NO>>{};
      for (size_t rayID = 0; rayID < rayNo; ++rayID)
      {
        auto const direction = GetNormalized<DIMENSION_NO>(rayDirections[rayNo - 1 - rayID]);
        if (!direction)
        {
          continue;
        }
        rays.push_back(RayND<DIMENSION_NO>{ .Origin = rayOrigins[rayID], .Direction = *direction });
      }

      size_t entityID = 0;
      for (auto _ : state)
      {
        auto const& ray = rays[entityID % rays.size()];
        if constexpr (IS_FIRST)
        {
          benchmark::DoNotOptimize(tree.RayIntersectedFirst(ray.Origin, ray.Direction, entities, 0));
        }
        else
        {
          benchmark::DoNotOptimize(tree.RayIntersectedAll(ray.Origin, ray.Direction, entities, 0));
        }
        ++entityID;
      }
    }

    static void RayIntersectedFirst(benchmark::State& state) { RayIntersectedGeneral<true>(state); }

    static void RayIntersectedAll(benchmark::State& state) { RayIntersectedGeneral<false>(state); }
  }; // struct BoxBenchmarks

  namespace Hashing
  {
    template<typename THash>
    static void Morton_UnorderedMap_Lookup(benchmark::State& state)
    {
      using GA = GeneralGeometryAdapterND<3>;
      constexpr dim_t DIMENSION_NO = 3;
      constexpr depth_t depth = 19;
      using SI = detail::MortonSpaceIndexing<DIMENSION_NO, depth>;
      using NodeID = typename SI::NodeID;
      using Node = detail::OrthoTreeNodeData<SI::CHILD_NO, NodeID, typename SI::ChildID, uint32_t, PointND<DIMENSION_NO>>;

      size_t const entityNo = state.range();
      auto const points = GeneratePointsRandom<DIMENSION_NO>(entityNo);
      auto const boxSpace = CreateSearcBox<DIMENSION_NO>(0, rMax);
      detail::MortonGridSpaceIndexing<GA, true, 1.0, depth> modelSpaceGrid(depth, detail::InternalGeometryModule<GA>::GetBoxAD(boxSpace));

      std::vector<NodeID> nodeIDs;
      nodeIDs.reserve(std::bit_ceil(entityNo));
      for (auto const& p : points)
        nodeIDs.push_back(modelSpaceGrid.GetNodeID(p));

      std::unordered_map<NodeID, Node, THash> nodeMap;
      nodeMap.reserve(std::bit_ceil(entityNo));
      for (auto const& id : nodeIDs)
        nodeMap.emplace(id, Node{});

      for (auto _ : state)
      {
        for (auto const& id : nodeIDs)
          benchmark::DoNotOptimize(nodeMap.at(id));
      }

      state.SetItemsProcessed(state.iterations() * entityNo);
    }
  } // namespace Hashing

  namespace Memory
  {
    template<typename TMemoryResource>
    static void Workload(benchmark::State& state)
    {
      using T = uint64_t;
      TMemoryResource mr;

      struct LocalSegment
      {
        typename TMemoryResource::MemorySegment ms;
        size_t size = 0;
      };

      std::vector<LocalSegment> segments;
      segments.reserve(state.range(0));

      std::mt19937 rng(42);
      // Probabilities for size categories: 70% (<4), 20% (<16), 7% (>100), 3% (>1000)
      std::discrete_distribution<> sizeDist({ 70, 20, 7, 3 });
      std::uniform_int_distribution<size_t> smallDist(1, 3);
      std::uniform_int_distribution<size_t> mediumDist(4, 15);
      std::uniform_int_distribution<size_t> largeDist(101, 200);
      std::uniform_int_distribution<size_t> hugeDist(1001, 2000);

      // Probabilities for actions: 40% Allocate, 40% Deallocate, 10% Grow (+1), 10% Shrink (-1)
      std::discrete_distribution<> actionDist({ 40, 40, 10, 10 });

      for (auto _ : state)
      {
        int action = actionDist(rng);
        if (action == 0 || segments.empty()) // Allocate
        {
          int cat = sizeDist(rng);
          size_t sz = 0;
          switch (cat)
          {
          case 0: sz = smallDist(rng); break;
          case 1: sz = mediumDist(rng); break;
          case 2: sz = largeDist(rng); break;
          case 3: sz = hugeDist(rng); break;
          }
          segments.push_back({ mr.Allocate(sz), sz });
        }
        else if (action == 1) // Deallocate
        {
          std::uniform_int_distribution<size_t> indexDist(0, segments.size() - 1);
          size_t idx = indexDist(rng);
          mr.Deallocate(segments[idx].ms);
          segments[idx] = segments.back();
          segments.pop_back();
        }
        else if (action == 2) // Grow (1-1 elem addition/increase)
        {
          std::uniform_int_distribution<size_t> indexDist(0, segments.size() - 1);
          size_t idx = indexDist(rng);
          mr.IncreaseSegment(segments[idx].ms, 1);
          segments[idx].size += 1;
        }
        else // Shrink (1-1 elem deletion/decrease)
        {
          std::uniform_int_distribution<size_t> indexDist(0, segments.size() - 1);
          size_t idx = indexDist(rng);
          if (segments[idx].size > 1)
          {
            mr.DecreaseSegment(segments[idx].ms, 1);
            segments[idx].size -= 1;
          }
        }
      }

      for (auto& s : segments)
        mr.Deallocate(s.ms);

      state.SetItemsProcessed(state.iterations());
    }

    // TreeLikePattern: simulates a complete tree lifecycle, fully measured.
    //
    // state.range(0) = total entity count (entityNo).
    //
    // Each benchmark iteration = one full tree lifecycle:
    //
    //   Phase 1 — Create (75%, MEASURED):
    //     Init primary to createEntityNo = entityNo * 3/4.
    //     Allocate createNodeCount node segments (1-11 entities each, summing to
    //     ~createEntityNo), filling the primary buffer.  No deletions here —
    //     mirrors tree Create().  MemoryResource serves these from the
    //     cache-friendly contiguous primary buffer.
    //
    //   Phase 2 — Dynamic ops (25%, MEASURED):
    //     dynPoolSize nodes start fresh on the fallback/heap (primary is full).
    //     dynOpCount random ops: 50% IncreaseSegment (+1 / AddNodeEntity),
    //                            40% DecreaseSegment (-1 / RemoveNodeEntity),
    //                            10% node rebuild (Deallocate + Allocate).
    //
    // SetItemsProcessed = (createNodeCount + dynOpCount) per iteration.
    template<typename TMemoryResource>
    static void TreeLikePattern(benchmark::State& state)
    {
      using T = uint64_t;
      size_t const entityNo       = static_cast<size_t>(state.range(0));
      size_t const createEntityNo = entityNo * 3 / 4;  // 75 %

      constexpr size_t AVG_PER_NODE = 6; // realistic for maxElementNum = 11
      size_t const createNodeCount  = std::max<size_t>(10, createEntityNo / AVG_PER_NODE);
      size_t const dynOpCount       = std::max<size_t>(5,  createNodeCount / 3); // ≈25 %
      constexpr size_t dynPoolSize  = 50;

      // Pre-generate all random data outside the measured loop.
      std::mt19937 rng(42);
      std::uniform_int_distribution<size_t> createSzDist(1, 11);
      std::uniform_int_distribution<size_t> dynSzDist(1, 8);
      std::uniform_int_distribution<size_t> dynIdxDist(0, dynPoolSize - 1);
      std::discrete_distribution<>          dynActionDist({ 50, 40, 10 });

      std::vector<size_t> createSizes(createNodeCount);
      for (auto& s : createSizes) s = createSzDist(rng);

      std::vector<size_t> dynInitSizes(dynPoolSize);
      for (auto& s : dynInitSizes) s = dynSzDist(rng);

      struct DynOp { size_t idx; int action; size_t newSize; };
      std::vector<DynOp> dynOps(dynOpCount);
      for (auto& op : dynOps)
        op = { dynIdxDist(rng), dynActionDist(rng), dynSzDist(rng) };

      struct NodeSegment
      {
        typename TMemoryResource::MemorySegment ms;
        size_t                                  size = 0;
      };
      std::vector<NodeSegment>           createNodes(createNodeCount);
      std::array<NodeSegment, dynPoolSize> dynNodes;

      // --- Measured loop: full lifecycle per iteration -------------------
      for (auto _ : state)
      {
        TMemoryResource mr;

        // Phase 1: Create — fill primary buffer with create-phase node segments.
        if constexpr (requires { mr.Init(createEntityNo); })
          mr.Init(createEntityNo);

        {
          size_t budget = createEntityNo;
          for (size_t i = 0; i < createNodeCount; ++i)
          {
            size_t const sz  = (budget > 0) ? std::min(createSizes[i], budget) : 1;
            createNodes[i]   = { mr.Allocate(sz), sz };
            budget           = (budget >= sz) ? budget - sz : 0;
          }
        }

        // Phase 2: Dynamic ops — primary full, all dyn nodes land on fallback.
        for (size_t i = 0; i < dynPoolSize; ++i)
          dynNodes[i] = { mr.Allocate(dynInitSizes[i]), dynInitSizes[i] };

        for (auto const& op : dynOps)
        {
          auto& node = dynNodes[op.idx];
          switch (op.action)
          {
          case 0: // AddNodeEntity: grow by 1
            mr.IncreaseSegment(node.ms, 1);
            ++node.size;
            break;
          case 1: // RemoveNodeEntity: shrink by 1
            if (node.size > 1) { mr.DecreaseSegment(node.ms, 1); --node.size; }
            break;
          default: // Node rebuild
            mr.Deallocate(node.ms);
            node = { mr.Allocate(op.newSize), op.newSize };
            break;
          }
        }

        // Cleanup
        for (auto& n : dynNodes)    mr.Deallocate(n.ms);
        for (auto& n : createNodes) mr.Deallocate(n.ms);
      }

      state.SetItemsProcessed(state.iterations() * static_cast<int64_t>(createNodeCount + dynOpCount));
    }

    // Helper for std::allocator comparison baseline
    struct StdAllocatorWrapper
    {
      using T = uint64_t;
      struct MemorySegment
      {
        std::vector<T> data;
      };
      MemorySegment Allocate(size_t n) { return { std::vector<T>(n) }; }
      void Deallocate(MemorySegment& ms) { std::vector<T>().swap(ms.data); } // release memory immediately (fair comparison)
      void IncreaseSegment(MemorySegment& ms, size_t n) { ms.data.resize(ms.data.size() + n); }
      void DecreaseSegment(MemorySegment& ms, size_t n)
      {
        if (ms.data.size() >= n)
          ms.data.resize(ms.data.size() - n);
      }
    };

  } // namespace Memory
} // namespace Benchmarks

static constexpr auto unit = benchmark::kMicrosecond;


BENCHMARK(Benchmarks::Memory::Workload<detail::MemoryResource<uint64_t>>)->Arg(1000)->Arg(10000)->Arg(100'000)->Unit(benchmark::kMicrosecond);
BENCHMARK(Benchmarks::Memory::Workload<Benchmarks::Memory::StdAllocatorWrapper>)->Arg(1000)->Arg(10000)->Arg(100'000)->Unit(benchmark::kMicrosecond);

BENCHMARK(Benchmarks::Memory::TreeLikePattern<detail::MemoryResource<uint64_t>>)->Arg(1000)->Arg(10000)->Arg(100'000)->Unit(benchmark::kMicrosecond);
BENCHMARK(Benchmarks::Memory::TreeLikePattern<Benchmarks::Memory::StdAllocatorWrapper>)->Arg(1000)->Arg(10000)->Arg(100'000)->Unit(benchmark::kMicrosecond);


BENCHMARK(Benchmarks::Hashing::Morton_UnorderedMap_Lookup<std::hash<uint32_t>>)->Arg(1000)->Arg(10000)->Arg(100000)->Unit(benchmark::kNanosecond);
BENCHMARK(Benchmarks::Hashing::Morton_UnorderedMap_Lookup<detail::SentinelMortonHash<uint32_t>>)->Arg(1000)->Arg(10000)->Arg(100000)->Unit(benchmark::kNanosecond);


BENCHMARK(Benchmarks::Base::GetNodeID)->Arg(1000)->Unit(benchmark::kNanosecond);
BENCHMARK(Benchmarks::Base::GetDepthID)->Arg(1000)->Unit(benchmark::kNanosecond);
// BENCHMARK(Benchmarks::Base::GetNodeEntities)->Arg(10000)->Arg(100000)->Unit(benchmark::kNanosecond);
BENCHMARK(Benchmarks::Base::GridSpaceIndexing_GetPointGridID)->Arg(1000)->Unit(benchmark::kNanosecond);
BENCHMARK(Benchmarks::Base::GridSpaceIndexing_GetBoxGridID)->Arg(1000)->Unit(benchmark::kNanosecond);


// Point tree dynamic-only benchmarks
#define POINT_BENCHMARKS_DYN_FUNCS_Dynamic(NAME)                                                                                                            \
  BENCHMARK(Benchmarks::NAME::Insert__Bulk<3, SeqExec>)->Arg(10)->Arg(20)->Arg(50)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Arg(1000000)->Unit(unit); \
  BENCHMARK(Benchmarks::NAME::Insert__Bulk<3, ParExec>)->Arg(10)->Arg(20)->Arg(50)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Arg(1000000)->Unit(unit); \
  BENCHMARK(Benchmarks::NAME::Insert__RebalanceMode)->Arg(10)->Arg(20)->Arg(50)->Arg(100)->Arg(1000)->Arg(10000)->Unit(unit);                               \
  BENCHMARK(Benchmarks::NAME::InsertIntoLeaf)->Arg(10)->Arg(20)->Arg(50)->Arg(100)->Arg(1000)->Arg(10000)->Unit(unit);                                      \
  BENCHMARK(Benchmarks::NAME::InsertUnique)->Arg(10)->Arg(20)->Arg(50)->Arg(100)->Arg(1000)->Arg(10000)->Unit(unit);                                        \
  BENCHMARK(Benchmarks::NAME::Update)->Arg(10)->Arg(20)->Arg(50)->Arg(100)->Arg(1000)->Arg(10000)->Unit(unit);
#define POINT_BENCHMARKS_DYN_FUNCS_Static(NAME)

// Box tree dynamic-only benchmarks (InsertToLeaf instead of InsertIntoLeaf/InsertUnique)
#define BOX_BENCHMARKS_DYN_FUNCS_Dynamic(NAME)                                                                                                              \
  BENCHMARK(Benchmarks::NAME::Insert__Bulk<3, SeqExec>)->Arg(10)->Arg(20)->Arg(50)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Arg(1000000)->Unit(unit); \
  BENCHMARK(Benchmarks::NAME::Insert__Bulk<3, ParExec>)->Arg(10)->Arg(20)->Arg(50)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Arg(1000000)->Unit(unit); \
  BENCHMARK(Benchmarks::NAME::InsertToLeaf)->Arg(10)->Arg(20)->Arg(50)->Arg(100)->Arg(1000)->Arg(10000)->Unit(unit);                                        \
  BENCHMARK(Benchmarks::NAME::Insert__RebalanceMode)->Arg(10)->Arg(20)->Arg(50)->Arg(100)->Arg(1000)->Arg(10000)->Unit(unit);                               \
  BENCHMARK(Benchmarks::NAME::Update)->Arg(10)->Arg(20)->Arg(50)->Arg(100)->Arg(1000)->Arg(10000)->Unit(unit);
#define BOX_BENCHMARKS_DYN_FUNCS_Static(NAME)

#define POINT_BENCHMARKS(NAME, IS_DYN, NODE_STORAGE, USE_REVERSE_MAPPING, USE_PMR)                                                                    \
  namespace Benchmarks                                                                                                                                \
  {                                                                                                                                                   \
    using NAME = PointBenchmarks<TreeType::CONCAT(IS_DYN, Ortho), NODE_STORAGE, USE_REVERSE_MAPPING>;                                                 \
  }                                                                                                                                                   \
  BENCHMARK(Benchmarks::NAME::Create<3, SeqExec>)->Arg(10)->Arg(20)->Arg(50)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Arg(1000000)->Unit(unit); \
  BENCHMARK(Benchmarks::NAME::Create<3, ParExec>)->Arg(10)->Arg(20)->Arg(50)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Arg(1000000)->Unit(unit); \
  BENCHMARK(Benchmarks::NAME::Create__OversizedModelspace<3, SeqExec>)                                                                                \
    ->Arg(10)                                                                                                                                         \
    ->Arg(20)                                                                                                                                         \
    ->Arg(50)                                                                                                                                         \
    ->Arg(100)                                                                                                                                        \
    ->Arg(1000)                                                                                                                                       \
    ->Arg(10000)                                                                                                                                      \
    ->Arg(100000)                                                                                                                                     \
    ->Arg(1000000)                                                                                                                                    \
    ->Unit(unit);                                                                                                                                     \
  BENCHMARK(Benchmarks::NAME::Create__OversizedModelspace<3, ParExec>)                                                                                \
    ->Arg(10)                                                                                                                                         \
    ->Arg(20)                                                                                                                                         \
    ->Arg(50)                                                                                                                                         \
    ->Arg(100)                                                                                                                                        \
    ->Arg(1000)                                                                                                                                       \
    ->Arg(10000)                                                                                                                                      \
    ->Arg(100000)                                                                                                                                     \
    ->Arg(1000000)                                                                                                                                    \
    ->Unit(unit);                                                                                                                                     \
  BENCHMARK(Benchmarks::NAME::Create<6, SeqExec>)->Arg(10)->Arg(20)->Arg(50)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Arg(1000000)->Unit(unit); \
  BENCHMARK(Benchmarks::NAME::Create<6, ParExec>)->Arg(10)->Arg(20)->Arg(50)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Arg(1000000)->Unit(unit); \
  POINT_BENCHMARKS_DYN_FUNCS_##IS_DYN(NAME) BENCHMARK(Benchmarks::NAME::RangeSearch)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000);                   \
  BENCHMARK(Benchmarks::NAME::GetNearestNeighbors)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Unit(unit);                                         \
  BENCHMARK(Benchmarks::NAME::GetNearestNeighbors<6>)->Arg(1000)->Arg(10000)->Unit(unit);                                                             \
  BENCHMARK(Benchmarks::NAME::FrustumCulling)->Arg(1000)->Arg(10000)->Unit(unit);

#define BOX_BENCHMARKS(NAME, TREE_TYPE, TREE_IMPL, LOOSE_MODE, NODE_STORAGE, USE_REVERSE_MAPPING, USE_PMR)                                                               \
  namespace Benchmarks                                                                                                                                                   \
  {                                                                                                                                                                      \
    using NAME = BoxBenchmarks<TreeType::CONCAT(TREE_TYPE, TREE_IMPL), LOOSE_MODE, NODE_STORAGE, USE_REVERSE_MAPPING>;                                                   \
  }                                                                                                                                                                      \
  BENCHMARK(Benchmarks::NAME::Create<3, SeqExec>)->Arg(10)->Arg(20)->Arg(50)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Arg(1000000)->Unit(benchmark::kMicrosecond); \
  BENCHMARK(Benchmarks::NAME::Create<3, ParExec>)->Arg(10)->Arg(20)->Arg(50)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Arg(1000000)->Unit(benchmark::kMicrosecond); \
  BENCHMARK(Benchmarks::NAME::Create__OversizedModelspace<3, SeqExec>)                                                                                                   \
    ->Arg(10)                                                                                                                                                            \
    ->Arg(20)                                                                                                                                                            \
    ->Arg(50)                                                                                                                                                            \
    ->Arg(100)                                                                                                                                                           \
    ->Arg(1000)                                                                                                                                                          \
    ->Arg(10000)                                                                                                                                                         \
    ->Arg(100000)                                                                                                                                                        \
    ->Arg(1000000)                                                                                                                                                       \
    ->Unit(benchmark::kMicrosecond);                                                                                                                                     \
  BENCHMARK(Benchmarks::NAME::Create__OversizedModelspace<3, ParExec>)                                                                                                   \
    ->Arg(10)                                                                                                                                                            \
    ->Arg(20)                                                                                                                                                            \
    ->Arg(50)                                                                                                                                                            \
    ->Arg(100)                                                                                                                                                           \
    ->Arg(1000)                                                                                                                                                          \
    ->Arg(10000)                                                                                                                                                         \
    ->Arg(100000)                                                                                                                                                        \
    ->Arg(1000000)                                                                                                                                                       \
    ->Unit(unit);                                                                                                                                                        \
  BOX_BENCHMARKS_DYN_FUNCS_##TREE_TYPE(NAME) BENCHMARK(Benchmarks::NAME::PickSearch)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000);                                      \
  BENCHMARK(Benchmarks::NAME::RangeSearch)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000);                                                                                \
  BENCHMARK(Benchmarks::NAME::FrustumCulling)->Arg(1000)->Arg(10000)->Unit(unit);                                                                                        \
  BENCHMARK(Benchmarks::NAME::CollisionDetection<SeqExec>)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Unit(unit);                                                    \
  BENCHMARK(Benchmarks::NAME::CollisionDetection<ParExec>)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Unit(unit);                                                    \
  BENCHMARK(Benchmarks::NAME::CollisionDetection__WithOtherTree)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Unit(unit);                                              \
  BENCHMARK(Benchmarks::NAME::RayIntersectedFirst)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Unit(unit);                                                            \
  BENCHMARK(Benchmarks::NAME::RayIntersectedAll)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Unit(unit);

BOX_BENCHMARKS(StaticBVHBox__MBR_WithoutReverseMap, Static, BVH, LooseMode::Regular, NodeGeometryStorage::MBR, false, false);

POINT_BENCHMARKS(DynamicOrthoTreePoint__MinPoint_Regular_WithoutReverseMap, Dynamic, NodeGeometryStorage::MinPoint, false, false);
POINT_BENCHMARKS(DynamicOrthoTreePoint__MinPoint_Regular_WithReverseMap, Dynamic, NodeGeometryStorage::MinPoint, true, false);
POINT_BENCHMARKS(DynamicOrthoTreePoint__MBR_Regular_WithoutReverseMap, Dynamic, NodeGeometryStorage::MBR, false, false);
POINT_BENCHMARKS(DynamicOrthoTreePoint__MBR_Regular_WithReverseMap, Dynamic, NodeGeometryStorage::MBR, true, false);

POINT_BENCHMARKS(StaticOrthoTreePoint__MBR_Regular_WithoutReverseMap, Static, NodeGeometryStorage::MBR, false, false);
POINT_BENCHMARKS(StaticOrthoTreePoint__MBR_Regular_WithReverseMap, Static, NodeGeometryStorage::MBR, true, false);


BOX_BENCHMARKS(DynamicOrthoTreeBox__MinPoint_Regular_WithoutReverseMap, Dynamic, Ortho, LooseMode::Regular, NodeGeometryStorage::MinPoint, false, false);
BOX_BENCHMARKS(DynamicOrthoTreeBox__MinPoint_Loose_WithoutReverseMap, Dynamic, Ortho, LooseMode::Loose, NodeGeometryStorage::MinPoint, false, false);
BOX_BENCHMARKS(DynamicOrthoTreeBox__MBR_Regular_WithoutReverseMap, Dynamic, Ortho, LooseMode::Regular, NodeGeometryStorage::MBR, false, false);
BOX_BENCHMARKS(DynamicOrthoTreeBox__MBR_Loose_WithoutReverseMap, Dynamic, Ortho, LooseMode::Loose, NodeGeometryStorage::MBR, false, false);
BOX_BENCHMARKS(DynamicOrthoTreeBox__MinPoint_Regular_WithReverseMap, Dynamic, Ortho, LooseMode::Regular, NodeGeometryStorage::MinPoint, true, false);
BOX_BENCHMARKS(DynamicOrthoTreeBox__MinPoint_Loose_WithReverseMap, Dynamic, Ortho, LooseMode::Loose, NodeGeometryStorage::MinPoint, true, false);
BOX_BENCHMARKS(DynamicOrthoTreeBox__MBR_Regular_WithReverseMap, Dynamic, Ortho, LooseMode::Regular, NodeGeometryStorage::MBR, true, false);
BOX_BENCHMARKS(DynamicOrthoTreeBox__MBR_Loose_WithReverseMap, Dynamic, Ortho, LooseMode::Loose, NodeGeometryStorage::MBR, true, false);


BOX_BENCHMARKS(StaticOrthoTreeBox__MinPoint_Regular_WithoutReverseMap, Static, Ortho, LooseMode::Regular, NodeGeometryStorage::MinPoint, false, false);
BOX_BENCHMARKS(StaticOrthoTreeBox__MinPoint_Loose_WithoutReverseMap, Static, Ortho, LooseMode::Loose, NodeGeometryStorage::MinPoint, false, false);
BOX_BENCHMARKS(StaticOrthoTreeBox__MBR_Regular_WithoutReverseMap, Static, Ortho, LooseMode::Regular, NodeGeometryStorage::MBR, false, false);
BOX_BENCHMARKS(StaticOrthoTreeBox__MBR_Loose_WithoutReverseMap, Static, Ortho, LooseMode::Loose, NodeGeometryStorage::MBR, false, false);

// Run the benchmark
BENCHMARK_MAIN();
