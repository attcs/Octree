# C++ Octree/Quadtree/N-dimensional linear tree
Lightweight, non-storing implementation of an Octree using Morton Z curve-based location code ordering.<br>
Usually, geometrical objects and their metadata are already stored in a database, to making faster the creation process, only the id-s are stored. Point and Bounding box based solution was distinguished.<br>
A major advantage of this solution, that can be easily adapted for custom-defined Point and Bounding box object using AdaptorConcept.<br>
<br>
What is an Octree and what is good for? https://en.wikipedia.org/wiki/Octree

## Requirements
* Language standard: C++20 or above

## Usage
* Call the static member function `Create()` for a contiguous container of Points or Bounding boxes to build the tree
* Call `PickSearch()` / `RangeSearch()` member functions to collect the wanted id-s
* Call edit functions `Insert()`, `Update()`, `UpdateIndexes()`, `Erase()` if the some of the underlying geometrical elements were changed
* Call `CollisionDetection()` member function for two trees overlap examination.


## Notes
* Header only implementation.
* Edit functions are available but not recommended to majorly build the tree.
* If less element is collected in a node than the max element then the child node won't be created.
* Unit tests are attached. (Microsoft Unit Testing Framework for C++)
* Basic point and bounding box objects are available, but adaptors can be used to attach into an already developed system.
* The underlying container is a hash-table (`std::unordered_map`).
* Higher dimensional tree definition is available for more advanced problems.


## Basic examples
```C++
    include "octree.h"
    using namespace NTree;
    
    auto constexpr points = array{ Point3D{0,0,0}, Point3D{1,1,1}, Point3D{2,2,2} };
    auto const octree = OctreePoint::Create(points, 3 /*max depth*/);
    
    auto const search_box = BoundingBox3D{ {0.5, 0.5, 0.5}, {2.5, 2.5, 2.5}}
    auto const ids = octree.RangeSearch(search_box, points); // returns { 1, 2 }
```


## Using adaptor for custom-defined Point / Bounding box classes
```C++
    namespace CustomGeometryType
    {
      // Custom already used geometrical elements
      struct Point2DCustom
      {
        float x;
        float y;
      };

      using BoundingBox2DCustom = std::array<Point2DCustom, 2>;

      // Adaptor
      struct AdaptorBasicsCustom
      {
        static constexpr float& point_comp(Point2DCustom& pt, dim_type iDimension) {
          switch (iDimension)
          {
            case 0: return pt.x;
            case 1: return pt.y;
            default: assert(false); return pt.x;
          }
        }

        static constexpr float const& point_comp_c(Point2DCustom const& pt, dim_type iDimension) {
          switch (iDimension)
          {
            case 0: return pt.x;
            case 1: return pt.y;
            default: assert(false); return pt.x;
          }
        }

        static constexpr Point2DCustom& box_min(BoundingBox2DCustom& box) { return box[0]; }
        static constexpr Point2DCustom& box_max(BoundingBox2DCustom& box) { return box[1]; }
        static constexpr Point2DCustom const& box_min_c(BoundingBox2DCustom const& box) { return box[0]; }
        static constexpr Point2DCustom const& box_max_c(BoundingBox2DCustom const& box) { return box[1]; }
      };

      // Adapted Quadtree
      using QuadtreePointCustom = NTree::NTreePoint<2, Point2DCustom, BoundingBox2DCustom, NTree::AdaptorGeneralBase<2, Point2DCustom, BoundingBox2DCustom, AdaptorBasicsCustom, float>, float>;
    }
```
