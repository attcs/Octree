# Changelog

## 2026-05 v3.2.2

Bugfixes
* Fix bulk-Insert logic

## 2026-04 v3.2.1

Performance improvements
* Better `MemoryResource`

## 2026-03 v3.2.0

New feature
* Serialization support

## 2026-03 v3.1.1

Bugfixes
* Memory resource could cause memory leak.
* Typos in adapter headers.

Miscellaneous
* Managed types now have a new constructor that takes only the bounding box and the depth.

## 2026-03 v3.1.0

New features
* Static BVH linear core is added.

## 2026-03 v3.0.0
Major refactorization with large interface change.

> [!CAUTION]
> `RayIntersectedAll()` has a new, `toleranceIncrement` parameter, which can be misinterpreted as `maxExaminationDistance`, causing **false positive hits**.

Interface changes
* Geometric types and `DIMENSION_NO` moved into the `BaseGeometryAdapter` (formerly `AdaptorGeneralBasics`)
* `BASE_TOLERANCE` is added to the `BaseGeometryAdapter`, and it is applied as default parameter.
* `TFloatScalar` is introduced: integer based geometry calculation is very hectic. Internal calculation uses this type and tolerances.
* EntityAdapter is introduced: this allows any type of system adoption. For more details see the [EntityAdapter](./include/orthotree/core/entity_adapter.h).
* Configuration is introduced. For more details see the [configuration.h](./include/orthotree/core/configuration.h).
* `Container` types are renamed to `Managed`, and `C` postfix is changed to `M`
* Split strategy is removed completely: Last iteration (single depth split) was an inferior solution in every aspect to the regular octree. Loose tree is introduces instead.
* `Visit*()` functions are changed: `TraverseNodesBreadthFirst()`/`TraverseNodesDepthFirst()`/`TraverseNodesByPriority()`. 
  Only one lambda is required which 
  * input is `NodeValue` (currently std::pair<const NodeID, Node>, but do not rely on in long term), other API function are available to ask node geometry data and entities (`GetNodeMinPoint()`, `GetNodeSize()`, `GetNodeEntities()`).
  * must return `TraverseControl` enum to control the traverse.
* Out of modelspace entities handling now is configurable. Default behavior is changed to allow.
  * Allow = true [Default]: OoM entities will be placed in the Root. Root bounding box will be changed to enclose the new entity.
  * Allow = false: OoM entity related operations will be failed, and no changes will be applied to the tree. (E.g. Insert/Update) Creation will be partial.
* `Insert()` is changed: it will split the overflowed node. 
  * `InsertWithRebalance()` is removed in favor of `Insert()`.
  * `InsertIntoLeaf()` inherited the original behavior. An  `InsertionMode` enum controls whether to be restricted to the existing nodes or create new ones.
  * Efficient bulk insert overload is added.
* `Query()` function is added for complex combined queries: simultanious frustum culling, plane intersection, range search and custom tester.
* Custom template `EntityTester`-s were added to queries. (Both entityID/Entity and geometry-related testers are allowed.)
* `RangeSearch()` function is changes: `isFullyContain` template parameter is removed and `RangeSearchMode` enum is added as function parameter.
* `bool`-flagged pararellism parameters are replaced with `ExecutionTags`.
* Required adapter includes are added to the public headers. For custom include path, a build macro is available. (E.g. Eigen: `#define ORTHOTREE_EIGEN_INCLUDE <my_vendor/Eigen/Geometry>`)

New features
* Static octree is implemented. Inmutable orthotree with contiguous memory layout.
* Bulk `Insert()` for dynamic octree.
* Loose octree option.
* MBR node storage option.

Miscellaneous
* Code is restructured from single to multi header structure.
* Double precision is used in grid calculation for float32 based geometry with larger depth to avoid numerical issues.
* `RayInsersection` functions was not prepared for fast-math compiler options, it is fixed.
 
Performance improvements
* Loose octree has major performance advantage for picking.

## 2025-11 v2.7
Interface changes
* GetNearestNeighbors()` supports boxes, and changed to be more numerically robust.
* `RayIntersected*()` functions changed to be more numerically robust.

## 2025-05 v2.6
Interface changes
* Exposing of Node's entities responsibility was moved to the Tree class
* General split depth increasement strategy is removed. The one-level deeper solution is applied: `DO_SPLIT_PARENT_ENTITIES`
* Default EntityID type is changed to `uint32_t`
* `Node::GetChildren()` is a range now instead of `std::vector` 
* Naming issues aroung node's max element and max depth ID are solved
* Distance sorted result requirement is added to `GetNearestNeighbors()`

Performance improvements
* Box creation
* Both the internal and between-tree version of `CollisionDetection()`
* `RayIntersectedFirst()`
* `GetNearestNeighbors()`

Miscellaneous
* Automatic Benchmark is added
* Children key storage is changed to bitflag for smaller dimensions.
* W4 warning level

Bugfixes
* Minor bugfixes in `InsertWithRebalance()`
* Minor bugfix in `GetNearestNeighbors()`

New benchmarks
* Creation for 6D points
* `GetNearestNeighbors()` for 6D and 63D

## 2025-01-19 v2.5
Interface changes
* `std::execution` is dropped from the interface. `bool IS_PARALLEL_EXEC` is used instead.
* `Id` is replaced to `Entity`: `CollectAllEntitiesInDFS`/`BFS` `EraseEntity`

New features
* `InsertUnique()` for unique insertion of points
* `RangeSearchNodes()` for range search of nodes
* `BMI2` instruction set to enhance `Morton` `Encode()`/`Decode()` (Use `AVX2`)

Maintenance
* Increased performance of `GetNearestNeighbor()`
* `AddAndRebalance` and `AddUnique` is added to the `Container`

## 2025-01-14 v2.4
Refactorizations
* Morton Z-order space indexing separation
* Grid space indexing separation

Bugfixes
* Container type move constructor fix

## 2024-12-26 v2.3
New features
* Support of abstract classes

Refactorizations
* Internal geometry module

## 2024-12-22 v2.2
New features
* Allowing associative container type for the entity-geometries

Maintenance
* Support compilers w/o `std::execution` (e.g. Apple Clang)

## 2024-07-28 v2.1
New features
* Rebalancing insert
* Removing empty nodes during `Erase`

## 2024-03-24 v2.0
New features
* New adaptors: glm, boost, CGAL
* Built-in `Ray` and `Plane` types

Refactorizations
* New, more capable adaptor concept
* Separated Adapator tests
* Support of `Eigen::Hyperplane` and `Eigen::ParametrizedLine`
* Support of Unreal Engine `FRay`, `FPlane`

Maintenance
* GCC compile action
* Fix a bug in `Insert()` 

## 2024-03-02
New features
* New adaptors: Eigen, Unreal Engine, XYZ
* Plane related search and intersection check
* Frustum culling
* User-defined function for Collision detection
* Depth-first search visitor

Refactorizations
* Clang format and naming changes
* entity_id_type and max_element_type are replaced with size_t
* ID generation is removed

## 2024-01-16
* Tolerance is added to the Ray intersection functions
* Bugfixes

## 2023-01-11
* Split strategy performance upgrade
* GCC support
* Bugfixes

## 2022-01-30 v1.0
New features
* Container type
* Split strategy for boundingbox-type tree
* Collision detection inside the tree
* Ray intersections
* k-Nearest Neighbors

Maintenance
* Benchmarks
* Bugfixes

## 2021-04-26
* First commits