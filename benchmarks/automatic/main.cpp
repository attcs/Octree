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
        constexpr depth_t depth = 3;

        size_t entityNo = state.range();

        const auto points = GeneratePointsRandom<DIMENSION_NO>(entityNo);
        const auto tree = TreePointND<DIMENSION_NO>(points, depth);
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
        constexpr depth_t depth = 3;

        size_t entityNo = state.range();

        const auto points = GeneratePointsRandom<DIMENSION_NO>(entityNo);
        const auto tree = TreePointND<DIMENSION_NO>(points, depth);
        using NodeID = typename TreePointND<DIMENSION_NO>::MortonNodeID;
        std::vector<NodeID> entityIDs;
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
    } // namespace Base

    namespace Point
    {
      template<bool IS_PARALLEL_EXEC>
      void Create(benchmark::State& state)
      {
        constexpr dim_t DIMENSION_NO = 3;
        constexpr depth_t depth = 3;

        size_t entityNo = state.range();

        const auto points = GeneratePointsRandom<DIMENSION_NO>(entityNo);
        for (auto _ : state)
        {
          CreateTreePoint<DIMENSION_NO>(depth, points, IS_PARALLEL_EXEC);
        }
      }

      static void InsertToLeaf(benchmark::State& state)
      {
        constexpr dim_t DIMENSION_NO = 3;
        constexpr depth_t depth = 5;

        size_t entityNo = state.range();

        const auto points = GeneratePointsRandom<DIMENSION_NO>(entityNo);
        auto boxSpace = BoundingBoxND<DIMENSION_NO>{};
        boxSpace.Max.fill(rMax);

        using NodeID = typename TreePointND<DIMENSION_NO>::MortonNodeID;
        for (auto _ : state)
        {
          auto tree = TreePointND<DIMENSION_NO>();
          tree.Init(boxSpace, depth);

          std::size_t entityID = 0;
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

        const auto points = GeneratePointsRandom<DIMENSION_NO>(entityNo);
        auto boxSpace = BoundingBoxND<DIMENSION_NO>{};
        boxSpace.Max.fill(rMax);

        for (auto _ : state)
        {
          auto tree = TreePointND<DIMENSION_NO>();
          tree.Init(boxSpace, depth);

          std::size_t entityID = 0;
          for (auto const& point : points)
          {
            tree.InsertWithRebalancing(entityID, point, points);
            ++entityID;
          }
        }
      }

      static void Update(benchmark::State& state)
      {
        constexpr dim_t DIMENSION_NO = 3;
        constexpr depth_t depth = 5;

        size_t entityNo = state.range();

        const auto points = GeneratePointsRandom<DIMENSION_NO>(entityNo * 10);
        const auto updatePoints = GeneratePointsRandom<DIMENSION_NO>(entityNo);

        auto tree = TreePointND<DIMENSION_NO>(points, depth);
        for (auto _ : state)
        {
          for (size_t entityID = 0; entityID < updatePoints.size(); ++entityID)
          {
            tree.Update(entityID, points[entityID], updatePoints[entityID], points);
          }
        }
      }

      static void GetNearestNeighbors(benchmark::State& state)
      {
        constexpr dim_t DIMENSION_NO = 3;
        constexpr depth_t depth = 5;

        size_t entityNo = state.range();

        const auto points = GeneratePointsRandom<DIMENSION_NO>(entityNo);
        const auto tree = TreePointND<DIMENSION_NO>(points, depth);

        const auto searchPoints = GeneratePointsRandom<DIMENSION_NO>(std::max<std::size_t>(20, entityNo / 100));
        const auto pointNo = searchPoints.size();
        size_t pointID = 0;
        for (auto _ : state)
        {
          const auto& searchPoint = searchPoints[pointID % pointNo];
          tree.GetNearestNeighbors(searchPoint, std::max(2, static_cast<int>(std::floor(searchPoint[0]))), points);
          ++pointID;
        }
      }


    } // namespace
  } // namespace Benchmarks
} // namespace

BENCHMARK(Benchmarks::Base::GetNodeID)->Arg(1000);
BENCHMARK(Benchmarks::Base::GetDepthID)->Arg(1000);

BENCHMARK(Benchmarks::Point::Create<false>)->Arg(10)->Arg(20)->Arg(50)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Arg(1000000);
BENCHMARK(Benchmarks::Point::Create<true>)->Arg(10)->Arg(20)->Arg(50)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Arg(1000000);
BENCHMARK(Benchmarks::Point::InsertToLeaf)->Arg(10)->Arg(20)->Arg(50)->Arg(100)->Arg(1000)->Arg(10000);
BENCHMARK(Benchmarks::Point::InsertWithRebalancing)->Arg(10)->Arg(20)->Arg(50)->Arg(100)->Arg(1000)->Arg(10000);
BENCHMARK(Benchmarks::Point::Update)->Arg(10)->Arg(20)->Arg(50)->Arg(100)->Arg(1000)->Arg(10000);
BENCHMARK(Benchmarks::Point::GetNearestNeighbors)->Arg(10000);

// Run the benchmark
BENCHMARK_MAIN();