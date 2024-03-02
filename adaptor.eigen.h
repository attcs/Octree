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
#include <Eigen/Geometry>

namespace OrthoTree
{
  namespace EigenAdaptor
  {
    using namespace Eigen;

    template<typename Scalar_, int AmbientDim_>
    struct EigenAdaptorBasics
    {
      using VectorType_ = Matrix<Scalar_, AmbientDim_, 1>;
      using AlignedBox_ = AlignedBox<Scalar_, AmbientDim_>;

      static constexpr Scalar_& point_comp(VectorType_& pt, dim_type dimensionID) { return pt(dimensionID); }

      static constexpr Scalar_ point_comp_c(VectorType_ const& pt, dim_type dimensionID) { return pt(dimensionID); }

      static constexpr VectorType_& box_min(AlignedBox_& box) { return box.min(); }
      static constexpr VectorType_& box_max(AlignedBox_& box) { return box.max(); }
      static constexpr VectorType_ const& box_min_c(AlignedBox_ const& box) { return box.min(); }
      static constexpr VectorType_ const& box_max_c(AlignedBox_ const& box) { return box.max(); }
    };

    template<typename Scalar_, int AmbientDim_>
    struct EigenAdaptorGeneralBase : EigenAdaptorBasics<Scalar_, AmbientDim_>
    {
      using base = EigenAdaptorBasics<Scalar_, AmbientDim_>;
      using VectorType_ = base::VectorType_;
      using AlignedBox_ = base::AlignedBox_;

      static_assert(AdaptorBasicsConcept<base, VectorType_, AlignedBox_, Scalar_>);

      static constexpr Scalar_ size2(VectorType_ const& v) noexcept { return v.squaredNorm(); }

      static constexpr Scalar_ size(VectorType_ const& v) noexcept { return v.norm(); }

      static constexpr VectorType_ add(VectorType_ const& v1, VectorType_ const& v2) noexcept { return v1 + v2; }

      static constexpr VectorType_ subtract(VectorType_ const& v1, VectorType_ const& v2) noexcept { return v1 - v2; }

      static constexpr VectorType_ multiply(VectorType_ const& v, double scalarFactor) noexcept { return v * scalarFactor; }

      static constexpr Scalar_ dot(VectorType_ const& v1, VectorType_ const& v2) noexcept { return v1.dot(v2); }

      static constexpr Scalar_ distance(VectorType_ const& v1, VectorType_ const& v2) noexcept { return size(v1 - v2); }

      static constexpr Scalar_ distance2(VectorType_ const& v1, VectorType_ const& v2) noexcept { return size2(v1 - v2); }

      static constexpr bool are_points_equal(VectorType_ const& v1, VectorType_ const& v2, Scalar_ tolerance) noexcept
      {
        return distance2(v1, v2) <= tolerance * tolerance;
      }

      static constexpr bool is_normalized_vector(VectorType_ const& normal) noexcept { return abs(size2(normal) - 1.0) < 0.000001; }

      static constexpr bool does_box_contain_point(AlignedBox_ const& box, VectorType_ const& point) noexcept { return box.contains(point); }

      static constexpr bool does_box_contain_point_strict(AlignedBox_ const& box, VectorType_ const& point) noexcept
      {
        for (dim_type dimensionID = 0; dimensionID < AmbientDim_; ++dimensionID)
          if (!(base::point_comp_c(base::box_min_c(box), dimensionID) < base::point_comp_c(point, dimensionID) &&
                base::point_comp_c(point, dimensionID) < base::point_comp_c(base::box_max_c(box), dimensionID)))
            return false;

        return true;
      }


      static constexpr bool does_point_touch_box(AlignedBox_ const& box, VectorType_ const& point) noexcept
      {
        for (dim_type dimensionID = 0; dimensionID < AmbientDim_; ++dimensionID)
          if ((base::point_comp_c(base::box_min_c(box), dimensionID) == base::point_comp_c(point, dimensionID)))
            return false;

        return true;
      }

      static constexpr bool are_boxes_overlapped_strict(AlignedBox_ const& e1, AlignedBox_ const& e2) noexcept
      {
        autoc e3 = e1.intersection(e2);
        autoc sizes = e3.sizes();
        for (dim_type dimensionID = 0; dimensionID < AmbientDim_; ++dimensionID)
          if (sizes[dimensionID] <= 0.0)
            return false;

        return true;
      }

      static constexpr bool are_boxes_overlapped(
        AlignedBox_ const& e1, AlignedBox_ const& e2, bool e1_must_contain_e2 = true, bool fOverlapPtTouchAllowed = false) noexcept
      {
        if (e1_must_contain_e2)
          return e1.contains(e2);
        else if (!fOverlapPtTouchAllowed)
          return are_boxes_overlapped_strict(e1, e2);
        else
          return e1.intersects(e2);
      }

      static AlignedBox_ box_of_points(std::span<VectorType_ const> const& points) noexcept
      {
        auto ext = points.size() == 0 ? AlignedBox_{} : AlignedBox_(points[0]);
        for (autoc& point : points)
          ext.extend(point);

        return ext;
      }

      static AlignedBox_ box_of_boxes(std::span<AlignedBox_ const> const& extents) noexcept
      {
        auto ext = extents.size() == 0 ? AlignedBox_{} : extents[0];
        for (autoc& extent : extents)
          ext.extend(extent);

        return ext;
      }

      static void move_box(AlignedBox_& box, VectorType_ const& moveVector) noexcept { box.translate(moveVector); }

      static constexpr std::optional<double> is_ray_hit(
        AlignedBox_ const& box, VectorType_ const& rayBasePoint, VectorType_ const& rayHeading, Scalar_ tolerance) noexcept
      {
        autoc toleranceVector = VectorType_::Ones() * tolerance;
        autoc rayBasePointBox = AlignedBox_(rayBasePoint - toleranceVector, rayBasePoint + toleranceVector);
        if (box.intersects(rayBasePointBox))
          return 0.0;

        autoc& minBoxPoint = base::box_min_c(box);
        autoc& maxBoxPoint = base::box_max_c(box);

        autoce inf = std::numeric_limits<double>::infinity();

        auto minDistances = std::array<double, AmbientDim_>{};
        auto maxDistances = std::array<double, AmbientDim_>{};
        for (dim_type dimensionID = 0; dimensionID < AmbientDim_; ++dimensionID)
        {
          autoc hComp = base::point_comp_c(rayHeading, dimensionID);
          if (hComp == 0)
          {
            if (base::point_comp_c(maxBoxPoint, dimensionID) + tolerance < base::point_comp_c(rayBasePoint, dimensionID))
              return std::nullopt;

            if (base::point_comp_c(minBoxPoint, dimensionID) - tolerance > base::point_comp_c(rayBasePoint, dimensionID))
              return std::nullopt;

            minDistances[dimensionID] = -inf;
            maxDistances[dimensionID] = +inf;
            continue;
          }

          minDistances[dimensionID] =
            (base::point_comp_c(hComp > 0.0 ? minBoxPoint : maxBoxPoint, dimensionID) - tolerance - base::point_comp_c(rayBasePoint, dimensionID)) / hComp;
          maxDistances[dimensionID] =
            (base::point_comp_c(hComp < 0.0 ? minBoxPoint : maxBoxPoint, dimensionID) + tolerance - base::point_comp_c(rayBasePoint, dimensionID)) / hComp;
        }

        autoc rMin = *std::ranges::max_element(minDistances);
        autoc rMax = *std::ranges::min_element(maxDistances);
        if (rMin > rMax || rMax < 0.0)
          return std::nullopt;

        return rMin < 0 ? rMax : rMin;
      }

      // Get point-Hyperplane relation (Plane equation: dotProduct(planeNormal, point) = distanceOfOrigo)
      static constexpr PlaneRelation get_point_plane_relation(
        VectorType_ const& point, Scalar_ distanceOfOrigo, VectorType_ const& planeNormal, Scalar_ tolerance) noexcept
      {
        assert(is_normalized_vector(planeNormal));

        autoc pointProjected = dot(planeNormal, point);

        if (pointProjected < distanceOfOrigo - tolerance)
          return PlaneRelation::Negative;

        if (pointProjected > distanceOfOrigo + tolerance)
          return PlaneRelation::Positive;

        return PlaneRelation::Hit;
      }

      // Get box-Hyperplane relation (Plane equation: dotProduct(planeNormal, point) = distanceOfOrigo)
      static constexpr PlaneRelation get_box_plane_relation(
        AlignedBox_ const& box, Scalar_ distanceOfOrigo, VectorType_ const& planeNormal, Scalar_ tolerance) noexcept
      {
        assert(is_normalized_vector(planeNormal));

        autoc& minPoint = base::box_min_c(box);
        autoc& maxPoint = base::box_max_c(box);

        autoc center = multiply(add(minPoint, maxPoint), 0.5);
        autoc radius = subtract(maxPoint, center);

        auto radiusProjected = double(tolerance);
        for (dim_type dimensionID = 0; dimensionID < AmbientDim_; ++dimensionID)
          radiusProjected += base::point_comp_c(radius, dimensionID) * std::abs(base::point_comp_c(planeNormal, dimensionID));

        autoc centerProjected = dot(planeNormal, center);

        if (centerProjected + radiusProjected < distanceOfOrigo)
          return PlaneRelation::Negative;

        if (centerProjected - radiusProjected > distanceOfOrigo)
          return PlaneRelation::Positive;

        return PlaneRelation::Hit;
      }
    };

  } // namespace EigenAdaptor
} // namespace OrthoTree


namespace Eigen
{
  using namespace OrthoTree;
  using namespace OrthoTree::EigenAdaptor;

  // Basic OrthoTree types
  template<typename Scalar_, int AmbientDim_>
  using EigenOrthoTreePoint =
    OrthoTreePoint<AmbientDim_, Matrix<Scalar_, AmbientDim_, 1>, AlignedBox<Scalar_, AmbientDim_>, EigenAdaptorGeneralBase<Scalar_, AmbientDim_>, Scalar_>;

  template<typename Scalar_, int AmbientDim_, uint32_t AdditionalDepthOfSplitStrategy_ = 2>
  using EigenOrthoTreeBox =
    OrthoTreeBoundingBox<AmbientDim_, Matrix<Scalar_, AmbientDim_, 1>, AlignedBox<Scalar_, AmbientDim_>, EigenAdaptorGeneralBase<Scalar_, AmbientDim_>, Scalar_, AdditionalDepthOfSplitStrategy_>;

  template<typename Scalar_, int AmbientDim_>
  using OrthoTreeContainerPointC = OrthoTreeContainerPoint<EigenOrthoTreePoint<Scalar_, AmbientDim_>, Matrix<Scalar_, AmbientDim_, 1>>;

  template<typename Scalar_, int AmbientDim_, uint32_t AdditionalDepthOfSplitStrategy_ = 2>
  using OrthoTreeContainerBoxC =
    OrthoTreeContainerBox<EigenOrthoTreeBox<Scalar_, AmbientDim_, AdditionalDepthOfSplitStrategy_>, AlignedBox<Scalar_, AmbientDim_>>;

  // Non-owning types
  using QuadtreePoint2f = EigenOrthoTreePoint<float, 2>;

  using QuadtreePoint2d = EigenOrthoTreePoint<double, 2>;

  using OctreePoint3f = EigenOrthoTreePoint<float, 3>;

  using OctreePoint3d = EigenOrthoTreePoint<double, 3>;

  template<uint32_t AdditionalDepthOfSplitStrategy_ = 2>
  using QuadtreeBox2f = EigenOrthoTreeBox<float, 2, AdditionalDepthOfSplitStrategy_>;

  template<uint32_t AdditionalDepthOfSplitStrategy_ = 2>
  using QuadtreeBox2d = EigenOrthoTreeBox<double, 2, AdditionalDepthOfSplitStrategy_>;

  template<uint32_t AdditionalDepthOfSplitStrategy_ = 2>
  using OctreeBox3f = EigenOrthoTreeBox<float, 3, AdditionalDepthOfSplitStrategy_>;

  template<uint32_t AdditionalDepthOfSplitStrategy_ = 2>
  using OctreeBox3d = EigenOrthoTreeBox<double, 3, AdditionalDepthOfSplitStrategy_>;

  // Container types
  using QuadtreePointC2f = OrthoTreeContainerPointC<float, 2>;

  using QuadtreePointC2d = OrthoTreeContainerPointC<double, 2>;

  using OctreePointC3f = OrthoTreeContainerPointC<float, 3>;

  using OctreePointC3d = OrthoTreeContainerPointC<double, 3>;

  template<uint32_t AdditionalDepthOfSplitStrategy_ = 2>
  using QuadtreeBoxC2f = OrthoTreeContainerBoxC<float, 2, AdditionalDepthOfSplitStrategy_>;

  template<uint32_t AdditionalDepthOfSplitStrategy_ = 2>
  using QuadtreeBoxC2d = OrthoTreeContainerBoxC<double, 2, AdditionalDepthOfSplitStrategy_>;

  template<uint32_t AdditionalDepthOfSplitStrategy_ = 2>
  using OctreeBoxC3f = OrthoTreeContainerBoxC<float, 3, AdditionalDepthOfSplitStrategy_>;

  template<uint32_t AdditionalDepthOfSplitStrategy_ = 2>
  using OctreeBoxC3d = OrthoTreeContainerBoxC<double, 3, AdditionalDepthOfSplitStrategy_>;
} // namespace Eigen