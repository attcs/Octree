# Octree/Quadtree/N-dimensional linear tree
[![MSBuild and Unittests](https://github.com/attcs/Octree/actions/workflows/msbuild.yml/badge.svg)](https://github.com/attcs/Octree/actions/workflows/msbuild.yml)
<br>
Lightweight, parallelizable C++ implementation of an Octree/Quadtree/N-d orthotree using Morton Z curve-based location code ordering.<br>
<br>
What is the Octree and what is good for? https://en.wikipedia.org/wiki/Octree

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
  * Plane intersection
  * Frustum culling
* Collision detection
* Nodes can be accessed in O(1) time
* Search is accelerated by Morton Z curve based location code
* Both the non-owning `Core` and the `Container` wrapper is provided

## Limitations
* Maximum number of dimensions is 63.
* Maximum depth of octree solutions is 10.
* Abstract classes cannot be used for `TVector` and `TBox`

## Requirements
* Language standard: C++20 or above

## Usage
* Use `AdaptorBasicsConcept` or `AdaptorConcept` to adapt the actual geometric system. It is not a necessary step, basic point/vector and bounding box objects are available.
* Use the static member function `Create()` for a contiguous container (any `std::span` compatible) of Points or Bounding boxes to build the tree. It supports `std::execution` policies (e.g.: `std::execution::parallel_unsequenced_policy`) which can be effectively used to parallelize the creation process. (Template argument of the `Create()` functions)
* Use `PickSearch()` / `RangeSearch()` member functions to collect the wanted id-s
* Use `PlaneSearch()` / `PlaneIntersection()` / `PlanePositiveSegmentation()` member functions for hyperplane related searches
* Use `FrustumCulling()` to get entities in the multi-plane-bounded space/frustum
* Use `Core` edit functions `Insert()`, `Update()`, `UpdateIndexes()`, `Erase()` if the some of the underlying geometrical elements were changed or reordered
* Use `Container` edit functions `Add()`, `Update()`, `Erase()` if one of the underlying geometrical element was changed 
* Use `CollisionDetection()` member function for bounding box overlap examination.
* Use `VisitNodes()` / `VisitNodesInDFS()` to traverse the tree from up to down (former is breadth-first search) with user-defined `selector()` and `procedure()`.
* Use `GetNearestNeighbors()` for kNN search in point based tree. https://en.wikipedia.org/wiki/K-nearest_neighbors_algorithm
* Use `RayIntersectedFirst()` or `RayIntersectedAll()` to get intersected bounding boxes in order by a ray.


## Notes
* Header only implementation.
* Point and Bounding box-based solution is distinguished.
* Core types store only the entity ids, use Container types to store. Core types advantages: not copying and managing the entity information; disadvantages: this information may have to be provided again for the member function call.
* Container types have "C" postfix (e.g.: core `OctreeBox`'s container is `OctreeBoxC`).
* Bounding box-based solution stores item id in the parent node if it is not fit into any child node. Using `SPLIT_DEPTH_INCREASEMENT` template parameter, these boxes can be splitted then placed on the deeper level of the tree. The `SPLIT_DEPTH_INCREASEMENT` default is 2 and this split method is applied by default.
* Edit functions are available but not recommended to majorly build the tree.
* If less element is collected in a node than the max element then the child node won't be created.
* The underlying container is a hash-table (`std::unordered_map`) under 16D, which only stores the id-s and the bounding box of the child nodes.
* Original geometry data is not stored, so any search function needs them as an input.
* Unit tests are attached. (Microsoft Unit Testing Framework for C++)
* Tested compilers: MSVC 2022, Clang 12.0.0, GCC 11.3

## Attached adapters
* Default: 2D, 3D...63D; `std::array` based structures (`PointND`, `VectorND`, `BoundingBoxND`, `RayND`, `PlaneND`)
* CGAL: 2D, 3D; `CGAL::OctreePoint`, `OctreeBox`, `OctreePointC`, `OctreeBoxC`, etc. (adaptor.cgal.h)
* Eigen: 2D, 3D; `Eigen::OctreePoint3d`, `OctreePointC3d`, `OctreeBox3d`, `OctreeBoxC3d`, etc. (adaptor.eigen.h)
* glm: 2D, 3D, 4D; `glm::octree_point`, `octree_box`, `octree_point_c`, `octree_box_c`, etc. (adaptor.glm.h)
* Unreal Engine: 2D, 3D; `FOctreePoint`, `FOctreePointC`, `FOctreeBox`, `FOctreeBoxC`, etc. (adaptor.unreal.h)
* Boost: 2D, 3D; `boost::geometry::octree_point`, `octree_box`, etc. (adaptor.boost.h)
* `struct{x,y,z}`: 2D, 3D; (adaptor.xyz.h)

## Major aliases in OrthoTree
```C++
  /// Default geometrical base elements

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


  /// Core types

  // Quadtree for points
  using QuadtreePoint = TreePointND<2, BaseGeometryType>;

  // Quadtree for bounding boxes
  using QuadtreeBox = TreeBoxND<2, 2, BaseGeometryType>;

  // Octree for points
  using OctreePoint = TreePointND<3, BaseGeometryType>;

  // Octree for bounding boxes
  using OctreeBox = TreeBoxND<3, 2, BaseGeometryType>;

  // Hexatree for points
  using HexatreePoint = TreePointND<4, BaseGeometryType>;

  // Hexatree for bounding boxes
  using HexatreeBox = TreeBoxND<4, 2, BaseGeometryType>;

  // NTrees for higher dimensions
  using TreePoint16D = TreePointND<16, BaseGeometryType>;
  using TreeBox16D = TreeBoxND<16, 2, BaseGeometryType>;


  /// Container types

  // Quadtree for points
  using QuadtreePointC = TreePointContainerND<2, BaseGeometryType>;

  // Quadtree for bounding boxes
  template<uint32_t SPLIT_DEPTH_INCREASEMENT = 2>
  using QuadtreeBoxCs = TreeBoxContainerND<2, SPLIT_DEPTH_INCREASEMENT, BaseGeometryType>;
  using QuadtreeBoxC = TreeBoxContainerND<2, 2, BaseGeometryType>;

  // Octree for points
  using OctreePointC = TreePointContainerND<3, BaseGeometryType>;

  // Octree for bounding boxes
  template<uint32_t SPLIT_DEPTH_INCREASEMENT = 2>
  using OctreeBoxCs = TreeBoxContainerND<3, 2, BaseGeometryType>;
  using OctreeBoxC = TreeBoxContainerND<3, 2, BaseGeometryType>;
```

## Basic examples

Usage of Container types
```C++
    #include "octree.h"
    using namespace OrthoTree;
    
    // Example #1: Octree for points
    {
      auto constexpr points = array{ Point3D{0,0,0}, Point3D{1,1,1}, Point3D{2,2,2} };
      auto const octree = OctreePointC(points, 3 /*max depth*/);

      auto const searchBox = BoundingBox3D{ {0.5, 0.5, 0.5}, {2.5, 2.5, 2.5} };
      auto const pointIDs = octree.RangeSearch(searchBox); //: { 1, 2 }

      auto neighborNo = 2;
      auto pointIDsByKNN = octree.GetNearestNeighbors(Point3D{ 1.1, 1.1, 1.1 }
        , neighborNo
      ); //: { 1, 2 }
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

      auto quadtree = QuadtreeBoxC(boxes
        , 3            // max depth
        , std::nullopt // user-provided bounding Box for all
        , 2            // max element in a node 
        , false        // parallel calculation option
      );

      auto collidingIDPairs = quadtree.CollisionDetection(); //: { {1,4}, {2,4} }

      auto searchBox = BoundingBox2D{ { 1.0, 1.0 }, { 3.1, 3.1 } };

      // Boxes within the range
      auto insideBoxIDs = quadtree.RangeSearch(searchBox); //: { 1, 2, 4 }

      // Overlapping Boxes with the range
      constexpr bool shouldFullyContain = false; // overlap is enough
      auto overlappingBoxIDs = quadtree.RangeSearch<shouldFullyContain>(searchBox); 
                               //: { 1, 2, 3, 4 }

      // Picked boxes
      auto pickPoint = Point2D{ 2.5, 2.5 };
      auto pickedIDs = quadtree.PickSearch(pickPoint); //: { 2, 4 }
    }
    
    // Example #3: Parallel creation of octree for bounding boxes
    {
      auto boxes = vector
      { 
        BoundingBox3D{ { 0.0, 0.0, 0.0 }, { 1.0, 1.0, 1.0 } } 
        /* and more... */
      };

      auto octreeUsingCtor = OctreeBoxC(boxes
        , 3
        , std::nullopt
        , OctreeBox::DEFAULT_MAX_ELEMENT
        , true // Set std::execution::parallel_unsequenced_policy
      );

      using namespace std::execution;
      auto octreeUsingCreate = OctreeBoxC::Create<parallel_unsequenced_policy>(boxes
        , 3
      );
    }
```

Usage of Core types
```C++
    #include "octree.h"
    using namespace OrthoTree;
    
    // Example #1: Octree for points
    {
      auto constexpr points = array{ Point3D{0,0,0}, Point3D{1,1,1}, Point3D{2,2,2} };
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
      auto boxes = vector
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
        , 2            // max element in a node 
      );

      auto collidingIDPairs = qt.CollisionDetection(boxes); //: { {1,4}, {2,4} }

      auto searchBox = BoundingBox2D{ { 1.0, 1.0 }, { 3.1, 3.1 } };

      // Boxes within the range
      auto insideBoxIDs = qt.RangeSearch(searchBox, boxes); //: { 1, 2, 4 }

      // Overlapping Boxes with the range
      constexpr bool shouldFullyContain = false;
      auto overlappingBoxIDs = qt.RangeSearch<shouldFullyContain>(searchBox
        , boxes
      ); //: { 1, 2, 3, 4 }

      // Picked boxes
      auto pickPoint = Point2D{ 2.5, 2.5 };
      auto pickedBoxIDs = qt.PickSearch(pickPoint, boxes); //: { 2, 4 }
    }
```
    
For more examples, see the unit tests.
<div align="center" width="100%"><img src="https://github.com/attcs/Octree/blob/master/docs/quadtree_example.PNG " align="center" height="300"></div>


## Adapting Octree/Quadtree to user-defined Point / Bounding box objects
```C++
// User-defined geometrical objects

struct MyPoint2D { float x; float y; };
using MyBox2D = std::array<MyPoint2D, 2>;
using MyRay2D = std::array<MyPoint2D, 2>;
struct MyPlane2D { float OrigoDistance; MyPoint2D Normal; };

// Adaptor

struct AdaptorBasicsCustom
{
  static float GetPointC(MyPoint2D const& pt, OrthoTree::dim_t i)
  {
    switch (i)
    {
    case 0: return pt.x;
    case 1: return pt.y;
    default: assert(false); return pt.x;
    }
  }

  static void SetPointC(MyPoint2D& pt, OrthoTree::dim_t i, float v)
  {
    switch (i)
    {
      case 0:  pt.x = v; break;
      case 1:  pt.y = v; break;
      default: assert(false);
    }
  }

  static void SetBoxMinC(MyBox2D& box, dim_t i, float v) { SetPointC(box[0], i, v); }
  static void SetBoxMaxC(MyBox2D& box, dim_t i, float v) { SetPointC(box[1], i, v); }
  static float GetBoxMinC(MyBox2D const& box, dim_t i) { return GetPointC(box[0], i); }
  static float GetBoxMaxC(MyBox2D const& box, dim_t i) { return GetPointC(box[1], i); }

  static MyPoint2D const& GetRayDirection(MyRay2D const& ray) { return ray[1]; }
  static MyPoint2D const& GetRayOrigin(MyRay2D const& ray) { return ray[0]; }

  static MyPoint2D const& GetPlaneNormal(MyPlane2D const& plane) { return plane.Normal; }
  static float GetPlaneOrigoDistance(MyPlane2D const& plane) { return plane.OrigoDistance; }
};


using AdaptorCustom = OrthoTree::AdaptorGeneralBase<
  2, 
  MyPoint2D, 
  MyBox2D, 
  MyRay2D, 
  MyPlane2D, 
  float, 
  AdaptorBasicsCustom>;


// Tailored Quadtree objects

using QuadtreePointCustom = OrthoTree::OrthoTreePoint<
  2, 
  MyPoint2D, 
  MyBox2D, 
  MyRay2D, 
  MyPlane2D, 
  float, 
  AdaptorCustom>;

using QuadtreeBoxCustom = OrthoTree::OrthoTreeBoundingBox<
  2, 
  MyPoint2D, 
  MyBox2D, 
  MyRay2D, 
  MyPlane2D, 
  float, 
  2, 
  AdaptorCustom>;
```

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

