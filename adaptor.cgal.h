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

#include "octree.h"
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
  namespace CGALAdaptor
  {
    struct AdaptorGeneralBasics2D
    {
      using TGeometry = double;
      using TVector = CGAL::Point_2<CGAL::Cartesian<TGeometry>>;
      using TBox = CGAL::Bbox_2;
      using TRay = CGAL::Ray_2<CGAL::Cartesian<TGeometry>>;
      using TPlane = CGAL::Plane_2;

      static inline TGeometry GetPointC(TVector const& point, dim_t dimensionID) noexcept { return point[dimensionID]; }
      static inline void SetPointC(TVector& point, dim_t dimensionID, TGeometry value) noexcept
      {
        if (dimensionID == 0)
          point = TVector(value, point.y());
        else
          point = TVector(point.x(), value);
      }

      static inline TGeometry GetBoxMinC(TBox const& box, dim_t dimensionID) noexcept { return box.min(dimensionID); }
      static inline TGeometry GetBoxMaxC(TBox const& box, dim_t dimensionID) noexcept { return box.max(dimensionID); }
      static inline void SetBoxMinC(TBox& box, dim_t dimensionID, TGeometry value) noexcept
      {
        if (dimensionID == 0)
          box = TBox(value, box.ymin(), box.xmax(), box.ymax());
        else
          box = TBox(box.xmin(), value, box.xmax(), box.ymax());
      }
      static inline void SetBoxMaxC(TBox& box, dim_t dimensionID, TGeometry value) noexcept
      {
        if (dimensionID == 0)
          box = TBox(box.xmin(), box.ymin(), value, box.ymax());
        else
          box = TBox(box.xmin(), box.ymin(), box.xmax(), value);
      }

      static inline TVector GetRayDirection(TRay const& ray) noexcept
      {
        auto const direction = ray.to_vector();
        return TVector(direction.x(), direction.y());
      }
      static inline TVector GetRayOrigin(TRay const& ray) noexcept { return ray.source(); }

      static inline TVector const& GetPlaneNormal(TPlane const& plane) noexcept { return plane.normal; }
      static inline TGeometry GetPlaneOrigoDistance(TPlane const& plane) noexcept { return -plane.offset; }
    };

    using CGALAdaptorGeneral2D =
      AdaptorGeneralBase<2, CGAL::Point_2<CGAL::Cartesian<double>>, CGAL::Bbox_2, CGAL::Ray_2<CGAL::Cartesian<double>>, CGAL::Plane_2, double, AdaptorGeneralBasics2D>;


    struct AdaptorGeneralBasics3D
    {
      using TGeometry = double;
      using TVector = CGAL::Point_3<CGAL::Cartesian<TGeometry>>;
      using TBox = CGAL::Bbox_3;
      using TRay = CGAL::Ray_3<CGAL::Cartesian<TGeometry>>;
      using TPlane = CGAL::Plane_3<CGAL::Cartesian<TGeometry>>;

      static inline TGeometry GetPointC(TVector const& point, dim_t dimensionID) noexcept { return point[dimensionID]; }
      static inline void SetPointC(TVector& point, dim_t dimensionID, TGeometry value) noexcept
      {
        switch (dimensionID)
        {
        case 0: point = TVector(value, point.y(), point.z()); return;
        case 1: point = TVector(point.x(), value, point.z()); return;
        case 2: point = TVector(point.x(), point.y(), value); return;
        }
        assert(false);
        std::terminate();
      }

      static inline TGeometry GetBoxMinC(TBox const& box, dim_t dimensionID) noexcept { return box.min(dimensionID); }
      static inline TGeometry GetBoxMaxC(TBox const& box, dim_t dimensionID) noexcept { return box.max(dimensionID); }
      static inline void SetBoxMinC(TBox& box, dim_t dimensionID, TGeometry value) noexcept
      {
        switch (dimensionID)
        {
        case 0: box = TBox(value, box.ymin(), box.zmin(), box.xmax(), box.ymax(), box.zmax()); return;
        case 1: box = TBox(box.xmin(), value, box.zmin(), box.xmax(), box.ymax(), box.zmax()); return;
        case 2: box = TBox(box.xmin(), box.ymin(), value, box.xmax(), box.ymax(), box.zmax()); return;
        }
        assert(false);
        std::terminate();
      }

      static inline void SetBoxMaxC(TBox& box, dim_t dimensionID, TGeometry value) noexcept
      {
        switch (dimensionID)
        {
        case 0: box = TBox(box.xmin(), box.ymin(), box.zmin(), value, box.ymax(), box.zmax()); return;
        case 1: box = TBox(box.xmin(), box.ymin(), box.zmin(), box.xmax(), value, box.zmax()); return;
        case 2: box = TBox(box.xmin(), box.ymin(), box.zmin(), box.xmax(), box.ymax(), value); return;
        }
        assert(false);
        std::terminate();
      }

      static inline TVector GetRayDirection(TRay const& ray) noexcept
      {
        auto const direction = ray.to_vector();
        return TVector(direction.x(), direction.y(), direction.z());
      }
      static inline TVector GetRayOrigin(TRay const& ray) noexcept { return ray.source(); }

      static inline TVector GetPlaneNormal(TPlane const& plane) noexcept { return TVector(plane.a(), plane.b(), plane.c()); }
      static inline TGeometry GetPlaneOrigoDistance(TPlane const& plane) noexcept { return -plane.d(); }
    };
    using CGALAdaptorGeneral3D =
      AdaptorGeneralBase<3, CGAL::Point_3<CGAL::Cartesian<double>>, CGAL::Bbox_3, CGAL::Ray_3<CGAL::Cartesian<double>>, CGAL::Plane_3<CGAL::Cartesian<double>>, double, AdaptorGeneralBasics3D>;


  } // namespace CGALAdaptor
} // namespace OrthoTree

namespace CGAL
{
  using namespace OrthoTree::CGALAdaptor;

  // Core types
  using QuadtreePoint =
    OrthoTree::OrthoTreePoint<2, CGAL::Point_2<CGAL::Cartesian<double>>, CGAL::Bbox_2, CGAL::Ray_2<CGAL::Cartesian<double>>, CGAL::Plane_2, double, CGALAdaptorGeneral2D>;

  using OctreePoint = OrthoTree::
    OrthoTreePoint<3, CGAL::Point_3<CGAL::Cartesian<double>>, CGAL::Bbox_3, CGAL::Ray_3<CGAL::Cartesian<double>>, CGAL::Plane_3<CGAL::Cartesian<double>>, double, CGALAdaptorGeneral3D>;

  template<int SPLIT_DEPTH_INCREASEMENT>
  using QuadtreeBoxs =
    OrthoTree::OrthoTreeBoundingBox<2, CGAL::Point_2<CGAL::Cartesian<double>>, CGAL::Bbox_2, CGAL::Ray_2<CGAL::Cartesian<double>>, CGAL::Plane_2, double, SPLIT_DEPTH_INCREASEMENT, CGALAdaptorGeneral2D>;

  using QuadtreeBox = QuadtreeBoxs<2>;

  template<int SPLIT_DEPTH_INCREASEMENT>
  using OctreeBoxs = OrthoTree::OrthoTreeBoundingBox<
    3,
    CGAL::Point_3<CGAL::Cartesian<double>>,
    CGAL::Bbox_3,
    CGAL::Ray_3<CGAL::Cartesian<double>>,
    CGAL::Plane_3<CGAL::Cartesian<double>>,
    double,
    SPLIT_DEPTH_INCREASEMENT,
    CGALAdaptorGeneral3D>;

  using OctreeBox = OctreeBoxs<2>;


  // Container types

  using QuadtreePointC = OrthoTree::OrthoTreeContainerPoint<QuadtreePoint, Point_2<Cartesian<double>>>;
  using OctreePointC = OrthoTree::OrthoTreeContainerPoint<OctreePoint, Point_3<Cartesian<double>>>;

  template<int SPLIT_DEPTH_INCREASEMENT>
  using QuadtreeBoxCs = OrthoTree::OrthoTreeContainerBox<QuadtreeBoxs<SPLIT_DEPTH_INCREASEMENT>, Bbox_2>;
  using QuadtreeBoxC = QuadtreeBoxCs<2>;
  template<int SPLIT_DEPTH_INCREASEMENT>
  using OctreeBoxCs = OrthoTree::OrthoTreeContainerBox<OctreeBoxs<SPLIT_DEPTH_INCREASEMENT>, Bbox_3>;
  using OctreeBoxC = OctreeBoxCs<2>;
} // namespace CGAL