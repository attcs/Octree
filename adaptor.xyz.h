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
} // namespace


namespace OrthoTree
{
  namespace XYAdaptor2D
  {
    using xy_geometry_type = BasicTypesXYZ::float_t;
    using XYPoint2D = BasicTypesXYZ::Point2D;
    using XYBoundingBox2D = BasicTypesXYZ::BoundingBox2D;

    struct XYAdaptorBasics
    {
      static constexpr xy_geometry_type point_comp_c(XYPoint2D const& pt, dim_t dimensionID)
      {
        switch (dimensionID)
        {
        case 0: return pt.x;
        case 1: return pt.y;
        default: assert(false); std::terminate();
        }
      }

      static constexpr void point_comp_set(XYPoint2D& pt, dim_t dimensionID, xy_geometry_type value)
      {
        switch (dimensionID)
        {
        case 0: pt.x = value; break;
        case 1: pt.y = value; break;
        default: assert(false); std::terminate();
        }
      }


      static constexpr void box_min_comp_set(XYBoundingBox2D& box, dim_t dimensionID, xy_geometry_type value) { point_comp_set(box.Min, dimensionID, value); }
      static constexpr void box_max_comp_set(XYBoundingBox2D& box, dim_t dimensionID, xy_geometry_type value) { point_comp_set(box.Max, dimensionID, value); }
      static constexpr xy_geometry_type box_min_comp(XYBoundingBox2D const& box, dim_t dimensionID) { return point_comp_c(box.Min, dimensionID); }
      static constexpr xy_geometry_type box_max_comp(XYBoundingBox2D const& box, dim_t dimensionID) { return point_comp_c(box.Max, dimensionID); }
    };

    using XYAdaptorGeneral = AdaptorGeneralBase<2, XYPoint2D, XYBoundingBox2D, XYAdaptorBasics, xy_geometry_type>;
  } // namespace XYAdaptor2D


  namespace XYZAdaptor3D
  {
    using xyz_geometry_type = BasicTypesXYZ::float_t;
    using XYZPoint3D = BasicTypesXYZ::Point3D;
    using XYZBoundingBox3D = BasicTypesXYZ::BoundingBox3D;

    struct XYZAdaptorBasics
    {
      static constexpr xyz_geometry_type point_comp_c(XYZPoint3D const& pt, dim_t dimensionID) noexcept
      {
        switch (dimensionID)
        {
        case 0: return pt.x;
        case 1: return pt.y;
        case 2: return pt.z;
        default: assert(false); std::terminate();
        }
      }

      static constexpr void point_comp_set(XYZPoint3D& pt, dim_t dimensionID, xyz_geometry_type value) noexcept
      {
        switch (dimensionID)
        {
        case 0: pt.x = value; break;
        case 1: pt.y = value; break;
        case 2: pt.z = value; break;
        default: assert(false); std::terminate();
        }
      }

      static constexpr void box_min_comp_set(XYZBoundingBox3D& box, dim_t dimensionID, xyz_geometry_type value) { point_comp_set(box.Min, dimensionID, value); }
      static constexpr void box_max_comp_set(XYZBoundingBox3D& box, dim_t dimensionID, xyz_geometry_type value) { point_comp_set(box.Max, dimensionID, value); }
      static constexpr xyz_geometry_type box_min_comp(XYZBoundingBox3D const& box, dim_t dimensionID) { return point_comp_c(box.Min, dimensionID); }
      static constexpr xyz_geometry_type box_max_comp(XYZBoundingBox3D const& box, dim_t dimensionID) { return point_comp_c(box.Max, dimensionID); }
    };

    using XYZAdaptorGeneral = AdaptorGeneralBase<3, XYZPoint3D, XYZBoundingBox3D, XYZAdaptorBasics, xyz_geometry_type>;


  } // namespace XYAdaptor2D
}

namespace XYZ
{
  using namespace OrthoTree;
  using namespace OrthoTree::XYAdaptor2D;
  using namespace OrthoTree::XYZAdaptor3D;

  using QuadtreePoint = OrthoTreePoint<2, XYPoint2D, XYBoundingBox2D, XYAdaptorGeneral, xy_geometry_type>;

  template<uint32_t SPLIT_DEPTH_INCREASEMENT = 2>
  using QuadtreeBox = OrthoTreeBoundingBox<2, XYPoint2D, XYBoundingBox2D, XYAdaptorGeneral, xy_geometry_type, SPLIT_DEPTH_INCREASEMENT>;

  using QuadtreePointC = OrthoTreeContainerPoint<QuadtreePoint, XYPoint2D>;

  template<uint32_t SPLIT_DEPTH_INCREASEMENT = 2>
  using QuadtreeBoxC = OrthoTreeContainerBox<QuadtreeBox<SPLIT_DEPTH_INCREASEMENT>, XYBoundingBox2D>;

  
  using OctreePoint = OrthoTreePoint<3, XYZPoint3D, XYZBoundingBox3D, XYZAdaptorGeneral, xyz_geometry_type>;
  
  template<uint32_t SPLIT_DEPTH_INCREASEMENT = 2>
  using OctreeBox =
    OrthoTreeBoundingBox<3, XYZPoint3D, XYZBoundingBox3D, XYZAdaptorGeneral, xyz_geometry_type, SPLIT_DEPTH_INCREASEMENT>;
  

  using OcreePointC = OrthoTreeContainerPoint<OctreePoint, XYZPoint3D>;

  template<uint32_t SPLIT_DEPTH_INCREASEMENT = 2>
  using OctreeBoxC = OrthoTreeContainerBox<QuadtreeBox<SPLIT_DEPTH_INCREASEMENT>, XYZBoundingBox3D>;
}