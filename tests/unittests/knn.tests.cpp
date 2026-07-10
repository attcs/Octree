#include "pch.h"

#include <numeric>
#include <random>

using namespace OrthoTree;
using namespace OrthoTreeTestHelpers;
using std::array;
using std::vector;

namespace Point_kNNTest
{
  TEST(Point_kNNTest, N103_k2_RemainInSmallestNode__17_18)
  {
    auto constexpr N = 2;
    auto constexpr points = getPointSetNo1<N>();
    auto const tree = OrthoTreePointND<N>(points, 3, std::nullopt, 3);

    auto constexpr pt = PointND<N>{ 3.5, 5.5 };
    auto const vnn = tree.GetNearestNeighbors(pt, 2, points, 0.0);
    EXPECT_TRUE((std::ranges::is_permutation(vector<EntityID>{ 17, 18 }, vnn)));
  }

  TEST(Point_kNNTest, N103_k3__14_17_18)
  {
    auto constexpr N = 2;
    auto constexpr points = getPointSetNo1<N>();
    auto const tree = OrthoTreePointND<N>(points, 3, std::nullopt, 3);

    auto constexpr pt = PointND<N>{ 3.5, 5.5 };
    auto const vnn = tree.GetNearestNeighbors(pt, 3, points);
    EXPECT_TRUE((std::ranges::is_permutation(vector<EntityID>{ 17, 18, 14 }, vnn)));
  }

  TEST(Point_kNNTest, N103_k4__14_16_17_18)
  {
    auto constexpr N = 2;
    auto constexpr points = getPointSetNo1<N>();
    auto const tree = OrthoTreePointND<N>(points, 3, std::nullopt, 3);

    auto constexpr pt = PointND<N>{ 3.5, 5.5 };
    auto const vnn = tree.GetNearestNeighbors(pt, 4, points);
    EXPECT_TRUE((std::ranges::is_permutation(vector<EntityID>{ 17, 18, 14, 16 }, vnn)));
  }

  TEST(Point_kNNTest, N103_k4_within0__17_18)
  {
    auto constexpr N = 2;
    auto constexpr points = getPointSetNo1<N>();
    auto const tree = OrthoTreePointND<N>(points, 3, std::nullopt, 3);

    auto constexpr pt = PointND<N>{ 3.5, 5.5 };
    auto const vnn = tree.GetNearestNeighbors(pt, 4, 0, points);
    EXPECT_TRUE((std::ranges::is_permutation(vector<EntityID>{ 17, 18 }, vnn)));
  }

  TEST(Point_kNNTest, N103_k5_within1__14_17_18_16)
  {
    auto constexpr N = 2;
    auto constexpr points = getPointSetNo1<N>();
    auto const tree = OrthoTreePointND<N>(points, 3, std::nullopt, 3);

    auto constexpr pt = PointND<N>{ 3.5, 5.5 };
    auto const vnn = tree.GetNearestNeighbors(pt, 5, 1.0, points);
    EXPECT_TRUE((std::ranges::is_permutation(vector<EntityID>{ 17, 18, 14, 16 }, vnn)));
  }

  TEST(Point_kNNTest, N103_k100_OverTheContainingElements__All)
  {
    auto constexpr N = 2;
    auto constexpr points = getPointSetNo1<N>();
    auto const tree = OrthoTreePointND<N>(points, 3, std::nullopt, 3);

    auto constexpr pt = PointND<N>{ 3.5, 5.5 };
    auto const vnn = tree.GetNearestNeighbors(pt, 100, points);
    EXPECT_EQ(points.size(), vnn.size());

    auto Entities = vector<EntityID>(points.size());
    std::iota(begin(Entities), end(Entities), 0);
    EXPECT_TRUE((std::ranges::is_permutation(Entities, vnn)));
  }

  TEST(Point_kNNTest, N4_k1__1)
  {
    auto constexpr N = 2;
    auto constexpr points = getPointSetNo1<N>();
    auto const tree = OrthoTreePointND<N>(points, 3, std::nullopt, 3);

    auto constexpr pt = PointND<N>{ 2.0, 2.0 };
    auto const vnn = tree.GetNearestNeighbors(pt, 1, points);
    EXPECT_TRUE((std::ranges::is_permutation(vector<EntityID>{ 1 }, vnn)));
  }

  TEST(Point_kNNTest, N4_k2__1_11_12)
  {
    auto constexpr N = 2;
    auto constexpr points = getPointSetNo1<N>();
    auto const tree = OrthoTreePointND<N>(points, 3, std::nullopt, 3);

    auto constexpr pt = PointND<N>{ 2.0, 2.0 };
    auto const vnn = tree.GetNearestNeighbors(pt, 2, points);
    EXPECT_TRUE((std::ranges::is_permutation(vector<EntityID>{ 1, 11 }, vnn)));
  }

  TEST(Point_kNNTest, N4_k3__1_11_12)
  {
    auto constexpr N = 2;
    auto constexpr points = getPointSetNo1<N>();
    auto const tree = OrthoTreePointND<N>(points, 3, std::nullopt, 3);

    auto constexpr pt = PointND<N>{ 2.0, 2.0 };
    auto const vnn = tree.GetNearestNeighbors(pt, 3, points);
    EXPECT_TRUE((std::ranges::is_permutation(vector<EntityID>{ 1, 11, 12 }, vnn)));
  }

  TEST(Point_kNNTest, OutSide_k1__0)
  {
    auto constexpr N = 2;
    auto constexpr points = getPointSetNo1<N>();
    auto const tree = OrthoTreePointND<N>(points, 3, std::nullopt, 3);

    auto constexpr pt = PointND<N>{ -1.0, -1.0 };
    auto const vnn = tree.GetNearestNeighbors(pt, 1, points);
    EXPECT_TRUE((std::ranges::is_permutation(vector<EntityID>{ 0 }, vnn)));
  }

  TEST(Point_kNNTest, OutSide_k5__0_1_5_6_11)
  {
    auto constexpr N = 2;
    auto constexpr points = getPointSetNo1<N>();
    auto const tree = OrthoTreePointND<N>(points, 3, std::nullopt, 3);

    auto constexpr pt = PointND<N>{ -1.0, -1.0 };
    auto const vnn = tree.GetNearestNeighbors(pt, 5, points);
    EXPECT_TRUE((std::ranges::is_permutation(vector<EntityID>{ 0, 1, 5, 6, 11 }, vnn)));
  }

  TEST(Point_kNNTest, OutSide_k5_16D__0_1_5_6_11)
  {
    auto constexpr N = 23;
    auto constexpr points = getPointSetNo1<N>();
    auto const tree = OrthoTreePointND<N>(points, 3, std::nullopt, 3);

    auto constexpr pt = PointND<N>{ -1.0, -1.0 };
    auto const vnn = tree.GetNearestNeighbors(pt, 5, points);
    EXPECT_TRUE((std::ranges::is_permutation(vector<EntityID>{ 0, 1, 5, 6, 11 }, vnn)));
  }

  TEST(Point_kNNTest, GetNearestNeighbors_EntityDistanceFn)
  {
    using Box = BoundingBox2D;
    using Pt = Point2D;
    constexpr array<Box, 3> boxes = {
      Box{ { 0.0, 0.0 }, { 2.0, 2.0 } }, // id=0
      Box{ { 2.0, 2.0 }, { 4.0, 4.0 } }, // id=1
      Box{ { 5.0, 5.0 }, { 6.0, 6.0 } }  // id=2
    };
    const auto tree = QuadtreeBoxs<false>(boxes, 3, std::nullopt, 2);

    const Pt searchPt{ 2.5, 2.5 };

    auto realEntityDistance = [&](const Pt&, EntityID id) -> double {
      switch (id)
      {
      case 0: return 1.0;
      case 1: return 1.25;
      case 2: return 4.0;
      }

      return double(id * 10);
    };

    auto const result =
      tree.GetNearestNeighbors(searchPt, 1, std::numeric_limits<double>::max(), boxes, std::numeric_limits<double>::epsilon(), realEntityDistance);

    vector<EntityID> expected{ 0 };
    EXPECT_EQ(1, result.size());
    EXPECT_TRUE((std::ranges::is_permutation(result, expected)));
  }

  TEST(Point_kNNTest, GetNearestNeighbors_MinMaxDistance)
  {
    using Box = BoundingBox2D;
    using Pt = Point2D;

    constexpr auto boxes = std::array{
      Box{ { +0.20, 0.00 }, { +2.31, 2.31 } }, // od: 0.20, pd: 2.31 // winner in tie
      Box{ { +1.00, 0.00 }, { +1.01, 1.41 } }, // od: 1.00, pd: 1.41 // closed out by [2]
      Box{ { +0.40, 0.00 }, { +0.89, 0.89 } }, // od: 0.40, pd: 0.89 // possible winner by pessimistic distance
      Box{ { +2.10, 0.00 }, { +3.66, 3.66 } }, // od: 2.10, pd: 3.66 // obviously ruled out
      Box{ { -2.21, 0.00 }, { -0.20, 2.21 } }, // od: 0.20, pd: 2.21 // winner in tie
      Box{ { -2.50, 0.00 }, { -0.90, 2.47 } }, // od: 0.90, pd: 2.47 // ruled out by [2]
    };
    const auto tree = QuadtreeBoxs<false>(boxes, 3, std::nullopt, 2);

    const Pt searchPt{ 0.0, 0.0 };

    auto const result = tree.GetNearestNeighbors(searchPt, 1, std::numeric_limits<double>::max(), boxes, std::numeric_limits<double>::epsilon());

    vector<EntityID> expected{ 0, 2, 4 };
    EXPECT_EQ(expected.size(), result.size());
    EXPECT_TRUE((std::ranges::is_permutation(result, expected)));
  }

  TEST(Point_kNNTest, Issue9_2D)
  {
    auto const poses = vector<array<double, 2>>{
      { 78.2619,  77.843 },
      { 90.3005, 90.5172 },
      { 69.8652, 12.2467 },
      { 48.4675, 48.4948 },
      { 36.3226, 68.4619 },
      { 98.8799, 42.7149 },
      {  31.412, 38.6866 },
      { 63.2748, 77.0524 },
      { 62.8844, 17.0536 },
      { 80.8931, 39.8099 },
      {  77.426, 64.9844 },
      { 81.9552,  25.009 },
      { 87.6088,  51.319 },
      { 78.5609, 80.4623 },
      { 51.3967, 39.5269 },
      { 32.2042, 81.8779 },
      { 79.1739, 81.5467 },
      { 95.2619, 40.4742 },
      {  86.437, 92.4406 },
      { 3.95388, 60.2327 },
      { 31.1283, 44.4917 },
      { 35.6778, 79.8545 },
      { 50.9926, 66.1373 },
      { 3.16271, 65.2519 },
      { 56.3665, 45.3819 }
    };

    auto constexpr search_point = array<double, 2>{ 43.6406, 57.5691 };
    using GA = OrthoTree::GeneralGeometryAdapterTemplate<2, array<double, 2>, OrthoTree::BoundingBox2D, OrthoTree::Ray2D, OrthoTree::Plane2D>;
    auto const itMin = std::ranges::min_element(poses, [&search_point](auto const& lhs, auto const& rhs) {
      return GA::Distance2(lhs, search_point) < GA::Distance2(rhs, search_point);
    });

    std::array<double, 2> inspection_space_min = { 0.0, 0.0 };
    std::array<double, 2> inspection_space_max = { 100.0, 100.0 };
    OrthoTree::BoundingBox2D inspection_space;
    inspection_space.Min = inspection_space_min;
    inspection_space.Max = inspection_space_max;

    auto tree = QuadtreePointM(poses, 9, inspection_space);

    auto neighbors = tree.GetNearestNeighbors(search_point, 1);
    EXPECT_EQ(EntityID(std::distance(poses.begin(), itMin)), neighbors[0]);
  }

  TEST(Point_kNNTest, Issue9_6D)
  {
    using VectorType = array<double, 6>;

    auto const poses = vector<VectorType>{
      {  50.2232, 0.276687, 37.7662, 41.2776,  26.3818,  74.0284 },
      {  35.8946,  83.7503, 97.1127, 47.2895,  40.9232,  83.7666 },
      {  33.3541,  63.0669, 3.86075, 47.7923,  19.8039,  87.5608 },
      {  89.0684,  67.3278,  50.867, 49.8193,  72.6692,  54.0271 },
      {  75.7099,  53.1241,  39.624, 40.4669,  13.6433,  88.6247 },
      {  43.7641,  93.7985, 68.8286, 9.71882,  2.16644,  12.1925 },
      {  82.5195,  30.6964, 71.0556, 48.4744,  99.2295,  70.4137 },
      {  28.4568,  37.8366, 74.8597,  27.897,  60.6816, 0.247821 },
      {  58.3617,  17.0165, 15.1021, 70.9832,  22.5325,  34.8085 },
      {  33.1004,  72.6729, 35.7043, 35.2888,  94.9917,   17.652 },
      {  80.9693,  7.41406, 2.18394, 40.2606,  1.63274,  65.8996 },
      {  88.4851,  73.8366, 55.0264, 77.6467,  61.6751,  33.7444 },
      {  48.1777,  90.1285, 75.9069, 49.1867,  39.7186,  74.2862 },
      {  68.0826,  73.0622, 7.85933, 60.8879,  41.3229,  16.6928 },
      {  19.1693,  49.1172,  91.055, 52.5548,  16.2326, 0.610075 },
      {  19.3085,  81.9363, 55.8924, 1.77161,  29.4124,  72.3873 },
      {  57.8801,  28.3918, 45.7062, 92.1774, 0.742745,  29.0499 },
      {  74.2952,  84.6691, 46.3357, 76.7894,  68.8394,  24.1707 },
      { 0.199418,  93.2845, 94.9628, 38.2295,  82.1998,  59.2499 },
      {  68.4689, 0.261607, 75.5001,  58.427,  55.4243,  18.7392 },
      {  53.9164,  95.4966,  59.657, 71.0292,  82.4362,  53.9452 },
    };

    auto const search_point = VectorType{ 78.8658, 64.0361, 18.7755, 61.4618, 14.3312, 40.0196 };
    using GA = OrthoTree::GeneralGeometryAdapterTemplate<6, VectorType, OrthoTree::BoundingBoxND<6>, OrthoTree::RayND<6>, OrthoTree::PlaneND<6>>;
    auto const itMinExpected = std::ranges::min_element(poses, [&search_point](auto const& lhs, auto const& rhs) {
      return GA::Distance2(lhs, search_point) < GA::Distance2(rhs, search_point);
    });

    auto const inspection_space = OrthoTree::BoundingBoxND<6>{
      {   0.0,   0.0,   0.0,   0.0,   0.0,   0.0 },
      { 100.0, 100.0, 100.0, 100.0, 100.0, 100.0 }
    };

    auto tree = OrthoTreePointND<6>();
    tree.Create(tree, poses, 9, inspection_space);

    auto const neighbors = tree.GetNearestNeighbors(search_point, 1, poses);
    auto const idMinExpected = EntityID(std::distance(poses.begin(), itMinExpected));
    EXPECT_EQ(idMinExpected, neighbors[0]);
  }

  TEST(Point_kNNTest, BoxNN_k1__InsideSingleBox)
  {
    auto constexpr boxes = array{
      BoundingBox2D{ { 0.0, 0.0 }, { 1.0, 1.0 } }, // 0
      BoundingBox2D{ { 1.0, 1.0 }, { 3.0, 3.0 } }, // 1
      BoundingBox2D{ { 4.0, 4.0 }, { 5.0, 5.0 } }  // 2
    };

    auto const tree = QuadtreeBoxs<false>(boxes, 3, std::nullopt, 2);

    auto constexpr pt = Point2D{ 2.0, 2.0 }; // inside box 1
    auto const vnn = tree.GetNearestNeighbors(pt, 1, boxes);

    EXPECT_TRUE((std::ranges::is_permutation(vector<EntityID>{ 1 }, vnn)));
  }

  TEST(Point_kNNTest, BoxNN_k1__OutsideSingleBox)
  {
    auto constexpr boxes = array{
      BoundingBox2D{ { 0.0, 0.0 }, { 1.0, 1.0 } }, // 0
      BoundingBox2D{ { 1.0, 1.0 }, { 3.0, 3.0 } }, // 1
      BoundingBox2D{ { 4.0, 4.0 }, { 5.0, 5.0 } }  // 2
    };

    auto const tree = QuadtreeBoxs<false>(boxes, 3, std::nullopt, 2);

    auto constexpr pt = Point2D{ -1.0, 1.0 };
    auto const vnn = tree.GetNearestNeighbors(pt, 1, boxes);

    EXPECT_TRUE((std::ranges::is_permutation(vector<EntityID>{ 0 }, vnn)));
  }

  static std::vector<EntityID> kNNSearchBruteForce(std::vector<Point2D> const& points, Point2D const& point, size_t k)
  {
    auto dist_fn = [&](auto const& p) {
      double dx = point[0] - p[0];
      double dy = point[1] - p[1];
      return std::sqrt(dx * dx + dy * dy);
    };

    std::vector<EntityID> ids(points.size());
    std::iota(ids.begin(), ids.end(), 0);
    std::partial_sort(ids.begin(), ids.begin() + std::min(k + 10, points.size()), ids.end(), [&](auto const& i1, auto const& i2) {
      return dist_fn(points[i1]) < dist_fn(points[i2]);
    });

    auto result = std::vector<EntityID>(ids.begin(), ids.begin() + k);
    auto distLast = dist_fn(points[ids[k - 1]]);
    double tolerance = std::numeric_limits<double>::epsilon() * 100.0;
    auto limit = std::max(tolerance, distLast * (1.0 + tolerance));
    for (size_t i = k; i < ids.size(); ++i)
    {
      auto dist = dist_fn(points[ids[i]]);
      if (dist < limit)
        result.push_back(ids[i]);
      else
        return result;
    }

    return result;
  }

  TEST(Point_kNNTest, Issue36_kNN_Random)
  {
    constexpr dim_t N = 2;

    using GA = GeneralGeometryAdapterTemplate<N, VectorND<N>, BoundingBoxND<N>, RayND<N>, PlaneND<N>>;

    auto pointsNo = 1000;
    auto points = std::vector<Point2D>(pointsNo);
    auto rng = std::mt19937(0);

    for (std::size_t c = 0; c < 100; ++c)
    {
      for (int i = 0; i < pointsNo; ++i)
        for (dim_t d = 0; d < N; ++d)
          points[i][d] = double(rng() % 100000) / 1000.0;

      auto searchPoints = std::vector<Point2D>(pointsNo);
      for (int i = 0; i < pointsNo; ++i)
        for (dim_t d = 0; d < N; ++d)
          searchPoints[i][d] = double(rng() % 100000) / 1000.0;

      auto const tree = OrthoTreePointND<N>(points, 10, std::nullopt, 5, false);
      auto const k = 4;
      for (auto const& searchPoint : searchPoints)
      {
        auto expected = kNNSearchBruteForce(points, searchPoint, k);
        auto actual = tree.GetNearestNeighbors(searchPoint, k, points);
        EXPECT_TRUE((expected.size() == actual.size()));

        auto const areResultsEqual = expected == actual;

        if (areResultsEqual)
          continue;

        for (std::size_t i = 0; i < expected.size(); ++i)
        {
          if (expected[i] == actual[i])
            continue;

          auto const expectedDistance = GA::Distance2(searchPoint, points[expected[i]]);
          auto const actualDistance = GA::Distance2(searchPoint, points[actual[i]]);
          EXPECT_TRUE((std::abs(actualDistance - expectedDistance) < std::numeric_limits<double>::epsilon() * 10.0));
        }
      }
    }
  }
} // namespace Point_kNNTest
