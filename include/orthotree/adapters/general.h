
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

#pragma once

#include <array>
#include <cassert>
#include <cmath>
#include <limits>
#include <optional>

#include "../detail/common.h"
#include "../octree.h"
#include "concepts.h"

namespace OrthoTree
{
  // Adapters

  // Provides basic accessor and mutator methods for generic geometric types (points, boxes, rays, planes).
  template<dim_t DIMENSION_NO_, typename TVector, typename TBox, typename TRay, typename TPlane, typename TScalar = double, typename TFloatScalar = double>
  struct GeneralBaseGeometryAdapter
  {
    using Scalar = TScalar;
    using FloatScalar = TFloatScalar;
    using Vector = TVector;
    using Box = TBox;
    using Ray = TRay;
    using Plane = TPlane;

    static constexpr dim_t DIMENSION_NO = DIMENSION_NO_;
    static constexpr FloatScalar BASE_TOLERANCE = std::numeric_limits<FloatScalar>::epsilon() * FloatScalar(100);

    static constexpr Vector MakePoint() noexcept { return {}; };
    static constexpr Scalar GetPointC(Vector const& point, dim_t dimensionID) noexcept { return point[dimensionID]; }
    static constexpr void SetPointC(Vector& point, dim_t dimensionID, TScalar value) noexcept { point[dimensionID] = value; }

    static constexpr Box MakeBox() noexcept { return {}; };
    static constexpr Scalar GetBoxMinC(Box const& box, dim_t dimensionID) noexcept { return box.Min[dimensionID]; }
    static constexpr Scalar GetBoxMaxC(Box const& box, dim_t dimensionID) noexcept { return box.Max[dimensionID]; }
    static constexpr void SetBoxMinC(Box& box, dim_t dimensionID, TScalar value) noexcept { box.Min[dimensionID] = value; }
    static constexpr void SetBoxMaxC(Box& box, dim_t dimensionID, TScalar value) noexcept { box.Max[dimensionID] = value; }

    static constexpr Vector const& GetRayDirection(Ray const& ray) noexcept { return ray.Direction; }
    static constexpr Vector const& GetRayOrigin(Ray const& ray) noexcept { return ray.Origin; }

    static constexpr Vector const& GetPlaneNormal(Plane const& plane) noexcept { return plane.Normal; }
    static constexpr Scalar GetPlaneOrigoDistance(Plane const& plane) noexcept { return plane.OrigoDistance; }
  };

  // Provides general vector/box/ray/plane operations based on a basic adapter interface. If the geometric types are connected to an BLAS, it is recommended to implement a custom AdaptorGeneral.
  template<typename TBaseGeometryAdapter>
  struct GeneralGeometryAdapter : TBaseGeometryAdapter
  {
    using Base = TBaseGeometryAdapter;

    using Scalar = Base::Scalar;
    using FloatScalar = Base::FloatScalar;
    using Vector = Base::Vector;
    using Box = Base::Box;
    using Ray = Base::Ray;
    using Plane = Base::Plane;

    static constexpr dim_t DIMENSION_NO = Base::DIMENSION_NO;

    static_assert(BaseGeometryAdapterConcept<Base, DIMENSION_NO, Vector, Box, Ray, Plane, Scalar, FloatScalar>);

    static constexpr Vector Add(Vector const& ptL, Vector const& ptR) noexcept
    {
      auto point = Vector{};
      for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
        Base::SetPointC(point, dimensionID, Base::GetPointC(ptL, dimensionID) + Base::GetPointC(ptR, dimensionID));

      return point;
    }

    static void MoveBox(Box& box, Vector const& moveVector) noexcept
    {
      ORTHOTREE_LOOPIVDEP
      for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
      {
        Base::SetBoxMinC(box, dimensionID, Base::GetBoxMinC(box, dimensionID) + Base::GetPointC(moveVector, dimensionID));
        Base::SetBoxMaxC(box, dimensionID, Base::GetBoxMaxC(box, dimensionID) + Base::GetPointC(moveVector, dimensionID));
      }
    }

    static constexpr Scalar Size2(Vector const& point) noexcept
    {
      auto d2 = Scalar{ 0 };
      for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
      {
        auto const d = Base::GetPointC(point, dimensionID);
        d2 += d * d;
      }
      return d2;
    }

    static constexpr Scalar Size(Vector const& point) noexcept { return std::sqrt(Size2(point)); }

    static constexpr Scalar Dot(Vector const& ptL, Vector const& ptR) noexcept
    {
      auto value = Scalar{};
      for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
        value += Base::GetPointC(ptL, dimensionID) * Base::GetPointC(ptR, dimensionID);

      return value;
    }

    static constexpr Scalar Distance2(Vector const& ptL, Vector const& ptR) noexcept
    {
      auto d2 = Scalar{ 0 };
      for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
      {
        auto const d = Base::GetPointC(ptL, dimensionID) - Base::GetPointC(ptR, dimensionID);
        d2 += d * d;
      }
      return d2;
    }

    static constexpr Scalar Distance(Vector const& ptL, Vector const& ptR) noexcept { return std::sqrt(Distance2(ptL, ptR)); }

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

      Scalar farthestInsideDistance2 = std::numeric_limits<Scalar>::max();
      Scalar largestMinMax2Difference = {};
      auto isInside = true;
      for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
      {
        const auto v = Base::GetPointC(pt, dimensionID);
        const auto minC = Base::GetBoxMinC(box, dimensionID);
        const auto maxC = Base::GetBoxMaxC(box, dimensionID);

        const auto minDist = v - minC;
        const auto maxDist = maxC - v;

        bool isInsideInComponent = (-tolerance <= minDist && -tolerance <= maxDist);
        isInside &= isInsideInComponent;

        auto minDist2 = minDist * minDist;
        auto maxDist2 = maxDist * maxDist;

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

    static constexpr bool ArePointsEqual(Vector const& ptL, Vector const& ptR, FloatScalar tolerance) noexcept
    {
      return Distance2(ptL, ptR) <= tolerance * tolerance;
    }

    static constexpr bool IsNormalizedVector(Vector const& normal) noexcept { return std::abs(Size2(normal) - 1.0) < 0.000001; }

    static constexpr bool DoesBoxContainPoint(Box const& box, Vector const& point, FloatScalar tolerance = 0) noexcept
    {
      if (tolerance != 0.0)
      {
        assert(tolerance > 0);
        for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
          if (!(Base::GetBoxMinC(box, dimensionID) - tolerance < Base::GetPointC(point, dimensionID) &&
                Base::GetPointC(point, dimensionID) < Base::GetBoxMaxC(box, dimensionID) + tolerance))
            return false;
      }
      else
      {
        for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
          if (!(Base::GetBoxMinC(box, dimensionID) <= Base::GetPointC(point, dimensionID) &&
                Base::GetPointC(point, dimensionID) <= Base::GetBoxMaxC(box, dimensionID)))
            return false;
      }
      return true;
    }

    static constexpr bool IsAlmostEqual(FloatScalar lhs, FloatScalar rhs, FloatScalar tolerance) noexcept { return std::abs(lhs - rhs) <= tolerance; }
    static constexpr bool IsLess(FloatScalar lhs, FloatScalar rhs, FloatScalar tolerance) noexcept { return lhs + tolerance < rhs; }
    static constexpr bool IsAlmostEqualS(Scalar lhs, Scalar rhs, FloatScalar tolerance) noexcept
    {
      return IsAlmostEqual(FloatScalar(lhs), FloatScalar(rhs), tolerance);
    }
    static constexpr bool IsLessS(Scalar lhs, Scalar rhs, FloatScalar tolerance) noexcept
    {
      return IsLess(FloatScalar(lhs), FloatScalar(rhs), tolerance);
    }

    enum class EBoxRelation
    {
      Overlapped = -1,
      Adjecent = 0,
      Separated = 1
    };
    static constexpr EBoxRelation GetBoxRelation(Box const& e1, Box const& e2, FloatScalar tolerance = {}) noexcept
    {
      enum EBoxRelationCandidate : uint8_t
      {
        OverlappedC = 0x1,
        AdjecentC = 0x2,
        SeparatedC = 0x4
      };
      uint8_t rel = 0;

      for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
      {
        if (
          IsLessS(Base::GetBoxMinC(e1, dimensionID), Base::GetBoxMaxC(e2, dimensionID), tolerance) &&
          IsLessS(Base::GetBoxMinC(e2, dimensionID), Base::GetBoxMaxC(e1, dimensionID), tolerance))
          rel |= EBoxRelationCandidate::OverlappedC;
        else if (
          IsAlmostEqualS(Base::GetBoxMinC(e1, dimensionID), Base::GetBoxMaxC(e2, dimensionID), tolerance) ||
          IsAlmostEqualS(Base::GetBoxMaxC(e1, dimensionID), Base::GetBoxMinC(e2, dimensionID), tolerance))
          rel |= EBoxRelationCandidate::AdjecentC;
        else if (
          IsLessS(Base::GetBoxMaxC(e2, dimensionID), Base::GetBoxMinC(e1, dimensionID), tolerance) ||
          IsLessS(Base::GetBoxMaxC(e1, dimensionID), Base::GetBoxMinC(e2, dimensionID), tolerance))
          return EBoxRelation::Separated;
      }

      return (rel & EBoxRelationCandidate::AdjecentC) ? EBoxRelation::Adjecent : EBoxRelation::Overlapped;
    }

    static constexpr bool AreBoxesOverlappedStrict(Box const& e1, Box const& e2) noexcept
    {
      return GetBoxRelation(e1, e2, 0) == EBoxRelation::Overlapped;
    }

    static constexpr bool AreBoxesOverlapped(
      Box const& e1, Box const& e2, bool e1_must_contain_e2 = true, bool fOverlapPtTouchAllowed = false, FloatScalar tolerance = {}) noexcept
    {
      if (e1_must_contain_e2)
      {
        for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
        {
          if (Base::GetBoxMinC(e1, dimensionID) > Base::GetBoxMinC(e2, dimensionID) || Base::GetBoxMinC(e2, dimensionID) > Base::GetBoxMaxC(e1, dimensionID))
            return false;

          if (Base::GetBoxMinC(e1, dimensionID) > Base::GetBoxMaxC(e2, dimensionID) || Base::GetBoxMaxC(e2, dimensionID) > Base::GetBoxMaxC(e1, dimensionID))
            return false;
        }
        return true;
      }
      else
      {
        auto const rel = GetBoxRelation(e1, e2, tolerance);
        if (fOverlapPtTouchAllowed)
          return rel == EBoxRelation::Adjecent || rel == EBoxRelation::Overlapped;
        else
          return rel == EBoxRelation::Overlapped;
      }
    }

    static constexpr std::optional<double> GetRayBoxDistance(Box const& box, Vector const& rayOrigin, Vector const& rayDirection, FloatScalar tolerance) noexcept
    {
      assert(tolerance >= 0 && "Tolerance cannot be negative!");

      if (DoesBoxContainPoint(box, rayOrigin, tolerance))
        return 0.0;

      auto constexpr inf = std::numeric_limits<double>::max();

      double minBoxDistance = -inf;
      double maxBoxDistance = +inf;
      for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
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
  };


  template<dim_t DIMENSION_NO, typename TVector, typename TBox, typename TRay, typename TPlane, typename TScalar = double>
  using AdaptorGeneral = GeneralGeometryAdapter<
    GeneralBaseGeometryAdapter<DIMENSION_NO, TVector, TBox, TRay, TPlane, TScalar, std::conditional_t<std::is_integral_v<TScalar>, float, TScalar>>>;

  template<dim_t DIMENSION_NO, typename TScalar = double>
  using VectorND = std::array<TScalar, DIMENSION_NO>;

  template<dim_t DIMENSION_NO, typename TScalar = double>
  using PointND = VectorND<DIMENSION_NO, TScalar>;

  template<dim_t DIMENSION_NO, typename TScalar = double>
  struct BoundingBoxND
  {
    VectorND<DIMENSION_NO, TScalar> Min;
    VectorND<DIMENSION_NO, TScalar> Max;
  };

  template<dim_t DIMENSION_NO, typename TScalar = double>
  struct RayND
  {
    VectorND<DIMENSION_NO, TScalar> Origin;
    VectorND<DIMENSION_NO, TScalar> Direction;
  };

  template<dim_t DIMENSION_NO, typename TScalar = double>
  struct PlaneND
  {
    TScalar OrigoDistance;
    VectorND<DIMENSION_NO, TScalar> Normal;
  };


  // Aliases
  using BaseGeometryType = double;
  using Vector1D = OrthoTree::VectorND<1, BaseGeometryType>;
  using Vector2D = OrthoTree::VectorND<2, BaseGeometryType>;
  using Vector3D = OrthoTree::VectorND<3, BaseGeometryType>;
  using Point1D = OrthoTree::PointND<1, BaseGeometryType>;
  using Point2D = OrthoTree::PointND<2, BaseGeometryType>;
  using Point3D = OrthoTree::PointND<3, BaseGeometryType>;
  using BoundingBox1D = OrthoTree::BoundingBoxND<1, BaseGeometryType>;
  using BoundingBox2D = OrthoTree::BoundingBoxND<2, BaseGeometryType>;
  using BoundingBox3D = OrthoTree::BoundingBoxND<3, BaseGeometryType>;
  using Ray2D = OrthoTree::RayND<2, BaseGeometryType>;
  using Ray3D = OrthoTree::RayND<3, BaseGeometryType>;
  using Plane2D = OrthoTree::PlaneND<2, BaseGeometryType>;
  using Plane3D = OrthoTree::PlaneND<3, BaseGeometryType>;

  template<dim_t DIMENSION_NO, typename TScalar = BaseGeometryType>
  using GeneralGeometryAdapterND = AdaptorGeneral<
    DIMENSION_NO,
    OrthoTree::VectorND<DIMENSION_NO, TScalar>,
    OrthoTree::BoundingBoxND<DIMENSION_NO, TScalar>,
    OrthoTree::RayND<DIMENSION_NO, TScalar>,
    OrthoTree::PlaneND<DIMENSION_NO, TScalar>,
    TScalar>;

  template<dim_t DIMENSION_NO, typename TScalar = BaseGeometryType, bool IS_CONTIOGUOS_CONTAINER = true>
  using TreePointND = OrthoTree::OrthoTreeBase<
    std::conditional_t<IS_CONTIOGUOS_CONTAINER, PointEntitySpanAdapter<PointND<DIMENSION_NO, TScalar>>, PointEntityMapAdapter<PointND<DIMENSION_NO, TScalar>>>,
    GeneralGeometryAdapterND<DIMENSION_NO, TScalar>,
    PointConfiguration>;

  template<dim_t DIMENSION_NO, bool IS_LOOSE_TREE = true, typename TScalar = BaseGeometryType, bool IS_CONTIOGUOS_CONTAINER = true>
  using TreeBoxND = OrthoTree::OrthoTreeBase<
    std::conditional_t<IS_CONTIOGUOS_CONTAINER, BoxEntitySpanAdapter<BoundingBoxND<DIMENSION_NO, TScalar>>, BoxEntityMapAdapter<BoundingBoxND<DIMENSION_NO, TScalar>>>,
    GeneralGeometryAdapterND<DIMENSION_NO, TScalar>,
    BoxConfiguration<IS_LOOSE_TREE>>;

  template<dim_t DIMENSION_NO, typename TScalar, typename TEntityContainer>
  using TreePointNDUD =
    OrthoTree::OrthoTreeBase<PointEntityMapAdapter<PointND<DIMENSION_NO, TScalar>, TEntityContainer>, GeneralGeometryAdapterND<DIMENSION_NO, TScalar>, PointConfiguration>;

  template<dim_t DIMENSION_NO, bool IS_LOOSE_TREE, typename TScalar, typename TEntityContainer>
  using TreeBoxNDUD = OrthoTree::OrthoTreeBase<
    BoxEntityMapAdapter<BoundingBoxND<DIMENSION_NO, TScalar>, TEntityContainer>,
    GeneralGeometryAdapterND<DIMENSION_NO, TScalar>,
    BoxConfiguration<IS_LOOSE_TREE>>;

  // Dualtree for points
  using DualtreePoint = TreePointND<1, BaseGeometryType>;

  // Dualtree for bounding boxes
  using DualtreeBox = TreeBoxND<1, true, BaseGeometryType>;

  // Quadtree for points
  using QuadtreePoint = TreePointND<2, BaseGeometryType>;

  // Quadtree for bounding boxes
  using QuadtreeBox = TreeBoxND<2, true, BaseGeometryType>;

  // Octree for points
  using OctreePoint = TreePointND<3, BaseGeometryType>;

  // Octree for bounding boxes
  using OctreeBox = TreeBoxND<3, true, BaseGeometryType>;

  // Hexatree for points
  using HexatreePoint = TreePointND<4, BaseGeometryType>;

  // Hexatree for bounding boxes
  using HexatreeBox = TreeBoxND<4, true, BaseGeometryType>;

  // OrthoTrees for higher dimensions
  using TreePoint16D = TreePointND<16, BaseGeometryType>;
  using TreeBox16D = TreeBoxND<16, true, BaseGeometryType>;


  // Dualtree for bounding boxes with split-depth settings
  template<bool IS_LOOSE_TREE>
  using DualtreeBoxs = TreeBoxND<1, IS_LOOSE_TREE, BaseGeometryType>;

  // Quadtree for bounding boxes with split-depth settings
  template<bool IS_LOOSE_TREE>
  using QuadtreeBoxs = TreeBoxND<2, IS_LOOSE_TREE, BaseGeometryType>;

  // Octree for bounding boxes with split-depth settings
  template<bool IS_LOOSE_TREE>
  using OctreeBoxs = TreeBoxND<3, IS_LOOSE_TREE, BaseGeometryType>;

  // Hexatree for bounding boxes with split-depth settings
  template<bool IS_LOOSE_TREE>
  using HexatreeBoxs = TreeBoxND<4, IS_LOOSE_TREE, BaseGeometryType>;

  // OrthoTrees for higher dimensions with split-depth settings
  template<bool IS_LOOSE_TREE>
  using TreeBox16Ds = TreeBoxND<16, IS_LOOSE_TREE, BaseGeometryType>;


  // OrthoTrees with std::unordered_map

  // std::unordered_map-based Quadtree for points
  using QuadtreePointMap = TreePointND<2, BaseGeometryType, false>;

  // std::unordered_map-based Octree for points
  using OctreePointMap = TreePointND<3, BaseGeometryType, false>;

  // std::unordered_map-based Octree for bounding boxes
  using QuadreeBoxMap = TreeBoxND<2, true, BaseGeometryType, false>;

  // std::unordered_map-based Octree for bounding boxes
  using OctreeBoxMap = TreeBoxND<3, true, BaseGeometryType, false>;

  // std::unordered_map-based Quadtree for bounding boxes with split-depth settings
  template<bool IS_LOOSE_TREE>
  using QuadtreeBoxsMap = TreeBoxND<2, IS_LOOSE_TREE, BaseGeometryType, false>;
  using QuadtreeBoxMap = TreeBoxND<2, true, BaseGeometryType, false>;

  // std::unordered_map-based Octree for bounding boxes with split-depth settings
  template<bool IS_LOOSE_TREE>
  using OctreeBoxsMap = TreeBoxND<3, IS_LOOSE_TREE, BaseGeometryType, false>;
  using OctreeBoxMap = TreeBoxND<3, true, BaseGeometryType, false>;


  template<dim_t DIMENSION_NO, typename TScalar = BaseGeometryType, typename TEntityAdapter = PointEntitySpanAdapter<PointND<DIMENSION_NO, TScalar>>>
  using TreePointEAND = OrthoTree::OrthoTreeBase<TEntityAdapter, GeneralGeometryAdapterND<DIMENSION_NO, TScalar>, PointConfiguration>;

  template<dim_t DIMENSION_NO, bool IS_LOOSE_TREE = true, typename TScalar = BaseGeometryType, typename TEntityAdapter = BoxEntitySpanAdapter<BoundingBoxND<DIMENSION_NO, TScalar>>>
  using TreeBoxEAND = OrthoTree::OrthoTreeBase<TEntityAdapter, GeneralGeometryAdapterND<DIMENSION_NO, TScalar>, BoxConfiguration<IS_LOOSE_TREE>>;


  // User-defined container-based Quadtree for points
  template<typename EntityAdapter>
  using QuadtreePointUDMap = TreePointEAND<2, BaseGeometryType, EntityAdapter>;

  // User-defined container-based Octree for points
  template<typename EntityAdapter>
  using OctreePointUDMap = TreePointEAND<3, BaseGeometryType, EntityAdapter>;

  // User-defined container-based Octree for bounding boxes
  template<typename EntityAdapter>
  using QuadreeBoxUDMap = TreeBoxEAND<2, true, BaseGeometryType, EntityAdapter>;

  // User-defined container-based Octree for bounding boxes
  template<typename EntityAdapter>
  using OctreeBoxUDMap = TreeBoxEAND<3, true, BaseGeometryType, EntityAdapter>;

  // User-defined container-based Quadtree for bounding boxes with split-depth settings
  template<bool IS_LOOSE_TREE, typename EntityAdapter>
  using QuadtreeBoxsUDMap = TreeBoxEAND<2, IS_LOOSE_TREE, BaseGeometryType, EntityAdapter>;

  // User-defined container-based Octree for bounding boxes with split-depth settings
  template<bool IS_LOOSE_TREE, typename EntityAdapter>
  using OctreeBoxsUDMap = TreeBoxEAND<3, IS_LOOSE_TREE, BaseGeometryType, EntityAdapter>;

} // namespace OrthoTree
