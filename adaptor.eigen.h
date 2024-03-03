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

      static constexpr Scalar_ GetPointC(VectorType_ const& point, dim_t dimensionID) noexcept { return point(dimensionID); }
      static constexpr void SetPointC(VectorType_& point, dim_t dimensionID, Scalar_ value) noexcept { point(dimensionID) = value; }

      static constexpr Scalar_ GetBoxMinC(AlignedBox_ const& box, dim_t dimensionID) { return box.min()(dimensionID); }
      static constexpr Scalar_ GetBoxMaxC(AlignedBox_ const& box, dim_t dimensionID) { return box.max()(dimensionID); }
      static constexpr void SetBoxMinC(AlignedBox_& box, dim_t dimensionID, Scalar_ value) { box.min()(dimensionID) = value; }
      static constexpr void SetBoxMaxC(AlignedBox_& box, dim_t dimensionID, Scalar_ value) { box.max()(dimensionID) = value; }
    };

    template<typename Scalar_, int AmbientDim_>
    struct EigenAdaptorGeneralBase : EigenAdaptorBasics<Scalar_, AmbientDim_>
    {
      using Base = EigenAdaptorBasics<Scalar_, AmbientDim_>;
      using VectorType_ = Base::VectorType_;
      using AlignedBox_ = Base::AlignedBox_;

      static_assert(AdaptorBasicsConcept<Base, VectorType_, AlignedBox_, Scalar_>);

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
        autoc e3 = e1.intersection(e2);
        autoc sizes = e3.sizes();
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
        for (autoc& point : points)
          ext.extend(point);

        return ext;
      }

      static AlignedBox_ GetBoxOfBoxes(std::span<AlignedBox_ const> const& extents) noexcept
      {
        auto ext = extents.size() == 0 ? AlignedBox_{} : extents[0];
        for (autoc& extent : extents)
          ext.extend(extent);

        return ext;
      }

      static void MoveBox(AlignedBox_& box, VectorType_ const& moveVector) noexcept { box.translate(moveVector); }

      static constexpr std::optional<double> IsRayHit(
        AlignedBox_ const& box, VectorType_ const& rayBasePoint, VectorType_ const& rayHeading, Scalar_ tolerance) noexcept
      {
        autoc toleranceVector = VectorType_::Ones() * tolerance;
        autoc rayBasePointBox = AlignedBox_(rayBasePoint - toleranceVector, rayBasePoint + toleranceVector);
        if (box.intersects(rayBasePointBox))
          return 0.0;

        autoce inf = std::numeric_limits<double>::infinity();

        auto minDistances = std::array<double, AmbientDim_>{};
        auto maxDistances = std::array<double, AmbientDim_>{};
        for (dim_t dimensionID = 0; dimensionID < AmbientDim_; ++dimensionID)
        {
          autoc hComp = Base::GetPointC(rayHeading, dimensionID);
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

          minDistances[dimensionID] = ((hComp > 0.0 ? Base::GetBoxMinC(box, dimensionID) : Base::GetBoxMaxC(box, dimensionID)) - tolerance -
                                       Base::GetPointC(rayBasePoint, dimensionID)) /
                                      hComp;
          maxDistances[dimensionID] = ((hComp < 0.0 ? Base::GetBoxMinC(box, dimensionID) : Base::GetBoxMaxC(box, dimensionID)) + tolerance -
                                       Base::GetPointC(rayBasePoint, dimensionID)) /
                                      hComp;
        }

        autoc rMin = *std::ranges::max_element(minDistances);
        autoc rMax = *std::ranges::min_element(maxDistances);
        if (rMin > rMax || rMax < 0.0)
          return std::nullopt;

        return rMin < 0 ? rMax : rMin;
      }

      // Get point-Hyperplane relation (Plane equation: dotProduct(planeNormal, point) = distanceOfOrigo)
      static constexpr PlaneRelation GetPointPlaneRelation(
        VectorType_ const& point, Scalar_ distanceOfOrigo, VectorType_ const& planeNormal, Scalar_ tolerance) noexcept
      {
        assert(IsNormalizedVector(planeNormal));

        autoc pointProjected = Dot(planeNormal, point);

        if (pointProjected < distanceOfOrigo - tolerance)
          return PlaneRelation::Negative;

        if (pointProjected > distanceOfOrigo + tolerance)
          return PlaneRelation::Positive;

        return PlaneRelation::Hit;
      }

      // Get box-Hyperplane relation (Plane equation: dotProduct(planeNormal, point) = distanceOfOrigo)
      static constexpr PlaneRelation GetBoxPlaneRelation(
        AlignedBox_ const& box, Scalar_ distanceOfOrigo, VectorType_ const& planeNormal, Scalar_ tolerance) noexcept
      {
        assert(IsNormalizedVector(planeNormal));

        VectorType_ center, radius;
        for (dim_t dimensionID = 0; dimensionID < AmbientDim_; ++dimensionID)
        {
          autoc minComponent = Base::GetBoxMinC(box, dimensionID);
          autoc maxComponent = Base::GetBoxMaxC(box, dimensionID);
          autoc centerComponent = static_cast<Scalar_>((minComponent + maxComponent) * 0.5);
          Base::SetPointC(center, dimensionID, centerComponent);
          Base::SetPointC(radius, dimensionID, centerComponent - minComponent);
        }

        auto radiusProjected = double(tolerance);
        for (dim_t dimensionID = 0; dimensionID < AmbientDim_; ++dimensionID)
          radiusProjected += Base::GetPointC(radius, dimensionID) * std::abs(Base::GetPointC(planeNormal, dimensionID));

        autoc centerProjected = Dot(planeNormal, center);

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

  template<typename Scalar_, int AmbientDim_, uint32_t SPLIT_DEPTH_INCREASEMENT = 2>
  using EigenOrthoTreeBox =
    OrthoTreeBoundingBox<AmbientDim_, Matrix<Scalar_, AmbientDim_, 1>, AlignedBox<Scalar_, AmbientDim_>, EigenAdaptorGeneralBase<Scalar_, AmbientDim_>, Scalar_, SPLIT_DEPTH_INCREASEMENT>;

  template<typename Scalar_, int AmbientDim_>
  using OrthoTreeContainerPointC = OrthoTreeContainerPoint<EigenOrthoTreePoint<Scalar_, AmbientDim_>, Matrix<Scalar_, AmbientDim_, 1>>;

  template<typename Scalar_, int AmbientDim_, uint32_t SPLIT_DEPTH_INCREASEMENT = 2>
  using OrthoTreeContainerBoxC =
    OrthoTreeContainerBox<EigenOrthoTreeBox<Scalar_, AmbientDim_, SPLIT_DEPTH_INCREASEMENT>, AlignedBox<Scalar_, AmbientDim_>>;

  // Non-owning types
  using QuadtreePoint2f = EigenOrthoTreePoint<float, 2>;

  using QuadtreePoint2d = EigenOrthoTreePoint<double, 2>;

  using OctreePoint3f = EigenOrthoTreePoint<float, 3>;

  using OctreePoint3d = EigenOrthoTreePoint<double, 3>;

  template<uint32_t SPLIT_DEPTH_INCREASEMENT = 2>
  using QuadtreeBox2f = EigenOrthoTreeBox<float, 2, SPLIT_DEPTH_INCREASEMENT>;

  template<uint32_t SPLIT_DEPTH_INCREASEMENT = 2>
  using QuadtreeBox2d = EigenOrthoTreeBox<double, 2, SPLIT_DEPTH_INCREASEMENT>;

  template<uint32_t SPLIT_DEPTH_INCREASEMENT = 2>
  using OctreeBox3f = EigenOrthoTreeBox<float, 3, SPLIT_DEPTH_INCREASEMENT>;

  template<uint32_t SPLIT_DEPTH_INCREASEMENT = 2>
  using OctreeBox3d = EigenOrthoTreeBox<double, 3, SPLIT_DEPTH_INCREASEMENT>;

  // Container types
  using QuadtreePointC2f = OrthoTreeContainerPointC<float, 2>;

  using QuadtreePointC2d = OrthoTreeContainerPointC<double, 2>;

  using OctreePointC3f = OrthoTreeContainerPointC<float, 3>;

  using OctreePointC3d = OrthoTreeContainerPointC<double, 3>;

  template<uint32_t SPLIT_DEPTH_INCREASEMENT = 2>
  using QuadtreeBoxC2f = OrthoTreeContainerBoxC<float, 2, SPLIT_DEPTH_INCREASEMENT>;

  template<uint32_t SPLIT_DEPTH_INCREASEMENT = 2>
  using QuadtreeBoxC2d = OrthoTreeContainerBoxC<double, 2, SPLIT_DEPTH_INCREASEMENT>;

  template<uint32_t SPLIT_DEPTH_INCREASEMENT = 2>
  using OctreeBoxC3f = OrthoTreeContainerBoxC<float, 3, SPLIT_DEPTH_INCREASEMENT>;

  template<uint32_t SPLIT_DEPTH_INCREASEMENT = 2>
  using OctreeBoxC3d = OrthoTreeContainerBoxC<double, 3, SPLIT_DEPTH_INCREASEMENT>;
} // namespace Eigen