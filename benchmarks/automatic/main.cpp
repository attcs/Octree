#include <iostream>

#include "external/benchmark/include/benchmark/benchmark.h"

#include "../../octree.h"
#include "../manual/generators.h"
// #include "../unittests/compile_test.h"

using namespace OrthoTree;
namespace
{
  void SetIterationNo(benchmark::State& state, std::size_t no)
  {
    state.SetComplexityN(no);
    state.SetItemsProcessed(state.iterations() * no);
  }

  namespace Benchmarks
  {
    namespace Base
    {
      void GetNodeID(benchmark::State& state)
      {
        constexpr dim_t DIMENSION_NO = 3;
        constexpr depth_t depth = 5;

        size_t entityNo = state.range();

        auto const points = GeneratePointsRandom<DIMENSION_NO>(entityNo);
        auto const tree = TreePointND<DIMENSION_NO>(points, depth);
        using NodeID = typename TreePointND<DIMENSION_NO>::MortonNodeID;

        std::vector<NodeID> entityIDs(entityNo);
        for (auto _ : state)
        {
          for (int i = 0; i < entityNo; ++i)
          {
            entityIDs[i] = tree.GetNodeID(points[i]);
          }
        }

        SetIterationNo(state, entityNo);
      }

      static void GetDepthID(benchmark::State& state)
      {
        constexpr dim_t DIMENSION_NO = 3;
        constexpr depth_t depth = 5;

        size_t entityNo = state.range();

        auto const points = GeneratePointsRandom<DIMENSION_NO>(entityNo);
        auto const tree = TreePointND<DIMENSION_NO>(points, depth);

        using NodeID = typename TreePointND<DIMENSION_NO>::MortonNodeID;
        auto entityIDs = std::vector<NodeID>{};
        for (auto const& point : points)
          entityIDs.emplace_back(tree.GetNodeID(point));

        std::vector<depth_t> depthIDs(entityNo);
        for (auto _ : state)
        {
          for (int i = 0; i < entityNo; ++i)
          {
            depthIDs[i] = detail::MortonSpaceIndexing<DIMENSION_NO>::GetDepthID(entityIDs[i]);
          }
        }
        SetIterationNo(state, entityNo);
      }

      static void GetNodeEntities(benchmark::State& state)
      {
        constexpr dim_t DIMENSION_NO = 3;
        constexpr depth_t depth = 5;

        using Node = typename TreePointND<DIMENSION_NO>::Node;

        size_t entityNo = state.range();

        auto const points = GeneratePointsRandom<DIMENSION_NO>(entityNo);
        auto const tree = TreePointND<DIMENSION_NO>(points, depth);

        auto const& nodes = tree.GetNodes();

        auto nodePtrs = std::vector<Node const*>();
        for (auto const& [key, node] : nodes)
          nodePtrs.emplace_back(&node);

        for (auto _ : state)
        {
          for (auto const* nodePtr : nodePtrs)
            tree.GetNodeEntities(*nodePtr);
        }
        SetIterationNo(state, entityNo);
      }

      static void GridSpaceIndexing_GetPointGridID(benchmark::State& state)
      {
        constexpr dim_t DIMENSION_NO = 3;
        constexpr depth_t depth = 5;
        using GridSpaceIndexing =
          detail::GridSpaceIndexing<DIMENSION_NO, OrthoTree::BaseGeometryType, VectorND<DIMENSION_NO>, BoundingBoxND<DIMENSION_NO>, AdaptorGeneralND<DIMENSION_NO>>;

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
        using GridSpaceIndexing =
          detail::GridSpaceIndexing<DIMENSION_NO, OrthoTree::BaseGeometryType, VectorND<DIMENSION_NO>, BoundingBoxND<DIMENSION_NO>, AdaptorGeneralND<DIMENSION_NO>>;

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


    namespace Point
    {
      template<dim_t DIMENSION_NO, bool IS_PARALLEL_EXEC>
      void Create(benchmark::State& state)
      {
        constexpr depth_t depth = 5;

        size_t entityNo = state.range();
        auto const boxSpace = CreateSearcBox<DIMENSION_NO>(0, rMax);
        auto const points = GeneratePointsRandom<DIMENSION_NO>(entityNo);
        auto constexpr executionTag = std::conditional_t<IS_PARALLEL_EXEC, ExecutionTags::Parallel, ExecutionTags::Sequential>{};
        for (auto _ : state)
        {
          auto tree = TreePointND<DIMENSION_NO>(executionTag, points, depth, boxSpace, DEFAULT_MAX_ELEMENT_IN_NODES);
        }
      }


      static void InsertToLeaf(benchmark::State& state)
      {
        constexpr dim_t DIMENSION_NO = 3;
        constexpr depth_t depth = 5;

        size_t entityNo = state.range();

        auto const points = GeneratePointsRandom<DIMENSION_NO>(entityNo);
        auto const boxSpace = CreateSearcBox<DIMENSION_NO>(0, rMax);

        for (auto _ : state)
        {
          auto tree = TreePointND<DIMENSION_NO>();
          tree.Init(boxSpace, depth);

          auto entityID = TreePointND<DIMENSION_NO>::TEntityID(0);
          for (auto const& point : points)
          {
            tree.Insert(entityID, point, true);
            ++entityID;
          }
        }
      }


      static void InsertWithRebalancing(benchmark::State& state)
      {
        constexpr dim_t DIMENSION_NO = 3;
        constexpr depth_t depth = 5;

        size_t entityNo = state.range();

        auto const points = GeneratePointsRandom<DIMENSION_NO>(entityNo);
        auto const boxSpace = CreateSearcBox<DIMENSION_NO>(0, rMax);

        for (auto _ : state)
        {
          auto tree = TreePointND<DIMENSION_NO>();
          tree.Init(boxSpace, depth);

          auto entityID = TreePointND<DIMENSION_NO>::TEntityID(0);
          for (auto const& point : points)
          {
            tree.InsertWithRebalancing(entityID, point, points);
            ++entityID;
          }
        }
      }


      static void InsertUnique(benchmark::State& state)
      {
        constexpr dim_t DIMENSION_NO = 3;
        constexpr depth_t depth = 5;

        size_t entityNo = state.range();

        auto const points = GeneratePointsRandom<DIMENSION_NO>(entityNo);
        auto const boxSpace = CreateSearcBox<DIMENSION_NO>(0, rMax);

        for (auto _ : state)
        {
          auto tree = TreePointND<DIMENSION_NO>();
          tree.Init(boxSpace, depth);

          auto entityID = TreePointND<DIMENSION_NO>::TEntityID(0);
          for (auto const& point : points)
          {
            if (tree.InsertUnique(entityID, point, rMax / 100.0, points))
            {
              ++entityID;
            }
          }
        }
      }


      static void Update(benchmark::State& state)
      {
        constexpr dim_t DIMENSION_NO = 3;
        constexpr depth_t depth = 5;

        size_t entityNo = state.range();

        auto const points = GeneratePointsRandom<DIMENSION_NO>(entityNo * 100, 0);
        auto const updatePoints = GeneratePointsRandom<DIMENSION_NO>(entityNo, 1);
        auto const updatePointsNo = TreePointND<DIMENSION_NO>::TEntityID(updatePoints.size());
        auto tree = TreePointND<DIMENSION_NO>(points, depth);
        for (auto _ : state)
        {
          for (TreePointND<DIMENSION_NO>::TEntityID entityID = 0; entityID < updatePointsNo; ++entityID)
          {
            tree.Update(entityID, points[entityID], updatePoints[entityID], points);
          }
        }
        SetIterationNo(state, entityNo);
      }


      static void Contains(benchmark::State& state)
      {
        constexpr dim_t DIMENSION_NO = 3;
        constexpr depth_t depth = 5;

        size_t entityNo = state.range();

        auto const points = GeneratePointsRandom<DIMENSION_NO>(entityNo);
        auto const tree = TreePointND<DIMENSION_NO>(points, depth);

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
        constexpr depth_t depth = 5;

        size_t entityNo = state.range();

        auto const points = GeneratePointsRandom<DIMENSION_NO>(entityNo);
        auto const tree = TreePointND<DIMENSION_NO>(points, depth);
        constexpr size_t boxNo = 100;
        auto const searchBoxes = GenerateBoxesRandom<DIMENSION_NO>(boxNo);

        size_t entityID = 0;
        for (auto _ : state)
        {
          tree.RangeSearch(searchBoxes[entityID % boxNo], points);
          ++entityID;
        }
      }


      static void GetNearestNeighbors(benchmark::State& state)
      {
        constexpr dim_t DIMENSION_NO = 3;
        constexpr depth_t depth = 5;

        size_t entityNo = state.range();

        auto const points = GeneratePointsRandom<DIMENSION_NO>(entityNo, 0);
        auto const tree = TreePointND<DIMENSION_NO>(points, depth);

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
        constexpr depth_t depth = 5;

        size_t entityNo = state.range();

        auto const points = GeneratePointsRandom<DIMENSION_NO>(entityNo);
        auto const tree = TreePointND<DIMENSION_NO>(points, depth);

        auto const planes = std::vector{
          PlaneND<DIMENSION_NO>{ .OrigoDistance = rMax * 0.9, .Normal = { 1.0, 0.0, 0.0 } },
          PlaneND<DIMENSION_NO>{ .OrigoDistance = rMax * 0.9, .Normal = { 0.0, 1.0, 0.0 } },
          PlaneND<DIMENSION_NO>{ .OrigoDistance = rMax * 0.9, .Normal = { 0.0, 0.0, 1.0 } }
        };
        for (auto _ : state)
        {
          tree.FrustumCulling(planes, 0.1, points);
        }
      }
    } // namespace Point


    namespace Box
    {
      template<dim_t DIMENSION_NO, uint32_t SPLIT_DEPTH_INCREASEMENT, bool IS_PARALLEL_EXEC>
      void Create(benchmark::State& state)
      {
        constexpr bool DO_SPLIT_PARENT_ENTITIES = SPLIT_DEPTH_INCREASEMENT > 0;
        constexpr depth_t depth = 5;

        size_t entityNo = state.range();
        auto const entities = GenerateBoxesRandom<DIMENSION_NO>(entityNo);
        auto const boxSpace = CreateSearcBox<DIMENSION_NO>(0, rMax);
        auto constexpr executionTag = std::conditional_t<IS_PARALLEL_EXEC, ExecutionTags::Parallel, ExecutionTags::Sequential>{};
        for (auto _ : state)
        {
          auto const tree = TreeBoxND<DIMENSION_NO, DO_SPLIT_PARENT_ENTITIES>(executionTag, entities, depth, boxSpace, DEFAULT_MAX_ELEMENT_IN_NODES);
        }
      }

      template<uint32_t SPLIT_DEPTH_INCREASEMENT>
      static void InsertToLeaf(benchmark::State& state)
      {
        constexpr bool DO_SPLIT_PARENT_ENTITIES = SPLIT_DEPTH_INCREASEMENT > 0;
        constexpr dim_t DIMENSION_NO = 3;
        constexpr depth_t depth = 5;

        using EntityID = TreeBoxND<DIMENSION_NO, DO_SPLIT_PARENT_ENTITIES>::TEntityID;

        size_t entityNo = state.range();

        auto const entities = GenerateBoxesRandom<DIMENSION_NO>(entityNo);
        auto const boxSpace = CreateSearcBox<DIMENSION_NO>(0, rMax);

        for (auto _ : state)
        {
          auto tree = TreeBoxND<DIMENSION_NO, DO_SPLIT_PARENT_ENTITIES>();
          tree.Init(boxSpace, depth);

          EntityID entityID = 0;
          for (auto const& entity : entities)
          {
            tree.Insert(entityID, entity, true);
            ++entityID;
          }
        }
      }


      template<uint32_t SPLIT_DEPTH_INCREASEMENT>
      static void InsertWithRebalancing(benchmark::State& state)
      {
        constexpr bool DO_SPLIT_PARENT_ENTITIES = SPLIT_DEPTH_INCREASEMENT > 0;
        constexpr dim_t DIMENSION_NO = 3;
        constexpr depth_t depth = 5;

        using EntityID = TreeBoxND<DIMENSION_NO, DO_SPLIT_PARENT_ENTITIES>::TEntityID;

        size_t entityNo = state.range();

        auto const entities = GenerateBoxesRandom<DIMENSION_NO>(entityNo);
        auto const boxSpace = CreateSearcBox<DIMENSION_NO>(0, rMax);

        for (auto _ : state)
        {
          auto tree = TreeBoxND<DIMENSION_NO, DO_SPLIT_PARENT_ENTITIES>();
          tree.Init(boxSpace, depth);

          EntityID entityID = 0;
          for (auto const& entity : entities)
          {
            tree.InsertWithRebalancing(entityID, entity, entities);
            ++entityID;
          }
        }
      }


      template<uint32_t SPLIT_DEPTH_INCREASEMENT = 0>
      static void Update(benchmark::State& state)
      {
        constexpr bool DO_SPLIT_PARENT_ENTITIES = SPLIT_DEPTH_INCREASEMENT > 0;
        constexpr dim_t DIMENSION_NO = 3;
        constexpr depth_t depth = 5;

        using EntityID = TreeBoxND<DIMENSION_NO, DO_SPLIT_PARENT_ENTITIES>::TEntityID;

        size_t entityNo = state.range();

        auto const boxSpace = CreateSearcBox<DIMENSION_NO>(0, rMax);
        auto entities = GenerateBoxesRandom<DIMENSION_NO>(entityNo * 10);
        auto const updateEntities = GenerateBoxesRandom<DIMENSION_NO>(entityNo, 1);
        auto const updateEntitiesNo = EntityID(updateEntities.size());

        auto tree = TreeBoxND<DIMENSION_NO, DO_SPLIT_PARENT_ENTITIES>(entities, depth, boxSpace);
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


      template<uint32_t SPLIT_DEPTH_INCREASEMENT>
      static void PickSearch(benchmark::State& state)
      {
        constexpr bool DO_SPLIT_PARENT_ENTITIES = SPLIT_DEPTH_INCREASEMENT > 0;
        constexpr dim_t DIMENSION_NO = 3;
        constexpr depth_t depth = 5;
        using EntityID = TreeBoxND<DIMENSION_NO, DO_SPLIT_PARENT_ENTITIES>::TEntityID;

        size_t entityNo = state.range();

        auto const entities = GenerateBoxesRandom<DIMENSION_NO>(entityNo);
        auto const boxSpace = CreateSearcBox<DIMENSION_NO>(0, rMax);
        auto const tree = TreeBoxND<DIMENSION_NO, DO_SPLIT_PARENT_ENTITIES>(entities, depth, boxSpace);

        constexpr size_t pointNo = 100;
        auto const searchPoints = GeneratePointsRandom<DIMENSION_NO>(pointNo, 1);

        EntityID entityID = 0;
        for (auto _ : state)
        {
          tree.PickSearch(searchPoints[entityID % pointNo], entities);
          ++entityID;
        }
      }


      template<uint32_t SPLIT_DEPTH_INCREASEMENT>
      static void RangeSearch(benchmark::State& state)
      {
        constexpr bool DO_SPLIT_PARENT_ENTITIES = SPLIT_DEPTH_INCREASEMENT > 0;
        constexpr dim_t DIMENSION_NO = 3;
        constexpr depth_t depth = 5;

        size_t entityNo = state.range();

        auto const entities = GenerateBoxesRandom<DIMENSION_NO>(entityNo);
        auto const boxSpace = CreateSearcBox<DIMENSION_NO>(0, rMax);
        auto const tree = TreeBoxND<DIMENSION_NO, DO_SPLIT_PARENT_ENTITIES>(entities, depth, boxSpace);

        constexpr size_t boxNo = 100;
        auto const searchBoxes = GenerateBoxesRandom<DIMENSION_NO>(boxNo, 1);

        size_t entityID = 0;
        for (auto _ : state)
        {
          tree.RangeSearch(searchBoxes[entityID % boxNo], entities);
          ++entityID;
        }
      }


      template<uint32_t SPLIT_DEPTH_INCREASEMENT>
      static void FrustumCulling(benchmark::State& state)
      {
        constexpr bool DO_SPLIT_PARENT_ENTITIES = SPLIT_DEPTH_INCREASEMENT > 0;
        constexpr dim_t DIMENSION_NO = 3;
        constexpr depth_t depth = 5;

        size_t entityNo = state.range();

        auto const entities = GenerateBoxesRandom<DIMENSION_NO>(entityNo);
        auto const boxSpace = CreateSearcBox<DIMENSION_NO>(0, rMax);
        auto const tree = TreeBoxND<DIMENSION_NO, DO_SPLIT_PARENT_ENTITIES>(entities, depth, boxSpace);

        auto const planes = std::vector{
          PlaneND<DIMENSION_NO>{ .OrigoDistance = rMax * 0.9, .Normal = { 1.0, 0.0, 0.0 } },
          PlaneND<DIMENSION_NO>{ .OrigoDistance = rMax * 0.9, .Normal = { 0.0, 1.0, 0.0 } },
          PlaneND<DIMENSION_NO>{ .OrigoDistance = rMax * 0.9, .Normal = { 0.0, 0.0, 1.0 } }
        };
        for (auto _ : state)
        {
          tree.FrustumCulling(planes, 0.1, entities);
        }
      }


      template<uint32_t SPLIT_DEPTH_INCREASEMENT, bool IS_PARALLEL_EXEC>
      static void CollisionDetection(benchmark::State& state)
      {
        constexpr bool DO_SPLIT_PARENT_ENTITIES = SPLIT_DEPTH_INCREASEMENT > 0;
        constexpr dim_t DIMENSION_NO = 3;
        constexpr depth_t depth = 5;

        size_t entityNo = state.range();
        auto const entities = GenerateBoxesRandom<DIMENSION_NO>(entityNo);
        auto const boxSpace = CreateSearcBox<DIMENSION_NO>(0, rMax);
        auto const tree = TreeBoxND<DIMENSION_NO, DO_SPLIT_PARENT_ENTITIES>(entities, depth, boxSpace);

        for (auto _ : state)
        {
          tree.template CollisionDetection<IS_PARALLEL_EXEC>(entities);
        }
      }


      template<uint32_t SPLIT_DEPTH_INCREASEMENT>
      static void CollisionDetection_WithOtherTree(benchmark::State& state)
      {
        constexpr bool DO_SPLIT_PARENT_ENTITIES = SPLIT_DEPTH_INCREASEMENT > 0;
        constexpr dim_t DIMENSION_NO = 3;
        constexpr depth_t depth = 5;

        size_t entityNo = state.range();
        auto const entities0 = GenerateBoxesRandom<DIMENSION_NO>(entityNo, 0);
        auto const entities1 = GenerateBoxesRandom<DIMENSION_NO>(entityNo, 1);
        auto const boxSpace = CreateSearcBox<DIMENSION_NO>(0, rMax);
        auto const tree0 = TreeBoxND<DIMENSION_NO, DO_SPLIT_PARENT_ENTITIES>(entities0, depth, boxSpace);
        auto const tree1 = TreeBoxND<DIMENSION_NO, DO_SPLIT_PARENT_ENTITIES>(entities1, depth, boxSpace);

        for (auto _ : state)
        {
          TreeBoxND<DIMENSION_NO, SPLIT_DEPTH_INCREASEMENT>::CollisionDetection(tree0, entities0, tree1, entities1);
        }
      }


      template<dim_t DIMENSION_NO>
      std::optional<PointND<DIMENSION_NO>> GetNormalized(PointND<DIMENSION_NO> const& direction)
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


      template<uint32_t SPLIT_DEPTH_INCREASEMENT, bool IS_FIRST>
      static void RayIntersectedGeneral(benchmark::State& state)
      {
        constexpr bool DO_SPLIT_PARENT_ENTITIES = SPLIT_DEPTH_INCREASEMENT > 0;
        constexpr dim_t DIMENSION_NO = 3;
        constexpr depth_t depth = 5;

        size_t entityNo = state.range();

        auto const entities = GenerateBoxesRandom<DIMENSION_NO>(entityNo, 2);
        auto const boxSpace = CreateSearcBox<DIMENSION_NO>(0, rMax);
        auto const tree = TreeBoxND<DIMENSION_NO, DO_SPLIT_PARENT_ENTITIES>(entities, depth, boxSpace);

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
            tree.RayIntersectedFirst(ray.Origin, ray.Direction, entities, 0);
          }
          else
          {
            tree.RayIntersectedAll(ray.Origin, ray.Direction, entities, 0);
          }
          ++entityID;
        }
      }

      template<uint32_t SPLIT_DEPTH_INCREASEMENT>
      static void RayIntersectedFirst(benchmark::State& state)
      {
        RayIntersectedGeneral<SPLIT_DEPTH_INCREASEMENT, true>(state);
      }

      template<uint32_t SPLIT_DEPTH_INCREASEMENT>
      static void RayIntersectedAll(benchmark::State& state)
      {
        RayIntersectedGeneral<SPLIT_DEPTH_INCREASEMENT, false>(state);
      }
    } // namespace Box
  } // namespace Benchmarks
} // namespace

/*
BENCHMARK(Benchmarks::Base::GetNodeID)->Arg(1000)->Unit(benchmark::kNanosecond);
BENCHMARK(Benchmarks::Base::GetDepthID)->Arg(1000)->Unit(benchmark::kNanosecond);
BENCHMARK(Benchmarks::Base::GetNodeEntities)->Arg(10000)->Unit(benchmark::kNanosecond);
BENCHMARK(Benchmarks::Base::GridSpaceIndexing_GetPointGridID)->Arg(1000)->Unit(benchmark::kNanosecond);
BENCHMARK(Benchmarks::Base::GridSpaceIndexing_GetBoxGridID)->Arg(1000)->Unit(benchmark::kNanosecond);
*/
BENCHMARK(Benchmarks::Point::Create<3, false>)->Arg(10)->Arg(20)->Arg(50)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Arg(1000000)->Unit(benchmark::kMillisecond);
BENCHMARK(Benchmarks::Point::Create<3, true>)->Arg(10)->Arg(20)->Arg(50)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Arg(1000000)->Unit(benchmark::kMillisecond);
/*
BENCHMARK(Benchmarks::Point::InsertToLeaf)->Arg(10)->Arg(20)->Arg(50)->Arg(100)->Arg(1000)->Arg(10000)->Unit(benchmark::kMillisecond);
BENCHMARK(Benchmarks::Point::InsertWithRebalancing)->Arg(10)->Arg(20)->Arg(50)->Arg(100)->Arg(1000)->Arg(10000)->Unit(benchmark::kMillisecond);
BENCHMARK(Benchmarks::Point::InsertUnique)->Arg(10)->Arg(20)->Arg(50)->Arg(100)->Arg(1000)->Arg(10000)->Unit(benchmark::kMillisecond);
BENCHMARK(Benchmarks::Point::Update)->Arg(10)->Arg(20)->Arg(50)->Arg(100)->Arg(1000)->Arg(10000)->Unit(benchmark::kMillisecond);
BENCHMARK(Benchmarks::Point::Contains)->Arg(1000)->Arg(10000);
BENCHMARK(Benchmarks::Point::RangeSearch)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000);
BENCHMARK(Benchmarks::Point::GetNearestNeighbors)->Arg(1000)->Arg(10000)->Unit(benchmark::kMillisecond);
BENCHMARK(Benchmarks::Point::FrustumCulling)->Arg(1000)->Arg(10000)->Unit(benchmark::kMillisecond);
BENCHMARK(Benchmarks::Box::Create<3, 0, false>)->Arg(10)->Arg(20)->Arg(50)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Arg(1000000)->Unit(benchmark::kMillisecond);
BENCHMARK(Benchmarks::Box::Create<3, 0, true>)->Arg(10)->Arg(20)->Arg(50)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Arg(1000000)->Unit(benchmark::kMillisecond);
BENCHMARK(Benchmarks::Box::Create<3, 1, false>)->Arg(10)->Arg(20)->Arg(50)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Arg(1000000)->Unit(benchmark::kMillisecond);
BENCHMARK(Benchmarks::Box::Create<3, 1, true>)->Arg(10)->Arg(20)->Arg(50)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Arg(1000000)->Unit(benchmark::kMillisecond);
BENCHMARK(Benchmarks::Box::InsertToLeaf<0>)->Arg(10)->Arg(20)->Arg(50)->Arg(100)->Arg(1000)->Arg(10000)->Unit(benchmark::kMillisecond);
BENCHMARK(Benchmarks::Box::InsertToLeaf<1>)->Arg(10)->Arg(20)->Arg(50)->Arg(100)->Arg(1000)->Arg(10000)->Unit(benchmark::kMillisecond);
BENCHMARK(Benchmarks::Box::InsertWithRebalancing<0>)->Arg(10)->Arg(20)->Arg(50)->Arg(100)->Arg(1000)->Arg(10000)->Unit(benchmark::kMillisecond);
BENCHMARK(Benchmarks::Box::InsertWithRebalancing<1>)->Arg(10)->Arg(20)->Arg(50)->Arg(100)->Arg(1000)->Arg(10000)->Unit(benchmark::kMillisecond);
BENCHMARK(Benchmarks::Box::Update<0>)->Arg(10)->Arg(20)->Arg(50)->Arg(100)->Arg(1000)->Arg(10000)->Unit(benchmark::kMillisecond);
BENCHMARK(Benchmarks::Box::Update<1>)->Arg(10)->Arg(20)->Arg(50)->Arg(100)->Arg(1000)->Arg(10000)->Unit(benchmark::kMillisecond);
BENCHMARK(Benchmarks::Box::PickSearch<0>)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000);
BENCHMARK(Benchmarks::Box::PickSearch<1>)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000);
BENCHMARK(Benchmarks::Box::RangeSearch<0>)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000);
BENCHMARK(Benchmarks::Box::RangeSearch<1>)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000);
BENCHMARK(Benchmarks::Box::FrustumCulling<0>)->Arg(1000)->Arg(10000)->Unit(benchmark::kMillisecond);
BENCHMARK(Benchmarks::Box::FrustumCulling<1>)->Arg(1000)->Arg(10000)->Unit(benchmark::kMillisecond);
BENCHMARK(Benchmarks::Box::CollisionDetection<0, false>)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Unit(benchmark::kMillisecond);
BENCHMARK(Benchmarks::Box::CollisionDetection<0, true>)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Unit(benchmark::kMillisecond);
BENCHMARK(Benchmarks::Box::CollisionDetection<1, false>)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Unit(benchmark::kMillisecond);
BENCHMARK(Benchmarks::Box::CollisionDetection<1, true>)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Unit(benchmark::kMillisecond);
BENCHMARK(Benchmarks::Box::CollisionDetection_WithOtherTree<0>)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Unit(benchmark::kMillisecond);
BENCHMARK(Benchmarks::Box::CollisionDetection_WithOtherTree<1>)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Unit(benchmark::kMillisecond);
BENCHMARK(Benchmarks::Box::RayIntersectedFirst<0>)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Unit(benchmark::kMillisecond);
BENCHMARK(Benchmarks::Box::RayIntersectedFirst<1>)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Unit(benchmark::kMillisecond);
BENCHMARK(Benchmarks::Box::RayIntersectedAll<0>)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Unit(benchmark::kMillisecond);
BENCHMARK(Benchmarks::Box::RayIntersectedAll<1>)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Unit(benchmark::kMillisecond);
*/
// Run the benchmark
BENCHMARK_MAIN();