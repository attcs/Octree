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


namespace BasicTypesXYZ // Replaceble with simirarly build geometry types
{
  using float_t = float;

  struct Point2D
  {
    float_t x;
    float_t y;
  };

  struct Point3D
  {
    float_t x;
    float_t y;
    float_t z;
  };

  struct BoundingBox2D
  {
    Point2D Min;
    Point2D Max;
  };

  struct BoundingBox3D
  {
    Point3D Min;
    Point3D Max;
  };

  struct Ray2D
  {
    Point2D BasePoint;
    Point2D Heading;
  };

  struct Ray3D
  {
    Point3D BasePoint;
    Point3D Heading;
  };

  struct Plane2D
  {
    float_t OrigoDistance;
    Point2D Normal;
  };

  struct Plane3D
  {
    float_t OrigoDistance;
    Point3D Normal;
  };
} // namespace BasicTypesXYZ


namespace OrthoTree
{
  namespace XYAdaptor2D
  {
    using xy_geometry_type = BasicTypesXYZ::float_t;
    using XYPoint2D = BasicTypesXYZ::Point2D;
    using XYBoundingBox2D = BasicTypesXYZ::BoundingBox2D;
    using XYRay2D = BasicTypesXYZ::Ray2D;
    using XYPlane2D = BasicTypesXYZ::Plane2D;

    struct XYAdaptorBasics
    {
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


      static constexpr void SetBoxMinC(XYBoundingBox2D& box, dim_t dimensionID, xy_geometry_type value) { SetPointC(box.Min, dimensionID, value); }
      static constexpr void SetBoxMaxC(XYBoundingBox2D& box, dim_t dimensionID, xy_geometry_type value) { SetPointC(box.Max, dimensionID, value); }
      static constexpr xy_geometry_type GetBoxMinC(XYBoundingBox2D const& box, dim_t dimensionID) { return GetPointC(box.Min, dimensionID); }
      static constexpr xy_geometry_type GetBoxMaxC(XYBoundingBox2D const& box, dim_t dimensionID) { return GetPointC(box.Max, dimensionID); }

      static constexpr XYPoint2D const& GetRayDirection(XYRay2D const& ray) noexcept { return ray.Heading; }
      static constexpr XYPoint2D const& GetRayOrigin(XYRay2D const& ray) noexcept { return ray.BasePoint; }

      static constexpr XYPoint2D const& GetPlaneNormal(XYPlane2D const& plane) noexcept { return plane.Normal; }
      static constexpr xy_geometry_type GetPlaneOrigoDistance(XYPlane2D const& plane) noexcept { return plane.OrigoDistance; }
    };

    using XYAdaptorGeneral = AdaptorGeneralBase<2, XYPoint2D, XYBoundingBox2D, XYRay2D, XYPlane2D, xy_geometry_type, XYAdaptorBasics>;
  } // namespace XYAdaptor2D


  namespace XYZAdaptor3D
  {
    using xyz_geometry_type = BasicTypesXYZ::float_t;
    using XYZPoint3D = BasicTypesXYZ::Point3D;
    using XYZBoundingBox3D = BasicTypesXYZ::BoundingBox3D;
    using XYZRay3D = BasicTypesXYZ::Ray3D;
    using XYZPlane3D = BasicTypesXYZ::Plane3D;

    struct XYZAdaptorBasics
    {
      static constexpr xyz_geometry_type GetPointC(XYZPoint3D const& pt, dim_t dimensionID) noexcept
      {
        switch (dimensionID)
        {
        case 0: return pt.x;
        case 1: return pt.y;
        case 2: return pt.z;
        default: assert(false); std::terminate();
        }
      }

      static constexpr void SetPointC(XYZPoint3D& pt, dim_t dimensionID, xyz_geometry_type value) noexcept
      {
        switch (dimensionID)
        {
        case 0: pt.x = value; break;
        case 1: pt.y = value; break;
        case 2: pt.z = value; break;
        default: assert(false); std::terminate();
        }
      }

      static constexpr void SetBoxMinC(XYZBoundingBox3D& box, dim_t dimensionID, xyz_geometry_type value) { SetPointC(box.Min, dimensionID, value); }
      static constexpr void SetBoxMaxC(XYZBoundingBox3D& box, dim_t dimensionID, xyz_geometry_type value) { SetPointC(box.Max, dimensionID, value); }
      static constexpr xyz_geometry_type GetBoxMinC(XYZBoundingBox3D const& box, dim_t dimensionID) { return GetPointC(box.Min, dimensionID); }
      static constexpr xyz_geometry_type GetBoxMaxC(XYZBoundingBox3D const& box, dim_t dimensionID) { return GetPointC(box.Max, dimensionID); }

      static constexpr XYZPoint3D const& GetRayDirection(XYZRay3D const& ray) noexcept { return ray.Heading; }
      static constexpr XYZPoint3D const& GetRayOrigin(XYZRay3D const& ray) noexcept { return ray.BasePoint; }

      static constexpr XYZPoint3D const& GetPlaneNormal(XYZPlane3D const& plane) noexcept { return plane.Normal; }
      static constexpr xyz_geometry_type GetPlaneOrigoDistance(XYZPlane3D const& plane) noexcept { return plane.OrigoDistance; }
    };

    using XYZAdaptorGeneral = AdaptorGeneralBase<3, XYZPoint3D, XYZBoundingBox3D, XYZRay3D, XYZPlane3D, xyz_geometry_type, XYZAdaptorBasics>;


  } // namespace XYZAdaptor3D
} // namespace OrthoTree

namespace XYZ
{
  using namespace OrthoTree;
  using namespace OrthoTree::XYAdaptor2D;
  using namespace OrthoTree::XYZAdaptor3D;

  using QuadtreePoint = OrthoTreePoint<2, XYPoint2D, XYBoundingBox2D, XYRay2D, XYPlane2D, xy_geometry_type, XYAdaptorGeneral>;

  template<uint32_t SPLIT_DEPTH_INCREASEMENT = 2>
  using QuadtreeBox =
    OrthoTreeBoundingBox<2, XYPoint2D, XYBoundingBox2D, XYRay2D, XYPlane2D, xy_geometry_type, SPLIT_DEPTH_INCREASEMENT, XYAdaptorGeneral>;

  using QuadtreePointC = OrthoTreeContainerPoint<QuadtreePoint, XYPoint2D>;

  template<uint32_t SPLIT_DEPTH_INCREASEMENT = 2>
  using QuadtreeBoxCs = OrthoTreeContainerBox<QuadtreeBox<SPLIT_DEPTH_INCREASEMENT>, XYBoundingBox2D>;
  using QuadtreeBoxC = QuadtreeBoxCs<2>;

  using OctreePoint = OrthoTreePoint<3, XYZPoint3D, XYZBoundingBox3D, XYZRay3D, XYZPlane3D, xyz_geometry_type, XYZAdaptorGeneral>;

  template<uint32_t SPLIT_DEPTH_INCREASEMENT = 2>
  using OctreeBox =
    OrthoTreeBoundingBox<3, XYZPoint3D, XYZBoundingBox3D, XYZRay3D, XYZPlane3D, xyz_geometry_type, SPLIT_DEPTH_INCREASEMENT, XYZAdaptorGeneral>;


  using OcreePointC = OrthoTreeContainerPoint<OctreePoint, XYZPoint3D>;

  template<uint32_t SPLIT_DEPTH_INCREASEMENT = 2>
  using OctreeBoxCs = OrthoTreeContainerBox<QuadtreeBox<SPLIT_DEPTH_INCREASEMENT>, XYZBoundingBox3D>;
  using OctreeBoxC = OctreeBoxCs<2>;

} // namespace XYZ