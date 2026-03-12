#include "pch.h"

#define ORTHOTREE__PUBLIC_NODE_INTERFACE
#include "../include/orthotree/bvh.h"

#include <random>
#include <set>
#include <vector>

using namespace OrthoTree;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BVHTests
{
  TEST_CLASS(BVHStaticLinearTests)
  {
    static constexpr std::size_t MAX_ELEMENT_NUM = 2;

    template<typename TCore>
    static std::vector<index_t> CollectAllEntities(TCore const& bvh)
    {
      auto result = std::vector<index_t>{};
      bvh.TraverseNodesDepthFirst([&](auto const nodeValue) {
        for (index_t eid : bvh.GetNodeEntities(nodeValue))
          result.push_back(eid);
        return TraverseControl::Continue;
      });
      return result;
    }

  public:
    TEST_METHOD(Test_Create_Empty)
    {
      StaticBVHBox3D bvh;
      auto ok = bvh.Create(std::vector<BoundingBox3D>{}, MAX_ELEMENT_NUM);
      Assert::IsTrue(ok);
      Assert::IsTrue(bvh.GetNodeCount() == 0);
    }

    TEST_METHOD(Test_Create_SingleEntity)
    {
      auto boxes = std::vector<BoundingBox3D>{
        { { 1.0, 2.0, 3.0 }, { 4.0, 5.0, 6.0 } }
      };
      StaticBVHBox3D bvh;
      bvh.Create(boxes, MAX_ELEMENT_NUM);
      Assert::IsTrue(bvh.GetNodeCount() == 1);
      Assert::IsTrue(bvh.GetNodeEntityCount(bvh.GetRootNodeValue()) == 1);
    }

    TEST_METHOD(Test_Create_AllEntitiesPresent)
    {
      auto boxes = std::vector<BoundingBox3D>{
        { { 0.0, 0.0, 0.0 }, { 1.0, 1.0, 1.0 } },
        { { 2.0, 2.0, 2.0 }, { 3.0, 3.0, 3.0 } },
        { { 4.0, 4.0, 4.0 }, { 5.0, 5.0, 5.0 } },
        { { 6.0, 6.0, 6.0 }, { 7.0, 7.0, 7.0 } },
        { { 1.0, 1.0, 1.0 }, { 2.0, 2.0, 2.0 } },
        { { 3.0, 3.0, 3.0 }, { 4.0, 4.0, 4.0 } },
        { { 5.0, 5.0, 5.0 }, { 6.0, 6.0, 6.0 } },
      };
      auto const N = boxes.size();
      StaticBVHBox3D bvh;
      bvh.Create(boxes, MAX_ELEMENT_NUM);
      Assert::IsTrue(bvh.GetNodeCount() > 0);

      auto ids = CollectAllEntities(bvh);
      Assert::IsTrue(ids.size() == N);

      auto idSet = std::set<index_t>(ids.begin(), ids.end());
      for (index_t i = 0; i < N; ++i)
        Assert::IsTrue(idSet.count(i) > 0);
    }

    TEST_METHOD(Test_Create_NodeCountReasonable)
    {
      constexpr int N = 100;
      auto boxes = std::vector<BoundingBox3D>(N);
      std::mt19937 rng(42);
      std::uniform_real_distribution<double> dist(0.0, 100.0);
      std::uniform_real_distribution<double> sizeDist(0.1, 2.0);
      for (int i = 0; i < N; ++i)
      {
        double x = dist(rng), y = dist(rng), z = dist(rng), s = sizeDist(rng);
        boxes[i] = {
          {     x,     y,     z },
          { x + s, y + s, z + s }
        };
      }

      StaticBVHBox3D bvh;
      bvh.Create(std::span<BoundingBox3D const>(boxes), MAX_ELEMENT_NUM);
      Assert::IsTrue(bvh.GetNodeCount() >= 1);
      Assert::IsTrue(bvh.GetNodeCount() <= std::size_t(N) * 4);
    }

    TEST_METHOD(Test_Create_Points2D)
    {
      auto points = std::vector<Point2D>{
        {  0.0,  0.0 },
        {  1.0,  1.0 },
        {  2.0,  2.0 },
        {  3.0,  3.0 },
        { 10.0, 10.0 },
        { 11.0, 11.0 },
        { 12.0, 12.0 },
      };
      StaticBVHPoint2D bvh;
      bvh.Create(std::span<Point2D const>(points), MAX_ELEMENT_NUM);
      Assert::IsTrue(bvh.GetNodeCount() >= 1);

      auto ids = CollectAllEntities(bvh);
      Assert::IsTrue(ids.size() == points.size());
    }

    TEST_METHOD(Test_Create_1D)
    {
      auto boxes = std::vector<BoundingBox1D>{
        {  { 0.0 },  { 1.0 } },
        {  { 2.0 },  { 3.0 } },
        {  { 4.0 },  { 5.0 } },
        {  { 6.0 },  { 7.0 } },
        {  { 8.0 },  { 9.0 } },
        { { 10.0 }, { 11.0 } },
        { { 12.0 }, { 13.0 } },
        { { 14.0 }, { 15.0 } },
      };
      StaticBVHBox1D bvh;
      bvh.Create(boxes, 2);
      Assert::IsTrue(bvh.GetNodeCount() >= 1);

      auto ids = CollectAllEntities(bvh);
      Assert::IsTrue(ids.size() == boxes.size());
    }

    TEST_METHOD(Test_Create_4D)
    {
      constexpr int N = 50;
      auto boxes = std::vector<BoundingBoxND<4>>(N);
      std::mt19937 rng(123);
      std::uniform_real_distribution<double> dist(0.0, 50.0);
      std::uniform_real_distribution<double> sizeDist(0.1, 1.0);
      for (int i = 0; i < N; ++i)
        for (int d = 0; d < 4; ++d)
        {
          double v = dist(rng), s = sizeDist(rng);
          boxes[i].Min[d] = v;
          boxes[i].Max[d] = v + s;
        }

      StaticBVHBox4D bvh;
      bvh.Create(std::span<BoundingBoxND<4> const>(boxes), MAX_ELEMENT_NUM);
      Assert::IsTrue(bvh.GetNodeCount() >= 1);

      auto ids = CollectAllEntities(bvh);
      Assert::IsTrue(ids.size() == std::size_t(N));
    }

    TEST_METHOD(Test_Create_CustomChildNum)
    {
      auto boxes = std::vector<BoundingBox3D>();
      for (int i = 0; i < 64; ++i)
      {
        double x = static_cast<double>(i % 4) * 10.0;
        double y = static_cast<double>((i / 4) % 4) * 10.0;
        double z = static_cast<double>(i / 16) * 10.0;
        boxes.push_back({
          {       x,       y,       z },
          { x + 1.0, y + 1.0, z + 1.0 }
        });
      }

      StaticBVHNBoxND<3, 4> bvh4;
      bvh4.Create(boxes, MAX_ELEMENT_NUM);
      StaticBVHNBoxND<3, 16> bvh16;
      bvh16.Create(boxes, MAX_ELEMENT_NUM);
      Assert::IsTrue(bvh4.GetNodeCount() >= 1);
      Assert::IsTrue(bvh16.GetNodeCount() >= 1);
    }

    TEST_METHOD(Test_Create_DuplicateEntities)
    {
      auto boxes = std::vector<BoundingBox3D>{
        { { 1.0, 1.0, 1.0 }, { 2.0, 2.0, 2.0 } },
        { { 1.0, 1.0, 1.0 }, { 2.0, 2.0, 2.0 } },
        { { 1.0, 1.0, 1.0 }, { 2.0, 2.0, 2.0 } },
        { { 1.0, 1.0, 1.0 }, { 2.0, 2.0, 2.0 } },
      };
      StaticBVHBox3D bvh;
      bvh.Create(std::span<BoundingBox3D const>(boxes), MAX_ELEMENT_NUM);
      Assert::IsTrue(bvh.GetNodeCount() >= 1);

      auto ids = CollectAllEntities(bvh);
      Assert::IsTrue(ids.size() == 4);
    }

    TEST_METHOD(Test_Create_LargeDataset)
    {
      constexpr int N = 10000;
      auto boxes = std::vector<BoundingBox3D>(N);
      std::mt19937 rng(999);
      std::uniform_real_distribution<double> dist(0.0, 1000.0);
      std::uniform_real_distribution<double> sizeDist(0.01, 5.0);
      for (int i = 0; i < N; ++i)
      {
        double x = dist(rng), y = dist(rng), z = dist(rng), s = sizeDist(rng);
        boxes[i] = {
          {     x,     y,     z },
          { x + s, y + s, z + s }
        };
      }

      StaticBVHBox3D bvh;
      bvh.Create(std::span<BoundingBox3D const>(boxes), MAX_ELEMENT_NUM);
      Assert::IsTrue(bvh.GetNodeCount() >= 1);

      auto ids = CollectAllEntities(bvh);
      Assert::IsTrue(ids.size() == std::size_t(N));
    }

    TEST_METHOD(Test_Create_FewEntities)
    {
      auto boxes = std::vector<BoundingBox3D>{
        { { 0.0, 0.0, 0.0 }, { 1.0, 1.0, 1.0 } },
        { { 2.0, 2.0, 2.0 }, { 3.0, 3.0, 3.0 } },
        { { 4.0, 4.0, 4.0 }, { 5.0, 5.0, 5.0 } },
      };
      StaticBVHBox3D bvh;
      bvh.Create(std::span<BoundingBox3D const>(boxes), MAX_ELEMENT_NUM);
      Assert::IsTrue(bvh.GetNodeCount() == 1);
      Assert::IsTrue(bvh.GetNodeEntityCount(bvh.GetRootNodeValue()) == 3);
    }

    TEST_METHOD(Test_Create_SmallMaxElements)
    {
      auto boxes = std::vector<BoundingBox3D>{
        { { 0.0, 0.0, 0.0 }, { 1.0, 1.0, 1.0 } },
        { { 2.0, 2.0, 2.0 }, { 3.0, 3.0, 3.0 } },
        { { 4.0, 4.0, 4.0 }, { 5.0, 5.0, 5.0 } },
        { { 6.0, 6.0, 6.0 }, { 7.0, 7.0, 7.0 } },
        { { 8.0, 8.0, 8.0 }, { 9.0, 9.0, 9.0 } },
      };
      StaticBVHBox3D bvh;
      bvh.Create(std::span<BoundingBox3D const>(boxes), 1);
      Assert::IsTrue(bvh.GetNodeCount() > 1);

      auto ids = CollectAllEntities(bvh);
      Assert::IsTrue(ids.size() == 5);
    }

    TEST_METHOD(Test_Create_2DBoxes)
    {
      auto boxes = std::vector<BoundingBox2D>{
        { { 0.0, 0.0 }, { 1.0, 1.0 } },
        { { 1.0, 1.0 }, { 2.0, 2.0 } },
        { { 2.0, 2.0 }, { 3.0, 3.0 } },
        { { 3.0, 3.0 }, { 4.0, 4.0 } },
        { { 1.2, 1.2 }, { 2.8, 2.8 } },
      };
      StaticBVHBox2D bvh;
      bvh.Create(boxes, MAX_ELEMENT_NUM);
      Assert::IsTrue(bvh.GetNodeCount() >= 1);
    }

    TEST_METHOD(Test_Create_EntitiesOnLine)
    {
      auto boxes = std::vector<BoundingBox3D>();
      for (int i = 0; i < 100; ++i)
      {
        double x = static_cast<double>(i);
        boxes.push_back({
          {       x, 0.0, 0.0 },
          { x + 0.5, 0.5, 0.5 }
        });
      }
      StaticBVHBox3D bvh;
      bvh.Create(boxes, 4);
      Assert::IsTrue(bvh.GetNodeCount() >= 1);

      auto ids = CollectAllEntities(bvh);
      Assert::IsTrue(ids.size() == 100);
    }

    TEST_METHOD(Test_Create_Clustered)
    {
      auto boxes = std::vector<BoundingBox3D>();
      for (int i = 0; i < 20; ++i)
      {
        double v = static_cast<double>(i) * 0.1;
        boxes.push_back({
          {        v,        v,        v },
          { v + 0.05, v + 0.05, v + 0.05 }
        });
      }
      for (int i = 0; i < 20; ++i)
      {
        double v = 100.0 + static_cast<double>(i) * 0.1;
        boxes.push_back({
          {        v,        v,        v },
          { v + 0.05, v + 0.05, v + 0.05 }
        });
      }

      StaticBVHBox3D bvh;
      bvh.Create(boxes, 4);
      Assert::IsTrue(bvh.GetNodeCount() >= 1);

      auto ids = CollectAllEntities(bvh);
      Assert::IsTrue(ids.size() == 40);
    }

    TEST_METHOD(Test_ChildrenInRange)
    {
      constexpr int N = 200;
      auto boxes = std::vector<BoundingBox3D>(N);
      std::mt19937 rng(7777);
      std::uniform_real_distribution<double> dist(0.0, 100.0);
      std::uniform_real_distribution<double> sizeDist(0.1, 2.0);
      for (int i = 0; i < N; ++i)
      {
        double x = dist(rng), y = dist(rng), z = dist(rng), s = sizeDist(rng);
        boxes[i] = {
          {     x,     y,     z },
          { x + s, y + s, z + s }
        };
      }

      StaticBVHBox3D bvh;
      bvh.Create(std::span<BoundingBox3D const>(boxes), 4);
      auto const nodeCount = bvh.GetNodeCount();

      bvh.TraverseNodesDepthFirst([&](auto const nodeValue) {
        for (auto const childID : bvh.GetNodeChildren(nodeValue))
          Assert::IsTrue(static_cast<std::size_t>(childID) < nodeCount);
        return TraverseControl::Continue;
      });
    }

    TEST_METHOD(Test_TwoFarApart)
    {
      auto boxes = std::vector<BoundingBox3D>{
        {       { 0.0, 0.0, 0.0 },       { 1.0, 1.0, 1.0 } },
        { { 100.0, 100.0, 100.0 }, { 101.0, 101.0, 101.0 } },
      };
      StaticBVHBox3D bvh;
      bvh.Create(boxes, MAX_ELEMENT_NUM);
      Assert::IsTrue(bvh.GetNodeCount() >= 1);

      auto ids = CollectAllEntities(bvh);
      Assert::IsTrue(ids.size() == 2);
    }

  };
}
