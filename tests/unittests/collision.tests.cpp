#include "pch.h"

#include <charconv>
#include <filesystem>
#include <fstream>
#include <random>
#include <string>
#include <string_view>

using namespace OrthoTree;
using namespace OrthoTreeTestHelpers;
using std::array;
using std::vector;

namespace Box_SearchTest
{
  TEST(Box_SearchTest, CollistionDetection__0040_3565__P30)
  {
    auto constexpr vBoxL = array{
      BoundingBox1D{ 0.0, 1.0 },
      BoundingBox1D{ 1.0, 2.0 },
      BoundingBox1D{ 2.0, 3.0 },
      BoundingBox1D{ 3.0, 4.0 }
    };
    auto const treeL = DualtreeBox(vBoxL, 3, std::nullopt, 1);

    auto constexpr vBoxR = array{
      BoundingBox1D{ 3.5, 4.5 },
      BoundingBox1D{ 4.5, 5.5 },
      BoundingBox1D{ 5.5, 6.5 }
    };
    auto const treeR = DualtreeBox(vBoxR, 3, std::nullopt, 1);

    auto const ret = DualtreeBox::CollisionDetection(treeL, vBoxL, treeR, vBoxR);

    auto constexpr aExpected = std::pair{ 3, 0 };
    EXPECT_EQ(ret.size(), 1);
    EXPECT_EQ(ret[0].first, aExpected.first);
    EXPECT_EQ(ret[0].second, aExpected.second);
  }

  TEST(Box_SearchTest, CollistionDetection__Complex1)
  {
    auto constexpr vBoxL = array{
      BoundingBox1D{ 0.0, 4.0 },
      BoundingBox1D{ 0.0, 2.0 },
      BoundingBox1D{ 2.0, 4.0 },
      BoundingBox1D{ 0.0, 1.0 },
      BoundingBox1D{ 1.0, 2.0 },
      BoundingBox1D{ 2.0, 3.0 },
      BoundingBox1D{ 3.0, 4.0 }
    };
    auto const treeL = DualtreeBox(vBoxL, 3, std::nullopt, 1);

    auto constexpr vBoxR = array{
      BoundingBox1D{ 2.0, 6.0 },
      BoundingBox1D{ 2.0, 4.0 },
      BoundingBox1D{ 4.0, 6.0 },
      BoundingBox1D{ 2.0, 3.0 },
      BoundingBox1D{ 3.0, 4.0 },
      BoundingBox1D{ 4.0, 5.0 },
      BoundingBox1D{ 5.0, 6.0 }
    };
    auto const treeR = DualtreeBox(vBoxR, 3, std::nullopt, 1);

    auto const vActual = DualtreeBox::CollisionDetection(treeL, vBoxL, treeR, vBoxR);

    using EntityPair = std::pair<EntityID, EntityID>;
    auto constexpr aExpected = std::array{
      EntityPair{ 0, 0 }, // Level 0
      EntityPair{ 0, 1 },
      EntityPair{ 2, 0 }, // Level 1
      EntityPair{ 2, 1 },
      EntityPair{ 0, 3 }, // Level 2
      EntityPair{ 2, 3 },
      EntityPair{ 5, 0 },
      EntityPair{ 5, 1 },
      EntityPair{ 5, 3 }, // ==
      EntityPair{ 0, 4 },
      EntityPair{ 2, 4 },
      EntityPair{ 6, 0 },
      EntityPair{ 6, 1 },
      EntityPair{ 6, 4 }, // ==
    };
    EXPECT_EQ(aExpected.size(), vActual.size());
    EXPECT_TRUE((std::ranges::is_permutation(aExpected, vActual, [](auto const& p1, auto const& p2) { return p1.first == p2.first && p1.second == p2.second; })));
  }

  TEST(Box_SearchTest, CollistionDetection__33_24_34)
  {
    auto constexpr boxesL = array{
      BoundingBox2D{ { 0.0, 0.0 }, { 1.0, 1.0 } },
      BoundingBox2D{ { 1.0, 1.0 }, { 2.0, 2.0 } },
      BoundingBox2D{ { 2.0, 2.0 }, { 3.0, 3.0 } },
      BoundingBox2D{ { 3.0, 3.0 }, { 4.0, 4.0 } },
    };

    auto constexpr boxesR = array{
      BoundingBox2D{ { 2.0, 0.0 }, { 3.0, 1.0 } },
      BoundingBox2D{ { 3.0, 1.0 }, { 4.0, 2.0 } },
      BoundingBox2D{ { 3.0, 2.0 }, { 4.0, 3.0 } },
      BoundingBox2D{ { 2.0, 3.0 }, { 4.0, 4.0 } },
      BoundingBox2D{ { 2.5, 2.5 }, { 3.5, 3.5 } },
    };

    auto const qtL = QuadtreeBox(boxesL, 3, std::nullopt, 2);
    auto const qtR = QuadtreeBox(boxesR, 3, std::nullopt, 2);

    auto const aActualPair = QuadtreeBox::CollisionDetection(qtL, boxesL, qtR, boxesR); // { {3, 3}, {2, 4}, {3, 4} }

    auto constexpr aExpectedPair = array{
      std::pair<EntityID, EntityID>{ 3, 3 }, // Level 0
      std::pair<EntityID, EntityID>{ 2, 4 },
      std::pair<EntityID, EntityID>{ 3, 4 }, // Level 1
    };
    EXPECT_EQ(aExpectedPair.size(), aActualPair.size());
    EXPECT_TRUE((std::ranges::is_permutation(aExpectedPair, aActualPair, [](auto const& p1, auto const& p2) {
      return p1.first == p2.first && p1.second == p2.second;
    })));
  }
} // namespace Box_SearchTest

namespace LongIntTest
{
  template<dim_t nDim, typename TScalar>
  static vector<BoundingBoxND<nDim, TScalar>> readBoxCloud(std::filesystem::path const& path)
  {
    auto boxes = vector<BoundingBoxND<nDim, TScalar>>{};
    auto file = std::ifstream(path, std::ios::in);
    if (file.fail())
      return boxes;

    auto line = std::string{};
    while (std::getline(file, line))
    {
      if (file.fail())
        return boxes;

      auto& box = boxes.emplace_back();
      auto sw = std::string_view(line).substr(22);
      for (int iDim = 0; iDim < nDim; ++iDim)
      {
        auto const [ptr, ec] = std::from_chars(sw.data(), sw.data() + sw.length(), box.Min[iDim]);
        if (ec != std::errc{})
          return boxes;

        sw.remove_prefix(ptr - sw.data());
        if (!sw.empty())
          sw.remove_prefix(2);
      }

      sw.remove_prefix(9);
      for (int iDim = 0; iDim < nDim; ++iDim)
      {
        auto const [ptr, ec] = std::from_chars(sw.data(), sw.data() + sw.length(), box.Max[iDim]);
        if (ec != std::errc{})
          return boxes;

        sw.remove_prefix(ptr - sw.data());
        if (!sw.empty())
          sw.remove_prefix(2);
      }
    }

    return boxes;
  }

  template<int nDim, typename TScalar>
  static vector<PointND<nDim, TScalar>> readPointCloud(std::filesystem::path const& path)
  {
    auto points = vector<PointND<nDim, TScalar>>{};
    auto file = std::ifstream(path, std::ios::in);
    if (file.fail())
      return points;

    auto line = std::string{};
    while (std::getline(file, line))
    {
      if (file.fail())
        return points;

      auto& point = points.emplace_back();
      auto sw = std::string_view(line).substr(18);
      for (int iDim = 0; iDim < nDim; ++iDim)
      {
        auto const [ptr, ec] = std::from_chars(sw.data(), sw.data() + sw.length(), point[iDim]);
        if (ec != std::errc{})
          return points;

        sw.remove_prefix(ptr - sw.data());
        if (iDim < nDim - 1)
          sw.remove_prefix(3);
      }
    }

    return points;
  }

  template<int nDim, typename TScalar>
  [[maybe_unused]] static std::vector<PointND<nDim, TScalar>> readPointCloud__DXF_CSV(std::filesystem::path const& path)
  {
    auto points = std::vector<PointND<nDim, TScalar>>{};
    auto file = std::ifstream(path, std::ios::in);
    if (file.fail())
      return points;

    auto line = std::string{};
    while (std::getline(file, line))
    {
      if (file.fail())
        return points;

      if (line.find("AcDbEntity:AcDbPoint") == std::string::npos)
        continue;

      auto& point = points.emplace_back();
      auto sw = std::string_view(line);
      for (int iDim = 0; iDim < nDim; ++iDim)
      {
        auto const [ptr, ec] = std::from_chars(sw.data(), sw.data() + sw.length(), point[iDim]);
        if (ec != std::errc{})
          return points;

        sw.remove_prefix(ptr - sw.data());
        if (iDim < nDim - 1)
          sw.remove_prefix(1);
      }
    }

    return points;
  }

  TEST(LongIntTest, Issue26)
  {
    const auto boxes = readBoxCloud<3, float>("../../../bbox_output.txt");
    if (boxes.empty())
      return;

    using Tree = OrthoTreeBoxManagedND<3, true, float>;
    auto const tree = Tree(boxes, 4, std::nullopt, 21, false);

    const auto boxOfTree = tree.GetCore().GetBox();
    const float rayIntersectTolerance = 0.1f;

    auto points = readPointCloud<3, float>("../../../pos_input.txt");
    auto searchSizeYs = vector(points.size(), 100.0f);
    if (points.empty())
    {
      srand(0);
      for (int iTry = 0; iTry < 100; ++iTry)
      {
        constexpr int mod = 1000;
        constexpr auto modr = 0.0011f;
        const float searchSizeY = float(rand() % mod) * modr * (boxOfTree.Max[1] - boxOfTree.Min[1]);
        searchSizeYs.emplace_back(searchSizeY);

        const auto x = float(rand() % mod) * modr * (boxOfTree.Max[0] - boxOfTree.Min[0]);
        const auto y = float(rand() % mod) * modr * (boxOfTree.Max[1] - boxOfTree.Min[1]);
        const auto z = float(rand() % mod) * modr * (boxOfTree.Max[2] - boxOfTree.Min[2]);
        points.emplace_back(Tree::TVector{ x, y, z });
      }
    }

    for (std::size_t pointID = 0; pointID < points.size(); ++pointID)
    {
      const auto searchBox = Tree::TBox{
        { points[pointID][0] - rayIntersectTolerance, points[pointID][1] - searchSizeYs[pointID], points[pointID][2] - rayIntersectTolerance },
        { points[pointID][0] + rayIntersectTolerance, points[pointID][1] + rayIntersectTolerance, points[pointID][2] + rayIntersectTolerance }
      };

      auto resultOfBruteForce = vector<EntityID>{};
      auto const boxNo = boxes.size();
      for (EntityID id = 0; id < boxNo; ++id)
      {
        if (Tree::GA::AreBoxesOverlapped(searchBox, boxes[id], false, false))
          resultOfBruteForce.emplace_back(id);
      }

      auto resultOfSearchBox = tree.RangeSearch(searchBox, RangeSearchMode::Overlap);
      std::ranges::sort(resultOfSearchBox);

      std::vector<OctreeBox::NodeID> nodeIDs;
      for (auto entityID : resultOfBruteForce)
        nodeIDs.emplace_back(tree.GetCore().Find(entityID));

      auto resultOfRay = tree.RayIntersectedAll(points[pointID], { 0.0, -1.0, 0.0 }, rayIntersectTolerance, searchSizeYs[pointID]);
      std::ranges::sort(resultOfRay);
      std::vector<OctreeBox::NodeID> nodeIDRs;
      for (auto entityID : resultOfRay)
        nodeIDRs.emplace_back(tree.GetCore().Find(entityID));

      EXPECT_TRUE((resultOfBruteForce == resultOfSearchBox));
      EXPECT_TRUE((resultOfBruteForce == resultOfRay));
    }
  }

  template<dim_t N>
  static vector<std::pair<EntityID, EntityID>> BruteForceCollisionDetection(vector<BoundingBoxND<N>> const& boxes)
  {
    auto collidedEntities = vector<std::pair<EntityID, EntityID>>{};
    auto const nid = EntityID(boxes.size());
    for (EntityID i = 0; i < nid; ++i)
      for (EntityID j = i + 1; j < nid; ++j)
        if (GeneralGeometryAdapterTemplate<N, PointND<N>, BoundingBoxND<N>, RayND<N>, PlaneND<N>>::AreBoxesOverlapped(boxes[i], boxes[j], false))
          collidedEntities.emplace_back(i, j);

    return collidedEntities;
  }

  static bool IsAllElementInsideNodes(auto const& tree, auto const& boxes)
  {
    using GA = typename std::remove_cvref_t<decltype(tree)>::GA;
    bool isOK = true;
    tree.TraverseNodesDepthFirst([&](auto const nodeValue) {
      auto const& entityIDs = tree.GetNodeEntities(nodeValue);
      auto const& nodeBox = tree.GetNodeBox(nodeValue);
      for (auto const entityID : entityIDs)
      {
        if (!GA::AreBoxesOverlapped(nodeBox, boxes[entityID], true))
        {
          isOK = false;
          return TraverseControl::Terminate;
        }
      }
      return TraverseControl::Continue;
    });

    return isOK;
  }

  TEST(LongIntTest, Issue38_ParallelCollisionDetection_Random)
  {
    constexpr dim_t N = 3;
    constexpr bool IS_LOOSE_TREE = true;
    constexpr auto EXEC_MODE = SEQ_EXEC;
    auto execTag = PAR_EXEC;

    constexpr std::size_t boxNo = 1000;

    auto const spaceMax = 8.0;
    auto const sizeMax = 2.0;
    auto rng = std::mt19937(0);
    for (std::size_t c = 0; c < 10; ++c)
    {
      auto boxes = vector<BoundingBoxND<N>>(boxNo);
      boxes[0].Min.fill(0.0);
      boxes[0].Max.fill(sizeMax);
      boxes[1].Min.fill(spaceMax - sizeMax);
      boxes[1].Max.fill(spaceMax);

      for (std::size_t i = 2; i < boxNo; ++i)
      {
        for (dim_t d = 0; d < N; ++d)
        {
          boxes[i].Min[d] = std::min(double(rng() % 10000) / 10000.0 * spaceMax, spaceMax - 0.1);
          boxes[i].Max[d] = std::min(boxes[i].Min[d] + double(rng() % 10000) / 10000.0 * sizeMax, spaceMax);
        }
      }

      auto const tree = OrthoTreeBoxND<N, IS_LOOSE_TREE>(EXEC_MODE, boxes, 10, std::nullopt, 5);
      IsAllElementInsideNodes(tree, boxes);
      auto const expectedResult = BruteForceCollisionDetection(boxes);
      auto actualResult = tree.template CollisionDetection<decltype(execTag)>(boxes);
      for (auto& [a, b] : actualResult)
        if (a > b)
          std::swap(a, b);

      std::ranges::sort(actualResult);
      EXPECT_TRUE((actualResult == expectedResult));
    }
  }
} // namespace LongIntTest
