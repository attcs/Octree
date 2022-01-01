# Octree/Quadtree/N-dimensional linear tree
[![MSBuild and Unittests](https://github.com/attcs/Octree/actions/workflows/msbuild.yml/badge.svg)](https://github.com/attcs/Octree/actions/workflows/msbuild.yml)
<br>
Lightweight, parallelizable, non-storing/non-owning C++ implementation of an Octree/Quadtree/N-d orthotree using Morton Z curve-based location code ordering.<br>
<br>
Why is it non-owning? Usually, the geometric objects and their metadata are already stored in a database, making the creation process efficient and avoiding the responsibility of the complex data management, this solution stores only id-s.<br>
<br>
What is an Octree and what is good for? https://en.wikipedia.org/wiki/Octree

## Features
* Adaptable to any existing geometric system
* Arbitrary number of dimensions for other scientific usages
* Support of `std::execution` policies (so it is parallelizable)
* Edit functions to Insert/Update/Erase entities
* Wide range of search functions
  * Range search
  * Pick search
  * K - Nearest neighbor search
  * Ray-traced search
* Collision detection
* Nodes can be accessed in O(1) time
* Search is accelerated by Morton Z curve based location code

## Limitations
* Maximum number of dimensions is 63.
* Maximum depth of octree solutions is 10.

## Requirements
* Language standard: C++20 or above

## Time complexity
* Creation: O(n)
* Range search: O(log{2^N}(n)) (where N is the number of the dimension)
* Access any node: O(1)

## Usage
* Use `AdaptorBasicsConcept` or `AdaptorConcept` to adapt the actual geometric system. It is not a necessary step, basic point/vector and bounding box objects are available.
* Call the static member function `Create()` for a contiguous container (any `std::span` compatible) of Points or Bounding boxes to build the tree. It supports `std::execution` policies (e.g.: `std::execution::parallel_unsequenced_policy`) which can be effectively used to parallelize the creation process. (Template argument of the `Create()` functions)
* Call `PickSearch()` / `RangeSearch()` member functions to collect the wanted id-s
* Call edit functions `Insert()`, `Update()`, `UpdateIndexes()`, `Erase()` if the some of the underlying geometrical elements were changed or reordered
* Call `CollisionDetection()` member function for two bounding box trees overlap examination.
* Call `VisitNodes()` to traverse the tree from up to down (breadth-first search) with user-defined `selector()` and `procedure()`.
* Call `GetNearestNeighbors()` for kNN search in point based tree. https://en.wikipedia.org/wiki/K-nearest_neighbors_algorithm
* Call `RayIntersectedFirst()` or `RayIntersectedAll()` to get intersected bounding boxes in order by a ray.


## Notes
* Header only implementation.
* Point and Bounding box-based solution was distinguished.
* Bounding box-based solution stores item id in the parent node if it is not fit into any child node.
* Edit functions are available but not recommended to majorly build the tree.
* If less element is collected in a node than the max element then the child node won't be created.
* The underlying container is a hash-table (`std::unordered_map`) under 16D, which only stores the id-s and the bounding box of the child nodes.
* Original geometry data is not stored, so any search function needs them as an input.
* Unit tests are attached. (Microsoft Unit Testing Framework for C++)
* Tested compilers: MSVC 2019, Clang 12.0.0

## Major aliases in OrthoTree
```C++
  // Default geometrical base elements
  using Point2D = OrthoTree::PointND<2>;
  using Point3D = OrthoTree::PointND<3>;
  using BoundingBox2D = OrthoTree::BoundingBoxND<2>;
  using BoundingBox3D = OrthoTree::BoundingBoxND<3>;

  // Quadtree for points (2D)
  using QuadtreePoint = TreePointND<2>;

  // Quadtree for bounding boxes (2D)
  using QuadtreeBox = TreeBoxND<2>;

  // Octree for points (3D)
  using OctreePoint = TreePointND<3>;

  // Octree for bounding boxes (3D)
  using OctreeBox = TreeBoxND<3>;

  // Hexatree for points (4D)
  using HexatreePoint = TreePointND<4>;
  
  // ...
  using TreePoint16D = TreePointND<16>;
```


## Basic examples
```C++
    #include "octree.h"
    using namespace OrthoTree;
    
    // Example #1: Octree for points
    {
        auto constexpr points = array{ Point3D{0,0,0}, Point3D{1,1,1}, Point3D{2,2,2} };
        auto const octree = OctreePoint::Create(points, 3 /*max depth*/);

        auto const search_box = BoundingBox3D{ {0.5, 0.5, 0.5}, {2.5, 2.5, 2.5} };
        auto ids = octree.RangeSearch(search_box, points); // -> { 1, 2 }
        auto knn_ids = octree.GetNearestNeighbors(Point3D{ 1.1,1.1,1.1 }, 2 /*k*/, points); // -> { 1, 2 }
    }
    
    // Example #2: Quadtree for bounding boxes
    {
        auto boxes = vector
        {
            BoundingBox2D{ { 0.0, 0.0 }, { 1.0, 1.0 } },
            BoundingBox2D{ { 1.0, 1.0 }, { 2.0, 2.0 } },
            BoundingBox2D{ { 2.0, 2.0 }, { 3.0, 3.0 } },
            BoundingBox2D{ { 3.0, 3.0 }, { 4.0, 4.0 } },
            BoundingBox2D{ { 1.2, 1.2 }, { 2.8, 2.8 } }
        };

        auto quadtreebox = QuadtreeBox::Create(boxes, 3
            , std::nullopt // user-provided bounding box for all
            , 2            // max element in a node 
            );

        auto search_box = BoundingBox2D{ { 1.0, 1.0 }, { 3.1, 3.1 } };
        
        // Boxes within the range
        auto ids_inside = quadtreebox.RangeSearch(search_box, boxes); // -> { 1, 2, 4 }
        
        // Overlaping Boxes with the range
        auto ids_overlaping = quadtreebox.RangeSearch<false/*overlap is enough*/>(search_box, boxes);
            // -> { 1, 2, 3, 4 }
        
        // Picked boxes
        auto ptPick = Point2D{ 2.5, 2.5 };
        auto ids_picked = quadtreebox.PickSearch(ptPick, boxes); // -> { 2, 4 }
    }
    
    // Example #3: Parallel creation of octree for bounding boxes
    {
        auto boxes = vector{ BoundingBox3D{ { 0.0, 0.0, 0.0 }, { 1.0, 1.0, 1.0 } } /* and more... */ };
        auto octreebox = OctreeBox::Create<std::execution::parallel_unsequenced_policy>(boxes, 3);
        // or
        // TreeBoxND<3>::template Create<std::execution::parallel_unsequenced_policy>(boxes, 3);
    }
    
    // For more examples, see the unit tests.
```
<div align="center" width="100%"><img src="https://github.com/attcs/Octree/blob/master/docs/quadtree_example.PNG " align="center" height="300"></div>


## Adapting Octree/Quadtree to user-defined Point / Bounding box objects
```C++
  // User-defined geometrical objects

  struct Point2DCustom { float x; float y; };
  using BoundingBox2DCustom = std::array<Point2DCustom, 2>;


  // Adaptor

  struct AdaptorBasicsCustom
  {
    static inline float& point_comp(Point2DCustom& pt, OrthoTree::dim_type iDimension)
    {
      switch (iDimension)
      {
        case 0: return pt.x;
        case 1: return pt.y;
        default: assert(false); return pt.x;
      }
    }

    static constexpr float point_comp_c(Point2DCustom const& pt, OrthoTree::dim_type iDimension)
    {
      switch (iDimension)
      {
        case 0: return pt.x;
        case 1: return pt.y;
        default: assert(false); return pt.x;
      }
    }

    static inline Point2DCustom& box_min(BoundingBox2DCustom& box) { return box[0]; }
    static inline Point2DCustom& box_max(BoundingBox2DCustom& box) { return box[1]; }
    static constexpr Point2DCustom const& box_min_c(BoundingBox2DCustom const& box) { return box[0]; }
    static constexpr Point2DCustom const& box_max_c(BoundingBox2DCustom const& box) { return box[1]; }
  };

  using AdaptorCustom = OrthoTree::AdaptorGeneralBase<2, Point2DCustom, BoundingBox2DCustom, AdaptorBasicsCustom, float>;


  // Tailored Quadtree objects

  using QuadtreePointCustom = OrthoTree::OrthoTreePoint<2, Point2DCustom, BoundingBox2DCustom, AdaptorCustom, float>;
  using QuadtreeBoxCustom = OrthoTree::OrthoTreeBoundingBox<2, Point2DCustom, BoundingBox2DCustom, AdaptorCustom, float>;
```

## Benchmarks
Octree creation for 3 point sets using different placing strategy, and Cylindrical point set generation time:<br>
<div align="center" width="100%"><img src="https://github.com/attcs/Octree/blob/master/docs/octree_point_perf_3sets.png" align="center"></div>
<br><br>
Octree creation for 3 box sets using different placing strategy, and Cylindrical box set generation time:<br><br>
<div align="center" width="100%"><img src="https://github.com/attcs/Octree/blob/master/docs/octree_box_perf_3sets.png" align="center"></div>
<br><br>
Point rangesearch, cyilndrical placing, number of ranges are 10% of number of points:<br><br>
<div align="center" width="100%"><img src="https://github.com/attcs/Octree/blob/master/docs/range_search.png" align="center"></div>
<br><br>
Current Z order implementation compared to a typical ortho tree in 3D and 8D:<br><br>
<div align="center" width="100%"><img src="https://github.com/attcs/Octree/blob/master/docs/zorder_vs_typical.png" align="center"></div>
<br>
<br>
*CPU: Intel Core i5-6500 @ 3.20GHz, CPU benchmark: 5640

