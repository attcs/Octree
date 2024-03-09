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
// #include <boost/geometry.hpp>

namespace boost::geometry::model
{
  // Define related elements

  template<OrthoTree::dim_t DIMENSION_NO, typename TGeometry>
  using pointNd_t = point<TGeometry, DIMENSION_NO, boost::geometry::cs::cartesian>;

  template<OrthoTree::dim_t DIMENSION_NO, typename TGeometry>
  using boxNd_t = box<pointNd_t<DIMENSION_NO, TGeometry>>;

  template<OrthoTree::dim_t DIMENSION_NO, typename TGeometry>
  struct rayNd_t
  {
    pointNd_t<DIMENSION_NO, TGeometry> origin;
    pointNd_t<DIMENSION_NO, TGeometry> direction;
  };

  template<OrthoTree::dim_t DIMENSION_NO, typename TGeometry>
  struct planeNd_t
  {
    TGeometry origo_distance;                  // origo_distance = dot_product(normal, any_point)
    pointNd_t<DIMENSION_NO, TGeometry> normal; // should be normalized
  };
} // namespace boost::geometry::model


namespace OrthoTree
{
  namespace BoostAdaptor
  {

    template<dim_t DIMENSION_NO, typename TGeometry = double>
    struct AdaptorGeneralBasics
    {
      using TVector = boost::geometry::model::pointNd_t<DIMENSION_NO, TGeometry>;
      using TBox = boost::geometry::model::boxNd_t<DIMENSION_NO, TGeometry>;
      using TRay = boost::geometry::model::rayNd_t<DIMENSION_NO, TGeometry>;
      using TPlane = boost::geometry::model::planeNd_t<DIMENSION_NO, TGeometry>;


      static constexpr TGeometry GetPointC(TVector const& point, dim_t dimensionID) noexcept
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


      static constexpr void SetPointC(TVector& point, dim_t dimensionID, TGeometry value) noexcept
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

      static constexpr TGeometry GetBoxMinC(TBox const& box, dim_t dimensionID) noexcept { return GetPointC(box.min_corner(), dimensionID); }
      static constexpr TGeometry GetBoxMaxC(TBox const& box, dim_t dimensionID) noexcept { return GetPointC(box.max_corner(), dimensionID); }
      static constexpr void SetBoxMinC(TBox& box, dim_t dimensionID, TGeometry value) noexcept { SetPointC(box.min_corner(), dimensionID, value); }
      static constexpr void SetBoxMaxC(TBox& box, dim_t dimensionID, TGeometry value) noexcept { SetPointC(box.max_corner(), dimensionID, value); }

      static constexpr TVector const& GetRayDirection(TRay const& ray) noexcept { return ray.direction; }
      static constexpr TVector const& GetRayOrigin(TRay const& ray) noexcept { return ray.origin; }

      static constexpr TVector const& GetPlaneNormal(TPlane const& plane) noexcept { return plane.normal; }
      static constexpr TGeometry GetPlaneOrigoDistance(TPlane const& plane) noexcept { return plane.origo_distance; }
    };

    template<dim_t DIMENSION_NO, typename TGeometry>
    using BoostAdaptorGeneral = AdaptorGeneralBase<
      DIMENSION_NO,
      boost::geometry::model::pointNd_t<DIMENSION_NO, TGeometry>,
      boost::geometry::model::boxNd_t<DIMENSION_NO, TGeometry>,
      boost::geometry::model::rayNd_t<DIMENSION_NO, TGeometry>,
      boost::geometry::model::planeNd_t<DIMENSION_NO, TGeometry>,
      TGeometry,
      AdaptorGeneralBasics<DIMENSION_NO, TGeometry>>;

    template<dim_t DIMENSION_NO, typename TGeometry>
    using BoostOrthoTreePoint = OrthoTreePoint<
      DIMENSION_NO,
      boost::geometry::model::pointNd_t<DIMENSION_NO, TGeometry>,
      boost::geometry::model::boxNd_t<DIMENSION_NO, TGeometry>,
      boost::geometry::model::rayNd_t<DIMENSION_NO, TGeometry>,
      boost::geometry::model::planeNd_t<DIMENSION_NO, TGeometry>,
      TGeometry,
      BoostAdaptorGeneral<DIMENSION_NO, TGeometry>>;

    template<dim_t DIMENSION_NO, uint32_t SPLIT_DEPTH_INCREASEMENT, typename TGeometry>
    using BoostOrthoTreeBoundingBox = OrthoTreeBoundingBox<
      DIMENSION_NO,
      boost::geometry::model::pointNd_t<DIMENSION_NO, TGeometry>,
      boost::geometry::model::boxNd_t<DIMENSION_NO, TGeometry>,
      boost::geometry::model::rayNd_t<DIMENSION_NO, TGeometry>,
      boost::geometry::model::planeNd_t<DIMENSION_NO, TGeometry>,
      TGeometry,
      SPLIT_DEPTH_INCREASEMENT,
      BoostAdaptorGeneral<DIMENSION_NO, TGeometry>>;
  } // namespace BoostAdaptor
} // namespace OrthoTree

namespace boost::geometry
{
  using namespace OrthoTree::BoostAdaptor;

  // Core types

  template<int DIMENSION_NO, typename TGeometry = double>
  using orthotree_point_t = BoostOrthoTreePoint<DIMENSION_NO, TGeometry>;

  using quadtree_point_d = BoostOrthoTreePoint<2, double>;
  using quadtree_point_f = BoostOrthoTreePoint<2, float>;
  using quadtree_point_i = BoostOrthoTreePoint<2, int>;
  using quadtree_point = quadtree_point_d;

  using octree_point_d = BoostOrthoTreePoint<3, double>;
  using octree_point_f = BoostOrthoTreePoint<3, float>;
  using octree_point_i = BoostOrthoTreePoint<3, int>;
  using octree_point = octree_point_d;


  template<int DIMENSION_NO, uint32_t SPLIT_DEPTH_INCREASEMENT = 2, typename TGeometry = double>
  using orthotree_box_t = BoostOrthoTreeBoundingBox<DIMENSION_NO, SPLIT_DEPTH_INCREASEMENT, TGeometry>;

  template<uint32_t SPLIT_DEPTH_INCREASEMENT = 2>
  using quadtree_box_ds = BoostOrthoTreeBoundingBox<2, SPLIT_DEPTH_INCREASEMENT, double>;
  using quadtree_box_d = BoostOrthoTreeBoundingBox<2, 2, double>;
  using quadtree_box_f = BoostOrthoTreeBoundingBox<2, 2, float>;
  using quadtree_box_i = BoostOrthoTreeBoundingBox<2, 2, int>;
  using quadtree_box = quadtree_box_d;

  template<uint32_t SPLIT_DEPTH_INCREASEMENT = 2>
  using octree_box_ds = BoostOrthoTreeBoundingBox<3, SPLIT_DEPTH_INCREASEMENT, double>;
  using octree_box_d = BoostOrthoTreeBoundingBox<3, 2, double>;
  using octree_box_f = BoostOrthoTreeBoundingBox<3, 2, float>;
  using octree_box_i = BoostOrthoTreeBoundingBox<3, 2, int>;
  using octree_box = octree_box_d;

  // Container types

  template<int DIMENSION_NO, typename TGeometry = double>
  using orthotree_point_c_t = OrthoTree::OrthoTreeContainerPoint<orthotree_point_t<DIMENSION_NO, TGeometry>, model::pointNd_t<DIMENSION_NO, TGeometry>>;

  using quadtree_point_c_d = orthotree_point_c_t<2, double>;
  using quadtree_point_c_f = orthotree_point_c_t<2, float>;
  using quadtree_point_c_i = orthotree_point_c_t<2, int>;
  using quadtree_point_c = quadtree_point_c_d;

  using octree_point_c_d = orthotree_point_c_t<3, double>;
  using octree_point_c_f = orthotree_point_c_t<3, float>;
  using octree_point_c_i = orthotree_point_c_t<3, int>;
  using octree_point_c = octree_point_c_d;


  template<int DIMENSION_NO, uint32_t SPLIT_DEPTH_INCREASEMENT = 2, typename TGeometry = double>
  using orthotree_box_c_t =
    OrthoTree::OrthoTreeContainerBox<orthotree_box_t<DIMENSION_NO, SPLIT_DEPTH_INCREASEMENT, TGeometry>, model::boxNd_t<DIMENSION_NO, TGeometry>>;

  template<uint32_t SPLIT_DEPTH_INCREASEMENT = 2>
  using quadtree_box_c_ds = orthotree_box_c_t<2, SPLIT_DEPTH_INCREASEMENT, double>;
  using quadtree_box_c_d = orthotree_box_c_t<2, 2, double>;
  using quadtree_box_c_f = orthotree_box_c_t<2, 2, float>;
  using quadtree_box_c_i = orthotree_box_c_t<2, 2, int>;
  using quadtree_box_c = quadtree_box_c_d;

  template<uint32_t SPLIT_DEPTH_INCREASEMENT = 2>
  using octree_box_c_ds = orthotree_box_c_t<3, SPLIT_DEPTH_INCREASEMENT, double>;
  using octree_box_c_d = orthotree_box_c_t<3, 2, double>;
  using octree_box_c_f = orthotree_box_c_t<3, 2, float>;
  using octree_box_c_i = orthotree_box_c_t<3, 2, int>;
  using octree_box_c = octree_box_c_d;
} // namespace boost::geometry