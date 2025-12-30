
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
  template<dim_t DIMENSION_NO_, typename TVector_, typename TBox_, typename TRay_, typename TPlane_, typename TScalar_ = double, typename TFloatScalar_ = double>
  struct AdaptorGeneralBasics
  {
    static constexpr dim_t DIMENSION_NO = DIMENSION_NO;

    using TScalar = TScalar_;
    using TFloatScalar = TFloatScalar_;
    using TVector = TVector_;
    using TBox = TBox_;
    using TRay = TRay_;
    using TPlane = TPlane_;

    static constexpr TScalar GetPointC(TVector const& point, dim_t dimensionID) noexcept { return point[dimensionID]; }
    static constexpr void SetPointC(TVector& point, dim_t dimensionID, TScalar value) noexcept { point[dimensionID] = value; }

    static constexpr TScalar GetBoxMinC(TBox const& box, dim_t dimensionID) noexcept { return box.Min[dimensionID]; }
    static constexpr TScalar GetBoxMaxC(TBox const& box, dim_t dimensionID) noexcept { return box.Max[dimensionID]; }
    static constexpr void SetBoxMinC(TBox& box, dim_t dimensionID, TScalar value) noexcept { box.Min[dimensionID] = value; }
    static constexpr void SetBoxMaxC(TBox& box, dim_t dimensionID, TScalar value) noexcept { box.Max[dimensionID] = value; }

    static constexpr TVector const& GetRayDirection(TRay const& ray) noexcept { return ray.Direction; }
    static constexpr TVector const& GetRayOrigin(TRay const& ray) noexcept { return ray.Origin; }

    static constexpr TVector const& GetPlaneNormal(TPlane const& plane) noexcept { return plane.Normal; }
    static constexpr TScalar GetPlaneOrigoDistance(TPlane const& plane) noexcept { return plane.OrigoDistance; }
  };

  // Provides general vector/box/ray/plane operations based on a basic adaptor interface. If the geometric types are connected to an BLAS, it is recommended to implement a custom AdaptorGeneral.
  template<typename TAdaptorBasics>
  struct AdaptorGeneralBase : TAdaptorBasics
  {
    using Base = TAdaptorBasics;

    using TScalar = Base::TScalar;
    using TFloatScalar = Base::TFloatScalar;
    using TVector = Base::TVector;
    using TBox = Base::TBox;
    using TRay = Base::TRay;
    using TPlane = Base::TPlane;

    static constexpr dim_t DIMENSION_NO = Base::DIMENSION_NO;

    static_assert(AdaptorBasicsConcept<Base, TVector, TBox, TRay, TPlane, TScalar>);

    static constexpr TVector Add(TVector const& ptL, TVector const& ptR) noexcept
    {
      auto point = TVector{};
      for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
        Base::SetPointC(point, dimensionID, Base::GetPointC(ptL, dimensionID) + Base::GetPointC(ptR, dimensionID));

      return point;
    }

    static void MoveBox(TBox& box, TVector const& moveVector) noexcept
    {
      LOOPIVDEP
      for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
      {
        Base::SetBoxMinC(box, dimensionID, Base::GetBoxMinC(box, dimensionID) + Base::GetPointC(moveVector, dimensionID));
        Base::SetBoxMaxC(box, dimensionID, Base::GetBoxMaxC(box, dimensionID) + Base::GetPointC(moveVector, dimensionID));
      }
    }

    static constexpr TScalar Size2(TVector const& point) noexcept
    {
      auto d2 = TScalar{ 0 };
      for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
      {
        auto const d = Base::GetPointC(point, dimensionID);
        d2 += d * d;
      }
      return d2;
    }

    static constexpr TScalar Size(TVector const& point) noexcept { return std::sqrt(Size2(point)); }

    static constexpr TScalar Dot(TVector const& ptL, TVector const& ptR) noexcept
    {
      auto value = TScalar{};
      for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
        value += Base::GetPointC(ptL, dimensionID) * Base::GetPointC(ptR, dimensionID);

      return value;
    }

    static constexpr TScalar Distance2(TVector const& ptL, TVector const& ptR) noexcept
    {
      auto d2 = TScalar{ 0 };
      for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
      {
        auto const d = Base::GetPointC(ptL, dimensionID) - Base::GetPointC(ptR, dimensionID);
        d2 += d * d;
      }
      return d2;
    }

    static constexpr TScalar Distance(TVector const& ptL, TVector const& ptR) noexcept { return std::sqrt(Distance2(ptL, ptR)); }

    struct PointBoxMinMaxDistance
    {
      // Minimum possible distance from the query point to any object contained in the box.
      // If the point lies inside the box, this value is zero.
      TScalar min = {};

      // Maximum possible nearest-distance in the worst case.
      // Assumes an object inside the box is placed adversarially so as to maximize
      // its minimum distance to the query point (i.e. the worst-case nearest object).
      TScalar minMax = {};
    };
    static constexpr PointBoxMinMaxDistance MinMaxDistance2(TVector const& pt, TBox const& box, TScalar tolerance) noexcept
    {
      // N. Roussopoulos, S. Kelley, F. Vincent - Nearest Neighbor Queries (1995) DOI.10.1145 / 223784.223794
      // MINMAXDIST

      auto dist2 = PointBoxMinMaxDistance{};

      TScalar farthestInsideDistance2 = std::numeric_limits<TScalar>::max();
      TScalar largestMinMax2Difference = {};
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

    static constexpr PointBoxMinMaxDistance MinMaxDistance(TVector const& pt, TBox const& box, TFloatScalar tolerance) noexcept
    {
      auto dist = MinMaxDistance2(pt, box, tolerance);
      dist.min = std::sqrt(dist.min);
      dist.minMax = std::sqrt(dist.minMax);
      return dist;
    }

    static constexpr bool ArePointsEqual(TVector const& ptL, TVector const& ptR, TFloatScalar tolerance) noexcept
    {
      return Distance2(ptL, ptR) <= tolerance * tolerance;
    }

    static constexpr bool IsNormalizedVector(TVector const& normal) noexcept { return std::abs(Size2(normal) - 1.0) < 0.000001; }

    static constexpr bool DoesBoxContainPoint(TBox const& box, TVector const& point, TScalar tolerance = 0) noexcept
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

    enum class EBoxRelation
    {
      Overlapped = -1,
      Adjecent = 0,
      Separated = 1
    };
    static constexpr EBoxRelation GetBoxRelation(TBox const& e1, TBox const& e2) noexcept
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
        if (Base::GetBoxMinC(e1, dimensionID) < Base::GetBoxMaxC(e2, dimensionID) && Base::GetBoxMaxC(e1, dimensionID) > Base::GetBoxMinC(e2, dimensionID))
          rel |= EBoxRelationCandidate::OverlappedC;
        else if (Base::GetBoxMinC(e1, dimensionID) == Base::GetBoxMaxC(e2, dimensionID) || Base::GetBoxMaxC(e1, dimensionID) == Base::GetBoxMinC(e2, dimensionID))
          rel |= EBoxRelationCandidate::AdjecentC;
        else if (Base::GetBoxMinC(e1, dimensionID) > Base::GetBoxMaxC(e2, dimensionID) || Base::GetBoxMaxC(e1, dimensionID) < Base::GetBoxMinC(e2, dimensionID))
          return EBoxRelation::Separated;
      }

      return (rel & EBoxRelationCandidate::AdjecentC) ? EBoxRelation::Adjecent : EBoxRelation::Overlapped;
    }

    static constexpr bool AreBoxesOverlappedStrict(TBox const& e1, TBox const& e2) noexcept
    {
      return GetBoxRelation(e1, e2) == EBoxRelation::Overlapped;
    }

    static constexpr bool AreBoxesOverlapped(TBox const& e1, TBox const& e2, bool e1_must_contain_e2 = true, bool fOverlapPtTouchAllowed = false) noexcept
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
        auto const rel = GetBoxRelation(e1, e2);
        if (fOverlapPtTouchAllowed)
          return rel == EBoxRelation::Adjecent || rel == EBoxRelation::Overlapped;
        else
          return rel == EBoxRelation::Overlapped;
      }
    }

    static constexpr std::optional<double> GetRayBoxDistance(TBox const& box, TVector const& rayOrigin, TVector const& rayDirection, TScalar tolerance) noexcept
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

    static constexpr std::optional<double> GetRayBoxDistance(TBox const& box, TRay const& ray, TScalar tolerance) noexcept
    {
      return GetRayBoxDistance(box, Base::GetRayOrigin(ray), Base::GetRayDirection(ray), tolerance);
    }

    // Get point-Hyperplane relation (Plane equation: dotProduct(planeNormal, point) = distanceOfOrigo)
    static constexpr PlaneRelation GetPointPlaneRelation(TVector const& point, TScalar distanceOfOrigo, TVector const& planeNormal, TScalar tolerance) noexcept
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
  using AdaptorGeneral =
    AdaptorGeneralBase<DIMENSION_NO, TVector, TBox, TRay, TPlane, TScalar, AdaptorGeneralBasics<DIMENSION_NO, TVector, TBox, TRay, TPlane, TScalar>>;

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
  using AdaptorGeneralND = AdaptorGeneral<
    DIMENSION_NO,
    OrthoTree::VectorND<DIMENSION_NO, TScalar>,
    OrthoTree::BoundingBoxND<DIMENSION_NO, TScalar>,
    OrthoTree::RayND<DIMENSION_NO, TScalar>,
    OrthoTree::PlaneND<DIMENSION_NO, TScalar>,
    TScalar>;

  template<dim_t DIMENSION_NO, typename TScalar = BaseGeometryType, typename TContainer = std::span<OrthoTree::VectorND<DIMENSION_NO, TScalar> const>>
  using TreePointND = OrthoTree::OrthoTreePoint<
    DIMENSION_NO,
    OrthoTree::VectorND<DIMENSION_NO, TScalar>,
    OrthoTree::BoundingBoxND<DIMENSION_NO, TScalar>,
    OrthoTree::RayND<DIMENSION_NO, TScalar>,
    OrthoTree::PlaneND<DIMENSION_NO, TScalar>,
    TScalar,
    AdaptorGeneralND<DIMENSION_NO, TScalar>,
    TContainer>;

  template<
    dim_t DIMENSION_NO,
    bool DO_SPLIT_PARENT_ENTITIES = true,
    typename TScalar = BaseGeometryType,
    typename TContainer = std::span<OrthoTree::BoundingBoxND<DIMENSION_NO, TScalar> const>>
  using TreeBoxND = OrthoTree::OrthoTreeBoundingBox<
    DIMENSION_NO,
    OrthoTree::VectorND<DIMENSION_NO, TScalar>,
    OrthoTree::BoundingBoxND<DIMENSION_NO, TScalar>,
    OrthoTree::RayND<DIMENSION_NO, TScalar>,
    OrthoTree::PlaneND<DIMENSION_NO, TScalar>,
    TScalar,
    DO_SPLIT_PARENT_ENTITIES,
    AdaptorGeneralND<DIMENSION_NO, TScalar>,
    TContainer>;

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
  template<bool DO_SPLIT_PARENT_ENTITIES>
  using DualtreeBoxs = TreeBoxND<1, DO_SPLIT_PARENT_ENTITIES, BaseGeometryType>;

  // Quadtree for bounding boxes with split-depth settings
  template<bool DO_SPLIT_PARENT_ENTITIES>
  using QuadtreeBoxs = TreeBoxND<2, DO_SPLIT_PARENT_ENTITIES, BaseGeometryType>;

  // Octree for bounding boxes with split-depth settings
  template<bool DO_SPLIT_PARENT_ENTITIES>
  using OctreeBoxs = TreeBoxND<3, DO_SPLIT_PARENT_ENTITIES, BaseGeometryType>;

  // Hexatree for bounding boxes with split-depth settings
  template<bool DO_SPLIT_PARENT_ENTITIES>
  using HexatreeBoxs = TreeBoxND<4, DO_SPLIT_PARENT_ENTITIES, BaseGeometryType>;

  // OrthoTrees for higher dimensions with split-depth settings
  template<bool DO_SPLIT_PARENT_ENTITIES>
  using TreeBox16Ds = TreeBoxND<16, DO_SPLIT_PARENT_ENTITIES, BaseGeometryType>;


  // OrthoTrees with std::unordered_map

  // std::unordered_map-based Quadtree for points
  using QuadtreePointMap = TreePointND<2, BaseGeometryType, std::unordered_map<index_t, OrthoTree::Vector2D>>;

  // std::unordered_map-based Octree for points
  using OctreePointMap = TreePointND<3, BaseGeometryType, std::unordered_map<index_t, OrthoTree::Vector3D>>;

  // std::unordered_map-based Octree for bounding boxes
  using QuadreeBoxMap = TreeBoxND<2, true, BaseGeometryType, std::unordered_map<index_t, OrthoTree::BoundingBox2D>>;

  // std::unordered_map-based Octree for bounding boxes
  using OctreeBoxMap = TreeBoxND<3, true, BaseGeometryType, std::unordered_map<index_t, OrthoTree::BoundingBox3D>>;

  // std::unordered_map-based Quadtree for bounding boxes with split-depth settings
  template<bool DO_SPLIT_PARENT_ENTITIES>
  using QuadtreeBoxsMap = TreeBoxND<2, DO_SPLIT_PARENT_ENTITIES, BaseGeometryType, std::unordered_map<index_t, OrthoTree::BoundingBox2D>>;
  using QuadtreeBoxMap = TreeBoxND<2, true, BaseGeometryType, std::unordered_map<index_t, OrthoTree::BoundingBox2D>>;

  // std::unordered_map-based Octree for bounding boxes with split-depth settings
  template<bool DO_SPLIT_PARENT_ENTITIES>
  using OctreeBoxsMap = TreeBoxND<3, DO_SPLIT_PARENT_ENTITIES, BaseGeometryType, std::unordered_map<index_t, OrthoTree::BoundingBox3D>>;
  using OctreeBoxMap = TreeBoxND<3, true, BaseGeometryType, std::unordered_map<index_t, OrthoTree::BoundingBox3D>>;


  // User-defined container-based Quadtree for points
  template<typename UDMap>
  using QuadtreePointUDMap = TreePointND<2, BaseGeometryType, UDMap>;

  // User-defined container-based Octree for points
  template<typename UDMap>
  using OctreePointUDMap = TreePointND<3, BaseGeometryType, UDMap>;

  // User-defined container-based Octree for bounding boxes
  template<typename UDMap>
  using QuadreeBoxUDMap = TreeBoxND<2, true, BaseGeometryType, UDMap>;

  // User-defined container-based Octree for bounding boxes
  template<typename UDMap>
  using OctreeBoxUDMap = TreeBoxND<3, true, BaseGeometryType, UDMap>;

  // User-defined container-based Quadtree for bounding boxes with split-depth settings
  template<bool DO_SPLIT_PARENT_ENTITIES, typename UDMap>
  using QuadtreeBoxsUDMap = TreeBoxND<2, DO_SPLIT_PARENT_ENTITIES, BaseGeometryType, UDMap>;

  // User-defined container-based Octree for bounding boxes with split-depth settings
  template<bool DO_SPLIT_PARENT_ENTITIES, typename UDMap>
  using OctreeBoxsUDMap = TreeBoxND<3, DO_SPLIT_PARENT_ENTITIES, BaseGeometryType, UDMap>;

} // namespace OrthoTree
