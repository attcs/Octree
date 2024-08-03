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
#ifdef ORTHOTREE_INDEX_T__INT
  using index_t = int;
#else
  using index_t = std::size_t;
#endif // ORTHOTREE_INDEX_INT


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

  namespace detail
  {
    template<typename TContainer, typename TKey>
    concept HasAt = requires(TContainer container, TKey key) { container.at(key); };

    template<typename T>
    concept HasFirst = requires(T value) { value.first; };
    template<typename T>
    concept HasSecond = requires(T value) { value.second; };

    template<typename, typename key_type = std::void_t<>>
    struct container_key_type
    {
      using type = index_t;
    };

    template<typename TContainer>
    struct container_key_type<TContainer, std::void_t<typename TContainer::key_type>>
    {
      using type = TContainer::key_type;
    };


    template<typename T, std::size_t N>
    constexpr index_t getID(std::array<T, N> const& container, T const& value)
    {
      return index_t(std::distance(&container[0], &value));
    }

    template<typename T>
    constexpr index_t getID(std::vector<T> const& container, T const& value)
    {
      return index_t(std::distance(&container[0], &value));
    }


    template<typename T>
    constexpr index_t getID(std::span<T const> const& container, T const& value)
    {
      return index_t(std::distance(&container[0], &value));
    }

    template<typename TContainer>
    constexpr typename TContainer::key_type getKeyPart(TContainer const& container, typename TContainer::value_type const& value)
      requires(HasFirst<typename TContainer::value_type>)
    {
      return value.first;
    }

    template<typename TContainer>
    constexpr index_t getKeyPart(TContainer const& container, typename TContainer::value_type const& value)
      requires(std::contiguous_iterator<typename TContainer::iterator>)
    {
      return index_t(std::distance(&container[0], &value));
    }

    template<typename T>
    constexpr const auto& getValuePart(T const& value)
      requires(HasSecond<T>)
    {
      return value.second;
    }

    template<typename value_type>
    constexpr const auto& getValuePart(value_type const& value)
    {
      return value;
    }

    template<typename value_type, typename entity_type>
    constexpr void setValuePart(value_type& value, entity_type const& entity)
      requires(HasSecond<value_type>)
    {
      value.second = entity;
    }

    template<typename value_type, typename entity_type>
    constexpr void setValuePart(value_type& value, entity_type const& entity)
    {
      value = entity;
    }


    template<typename TContainer, typename TKey>
    constexpr const auto& at(TContainer const& container, TKey key)
      requires(HasAt<TContainer, TKey>)
    {
      return container.at(key);
    }

    template<typename TContainer, typename TKey>
    constexpr const auto& at(TContainer const& continer, TKey key)
    {
      return continer[key];
    }

    template<typename TContainer, typename TKey, typename TValue>
    constexpr void set(TContainer& container, TKey key, TValue const& value)
      requires(HasAt<TContainer, TKey>)
    {
      container.at(key) = value;
    }

    template<typename TContainer, typename TKey, typename TValue>
    constexpr void set(TContainer& continer, TKey key, TValue const& value)
    {
      continer[key] = value;
    }
  } // namespace detail

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
        TAdapter::GetRayBoxDistance(box, rayBasePoint, rayHeading, tolerance)
      } -> std::convertible_to<std::optional<double>>;
    } && requires(TBox const& box, TRay const& ray, TGeometry tolerance) {
      {
        TAdapter::GetRayBoxDistance(box, ray, tolerance)
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

    static TBox GetBoxOfBoxes(std::span<TBox const> const& boxes) noexcept
    {
      auto ext = BoxInvertedInit();
      for (autoc& e : boxes)
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

    static constexpr std::optional<double> GetRayBoxDistance(TBox const& box, TVector const& rayBasePoint, TVector const& rayHeading, TGeometry tolerance) noexcept
    {
      if (DoesBoxContainPoint(box, rayBasePoint, tolerance))
        return 0.0;

      autoce inf = std::numeric_limits<double>::infinity();

      auto minBoxDistances = std::array<double, DIMENSION_NO>{};
      auto maxBoxDistances = std::array<double, DIMENSION_NO>{};
      for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
      {
        autoc dirComp = Base::GetPointC(rayHeading, dimensionID);
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
          autoc minBox = Base::GetBoxMinC(box, dimensionID) - tolerance;
          autoc maxBox = Base::GetBoxMaxC(box, dimensionID) + tolerance;
          autoc pointComp = Base::GetPointC(rayBasePoint, dimensionID);
          autoc dirCompRecip = 1.0 / dirComp;
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

      autoc minBoxDistance = *std::ranges::max_element(minBoxDistances);
      autoc maxBoxDistance = *std::ranges::min_element(maxBoxDistances);
      if (minBoxDistance > maxBoxDistance || maxBoxDistance < 0.0)
        return std::nullopt;
      else
        return minBoxDistance < 0 ? maxBoxDistance : minBoxDistance;
    }

    static constexpr std::optional<double> GetRayBoxDistance(TBox const& box, TRay const& ray, TGeometry tolerance) noexcept
    {
      return GetRayBoxDistance(box, Base::GetRayOrigin(ray), Base::GetRayDirection(ray), tolerance);
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


  // OrthoTrees

  // OrthoTree: Non-owning Base container which spatially organize data ids in N dimension space into a hash-table by Morton Z order.
  template<
    dim_t DIMENSION_NO,
    typename TEntity_,
    typename TVector_,
    typename TBox_,
    typename TRay_,
    typename TPlane_,
    typename TGeometry_ = double,
    typename TAdapter = AdaptorGeneral<DIMENSION_NO, TVector_, TBox_, TRay_, TPlane_, TGeometry_>,
    typename TContainer_ = std::span<TEntity_ const>>
  class OrthoTreeBase
  {
  public:
    static autoce IS_LINEAR_TREE = DIMENSION_NO < 15;
    static autoce IS_BOX_TYPE = std::is_same<TEntity_, TBox_>::value;
    static autoce IS_CONTIGOUS_CONTAINER = std::contiguous_iterator<typename TContainer_::iterator>;

    // Max value: 2 ^ DIMENSION_NO
    using ChildID = std::conditional_t < DIMENSION_NO<4, uint32_t, uint64_t>;

    // Max value: 2 ^ nDepth ^ DIMENSION_NO * 2 (signal bit)
    using LinearMortonGridID = ChildID;
    using NonLinearMortonGridID = bitset_arithmetic<DIMENSION_NO * 4 + 1>;
    using MortonGridID = typename std::conditional_t<IS_LINEAR_TREE, LinearMortonGridID, NonLinearMortonGridID>;

    using MortonNodeID = MortonGridID; // same as the MortonGridID, but depth is signed by a sentinel bit.
    using MortonGridIDCR = typename std::conditional_t<IS_LINEAR_TREE, MortonNodeID const, MortonNodeID const&>;
    using MortonNodeIDCR = MortonGridIDCR;
    using TGeometry = TGeometry_;
    using TVector = TVector_;
    using TBox = TBox_;
    using TRay = TRay_;
    using TPlane = TPlane_;
    using TContainer = TContainer_;
    using TEntityID = detail::container_key_type<TContainer>::type;
    using TEntity = TEntity_;

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
      TBox m_box = {};

    public: // Public members
      std::vector<TEntityID> Entities = {};

    public:
      constexpr void SetBox(TBox const& box) noexcept { m_box = box; }
      constexpr void SetBox(TBox&& box) noexcept { m_box = std::move(box); }
      constexpr TBox const& GetBoxInternal() const noexcept { return m_box; }
      constexpr TBox const& GetBox() const noexcept { return m_box; }

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

      constexpr void RemoveChild(MortonNodeIDCR childKey) noexcept
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
      TEntityID EntityID;
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
    using LinearUnderlyingContainer = std::unordered_map<MortonNodeID, TData>;

    template<typename TData>
    using NonLinearUnderlyingContainer = std::map<MortonNodeID, TData, bitset_arithmetic_compare>;

    template<typename TData>
    using UnderlyingContainer = typename std::conditional<IS_LINEAR_TREE, LinearUnderlyingContainer<TData>, NonLinearUnderlyingContainer<TData>>::type;

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
    static inline ChildID castMortonIdToChildId(NonLinearMortonGridID const& bs) noexcept
    {
      assert(bs <= NonLinearMortonGridID(std::numeric_limits<std::size_t>::max()));
      return bs.to_ullong();
    }
    static constexpr ChildID castMortonIdToChildId(LinearMortonGridID morton) noexcept { return morton; }

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

    template<bool ALLOW_OUT_OF_BOX_GEOMETRY = false>
    constexpr DimArray<GridID> getGridIdPoint(TVector const& point) const noexcept
    {
      auto gridIDs = DimArray<GridID>{};
      for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
      {
        auto pointComponent = AD::GetPointC(point, dimensionID) - AD::GetBoxMinC(this->m_boxSpace, dimensionID);
        if constexpr (ALLOW_OUT_OF_BOX_GEOMETRY)
        {
          if (pointComponent < 0.0)
            pointComponent = 0.0;
        }
        else
        {
          assert(pointComponent >= 0.0);
        }

        autoc rasterID = GridID(pointComponent * this->m_rasterizerFactors[dimensionID]);
        gridIDs[dimensionID] = std::min<GridID>(this->m_maxRasterID, rasterID);
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

        TBox childBox;
        LOOPIVDEP
        for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
        {
          AD::SetBoxMinC(childBox, dimensionID, static_cast<TGeometry>(minNodePoint[dimensionID]));
          AD::SetBoxMaxC(childBox, dimensionID, static_cast<TGeometry>(maxNodePoint[dimensionID]));
        }

        nodeChild.SetBox(std::move(childBox));
      }
      else
      {
        TBox childBox;
        LOOPIVDEP
        for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
        {
          autoc isGreater = ((ChildID{ 1 } << dimensionID) & childID) > 0;
          AD::SetBoxMinC(
            childBox,
            dimensionID,
            isGreater * (AD::GetBoxMaxC(parentNode.GetBoxInternal(), dimensionID) + AD::GetBoxMinC(parentNode.GetBoxInternal(), dimensionID)) *
                TGeometry{ 0.5 } +
              (!isGreater) * AD::GetBoxMinC(parentNode.GetBoxInternal(), dimensionID));

          AD::SetBoxMaxC(
            childBox,
            dimensionID,
            isGreater * AD::GetBoxMaxC(parentNode.GetBoxInternal(), dimensionID) +
              (!isGreater) * ((AD::GetBoxMaxC(parentNode.GetBoxInternal(), dimensionID) + AD::GetBoxMinC(parentNode.GetBoxInternal(), dimensionID)) *
                              TGeometry{ 0.5 }));
        }
        nodeChild.SetBox(std::move(childBox));
      }
      return nodeChild;
    }

    template<bool ALLOW_OUT_OF_BOX_GEOMETRY = false>
    constexpr MortonGridID getLocationID(TVector const& point) const noexcept
    {
      return MortonEncode(this->getGridIdPoint<ALLOW_OUT_OF_BOX_GEOMETRY>(point));
    }

    template<bool ALLOW_OUT_OF_BOX_GEOMETRY = false>
    constexpr std::tuple<depth_t, MortonGridID> getDepthAndLocationID(TVector const& point) const noexcept
    {
      return { this->m_maxDepthNo, this->getLocationID<ALLOW_OUT_OF_BOX_GEOMETRY>(point) };
    }

    template<bool ALLOW_OUT_OF_BOX_GEOMETRY = false>
    constexpr std::tuple<depth_t, MortonGridID> getDepthAndLocationID(TBox const& box) const noexcept
    {
      autoc entityMinMaxGridID = this->getGridIdBox<ALLOW_OUT_OF_BOX_GEOMETRY>(box);
      auto minLocationID = MortonEncode(entityMinMaxGridID[0]);
      autoc maxLocationID = MortonEncode(entityMinMaxGridID[1]);

      auto depthNo = this->m_maxDepthNo;
      for (auto locationDiffFlag = minLocationID ^ maxLocationID; IsValidKey(locationDiffFlag); locationDiffFlag >>= DIMENSION_NO, --depthNo)
        minLocationID >>= DIMENSION_NO;

      return { depthNo, minLocationID };
    }

    static inline TBox getBoxInvertedInit() noexcept
    {
      auto ext = TBox{};

      LOOPIVDEP
      for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
      {
        AD::SetBoxMinC(ext, dimensionID, std::numeric_limits<TGeometry>::max());
        AD::SetBoxMaxC(ext, dimensionID, std::numeric_limits<TGeometry>::lowest());
      }

      return ext;
    }

    bool isEveryItemIdUnique() const noexcept
    {
      auto ids = std::vector<TEntityID>();
      ids.reserve(100);
      std::ranges::for_each(m_nodes, [&](auto& node) { ids.insert(ids.end(), node.second.Entities.begin(), node.second.Entities.end()); });

      std::ranges::sort(ids);
      autoc itEndUnique = std::unique(ids.begin(), ids.end());
      return itEndUnique == ids.end();
    }

    static constexpr ChildID getChildIDByDepth(depth_t parentDepth, depth_t childDepth, MortonNodeIDCR childNodeKey)
    {
      autoc depthDifference = childDepth - parentDepth;
      assert(depthDifference > 0);
      return getChildPartOfLocation(childNodeKey >> (DIMENSION_NO * (depthDifference - 1)));
    }

    template<bool DO_UNIQUENESS_CHECK_TO_INDICIES>
    bool insertWithRebalancing(
      MortonNodeIDCR parentNodeKey,
      depth_t parentDepth,
      MortonNodeIDCR entitiyNodeKey,
      depth_t entityDepth,
      TEntityID newEntityID,
      TContainer const& geometryCollection) noexcept
    {
      auto& parentNode = this->m_nodes.at(parentNodeKey);
      autoc shouldInsertInParentNode = entitiyNodeKey == parentNodeKey;

      enum class ControlFlow
      {
        InsertInParentNode,
        ShouldCreateOnlyOneChild,
        FullRebalancing,
      };

      autoc cf = [&] {
        if (parentDepth < this->m_maxDepthNo)
        {
          autoc isParentNotLeafNode = parentNode.IsAnyChildExist();
          // If possible, entity should be placed in a leaf node, therefore if the parent node is not a leaf, a new child should be created.
          if (isParentNotLeafNode && !shouldInsertInParentNode)
            return ControlFlow::ShouldCreateOnlyOneChild;
          else if (parentNode.Entities.size() + 1 >= this->m_maxElementNo)
            return ControlFlow::FullRebalancing;
        }
        return ControlFlow::InsertInParentNode;
      }();

      switch (cf)
      {
      case ControlFlow::ShouldCreateOnlyOneChild: {
        autoc childID = getChildIDByDepth(parentDepth, entityDepth, entitiyNodeKey);
        autoc parentFlag = parentNodeKey << DIMENSION_NO;
        autoc childNodeKey = parentFlag | MortonGridID(childID);

        parentNode.AddChildInOrder(childNodeKey);
        auto& childNode = this->createChild(parentNode, childID, childNodeKey);
        childNode.Entities.emplace_back(newEntityID);

        break;
      }

      case ControlFlow::FullRebalancing: {
        autoce isPointSolution = std::is_same_v<TEntity, TVector>;
        autoc parentFlag = parentNodeKey << DIMENSION_NO;

        if (!shouldInsertInParentNode)
        {
          autoc childID = getChildIDByDepth(parentDepth, entityDepth, entitiyNodeKey);
          autoc childNodeKey = parentFlag | MortonGridID(childID);

          parentNode.AddChildInOrder(childNodeKey);
          auto& childNode = this->createChild(parentNode, childID, childNodeKey);
          childNode.Entities.emplace_back(newEntityID);
        }

        size_t remainingEntityNo = parentNode.Entities.size();
        for (size_t i = 0; i < remainingEntityNo; ++i)
        {
          auto entityID = parentNode.Entities[i];
          autoc[depthID, locationID] = this->getDepthAndLocationID(geometryCollection[entityID]);
          if (depthID <= parentDepth)
            continue;

          autoc childID = getChildIDByDepth(parentDepth, depthID, locationID);
          autoc childNodeKey = parentFlag | MortonGridID(childID);
          if (parentNode.HasChild(childNodeKey))
          {
            autoc entitiyNodeKey_ = GetHash(depthID, locationID);
            autoc[parentNodeKey_, parentDepthID_] = FindSmallestNodeKeyWithDepth(entitiyNodeKey_);
            insertWithRebalancing<false>(parentNodeKey_, parentDepthID_, entitiyNodeKey_, depthID, entityID, geometryCollection);
          }
          else
          {
            parentNode.AddChildInOrder(childNodeKey);
            auto& childNode = this->createChild(parentNode, childID, childNodeKey);
            childNode.Entities.emplace_back(entityID);
          }

          if constexpr (!isPointSolution)
          {
            --remainingEntityNo;
            parentNode.Entities[i] = parentNode.Entities[remainingEntityNo];
            --i;
          }
        }

        if constexpr (isPointSolution)
          parentNode.Entities = {}; // All reserved memory should be freed.
        else
        {
          if (shouldInsertInParentNode)
          {
            if (remainingEntityNo == parentNode.Entities.size())
              parentNode.Entities.emplace_back(newEntityID);
            else
            {
              parentNode.Entities[remainingEntityNo] = newEntityID;
              ++remainingEntityNo;
            }
          }
          parentNode.Entities.resize(remainingEntityNo);
        }

        break;
      }

      case ControlFlow::InsertInParentNode:
      default: parentNode.Entities.emplace_back(newEntityID); break;
      }


      if constexpr (DO_UNIQUENESS_CHECK_TO_INDICIES)
        assert(this->isEveryItemIdUnique()); // Assert means: index is already added. Wrong input!

      return true;
    }

    template<bool DO_UNIQUENESS_CHECK_TO_INDICIES>
    bool insertWithoutRebalancing(MortonNodeIDCR parentNodeKey, MortonNodeIDCR entityNodeKey, TEntityID entityID, bool doInsertToLeaf) noexcept
    {
      if (entityNodeKey == parentNodeKey)
      {
        cont_at(this->m_nodes, entityNodeKey).Entities.emplace_back(entityID);
        if constexpr (DO_UNIQUENESS_CHECK_TO_INDICIES)
          assert(this->isEveryItemIdUnique()); // Assert means: index is already added. Wrong input!
        return true;
      }

      if (doInsertToLeaf)
      {
        auto& newNode = this->m_nodes[entityNodeKey];
        newNode.Entities.emplace_back(entityID);
        newNode.SetBox(this->CalculateExtent(entityNodeKey));

        // Create all child between the new (entityNodeKey) and the smallest existing one (parentNodeKey)
        auto newParentNodeKey = entityNodeKey;
        do
        {
          auto childNodeKey = newParentNodeKey;
          newParentNodeKey >>= DIMENSION_NO;
          assert(IsValidKey(parentNodeKey));
          auto& newParentNode = this->m_nodes[newParentNodeKey];
          newParentNode.AddChildInOrder(childNodeKey);
          newParentNode.SetBox(this->CalculateExtent(newParentNodeKey));
        } while (newParentNodeKey != parentNodeKey);
      }
      else
      {
        auto& parentNode = this->m_nodes.at(parentNodeKey);
        if (parentNode.IsAnyChildExist())
        {
          autoc parentDepth = this->GetDepthID(parentNodeKey);
          autoc parentFlag = parentNodeKey << DIMENSION_NO;

          autoc childID = getChildIDByDepth(parentDepth, this->GetDepthID(entityNodeKey), entityNodeKey);
          autoc childNodeKey = parentFlag | MortonGridID(childID);

          parentNode.AddChildInOrder(childNodeKey);
          auto& nodeChild = this->createChild(parentNode, childID, childNodeKey);
          nodeChild.Entities.emplace_back(entityID);
        }
        else
          parentNode.Entities.emplace_back(entityID);
      }

      if constexpr (DO_UNIQUENESS_CHECK_TO_INDICIES)
        assert(this->isEveryItemIdUnique()); // Assert means: index is already added. Wrong input!

      return true;
    }

    template<typename TData>
    static void reserveContainer(NonLinearUnderlyingContainer<TData>&, std::size_t) noexcept {};

    template<typename TData>
    static void reserveContainer(LinearUnderlyingContainer<TData>& m, std::size_t n) noexcept
    {
      m.reserve(n);
    };

    void removeNodeIfPossible(MortonNodeIDCR nodeKey, Node const& node) noexcept
    {
      if (nodeKey == GetRootKey())
        return;

      if (node.IsAnyChildExist() || !node.Entities.empty())
        return;

      autoc parentKey = nodeKey >> DIMENSION_NO;
      auto& parentNode = this->m_nodes.at(parentKey);
      parentNode.RemoveChild(nodeKey);
      this->m_nodes.erase(nodeKey);
    }

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

    static constexpr bool IsValidKey(LinearMortonGridID key) noexcept { return key > 0; }

    static inline bool IsValidKey(NonLinearMortonGridID const& key) noexcept { return key.any(); }

    static constexpr depth_t GetDepthID(MortonNodeID key) noexcept
    {
      // Keep shifting off DIMENSION_NO bits at a time, increasing depth counter
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
        return castMortonIdToChildId(key & maskLastBits1);
      }
      else
      {
        auto childID = MortonNodeID{};
        for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
          childID[dimensionID] = key[dimensionID];

        return castMortonIdToChildId(childID);
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
        return { RemoveSentinelBit(nodeKey) << (maxDepthNo - GetDepthID(nodeKey)) };
      else
      {
        autoc depthID = GetDepthID(nodeKey);

        autoce mask = MortonGridID{ 1 };
        for (depth_t iDepth = maxDepthNo - depthID, shift = 0; iDepth < maxDepthNo; ++iDepth)
          for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID, ++shift)
          {
            if constexpr (IS_LINEAR_TREE)
            {
              gridID[dimensionID] |= ((nodeKey >> shift) & mask) << iDepth;
            }
            else
            {
              gridID[dimensionID] |= GridID{ nodeKey[shift] } << iDepth;
            }
          }
      }
      return gridID;
    }


  public: // Getters
    constexpr auto const& GetNodes() const noexcept { return m_nodes; }
    inline bool HasNode(MortonNodeIDCR key) const noexcept { return m_nodes.contains(key); }
    inline auto const& GetNode(MortonNodeIDCR key) const noexcept { return m_nodes.at(key); }
    constexpr auto const& GetBox() const noexcept { return m_boxSpace; }
    constexpr auto GetDepthMax() const noexcept { return m_maxDepthNo; }
    constexpr auto GetResolutionMax() const noexcept { return m_maxRasterResolution; }
    inline auto GetNodeIDByEntity(TEntityID entityID) const noexcept
    {
      autoc it = std::ranges::find_if(m_nodes, [&](autoc& keyAndValue) {
        return std::ranges::find(keyAndValue.second.Entities, entityID) != keyAndValue.second.Entities.end();
      });

      return it == m_nodes.end() ? MortonNodeID{} : it->first;
    }

  public: // Main service functions
    // Alternative creation mode (instead of Create), Init then Insert items into leafs one by one. NOT RECOMMENDED.
    constexpr void Init(TBox const& box, depth_t maxDepthNo, std::size_t maxElementNo = 11) noexcept
    {
      assert(this->m_nodes.empty()); // To build/setup/create the tree, use the Create() [recommended] or Init() function. If an already builded
                                     // tree is wanted to be reset, use the Reset() function before init.
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
      nodeRoot.SetBox(box);
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
        MortonNodeIDCR Key;
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

        for (MortonNodeIDCR childKey : node.GetChildren())
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
      for (MortonNodeIDCR childKey : node.GetChildren())
        VisitNodesInDFS(childKey, procedure, selector);
    }


    // Collect all item id, traversing the tree in breadth-first search order
    std::vector<TEntityID> CollectAllIdInBFS(MortonNodeIDCR rootKey = GetRootKey()) const noexcept
    {
      auto entityIDs = std::vector<TEntityID>();
      entityIDs.reserve(m_nodes.size() * std::max<std::size_t>(2, m_maxElementNo / 2));

      VisitNodes(rootKey, [&entityIDs](MortonNodeIDCR, autoc& node) { entityIDs.insert(entityIDs.end(), node.Entities.begin(), node.Entities.end()); });
      return entityIDs;
    }

  private:
    void collectAllIdInDFS(Node const& parentNode, std::vector<TEntityID>& foundEntities) const noexcept
    {
      foundEntities.insert(foundEntities.end(), parentNode.Entities.begin(), parentNode.Entities.end());
      for (MortonNodeIDCR childKey : parentNode.GetChildren())
        collectAllIdInDFS(this->GetNode(childKey), foundEntities);
    }

  public:
    std::vector<TEntityID> CollectAllIdInDFS(MortonGridIDCR parentKey = GetRootKey()) const noexcept
    {
      auto entityIDs = std::vector<TEntityID>{};
      collectAllIdInDFS(GetNode(parentKey), entityIDs);
      return entityIDs;
    }

    // Update all element which are in the given hash-table.
    template<typename TExecutionPolicy = std::execution::unsequenced_policy, bool DO_UNIQUENESS_CHECK_TO_INDICIES = false>
    void UpdateIndexes(std::unordered_map<TEntityID, std::optional<TEntityID>> const& updateMap) noexcept
    {
      autoc updateMapEndIterator = updateMap.end();

      autoce ep = TExecutionPolicy{};
      std::for_each(ep, m_nodes.begin(), m_nodes.end(), [&](auto& node) {
        decltype(Node::Entities) entitiesNew;
        for (autoc& id : node.second.Entities)
        {
          autoc it = updateMap.find(id);
          if (it == updateMapEndIterator)
            entitiesNew.emplace_back(id);
          else if (it->second)
            entitiesNew.emplace_back(*it->second);
        }

        node.second.Entities = std::move(entitiesNew);
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
      std::for_each(ep, m_nodes.begin(), m_nodes.end(), [&moveVector](auto& pairKeyNode) {
        auto box = pairKeyNode.second.GetBoxInternal();
        AD::MoveBox(box, moveVector);
        pairKeyNode.second.SetBox(std::move(box));
      });
      AD::MoveBox(this->m_boxSpace, moveVector);
    }

    std::tuple<MortonNodeID, depth_t> FindSmallestNodeKeyWithDepth(MortonNodeID searchKey) const noexcept
    {
      for (depth_t depthID = this->m_maxDepthNo; IsValidKey(searchKey); searchKey >>= DIMENSION_NO, --depthID)
        if (this->m_nodes.contains(searchKey))
          return { searchKey, depthID };

      return {}; // Not found
    }

    MortonNodeID FindSmallestNodeKey(MortonNodeID searchKey) const noexcept
    {
      for (; IsValidKey(searchKey); searchKey >>= DIMENSION_NO)
        if (this->m_nodes.contains(searchKey))
          return searchKey;

      return MortonNodeID{}; // Not found
    }

    // Get Node ID of a point
    template<bool ALLOW_OUT_OF_BOX_GEOMETRY = false>
    MortonNodeID GetNodeID(TVector const& searchPoint) const noexcept
    {
      autoc locationID = this->getLocationID<ALLOW_OUT_OF_BOX_GEOMETRY>(searchPoint);
      return this->GetHash(this->m_maxDepthNo, locationID);
    }

    // Get Node ID of a point
    MortonNodeID GetNodeID(TBox const& box) const noexcept
    {
      autoc[depthNo, locationID] = this->getDepthAndLocationID(box);
      return this->GetHash(depthNo, locationID);
    }

    // Find smallest node which contains the box
    template<bool ALLOW_OUT_OF_BOX_GEOMETRY = false>
    MortonNodeID FindSmallestNode(TVector const& searchPoint) const noexcept
    {
      if constexpr (!ALLOW_OUT_OF_BOX_GEOMETRY)
      {
        if (!AD::DoesBoxContainPoint(this->m_boxSpace, searchPoint))
          return MortonNodeID{};
      }
      return this->FindSmallestNodeKey(this->GetNodeID<ALLOW_OUT_OF_BOX_GEOMETRY>(searchPoint));
    }

    // Find smallest node which contains the box
    MortonNodeID FindSmallestNode(TBox const& box) const noexcept
    {
      if (!AD::AreBoxesOverlapped(this->m_boxSpace, box))
        return MortonNodeID{};

      return FindSmallestNodeKey(this->GetNodeID(box));
    }

    MortonNodeID Find(TEntityID entityID) const noexcept
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


    template<bool DO_RANGE_MUST_FULLY_CONTAIN = false>
    constexpr void rangeSearchCopy(
      TBox const& range, TContainer const& geometryCollection, Node const& parentNode, std::vector<TEntityID>& foundEntities) const noexcept
    {
      for (autoc entityID : parentNode.Entities)
      {
        bool fAdd = false;
        if constexpr (IS_BOX_TYPE)
        {
          if constexpr (DO_RANGE_MUST_FULLY_CONTAIN)
            fAdd = AD::AreBoxesOverlapped(range, detail::at(geometryCollection, entityID), DO_RANGE_MUST_FULLY_CONTAIN);
          else
            fAdd = AD::AreBoxesOverlappedStrict(range, detail::at(geometryCollection, entityID));
        }
        else
        {
          fAdd = AD::DoesBoxContainPoint(range, detail::at(geometryCollection, entityID));
        }

        if (fAdd)
          foundEntities.emplace_back(entityID);
      }
    }


    template<bool DO_RANGE_MUST_FULLY_CONTAIN = false>
    void rangeSearch(TBox const& range, TContainer const& geometryCollection, Node const& currentNode, std::vector<TEntityID>& foundEntities) const noexcept
    {
      rangeSearchCopy<DO_RANGE_MUST_FULLY_CONTAIN>(range, geometryCollection, currentNode, foundEntities);

      for (MortonNodeIDCR keyChild : currentNode.GetChildren())
      {
        autoc& childNode = this->GetNode(keyChild);

        auto isOverlapped = true;
        for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO && isOverlapped; ++dimensionID)
        {
          autoc isUpperNodeInTheDimension = IsValidKey(keyChild & (MortonNodeID{ 1 } << dimensionID));
          if (isUpperNodeInTheDimension)
            isOverlapped &= AD::GetBoxMinC(childNode.GetBoxInternal(), dimensionID) <= AD::GetBoxMaxC(range, dimensionID);
          else
            isOverlapped &= AD::GetBoxMaxC(childNode.GetBoxInternal(), dimensionID) >= AD::GetBoxMinC(range, dimensionID);
        }
        if (!isOverlapped)
          continue;

        if (AD::AreBoxesOverlapped(range, childNode.GetBoxInternal()))
          collectAllIdInDFS(childNode, foundEntities);
        else
          rangeSearch<DO_RANGE_MUST_FULLY_CONTAIN>(range, geometryCollection, childNode, foundEntities);
      }
    }

    template<bool DO_RANGE_MUST_FULLY_CONTAIN = false, bool DOES_LEAF_NODE_CONTAIN_ELEMENT_ONLY = true>
    bool rangeSearchRoot(TBox const& range, TContainer const& geometryCollection, std::vector<TEntityID>& foundEntities) const noexcept
    {
      autoc entityNo = geometryCollection.size();
      if (AD::AreBoxesOverlapped(range, this->m_boxSpace))
      {
        foundEntities.resize(entityNo);

        if constexpr (IS_CONTIGOUS_CONTAINER)
          std::iota(foundEntities.begin(), foundEntities.end(), 0);
        else
          std::transform(geometryCollection.begin(), geometryCollection.end(), foundEntities.begin(), [&geometryCollection](autoc& item) {
            return detail::getKeyPart(geometryCollection, item);
          });

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

      autoc foundEntityNoEstimation =
        this->m_volumeOfOverallSpace < 0.01 ? 10 : static_cast<std::size_t>((rangeVolume * entityNo) / this->m_volumeOfOverallSpace);

      foundEntities.reserve(foundEntityNoEstimation);
      autoc& node = this->GetNode(smallestNodeKey);
      rangeSearch<DO_RANGE_MUST_FULLY_CONTAIN>(range, geometryCollection, node, foundEntities);

      if constexpr (!DOES_LEAF_NODE_CONTAIN_ELEMENT_ONLY)
      {
        for (smallestNodeKey >>= DIMENSION_NO; IsValidKey(smallestNodeKey); smallestNodeKey >>= DIMENSION_NO)
          rangeSearchCopy<DO_RANGE_MUST_FULLY_CONTAIN>(range, geometryCollection, this->GetNode(smallestNodeKey), foundEntities);
      }

      return true;
    }

    static PlaneRelation getEntityPlaneRelation(TEntity const& entity, TGeometry distanceOfOrigo, TVector const& planeNormal, TGeometry tolerance)
    {
      if constexpr (std::is_same<TEntity, TBox>::value)
        return AD::GetBoxPlaneRelation(entity, distanceOfOrigo, planeNormal, tolerance);
      else
        return AD::GetPointPlaneRelation(entity, distanceOfOrigo, planeNormal, tolerance);
    }

    std::vector<TEntityID> planeIntersection(TGeometry distanceOfOrigo, TVector const& planeNormal, TGeometry tolerance, TContainer const& data) const noexcept
    {
      assert(AD::IsNormalizedVector(planeNormal));

      auto results = std::vector<TEntityID>{};
      autoc selector = [&](MortonNodeIDCR, Node const& node) -> bool {
        return AD::GetBoxPlaneRelation(node.GetBoxInternal(), distanceOfOrigo, planeNormal, tolerance) == PlaneRelation::Hit;
      };

      autoc procedure = [&](MortonNodeIDCR, Node const& node) {
        for (autoc entityID : node.Entities)
          if (getEntityPlaneRelation(detail::at(data, entityID), distanceOfOrigo, planeNormal, tolerance) == PlaneRelation::Hit)
            if (std::find(results.begin(), results.end(), entityID) == results.end())
              results.emplace_back(entityID);
      };

      this->VisitNodesInDFS(GetRootKey(), procedure, selector);

      return results;
    }

    std::vector<TEntityID> planePositiveSegmentation(
      TGeometry const& distanceOfOrigo, TVector const& planeNormal, TGeometry tolerance, TContainer const& data) const noexcept
    {
      assert(AD::IsNormalizedVector(planeNormal));

      auto results = std::vector<TEntityID>{};
      autoc selector = [&](MortonNodeIDCR, Node const& node) -> bool {
        autoc relation = AD::GetBoxPlaneRelation(node.GetBoxInternal(), distanceOfOrigo, planeNormal, tolerance);
        return relation != PlaneRelation::Negative;
      };

      autoc procedure = [&](MortonNodeIDCR, Node const& node) {
        for (autoc entityID : node.Entities)
        {
          autoc relation = getEntityPlaneRelation(detail::at(data, entityID), distanceOfOrigo, planeNormal, tolerance);
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
    std::vector<TEntityID> frustumCulling(std::span<TPlane const> const& boundaryPlanes, TGeometry tolerance, TContainer const& data) const noexcept
    {
      auto results = std::vector<TEntityID>{};
      if (boundaryPlanes.empty())
        return results;

      assert(std::all_of(boundaryPlanes.begin(), boundaryPlanes.end(), [](autoc& plane) { return AD::IsNormalizedVector(AD::GetPlaneNormal(plane)); }));

      autoc selector = [&](MortonNodeIDCR, Node const& node) -> bool {
        for (autoc& plane : boundaryPlanes)
        {
          autoc relation = AD::GetBoxPlaneRelation(node.GetBoxInternal(), AD::GetPlaneOrigoDistance(plane), AD::GetPlaneNormal(plane), tolerance);
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
            relation = getEntityPlaneRelation(detail::at(data, entityID), AD::GetPlaneOrigoDistance(plane), AD::GetPlaneNormal(plane), tolerance);
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
    typename TAdapter_ = AdaptorGeneral<DIMENSION_NO, TVector_, TBox_, TRay_, TPlane_, TGeometry_>,
    typename TContainer_ = std::span<TVector_ const>>
  class OrthoTreePoint final : public OrthoTreeBase<DIMENSION_NO, TVector_, TVector_, TBox_, TRay_, TPlane_, TGeometry_, TAdapter_, TContainer_>
  {
  protected:
    using Base = OrthoTreeBase<DIMENSION_NO, TVector_, TVector_, TBox_, TRay_, TPlane_, TGeometry_, TAdapter_, TContainer_>;
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
    using TEntity = typename Base::TEntity;
    using TEntityID = typename Base::TEntityID;
    using TContainer = typename Base::TContainer;

    static constexpr std::size_t DEFAULT_MAX_ELEMENT = 21;

  private: // Aid functions
    struct Location
    {
      TEntityID EntityID;
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
        autoc actualChildID = Base::castMortonIdToChildId((locationBeginIterator->GridID - gridID) >> shift);
        autoc actualEndIterator = std::partition_point(locationBeginIterator, locationEndIterator, [&](autoc& location) {
          return actualChildID == Base::castMortonIdToChildId((location.GridID - gridID) >> shift);
        });

        autoc actualChildGridID = MortonGridID(actualChildID);
        MortonGridID const childKey = parentKeyFlag | actualChildGridID;
        MortonGridID const beginChildGridID = gridID + actualChildGridID * stepNo;

        parentNode.AddChild(childKey);
        auto& childNode = this->createChild(parentNode, actualChildID, childKey);
        this->addNodes(childNode, childKey, locationBeginIterator, actualEndIterator, beginChildGridID, remainingDepth);
      }
    }

    static TBox getBoxOfPoints(TContainer const& points) noexcept
    {
      auto ext = Base::getBoxInvertedInit();
      for (autoc& point : points)
        for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
        {
          if (AD::GetBoxMinC(ext, dimensionID) > AD::GetPointC(detail::getValuePart(point), dimensionID))
            AD::SetBoxMinC(ext, dimensionID, AD::GetPointC(detail::getValuePart(point), dimensionID));

          if (AD::GetBoxMaxC(ext, dimensionID) < AD::GetPointC(detail::getValuePart(point), dimensionID))
            AD::SetBoxMaxC(ext, dimensionID, AD::GetPointC(detail::getValuePart(point), dimensionID));
        }

      return ext;
    }

  public: // Create
    // Ctors
    OrthoTreePoint() = default;
    OrthoTreePoint(
      TContainer const& points,
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
      TContainer const& points,
      std::optional<depth_t> maxDepthNoIn = std::nullopt,
      std::optional<TBox> const& boxSpaceOptional = std::nullopt,
      std::size_t maxElementNoInNode = DEFAULT_MAX_ELEMENT) noexcept
    {
      autoc boxSpace = boxSpaceOptional.has_value() ? *boxSpaceOptional : getBoxOfPoints(points);
      autoc pointNo = points.size();

      autoc maxDepthNo = (!maxDepthNoIn || maxDepthNoIn == 0) ? Base::EstimateMaxDepth(pointNo, maxElementNoInNode) : *maxDepthNoIn;
      tree.Init(boxSpace, maxDepthNo, maxElementNoInNode);
      Base::reserveContainer(tree.m_nodes, Base::EstimateNodeNumber(pointNo, maxDepthNo, maxElementNoInNode));
      if (points.empty())
        return;

      auto pointLocations = std::vector<Location>(pointNo);
      auto ept = TExecutionPolicy{}; // GCC 11.3 only accept in this form
      std::transform(ept, points.begin(), points.end(), pointLocations.begin(), [&](autoc& point) {
        return Location{ detail::getKeyPart(points, point), tree.getLocationID(detail::getValuePart(point)) };
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
    bool InsertWithRebalancing(TEntityID newEntityID, TVector const& newPoint, TContainer const& points) noexcept
    {
      if (!AD::DoesBoxContainPoint(this->m_boxSpace, newPoint))
        return false;

      autoc[entityDepth, entityLocation] = this->getDepthAndLocationID(newPoint);
      autoc entityNodeKey = Base::GetHash(entityDepth, entityLocation);
      autoc[parentNodeKey, parentDepthID] = this->FindSmallestNodeKeyWithDepth(entityNodeKey);
      if (!Base::IsValidKey(parentNodeKey))
        return false;

      return this->template insertWithRebalancing<true>(parentNodeKey, parentDepthID, entityNodeKey, entityDepth, newEntityID, points);
    }

    // Insert item into a node. If doInsertToLeaf is true: The smallest node will be chosen by the max depth. If doInsertToLeaf is false: The smallest existing level on the branch will be chosen.
    bool Insert(TEntityID entityID, TVector const& newPoint, bool doInsertToLeaf = false) noexcept
    {
      if (!AD::DoesBoxContainPoint(this->m_boxSpace, newPoint))
        return false;

      autoc entityNodeKey = this->GetNodeID(newPoint);
      autoc smallestNodeKey = this->FindSmallestNodeKey(entityNodeKey);
      if (!Base::IsValidKey(smallestNodeKey))
        return false;

      return this->template insertWithoutRebalancing<true>(smallestNodeKey, entityNodeKey, entityID, doInsertToLeaf);
    }

    // Erase an id. Traverse all node if it is needed, which has major performance penalty.
    template<bool DO_UPDATE_ENTITY_IDS = std::is_same_v<TEntity, typename TContainer::value_type>>
    constexpr bool EraseId(TEntityID entityID) noexcept
    {
      bool isErased = false;
      for (auto& [nodeKey, node] : this->m_nodes)
      {
        if (std::erase(node.Entities, entityID))
        {
          this->removeNodeIfPossible(nodeKey, node);
          isErased = true;
          break;
        }
      }

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
    template<bool DO_UPDATE_ENTITY_IDS = std::is_same_v<TEntity, typename TContainer::value_type>>
    bool Erase(TEntityID entitiyID, TVector const& entityOriginalPoint) noexcept
    {
      autoc nodeKey = this->FindSmallestNode(entityOriginalPoint);
      if (!Base::IsValidKey(nodeKey))
        return false; // old box is not in the handled space domain

      auto& node = this->m_nodes.at(nodeKey);
      autoc endIteratorAfterRemove = std::remove(node.Entities.begin(), node.Entities.end(), entitiyID);
      if (endIteratorAfterRemove == node.Entities.end())
        return false; // id was not registered previously.

      node.Entities.erase(endIteratorAfterRemove, node.Entities.end());

      if constexpr (DO_UPDATE_ENTITY_IDS)
      {
        std::ranges::for_each(this->m_nodes, [entitiyID](auto& pairNode) {
          for (auto& id : pairNode.second.Entities)
            id -= entitiyID < id;
        });
      }

      this->removeNodeIfPossible(nodeKey, node);

      return true;
    }


    // Update id by the new point information
    bool Update(TEntityID entityID, TVector const& newPoint, bool doesInsertToLeaf = false) noexcept
    {
      if (!AD::DoesBoxContainPoint(this->m_boxSpace, newPoint))
        return false;

      if (!this->EraseId<false>(entityID))
        return false;

      return this->Insert(entityID, newPoint, doesInsertToLeaf);
    }


    // Update id by the new point information and the erase part is aided by the old point geometry data
    bool Update(TEntityID entityID, TVector const& oldPoint, TVector const& newPoint, bool doesInsertToLeaf = false) noexcept
    {
      if (!AD::DoesBoxContainPoint(this->m_boxSpace, newPoint))
        return false;

      if (!this->Erase<false>(entityID, oldPoint))
        return false;

      return this->Insert(entityID, newPoint, doesInsertToLeaf);
    }


    // Update id with rebalancing by the new point information
    bool Update(TEntityID entityID, TVector const& newPoint, TContainer const& points) noexcept
    {
      if (!AD::DoesBoxContainPoint(this->m_boxSpace, newPoint))
        return false;

      if (!this->EraseId<false>(entityID))
        return false;

      return this->InsertWithRebalancing(entityID, newPoint, points);
    }


    // Update id with rebalacing by the new point information and the erase part is aided by the old point geometry data
    bool Update(TEntityID entityID, TVector const& oldPoint, TVector const& newPoint, TContainer const& points) noexcept
    {
      if (!AD::DoesBoxContainPoint(this->m_boxSpace, newPoint))
        return false;

      if (!this->Erase<false>(entityID, oldPoint))
        return false;

      return this->InsertWithRebalancing(entityID, newPoint, points);
    }

  public: // Search functions
    bool Contains(TVector const& searchPoint, TContainer const& points, TGeometry tolerance) const noexcept
    {
      autoc smallestNodeKey = this->FindSmallestNode(searchPoint);
      if (!Base::IsValidKey(smallestNodeKey))
        return false;

      autoc& node = this->GetNode(smallestNodeKey);
      return std::ranges::any_of(node.Entities, [&](autoc& entityID) {
        return AD::ArePointsEqual(searchPoint, detail::at(points, entityID), tolerance);
      });
    }

    // Range search
    template<bool DOES_LEAF_NODE_CONTAIN_ELEMENT_ONLY = false>
    std::vector<TEntityID> RangeSearch(TBox const& range, TContainer const& points) const noexcept
    {
      auto foundEntityIDs = std::vector<TEntityID>();

      if (!this->template rangeSearchRoot<false, DOES_LEAF_NODE_CONTAIN_ELEMENT_ONLY>(range, points, foundEntityIDs))
        return {};

      return foundEntityIDs;
    }

    // Hyperplane intersection (Plane equation: dotProduct(planeNormal, point) = distanceOfOrigo)
    inline std::vector<TEntityID> PlaneSearch(
      TGeometry const& distanceOfOrigo, TVector const& planeNormal, TGeometry tolerance, TContainer const& points) const noexcept
    {
      return this->planeIntersection(distanceOfOrigo, planeNormal, tolerance, points);
    }

    // Hyperplane intersection using built-in plane
    inline std::vector<TEntityID> PlaneSearch(TPlane const& plane, TGeometry tolerance, TContainer const& points) const noexcept
    {
      return this->planeIntersection(AD::GetPlaneOrigoDistance(plane), AD::GetPlaneNormal(plane), tolerance, points);
    }

    // Hyperplane segmentation, get all elements in positive side (Plane equation: dotProduct(planeNormal, point) = distanceOfOrigo)
    inline std::vector<TEntityID> PlanePositiveSegmentation(
      TGeometry distanceOfOrigo, TVector const& planeNormal, TGeometry tolerance, TContainer const& points) const noexcept
    {
      return this->planePositiveSegmentation(distanceOfOrigo, planeNormal, tolerance, points);
    }

    // Hyperplane segmentation, get all elements in positive side (Plane equation: dotProduct(planeNormal, point) = distanceOfOrigo)
    inline std::vector<TEntityID> PlanePositiveSegmentation(TPlane const& plane, TGeometry tolerance, TContainer const& points) const noexcept
    {
      return this->planePositiveSegmentation(AD::GetPlaneOrigoDistance(plane), AD::GetPlaneNormal(plane), tolerance, points);
    }

    // Hyperplane segmentation, get all elements in positive side (Plane equation: dotProduct(planeNormal, point) = distanceOfOrigo)
    inline std::vector<TEntityID> FrustumCulling(std::span<TPlane const> const& boundaryPlanes, TGeometry tolerance, TContainer const& points) const noexcept
    {
      return this->frustumCulling(boundaryPlanes, tolerance, points);
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


    static void createEntityDistance(Node const& node, TVector const& searchPoint, TContainer const& points, std::multiset<EntityDistance>& neighborEntities) noexcept
    {
      for (autoc id : node.Entities)
        neighborEntities.insert({ { AD::Distance(searchPoint, detail::at(points, id)) }, id });
    }

    static TGeometry getFarestDistance(std::multiset<EntityDistance>& neighborEntities, std::size_t neighborNo) noexcept
    {
      if (neighborEntities.size() < neighborNo)
        return std::numeric_limits<TGeometry>::max();

      return std::next(neighborEntities.begin(), neighborNo - 1)->Distance;
    }

    static std::vector<TEntityID> convertEntityDistanceToList(std::multiset<EntityDistance>& neighborEntities, std::size_t neighborNo) noexcept
    {
      autoc entityNo = std::min(neighborNo, neighborEntities.size());
      auto entityIDs = std::vector<TEntityID>(entityNo);
      std::transform(neighborEntities.begin(), std::next(neighborEntities.begin(), entityNo), entityIDs.begin(), [](autoc& ed) { return ed.EntityID; });
      return entityIDs;
    }

  public:
    // K Nearest Neighbor
    std::vector<TEntityID> GetNearestNeighbors(TVector const& searchPoint, std::size_t neighborNo, TContainer const& points) const noexcept
    {
      auto neighborEntities = std::multiset<EntityDistance>();
      autoc smallestNodeKey = this->template FindSmallestNode<true>(searchPoint);
      if (Base::IsValidKey(smallestNodeKey))
      {
        autoc& smallestNode = this->GetNode(smallestNodeKey);
        autoc wallDistance = getMinBoxWallDistance(searchPoint, smallestNode.GetBoxInternal());
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
          autoc dMin = AD::GetBoxMinC(node.GetBoxInternal(), dimensionID) - AD::GetPointC(searchPoint, dimensionID);
          autoc dMax = AD::GetBoxMaxC(node.GetBoxInternal(), dimensionID) - AD::GetPointC(searchPoint, dimensionID);

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
    typename TAdapter_ = AdaptorGeneral<DIMENSION_NO, TVector_, TBox_, TRay_, TPlane_, TGeometry_>,
    typename TContainer_ = std::span<TBox_ const>>
  class OrthoTreeBoundingBox final : public OrthoTreeBase<DIMENSION_NO, TBox_, TVector_, TBox_, TRay_, TPlane_, TGeometry_, TAdapter_, TContainer_>
  {
  protected:
    using Base = OrthoTreeBase<DIMENSION_NO, TBox_, TVector_, TBox_, TRay_, TPlane_, TGeometry_, TAdapter_, TContainer_>;
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
    using TEntity = typename Base::TEntity;
    using TEntityID = typename Base::TEntityID;
    using TContainer = typename Base::TContainer;

    static constexpr std::size_t DEFAULT_MAX_ELEMENT = 21;

  private: // Aid functions
    struct Location
    {
      TEntityID EntityID;
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
      return Base::castMortonIdToChildId(gridIDOnCurrentDepth - nodeIDOnCurrentDepth);
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

        parentNode.AddChild(childKey);
        auto& nodeChild = this->createChild(parentNode, actualChildID, childKey);
        this->addNodes(nodeChild, childKey, beginLocationIterator, actualEndLocationIterator, firstChildLocationID, remainingDepthNo);
      }
    }


    void split(std::array<DimArray<GridID>, 2> const& boxMinMaxGridID, Location& location, LocationContainer& additionalLocations) const noexcept
    {
      depth_t depthID = location.DepthID + SPLIT_DEPTH_INCREASEMENT;
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


      // First element into locationID
      location.DepthID = depthID;
      location.MinGridID = Base::MortonEncode(gridIDs[0]) >> shift;
      autoc entityID = location.EntityID;

      autoc additionalBoxNo = boxNo - 1;
      autoc locationNo = additionalLocations.size();
      additionalLocations.resize(locationNo + additionalBoxNo);

      LOOPIVDEP
      for (std::size_t iBox = 0; iBox < additionalBoxNo; ++iBox)
      {
        auto& location = additionalLocations.at(locationNo + iBox);
        location.EntityID = entityID;
        location.DepthID = depthID;
        location.MinGridID = Base::MortonEncode(gridIDs[iBox + 1]) >> shift;
      }
    }


    Location getLocation(TEntityID entityID, TBox const& box, LocationContainer* additionalLocations) const noexcept
    {
      autoc boxMinMaxGridID = this->getGridIdBox(box);

      auto location = Location{ .EntityID = entityID, .MinGridID = Base::MortonEncode(boxMinMaxGridID[0]), .DepthID = this->m_maxDepthNo };

      autoc maxGridID = Base::MortonEncode(boxMinMaxGridID[1]);
      if (location.MinGridID == maxGridID)
        return location;

      autoc minGridID = location.MinGridID;
      for (auto flagDiffOfLocation = location.MinGridID ^ maxGridID; Base::IsValidKey(flagDiffOfLocation);
           flagDiffOfLocation >>= DIMENSION_NO, --location.DepthID)
        location.MinGridID >>= DIMENSION_NO;

      if constexpr (SPLIT_DEPTH_INCREASEMENT > 0)
      {
        autoc remainingDepthNo = this->m_maxDepthNo - location.DepthID;

        // if not all nodes are touched, we split
        if (!Base::IsValidKey((maxGridID - minGridID) >> (remainingDepthNo * DIMENSION_NO - 1)))
          this->split(boxMinMaxGridID, location, *additionalLocations);
      }

      return location;
    }

    static TBox getBoxOfBoxes(TContainer const& boxes) noexcept
    {
      auto ext = Base::getBoxInvertedInit();
      for (autoc& e : boxes)
      {
        for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
        {
          if (AD::GetBoxMinC(ext, dimensionID) > AD::GetBoxMinC(detail::getValuePart(e), dimensionID))
            AD::SetBoxMinC(ext, dimensionID, AD::GetBoxMinC(detail::getValuePart(e), dimensionID));

          if (AD::GetBoxMaxC(ext, dimensionID) < AD::GetBoxMaxC(detail::getValuePart(e), dimensionID))
            AD::SetBoxMaxC(ext, dimensionID, AD::GetBoxMaxC(detail::getValuePart(e), dimensionID));
        }
      }
      return ext;
    }

  public: // Create
    // Ctors
    OrthoTreeBoundingBox() = default;
    OrthoTreeBoundingBox(
      TContainer const& boxes,
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
      TContainer const& boxes,
      std::optional<depth_t> maxDepthIn = std::nullopt,
      std::optional<TBox> const& boxSpaceOptional = std::nullopt,
      std::size_t maxElementNoInNode = DEFAULT_MAX_ELEMENT) noexcept
    {
      autoc boxSpace = boxSpaceOptional.has_value() ? *boxSpaceOptional : getBoxOfBoxes(boxes);
      autoc entityNo = boxes.size();
      autoc maxDepthNo = (!maxDepthIn || maxDepthIn == 0) ? Base::EstimateMaxDepth(entityNo, maxElementNoInNode) : *maxDepthIn;
      tree.Init(boxSpace, maxDepthNo, maxElementNoInNode);

      Base::reserveContainer(tree.m_nodes, Base::EstimateNodeNumber(entityNo, maxDepthNo, maxElementNoInNode));
      if (entityNo == 0)
        return;

      autoc rootKey = Base::GetRootKey();
      auto& nodeRoot = cont_at(tree.m_nodes, rootKey);

      autoce NON_SPLITTED = SPLIT_DEPTH_INCREASEMENT == 0;
      autoce NON_PARALLEL = std::is_same<TExecutionPolicy, std::execution::unsequenced_policy>::value ||
                            std::is_same<TExecutionPolicy, std::execution::sequenced_policy>::value;

      auto epf = TExecutionPolicy{}; // GCC 11.3
      auto locations = LocationContainer(entityNo);

      if constexpr (NON_SPLITTED)
      {
        std::transform(epf, boxes.begin(), boxes.end(), locations.begin(), [&tree, &boxes](autoc& box) {
          return tree.getLocation(detail::getKeyPart(boxes, box), detail::getValuePart(box), nullptr);
        });
      }
      else if constexpr (NON_PARALLEL)
      {
        locations.reserve(entityNo * std::min<std::size_t>(10, Base::CHILD_NO * SPLIT_DEPTH_INCREASEMENT));

        std::size_t locationID = 0;
        std::for_each(epf, boxes.begin(), boxes.end(), [&tree, &boxes, &locations, &locationID](autoc& box) {
          locations[locationID] = tree.getLocation(detail::getKeyPart(boxes, box), detail::getValuePart(box), &locations);
          ++locationID;
        });
      }
      else // Splitted with parallel execution
      {
        auto additionalLocations = std::unordered_map<TEntityID, LocationContainer>{};
        for (autoc& entity : boxes)
          additionalLocations[detail::getKeyPart(boxes, entity)];

        locations.reserve(entityNo * std::min<std::size_t>(10, Base::CHILD_NO * SPLIT_DEPTH_INCREASEMENT));
        std::transform(epf, boxes.begin(), boxes.end(), locations.begin(), [&tree, &boxes, &additionalLocations](autoc& box) {
          autoc entityID = detail::getKeyPart(boxes, box);
          return tree.getLocation(entityID, detail::getValuePart(box), &additionalLocations.at(entityID));
        });

        auto additionalLocationPositions = std::unordered_map<TEntityID, std::size_t>(entityNo);
        std::size_t position = entityNo;
        for (autoc & [entityID, adds] : additionalLocations)
        {
          additionalLocationPositions[entityID] = position;
          position += adds.size();
        }

        locations.resize(position);
        auto epf2 = TExecutionPolicy{}; // GCC 11.3
        std::for_each(
          epf2,
          additionalLocations.begin(),
          additionalLocations.end(),
          [&locations, &additionalLocationPositions](auto& additionalLocation) {
            if (additionalLocation.second.empty())
              return;

            std::copy(
              additionalLocation.second.begin(),
              additionalLocation.second.end(),
              std::next(locations.begin(), additionalLocationPositions.at(additionalLocation.first)));
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
    bool InsertWithRebalancing(TEntityID newEntityID, TBox const& newBox, TContainer const& boxes) noexcept
    {
      if (!AD::AreBoxesOverlapped(this->m_boxSpace, newBox))
        return false;

      auto locations = std::vector<Location>(1);
      locations[0] = this->getLocation(newEntityID, newBox, &locations);

      for (autoc& location : locations)
      {
        autoc entityNodeKey = this->GetHash(location.DepthID, location.MinGridID);
        autoc parentNodeKey = this->FindSmallestNodeKey(entityNodeKey);

        if (!this->template insertWithRebalancing<SPLIT_DEPTH_INCREASEMENT == 0>(
              parentNodeKey, Base::GetDepthID(parentNodeKey), entityNodeKey, location.DepthID, newEntityID, boxes))
          return false;
      }

      return true;
    }


    // Insert item into a node. If doInsertToLeaf is true: The smallest node will be chosen by the max depth. If doInsertToLeaf is false: The smallest existing level on the branch will be chosen.
    bool Insert(TEntityID newEntityID, TBox const& newBox, bool doInsertToLeaf = false) noexcept
    {
      if (!AD::AreBoxesOverlapped(this->m_boxSpace, newBox))
        return false;

      autoc smallestNodeKey = this->FindSmallestNode(newBox);
      if (!Base::IsValidKey(smallestNodeKey))
        return false; // new box is not in the handled space domain

      auto locations = std::vector<Location>(1);
      locations[0] = this->getLocation(newEntityID, newBox, &locations);

      for (autoc& location : locations)
      {
        autoc entityNodeKey = this->GetHash(location.DepthID, location.MinGridID);
        if (!this->template insertWithoutRebalancing<SPLIT_DEPTH_INCREASEMENT == 0>(smallestNodeKey, entityNodeKey, newEntityID, doInsertToLeaf))
          return false;
      }

      return true;
    }


  private:
    bool doErase(Node& node, TEntityID entityID) noexcept
    {
      auto& idList = node.Entities;
      autoc endIteratorAfterRemove = std::remove(idList.begin(), idList.end(), entityID);
      if (endIteratorAfterRemove == idList.end())
        return false; // id was not registered previously.

      idList.erase(endIteratorAfterRemove, idList.end());
      return true;
    }


    template<depth_t REMAINING_DEPTH>
    bool doEraseRec(MortonNodeIDCR nodeKey, TEntityID entityID) noexcept
    {
      auto& node = this->m_nodes.at(nodeKey);
      auto ret = this->doErase(node, entityID);
      if constexpr (REMAINING_DEPTH > 0)
      {
        autoc children = node.GetChildren();
        for (MortonNodeIDCR childKey : children)
          ret |= doEraseRec<REMAINING_DEPTH - 1>(childKey, entityID);
      }

      this->removeNodeIfPossible(nodeKey, node);

      return ret;
    }


  public:
    // Erase id, aided with the original bounding box
    template<bool DO_UPDATE_ENTITY_IDS = true>
    bool Erase(TEntityID entityIDToErase, TBox const& box) noexcept
    {
      autoc smallestNodeKey = this->FindSmallestNode(box);
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
    constexpr bool EraseId(TEntityID idErase) noexcept
    {
      bool isErased = false;
      if constexpr (SPLIT_DEPTH_INCREASEMENT == 0)
      {
        for (auto& [nodeKey, node] : this->m_nodes)
        {
          if (std::erase(node.Entities, idErase) > 0)
          {
            this->removeNodeIfPossible(nodeKey, node);
            isErased = true;
            break;
          }
        }
      }
      else
      {
        auto erasableNodes = std::vector<MortonNodeID>{};
        for (auto& [nodeKey, node] : this->m_nodes)
        {
          autoc isErasedInCurrent = std::erase(node.Entities, idErase) > 0;
          if (isErasedInCurrent)
            erasableNodes.emplace_back(nodeKey);

          isErased |= isErasedInCurrent;
        }

        for (MortonNodeIDCR nodeKey : erasableNodes)
          this->removeNodeIfPossible(nodeKey, this->GetNode(nodeKey));
      }

      if (!isErased)
        return false;

      if constexpr (DO_UPDATE_ENTITY_IDS)
        std::ranges::for_each(this->m_nodes, [&](auto& pairNode) {
          for (auto& id : pairNode.second.Entities)
            id -= idErase < id;
        });

      return true;
    }


    // Update id by the new bounding box information
    bool Update(TEntityID entityID, TBox const& boxNew, bool doInsertToLeaf = false) noexcept
    {
      if (!AD::AreBoxesOverlapped(this->m_boxSpace, boxNew))
        return false;

      if (!this->EraseId<false>(entityID))
        return false;

      return this->Insert(entityID, boxNew, doInsertToLeaf);
    }


    // Update id by the new bounding box information and the erase part is aided by the old bounding box geometry data
    bool Update(TEntityID entityID, TBox const& oldBox, TBox const& newBox, bool doInsertToLeaf = false) noexcept
    {
      if (!AD::AreBoxesOverlapped(this->m_boxSpace, newBox))
        return false;

      if constexpr (SPLIT_DEPTH_INCREASEMENT == 0)
        if (this->FindSmallestNode(oldBox) == this->FindSmallestNode(newBox))
          return true;

      if (!this->Erase<false>(entityID, oldBox))
        return false; // entityID was not registered previously.

      return this->Insert(entityID, newBox, doInsertToLeaf);
    }


    // Update id with rebalancing by the new bounding box information
    bool Update(TEntityID entityID, TBox const& boxNew, TContainer const& boxes) noexcept
    {
      if (!AD::AreBoxesOverlapped(this->m_boxSpace, boxNew))
        return false;

      if (!this->EraseId<false>(entityID))
        return false;

      return this->InsertWithRebalancing(entityID, boxNew, boxes);
    }


    // Update id with rebalancing by the new bounding box information and the erase part is aided by the old bounding box geometry data
    bool Update(TEntityID entityID, TBox const& oldBox, TBox const& newBox, TContainer const& boxes) noexcept
    {
      if (!AD::AreBoxesOverlapped(this->m_boxSpace, newBox))
        return false;

      if constexpr (SPLIT_DEPTH_INCREASEMENT == 0)
        if (this->FindSmallestNode(oldBox) == this->FindSmallestNode(newBox))
          return true;

      if (!this->Erase<false>(entityID, oldBox))
        return false; // entityID was not registered previously.

      return this->InsertWithRebalancing(entityID, newBox, boxes);
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


    void pickSearch(TVector const& pickPoint, TContainer const& boxes, Node const& parentNode, std::vector<TEntityID>& foundEntitiyIDs) const noexcept
    {
      std::ranges::copy_if(parentNode.Entities, back_inserter(foundEntitiyIDs), [&](autoc entityID) {
        return AD::DoesBoxContainPoint(detail::at(boxes, entityID), pickPoint);
      });

      for (MortonNodeIDCR keyChild : parentNode.GetChildren())
      {
        autoc& childNode = this->GetNode(keyChild);
        if (!AD::DoesBoxContainPoint(childNode.GetBoxInternal(), pickPoint))
          continue;

        pickSearch(pickPoint, boxes, childNode, foundEntitiyIDs);
      }
    }


  public: // Search functions
    // Pick search
    std::vector<TEntityID> PickSearch(TVector const& pickPoint, TContainer const& boxes) const noexcept
    {
      auto foundEntitiyIDs = std::vector<TEntityID>();
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
          return AD::DoesBoxContainPoint(detail::at(boxes, entityID), pickPoint);
        });
      }

      return foundEntitiyIDs;
    }


    // Range search
    template<bool DO_MUST_FULLY_CONTAIN = true>
    std::vector<TEntityID> RangeSearch(TBox const& range, TContainer const& boxes) const noexcept
    {
      auto foundEntities = std::vector<TEntityID>();

      if (!this->template rangeSearchRoot<DO_MUST_FULLY_CONTAIN, false>(range, boxes, foundEntities))
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
    inline std::vector<TEntityID> PlaneIntersection(
      TGeometry const& distanceOfOrigo, TVector const& planeNormal, TGeometry tolerance, TContainer const& boxes) const noexcept
    {
      return this->planeIntersection(distanceOfOrigo, planeNormal, tolerance, boxes);
    }

    // Hyperplane intersection using built-in plane
    inline std::vector<TEntityID> PlaneIntersection(TPlane const& plane, TGeometry tolerance, TContainer const& boxes) const noexcept
    {
      return this->planeIntersection(AD::GetPlaneOrigoDistance(plane), AD::GetPlaneNormal(plane), tolerance, boxes);
    }

    // Hyperplane segmentation, get all elements in positive side (Plane equation: dotProduct(planeNormal, point) = distanceOfOrigo)
    inline std::vector<TEntityID> PlanePositiveSegmentation(
      TGeometry distanceOfOrigo, TVector const& planeNormal, TGeometry tolerance, TContainer const& boxes) const noexcept
    {
      return this->planePositiveSegmentation(distanceOfOrigo, planeNormal, tolerance, boxes);
    }

    // Hyperplane segmentation, get all elements in positive side (Plane equation: dotProduct(planeNormal, point) = distanceOfOrigo)
    inline std::vector<TEntityID> PlanePositiveSegmentation(TPlane const& plane, TGeometry tolerance, TContainer const& boxes) const noexcept
    {
      return this->planePositiveSegmentation(AD::GetPlaneOrigoDistance(plane), AD::GetPlaneNormal(plane), tolerance, boxes);
    }

    // Hyperplane segmentation, get all elements in positive side (Plane equation: dotProduct(planeNormal, point) = distanceOfOrigo)
    inline std::vector<TEntityID> FrustumCulling(std::span<TPlane const> const& boundaryPlanes, TGeometry tolerance, TContainer const& boxes) const noexcept
    {
      return this->frustumCulling(boundaryPlanes, tolerance, boxes);
    }


    // Client-defined Collision detector (params: id1, e1, id2, e2). It supplemented with the box intersection, the Client should not add.
    using FCollisionDetector = std::function<bool(TEntityID, TBox const&, TEntityID, TBox const&)>;

    // Collision detection: Returns all overlapping boxes from the source trees.
    static std::vector<std::pair<TEntityID, TEntityID>> CollisionDetection(
      OrthoTreeBoundingBox const& leftTree, TContainer const& leftBoxes, OrthoTreeBoundingBox const& rightTree, TContainer const& rightBoxes) noexcept
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

      auto results = std::vector<std::pair<TEntityID, TEntityID>>{};
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
              if (AD::AreBoxesOverlapped(detail::at(leftBoxes, leftEntityID), detail::at(rightBoxes, rightEntityID), false))
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
              if (AD::AreBoxesOverlapped(leftChildNode.Iterator->second.GetBoxInternal(), rightChildNode.Iterator->second.GetBoxInternal(), false))
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
    inline std::vector<std::pair<TEntityID, TEntityID>> CollisionDetection(
      TContainer const& boxes, OrthoTreeBoundingBox const& otherTree, TContainer const& otherBoxes) const noexcept
    {
      return CollisionDetection(*this, boxes, otherTree, otherBoxes);
    }

  private:
    // Collision detection between the stored elements from bottom to top logic
    template<typename TExecutionPolicy = std::execution::unsequenced_policy>
    std::vector<std::pair<TEntityID, TEntityID>> collisionDetection(TContainer const& boxes, FCollisionDetector&& collisionDetector) const noexcept
    {
      using CollisionDetectionContainer = std::vector<std::pair<TEntityID, TEntityID>>;

      autoc entityNo = boxes.size();
      auto collidedEntityPairs = CollisionDetectionContainer();
      collidedEntityPairs.reserve(boxes.size());


      // SPLIT_DEPTH_INCREASEMENT version of this algorithm needs a reverse std::map
      auto entityIDNodeMap = std::unordered_map<TEntityID, std::vector<MortonNodeID>>(entityNo);
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
            std::ranges::sort(keys.second);
          else
            std::ranges::sort(keys.second, bitset_arithmetic_compare{});
        });
      }


      // Entities which contain all of the tree could slow the algorithm, so these are eliminated
      auto entityIDsInRoot = std::vector<TEntityID>();
      {
        autoc& nodeRoot = this->GetNode(this->GetRootKey());
        std::set<TEntityID> largeEntities;
        for (autoc entityID : nodeRoot.Entities)
        {
          if (AD::AreBoxesOverlapped(detail::at(boxes, entityID), this->m_boxSpace))
          {
            largeEntities.insert(entityID);

            for (autoc& boxOther : boxes)
            {
              autoc entityIDOther = detail::getKeyPart(boxes, boxOther);
              if (!largeEntities.contains(entityIDOther))
                collidedEntityPairs.emplace_back(entityID, entityIDOther);
            }
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
                  if (collisionDetector(entityID, detail::at(boxes, entityID), entityIDFromParent, detail::at(boxes, entityIDFromParent)))
                    collidedEntityPairsInsideNode.emplace_back(entityID, entityIDFromParent);
            }
            else
            {
              // SPLIT_DEPTH_INCREASEMENT: entityID could occur in multiple node. This algorithm aims to check only the first occurrence's parents.

              auto entityIDsToCheckOnOtherBranch = std::vector<TEntityID>{};
              entityIDsToCheckOnOtherBranch.reserve(entityNoInNode);
              auto entityIDPairsFromOtherBranch = std::unordered_map<TEntityID, std::set<TEntityID>>{};
              for (autoc entityID : entityIDs)
              {
                autoc& currentNodeKeys = entityIDNodeMap.at(entityID);
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
                    for (std::size_t iNodeKey = 1; iNodeKey < currentNodeKeysNo; ++iNodeKey)
                    {
                      // An earlier node is already check this level
                      {
                        auto& keyOfEntitysDepth = keysOfEntitysDepth.emplace_back(currentNodeKeys[iNodeKey - 1]);
                        autoc prevDepthID = this->GetDepthID(keyOfEntitysDepth);
                        if (prevDepthID > parentDepthID)
                          keyOfEntitysDepth >>= DIMENSION_NO * (prevDepthID - parentDepthID);

                        if (keyOfEntitysDepth == parentKey)
                          break;
                      }

                      if (currentNodeKeys[iNodeKey] != nodeKey)
                        continue;

                      // On other branch splitted boxes could conflict already
                      for (autoc entityIDFromParent : parentEntityIDs)
                      {
                        autoc& parentNodeKeys = entityIDNodeMap.at(entityIDFromParent);
                        autoc parentNodeKeysNo = parentNodeKeys.size();

                        for (std::size_t iNodePrev = 0, iNodeParent = 0; iNodePrev < iNodeKey && iNodeParent < parentNodeKeysNo;)
                        {
                          if (parentNodeKeys[iNodeParent] == currentNodeKeys[iNodePrev] || (parentNodeKeys[iNodeParent] == keysOfEntitysDepth[iNodePrev]))
                          {
                            // Found an earlier common key
                            entityIDPairsFromOtherBranch[entityID].emplace(entityIDFromParent);
                            break;
                          }
                          else if (parentNodeKeys[iNodeParent] < currentNodeKeys[iNodePrev])
                            ++iNodeParent;
                          else
                            ++iNodePrev;
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
                    if (collisionDetector(entityID, detail::at(boxes, entityID), entityIDFromParent, detail::at(boxes, entityIDFromParent)))
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
                    if (!isAlreadyContained && collisionDetector(entityID, detail::at(boxes, entityID), entityIDFromParent, detail::at(boxes, entityIDFromParent)))
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
            autoc& entityKeys = entityIDNodeMap.at(iEntityID);
            autoc entityKeysNo = entityKeys.size();

            for (std::size_t jEntity = iEntity + 1; jEntity < entityNoInNode; ++jEntity)
            {
              autoc jEntityID = entityIDs[jEntity];
              if constexpr (SPLIT_DEPTH_INCREASEMENT == 0)
              {
                if (collisionDetector(iEntityID, detail::at(boxes, iEntityID), jEntityID, detail::at(boxes, jEntityID)))
                  collidedEntityPairsInsideNode.emplace_back(iEntityID, jEntityID);
              }
              else
              {
                // Same entities could collide in other nodes, but only the first occurrence should be checked
                autoc& entityKeysOfJ = entityIDNodeMap.at(jEntityID);
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
                  if (collisionDetector(iEntityID, detail::at(boxes, iEntityID), jEntityID, detail::at(boxes, jEntityID)))
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
    inline std::vector<std::pair<TEntityID, TEntityID>> CollisionDetection(TContainer const& boxes) const noexcept
    {
      return collisionDetection<TExecutionPolicy>(boxes, [](TEntityID id1, TBox const& e1, TEntityID id2, TBox const& e2) {
        return AD::AreBoxesOverlappedStrict(e1, e2);
      });
    }


    // Collision detection between the stored elements from bottom to top logic
    template<typename TExecutionPolicy = std::execution::unsequenced_policy>
    inline std::vector<std::pair<TEntityID, TEntityID>> CollisionDetection(TContainer const& boxes, FCollisionDetector&& collisionDetector) const noexcept
    {
      return collisionDetection<TExecutionPolicy>(boxes, [collisionDetector](TEntityID id1, TBox const& e1, TEntityID id2, TBox const& e2) {
        return AD::AreBoxesOverlappedStrict(e1, e2) && collisionDetector(id1, e1, id2, e2);
      });
    }

  private:
    void getRayIntersectedAll(
      Node const& node,
      TContainer const& boxes,
      TVector const& rayBasePoint,
      TVector const& rayHeading,
      TGeometry tolerance,
      TGeometry maxExaminationDistance,
      std::vector<EntityDistance>& foundEntities) const noexcept
    {
      autoc isNodeHit = AD::GetRayBoxDistance(node.GetBoxInternal(), rayBasePoint, rayHeading, tolerance);
      if (!isNodeHit)
        return;

      for (autoc entityID : node.Entities)
      {
        autoc entityDistance = AD::GetRayBoxDistance(detail::at(boxes, entityID), rayBasePoint, rayHeading, tolerance);
        if (entityDistance && (maxExaminationDistance == 0 || entityDistance.value() <= maxExaminationDistance))
          foundEntities.push_back({ { TGeometry(entityDistance.value()) }, entityID });
      }

      for (autoc childKey : node.GetChildren())
        getRayIntersectedAll(this->GetNode(childKey), boxes, rayBasePoint, rayHeading, tolerance, maxExaminationDistance, foundEntities);
    }


    void getRayIntersectedFirst(
      Node const& node,
      TContainer const& boxes,
      TVector const& rayBasePoint,
      TVector const& rayHeading,
      TGeometry tolerance,
      std::multiset<EntityDistance>& foundEntities) const noexcept
    {
      autoc maxExaminationDistance =
        foundEntities.empty() ? std::numeric_limits<double>::infinity() : static_cast<double>(foundEntities.rbegin()->Distance);
      for (autoc entityID : node.Entities)
      {
        autoc distance = AD::GetRayBoxDistance(detail::at(boxes, entityID), rayBasePoint, rayHeading, tolerance);
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
        autoc distance = AD::GetRayBoxDistance(nodeChild.GetBoxInternal(), rayBasePoint, rayHeading, tolerance);
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
    std::vector<TEntityID> RayIntersectedAll(
      TVector const& rayBasePointPoint, TVector const& rayHeading, TContainer const& boxes, TGeometry tolerance, TGeometry maxExaminationDistance = 0) const noexcept
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

      auto foundEntityIDs = std::vector<TEntityID>(std::distance(beginIteratorOfEntities, endIteratorOfEntities));
      std::transform(beginIteratorOfEntities, endIteratorOfEntities, foundEntityIDs.begin(), [](autoc& entityDistance) {
        return entityDistance.EntityID;
      });
      return foundEntityIDs;
    }


    // Get first box which is intersected by the ray
    std::optional<TEntityID> RayIntersectedFirst(TVector const& rayBasePoint, TVector const& rayHeading, TContainer const& boxes, TGeometry tolerance) const noexcept
    {
      autoc& node = this->GetNode(this->GetRootKey());
      autoc distance = AD::GetRayBoxDistance(node.GetBoxInternal(), rayBasePoint, rayHeading, tolerance);
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

  template<dim_t DIMENSION_NO, typename TGeometry = BaseGeometryType>
  using AdaptorGeneralND = AdaptorGeneral<
    DIMENSION_NO,
    OrthoTree::VectorND<DIMENSION_NO, TGeometry>,
    OrthoTree::BoundingBoxND<DIMENSION_NO, TGeometry>,
    OrthoTree::RayND<DIMENSION_NO, TGeometry>,
    OrthoTree::PlaneND<DIMENSION_NO, TGeometry>,
    TGeometry>;

  template<dim_t DIMENSION_NO, typename TGeometry = BaseGeometryType, typename TContainer = std::span<OrthoTree::VectorND<DIMENSION_NO, TGeometry> const>>
  using TreePointND = OrthoTree::OrthoTreePoint<
    DIMENSION_NO,
    OrthoTree::VectorND<DIMENSION_NO, TGeometry>,
    OrthoTree::BoundingBoxND<DIMENSION_NO, TGeometry>,
    OrthoTree::RayND<DIMENSION_NO, TGeometry>,
    OrthoTree::PlaneND<DIMENSION_NO, TGeometry>,
    TGeometry,
    AdaptorGeneralND<DIMENSION_NO, TGeometry>,
    TContainer>;

  template<
    dim_t DIMENSION_NO,
    uint32_t SPLIT_DEPTH_INCREASEMENT = 2,
    typename TGeometry = BaseGeometryType,
    typename TContainer = std::span<OrthoTree::BoundingBoxND<DIMENSION_NO, TGeometry> const>>
  using TreeBoxND = OrthoTree::OrthoTreeBoundingBox<
    DIMENSION_NO,
    OrthoTree::VectorND<DIMENSION_NO, TGeometry>,
    OrthoTree::BoundingBoxND<DIMENSION_NO, TGeometry>,
    OrthoTree::RayND<DIMENSION_NO, TGeometry>,
    OrthoTree::PlaneND<DIMENSION_NO, TGeometry>,
    TGeometry,
    SPLIT_DEPTH_INCREASEMENT,
    AdaptorGeneralND<DIMENSION_NO, TGeometry>,
    TContainer>;

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

  // OrthoTrees for higher dimensions
  using TreePoint16D = TreePointND<16, BaseGeometryType>;
  using TreeBox16D = TreeBoxND<16, 2, BaseGeometryType>;


  // Dualtree for bounding boxes with split-depth settings
  template<uint32_t SPLIT_DEPTH_INCREASEMENT>
  using DualtreeBoxs = TreeBoxND<1, SPLIT_DEPTH_INCREASEMENT, BaseGeometryType>;

  // Quadtree for bounding boxes with split-depth settings
  template<uint32_t SPLIT_DEPTH_INCREASEMENT>
  using QuadtreeBoxs = TreeBoxND<2, SPLIT_DEPTH_INCREASEMENT, BaseGeometryType>;

  // Octree for bounding boxes with split-depth settings
  template<uint32_t SPLIT_DEPTH_INCREASEMENT>
  using OctreeBoxs = TreeBoxND<3, SPLIT_DEPTH_INCREASEMENT, BaseGeometryType>;

  // Hexatree for bounding boxes with split-depth settings
  template<uint32_t SPLIT_DEPTH_INCREASEMENT>
  using HexatreeBoxs = TreeBoxND<4, SPLIT_DEPTH_INCREASEMENT, BaseGeometryType>;

  // OrthoTrees for higher dimensions with split-depth settings
  template<uint32_t SPLIT_DEPTH_INCREASEMENT>
  using TreeBox16Ds = TreeBoxND<16, SPLIT_DEPTH_INCREASEMENT, BaseGeometryType>;


  // OrthoTrees with std::unordered_map

  // std::unordered_map-based Quadtree for points
  using QuadtreePointMap = TreePointND<2, BaseGeometryType, std::unordered_map<index_t, OrthoTree::Vector2D>>;

  // std::unordered_map-based Octree for points
  using OctreePointMap = TreePointND<3, BaseGeometryType, std::unordered_map<index_t, OrthoTree::Vector3D>>;

  // std::unordered_map-based Octree for bounding boxes
  using QuadreeBoxMap = TreeBoxND<2, 2, BaseGeometryType, std::unordered_map<index_t, OrthoTree::BoundingBox2D>>;

  // std::unordered_map-based Octree for bounding boxes
  using OctreeBoxMap = TreeBoxND<3, 2, BaseGeometryType, std::unordered_map<index_t, OrthoTree::BoundingBox3D>>;

  // std::unordered_map-based Quadtree for bounding boxes with split-depth settings
  template<uint32_t SPLIT_DEPTH_INCREASEMENT>
  using QuadtreeBoxsMap = TreeBoxND<2, SPLIT_DEPTH_INCREASEMENT, BaseGeometryType, std::unordered_map<index_t, OrthoTree::BoundingBox2D>>;
  using QuadtreeBoxMap = TreeBoxND<2, 2, BaseGeometryType, std::unordered_map<index_t, OrthoTree::BoundingBox2D>>;

  // std::unordered_map-based Octree for bounding boxes with split-depth settings
  template<uint32_t SPLIT_DEPTH_INCREASEMENT>
  using OctreeBoxsMap = TreeBoxND<3, SPLIT_DEPTH_INCREASEMENT, BaseGeometryType, std::unordered_map<index_t, OrthoTree::BoundingBox3D>>;
  using OctreeBoxMap = TreeBoxND<3, 2, BaseGeometryType, std::unordered_map<index_t, OrthoTree::BoundingBox3D>>;


  // User-defined container-based Quadtree for points
  template<typename UDMap>
  using QuadtreePointUDMap = TreePointND<2, BaseGeometryType, UDMap>;

  // User-defined container-based Octree for points
  template<typename UDMap>
  using OctreePointUDMap = TreePointND<3, BaseGeometryType, UDMap>;

  // User-defined container-based Octree for bounding boxes
  template<typename UDMap>
  using QuadreeBoxUDMap = TreeBoxND<2, 2, BaseGeometryType, UDMap>;

  // User-defined container-based Octree for bounding boxes
  template<typename UDMap>
  using OctreeBoxUDMap = TreeBoxND<3, 2, BaseGeometryType, UDMap>;

  // User-defined container-based Quadtree for bounding boxes with split-depth settings
  template<uint32_t SPLIT_DEPTH_INCREASEMENT, typename UDMap>
  using QuadtreeBoxsUDMap = TreeBoxND<2, SPLIT_DEPTH_INCREASEMENT, BaseGeometryType, UDMap>;

  // User-defined container-based Octree for bounding boxes with split-depth settings
  template<uint32_t SPLIT_DEPTH_INCREASEMENT, typename UDMap>
  using OctreeBoxsUDMap = TreeBoxND<3, SPLIT_DEPTH_INCREASEMENT, BaseGeometryType, UDMap>;
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
