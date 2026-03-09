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
// Override the GLM include if your GLM installation is in a non-standard location.
// Example: -DORTHOTREE_GLM_INCLUDE="<my_vendor/glm/glm.hpp>"
#ifndef ORTHOTREE_GLM_INCLUDE
#define ORTHOTREE_GLM_INCLUDE <glm/glm.hpp>
#endif

#include ORTHOTREE_GLM_INCLUDE

namespace glm
{
  // Define related elements

  template<OrthoTree::dim_t DIMENSION_NO, typename TScalar>
  struct boxNd_t
  {
    vec<DIMENSION_NO, TScalar> min;
    vec<DIMENSION_NO, TScalar> max;
  };

  template<OrthoTree::dim_t DIMENSION_NO, typename TScalar>
  struct rayNd_t
  {
    vec<DIMENSION_NO, TScalar> origin;
    vec<DIMENSION_NO, TScalar> direction;
  };

  template<OrthoTree::dim_t DIMENSION_NO, typename TScalar>
  struct planeNd_t
  {
    TScalar origo_distance;            // origo_distance = dot_product(normal, any_point)
    vec<DIMENSION_NO, TScalar> normal; // should be normalized
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

    template<dim_t DIMENSION_NO_, typename TScalar = double>
    struct GlmBaseGeometryAdapter
    {
      using Scalar = TScalar;
      using FloatScalar = std::conditional_t<std::is_floating_point_v<TScalar>, TScalar, float>;
      using Vector = glm::vec<DIMENSION_NO_, TScalar>;
      using Box = glm::boxNd_t<DIMENSION_NO_, TScalar>;
      using Ray = glm::rayNd_t<DIMENSION_NO_, TScalar>;
      using Plane = glm::planeNd_t<DIMENSION_NO_, TScalar>;

      static constexpr dim_t DIMENSION_NO = DIMENSION_NO_;
      static constexpr FloatScalar BASE_TOLERANCE = std::numeric_limits<FloatScalar>::epsilon() * FloatScalar(10);

      static constexpr Vector MakePoint() noexcept { return {}; };
      static constexpr Box MakeBox() noexcept { return {}; };

      static constexpr TScalar GetPointC(Vector const& point, dim_t dimensionID) noexcept { return point[dimensionID]; }
      static constexpr void SetPointC(Vector& point, dim_t dimensionID, TScalar value) noexcept { point[dimensionID] = value; }

      static constexpr TScalar GetBoxMinC(Box const& box, dim_t dimensionID) noexcept { return GetPointC(box.min, dimensionID); }
      static constexpr TScalar GetBoxMaxC(Box const& box, dim_t dimensionID) noexcept { return GetPointC(box.max, dimensionID); }
      static constexpr void SetBoxMinC(Box& box, dim_t dimensionID, TScalar value) noexcept { SetPointC(box.min, dimensionID, value); }
      static constexpr void SetBoxMaxC(Box& box, dim_t dimensionID, TScalar value) noexcept { SetPointC(box.max, dimensionID, value); }

      static constexpr Vector const& GetRayDirection(Ray const& ray) noexcept { return ray.direction; }
      static constexpr Vector const& GetRayOrigin(Ray const& ray) noexcept { return ray.origin; }

      static constexpr Vector const& GetPlaneNormal(Plane const& plane) noexcept { return plane.normal; }
      static constexpr TScalar GetPlaneOrigoDistance(Plane const& plane) noexcept { return plane.origo_distance; }
    };

    template<dim_t DIMENSION_NO, typename TScalar>
    using GlmAdaptorGeneral = GeneralGeometryAdapter<GlmBaseGeometryAdapter<DIMENSION_NO, TScalar>>;

    template<dim_t DIMENSION_NO, typename TScalar>
    using GlmOrthoTreePoint =
      OrthoTreeBase<PointEntitySpanAdapter<glm::vec<DIMENSION_NO, TScalar>>, GlmAdaptorGeneral<DIMENSION_NO, TScalar>, PointConfiguration<>>;

    template<dim_t DIMENSION_NO, bool IS_LOOSE_TREE, typename TScalar>
    using GlmOrthoTreeBoundingBox =
      OrthoTreeBase<BoxEntitySpanAdapter<glm::boxNd_t<DIMENSION_NO, TScalar>>, GlmAdaptorGeneral<DIMENSION_NO, TScalar>, BoxConfiguration<IS_LOOSE_TREE>>;

    template<dim_t DIMENSION_NO, typename TScalar, typename TContainer = std::unordered_map<index_t, glm::vec<DIMENSION_NO, TScalar>>>
    using GlmOrthoTreePointMap =
      OrthoTreeBase<PointEntityMapAdapter<glm::vec<DIMENSION_NO, TScalar>, TContainer>, GlmAdaptorGeneral<DIMENSION_NO, TScalar>, PointConfiguration<>>;

    template<dim_t DIMENSION_NO, bool IS_LOOSE_TREE, typename TScalar, typename TContainer = std::unordered_map<index_t, glm::boxNd_t<DIMENSION_NO, TScalar>>>
    using GlmOrthoTreeBoundingBoxMap =
      OrthoTreeBase<BoxEntityMapAdapter<glm::boxNd_t<DIMENSION_NO, TScalar>, TContainer>, GlmAdaptorGeneral<DIMENSION_NO, TScalar>, BoxConfiguration<IS_LOOSE_TREE>>;


  } // namespace GlmAdaptor
} // namespace OrthoTree

namespace glm
{
  using namespace OrthoTree::GlmAdaptor;

  // Core types

  template<int DIMENSION_NO, typename TScalar = double>
  using orthotree_point_t = GlmOrthoTreePoint<DIMENSION_NO, TScalar>;

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

  template<int DIMENSION_NO, bool IS_LOOSE_TREE = true, typename TScalar = double>
  using orthotree_box_t = GlmOrthoTreeBoundingBox<DIMENSION_NO, IS_LOOSE_TREE, TScalar>;

  template<bool IS_LOOSE_TREE = true>
  using quadtree_box_ds = GlmOrthoTreeBoundingBox<2, IS_LOOSE_TREE, double>;
  using quadtree_box_d = GlmOrthoTreeBoundingBox<2, true, double>;
  using quadtree_box_f = GlmOrthoTreeBoundingBox<2, true, float>;
  using quadtree_box_i = GlmOrthoTreeBoundingBox<2, true, int>;
  using quadtree_box = quadtree_box_f;

  template<bool IS_LOOSE_TREE = true>
  using octree_box_ds = GlmOrthoTreeBoundingBox<3, IS_LOOSE_TREE, double>;
  using octree_box_d = GlmOrthoTreeBoundingBox<3, true, double>;
  using octree_box_f = GlmOrthoTreeBoundingBox<3, true, float>;
  using octree_box_i = GlmOrthoTreeBoundingBox<3, true, int>;
  using octree_box = octree_box_f;

  template<bool IS_LOOSE_TREE = true>
  using hextree_box_ds = GlmOrthoTreeBoundingBox<4, IS_LOOSE_TREE, double>;
  using hextree_box_d = GlmOrthoTreeBoundingBox<4, true, double>;
  using hextree_box_f = GlmOrthoTreeBoundingBox<4, true, float>;
  using hextree_box_i = GlmOrthoTreeBoundingBox<4, true, int>;
  using hextree_box = hextree_box_f;

  // Container types

  template<int DIMENSION_NO, typename TScalar = double>
  using orthotree_point_c_t = OrthoTree::OrthoTreeManaged<orthotree_point_t<DIMENSION_NO, TScalar>>;

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


  template<int DIMENSION_NO, bool IS_LOOSE_TREE = true, typename TScalar = double>
  using orthotree_box_c_t = OrthoTree::OrthoTreeManaged<orthotree_box_t<DIMENSION_NO, IS_LOOSE_TREE, TScalar>>;

  template<bool IS_LOOSE_TREE = true>
  using quadtree_box_c_ds = orthotree_box_c_t<2, IS_LOOSE_TREE, double>;
  using quadtree_box_c_d = orthotree_box_c_t<2, true, double>;
  using quadtree_box_c_f = orthotree_box_c_t<2, true, float>;
  using quadtree_box_c_i = orthotree_box_c_t<2, true, int>;
  using quadtree_box_c = quadtree_box_c_f;

  template<bool IS_LOOSE_TREE = true>
  using octree_box_c_ds = orthotree_box_c_t<3, IS_LOOSE_TREE, double>;
  using octree_box_c_d = orthotree_box_c_t<3, true, double>;
  using octree_box_c_f = orthotree_box_c_t<3, true, float>;
  using octree_box_c_i = orthotree_box_c_t<3, true, int>;
  using octree_box_c = octree_box_c_f;

  template<bool IS_LOOSE_TREE = true>
  using hextree_box_c_ds = orthotree_box_c_t<4, IS_LOOSE_TREE, double>;
  using hextree_box_c_d = orthotree_box_c_t<4, true, double>;
  using hextree_box_c_f = orthotree_box_c_t<4, true, float>;
  using hextree_box_c_i = orthotree_box_c_t<4, true, int>;
  using hextree_box_c = hextree_box_c_f;


  // Map types

  template<typename T>
  using GlmContainer = std::unordered_map<int, T>;

  //  Core types

  template<int DIMENSION_NO, typename TScalar = double>
  using orthotree_point_map_t = GlmOrthoTreePointMap<DIMENSION_NO, TScalar, GlmContainer<glm::vec<DIMENSION_NO, TScalar>>>;

  using quadtree_point_map_d = GlmOrthoTreePointMap<2, double, GlmContainer<glm::vec<2, double>>>;
  using quadtree_point_map_f = GlmOrthoTreePointMap<2, float, GlmContainer<glm::vec<2, float>>>;
  using quadtree_point_map_i = GlmOrthoTreePointMap<2, int, GlmContainer<glm::vec<2, int>>>;
  using quadtree_point_map = quadtree_point_map_f;

  using octree_point_map_d = GlmOrthoTreePointMap<3, double, GlmContainer<glm::vec<3, double>>>;
  using octree_point_map_f = GlmOrthoTreePointMap<3, float, GlmContainer<glm::vec<3, float>>>;
  using octree_point_map_i = GlmOrthoTreePointMap<3, int, GlmContainer<glm::vec<3, int>>>;
  using octree_point_map = octree_point_map_f;

  using hextree_point_map_d = GlmOrthoTreePointMap<4, double, GlmContainer<glm::vec<4, double>>>;
  using hextree_point_map_f = GlmOrthoTreePointMap<4, float, GlmContainer<glm::vec<4, float>>>;
  using hextree_point_map_i = GlmOrthoTreePointMap<4, int, GlmContainer<glm::vec<4, int>>>;
  using hextree_point_map = hextree_point_map_f;

  template<int DIMENSION_NO, bool IS_LOOSE_TREE = true, typename TScalar = double, typename TContainer = GlmContainer<glm::boxNd_t<DIMENSION_NO, TScalar>>>
  using orthotree_box_map_t = GlmOrthoTreeBoundingBoxMap<DIMENSION_NO, IS_LOOSE_TREE, TScalar, TContainer>;

  template<bool IS_LOOSE_TREE = true>
  using quadtree_box_map_ds = GlmOrthoTreeBoundingBoxMap<2, IS_LOOSE_TREE, double, GlmContainer<glm::boxNd_t<2, double>>>;
  using quadtree_box_map_d = GlmOrthoTreeBoundingBoxMap<2, true, double, GlmContainer<glm::boxNd_t<2, double>>>;
  using quadtree_box_map_f = GlmOrthoTreeBoundingBoxMap<2, true, float, GlmContainer<glm::boxNd_t<2, float>>>;
  using quadtree_box_map_i = GlmOrthoTreeBoundingBoxMap<2, true, int, GlmContainer<glm::boxNd_t<2, int>>>;
  using quadtree_box_map = quadtree_box_f;

  template<bool IS_LOOSE_TREE = true>
  using octree_box_map_ds = GlmOrthoTreeBoundingBoxMap<3, IS_LOOSE_TREE, double, GlmContainer<glm::boxNd_t<2, double>>>;
  using octree_box_map_d = GlmOrthoTreeBoundingBoxMap<3, true, double, GlmContainer<glm::boxNd_t<2, double>>>;
  using octree_box_map_f = GlmOrthoTreeBoundingBoxMap<3, true, float, GlmContainer<glm::boxNd_t<2, double>>>;
  using octree_box_map_i = GlmOrthoTreeBoundingBoxMap<3, true, int, GlmContainer<glm::boxNd_t<2, double>>>;
  using octree_box_map = octree_box_f;

  template<bool IS_LOOSE_TREE = true>
  using hextree_box_map_ds = GlmOrthoTreeBoundingBoxMap<4, IS_LOOSE_TREE, double, GlmContainer<glm::boxNd_t<2, double>>>;
  using hextree_box_map_d = GlmOrthoTreeBoundingBoxMap<4, true, double, GlmContainer<glm::boxNd_t<2, double>>>;
  using hextree_box_map_f = GlmOrthoTreeBoundingBoxMap<4, true, float, GlmContainer<glm::boxNd_t<2, double>>>;
  using hextree_box_map_i = GlmOrthoTreeBoundingBoxMap<4, true, int, GlmContainer<glm::boxNd_t<2, double>>>;
  using hextree_box_map = hextree_box_map_f;

  // Container types

  template<int DIMENSION_NO, typename TScalar = double>
  using orthotree_point_map_c_t =
    OrthoTree::OrthoTreeManaged<GlmOrthoTreePointMap<DIMENSION_NO, TScalar, GlmContainer<glm::vec<DIMENSION_NO, TScalar>>>>;

  using quadtree_point_map_c_d = orthotree_point_map_c_t<2, double>;
  using quadtree_point_map_c_f = orthotree_point_map_c_t<2, float>;
  using quadtree_point_map_c_i = orthotree_point_map_c_t<2, int>;
  using quadtree_point_map_c = quadtree_point_map_c_d;

  using octree_point_map_c_d = orthotree_point_map_c_t<3, double>;
  using octree_point_map_c_f = orthotree_point_map_c_t<3, float>;
  using octree_point_map_c_i = orthotree_point_map_c_t<3, int>;
  using octree_point_map_c = octree_point_map_c_f;

  using hextree_point_map_c_d = orthotree_point_map_c_t<4, double>;
  using hextree_point_map_c_f = orthotree_point_map_c_t<4, float>;
  using hextree_point_map_c_i = orthotree_point_map_c_t<4, int>;
  using hextree_point_map_c = hextree_point_map_c_f;


  template<int DIMENSION_NO, bool IS_LOOSE_TREE = true, typename TScalar = double>
  using orthotree_box_map_c_t =
    OrthoTree::OrthoTreeManaged<orthotree_box_map_t<DIMENSION_NO, IS_LOOSE_TREE, TScalar, GlmContainer<glm::boxNd_t<DIMENSION_NO, TScalar>>>>;

  template<bool IS_LOOSE_TREE = true>
  using quadtree_box_map_c_ds = orthotree_box_map_c_t<2, IS_LOOSE_TREE, double>;
  using quadtree_box_map_c_d = orthotree_box_map_c_t<2, true, double>;
  using quadtree_box_map_c_f = orthotree_box_map_c_t<2, true, float>;
  using quadtree_box_map_c_i = orthotree_box_map_c_t<2, true, int>;
  using quadtree_box_map_c = quadtree_box_c_f;

  template<bool IS_LOOSE_TREE = true>
  using octree_box_map_c_ds = orthotree_box_map_c_t<3, IS_LOOSE_TREE, double>;
  using octree_box_map_c_d = orthotree_box_map_c_t<3, true, double>;
  using octree_box_map_c_f = orthotree_box_map_c_t<3, true, float>;
  using octree_box_map_c_i = orthotree_box_map_c_t<3, true, int>;
  using octree_box_map_c = octree_box_c_f;

  template<bool IS_LOOSE_TREE = true>
  using hextree_box_map_c_ds = orthotree_box_map_c_t<4, IS_LOOSE_TREE, double>;
  using hextree_box_map_c_d = orthotree_box_map_c_t<4, true, double>;
  using hextree_box_map_c_f = orthotree_box_map_c_t<4, true, float>;
  using hextree_box_map_c_i = orthotree_box_map_c_t<4, true, int>;
  using hextree_box_map_c = hextree_box_map_c_f;
} // namespace glm
