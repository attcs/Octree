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
#include <boost/geometry.hpp>

namespace boost::geometry::model
{
  // Define related elements

  template<OrthoTree::dim_t DIMENSION_NO, typename TScalar>
  using pointNd_t = point<TScalar, DIMENSION_NO, boost::geometry::cs::cartesian>;

  template<OrthoTree::dim_t DIMENSION_NO, typename TScalar>
  using boxNd_t = box<pointNd_t<DIMENSION_NO, TScalar>>;

  template<OrthoTree::dim_t DIMENSION_NO, typename TScalar>
  struct rayNd_t
  {
    pointNd_t<DIMENSION_NO, TScalar> origin;
    pointNd_t<DIMENSION_NO, TScalar> direction;
  };

  template<OrthoTree::dim_t DIMENSION_NO, typename TScalar>
  struct planeNd_t
  {
    TScalar origo_distance;                  // origo_distance = dot_product(normal, any_point)
    pointNd_t<DIMENSION_NO, TScalar> normal; // should be normalized
  };
} // namespace boost::geometry::model


namespace OrthoTree
{
  namespace BoostAdaptor
  {
    template<dim_t DIMENSION_NO_, typename TScalar = double>
    struct BoostBaseGeometryAdapter
    {
      using Scalar = TScalar;
      using FloatScalar = std::conditional_t<std::is_floating_point_v<TScalar>, TScalar, float>;
      using Vector = boost::geometry::model::pointNd_t<DIMENSION_NO_, TScalar>;
      using Box = boost::geometry::model::boxNd_t<DIMENSION_NO_, TScalar>;
      using Ray = boost::geometry::model::rayNd_t<DIMENSION_NO_, TScalar>;
      using Plane = boost::geometry::model::planeNd_t<DIMENSION_NO_, TScalar>;

      static constexpr dim_t DIMENSION_NO = DIMENSION_NO_;
      static constexpr FloatScalar BASE_TOLERANCE = std::numeric_limits<FloatScalar>::epsilon() * FloatScalar(10.0);

      static constexpr Vector MakePoint() noexcept { return {}; };
      static constexpr Box MakeBox() noexcept { return {}; };

      static constexpr Scalar GetPointC(Vector const& point, dim_t dimensionID) noexcept
      {
        static_assert(DIMENSION_NO < 6, "For this dimension, the adaptor is not implemented. Supplement the function!");

        if constexpr (DIMENSION_NO == 1)
        {
          return point.template get<0>();
        }
        else if constexpr (DIMENSION_NO == 2)
        {
          switch (dimensionID)
          {
          case 0: return point.template get<0>();
          case 1: return point.template get<1>();
          }
        }
        else if constexpr (DIMENSION_NO == 3)
        {
          switch (dimensionID)
          {
          case 0: return point.template get<0>();
          case 1: return point.template get<1>();
          case 2: return point.template get<2>();
          }
        }
        else if constexpr (DIMENSION_NO == 4)
        {
          switch (dimensionID)
          {
          case 0: return point.template get<0>();
          case 1: return point.template get<1>();
          case 2: return point.template get<2>();
          case 3: return point.template get<3>();
          }
        }
        else
        {
          switch (dimensionID)
          {
          case 0: return point.template get<0>();
          case 1: return point.template get<1>();
          case 2: return point.template get<2>();
          case 3: return point.template get<3>();
          case 4: return point.template get<4>();
          }
        }

        assert(false);
        std::terminate();
      }


      static constexpr void SetPointC(Vector& point, dim_t dimensionID, TScalar value) noexcept
      {
        static_assert(DIMENSION_NO < 6, "For this dimension, the adaptor is not implemented. Supplement the function!");

        if constexpr (DIMENSION_NO == 1)
        {
          point.set<0>(value);
        }
        else if constexpr (DIMENSION_NO == 2)
        {
          switch (dimensionID)
          {
          case 0: point.template set<0>(value); return;
          case 1: point.template set<1>(value); return;
          }
        }
        else if constexpr (DIMENSION_NO == 3)
        {
          switch (dimensionID)
          {
          case 0: point.template set<0>(value); return;
          case 1: point.template set<1>(value); return;
          case 2: point.template set<2>(value); return;
          }
        }
        else if constexpr (DIMENSION_NO == 4)
        {
          switch (dimensionID)
          {
          case 0: point.template set<0>(value); return;
          case 1: point.template set<1>(value); return;
          case 2: point.template set<2>(value); return;
          case 3: point.template set<3>(value); return;
          }
        }
        else
        {
          switch (dimensionID)
          {
          case 0: point.template set<0>(value); return;
          case 1: point.template set<1>(value); return;
          case 2: point.template set<2>(value); return;
          case 3: point.template set<3>(value); return;
          case 4: point.template set<4>(value); return;
          }
        }

        assert(false);
        std::terminate();
      }

      static constexpr Scalar GetBoxMinC(Box const& box, dim_t dimensionID) noexcept { return GetPointC(box.min_corner(), dimensionID); }
      static constexpr Scalar GetBoxMaxC(Box const& box, dim_t dimensionID) noexcept { return GetPointC(box.max_corner(), dimensionID); }
      static constexpr void SetBoxMinC(Box& box, dim_t dimensionID, Scalar value) noexcept { SetPointC(box.min_corner(), dimensionID, value); }
      static constexpr void SetBoxMaxC(Box& box, dim_t dimensionID, Scalar value) noexcept { SetPointC(box.max_corner(), dimensionID, value); }

      static constexpr Vector const& GetRayDirection(Ray const& ray) noexcept { return ray.direction; }
      static constexpr Vector const& GetRayOrigin(Ray const& ray) noexcept { return ray.origin; }

      static constexpr Vector const& GetPlaneNormal(Plane const& plane) noexcept { return plane.normal; }
      static constexpr Scalar GetPlaneOrigoDistance(Plane const& plane) noexcept { return plane.origo_distance; }
    };

    template<dim_t DIMENSION_NO, typename TScalar>
    using BoostGeometryAdapter = GeneralGeometryAdapter<BoostBaseGeometryAdapter<DIMENSION_NO, TScalar>>;

    template<dim_t DIMENSION_NO, typename TScalar, typename TEntityAdapter = PointEntitySpanAdapter<boost::geometry::model::pointNd_t<DIMENSION_NO, TScalar>>>
    using BoostOrthoTreePoint = OrthoTreeBase<TEntityAdapter, BoostGeometryAdapter<DIMENSION_NO, TScalar>, PointConfiguration>;

    template<dim_t DIMENSION_NO, bool IS_LOOSE_TREE, typename TScalar, typename TEntityAdapter = BoxEntitySpanAdapter<boost::geometry::model::boxNd_t<DIMENSION_NO, TScalar>>>
    using BoostOrthoTreeBoundingBox = OrthoTreeBase<TEntityAdapter, BoostGeometryAdapter<DIMENSION_NO, TScalar>, BoxConfiguration<IS_LOOSE_TREE>>;
  } // namespace BoostAdaptor
} // namespace OrthoTree

namespace boost::geometry
{
  using namespace OrthoTree::BoostAdaptor;

  // Core types

  template<int DIMENSION_NO, typename TScalar = double, typename TEntityAdapter = OrthoTree::PointEntitySpanAdapter<boost::geometry::model::boxNd_t<DIMENSION_NO, TScalar>>>
  using orthotree_point_t = BoostOrthoTreePoint<DIMENSION_NO, TScalar, TEntityAdapter>;

  template<
    int DIMENSION_NO,
    bool IS_LOOSE_TREE = true,
    typename TScalar = double,
    typename TEntityAdapter = OrthoTree::BoxEntitySpanAdapter<boost::geometry::model::boxNd_t<DIMENSION_NO, TScalar>>>
  using orthotree_box_t = BoostOrthoTreeBoundingBox<DIMENSION_NO, IS_LOOSE_TREE, TScalar, TEntityAdapter>;


  using quadtree_point_d = BoostOrthoTreePoint<2, double>;
  using quadtree_point_f = BoostOrthoTreePoint<2, float>;
  using quadtree_point_i = BoostOrthoTreePoint<2, int>;
  using quadtree_point = quadtree_point_d;

  using octree_point_d = BoostOrthoTreePoint<3, double>;
  using octree_point_f = BoostOrthoTreePoint<3, float>;
  using octree_point_i = BoostOrthoTreePoint<3, int>;
  using octree_point = octree_point_d;


  template<bool IS_LOOSE_TREE = true>
  using quadtree_box_ds = BoostOrthoTreeBoundingBox<2, IS_LOOSE_TREE, double>;
  using quadtree_box_d = BoostOrthoTreeBoundingBox<2, true, double>;
  using quadtree_box_f = BoostOrthoTreeBoundingBox<2, true, float>;
  using quadtree_box_i = BoostOrthoTreeBoundingBox<2, true, int>;
  using quadtree_box = quadtree_box_d;

  template<bool IS_LOOSE_TREE = true>
  using octree_box_ds = BoostOrthoTreeBoundingBox<3, IS_LOOSE_TREE, double>;
  using octree_box_d = BoostOrthoTreeBoundingBox<3, true, double>;
  using octree_box_f = BoostOrthoTreeBoundingBox<3, true, float>;
  using octree_box_i = BoostOrthoTreeBoundingBox<3, true, int>;
  using octree_box = octree_box_d;

  // Container types

  template<int DIMENSION_NO, typename TScalar = double>
  using orthotree_point_c_t = OrthoTree::OrthoTreeContainer<orthotree_point_t<DIMENSION_NO, TScalar>>;

  using quadtree_point_c_d = orthotree_point_c_t<2, double>;
  using quadtree_point_c_f = orthotree_point_c_t<2, float>;
  using quadtree_point_c_i = orthotree_point_c_t<2, int>;
  using quadtree_point_c = quadtree_point_c_d;

  using octree_point_c_d = orthotree_point_c_t<3, double>;
  using octree_point_c_f = orthotree_point_c_t<3, float>;
  using octree_point_c_i = orthotree_point_c_t<3, int>;
  using octree_point_c = octree_point_c_d;


  template<int DIMENSION_NO, bool IS_LOOSE_TREE = true, typename TScalar = double>
  using orthotree_box_c_t = OrthoTree::OrthoTreeContainer<orthotree_box_t<DIMENSION_NO, IS_LOOSE_TREE, TScalar>>;

  template<bool IS_LOOSE_TREE = true>
  using quadtree_box_c_ds = orthotree_box_c_t<2, IS_LOOSE_TREE, double>;
  using quadtree_box_c_d = orthotree_box_c_t<2, true, double>;
  using quadtree_box_c_f = orthotree_box_c_t<2, true, float>;
  using quadtree_box_c_i = orthotree_box_c_t<2, true, int>;
  using quadtree_box_c = quadtree_box_c_d;

  template<bool IS_LOOSE_TREE = true>
  using octree_box_c_ds = orthotree_box_c_t<3, IS_LOOSE_TREE, double>;
  using octree_box_c_d = orthotree_box_c_t<3, true, double>;
  using octree_box_c_f = orthotree_box_c_t<3, true, float>;
  using octree_box_c_i = orthotree_box_c_t<3, true, int>;
  using octree_box_c = octree_box_c_d;


  // Map types

  template<int DIMENSION_NO, typename TScalar>
  using pointmap_container = OrthoTree::PointEntityMapAdapter<boost::geometry::model::pointNd_t<DIMENSION_NO, TScalar>>;
  template<int DIMENSION_NO, typename TScalar>
  using boxmap_container = OrthoTree::BoxEntityMapAdapter<boost::geometry::model::boxNd_t<DIMENSION_NO, TScalar>>;

  using quadtree_point_map_d = BoostOrthoTreePoint<2, double, pointmap_container<2, double>>;
  using quadtree_point_map_f = BoostOrthoTreePoint<2, float, pointmap_container<2, float>>;
  using quadtree_point_map_i = BoostOrthoTreePoint<2, int, pointmap_container<2, int>>;
  using quadtree_point_map = quadtree_point_map_d;

  using octree_point_map_d = BoostOrthoTreePoint<3, double, pointmap_container<3, double>>;
  using octree_point_map_f = BoostOrthoTreePoint<3, float, pointmap_container<3, float>>;
  using octree_point_map_i = BoostOrthoTreePoint<3, int, pointmap_container<3, int>>;
  using octree_point_map = octree_point_map_d;


  template<bool IS_LOOSE_TREE = true>
  using quadtree_box_map_ds = BoostOrthoTreeBoundingBox<2, IS_LOOSE_TREE, double, boxmap_container<2, double>>;
  using quadtree_box_map_d = BoostOrthoTreeBoundingBox<2, true, double, boxmap_container<2, double>>;
  using quadtree_box_map_f = BoostOrthoTreeBoundingBox<2, true, float, boxmap_container<2, float>>;
  using quadtree_box_map_i = BoostOrthoTreeBoundingBox<2, true, int, boxmap_container<2, int>>;
  using quadtree_box_map = quadtree_box_map_d;

  template<bool IS_LOOSE_TREE = true>
  using octree_box_map_ds = BoostOrthoTreeBoundingBox<3, IS_LOOSE_TREE, double, boxmap_container<3, double>>;
  using octree_box_map_d = BoostOrthoTreeBoundingBox<3, true, double, boxmap_container<3, double>>;
  using octree_box_map_f = BoostOrthoTreeBoundingBox<3, true, float, boxmap_container<3, float>>;
  using octree_box_map_i = BoostOrthoTreeBoundingBox<3, true, int, boxmap_container<3, int>>;
  using octree_box_map = octree_box_map_d;

  // Container types

  template<int DIMENSION_NO, typename TScalar = double>
  using orthotree_point_map_c_t = OrthoTree::OrthoTreeContainer<orthotree_point_t<DIMENSION_NO, TScalar, pointmap_container<DIMENSION_NO, TScalar>>>;

  using quadtree_point_map_c_d = orthotree_point_map_c_t<2, double>;
  using quadtree_point_map_c_f = orthotree_point_map_c_t<2, float>;
  using quadtree_point_map_c_i = orthotree_point_map_c_t<2, int>;
  using quadtree_point_map_c = quadtree_point_map_c_d;

  using octree_point_map_c_d = orthotree_point_c_t<3, double>;
  using octree_point_map_c_f = orthotree_point_c_t<3, float>;
  using octree_point_map_c_i = orthotree_point_c_t<3, int>;
  using octree_point_map_c = octree_point_map_c_d;


  template<int DIMENSION_NO, bool IS_LOOSE_TREE = true, typename TScalar = double>
  using orthotree_box_map_c_t =
    OrthoTree::OrthoTreeContainer<orthotree_box_t<DIMENSION_NO, IS_LOOSE_TREE, TScalar, boxmap_container<DIMENSION_NO, TScalar>>>;

  template<bool IS_LOOSE_TREE = true>
  using quadtree_box_map_c_ds = orthotree_box_map_c_t<2, IS_LOOSE_TREE, double>;
  using quadtree_box_map_c_d = orthotree_box_map_c_t<2, true, double>;
  using quadtree_box_map_c_f = orthotree_box_map_c_t<2, true, float>;
  using quadtree_box_map_c_i = orthotree_box_map_c_t<2, true, int>;
  using quadtree_box_map_c = quadtree_box_map_c_d;

  template<bool IS_LOOSE_TREE = true>
  using octree_box_map_c_ds = orthotree_box_map_c_t<3, IS_LOOSE_TREE, double>;
  using octree_box_map_c_d = orthotree_box_map_c_t<3, true, double>;
  using octree_box_map_c_f = orthotree_box_map_c_t<3, true, float>;
  using octree_box_map_c_i = orthotree_box_map_c_t<3, true, int>;
  using octree_box_map_c = octree_box_map_c_d;

} // namespace boost::geometry
