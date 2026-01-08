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
#include <type_traits>

#include "../adapters/concepts.h"
#include "common.h"

namespace OrthoTree::detail
{

  // Internal geometry system which
  //  - can be instantiated
  //  - is float-based (and not suffer from integer arithmetic issues)
  template<typename TGeometryAdapter>
  struct InternalGeometryModule
  {
    using GA = TGeometryAdapter;

    using TScalar = typename GA::Scalar;
    using TFloatScalar = typename GA::FloatScalar;
    using TVector = typename GA::Vector;
    using TBox = typename GA::Box;
    using TRay = typename GA::Ray;
    using TPlane = typename GA::Plane;

    static constexpr dim_t DIMENSION_NO = GA::DIMENSION_NO;

    using Geometry = GA::FloatScalar;
    using Vector = std::array<Geometry, DIMENSION_NO>;

    struct Box
    {
      Vector Min, Max;
    };

    static constexpr Geometry Size2(Vector const& vector) noexcept
    {
      auto d2 = Geometry{ 0 };
      ORTHOTREE_LOOPIVDEP
      for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
        d2 += vector[dimensionID] * vector[dimensionID];

      return d2;
    }

    static Geometry Size(Vector const& vector) noexcept { return std::sqrt(Size2(vector)); }

    static constexpr Vector GetBoxCenter(Box const& box) noexcept
    {
      Vector center;
      static_for<DIMENSION_NO>([&](auto dimensionID) { center[dimensionID] = (box.Min[dimensionID] + box.Max[dimensionID]) * Geometry(0.5); });
      return center;
    }

    static constexpr Vector GetBoxCenterAD(TBox const& box) noexcept
    {
      Vector center;
      static_for<DIMENSION_NO>(
        [&](auto dimensionID) { center[dimensionID] = (GA::GetBoxMinC(box, dimensionID) + GA::GetBoxMaxC(box, dimensionID)) * Geometry(0.5); });
      return center;
    }

    static constexpr Vector GetBoxSizeAD(TBox const& box) noexcept
    {
      Vector sizes;
      static_for<DIMENSION_NO>([&](auto dimensionID) { sizes[dimensionID] = Geometry((GA::GetBoxMaxC(box, dimensionID) - GA::GetBoxMinC(box, dimensionID))); });
      return sizes;
    }

    static constexpr Vector GetBoxHalfSizeAD(TBox const& box) noexcept
    {
      Vector halfSize;
      ORTHOTREE_LOOPIVDEP
      for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
        halfSize[dimensionID] = (GA::GetBoxMaxC(box, dimensionID) - GA::GetBoxMinC(box, dimensionID)) * Geometry(0.5);

      return halfSize;
    }

    static bool AreBoxesOverlappingByCenter(Vector const& centerLhs, Vector const& halfSizeLhs, Vector const& centerRhs, Vector const& halfSizeRhs) noexcept
    {
      Vector distance;
      ORTHOTREE_LOOPIVDEP
      for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
        distance[dimensionID] = centerLhs[dimensionID] - centerRhs[dimensionID];

      Vector sizeLimit;
      ORTHOTREE_LOOPIVDEP
      for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
        sizeLimit[dimensionID] = halfSizeLhs[dimensionID] + halfSizeRhs[dimensionID];

      for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
        if (sizeLimit[dimensionID] <= std::abs(distance[dimensionID]))
          return false;

      return true;
    }

    static constexpr void MoveAD(Vector& v, TVector const& moveVector) noexcept
    {
      ORTHOTREE_LOOPIVDEP
      for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
        v[dimensionID] += GA::GetPointC(moveVector, dimensionID);
    }

    static constexpr void MoveAD(Box& box, TVector const& moveVector) noexcept
    {
      ORTHOTREE_LOOPIVDEP
      for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
      {
        box.Min[dimensionID] += GA::GetPointC(moveVector, dimensionID);
        box.Max[dimensionID] += GA::GetPointC(moveVector, dimensionID);
      }
    }

    static constexpr TScalar DotAD(TVector const& ptL, Vector const& ptR) noexcept
    {
      auto value = TScalar{};
      ORTHOTREE_LOOPIVDEP
      for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
        value += GA::GetPointC(ptL, dimensionID) * ptR[dimensionID];

      return value;
    }

    template<typename TGeometryRange, typename TGeometryBox>
    static constexpr bool DoesRangeContainBox(TGeometryRange rangeMin, TGeometryRange rangeMax, TGeometryBox boxMin, TGeometryBox boxMax) noexcept
    {
      if (rangeMin > boxMin || boxMin > rangeMax)
        return false;

      if (rangeMin > boxMax || boxMax > rangeMax)
        return false;

      return true;
    }

    static constexpr bool DoesRangeContainBoxAD(TBox const& range, Box const& box) noexcept
    {
      for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
      {
        if (!DoesRangeContainBox(GA::GetBoxMinC(range, dimensionID), GA::GetBoxMaxC(range, dimensionID), box.Min[dimensionID], box.Max[dimensionID]))
        {
          return false;
        }
      }
      return true;
    }

    static constexpr bool DoesRangeContainBoxAD(Box const& range, TBox const& box) noexcept
    {
      for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
      {
        if (!DoesRangeContainBox(range.Min[dimensionID], range.Max[dimensionID], GA::GetBoxMinC(box, dimensionID), GA::GetBoxMaxC(box, dimensionID)))
        {
          return false;
        }
      }
      return true;
    }

    static constexpr bool DoesRangeContainBoxAD(Box const& range, Box const& box) noexcept
    {
      for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
      {
        if (!DoesRangeContainBox(range.Min[dimensionID], range.Max[dimensionID], box.Min[dimensionID], box.Max[dimensionID]))
        {
          return false;
        }
      }
      return true;
    }

    static constexpr bool DoesRangeContainBoxAD(TBox const& range, Vector const& center, Vector const& halfSize) noexcept
    {
      for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
      {
        const auto boxMin = center[dimensionID] - halfSize[dimensionID];
        const auto boxMax = center[dimensionID] + halfSize[dimensionID];

        if (!DoesRangeContainBox(GA::GetBoxMinC(range, dimensionID), GA::GetBoxMaxC(range, dimensionID), boxMin, boxMax))
        {
          return false;
        }
      }
      return true;
    }

    static constexpr PlaneRelation GetBoxPlaneRelationAD(
      Vector const& center, Vector const& halfSize, TScalar distanceOfOrigo, TVector const& planeNormal, TFloatScalar tolerance) noexcept
    {
      assert(GA::IsNormalizedVector(planeNormal));

      auto radiusProjected = double(tolerance);
      for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
        radiusProjected += halfSize[dimensionID] * std::abs(GA::GetPointC(planeNormal, dimensionID));

      auto const centerProjected = DotAD(planeNormal, center) - distanceOfOrigo;

      if (centerProjected + radiusProjected < 0.0)
        return PlaneRelation::Negative;

      if (centerProjected - radiusProjected > 0.0)
        return PlaneRelation::Positive;

      return PlaneRelation::Hit;
    }

    static consteval Box BoxInvertedInit() noexcept
    {
      Box ext;
      ext.Min.fill(+std::numeric_limits<Geometry>::max());
      ext.Max.fill(-std::numeric_limits<Geometry>::max());
      return ext;
    }

    static constexpr Box GetBoxAD(TBox const& box) noexcept
    {
      Box boxIGM;
      ORTHOTREE_LOOPIVDEP
      for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
      {
        boxIGM.Min[dimensionID] = Geometry(GA::GetBoxMinC(box, dimensionID));
        boxIGM.Max[dimensionID] = Geometry(GA::GetBoxMaxC(box, dimensionID));
      }

      return boxIGM;
    }

    template<typename TContainer>
    static constexpr Box GetBoxOfPointsAD(TContainer const& points) noexcept
    {
      auto ext = BoxInvertedInit();
      for (auto const& e : points)
      {
        auto const& point = detail::getValuePart(e);
        for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
        {
          if (ext.Min[dimensionID] > GA::GetPointC(point, dimensionID))
            ext.Min[dimensionID] = Geometry(GA::GetPointC(point, dimensionID));

          if (ext.Max[dimensionID] < GA::GetPointC(point, dimensionID))
            ext.Max[dimensionID] = Geometry(GA::GetPointC(point, dimensionID));
        }
      }
      return ext;
    }

    template<typename TContainer>
    static constexpr Box GetBoxOfBoxesAD(TContainer const& boxes) noexcept
    {
      auto ext = BoxInvertedInit();
      for (auto const& e : boxes)
      {
        auto const& box = detail::getValuePart(e);
        for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
        {
          if (ext.Min[dimensionID] > GA::GetBoxMinC(box, dimensionID))
            ext.Min[dimensionID] = Geometry(GA::GetBoxMinC(box, dimensionID));

          if (ext.Max[dimensionID] < GA::GetBoxMaxC(box, dimensionID))
            ext.Max[dimensionID] = Geometry(GA::GetBoxMaxC(box, dimensionID));
        }
      }
      return ext;
    }

    static constexpr bool DoesBoxContainPointAD(Box const& box, TVector const& point, TScalar tolerance = 0) noexcept
    {
      if (tolerance != 0.0)
      {
        assert(tolerance > 0);
        for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
          if (!(box.Min[dimensionID] - tolerance < GA::GetPointC(point, dimensionID) &&
                GA::GetPointC(point, dimensionID) < box.Max[dimensionID] + tolerance))
            return false;
      }
      else
      {
        for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
          if (!(box.Min[dimensionID] <= GA::GetPointC(point, dimensionID) && GA::GetPointC(point, dimensionID) <= box.Max[dimensionID]))
            return false;
      }
      return true;
    }

    static constexpr bool DoesBoxContainPointAD(Vector const& center, Vector const& halfSizes, TVector const& point, TScalar tolerance = 0) noexcept
    {
      if (tolerance != 0.0)
      {
        assert(tolerance > 0);
        for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
        {
          auto const pointDistance = std::abs(Geometry(GA::GetPointC(point, dimensionID)) - center[dimensionID]);
          auto const halfSize = halfSizes[dimensionID] + tolerance;
          if (pointDistance >= halfSize)
            return false;
        }
      }
      else
      {
        for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
        {
          auto const pointDistance = std::abs(Geometry(GA::GetPointC(point, dimensionID)) - center[dimensionID]);
          if (pointDistance > halfSizes[dimensionID])
            return false;
        }
      }
      return true;
    }

    static Geometry GetBoxWallDistanceAD(TVector const& searchPoint, Vector const& centerPoint, Vector const& halfSize, bool isInsideConsideredAsZero) noexcept
    {
      Vector centerDistance;
      bool isInside = true;
      for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
      {
        centerDistance[dimensionID] = std::abs(centerPoint[dimensionID] - Geometry(GA::GetPointC(searchPoint, dimensionID)));
        isInside &= centerDistance[dimensionID] <= halfSize[dimensionID];
      }

      if (isInside)
      {
        if (isInsideConsideredAsZero)
          return Geometry{};

        auto minWallDistance = halfSize[0];
        for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
        {
          auto const wallDistance = halfSize[dimensionID] - centerDistance[dimensionID];
          if (minWallDistance > wallDistance)
            minWallDistance = wallDistance;
        }
        return minWallDistance;
      }
      else
      {
        Vector distance;
        for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
        {
          distance[dimensionID] = std::max(Geometry{}, centerDistance[dimensionID] - halfSize[dimensionID]);
        }
        return Size(distance);
      }
    }

    static constexpr std::optional<Geometry> GetRayBoxDistanceAD(
      Vector const& center, Vector const& halfSizes, TVector const& rayOrigin, TVector const& rayDirection, TScalar tolerance) noexcept
    {
      assert(tolerance >= 0 && "Tolerance cannot be negative!");
      if (DoesBoxContainPointAD(center, halfSizes, rayOrigin, tolerance))
        return Geometry{};

      auto constexpr inf = std::numeric_limits<Geometry>::max();
      auto minBoxDistance = -inf;
      auto maxBoxDistance = +inf;
      auto const tolerance_ = Geometry(tolerance);

      for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
      {
        auto const origin = Geometry(GA::GetPointC(rayOrigin, dimensionID));
        auto const direction = Geometry(GA::GetPointC(rayDirection, dimensionID));
        auto const boxMin = center[dimensionID] - halfSizes[dimensionID] - tolerance_;
        auto const boxMax = center[dimensionID] + halfSizes[dimensionID] + tolerance_;
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
          auto const directionReciprocal = Geometry(1) / direction;
          auto t1 = (boxMin - origin) * directionReciprocal;
          auto t2 = (boxMax - origin) * directionReciprocal;
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

    static constexpr Geometry GetVolumeAD(Box const& range) noexcept
    {
      Geometry volume = 1.0;
      for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
      {
        volume *= range.Max[dimensionID] - range.Min[dimensionID];
      }
      return volume;
    }

    static constexpr Geometry GetVolumeAD(TBox const& range) noexcept
    {
      Geometry volume = 1.0;
      for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
      {
        volume *= Geometry(GA::GetBoxMaxC(range, dimensionID) - GA::GetBoxMinC(range, dimensionID));
      }
      return volume;
    }

    class RayHitTester
    {
    public:
      constexpr static std::optional<RayHitTester> Make(const TVector& origin, const TVector& normalizedDirection, Geometry minTolerance, Geometry toleranceIncrement)
      {
        if (!GA::IsNormalizedVector(normalizedDirection))
        {
          assert(false && "Normalized vector is required for Ray direction!");
          return std::nullopt;
        }

        auto boxPickTester = std::optional<RayHitTester>(std::in_place, RayHitTester{});
        for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
          boxPickTester->m_origin[dimensionID] = GA::GetPointC(origin, dimensionID);

        boxPickTester->m_minTolerance = minTolerance;
        boxPickTester->m_toleranceIncrement = toleranceIncrement;

        detail::static_for<DIMENSION_NO>(
          [&](dim_t dimensionID) noexcept { boxPickTester->m_normalizedDirection[dimensionID] = GA::GetPointC(normalizedDirection, dimensionID); });

        for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
        {
          auto const nd = Geometry(GA::GetPointC(normalizedDirection, dimensionID));
          auto const isNanCompnent = std::fabs(nd) < 2.0 * std::numeric_limits<Geometry>::epsilon();
          boxPickTester->m_inverseDirection[dimensionID] = isNanCompnent ? std::numeric_limits<Geometry>::quiet_NaN() : (Geometry(1) / nd);
          boxPickTester->m_signInfo.set(dimensionID, nd < 0);

          boxPickTester->m_hasNaNComponent |= isNanCompnent;
        }

        return boxPickTester;
      }

      struct PickResult
      {
        Geometry enterDistance, exitDistance;
      };

      template<bool isConeToleranceConsidered = true>
      constexpr std::optional<PickResult> Hit(const Vector& center, const Vector& halfSize) const noexcept
      {
        Vector minDifference, maxDifference;
        detail::static_for<DIMENSION_NO>(
          [&](dim_t dimensionID) noexcept { minDifference[dimensionID] = center[dimensionID] - halfSize[dimensionID] - m_origin[dimensionID]; });
        detail::static_for<DIMENSION_NO>(
          [&](dim_t dimensionID) noexcept { maxDifference[dimensionID] = center[dimensionID] + halfSize[dimensionID] - m_origin[dimensionID]; });

        return BoxHitTest<isConeToleranceConsidered>(minDifference, maxDifference);
      }

      template<bool isConeToleranceConsidered = true>
      constexpr std::optional<PickResult> Hit(const TBox& box) const noexcept
      {
        Vector minDifference, maxDifference;
        detail::static_for<DIMENSION_NO>(
          [&](dim_t dimensionID) noexcept { minDifference[dimensionID] = GA::GetBoxMinC(box, dimensionID) - m_origin[dimensionID]; });
        detail::static_for<DIMENSION_NO>(
          [&](dim_t dimensionID) noexcept { maxDifference[dimensionID] = GA::GetBoxMaxC(box, dimensionID) - m_origin[dimensionID]; });

        return BoxHitTest<isConeToleranceConsidered>(minDifference, maxDifference);
      }

      template<bool isConeToleranceConsidered = true>
      constexpr std::optional<PickResult> Hit(const TVector& point) const noexcept
      {
        Vector v;
        detail::static_for<DIMENSION_NO>([&](dim_t d) noexcept { v[d] = GA::GetPointC(point, d) - m_origin[d]; });

        auto t = Geometry{};
        detail::static_for<DIMENSION_NO>([&](dim_t d) noexcept { t += v[d] * m_normalizedDirection[d]; });

        auto length2 = Geometry{};
        detail::static_for<DIMENSION_NO>([&](dim_t d) noexcept { length2 += v[d] * v[d]; });
        auto const dist2 = std::max(Geometry(0), length2 - t * t);

        Geometry currentTolerance;
        if constexpr (isConeToleranceConsidered)
        {
          currentTolerance = std::max(Geometry(0), t) * m_toleranceIncrement + m_minTolerance;
        }
        else
        {
          currentTolerance = m_minTolerance;
        }

        if (dist2 <= (currentTolerance * currentTolerance) && t >= -currentTolerance)
          return PickResult{ .enterDistance = std::max(Geometry(0), t - currentTolerance), .exitDistance = t + currentTolerance };

        return std::nullopt;
      }

    private:
      constexpr RayHitTester() = default;

      template<bool isConeToleranceConsidered = true>
      constexpr std::optional<PickResult> BoxHitTest(const Vector& minDifference, const Vector& maxDifference) const noexcept
      {
        // plane distances
        std::array<Vector, 2> pd;
        detail::static_for<DIMENSION_NO>(
          [&](dim_t dimensionID) noexcept { pd[0][dimensionID] = minDifference[dimensionID] * m_inverseDirection[dimensionID]; });
        detail::static_for<DIMENSION_NO>(
          [&](dim_t dimensionID) noexcept { pd[1][dimensionID] = maxDifference[dimensionID] * m_inverseDirection[dimensionID]; });

        std::optional pickResult =
          PickResult{ .enterDistance = -std::numeric_limits<Geometry>::max(), .exitDistance = std::numeric_limits<Geometry>::max() };

        // Find the largest entering distance and the smallest exiting distance. fmax/fmin handles NaN correctly.
        if (m_hasNaNComponent)
        {
          detail::static_for<DIMENSION_NO>([&](dim_t dimensionID) noexcept {
            pickResult->enterDistance = std::fmax(pickResult->enterDistance, pd[m_signInfo[dimensionID]][dimensionID]);
          });
          detail::static_for<DIMENSION_NO>([&](dim_t dimensionID) noexcept {
            pickResult->exitDistance = std::fmin(pickResult->exitDistance, pd[1 - m_signInfo[dimensionID]][dimensionID]);
          });
        }
        else
        {
          detail::static_for<DIMENSION_NO>([&](dim_t dimensionID) noexcept {
            pickResult->enterDistance = std::max(pickResult->enterDistance, pd[m_signInfo[dimensionID]][dimensionID]);
          });
          detail::static_for<DIMENSION_NO>([&](dim_t dimensionID) noexcept {
            pickResult->exitDistance = std::min(pickResult->exitDistance, pd[1 - m_signInfo[dimensionID]][dimensionID]);
          });
        }

        // enterDistance/exitDistance is not Nan from here

        // Apply tolerance
        auto exitTolerance = Geometry{};
        if constexpr (isConeToleranceConsidered)
        {
          exitTolerance = std::max(Geometry(0), pickResult->exitDistance) * m_toleranceIncrement + m_minTolerance;

          pickResult->enterDistance -= Geometry(0.5) * (std::max(Geometry(0), pickResult->enterDistance) * m_toleranceIncrement + m_minTolerance);
          pickResult->exitDistance += Geometry(0.5) * (std::max(Geometry(0), pickResult->exitDistance) * m_toleranceIncrement + m_minTolerance);
        }
        else
        { // Numerical inaccuracies could cause false-miss
          exitTolerance = Geometry(0.5) * std::max(Geometry(1), pickResult->exitDistance) * std::numeric_limits<Geometry>::epsilon();

          pickResult->enterDistance -= exitTolerance;
          pickResult->exitDistance += exitTolerance;
        }

        // Only real, ray-direction hit is allowed
        if (pickResult->enterDistance >= pickResult->exitDistance || pickResult->exitDistance <= Geometry(0))
        {
          pickResult = std::nullopt;
          return pickResult;
        }

        // Ray origin inside the box case
        pickResult->enterDistance = std::max(Geometry(0), pickResult->enterDistance);

        // Handle zero direction components
        if (m_hasNaNComponent)
        {
          for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
          {
            if (!std::isnan(m_inverseDirection[dimensionID]))
              continue;

            if (minDifference[dimensionID] > exitTolerance || maxDifference[dimensionID] < -exitTolerance)
            {
              pickResult.reset();
              return pickResult;
            }
          }
        }

        return pickResult;
      }

    private:
      Vector m_origin;
      Vector m_inverseDirection;
      Vector m_normalizedDirection;
      Geometry m_minTolerance;
      Geometry m_toleranceIncrement;
      std::bitset<DIMENSION_NO> m_signInfo;
      bool m_hasNaNComponent = false;
    };
  };
} // namespace OrthoTree::detail
