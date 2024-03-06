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
// #include <glm/glm.hpp>
namespace glm
{
  // Define related elements

  template<OrthoTree::dim_t DIMENSION_NO, typename TGeometry>
  struct boxNd_t
  {
    vec<DIMENSION_NO, TGeometry> min;
    vec<DIMENSION_NO, TGeometry> max;
  };

  template<OrthoTree::dim_t DIMENSION_NO, typename TGeometry>
  struct rayNd_t
  {
    vec<DIMENSION_NO, TGeometry> origin;
    vec<DIMENSION_NO, TGeometry> direction;
  };

  template<OrthoTree::dim_t DIMENSION_NO, typename TGeometry>
  struct planeNd_t
  {
    TGeometry origo_distance;            // origo_distance = dot_product(normal, any_point)
    vec<DIMENSION_NO, TGeometry> normal; // should be normalized
  };

  using box2 = boxNd_t<2, float>;
  using ray2 = rayNd_t<2, float>;
  using plane2 = planeNd_t<2, float>;

  using box3 = boxNd_t<3, float>;
  using ray3 = rayNd_t<3, float>;
  using plane3 = planeNd_t<3, float>;

  using box4 = boxNd_t<4, float>;
  using ray4 = rayNd_t<4, float>;
  using plane4 = planeNd_t<4, float>;
} // namespace glm


namespace OrthoTree
{
  namespace GlmAdaptor
  {

    template<dim_t DIMENSION_NO, typename TGeometry = double>
    struct AdaptorGeneralBasics
    {
      using TVector = glm::vec<DIMENSION_NO, TGeometry>;
      using TBox = glm::boxNd_t<DIMENSION_NO, TGeometry>;
      using TRay = glm::rayNd_t<DIMENSION_NO, TGeometry>;
      using TPlane = glm::planeNd_t<DIMENSION_NO, TGeometry>;


      static constexpr TGeometry GetPointC(TVector const& point, dim_t dimensionID) noexcept { return point[dimensionID]; }
      static constexpr void SetPointC(TVector& point, dim_t dimensionID, TGeometry value) noexcept { point[dimensionID] = value; }

      static constexpr TGeometry GetBoxMinC(TBox const& box, dim_t dimensionID) noexcept { return GetPointC(box.min, dimensionID); }
      static constexpr TGeometry GetBoxMaxC(TBox const& box, dim_t dimensionID) noexcept { return GetPointC(box.max, dimensionID); }
      static constexpr void SetBoxMinC(TBox& box, dim_t dimensionID, TGeometry value) noexcept { SetPointC(box.min, dimensionID, value); }
      static constexpr void SetBoxMaxC(TBox& box, dim_t dimensionID, TGeometry value) noexcept { SetPointC(box.max, dimensionID, value); }

      static constexpr TVector const& GetRayDirection(TRay const& ray) noexcept { return ray.direction; }
      static constexpr TVector const& GetRayOrigin(TRay const& ray) noexcept { return ray.origin; }

      static constexpr TVector const& GetPlaneNormal(TPlane const& plane) noexcept { return plane.normal; }
      static constexpr TGeometry GetPlaneOrigoDistance(TPlane const& plane) noexcept { return plane.origo_distance; }
    };

    template<dim_t DIMENSION_NO, typename TGeometry>
    using GlmAdaptorGeneral = AdaptorGeneralBase<
      DIMENSION_NO,
      glm::vec<DIMENSION_NO, TGeometry>,
      glm::boxNd_t<DIMENSION_NO, TGeometry>,
      glm::rayNd_t<DIMENSION_NO, TGeometry>,
      glm::planeNd_t<DIMENSION_NO, TGeometry>,
      TGeometry,
      AdaptorGeneralBasics<DIMENSION_NO, TGeometry>>;

    template<dim_t DIMENSION_NO, typename TGeometry>
    using GlmOrthoTreePoint = OrthoTreePoint<
      DIMENSION_NO,
      glm::vec<DIMENSION_NO, TGeometry>,
      glm::boxNd_t<DIMENSION_NO, TGeometry>,
      glm::rayNd_t<DIMENSION_NO, TGeometry>,
      glm::planeNd_t<DIMENSION_NO, TGeometry>,
      TGeometry,
      GlmAdaptorGeneral<DIMENSION_NO, TGeometry>>;

    template<dim_t DIMENSION_NO, uint32_t SPLIT_DEPTH_INCREASEMENT, typename TGeometry>
    using GlmOrthoTreeBoundingBox = OrthoTreeBoundingBox<
      DIMENSION_NO,
      glm::vec<DIMENSION_NO, TGeometry>,
      glm::boxNd_t<DIMENSION_NO, TGeometry>,
      glm::rayNd_t<DIMENSION_NO, TGeometry>,
      glm::planeNd_t<DIMENSION_NO, TGeometry>,
      TGeometry,
      SPLIT_DEPTH_INCREASEMENT,
      GlmAdaptorGeneral<DIMENSION_NO, TGeometry>>;
  } // namespace GlmAdapter
} // namespace OrthoTree

namespace glm
{
  using namespace OrthoTree::GlmAdaptor;

  // Core types

  template<int DIMENSION_NO, typename TGeometry = double>
  using orthotree_point_t = GlmOrthoTreePoint<DIMENSION_NO, TGeometry>;

  using quadtree_point_d = GlmOrthoTreePoint<2, double>;
  using quadtree_point_f = GlmOrthoTreePoint<2, float>;
  using quadtree_point_i = GlmOrthoTreePoint<2, int>;
  using quadtree_point = quadtree_point_f;

  using octree_point_d = GlmOrthoTreePoint<3, double>;
  using octree_point_f = GlmOrthoTreePoint<3, float>;
  using octree_point_i = GlmOrthoTreePoint<3, int>;
  using octree_point = octree_point_f;

  using hextree_point_d = GlmOrthoTreePoint<4, double>;
  using hextree_point_f = GlmOrthoTreePoint<4, float>;
  using hextree_point_i = GlmOrthoTreePoint<4, int>;
  using hextree_point = hextree_point_f;

  template<int DIMENSION_NO, uint32_t SPLIT_DEPTH_INCREASEMENT = 2, typename TGeometry = double>
  using orthotree_box_t = GlmOrthoTreeBoundingBox<DIMENSION_NO, SPLIT_DEPTH_INCREASEMENT, TGeometry>;

  template<uint32_t SPLIT_DEPTH_INCREASEMENT = 2>
  using quadtree_box_ds = GlmOrthoTreeBoundingBox<2, SPLIT_DEPTH_INCREASEMENT, double>;
  using quadtree_box_d = GlmOrthoTreeBoundingBox<2, 2, double>;
  using quadtree_box_f = GlmOrthoTreeBoundingBox<2, 2, float>;
  using quadtree_box_i = GlmOrthoTreeBoundingBox<2, 2, int>;
  using quadtree_box = quadtree_box_f;

  template<uint32_t SPLIT_DEPTH_INCREASEMENT = 2>
  using octree_box_ds = GlmOrthoTreeBoundingBox<3, SPLIT_DEPTH_INCREASEMENT, double>;
  using octree_box_d = GlmOrthoTreeBoundingBox<3, 2, double>;
  using octree_box_f = GlmOrthoTreeBoundingBox<3, 2, float>;
  using octree_box_i = GlmOrthoTreeBoundingBox<3, 2, int>;
  using octree_box = octree_box_f;

  template<uint32_t SPLIT_DEPTH_INCREASEMENT = 2>
  using hextree_box_ds = GlmOrthoTreeBoundingBox<4, SPLIT_DEPTH_INCREASEMENT, double>;
  using hextree_box_d = GlmOrthoTreeBoundingBox<4, 2, double>;
  using hextree_box_f = GlmOrthoTreeBoundingBox<4, 2, float>;
  using hextree_box_i = GlmOrthoTreeBoundingBox<4, 2, int>;
  using hextree_box = hextree_box_f;

  // Container types

  template<int DIMENSION_NO, typename TGeometry = double>
  using orthotree_point_c_t = OrthoTree::OrthoTreeContainerPoint<orthotree_point_t<DIMENSION_NO, TGeometry>, vec<DIMENSION_NO, TGeometry>>;

  using quadtree_point_c_d = orthotree_point_c_t<2, double>;
  using quadtree_point_c_f = orthotree_point_c_t<2, float>;
  using quadtree_point_c_i = orthotree_point_c_t<2, int>;
  using quadtree_point_c = quadtree_point_c_d;

  using octree_point_c_d = orthotree_point_c_t<3, double>;
  using octree_point_c_f = orthotree_point_c_t<3, float>;
  using octree_point_c_i = orthotree_point_c_t<3, int>;
  using octree_point_c = octree_point_c_f;

  using hextree_point_c_d = orthotree_point_c_t<4, double>;
  using hextree_point_c_f = orthotree_point_c_t<4, float>;
  using hextree_point_c_i = orthotree_point_c_t<4, int>;
  using hextree_point_c = hextree_point_c_f;


  template<int DIMENSION_NO, uint32_t SPLIT_DEPTH_INCREASEMENT = 2, typename TGeometry = double>
  using orthotree_box_c_t =
    OrthoTree::OrthoTreeContainerBox<orthotree_box_t<DIMENSION_NO, SPLIT_DEPTH_INCREASEMENT, TGeometry>, boxNd_t<DIMENSION_NO, TGeometry>>;

  template<uint32_t SPLIT_DEPTH_INCREASEMENT = 2>
  using quadtree_box_c_ds = orthotree_box_c_t<2, SPLIT_DEPTH_INCREASEMENT, double>;
  using quadtree_box_c_d = orthotree_box_c_t<2, 2, double>;
  using quadtree_box_c_f = orthotree_box_c_t<2, 2, float>;
  using quadtree_box_c_i = orthotree_box_c_t<2, 2, int>;
  using quadtree_box_c = quadtree_box_c_f;

  template<uint32_t SPLIT_DEPTH_INCREASEMENT = 2>
  using octree_box_c_ds = orthotree_box_c_t<3, SPLIT_DEPTH_INCREASEMENT, double>;
  using octree_box_c_d = orthotree_box_c_t<3, 2, double>;
  using octree_box_c_f = orthotree_box_c_t<3, 2, float>;
  using octree_box_c_i = orthotree_box_c_t<3, 2, int>;
  using octree_box_c = octree_box_c_f;

  template<uint32_t SPLIT_DEPTH_INCREASEMENT = 2>
  using hextree_box_c_ds = orthotree_box_c_t<4, SPLIT_DEPTH_INCREASEMENT, double>;
  using hextree_box_c_d = orthotree_box_c_t<4, 2, double>;
  using hextree_box_c_f = orthotree_box_c_t<4, 2, float>;
  using hextree_box_c_i = orthotree_box_c_t<4, 2, int>;
  using hextree_box_c = hextree_box_c_f;
} // namespace boost::geometry