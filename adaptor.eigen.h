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
// #include <Eigen/Geometry>

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
      using Ray_ = ParametrizedLine<Scalar_, AmbientDim_>;
      using Plane_ = Hyperplane<Scalar_, AmbientDim_>;

      static constexpr Scalar_ GetPointC(VectorType_ const& point, dim_t dimensionID) noexcept { return point(dimensionID); }
      static constexpr void SetPointC(VectorType_& point, dim_t dimensionID, Scalar_ value) noexcept { point(dimensionID) = value; }

      static constexpr Scalar_ GetBoxMinC(AlignedBox_ const& box, dim_t dimensionID) { return box.min()(dimensionID); }
      static constexpr Scalar_ GetBoxMaxC(AlignedBox_ const& box, dim_t dimensionID) { return box.max()(dimensionID); }
      static constexpr void SetBoxMinC(AlignedBox_& box, dim_t dimensionID, Scalar_ value) { box.min()(dimensionID) = value; }
      static constexpr void SetBoxMaxC(AlignedBox_& box, dim_t dimensionID, Scalar_ value) { box.max()(dimensionID) = value; }

      static constexpr VectorType_ const& GetRayDirection(Ray_ const& ray) noexcept { return ray.direction(); }
      static constexpr VectorType_ const& GetRayOrigin(Ray_ const& ray) noexcept { return ray.origin(); }

      static constexpr VectorType_ GetPlaneNormal(Plane_ const& plane) noexcept { return plane.normal(); }
      static constexpr Scalar_ GetPlaneOrigoDistance(Plane_ const& plane) noexcept { return -plane.offset(); }
    };

    template<typename Scalar_, int AmbientDim_>
    struct EigenAdaptorGeneralBase : EigenAdaptorBasics<Scalar_, AmbientDim_>
    {
      using Base = EigenAdaptorBasics<Scalar_, AmbientDim_>;
      using VectorType_ = Base::VectorType_;
      using AlignedBox_ = Base::AlignedBox_;
      using Ray_ = ParametrizedLine<Scalar_, AmbientDim_>;
      using Plane_ = Hyperplane<Scalar_, AmbientDim_>;

      static_assert(AdaptorBasicsConcept<Base, VectorType_, AlignedBox_, Ray_, Plane_, Scalar_>);

      static constexpr Scalar_ Size2(VectorType_ const& v) noexcept { return v.squaredNorm(); }

      static constexpr Scalar_ Size(VectorType_ const& v) noexcept { return v.norm(); }

      static constexpr VectorType_ Add(VectorType_ const& v1, VectorType_ const& v2) noexcept { return v1 + v2; }

      static constexpr VectorType_ Subtract(VectorType_ const& v1, VectorType_ const& v2) noexcept { return v1 - v2; }

      static constexpr VectorType_ Multiply(VectorType_ const& v, double scalarFactor) noexcept { return v * scalarFactor; }

      static constexpr Scalar_ Dot(VectorType_ const& v1, VectorType_ const& v2) noexcept { return v1.dot(v2); }

      static constexpr Scalar_ Distance(VectorType_ const& v1, VectorType_ const& v2) noexcept { return Size(v1 - v2); }

      static constexpr Scalar_ Distance2(VectorType_ const& v1, VectorType_ const& v2) noexcept { return Size2(v1 - v2); }

      static constexpr bool ArePointsEqual(VectorType_ const& v1, VectorType_ const& v2, Scalar_ tolerance) noexcept
      {
        return Distance2(v1, v2) <= tolerance * tolerance;
      }

      static constexpr bool IsNormalizedVector(VectorType_ const& normal) noexcept { return std::abs(Size2(normal) - 1.0) < 0.000001; }

      static constexpr bool DoesBoxContainPoint(AlignedBox_ const& box, VectorType_ const& point) noexcept { return box.contains(point); }

      static constexpr bool DoesBoxContainPointStrict(AlignedBox_ const& box, VectorType_ const& point) noexcept
      {
        for (dim_t dimensionID = 0; dimensionID < AmbientDim_; ++dimensionID)
          if (!(Base::GetBoxMinC(box, dimensionID) < Base::GetPointC(point, dimensionID) &&
                Base::GetPointC(point, dimensionID) < Base::GetBoxMaxC(box, dimensionID)))
            return false;

        return true;
      }

      static constexpr bool AreBoxesOverlappedStrict(AlignedBox_ const& e1, AlignedBox_ const& e2) noexcept
      {
        auto const e3 = e1.intersection(e2);
        auto const sizes = e3.sizes();
        for (dim_t dimensionID = 0; dimensionID < AmbientDim_; ++dimensionID)
          if (sizes[dimensionID] <= 0.0)
            return false;

        return true;
      }

      static constexpr bool AreBoxesOverlapped(
        AlignedBox_ const& e1, AlignedBox_ const& e2, bool e1_must_contain_e2 = true, bool fOverlapPtTouchAllowed = false) noexcept
      {
        if (e1_must_contain_e2)
          return e1.contains(e2);
        else if (!fOverlapPtTouchAllowed)
          return AreBoxesOverlappedStrict(e1, e2);
        else
          return e1.intersects(e2);
      }

      static AlignedBox_ GetBoxOfPoints(std::span<VectorType_ const> const& points) noexcept
      {
        auto ext = points.size() == 0 ? AlignedBox_{} : AlignedBox_(points[0]);
        for (auto const& point : points)
          ext.extend(point);

        return ext;
      }

      static AlignedBox_ GetBoxOfBoxes(std::span<AlignedBox_ const> const& extents) noexcept
      {
        auto ext = extents.size() == 0 ? AlignedBox_{} : extents[0];
        for (auto const& extent : extents)
          ext.extend(extent);

        return ext;
      }

      static void MoveBox(AlignedBox_& box, VectorType_ const& moveVector) noexcept { box.translate(moveVector); }

      static constexpr std::optional<double> IsRayHit(
        AlignedBox_ const& box, VectorType_ const& rayBasePoint, VectorType_ const& rayHeading, Scalar_ tolerance) noexcept
      {
        auto const toleranceVector = VectorType_::Ones() * tolerance;
        auto const rayBasePointBox = AlignedBox_(rayBasePoint - toleranceVector, rayBasePoint + toleranceVector);
        if (box.intersects(rayBasePointBox))
          return 0.0;

        auto constexpr inf = std::numeric_limits<double>::infinity();

        auto minDistances = std::array<double, AmbientDim_>{};
        auto maxDistances = std::array<double, AmbientDim_>{};
        for (dim_t dimensionID = 0; dimensionID < AmbientDim_; ++dimensionID)
        {
          auto const hComp = Base::GetPointC(rayHeading, dimensionID);
          if (hComp == 0)
          {
            if (Base::GetBoxMaxC(box, dimensionID) + tolerance < Base::GetPointC(rayBasePoint, dimensionID))
              return std::nullopt;

            if (Base::GetBoxMinC(box, dimensionID) - tolerance > Base::GetPointC(rayBasePoint, dimensionID))
              return std::nullopt;

            minDistances[dimensionID] = -inf;
            maxDistances[dimensionID] = +inf;
            continue;
          }

          minDistances[dimensionID] =
            ((hComp > 0.0 ? (Base::GetBoxMinC(box, dimensionID) - tolerance) : (Base::GetBoxMaxC(box, dimensionID) + tolerance)) -
             Base::GetPointC(rayBasePoint, dimensionID)) /
            hComp;
          maxDistances[dimensionID] =
            ((hComp < 0.0 ? (Base::GetBoxMinC(box, dimensionID) - tolerance) : (Base::GetBoxMaxC(box, dimensionID) + tolerance)) -
             Base::GetPointC(rayBasePoint, dimensionID)) /
            hComp;
        }

        auto const rMin = *std::ranges::max_element(minDistances);
        auto const rMax = *std::ranges::min_element(maxDistances);
        if (rMin > rMax || rMax < 0.0)
          return std::nullopt;

        return rMin < 0 ? rMax : rMin;
      }

      static constexpr std::optional<double> IsRayHit(AlignedBox_ const& box, Ray_ const& ray, Scalar_ tolerance) noexcept
      {
        return IsRayHit(box, Base::GetRayOrigin(ray), Base::GetRayDirection(ray), tolerance);
      }

      // Get point-Hyperplane relation (Plane equation: dotProduct(planeNormal, point) = distanceOfOrigo)
      static constexpr PlaneRelation GetPointPlaneRelation(
        VectorType_ const& point, Scalar_ distanceOfOrigo, VectorType_ const& planeNormal, Scalar_ tolerance) noexcept
      {
        assert(IsNormalizedVector(planeNormal));

        auto const pointProjected = Dot(planeNormal, point);

        if (pointProjected < distanceOfOrigo - tolerance)
          return PlaneRelation::Negative;

        if (pointProjected > distanceOfOrigo + tolerance)
          return PlaneRelation::Positive;

        return PlaneRelation::Hit;
      }

      // Get box-Hyperplane relation (Plane equation: dotProduct(planeNormal, point) = distanceOfOrigo)
      static constexpr PlaneRelation GetBoxPlaneRelation(AlignedBox_ const& box, Scalar_ distanceOfOrigo, VectorType_ const& planeNormal, Scalar_ tolerance) noexcept
      {
        assert(IsNormalizedVector(planeNormal));

        VectorType_ center, radius;
        for (dim_t dimensionID = 0; dimensionID < AmbientDim_; ++dimensionID)
        {
          auto const minComponent = Base::GetBoxMinC(box, dimensionID);
          auto const maxComponent = Base::GetBoxMaxC(box, dimensionID);
          auto const centerComponent = static_cast<Scalar_>((minComponent + maxComponent) * 0.5);
          Base::SetPointC(center, dimensionID, centerComponent);
          Base::SetPointC(radius, dimensionID, centerComponent - minComponent);
        }

        auto radiusProjected = double(tolerance);
        for (dim_t dimensionID = 0; dimensionID < AmbientDim_; ++dimensionID)
          radiusProjected += Base::GetPointC(radius, dimensionID) * std::abs(Base::GetPointC(planeNormal, dimensionID));

        auto const centerProjected = Dot(planeNormal, center);

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
  using namespace OrthoTree::EigenAdaptor;

  // Basic OrthoTree types
  template<typename Scalar_, int AmbientDim_>
  using EigenOrthoTreePoint = OrthoTree::OrthoTreePoint<
    AmbientDim_,
    Matrix<Scalar_, AmbientDim_, 1>,
    AlignedBox<Scalar_, AmbientDim_>,
    ParametrizedLine<Scalar_, AmbientDim_>,
    Hyperplane<Scalar_, AmbientDim_>,
    Scalar_,
    EigenAdaptorGeneralBase<Scalar_, AmbientDim_>>;

  template<typename Scalar_, int AmbientDim_, uint32_t SPLIT_DEPTH_INCREASEMENT = 2>
  using EigenOrthoTreeBox = OrthoTree::OrthoTreeBoundingBox<
    AmbientDim_,
    Matrix<Scalar_, AmbientDim_, 1>,
    AlignedBox<Scalar_, AmbientDim_>,
    ParametrizedLine<Scalar_, AmbientDim_>,
    Hyperplane<Scalar_, AmbientDim_>,
    Scalar_,
    SPLIT_DEPTH_INCREASEMENT,
    EigenAdaptorGeneralBase<Scalar_, AmbientDim_>>;

  template<typename Scalar_, int AmbientDim_>
  using OrthoTreeContainerPointC = OrthoTree::OrthoTreeContainerPoint<EigenOrthoTreePoint<Scalar_, AmbientDim_>, Matrix<Scalar_, AmbientDim_, 1>>;

  template<typename Scalar_, int AmbientDim_, uint32_t SPLIT_DEPTH_INCREASEMENT = 2>
  using OrthoTreeContainerBoxC =
    OrthoTree::OrthoTreeContainerBox<EigenOrthoTreeBox<Scalar_, AmbientDim_, SPLIT_DEPTH_INCREASEMENT>, AlignedBox<Scalar_, AmbientDim_>>;

  // Non-owning types
  using QuadtreePoint2f = EigenOrthoTreePoint<float, 2>;

  using QuadtreePoint2d = EigenOrthoTreePoint<double, 2>;

  using OctreePoint3f = EigenOrthoTreePoint<float, 3>;

  using OctreePoint3d = EigenOrthoTreePoint<double, 3>;

  template<uint32_t SPLIT_DEPTH_INCREASEMENT = 2>
  using QuadtreeBox2fs = EigenOrthoTreeBox<float, 2, SPLIT_DEPTH_INCREASEMENT>;
  using QuadtreeBox2f = QuadtreeBox2fs<2>;

  template<uint32_t SPLIT_DEPTH_INCREASEMENT = 2>
  using QuadtreeBox2ds = EigenOrthoTreeBox<double, 2, SPLIT_DEPTH_INCREASEMENT>;
  using QuadtreeBox2d = QuadtreeBox2ds<2>;

  template<uint32_t SPLIT_DEPTH_INCREASEMENT = 2>
  using OctreeBox3fs = EigenOrthoTreeBox<float, 3, SPLIT_DEPTH_INCREASEMENT>;
  using OctreeBox3f = OctreeBox3fs<2>;

  template<uint32_t SPLIT_DEPTH_INCREASEMENT = 2>
  using OctreeBox3ds = EigenOrthoTreeBox<double, 3, SPLIT_DEPTH_INCREASEMENT>;
  using OctreeBox3d = OctreeBox3ds<2>;

  // Container types
  using QuadtreePointC2f = OrthoTreeContainerPointC<float, 2>;

  using QuadtreePointC2d = OrthoTreeContainerPointC<double, 2>;

  using OctreePointC3f = OrthoTreeContainerPointC<float, 3>;

  using OctreePointC3d = OrthoTreeContainerPointC<double, 3>;

  template<uint32_t SPLIT_DEPTH_INCREASEMENT = 2>
  using QuadtreeBoxC2fs = OrthoTreeContainerBoxC<float, 2, SPLIT_DEPTH_INCREASEMENT>;
  using QuadtreeBoxC2f = QuadtreeBoxC2fs<2>;

  template<uint32_t SPLIT_DEPTH_INCREASEMENT = 2>
  using QuadtreeBoxC2ds = OrthoTreeContainerBoxC<double, 2, SPLIT_DEPTH_INCREASEMENT>;
  using QuadtreeBoxC2d = QuadtreeBoxC2ds<2>;

  template<uint32_t SPLIT_DEPTH_INCREASEMENT = 2>
  using OctreeBoxC3fs = OrthoTreeContainerBoxC<float, 3, SPLIT_DEPTH_INCREASEMENT>;
  using OctreeBoxC3f = OctreeBoxC3fs<2>;

  template<uint32_t SPLIT_DEPTH_INCREASEMENT = 2>
  using OctreeBoxC3ds = OrthoTreeContainerBoxC<double, 3, SPLIT_DEPTH_INCREASEMENT>;
  using OctreeBoxC3d = OctreeBoxC3ds<2>;

} // namespace Eigen