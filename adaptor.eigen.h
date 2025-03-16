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

#ifdef _MSC_VER
// Warning: std::optional + Eigen::AlignedBox: "structure was padded due to alignment specifier"
// Disable: This value will be immediately converted to another type, there is no alignment relevant instructions.
#pragma warning(disable : 4324)
#endif

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

      static constexpr std::optional<double> GetRayBoxDistance(
        AlignedBox_ const& box, VectorType_ const& rayBasePoint, VectorType_ const& rayHeading, Scalar_ tolerance) noexcept
      {
        auto const toleranceVector = VectorType_::Ones() * tolerance;
        auto const rayBasePointBox = AlignedBox_(rayBasePoint - toleranceVector, rayBasePoint + toleranceVector);
        if (box.intersects(rayBasePointBox))
          return 0.0;

        auto constexpr inf = std::numeric_limits<double>::max();

        auto minBoxDistances = std::array<double, AmbientDim_>{};
        auto maxBoxDistances = std::array<double, AmbientDim_>{};
        for (dim_t dimensionID = 0; dimensionID < AmbientDim_; ++dimensionID)
        {
          auto const dirComp = Base::GetPointC(rayHeading, dimensionID);
          if (dirComp == 0)
          {
            if (tolerance != 0.0)
            {
              // Box should be within tolerance (<, not <=)

              assert(tolerance > 0);
              if (Base::GetBoxMaxC(box, dimensionID) + tolerance <= Base::GetPointC(rayBasePoint, dimensionID))
                return std::nullopt;

              if (Base::GetBoxMinC(box, dimensionID) - tolerance >= Base::GetPointC(rayBasePoint, dimensionID))
                return std::nullopt;
            }
            else
            {
              if (Base::GetBoxMaxC(box, dimensionID) < Base::GetPointC(rayBasePoint, dimensionID))
                return std::nullopt;

              if (Base::GetBoxMinC(box, dimensionID) > Base::GetPointC(rayBasePoint, dimensionID))
                return std::nullopt;
            }

            minBoxDistances[dimensionID] = -inf;
            maxBoxDistances[dimensionID] = +inf;
          }
          else
          {
            auto const minBox = Base::GetBoxMinC(box, dimensionID) - tolerance;
            auto const maxBox = Base::GetBoxMaxC(box, dimensionID) + tolerance;
            auto const pointComp = Base::GetPointC(rayBasePoint, dimensionID);
            auto const dirCompRecip = 1.0 / dirComp;
            if (dirComp < 0.0)
            {
              minBoxDistances[dimensionID] = (maxBox - pointComp) * dirCompRecip;
              maxBoxDistances[dimensionID] = (minBox - pointComp) * dirCompRecip;
            }
            else
            {
              minBoxDistances[dimensionID] = (minBox - pointComp) * dirCompRecip;
              maxBoxDistances[dimensionID] = (maxBox - pointComp) * dirCompRecip;
            }
          }
        }

        auto const minBoxDistance = *std::ranges::max_element(minBoxDistances);
        auto const maxBoxDistance = *std::ranges::min_element(maxBoxDistances);
        if (minBoxDistance > maxBoxDistance || maxBoxDistance < 0.0)
          return std::nullopt;
        else
          return minBoxDistance < 0 ? maxBoxDistance : minBoxDistance;
      }

      static constexpr std::optional<double> GetRayBoxDistance(AlignedBox_ const& box, Ray_ const& ray, Scalar_ tolerance) noexcept
      {
        return GetRayBoxDistance(box, Base::GetRayOrigin(ray), Base::GetRayDirection(ray), tolerance);
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

  template<typename Scalar_, int AmbientDim_>
  using PointSpan = std::span<Matrix<Scalar_, AmbientDim_, 1> const>;

  template<typename Scalar_, int AmbientDim_>
  using BoxSpan = std::span<AlignedBox<Scalar_, AmbientDim_> const>;

  template<typename Scalar_, int AmbientDim_>
  using PointMap = std::unordered_map<int, Matrix<Scalar_, AmbientDim_, 1>>;

  template<typename Scalar_, int AmbientDim_>
  using BoxMap = std::unordered_map<int, AlignedBox<Scalar_, AmbientDim_>>;

  // Basic OrthoTree types
  template<typename Scalar_, int AmbientDim_, typename Container_ = PointSpan<Scalar_, AmbientDim_>>
  using EigenOrthoTreePoint = OrthoTree::OrthoTreePoint<
    AmbientDim_,
    Matrix<Scalar_, AmbientDim_, 1>,
    AlignedBox<Scalar_, AmbientDim_>,
    ParametrizedLine<Scalar_, AmbientDim_>,
    Hyperplane<Scalar_, AmbientDim_>,
    Scalar_,
    EigenAdaptorGeneralBase<Scalar_, AmbientDim_>,
    Container_>;

  template<typename Scalar_, int AmbientDim_, bool DO_SPLIT_PARENT_ENTITIES = true, typename Container_ = BoxSpan<Scalar_, AmbientDim_>>
  using EigenOrthoTreeBox = OrthoTree::OrthoTreeBoundingBox<
    AmbientDim_,
    Matrix<Scalar_, AmbientDim_, 1>,
    AlignedBox<Scalar_, AmbientDim_>,
    ParametrizedLine<Scalar_, AmbientDim_>,
    Hyperplane<Scalar_, AmbientDim_>,
    Scalar_,
    DO_SPLIT_PARENT_ENTITIES,
    EigenAdaptorGeneralBase<Scalar_, AmbientDim_>,
    Container_>;


  template<typename Scalar_, int AmbientDim_, typename Container_ = PointSpan<Scalar_, AmbientDim_>>
  using OrthoTreeContainerPointC = OrthoTree::OrthoTreeContainerPoint<EigenOrthoTreePoint<Scalar_, AmbientDim_, Container_>>;

  template<typename Scalar_, int AmbientDim_, bool DO_SPLIT_PARENT_ENTITIES = true, typename Container_ = BoxSpan<Scalar_, AmbientDim_>>
  using OrthoTreeContainerBoxC = OrthoTree::OrthoTreeContainerBox<EigenOrthoTreeBox<Scalar_, AmbientDim_, DO_SPLIT_PARENT_ENTITIES, Container_>>;

  // Non-owning types
  using QuadtreePoint2f = EigenOrthoTreePoint<float, 2>;

  using QuadtreePoint2d = EigenOrthoTreePoint<double, 2>;

  using OctreePoint3f = EigenOrthoTreePoint<float, 3>;

  using OctreePoint3d = EigenOrthoTreePoint<double, 3>;

  template<bool DO_SPLIT_PARENT_ENTITIES = true>
  using QuadtreeBox2fs = EigenOrthoTreeBox<float, true, DO_SPLIT_PARENT_ENTITIES>;
  using QuadtreeBox2f = QuadtreeBox2fs<true>;

  template<bool DO_SPLIT_PARENT_ENTITIES = true>
  using QuadtreeBox2ds = EigenOrthoTreeBox<double, true, DO_SPLIT_PARENT_ENTITIES>;
  using QuadtreeBox2d = QuadtreeBox2ds<true>;

  template<bool DO_SPLIT_PARENT_ENTITIES = true>
  using OctreeBox3fs = EigenOrthoTreeBox<float, 3, DO_SPLIT_PARENT_ENTITIES>;
  using OctreeBox3f = OctreeBox3fs<true>;

  template<bool DO_SPLIT_PARENT_ENTITIES = true>
  using OctreeBox3ds = EigenOrthoTreeBox<double, 3, DO_SPLIT_PARENT_ENTITIES>;
  using OctreeBox3d = OctreeBox3ds<true>;

  // Container types
  using QuadtreePointC2f = OrthoTreeContainerPointC<float, 2>;

  using QuadtreePointC2d = OrthoTreeContainerPointC<double, 2>;

  using OctreePointC3f = OrthoTreeContainerPointC<float, 3>;

  using OctreePointC3d = OrthoTreeContainerPointC<double, 3>;

  template<bool DO_SPLIT_PARENT_ENTITIES = true>
  using QuadtreeBoxC2fs = OrthoTreeContainerBoxC<float, 2, DO_SPLIT_PARENT_ENTITIES>;
  using QuadtreeBoxC2f = QuadtreeBoxC2fs<true>;

  template<bool DO_SPLIT_PARENT_ENTITIES = true>
  using QuadtreeBoxC2ds = OrthoTreeContainerBoxC<double, 2, DO_SPLIT_PARENT_ENTITIES>;
  using QuadtreeBoxC2d = QuadtreeBoxC2ds<true>;

  template<bool DO_SPLIT_PARENT_ENTITIES = true>
  using OctreeBoxC3fs = OrthoTreeContainerBoxC<float, 3, DO_SPLIT_PARENT_ENTITIES>;
  using OctreeBoxC3f = OctreeBoxC3fs<true>;

  template<bool DO_SPLIT_PARENT_ENTITIES = true>
  using OctreeBoxC3ds = OrthoTreeContainerBoxC<double, 3, DO_SPLIT_PARENT_ENTITIES>;
  using OctreeBoxC3d = OctreeBoxC3ds<true>;


  // Map types

  // Non-owning types
  using QuadtreePointMap2f = EigenOrthoTreePoint<float, 2, PointMap<float, 2>>;

  using QuadtreePointMap2d = EigenOrthoTreePoint<double, 2, PointMap<double, 2>>;

  using OctreePointMap3f = EigenOrthoTreePoint<float, 3, PointMap<float, 3>>;

  using OctreePointMap3d = EigenOrthoTreePoint<double, 3, PointMap<double, 3>>;

  template<bool DO_SPLIT_PARENT_ENTITIES = true>
  using QuadtreeBox2Mapfs = EigenOrthoTreeBox<float, 2, DO_SPLIT_PARENT_ENTITIES, BoxMap<float, 2>>;
  using QuadtreeBox2Mapf = QuadtreeBox2Mapfs<true>;

  template<bool DO_SPLIT_PARENT_ENTITIES = true>
  using QuadtreeBox2Mapds = EigenOrthoTreeBox<double, 2, DO_SPLIT_PARENT_ENTITIES, BoxMap<double, 2>>;
  using QuadtreeBox2Mapd = QuadtreeBox2Mapds<true>;

  template<bool DO_SPLIT_PARENT_ENTITIES = true>
  using OctreeBox3Mapfs = EigenOrthoTreeBox<float, 3, DO_SPLIT_PARENT_ENTITIES, BoxMap<float, 3>>;
  using OctreeBox3Mapf = OctreeBox3Mapfs<true>;

  template<bool DO_SPLIT_PARENT_ENTITIES = true>
  using OctreeBox3Mapds = EigenOrthoTreeBox<double, 3, DO_SPLIT_PARENT_ENTITIES, BoxMap<double, 3>>;
  using OctreeBox3Mapd = OctreeBox3Mapds<true>;

  // Container types
  using QuadtreePointCMap2f = OrthoTreeContainerPointC<float, 2, PointMap<float, 2>>;

  using QuadtreePointCMap2d = OrthoTreeContainerPointC<double, 2, PointMap<double, 2>>;

  using OctreePointCMap3f = OrthoTreeContainerPointC<float, 3, PointMap<float, 3>>;

  using OctreePointCMap3d = OrthoTreeContainerPointC<double, 3, PointMap<double, 3>>;

  template<bool DO_SPLIT_PARENT_ENTITIES = true>
  using QuadtreeBoxCMap2fs = OrthoTreeContainerBoxC<float, 2, DO_SPLIT_PARENT_ENTITIES, BoxMap<float, 2>>;
  using QuadtreeBoxCMap2f = QuadtreeBoxCMap2fs<true>;

  template<bool DO_SPLIT_PARENT_ENTITIES = true>
  using QuadtreeBoxCMap2ds = OrthoTreeContainerBoxC<double, 2, DO_SPLIT_PARENT_ENTITIES, BoxMap<double, 2>>;
  using QuadtreeBoxCMap2d = QuadtreeBoxCMap2ds<true>;

  template<bool DO_SPLIT_PARENT_ENTITIES = true>
  using OctreeBoxCMap3fs = OrthoTreeContainerBoxC<float, 3, DO_SPLIT_PARENT_ENTITIES, BoxMap<float, 3>>;
  using OctreeBoxCMap3f = OctreeBoxCMap3fs<true>;

  template<bool DO_SPLIT_PARENT_ENTITIES = true>
  using OctreeBoxC3Mapds = OrthoTreeContainerBoxC<double, 3, DO_SPLIT_PARENT_ENTITIES, BoxMap<double, 3>>;
  using OctreeBoxC3Mapd = OctreeBoxC3Mapds<true>;
} // namespace Eigen
