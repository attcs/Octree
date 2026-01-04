#pragma once
/*
MIT License

Copyright (c) 2021 Attila Csikós

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "../octree.h"
/* Required headers
#include <CGAL/Bbox_2.h>
#include <CGAL/Bbox_3.h>
#include <CGAL/Cartesian.h>
#include <CGAL/Origin.h>
#include <CGAL/Plane_3.h>
#include <CGAL/Point_2.h>
#include <CGAL/Point_3.h>
#include <CGAL/Ray_2.h>
#include <CGAL/Ray_3.h>
#include <CGAL/basic.h>
#include <CGAL/cartesian.h>
*/
namespace CGAL
{
  // 2d Hyperplane
  struct Plane_2
  {
    double offset; // n1*x + n2*y + offset = 0
    Point_2<Cartesian<double>> normal;
  };
} // namespace CGAL


namespace OrthoTree
{
  namespace CGALAdapter
  {
    struct BaseGeometryAdapter2D
    {
      using Scalar = double;
      using FloatScalar = double;
      using Vector = CGAL::Point_2<CGAL::Cartesian<Scalar>>;
      using Box = CGAL::Bbox_2;
      using Ray = CGAL::Ray_2<CGAL::Cartesian<Scalar>>;
      using Plane = CGAL::Plane_2;

      static constexpr dim_t DIMENSION_NO = 2;
      static constexpr FloatScalar BASE_TOLERANCE = std::numeric_limits<FloatScalar>::epsilon() * FloatScalar(10);

      static Scalar GetPointC(Vector const& point, dim_t dimensionID) noexcept { return point[dimensionID]; }
      static void SetPointC(Vector& point, dim_t dimensionID, Scalar value) noexcept
      {
        if (dimensionID == 0)
          point = Vector(value, point.y());
        else
          point = Vector(point.x(), value);
      }

      static Scalar GetBoxMinC(Box const& box, dim_t dimensionID) noexcept { return box.min(dimensionID); }
      static Scalar GetBoxMaxC(Box const& box, dim_t dimensionID) noexcept { return box.max(dimensionID); }
      static void SetBoxMinC(Box& box, dim_t dimensionID, Scalar value) noexcept
      {
        if (dimensionID == 0)
          box = Box(value, box.ymin(), box.xmax(), box.ymax());
        else
          box = Box(box.xmin(), value, box.xmax(), box.ymax());
      }
      static void SetBoxMaxC(Box& box, dim_t dimensionID, Scalar value) noexcept
      {
        if (dimensionID == 0)
          box = Box(box.xmin(), box.ymin(), value, box.ymax());
        else
          box = Box(box.xmin(), box.ymin(), box.xmax(), value);
      }

      static Vector GetRayDirection(Ray const& ray) noexcept
      {
        auto const direction = ray.to_vector();
        return Vector(direction.x(), direction.y());
      }
      static Vector GetRayOrigin(Ray const& ray) noexcept { return ray.source(); }

      static Vector const& GetPlaneNormal(Plane const& plane) noexcept { return plane.normal; }
      static Scalar GetPlaneOrigoDistance(Plane const& plane) noexcept { return -plane.offset; }
    };

    using CGALGeometryAdapter2D = GeneralGeometryAdapter<BaseGeometryAdapter2D>;


    struct BaseGeometryAdapter3D
    {
      using Scalar = double;
      using FloatScalar = double;
      using Vector = CGAL::Point_3<CGAL::Cartesian<Scalar>>;
      using Box = CGAL::Bbox_3;
      using Ray = CGAL::Ray_3<CGAL::Cartesian<Scalar>>;
      using Plane = CGAL::Plane_3<CGAL::Cartesian<Scalar>>;

      static constexpr dim_t DIMENSION_NO = 2;
      static constexpr FloatScalar BASE_TOLERANCE = std::numeric_limits<FloatScalar>::epsilon() * FloatScalar(10);

      static Scalar GetPointC(Vector const& point, dim_t dimensionID) noexcept { return point[dimensionID]; }
      static void SetPointC(Vector& point, dim_t dimensionID, Scalar value) noexcept
      {
        switch (dimensionID)
        {
        case 0: point = Vector(value, point.y(), point.z()); return;
        case 1: point = Vector(point.x(), value, point.z()); return;
        case 2: point = Vector(point.x(), point.y(), value); return;
        }
        assert(false);
        std::terminate();
      }

      static Scalar GetBoxMinC(Box const& box, dim_t dimensionID) noexcept { return box.min(dimensionID); }
      static Scalar GetBoxMaxC(Box const& box, dim_t dimensionID) noexcept { return box.max(dimensionID); }
      static void SetBoxMinC(Box& box, dim_t dimensionID, Scalar value) noexcept
      {
        switch (dimensionID)
        {
        case 0: box = Box(value, box.ymin(), box.zmin(), box.xmax(), box.ymax(), box.zmax()); return;
        case 1: box = Box(box.xmin(), value, box.zmin(), box.xmax(), box.ymax(), box.zmax()); return;
        case 2: box = Box(box.xmin(), box.ymin(), value, box.xmax(), box.ymax(), box.zmax()); return;
        }
        assert(false);
        std::terminate();
      }

      static void SetBoxMaxC(Box& box, dim_t dimensionID, Scalar value) noexcept
      {
        switch (dimensionID)
        {
        case 0: box = Box(box.xmin(), box.ymin(), box.zmin(), value, box.ymax(), box.zmax()); return;
        case 1: box = Box(box.xmin(), box.ymin(), box.zmin(), box.xmax(), value, box.zmax()); return;
        case 2: box = Box(box.xmin(), box.ymin(), box.zmin(), box.xmax(), box.ymax(), value); return;
        }
        assert(false);
        std::terminate();
      }

      static Vector GetRayDirection(Ray const& ray) noexcept
      {
        auto const direction = ray.to_vector();
        return Vector(direction.x(), direction.y(), direction.z());
      }
      static Vector GetRayOrigin(Ray const& ray) noexcept { return ray.source(); }

      static Vector GetPlaneNormal(Plane const& plane) noexcept { return Vector(plane.a(), plane.b(), plane.c()); }
      static Scalar GetPlaneOrigoDistance(Plane const& plane) noexcept { return -plane.d(); }
    };

    using CGALGeometryAdapter3D = GeneralGeometryAdapter<BaseGeometryAdapter3D>;
  } // namespace CGALAdapter
} // namespace OrthoTree

namespace CGAL
{
  using namespace OrthoTree::CGALAdapter;

  // Core types
  using QuadtreePoint =
    OrthoTree::OrthoTreeBase<OrthoTree::PointEntitySpanAdapter<CGAL::Point_2<CGAL::Cartesian<double>>>, CGALGeometryAdapter2D, OrthoTree::PointConfiguration>;

  using OctreePoint =
    OrthoTree::OrthoTreeBase<OrthoTree::PointEntitySpanAdapter<CGAL::Point_3<CGAL::Cartesian<double>>>, CGALGeometryAdapter3D, OrthoTree::PointConfiguration>;

  template<bool IS_LOOSE_TREE>
  using QuadtreeBoxs =
    OrthoTree::OrthoTreeBase<OrthoTree::BoxEntitySpanAdapter<CGAL::Bbox_2>, CGALGeometryAdapter2D, OrthoTree::BoxConfiguration<IS_LOOSE_TREE>>;

  using QuadtreeBox = QuadtreeBoxs<true>;

  template<bool IS_LOOSE_TREE>
  using OctreeBoxs =
    OrthoTree::OrthoTreeBase<OrthoTree::BoxEntitySpanAdapter<CGAL::Bbox_3>, CGALGeometryAdapter3D, OrthoTree::BoxConfiguration<IS_LOOSE_TREE>>;

  using OctreeBox = OctreeBoxs<true>;


  // Container types

  using QuadtreePointC = OrthoTree::OrthoTreeContainer<QuadtreePoint>;
  using OctreePointC = OrthoTree::OrthoTreeContainer<OctreePoint>;

  template<bool IS_LOOSE_TREE>
  using QuadtreeBoxCs = OrthoTree::OrthoTreeContainer<QuadtreeBoxs<IS_LOOSE_TREE>>;
  using QuadtreeBoxC = QuadtreeBoxCs<true>;
  template<bool IS_LOOSE_TREE>
  using OctreeBoxCs = OrthoTree::OrthoTreeContainer<OctreeBoxs<IS_LOOSE_TREE>>;
  using OctreeBoxC = OctreeBoxCs<true>;


  // Map types

  template<typename T>
  using CGALContainer = std::unordered_map<std::size_t, T>;

  // Core types
  using QuadtreePointMap =
    OrthoTree::OrthoTreeBase<OrthoTree::PointEntityMapAdapter<CGAL::Point_2<CGAL::Cartesian<double>>>, CGALGeometryAdapter2D, OrthoTree::PointConfiguration>;


  using OctreePointMap =
    OrthoTree::OrthoTreeBase<OrthoTree::PointEntityMapAdapter<CGAL::Point_3<CGAL::Cartesian<double>>>, CGALGeometryAdapter3D, OrthoTree::PointConfiguration>;


  template<bool IS_LOOSE_TREE>
  using QuadtreeBoxsMap =
    OrthoTree::OrthoTreeBase<OrthoTree::BoxEntityMapAdapter<CGAL::Bbox_2>, CGALGeometryAdapter2D, OrthoTree::BoxConfiguration<IS_LOOSE_TREE>>;

  using QuadtreeBoxMap = QuadtreeBoxs<true>;

  template<bool IS_LOOSE_TREE>
  using OctreeBoxsMap =
    OrthoTree::OrthoTreeBase<OrthoTree::BoxEntityMapAdapter<CGAL::Bbox_3>, CGALGeometryAdapter3D, OrthoTree::BoxConfiguration<IS_LOOSE_TREE>>;

  using OctreeBoxMap = OctreeBoxsMap<true>;


  // Container types

  using QuadtreePointMapC = OrthoTree::OrthoTreeContainer<QuadtreePointMap>;
  using OctreePointMapC = OrthoTree::OrthoTreeContainer<OctreePointMap>;

  template<bool IS_LOOSE_TREE>
  using QuadtreeBoxMapCs = OrthoTree::OrthoTreeContainer<QuadtreeBoxsMap<IS_LOOSE_TREE>>;
  using QuadtreeBoxMapC = QuadtreeBoxMapCs<true>;
  template<bool IS_LOOSE_TREE>
  using OctreeBoxMapCs = OrthoTree::OrthoTreeContainer<OctreeBoxsMap<IS_LOOSE_TREE>>;
  using OctreeBoxMapC = OctreeBoxMapCs<true>;
} // namespace CGAL
