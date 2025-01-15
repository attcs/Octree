# Changelog

## Latest
Interface changes
* `std::execution` is dropped from the interface. `bool IS_PARALLEL_EXEC` is used instead.
* `Id` is replaced to `Entity`: `CollectAllEntitiesInDFS`/`BFS` `EraseEntity`

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