# Documentation

## Introduction
OrthoTree is a generic geometric data structure that represents coordinate space recursively. For storage and structural representation, it only handles Axis-Aligned Bounding Boxes (AABBs) and points. Other geometries, such as rays and planes, are used for search and querying operations.

For ease of use, the library provides several aliases ([core/aliases.h](../include/orthotree/core/aliases.h)) for common configurations (e.g., `OctreePoint`, `OctreeBox`, `QuadtreePoint`, `QuadtreeBox`).

## Code structure
* **octree.h**: Main header file. 
* **Static Core** (`ot_static_linear_core.h`): An immutable, linear memory layout tree. It is built once and cannot be modified (no insertion/removal after build), but it provides the highest cache coherency and fastest query performance. Best for static environments and one-time spatial setups. It aims minimal memory footprint. E.g: `StaticOctreeBox`
* **Dynamic Core** (`ot_dynamic_hash_core.h`): A mutable tree structure based on a hashing approach. It allows adding, removing, and updating entities dynamically at runtime. Best for changing environments, simulators, and physics engines. E.g: `DynamicOctreeBox`/`OctreeBox`
* **Query** layer (`ot_query.h`): It is an additional layer on top of the cores. Contains the geometric search algorithms that operate on the trees, such as Range search, Collision detection, K-Nearest Neighbors (KNN), and Raycasting. With the proper interface it can be used for other types of Cores (e.g., BVH, RTree, etc.).
* **Managed** (`ot_managed.h`): High-level wrapper classes (e.g., `OctreePointM`) that manage both the tree structure and the user's entity storage. They provide a simpler, object-oriented API for interacting with the tree.
* **Adapters** (`adapters/*.h`): Adapters map user-defined or third-party geometric types (vector, box) to the generic concepts required by OrthoTree. Ready-made adapters exist for GLM, Eigen, Unreal, CGAL, etc.
* **`core/`**: Strongly connected Core functionalities for internal use.
* **`detail/`**: Other utilities.

> [!CAUTION]
> **Core types**: Tree cores do not store `Entities`, just `EntityID`s. Usage requires attention to the proper updating order: if element geometries are modified before calling `Update` or `Insert`, the overflowing mechanism may find elements that do not geographically fit in the current node.
> **Typical issues**: Modifying an element's geometry without updating the tree, forgetting to add an element, or overwriting an existing entity's geometry before it is removed from the tree.
> Debug asserts will try to guide you in the case of misuse, with messages like: *"Existing entities are out-of-sync with the tree. Client code may have modified the geometry of the existing entities without updating the tree."*
> **Recommendation**: It is safer to use the `Managed` wrapper types, which manage this fragility automatically.

### Design notes
* **Error handling**: OrthoTree is designed as a `noexcept`-only library. All public interfaces are marked `noexcept`, and no exceptions are thrown internally. If memory allocation fails (e.g., `std::bad_alloc`), the program will terminate. User-provided callbacks (entity testers, traversal procedures, priority calculators) **must not throw exceptions**; doing so will result in `std::terminate` being called.
Errors are typically communicated through `std::optional` wrapped results or error capable structs as return values.
* Header only implementation.
* Node interface is not public by default.

## Adapt to your use-case

OrthoTree is a generic data structure that can be adapted to work with any geometrical system and any entity type. The two main adaptation points are: defining how OrthoTree identifies and accesses your **entities**, and mapping your **geometry types** to the OrthoTree concepts through an adapter.

### Entity adapting

An entity typically consists of its geometry (e.g., a bounding box) and potentially some metadata (e.g., user flags, pointers, identifiers). OrthoTree operates purely on the structural geometry, whereas the client code uses mathematical or physical attributes associated with the entity. 

To bridge this gap, OrthoTree tracks entities using an `EntityID`. The client stores the metadata directly, and uses the `EntityID` returned by queries to access it.
* `EntityID` must be *trivially copyable*, but it does not need to be *default constructible*. It is typically a fundamental integer type or a lightweight handle, but it can be any type that can be *hash-able* with a client-provided `hash` function. *It is recommended to use the smallest possible type to minimize memory usage.*
* The `Entity` itself must be *movable* if the `OrthoTreeManaged` is used, enabling construction and reorganization of the tree internals.

The `EntityAdapter` also may contain container mutation functions. These are used by `OrthoTreeManaged` internally to know how to insert, append, or assign elements into the specific custom container type used by the client.

For a full reference implementation, see [entity_adapter.h](../include/orthotree/core/entity_adapter.h) which defines `EntityAdapterDefault`, `PointEntitySpanAdapter`, `BoxEntitySpanAdapter`, and map-based variants.

`ENTITY_ID_STRATEGY` [EntityIdStrategy]: Entity adapter is designed for both map-based and span-based entity storage, latter does not require keying. 
* `ContiguousIndex`: [default for span-based storage] Index-based ID, and the tree actively maintains the continuity of indexes (e.g. by decrementing the EntityID of the following elements on removal). Index-based storage is typically implemented using `std::vector` or `std::span`.
* `StableIndex`: Index-based ID, contiguous upon creation, but removals may create "holes", keeping remaining indexes stable. Index-based storage is typically implemented using `std::vector` or `std::span`, but indexing in the tree will not be mutated on removal.
* `EntityKeyed`: [default for map-based storage] The entity itself contains the ID. (e.g., std::pair<EntityID, Entity> in an std::unordered_map entity container)

<details><summary>Complex adapter example</summary>


```C++
    // Non-default constructible geometry
    struct RigidPoint
    {
      float x, y;
      RigidPoint() = delete;
      constexpr RigidPoint(float x, float y)
      : x(x)
      , y(y)
      {}
    };

    struct RigidBox
    {
      RigidPoint Min, Max;
      RigidBox() = delete;
      constexpr RigidBox(RigidPoint min, RigidPoint max)
      : Min(min)
      , Max(max)
      {}
    };

    // Complex EntityID
    struct GameObjectID
    {
      int teamId;
      int unitId;
      auto operator<=>(GameObjectID const&) const = default;
    };

    struct GameObjectIDHash
    {
      std::size_t operator()(GameObjectID const& id) const noexcept { return std::hash<int>()(id.teamId) ^ (std::hash<int>()(id.unitId) << 1); }
    };

    // Abstract Move-Only Entity
    class IGameObject
    {
    public:
      virtual ~IGameObject() = default;
      virtual RigidBox const& GetBounds() const = 0;
    };

    class Enemy : public IGameObject
    {
      RigidBox bounds;
      // +visibility, etc

    public:
      Enemy(RigidBox b)
      : bounds(std::move(b))
      {}
      Enemy(Enemy const&) = delete;
      Enemy& operator=(Enemy const&) = delete;
      Enemy(Enemy&&) = default;
      Enemy& operator=(Enemy&&) = default;

      virtual RigidBox const& GetBounds() const override { return bounds; }
    };

    // Geometry adapter
    struct CustomRigidGeometryAdapter
    {
      using Scalar = float;
      using FloatScalar = float;
      using Vector = RigidPoint;
      using Box = RigidBox;
      struct Ray
      {
        RigidPoint Origin;
        RigidPoint Direction;
      };
      struct Plane
      {
        float OrigoDistance;
        RigidPoint Normal;
      };

      static constexpr OrthoTree::dim_t DIMENSION_NO = 2;
      static constexpr FloatScalar BASE_TOLERANCE = std::numeric_limits<FloatScalar>::epsilon() * FloatScalar(10);

      static constexpr Vector MakePoint() noexcept { return RigidPoint(0.0f, 0.0f); };
      static constexpr Box MakeBox() noexcept { return RigidBox(RigidPoint(0.0f, 0.0f), RigidPoint(0.0f, 0.0f)); };

      static constexpr float GetPointC(RigidPoint const& pt, OrthoTree::dim_t i) noexcept { return i == 0 ? pt.x : pt.y; }
      static constexpr void SetPointC(RigidPoint& pt, OrthoTree::dim_t i, float v) noexcept
      {
        if (i == 0)
          pt.x = v;
        else
          pt.y = v;
      }

      static constexpr void SetBoxMinC(RigidBox& box, OrthoTree::dim_t i, float v) noexcept { SetPointC(box.Min, i, v); }
      static constexpr void SetBoxMaxC(RigidBox& box, OrthoTree::dim_t i, float v) noexcept { SetPointC(box.Max, i, v); }
      static constexpr float GetBoxMinC(RigidBox const& box, OrthoTree::dim_t i) noexcept { return GetPointC(box.Min, i); }
      static constexpr float GetBoxMaxC(RigidBox const& box, OrthoTree::dim_t i) noexcept { return GetPointC(box.Max, i); }

      static constexpr Vector const& GetRayDirection(Ray const& ray) noexcept { return ray.Direction; }
      static constexpr Vector const& GetRayOrigin(Ray const& ray) noexcept { return ray.Origin; }

      static constexpr Vector const& GetPlaneNormal(Plane const& plane) noexcept { return plane.Normal; }
      static constexpr Scalar GetPlaneOrigoDistance(Plane const& plane) noexcept { return plane.OrigoDistance; }
    };

    using CustomGeometryAdapter = OrthoTree::GeneralGeometryAdapter<CustomRigidGeometryAdapter>;

    // EntityAdapter
    using GameObjectContainer = std::unordered_map<GameObjectID, std::unique_ptr<IGameObject>, GameObjectIDHash>;
    using GameObjectContainerView = GameObjectContainer const&;

    struct GameObjectEntityAdapter
    {
      using EntityContainer = GameObjectContainer;
      using EntityContainerView = GameObjectContainerView;
      using Entity = std::pair<GameObjectID const, std::unique_ptr<IGameObject>>;
      using EntityID = GameObjectID;
      using Geometry = RigidBox;
      using Hash = GameObjectIDHash;

      static constexpr OrthoTree::GeometryType GEOMETRY_TYPE = OrthoTree::GeometryType::Box;
      static constexpr OrthoTree::EntityIdStrategy ENTITY_ID_STRATEGY = OrthoTree::EntityIdStrategy::EntityKeyed;

      static Entity const& GetEntity(EntityContainerView entities, EntityID const& entityID) noexcept { return *entities.find(entityID); }
      static constexpr EntityID GetEntityID(Entity const& entity) noexcept { return entity.first; }
      static constexpr EntityID GetEntityID(EntityContainerView /*entities*/, Entity const& entity) noexcept { return entity.first; }

      static Geometry const& GetGeometry(EntityContainerView entities, EntityID const& entityID) noexcept
      {
        return entities.at(entityID)->GetBounds();
      }
      static constexpr Geometry const& GetGeometry(Entity const& entity) noexcept { return entity.second->GetBounds(); }

      static std::size_t GetEntityCount(EntityContainerView entities) noexcept { return entities.size(); }

      static EntityID Insert(EntityContainer& entities, Entity&& entity) noexcept
      {
        auto [it, _] = entities.emplace(std::move(entity));
        return it->first;
      }

      static void Erase(EntityContainer& entities, EntityID entityID) noexcept { entities.erase(entityID); }

      static auto Exchange(EntityContainer& entities, EntityID entityID, Entity&& entity) noexcept
      {
        auto it = entities.find(entityID);
        return Entity{ entityID, std::exchange(it->second, std::forward<decltype(entity.second)>(entity.second)) };
      }

      static void Clear(EntityContainer& entities) noexcept { entities.clear(); }

      static bool Contains(EntityContainer& entities, EntityID entityID) noexcept { return entities.contains(entityID); }
    };


    using QuadtreeGameObjects = OrthoTree::OrthoTreeBase<GameObjectEntityAdapter, CustomGeometryAdapter, OrthoTree::BoxConfiguration<true>>;
    using QuadtreeGameObjectsM = OrthoTreeManaged<QuadtreeGameObjects>;

```
</details>


### Geometrical system adapting

A geometrical system simply represents the mathematical types (Vector/Point, Box, Ray, etc.) you use, which is strictly separate from the custom entities that hold metadata. You must map your geometry types to the generic concepts required by OrthoTree through an Adapter.

Geometrical elements should be constructible using user-provided functions (e.g., as defined in the `BaseGeometryAdapterConcept`).

For an integration example using standard bounding boxes and custom points, you can review the adapters (E.g., [adapters/xyz.h](../include/orthotree/adapters/xyz.h)).

```C++
#include "orthotree/adapters/general.h"
#include "orthotree/adapters/octree.h"


using xy_geometry_type = BasicTypesXYZ::float_t;
using XYPoint2D = BasicTypesXYZ::Point2D;
using XYBoundingBox2D = BasicTypesXYZ::BoundingBox2D;
using XYRay2D = BasicTypesXYZ::Ray2D;
using XYPlane2D = BasicTypesXYZ::Plane2D;

struct XYBaseGeometryAdapter
{
  using Scalar = xy_geometry_type;
  using FloatScalar = xy_geometry_type;
  using Vector = XYPoint2D;
  using Box = XYBoundingBox2D;
  using Ray = XYRay2D;
  using Plane = XYPlane2D;

  static constexpr dim_t DIMENSION_NO = 2;
  static constexpr FloatScalar BASE_TOLERANCE = std::numeric_limits<FloatScalar>::epsilon() * FloatScalar(10);

  static constexpr XYPoint2D MakePoint() noexcept { return {}; };
  static constexpr XYBoundingBox2D MakeBox() noexcept { return {}; };

  static constexpr xy_geometry_type GetPointC(XYPoint2D const& pt, dim_t dimensionID)
  {
    switch (dimensionID)
    {
    case 0: return pt.x;
    case 1: return pt.y;
    default: assert(false); std::terminate();
    }
  }

  static constexpr void SetPointC(XYPoint2D& pt, dim_t dimensionID, xy_geometry_type value)
  {
    switch (dimensionID)
    {
    case 0: pt.x = value; break;
    case 1: pt.y = value; break;
    default: assert(false); std::terminate();
    }
  }


  static constexpr void SetBoxMinC(XYBoundingBox2D& box, dim_t dimensionID, xy_geometry_type value) { 
    SetPointC(box.Min, dimensionID, value); 
  }
  static constexpr void SetBoxMaxC(XYBoundingBox2D& box, dim_t dimensionID, xy_geometry_type value) { 
    SetPointC(box.Max, dimensionID, value); 
  }
  static constexpr xy_geometry_type GetBoxMinC(XYBoundingBox2D const& box, dim_t dimensionID) { 
    return GetPointC(box.Min, dimensionID); 
  }
  static constexpr xy_geometry_type GetBoxMaxC(XYBoundingBox2D const& box, dim_t dimensionID) { 
    return GetPointC(box.Max, dimensionID); 
  }

  static constexpr XYPoint2D const& GetRayDirection(XYRay2D const& ray) noexcept { return ray.Heading; }
  static constexpr XYPoint2D const& GetRayOrigin(XYRay2D const& ray) noexcept { return ray.BasePoint; }

  static constexpr XYPoint2D const& GetPlaneNormal(XYPlane2D const& plane) noexcept { return plane.Normal; }
  static constexpr xy_geometry_type GetPlaneOrigoDistance(XYPlane2D const& plane) noexcept { return plane.OrigoDistance; }
};

using XYGeometryAdapter = GeneralGeometryAdapter<XYBaseGeometryAdapter>;


using QuadtreePoint = OrthoTreeBase<PointEntitySpanAdapter<XYPoint2D>, XYGeometryAdapter, PointConfiguration<>>;

template<bool IS_LOOSE_TREE = true>
using QuadtreeBox = OrthoTreeBase<BoxEntitySpanAdapter<XYBoundingBox2D>, XYGeometryAdapter, BoxConfiguration<IS_LOOSE_TREE>>;

```

Default geometrical base elements
```C++
  using BaseGeometryType = double;
  using Vector1D = OrthoTree::VectorND<1, BaseGeometryType>;
  using Vector2D = OrthoTree::VectorND<2, BaseGeometryType>;
  using Vector3D = OrthoTree::VectorND<3, BaseGeometryType>;
  using Point1D = OrthoTree::PointND<1, BaseGeometryType>;
  using Point2D = OrthoTree::PointND<2, BaseGeometryType>;
  using Point3D = OrthoTree::PointND<3, BaseGeometryType>;
  using BoundingBox1D = OrthoTree::BoundingBoxND<1, BaseGeometryType>;
  using BoundingBox2D = OrthoTree::BoundingBoxND<2, BaseGeometryType>;
  using BoundingBox3D = OrthoTree::BoundingBoxND<3, BaseGeometryType>;
  using Ray2D = OrthoTree::RayND<2, BaseGeometryType>;
  using Ray3D = OrthoTree::RayND<3, BaseGeometryType>;
  using Plane2D = OrthoTree::PlaneND<2, BaseGeometryType>;
  using Plane3D = OrthoTree::PlaneND<3, BaseGeometryType>;
```


### Included geometry adapters
The following geometry adapters are included in the library:
* `general.h`: General geometry adapter.
* `eigen.h`: Eigen geometry adapter. Use `ORTHOTREE_EIGEN_INCLUDE` to specify the path to Eigen headers.
* `glm.h`: GLM geometry adapter. Use `ORTHOTREE_GLM_INCLUDE` to specify the path to GLM headers.
* `cgal.h`: CGAL geometry adapter. Use `ORTHOTREE_CGAL_INCLUDE` if you want to manually include CGAL headers.
* `boost.h`: Boost geometry adapter. Use `ORTHOTREE_BOOST_INCLUDE` to specify the path to Boost headers.
* `unreal.h`: Unreal Engine geometry adapter. Use `ORTHOTREE_UNREAL_INCLUDE` to specify the path to Unreal Engine headers.

#### int-based Scalar
The int-based Scalar requires Float-cast on every geometrical checks, therefore its usage could bring some performance drawbacks.
E.g. Let assume the model is based on some kind if pixel representation: Meanwhile, in a range search pixel-based tolerance would perfectly fine, in a ray search would not. Therefore the most robust solution is a float-based tolerance handling with float casting the underlying Scalar type.

#### float-based Scalar
When using standard floating-point coordinates, the library handles potential precision issues internally. Specifically, if `maxDepthID` is larger than 16, the system automatically resolves numerical inaccuracies during Morton code calculation by casting to `double`. As a client, you should only be aware of the minor performance impact of this internal casting, rather than worrying about spatial precision loss during grid quantization.

#### Tolerance handling
The library defines a `BASE_TOLERANCE` trait in the adapters to handle the standard epsilon needed for safe floating-point arithmetic. If you are using integer coordinates, you must still specify a corresponding floating-point type (`FloatScalar`) and a `BASE_TOLERANCE` for precise raycasting and overlap checks.

## Settings and Configuration

### Configuration
`ALLOW_OUT_OF_SPACE_INSERTION`: [default: true] If true, the tree will allow insertion of entities that are outside the tree's bounding box. This can be useful for dynamic datasets where the tree's bounding box needs to grow with the data. Out of space entities are stored in the root. Real size of the tree will be adjusted due to the elements. If false, insertion will fail if the entity is outside the tree's bounding box.

`LOOSE_FACTOR`: [default: 2.0] **Looseness factor**, a loose tree enlarges each node's bounding box by a factor, so that an entity can fit entirely within a single node even if it straddles a split boundary. `1.0` means no enlargement (regular, tight octree). `2.0` (the recommended value for box trees with ray-picking) doubles each node's size. For AABB-based trees 2.0 loose tree is the default.
   * *Advantage*: faster insertion and better spatial locality for overlapping geometries, because the elements could be inserted into the level of the tree where they fit by size, not by position. 
   * *Disadvantage*: during queries, more nodes may be visited due to overlapping node boundaries, increasing query cost. To counteract this, **MBR** *node geometry storage* could be used. See `NODE_GEOMETRY_STORAGE`.

`DEFAULT_TARGET_ELEMENT_NUM_IN_NODES`: Target element count in the nodes. It controls the node overflow behavior. 
  * Small number causes deeper tree, slowing down the creation and the searches
  * Large number causes too many tests during the searches.

`NODE_GEOMETRY_STORAGE`: Geometry of the nodes can be compacted.
  * `None`:     Node does not contain geometry information, every geometry queries requires its calculation.
  * `MinPoint`: Node contains only the min-point. Node sizes are stored level-wise.
  * `MBR`:      Node contains the min-point and the size of the *minimal bounding rectangle*. It requires calculation on every insertion. 
                Entity removal does not shrink it. Recommended for Loose trees, where the node overlapping would be large. MBR continuously grows until the size limit of the node.

`MAX_ALLOWED_DEPTH_ID`: [default: 19] Compile-time hard depth limit of the tree. It determines type of the LocationID (uint32_t or uint64_t) and certain internal simplifications. Runtime `maxDepthID` is also still available to customize the same tree type for the task.

`USE_REVERSE_MAPPING`: [default: true] **Reverse mapping**, when enabled, maintains a secondary map from each `EntityID` directly to its containing node. This makes `Erase()` and `Update()` O(1) instead of O(N), at the cost of additional memory. Recommended for dynamic datasets with frequent removals or large entity number. Static tree ignores this flag. It has large impact on creation/insertion time and memory usage.
> [!TIP]
> If the original entity is still available at the time of the `Update()`/`Erase()` (`Managed` types have this capability), it can be used to find the related node in O(1) time, it may be more advantageous for the overall system performance than the reverse-mapping.

`UMapNodeContainer` / `MapNodeContainer`: **Custom node containers**, by default, the tree nodes are stored in an `std::unordered_map` under 16D, above `std::map`. This can be replaced with any drop-in compatible associative container (e.g., Abseil's `flat_hash_map`) by overriding the `UMapNodeContainer` template alias in a custom `Configuration`.

While the `OrthoTree::detail` namespace is reserved for internal implementation details, SFINAE can be utilized as a fallback to extend support for custom container-related functions.

For latest version with the all available parameters see the [configuration.h](../include/orthotree/core/configuration.h).

Recommended usage: The Configuration parameters could be changed time-to-time, the recommended usage is inheritance with overridden parameters.
```C++
  struct UserDefinedConfigurationForBox : Configuration<2.0>
  {
    // Overrided parameters:
    static constexpr bool ALLOW_OUT_OF_SPACE_INSERTION = false;
    static constexpr std::size_t DEFAULT_TARGET_ELEMENT_NUM_IN_NODES = 10;
  };
```

### Compile-time flags
Configuration can be further tuned using macros defined in [build_config.h](../include/orthotree/core/build_config.h). Define these before including the library headers:
* `ORTHOTREE__PUBLIC_NODE_INTERFACE`: Expose node-related internal functions.
* `ORTHOTREE__PUBLIC_LINEARDATA_INTERFACE`: Expose linear internal data on static trees.
* `ORTHOTREE__DEBUG_CHECKS`: Enable internal debug assertions.
* `ORTHOTREE__USE_PMR`: Force use of Polymorphic Memory Resources (default for MSVC) with the default Configuration.
* `ORTHOTREE__DISABLE_PMR`: Disable PMR usage on all platforms.
* `ORTHOTREE__LOCATIONCODE_32`: Use 32-bit location codes (sufficient for depths < 10, otherwise 64-bit is used).
* `ORTHOTREE__LARGE_DATASET`: Over 4 billion elements in the tree use 64bit index type for memory resource.
* `ORTHOTREE_INDEX_T__INT` / `SIZE_T` / `UINT_FAST32_T`: Specify the underlying type for `index_t`.

## Creation, Insertion and Updates

The dynamic tree and managed container offer various ways to insert elements, depending on the need for maintaining balance and uniqueness.

### `Create()`/ Constructors

`Create()` and the parameterized constructors are designed for bulk insertion (e.g., opening a model with a huge dataset, followed by smaller edits later). Using `ExecutionTags::Parallel`, the creation process can be effectively multi-threaded.

### `Insert()` / `InsertIntoLeaf()`

> [!WARNING]
> Insert functions (other than bulk insert) should only be used *after* `Init()` or `Create()`. These initialization functions define the initial tree spatial boundaries (modelspace domain). 
> *Note:* The default constructor is provided for flexibility and easy adaptation, but it requires manual initialization to prevent misuse.

Insertion logic revolves around handling node overflows:
* `Insert()` provides the standard insertion method, automatically rebalancing and splitting the tree when nodes overflow. 
* `InsertIntoLeaf()` allows forcing elements into leaf nodes without node splitting. Its behavior is governed by the `InsertionMode` enum:
  * `InsertionMode::Balanced`: Uses standard balanced `Insert()` (Not applicable directly to `InsertIntoLeaf()`, but used as the default enum option in containers).
  * `InsertionMode::LowestLeaf`: Inserts the element into the bottom-most leaf node that geographically houses it.
  * `InsertionMode::ExistingLeaf`: Inserts the element into the deepest already-existing leaf node, avoiding the creation of deeper nodes.

Recommended insert usage:
```C++
  Point3D p = {};
  auto entityID = workingPoints.size();
  if (coreTree.Insert(entityID, p, workingPoints))
    workingPoints.push_back(p);
```

> [!WARNING]
> Do not intermix `Insert()` with `InsertIntoLeaf()` calls. `Insert()` will not split a node if it already has child nodes. Since `InsertIntoLeaf(LowestLeaf)` unconditionally creates child nodes, subsequent overflow-based splits for parent elements might not trigger. Furthermore, `Insert()` assumes that only the currently inserted element causes an overflow, whereas `InsertIntoLeaf(ExistingLeaf)` can silently overflow nodes with multiple elements.

High-level wrappers like the `OrthoTreeManaged` typically use the `Add()` or `Update()` interface, taking the `InsertionMode` enum as a parameter to route internally to `Insert()` or `InsertIntoLeaf()`.

Managed tree example:
```C++
  std::optional<EntityID> entityID = managedTree.Add(Point3D{...}/*, InsertionMode::Balanced*/);
```

### Bulk `Insert()`
There is an `Insert` overload intended for bulk-insertion purposes. Passing an array or range of elements to `Insert` performantly integrates everything into the tree simultaneously, significantly reducing initialization and loading times.

### `InsertUnique()`

`InsertUnique()` adds an element only if an overlapping geometry is not already present in the tree. Because it iterates existing geometry to ensure uniqueness, you must execute the verification *before* adding the geometry to your underlying storage.

> [!CAUTION]
> Do not add the element to your `storedEntities` vector (dense, non-entity-keyed containers) before calling `InsertUnique()`. Provide the potential new geometry directly to the function instead.

Example of correct `InsertUnique()` usage:
```cpp
auto newEntityID = storedEntities.size();
if (tree.InsertUnique(newEntityID, newEntityGeometry, storedEntities, tolerance)) {
  storedEntities.emplace_back(newEntityGeometry);
```

### `Update()` / `Erase()`

`Erase()` removes the `EntityID` from the tree, `Update()` removes from old node and add to the new node.
The following strategies are used:
* If `USE_REVERSE_MAPPING` is active in the `Configuration`: Related node can be found in `O(1)` time.
* If the original entity is supplied to the function: Related node can be found in `O(1)` time.
* If there is no additional information: Linear search will happen `O(N)`.
Using wrappers like `OrthoTreeManaged` will securely update / erase the entity from both your underlying collection and the tree structure simultaneously.


It returns `true` if the entity was updated, `false` otherwise. 
* If the entity is not found, it returns `false`.
* If `ALLOW_OUT_OF_SPACE_INSERTION` is disabled and the entity is not in the handled range, it returns `false`, and the original entity is removed.

> [!CAUTION]
> Do not update the entity in your underlying storage before calling `Update()`. Provide the potential new geometry directly to the function instead. It could cause issues during node overflow. 

Recommended bulk update usage:
```C++
for (int entityID = 0; entityID < updatePointsNo; ++entityID)
  if (tree.Update(entityID, workingPoints[entityID], updatePoints[entityID], workingPoints))
    workingPoints[entityID] = updatePoints[entityID];
```

## Querying

### Entity testers
During query operations, the structural layout of the tree only knows about bounding geometries (AABBs or points).
This usually only enough for prefiltering, to reduce the number of the precise geometry tests. Also, often, you need to filter search results based on user-defined attributes or custom entity metadata (e.g., checking if an entity's physical material matches, or if a flag is active). 

OrthoTree supports various tester signatures for different query types:
* **Basic Filter**: `bool(EntityID)` or `bool(Entity)`
* **Raycasting**: `bool(EntityID, TVector rayOrigin, TVector rayDir)` or `std::optional<TScalar>(...)` for custom hit distance.
* **Plane/Frustum**: `bool(EntityID, TPlane)`
* **Range**: `bool(EntityID, TBox)`

To accomplish this, you can provide an **Entity Tester** (e.g., a lambda) passed to the query function that acts as a custom filter.

```cpp
auto activeEntities = managedTree.RangeSearch(searchBox, RangeSearchMode::Inside, GA::BASE_TOLERANCE, [&](EntityID id) {
  return storedEntities[id].isActive; // only include active entities
});
```

### Query types
OrthoTree implements several ready-made, highly-optimized spatial queries out-of-the-box:
* **Range Search**: `RangeSearch(range, ...)` Finds entities that overlap or are entirely within a bounding box.
* **Collision Detection**: `CollisionDetection(...)` Retrieves pairs of entities whose geometrical boundaries intersect.
* **KNN**: `GetNearestNeighbors(point, k, ...)` Finds the k-closest entities to a given point. For boxes, it considers optimistic and pessimistic distances, which could result in more than k entities that surely contain the real k-closest entities after precise tests.
* **Pick Search**: `PickSearch(point, ...)` Finds entities containing a given point.
* **Raycasting**: `RayIntersectedAll(ray, ...)` or `RayIntersectedFirst(ray, ...)` finds intersections along a ray within a conical tolerance.
* **Plane Search**: `PlaneSearch(plane, ...)` Finds entities intersected by a hyperplane.
* **Frustum Culling**: `FrustumCulling(planes, ...)` Finds entities within or intersected by a set of boundary planes.

### `Query()`
The generic `Query` interface allows for complex, multi-condition searches. You can combine multiple search criteria (Range, Frustum, Custom Testers) using logical AND or OR.

```cpp
auto conditions = std::vector<OctreeBox::QueryCondition>{
  OctreeBox::ByWithin(boundingBox),
  OctreeBox::BySatisfies([](EntityID id) { return id % 2 == 0; })
};
// Combined with logical AND by default
auto results = tree.Query(conditions, entities); 

// alternative:
auto results = tree.Query(std::array{
  OctreeBox::ByWithin(boundingBox1),
  OctreeBox::ByOverlaps(boundingBox2),
  OctreeBox::ByInFrustum(frustumPlanesWithAllowedHitFlags),
  OctreeBox::BySatisfies([](Entity const& entity) { return entity.isActive; }),
  OctreeBox::BySatisfies([](EntityID id) { return id % 2 == 0; })
}, entities); 

// Combined with logical Or
auto resultsOr = tree.Query<LogicalOperator::Or>(conditions, entities); 
```

### Traversals
For advanced users needing direct access to the tree structure, several traversal methods are provided:
* **Depth-First**: `TraverseEntitiesDepthFirst(procedure)` or `GetEntitiesDepthFirst()`
* **Breadth-First**: `TraverseEntitiesBreadthFirst(procedure)` or `GetEntitiesBreadthFirst()`
* **Priority-Based**: `TraverseEntitiesByPriority(procedure, priorityCalculator)` useful for distance-ordered processing.

For `TraverseEntitiesByPriority`, the `priorityCalculator` receives a `TBox const& nodeBox` and must return a priority value (any comparable type). Wrap it in `std::optional<TPriority>` to **skip** a subtree entirely when returning `std::nullopt`.

All traversal `procedure` callbacks must return a `TraverseControl` value to guide the traversal:
* `TraverseControl::Continue` — proceed normally into children.
* `TraverseControl::SkipChildren` — visit this node but do not descend into its children.
* `TraverseControl::Terminate` — stop the entire traversal immediately.

```cpp
tree.TraverseEntitiesByPriority(
  [&](auto const& entitiesInNode, TBox const& nodeBox, float priority) {
    for (auto id : entitiesInNode)
      process(id);
    return TraverseControl::Continue;
  },
  [&](TBox const& nodeBox) -> std::optional<float> {
    float dist = distanceTo(nodeBox);
    if (dist > maxRange)
      return std::nullopt; // skip this entire subtree
    return dist;
  });
```