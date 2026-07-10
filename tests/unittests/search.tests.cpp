#include "pch.h"

using namespace OrthoTree;
using namespace OrthoTreeTestHelpers;
using std::array;
using std::vector;

namespace Point_CreateTest
{
  TEST(Point_CreateTest, Empty)
  {
    auto const tree = DualtreePoint({}, 2);
    EXPECT_TRUE((tree.GetNodeCount() == 1));
    EXPECT_TRUE((tree.IsNodeEntitiesEmpty(tree.GetNodeValue(1))));

    auto const& box = tree.GetBox();
    EXPECT_TRUE((AreEqualAlmost(BoundingBox1D{ .Min = box.Min, .Max = box.Max }, {})));
  }

  TEST(Point_CreateTest, NoPt1)
  {
    auto const tree = DualtreePoint(vector<Point1D>{ { 1.0 } }, 2);
    EXPECT_TRUE((tree.GetNodeCount() == 1));
    EXPECT_FALSE((tree.IsNodeEntitiesEmpty(tree.GetNodeValue(1))));

    auto const& box = tree.GetBox();
    EXPECT_TRUE((AreEqualAlmost(BoundingBox1D{ .Min = box.Min, .Max = box.Max }, BoundingBox1D{ Point1D{ 1.0 }, Point1D{ 1.0 } })));
  }

  TEST(Point_CreateTest, NoPt2)
  {
    auto const tree = DualtreePoint({}, 2);
    EXPECT_TRUE((tree.GetNodeCount() == 1));
    EXPECT_TRUE((tree.IsNodeEntitiesEmpty(tree.GetNodeValue(1))));
  }

  TEST(Point_CreateTest, NoPt4)
  {
    auto constexpr vpt = array{ Point1D{ 0.0 }, Point1D{ 1.0 }, Point1D{ 2.0 }, Point1D{ 3.0 } };
    auto const tree = DualtreePoint(vpt, 2, std::nullopt, 1);

    EXPECT_EQ(tree.GetNodeCount(), 7);

    auto const& box = tree.GetBox();
    EXPECT_TRUE((AreEqualAlmost(BoundingBox1D{ .Min = box.Min, .Max = box.Max }, BoundingBox1D{ Point1D{ 0.0 }, Point1D{ 3.0 } })));
  }

  TEST(Point_CreateTest, Init)
  {
    auto tree = DualtreePoint{};
    tree.Init(BoundingBox1D{ { -1.0 }, { +1.0 } }, 2);

    EXPECT_TRUE((tree.GetNodeCount() == 1));
    EXPECT_TRUE((tree.IsNodeEntitiesEmpty(tree.GetRootNodeValue())));
  }
} // namespace Point_CreateTest

namespace Point_SearchTest
{
  TEST(Point_SearchTest, RangeSearch__EqualAtBorder__12)
  {
    auto constexpr vpt = array{ Point1D{ 0.0 }, Point1D{ 1.0 }, Point1D{ 2.0 }, Point1D{ 3.0 } };
    auto const tree = DualtreePoint(vpt, 2, std::nullopt, 1);
    auto const ids = tree.RangeSearch({ 1.0, 2.0 }, vpt);

    EXPECT_TRUE((std::ranges::is_permutation(vector<EntityID>{ 1, 2 }, ids)));
  }

  TEST(Point_SearchTest, RangeSearch__Usual__23)
  {
    auto constexpr vpt = array{ Point1D{ 0.0 }, Point1D{ 1.0 }, Point1D{ 2.0 }, Point1D{ 3.0 } };
    auto const tree = DualtreePoint(vpt, 2, std::nullopt, 1);
    auto const ids = tree.RangeSearch({ 1.1, 3.1 }, vpt);

    EXPECT_TRUE((std::ranges::is_permutation(vector<EntityID>{ 2, 3 }, ids)));
  }

  TEST(Point_SearchTest, RangeSearch__AtNodeBorder__2)
  {
    auto constexpr vpt = array{ Point1D{ 0.0 }, Point1D{ 1.0 }, Point1D{ 2.0 }, Point1D{ 3.0 } };
    auto const tree = DualtreePoint(vpt, 2, std::nullopt, 1);
    auto const ids = tree.RangeSearch({ 1.5, 2.25 }, vpt);

    EXPECT_TRUE((std::ranges::is_permutation(vector<EntityID>{ 2 }, ids)));
  }

  TEST(Point_SearchTest, RangeSearch__ZeroBox__Empty)
  {
    auto constexpr vpt = array{ Point1D{ 0.0 }, Point1D{ 1.0 }, Point1D{ 2.0 }, Point1D{ 3.0 } };
    auto const tree = DualtreePoint(vpt, 2, std::nullopt, 1);
    auto const ids = tree.RangeSearch({}, vpt);

    EXPECT_TRUE((ids.empty()));
  }

  TEST(Point_SearchTest, RangeSearch__Empty)
  {
    auto constexpr vpt = array{ Point1D{ 0.0 }, Point1D{ 1.0 }, Point1D{ 2.0 }, Point1D{ 3.0 } };
    auto const tree = DualtreePoint(vpt, 2, std::nullopt, 1);
    auto const ids = tree.RangeSearch({ 1.5, 1.75 }, vpt);

    EXPECT_TRUE((ids.empty()));
  }

  TEST(Point_SearchTest, Query)
  {
    auto constexpr vpt = array{
      Point2D{ 0.0, 0.0 },
      Point2D{ 8.0, 8.0 },
      Point2D{ 2.0, 1.0 },
      Point2D{ 3.5, 3.0 },
      Point2D{ 4.5, 3.0 },
      Point2D{ 5.5, 4.0 },
      Point2D{ 6.5, 5.5 },
      Point2D{ 4.5, 5.5 },
      Point2D{ 4.0, 4.0 }
    };
    auto const tree = QuadtreePoint(vpt, 3, std::nullopt, 1);

    auto const conditions = std::array<QuadtreePoint::QueryCondition, 4>{
      QuadtreePoint::FrustumCondition{
        .boundaryPlanes = { { Plane2D{ .OrigoDistance = std::numbers::sqrt2 * 4.0, .Normal = { std::numbers::sqrt2 * 0.5, std::numbers::sqrt2 * 0.5 } }, true } }
      },
      QuadtreePoint::RangeCondition{ .range{ .Min = { 3.0, 2.0 }, .Max = { 7.0, 6.0 } }, .rangeSearchMode = RangeSearchMode::Inside },
      QuadtreePoint::PlaneIntersectionCondition{ .plane = Plane2D{ .OrigoDistance = 5.0, .Normal = { 1.0, 0.0 } }, .tolerance = 1.0 },
      QuadtreePoint::EntityIDCondition([](QuadtreePoint::EntityID id) { return id > 5; })
    };

    auto const idsAnd = tree.Query(conditions, vpt);
    EXPECT_TRUE((std::ranges::is_permutation(vector<EntityID>{ 7, 8 }, idsAnd)));

    auto const idsOr = tree.Query<LogicalOperator::Or>(conditions, vpt);
    EXPECT_TRUE((std::ranges::is_permutation(vector<EntityID>{ 1, 3, 4, 5, 6, 7, 8 }, idsOr)));

    auto const treeC = QuadtreePointM(vpt, 3, std::nullopt, 1);
    auto const idsAndC = treeC.Query(conditions);
    EXPECT_TRUE((std::ranges::is_permutation(vector<EntityID>{ 7, 8 }, idsAndC)));
  }
} // namespace Point_SearchTest

namespace Point_General
{
  TEST(Point_General, Create_SetNo1)
  {
    auto constexpr N = 2;
    auto constexpr points = getPointSetNo1<N>();
    auto const nt = OrthoTreePointND<N>(points, 3, std::nullopt, 2);
    auto const& nodes = nt.GetNodes();
    EXPECT_EQ(22, nodes.size());
  }

  TEST(Point_General, PlaneSearch_3D_XY_dP0)
  {
    auto const vpt = vector{
      Point3D{ 2.0, -1.0,     0.0 },
      Point3D{ 3.0, -2.0,     1.0 },
      Point3D{ 4.0, -3.0,    0.02 },
      Point3D{ 4.0, -2.0,    0.00 },
      Point3D{ 5.0,  0.0, 0.00999 }
    };

    auto const tree = OctreePoint(vpt, 3, std::nullopt, 2);

    auto const ids = tree.PlaneSearch(0.0, Point3D{ 0.0, 0.0, 1.0 }, vpt, 0.01);
    EXPECT_TRUE((std::ranges::is_permutation(vector<EntityID>{ 0, 3, 4 }, ids)));
  }

  TEST(Point_General, PlaneSearch_3D_XY_dP1)
  {
    auto const vpt = vector{
      Point3D{ 2.0, -1.0,     0.0 },
      Point3D{ 3.0, -2.0,     1.0 },
      Point3D{ 4.0, -3.0,    0.02 },
      Point3D{ 4.0, -2.0,    0.00 },
      Point3D{ 5.0,  0.0, 1.00999 }
    };

    auto const tree = OctreePoint(vpt, 3, std::nullopt, 2);

    auto const ids = tree.PlaneSearch(1.0, Point3D{ 0.0, 0.0, 1.0 }, vpt, 0.01);
    EXPECT_TRUE((std::ranges::is_permutation(vector<EntityID>{ 1, 4 }, ids)));
  }

  TEST(Point_General, PlaneSearch_3D_XZ_dP4)
  {
    auto const vpt = vector{
      Point3D{ 2.0, -1.0,     0.0 },
      Point3D{ 3.0, -2.0,     1.0 },
      Point3D{ 4.0, -3.0,    0.02 },
      Point3D{ 4.0, -2.0,    0.00 },
      Point3D{ 5.0,  0.0, 1.00999 }
    };

    auto const tree = OctreePoint(vpt, 3, std::nullopt, 2);

    auto const ids = tree.PlaneSearch(4.0, Point3D{ 1.0, 0.0, 0.0 }, vpt, 0.01);
    EXPECT_TRUE((std::ranges::is_permutation(vector<EntityID>{ 2, 3 }, ids)));
  }

  TEST(Point_General, PlaneSearchC_3D_XZ_dP4)
  {
    auto const vpt = vector{
      Point3D{ 2.0, -1.0,     0.0 },
      Point3D{ 3.0, -2.0,     1.0 },
      Point3D{ 4.0, -3.0,    0.02 },
      Point3D{ 4.0, -2.0,    0.00 },
      Point3D{ 5.0,  0.0, 1.00999 }
    };

    auto const tree = OctreePointM(vpt, 3, std::nullopt, 2);

    auto const ids = tree.PlaneSearch(4.0, Point3D{ 1.0, 0.0, 0.0 }, 0.01);
    EXPECT_TRUE((std::ranges::is_permutation(vector<EntityID>{ 2, 3 }, ids)));
  }
} // namespace Point_General

namespace Point_Insert
{
  TEST(Point_Insert, CreateWithDataThenInsert)
  {
    auto points = std::vector<Point3D>{
      { -2.0, -2.0, -2.0 },
      { -1.0, -1.0, +2.0 },
      { +2.0, +2.0, +2.0 },
      { +4.0, +1.0, +1.0 },
      { +2.5, +2.5, +2.5 },
      { +3.3, +3.3, +3.3 },
    };
    auto pointNo = EntityID(points.size());

    OctreePoint tree(
      points,
      8,
      BoundingBox3D{
        { -4, -4, -4 },
        { +4, +4, +4 }
      },
      1);
    tree.UpdateIndexes({}, SEQ_EXEC);

    points.emplace_back(Point3D{ +5.0, +4.0, +4.0 });
    auto const isOutsiderInserted = tree.InsertIntoLeaf(pointNo, points.back(), InsertionMode::ExistingLeaf);
    EXPECT_TRUE((isOutsiderInserted));
    ++pointNo;

    points.emplace_back(Point3D{ +3.0, +3.0, +3.0 });
    tree.InsertIntoLeaf(pointNo, points.back(), InsertionMode::ExistingLeaf);
    auto const nodeID_6 = tree.Find(pointNo);
    EXPECT_EQ(nodeID_6, 1023);
    ++pointNo;

    points.emplace_back(Point3D{ +2.0, +2.0, +2.0 });
    tree.InsertIntoLeaf(pointNo, points.back(), InsertionMode::LowestLeaf);
    auto const nodeID_7 = tree.Find(pointNo);
    EXPECT_EQ(nodeID_7, 33292288);
    ++pointNo;

    points.emplace_back(Point3D{ +3.25, +3.25, +3.25 });
    tree.InsertIntoLeaf(pointNo, points.back(), InsertionMode::ExistingLeaf);
    auto const nodeID_8 = tree.Find(pointNo);
    EXPECT_EQ(nodeID_8, 1023);
    ++pointNo;

    {
      points.emplace_back(Point3D{ +3.15, +3.15, +3.15 });
      tree.InsertIntoLeaf(pointNo, points.back(), InsertionMode::ExistingLeaf);
      auto const nodeID_9 = tree.Find(pointNo);
      EXPECT_EQ(nodeID_9, 1023);
      ++pointNo;

      points.emplace_back(Point3D{ +3.0, +3.0, +3.5 });
      tree.InsertIntoLeaf(pointNo, points.back(), InsertionMode::ExistingLeaf);
      auto const nodeID_10 = tree.Find(pointNo);
      EXPECT_EQ(nodeID_10, 1023);
      ++pointNo;

      {
        points.emplace_back(Point3D{ +3.0, +3.0, +3.15 });
        tree.InsertIntoLeaf(pointNo, points.back(), InsertionMode::ExistingLeaf);
        auto const nodeID_11 = tree.Find(pointNo);
        EXPECT_EQ(nodeID_11, 1023);
        ++pointNo;

        auto const& nodes = tree.GetNodes();
        auto const entitiesInBFS = tree.GetEntitiesBreadthFirst();
        auto const entitiesInDFS = tree.GetEntitiesDepthFirst();

        EXPECT_EQ(11, nodes.size());
        EXPECT_TRUE((entitiesInBFS == std::vector<EntityID>{ 6, 0, 1, 3, 5, 7, 9, 10, 11, 12, 2, 4, 8 }));
        EXPECT_TRUE((entitiesInDFS == std::vector<EntityID>{ 6, 0, 1, 3, 2, 8, 4, 5, 7, 9, 10, 11, 12 }));
      }

      {
        points.emplace_back(Point3D{ +3.75, +3.75, +3.75 });
        tree.Insert(pointNo, points.back(), points);
        auto const nodeID_12 = tree.Find(pointNo);
        EXPECT_EQ(nodeID_12, 33553920);
        ++pointNo;

        auto const nodeID_10_u = tree.Find(10);
        auto const nodeID_11_u = tree.Find(11);
        auto const nodeID_12_u = tree.Find(12);
        auto const& nodes = tree.GetNodes();
        auto const entitiesInBFS = tree.GetEntitiesBreadthFirst();
        auto const entitiesInDFS = tree.GetEntitiesDepthFirst();

        EXPECT_EQ(nodeID_10_u, 8184);
        EXPECT_EQ(nodeID_11_u, 33538048);
        EXPECT_EQ(nodeID_12_u, 8184);
        EXPECT_EQ(14, nodes.size());
        EXPECT_TRUE((entitiesInBFS == std::vector<EntityID>{ 6, 0, 1, 3, 2, 4, 5, 7, 9, 10, 12, 11, 13, 8 }));
        EXPECT_TRUE((entitiesInDFS == std::vector<EntityID>{ 6, 0, 1, 3, 2, 8, 4, 5, 7, 9, 10, 12, 11, 13 }));
      }

      points.emplace_back(Point3D{ -2.0, -2.0, -2.0 });
      tree.Insert(pointNo, points.back(), points);
      auto const nodeID_13 = tree.Find(pointNo);
      EXPECT_EQ(nodeID_13, 18612224);
      EXPECT_EQ(8, SI3D::GetDepthID(nodeID_13));
      ++pointNo;
    }

    {
      auto const p1 = Point3D{ +3.75, +3.75, +3.76 };
      auto const isP1Inserted = tree.InsertUnique(p1, points, 0.01);
      EXPECT_FALSE((isP1Inserted));

      auto const p2 = Point3D{ +3.75, +3.75, +3.75 };
      auto const isP2Inserted = tree.InsertUnique(p2, points, 0.0);
      EXPECT_TRUE((isP2Inserted));
      if (isP2Inserted)
      {
        points.emplace_back(p2);
        ++pointNo;
      }

      auto const p3 = Point3D{ +3.75, +3.75, +3.77 };
      auto const isP3Inserted = tree.InsertUnique(p3, points, 0.001);
      EXPECT_TRUE((isP3Inserted));
      if (isP3Inserted)
      {
        points.emplace_back(p3);
        ++pointNo;
      }
    }

    tree.Update(2, Point3D{ +2.0, +2.2, +2.0 });
    auto const nodeID_2_u1 = tree.Find(2);

    points[2] = { +2.0, +2.0, +2.6 };
    tree.Update(2, points[2]);
    auto const nodeID_2_u2 = tree.Find(2);
    EXPECT_EQ(nodeID_2_u1, 65024);
    EXPECT_EQ(nodeID_2_u2, 8132);
    EXPECT_FALSE((tree.HasNode(520194)));

    points[6] = { -2.0, -2.0, -2.0 };
    tree.Update(6, points[6]);
    auto const nodeID_6_u1 = tree.Find(6);

    auto const oldbox2 = points[2];
    points[2] = { -2.0, -2.5, -2.0 };
    tree.Update(2, oldbox2, points[2]);
    auto const nodeID_2_u3 = tree.Find(2);

    auto const oldbox4 = points[4];
    points[4] = { +3.50, +3.50, +3.50 };
    tree.Update(4, oldbox4, points[4], points);
    auto const nodeID_4_u1 = tree.Find(4);
    auto const nodeID_9_u1 = tree.Find(9);
    {
      points[8] = { +3.0, +3.0, +3.75 };
      tree.Update(8, points[8], points);
      auto const nodeID_8_u1 = tree.Find(8);

      auto const& nodes = tree.GetNodes();
      auto const entitiesInBFS = tree.GetEntitiesBreadthFirst();
      auto const entitiesInDFS = tree.GetEntitiesDepthFirst();

      EXPECT_EQ(nodeID_6_u1, 18612224);
      EXPECT_EQ(nodeID_2_u3, 69);
      EXPECT_EQ(nodeID_4_u1, 8191);
      EXPECT_EQ(nodeID_9_u1, 8184);
      EXPECT_EQ(nodeID_8_u1, 8188);

      EXPECT_EQ(14, nodes.size());
      EXPECT_TRUE((entitiesInBFS == std::vector<EntityID>{ 1, 2, 0, 14, 6, 3, 5, 7, 9, 10, 12, 8, 4, 11, 13, 15, 16 }));
      EXPECT_TRUE((entitiesInDFS == std::vector<EntityID>{ 2, 0, 14, 6, 1, 3, 5, 7, 9, 10, 12, 8, 11, 4, 13, 15, 16 }));
    }
  }
} // namespace Point_Insert

namespace Point_EditTest
{
  struct UserDefinedConfig : Configuration<>
  {
    static constexpr bool ALLOW_OUT_OF_SPACE_INSERTION = false;
    static constexpr std::size_t DEFAULT_TARGET_ELEMENT_NUM_IN_NODES = 10;
  };
  using DualtreePointStrictBound = OrthoTreeBase<PointEntitySpanAdapter<Point1D>, GeneralGeometryAdapterND<1>, UserDefinedConfig>;

  TEST(Point_EditTest, Insert__NonLeaf__Successful)
  {
    auto constexpr vpt = array{ Point1D{ 0.0 }, Point1D{ 1.0 }, Point1D{ 2.0 }, Point1D{ 3.0 } };
    auto tree = DualtreePoint(vpt, 3, std::nullopt, 1);

    EXPECT_TRUE((tree.InsertIntoLeaf(4, Point1D{ 2.5 }, InsertionMode::ExistingLeaf)));

    EXPECT_EQ(7, tree.GetNodeCount());
    EXPECT_TRUE((AreContainersItemsEqual(tree.GetNodeEntities(tree.GetNodeValue(7)), vector<EntityID>{ 3, 4 })));
  }

  TEST(Point_EditTest, Insert__Leaf__Successful)
  {
    auto constexpr vpt = array{ Point1D{ 0.0 }, Point1D{ 1.0 }, Point1D{ 2.0 }, Point1D{ 3.0 } };
    auto tree = DualtreePoint(vpt, 3, std::nullopt, 1);

    EXPECT_TRUE((tree.InsertIntoLeaf(4, Point1D{ 2.5 }, InsertionMode::LowestLeaf)));

    auto const& nodes = tree.GetNodes();
    EXPECT_EQ(nodes.size(), 8);
    EXPECT_TRUE((AreContainersItemsEqual(tree.GetNodeEntities(tree.GetNodeValue(14)), vector<EntityID>{ 4 })));
  }

  TEST(Point_EditTest, Insert__OutOfSpace__ReturnsFalse)
  {
    auto constexpr vpt = array{ Point1D{ 0.0 }, Point1D{ 1.0 }, Point1D{ 2.0 }, Point1D{ 3.0 } };
    auto tree = DualtreePointStrictBound(vpt, 3, std::nullopt, 1);
    EXPECT_FALSE((tree.InsertIntoLeaf(4, Point1D{ 4.0 }, InsertionMode::LowestLeaf)));

    auto const& nodes = tree.GetNodes();
    EXPECT_EQ(nodes.size(), 7);
    EXPECT_TRUE((AreContainersItemsEqual(tree.GetNodeEntities(tree.GetNodeValue(7)), vector<EntityID>{ 3 })));
  }

  TEST(Point_EditTest, Update2p__Update3__Successful)
  {
    auto vpt = vector{ Point1D{ 0.0 }, Point1D{ 1.0 }, Point1D{ 2.0 }, Point1D{ 3.0 } };
    auto tree = DualtreePoint(vpt, 3, std::nullopt, 1);

    EXPECT_TRUE((tree.Update(3, { 1.1 })));
    auto const ids = tree.GetEntitiesBreadthFirst();
    EXPECT_TRUE((ids == vector<EntityID>{ 0, 1, 3, 2 }));
  }

  TEST(Point_EditTest, Update2p__Update2__Successful)
  {
    auto vpt = vector{ Point1D{ 0.0 }, Point1D{ 1.0 }, Point1D{ 2.0 }, Point1D{ 3.0 } };
    auto tree = DualtreePoint(vpt, 3, std::nullopt, 1);

    EXPECT_TRUE((tree.Update(2, { 1.1 })));
    auto const ids = tree.GetEntitiesBreadthFirst();
    EXPECT_TRUE((ids == vector<EntityID>{ 0, 1, 2, 3 }));
  }

  TEST(Point_EditTest, Update2p__Rewrite3OutOfBox__ReturnsFalse)
  {
    auto vpt = vector{ Point1D{ 0.0 }, Point1D{ 1.0 }, Point1D{ 2.0 }, Point1D{ 3.0 } };
    auto tree = DualtreePointStrictBound(vpt, 3, std::nullopt, 1);

    auto const idsPre = tree.GetEntitiesBreadthFirst();
    EXPECT_FALSE((tree.Update(2, { 3.1 })));
    auto const idsPost = tree.GetEntitiesBreadthFirst();

    EXPECT_TRUE((idsPre == idsPost));
  }

  TEST(Point_EditTest, Erase__3__Removed)
  {
    auto constexpr vpt = array{ Point1D{ 0.0 }, Point1D{ 1.0 }, Point1D{ 2.0 }, Point1D{ 3.0 } };
    auto tree = DualtreePoint(vpt, 3, std::nullopt, 1);

    auto const idsPre = tree.GetEntitiesBreadthFirst();
    EXPECT_TRUE((tree.Erase(3)));
    auto const idsPost = tree.GetEntitiesBreadthFirst();

    EXPECT_EQ(idsPre.size() - 1, idsPost.size());
    EXPECT_TRUE((idsPost == vector<EntityID>{ 0, 1, 2 }));
  }

  TEST(Point_EditTest, Erase__Nonexist__ReturnsFalse)
  {
    auto constexpr vpt = array{ Point1D{ 0.0 }, Point1D{ 1.0 }, Point1D{ 2.0 }, Point1D{ 3.0 } };
    auto tree = DualtreePoint(vpt, 3, std::nullopt, 1);

    auto const idsPre = tree.GetEntitiesBreadthFirst();
    EXPECT_FALSE((tree.Erase(4)));
    auto const idsPost = tree.GetEntitiesBreadthFirst();

    EXPECT_TRUE((idsPre == idsPost));
  }

  TEST(Point_EditTest, Erase__WrongPointWasGiven__ReturnsFalse)
  {
    auto constexpr vpt = array{ Point1D{ 0.0 }, Point1D{ 1.0 }, Point1D{ 2.0 }, Point1D{ 3.0 } };
    auto tree = DualtreePoint(vpt, 3, std::nullopt, 1);

    auto const idsPre = tree.GetEntitiesBreadthFirst();
    EXPECT_FALSE((tree.Erase(3, vpt.front())));
    auto const idsPost = tree.GetEntitiesBreadthFirst();

    EXPECT_TRUE((idsPre == idsPost));
  }
} // namespace Point_EditTest

namespace Box_General
{
  TEST(Box_General, Create_d3__Node7)
  {
    auto constexpr boxes = array{
      BoundingBox2D{ { 0.0, 0.0 }, { 1.0, 1.0 } },
      BoundingBox2D{ { 1.0, 1.0 }, { 2.0, 2.0 } },
      BoundingBox2D{ { 2.0, 2.0 }, { 3.0, 3.0 } },
      BoundingBox2D{ { 3.0, 3.0 }, { 4.0, 4.0 } },
      BoundingBox2D{ { 1.2, 1.2 }, { 2.8, 2.8 } }
    };

    auto const quadtreebox = QuadtreeBox(boxes, 3, std::nullopt, 1);
    auto const nodes = quadtreebox.GetNodes();
    EXPECT_EQ(7, nodes.size());
    EXPECT_TRUE((nodes.contains(1)));
    EXPECT_TRUE((quadtreebox.IsNodeEntitiesEmpty(quadtreebox.GetRootNodeValue())));

    EXPECT_TRUE((nodes.contains(4)));
    EXPECT_TRUE((quadtreebox.IsNodeEntitiesEmpty(quadtreebox.GetRootNodeValue())));

    EXPECT_TRUE((nodes.contains(7)));
    EXPECT_TRUE((AreContainersItemsEqual(quadtreebox.GetNodeEntities(quadtreebox.GetNodeValue(7)), vector<EntityID>{ 4 })));

    EXPECT_TRUE((nodes.contains(16)));
    EXPECT_TRUE((AreContainersItemsEqual(quadtreebox.GetNodeEntities(quadtreebox.GetNodeValue(16)), vector<EntityID>{ 0 })));

    EXPECT_TRUE((nodes.contains(19)));
    EXPECT_TRUE((AreContainersItemsEqual(quadtreebox.GetNodeEntities(quadtreebox.GetNodeValue(19)), vector<EntityID>{ 1 })));

    EXPECT_TRUE((nodes.contains(28)));
    EXPECT_TRUE((AreContainersItemsEqual(quadtreebox.GetNodeEntities(quadtreebox.GetNodeValue(28)), vector<EntityID>{ 2 })));

    EXPECT_TRUE((nodes.contains(31)));
    EXPECT_TRUE((AreContainersItemsEqual(quadtreebox.GetNodeEntities(quadtreebox.GetNodeValue(31)), vector<EntityID>{ 3 })));
  }

  TEST(Box_General, Create_d3__Node8)
  {
    auto constexpr boxes = array{
      BoundingBox2D{ { 0.0, 0.0 }, { 1.0, 1.0 } },
      BoundingBox2D{ { 1.0, 1.0 }, { 2.0, 2.0 } },
      BoundingBox2D{ { 2.0, 2.0 }, { 3.0, 3.0 } },
      BoundingBox2D{ { 3.0, 3.0 }, { 4.0, 4.0 } },
      BoundingBox2D{ { 1.2, 1.2 }, { 2.8, 2.8 } },
      BoundingBox2D{ { 1.5, 1.5 }, { 2.0, 2.0 } },
    };

    auto const quadtreebox = QuadtreeBox(boxes, 3, std::nullopt, 1);
    auto const nodes = quadtreebox.GetNodes();
    EXPECT_EQ(8, nodes.size());

    EXPECT_TRUE((nodes.contains(1)));
    EXPECT_TRUE((quadtreebox.IsNodeEntitiesEmpty(quadtreebox.GetNodeValue(1))));

    EXPECT_TRUE((nodes.contains(4)));
    EXPECT_TRUE((quadtreebox.IsNodeEntitiesEmpty(quadtreebox.GetNodeValue(4))));

    EXPECT_TRUE((nodes.contains(7)));
    EXPECT_TRUE((AreContainersItemsEqual(quadtreebox.GetNodeEntities(quadtreebox.GetNodeValue(7)), vector<EntityID>{ 4 })));

    EXPECT_TRUE((nodes.contains(16)));
    EXPECT_TRUE((AreContainersItemsEqual(quadtreebox.GetNodeEntities(quadtreebox.GetNodeValue(16)), vector<EntityID>{ 0 })));

    EXPECT_TRUE((nodes.contains(19)));
    EXPECT_TRUE((AreContainersItemsEqual(quadtreebox.GetNodeEntities(quadtreebox.GetNodeValue(19)), vector<EntityID>{ 1 })));

    EXPECT_TRUE((nodes.contains(28)));
    EXPECT_TRUE((AreContainersItemsEqual(quadtreebox.GetNodeEntities(quadtreebox.GetNodeValue(28)), vector<EntityID>{ 2 })));

    EXPECT_TRUE((nodes.contains(31)));
    EXPECT_TRUE((AreContainersItemsEqual(quadtreebox.GetNodeEntities(quadtreebox.GetNodeValue(31)), vector<EntityID>{ 3 })));

    EXPECT_TRUE((nodes.contains(79)));
    EXPECT_TRUE((AreContainersItemsEqual(quadtreebox.GetNodeEntities(quadtreebox.GetNodeValue(79)), vector<EntityID>{ 5 })));
  }
} // namespace Box_General

namespace Box_SearchTest
{
  TEST(Box_SearchTest, LooseStrategy_Seq)
  {
    auto const boxes = array{
      BoundingBox2D{ { 0.0, 0.0 }, { 1.0, 1.0 } },
      BoundingBox2D{ { 1.0, 1.0 }, { 2.0, 2.0 } },
      BoundingBox2D{ { 2.0, 2.0 }, { 3.0, 3.0 } },
      BoundingBox2D{ { 3.0, 3.0 }, { 4.0, 4.0 } },
      BoundingBox2D{ { 1.2, 1.2 }, { 2.8, 2.8 } },
      BoundingBox2D{ { 1.2, 1.2 }, { 2.8, 1.8 } },
      BoundingBox2D{ { 0.5, 0.0 }, { 1.5, 0.5 } }
    };

    auto quadtreebox = QuadtreeBox(boxes, 2, std::nullopt, 1);

    auto const& nodes = quadtreebox.GetNodes();
    auto const entitiesInBFS = quadtreebox.GetEntitiesBreadthFirst();
    auto const entitiesInDFS = quadtreebox.GetEntitiesDepthFirst();

    EXPECT_EQ(nodes.size(), 9);
    EXPECT_TRUE((AreContainersItemsEqual(entitiesInBFS, vector<EntityID>{ 5, 4, 0, 6, 1, 2, 3 })));
    EXPECT_TRUE((AreContainersItemsEqual(entitiesInDFS, vector<EntityID>{ 0, 6, 1, 5, 4, 2, 3 })));
  }

  TEST(Box_SearchTest, LooseStrategy_Par)
  {
    auto const boxes = array{
      BoundingBox2D{ { 0.0, 0.0 }, { 1.0, 1.0 } },
      BoundingBox2D{ { 1.0, 1.0 }, { 2.0, 2.0 } },
      BoundingBox2D{ { 2.0, 2.0 }, { 3.0, 3.0 } },
      BoundingBox2D{ { 3.0, 3.0 }, { 4.0, 4.0 } },
      BoundingBox2D{ { 1.2, 1.2 }, { 2.8, 2.8 } },
      BoundingBox2D{ { 1.2, 1.2 }, { 2.8, 1.8 } },
      BoundingBox2D{ { 0.5, 0.0 }, { 1.5, 0.5 } }
    };

    auto quadtreebox = QuadtreeBox{};
    QuadtreeBox::Create<ExecutionTags::Parallel>(quadtreebox, boxes, 2, std::nullopt, 1);

    auto const& nodes = quadtreebox.GetNodes();
    auto const entitiesInBFS = quadtreebox.GetEntitiesBreadthFirst();
    auto const entitiesInDFS = quadtreebox.GetEntitiesDepthFirst();

    EXPECT_EQ(nodes.size(), 9);
    EXPECT_TRUE((AreContainersItemsEqual(entitiesInBFS, vector<EntityID>{ 5, 4, 0, 6, 1, 2, 3 })));
    EXPECT_TRUE((AreContainersItemsEqual(entitiesInDFS, vector<EntityID>{ 0, 6, 1, 5, 4, 2, 3 })));
  }

  TEST(Box_SearchTest, RangeSearch__Inside__124)
  {
    auto constexpr boxes = array{
      BoundingBox2D{ { 0.0, 0.0 }, { 1.0, 1.0 } },
      BoundingBox2D{ { 1.0, 1.0 }, { 2.0, 2.0 } },
      BoundingBox2D{ { 2.0, 2.0 }, { 3.0, 3.0 } },
      BoundingBox2D{ { 3.0, 3.0 }, { 4.0, 4.0 } },
      BoundingBox2D{ { 1.2, 1.2 }, { 2.8, 2.8 } }
    };

    auto const quadtreebox = QuadtreeBox(boxes, 3, std::nullopt, 2);

    auto const search_box = BoundingBox2D{
      { 1.0, 1.0 },
      { 3.1, 3.1 }
    };
    auto const ids = quadtreebox.RangeSearch(search_box, boxes);

    EXPECT_TRUE((std::ranges::is_permutation(vector<EntityID>{ 1, 2, 4 }, ids)));
  }

  TEST(Box_SearchTest, RangeSearch__Overlap__1234)
  {
    auto constexpr boxes = array{
      BoundingBox2D{ { 0.0, 0.0 }, { 1.0, 1.0 } },
      BoundingBox2D{ { 1.0, 1.0 }, { 2.0, 2.0 } },
      BoundingBox2D{ { 2.0, 2.0 }, { 3.0, 3.0 } },
      BoundingBox2D{ { 3.0, 3.0 }, { 4.0, 4.0 } },
      BoundingBox2D{ { 1.2, 1.2 }, { 2.8, 2.8 } }
    };

    auto const quadtreebox = QuadtreeBox(boxes, 3, std::nullopt, 2);

    auto const search_box = BoundingBox2D{
      { 1.0, 1.0 },
      { 3.1, 3.1 }
    };
    auto const ids = quadtreebox.RangeSearch(search_box, boxes, RangeSearchMode::Overlap);

    EXPECT_TRUE((std::ranges::is_permutation(vector<EntityID>{ 1, 2, 3, 4 }, ids)));
  }

  TEST(Box_SearchTest, PickSearch__24)
  {
    auto constexpr boxes = array{
      BoundingBox2D{ { 0.0, 0.0 }, { 1.0, 1.0 } },
      BoundingBox2D{ { 1.0, 1.0 }, { 2.0, 2.0 } },
      BoundingBox2D{ { 2.0, 2.0 }, { 3.0, 3.0 } },
      BoundingBox2D{ { 3.0, 3.0 }, { 4.0, 4.0 } },
      BoundingBox2D{ { 1.2, 1.2 }, { 2.8, 2.8 } }
    };

    auto const quadtreebox = QuadtreeBox(boxes, 3, std::nullopt, 2);

    auto const ptPick = Point2D{ 2.5, 2.5 };
    auto ids_picked = quadtreebox.PickSearch(ptPick, boxes);

    EXPECT_TRUE((std::ranges::is_permutation(vector<EntityID>{ 2, 4 }, ids_picked)));
  }

  TEST(Box_SearchTest, PickSearch__124)
  {
    auto constexpr boxes = array{
      BoundingBox2D{ { 0.0, 0.0 }, { 1.0, 1.0 } },
      BoundingBox2D{ { 1.0, 1.0 }, { 2.0, 2.0 } },
      BoundingBox2D{ { 2.0, 2.0 }, { 3.0, 3.0 } },
      BoundingBox2D{ { 3.0, 3.0 }, { 4.0, 4.0 } },
      BoundingBox2D{ { 1.2, 1.2 }, { 2.8, 2.8 } }
    };

    auto const quadtreebox = QuadtreeBox(boxes, 3, std::nullopt, 2);

    auto const ptPick = Point2D{ 2.0, 2.0 };
    auto ids_picked = quadtreebox.PickSearch(ptPick, boxes);

    EXPECT_TRUE((std::ranges::is_permutation(vector<EntityID>{ 1, 2, 4 }, ids_picked)));
  }

  TEST(Box_SearchTest, PlaneIntersection_3D_XY_d0)
  {
    auto constexpr boxes = array{
      BoundingBox3D{  { 0.0, 0.0, 0.0 },  { 1.0, 1.0, 0.0 } },
      BoundingBox3D{ { 1.0, 1.0, -1.0 },  { 2.0, 2.0, 0.0 } },
      BoundingBox3D{ { 2.0, 2.0, -1.0 }, { 3.0, 3.0, -0.2 } },
      BoundingBox3D{  { 3.0, 3.0, 1.0 },  { 4.0, 4.0, 1.2 } },
      BoundingBox3D{ { 1.2, 1.2, -1.0 },  { 2.8, 2.8, 1.0 } }
    };

    auto const octreebox = OctreeBox(boxes, 3, std::nullopt, 2);
    auto const intersectedBoxes = octreebox.PlaneIntersection(0.0, Point3D{ 0.0, 0.0, 1.0 }, boxes, 0.01);

    EXPECT_TRUE((std::ranges::is_permutation(vector<EntityID>{ 0, 1, 4 }, intersectedBoxes)));
  }

  TEST(Box_SearchTest, PlaneIntersection_3D_XY_dP1)
  {
    auto constexpr boxes = array{
      BoundingBox3D{  { 0.0, 0.0, 0.0 },  { 1.0, 1.0, 0.0 } },
      BoundingBox3D{ { 1.0, 1.0, -1.0 },  { 2.0, 2.0, 0.0 } },
      BoundingBox3D{ { 2.0, 2.0, -1.0 }, { 3.0, 3.0, -0.2 } },
      BoundingBox3D{  { 3.0, 3.0, 0.8 },  { 4.0, 4.0, 0.9 } },
      BoundingBox3D{ { 1.2, 1.2, -1.0 },  { 2.8, 2.8, 1.1 } }
    };

    auto const octreebox = OctreeBox(boxes, 3, std::nullopt, 2);
    auto const intersectedBoxes = octreebox.PlaneIntersection(1.0, Point3D{ 0.0, 0.0, 1.0 }, boxes, 0.01);

    EXPECT_TRUE((std::ranges::is_permutation(vector<EntityID>{ 4 }, intersectedBoxes)));
  }

  TEST(Box_SearchTest, PlaneIntersection_3D_XY_dN1)
  {
    auto constexpr boxes = array{
      BoundingBox3D{  { 0.0, 0.0, 0.0 },  { 1.0, 1.0, 0.0 } },
      BoundingBox3D{ { 1.0, 1.0, -1.0 },  { 2.0, 2.0, 0.0 } },
      BoundingBox3D{ { 2.0, 2.0, -1.0 }, { 3.0, 3.0, -0.2 } },
      BoundingBox3D{  { 3.0, 3.0, 1.0 },  { 4.0, 4.0, 1.2 } },
      BoundingBox3D{ { 1.2, 1.2, -1.1 },  { 2.8, 2.8, 1.1 } }
    };

    auto const octreebox = OctreeBox(boxes, 3, std::nullopt, 2);
    auto const intersectedBoxes = octreebox.PlaneIntersection(-1.0, Point3D{ 0.0, 0.0, 1.0 }, boxes, 0.01);

    EXPECT_TRUE((std::ranges::is_permutation(vector<EntityID>{ 1, 2, 4 }, intersectedBoxes)));
  }

  TEST(Box_SearchTest, PlaneIntersection_3D_YZ_dP1)
  {
    auto constexpr boxes = array{
      BoundingBox3D{  { 0.0, 0.0, 0.0 },  { 1.0, 1.0, 0.0 } },
      BoundingBox3D{ { 1.0, 1.0, -1.0 },  { 2.0, 2.0, 0.0 } },
      BoundingBox3D{ { 2.0, 2.0, -1.0 }, { 3.0, 3.0, -0.2 } },
      BoundingBox3D{  { 3.0, 3.0, 1.0 },  { 4.0, 4.0, 1.2 } },
      BoundingBox3D{ { 1.2, 1.2, -1.1 },  { 2.8, 2.8, 1.1 } }
    };

    auto const octreebox = OctreeBox(boxes, 3, std::nullopt, 2);
    auto const intersectedBoxes = octreebox.PlaneIntersection(1.0, Point3D{ 1.0, 0.0, 0.0 }, boxes, 0.01);

    EXPECT_TRUE((std::ranges::is_permutation(vector<EntityID>{ 0, 1 }, intersectedBoxes)));
  }

  TEST(Box_SearchTest, PlaneIntersection_3D_YZ_dN1)
  {
    auto constexpr boxes = array{
      BoundingBox3D{  { 0.0, 0.0, 0.0 },  { 1.0, 1.0, 0.0 } },
      BoundingBox3D{ { 1.0, 1.0, -1.0 },  { 2.0, 2.0, 0.0 } },
      BoundingBox3D{ { 2.0, 2.0, -1.0 }, { 3.0, 3.0, -0.2 } },
      BoundingBox3D{  { 3.0, 3.0, 1.0 },  { 4.0, 4.0, 1.2 } },
      BoundingBox3D{ { 1.2, 1.2, -1.1 },  { 2.8, 2.8, 1.1 } }
    };

    auto const octreebox = OctreeBox(boxes, 3, std::nullopt, 2);
    auto const intersectedBoxes = octreebox.PlaneIntersection(-1.0, Point3D{ 1.0, 0.0, 0.0 }, boxes, 0.01);

    EXPECT_TRUE((std::ranges::is_permutation(vector<EntityID>{}, intersectedBoxes)));
  }

  TEST(Box_SearchTest, PlaneIntersectionC_3D_YZ_dN1)
  {
    auto constexpr boxes = array{
      BoundingBox3D{  { 0.0, 0.0, 0.0 },  { 1.0, 1.0, 0.0 } },
      BoundingBox3D{ { 1.0, 1.0, -1.0 },  { 2.0, 2.0, 0.0 } },
      BoundingBox3D{ { 2.0, 2.0, -1.0 }, { 3.0, 3.0, -0.2 } },
      BoundingBox3D{  { 3.0, 3.0, 1.0 },  { 4.0, 4.0, 1.2 } },
      BoundingBox3D{ { 1.2, 1.2, -1.1 },  { 2.8, 2.8, 1.1 } }
    };

    auto const octreebox = OctreeBoxM(boxes, 3, std::nullopt, 2);
    auto const intersectedBoxes = octreebox.PlaneIntersection(-1.0, Point3D{ 1.0, 0.0, 0.0 }, 0.01);

    EXPECT_TRUE((std::ranges::is_permutation(vector<EntityID>{}, intersectedBoxes)));
  }

  TEST(Box_SearchTest, PlaneIntersection_3D_YZA_dPSQRT2)
  {
    auto constexpr boxes = array{
      BoundingBox3D{  { 0.0, 0.0, 0.0 },  { 1.0, 1.0, 0.0 } },
      BoundingBox3D{ { 1.0, 1.0, -1.0 },  { 2.0, 2.0, 0.0 } },
      BoundingBox3D{ { 2.0, 2.0, -1.0 }, { 3.0, 3.0, -0.2 } },
      BoundingBox3D{  { 3.0, 3.0, 1.0 },  { 4.0, 4.0, 1.2 } },
      BoundingBox3D{ { 1.2, 1.2, -1.1 },  { 2.8, 2.8, 1.1 } }
    };

    auto const octreebox = OctreeBox(boxes, 3, std::nullopt, 2);
    auto const sqrt2__2 = std::sqrt(2.0) * 0.5;
    auto const intersectedBoxes = octreebox.PlaneIntersection(std::sqrt(2.0), Point3D{ 0.0, sqrt2__2, sqrt2__2 }, boxes, 0.01);

    EXPECT_TRUE((std::ranges::is_permutation(vector<EntityID>{ 1, 2, 4 }, intersectedBoxes)));
  }
} // namespace Box_SearchTest

namespace Box_InsertTest
{
  TEST(Box_InsertTest, CreateWithData)
  {
    std::vector<BoundingBox3D> boxes = {
      { { -2.00375, -0.20375, +0.19625 }, { -1.52125, -0.19625, +0.20375 } },
      { { -0.88692, -1.05210, -0.80026 }, { +0.88692, +0.72175, +0.97359 } },
      { { -0.78692, -1.05210, -0.80026 }, { +0.98692, +0.72175, +0.97359 } },
      { { -0.68692, -1.05210, -0.80026 }, { +1.08692, +0.72175, +0.97359 } },
      { { -0.58692, -1.05210, -0.80026 }, { +1.18692, +0.72175, +0.97359 } },
    };

    OctreeBox tree(
      boxes,
      8,
      BoundingBox3D{
        { -10, -10, -10 },
        { +10, +10, +10 }
      },
      2);
    tree.UpdateIndexes({});
  }

  TEST(Box_InsertTest, AddToEmptyTree)
  {
    std::vector<BoundingBox3D> boxes0;
    OctreeBox tree(
      boxes0,
      21,
      BoundingBox3D{
        { -10, -10, -10 },
        { +10, +10, +10 }
      },
      2);

    std::vector<BoundingBox3D> boxes = {
      { { -2.00375, -0.20375, +0.19625 }, { -1.52125, -0.19625, +0.20375 } },
      { { -0.88692, -1.05210, -0.80026 }, { +0.88692, +0.72175, +0.97359 } },
      { { -0.78692, -1.05210, -0.80026 }, { +0.98692, +0.72175, +0.97359 } },
      { { -0.68692, -1.05210, -0.80026 }, { +1.08692, +0.72175, +0.97359 } },
      { { -0.58692, -1.05210, -0.80026 }, { +1.18692, +0.72175, +0.97359 } },
    };

    for (EntityID i = 0; i < boxes.size(); ++i)
    {
      EXPECT_TRUE((tree.InsertIntoLeaf(i, boxes[i], InsertionMode::LowestLeaf)));
      boxes0.emplace_back(boxes[i]);
    }
    tree.UpdateIndexes({});
  }

  TEST(Box_InsertTest, CreateWithDataThenInsert)
  {
    std::vector<BoundingBox3D> boxes = {
      { { -2.0, -2.0, -2.0 }, { -1.0, -1.0, +2.0 } },
      { { +1.0, +1.0, +1.0 }, { +4.0, +3.1, +3.4 } },
      { { +2.0, +2.0, +2.0 }, { +3.1, +3.1, +3.1 } },
    };
    auto boxNo = EntityID(boxes.size());

    OctreeBox tree(
      boxes,
      8,
      BoundingBox3D{
        { -4, -4, -4 },
        { +4, +4, +4 }
      },
      1);

    {
      auto const& nodes = tree.GetNodes();
      auto const entitiesInBFS = tree.GetEntitiesBreadthFirst();
      auto const entitiesInDFS = tree.GetEntitiesDepthFirst();

      EXPECT_EQ(4, nodes.size());
      EXPECT_TRUE((AreContainersItemsEqual(std::vector<EntityID>{ 0, 1, 2 }, entitiesInBFS)));
      EXPECT_TRUE((AreContainersItemsEqual(std::vector<EntityID>{ 0, 1, 2 }, entitiesInDFS)));
    }

    tree.UpdateIndexes({});

    boxes.emplace_back(
      BoundingBox3D{
        { +3.0, +3.0, +3.0 },
        { +5.0, +4.0, +4.0 }
    });
    auto const isOutsiderInserted = tree.InsertIntoLeaf(boxNo, boxes.back(), InsertionMode::ExistingLeaf);
    ++boxNo;

    EXPECT_TRUE((isOutsiderInserted));

    {
      boxes.emplace_back(
        BoundingBox3D{
          { +3.0, +3.0, +3.0 },
          { +4.0, +4.0, +4.0 }
      });
      tree.InsertIntoLeaf(boxNo, boxes.back(), InsertionMode::ExistingLeaf);
      ++boxNo;

      auto const& nodes = tree.GetNodes();
      auto const entitiesInBFS = tree.GetEntitiesBreadthFirst();
      auto const entitiesInDFS = tree.GetEntitiesDepthFirst();

      EXPECT_EQ(4, nodes.size());
      EXPECT_TRUE((AreContainersItemsEqual(std::vector<EntityID>{ 3, 0, 1, 2, 4 }, entitiesInBFS)));
      EXPECT_TRUE((AreContainersItemsEqual(std::vector<EntityID>{ 3, 0, 1, 2, 4 }, entitiesInDFS)));
    }

    {
      boxes.emplace_back(
        BoundingBox3D{
          { +2.0, +2.0, +2.0 },
          { +3.0, +3.0, +3.0 }
      });
      tree.InsertIntoLeaf(boxNo, boxes.back(), InsertionMode::LowestLeaf);

      auto const& nodes = tree.GetNodes();
      auto const entitiesInBFS = tree.GetEntitiesBreadthFirst();
      auto const entitiesInDFS = tree.GetEntitiesDepthFirst();

      EXPECT_EQ(5, nodes.size());
      EXPECT_TRUE((AreContainersItemsEqual(std::vector<EntityID>{ 3, 0, 1, 2, 4, 5 }, entitiesInBFS)));
      EXPECT_TRUE((AreContainersItemsEqual(std::vector<EntityID>{ 3, 0, 1, 2, 4, 5 }, entitiesInDFS)));

      ++boxNo;
    }

    boxes.emplace_back(
      BoundingBox3D{
        { +3.0, +3.0, +3.0 },
        { +4.0, +4.0, +4.0 }
    });
    tree.InsertIntoLeaf(boxNo, boxes.back(), InsertionMode::ExistingLeaf);
    ++boxNo;

    {
      boxes.emplace_back(
        BoundingBox3D{
          { +3.0, +3.0, +3.0 },
          { +3.5, +3.5, +3.5 }
      });
      tree.InsertIntoLeaf(boxNo, boxes.back(), InsertionMode::ExistingLeaf);
      ++boxNo;

      boxes.emplace_back(
        BoundingBox3D{
          { +3.0, +3.0, +3.5 },
          { +3.5, +3.5, +4.0 }
      });
      tree.InsertIntoLeaf(boxNo, boxes.back(), InsertionMode::ExistingLeaf);
      ++boxNo;
      {
        boxes.emplace_back(
          BoundingBox3D{
            { +3.0, +3.0, +3.1 },
            { +3.5, +3.5, +3.2 }
        });
        tree.InsertIntoLeaf(boxNo, boxes.back(), InsertionMode::ExistingLeaf);
        ++boxNo;

        auto const& nodes = tree.GetNodes();
        auto const entitiesInBFS = tree.GetEntitiesBreadthFirst();
        auto const entitiesInDFS = tree.GetEntitiesDepthFirst();

        EXPECT_EQ(6, nodes.size());
        EXPECT_TRUE((AreContainersItemsEqual(std::vector<EntityID>{ 3, 0, 1, 2, 4, 5, 6, 7, 8, 9 }, entitiesInBFS)));
        EXPECT_TRUE((AreContainersItemsEqual(std::vector<EntityID>{ 3, 0, 1, 2, 4, 5, 6, 7, 8, 9 }, entitiesInDFS)));
      }

      {
        boxes.emplace_back(
          BoundingBox3D{
            { +3.75, +3.75, +3.75 },
            {  +4.0,  +4.0,  +4.0 }
        });
        tree.Insert(boxNo, boxes.back(), boxes);
        ++boxNo;

        auto const& nodes = tree.GetNodes();
        auto const entitiesInBFS = tree.GetEntitiesBreadthFirst();
        auto const entitiesInDFS = tree.GetEntitiesDepthFirst();

        EXPECT_EQ(9, nodes.size());
        EXPECT_TRUE((AreContainersItemsEqual(std::vector<EntityID>{ 3, 0, 1, 2, 4, 5, 6, 7, 9, 8, 10 }, entitiesInBFS)));
        EXPECT_TRUE((AreContainersItemsEqual(std::vector<EntityID>{ 3, 0, 1, 2, 4, 5, 6, 7, 9, 8, 10 }, entitiesInDFS)));
      }

      {
        boxes.emplace_back(
          BoundingBox3D{
            { -2.0, -2.0, -2.0 },
            { -1.0, -1.0, -1.0 }
        });
        tree.Insert(boxNo, boxes.back(), boxes);
        ++boxNo;

        auto const& nodes = tree.GetNodes();
        auto const entitiesInBFS = tree.GetEntitiesBreadthFirst();
        auto const entitiesInDFS = tree.GetEntitiesDepthFirst();

        EXPECT_EQ(10, nodes.size());
        EXPECT_TRUE((AreContainersItemsEqual(std::vector<EntityID>{ 3, 11, 0, 1, 2, 4, 5, 6, 7, 9, 8, 10 }, entitiesInBFS)));
        EXPECT_TRUE((AreContainersItemsEqual(std::vector<EntityID>{ 3, 11, 0, 1, 2, 4, 5, 6, 7, 9, 8, 10 }, entitiesInDFS)));
      }
    }

    tree.Update(
      2,
      {
        { +2.0, +2.0, +2.0 },
        { +3.2, +3.2, +3.1 }
    });

    boxes[2] = {
      { +2.0, +2.0, +2.0 },
      { +2.6, +2.6, +2.2 }
    };
    tree.Update(2, boxes[2]);

    boxes[6] = {
      { -2.0, -2.0, -2.0 },
      { +2.0, +2.0, +2.0 }
    };
    tree.Update(6, boxes[6]);

    auto const oldbox2 = boxes[2];
    boxes[2] = {
      { -2.0, -2.0, -2.0 },
      { +0.0, +0.0, +0.0 }
    };
    tree.Update(2, oldbox2, boxes[2]);

    auto const oldbox4 = boxes[4];
    boxes[4] = {
      { +3.50, +3.50, +3.50 },
      { +3.75, +3.75, +3.75 }
    };
    tree.Update(4, oldbox4, boxes[4], boxes);

    boxes[8] = {
      {  +3.0,  +3.0, +3.75 },
      { +3.25, +3.25,  +4.0 }
    };
    tree.Update(8, boxes[8], boxes);

    auto const& nodes = tree.GetNodes();
    auto const entitiesInBFS = tree.GetEntitiesBreadthFirst();
    auto const entitiesInDFS = tree.GetEntitiesDepthFirst();

    EXPECT_EQ(11, nodes.size());
    EXPECT_TRUE((entitiesInBFS == std::vector<EntityID>{ 3, 11, 2, 0, 1, 6, 5, 7, 9, 8, 4, 10 }));
    EXPECT_TRUE((entitiesInDFS == std::vector<EntityID>{ 3, 11, 2, 0, 1, 6, 5, 7, 9, 8, 4, 10 }));
  }
} // namespace Box_InsertTest

namespace Box_EditTest
{
  TEST(Box_EditTest, Insert_IntoRoot_Successful)
  {
    auto constexpr vBoxL = array{
      BoundingBox1D{ 0.0, 1.0 },
      BoundingBox1D{ 1.0, 2.0 },
      BoundingBox1D{ 2.0, 3.0 },
      BoundingBox1D{ 3.0, 4.0 }
    };
    auto tree = DualtreeBox(vBoxL, 3, std::nullopt, 1);

    EXPECT_TRUE((tree.InsertIntoLeaf(4, BoundingBox1D{ 0, 4 }, InsertionMode::ExistingLeaf)));

    auto const& nodes = tree.GetNodes();
    EXPECT_EQ(nodes.size(), 7);
    EXPECT_TRUE((AreContainersItemsEqual(tree.GetNodeEntities(tree.GetRootNodeValue()), vector<EntityID>{ 4 })));
  }

  TEST(Box_EditTest, Insert_NonLeaf_Successful)
  {
    auto constexpr vBoxL = array{
      BoundingBox1D{ 0.0, 1.0 },
      BoundingBox1D{ 1.0, 2.0 },
      BoundingBox1D{ 2.0, 3.0 },
      BoundingBox1D{ 3.0, 4.0 }
    };
    auto tree = DualtreeBox(vBoxL, 3, std::nullopt, 1);

    EXPECT_TRUE((tree.InsertIntoLeaf(4, BoundingBox1D{ 3.5, 3.7 }, InsertionMode::ExistingLeaf)));

    auto const& nodes = tree.GetNodes();
    EXPECT_EQ(nodes.size(), 7);
    EXPECT_TRUE((AreContainersItemsEqual(tree.GetNodeEntities(tree.GetNodeValue(7)), vector<EntityID>{ 3, 4 })));
  }

  TEST(Box_EditTest, Insert_Leaf_Successful)
  {
    auto constexpr vBoxL = array{
      BoundingBox1D{ 0.0, 1.0 },
      BoundingBox1D{ 1.0, 2.0 },
      BoundingBox1D{ 2.0, 3.0 },
      BoundingBox1D{ 3.0, 4.0 }
    };
    auto tree = DualtreeBox(vBoxL, 3, std::nullopt, 1);

    EXPECT_TRUE((tree.InsertIntoLeaf(4, BoundingBox1D{ 3.5, 3.7 }, InsertionMode::LowestLeaf)));

    auto const& nodes = tree.GetNodes();
    EXPECT_EQ(nodes.size(), 8);
    EXPECT_TRUE((AreContainersItemsEqual(tree.GetNodeEntities(tree.GetNodeValue(15)), vector<EntityID>{ 4 })));
  }

  TEST(Box_EditTest, Insert_LeafBut1_Successful)
  {
    auto constexpr vBoxL = array{
      BoundingBox1D{ 0.0, 1.0 },
      BoundingBox1D{ 1.0, 2.0 },
      BoundingBox1D{ 2.0, 3.0 },
      BoundingBox1D{ 3.0, 4.0 }
    };
    auto tree = DualtreeBox(vBoxL, 3, std::nullopt, 1);

    EXPECT_TRUE((tree.InsertIntoLeaf(4, BoundingBox1D{ 1.0, 3.0 }, InsertionMode::LowestLeaf)));

    auto const& nodes = tree.GetNodes();
    auto const entitiesInBFS = tree.GetEntitiesBreadthFirst();
    auto const entitiesInDFS = tree.GetEntitiesDepthFirst();

    EXPECT_EQ(nodes.size(), 7);
    EXPECT_TRUE((AreContainersItemsEqual(entitiesInBFS, vector<EntityID>{ 4, 0, 1, 2, 3 })));
    EXPECT_TRUE((AreContainersItemsEqual(entitiesInDFS, vector<EntityID>{ 0, 1, 4, 2, 3 })));
  }

  TEST(Box_EditTest, Insert_LeafBut2_Successful)
  {
    auto constexpr vBoxL = array{
      BoundingBox1D{ 0.0, 1.0 },
      BoundingBox1D{ 1.0, 2.0 },
      BoundingBox1D{ 2.0, 3.0 },
      BoundingBox1D{ 3.0, 4.0 }
    };
    auto tree = DualtreeBox(vBoxL, 3, std::nullopt, 1);

    EXPECT_TRUE((tree.InsertIntoLeaf(4, BoundingBox1D{ 0.0, 2.0 }, InsertionMode::LowestLeaf)));

    auto const& nodes = tree.GetNodes();
    auto const entitiesInBFS = tree.GetEntitiesBreadthFirst();
    auto const entitiesInDFS = tree.GetEntitiesDepthFirst();

    EXPECT_EQ(nodes.size(), 7);
    EXPECT_TRUE((AreContainersItemsEqual(entitiesInBFS, vector<EntityID>{ 4, 0, 1, 2, 3 })));
    EXPECT_TRUE((AreContainersItemsEqual(entitiesInDFS, vector<EntityID>{ 4, 0, 1, 2, 3 })));
  }

  TEST(Box_EditTest, Complex)
  {
    auto constexpr vBoxL = array{
      BoundingBox1D{ 0.0, 1.0 },
      BoundingBox1D{ 1.0, 2.0 },
      BoundingBox1D{ 2.0, 3.0 },
      BoundingBox1D{ 3.0, 4.0 }
    };
    auto tree = DualtreeBox(vBoxL, 3, std::nullopt, 1);
    tree.InsertIntoLeaf(4, BoundingBox1D{ 3.5, 3.7 }, InsertionMode::ExistingLeaf);
    tree.InsertIntoLeaf(5, BoundingBox1D{ 3.5, 3.7 }, InsertionMode::LowestLeaf);
    tree.Erase(3);
    tree.Update(4, BoundingBox1D{ 3.8, 3.9 });
    tree.Update(0, vBoxL.front(), BoundingBox1D{ 3.8, 3.9 });
    tree.Update(1, vBoxL[1]);
  }
} // namespace Box_EditTest
