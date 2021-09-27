# Octree/Quadtree/N-dimensional linear tree
Lightweight, non-storing C++ implementation of an Octree using Morton Z curve-based location code ordering.<br>
Usually, geometrical objects and their metadata are already stored in a database, to making the creation process efficient, only the id-s are stored. <br>
A major advantage of this solution, that can be easily adapted for custom-defined Point and Bounding box object using AdaptorConcept.<br>
<br>
What is an Octree and what is good for? https://en.wikipedia.org/wiki/Octree

## Requirements
* Language standard: C++20 or above

## Usage
* Call the static member function `Create()` for a contiguous container (any `std::span` compatible) of Points or Bounding boxes to build the tree
* Call `PickSearch()` / `RangeSearch()` member functions to collect the wanted id-s
* Call edit functions `Insert()`, `Update()`, `UpdateIndexes()`, `Erase()` if the some of the underlying geometrical elements were changed or reordered
* Call `CollisionDetection()` member function for two bounding box trees overlap examination.
* Call `VisitNodes()` to traverse the tree from up to down (breadth-first search) with user-defined `selector()` and `procedure()`.
* Call `GetNearestNeighbors()` for kNN search in point based tree. https://en.wikipedia.org/wiki/K-nearest_neighbors_algorithm

## Notes
* Header only implementation.
* Point and Bounding box based solution was distinguished.
* Bounding box based solution stores item id in the parent node if it is not fit into any child node.
* Edit functions are available but not recommended to majorly build the tree.
* If less element is collected in a node than the max element then the child node won't be created.
* Basic point and bounding box objects are available, but adaptors can be used to attach into an already developed system.
* The underlying container is a hash-table (`std::unordered_map`), which only stores the id-s and the bounding box of the child nodes.
* Original geometry data is not stored, so any search function needs them as an input.
* Higher dimensional tree definition is available for more advanced problems (up to ~23).
* Unit tests are attached. (Microsoft Unit Testing Framework for C++)

## Major aliases in NTree
```C++
  // Default geometrical base elements
  using Point2D = NTree::Point<2>;
  using Point3D = NTree::Point<3>;
  using BoundingBox2D = NTree::BoundingBox<2>;
  using BoundingBox3D = NTree::BoundingBox<3>;

  // Quadtree for points (2D)
  using QuadtreePoint = NTree::NTreePoint<2, Point2D, BoundingBox2D>;

  // Quadtree for bounding boxes (2D)
  using QuadtreeBox = NTree::NTreeBoundingBox<2, Point2D, BoundingBox2D>;

  // Octree for points (3D)
  using OctreePoint = NTree::NTreePoint<3, Point3D, BoundingBox3D>;

  // Octree for bounding boxes (3D)
  using OctreeBox = NTree::NTreeBoundingBox<3, Point3D, BoundingBox3D>;

  // Hexatree for points (4D)
  using HexatreePoint = NTree::NTreePoint<4, NTree::Point<4>, NTree::BoundingBox<4>>;
```


## Basic examples
```C++
    #include "octree.h"
    using namespace NTree;
    
    // Example #1: Octree for points
    {
        auto constexpr points = array{ Point3D{0,0,0}, Point3D{1,1,1}, Point3D{2,2,2} };
        auto const octree = OctreePoint::Create(points, 3 /*max depth*/);

        auto const search_box = BoundingBox3D{ {0.5, 0.5, 0.5}, {2.5, 2.5, 2.5}}
        auto const ids = octree.RangeSearch(search_box, points); // -> { 1, 2 }
        auto const knn_ids = octree.GetNearestNeighbors(Point3D{1.1,1.1,1.1}, 2 /*k*/, points); // -> { 1, 2 }
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
        auto ids_overlaping = quadtreebox.RangeSearch(search_box, boxes
            , false // overlap is enough
            ); // -> { 1, 2, 3, 4 }
        
        // Picked boxes
        auto ptPick = Point2D{ 2.5, 2.5 };
        auto ids_picked = quadtreebox.PickSearch(ptPick, boxes); // -> { 2, 4 }
    }
    
    // For more examples, see the unit tests.
```
<div align="center" width="100%"><img src="https://github.com/attcs/Octree/blob/master/quadtree_example.PNG " align="center" height="300"></div>


## Adapting Octree/Quadtree to user-defined Point / Bounding box objects
```C++
  // User-defined geometrical objects

  struct Point2DCustom { float x; float y; };
  using BoundingBox2DCustom = std::array<Point2DCustom, 2>;


  // Adaptor

  struct AdaptorBasicsCustom
  {
    static inline float& point_comp(Point2DCustom& pt, NTree::dim_type iDimension)
    {
      switch (iDimension)
      {
        case 0: return pt.x;
        case 1: return pt.y;
        default: assert(false); return pt.x;
      }
    }

    static constexpr float point_comp_c(Point2DCustom const& pt, NTree::dim_type iDimension)
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

  using AdaptorCustom = NTree::AdaptorGeneralBase<2, Point2DCustom, BoundingBox2DCustom, AdaptorBasicsCustom, float>;


  // Tailored Quadtree objects

  using QuadtreePointCustom = NTree::NTreePoint<2, Point2DCustom, BoundingBox2DCustom, AdaptorCustom, float>;
  using QuadtreeBoxCustom = NTree::NTreeBoundingBox<2, Point2DCustom, BoundingBox2DCustom, AdaptorCustom, float>;
```
