#include "pch.h"

using namespace OrthoTree;
using namespace OrthoTreeTestHelpers;
using std::array;
using std::vector;

namespace StaticTree
{
  TEST(StaticTree, OctreeMap_Create)
  {
    auto const points = std::unordered_map<index_t, Point3D>{
      { 1,   Point3D{ 0.0, 0.0, 0.0 } },
      { 2,   Point3D{ 1.0, 0.0, 0.0 } },
      { 3,   Point3D{ 2.0, 0.0, 0.0 } },
      { 4,   Point3D{ 3.0, 0.0, 0.0 } },
      { 5,   Point3D{ 4.0, 4.0, 4.0 } },

      { 6,   Point3D{ 0.5, 0.0, 0.0 } },
      { 7,  Point3D{ 0.25, 0.0, 0.0 } },
      { 8, Point3D{ 0.25, 0.25, 0.0 } },
      { 9, Point3D{ 0.125, 0.0, 0.0 } },
    };

    auto const octree = StaticOctreePointMap(points, std::nullopt, std::nullopt, 1);
  }
} // namespace StaticTree

namespace EntityAdapterFlexibilityTest
{
  // ============================================================
  // Test 1: Move-only entity with map-based container
  // Entity that wraps a geometry + payload, but is NOT copyable
  // ============================================================

  struct MoveOnlyEntity
  {
    Point3D position;
    std::unique_ptr<int> payload;

    MoveOnlyEntity() = default;
    MoveOnlyEntity(Point3D pos, int val)
    : position(pos)
    , payload(std::make_unique<int>(val))
    {}

    MoveOnlyEntity(MoveOnlyEntity&&) noexcept = default;
    MoveOnlyEntity& operator=(MoveOnlyEntity&&) noexcept = default;
    MoveOnlyEntity(MoveOnlyEntity const&) = delete;
    MoveOnlyEntity& operator=(MoveOnlyEntity const&) = delete;
  };

  struct MoveOnlyEntityAdapter : public PointEntityMapAdapter<Point3D, std::unordered_map<index_t, MoveOnlyEntity>>
  {
    using Base = PointEntityMapAdapter<Point3D, std::unordered_map<index_t, MoveOnlyEntity>>;

    static Point3D const& GetGeometry(MoveOnlyEntity const& entity) { return entity.position; }
    static Point3D const& GetGeometry(Base::Entity const& entity) { return entity.second.position; }
    static Point3D const& GetGeometry(Base::EntityContainerView entities, Base::EntityID entityID) { return entities.at(entityID).position; }
  };

  using MoveOnlyOctreeCore =
    OrthoTreeBase<MoveOnlyEntityAdapter, GeneralGeometryAdapterTemplate<3, Point3D, BoundingBox3D, Ray3D, Plane3D>, PointConfiguration<>>;

  using MoveOnlyOctreeContainer = OrthoTreeManaged<MoveOnlyOctreeCore>;

  // ============================================================
  // Test 2: Non-default-constructible EntityID (trivially copyable)
  // ============================================================

  struct StrongEntityID
  {
    uint32_t value;

    explicit constexpr StrongEntityID(uint32_t v) noexcept
    : value(v)
    {}

    constexpr bool operator==(StrongEntityID const&) const noexcept = default;
    constexpr auto operator<=>(StrongEntityID const&) const noexcept = default;
  };
  static_assert(std::is_trivially_copyable_v<StrongEntityID>);
  static_assert(!std::is_default_constructible_v<StrongEntityID>);

  struct StrongEntityIDHash
  {
    std::size_t operator()(StrongEntityID const& id) const noexcept { return std::hash<uint32_t>{}(id.value); }
  };

  struct StrongIDEntityAdapter
  : public PointEntityMapAdapter<Point3D, std::unordered_map<StrongEntityID, Point3D, StrongEntityIDHash>, std::unordered_map<StrongEntityID, Point3D, StrongEntityIDHash> const&, StrongEntityIDHash>
  {};

  using StrongIDOctreeCore =
    OrthoTreeBase<StrongIDEntityAdapter, GeneralGeometryAdapterTemplate<3, Point3D, BoundingBox3D, Ray3D, Plane3D>, PointConfiguration<>>;

  using StrongIDOctreeContainer = OrthoTreeManaged<StrongIDOctreeCore>;

  TEST(EntityAdapterFlexibilityTests, MoveOnlyEntity_CoreCreate)
  {
    auto entities = MoveOnlyEntityAdapter::EntityContainer{};
    entities.emplace(0, MoveOnlyEntity{ Point3D{ 0.0, 0.0, 0.0 }, 100 });
    entities.emplace(1, MoveOnlyEntity{ Point3D{ 1.0, 1.0, 1.0 }, 200 });
    entities.emplace(2, MoveOnlyEntity{ Point3D{ 2.0, 2.0, 2.0 }, 300 });
    entities.emplace(3, MoveOnlyEntity{ Point3D{ 3.0, 3.0, 3.0 }, 400 });
    entities.emplace(4, MoveOnlyEntity{ Point3D{ 4.0, 4.0, 4.0 }, 500 });

    MoveOnlyOctreeCore tree;
    tree.Create(entities, 3, std::nullopt, 2);

    auto const box = tree.GetBox();
    EXPECT_TRUE((true));
  }

  TEST(EntityAdapterFlexibilityTests, MoveOnlyEntity_CoreRangeSearch)
  {
    auto entities = MoveOnlyEntityAdapter::EntityContainer{};
    entities.emplace(0, MoveOnlyEntity{ Point3D{ 0.0, 0.0, 0.0 }, 10 });
    entities.emplace(1, MoveOnlyEntity{ Point3D{ 1.0, 1.0, 1.0 }, 20 });
    entities.emplace(2, MoveOnlyEntity{ Point3D{ 2.0, 2.0, 2.0 }, 30 });
    entities.emplace(3, MoveOnlyEntity{ Point3D{ 3.0, 3.0, 3.0 }, 40 });
    entities.emplace(4, MoveOnlyEntity{ Point3D{ 4.0, 4.0, 4.0 }, 50 });

    auto tree = MoveOnlyOctreeCore();
    MoveOnlyOctreeCore::Create(tree, entities, 3, std::nullopt, 2);

    auto const searchBox = BoundingBox3D{
      Point3D{ 0.5, 0.5, 0.5 },
      Point3D{ 2.5, 2.5, 2.5 }
    };
    auto results = tree.RangeSearch(searchBox, entities);

    EXPECT_EQ(std::size_t(2), results.size());
    std::ranges::sort(results);
    EXPECT_EQ(index_t(1), results[0]);
    EXPECT_EQ(index_t(2), results[1]);
  }

  TEST(EntityAdapterFlexibilityTests, MoveOnlyEntity_CoreInsert)
  {
    auto entities = MoveOnlyEntityAdapter::EntityContainer{};
    entities.emplace(0, MoveOnlyEntity{ Point3D{ 0.0, 0.0, 0.0 }, 10 });
    entities.emplace(1, MoveOnlyEntity{ Point3D{ 4.0, 4.0, 4.0 }, 20 });

    auto tree = MoveOnlyOctreeCore();
    MoveOnlyOctreeCore::Create(tree, entities, 3, std::nullopt, 2);

    entities.emplace(2, MoveOnlyEntity{ Point3D{ 2.0, 2.0, 2.0 }, 30 });
    bool inserted = tree.Insert(index_t(2), Point3D{ 2.0, 2.0, 2.0 }, entities);
    EXPECT_TRUE((inserted));

    auto const searchBox = BoundingBox3D{
      Point3D{ 1.5, 1.5, 1.5 },
      Point3D{ 2.5, 2.5, 2.5 }
    };
    auto results = tree.RangeSearch(searchBox, entities);
    EXPECT_EQ(std::size_t(1), results.size());
    EXPECT_EQ(index_t(2), results[0]);
  }

  TEST(EntityAdapterFlexibilityTests, MoveOnlyEntity_ContainerCreate)
  {
    auto entities = MoveOnlyEntityAdapter::EntityContainer{};
    entities.emplace(0, MoveOnlyEntity{ Point3D{ 0.0, 0.0, 0.0 }, 100 });
    entities.emplace(1, MoveOnlyEntity{ Point3D{ 1.0, 1.0, 1.0 }, 200 });
    entities.emplace(2, MoveOnlyEntity{ Point3D{ 2.0, 2.0, 2.0 }, 300 });
    entities.emplace(3, MoveOnlyEntity{ Point3D{ 3.0, 3.0, 3.0 }, 400 });
    entities.emplace(4, MoveOnlyEntity{ Point3D{ 4.0, 4.0, 4.0 }, 500 });

    auto tree = MoveOnlyOctreeContainer(std::move(entities), 3, std::nullopt, 2);

    auto const& data = tree.GetData();
    EXPECT_EQ(std::size_t(5), data.size());
  }

  TEST(EntityAdapterFlexibilityTests, MoveOnlyEntity_ContainerRangeSearch)
  {
    auto entities = MoveOnlyEntityAdapter::EntityContainer{};
    entities.emplace(0, MoveOnlyEntity{ Point3D{ 0.0, 0.0, 0.0 }, 10 });
    entities.emplace(1, MoveOnlyEntity{ Point3D{ 1.0, 1.0, 1.0 }, 20 });
    entities.emplace(2, MoveOnlyEntity{ Point3D{ 2.0, 2.0, 2.0 }, 30 });
    entities.emplace(3, MoveOnlyEntity{ Point3D{ 3.0, 3.0, 3.0 }, 40 });
    entities.emplace(4, MoveOnlyEntity{ Point3D{ 4.0, 4.0, 4.0 }, 50 });

    auto tree = MoveOnlyOctreeContainer(std::move(entities), 3, std::nullopt, 2);

    auto const searchBox = BoundingBox3D{
      Point3D{ 0.5, 0.5, 0.5 },
      Point3D{ 2.5, 2.5, 2.5 }
    };
    auto results = tree.RangeSearch(searchBox);

    EXPECT_EQ(std::size_t(2), results.size());
    std::ranges::sort(results);
    EXPECT_EQ(index_t(1), results[0]);
    EXPECT_EQ(index_t(2), results[1]);
  }

  TEST(EntityAdapterFlexibilityTests, StrongEntityID_CoreCreate)
  {
    auto entities = StrongIDEntityAdapter::EntityContainer{};
    entities.emplace(StrongEntityID{ 0 }, Point3D{ 0.0, 0.0, 0.0 });
    entities.emplace(StrongEntityID{ 1 }, Point3D{ 1.0, 1.0, 1.0 });
    entities.emplace(StrongEntityID{ 2 }, Point3D{ 2.0, 2.0, 2.0 });
    entities.emplace(StrongEntityID{ 3 }, Point3D{ 3.0, 3.0, 3.0 });
    entities.emplace(StrongEntityID{ 4 }, Point3D{ 4.0, 4.0, 4.0 });

    auto tree = StrongIDOctreeCore();
    StrongIDOctreeCore::Create(tree, entities, 3, std::nullopt, 2);

    auto const box = tree.GetBox();
    EXPECT_TRUE((true));
  }

  TEST(EntityAdapterFlexibilityTests, StrongEntityID_CoreRangeSearch)
  {
    auto entities = StrongIDEntityAdapter::EntityContainer{};
    entities.emplace(StrongEntityID{ 0 }, Point3D{ 0.0, 0.0, 0.0 });
    entities.emplace(StrongEntityID{ 1 }, Point3D{ 1.0, 1.0, 1.0 });
    entities.emplace(StrongEntityID{ 2 }, Point3D{ 2.0, 2.0, 2.0 });
    entities.emplace(StrongEntityID{ 3 }, Point3D{ 3.0, 3.0, 3.0 });
    entities.emplace(StrongEntityID{ 4 }, Point3D{ 4.0, 4.0, 4.0 });

    auto tree = StrongIDOctreeCore();
    StrongIDOctreeCore::Create(tree, entities, 3, std::nullopt, 2);

    auto const searchBox = BoundingBox3D{
      Point3D{ 0.5, 0.5, 0.5 },
      Point3D{ 2.5, 2.5, 2.5 }
    };
    auto results = tree.RangeSearch(searchBox, entities);

    EXPECT_EQ(std::size_t(2), results.size());
    std::ranges::sort(results);
    EXPECT_TRUE((StrongEntityID{ 1 } == results[0]));
    EXPECT_TRUE((StrongEntityID{ 2 } == results[1]));
  }

  TEST(EntityAdapterFlexibilityTests, StrongEntityID_CoreInsert)
  {
    auto entities = StrongIDEntityAdapter::EntityContainer{};
    entities.emplace(StrongEntityID{ 0 }, Point3D{ 0.0, 0.0, 0.0 });
    entities.emplace(StrongEntityID{ 1 }, Point3D{ 4.0, 4.0, 4.0 });

    auto tree = StrongIDOctreeCore();
    StrongIDOctreeCore::Create(tree, entities, 3, std::nullopt, 2);

    entities.emplace(StrongEntityID{ 2 }, Point3D{ 2.0, 2.0, 2.0 });
    bool inserted = tree.Insert(StrongEntityID{ 2 }, Point3D{ 2.0, 2.0, 2.0 }, entities);
    EXPECT_TRUE((inserted));

    auto const searchBox = BoundingBox3D{
      Point3D{ 1.5, 1.5, 1.5 },
      Point3D{ 2.5, 2.5, 2.5 }
    };
    auto results = tree.RangeSearch(searchBox, entities);
    EXPECT_EQ(std::size_t(1), results.size());
    EXPECT_TRUE((StrongEntityID{ 2 } == results[0]));
  }

  TEST(EntityAdapterFlexibilityTests, StrongEntityID_CoreErase)
  {
    auto entities = StrongIDEntityAdapter::EntityContainer{};
    entities.emplace(StrongEntityID{ 0 }, Point3D{ 0.0, 0.0, 0.0 });
    entities.emplace(StrongEntityID{ 1 }, Point3D{ 1.0, 1.0, 1.0 });
    entities.emplace(StrongEntityID{ 2 }, Point3D{ 2.0, 2.0, 2.0 });
    entities.emplace(StrongEntityID{ 3 }, Point3D{ 4.0, 4.0, 4.0 });

    auto tree = StrongIDOctreeCore();
    StrongIDOctreeCore::Create(tree, entities, 3, std::nullopt, 2);

    bool erased = tree.Erase(StrongEntityID{ 1 }, Point3D{ 1.0, 1.0, 1.0 });
    EXPECT_TRUE((erased));

    auto const searchBox = BoundingBox3D{
      Point3D{ 0.5, 0.5, 0.5 },
      Point3D{ 1.5, 1.5, 1.5 }
    };
    auto results = tree.RangeSearch(searchBox, entities);
    EXPECT_EQ(std::size_t(0), results.size());
  }

  TEST(EntityAdapterFlexibilityTests, StrongEntityID_ContainerCreate)
  {
    auto entities = StrongIDEntityAdapter::EntityContainer{};
    entities.emplace(StrongEntityID{ 0 }, Point3D{ 0.0, 0.0, 0.0 });
    entities.emplace(StrongEntityID{ 1 }, Point3D{ 1.0, 1.0, 1.0 });
    entities.emplace(StrongEntityID{ 2 }, Point3D{ 2.0, 2.0, 2.0 });
    entities.emplace(StrongEntityID{ 3 }, Point3D{ 3.0, 3.0, 3.0 });
    entities.emplace(StrongEntityID{ 4 }, Point3D{ 4.0, 4.0, 4.0 });

    auto tree = StrongIDOctreeContainer(entities, 3, std::nullopt, 2);

    auto const& data = tree.GetData();
    EXPECT_EQ(std::size_t(5), data.size());
  }

  TEST(EntityAdapterFlexibilityTests, StrongEntityID_ContainerAdd)
  {
    auto entities = StrongIDEntityAdapter::EntityContainer{};
    entities.emplace(StrongEntityID{ 0 }, Point3D{ 0.0, 0.0, 0.0 });
    entities.emplace(StrongEntityID{ 1 }, Point3D{ 4.0, 4.0, 4.0 });

    auto tree = StrongIDOctreeContainer(entities, 3, std::nullopt, 2);

    auto added = tree.Add(
      StrongIDEntityAdapter::Entity{
        StrongEntityID{ 2 },
        Point3D{ 2.0, 2.0, 2.0 }
    });
    EXPECT_TRUE((added.has_value()));

    auto const searchBox = BoundingBox3D{
      Point3D{ 1.5, 1.5, 1.5 },
      Point3D{ 2.5, 2.5, 2.5 }
    };
    auto results = tree.RangeSearch(searchBox);
    EXPECT_EQ(std::size_t(1), results.size());
    EXPECT_TRUE((StrongEntityID{ 2 } == results[0]));
  }

  TEST(EntityAdapterFlexibilityTests, StrongEntityID_ContainerErase)
  {
    auto entities = StrongIDEntityAdapter::EntityContainer{};
    entities.emplace(StrongEntityID{ 0 }, Point3D{ 0.0, 0.0, 0.0 });
    entities.emplace(StrongEntityID{ 1 }, Point3D{ 1.0, 1.0, 1.0 });
    entities.emplace(StrongEntityID{ 2 }, Point3D{ 2.0, 2.0, 2.0 });
    entities.emplace(StrongEntityID{ 3 }, Point3D{ 4.0, 4.0, 4.0 });

    auto tree = StrongIDOctreeContainer(entities, 3, std::nullopt, 2);

    bool erased = tree.Erase(StrongEntityID{ 1 });
    EXPECT_TRUE((erased));

    auto const searchBox = BoundingBox3D{
      Point3D{ 0.5, 0.5, 0.5 },
      Point3D{ 1.5, 1.5, 1.5 }
    };
    auto results = tree.RangeSearch(searchBox);
    EXPECT_EQ(std::size_t(0), results.size());
  }

  TEST(EntityAdapterFlexibilityTests, StrongEntityID_ContainerUpdate)
  {
    auto entities = StrongIDEntityAdapter::EntityContainer{};
    entities.emplace(StrongEntityID{ 0 }, Point3D{ 0.0, 0.0, 0.0 });
    entities.emplace(StrongEntityID{ 1 }, Point3D{ 1.0, 1.0, 1.0 });
    entities.emplace(StrongEntityID{ 2 }, Point3D{ 4.0, 4.0, 4.0 });

    auto tree = StrongIDOctreeContainer(entities, 3, std::nullopt, 2);

    bool updated = tree.Update(
      {
        StrongEntityID{ 1 },
        Point3D{ 3.0, 3.0, 3.0 }
    });
    EXPECT_TRUE((updated));

    auto const oldSearchBox = BoundingBox3D{
      Point3D{ 0.5, 0.5, 0.5 },
      Point3D{ 1.5, 1.5, 1.5 }
    };
    auto oldResults = tree.RangeSearch(oldSearchBox);
    EXPECT_EQ(std::size_t(0), oldResults.size());

    auto const newSearchBox = BoundingBox3D{
      Point3D{ 2.5, 2.5, 2.5 },
      Point3D{ 3.5, 3.5, 3.5 }
    };
    auto newResults = tree.RangeSearch(newSearchBox);
    EXPECT_EQ(std::size_t(1), newResults.size());
    EXPECT_TRUE((StrongEntityID{ 1 } == newResults[0]));
  }

  TEST(EntityAdapterFlexibilityTests, StrongEntityID_ContainerPickSearch)
  {
    auto entities = StrongIDEntityAdapter::EntityContainer{};
    entities.emplace(StrongEntityID{ 10 }, Point3D{ 1.0, 1.0, 1.0 });
    entities.emplace(StrongEntityID{ 20 }, Point3D{ 2.0, 2.0, 2.0 });
    entities.emplace(StrongEntityID{ 30 }, Point3D{ 3.0, 3.0, 3.0 });

    auto tree = StrongIDOctreeContainer(entities, 3, std::nullopt, 2);

    auto results = tree.PickSearch(Point3D{ 2.0, 2.0, 2.0 }, 0.1);
    EXPECT_EQ(std::size_t(1), results.size());
    EXPECT_TRUE((StrongEntityID{ 20 } == results[0]));
  }
} // namespace EntityAdapterFlexibilityTest

namespace DynamicVsStaticTest
{
  template<dim_t DIM, bool IS_POINT, bool IS_LOOSE>
  void _compare()
  {
    using PointD = PointND<DIM>;
    using BoxD = BoundingBoxND<DIM>;
    using EntityGeometry = std::conditional_t<IS_POINT, PointD, BoxD>;

    auto const domain = BoxD{
      {  0.0,  0.0,  0.0 },
      { 10.0, 10.0, 10.0 }
    };
    auto const maxDepth = depth_t{ 10 };
    auto const maxElement = std::size_t{ 2 };

    auto entities = vector<EntityGeometry>();
    if constexpr (IS_POINT)
    {
      entities = {
        PointD{ 1.0, 1.0, 1.0 },
        PointD{ 1.1, 1.1, 1.1 },
        PointD{ 5.0, 5.0, 5.0 },
        PointD{ 9.0, 9.0, 9.0 },
        PointD{ 9.1, 9.1, 9.1 }
      };
    }
    else
    {
      entities = {
        BoxD{ { 1.0, 1.0, 1.0 }, { 2.0, 2.0, 2.0 } },
        BoxD{ { 1.1, 1.1, 1.1 }, { 2.1, 2.1, 2.1 } },
        BoxD{ { 4.5, 4.5, 4.5 }, { 5.5, 5.5, 5.5 } },
        BoxD{ { 8.0, 8.0, 8.0 }, { 9.0, 9.0, 9.0 } },
        BoxD{ { 8.1, 8.1, 8.1 }, { 9.1, 9.1, 9.1 } }
      };
    }

    using TreeDynamic = std::conditional_t<IS_POINT, OrthoTreePointND<DIM>, OrthoTreeBoxND<DIM, IS_LOOSE>>;
    using TreeStatic = std::conditional_t<IS_POINT, StaticOrthoTreePointND<DIM>, StaticOrthoTreeBoxND<DIM, IS_LOOSE>>;

    auto treeDynamic = TreeDynamic(entities, maxDepth, domain, maxElement);
    auto treeStatic = TreeStatic(entities, maxDepth, domain, maxElement);

    EXPECT_EQ(treeDynamic.GetNodeCount(), treeStatic.GetNodeCount());
    EXPECT_TRUE((treeDynamic.IsEqualTo(treeStatic)));
    EXPECT_TRUE((treeStatic.IsEqualTo(treeDynamic)));

    if (entities.size() > 0)
    {
      auto entitiesModified = entities;
      if constexpr (IS_POINT)
        entitiesModified[0][0] += 2.0;
      else
        entitiesModified[0].Min[0] -= 1.5;

      auto treeStaticModified = TreeStatic(entitiesModified, maxDepth, domain, maxElement);
      EXPECT_FALSE((treeDynamic.IsEqualTo(treeStaticModified)));
    }
  }

  TEST(DynamicVsStaticTest, ComparisonTest_MBR_NotLoose)
  {
    _compare<3, false, false>();
  }

  TEST(DynamicVsStaticTest, ComparisonTest_MBR_Loose)
  {
    _compare<3, false, true>();
  }
} // namespace DynamicVsStaticTest

namespace DynamicHashCoreInsertTest
{
  TEST(DynamicHashCoreInsertTest, CoreInsert_SpanOfGeometry_2D)
  {
    using Tree = OrthoTreeBoxND<2>;
    using Box = BoundingBoxND<2>;

    auto core = Tree();
    core.Init(
      Box{
        {  0.0,  0.0 },
        { 10.0, 10.0 }
      },
      5,
      2);

    auto const entities = std::vector<Box>{
      { { 1.0, 1.0 }, { 2.0, 2.0 } },
      { { 3.0, 3.0 }, { 4.0, 4.0 } }
    };

    auto const existing = std::vector<Box>{};
    bool success = core.Insert(std::span(entities), std::span(existing));
    EXPECT_TRUE((success));
    EXPECT_EQ(size_t(2), core.GetEntitiesBreadthFirst().size());
  }

  TEST(DynamicHashCoreInsertTest, CoreInsert_VectorOfPair_3D)
  {
    using Tree = OrthoTreeBoxND<3>;
    using Box = BoundingBoxND<3>;
    using EntityID = Tree::EntityID;

    auto core = Tree();
    core.Init(
      Box{
        {  0.0,  0.0,  0.0 },
        { 10.0, 10.0, 10.0 }
      },
      5,
      2);

    auto const newEntities = std::vector<std::pair<EntityID, Box>>{
      { 1, { { 1.0, 1.0, 1.0 }, { 2.0, 2.0, 2.0 } } },
      { 0, { { 3.0, 3.0, 3.0 }, { 4.0, 4.0, 4.0 } } }
    };

    auto const existing = std::vector<Box>{};
    bool success = core.Insert(newEntities, std::span(existing));
    EXPECT_TRUE((success));
    EXPECT_EQ(size_t(2), core.GetEntitiesBreadthFirst().size());
  }

  TEST(DynamicHashCoreInsertTest, CoreInsert_VectorOfEntityStruct_6D)
  {
    using Tree = OrthoTreePointND<6>;
    using Point = PointND<6>;
    using EntityID = Tree::EntityID;

    auto core = Tree();
    core.Init(
      BoundingBoxND<6>{
        {  0.0,  0.0,  0.0,  0.0,  0.0,  0.0 },
        { 10.0, 10.0, 10.0, 10.0, 10.0, 10.0 }
      },
      5,
      2);

    struct MyEntity
    {
      EntityID id;
      Point pt;
    };

    auto const newEntities = std::vector<MyEntity>{
      { 5, { 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 } },
      { 6, { 3.0, 3.0, 3.0, 3.0, 3.0, 3.0 } }
    };

    auto const existing = std::vector<Point>{};
    bool success = core.Insert(newEntities, std::span(existing));
    EXPECT_TRUE((success));
    EXPECT_EQ(size_t(2), core.GetEntitiesBreadthFirst().size());
  }

  TEST(DynamicHashCoreInsertTest, CoreInsert_SpanOfGeometry_12D)
  {
    using Tree = OrthoTreePointND<12>;
    using Point = PointND<12>;

    auto core = Tree();
    core.Init(
      BoundingBoxND<12>{
        {  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0 },
        { 10.0, 10.0, 10.0, 10.0, 10.0, 10.0, 10.0, 10.0, 10.0, 10.0, 10.0, 10.0 }
      },
      5,
      2);

    auto const entities = std::vector<Point>{
      { 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 },
      { 3.0, 3.0, 3.0, 3.0, 3.0, 3.0, 3.0, 3.0, 3.0, 3.0, 3.0, 3.0 }
    };

    auto const existing = std::vector<Point>{};
    bool success = core.Insert(std::span(entities), std::span(existing));
    EXPECT_TRUE((success));
    EXPECT_EQ(size_t(2), core.GetEntitiesBreadthFirst().size());
  }
} // namespace DynamicHashCoreInsertTest

namespace OrthoTreeManagedAddTest
{
  TEST(OrthoTreeManagedAddTest, ContainerAdd_SpanOfGeometry_2D)
  {
    using Container = OrthoTreeBoxManagedND<2>;
    using Box = BoundingBoxND<2>;

    auto container = Container();
    container.Init(
      Box{
        {  0.0,  0.0 },
        { 10.0, 10.0 }
      },
      5,
      2);

    auto const entities = std::vector<Box>{
      { { 1.0, 1.0 }, { 2.0, 2.0 } },
      { { 3.0, 3.0 }, { 4.0, 4.0 } }
    };

    container.Add(std::span(entities), SeqExec{});
    EXPECT_EQ(size_t(2), container.GetEntitiesBreadthFirst().size());
  }

  TEST(OrthoTreeManagedAddTest, ContainerAdd_VectorOfGeometry_3D)
  {
    using Container = OrthoTreeBoxManagedND<3>;
    using Box = BoundingBoxND<3>;

    auto container = Container();
    container.Init(
      Box{
        {  0.0,  0.0,  0.0 },
        { 10.0, 10.0, 10.0 }
      },
      5,
      2);

    auto const entities = std::vector<Box>{
      { { 1.0, 1.0, 1.0 }, { 2.0, 2.0, 2.0 } },
      { { 3.0, 3.0, 3.0 }, { 4.0, 4.0, 4.0 } }
    };

    container.Add(entities, SeqExec{});
    EXPECT_EQ(size_t(2), container.GetEntitiesBreadthFirst().size());
  }

  TEST(OrthoTreeManagedAddTest, ContainerAdd_VectorOfGeometry_6D)
  {
    using Container = OrthoTreePointManagedND<6>;
    using Point = PointND<6>;

    auto container = Container();
    container.Init(
      BoundingBoxND<6>{
        {  0.0,  0.0,  0.0,  0.0,  0.0,  0.0 },
        { 10.0, 10.0, 10.0, 10.0, 10.0, 10.0 }
      },
      5,
      2);

    auto const entities = std::vector<Point>{
      { 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 },
      { 3.0, 3.0, 3.0, 3.0, 3.0, 3.0 }
    };

    container.Add(entities, SeqExec{});
    EXPECT_EQ(size_t(2), container.GetEntitiesBreadthFirst().size());
  }

  TEST(OrthoTreeManagedAddTest, ContainerAdd_SpanOfGeometry_12D)
  {
    using Container = OrthoTreePointManagedND<12>;
    using Point = PointND<12>;

    auto container = Container();
    container.Init(
      BoundingBoxND<12>{
        {  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0 },
        { 10.0, 10.0, 10.0, 10.0, 10.0, 10.0, 10.0, 10.0, 10.0, 10.0, 10.0, 10.0 }
      },
      5,
      2);

    auto const entities = std::vector<Point>{
      { 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 },
      { 3.0, 3.0, 3.0, 3.0, 3.0, 3.0, 3.0, 3.0, 3.0, 3.0, 3.0, 3.0 }
    };

    container.Add(std::span(entities), SeqExec{});
    EXPECT_EQ(size_t(2), container.GetEntitiesBreadthFirst().size());
  }
} // namespace OrthoTreeManagedAddTest
