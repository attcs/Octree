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

#include "../detail/common.h"
#include "../octree.h"
#include <Eigen/Geometry>

namespace OrthoTree
{
  namespace EigenAdapter
  {
    using namespace Eigen;

    template<typename TScalar, int AmbientDim>
    struct EigenBaseGeometryAdapter
    {
      using Scalar = TScalar;
      using FloatScalar = std::conditional_t<std::is_floating_point_v<Scalar>, Scalar, float>;
      using Vector = Matrix<Scalar, AmbientDim, 1>;
      using Box = AlignedBox<Scalar, AmbientDim>;
      using Ray = ParametrizedLine<Scalar, AmbientDim>;
      using Plane = Hyperplane<Scalar, AmbientDim>;

      static constexpr dim_t DIMENSION_NO = AmbientDim;
      static constexpr FloatScalar BASE_TOLERANCE = std::numeric_limits<FloatScalar>::epsilon() * FloatScalar(10);

      static constexpr Scalar GetPointC(Vector const& point, dim_t dimensionID) noexcept { return point(dimensionID); }
      static constexpr void SetPointC(Vector& point, dim_t dimensionID, Scalar value) noexcept { point(dimensionID) = value; }

      static constexpr Scalar GetBoxMinC(Box const& box, dim_t dimensionID) { return box.min()(dimensionID); }
      static constexpr Scalar GetBoxMaxC(Box const& box, dim_t dimensionID) { return box.max()(dimensionID); }
      static constexpr void SetBoxMinC(Box& box, dim_t dimensionID, Scalar value) { box.min()(dimensionID) = value; }
      static constexpr void SetBoxMaxC(Box& box, dim_t dimensionID, Scalar value) { box.max()(dimensionID) = value; }

      static constexpr Vector const& GetRayDirection(Ray const& ray) noexcept { return ray.direction(); }
      static constexpr Vector const& GetRayOrigin(Ray const& ray) noexcept { return ray.origin(); }

      static constexpr Vector GetPlaneNormal(Plane const& plane) noexcept { return plane.normal(); }
      static constexpr Scalar GetPlaneOrigoDistance(Plane const& plane) noexcept { return -plane.offset(); }
    };

    template<typename TScalar, int AmbientDim>
    struct EigenGeometryAdapter : EigenBaseGeometryAdapter<TScalar, AmbientDim>
    {
      using Base = EigenBaseGeometryAdapter<TScalar, AmbientDim>;

      using Scalar = Base::Scalar;
      using FloatScalar = Base::FloatScalar;
      using Vector = Base::Vector;
      using Box = Base::Box;
      using Ray = Base::Ray;
      using Plane = Base::Plane;

      static_assert(BaseGeometryAdapterConcept<Base, AmbientDim, Vector, Box, Ray, Plane, Scalar, FloatScalar>);

      static constexpr Scalar Size2(Vector const& v) noexcept { return v.squaredNorm(); }

      static constexpr Scalar Size(Vector const& v) noexcept { return v.norm(); }

      static constexpr Vector Add(Vector const& v1, Vector const& v2) noexcept { return v1 + v2; }

      static constexpr Vector Subtract(Vector const& v1, Vector const& v2) noexcept { return v1 - v2; }

      static constexpr Vector Multiply(Vector const& v, FloatScalar scalarFactor) noexcept { return v * scalarFactor; }

      static constexpr Scalar Dot(Vector const& v1, Vector const& v2) noexcept { return v1.dot(v2); }

      static constexpr Scalar Distance(Vector const& v1, Vector const& v2) noexcept { return Size(v1 - v2); }

      static constexpr Scalar Distance2(Vector const& v1, Vector const& v2) noexcept { return Size2(v1 - v2); }


      struct PointBoxMinMaxDistance
      {
        // Minimum possible distance from the query point to any object contained in the box.
        // If the point lies inside the box, this value is zero.
        Scalar min = {};

        // Maximum possible nearest-distance in the worst case.
        // Assumes an object inside the box is placed adversarially so as to maximize
        // its minimum distance to the query point (i.e. the worst-case nearest object).
        Scalar minMax = {};
      };

      static constexpr PointBoxMinMaxDistance MinMaxDistance2(Vector const& pt, Box const& box, FloatScalar tolerance) noexcept
      {
        // N. Roussopoulos, S. Kelley, F. Vincent - Nearest Neighbor Queries (1995) DOI.10.1145 / 223784.223794
        // MINMAXDIST

        auto dist2 = PointBoxMinMaxDistance{};

        const Vector minDist = pt - box.min();
        const Vector maxDist = box.max() - pt;

        Scalar farthestInsideDistance2 = std::numeric_limits<Scalar>::max();
        Scalar largestMinMax2Difference = {};
        auto isInside = true;
        for (dim_t dimensionID = 0; dimensionID < AmbientDim; ++dimensionID)
        {
          bool isInsideInComponent = (-tolerance <= minDist[dimensionID] && -tolerance <= maxDist[dimensionID]);
          isInside &= isInsideInComponent;

          auto minDist2 = minDist[dimensionID] * minDist[dimensionID];
          auto maxDist2 = maxDist[dimensionID] * maxDist[dimensionID];

          if (maxDist2 < minDist2)
            std::swap(minDist2, maxDist2);

          if (isInside)
            farthestInsideDistance2 = std::min(farthestInsideDistance2, maxDist2);

          if (!isInsideInComponent)
            dist2.min += minDist2;

          largestMinMax2Difference = std::max(largestMinMax2Difference, maxDist2 - minDist2);
          dist2.minMax += maxDist2;
        }

        if (isInside)
        {
          dist2.min = {};
          dist2.minMax = farthestInsideDistance2;
        }
        else
        {
          dist2.minMax -= largestMinMax2Difference;
        }

        return dist2;
      }

      static constexpr PointBoxMinMaxDistance MinMaxDistance(Vector const& pt, Box const& box, FloatScalar tolerance) noexcept
      {
        auto dist = MinMaxDistance2(pt, box, tolerance);
        dist.min = std::sqrt(dist.min);
        dist.minMax = std::sqrt(dist.minMax);
        return dist;
      }

      static constexpr bool ArePointsEqual(Vector const& v1, Vector const& v2, FloatScalar tolerance) noexcept
      {
        return Distance2(v1, v2) <= tolerance * tolerance;
      }

      static constexpr bool IsNormalizedVector(Vector const& normal) noexcept { return std::abs(Size2(normal) - 1.0) < 0.000001; }

      static constexpr bool DoesBoxContainPoint(Box const& box, Vector const& point) noexcept { return box.contains(point); }

      static constexpr bool DoesBoxContainPointStrict(Box const& box, Vector const& point) noexcept
      {
        for (dim_t dimensionID = 0; dimensionID < AmbientDim; ++dimensionID)
          if (!(Base::GetBoxMinC(box, dimensionID) < Base::GetPointC(point, dimensionID) &&
                Base::GetPointC(point, dimensionID) < Base::GetBoxMaxC(box, dimensionID)))
            return false;

        return true;
      }

      static constexpr bool AreBoxesOverlappedStrict(Box const& e1, Box const& e2) noexcept
      {
        auto const e3 = e1.intersection(e2);
        auto const sizes = e3.sizes();
        for (dim_t dimensionID = 0; dimensionID < AmbientDim; ++dimensionID)
          if (sizes[dimensionID] <= 0.0)
            return false;

        return true;
      }

      static constexpr bool AreBoxesOverlapped(Box const& e1, Box const& e2, bool e1_must_contain_e2 = true, bool fOverlapPtTouchAllowed = false) noexcept
      {
        if (e1_must_contain_e2)
          return e1.contains(e2);
        else if (!fOverlapPtTouchAllowed)
          return AreBoxesOverlappedStrict(e1, e2);
        else
          return e1.intersects(e2);
      }

      static Box GetBoxOfPoints(std::span<Vector const> const& points) noexcept
      {
        auto ext = points.size() == 0 ? Box{} : Box(points[0]);
        for (auto const& point : points)
          ext.extend(point);

        return ext;
      }

      static Box GetBoxOfBoxes(std::span<Box const> const& extents) noexcept
      {
        auto ext = extents.size() == 0 ? Box{} : extents[0];
        for (auto const& extent : extents)
          ext.extend(extent);

        return ext;
      }

      static void MoveBox(Box& box, Vector const& moveVector) noexcept { box.translate(moveVector); }

      static constexpr std::optional<double> GetRayBoxDistance(Box const& box, Vector const& rayOrigin, Vector const& rayDirection, FloatScalar tolerance) noexcept
      {
        assert(tolerance >= 0 && "Tolerance cannot be negative!");
        auto const toleranceVector = Vector::Ones() * tolerance;
        auto const rayBasePointBox = Box(rayOrigin - toleranceVector, rayOrigin + toleranceVector);
        if (box.intersects(rayBasePointBox))
          return 0.0;

        auto constexpr inf = std::numeric_limits<double>::max();

        double minBoxDistance = -inf;
        double maxBoxDistance = +inf;
        for (dim_t dimensionID = 0; dimensionID < AmbientDim; ++dimensionID)
        {
          auto const origin = Base::GetPointC(rayOrigin, dimensionID);
          auto const direction = Base::GetPointC(rayDirection, dimensionID);
          auto const boxMin = Base::GetBoxMinC(box, dimensionID) - tolerance;
          auto const boxMax = Base::GetBoxMaxC(box, dimensionID) + tolerance;

          if (direction == 0)
          {
            if (tolerance != 0.0)
            {
              // Box should be within tolerance (<, not <=)
              if (origin <= boxMin || boxMax <= origin)
                return std::nullopt;
            }
            else
            {
              if (origin < boxMin || boxMax < origin)
                return std::nullopt;
            }
          }
          else
          {
            double const directionReciprocal = 1.0 / direction;
            double t1 = (boxMin - origin) * directionReciprocal;
            double t2 = (boxMax - origin) * directionReciprocal;
            if (t1 > t2)
              std::swap(t1, t2);

            minBoxDistance = std::max(minBoxDistance, t1);
            maxBoxDistance = std::min(maxBoxDistance, t2);
          }
        }

        assert(maxBoxDistance != inf && "rayDirection is a zero vector!");
        if (minBoxDistance > maxBoxDistance || maxBoxDistance < 0.0)
          return std::nullopt;
        else
          return minBoxDistance < 0 ? maxBoxDistance : minBoxDistance;
      }

      static constexpr std::optional<double> GetRayBoxDistance(Box const& box, Ray const& ray, FloatScalar tolerance) noexcept
      {
        return GetRayBoxDistance(box, Base::GetRayOrigin(ray), Base::GetRayDirection(ray), tolerance);
      }

      // Get point-Hyperplane relation (Plane equation: dotProduct(planeNormal, point) = distanceOfOrigo)
      static constexpr PlaneRelation GetPointPlaneRelation(Vector const& point, Scalar distanceOfOrigo, Vector const& planeNormal, FloatScalar tolerance) noexcept
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
      static constexpr PlaneRelation GetBoxPlaneRelation(Box const& box, Scalar distanceOfOrigo, Vector const& planeNormal, FloatScalar tolerance) noexcept
      {
        assert(IsNormalizedVector(planeNormal));

        Vector center, radius;
        for (dim_t dimensionID = 0; dimensionID < AmbientDim; ++dimensionID)
        {
          auto const minComponent = Base::GetBoxMinC(box, dimensionID);
          auto const maxComponent = Base::GetBoxMaxC(box, dimensionID);
          auto const centerComponent = static_cast<Scalar>((minComponent + maxComponent) * 0.5);
          Base::SetPointC(center, dimensionID, centerComponent);
          Base::SetPointC(radius, dimensionID, centerComponent - minComponent);
        }

        auto radiusProjected = double(tolerance);
        for (dim_t dimensionID = 0; dimensionID < AmbientDim; ++dimensionID)
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
  using namespace OrthoTree::EigenAdapter;

  // Basic OrthoTree types
  template<typename TScalar, int AmbientDim>
  using EigenOrthoTreePointSpan =
    OrthoTree::OrthoTreeBase<OrthoTree::PointEntitySpanAdapter<Matrix<TScalar, AmbientDim, 1>>, EigenGeometryAdapter<TScalar, AmbientDim>, OrthoTree::PointConfiguration>;

  template<typename TScalar, int AmbientDim, bool IS_LOOSE_TREE = true>
  using EigenOrthoTreeBoxSpan =
    OrthoTree::OrthoTreeBase<OrthoTree::BoxEntitySpanAdapter<AlignedBox<TScalar, AmbientDim>>, EigenGeometryAdapter<TScalar, AmbientDim>, OrthoTree::BoxConfiguration<IS_LOOSE_TREE>>;

  template<typename TScalar, int AmbientDim, typename TContainer = std::unordered_map<OrthoTree::index_t, AlignedBox<TScalar, AmbientDim>>>
  using EigenOrthoTreePointMap =
    OrthoTree::OrthoTreeBase<OrthoTree::PointEntityMapAdapter<Matrix<TScalar, AmbientDim, 1>, TContainer>, EigenGeometryAdapter<TScalar, AmbientDim>, OrthoTree::PointConfiguration>;

  template<typename TScalar, int AmbientDim, bool IS_LOOSE_TREE = true, typename TContainer = std::unordered_map<OrthoTree::index_t, AlignedBox<TScalar, AmbientDim>>>
  using EigenOrthoTreeBoxMap = OrthoTree::OrthoTreeBase<
    OrthoTree::BoxEntityMapAdapter<AlignedBox<TScalar, AmbientDim>, TContainer>,
    EigenGeometryAdapter<TScalar, AmbientDim>,
    OrthoTree::BoxConfiguration<IS_LOOSE_TREE>>;


  template<typename TScalar, int AmbientDim>
  using OrthoTreeContainerPointC = OrthoTree::OrthoTreeContainer<EigenOrthoTreePointSpan<TScalar, AmbientDim>>;

  template<typename TScalar, int AmbientDim, bool IS_LOOSE_TREE = true>
  using OrthoTreeContainerBoxC = OrthoTree::OrthoTreeContainer<EigenOrthoTreeBoxSpan<TScalar, AmbientDim, IS_LOOSE_TREE>>;

  template<typename TScalar, int AmbientDim, bool IS_LOOSE_TREE = true, typename TContainer = std::unordered_map<OrthoTree::index_t, AlignedBox<TScalar, AmbientDim>>>
  using OrthoTreeContainerPointMapC = OrthoTree::OrthoTreeContainer<EigenOrthoTreePointMap<TScalar, AmbientDim, TContainer>>;

  template<typename TScalar, int AmbientDim, bool IS_LOOSE_TREE = true, typename TContainer = std::unordered_map<OrthoTree::index_t, AlignedBox<TScalar, AmbientDim>>>
  using OrthoTreeContainerBoxMapC = OrthoTree::OrthoTreeContainer<EigenOrthoTreeBoxMap<TScalar, AmbientDim, IS_LOOSE_TREE, TContainer>>;

  // Non-owning types
  using QuadtreePoint2f = EigenOrthoTreePointSpan<float, 2>;

  using QuadtreePoint2d = EigenOrthoTreePointSpan<double, 2>;

  using OctreePoint3f = EigenOrthoTreePointSpan<float, 3>;

  using OctreePoint3d = EigenOrthoTreePointSpan<double, 3>;

  template<bool IS_LOOSE_TREE = true>
  using QuadtreeBox2fs = EigenOrthoTreeBoxSpan<float, 2, IS_LOOSE_TREE>;
  using QuadtreeBox2f = QuadtreeBox2fs<true>;

  template<bool IS_LOOSE_TREE = true>
  using QuadtreeBox2ds = EigenOrthoTreeBoxSpan<double, 2, IS_LOOSE_TREE>;
  using QuadtreeBox2d = QuadtreeBox2ds<true>;

  template<bool IS_LOOSE_TREE = true>
  using OctreeBox3fs = EigenOrthoTreeBoxSpan<float, 3, IS_LOOSE_TREE>;
  using OctreeBox3f = OctreeBox3fs<true>;

  template<bool IS_LOOSE_TREE = true>
  using OctreeBox3ds = EigenOrthoTreeBoxSpan<double, 3, IS_LOOSE_TREE>;
  using OctreeBox3d = OctreeBox3ds<true>;

  // Container types
  using QuadtreePointC2f = OrthoTreeContainerPointC<float, 2>;

  using QuadtreePointC2d = OrthoTreeContainerPointC<double, 2>;

  using OctreePointC3f = OrthoTreeContainerPointC<float, 3>;

  using OctreePointC3d = OrthoTreeContainerPointC<double, 3>;

  template<bool IS_LOOSE_TREE = true>
  using QuadtreeBoxC2fs = OrthoTreeContainerBoxC<float, 2, IS_LOOSE_TREE>;
  using QuadtreeBoxC2f = QuadtreeBoxC2fs<true>;

  template<bool IS_LOOSE_TREE = true>
  using QuadtreeBoxC2ds = OrthoTreeContainerBoxC<double, 2, IS_LOOSE_TREE>;
  using QuadtreeBoxC2d = QuadtreeBoxC2ds<true>;

  template<bool IS_LOOSE_TREE = true>
  using OctreeBoxC3fs = OrthoTreeContainerBoxC<float, 3, IS_LOOSE_TREE>;
  using OctreeBoxC3f = OctreeBoxC3fs<true>;

  template<bool IS_LOOSE_TREE = true>
  using OctreeBoxC3ds = OrthoTreeContainerBoxC<double, 3, IS_LOOSE_TREE>;
  using OctreeBoxC3d = OctreeBoxC3ds<true>;


  // Map types

  // Non-owning types
  using QuadtreePointMap2f = EigenOrthoTreePointMap<float, 2>;

  using QuadtreePointMap2d = EigenOrthoTreePointMap<double, 2>;

  using OctreePointMap3f = EigenOrthoTreePointMap<float, 3>;

  using OctreePointMap3d = EigenOrthoTreePointMap<double, 3>;

  template<bool IS_LOOSE_TREE = true>
  using QuadtreeBox2Mapfs = EigenOrthoTreeBoxMap<float, 2, IS_LOOSE_TREE>;
  using QuadtreeBox2Mapf = QuadtreeBox2Mapfs<true>;

  template<bool IS_LOOSE_TREE = true>
  using QuadtreeBox2Mapds = EigenOrthoTreeBoxMap<double, 2, IS_LOOSE_TREE>;
  using QuadtreeBox2Mapd = QuadtreeBox2Mapds<true>;

  template<bool IS_LOOSE_TREE = true>
  using OctreeBox3Mapfs = EigenOrthoTreeBoxMap<float, 3, IS_LOOSE_TREE>;
  using OctreeBox3Mapf = OctreeBox3Mapfs<true>;

  template<bool IS_LOOSE_TREE = true>
  using OctreeBox3Mapds = EigenOrthoTreeBoxMap<double, 3, IS_LOOSE_TREE>;
  using OctreeBox3Mapd = OctreeBox3Mapds<true>;

  // Container types
  using QuadtreePointCMap2f = OrthoTreeContainerPointMapC<float, 2>;

  using QuadtreePointCMap2d = OrthoTreeContainerPointMapC<double, 2>;

  using OctreePointCMap3f = OrthoTreeContainerPointMapC<float, 3>;

  using OctreePointCMap3d = OrthoTreeContainerPointMapC<double, 3>;

  template<bool IS_LOOSE_TREE = true>
  using QuadtreeBoxCMap2fs = OrthoTreeContainerBoxMapC<float, 2, IS_LOOSE_TREE>;
  using QuadtreeBoxCMap2f = QuadtreeBoxCMap2fs<true>;

  template<bool IS_LOOSE_TREE = true>
  using QuadtreeBoxCMap2ds = OrthoTreeContainerBoxMapC<double, 2, IS_LOOSE_TREE>;
  using QuadtreeBoxCMap2d = QuadtreeBoxCMap2ds<true>;

  template<bool IS_LOOSE_TREE = true>
  using OctreeBoxCMap3fs = OrthoTreeContainerBoxMapC<float, 3, IS_LOOSE_TREE>;
  using OctreeBoxCMap3f = OctreeBoxCMap3fs<true>;

  template<bool IS_LOOSE_TREE = true>
  using OctreeBoxC3Mapds = OrthoTreeContainerBoxMapC<double, 3, IS_LOOSE_TREE>;
  using OctreeBoxC3Mapd = OctreeBoxC3Mapds<true>;
} // namespace Eigen
