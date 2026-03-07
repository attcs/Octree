# Octree/Quadtree/N-dimensional linear tree
[![MSBuild and Unittests](https://github.com/attcs/Octree/actions/workflows/msbuild.yml/badge.svg)](https://github.com/attcs/Octree/actions/workflows/msbuild.yml)
<br>
Lightweight, parallelizable C++ implementation of an Octree/Quadtree/N-d orthotree using Morton Z curve-based location code ordering.<br>
<br>
What is the Octree and what is good for? https://en.wikipedia.org/wiki/Octree <br>
What is Morton-Z space-filling curve? https://en.wikipedia.org/wiki/Z-order_curve <br>
What is Loose Octree? https://anteru.net/blog/2008/loose-octrees/

[CHANGELOG](./CHANGELOG.md) | [DOCUMENTATION](./docs/README.md) | [BENCHMARKS](https://attcs.github.io/Octree/dev/bench/)

## Features
* Adaptable to any existing geometric system
* Adaptable to the original container of geometrical entities
* Arbitrary number of dimensions for other scientific usages
* Differentiated Static and Dynamic solution
* Optionally Loose octree
* Parallelization is available (via `std::execution` policies)
* Edit functions to Insert/Update/Erase entities
* Wide range of search functions for both AABBs and points
  * Range search
  * Pick search
  * K - Nearest neighbor search
  * Ray-traced search
  * Plane intersection
  * Frustum culling
* Collision detection
* Additional entity testers for the search functions
* Nodes can be accessed in O(1) time
* Search is accelerated by Morton Z curve based location code
* Both the non-owning `Core` and the `Managed` wrappers are provided

## Limitations
* Maximum number of dimensions is 63.
* Maximum depth of octree solutions is 21.

## Requirements
* Language standard: C++20 or above

## Usage
* Use `AdaptorBasicsConcept` or `AdaptorConcept` to adapt the actual geometric system. It is not a necessary step, basic point/vector and bounding box objects are available.
* Decide to let the geometry management for octree or not. `Managed` types could manage the geometries life-cycle, meanwhile the `non-managed` types are just update the relevant metadata about the changes.
* Use `PAR_EXEC` tag as first parameter of constructor for parallel execution
* Use `PickSearch()` / `RangeSearch()` member functions to collect the wanted id-s
* Use `PlaneSearch()` / `PlaneIntersection()` / `PlanePositiveSegmentation()` member functions for hyperplane related searches
* Use `FrustumCulling()` to get entities in the multi-plane-bounded space/frustum
* Use `Core` edit functions `Insert()`, `Update()`, `UpdateIndexes()`, `Erase()` if some of the underlying geometrical elements were changed or reordered
* Use `InsertUnique()` if tolerance-based unique insertion is needed for points
* Use `InsertWithRebalance()` for rebalancing the tree during insertion
* Use `Managed` edit functions `Add()`, `Update()`, `Erase()` if one of the underlying geometrical element was changed 
* Use `CollisionDetection()` member function for bounding box overlap examination.
* Use `VisitNodes()` / `VisitNodesInDFS()` to traverse the tree from up to down (former is breadth-first search) with user-defined `selector()` and `procedure()`.
* Use `GetNearestNeighbors()` for kNN search in point-based tree. https://en.wikipedia.org/wiki/K-nearest_neighbors_algorithm
* Use `RayIntersectedFirst()` or `RayIntersectedAll()` to get intersected bounding boxes in order by a ray.

## Notes
* Header only implementation.
* Point and Bounding box-based solutions are distinguished.
* Core types store only the entity ids; use Managed types to manage both the tree and the original entity data.
* Naming
  * Managed types have "M" postfix (e.g., `OctreeBoxM`).
  * `Map` named aliases are declared for `std::unordered_map` geometry containers (e.g.: `QuadtreeBoxMap`, `OctreeBoxMap`, `OctreeBoxMapM`). Non-`Map` named aliases uses `std::span`, which is compatible with `std::vector`, `std::array` or any contiguous container.
  * `s` means adjustable `LOOSE_TREE` for box-types (e.g., `OctreeBoxCs`).
* For box types 2.0 loose tree is the default.
* Edit functions are available but not recommended to fully build the tree with them.
* If less element is collected in a node than the max element then the child node won't be created.
* Original geometry data is not stored in Core types, so any search function needs them as an input.
* Tested compilers: MSVC 2022, Clang 12.0.0, GCC 11.3, AppleClang 16.0.0

## Attached adapters
* Default: 2D, 3D...63D; `std::array` based structures (`PointND`, `VectorND`, `BoundingBoxND`, `RayND`, `PlaneND`)
* CGAL: 2D, 3D; `CGAL::OctreePoint`, `OctreeBox`, `OctreePointM`, `OctreeBoxM`, etc. (adaptor.cgal.h)
* Eigen: 2D, 3D; `Eigen::OctreePoint3d`, `OctreePointC3d`, `OctreeBox3d`, `OctreeBoxC3d`, etc. (adaptor.eigen.h)
* glm: 2D, 3D, 4D; `glm::octree_point`, `octree_box`, `octree_point_c`, `octree_box_c`, etc. (adaptor.glm.h)
* Unreal Engine: 2D, 3D; `FOctreePoint`, `FOctreePointC`, `FOctreeBox`, `FOctreeBoxC`, etc. (adaptor.unreal.h)
* Boost: 2D, 3D; `boost::geometry::octree_point`, `octree_box`, etc. (adaptor.boost.h)
* `struct{x,y,z}`: 2D, 3D; (adaptor.xyz.h)

## Major aliases in OrthoTree
* `Core` types
  * Static: 
    * Contiguous: `StaticQuadtreePoint`, `StaticOctreePoint`, `StaticOrthoTreePointND<dim, ...>`, `StaticQuadtreeBox`, `StaticOctreeBox`, `StaticOrthoTreeBoxND<dim, ...>`
    * Custom keyed: `StaticQuadtreePointMap`, `StaticOctreePointMap`, `StaticQuadtreeBoxMap`, `StaticOctreeBoxMap`
  * Dynamic:
    * Contiguous: `QuadtreePoint`, `OctreePoint`, `OrthoTreePointND<dim, ...>`, `QuadtreeBox`, `OctreeBox`, `OrthoTreeBoxND<dim, ...>`
    * Custom keyed: `QuadtreePointMap`, `OctreePointMap`, `QuadtreeBoxMap`, `OctreeBoxMap`
* `Managed` types:
  * Static:
    * Contiguous: `StaticQuadtreePointM`, `StaticOctreePointM`, `StaticQuadtreeBoxM`,`StaticOctreeBoxM`,  `StaticTreePointManagedND<dim, ...>`, `StaticTreeBoxManagedND<dim, ...>`
    * Custom keyed: `StaticQuadtreePointMapM`, `StaticOctreePointMapM`, `StaticQuadtreeBoxMapM`, `StaticOctreeBoxMapM`
  * Dynamic:
    * Contiguous: `QuadtreePointM`, `OctreePointM`, `QuadtreeBoxM`, `OctreeBoxM`, `TreePointManagedND<dim, ...>`, `TreeBoxManagedND<dim, ...>`
    * Custom keyed: `QuadtreePointMapM`, `OctreePointMapM`, `QuadtreeBoxMapM`, `OctreeBoxMapM`

See the full list of the default aliases in [core/aliases.h](./include/orthotree/core/aliases.h)

## Basic examples

Usage of Managed types (Recommended)
```C++
    #include "octree.h"
    using namespace OrthoTree;
    
    // Example #1: Octree for points
    {
      auto constexpr points = std::array{ Point3D{0,0,0}, Point3D{1,1,1}, Point3D{2,2,2} };
      auto const octree = OctreePointM(points, 3 /*max depth*/);

      auto const searchBox = BoundingBox3D{ {0.5, 0.5, 0.5}, {2.5, 2.5, 2.5} };
      auto const pointIDs = octree.RangeSearch(searchBox); //: { 1, 2 }

      auto neighborNo = 2;
      auto pointIDsByKNN = octree.GetNearestNeighbors(Point3D{ 1.1, 1.1, 1.1 }
        , neighborNo
      ); //: { 1, 2 }
    }
    
    // Example #2: Quadtree for bounding boxes
    {
      auto boxes = std::vector
      {
        BoundingBox2D{ { 0.0, 0.0 }, { 1.0, 1.0 } },
        BoundingBox2D{ { 1.0, 1.0 }, { 2.0, 2.0 } },
        BoundingBox2D{ { 2.0, 2.0 }, { 3.0, 3.0 } },
        BoundingBox2D{ { 3.0, 3.0 }, { 4.0, 4.0 } },
        BoundingBox2D{ { 1.2, 1.2 }, { 2.8, 2.8 } }
      };

      auto quadtree = QuadtreeBoxM(boxes
        , 3            // max depth
        , std::nullopt // user-provided bounding Box for all
        , 1            // max element in a node 
      );

      auto collidingIDPairs = quadtree.CollisionDetection(); //: { {1,4}, {2,4} }

      auto searchBox = BoundingBox2D{ { 1.0, 1.0 }, { 3.1, 3.1 } };

      // Boxes within the range
      auto insideBoxIDs = quadtree.RangeSearch(searchBox); //: { 1, 2, 4 }

      // Overlapping Boxes with the range
      auto overlappingBoxIDs = quadtree.RangeSearch(searchBox, RangeSearchMode::Overlap); 
                               //: { 1, 2, 3, 4 }

      // Picked boxes
      auto pickPoint = Point2D{ 2.5, 2.5 };
      auto pickedIDs = quadtree.PickSearch(pickPoint); //: { 2, 4 }
    }
    
    // Example #3: Parallel creation of octree for bounding boxes
    {
      auto boxes = std::vector
      { 
        BoundingBox3D{ { 0.0, 0.0, 0.0 }, { 1.0, 1.0, 1.0 } } 
        /* and more... */
      };

      auto octreeUsingCtor = OctreeBoxM(PAR_EXEC
        , boxes
        , 3
        , std::nullopt
        , OctreeBox::DEFAULT_MAX_ELEMENT
      );

      auto octreeUsingCreate = OctreeBoxM::Create<true>(boxes, 3);
    }

    
    // Example #4: Using std::unordered_map-based container
    {
      auto boxes = std::unordered_map<OrthoTree::index_t, BoundingBox2D>{
        { 10, BoundingBox2D{{ 0.0, 0.0 }, { 1.0, 1.0 }}},
        { 13, BoundingBox2D{{ 3.0, 3.0 }, { 4.0, 4.0 }}},
        { 11, BoundingBox2D{{ 1.0, 1.0 }, { 2.0, 2.0 }}},
        { 14, BoundingBox2D{{ 1.2, 1.2 }, { 2.8, 2.8 }}},
        { 12, BoundingBox2D{{ 2.0, 2.0 }, { 3.0, 3.0 }}}
      };

      auto qt = QuadtreeBoxMap(
        boxes,
        3, // max depth
        std::nullopt, // user-provided bounding Box for all
        1 // max element in a node
      );
    }
```

Usage of Core types
```C++
    #include "octree.h"
    using namespace OrthoTree;
    
    // Example #1: Octree for points
    {
      auto constexpr points = std::array{ Point3D{0,0,0}, Point3D{1,1,1}, Point3D{2,2,2} };
      auto const octree = OctreePoint(points, 3 /*max depth*/);

      auto const searchBox = BoundingBox3D{ {0.5, 0.5, 0.5}, {2.5, 2.5, 2.5} };
      auto pointIDsByRange = octree.RangeSearch(searchBox, points); //: { 1, 2 }
      auto pointIDsByKNN = octree.GetNearestNeighbors(Point3D{ 1.1,1.1,1.1 }
        , 2 // k neighbor
        , points
      ); //: { 1, 2 }
    }
    
    // Example #2: Quadtree for bounding boxes
    {
      auto boxes = std::vector
      {
        BoundingBox2D{ { 0.0, 0.0 }, { 1.0, 1.0 } },
        BoundingBox2D{ { 1.0, 1.0 }, { 2.0, 2.0 } },
        BoundingBox2D{ { 2.0, 2.0 }, { 3.0, 3.0 } },
        BoundingBox2D{ { 3.0, 3.0 }, { 4.0, 4.0 } },
        BoundingBox2D{ { 1.2, 1.2 }, { 2.8, 2.8 } }
      };

      auto qt = QuadtreeBox(boxes
        , 3            // max depth
        , std::nullopt // user-provided bounding Box for all
        , 1            // max element in a node 
      );

      auto collidingIDPairs = qt.CollisionDetection(boxes); //: { {1,4}, {2,4} }

      auto searchBox = BoundingBox2D{ { 1.0, 1.0 }, { 3.1, 3.1 } };

      // Boxes within the range
      auto insideBoxIDs = qt.RangeSearch(searchBox, boxes); //: { 1, 2, 4 }

      // Overlapping Boxes with the range
      auto overlappingBoxIDs = qt.RangeSearch(searchBox, boxes, RangeSearchMode::Overlap); //: { 1, 2, 3, 4 }

      // Picked boxes
      auto pickPoint = Point2D{ 2.5, 2.5 };
      auto pickedBoxIDs = qt.PickSearch(pickPoint, boxes); //: { 2, 4 }
    }
```
    
For more examples, see the unit tests. E.g., [example.tests.cpp](./tests/unittests/example.tests.cpp).
<div align="center" width="100%"><img src="https://github.com/attcs/Octree/blob/master/docs/quadtree_example.PNG " align="center" height="300"></div>




## Benchmarks
Octree creation for 3 point sets using different placing strategy, and Cylindrical point set generation time:<br>
<div align="center" width="100%"><img src="https://github.com/attcs/Octree/blob/master/docs/octree_point_perf_3sets.png" align="center"></div>
<br><br>
Octree creation for 3 box sets using different placing strategy, and Cylindrical box set generation time:<br><br>
<div align="center" width="100%"><img src="https://github.com/attcs/Octree/blob/master/docs/octree_box_perf_3sets.png" align="center"></div>
<br><br>
Collision detection:<br><br>
<div align="center" width="100%"><img src="https://github.com/attcs/Octree/blob/master/docs/collisiondetection.png" align="center"></div>
<br>
<br>
*CPU: AMD Ryzen 5 5600X 6-Core @ 3.70GHz, CPU benchmark: 22146

