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

#ifndef ORTHOTREE_GUARD
#define ORTHOTREE_GUARD

#include <algorithm>
#include <concepts>
#include <execution>
#include <functional>
#include <iterator>
#include <numeric>
#include <optional>
#include <stdexcept>
#include <type_traits>

#include <array>
#include <bitset>
#include <map>
#include <queue>
#include <set>
#include <span>
#include <tuple>
#include <unordered_map>
#include <vector>

#include <assert.h>
#include <math.h>

#ifndef autoc
#define autoc auto const
#define undef_autoc
#endif

#ifndef autoce
#define autoce auto constexpr
#define undef_autoce
#endif


#if defined(__clang__)
#define LOOPIVDEP
#elif defined(__INTEL_COMPILER)
#define LOOPIVDEP _Pragma("ivdep")
#elif defined(__GNUC__)
#define LOOPIVDEP _Pragma("GCC ivdep")
#elif defined(_MSC_VER)
#define LOOPIVDEP _Pragma("loop(ivdep)")
#else
#define LOOPIVDEP
#endif


namespace OrthoTree
{
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wreturn-type"
#endif

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
#endif

#ifdef _MSC_VER
#pragma warning(disable : 4715)
#endif

  // Crash the program if out_of_range exception is raised
  template<typename container_type>
  inline auto& cont_at(container_type& container, typename std::remove_reference_t<container_type>::key_type const& id) noexcept
  {
    return container.at(id);
  }


#ifdef _MSC_VER
#pragma warning(default : 4715)
#endif

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

  template<uint8_t e>
  consteval std::size_t pow2_ce()
  {
    constexpr auto bitSize = sizeof(std::size_t) * CHAR_BIT;
    static_assert(e >= 0 && e < bitSize);
    return std::size_t{ 1 } << e;
  }

  constexpr int32_t pow2(int32_t e)
  {
    assert(e >= 0 && e < 31);
    return int32_t(1) << e;
  }

  // Type of the dimension
  using dim_t = int;

  // Type of depth
  using depth_t = int;

  // Grid id
  using GridID = uint32_t;

  // Enum of relation with Planes
  enum class PlaneRelation : char
  {
    Negative,
    Hit,
    Positive
  };

  // Adaptor concepts

  template<class TAdapter, typename TVector, typename TBox, typename TRay, typename TPlane, typename TGeometry = double>
  concept AdaptorBasicsConcept = requires(TVector& point, dim_t dimensionID, TGeometry value) {
    {
      TAdapter::SetPointC(point, dimensionID, value)
    };
  } && requires(TVector const& point, dim_t dimensionID) {
    {
      TAdapter::GetPointC(point, dimensionID)
    } -> std::convertible_to<TGeometry>;
  } && requires(TBox& box, dim_t dimensionID, TGeometry value) {
    {
      TAdapter::SetBoxMinC(box, dimensionID, value)
    };
  } && requires(TBox& box, dim_t dimensionID, TGeometry value) {
    {
      TAdapter::SetBoxMaxC(box, dimensionID, value)
    };
  } && requires(TBox const& box, dim_t dimensionID) {
    {
      TAdapter::GetBoxMinC(box, dimensionID)
    } -> std::convertible_to<TGeometry>;
  } && requires(TBox const& box, dim_t dimensionID) {
    {
      TAdapter::GetBoxMaxC(box, dimensionID)
    } -> std::convertible_to<TGeometry>;
  } && requires(TRay const& ray) {
    {
      TAdapter::GetRayOrigin(ray)
    } -> std::convertible_to<TVector>;
  } && requires(TRay const& ray) {
    {
      TAdapter::GetRayDirection(ray)
    } -> std::convertible_to<TVector>;
  } && requires(TPlane const& plane) {
    {
      TAdapter::GetPlaneNormal(plane)
    } -> std::convertible_to<TVector>;
  } && requires(TPlane const& plane) {
    {
      TAdapter::GetPlaneOrigoDistance(plane)
    } -> std::convertible_to<TGeometry>;
  };

  template<class TAdapter, typename TVector, typename TBox, typename TRay, typename TPlane, typename TGeometry = double>
  concept AdaptorConcept =
    requires { AdaptorBasicsConcept<TAdapter, TVector, TBox, TRay, TPlane, TGeometry>; } && requires(TBox const& box, TVector const& point) {
      {
        TAdapter::DoesBoxContainPoint(box, point)
      } -> std::convertible_to<bool>;
    } && requires(TBox const& e1, TBox const& e2, bool e1_must_contain_e2) {
      {
        TAdapter::AreBoxesOverlapped(e1, e2, e1_must_contain_e2)
      } -> std::convertible_to<bool>;
    } && requires(TBox const& e1, TBox const& e2) {
      {
        TAdapter::AreBoxesOverlappedStrict(e1, e2)
      } -> std::convertible_to<bool>;
    } && requires(std::span<TVector const> const& points) {
      {
        TAdapter::GetBoxOfPoints(points)
      } -> std::convertible_to<TBox>;
    } && requires(std::span<TBox const> const& boxes) {
      {
        TAdapter::GetBoxOfBoxes(boxes)
      } -> std::convertible_to<TBox>;
    } && requires(TVector const& box, TGeometry distanceOfOrigo, TVector const& planeNormal, TGeometry tolerance) {
      {
        TAdapter::GetPointPlaneRelation(box, distanceOfOrigo, planeNormal, tolerance)
      } -> std::convertible_to<PlaneRelation>;
    } && requires(TBox const& box, TGeometry distanceOfOrigo, TVector const& planeNormal, TGeometry tolerance) {
      {
        TAdapter::GetBoxPlaneRelation(box, distanceOfOrigo, planeNormal, tolerance)
      } -> std::convertible_to<PlaneRelation>;
    } && requires(TBox const& box, TVector const& rayBasePoint, TVector const& rayHeading, TGeometry tolerance) {
      {
        TAdapter::IsRayHit(box, rayBasePoint, rayHeading, tolerance)
      } -> std::convertible_to<std::optional<double>>;
    } && requires(TBox const& box, TRay const& ray, TGeometry tolerance) {
      {
        TAdapter::IsRayHit(box, ray, tolerance)
      } -> std::convertible_to<std::optional<double>>;
    };

  // Adaptors

  template<dim_t DIMENSION_NO, typename TVector, typename TBox, typename TRay, typename TPlane, typename TGeometry = double>
  struct AdaptorGeneralBasics
  {
    static constexpr TGeometry GetPointC(TVector const& point, dim_t dimensionID) noexcept { return point[dimensionID]; }
    static constexpr void SetPointC(TVector& point, dim_t dimensionID, TGeometry value) noexcept { point[dimensionID] = value; }

    static constexpr TGeometry GetBoxMinC(TBox const& box, dim_t dimensionID) noexcept { return box.Min[dimensionID]; }
    static constexpr TGeometry GetBoxMaxC(TBox const& box, dim_t dimensionID) noexcept { return box.Max[dimensionID]; }
    static constexpr void SetBoxMinC(TBox& box, dim_t dimensionID, TGeometry value) noexcept { box.Min[dimensionID] = value; }
    static constexpr void SetBoxMaxC(TBox& box, dim_t dimensionID, TGeometry value) noexcept { box.Max[dimensionID] = value; }

    static constexpr TVector const& GetRayDirection(TRay const& ray) noexcept { return ray.Direction; }
    static constexpr TVector const& GetRayOrigin(TRay const& ray) noexcept { return ray.Origin; }

    static constexpr TVector const& GetPlaneNormal(TPlane const& plane) noexcept { return plane.Normal; }
    static constexpr TGeometry GetPlaneOrigoDistance(TPlane const& plane) noexcept { return plane.OrigoDistance; }
  };


  template<dim_t DIMENSION_NO, typename TVector, typename TBox, typename TRay, typename TPlane, typename TGeometry, typename TAdaptorBasics>
  struct AdaptorGeneralBase : TAdaptorBasics
  {
    using Base = TAdaptorBasics;
    static_assert(AdaptorBasicsConcept<Base, TVector, TBox, TRay, TPlane, TGeometry>);

    static constexpr TGeometry Size2(TVector const& point) noexcept
    {
      auto d2 = TGeometry{ 0 };
      for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
      {
        autoc d = Base::GetPointC(point, dimensionID);
        d2 += d * d;
      }
      return d2;
    }

    static constexpr TGeometry Size(TVector const& point) noexcept { return std::sqrt(Size2(point)); }

    static constexpr TVector Add(TVector const& ptL, TVector const& ptR) noexcept
    {
      auto point = TVector{};
      for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
        Base::SetPointC(point, dimensionID, Base::GetPointC(ptL, dimensionID) + Base::GetPointC(ptR, dimensionID));

      return point;
    }

    static constexpr TVector Subtract(TVector const& ptL, TVector const& ptR) noexcept
    {
      auto point = TVector{};
      for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
        Base::SetPointC(point, dimensionID, Base::GetPointC(ptL, dimensionID) - Base::GetPointC(ptR, dimensionID));

      return point;
    }

    static constexpr TVector Multiply(TVector const& ptL, double rScalarR) noexcept
    {
      auto point = TVector{};
      for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
        Base::SetPointC(point, dimensionID, TGeometry(Base::GetPointC(ptL, dimensionID) * rScalarR));

      return point;
    }

    static constexpr TGeometry Dot(TVector const& ptL, TVector const& ptR) noexcept
    {
      auto value = TGeometry{};
      for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
        value += Base::GetPointC(ptL, dimensionID) * Base::GetPointC(ptR, dimensionID);

      return value;
    }

    static constexpr TGeometry Distance(TVector const& ptL, TVector const& ptR) noexcept { return Size(Subtract(ptL, ptR)); }

    static constexpr TGeometry Distance2(TVector const& ptL, TVector const& ptR) noexcept { return Size2(Subtract(ptL, ptR)); }

    static constexpr bool ArePointsEqual(TVector const& ptL, TVector const& ptR, TGeometry rAccuracy) noexcept
    {
      return Distance2(ptL, ptR) <= rAccuracy * rAccuracy;
    }

    static constexpr bool IsNormalizedVector(TVector const& normal) noexcept { return std::abs(Size2(normal) - 1.0) < 0.000001; }

    static constexpr bool DoesBoxContainPoint(TBox const& box, TVector const& point, TGeometry tolerance = 0) noexcept
    {
      for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
        if (!(Base::GetBoxMinC(box, dimensionID) - tolerance <= Base::GetPointC(point, dimensionID) &&
              Base::GetPointC(point, dimensionID) <= Base::GetBoxMaxC(box, dimensionID) + tolerance))
          return false;

      return true;
    }

    static constexpr bool DoesBoxContainPointStrict(TBox const& box, TVector const& point) noexcept
    {
      for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
        if (!(Base::GetBoxMinC(box, dimensionID) < Base::GetPointC(point, dimensionID) &&
              Base::GetPointC(point, dimensionID) < Base::GetBoxMaxC(box, dimensionID)))
          return false;

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

      return (rel & EBoxRelationCandidate::AdjecentC) == EBoxRelationCandidate::AdjecentC ? EBoxRelation::Adjecent : EBoxRelation::Overlapped;
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
        autoc rel = GetBoxRelation(e1, e2);
        if (fOverlapPtTouchAllowed)
          return rel == EBoxRelation::Adjecent || rel == EBoxRelation::Overlapped;
        else
          return rel == EBoxRelation::Overlapped;
      }
    }

    static inline TBox BoxInvertedInit() noexcept
    {
      auto ext = TBox{};

      LOOPIVDEP
      for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
      {
        Base::SetBoxMinC(ext, dimensionID, std::numeric_limits<TGeometry>::max());
        Base::SetBoxMaxC(ext, dimensionID, std::numeric_limits<TGeometry>::lowest());
      }

      return ext;
    }

    static TBox GetBoxOfPoints(std::span<TVector const> const& points) noexcept
    {
      auto ext = BoxInvertedInit();
      for (autoc& point : points)
        for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
        {
          if (Base::GetBoxMinC(ext, dimensionID) > Base::GetPointC(point, dimensionID))
            Base::SetBoxMinC(ext, dimensionID, Base::GetPointC(point, dimensionID));

          if (Base::GetBoxMaxC(ext, dimensionID) < Base::GetPointC(point, dimensionID))
            Base::SetBoxMaxC(ext, dimensionID, Base::GetPointC(point, dimensionID));
        }

      return ext;
    }

    static TBox GetBoxOfBoxes(std::span<TBox const> const& vExtent) noexcept
    {
      auto ext = BoxInvertedInit();
      for (autoc& e : vExtent)
      {
        for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
        {
          if (Base::GetBoxMinC(ext, dimensionID) > Base::GetBoxMinC(e, dimensionID))
            Base::SetBoxMinC(ext, dimensionID, Base::GetBoxMinC(e, dimensionID));

          if (Base::GetBoxMaxC(ext, dimensionID) < Base::GetBoxMaxC(e, dimensionID))
            Base::SetBoxMaxC(ext, dimensionID, Base::GetBoxMaxC(e, dimensionID));
        }
      }
      return ext;
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

    static constexpr std::optional<double> IsRayHit(TBox const& box, TVector const& rayBasePoint, TVector const& rayHeading, TGeometry tolerance) noexcept
    {
      if (DoesBoxContainPoint(box, rayBasePoint, tolerance))
        return 0.0;

      autoce inf = std::numeric_limits<double>::infinity();

      auto minDistances = std::array<double, DIMENSION_NO>{};
      auto maxDistances = std::array<double, DIMENSION_NO>{};
      for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
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

    static constexpr std::optional<double> IsRayHit(TBox const& box, TRay const& ray, TGeometry tolerance) noexcept
    {
      return IsRayHit(box, Base::GetRayOrigin(ray), Base::GetRayDirection(ray), tolerance);
    }

    // Get point-Hyperplane relation (Plane equation: dotProduct(planeNormal, point) = distanceOfOrigo)
    static constexpr PlaneRelation GetPointPlaneRelation(TVector const& point, TGeometry distanceOfOrigo, TVector const& planeNormal, TGeometry tolerance) noexcept
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
    static constexpr PlaneRelation GetBoxPlaneRelation(TBox const& box, TGeometry distanceOfOrigo, TVector const& planeNormal, TGeometry tolerance) noexcept
    {
      assert(IsNormalizedVector(planeNormal));

      TVector center, radius;
      for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
      {
        autoc minComponent = Base::GetBoxMinC(box, dimensionID);
        autoc maxComponent = Base::GetBoxMaxC(box, dimensionID);
        autoc centerComponent = static_cast<TGeometry>((minComponent + maxComponent) * 0.5);
        Base::SetPointC(center, dimensionID, centerComponent);
        Base::SetPointC(radius, dimensionID, centerComponent - minComponent);
      }

      auto radiusProjected = double(tolerance);
      for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
        radiusProjected += Base::GetPointC(radius, dimensionID) * std::abs(Base::GetPointC(planeNormal, dimensionID));

      autoc centerProjected = Dot(planeNormal, center);

      if (centerProjected + radiusProjected < distanceOfOrigo)
        return PlaneRelation::Negative;

      if (centerProjected - radiusProjected > distanceOfOrigo)
        return PlaneRelation::Positive;

      return PlaneRelation::Hit;
    }
  };


  template<dim_t DIMENSION_NO, typename TVector, typename TBox, typename TRay, typename TPlane, typename TGeometry = double>
  using AdaptorGeneral =
    AdaptorGeneralBase<DIMENSION_NO, TVector, TBox, TRay, TPlane, TGeometry, AdaptorGeneralBasics<DIMENSION_NO, TVector, TBox, TRay, TPlane, TGeometry>>;


  // Bitset helpers for higher dimensions


  template<std::size_t N>
  using bitset_arithmetic = std::bitset<N>;

  template<std::size_t N>
  bitset_arithmetic<N> operator+(bitset_arithmetic<N> const& lhs, bitset_arithmetic<N> const& rhs) noexcept
  {
    bool carry = false;
    auto ans = bitset_arithmetic<N>();
    for (std::size_t i = 0; i < N; ++i)
    {
      autoc sum = (lhs[i] ^ rhs[i]) ^ carry;
      carry = (lhs[i] && rhs[i]) || (lhs[i] && carry) || (rhs[i] && carry);
      ans[i] = sum;
    }

    assert(!carry); // unhandled overflow
    return ans;
  }

  template<std::size_t N>
  bitset_arithmetic<N> operator+(bitset_arithmetic<N> const& lhs, std::size_t rhs) noexcept
  {
    return lhs + bitset_arithmetic<N>(rhs);
  }

  template<std::size_t N>
  bitset_arithmetic<N> operator-(bitset_arithmetic<N> const& lhs, bitset_arithmetic<N> const& rhs) noexcept
  {
    assert(lhs >= rhs);

    auto ret = lhs;
    bool borrow = false;
    for (std::size_t i = 0; i < N; ++i)
    {
      if (borrow)
      {
        if (ret[i])
        {
          ret[i] = rhs[i];
          borrow = rhs[i];
        }
        else
        {
          ret[i] = !rhs[i];
          borrow = true;
        }
      }
      else
      {
        if (ret[i])
        {
          ret[i] = !rhs[i];
          borrow = false;
        }
        else
        {
          ret[i] = rhs[i];
          borrow = rhs[i];
        }
      }
    }

    return ret;
  }

  template<std::size_t N>
  bitset_arithmetic<N> operator-(bitset_arithmetic<N> const& lhs, std::size_t rhs) noexcept
  {
    return lhs - bitset_arithmetic<N>(rhs);
  }

  template<std::size_t N>
  bitset_arithmetic<N> operator*(bitset_arithmetic<N> const& lhs, bitset_arithmetic<N> const& rhs) noexcept
  {
    auto ret = bitset_arithmetic<N>{};

    if (lhs.count() < rhs.count())
    {
      for (std::size_t i = 0; i < N; ++i)
        if (lhs[i])
          ret = ret + (rhs << i);
    }
    else
    {
      for (std::size_t i = 0; i < N; ++i)
        if (rhs[i])
          ret = ret + (lhs << i);
    }

    return ret;
  }

  template<std::size_t N>
  bitset_arithmetic<N> operator*(bitset_arithmetic<N> const& lhs, std::size_t rhs) noexcept
  {
    return lhs * bitset_arithmetic<N>(rhs);
  }

  template<std::size_t N>
  bitset_arithmetic<N> operator*(std::size_t rhs, bitset_arithmetic<N> const& lhs) noexcept
  {
    return lhs * bitset_arithmetic<N>(rhs);
  }

  template<std::size_t N>
  static std::tuple<bitset_arithmetic<N>, bitset_arithmetic<N>> gf2_div(bitset_arithmetic<N> const& dividend, bitset_arithmetic<N> divisor) noexcept
  {
    if (divisor.none())
    {
      assert(false);
      return {};
    }

    if (dividend.none())
      return {};

    auto quotent = bitset_arithmetic<N>{ 0 };
    if (dividend == divisor)
      return { bitset_arithmetic<N>(1), quotent };

    if (dividend < divisor)
      return { quotent, dividend };


    std::size_t sig_dividend = 0;
    for (std::size_t i = 0, id = N - 1; i < N; ++i, --id)
      if (dividend[id])
      {
        sig_dividend = id;
        break;
      }

    std::size_t sig_divisor = 0;
    for (std::size_t i = 0, id = N - 1; i < N; ++i, --id)
      if (divisor[id])
      {
        sig_divisor = id;
        break;
      }

    std::size_t nAlignment = (sig_dividend - sig_divisor);
    divisor <<= nAlignment;
    nAlignment += 1;
    auto remainder = dividend;
    while (nAlignment--)
    {
      if (divisor <= remainder)
      {
        quotent[nAlignment] = true;
        remainder = remainder - divisor;
      }
      divisor >>= 1;
    }

    return { quotent, remainder };
  }

  template<std::size_t N>
  bitset_arithmetic<N> operator/(bitset_arithmetic<N> const& dividend, bitset_arithmetic<N> const& divisor) noexcept
  {
    return std::get<0>(gf2_div(dividend, divisor));
  }

  template<std::size_t N>
  auto operator<=>(bitset_arithmetic<N> const& lhs, bitset_arithmetic<N> const& rhs) noexcept
  {
    using R = std::strong_ordering;
    for (std::size_t i = 0, id = N - 1; i < N; ++i, --id)
      if (lhs[id] ^ rhs[id])
        return lhs[id] ? R::greater : R::less;

    return R::equal;
  }

  struct bitset_arithmetic_compare final
  {
    template<std::size_t N>
    bool operator()(bitset_arithmetic<N> const& lhs, bitset_arithmetic<N> const& rhs) const noexcept
    {
      return lhs < rhs;
    }
  };


  // NTrees


  // NTreeLinear: Non-owning Base container which spatially organize data ids in N dimension space into a hash-table by Morton Z order.
  template<
    dim_t DIMENSION_NO,
    typename TVector_,
    typename TBox_,
    typename TRay_,
    typename TPlane_,
    typename TGeometry_ = double,
    typename TAdapter = AdaptorGeneral<DIMENSION_NO, TVector_, TBox_, TRay_, TPlane_, TGeometry_>>
  class OrthoTreeBase
  {
  public:
    struct UpdateID
    {
      enum : std::size_t
      {
        ERASE = std::numeric_limits<std::size_t>::max()
      };
    };

    static autoce IS_LINEAR_TREE = DIMENSION_NO < 15;

    // Max value: 2 ^ DIMENSION_NO
    using ChildID = uint64_t;

    // Max value: 2 ^ nDepth ^ DIMENSION_NO * 2 (signal bit)
    using MortonGridID =
      typename std::conditional <
      DIMENSION_NO<4, uint32_t, typename std::conditional<IS_LINEAR_TREE, uint64_t, bitset_arithmetic<DIMENSION_NO * 4 + 1>>::type>::type;

    using MortonNodeID = MortonGridID; // same as the MortonGridID, but depth is signed by a sentinel bit.
    using MortonGridIDCR = typename std::conditional<IS_LINEAR_TREE, MortonNodeID const, MortonNodeID const&>::type;
    using MortonNodeIDCR = MortonGridIDCR;
    using TGeometry = TGeometry_;
    using TVector = TVector_;
    using TBox = TBox_;
    using TRay = TRay_;
    using TPlane = TPlane_;

    using AD = TAdapter;
    static_assert(AdaptorConcept<AD, TVector, TBox, TRay, TPlane, TGeometry>);
    static_assert(0 < DIMENSION_NO && DIMENSION_NO < 64);

  protected:
    // Max number of children
    static autoce CHILD_NO = pow2_ce<DIMENSION_NO>();
    // Type system determined maximal depth.
    static autoce MAX_THEORETICAL_DEPTH = depth_t((CHAR_BIT * sizeof(MortonNodeID) - 1 /*sentinal bit*/) / DIMENSION_NO);

  public:
    class Node
    {
    private:
      std::vector<MortonNodeID> m_children;

    public: // Public members
      std::vector<std::size_t> Entities = {};
      TBox Box = {};

    public:
      constexpr void AddChild(MortonNodeIDCR childKey) noexcept { m_children.emplace_back(childKey); }

      constexpr void AddChildInOrder(MortonNodeIDCR childKey) noexcept
      {
        auto it = std::end(m_children);
        if constexpr (IS_LINEAR_TREE)
          it = std::lower_bound(m_children.begin(), m_children.end(), childKey);
        else
          it = std::lower_bound(m_children.begin(), m_children.end(), childKey, bitset_arithmetic_compare{});

        if (it != m_children.end() && *it == childKey)
          return;

        m_children.insert(it, childKey);
      }

      constexpr bool HasChild(MortonNodeIDCR childKey) const noexcept
      {
        if constexpr (IS_LINEAR_TREE)
          return std::ranges::binary_search(m_children, childKey);
        else
          return std::ranges::binary_search(m_children, childKey, bitset_arithmetic_compare{});
      }

      constexpr bool IsChildNodeEnabled(ChildID childID) const noexcept
      {
        autoc childMortonID = MortonNodeID(childID);
        return std::find_if(m_children.begin(), m_children.end(), [childMortonID](autoc& childKey) {
          return (childKey & childMortonID) == childMortonID;
        });
      }

      constexpr void DisableChild(MortonNodeIDCR childKey) noexcept
      {
        auto it = std::end(m_children);
        if constexpr (IS_LINEAR_TREE)
          it = std::lower_bound(m_children.begin(), m_children.end(), childKey);
        else
          it = std::lower_bound(m_children.begin(), m_children.end(), childKey, bitset_arithmetic_compare{});

        if (it == std::end(m_children))
          return;

        m_children.erase(it);
      }

      constexpr bool IsAnyChildExist() const noexcept { return !m_children.empty(); }
      constexpr std::vector<MortonNodeID> const& GetChildren() const noexcept { return m_children; }
    };


  protected: // Aid struct to partitioning and distance ordering
    struct ItemDistance
    {
      TGeometry Distance;
      auto operator<=>(ItemDistance const& rhs) const = default;
    };

    struct EntityDistance : ItemDistance
    {
      std::size_t EntityID;
      auto operator<=>(EntityDistance const& rhs) const = default;
    };

    struct BoxDistance : ItemDistance
    {
      MortonNodeID NodeKey;
      Node const& NodeReference;
    };

    template<typename T>
    using DimArray = std::array<T, DIMENSION_NO>;

    template<typename TData>
    using UnderlyingContainer =
      typename std::conditional<IS_LINEAR_TREE, std::unordered_map<MortonNodeID, TData>, std::map<MortonNodeID, TData, bitset_arithmetic_compare>>::type;

  protected: // Member variables
    UnderlyingContainer<Node> m_nodes;
    TBox m_boxSpace = {};
    depth_t m_maxDepthNo = {};
    GridID m_maxRasterResolution = {};
    GridID m_maxRasterID = {};
    std::size_t m_maxElementNo = 11;
    double m_volumeOfOverallSpace = {};
    DimArray<double> m_rasterizerFactors;
    DimArray<double> m_sizeInDimensions;
    DimArray<double> m_minInDimensions;


  protected: // Aid functions
    template<std::size_t N>
    static inline ChildID convertMortonIdToChildId(bitset_arithmetic<N> const& bs) noexcept
    {
      assert(bs <= bitset_arithmetic<N>(std::numeric_limits<std::size_t>::max()));
      return bs.to_ullong();
    }
    static constexpr ChildID convertMortonIdToChildId(uint64_t morton) noexcept { return morton; }

  protected: // Grid functions
    struct RasterInfo
    {
      DimArray<double> rasterizerFactors;
      DimArray<double> sizeInDimensions;
    };
    static constexpr RasterInfo getGridRasterizer(TBox const& box, GridID subDivisionNo) noexcept
    {
      auto ri = RasterInfo{};
      autoc subDivisionNoFactor = static_cast<double>(subDivisionNo);
      for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
      {
        ri.sizeInDimensions[dimensionID] = static_cast<double>(AD::GetBoxMaxC(box, dimensionID) - AD::GetBoxMinC(box, dimensionID));
        ri.rasterizerFactors[dimensionID] = ri.sizeInDimensions[dimensionID] == 0 ? 1.0 : (subDivisionNoFactor / ri.sizeInDimensions[dimensionID]);
      }

      return ri;
    }


    constexpr DimArray<GridID> getGridIdPoint(TVector const& point) const noexcept
    {
      auto gridIDs = DimArray<GridID>{};
      for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
      {
        autoc pointComponent = AD::GetPointC(point, dimensionID) - AD::GetBoxMinC(this->m_boxSpace, dimensionID);
        auto rasterID = static_cast<double>(pointComponent) * this->m_rasterizerFactors[dimensionID];
        gridIDs[dimensionID] = std::min<GridID>(this->m_maxRasterID, static_cast<GridID>(rasterID));
      }
      return gridIDs;
    }

    template<bool DO_POINT_LIKE_CLASSIFICATION = false>
    constexpr std::array<DimArray<GridID>, 2> getGridIdBox(TBox const& box) const noexcept
    {
      auto aid = std::array<DimArray<GridID>, 2>{};
      for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
      {
        autoc minComponentRasterID =
          static_cast<double>(AD::GetBoxMinC(box, dimensionID) - AD::GetBoxMinC(m_boxSpace, dimensionID)) * m_rasterizerFactors[dimensionID];
        autoc maxComponentRasterID =
          static_cast<double>(AD::GetBoxMaxC(box, dimensionID) - AD::GetBoxMinC(m_boxSpace, dimensionID)) * m_rasterizerFactors[dimensionID];

        if constexpr (DO_POINT_LIKE_CLASSIFICATION)
        {
          aid[0][dimensionID] = std::min<GridID>(this->m_maxRasterID, static_cast<GridID>(minComponentRasterID));
          aid[1][dimensionID] = std::min<GridID>(this->m_maxRasterID, static_cast<GridID>(maxComponentRasterID));
        }
        else
        {
          if (minComponentRasterID < 1.0)
            aid[0][dimensionID] = 0;
          else if (minComponentRasterID > m_maxRasterID)
            aid[0][dimensionID] = m_maxRasterID;
          else
            aid[0][dimensionID] = static_cast<GridID>(minComponentRasterID);

          if (maxComponentRasterID < 1.0)
            aid[1][dimensionID] = 0;
          else if (maxComponentRasterID > m_maxRasterID)
            aid[1][dimensionID] = m_maxRasterID;
          else if (minComponentRasterID != maxComponentRasterID && std::floor(maxComponentRasterID) == maxComponentRasterID)
            aid[1][dimensionID] = static_cast<GridID>(maxComponentRasterID) - 1;
          else
            aid[1][dimensionID] = static_cast<GridID>(maxComponentRasterID);
        }
      }
      return aid;
    }


    inline Node& createChild(Node& parentNode, ChildID childID, MortonNodeIDCR childKey) noexcept
    {
      assert(childID < this->CHILD_NO);
      parentNode.AddChild(childKey);

      auto& nodeChild = m_nodes[childKey];
      if constexpr (std::is_integral_v<TGeometry>)
      {
        DimArray<double> minNodePoint = this->m_minInDimensions;
        DimArray<double> maxNodePoint;

        autoc nDepth = this->GetDepthID(childKey);

        auto scaleFactor = 1.0;
        if constexpr (IS_LINEAR_TREE)
        {
          auto mask = MortonNodeID{ 1 } << (nDepth * DIMENSION_NO - 1);
          for (depth_t iDepth = 0; iDepth < nDepth; ++iDepth)
          {
            scaleFactor *= 0.5;
            for (dim_t dimensionID = DIMENSION_NO; dimensionID > 0; --dimensionID)
            {
              bool isGreater;
              if constexpr (IS_LINEAR_TREE)
                isGreater = (childKey & mask);
              else
                isGreater = (childKey & mask).any();
              minNodePoint[dimensionID - 1] += isGreater * this->m_sizeInDimensions[dimensionID - 1] * scaleFactor;
              mask >>= 1;
            }
          }
        }
        else
        {
          auto bitID = (nDepth * DIMENSION_NO - 1);
          for (depth_t iDepth = 0; iDepth < nDepth; ++iDepth)
          {
            scaleFactor *= 0.5;
            for (dim_t dimensionID = DIMENSION_NO; dimensionID > 0; --dimensionID)
            {
              minNodePoint[dimensionID - 1] += childKey[bitID] * this->m_sizeInDimensions[dimensionID - 1] * scaleFactor;
              --bitID;
            }
          }
        }

        LOOPIVDEP
        for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
          maxNodePoint[dimensionID] = minNodePoint[dimensionID] + this->m_sizeInDimensions[dimensionID] * scaleFactor;

        LOOPIVDEP
        for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
        {
          AD::SetBoxMinC(nodeChild.Box, dimensionID, static_cast<TGeometry>(minNodePoint[dimensionID]));
          AD::SetBoxMaxC(nodeChild.Box, dimensionID, static_cast<TGeometry>(maxNodePoint[dimensionID]));
        }
      }
      else
      {
        LOOPIVDEP
        for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
        {
          autoc isGreater = ((ChildID{ 1 } << dimensionID) & childID) > 0;
          AD::SetBoxMinC(
            nodeChild.Box,
            dimensionID,
            isGreater * (AD::GetBoxMaxC(parentNode.Box, dimensionID) + AD::GetBoxMinC(parentNode.Box, dimensionID)) * TGeometry{ 0.5 } +
              (!isGreater) * AD::GetBoxMinC(parentNode.Box, dimensionID));

          AD::SetBoxMaxC(
            nodeChild.Box,
            dimensionID,
            isGreater * AD::GetBoxMaxC(parentNode.Box, dimensionID) +
              (!isGreater) * ((AD::GetBoxMaxC(parentNode.Box, dimensionID) + AD::GetBoxMinC(parentNode.Box, dimensionID)) * TGeometry{ 0.5 }));
        }
      }
      return nodeChild;
    }


    constexpr MortonGridID getLocationID(TVector const& point) const noexcept { return MortonEncode(this->getGridIdPoint(point)); }


    bool isEveryItemIdUnique() const noexcept
    {
      auto ids = std::vector<std::size_t>();
      ids.reserve(100);
      std::ranges::for_each(m_nodes, [&](auto& node) { ids.insert(ids.end(), node.second.Entities.begin(), node.second.Entities.end()); });

      std::ranges::sort(ids);
      autoc itEndUnique = std::unique(ids.begin(), ids.end());
      return itEndUnique == ids.end();
    }

    template<bool DO_UNIQUENESS_CHECK_TO_INDICIES>
    bool insert(MortonNodeIDCR nodeKey, MortonNodeIDCR smallestNodeKey, std::size_t entityID, bool doInsertToLeaf) noexcept
    {
      if (nodeKey == smallestNodeKey)
      {
        cont_at(this->m_nodes, nodeKey).Entities.emplace_back(entityID);
        if constexpr (DO_UNIQUENESS_CHECK_TO_INDICIES)
          assert(this->isEveryItemIdUnique()); // Assert means: index is already added. Wrong input!
        return true;
      }

      if (doInsertToLeaf)
      {
        auto& newNode = this->m_nodes[nodeKey];
        newNode.Entities.emplace_back(entityID);
        newNode.Box = this->CalculateExtent(nodeKey);

        // Create all child between the new (nodeKey) and the smallest existing one (smallestNodeKey)
        auto parentNodeKey = nodeKey;
        do
        {
          auto childNodeKey = parentNodeKey;
          parentNodeKey >>= DIMENSION_NO;
          assert(IsValidKey(parentNodeKey));
          auto& parentNode = this->m_nodes[parentNodeKey];
          parentNode.AddChildInOrder(childNodeKey);
          parentNode.Box = this->CalculateExtent(parentNodeKey);
        } while (parentNodeKey != smallestNodeKey);
      }
      else
      {
        autoc nodeIterator = this->m_nodes.find(smallestNodeKey);
        if (nodeIterator->second.IsAnyChildExist())
        {
          autoc nDepth = this->GetDepthID(smallestNodeKey);
          autoc childNodeKey = nodeKey << (DIMENSION_NO * (this->m_maxDepthNo - nDepth - 1));
          autoc childID = getChildPartOfLocation(childNodeKey);
          auto& nodeChild = this->createChild(nodeIterator->second, childID, childNodeKey);
          nodeChild.Entities.emplace_back(entityID);
        }
        else
          nodeIterator->second.Entities.emplace_back(entityID);
      }

      if constexpr (DO_UNIQUENESS_CHECK_TO_INDICIES)
        assert(this->isEveryItemIdUnique()); // Assert means: index is already added. Wrong input!

      return true;
    }

    template<typename TData = Node>
    static void reserveContainer(std::map<MortonNodeID, TData, bitset_arithmetic_compare>&, std::size_t) noexcept {};

    template<typename TData = Node>
    static void reserveContainer(std::unordered_map<MortonNodeID, TData>& m, std::size_t n) noexcept
    {
      m.reserve(n);
    };

  public: // Static aid functions
    static constexpr std::size_t EstimateNodeNumber(std::size_t elementNo, depth_t maxDepthNo, std::size_t maxElementNo) noexcept
    {
      assert(maxElementNo > 0);
      assert(maxDepthNo > 0);

      if (elementNo < 10)
        return 10;

      autoce rMult = 1.5;
      if ((maxDepthNo + 1) * DIMENSION_NO < 64)
      {
        std::size_t const nMaxChild = std::size_t{ 1 } << (maxDepthNo * DIMENSION_NO);
        autoc nElementInNode = elementNo / nMaxChild;
        if (nElementInNode > maxElementNo / 2)
          return nMaxChild;
      }

      autoc nElementInNodeAvg = static_cast<float>(elementNo) / static_cast<float>(maxElementNo);
      autoc nDepthEstimated = std::min(maxDepthNo, static_cast<depth_t>(ceil((log2f(nElementInNodeAvg) + 1.0) / static_cast<float>(DIMENSION_NO))));
      if (nDepthEstimated * DIMENSION_NO < 64)
        return static_cast<std::size_t>(rMult * (1 << nDepthEstimated * DIMENSION_NO));

      return static_cast<std::size_t>(rMult * nElementInNodeAvg);
    }


    static inline depth_t EstimateMaxDepth(std::size_t elementNo, std::size_t maxElementNo) noexcept
    {
      if (elementNo < maxElementNo)
        return 2;

      autoc nLeaf = elementNo / maxElementNo;
      // nLeaf = (2^nDepth)^DIMENSION_NO
      return std::clamp(static_cast<depth_t>(std::log2(nLeaf) / static_cast<double>(DIMENSION_NO)), depth_t(2), depth_t(10));
    }


    static inline MortonNodeID GetHash(depth_t depth, MortonNodeIDCR key) noexcept
    {
      assert(key < (MortonNodeID(1) << (depth * DIMENSION_NO)));
      return (MortonNodeID{ 1 } << (depth * DIMENSION_NO)) | key;
    }

    static constexpr MortonNodeID GetRootKey() noexcept { return MortonNodeID{ 1 }; }

    static constexpr bool IsValidKey(uint64_t key) noexcept { return key > 0; }

    template<std::size_t N>
    static inline bool IsValidKey(bitset_arithmetic<N> const& key) noexcept
    {
      return !key.none();
    }

    static depth_t GetDepthID(MortonNodeID key) noexcept
    {
      // Keep shifting off three bits at a time, increasing depth counter
      for (depth_t d = 0; IsValidKey(key); ++d, key >>= DIMENSION_NO)
        if (key == 1) // If only sentinel bit remains, exit with node depth
          return d;

      assert(false); // Bad key
      return 0;
    }

    static inline MortonNodeID RemoveSentinelBit(MortonNodeIDCR key, std::optional<depth_t> const& depthIDOptional = std::nullopt) noexcept
    {
      autoc depthID = depthIDOptional.value_or(GetDepthID(key));
      return key - (MortonNodeID{ 1 } << depthID);
    }


  private: // Morton aid functions
    static inline ChildID getChildPartOfLocation(MortonNodeIDCR key) noexcept
    {
      if constexpr (IS_LINEAR_TREE)
      {
        autoce maskLastBits1 = (MortonNodeID{ 1 } << DIMENSION_NO) - 1;
        return convertMortonIdToChildId(key & maskLastBits1);
      }
      else
      {
        auto childID = MortonNodeID{};
        for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
          childID[dimensionID] = key[dimensionID];

        return convertMortonIdToChildId(childID);
      }
    }

    static constexpr MortonGridID part1By2(GridID n) noexcept
    {
      // n = ----------------------9876543210 : Bits initially
      // n = ------98----------------76543210 : After (1)
      // n = ------98--------7654--------3210 : After (2)
      // n = ------98----76----54----32----10 : After (3)
      // n = ----9--8--7--6--5--4--3--2--1--0 : After (4)
      n = (n ^ (n << 16)) & 0xff0000ff; // (1)
      n = (n ^ (n << 8)) & 0x0300f00f;  // (2)
      n = (n ^ (n << 4)) & 0x030c30c3;  // (3)
      n = (n ^ (n << 2)) & 0x09249249;  // (4)
      return std::is_same<MortonGridID, bitset_arithmetic<DIMENSION_NO>>::value ? MortonGridID(n) : static_cast<MortonGridID>(n);
    }

    // Separates low 16 bits of input by one bit
    static constexpr MortonGridID part1By1(GridID n) noexcept
    {
      // n = ----------------fedcba9876543210 : Bits initially
      // n = --------fedcba98--------76543210 : After (1)
      // n = ----fedc----ba98----7654----3210 : After (2)
      // n = --fe--dc--ba--98--76--54--32--10 : After (3)
      // n = -f-e-d-c-b-a-9-8-7-6-5-4-3-2-1-0 : After (4)
      n = (n ^ (n << 8)) & 0x00ff00ff; // (1)
      n = (n ^ (n << 4)) & 0x0f0f0f0f; // (2)
      n = (n ^ (n << 2)) & 0x33333333; // (3)
      n = (n ^ (n << 1)) & 0x55555555; // (4)
      return std::is_same<MortonGridID, bitset_arithmetic<DIMENSION_NO>>::value ? MortonGridID(n) : static_cast<MortonGridID>(n);
    }

  public:
    static inline MortonGridID MortonEncode(DimArray<GridID> const& gridID) noexcept
    {
      if constexpr (DIMENSION_NO == 1)
        return MortonGridID(gridID[0]);
      else if constexpr (DIMENSION_NO == 2)
        return (part1By1(gridID[1]) << 1) + part1By1(gridID[0]);
      else if constexpr (DIMENSION_NO == 3)
        return (part1By2(gridID[2]) << 2) + (part1By2(gridID[1]) << 1) + part1By2(gridID[0]);
      else
      {
        auto msb = gridID[0];
        for (dim_t dimensionID = 1; dimensionID < DIMENSION_NO; ++dimensionID)
          msb |= gridID[dimensionID];

        MortonGridID id = 0;
        GridID mask = 1;
        for (dim_t i = 0; msb; mask <<= 1, msb >>= 1, ++i)
        {
          LOOPIVDEP
          for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
          {
            autoc shift = dimensionID + i * DIMENSION_NO;
            if constexpr (IS_LINEAR_TREE)
              id |= static_cast<MortonGridID>(gridID[dimensionID] & mask) << (shift - i);
            else
              id[shift] = gridID[dimensionID] & mask;
          }
        }
        return id;
      }
    }

    static DimArray<GridID> MortonDecode(MortonNodeIDCR nodeKey, depth_t maxDepthNo) noexcept
    {
      auto gridID = DimArray<GridID>{};
      if constexpr (DIMENSION_NO == 1)
        return { RemoveSentinelBit(nodeKey) };
      else
      {
        autoc depthID = GetDepthID(nodeKey);

        auto mask = MortonGridID{ 1 };
        for (depth_t iDepth = maxDepthNo - depthID, shift = 0; iDepth < maxDepthNo; ++iDepth)
          for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID, ++shift)
            if constexpr (IS_LINEAR_TREE)
            {
              gridID[dimensionID] |= (nodeKey & mask) >> (shift - iDepth);
              mask <<= 1;
            }
            else
              gridID[dimensionID] |= GridID{ nodeKey[shift] } << iDepth;
      }
      return gridID;
    }


  public: // Getters
    inline auto const& GetNodes() const noexcept { return m_nodes; }
    inline auto const& GetNode(MortonNodeIDCR key) const noexcept { return m_nodes.at(key); }
    inline auto const& GetBox() const noexcept { return m_boxSpace; }
    inline auto GetDepthMax() const noexcept { return m_maxDepthNo; }
    inline auto GetResolutionMax() const noexcept { return m_maxRasterResolution; }


  public: // Main service functions
    // Alternative creation mode (instead of Create), Init then Insert items into leafs one by one. NOT RECOMMENDED.
    constexpr void Init(TBox const& box, depth_t maxDepthNo, std::size_t maxElementNo = 11) noexcept
    {
      assert(this->m_nodes.empty()); // To build/setup/create the tree, use the Create() [recommended] or Init() function. If an already builded tree
                                     // is wanted to be reset, use the Reset() function before init.
      assert(maxDepthNo > 1);
      assert(maxDepthNo <= MAX_THEORETICAL_DEPTH);
      assert(maxDepthNo < std::numeric_limits<uint8_t>::max());
      assert(maxElementNo > 1);
      assert(CHAR_BIT * sizeof(GridID) >= m_maxDepthNo);

      this->m_boxSpace = box;
      this->m_maxDepthNo = maxDepthNo;
      this->m_maxRasterResolution = static_cast<GridID>(pow2(maxDepthNo));
      this->m_maxRasterID = this->m_maxRasterResolution - 1;
      this->m_maxElementNo = maxElementNo;

      auto& nodeRoot = this->m_nodes[GetRootKey()];
      nodeRoot.Box = box;
      autoc ri = this->getGridRasterizer(this->m_boxSpace, this->m_maxRasterResolution);
      this->m_rasterizerFactors = std::move(ri.rasterizerFactors);
      this->m_sizeInDimensions = std::move(ri.sizeInDimensions);
      this->m_volumeOfOverallSpace = 1.0;
      for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
        this->m_volumeOfOverallSpace *= this->m_sizeInDimensions[dimensionID];

      LOOPIVDEP
      for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
        this->m_minInDimensions[dimensionID] = static_cast<double>(AD::GetBoxMinC(this->m_boxSpace, dimensionID));
    }


    using FProcedure = std::function<void(MortonNodeIDCR, Node const&)>;
    using FProcedureUnconditional = std::function<void(MortonNodeIDCR, Node const&, bool)>;
    using FSelector = std::function<bool(MortonNodeIDCR, Node const&)>;
    using FSelectorUnconditional = std::function<bool(MortonNodeIDCR, Node const&)>;


    // Visit nodes with special selection and procedure in breadth-first search order
    void VisitNodes(MortonNodeIDCR rootKey, FProcedure const& procedure, FSelector const& selector) const noexcept
    {
      auto nodeIDsToProceed = std::queue<MortonNodeID>();
      for (nodeIDsToProceed.push(rootKey); !nodeIDsToProceed.empty(); nodeIDsToProceed.pop())
      {
        autoc& key = nodeIDsToProceed.front();
        autoc& node = GetNode(key);
        if (!selector(key, node))
          continue;

        procedure(key, node);

        for (MortonNodeIDCR childKey : node.GetChildren())
          nodeIDsToProceed.push(childKey);
      }
    }


    // Visit nodes with special selection and procedure in breadth-first search order
    inline void VisitNodes(MortonNodeIDCR rootKey, FProcedure const& procedure) const noexcept
    {
      VisitNodes(rootKey, procedure, [](MortonNodeIDCR, Node const&) { return true; });
    }


    // Visit nodes with special selection and procedure and if unconditional selection is fulfilled descendants will not be test with selector
    void VisitNodes(
      MortonNodeIDCR rootKey, FProcedureUnconditional const& procedure, FSelector const& selector, FSelectorUnconditional const& selectorUnconditional) const noexcept
    {
      struct Search
      {
        MortonNodeID Key;
        bool DoAvoidSelectionParent;
      };

      auto nodesToProceed = std::queue<Search>();
      for (nodesToProceed.push({ rootKey, false }); !nodesToProceed.empty(); nodesToProceed.pop())
      {
        autoc & [key, doAvoidSelectionParent] = nodesToProceed.front();

        autoc& node = GetNode(key);
        if (!doAvoidSelectionParent && !selector(key, node))
          continue;

        autoc doAvoidSelection = doAvoidSelectionParent || selectorUnconditional(key, node);
        procedure(key, node, doAvoidSelection);

        for (MortonNodeID childKey : node.GetChildren())
          nodesToProceed.push({ childKey, doAvoidSelection });
      }
    }


    // Visit nodes with special selection and procedure in depth-first search order
    void VisitNodesInDFS(MortonNodeIDCR key, FProcedure const& procedure, FSelector const& selector) const noexcept
    {
      autoc& node = GetNode(key);
      if (!selector(key, node))
        return;

      procedure(key, node);
      for (autoc childKey : node.GetChildren())
        VisitNodesInDFS(childKey, procedure, selector);
    }


    // Collect all item id, traversing the tree in breadth-first search order
    std::vector<std::size_t> CollectAllIdInBFS(MortonNodeIDCR rootKey = GetRootKey()) const noexcept
    {
      auto entityIDs = std::vector<std::size_t>();
      entityIDs.reserve(m_nodes.size() * std::max<std::size_t>(2, m_maxElementNo / 2));

      VisitNodes(rootKey, [&entityIDs](MortonNodeIDCR, autoc& node) { entityIDs.insert(entityIDs.end(), node.Entities.begin(), node.Entities.end()); });
      return entityIDs;
    }

  private:
    template<bool DO_COLLECT_ONLY_LARGER_THAN_MIN_ENTITY_ID = false>
    void collectAllIdInDFS(Node const& parentNode, std::vector<std::size_t>& foundEntities, std::size_t minEntityID = 0) const noexcept
    {
      if constexpr (DO_COLLECT_ONLY_LARGER_THAN_MIN_ENTITY_ID)
      {
        for (autoc entityID : parentNode.Entities)
          if (entityID > minEntityID)
            foundEntities.emplace_back(entityID);
      }
      else
        foundEntities.insert(foundEntities.end(), parentNode.Entities.begin(), parentNode.Entities.end());

      for (MortonNodeIDCR childKey : parentNode.GetChildren())
        collectAllIdInDFS<DO_COLLECT_ONLY_LARGER_THAN_MIN_ENTITY_ID>(this->GetNode(childKey), foundEntities, minEntityID);
    }

  public:
    std::vector<std::size_t> CollectAllIdInDFS(MortonGridIDCR parentKey = GetRootKey()) const noexcept
    {
      auto entityIDs = std::vector<std::size_t>{};
      collectAllIdInDFS(GetNode(parentKey), entityIDs);
      return entityIDs;
    }

    // Update all element which are in the given hash-table. Elements will be erased if the replacement id is std::numeric_limits<std::size_t>::max().
    template<bool DO_UNIQUENESS_CHECK_TO_INDICIES = false>
    void UpdateIndexes(std::unordered_map<std::size_t, std::size_t> const& indexUpdateMap) noexcept
    {
      autoc mapEndIterator = indexUpdateMap.end();
      std::ranges::for_each(m_nodes, [&](auto& node) {
        auto idList = std::vector<std::size_t>(node.second.Entities.size());
        std::ranges::transform(node.second.Entities, idList.begin(), [&](autoc& id) {
          autoc it = indexUpdateMap.find(id);
          return it == mapEndIterator ? id : it->second;
        });

        std::erase_if(idList, [](autoc id) { return id == UpdateID::ERASE; });
        node.second.Entities.swap(idList);
      });

      if constexpr (DO_UNIQUENESS_CHECK_TO_INDICIES)
        assert(isEveryItemIdUnique()); // Assert means: index replacements causes that multiple object has the same id. Wrong input!
    }


    // Calculate extent by box of the tree and the key of the node
    TBox CalculateExtent(MortonNodeIDCR nodeKey) const noexcept
    {
      auto nodeBox = TBox();

      LOOPIVDEP
      for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
        AD::SetBoxMinC(nodeBox, dimensionID, AD::GetBoxMinC(m_boxSpace, dimensionID));

      autoc depthIDOfNode = GetDepthID(nodeKey);
      autoc rasterResolutionNoAtNodeDepth = pow2(depthIDOfNode);
      autoc rasterFactorAtNodeDepth = 1.0 / static_cast<double>(rasterResolutionNoAtNodeDepth);

      autoce one = MortonGridID{ 1 };
      auto examinationKey = nodeKey;
      for (depth_t depthID = 0; depthID < depthIDOfNode; ++depthID)
      {
        autoc rasterFactorAtDepth = rasterFactorAtNodeDepth * (1 << depthID);

        LOOPIVDEP
        for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
        {
          autoc shouldApply = ((examinationKey >> dimensionID) & one) > MortonGridID{};
          AD::SetBoxMinC(
            nodeBox,
            dimensionID,
            AD::GetBoxMinC(nodeBox, dimensionID) + static_cast<TGeometry>((this->m_sizeInDimensions[dimensionID] * rasterFactorAtDepth)) * shouldApply);
        }
        examinationKey >>= DIMENSION_NO;
      }

      LOOPIVDEP
      for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
        AD::SetBoxMaxC(
          nodeBox,
          dimensionID,
          AD::GetBoxMinC(nodeBox, dimensionID) + static_cast<TGeometry>(this->m_sizeInDimensions[dimensionID] * rasterFactorAtNodeDepth));

      return nodeBox;
    }


    // Reset the tree
    void Reset() noexcept
    {
      m_nodes.clear();
      m_boxSpace = {};
      m_volumeOfOverallSpace = 0.0;
      m_rasterizerFactors = {};
    }


    // Remove all elements and ids, except Root
    void Clear() noexcept
    {
      std::erase_if(m_nodes, [](autoc& p) { return p.first != GetRootKey(); });
      cont_at(m_nodes, GetRootKey()).Entities.clear();
    }


    // Move the whole tree with a std::vector of the movement
    template<typename TExecutionPolicy = std::execution::unsequenced_policy>
    void Move(TVector const& moveVector) noexcept
    {
      auto ep = TExecutionPolicy{}; // GCC 11.3
      std::for_each(ep, m_nodes.begin(), m_nodes.end(), [&moveVector](auto& pairKeyNode) { AD::MoveBox(pairKeyNode.second.Box, moveVector); });
      AD::MoveBox(this->m_boxSpace, moveVector);
    }


    MortonNodeID FindSmallestNodeKey(MortonNodeID searchKey) const noexcept
    {
      for (; IsValidKey(searchKey); searchKey >>= DIMENSION_NO)
        if (this->m_nodes.contains(searchKey))
          return searchKey;

      return MortonNodeID{}; // Not found
    }

    MortonNodeID Find(std::size_t entityID) const noexcept
    {
      autoc it = find_if(this->m_nodes.begin(), this->m_nodes.end(), [entityID](autoc& keyAndNode) {
        return std::ranges::find(keyAndNode.second.Entities, entityID) != end(keyAndNode.second.Entities);
      });

      return it == this->m_nodes.end() ? 0 : it->first;
    }

  protected:
    struct GridBoundary
    {
      GridID MinGridID, BeginGridID, EndGridID;
    };
    template<dim_t DIMENSION_ID>
    static constexpr void constructGridIDListRecursively(
      GridID gridStepNo, DimArray<GridBoundary> const& gridIDBoundaries, DimArray<GridID>& currentGridID, std::vector<DimArray<GridID>>& allGridID) noexcept
    {
      if constexpr (DIMENSION_ID == 0)
        allGridID.emplace_back(currentGridID);
      else
      {
        autoc & [minGridID, beginGridID, endGridID] = gridIDBoundaries[DIMENSION_ID - 1];
        currentGridID[DIMENSION_ID - 1] = minGridID;
        constructGridIDListRecursively<DIMENSION_ID - 1>(gridStepNo, gridIDBoundaries, currentGridID, allGridID);
        for (auto gridID = beginGridID; gridID < endGridID; ++gridID)
        {
          currentGridID[DIMENSION_ID - 1] = gridID * gridStepNo;
          constructGridIDListRecursively<DIMENSION_ID - 1>(gridStepNo, gridIDBoundaries, currentGridID, allGridID);
        }
      }
    }


    template<typename TData, bool DO_RANGE_MUST_FULLY_CONTAIN = false, bool DO_COLLECT_ONLY_LARGER_THAN_MIN_ENTITY_ID = false>
    constexpr void rangeSearchCopy(
      TBox const& range,
      std::span<TData const> const& geometryCollection,
      Node const& parentNode,
      std::vector<std::size_t>& foundEntities,
      std::size_t minEntityID = 0) const noexcept
    {
      for (autoc entityID : parentNode.Entities)
      {
        if constexpr (std::is_same<TData, TBox>::value)
        {
          if constexpr (DO_COLLECT_ONLY_LARGER_THAN_MIN_ENTITY_ID)
          {
            if (entityID <= minEntityID)
              continue;

            bool fAdd = false;
            if constexpr (DO_RANGE_MUST_FULLY_CONTAIN)
              fAdd = AD::AreBoxesOverlapped(range, geometryCollection[entityID], true);
            else
              fAdd = AD::AreBoxesOverlappedStrict(range, geometryCollection[entityID]);

            if (fAdd)
              foundEntities.emplace_back(entityID);
          }
          else
          {
            bool fAdd = false;
            if constexpr (DO_RANGE_MUST_FULLY_CONTAIN)
              fAdd = AD::AreBoxesOverlapped(range, geometryCollection[entityID], DO_RANGE_MUST_FULLY_CONTAIN);
            else
              fAdd = AD::AreBoxesOverlappedStrict(range, geometryCollection[entityID]);

            if (fAdd)
              foundEntities.emplace_back(entityID);
          }
        }
        else
        {
          if (AD::DoesBoxContainPoint(range, geometryCollection[entityID]))
            foundEntities.emplace_back(entityID);
        }
      }
    }


    template<typename TData, bool DO_RANGE_MUST_FULLY_CONTAIN = false, bool DO_COLLECT_ONLY_LARGER_THAN_MIN_ENTITY_ID = false>
    void rangeSearch(
      TBox const& range,
      std::span<TData const> const& geometryCollection,
      double rangeVolume,
      double parentNodeVolume,
      Node const& currentNode,
      std::vector<std::size_t>& foundEntities,
      std::size_t minEntityID = 0) const noexcept
    {
      rangeSearchCopy<TData, DO_RANGE_MUST_FULLY_CONTAIN, DO_COLLECT_ONLY_LARGER_THAN_MIN_ENTITY_ID>(
        range, geometryCollection, currentNode, foundEntities, minEntityID);

      autoc currentNodeVolume = parentNodeVolume / this->CHILD_NO;
      for (MortonNodeIDCR keyChild : currentNode.GetChildren())
      {
        autoc& childNode = this->GetNode(keyChild);

        auto isOverlapped = true;
        for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO && isOverlapped; ++dimensionID)
        {
          autoc isUpperNodeInTheDimension = IsValidKey(keyChild & (MortonNodeID{ 1 } << dimensionID));
          if (isUpperNodeInTheDimension)
            isOverlapped &= AD::GetBoxMinC(childNode.Box, dimensionID) <= AD::GetBoxMaxC(range, dimensionID);
          else
            isOverlapped &= AD::GetBoxMaxC(childNode.Box, dimensionID) >= AD::GetBoxMinC(range, dimensionID);
        }
        if (!isOverlapped)
          continue;

        if (rangeVolume >= currentNodeVolume && AD::AreBoxesOverlapped(range, childNode.Box))
          collectAllIdInDFS<DO_COLLECT_ONLY_LARGER_THAN_MIN_ENTITY_ID>(childNode, foundEntities, minEntityID);
        else
          rangeSearch<TData, DO_RANGE_MUST_FULLY_CONTAIN, DO_COLLECT_ONLY_LARGER_THAN_MIN_ENTITY_ID>(
            range, geometryCollection, rangeVolume, currentNodeVolume, childNode, foundEntities, minEntityID);
      }
    }

    template<
      typename TData,
      bool DO_RANGE_MUST_FULLY_CONTAIN = false,
      bool DO_COLLECT_ONLY_LARGER_THAN_MIN_ENTITY_ID = false,
      bool DOES_LEAF_NODE_CONTAIN_ELEMENT_ONLY = true,
      bool IS_BOX_TYPE = false>
    bool rangeSearchRoot(
      TBox const& range, std::span<TData const> const& geometryCollection, std::vector<std::size_t>& foundEntities, std::size_t minEntityID = 0) const noexcept
    {
      autoc entityNo = geometryCollection.size();
      if (AD::AreBoxesOverlapped(range, this->m_boxSpace))
      {
        foundEntities.resize(DO_COLLECT_ONLY_LARGER_THAN_MIN_ENTITY_ID ? entityNo - minEntityID - 1 : entityNo);
        std::iota(foundEntities.begin(), foundEntities.end(), DO_COLLECT_ONLY_LARGER_THAN_MIN_ENTITY_ID ? minEntityID + 1 : 0);
        return entityNo > 0;
      }

      // If the range has zero volume, it could stuck at any node comparison with point/side touch. It is eliminated to work node bounding box independently.
      for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
        if (AD::GetBoxMinC(range, dimensionID) >= AD::GetBoxMaxC(range, dimensionID))
          return false;

      autoc[minGridID, maxGridID] = this->getGridIdBox<!IS_BOX_TYPE>(range);
      auto minLocationID = MortonEncode(minGridID);
      autoc maxLocationID = MortonEncode(maxGridID);

      auto depthNo = this->m_maxDepthNo;
      for (auto diffLocationFlag = minLocationID ^ maxLocationID; IsValidKey(diffLocationFlag); diffLocationFlag >>= DIMENSION_NO, --depthNo)
        minLocationID >>= DIMENSION_NO;

      autoc rangeKey = this->GetHash(depthNo, minLocationID);
      auto smallestNodeKey = this->FindSmallestNodeKey(rangeKey);
      if (!IsValidKey(smallestNodeKey))
        return false;

      auto rangeVolume = 1.0;
      for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
        rangeVolume *= AD::GetBoxMaxC(range, dimensionID) - AD::GetBoxMinC(range, dimensionID);

      autoc nodeVolume = this->m_volumeOfOverallSpace / static_cast<double>(1 << (DIMENSION_NO * depthNo));

      autoc foundEntityNoEstimation =
        this->m_volumeOfOverallSpace < 0.01 ? 10 : static_cast<std::size_t>((rangeVolume * entityNo) / this->m_volumeOfOverallSpace);
      foundEntities.reserve(foundEntityNoEstimation);
      autoc& node = this->GetNode(smallestNodeKey);
      rangeSearch<TData, DO_RANGE_MUST_FULLY_CONTAIN, DO_COLLECT_ONLY_LARGER_THAN_MIN_ENTITY_ID>(
        range, geometryCollection, rangeVolume, nodeVolume, node, foundEntities, minEntityID);

      if constexpr (!DOES_LEAF_NODE_CONTAIN_ELEMENT_ONLY)
      {
        for (smallestNodeKey >>= DIMENSION_NO; IsValidKey(smallestNodeKey); smallestNodeKey >>= DIMENSION_NO)
          rangeSearchCopy<TData, DO_RANGE_MUST_FULLY_CONTAIN, DO_COLLECT_ONLY_LARGER_THAN_MIN_ENTITY_ID>(
            range, geometryCollection, this->GetNode(smallestNodeKey), foundEntities, minEntityID);
      }

      return true;
    }

    template<typename TData>
    static PlaneRelation getEntityPlaneRelation(TData entity, TGeometry distanceOfOrigo, TVector const& planeNormal, TGeometry tolerance)
    {
      if constexpr (std::is_same<TData, TBox>::value)
        return AD::GetBoxPlaneRelation(entity, distanceOfOrigo, planeNormal, tolerance);
      else
        return AD::GetPointPlaneRelation(entity, distanceOfOrigo, planeNormal, tolerance);
    }

    template<typename TData>
    std::vector<std::size_t> planeIntersection(
      TGeometry distanceOfOrigo, TVector const& planeNormal, TGeometry tolerance, std::span<TData const> const& data) const noexcept
    {
      assert(AD::IsNormalizedVector(planeNormal));

      auto results = std::vector<std::size_t>{};
      autoc selector = [&](MortonNodeIDCR, Node const& node) -> bool {
        return AD::GetBoxPlaneRelation(node.Box, distanceOfOrigo, planeNormal, tolerance) == PlaneRelation::Hit;
      };

      autoc procedure = [&](MortonNodeIDCR, Node const& node) {
        for (autoc entityID : node.Entities)
          if (getEntityPlaneRelation(data[entityID], distanceOfOrigo, planeNormal, tolerance) == PlaneRelation::Hit)
            if (std::find(results.begin(), results.end(), entityID) == results.end())
              results.emplace_back(entityID);
      };

      this->VisitNodesInDFS(GetRootKey(), procedure, selector);

      return results;
    }

    template<typename TData>
    std::vector<std::size_t> planePositiveSegmentation(
      TGeometry const& distanceOfOrigo, TVector const& planeNormal, TGeometry tolerance, std::span<TData const> const& data) const noexcept
    {
      assert(AD::IsNormalizedVector(planeNormal));

      auto results = std::vector<std::size_t>{};
      autoc selector = [&](MortonNodeIDCR, Node const& node) -> bool {
        autoc relation = AD::GetBoxPlaneRelation(node.Box, distanceOfOrigo, planeNormal, tolerance);
        return relation != PlaneRelation::Negative;
      };

      autoc procedure = [&](MortonNodeIDCR, Node const& node) {
        for (autoc entityID : node.Entities)
        {
          autoc relation = getEntityPlaneRelation<TData>(data[entityID], distanceOfOrigo, planeNormal, tolerance);
          if (relation == PlaneRelation::Negative)
            continue;

          if (std::find(results.begin(), results.end(), entityID) == results.end())
            results.emplace_back(entityID);
        }
      };

      this->VisitNodesInDFS(GetRootKey(), procedure, selector);

      return results;
    }

    // Get all entities which relation is positive or intersected by the given space boundary planes
    template<typename TData>
    std::vector<std::size_t> frustumCulling(std::span<TPlane const> const& boundaryPlanes, TGeometry tolerance, std::span<TData const> const& data) const noexcept
    {
      auto results = std::vector<std::size_t>{};
      if (boundaryPlanes.empty())
        return results;

      assert(std::all_of(boundaryPlanes.begin(), boundaryPlanes.end(), [](autoc& plane) { return AD::IsNormalizedVector(AD::GetPlaneNormal(plane)); }));

      autoc selector = [&](MortonNodeIDCR, Node const& node) -> bool {
        for (autoc& plane : boundaryPlanes)
        {
          autoc relation = AD::GetBoxPlaneRelation(node.Box, AD::GetPlaneOrigoDistance(plane), AD::GetPlaneNormal(plane), tolerance);
          if (relation == PlaneRelation::Hit)
            return true;

          if (relation == PlaneRelation::Negative)
            return false;
        }
        return true;
      };

      autoc procedure = [&](MortonNodeIDCR, Node const& node) {
        for (autoc entityID : node.Entities)
        {
          auto relation = PlaneRelation::Negative;
          for (autoc& plane : boundaryPlanes)
          {
            relation = getEntityPlaneRelation<TData>(data[entityID], AD::GetPlaneOrigoDistance(plane), AD::GetPlaneNormal(plane), tolerance);
            if (relation != PlaneRelation::Positive)
              break;
          }

          if (relation == PlaneRelation::Negative)
            continue;

          if (std::find(results.begin(), results.end(), entityID) == results.end())
            results.emplace_back(entityID);
        }
      };

      this->VisitNodesInDFS(GetRootKey(), procedure, selector);

      return results;
    }
  };


  // OrthoTreePoint: Non-owning container which spatially organize point ids in N dimension space into a hash-table by Morton Z order.
  template<
    dim_t DIMENSION_NO,
    typename TVector_,
    typename TBox_,
    typename TRay_,
    typename TPlane_,
    typename TGeometry_ = double,
    typename TAdapter = AdaptorGeneral<DIMENSION_NO, TVector_, TBox_, TRay_, TPlane_, TGeometry_>>
  class OrthoTreePoint final : public OrthoTreeBase<DIMENSION_NO, TVector_, TBox_, TRay_, TPlane_, TGeometry_, TAdapter>
  {
  protected:
    using Base = OrthoTreeBase<DIMENSION_NO, TVector_, TBox_, TRay_, TPlane_, TGeometry_, TAdapter>;
    using EntityDistance = typename Base::EntityDistance;
    using BoxDistance = typename Base::BoxDistance;

  public:
    using AD = typename Base::AD;
    using MortonGridID = typename Base::MortonGridID;
    using MortonGridIDCR = typename Base::MortonGridIDCR;
    using MortonNodeID = typename Base::MortonNodeID;
    using MortonNodeIDCR = typename Base::MortonNodeIDCR;
    using ChildID = typename Base::ChildID;

    using Node = typename Base::Node;

    using TGeometry = TGeometry_;
    using TVector = TVector_;
    using TBox = TBox_;
    using TRay = TRay_;
    using TPlane = TPlane_;

    static constexpr std::size_t DEFAULT_MAX_ELEMENT = 21;

  private: // Aid functions
    struct Location
    {
      std::size_t EntityID;
      MortonGridID GridID;
    };

    using LocationIterator = typename std::vector<Location>::iterator;
    void addNodes(
      Node& parentNode,
      MortonNodeIDCR parentKey,
      LocationIterator& locationBeginIterator,
      LocationIterator const& locationEndIterator,
      MortonGridIDCR gridID,
      depth_t remainingDepth) noexcept
    {
      std::size_t const elementNo = std::distance(locationBeginIterator, locationEndIterator);
      if (elementNo < this->m_maxElementNo || remainingDepth == 0)
      {
        parentNode.Entities.resize(elementNo);
        std::transform(locationBeginIterator, locationEndIterator, parentNode.Entities.begin(), [](autoc& item) { return item.EntityID; });
        locationBeginIterator = locationEndIterator;
        return;
      }

      --remainingDepth;
      autoc shift = remainingDepth * DIMENSION_NO;
      autoc stepNo = MortonGridID{ 1 } << shift;
      autoc parentKeyFlag = parentKey << DIMENSION_NO;

      while (locationBeginIterator != locationEndIterator)
      {
        autoc actualChildID = Base::convertMortonIdToChildId((locationBeginIterator->GridID - gridID) >> shift);
        autoc actualEndIterator = std::partition_point(locationBeginIterator, locationEndIterator, [&](autoc& location) {
          return actualChildID == Base::convertMortonIdToChildId((location.GridID - gridID) >> shift);
        });

        autoc actualChildGridID = MortonGridID(actualChildID);
        MortonGridID const childKey = parentKeyFlag | actualChildGridID;
        MortonGridID const beginChildGridID = gridID + actualChildGridID * stepNo;

        auto& childNode = this->createChild(parentNode, actualChildID, childKey);
        this->addNodes(childNode, childKey, locationBeginIterator, actualEndIterator, beginChildGridID, remainingDepth);
      }
    }


  public: // Create
    // Ctors
    OrthoTreePoint() = default;
    OrthoTreePoint(
      std::span<TVector const> const& points,
      std::optional<depth_t> maxDepthNoIn = std::nullopt,
      std::optional<TBox> const& boxSpaceOptional = std::nullopt,
      std::size_t maxElementNoInNode = DEFAULT_MAX_ELEMENT) noexcept
    {
      Create(*this, points, maxDepthNoIn, boxSpaceOptional, maxElementNoInNode);
    }

    // Create
    template<typename TExecutionPolicy = std::execution::unsequenced_policy>
    static void Create(
      OrthoTreePoint& tree,
      std::span<TVector const> const& points,
      std::optional<depth_t> maxDepthNoIn = std::nullopt,
      std::optional<TBox> const& boxSpaceOptional = std::nullopt,
      std::size_t maxElementNoInNode = DEFAULT_MAX_ELEMENT) noexcept
    {
      autoc boxSpace = boxSpaceOptional.has_value() ? *boxSpaceOptional : AD::GetBoxOfPoints(points);
      autoc pointNo = points.size();

      autoc maxDepthNo = (!maxDepthNoIn || maxDepthNoIn == 0) ? Base::EstimateMaxDepth(pointNo, maxElementNoInNode) : *maxDepthNoIn;
      tree.Init(boxSpace, maxDepthNo, maxElementNoInNode);
      Base::reserveContainer(tree.m_nodes, Base::EstimateNodeNumber(pointNo, maxDepthNo, maxElementNoInNode));
      if (points.empty())
        return;

      auto pointLocations = std::vector<Location>(pointNo);
      auto ept = TExecutionPolicy{}; // GCC 11.3 only accept in this form
      std::transform(ept, points.begin(), points.end(), pointLocations.begin(), [&](autoc& point) {
        std::size_t const id = std::distance(&points[0], &point);
        return Location{ id, tree.getLocationID(point) };
      });

      auto eps = TExecutionPolicy{}; // GCC 11.3 only accept in this form
      std::sort(eps, pointLocations.begin(), pointLocations.end(), [&](autoc& leftLocation, autoc& rightLocation) {
        return leftLocation.GridID < rightLocation.GridID;
      });

      autoc rootKey = Base::GetRootKey();
      auto& nodeRoot = cont_at(tree.m_nodes, rootKey);

      auto beginIterator = pointLocations.begin();
      tree.addNodes(nodeRoot, rootKey, beginIterator, pointLocations.end(), MortonNodeID{ 0 }, maxDepthNo);
    }


  public: // Edit functions
    // Insert item into a node. If doInsertToLeaf is true: The smallest node will be chosen by the max depth. If doInsertToLeaf is false: The smallest existing level on the branch will be chosen.
    bool Insert(std::size_t entityID, TVector const& point, bool doInsertToLeaf = false) noexcept
    {
      if (!AD::DoesBoxContainPoint(this->m_boxSpace, point))
        return false;

      autoc smallestNodeKey = FindSmallestNode(point);
      if (!Base::IsValidKey(smallestNodeKey))
        return false;

      autoc idLocation = this->getLocationID(point);
      autoc nodeKey = this->GetHash(this->m_maxDepthNo, idLocation);

      return this->template insert<true>(nodeKey, smallestNodeKey, entityID, doInsertToLeaf);
    }

    // Erase an id. Traverse all node if it is needed, which has major performance penalty.
    template<bool DO_UPDATE_ENTITY_IDS = true>
    constexpr bool EraseId(std::size_t entityID) noexcept
    {
      autoc isErased = std::ranges::any_of(this->m_nodes, [&](auto& pairNode) { return erase(pairNode.second.Entities, entityID); });
      if (!isErased)
        return false;

      if constexpr (DO_UPDATE_ENTITY_IDS)
      {
        std::ranges::for_each(this->m_nodes, [entityID](auto& pairNode) {
          for (auto& id : pairNode.second.Entities)
            id -= entityID < id;
        });
      }

      return true;
    }

    // Erase id, aided with the original point
    template<bool DO_UPDATE_ENTITY_IDS = true>
    bool Erase(std::size_t entitiyID, TVector const& entityOriginalPoint) noexcept
    {
      autoc oldKey = FindSmallestNode(entityOriginalPoint);
      if (!Base::IsValidKey(oldKey))
        return false; // old box is not in the handled space domain

      auto& idList = cont_at(this->m_nodes, oldKey).Entities;
      autoc endIteratorAfterRemove = std::remove(idList.begin(), idList.end(), entitiyID);
      if (endIteratorAfterRemove == idList.end())
        return false; // id was not registered previously.

      idList.erase(endIteratorAfterRemove, idList.end());

      if constexpr (DO_UPDATE_ENTITY_IDS)
      {
        std::ranges::for_each(this->m_nodes, [entitiyID](auto& pairNode) {
          for (auto& id : pairNode.second.Entities)
            id -= entitiyID < id;
        });
      }

      return true;
    }


    // Update id by the new point information
    bool Update(std::size_t entityID, TVector const& newPoint, bool doesInsertToLeaf = false) noexcept
    {
      if (!AD::DoesBoxContainPoint(this->m_boxSpace, newPoint))
        return false;

      if (!this->EraseId<false>(entityID))
        return false;

      return this->Insert(entityID, newPoint, doesInsertToLeaf);
    }


    // Update id by the new point information and the erase part is aided by the old point geometry data
    bool Update(std::size_t entityID, TVector const& oldPoint, TVector const& newPoint, bool doesInsertToLeaf = false) noexcept
    {
      if (!AD::DoesBoxContainPoint(this->m_boxSpace, newPoint))
        return false;

      if (!this->Erase<false>(entityID, oldPoint))
        return false;

      return this->Insert(entityID, newPoint, doesInsertToLeaf);
    }


  public: // Search functions
    // Find smallest node which contains the box
    MortonNodeID FindSmallestNode(TVector const& searchPoint) const noexcept
    {
      if (!AD::DoesBoxContainPoint(this->m_boxSpace, searchPoint))
        return MortonNodeID{};

      autoc locationID = this->getLocationID(searchPoint);
      return this->FindSmallestNodeKey(this->GetHash(this->m_maxDepthNo, locationID));
    }

    bool Contains(TVector const& searchPoint, std::span<TVector const> const& points, TGeometry tolerance) const noexcept
    {
      autoc smallestNodeKey = this->FindSmallestNode(searchPoint);
      if (!Base::IsValidKey(smallestNodeKey))
        return false;

      autoc& node = this->GetNode(smallestNodeKey);
      return std::ranges::any_of(node.Entities, [&](autoc& entityID) { return AD::ArePointsEqual(searchPoint, points[entityID], tolerance); });
    }

    // Range search
    template<bool DOES_LEAF_NODE_CONTAIN_ELEMENT_ONLY = false>
    std::vector<std::size_t> RangeSearch(TBox const& range, std::span<TVector const> const& points) const noexcept
    {
      auto foundEntityIDs = std::vector<std::size_t>();

      if (!this->template rangeSearchRoot<TVector, false, false, DOES_LEAF_NODE_CONTAIN_ELEMENT_ONLY, false>(range, points, foundEntityIDs))
        return {};

      return foundEntityIDs;
    }

    // Hyperplane intersection (Plane equation: dotProduct(planeNormal, point) = distanceOfOrigo)
    inline std::vector<std::size_t> PlaneSearch(
      TGeometry const& distanceOfOrigo, TVector const& planeNormal, TGeometry tolerance, std::span<TVector const> const& points) const noexcept
    {
      return this->template planeIntersection<TVector>(distanceOfOrigo, planeNormal, tolerance, points);
    }

    // Hyperplane intersection using built-in plane
    inline std::vector<std::size_t> PlaneSearch(TPlane const& plane, TGeometry tolerance, std::span<TVector const> const& points) const noexcept
    {
      return this->template planeIntersection<TVector>(AD::GetPlaneOrigoDistance(plane), AD::GetPlaneNormal(plane), tolerance, points);
    }

    // Hyperplane segmentation, get all elements in positive side (Plane equation: dotProduct(planeNormal, point) = distanceOfOrigo)
    inline std::vector<std::size_t> PlanePositiveSegmentation(
      TGeometry distanceOfOrigo, TVector const& planeNormal, TGeometry tolerance, std::span<TVector const> const& points) const noexcept
    {
      return this->template planePositiveSegmentation<TVector>(distanceOfOrigo, planeNormal, tolerance, points);
    }

    // Hyperplane segmentation, get all elements in positive side (Plane equation: dotProduct(planeNormal, point) = distanceOfOrigo)
    inline std::vector<std::size_t> PlanePositiveSegmentation(TPlane const& plane, TGeometry tolerance, std::span<TVector const> const& points) const noexcept
    {
      return this->template planePositiveSegmentation<TVector>(AD::GetPlaneOrigoDistance(plane), AD::GetPlaneNormal(plane), tolerance, points);
    }

    // Hyperplane segmentation, get all elements in positive side (Plane equation: dotProduct(planeNormal, point) = distanceOfOrigo)
    inline std::vector<std::size_t> FrustumCulling(
      std::span<TPlane const> const& boundaryPlanes, TGeometry tolerance, std::span<TVector const> const& points) const noexcept
    {
      return this->template frustumCulling<TVector>(boundaryPlanes, tolerance, points);
    }


  private: // K Nearest Neighbor helpers
    static TGeometry getMinBoxWallDistance(TVector const& point, TBox const& box) noexcept
    {
      auto distances = std::vector<TGeometry>();
      distances.reserve(DIMENSION_NO);
      for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
      {
        autoc actualDistance = distances.emplace_back(std::min(
          std::abs(AD::GetPointC(point, dimensionID) - AD::GetBoxMinC(box, dimensionID)),
          std::abs(AD::GetPointC(point, dimensionID) - AD::GetBoxMaxC(box, dimensionID))));

        if (actualDistance == 0)
          return 0.0;
      }

      return *std::ranges::min_element(distances);
    }


    static void createEntityDistance(
      Node const& node, TVector const& searchPoint, std::span<TVector const> const& points, std::multiset<EntityDistance>& neighborEntities) noexcept
    {
      for (autoc id : node.Entities)
        neighborEntities.insert({ { AD::Distance(searchPoint, points[id]) }, id });
    }

    static TGeometry getFarestDistance(std::multiset<EntityDistance>& neighborEntities, std::size_t neighborNo) noexcept
    {
      if (neighborEntities.size() < neighborNo)
        return std::numeric_limits<TGeometry>::max();

      return std::next(neighborEntities.begin(), neighborNo - 1)->Distance;
    }

    static std::vector<std::size_t> convertEntityDistanceToList(std::multiset<EntityDistance>& neighborEntities, std::size_t neighborNo) noexcept
    {
      autoc entityNo = std::min(neighborNo, neighborEntities.size());
      auto entityIDs = std::vector<std::size_t>(entityNo);
      std::transform(neighborEntities.begin(), std::next(neighborEntities.begin(), entityNo), entityIDs.begin(), [](autoc& ed) { return ed.EntityID; });
      return entityIDs;
    }

  public:
    // K Nearest Neighbor
    std::vector<std::size_t> GetNearestNeighbors(TVector const& searchPoint, std::size_t neighborNo, std::span<TVector const> const& points) const noexcept
    {
      auto neighborEntities = std::multiset<EntityDistance>();
      autoc smallestNodeKey = FindSmallestNode(searchPoint);
      if (Base::IsValidKey(smallestNodeKey))
      {
        autoc& smallestNode = this->GetNode(smallestNodeKey);
        autoc wallDistance = getMinBoxWallDistance(searchPoint, smallestNode.Box);
        createEntityDistance(smallestNode, searchPoint, points, neighborEntities);
        if (!smallestNode.IsAnyChildExist())
          if (getFarestDistance(neighborEntities, neighborNo) < wallDistance)
            return convertEntityDistanceToList(neighborEntities, neighborNo);
      }

      auto nodeMinDistances = std::multiset<BoxDistance>();
      std::ranges::for_each(this->m_nodes, [&](autoc& pairOfKeyAndNode) {
        autoc & [key, node] = pairOfKeyAndNode;
        if (node.Entities.empty() || key == smallestNodeKey)
          return;

        auto aDist = TVector{};
        for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
        {
          autoc dMin = AD::GetBoxMinC(node.Box, dimensionID) - AD::GetPointC(searchPoint, dimensionID);
          autoc dMax = AD::GetBoxMaxC(node.Box, dimensionID) - AD::GetPointC(searchPoint, dimensionID);

          // If point projection in dimensionID is within min and max the wall distance should be calculated.
          AD::SetPointC(aDist, dimensionID, dMin * dMax < 0 ? 0 : std::min(std::abs(dMin), std::abs(dMax)));
        }
        nodeMinDistances.insert({ { AD::Size(aDist) }, key, node });
      });

      if (!nodeMinDistances.empty())
      {
        auto rLatestNodeDist = getFarestDistance(neighborEntities, neighborNo);
        for (autoc& nodeDist : nodeMinDistances)
        {
          autoc n = neighborEntities.size();
          if (neighborNo <= n && rLatestNodeDist < nodeDist.Distance)
            break;

          createEntityDistance(nodeDist.NodeReference, searchPoint, points, neighborEntities);
          rLatestNodeDist = getFarestDistance(neighborEntities, neighborNo);
        }
      }

      return convertEntityDistanceToList(neighborEntities, neighborNo);
    }
  };


  // OrthoTreeBoundingBox: Non-owning container which spatially organize bounding box ids in N dimension space into a hash-table by Morton Z order.
  // SPLIT_DEPTH_INCREASEMENT: if (SPLIT_DEPTH_INCREASEMENT > 0) Those items which are not fit in the child nodes may be stored in the children/grand-children instead of the parent.
  template<
    dim_t DIMENSION_NO,
    typename TVector_,
    typename TBox_,
    typename TRay_,
    typename TPlane_,
    typename TGeometry_ = double,
    depth_t SPLIT_DEPTH_INCREASEMENT = 2,
    typename TAdapter = AdaptorGeneral<DIMENSION_NO, TVector_, TBox_, TRay_, TPlane_, TGeometry_>>
  class OrthoTreeBoundingBox final : public OrthoTreeBase<DIMENSION_NO, TVector_, TBox_, TRay_, TPlane_, TGeometry_, TAdapter>
  {
  protected:
    using Base = OrthoTreeBase<DIMENSION_NO, TVector_, TBox_, TRay_, TPlane_, TGeometry_, TAdapter>;
    using EntityDistance = typename Base::EntityDistance;
    using BoxDistance = typename Base::BoxDistance;
    using GridBoundary = typename Base::GridBoundary;
    template<typename T>
    using DimArray = std::array<T, DIMENSION_NO>;

  public:
    using AD = typename Base::AD;
    using MortonGridID = typename Base::MortonGridID;
    using MortonGridIDCR = typename Base::MortonGridIDCR;
    using MortonNodeID = typename Base::MortonNodeID;
    using MortonNodeIDCR = typename Base::MortonNodeIDCR;
    using ChildID = typename Base::ChildID;

    using Node = typename Base::Node;

    using TGeometry = TGeometry_;
    using TVector = TVector_;
    using TBox = TBox_;
    using TRay = TRay_;
    using TPlane = TPlane_;

    static constexpr std::size_t DEFAULT_MAX_ELEMENT = 21;

  private: // Aid functions
    struct Location
    {
      std::size_t EntityID;
      MortonGridID MinGridID;
      depth_t DepthID;

      constexpr auto operator<(Location const& rightLocation) const
      {
        if (DepthID == rightLocation.DepthID)
          return MinGridID < rightLocation.MinGridID;
        else if (DepthID < rightLocation.DepthID)
        {
          autoc shift = (rightLocation.DepthID - DepthID) * DIMENSION_NO;
          autoc minGridIDOfRightOnTheSameDepth = rightLocation.MinGridID >> shift;
          if (MinGridID == minGridIDOfRightOnTheSameDepth)
            return true;

          return MinGridID < minGridIDOfRightOnTheSameDepth;
        }
        else
        {
          autoc shift = (DepthID - rightLocation.DepthID) * DIMENSION_NO;
          autoc minGridIDOfLeftOnTheSameDepth = MinGridID >> shift;
          if (rightLocation.MinGridID == minGridIDOfLeftOnTheSameDepth)
            return false;

          return minGridIDOfLeftOnTheSameDepth < rightLocation.MinGridID;
        }
      }
    };

    using LocationContainer = std::vector<Location>;
    using LocationIterator = typename LocationContainer::iterator;

    static constexpr ChildID getChildIDAtDepth(Location const& location, depth_t depthID, MortonNodeIDCR nodeIDOnCurrentDepth) noexcept
    {
      assert(depthID <= location.DepthID);
      autoc gridIDOnCurrentDepth = location.MinGridID >> ((location.DepthID - depthID) * DIMENSION_NO);
      return Base::convertMortonIdToChildId(gridIDOnCurrentDepth - nodeIDOnCurrentDepth);
    }


    void addNodes(
      Node& parentNode,
      MortonNodeIDCR parentKey,
      LocationIterator& beginLocationIterator,
      LocationIterator const& endLocationIterator,
      MortonGridIDCR firstLocationID,
      depth_t remainingDepthNo) noexcept
    {
      std::size_t const elementNo = std::distance(beginLocationIterator, endLocationIterator);
      if (elementNo < this->m_maxElementNo || remainingDepthNo == 0)
      {
        if (elementNo == 0)
          return;

        parentNode.Entities.resize(elementNo);
        std::transform(beginLocationIterator, endLocationIterator, parentNode.Entities.begin(), [](autoc& item) { return item.EntityID; });
        beginLocationIterator = endLocationIterator;
        return;
      }

      depth_t currentDepthID = this->m_maxDepthNo - remainingDepthNo;
      if (beginLocationIterator->DepthID == currentDepthID)
      {
        auto it = beginLocationIterator;
        beginLocationIterator = std::partition_point(it, endLocationIterator, [&](autoc& idLocation) { return idLocation.DepthID == it->DepthID; });
        autoc nElementCur = static_cast<int>(std::distance(it, beginLocationIterator));

        parentNode.Entities.resize(nElementCur);
        std::transform(it, beginLocationIterator, parentNode.Entities.begin(), [](autoc& item) { return item.EntityID; });
      }

      ++currentDepthID;
      --remainingDepthNo;

      autoc shift = remainingDepthNo * DIMENSION_NO;
      autoc locationStepNo = MortonGridID{ 1 } << shift;
      autoc parentFlag = parentKey << DIMENSION_NO;
      autoc currentDepthLocationID = firstLocationID >> shift;

      while (beginLocationIterator != endLocationIterator)
      {
        autoc actualChildID = getChildIDAtDepth(*beginLocationIterator, currentDepthID, currentDepthLocationID);
        autoc actualEndLocationIterator = std::partition_point(beginLocationIterator, endLocationIterator, [&](autoc& location) {
          autoc childID = getChildIDAtDepth(location, currentDepthID, currentDepthLocationID);
          return actualChildID == childID;
        });

        autoc actualChildID_ = MortonGridID(actualChildID);
        MortonGridID const childKey = parentFlag | actualChildID_;
        MortonGridID const firstChildLocationID = firstLocationID + actualChildID_ * locationStepNo;

        auto& nodeChild = this->createChild(parentNode, actualChildID, childKey);
        this->addNodes(nodeChild, childKey, beginLocationIterator, actualEndLocationIterator, firstChildLocationID, remainingDepthNo);
      }
    }


    void split(
      std::array<DimArray<GridID>, 2> const& boxMinMaxGridID,
      std::size_t entityID,
      LocationContainer& locations,
      LocationContainer* additionalLocations) const noexcept
    {
      depth_t depthID = locations[entityID].DepthID + SPLIT_DEPTH_INCREASEMENT;
      if (depthID > this->m_maxDepthNo)
        depthID = this->m_maxDepthNo;

      autoc remainingDepthNo = static_cast<depth_t>(this->m_maxDepthNo - depthID);
      autoc gridStepNo = static_cast<GridID>(pow2(remainingDepthNo));

      auto gridBoundaries = DimArray<GridBoundary>{};
      std::size_t boxNoByGrid = 1;
      for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
      {
        GridID const firstGridSplit = (boxMinMaxGridID[0][dimensionID] / gridStepNo) + GridID{ 1 };
        GridID const lastGridSplit = (boxMinMaxGridID[1][dimensionID] / gridStepNo);
        GridID const gridIDNo = (lastGridSplit < firstGridSplit ? 0 : (lastGridSplit - firstGridSplit + 1)) + 1;
        boxNoByGrid *= gridIDNo;
        if (boxNoByGrid >= this->CHILD_NO)
          return;

        gridBoundaries[dimensionID] = { boxMinMaxGridID[0][dimensionID], firstGridSplit, lastGridSplit + 1 };
      }

      auto gridIDs = std::vector<DimArray<GridID>>{};
      gridIDs.reserve(boxNoByGrid);
      auto temporaryGridID = DimArray<GridID>{};
      Base::template constructGridIDListRecursively<DIMENSION_NO>(gridStepNo, gridBoundaries, temporaryGridID, gridIDs);

      autoc boxNo = gridIDs.size();
      autoc shift = remainingDepthNo * DIMENSION_NO;


      // First element into entityID
      locations[entityID].DepthID = depthID;
      locations[entityID].MinGridID = Base::MortonEncode(gridIDs[0]) >> shift;

      std::size_t locationNo = 0;
      autoc additionalBoxNo = boxNo - 1;
      if (additionalLocations)
      {
        additionalLocations->resize(boxNo - 1);
      }
      else
      {
        locationNo = locations.size();
        locations.resize(locationNo + additionalBoxNo);
        additionalLocations = &locations;
      }

      LOOPIVDEP
      for (std::size_t iBox = 0; iBox < additionalBoxNo; ++iBox)
      {
        auto& location = additionalLocations->at(locationNo + iBox);
        location.EntityID = entityID;
        location.DepthID = depthID;
        location.MinGridID = Base::MortonEncode(gridIDs[iBox + 1]) >> shift;
      }
    }


    void setLocation(TBox const& box, std::size_t entityID, LocationContainer& locations, std::vector<LocationContainer>* additionalLocations = nullptr) const noexcept
    {
      autoc boxMinMaxGridID = this->getGridIdBox(box);

      auto& location = locations[entityID];
      location.EntityID = entityID;
      location.DepthID = this->m_maxDepthNo;

      location.MinGridID = Base::MortonEncode(boxMinMaxGridID[0]);
      autoc maxGridID = Base::MortonEncode(boxMinMaxGridID[1]);
      if (location.MinGridID == maxGridID)
        return;

      autoc minGridID = location.MinGridID;
      for (auto flagDiffOfLocation = location.MinGridID ^ maxGridID; Base::IsValidKey(flagDiffOfLocation);
           flagDiffOfLocation >>= DIMENSION_NO, --location.DepthID)
        location.MinGridID >>= DIMENSION_NO;

      if constexpr (SPLIT_DEPTH_INCREASEMENT > 0)
      {
        autoc remainingDepthNo = this->m_maxDepthNo - location.DepthID;
        if (Base::IsValidKey((maxGridID - minGridID) >> (remainingDepthNo * DIMENSION_NO - 1)))
          return; // all nodes are touched, it is leaved.

        this->split(boxMinMaxGridID, entityID, locations, additionalLocations ? &additionalLocations->at(entityID) : nullptr);
      }
    }

  public: // Create
    // Ctors
    OrthoTreeBoundingBox() = default;
    OrthoTreeBoundingBox(
      std::span<TBox const> const& boxes,
      std::optional<depth_t> maxDepthNo = std::nullopt,
      std::optional<TBox> const& oBoxSpace = std::nullopt,
      std::size_t nElementMaxInNode = DEFAULT_MAX_ELEMENT) noexcept
    {
      Create(*this, boxes, maxDepthNo, oBoxSpace, nElementMaxInNode);
    }

    // Create
    template<typename TExecutionPolicy = std::execution::unsequenced_policy>
    static void Create(
      OrthoTreeBoundingBox& tree,
      std::span<TBox const> const& boxes,
      std::optional<depth_t> maxDepthIn = std::nullopt,
      std::optional<TBox> const& boxSpaceOptional = std::nullopt,
      std::size_t maxElementNoInNode = DEFAULT_MAX_ELEMENT) noexcept
    {
      autoc boxSpace = boxSpaceOptional.has_value() ? *boxSpaceOptional : AD::GetBoxOfBoxes(boxes);
      autoc entityNo = boxes.size();
      autoc maxDepthNo = (!maxDepthIn || maxDepthIn == 0) ? Base::EstimateMaxDepth(entityNo, maxElementNoInNode) : *maxDepthIn;
      tree.Init(boxSpace, maxDepthNo, maxElementNoInNode);

      Base::reserveContainer(tree.m_nodes, Base::EstimateNodeNumber(entityNo, maxDepthNo, maxElementNoInNode));
      if (entityNo == 0)
        return;

      autoc rootKey = Base::GetRootKey();
      auto& nodeRoot = cont_at(tree.m_nodes, rootKey);

      autoce isNoSplit = SPLIT_DEPTH_INCREASEMENT == 0;
      autoce isNonParallel = std::is_same<TExecutionPolicy, std::execution::unsequenced_policy>::value ||
                             std::is_same<TExecutionPolicy, std::execution::sequenced_policy>::value;

      auto epf = TExecutionPolicy{}; // GCC 11.3
      auto locations = LocationContainer(entityNo);
      locations.reserve(isNoSplit ? entityNo : (entityNo * std::min<std::size_t>(10, Base::CHILD_NO * SPLIT_DEPTH_INCREASEMENT)));
      if constexpr (isNoSplit || isNonParallel)
      {
        std::for_each(epf, boxes.begin(), boxes.end(), [&tree, &boxes, &locations](autoc& box) {
          std::size_t const entityID = std::distance(&boxes[0], &box);
          tree.setLocation(box, entityID, locations);
        });
      }
      else
      {
        auto additionalLocations = std::vector<LocationContainer>(entityNo);
        std::for_each(epf, boxes.begin(), boxes.end(), [&tree, &boxes, &locations, &additionalLocations](autoc& box) {
          std::size_t const entityID = std::distance(&boxes[0], &box);
          tree.setLocation(box, entityID, locations, &additionalLocations);
        });

        auto additionalLocationSizes = std::vector<std::size_t>(entityNo);
        auto epe = TExecutionPolicy{};
        std::transform_exclusive_scan(
          epe, additionalLocations.begin(), additionalLocations.end(), additionalLocationSizes.begin(), entityNo, std::plus<std::size_t>(), [](autoc& adds) {
            return adds.size();
          });

        locations.resize(additionalLocationSizes.back() + additionalLocations.back().size());
        auto epf2 = TExecutionPolicy{}; // GCC 11.3
        std::for_each(
          epf2, additionalLocations.begin(), additionalLocations.end(), [&locations, &additionalLocationSizes, &additionalLocations](auto& additionalLocation) {
            if (additionalLocation.empty())
              return;

            std::size_t const entityID = std::distance(&additionalLocations[0], &additionalLocation);
            std::copy(additionalLocation.begin(), additionalLocation.end(), std::next(locations.begin(), additionalLocationSizes[entityID]));
          });
      }

      auto eps = TExecutionPolicy{}; // GCC 11.3
      std::sort(eps, locations.begin(), locations.end());

      auto beginLocationIterator = locations.begin();
      tree.addNodes(nodeRoot, rootKey, beginLocationIterator, locations.end(), MortonNodeID{ 0 }, maxDepthNo);
      if constexpr (SPLIT_DEPTH_INCREASEMENT > 0)
      {
        // Eliminate duplicates. Not all sub-nodes will be created due to the maxElementNoInNode, which cause duplicates in the parent nodes.
        auto epsp = TExecutionPolicy{}; // GCC 11.3
        std::for_each(epsp, tree.m_nodes.begin(), tree.m_nodes.end(), [](auto& pairOfKeyAndNode) {
          auto& node = pairOfKeyAndNode.second;
          std::ranges::sort(node.Entities);
          node.Entities.erase(std::unique(node.Entities.begin(), node.Entities.end()), node.Entities.end());
        });
      }
    }


  public: // Edit functions
    // Find smallest node which contains the box by grid id description
    MortonNodeID FindSmallestNode(std::array<DimArray<GridID>, 2> const& entityMinMaxGridID) const noexcept
    {
      auto minLocationID = Base::MortonEncode(entityMinMaxGridID[0]);
      auto maxLocationID = Base::MortonEncode(entityMinMaxGridID[1]);

      auto depthNo = this->m_maxDepthNo;
      for (auto locationDiffFlag = minLocationID ^ maxLocationID; Base::IsValidKey(locationDiffFlag); locationDiffFlag >>= DIMENSION_NO, --depthNo)
        minLocationID >>= DIMENSION_NO;

      autoc endIterator = this->m_nodes.end();
      for (auto smallestNodeKey = this->GetHash(depthNo, minLocationID); Base::IsValidKey(smallestNodeKey); smallestNodeKey >>= DIMENSION_NO)
        if (this->m_nodes.find(smallestNodeKey) != endIterator)
          return smallestNodeKey;

      return MortonNodeID{}; // Not found
    }


    // Find smallest node which contains the box
    MortonNodeID FindSmallestNode(TBox const& box) const noexcept
    {
      if (!AD::AreBoxesOverlapped(this->m_boxSpace, box))
        return MortonNodeID{};

      return FindSmallestNode(this->getGridIdBox(box));
    }


    // Insert item into a node. If doInsertToLeaf is true: The smallest node will be chosen by the max depth. If doInsertToLeaf is false: The smallest existing level on the branch will be chosen.
    bool Insert(std::size_t entityID, TBox const& box, bool doInsertToLeaf = false) noexcept
    {
      if (!AD::AreBoxesOverlapped(this->m_boxSpace, box))
        return false;

      autoc smallestNodeKey = FindSmallestNode(box);
      if (!Base::IsValidKey(smallestNodeKey))
        return false; // new box is not in the handled space domain

      auto locations = std::vector<Location>(1);
      setLocation(box, 0, locations);

      for (autoc& location : locations)
      {
        autoc nodeKey = this->GetHash(location.DepthID, location.MinGridID);
        if (!this->template insert<SPLIT_DEPTH_INCREASEMENT == 0>(nodeKey, smallestNodeKey, entityID, doInsertToLeaf))
          return false;
      }

      return true;
    }


  private:
    bool doErase(MortonNodeIDCR nodeKey, std::size_t entityID) noexcept
    {
      auto& idList = cont_at(this->m_nodes, nodeKey).Entities;
      autoc endIteratorAfterRemove = std::remove(idList.begin(), idList.end(), entityID);
      if (endIteratorAfterRemove == idList.end())
        return false; // id was not registered previously.

      idList.erase(endIteratorAfterRemove, idList.end());
      return true;
    }


    template<depth_t REMAINING_DEPTH>
    bool doEraseRec(MortonNodeIDCR nodeKey, std::size_t entityID) noexcept
    {
      auto ret = this->doErase(nodeKey, entityID);
      if constexpr (REMAINING_DEPTH > 0)
      {
        autoc& node = this->GetNode(nodeKey);
        for (MortonNodeIDCR childKey : node.GetChildren())
          ret |= doEraseRec<REMAINING_DEPTH - 1>(childKey, entityID);
      }
      return ret;
    }


  public:
    // Erase id, aided with the original bounding box
    template<bool DO_UPDATE_ENTITY_IDS = true>
    bool Erase(std::size_t entityIDToErase, TBox const& box) noexcept
    {
      autoc smallestNodeKey = FindSmallestNode(box);
      if (!Base::IsValidKey(smallestNodeKey))
        return false; // old box is not in the handled space domain

      if (doEraseRec<SPLIT_DEPTH_INCREASEMENT>(smallestNodeKey, entityIDToErase))
      {
        if constexpr (DO_UPDATE_ENTITY_IDS)
          std::ranges::for_each(this->m_nodes, [&](auto& pairNode) {
            for (auto& entityID : pairNode.second.Entities)
              entityID -= entityIDToErase < entityID;
          });

        return true;
      }
      else
        return false;
    }


    // Erase an id. Traverse all node if it is needed, which has major performance penalty.
    template<bool DO_UPDATE_ENTITY_IDS = true>
    constexpr bool EraseId(std::size_t idErase) noexcept
    {
      bool bErased = false;
      if constexpr (SPLIT_DEPTH_INCREASEMENT == 0)
        bErased = std::ranges::any_of(this->m_nodes, [&](auto& pairNode) { return erase(pairNode.second.Entities, idErase); });
      else
        std::ranges::for_each(this->m_nodes, [&](auto& pairNode) { bErased |= erase(pairNode.second.Entities, idErase) > 0; });

      if (!bErased)
        return false;

      if constexpr (DO_UPDATE_ENTITY_IDS)
        std::ranges::for_each(this->m_nodes, [&](auto& pairNode) {
          for (auto& id : pairNode.second.Entities)
            id -= idErase < id;
        });

      return true;
    }


    // Update id by the new bounding box information
    bool Update(std::size_t entityID, TBox const& boxNew, bool doInsertToLeaf = false) noexcept
    {
      if (!AD::AreBoxesOverlapped(this->m_boxSpace, boxNew))
        return false;

      if (!this->EraseId<false>(entityID))
        return false;

      return this->Insert(entityID, boxNew, doInsertToLeaf);
    }


    // Update id by the new point information and the erase part is aided by the old bounding box geometry data
    bool Update(std::size_t entityID, TBox const& oldBox, TBox const& newBox, bool doInsertToLeaf = false) noexcept
    {
      if (!AD::AreBoxesOverlapped(this->m_boxSpace, newBox))
        return false;

      if constexpr (SPLIT_DEPTH_INCREASEMENT == 0)
        if (FindSmallestNode(oldBox) == FindSmallestNode(newBox))
          return true;

      if (!this->Erase<false>(entityID, oldBox))
        return false; // entityID was not registered previously.

      return this->Insert(entityID, newBox, doInsertToLeaf);
    }


  private:
    constexpr std::array<DimArray<GridID>, 2> getGridIdPointEdge(TVector const& point) const noexcept
    {
      auto pointMinMaxGridID = std::array<DimArray<GridID>, 2>{};
      for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
      {
        autoc rasterID = static_cast<double>(AD::GetPointC(point, dimensionID) - AD::GetBoxMinC(this->m_boxSpace, dimensionID)) *
                         this->m_rasterizerFactors[dimensionID];
        pointMinMaxGridID[0][dimensionID] = pointMinMaxGridID[1][dimensionID] = static_cast<GridID>(rasterID);
        pointMinMaxGridID[0][dimensionID] -= (pointMinMaxGridID[0][dimensionID] > 0) && (floor(rasterID) == rasterID);
      }
      return pointMinMaxGridID;
    }


    void pickSearch(TVector const& pickPoint, std::span<TBox const> const& boxes, Node const& parentNode, std::vector<std::size_t>& foundEntitiyIDs) const noexcept
    {
      std::ranges::copy_if(parentNode.Entities, back_inserter(foundEntitiyIDs), [&](autoc id) { return AD::DoesBoxContainPoint(boxes[id], pickPoint); });

      for (MortonNodeIDCR keyChild : parentNode.GetChildren())
      {
        autoc& childNode = this->GetNode(keyChild);

        if (!AD::DoesBoxContainPoint(childNode.Box, pickPoint))
          continue;

        pickSearch(pickPoint, boxes, childNode, foundEntitiyIDs);
      }
    }


  public: // Search functions
    // Pick search
    std::vector<std::size_t> PickSearch(TVector const& pickPoint, std::span<TBox const> const& boxes) const noexcept
    {
      auto foundEntitiyIDs = std::vector<std::size_t>();
      if (!AD::DoesBoxContainPoint(this->m_boxSpace, pickPoint))
        return foundEntitiyIDs;

      foundEntitiyIDs.reserve(100);

      autoc endIteratorOfNodes = this->m_nodes.end();
      autoc[minGridID, maxGridID] = this->getGridIdPointEdge(pickPoint);
      auto locationID = Base::MortonEncode(minGridID);

      auto nodeKey = this->GetHash(this->m_maxDepthNo, locationID);
      if (minGridID != maxGridID) // Pick point is on the nodes edge. It must check more nodes downward.
      {
        autoc maxLocationID = Base::MortonEncode(maxGridID);
        auto depthID = this->m_maxDepthNo;
        for (auto locationDiffFlag = locationID ^ maxLocationID; Base::IsValidKey(locationDiffFlag); locationDiffFlag >>= DIMENSION_NO, --depthID)
          locationID >>= DIMENSION_NO;

        autoc rangeKey = this->GetHash(depthID, locationID);
        nodeKey = this->FindSmallestNodeKey(rangeKey);
        autoc nodeIterator = this->m_nodes.find(nodeKey);
        if (nodeIterator != endIteratorOfNodes)
          pickSearch(pickPoint, boxes, nodeIterator->second, foundEntitiyIDs);

        nodeKey >>= DIMENSION_NO;
      }

      for (; Base::IsValidKey(nodeKey); nodeKey >>= DIMENSION_NO)
      {
        autoc nodeIterator = this->m_nodes.find(nodeKey);
        if (nodeIterator == endIteratorOfNodes)
          continue;

        std::ranges::copy_if(nodeIterator->second.Entities, std::back_inserter(foundEntitiyIDs), [&](autoc entityID) {
          return AD::DoesBoxContainPoint(boxes[entityID], pickPoint);
        });
      }

      return foundEntitiyIDs;
    }


    // Range search
    template<bool DO_MUST_FULLY_CONTAIN = true>
    std::vector<std::size_t> RangeSearch(TBox const& range, std::span<TBox const> const& boxes) const noexcept
    {
      auto foundEntities = std::vector<std::size_t>();

      if (!this->template rangeSearchRoot<TBox, DO_MUST_FULLY_CONTAIN, false, false, true>(range, boxes, foundEntities))
        return {};

      if constexpr (SPLIT_DEPTH_INCREASEMENT > 0)
      {
        std::ranges::sort(foundEntities);
        autoc itEnd = std::unique(foundEntities.begin(), foundEntities.end());
        foundEntities.erase(itEnd, foundEntities.end());
      }

      return foundEntities;
    }

    // Hyperplane intersection (Plane equation: dotProduct(planeNormal, point) = distanceOfOrigo)
    inline std::vector<std::size_t> PlaneIntersection(
      TGeometry const& distanceOfOrigo, TVector const& planeNormal, TGeometry tolerance, std::span<TBox const> const& boxes) const noexcept
    {
      return this->template planeIntersection<TBox>(distanceOfOrigo, planeNormal, tolerance, boxes);
    }

    // Hyperplane intersection using built-in plane
    inline std::vector<std::size_t> PlaneIntersection(TPlane const& plane, TGeometry tolerance, std::span<TBox const> const& boxes) const noexcept
    {
      return this->template planeIntersection<TBox>(AD::GetPlaneOrigoDistance(plane), AD::GetPlaneNormal(plane), tolerance, boxes);
    }

    // Hyperplane segmentation, get all elements in positive side (Plane equation: dotProduct(planeNormal, point) = distanceOfOrigo)
    inline std::vector<std::size_t> PlanePositiveSegmentation(
      TGeometry distanceOfOrigo, TVector const& planeNormal, TGeometry tolerance, std::span<TBox const> const& boxes) const noexcept
    {
      return this->template planePositiveSegmentation<TBox>(distanceOfOrigo, planeNormal, tolerance, boxes);
    }

    // Hyperplane segmentation, get all elements in positive side (Plane equation: dotProduct(planeNormal, point) = distanceOfOrigo)
    inline std::vector<std::size_t> PlanePositiveSegmentation(TPlane const& plane, TGeometry tolerance, std::span<TBox const> const& boxes) const noexcept
    {
      return this->template planePositiveSegmentation<TBox>(AD::GetPlaneOrigoDistance(plane), AD::GetPlaneNormal(plane), tolerance, boxes);
    }

    // Hyperplane segmentation, get all elements in positive side (Plane equation: dotProduct(planeNormal, point) = distanceOfOrigo)
    inline std::vector<std::size_t> FrustumCulling(
      std::span<TPlane const> const& boundaryPlanes, TGeometry tolerance, std::span<TBox const> const& boxes) const noexcept
    {
      return this->template frustumCulling<TBox>(boundaryPlanes, tolerance, boxes);
    }


    // Client-defined Collision detector (params: id1, e1, id2, e2). It supplemented with the box intersection, the Client should not add.
    using FCollisionDetector = std::function<bool(std::size_t, TBox const&, std::size_t, TBox const&)>;

    // Collision detection: Returns all overlapping boxes from the source trees.
    static std::vector<std::pair<std::size_t, std::size_t>> CollisionDetection(
      OrthoTreeBoundingBox const& leftTree,
      std::span<TBox const> const& leftBoxes,
      OrthoTreeBoundingBox const& rightTree,
      std::span<TBox const> const& rightBoxes) noexcept
    {
      using NodeIterator = typename Base::template UnderlyingContainer<Node>::const_iterator;
      struct NodeIteratorAndStatus
      {
        NodeIterator Iterator;
        bool IsTraversed;
      };
      using ParentIteratorArray = std::array<NodeIteratorAndStatus, 2>;

      enum : bool
      {
        Left,
        Right
      };

      auto results = std::vector<std::pair<std::size_t, std::size_t>>{};
      results.reserve(leftBoxes.size() / 10);

      autoc rootKey = Base::GetRootKey();
      autoc trees = std::array{ &leftTree, &rightTree };

      auto nodePairToProceed = std::queue<ParentIteratorArray>{};
      nodePairToProceed.push({
        NodeIteratorAndStatus{ leftTree.m_nodes.find(rootKey), false},
        NodeIteratorAndStatus{rightTree.m_nodes.find(rootKey), false}
      });
      for (; !nodePairToProceed.empty(); nodePairToProceed.pop())
      {
        autoc& parentNodePair = nodePairToProceed.front();

        // Check the current ascendant content
        {
          for (autoc leftEntityID : parentNodePair[Left].Iterator->second.Entities)
            for (autoc rightEntityID : parentNodePair[Right].Iterator->second.Entities)
              if (AD::AreBoxesOverlapped(leftBoxes[leftEntityID], rightBoxes[rightEntityID], false))
                results.emplace_back(leftEntityID, rightEntityID);
        }

        // Collect children
        auto childNodes = std::array<std::vector<NodeIteratorAndStatus>, 2>{};
        for (autoc sideID : { Left, Right })
        {
          autoc & [nodeIterator, fTraversed] = parentNodePair[sideID];
          if (fTraversed)
            continue;

          autoc childIDs = nodeIterator->second.GetChildren();
          childNodes[sideID].resize(childIDs.size());
          std::ranges::transform(childIDs, childNodes[sideID].begin(), [&](MortonNodeIDCR childKey) -> NodeIteratorAndStatus {
            return { trees[sideID]->m_nodes.find(childKey), false };
          });
        }

        // Stop condition
        if (childNodes[Left].empty() && childNodes[Right].empty())
          continue;

        // Add parent if it has any element
        for (autoc sideID : { Left, Right })
          if (!parentNodePair[sideID].Iterator->second.Entities.empty())
            childNodes[sideID].push_back({ parentNodePair[sideID].Iterator, true });

        // Cartesian product of avitChildNode left and right
        for (autoc& leftChildNode : childNodes[Left])
          for (autoc& rightChildNode : childNodes[Right])
            if (!(leftChildNode.Iterator == parentNodePair[Left].Iterator && rightChildNode.Iterator == parentNodePair[Right].Iterator))
              if (AD::AreBoxesOverlapped(leftChildNode.Iterator->second.Box, rightChildNode.Iterator->second.Box, false))
                nodePairToProceed.emplace(std::array{ leftChildNode, rightChildNode });
      }

      if constexpr (SPLIT_DEPTH_INCREASEMENT > 0)
      {
        std::ranges::sort(results);
        results.erase(std::unique(results.begin(), results.end()), results.end());
      }

      return results;
    }


    // Collision detection: Returns all overlapping boxes from the source trees.
    inline std::vector<std::pair<std::size_t, std::size_t>> CollisionDetection(
      std::span<TBox const> const& boxes, OrthoTreeBoundingBox const& otherTree, std::span<TBox const> const& otherBoxes) const noexcept
    {
      return CollisionDetection(*this, boxes, otherTree, otherBoxes);
    }

  private:
    // Collision detection between the stored elements from top to bottom logic
    template<typename TExecutionPolicy = std::execution::unsequenced_policy>
    std::vector<std::pair<std::size_t, std::size_t>> CollisionDetectionObsolete(std::span<TBox const> const& boxes) const noexcept
    {
      autoc entityNo = boxes.size();

      auto vidCheck = std::vector<std::size_t>(entityNo);
      std::iota(vidCheck.begin(), vidCheck.end(), 0);

      auto vvidCollision = std::vector<std::vector<std::size_t>>(vidCheck.size());
      auto ep = TExecutionPolicy{}; // GCC 11.3
      std::transform(ep, vidCheck.begin(), vidCheck.end(), vvidCollision.begin(), [&boxes, this](autoc idCheck) -> std::vector<std::size_t> {
        auto foundEntities = std::vector<std::size_t>();

        autoc entityNo = boxes.size();
        if (!this->template rangeSearchRoot<TBox, false, true, false, true>(boxes[idCheck], boxes, foundEntities, idCheck))
          return {};

        if constexpr (SPLIT_DEPTH_INCREASEMENT > 0)
        {
          std::ranges::sort(foundEntities);
          autoc itEnd = std::unique(foundEntities.begin(), foundEntities.end());
          foundEntities.erase(itEnd, foundEntities.end());
        }

        return foundEntities;
      });

      auto vPair = std::vector<std::pair<std::size_t, std::size_t>>{};
      if (entityNo > 10)
        vPair.reserve(entityNo / 10);

      for (autoc idCheck : vidCheck)
        for (autoc idCollide : vvidCollision[idCheck])
          vPair.emplace_back(idCheck, idCollide);

      return vPair;
    }


    // Collision detection between the stored elements from bottom to top logic
    template<typename TExecutionPolicy = std::execution::unsequenced_policy>
    std::vector<std::pair<std::size_t, std::size_t>> collisionDetection(std::span<TBox const> const& boxes, FCollisionDetector&& collisionDetector) const noexcept
    {
      using CollisionDetectionContainer = std::vector<std::pair<std::size_t, std::size_t>>;

      autoc entityNo = boxes.size();
      auto collidedEntityPairs = CollisionDetectionContainer();
      collidedEntityPairs.reserve(boxes.size());


      // SPLIT_DEPTH_INCREASEMENT version of this algorithm needs a reverse std::map
      auto entityIDNodeMap = std::vector<std::vector<MortonNodeID>>(entityNo);
      if constexpr (SPLIT_DEPTH_INCREASEMENT > 0)
      {
        std::for_each(this->m_nodes.begin(), this->m_nodes.end(), [&entityIDNodeMap](autoc& pairKeyNode) {
          autoc & [nodeKey, node] = pairKeyNode;
          for (autoc entityID : node.Entities)
            entityIDNodeMap[entityID].emplace_back(nodeKey);
        });

        auto ep = TExecutionPolicy{}; // GCC 11.3
        std::for_each(ep, entityIDNodeMap.begin(), entityIDNodeMap.end(), [](auto& keys) {
          if constexpr (Base::IS_LINEAR_TREE)
            std::ranges::sort(keys);
          else
            std::ranges::sort(keys, bitset_arithmetic_compare{});
        });
      }


      // Entities which contain all of the tree could slow the algorithm, so these are eliminated
      auto entityIDsInRoot = std::vector<std::size_t>();
      {
        autoc& nodeRoot = this->GetNode(this->GetRootKey());
        for (autoc entityID : nodeRoot.Entities)
        {
          if (AD::AreBoxesOverlapped(boxes[entityID], this->m_boxSpace))
          {
            for (auto entityIDOther = entityID + 1; entityIDOther < entityNo; ++entityIDOther)
              collidedEntityPairs.emplace_back(entityID, entityIDOther);
          }
          else
            entityIDsInRoot.emplace_back(entityID);
        }
      }

      auto ep = TExecutionPolicy{}; // GCC 11.3

      // Collision detection node-by-node without duplication
      auto collidedEntityPairsInsideNodes = std::vector<CollisionDetectionContainer>(this->m_nodes.size());
      std::transform(ep, this->m_nodes.begin(), this->m_nodes.end(), collidedEntityPairsInsideNodes.begin(), [&](autoc& pairKeyNode) -> CollisionDetectionContainer {
        auto collidedEntityPairsInsideNode = CollisionDetectionContainer{};
        autoc & [nodeKey, node] = pairKeyNode;

        autoc nodeDepthID = this->GetDepthID(nodeKey);

        autoc& entityIDs = *(nodeDepthID == 0 ? &entityIDsInRoot : &node.Entities);
        autoc entityNoInNode = entityIDs.size();

        collidedEntityPairsInsideNode.reserve(entityNoInNode);

        // Collision detection with the parents
        if (nodeDepthID > 0)
        {
          auto parentDepthID = nodeDepthID - 1;
          auto depthDifference = depth_t(1);
          for (auto parentKey = nodeKey >> DIMENSION_NO; Base::IsValidKey(parentKey); parentKey >>= DIMENSION_NO, --parentDepthID, ++depthDifference)
          {
            autoc& parentEntityIDs = *(parentDepthID == 0 ? &entityIDsInRoot : &this->GetNode(parentKey).Entities);

            if constexpr (SPLIT_DEPTH_INCREASEMENT == 0)
            {
              for (autoc entityID : entityIDs)
                for (autoc entityIDFromParent : parentEntityIDs)
                  if (collisionDetector(entityID, boxes[entityID], entityIDFromParent, boxes[entityIDFromParent]))
                    collidedEntityPairsInsideNode.emplace_back(entityID, entityIDFromParent);
            }
            else
            {
              // SPLIT_DEPTH_INCREASEMENT: entityID could occur in multiple node. This algorithm aims to check only the first occurrence's parents.

              auto entityIDsToCheckOnOtherBranch = std::vector<std::size_t>{};
              entityIDsToCheckOnOtherBranch.reserve(entityNoInNode);
              auto entityIDPairsFromOtherBranch = std::unordered_map<std::size_t, std::set<std::size_t>>{};
              for (autoc entityID : entityIDs)
              {
                autoc& currentNodeKeys = entityIDNodeMap[entityID];
                autoc currentNodeKeysNo = currentNodeKeys.size();
                if (currentNodeKeysNo == 1)
                  entityIDsToCheckOnOtherBranch.emplace_back(entityID);
                else
                {
                  if (currentNodeKeys[0] == nodeKey)
                    entityIDsToCheckOnOtherBranch.emplace_back(entityID);
                  else if (depthDifference <= SPLIT_DEPTH_INCREASEMENT)
                  {
                    auto keysOfEntitysDepth = std::vector<MortonNodeID>();
                    for (std::size_t entityKeyID = 1; entityKeyID < currentNodeKeysNo; ++entityKeyID)
                    {
                      // An earlier node is already check this level
                      {
                        auto& keyOfEntitysDepth = keysOfEntitysDepth.emplace_back(currentNodeKeys[entityKeyID - 1]);
                        autoc prevDepthID = this->GetDepthID(keyOfEntitysDepth);
                        if (prevDepthID > parentDepthID)
                          keyOfEntitysDepth >>= DIMENSION_NO * (prevDepthID - parentDepthID);

                        if (keyOfEntitysDepth == parentKey)
                          break;
                      }

                      if (currentNodeKeys[entityKeyID] != nodeKey)
                        continue;

                      // On other branch splitted boxes could conflict already
                      for (autoc entityIDFromParent : parentEntityIDs)
                      {
                        autoc& parentNodeKeys = entityIDNodeMap[entityIDFromParent];
                        autoc parentNodeKeysNo = parentNodeKeys.size();

                        for (std::size_t prevEntityKeyID = 0, parentEntityKeyID = 0;
                             prevEntityKeyID < entityKeyID && parentEntityKeyID < parentNodeKeysNo;)
                        {
                          if (parentNodeKeys[parentEntityKeyID] == currentNodeKeys[prevEntityKeyID] || (parentNodeKeys[parentEntityKeyID] == keysOfEntitysDepth[prevEntityKeyID]))
                          {
                            // Found an earlier common key
                            entityIDPairsFromOtherBranch[entityID].emplace(entityIDFromParent);
                            break;
                          }
                          else if (parentNodeKeys[parentEntityKeyID] < currentNodeKeys[prevEntityKeyID])
                            ++parentEntityKeyID;
                          else
                            ++prevEntityKeyID;
                        }
                      }

                      entityIDsToCheckOnOtherBranch.emplace_back(entityID);
                      break;
                    }
                  }
                }
              }

              if (entityIDPairsFromOtherBranch.empty())
              {
                for (autoc entityID : entityIDsToCheckOnOtherBranch)
                  for (autoc entityIDFromParent : parentEntityIDs)
                    if (collisionDetector(entityID, boxes[entityID], entityIDFromParent, boxes[entityIDFromParent]))
                      collidedEntityPairsInsideNode.emplace_back(entityID, entityIDFromParent);
              }
              else
              {
                for (autoc entityID : entityIDsToCheckOnOtherBranch)
                {
                  autoc it = entityIDPairsFromOtherBranch.find(entityID);
                  autoc areThereAnyFromOtherBranch = it != entityIDPairsFromOtherBranch.end();

                  for (autoc entityIDFromParent : parentEntityIDs)
                  {
                    autoc isAlreadyContained = areThereAnyFromOtherBranch && it->second.contains(entityIDFromParent);
                    if (!isAlreadyContained && collisionDetector(entityID, boxes[entityID], entityIDFromParent, boxes[entityIDFromParent]))
                      collidedEntityPairsInsideNode.emplace_back(entityID, entityIDFromParent);
                  }
                }
              }
            }
          }
        }


        // Collision detection inside the node
        if (entityNoInNode > 1)
        {
          for (std::size_t iEntity = 0; iEntity < entityNoInNode; ++iEntity)
          {
            autoc iEntityID = entityIDs[iEntity];
            autoc& entityKeys = entityIDNodeMap[iEntityID];
            autoc entityKeysNo = entityKeys.size();

            for (std::size_t jEntity = iEntity + 1; jEntity < entityNoInNode; ++jEntity)
            {
              autoc jEntityID = entityIDs[jEntity];
              if constexpr (SPLIT_DEPTH_INCREASEMENT == 0)
              {
                if (collisionDetector(iEntityID, boxes[iEntityID], jEntityID, boxes[jEntityID]))
                  collidedEntityPairsInsideNode.emplace_back(iEntityID, jEntityID);
              }
              else
              {
                // Same entities could collide in other nodes, but only the first occurrence should be checked
                autoc& entityKeysOfJ = entityIDNodeMap[jEntityID];
                auto isFirstCollisionCheckHappening = entityKeysNo == 1 || entityKeysOfJ.size() == 1;
                if (!isFirstCollisionCheckHappening)
                {
                  for (std::size_t iEntityKey = 0, jEntityKey = 0; iEntityKey < entityKeysNo;)
                  {
                    if (entityKeysOfJ[jEntityKey] == entityKeys[iEntityKey])
                    {
                      isFirstCollisionCheckHappening = nodeKey == entityKeys[iEntityKey];
                      break;
                    }
                    else if (entityKeysOfJ[jEntityKey] < entityKeys[iEntityKey])
                      ++jEntityKey;
                    else
                      ++iEntityKey;
                  }
                }

                if (isFirstCollisionCheckHappening)
                  if (collisionDetector(iEntityID, boxes[iEntityID], jEntityID, boxes[jEntityID]))
                    collidedEntityPairsInsideNode.emplace_back(iEntityID, jEntityID);
              }
            }
          }
        }

        return collidedEntityPairsInsideNode;
      });

      for (autoc& vidCollisionNode : collidedEntityPairsInsideNodes)
        collidedEntityPairs.insert(collidedEntityPairs.end(), vidCollisionNode.begin(), vidCollisionNode.end());

      return collidedEntityPairs;
    }

  public:
    // Collision detection between the stored elements from bottom to top logic
    template<typename TExecutionPolicy = std::execution::unsequenced_policy>
    inline std::vector<std::pair<std::size_t, std::size_t>> CollisionDetection(std::span<TBox const> const& boxes) const noexcept
    {
      return collisionDetection<TExecutionPolicy>(boxes, [](std::size_t id1, TBox const& e1, std::size_t id2, TBox const& e2) {
        return AD::AreBoxesOverlappedStrict(e1, e2);
      });
    }


    // Collision detection between the stored elements from bottom to top logic
    template<typename TExecutionPolicy = std::execution::unsequenced_policy>
    inline std::vector<std::pair<std::size_t, std::size_t>> CollisionDetection(
      std::span<TBox const> const& boxes, FCollisionDetector&& collisionDetector) const noexcept
    {
      return collisionDetection<TExecutionPolicy>(boxes, [collisionDetector](std::size_t id1, TBox const& e1, std::size_t id2, TBox const& e2) {
        return AD::AreBoxesOverlappedStrict(e1, e2) && collisionDetector(id1, e1, id2, e2);
      });
    }

  private:
    void getRayIntersectedAll(
      Node const& node,
      std::span<TBox const> const& boxes,
      TVector const& rayBasePoint,
      TVector const& rayHeading,
      TGeometry tolerance,
      TGeometry maxExaminationDistance,
      std::vector<EntityDistance>& foundEntities) const noexcept
    {
      autoc isNodeHit = AD::IsRayHit(node.Box, rayBasePoint, rayHeading, tolerance);
      if (!isNodeHit)
        return;

      for (autoc entityID : node.Entities)
      {
        autoc entityDistance = AD::IsRayHit(boxes[entityID], rayBasePoint, rayHeading, tolerance);
        if (entityDistance && (maxExaminationDistance == 0 || entityDistance.value() <= maxExaminationDistance))
          foundEntities.push_back({ { TGeometry(entityDistance.value()) }, entityID });
      }

      for (autoc childKey : node.GetChildren())
        getRayIntersectedAll(this->GetNode(childKey), boxes, rayBasePoint, rayHeading, tolerance, maxExaminationDistance, foundEntities);
    }


    void getRayIntersectedFirst(
      Node const& node,
      std::span<TBox const> const& boxes,
      TVector const& rayBasePoint,
      TVector const& rayHeading,
      TGeometry tolerance,
      std::multiset<EntityDistance>& foundEntities) const noexcept
    {
      autoc maxExaminationDistance =
        foundEntities.empty() ? std::numeric_limits<double>::infinity() : static_cast<double>(foundEntities.rbegin()->Distance);
      for (autoc entityID : node.Entities)
      {
        autoc distance = AD::IsRayHit(boxes[entityID], rayBasePoint, rayHeading, tolerance);
        if (!distance)
          continue;

        if (*distance > maxExaminationDistance)
          continue;

        foundEntities.insert({ { TGeometry(*distance) }, entityID });
      }

      auto nodeDistances = std::multiset<BoxDistance>();
      for (autoc childKey : node.GetChildren())
      {
        autoc& nodeChild = this->GetNode(childKey);
        autoc distance = AD::IsRayHit(nodeChild.Box, rayBasePoint, rayHeading, tolerance);
        if (!distance)
          continue;

        if (*distance > maxExaminationDistance)
          continue;

        nodeDistances.insert({ { static_cast<TGeometry>(distance.value()) }, childKey, nodeChild });
      }

      for (autoc& nodeDistance : nodeDistances)
        getRayIntersectedFirst(nodeDistance.NodeReference, boxes, rayBasePoint, rayHeading, tolerance, foundEntities);
    }


  public:
    // Get all box which is intersected by the ray in order
    std::vector<std::size_t> RayIntersectedAll(
      TVector const& rayBasePointPoint,
      TVector const& rayHeading,
      std::span<TBox const> const& boxes,
      TGeometry tolerance,
      TGeometry maxExaminationDistance = 0) const noexcept
    {
      auto foundEntities = std::vector<EntityDistance>();
      foundEntities.reserve(20);
      getRayIntersectedAll(this->GetNode(this->GetRootKey()), boxes, rayBasePointPoint, rayHeading, tolerance, maxExaminationDistance, foundEntities);

      autoc beginIteratorOfEntities = foundEntities.begin();
      auto endIteratorOfEntities = foundEntities.end();
      std::sort(beginIteratorOfEntities, endIteratorOfEntities);
      if constexpr (SPLIT_DEPTH_INCREASEMENT > 0)
        endIteratorOfEntities =
          std::unique(beginIteratorOfEntities, endIteratorOfEntities, [](autoc& lhs, autoc& rhs) { return lhs.EntityID == rhs.EntityID; });

      auto foundEntityIDs = std::vector<std::size_t>(std::distance(beginIteratorOfEntities, endIteratorOfEntities));
      std::transform(beginIteratorOfEntities, endIteratorOfEntities, foundEntityIDs.begin(), [](autoc& entityDistance) {
        return entityDistance.EntityID;
      });
      return foundEntityIDs;
    }


    // Get first box which is intersected by the ray
    std::optional<std::size_t> RayIntersectedFirst(
      TVector const& rayBasePoint, TVector const& rayHeading, std::span<TBox const> const& boxes, TGeometry tolerance) const noexcept
    {
      autoc& node = this->GetNode(this->GetRootKey());
      autoc distance = AD::IsRayHit(node.Box, rayBasePoint, rayHeading, tolerance);
      if (!distance)
        return std::nullopt;

      auto foundEntities = std::multiset<EntityDistance>();
      getRayIntersectedFirst(node, boxes, rayBasePoint, rayHeading, tolerance, foundEntities);
      if (foundEntities.empty())
        return std::nullopt;

      return foundEntities.begin()->EntityID;
    }
  };


  template<dim_t DIMENSION_NO, typename TGeometry = double>
  using VectorND = std::array<TGeometry, DIMENSION_NO>;

  template<dim_t DIMENSION_NO, typename TGeometry = double>
  using PointND = VectorND<DIMENSION_NO, TGeometry>;

  template<dim_t DIMENSION_NO, typename TGeometry = double>
  struct BoundingBoxND
  {
    VectorND<DIMENSION_NO, TGeometry> Min;
    VectorND<DIMENSION_NO, TGeometry> Max;
  };

  template<dim_t DIMENSION_NO, typename TGeometry = double>
  struct RayND
  {
    VectorND<DIMENSION_NO, TGeometry> Origin;
    VectorND<DIMENSION_NO, TGeometry> Direction;
  };

  template<dim_t DIMENSION_NO, typename TGeometry = double>
  struct PlaneND
  {
    TGeometry OrigoDistance;
    VectorND<DIMENSION_NO, TGeometry> Normal;
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

  template<std::size_t DIMENSION_NO, typename TGeometry = BaseGeometryType>
  using TreePointND = OrthoTree::OrthoTreePoint<
    DIMENSION_NO,
    OrthoTree::VectorND<DIMENSION_NO, TGeometry>,
    OrthoTree::BoundingBoxND<DIMENSION_NO, TGeometry>,
    OrthoTree::RayND<DIMENSION_NO, TGeometry>,
    OrthoTree::PlaneND<DIMENSION_NO, TGeometry>,
    TGeometry>;

  template<std::size_t DIMENSION_NO, uint32_t SPLIT_DEPTH_INCREASEMENT = 2, typename TGeometry = BaseGeometryType>
  using TreeBoxND = OrthoTree::OrthoTreeBoundingBox<
    DIMENSION_NO,
    OrthoTree::VectorND<DIMENSION_NO, TGeometry>,
    OrthoTree::BoundingBoxND<DIMENSION_NO, TGeometry>,
    OrthoTree::RayND<DIMENSION_NO, TGeometry>,
    OrthoTree::PlaneND<DIMENSION_NO, TGeometry>,
    TGeometry,
    SPLIT_DEPTH_INCREASEMENT>;

  // Dualtree for points
  using DualtreePoint = TreePointND<1, BaseGeometryType>;

  // Dualtree for bounding boxes
  using DualtreeBox = TreeBoxND<1, 2, BaseGeometryType>;

  // Quadtree for points
  using QuadtreePoint = TreePointND<2, BaseGeometryType>;

  // Quadtree for bounding boxes
  using QuadtreeBox = TreeBoxND<2, 2, BaseGeometryType>;

  // Octree for points
  using OctreePoint = TreePointND<3, BaseGeometryType>;

  // Octree for bounding boxes
  using OctreeBox = TreeBoxND<3, 2, BaseGeometryType>;

  // Hexatree for points
  using HexatreePoint = TreePointND<4, BaseGeometryType>;

  // Hexatree for bounding boxes
  using HexatreeBox = TreeBoxND<4, 2, BaseGeometryType>;

  // NTrees for higher dimensions
  using TreePoint16D = TreePointND<16, BaseGeometryType>;
  using TreeBox16D = TreeBoxND<16, 2, BaseGeometryType>;


  // Dualtree for bounding boxes
  template<uint32_t SPLIT_DEPTH_INCREASEMENT>
  using DualtreeBoxs = TreeBoxND<1, SPLIT_DEPTH_INCREASEMENT, BaseGeometryType>;

  // Quadtree for bounding boxes
  template<uint32_t SPLIT_DEPTH_INCREASEMENT>
  using QuadtreeBoxs = TreeBoxND<2, SPLIT_DEPTH_INCREASEMENT, BaseGeometryType>;

  // Octree for bounding boxes
  template<uint32_t SPLIT_DEPTH_INCREASEMENT>
  using OctreeBoxs = TreeBoxND<3, SPLIT_DEPTH_INCREASEMENT, BaseGeometryType>;

  // Hexatree for bounding boxes
  template<uint32_t SPLIT_DEPTH_INCREASEMENT>
  using HexatreeBoxs = TreeBoxND<4, SPLIT_DEPTH_INCREASEMENT, BaseGeometryType>;

  // NTrees for higher dimensions
  template<uint32_t SPLIT_DEPTH_INCREASEMENT>
  using TreeBox16Ds = TreeBoxND<16, SPLIT_DEPTH_INCREASEMENT, BaseGeometryType>;
} // namespace OrthoTree


#include "octree_container.h"

#ifdef undef_autoc
#undef autoc
#undef undef_autoc
#endif

#ifdef undef_autoce
#undef autoce
#undef undef_autoce
#endif

#undef LOOPIVDEP

#endif // ORTHOTREE_GUARD
