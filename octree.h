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


/* Settings
* Use the following define-s before the header include

Node center is not stored within the nodes. It will be calculated ad-hoc everytime when it is required, e.g in search algorithm.
#define ORTHOTREE__DISABLED_NODECENTER

Node size is not stored within the nodes. It will be calculated ad-hoc everytime when it is required, e.g in search algorithm.
#define ORTHOTREE__DISABLED_NODESIZE
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
#include <version>

#include <array>
#include <bitset>
#include <map>
#include <queue>
#include <set>
#include <span>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <assert.h>
#include <math.h>

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

#ifdef __cpp_lib_execution
#define EXEC_POL_DEF(e) \
  std::conditional_t<IS_PARALLEL_EXEC, std::execution::parallel_unsequenced_policy, std::execution::unsequenced_policy> constexpr e
#define EXEC_POL_ADD(e) e,
#else
#define EXEC_POL_DEF(e)
#define EXEC_POL_ADD(e)
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
    constexpr index_t getID(std::array<T, N> const& container, T const& value) noexcept
    {
      return index_t(std::distance(container.data(), &value));
    }

    template<typename T>
    constexpr index_t getID(std::vector<T> const& container, T const& value) noexcept
    {
      return index_t(std::distance(container.data(), &value));
    }


    template<typename T>
    constexpr index_t getID(std::span<T const> const& container, T const& value) noexcept
    {
      return index_t(std::distance(container.data(), &value));
    }

    template<typename TContainer>
    constexpr typename TContainer::key_type getKeyPart(TContainer const& container, typename TContainer::value_type const& value) noexcept
      requires(HasFirst<typename TContainer::value_type>)
    {
      return value.first;
    }

    template<typename TContainer>
    constexpr index_t getKeyPart(TContainer const& container, typename TContainer::value_type const& value) noexcept
      requires(std::contiguous_iterator<typename TContainer::iterator>)
    {
      return index_t(std::distance(&container[0], &value));
    }

    template<typename T>
    constexpr const auto& getValuePart(T const& value) noexcept
      requires(HasSecond<T>)
    {
      return value.second;
    }

    template<typename value_type>
    constexpr const auto& getValuePart(value_type const& value) noexcept
    {
      return value;
    }

    template<typename value_type, typename entity_type>
    constexpr void setValuePart(value_type& value, entity_type const& entity) noexcept
      requires(HasSecond<value_type>)
    {
      value.second = entity;
    }

    template<typename value_type, typename entity_type>
    constexpr void setValuePart(value_type& value, entity_type const& entity) noexcept
    {
      value = entity;
    }


    template<typename TContainer, typename TKey>
    constexpr const auto& at(TContainer const& container, TKey const& key) noexcept
      requires(HasAt<TContainer, TKey>)
    {
      return container.at(key);
    }

    template<typename TContainer, typename TKey>
    constexpr auto& at(TContainer& container, TKey const& key) noexcept
      requires(HasAt<TContainer, TKey>)
    {
      return container.at(key);
    }

    template<typename TContainer, typename TKey>
    constexpr const auto& at(TContainer const& continer, TKey const& key) noexcept
    {
      return continer[key];
    }

    template<typename TContainer, typename TKey>
    constexpr auto& at(TContainer& continer, TKey const& key) noexcept
    {
      return continer[key];
    }

    template<typename TContainer, typename TKey, typename TValue>
    constexpr void set(TContainer& container, TKey key, TValue const& value) noexcept
      requires(HasAt<TContainer, TKey>)
    {
      container.at(key) = value;
    }

    template<typename TContainer, typename TKey, typename TValue>
    constexpr void set(TContainer& continer, TKey key, TValue const& value) noexcept
    {
      continer[key] = value;
    }


    struct pair_hash
    {
      template<typename T>
      static constexpr void hash_combine(std::size_t& seed, T value) noexcept
      {
        seed ^= value + std::size_t{ 0x9e3779b9 } + (seed << std::size_t{ 6 }) + (seed >> std::size_t{ 2 });
      }

      template<typename T1, typename T2>
      constexpr std::size_t operator()(std::pair<T1, T2> const& pair) const noexcept
      {
        std::size_t seed = 0;
        hash_combine(seed, pair.first);
        hash_combine(seed, pair.second);
        return seed;
      }
    };

    template<typename TContainer, typename... TElement>
    concept HasEmplaceBack = requires(TContainer container, TElement&&... elements) { container.emplace_back(std::forward<TElement>(elements)...); };

    template<typename TContainer, typename... TElement>
    concept HasEmplace = requires(TContainer container, TElement&&... elements) { container.emplace(std::forward<TElement>(elements)...); };

    template<HasEmplaceBack TContainer, typename... TElement>
    constexpr void emplace(TContainer& container, TElement&&... element) noexcept
    {
      container.emplace_back(std::forward<TElement>(element)...);
    }

    template<HasEmplace TContainer, typename... TElement>
    constexpr void emplace(TContainer& container, TElement&&... element) noexcept
    {
      container.emplace(std::forward<TElement>(element)...);
    }

    template<typename T, bool DOES_ORDER_MATTER>
    static std::pair<T, T> makePair(T a, T b) noexcept
    {
      if constexpr (DOES_ORDER_MATTER)
        return a < b ? std::pair<T, T>{ a, b } : std::pair<T, T>{ b, a };
      else
        return std::pair<T, T>{ a, b };
    }

    template<typename TContainer>
    void sortAndUnique(TContainer& c) noexcept
    {
      std::sort(c.begin(), c.end());
      c.erase(std::unique(c.begin(), c.end()), c.end());
    }

    template<typename TContainer, typename... TElement>
    concept HasReserve = requires(TContainer container) { container.reserve(0); };

    template<HasReserve TContainer>
    constexpr void reserve(TContainer& c, std::size_t n) noexcept
    {
      c.reserve(n);
    };

    template<typename TContainer>
    constexpr void reserve(TContainer& c, std::size_t n) noexcept {};

    template<uint8_t e, typename TOut = std::size_t>
    consteval TOut pow2_ce()
    {
      constexpr auto bitSize = sizeof(TOut) * CHAR_BIT;
      static_assert(e >= 0 && e < bitSize);
      return TOut{ 1 } << e;
    }

    template<typename TIn, typename TOut = std::size_t>
    constexpr TOut pow2(TIn e) noexcept
    {
      assert(e >= 0 && e < (sizeof(TOut) * CHAR_BIT));
      return TOut{ 1 } << e;
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
    { TAdapter::SetPointC(point, dimensionID, value) };
  } && requires(TVector const& point, dim_t dimensionID) {
    { TAdapter::GetPointC(point, dimensionID) } -> std::convertible_to<TGeometry>;
  } && requires(TBox& box, dim_t dimensionID, TGeometry value) {
    { TAdapter::SetBoxMinC(box, dimensionID, value) };
  } && requires(TBox& box, dim_t dimensionID, TGeometry value) {
    { TAdapter::SetBoxMaxC(box, dimensionID, value) };
  } && requires(TBox const& box, dim_t dimensionID) {
    { TAdapter::GetBoxMinC(box, dimensionID) } -> std::convertible_to<TGeometry>;
  } && requires(TBox const& box, dim_t dimensionID) {
    { TAdapter::GetBoxMaxC(box, dimensionID) } -> std::convertible_to<TGeometry>;
  } && requires(TRay const& ray) {
    { TAdapter::GetRayOrigin(ray) } -> std::convertible_to<TVector const&>;
  } && requires(TRay const& ray) {
    { TAdapter::GetRayDirection(ray) } -> std::convertible_to<TVector const&>;
  } && requires(TPlane const& plane) {
    { TAdapter::GetPlaneNormal(plane) } -> std::convertible_to<TVector const&>;
  } && requires(TPlane const& plane) {
    { TAdapter::GetPlaneOrigoDistance(plane) } -> std::convertible_to<TGeometry>;
  };

  template<class TAdapter, typename TVector, typename TBox, typename TRay, typename TPlane, typename TGeometry = double>
  concept AdaptorConcept =
    requires { requires AdaptorBasicsConcept<TAdapter, TVector, TBox, TRay, TPlane, TGeometry>; } && requires(TBox const& box, TVector const& point) {
      { TAdapter::DoesBoxContainPoint(box, point) } -> std::convertible_to<bool>;
    } && requires(TBox const& e1, TBox const& e2, bool e1_must_contain_e2) {
      { TAdapter::AreBoxesOverlapped(e1, e2, e1_must_contain_e2) } -> std::convertible_to<bool>;
    } && requires(TBox const& e1, TBox const& e2) {
      { TAdapter::AreBoxesOverlappedStrict(e1, e2) } -> std::convertible_to<bool>;
    } && requires(TVector const& box, TGeometry distanceOfOrigo, TVector const& planeNormal, TGeometry tolerance) {
      { TAdapter::GetPointPlaneRelation(box, distanceOfOrigo, planeNormal, tolerance) } -> std::convertible_to<PlaneRelation>;
    } && requires(TBox const& box, TVector const& rayBasePoint, TVector const& rayHeading, TGeometry tolerance) {
      { TAdapter::GetRayBoxDistance(box, rayBasePoint, rayHeading, tolerance) } -> std::convertible_to<std::optional<double>>;
    } && requires(TBox const& box, TRay const& ray, TGeometry tolerance) {
      { TAdapter::GetRayBoxDistance(box, ray, tolerance) } -> std::convertible_to<std::optional<double>>;
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

    static constexpr TGeometry Size2(TVector const& point) noexcept
    {
      auto d2 = TGeometry{ 0 };
      for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
      {
        auto const d = Base::GetPointC(point, dimensionID);
        d2 += d * d;
      }
      return d2;
    }

    static constexpr TGeometry Size(TVector const& point) noexcept { return std::sqrt(Size2(point)); }

    static constexpr TGeometry Dot(TVector const& ptL, TVector const& ptR) noexcept
    {
      auto value = TGeometry{};
      for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
        value += Base::GetPointC(ptL, dimensionID) * Base::GetPointC(ptR, dimensionID);

      return value;
    }

    static constexpr TGeometry Distance2(TVector const& ptL, TVector const& ptR) noexcept
    {
      auto d2 = TGeometry{ 0 };
      for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
      {
        auto const d = Base::GetPointC(ptL, dimensionID) - Base::GetPointC(ptR, dimensionID);
        d2 += d * d;
      }
      return d2;
    }

    static constexpr TGeometry Distance(TVector const& ptL, TVector const& ptR) noexcept { return std::sqrt(Distance2(ptL, ptR)); }

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


    static constexpr std::optional<double> GetRayBoxDistance(TBox const& box, TVector const& rayBasePoint, TVector const& rayHeading, TGeometry tolerance) noexcept
    {
      if (DoesBoxContainPoint(box, rayBasePoint, tolerance))
        return 0.0;

      auto constexpr inf = std::numeric_limits<double>::infinity();

      auto minBoxDistances = std::array<double, DIMENSION_NO>{};
      auto maxBoxDistances = std::array<double, DIMENSION_NO>{};
      for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
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

      auto const minBoxDistance = *std::max_element(minBoxDistances.begin(), minBoxDistances.end());
      auto const maxBoxDistance = *std::min_element(maxBoxDistances.begin(), maxBoxDistances.end());
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

      auto const pointProjected = Dot(planeNormal, point);

      if (pointProjected < distanceOfOrigo - tolerance)
        return PlaneRelation::Negative;

      if (pointProjected > distanceOfOrigo + tolerance)
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
  constexpr auto operator<=>(bitset_arithmetic<N> const& lhs, bitset_arithmetic<N> const& rhs) noexcept
  {
    using R = std::strong_ordering;
    for (std::size_t i = 0, id = N - 1; i < N; ++i, --id)
      if (lhs[id] ^ rhs[id])
        return lhs[id] ? R::greater : R::less;

    return R::equal;
  }

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wbitwise-instead-of-logical"
#endif
  template<std::size_t N>
  bitset_arithmetic<N> operator+(bitset_arithmetic<N> const& lhs, bitset_arithmetic<N> const& rhs) noexcept
  {
    auto result = bitset_arithmetic<N>();
    bool carry = false;
    for (std::size_t i = 0; i < N; ++i)
    {
      result.set(i, (lhs[i] ^ rhs[i]) ^ carry);
      carry = (lhs[i] & rhs[i]) | (lhs[i] & carry) | (rhs[i] & carry);
    }

    assert(!carry); // unhandled overflow
    return result;
  }

  template<std::size_t N>
  constexpr bitset_arithmetic<N> operator-(bitset_arithmetic<N> result, bitset_arithmetic<N> const& rhs) noexcept
  {
    bool borrow = false;
    for (std::size_t index = 0; index < N; ++index)
    {
      bool lhsBit = result[index];
      bool rhsBit = rhs[index];
      result.set(index, lhsBit ^ rhsBit ^ borrow);
      borrow = (!lhsBit & (rhsBit | borrow)) | (lhsBit & rhsBit & borrow);
    }
    return result;
  }
#ifdef __clang__
#pragma clang diagnostic pop
#endif

  template<std::size_t N>
  bitset_arithmetic<N> operator+(bitset_arithmetic<N> const& lhs, std::size_t rhs) noexcept
  {
    return lhs + bitset_arithmetic<N>(rhs);
  }

  template<std::size_t N>
  bitset_arithmetic<N> operator-(bitset_arithmetic<N> const& lhs, std::size_t rhs) noexcept
  {
    return lhs - bitset_arithmetic<N>(rhs);
  }

  template<std::size_t N>
  bitset_arithmetic<N> operator*(bitset_arithmetic<N> const& lhs, bitset_arithmetic<N> const& rhs) noexcept
  {
    auto constexpr mult = [](bitset_arithmetic<N> const& lhs, bitset_arithmetic<N> const& rhs) {
      auto result = bitset_arithmetic<N>{};
      for (std::size_t i = 0; i < N; ++i)
        if (lhs[i])
          result = result + (rhs << i);

      return result;
    };

    return lhs.count() < rhs.count() ? mult(lhs, rhs) : mult(rhs, lhs);
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

  struct bitset_arithmetic_compare final
  {
    template<std::size_t N>
    constexpr bool operator()(bitset_arithmetic<N> const& lhs, bitset_arithmetic<N> const& rhs) const noexcept
    {
      return lhs < rhs;
    }
  };

  namespace detail
  {
    // Internal geometry system which
    //  - can be instantiated
    //  - is float-based (and not suffer from integer aritmetic issues)
    template<dim_t DIMENSION_NO, typename TGeometry, typename TVector, typename TBox, typename AD>
    struct InternalGeometryModule
    {
      using Geometry = typename std::conditional_t<std::is_integral_v<TGeometry>, float, TGeometry>;
      using Vector = std::array<Geometry, DIMENSION_NO>;
      struct Box
      {
        Vector Min, Max;
      };

      static constexpr Geometry Size2(Vector const& vector) noexcept
      {
        auto d2 = Geometry{ 0 };
        LOOPIVDEP
        for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
          d2 += vector[dimensionID] * vector[dimensionID];

        return d2;
      }

      static Geometry Size(Vector const& vector) noexcept { return std::sqrt(Size2(vector)); }

      static constexpr Vector GetBoxCenter(Box const& box) noexcept
      {
        Vector center;
        LOOPIVDEP
        for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
          center[dimensionID] = (box.Min[dimensionID] + box.Max[dimensionID]) * Geometry(0.5);

        return center;
      }

      static constexpr Vector GetBoxCenterAD(TBox const& box) noexcept
      {
        Vector center;
        LOOPIVDEP
        for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
          center[dimensionID] = (AD::GetBoxMinC(box, dimensionID) + AD::GetBoxMaxC(box, dimensionID)) * Geometry(0.5);

        return center;
      }

      static constexpr Vector GetBoxHalfSizeAD(TBox const& box) noexcept
      {
        Vector halfSize;
        LOOPIVDEP
        for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
          halfSize[dimensionID] = (AD::GetBoxMaxC(box, dimensionID) - AD::GetBoxMinC(box, dimensionID)) * Geometry(0.5);

        return halfSize;
      }

      static bool AreBoxesOverlappingByCenter(Vector const& centerLhs, Vector const& centerRhs, Vector const& sizeLhs, Vector const& sizeRhs) noexcept
      {
        Vector distance;
        LOOPIVDEP
        for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
          distance[dimensionID] = centerLhs[dimensionID] - centerRhs[dimensionID];

        Vector sizeLimit;
        LOOPIVDEP
        for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
          sizeLimit[dimensionID] = (sizeLhs[dimensionID] + sizeRhs[dimensionID]) * Geometry(0.5);

        for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
          if (sizeLimit[dimensionID] <= std::abs(distance[dimensionID]))
            return false;

        return true;
      }

      static constexpr void MoveAD(Vector& v, TVector const& moveVector) noexcept
      {
        LOOPIVDEP
        for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
          v[dimensionID] += AD::GetPointC(moveVector, dimensionID);
      }

      static constexpr void MoveAD(Box& box, TVector const& moveVector) noexcept
      {
        LOOPIVDEP
        for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
        {
          box.Min[dimensionID] += AD::GetPointC(moveVector, dimensionID);
          box.Max[dimensionID] += AD::GetPointC(moveVector, dimensionID);
        }
      }

      static constexpr TGeometry DotAD(TVector const& ptL, Vector const& ptR) noexcept
      {
        auto value = TGeometry{};
        LOOPIVDEP
        for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
          value += AD::GetPointC(ptL, dimensionID) * ptR[dimensionID];

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
          if (!DoesRangeContainBox(AD::GetBoxMinC(range, dimensionID), AD::GetBoxMaxC(range, dimensionID), box.Min[dimensionID], box.Max[dimensionID]))
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
          if (!DoesRangeContainBox(range.Min[dimensionID], range.Max[dimensionID], AD::GetBoxMinC(box, dimensionID), AD::GetBoxMaxC(box, dimensionID)))
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

      static constexpr PlaneRelation GetBoxPlaneRelationAD(
        Vector const& center, Vector const& halfSize, TGeometry distanceOfOrigo, TVector const& planeNormal, TGeometry tolerance) noexcept
      {
        assert(AD::IsNormalizedVector(planeNormal));

        auto radiusProjected = double(tolerance);
        for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
          radiusProjected += halfSize[dimensionID] * std::abs(AD::GetPointC(planeNormal, dimensionID));

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
        LOOPIVDEP
        for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
        {
          boxIGM.Min[dimensionID] = Geometry(AD::GetBoxMinC(box, dimensionID));
          boxIGM.Max[dimensionID] = Geometry(AD::GetBoxMaxC(box, dimensionID));
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
            if (ext.Min[dimensionID] > AD::GetPointC(point, dimensionID))
              ext.Min[dimensionID] = Geometry(AD::GetPointC(point, dimensionID));

            if (ext.Max[dimensionID] < AD::GetPointC(point, dimensionID))
              ext.Max[dimensionID] = Geometry(AD::GetPointC(point, dimensionID));
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
            if (ext.Min[dimensionID] > AD::GetBoxMinC(box, dimensionID))
              ext.Min[dimensionID] = Geometry(AD::GetBoxMinC(box, dimensionID));

            if (ext.Max[dimensionID] < AD::GetBoxMaxC(box, dimensionID))
              ext.Max[dimensionID] = Geometry(AD::GetBoxMaxC(box, dimensionID));
          }
        }
        return ext;
      }

      static constexpr bool DoesBoxContainPointAD(Box const& box, TVector const& point, TGeometry tolerance = 0) noexcept
      {
        if (tolerance != 0.0)
        {
          assert(tolerance > 0);
          for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
            if (!(box.Min[dimensionID] - tolerance < AD::GetPointC(point, dimensionID) &&
                  AD::GetPointC(point, dimensionID) < box.Max[dimensionID] + tolerance))
              return false;
        }
        else
        {
          for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
            if (!(box.Min[dimensionID] <= AD::GetPointC(point, dimensionID) && AD::GetPointC(point, dimensionID) <= box.Max[dimensionID]))
              return false;
        }
        return true;
      }

      static constexpr bool DoesBoxContainPointAD(Vector const& center, Vector const& halfSizes, TVector const& point, TGeometry tolerance = 0) noexcept
      {
        if (tolerance != 0.0)
        {
          assert(tolerance > 0);
          for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
          {
            auto const pointDistance = std::abs(Geometry(AD::GetPointC(point, dimensionID)) - center[dimensionID]);
            auto const halfSize = halfSizes[dimensionID] + tolerance;
            if (pointDistance >= halfSize)
              return false;
          }
        }
        else
        {
          for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
          {
            auto const pointDistance = std::abs(Geometry(AD::GetPointC(point, dimensionID)) - center[dimensionID]);
            if (pointDistance > halfSizes[dimensionID])
              return false;
          }
        }
        return true;
      }

      static Vector GetBoxWallDistanceAD(TVector const& searchPoint, Vector const& centerPoint, Vector const& halfSize) noexcept
      {
        Vector distance;
        for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
        {
          auto const centerDistance = std::abs(centerPoint[dimensionID] - Geometry(AD::GetPointC(searchPoint, dimensionID)));
          distance[dimensionID] = halfSize[dimensionID] - centerDistance;
        }

        return distance;
      }

      static constexpr std::optional<double> GetRayBoxDistanceAD(
        Vector const& center, Vector const& halfSizes, TVector const& rayBasePoint, TVector const& rayHeading, TGeometry tolerance) noexcept
      {
        if (DoesBoxContainPointAD(center, halfSizes, rayBasePoint, tolerance))
          return 0.0;

        auto constexpr inf = std::numeric_limits<double>::infinity();

        auto minBoxDistances = Vector{};
        auto maxBoxDistances = Vector{};
        for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
        {
          auto const dirComp = Geometry(AD::GetPointC(rayHeading, dimensionID));
          auto const minBox = center[dimensionID] - halfSizes[dimensionID] - Geometry(tolerance);
          auto const maxBox = center[dimensionID] + halfSizes[dimensionID] + Geometry(tolerance);
          if (dirComp == 0)
          {
            if (tolerance != 0.0)
            {
              // Box should be within tolerance (<, not <=)

              assert(tolerance > 0);
              if (maxBox <= AD::GetPointC(rayBasePoint, dimensionID))
                return std::nullopt;

              if (minBox >= AD::GetPointC(rayBasePoint, dimensionID))
                return std::nullopt;
            }
            else
            {
              if (maxBox < AD::GetPointC(rayBasePoint, dimensionID))
                return std::nullopt;

              if (minBox > AD::GetPointC(rayBasePoint, dimensionID))
                return std::nullopt;
            }

            minBoxDistances[dimensionID] = -inf;
            maxBoxDistances[dimensionID] = +inf;
          }
          else
          {
            auto const pointComp = Geometry(AD::GetPointC(rayBasePoint, dimensionID));
            auto const dirCompRecip = Geometry(1.0) / dirComp;
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

        auto const minBoxDistance = *std::max_element(minBoxDistances.begin(), minBoxDistances.end());
        auto const maxBoxDistance = *std::min_element(maxBoxDistances.begin(), maxBoxDistances.end());
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
          volume *= Geometry(AD::GetBoxMaxC(range, dimensionID) - AD::GetBoxMinC(range, dimensionID));
        }
        return volume;
      }
    };


    template<dim_t DIMENSION_NO, typename TGeometry, typename TVector, typename TBox, typename AD>
    struct GridSpaceIndexing
    {
      template<typename T>
      using DimArray = std::array<T, DIMENSION_NO>;

      using IGM = InternalGeometryModule<DIMENSION_NO, TGeometry, TVector, TBox, AD>;
      using IGM_Geometry = typename IGM::Geometry;


      constexpr GridSpaceIndexing() = default;

      constexpr GridSpaceIndexing(depth_t maxDepthNo, IGM::Box const& boxSpace) noexcept
      : m_maxRasterResolution(detail::pow2<depth_t, GridID>(maxDepthNo))
      , m_maxRasterID(m_maxRasterResolution - 1)
      , m_boxSpace(boxSpace)
      {
        auto const subDivisionNoFactor = IGM_Geometry(m_maxRasterResolution);
        for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
        {
          m_sizeInDimensions[dimensionID] = m_boxSpace.Max[dimensionID] - m_boxSpace.Min[dimensionID];
          auto const isFlat = m_sizeInDimensions[dimensionID] == 0;
          m_rasterizerFactors[dimensionID] = isFlat ? IGM_Geometry(1.0) : (subDivisionNoFactor / m_sizeInDimensions[dimensionID]);
        }

        m_volumeOfOverallSpace = IGM::GetVolumeAD(m_boxSpace);
      }

      constexpr IGM::Vector const& GetSizes() const noexcept { return m_sizeInDimensions; }

      constexpr IGM::Geometry GetVolume() const noexcept { return m_volumeOfOverallSpace; }

      constexpr IGM::Box const& GetBoxSpace() const noexcept { return m_boxSpace; }

      constexpr void Move(IGM::Vector const& moveVector) noexcept { IGM::MoveAD(m_boxSpace, moveVector); }

      constexpr GridID GetResolution() const noexcept { return m_maxRasterResolution; }

      constexpr IGM::Vector CalculateCenter(DimArray<GridID> const& gridID, depth_t centerLevel) const noexcept
      {
        using IGM_Vector = typename IGM::Vector;

        auto const halfGrid = IGM_Geometry(detail::pow2(centerLevel)) * IGM_Geometry(0.5);

        IGM_Vector center;
        LOOPIVDEP
        for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
          center[dimensionID] = (IGM_Geometry(gridID[dimensionID]) + halfGrid) / m_rasterizerFactors[dimensionID] + m_boxSpace.Min[dimensionID];

        return center;
      }

      template<bool HANDLE_OUT_OF_TREE_GEOMETRY = false>
      constexpr DimArray<GridID> GetPointGridID(TVector const& point) const noexcept
      {
        auto gridIDs = DimArray<GridID>{};
        for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
        {
          auto pointComponent = IGM_Geometry(AD::GetPointC(point, dimensionID)) - m_boxSpace.Min[dimensionID];
          if constexpr (HANDLE_OUT_OF_TREE_GEOMETRY)
          {
            if (pointComponent < 0.0)
              pointComponent = 0.0;
          }
          else
          {
            assert(pointComponent >= 0.0);
          }

          auto const rasterID = GridID(pointComponent * m_rasterizerFactors[dimensionID]);
          gridIDs[dimensionID] = std::min<GridID>(m_maxRasterID, rasterID);
        }
        return gridIDs;
      }

      constexpr std::array<DimArray<GridID>, 2> GetEdgePointGridID(TVector const& point) const noexcept
      {
        auto pointMinMaxGridID = std::array<DimArray<GridID>, 2>{};
        for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
        {
          auto const rasterID = (IGM_Geometry(AD::GetPointC(point, dimensionID)) - m_boxSpace.Min[dimensionID]) * m_rasterizerFactors[dimensionID];
          pointMinMaxGridID[0][dimensionID] = pointMinMaxGridID[1][dimensionID] = static_cast<GridID>(rasterID);
          pointMinMaxGridID[0][dimensionID] -= (pointMinMaxGridID[0][dimensionID] > 0) && (floor(rasterID) == rasterID);
        }
        return pointMinMaxGridID;
      }

      template<bool DO_POINT_LIKE_CLASSIFICATION = false, typename TBox_ = TBox>
      constexpr std::array<DimArray<GridID>, 2> GetBoxGridID(TBox_ const& box) const noexcept
      {
        auto gridID = std::array<DimArray<GridID>, 2>{};
        for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
        {
          IGM_Geometry boxMin, boxMax;
          if constexpr (std::is_same_v<TBox_, TBox>)
          {
            boxMin = IGM_Geometry(AD::GetBoxMinC(box, dimensionID));
            boxMax = IGM_Geometry(AD::GetBoxMaxC(box, dimensionID));
          }
          else
          {
            boxMin = box.Min[dimensionID];
            boxMax = box.Max[dimensionID];
          }

          auto const minComponentRasterID = (boxMin - m_boxSpace.Min[dimensionID]) * m_rasterizerFactors[dimensionID];
          auto const maxComponentRasterID = (boxMax - m_boxSpace.Min[dimensionID]) * m_rasterizerFactors[dimensionID];

          if constexpr (DO_POINT_LIKE_CLASSIFICATION)
          {
            gridID[0][dimensionID] = std::min<GridID>(m_maxRasterID, static_cast<GridID>(minComponentRasterID));
            gridID[1][dimensionID] = std::min<GridID>(m_maxRasterID, static_cast<GridID>(maxComponentRasterID));
          }
          else
          {
            if (minComponentRasterID < IGM_Geometry(1))
              gridID[0][dimensionID] = 0;
            else if (minComponentRasterID > m_maxRasterID)
              gridID[0][dimensionID] = m_maxRasterID;
            else
              gridID[0][dimensionID] = static_cast<GridID>(minComponentRasterID);

            if (maxComponentRasterID < IGM_Geometry(1))
              gridID[1][dimensionID] = 0;
            else if (maxComponentRasterID > m_maxRasterID)
              gridID[1][dimensionID] = m_maxRasterID;
            else if (minComponentRasterID != maxComponentRasterID && std::floor(maxComponentRasterID) == maxComponentRasterID)
              gridID[1][dimensionID] = static_cast<GridID>(maxComponentRasterID) - 1;
            else
              gridID[1][dimensionID] = static_cast<GridID>(maxComponentRasterID);
          }
        }
        return gridID;
      }

    private:
      GridID m_maxRasterResolution = {};
      GridID m_maxRasterID = {};

      IGM::Box m_boxSpace = {};
      IGM::Geometry m_volumeOfOverallSpace = {};
      IGM::Vector m_rasterizerFactors = {};
      IGM::Vector m_sizeInDimensions = {};
    };

    template<dim_t DIMENSION_NO>
    struct MortonSpaceIndexing
    {
      // Indexing can be solved with integral types (above this, internal container will be changed to std::map)
      static auto constexpr IS_LINEAR_TREE = DIMENSION_NO < 15;

      // Max number of children
      static auto constexpr CHILD_NO = detail::pow2_ce<DIMENSION_NO>();

      // Max value: 2 ^ DIMENSION_NO
      using UnderlyingInt = std::conditional_t < DIMENSION_NO<4, uint32_t, uint64_t>;
      using ChildID = UnderlyingInt;

      // Max value: 2 ^ nDepth ^ DIMENSION_NO * 2 (signal bit)
      using LinearLocationID = UnderlyingInt;
      using NonLinearLocationID = bitset_arithmetic<DIMENSION_NO * 4 + 1>;
      using LocationID = typename std::conditional_t<IS_LINEAR_TREE, LinearLocationID, NonLinearLocationID>;
      using NodeID = LocationID; // same as the LocationID, but depth is signed by a sentinel bit.
      using LocationIDCR = typename std::conditional_t<IS_LINEAR_TREE, LocationID const, LocationID const&>;
      using NodeIDCR = LocationIDCR;
      template<typename T>
      using DimArray = std::array<T, DIMENSION_NO>;

      // Type system determined maximal depth.
      static auto constexpr MAX_THEORETICAL_DEPTH = static_cast<depth_t>((CHAR_BIT * sizeof(NodeID) - 1 /*sentinal bit*/) / DIMENSION_NO);

      struct DepthAndLocationID
      {
        depth_t DepthID;
        LocationID LocID;
      };

      class ChildLocationGenerator
      {
      public:
        constexpr ChildLocationGenerator(LocationIDCR startLocationID, depth_t examinedLevel) noexcept
        : m_shift(examinedLevel * DIMENSION_NO)
        , m_startLocationID(startLocationID)
        , m_startLocationIDOnExaminedLevel(startLocationID >> m_shift)
        , m_stepNo(LocationID{ 1 } << m_shift)
        {
        }

        // LocationID is on the base grid level
        constexpr ChildID GetChildID(LocationIDCR locationID) const noexcept
        {
          return CastMortonIDToChildID((locationID - m_startLocationID) >> m_shift);
        }

        // LocationID is on a custom depth
        constexpr ChildID GetChildID(DepthAndLocationID const& depthAndLocation, depth_t examinationDepthID) const noexcept
        {
          assert(examinationDepthID <= depthAndLocation.DepthID);
          auto const locationIDOnExaminationLevel = GetLocationIDOnExaminedLevel(depthAndLocation.LocID, depthAndLocation.DepthID - examinationDepthID);
          return CastMortonIDToChildID(locationIDOnExaminationLevel - m_startLocationIDOnExaminedLevel);
        }

        constexpr LocationID GetStartLocationID(ChildID childID) const noexcept { return m_startLocationID + LocationID(childID) * m_stepNo; }

      private:
        UnderlyingInt m_shift;
        LocationIDCR m_startLocationID;
        LocationID m_startLocationIDOnExaminedLevel;
        LocationID m_stepNo;
      };

      class ChildKeyGenerator
      {
      public:
        explicit constexpr ChildKeyGenerator(NodeIDCR parentNodeKey) noexcept
        : m_parentFlag(parentNodeKey << DIMENSION_NO)
        {
        }

        constexpr NodeID GetChildNodeKey(ChildID childID) const noexcept { return m_parentFlag | NodeID(childID); }

      private:
        NodeID m_parentFlag;
      };

      class GridConverter
      {
      public:
        explicit GridConverter(depth_t examinedLevel)
        : m_shift(examinedLevel * DIMENSION_NO)
        {
        }

        LocationID GetLocationID(DimArray<GridID> const& gridID) const noexcept { return Encode(gridID) >> m_shift; }

      private:
        UnderlyingInt m_shift;
      };

      static constexpr NodeID GetHash(auto&& location) noexcept
      {
        assert(location.LocID < (NodeID(1) << (location.DepthID * DIMENSION_NO)));
        return (NodeID{ 1 } << (location.DepthID * DIMENSION_NO)) | location.LocID;
      }

      static constexpr NodeID GetHash(depth_t depth, LocationIDCR locationID) noexcept
      {
        assert(locationID < (NodeID(1) << (depth * DIMENSION_NO)));
        return (NodeID{ 1 } << (depth * DIMENSION_NO)) | locationID;
      }

      static constexpr NodeID GetRootKey() noexcept { return NodeID{ 1 }; }

      static constexpr NodeID GetNoneKey() noexcept { return NodeID{ 0 }; }

      static constexpr bool IsValidKey(LinearLocationID key) noexcept { return key > 0; }

      static inline bool IsValidKey(NonLinearLocationID const& key) noexcept { return key.any(); }

      static constexpr NodeID GetParentKey(NodeIDCR key) noexcept { return key >> DIMENSION_NO; }

      static constexpr LocationID GetParentGridID(LocationIDCR locationID) noexcept { return locationID >> DIMENSION_NO; }

      static constexpr depth_t GetDepthID(NodeID key) noexcept
      {
        // Keep shifting off DIMENSION_NO bits at a time, increasing depth counter
        for (depth_t d = 0; IsValidKey(key); ++d, key = GetParentKey(key))
          if (key == 1) // If only sentinel bit remains, exit with node depth
            return d;

        assert(false); // Bad key
        return 0;
      }

      static constexpr NodeID RemoveSentinelBit(NodeIDCR key, std::optional<depth_t> depthIDOptional = std::nullopt) noexcept
      {
        auto const depthID = depthIDOptional ? *depthIDOptional : GetDepthID(key);
        return key - (NodeID{ 1 } << depthID);
      }

      static constexpr LocationID GetLocationIDOnExaminedLevel(LocationIDCR locationID, depth_t examinationLevel) noexcept
      {
        return locationID >> (examinationLevel * DIMENSION_NO);
      }

      static constexpr bool IsAllChildTouched(std::array<LocationID, 2> const& locationIDRange, depth_t examinationLevel) noexcept
      {
        return IsValidKey((locationIDRange[1] - locationIDRange[0]) >> (examinationLevel * DIMENSION_NO - 1));
      }

    private: // Morton aid functions
      static inline ChildID GetKeyChildPart(NodeIDCR key) noexcept
      {
        if constexpr (IS_LINEAR_TREE)
        {
          auto constexpr maskLastBits1 = (NodeID{ 1 } << DIMENSION_NO) - 1;
          return CastMortonIDToChildID(key & maskLastBits1);
        }
        else
        {
          auto childID = NodeID{};
          for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
            childID.set(dimensionID, key[dimensionID]);

          return CastMortonIDToChildID(childID);
        }
      }

      static constexpr LocationID Part1By2(GridID n) noexcept
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
        if constexpr (IS_LINEAR_TREE)
        {
          return n;
        }
        else
        {
          return LocationID(n);
        }
      }

      // Separates low 16 bits of input by one bit
      static constexpr LocationID Part1By1(GridID n) noexcept
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

        if constexpr (IS_LINEAR_TREE)
        {
          return n;
        }
        else
        {
          return LocationID(n);
        }
      }

    public:
      static inline LocationID Encode(DimArray<GridID> const& gridID) noexcept
      {
        if constexpr (DIMENSION_NO == 1)
          return LocationID(gridID[0]);
        else if constexpr (DIMENSION_NO == 2)
          return (Part1By1(gridID[1]) << 1) + Part1By1(gridID[0]);
        else if constexpr (DIMENSION_NO == 3)
          return (Part1By2(gridID[2]) << 2) + (Part1By2(gridID[1]) << 1) + Part1By2(gridID[0]);
        else
        {
          auto msb = gridID[0];
          for (dim_t dimensionID = 1; dimensionID < DIMENSION_NO; ++dimensionID)
            msb |= gridID[dimensionID];

          LocationID locationID = 0;
          GridID mask = 1;
          for (dim_t i = 0; msb; mask <<= 1, msb >>= 1, ++i)
          {
            LOOPIVDEP
            for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
            {
              auto const shift = dimensionID + i * DIMENSION_NO;
              if constexpr (IS_LINEAR_TREE)
                locationID |= static_cast<LocationID>(gridID[dimensionID] & mask) << (shift - i);
              else
                locationID.set(shift, gridID[dimensionID] & mask);
            }
          }
          return locationID;
        }
      }

      static DimArray<GridID> Decode(NodeIDCR nodeKey, depth_t maxDepthNo) noexcept
      {
        auto gridID = DimArray<GridID>{};
        if constexpr (DIMENSION_NO == 1)
          return { RemoveSentinelBit(nodeKey) << (maxDepthNo - GetDepthID(nodeKey)) };
        else
        {
          auto const depthID = GetDepthID(nodeKey);

          auto constexpr mask = LocationID{ 1 };
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

      static inline ChildID CastMortonIDToChildID(NonLinearLocationID const& bs) noexcept
      {
        assert(bs <= NonLinearLocationID(std::numeric_limits<ChildID>::max()));
        return bs.to_ullong();
      }

      static constexpr ChildID CastMortonIDToChildID(LinearLocationID morton) noexcept { return morton; }

      static constexpr ChildID GetChildIDByDepth(depth_t parentDepth, depth_t childDepth, LocationIDCR childNodeKey)
      {
        auto const depthDifference = childDepth - parentDepth;
        assert(depthDifference > 0);
        return GetKeyChildPart(childNodeKey >> (DIMENSION_NO * (depthDifference - 1)));
      }

      static constexpr bool IsChildInGreaterSegment(ChildID childID, dim_t dimensionID) noexcept { return (ChildID{ 1 } << dimensionID) & childID; }

      static constexpr std::array<LocationID, 2> GetRangeLocationID(std::array<DimArray<GridID>, 2> const& gridIDRange) noexcept
      {
        return { Encode(gridIDRange[0]), Encode(gridIDRange[1]) };
      }

      static constexpr DepthAndLocationID GetDepthAndLocationID(depth_t maxDepthNo, std::array<LocationID, 2> const& locationIDRange) noexcept
      {
        auto dl = DepthAndLocationID{ maxDepthNo, locationIDRange[0] };

        for (auto diffLocationFlag = locationIDRange[0] ^ locationIDRange[1]; IsValidKey(diffLocationFlag); diffLocationFlag >>= DIMENSION_NO, --dl.DepthID)
          dl.LocID = GetParentGridID(dl.LocID);

        return dl;
      }

      static constexpr DepthAndLocationID GetDepthAndLocationID(depth_t maxDepthNo, std::array<DimArray<GridID>, 2> const& gridIDRange) noexcept
      {
        return GetDepthAndLocationID(maxDepthNo, GetRangeLocationID(gridIDRange));
      }

      static constexpr NodeID GetNodeID(depth_t maxDepthNo, std::array<DimArray<GridID>, 2> const& gridIDRange) noexcept
      {
        return GetHash(GetDepthAndLocationID(maxDepthNo, gridIDRange));
      }

      static constexpr NodeID GetNodeID(depth_t maxDepthNo, std::array<LocationID, 2> const& locationIDRange) noexcept
      {
        return GetHash(GetDepthAndLocationID(maxDepthNo, locationIDRange));
      }

      static constexpr auto IsLess(DepthAndLocationID const& leftLocation, DepthAndLocationID const& rightLocation) noexcept
      {
        if (leftLocation.DepthID == rightLocation.DepthID)
          return leftLocation.LocID < rightLocation.LocID;
        else if (leftLocation.DepthID < rightLocation.DepthID)
        {
          auto const locationIDRight = GetLocationIDOnExaminedLevel(rightLocation.LocID, rightLocation.DepthID - leftLocation.DepthID);
          return leftLocation.LocID <= locationIDRight;
        }
        else
        {
          auto const locationIDLeft = GetLocationIDOnExaminedLevel(leftLocation.LocID, leftLocation.DepthID - rightLocation.DepthID);
          return locationIDLeft < rightLocation.LocID;
        }
      }
    };
  } // namespace detail

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
    static auto constexpr IS_BOX_TYPE = std::is_same_v<TEntity_, TBox_>;
    static auto constexpr IS_CONTIGOUS_CONTAINER = std::contiguous_iterator<typename TContainer_::iterator>;

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

    template<typename T>
    using DimArray = std::array<T, DIMENSION_NO>;
    using IGM = typename detail::InternalGeometryModule<DIMENSION_NO, TGeometry, TVector, TBox, TAdapter>;
    using IGM_Geometry = typename IGM::Geometry;

    using SI = detail::MortonSpaceIndexing<DIMENSION_NO>;
    using MortonNodeID = typename SI::NodeID;
    using MortonNodeIDCR = typename SI::NodeIDCR;
    using MortonLocationID = typename SI::LocationID;
    using MortonLocationIDCR = typename SI::LocationIDCR;
    using MortonChildID = typename SI::ChildID;

  public:
    class Node
    {
    private:
      std::vector<MortonNodeID> m_children;
#ifndef ORTHOTREE__DISABLED_NODECENTER
      IGM::Vector m_center;
#endif

    public: // Public members
      std::vector<TEntityID> Entities = {};

    public:
#ifndef ORTHOTREE__DISABLED_NODECENTER
      constexpr IGM::Vector const& GetCenter() const noexcept { return m_center; }
      constexpr void SetCenter(IGM::Vector&& center) noexcept { m_center = std::move(center); }
#endif // !ORTHOTREE__DISABLED_NODECENTER

      constexpr void AddChild(MortonNodeIDCR childKey) noexcept { m_children.emplace_back(childKey); }

      constexpr void AddChildInOrder(MortonNodeIDCR childKey) noexcept
      {
        auto it = std::end(m_children);
        if constexpr (SI::IS_LINEAR_TREE)
          it = std::lower_bound(m_children.begin(), m_children.end(), childKey);
        else
          it = std::lower_bound(m_children.begin(), m_children.end(), childKey, bitset_arithmetic_compare{});

        if (it != m_children.end() && *it == childKey)
          return;

        m_children.insert(it, childKey);
      }

      constexpr bool HasChild(MortonNodeIDCR childKey) const noexcept
      {
        if constexpr (SI::IS_LINEAR_TREE)
          return std::binary_search(m_children.begin(), m_children.end(), childKey);
        else
          return std::binary_search(m_children.begin(), m_children.end(), childKey, bitset_arithmetic_compare{});
      }

      constexpr bool IsChildNodeEnabled(MortonChildID childID) const noexcept
      {
        auto const childMortonID = MortonNodeID(childID);
        return std::find_if(m_children.begin(), m_children.end(), [childMortonID](auto const& childKey) {
          return (childKey & childMortonID) == childMortonID;
        });
      }

      constexpr void RemoveChild(MortonNodeIDCR childKey) noexcept
      {
        auto it = std::end(m_children);
        if constexpr (SI::IS_LINEAR_TREE)
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
      IGM_Geometry Distance;
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


    template<typename TData>
    using LinearUnderlyingContainer = std::unordered_map<MortonNodeID, TData>;

    template<typename TData>
    using NonLinearUnderlyingContainer = std::map<MortonNodeID, TData, bitset_arithmetic_compare>;

    template<typename TData>
    using UnderlyingContainer = typename std::conditional_t<SI::IS_LINEAR_TREE, LinearUnderlyingContainer<TData>, NonLinearUnderlyingContainer<TData>>;

  protected: // Member variables
    UnderlyingContainer<Node> m_nodes;

    std::size_t m_maxElementNo = 11;
    depth_t m_maxDepthNo = {};

    std::vector<typename IGM::Vector> m_nodeSizes;

    detail::GridSpaceIndexing<DIMENSION_NO, TGeometry, TVector, TBox, AD> m_grid;

  public: // Node helpers
    // Calculate extent by box of the tree and the key of the node
    constexpr IGM::Vector CalculateCenter(MortonNodeIDCR key) const noexcept
    {
      auto const gridID = SI::Decode(key, GetDepthMax());
      auto const centerLevel = GetDepthMax() - SI::GetDepthID(key);
      return m_grid.CalculateCenter(gridID, centerLevel);
    }

#ifdef ORTHOTREE__DISABLED_NODECENTER
    constexpr IGM::Vector GetNodeCenter(MortonNodeIDCR key) const noexcept { return CalculateCenter(key); }
#define GetNodeCenterMacro(inst, key, node) inst->GetNodeCenter(key)
#else
    inline IGM::Vector const& GetNodeCenter(MortonNodeIDCR key) const noexcept { return this->GetNode(key).GetCenter(); }
#define GetNodeCenterMacro(inst, key, node) node.GetCenter()
#endif // ORTHOTREE__DISABLED_NODECENTER

#ifdef ORTHOTREE__DISABLED_NODESIZE
    constexpr IGM::Vector GetNodeSize(depth_t depthID) const noexcept
    {
      auto const depthFactor = IGM_Geometry(1.0) / IGM_Geometry(detail::pow2(depthID));
      auto const& spaceSizes = this->m_grid.GetSizes();
      typename IGM::Vector size;
      LOOPIVDEP
      for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
        size[dimensionID] = spaceSizes[dimensionID] * depthFactor;

      return size;
    }
#else
    constexpr IGM::Vector const& GetNodeSize(depth_t depthID) const noexcept { return this->m_nodeSizes[depthID]; }
#endif // ORTHOTREE__DISABLED_NODESIZE

    constexpr IGM::Vector const& GetNodeSizeByKey(MortonNodeIDCR key) const noexcept { return this->GetNodeSize(SI::GetDepthID(key)); }

    constexpr IGM::Box GetNodeBox(depth_t depthID, IGM::Vector const& center) const noexcept
    {
      auto const& halfSize = this->GetNodeSize(depthID + 1); // +1: half size will be required
      typename IGM::Box box{ .Min = center, .Max = center };

      LOOPIVDEP
      for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
      {
        box.Min[dimensionID] -= halfSize[dimensionID];
        box.Max[dimensionID] += halfSize[dimensionID];
      }

      return box;
    }

    constexpr IGM::Box GetNodeBox(MortonNodeIDCR key) const noexcept { return this->GetNodeBox(SI::GetDepthID(key), this->GetNodeCenter(key)); }

  protected:
    inline Node& CreateChild(Node& parentNode, MortonChildID childID, MortonNodeIDCR childKey) noexcept
    {
      assert(childID < SI::CHILD_NO);
      auto& nodeChild = m_nodes[childKey];

#ifndef ORTHOTREE__DISABLED_NODECENTER
      auto const depthID = SI::GetDepthID(childKey);
      auto const& halfSizes = this->GetNodeSize(depthID + 1);
      auto const& parentCenter = parentNode.GetCenter();

      typename IGM::Vector childCenter;
      LOOPIVDEP
      for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
      {
        auto const isGreater = SI::IsChildInGreaterSegment(childID, dimensionID);
        auto const sign = IGM_Geometry(isGreater * 2 - 1);
        childCenter[dimensionID] = parentCenter[dimensionID] + sign * halfSizes[dimensionID];
      }

      nodeChild.SetCenter(std::move(childCenter));
#endif // ORTHOTREE__DISABLED_NODECENTER
      return nodeChild;
    }

    template<bool HANDLE_OUT_OF_TREE_GEOMETRY = false>
    constexpr MortonLocationID GetLocationID(TVector const& point) const noexcept
    {
      return SI::Encode(this->m_grid.template GetPointGridID<HANDLE_OUT_OF_TREE_GEOMETRY>(point));
    }

    template<bool HANDLE_OUT_OF_TREE_GEOMETRY = false>
    constexpr SI::DepthAndLocationID GetDepthAndLocationID(TVector const& point) const noexcept
    {
      return { this->m_maxDepthNo, this->GetLocationID<HANDLE_OUT_OF_TREE_GEOMETRY>(point) };
    }

    template<bool HANDLE_OUT_OF_TREE_GEOMETRY = false, typename TBoxItem = TBox>
    constexpr SI::DepthAndLocationID GetDepthAndLocationID(TBoxItem const& box) const noexcept
    {
      return SI::GetDepthAndLocationID(this->m_maxDepthNo, this->m_grid.template GetBoxGridID<HANDLE_OUT_OF_TREE_GEOMETRY, TBoxItem>(box));
    }

    static inline TBox GetBoxInvertedInit() noexcept
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

    bool IsEveryEntityUnique() const noexcept
    {
      auto ids = std::vector<TEntityID>();
      ids.reserve(100);
      std::for_each(m_nodes.begin(), m_nodes.end(), [&](auto& node) {
        ids.insert(ids.end(), node.second.Entities.begin(), node.second.Entities.end());
      });

      auto const idsSizeBeforeUnique = ids.size();
      detail::sortAndUnique(ids);
      return idsSizeBeforeUnique == ids.size();
    }

    template<bool DO_UNIQUENESS_CHECK_TO_INDICIES>
    bool InsertWithRebalancingBase(
      MortonNodeIDCR parentNodeKey,
      depth_t parentDepth,
      MortonNodeIDCR entitiyNodeKey,
      depth_t entityDepth,
      TEntityID newEntityID,
      TContainer const& geometryCollection) noexcept
    {
      auto& parentNode = this->m_nodes.at(parentNodeKey);
      auto const shouldInsertInParentNode = entitiyNodeKey == parentNodeKey;

      enum class ControlFlow
      {
        InsertInParentNode,
        ShouldCreateOnlyOneChild,
        FullRebalancing,
      };

      auto const cf = [&] {
        if (parentDepth < this->m_maxDepthNo)
        {
          auto const isParentNotLeafNode = parentNode.IsAnyChildExist();
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

        auto const childGenerator = typename SI::ChildKeyGenerator(parentNodeKey);
        auto const childID = SI::GetChildIDByDepth(parentDepth, entityDepth, entitiyNodeKey);
        auto const childNodeKey = childGenerator.GetChildNodeKey(childID);

        parentNode.AddChildInOrder(childNodeKey);
        auto& childNode = this->CreateChild(parentNode, childID, childNodeKey);
        childNode.Entities.emplace_back(newEntityID);

        break;
      }

      case ControlFlow::FullRebalancing: {
        auto const childGenerator = typename SI::ChildKeyGenerator(parentNodeKey);

        if (!shouldInsertInParentNode)
        {
          auto const childID = SI::GetChildIDByDepth(parentDepth, entityDepth, entitiyNodeKey);
          auto const childNodeKey = childGenerator.GetChildNodeKey(childID);

          parentNode.AddChildInOrder(childNodeKey);
          auto& childNode = this->CreateChild(parentNode, childID, childNodeKey);
          childNode.Entities.emplace_back(newEntityID);
        }

        size_t remainingEntityNo = parentNode.Entities.size();
        for (size_t i = 0; i < remainingEntityNo; ++i)
        {
          auto entityID = parentNode.Entities[i];
          auto const [depthID, locationID] = this->GetDepthAndLocationID(geometryCollection[entityID]);
          if (depthID <= parentDepth)
            continue;

          auto const childID = SI::GetChildIDByDepth(parentDepth, depthID, locationID);
          auto const childNodeKey = childGenerator.GetChildNodeKey(childID);
          if (parentNode.HasChild(childNodeKey))
          {
            auto const entitiyNodeKey_ = SI::GetHash(depthID, locationID);
            auto const [parentNodeKey_, parentDepthID_] = FindSmallestNodeKeyWithDepth(entitiyNodeKey_);
            InsertWithRebalancingBase<false>(parentNodeKey_, parentDepthID_, entitiyNodeKey_, depthID, entityID, geometryCollection);
          }
          else
          {
            parentNode.AddChildInOrder(childNodeKey);
            auto& childNode = this->CreateChild(parentNode, childID, childNodeKey);
            childNode.Entities.emplace_back(entityID);
          }

          if constexpr (IS_BOX_TYPE)
          {
            --remainingEntityNo;
            parentNode.Entities[i] = parentNode.Entities[remainingEntityNo];
            --i;
          }
        }

        if constexpr (!IS_BOX_TYPE)
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
        assert(this->IsEveryEntityUnique()); // Assert means: index is already added. Wrong input!

      return true;
    }

    template<bool DO_UNIQUENESS_CHECK_TO_INDICIES>
    bool InsertWithoutRebalancingBase(MortonNodeIDCR parentNodeKey, MortonNodeIDCR entityNodeKey, TEntityID entityID, bool doInsertToLeaf) noexcept
    {
      if (entityNodeKey == parentNodeKey)
      {
        detail::at(this->m_nodes, entityNodeKey).Entities.emplace_back(entityID);
        if constexpr (DO_UNIQUENESS_CHECK_TO_INDICIES)
          assert(this->IsEveryEntityUnique()); // Assert means: index is already added. Wrong input!
        return true;
      }

      if (doInsertToLeaf)
      {
        auto& newNode = this->m_nodes[entityNodeKey];
        newNode.Entities.emplace_back(entityID);
#ifndef ORTHOTREE__DISABLED_NODECENTER
        newNode.SetCenter(this->CalculateCenter(entityNodeKey));
#endif
        // Create all child between the new (entityNodeKey) and the smallest existing one (parentNodeKey)
        auto newParentNodeKey = entityNodeKey;
        do
        {
          auto childNodeKey = newParentNodeKey;
          newParentNodeKey = SI::GetParentKey(newParentNodeKey);
          assert(SI::IsValidKey(parentNodeKey));
          auto& newParentNode = this->m_nodes[newParentNodeKey];
          newParentNode.AddChildInOrder(childNodeKey);
#ifndef ORTHOTREE__DISABLED_NODECENTER
          newParentNode.SetCenter(this->CalculateCenter(newParentNodeKey));
#endif

        } while (newParentNodeKey != parentNodeKey);
      }
      else
      {
        auto& parentNode = detail::at(this->m_nodes, parentNodeKey);
        if (parentNode.IsAnyChildExist())
        {
          auto const parentDepth = SI::GetDepthID(parentNodeKey);
          auto const childID = SI::GetChildIDByDepth(parentDepth, SI::GetDepthID(entityNodeKey), entityNodeKey);
          auto const childGenerator = typename SI::ChildKeyGenerator(parentNodeKey);
          auto const childNodeKey = childGenerator.GetChildNodeKey(childID);

          parentNode.AddChildInOrder(childNodeKey);
          auto& nodeChild = this->CreateChild(parentNode, childID, childNodeKey);
          nodeChild.Entities.emplace_back(entityID);
        }
        else
          parentNode.Entities.emplace_back(entityID);
      }

      if constexpr (DO_UNIQUENESS_CHECK_TO_INDICIES)
        assert(this->IsEveryEntityUnique()); // Assert means: index is already added. Wrong input!

      return true;
    }

    void RemoveNodeIfPossible(MortonNodeIDCR nodeKey, Node const& node) noexcept
    {
      if (nodeKey == SI::GetRootKey())
        return;

      if (node.IsAnyChildExist() || !node.Entities.empty())
        return;

      auto const parentKey = SI::GetParentKey(nodeKey);
      auto& parentNode = detail::at(this->m_nodes, parentKey);
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

      auto constexpr rMult = 1.5;
      constexpr depth_t bitSize = sizeof(std::size_t) * CHAR_BIT;
      if ((maxDepthNo + 1) * DIMENSION_NO < bitSize)
      {
        auto const nMaxChild = detail::pow2(maxDepthNo * DIMENSION_NO);
        auto const nElementInNode = elementNo / nMaxChild;
        if (nElementInNode > maxElementNo / 2)
          return nMaxChild;
      }

      auto const nElementInNodeAvg = static_cast<float>(elementNo) / static_cast<float>(maxElementNo);
      auto const nDepthEstimated = std::min(maxDepthNo, static_cast<depth_t>(ceil((log2f(nElementInNodeAvg) + 1.0) / static_cast<float>(DIMENSION_NO))));
      if (nDepthEstimated * DIMENSION_NO < 64)
        return static_cast<std::size_t>(rMult * detail::pow2(nDepthEstimated * DIMENSION_NO));

      return static_cast<std::size_t>(rMult * nElementInNodeAvg);
    }

    static inline depth_t EstimateMaxDepth(std::size_t elementNo, std::size_t maxElementNo) noexcept
    {
      if (elementNo < maxElementNo)
        return 2;

      auto const nLeaf = elementNo / maxElementNo;
      // nLeaf = (2^nDepth)^DIMENSION_NO
      return std::clamp(static_cast<depth_t>(std::log2(nLeaf) / static_cast<double>(DIMENSION_NO)), depth_t(2), depth_t(10));
    }


  public: // Getters
    constexpr auto const& GetNodes() const noexcept { return m_nodes; }
    inline bool HasNode(MortonNodeIDCR key) const noexcept { return m_nodes.contains(key); }
    inline auto const& GetNode(MortonNodeIDCR key) const noexcept { return m_nodes.at(key); }
    constexpr auto const& GetBox() const noexcept { return m_grid.GetBoxSpace(); }
    constexpr auto GetDepthMax() const noexcept { return m_maxDepthNo; }
    constexpr auto GetResolutionMax() const noexcept { return m_grid.GetResolution(); }
    inline auto GetNodeIDByEntity(TEntityID entityID) const noexcept
    {
      auto const it = std::find_if(m_nodes.begin(), m_nodes.end(), [&](auto const& keyAndValue) {
        auto const& entities = keyAndValue.second.Entities;
        return std::find(entities.begin(), entities.end(), entityID) != entities.end();
      });

      return it == m_nodes.end() ? MortonNodeID{} : it->first;
    }

  protected:
    // Alternative creation mode (instead of Create), Init then Insert items into leafs one by one. NOT RECOMMENDED.
    constexpr void InitBase(IGM::Box const& boxSpace, depth_t maxDepthNo, std::size_t maxElementNo) noexcept
    {
      assert(this->m_nodes.empty()); // To build/setup/create the tree, use the Create() [recommended] or Init() function. If an already builded
                                     // tree is wanted to be reset, use the Reset() function before init.
      assert(maxDepthNo > 1);
      assert(maxDepthNo <= SI::MAX_THEORETICAL_DEPTH);
      assert(maxDepthNo < std::numeric_limits<uint8_t>::max());
      assert(maxElementNo > 1);
      assert(CHAR_BIT * sizeof(GridID) >= m_maxDepthNo);

      this->m_grid = detail::GridSpaceIndexing<DIMENSION_NO, TGeometry, TVector, TBox, AD>(maxDepthNo, boxSpace);
      this->m_maxDepthNo = maxDepthNo;
      this->m_maxElementNo = maxElementNo;

      [[maybe_unused]] auto& nodeRoot = this->m_nodes[SI::GetRootKey()];
#ifndef ORTHOTREE__DISABLED_NODECENTER
      nodeRoot.SetCenter(IGM::GetBoxCenter(boxSpace));
#endif // !ORTHOTREE__DISABLED_NODECENTER

      // the 0-based depth size of the tree is m_maxDepthNo+1, and a fictive childnode halfsize (+2) could be asked prematurely.
      depth_t constexpr additionalDepth = 3;
      auto const examinedDepthSize = this->m_maxDepthNo + additionalDepth;
      this->m_nodeSizes.resize(examinedDepthSize, this->m_grid.GetSizes());
      auto constexpr multiplier = IGM_Geometry(0.5);
      auto factor = multiplier;
      for (depth_t depthID = 1; depthID < examinedDepthSize; ++depthID, factor *= multiplier)
        for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
          this->m_nodeSizes[depthID][dimensionID] *= factor;
    }

  public: // Main service functions
    // Alternative creation mode (instead of Create), Init then Insert items into leafs one by one. NOT RECOMMENDED.
    constexpr void Init(TBox const& box, depth_t maxDepthNo, std::size_t maxElementNo = 11) noexcept
    {
      this->InitBase(IGM::GetBoxAD(box), maxDepthNo, maxElementNo);
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
        auto const& key = nodeIDsToProceed.front();
        auto const& node = GetNode(key);
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
        auto const& [key, doAvoidSelectionParent] = nodesToProceed.front();

        auto const& node = GetNode(key);
        if (!doAvoidSelectionParent && !selector(key, node))
          continue;

        auto const doAvoidSelection = doAvoidSelectionParent || selectorUnconditional(key, node);
        procedure(key, node, doAvoidSelection);

        for (MortonNodeIDCR childKey : node.GetChildren())
          nodesToProceed.push({ childKey, doAvoidSelection });
      }
    }


    // Visit nodes with special selection and procedure in depth-first search order
    void VisitNodesInDFS(MortonNodeIDCR key, FProcedure const& procedure, FSelector const& selector) const noexcept
    {
      auto const& node = GetNode(key);
      if (!selector(key, node))
        return;

      procedure(key, node);
      for (MortonNodeIDCR childKey : node.GetChildren())
        VisitNodesInDFS(childKey, procedure, selector);
    }


    // Collect all item id, traversing the tree in breadth-first search order
    std::vector<TEntityID> CollectAllEntitiesInBFS(MortonNodeIDCR rootKey = SI::GetRootKey()) const noexcept
    {
      auto entityIDs = std::vector<TEntityID>();
      entityIDs.reserve(m_nodes.size() * std::max<std::size_t>(2, m_maxElementNo / 2));

      VisitNodes(rootKey, [&entityIDs](MortonNodeIDCR, auto const& node) {
        entityIDs.insert(entityIDs.end(), node.Entities.begin(), node.Entities.end());
      });
      return entityIDs;
    }

  private:
    void CollectAllEntitiesInDFSRecursive(Node const& parentNode, std::vector<TEntityID>& foundEntities) const noexcept
    {
      foundEntities.insert(foundEntities.end(), parentNode.Entities.begin(), parentNode.Entities.end());
      for (MortonNodeIDCR childKey : parentNode.GetChildren())
        CollectAllEntitiesInDFSRecursive(this->GetNode(childKey), foundEntities);
    }

  public:
    std::vector<TEntityID> CollectAllEntitiesInDFS(MortonNodeIDCR parentKey = SI::GetRootKey()) const noexcept
    {
      auto entityIDs = std::vector<TEntityID>{};
      CollectAllEntitiesInDFSRecursive(GetNode(parentKey), entityIDs);
      return entityIDs;
    }

    // Update all element which are in the given hash-table.
    template<bool IS_PARALLEL_EXEC = false, bool DO_UNIQUENESS_CHECK_TO_INDICIES = false>
    void UpdateIndexes(std::unordered_map<TEntityID, std::optional<TEntityID>> const& updateMap) noexcept
    {
      auto const updateMapEndIterator = updateMap.end();

      EXEC_POL_DEF(ep);
      std::for_each(EXEC_POL_ADD(ep) m_nodes.begin(), m_nodes.end(), [&](auto& node) {
        decltype(Node::Entities) entitiesNew;
        for (auto const& id : node.second.Entities)
        {
          auto const it = updateMap.find(id);
          if (it == updateMapEndIterator)
            entitiesNew.emplace_back(id);
          else if (it->second)
            entitiesNew.emplace_back(*it->second);
        }

        node.second.Entities = std::move(entitiesNew);
      });

      if constexpr (DO_UNIQUENESS_CHECK_TO_INDICIES)
        assert(IsEveryEntityUnique()); // Assert means: index replacements causes that multiple object has the same id. Wrong input!
    }


    // Reset the tree
    void Reset() noexcept
    {
      m_nodes.clear();
      m_grid = {};
    }


    // Remove all elements and ids, except Root
    void Clear() noexcept
    {
      std::erase_if(m_nodes, [](auto const& p) { return p.first != SI::GetRootKey(); });
      detail::at(m_nodes, SI::GetRootKey()).Entities.clear();
    }


    // Move the whole tree with a std::vector of the movement
    template<bool IS_PARALLEL_EXEC = false>
    void Move(TVector const& moveVector) noexcept
    {
#ifndef ORTHOTREE__DISABLED_NODECENTER
      EXEC_POL_DEF(ep); // GCC 11.3
      std::for_each(EXEC_POL_ADD(ep) m_nodes.begin(), m_nodes.end(), [&moveVector](auto& pairKeyNode) {
        auto center = pairKeyNode.second.GetCenter();
        IGM::MoveAD(center, moveVector);
        pairKeyNode.second.SetCenter(std::move(center));
      });
#endif // !ORTHOTREE__DISABLED_NODECENTER
      m_grid.Move(moveVector);
    }

    std::tuple<MortonNodeID, depth_t> FindSmallestNodeKeyWithDepth(MortonNodeID searchKey) const noexcept
    {
      for (depth_t depthID = this->m_maxDepthNo; SI::IsValidKey(searchKey); searchKey = SI::GetParentKey(searchKey), --depthID)
        if (this->m_nodes.contains(searchKey))
          return { searchKey, depthID };

      return {}; // Not found
    }

    MortonNodeID FindSmallestNodeKey(MortonNodeID searchKey) const noexcept
    {
      for (; SI::IsValidKey(searchKey); searchKey = SI::GetParentKey(searchKey))
        if (this->m_nodes.contains(searchKey))
          return searchKey;

      return MortonNodeID{}; // Not found
    }

    // Get Node ID of a point
    template<bool HANDLE_OUT_OF_TREE_GEOMETRY = false>
    MortonNodeID GetNodeID(TVector const& searchPoint) const noexcept
    {
      return SI::GetHash(this->GetDepthAndLocationID<HANDLE_OUT_OF_TREE_GEOMETRY>(searchPoint));
    }

    // Get Node ID of a box
    template<bool HANDLE_OUT_OF_TREE_GEOMETRY = false>
    MortonNodeID GetNodeID(TBox const& box) const noexcept
    {
      return SI::GetHash(this->GetDepthAndLocationID<HANDLE_OUT_OF_TREE_GEOMETRY>(box));
    }

    // Find smallest node which contains the box
    template<bool HANDLE_OUT_OF_TREE_GEOMETRY = false>
    MortonNodeID FindSmallestNode(TVector const& searchPoint) const noexcept
    {
      if constexpr (!HANDLE_OUT_OF_TREE_GEOMETRY)
      {
        if (!IGM::DoesBoxContainPointAD(this->m_grid.GetBoxSpace(), searchPoint))
          return MortonNodeID{};
      }
      return this->FindSmallestNodeKey(this->GetNodeID<HANDLE_OUT_OF_TREE_GEOMETRY>(searchPoint));
    }

    // Find smallest node which contains the box
    MortonNodeID FindSmallestNode(TBox const& box) const noexcept
    {
      if (!IGM::DoesRangeContainBoxAD(this->m_grid.GetBoxSpace(), box))
        return MortonNodeID{};

      return FindSmallestNodeKey(this->GetNodeID(box));
    }

    MortonNodeID Find(TEntityID entityID) const noexcept { return GetNodeIDByEntity(entityID); }

  protected:
    struct GridBoundary
    {
      GridID MinGridID, BeginGridID, EndGridID;
    };
    template<dim_t DIMENSION_ID>
    static constexpr void ConstructGridIDListRecursively(
      GridID gridStepNo, DimArray<GridBoundary> const& gridIDBoundaries, DimArray<GridID>& currentGridID, std::vector<DimArray<GridID>>& allGridID) noexcept
    {
      if constexpr (DIMENSION_ID == 0)
        allGridID.emplace_back(currentGridID);
      else
      {
        auto const& [minGridID, beginGridID, endGridID] = gridIDBoundaries[DIMENSION_ID - 1];
        currentGridID[DIMENSION_ID - 1] = minGridID;
        ConstructGridIDListRecursively<DIMENSION_ID - 1>(gridStepNo, gridIDBoundaries, currentGridID, allGridID);
        for (auto gridID = beginGridID; gridID < endGridID; ++gridID)
        {
          currentGridID[DIMENSION_ID - 1] = gridID * gridStepNo;
          ConstructGridIDListRecursively<DIMENSION_ID - 1>(gridStepNo, gridIDBoundaries, currentGridID, allGridID);
        }
      }
    }


    template<bool DO_RANGE_MUST_FULLY_CONTAIN = false>
    constexpr void RangeSearchBaseCopy(
      TBox const& range, TContainer const& geometryCollection, Node const& parentNode, std::vector<TEntityID>& foundEntities) const noexcept
    {
      for (auto const entityID : parentNode.Entities)
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

    struct OverlappingSpaceSegments
    {
      // Flags to sign the overlapped segments dimension-wise
      MortonLocationID minSegmentFlag{}, maxSegmentFlag{};
    };

    static constexpr OverlappingSpaceSegments GetRelativeMinMaxLocation(IGM::Vector const& center, TBox const& range) noexcept
    {
      auto overlappedSegments = OverlappingSpaceSegments{};
      auto segmentBit = MortonLocationID{ 1 };
      for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID, segmentBit <<= 1)
      {
        overlappedSegments.minSegmentFlag |= segmentBit * (center[dimensionID] <= AD::GetBoxMinC(range, dimensionID));
        overlappedSegments.maxSegmentFlag |= segmentBit * (center[dimensionID] <= AD::GetBoxMaxC(range, dimensionID));
      }
      return overlappedSegments;
    }

    template<bool DO_RANGE_MUST_FULLY_CONTAIN = false>
    void RangeSearchBase(
      TBox const& range,
      TContainer const& geometryCollection,
      depth_t depthID,
      MortonNodeIDCR const& currentNodeKey,
      std::vector<TEntityID>& foundEntities) const noexcept
    {
      auto const& currentNode = this->GetNode(currentNodeKey);
      if (!currentNode.IsAnyChildExist())
      {
        RangeSearchBaseCopy<DO_RANGE_MUST_FULLY_CONTAIN>(range, geometryCollection, currentNode, foundEntities);
        return;
      }

      auto const& center = GetNodeCenterMacro(this, currentNodeKey, currentNode);
      auto const [minSegmentFlag, maxSegmentFlag] = GetRelativeMinMaxLocation(center, range);

      // Different min-max bit means: the dimension should be totally walked
      // Same min-max bit means: only the min or max should be walked

      // The key will have signal bit also, dimensionMask is applied to calculate only the last, dimension part of the key
      auto const dimensionMask = MortonLocationID{ SI::CHILD_NO - std::size_t{ 1 } };

      // Sign the dimensions which should not be walked fully
      auto const limitedDimensionsMask = (~(minSegmentFlag ^ maxSegmentFlag)) & dimensionMask;

      if (limitedDimensionsMask == MortonLocationID{} && IGM::DoesRangeContainBoxAD(range, this->GetNodeBox(depthID, center)))
      {
        CollectAllEntitiesInDFSRecursive(currentNode, foundEntities);
        return;
      }

      RangeSearchBaseCopy<DO_RANGE_MUST_FULLY_CONTAIN>(range, geometryCollection, currentNode, foundEntities);

      // Sign which element should be walked in the limited dimensions
      auto const dimensionBoundaries = (minSegmentFlag & maxSegmentFlag) & limitedDimensionsMask;

      ++depthID;
      for (MortonNodeIDCR keyChild : currentNode.GetChildren())
      {
        // keyChild should have the same elements in the limited dimensions
        auto const isOverlapped = (keyChild & limitedDimensionsMask) == dimensionBoundaries;
        if (!isOverlapped)
          continue;

        RangeSearchBase<DO_RANGE_MUST_FULLY_CONTAIN>(range, geometryCollection, depthID, keyChild, foundEntities);
      }
    }

    template<bool DO_RANGE_MUST_FULLY_CONTAIN = false, bool DOES_LEAF_NODE_CONTAIN_ELEMENT_ONLY = true>
    bool RangeSearchBaseRoot(TBox const& range, TContainer const& geometryCollection, std::vector<TEntityID>& foundEntities) const noexcept
    {
      auto const entityNo = geometryCollection.size();
      if (IGM::DoesRangeContainBoxAD(range, this->m_grid.GetBoxSpace()))
      {
        foundEntities.resize(entityNo);

        if constexpr (IS_CONTIGOUS_CONTAINER)
          std::iota(foundEntities.begin(), foundEntities.end(), 0);
        else
          std::transform(geometryCollection.begin(), geometryCollection.end(), foundEntities.begin(), [&geometryCollection](auto const& item) {
            return detail::getKeyPart(geometryCollection, item);
          });

        return entityNo > 0;
      }

      // If the range has zero volume, it could stuck at any node comparison with point/side touch. It is eliminated to work node bounding box independently.
      const auto rangeVolume = IGM::GetVolumeAD(range);
      if (rangeVolume <= 0.0)
      {
        return false;
      }

      auto const rangeKey = this->GetNodeID<!IS_BOX_TYPE>(range);
      auto smallestNodeKey = this->FindSmallestNodeKey(rangeKey);
      if (!SI::IsValidKey(smallestNodeKey))
        return false;

      auto const foundEntityNoEstimation =
        this->m_grid.GetVolume() < 0.01 ? 10 : static_cast<std::size_t>((rangeVolume * entityNo) / this->m_grid.GetVolume());

      foundEntities.reserve(foundEntityNoEstimation);
      RangeSearchBase<DO_RANGE_MUST_FULLY_CONTAIN>(range, geometryCollection, SI::GetDepthID(smallestNodeKey), smallestNodeKey, foundEntities);

      if constexpr (!DOES_LEAF_NODE_CONTAIN_ELEMENT_ONLY)
      {
        for (smallestNodeKey = SI::GetParentKey(smallestNodeKey); SI::IsValidKey(smallestNodeKey); smallestNodeKey = SI::GetParentKey(smallestNodeKey))
          RangeSearchBaseCopy<DO_RANGE_MUST_FULLY_CONTAIN>(range, geometryCollection, this->GetNode(smallestNodeKey), foundEntities);
      }

      return true;
    }

    template<typename TBoxRange = TBox>
    std::vector<MortonNodeID> RangeSearchNodes(TBoxRange const& range, MortonNodeIDCR excludeNodeKey = SI::GetNoneKey()) const noexcept
    {
      std::vector<MortonNodeID> foundNodes;

      auto const rangeVolume = IGM::GetVolumeAD(range);
      if (rangeVolume <= 0.0)
      {
        return foundNodes;
      }

      auto const rangeKey = SI::GetHash(this->GetDepthAndLocationID<!IS_BOX_TYPE, TBoxRange>(range));
      auto smallestNodeKey = this->FindSmallestNodeKey(rangeKey);
      if (!SI::IsValidKey(smallestNodeKey))
      {
        return foundNodes;
      }

      VisitNodes(
        smallestNodeKey,
        [&](MortonNodeIDCR key, Node const& node) { foundNodes.push_back(key); },
        [&](MortonNodeIDCR key, Node const& node) {
          return key != excludeNodeKey && IGM::DoesRangeContainBoxAD(GetNodeBox(SI::GetDepthID(key), GetNodeCenterMacro(this, key, node)), range);
        });

      for (auto parentNodeKey = SI::GetParentKey(smallestNodeKey); SI::IsValidKey(parentNodeKey); parentNodeKey = SI::GetParentKey(parentNodeKey))
      {
        foundNodes.push_back(parentNodeKey);
      }

      return foundNodes;
    }

    static PlaneRelation GetEntityPlaneRelation(TEntity const& entity, TGeometry distanceOfOrigo, TVector const& planeNormal, TGeometry tolerance)
    {
      if constexpr (IS_BOX_TYPE)
        return IGM::GetBoxPlaneRelationAD(IGM::GetBoxCenterAD(entity), IGM::GetBoxHalfSizeAD(entity), distanceOfOrigo, planeNormal, tolerance);
      else
        return AD::GetPointPlaneRelation(entity, distanceOfOrigo, planeNormal, tolerance);
    }

    std::vector<TEntityID> PlaneIntersectionBase(TGeometry distanceOfOrigo, TVector const& planeNormal, TGeometry tolerance, TContainer const& data) const noexcept
    {
      assert(AD::IsNormalizedVector(planeNormal));

      auto results = std::vector<TEntityID>{};
      auto const selector = [&](MortonNodeIDCR key, Node const& node) -> bool {
        auto const& halfSize = this->GetNodeSize(SI::GetDepthID(key) + 1);
        return IGM::GetBoxPlaneRelationAD(GetNodeCenterMacro(this, key, node), halfSize, distanceOfOrigo, planeNormal, tolerance) == PlaneRelation::Hit;
      };

      auto const procedure = [&](MortonNodeIDCR, Node const& node) {
        for (auto const entityID : node.Entities)
          if (GetEntityPlaneRelation(detail::at(data, entityID), distanceOfOrigo, planeNormal, tolerance) == PlaneRelation::Hit)
            if (std::find(results.begin(), results.end(), entityID) == results.end())
              results.emplace_back(entityID);
      };

      this->VisitNodesInDFS(SI::GetRootKey(), procedure, selector);

      return results;
    }

    std::vector<TEntityID> PlanePositiveSegmentationBase(
      TGeometry distanceOfOrigo, TVector const& planeNormal, TGeometry tolerance, TContainer const& data) const noexcept
    {
      assert(AD::IsNormalizedVector(planeNormal));

      auto results = std::vector<TEntityID>{};
      auto const selector = [&](MortonNodeIDCR key, Node const& node) -> bool {
        auto const& halfSize = this->GetNodeSize(SI::GetDepthID(key) + 1);
        auto const relation = IGM::GetBoxPlaneRelationAD(GetNodeCenterMacro(this, key, node), halfSize, distanceOfOrigo, planeNormal, tolerance);
        return relation != PlaneRelation::Negative;
      };

      auto const procedure = [&](MortonNodeIDCR, Node const& node) {
        for (auto const entityID : node.Entities)
        {
          auto const relation = GetEntityPlaneRelation(detail::at(data, entityID), distanceOfOrigo, planeNormal, tolerance);
          if (relation == PlaneRelation::Negative)
            continue;

          if (std::find(results.begin(), results.end(), entityID) == results.end())
            results.emplace_back(entityID);
        }
      };

      this->VisitNodesInDFS(SI::GetRootKey(), procedure, selector);

      return results;
    }

    // Get all entities which relation is positive or intersected by the given space boundary planes
    std::vector<TEntityID> FrustumCullingBase(std::span<TPlane const> const& boundaryPlanes, TGeometry tolerance, TContainer const& data) const noexcept
    {
      auto results = std::vector<TEntityID>{};
      if (boundaryPlanes.empty())
        return results;

      assert(std::all_of(boundaryPlanes.begin(), boundaryPlanes.end(), [](auto const& plane) -> bool {
        return AD::IsNormalizedVector(AD::GetPlaneNormal(plane));
      }));

      auto const selector = [&](MortonNodeIDCR key, Node const& node) -> bool {
        auto const& halfSize = this->GetNodeSize(SI::GetDepthID(key) + 1);
        auto const& center = GetNodeCenterMacro(this, key, node);

        for (auto const& plane : boundaryPlanes)
        {
          auto const relation = IGM::GetBoxPlaneRelationAD(center, halfSize, AD::GetPlaneOrigoDistance(plane), AD::GetPlaneNormal(plane), tolerance);
          if (relation == PlaneRelation::Hit)
            return true;

          if (relation == PlaneRelation::Negative)
            return false;
        }
        return true;
      };

      auto const procedure = [&](MortonNodeIDCR, Node const& node) {
        for (auto const entityID : node.Entities)
        {
          auto relation = PlaneRelation::Negative;
          for (auto const& plane : boundaryPlanes)
          {
            relation = GetEntityPlaneRelation(detail::at(data, entityID), AD::GetPlaneOrigoDistance(plane), AD::GetPlaneNormal(plane), tolerance);
            if (relation != PlaneRelation::Positive)
              break;
          }

          if (relation == PlaneRelation::Negative)
            continue;

          if (std::find(results.begin(), results.end(), entityID) == results.end())
            results.emplace_back(entityID);
        }
      };

      this->VisitNodesInDFS(SI::GetRootKey(), procedure, selector);

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
    using IGM = typename Base::IGM;
    using IGM_Geometry = typename IGM::Geometry;

  public:
    using AD = typename Base::AD;
    using SI = typename Base::SI;
    using MortonLocationID = typename Base::MortonLocationID;
    using MortonLocationIDCR = typename Base::MortonLocationIDCR;
    using MortonNodeID = typename Base::MortonNodeID;
    using MortonNodeIDCR = typename Base::MortonNodeIDCR;
    using MortonChildID = typename Base::MortonChildID;

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

  public: // Create
    // Ctors
    OrthoTreePoint() = default;
    explicit OrthoTreePoint(
      TContainer const& points,
      std::optional<depth_t> maxDepthNoIn = std::nullopt,
      std::optional<TBox> boxSpaceOptional = std::nullopt,
      std::size_t maxElementNoInNode = DEFAULT_MAX_ELEMENT) noexcept
    {
      Create(*this, points, maxDepthNoIn, std::move(boxSpaceOptional), maxElementNoInNode);
    }

  private: // Aid functions
    struct Location
    {
      TEntityID EntityID;
      MortonLocationID LocationID;
    };

    using LocationIterator = typename std::vector<Location>::iterator;
    void CreateChildNodes(
      Node& parentNode,
      MortonNodeIDCR parentKey,
      LocationIterator& locationBeginIterator,
      LocationIterator const& locationEndIterator,
      MortonLocationIDCR startLocationID,
      depth_t remainingDepth) noexcept
    {
      std::size_t const elementNo = std::distance(locationBeginIterator, locationEndIterator);
      if (elementNo < this->m_maxElementNo || remainingDepth == 0)
      {
        parentNode.Entities.resize(elementNo);
        std::transform(locationBeginIterator, locationEndIterator, parentNode.Entities.begin(), [](auto const& item) { return item.EntityID; });
        locationBeginIterator = locationEndIterator;
        return;
      }

      --remainingDepth;
      auto const keyGenerator = typename SI::ChildKeyGenerator(parentKey);
      auto const locationGenerator = typename SI::ChildLocationGenerator(startLocationID, remainingDepth);

      while (locationBeginIterator != locationEndIterator)
      {
        auto const actualChildID = locationGenerator.GetChildID(locationBeginIterator->LocationID);
        auto const actualEndIterator = std::partition_point(locationBeginIterator, locationEndIterator, [&](auto const& location) {
          return actualChildID == locationGenerator.GetChildID(location.LocationID);
        });

        auto const childKey = keyGenerator.GetChildNodeKey(actualChildID);
        parentNode.AddChild(childKey);
        auto& childNode = this->CreateChild(parentNode, actualChildID, childKey);
        this->CreateChildNodes(
          childNode, childKey, locationBeginIterator, actualEndIterator, locationGenerator.GetStartLocationID(actualChildID), remainingDepth);
      }
    }

  public: // Create
    // Create
    template<bool IS_PARALLEL_EXEC = false>
    static void Create(
      OrthoTreePoint& tree,
      TContainer const& points,
      std::optional<depth_t> maxDepthNoIn = std::nullopt,
      std::optional<TBox> boxSpaceOptional = std::nullopt,
      std::size_t maxElementNoInNode = DEFAULT_MAX_ELEMENT) noexcept
    {
      auto const boxSpace = boxSpaceOptional.has_value() ? IGM::GetBoxAD(*boxSpaceOptional) : IGM::GetBoxOfPointsAD(points);
      auto const pointNo = points.size();

      auto const maxDepthNo = (!maxDepthNoIn || maxDepthNoIn == 0) ? Base::EstimateMaxDepth(pointNo, maxElementNoInNode) : *maxDepthNoIn;
      tree.InitBase(boxSpace, maxDepthNo, maxElementNoInNode);
      detail::reserve(tree.m_nodes, Base::EstimateNodeNumber(pointNo, maxDepthNo, maxElementNoInNode));
      if (points.empty())
        return;

      auto pointLocations = std::vector<Location>(pointNo);
      EXEC_POL_DEF(ept); // GCC 11.3
      std::transform(EXEC_POL_ADD(ept) points.begin(), points.end(), pointLocations.begin(), [&](auto const& point) {
        return Location{ detail::getKeyPart(points, point), tree.GetLocationID(detail::getValuePart(point)) };
      });

      EXEC_POL_DEF(eps); // GCC 11.3
      std::sort(EXEC_POL_ADD(eps) pointLocations.begin(), pointLocations.end(), [&](auto const& leftLocation, auto const& rightLocation) {
        return leftLocation.LocationID < rightLocation.LocationID;
      });

      auto constexpr rootKey = SI::GetRootKey();
      auto& nodeRoot = detail::at(tree.m_nodes, rootKey);

      auto beginIterator = pointLocations.begin();
      tree.CreateChildNodes(nodeRoot, rootKey, beginIterator, pointLocations.end(), MortonNodeID{ 0 }, maxDepthNo);
    }

  public: // Edit functions
    bool InsertWithRebalancing(TEntityID newEntityID, TVector const& newPoint, TContainer const& points) noexcept
    {
      if (!IGM::DoesBoxContainPointAD(this->m_grid.GetBoxSpace(), newPoint))
        return false;

      auto const [entityDepth, entityLocation] = this->GetDepthAndLocationID(newPoint);
      auto const entityNodeKey = SI::GetHash(entityDepth, entityLocation);
      auto const [parentNodeKey, parentDepthID] = this->FindSmallestNodeKeyWithDepth(entityNodeKey);
      if (!SI::IsValidKey(parentNodeKey))
        return false;

      return this->template InsertWithRebalancingBase<true>(parentNodeKey, parentDepthID, entityNodeKey, entityDepth, newEntityID, points);
    }

    // Insert entity into a node. If doInsertToLeaf is true: The smallest node will be chosen by the max depth. If doInsertToLeaf is false: The smallest existing level on the branch will be chosen.
    bool Insert(TEntityID newEntityID, TVector const& newPoint, bool doInsertToLeaf = false) noexcept
    {
      if (!IGM::DoesBoxContainPointAD(this->m_grid.GetBoxSpace(), newPoint))
        return false;

      auto const entityNodeKey = this->GetNodeID(newPoint);
      auto const smallestNodeKey = this->FindSmallestNodeKey(entityNodeKey);
      if (!SI::IsValidKey(smallestNodeKey))
        return false;

      return this->template InsertWithoutRebalancingBase<true>(smallestNodeKey, entityNodeKey, newEntityID, doInsertToLeaf);
    }

    // Insert entity into a node, if there is no entity within the same location by tolerance.
    template<bool DO_INSERT_TO_LEAF = false>
    bool InsertUnique(TEntityID newEntityID, TVector const& newPoint, TGeometry tolerance, TContainer const& points, bool doInsertToLeaf = false)
    {
      if (!IGM::DoesBoxContainPointAD(this->m_grid.GetBoxSpace(), newPoint))
        return false;

      auto const [entityDepth, entityLocation] = this->GetDepthAndLocationID(newPoint);
      auto const entityNodeKey = SI::GetHash(entityDepth, entityLocation);
      auto const [parentNodeKey, parentDepthID] = this->FindSmallestNodeKeyWithDepth(entityNodeKey);
      if (!SI::IsValidKey(parentNodeKey))
        return false;

      auto const nearestEntityList = this->GetNearestNeighbors(newPoint, 1, tolerance, points);
      if (!nearestEntityList.empty())
        return false;

      if constexpr (DO_INSERT_TO_LEAF)
        return this->template InsertWithoutRebalancingBase<true>(parentNodeKey, entityNodeKey, newEntityID, DO_INSERT_TO_LEAF);
      else
        return this->template InsertWithRebalancingBase<true>(parentNodeKey, parentDepthID, entityNodeKey, entityDepth, newEntityID, points);
    }

    // Erase an id. Traverse all node if it is needed, which has major performance penalty.
    template<bool DO_UPDATE_ENTITY_IDS = std::is_same_v<TEntity, typename TContainer::value_type>>
    constexpr bool EraseEntity(TEntityID entityID) noexcept
    {
      bool isErased = false;
      for (auto& [nodeKey, node] : this->m_nodes)
      {
        if (std::erase(node.Entities, entityID))
        {
          this->RemoveNodeIfPossible(nodeKey, node);
          isErased = true;
          break;
        }
      }

      if (!isErased)
        return false;

      if constexpr (DO_UPDATE_ENTITY_IDS)
      {
        for (auto& [key, node] : this->m_nodes)
          for (auto& id : node.Entities)
            id -= entityID < id;
      }

      return true;
    }

    // Erase id, aided with the original point
    template<bool DO_UPDATE_ENTITY_IDS = std::is_same_v<TEntity, typename TContainer::value_type>>
    bool Erase(TEntityID entitiyID, TVector const& entityOriginalPoint) noexcept
    {
      auto const nodeKey = this->FindSmallestNode(entityOriginalPoint);
      if (!SI::IsValidKey(nodeKey))
        return false; // old box is not in the handled space domain

      auto& node = detail::at(this->m_nodes, nodeKey);
      auto const endIteratorAfterRemove = std::remove(node.Entities.begin(), node.Entities.end(), entitiyID);
      if (endIteratorAfterRemove == node.Entities.end())
        return false; // id was not registered previously.

      node.Entities.erase(endIteratorAfterRemove, node.Entities.end());

      if constexpr (DO_UPDATE_ENTITY_IDS)
      {
        for (auto& [key, node] : this->m_nodes)
          for (auto& id : node.Entities)
            id -= entitiyID < id;
      }

      this->RemoveNodeIfPossible(nodeKey, node);

      return true;
    }


    // Update id by the new point information
    bool Update(TEntityID entityID, TVector const& newPoint, bool doesInsertToLeaf = false) noexcept
    {
      if (!IGM::DoesBoxContainPointAD(this->m_grid.GetBoxSpace(), newPoint))
        return false;

      if (!this->EraseEntity<false>(entityID))
        return false;

      return this->Insert(entityID, newPoint, doesInsertToLeaf);
    }


    // Update id by the new point information and the erase part is aided by the old point geometry data
    bool Update(TEntityID entityID, TVector const& oldPoint, TVector const& newPoint, bool doesInsertToLeaf = false) noexcept
    {
      if (!IGM::DoesBoxContainPointAD(this->m_grid.GetBoxSpace(), newPoint))
        return false;

      if (!this->Erase<false>(entityID, oldPoint))
        return false;

      return this->Insert(entityID, newPoint, doesInsertToLeaf);
    }


    // Update id with rebalancing by the new point information
    bool Update(TEntityID entityID, TVector const& newPoint, TContainer const& points) noexcept
    {
      if (!IGM::DoesBoxContainPointAD(this->m_grid.GetBoxSpace(), newPoint))
        return false;

      if (!this->EraseEntity<false>(entityID))
        return false;

      return this->InsertWithRebalancing(entityID, newPoint, points);
    }


    // Update id with rebalacing by the new point information and the erase part is aided by the old point geometry data
    bool Update(TEntityID entityID, TVector const& oldPoint, TVector const& newPoint, TContainer const& points) noexcept
    {
      if (!IGM::DoesBoxContainPointAD(this->m_grid.GetBoxSpace(), newPoint))
        return false;

      if (!this->Erase<false>(entityID, oldPoint))
        return false;

      return this->InsertWithRebalancing(entityID, newPoint, points);
    }

  public: // Search functions
    bool Contains(TVector const& searchPoint, TContainer const& points, TGeometry tolerance) const noexcept
    {
      auto const smallestNodeKey = this->FindSmallestNode(searchPoint);
      if (!SI::IsValidKey(smallestNodeKey))
        return false;

      auto const& node = this->GetNode(smallestNodeKey);
      return std::any_of(node.Entities.begin(), node.Entities.end(), [&](auto const& entityID) {
        return AD::ArePointsEqual(searchPoint, detail::at(points, entityID), tolerance);
      });
    }

    // Range search
    template<bool DOES_LEAF_NODE_CONTAIN_ELEMENT_ONLY = false>
    std::vector<TEntityID> RangeSearch(TBox const& range, TContainer const& points) const noexcept
    {
      auto foundEntityIDs = std::vector<TEntityID>();
      this->template RangeSearchBaseRoot<false, DOES_LEAF_NODE_CONTAIN_ELEMENT_ONLY>(range, points, foundEntityIDs);
      return foundEntityIDs;
    }

    // Hyperplane intersection (Plane equation: dotProduct(planeNormal, point) = distanceOfOrigo)
    inline std::vector<TEntityID> PlaneSearch(
      TGeometry const& distanceOfOrigo, TVector const& planeNormal, TGeometry tolerance, TContainer const& points) const noexcept
    {
      return this->PlaneIntersectionBase(distanceOfOrigo, planeNormal, tolerance, points);
    }

    // Hyperplane intersection using built-in plane
    inline std::vector<TEntityID> PlaneSearch(TPlane const& plane, TGeometry tolerance, TContainer const& points) const noexcept
    {
      return this->PlaneIntersectionBase(AD::GetPlaneOrigoDistance(plane), AD::GetPlaneNormal(plane), tolerance, points);
    }

    // Hyperplane segmentation, get all elements in positive side (Plane equation: dotProduct(planeNormal, point) = distanceOfOrigo)
    inline std::vector<TEntityID> PlanePositiveSegmentation(
      TGeometry distanceOfOrigo, TVector const& planeNormal, TGeometry tolerance, TContainer const& points) const noexcept
    {
      return this->PlanePositiveSegmentationBase(distanceOfOrigo, planeNormal, tolerance, points);
    }

    // Hyperplane segmentation, get all elements in positive side (Plane equation: dotProduct(planeNormal, point) = distanceOfOrigo)
    inline std::vector<TEntityID> PlanePositiveSegmentation(TPlane const& plane, TGeometry tolerance, TContainer const& points) const noexcept
    {
      return this->PlanePositiveSegmentationBase(AD::GetPlaneOrigoDistance(plane), AD::GetPlaneNormal(plane), tolerance, points);
    }

    // Hyperplane segmentation, get all elements in positive side (Plane equation: dotProduct(planeNormal, point) = distanceOfOrigo)
    inline std::vector<TEntityID> FrustumCulling(std::span<TPlane const> const& boundaryPlanes, TGeometry tolerance, TContainer const& points) const noexcept
    {
      return this->FrustumCullingBase(boundaryPlanes, tolerance, points);
    }


  private: // K Nearest Neighbor helpers
    static void CreateEntityDistance(Node const& node, TVector const& searchPoint, TContainer const& points, std::multiset<EntityDistance>& neighborEntities) noexcept
    {
      for (auto const entityID : node.Entities)
        neighborEntities.insert({ { AD::Distance(searchPoint, detail::at(points, entityID)) }, entityID });
    }

    static IGM::Geometry GetFarestDistance(std::multiset<EntityDistance>& neighborEntities, std::size_t neighborNo) noexcept
    {
      if (neighborEntities.size() < neighborNo)
        return std::numeric_limits<IGM_Geometry>::max();

      return std::next(neighborEntities.begin(), neighborNo - 1)->Distance;
    }

    static std::vector<TEntityID> ConvertEntityDistanceToList(std::multiset<EntityDistance>& neighborEntities, std::size_t neighborNo, TGeometry maxDistance) noexcept
    {
      auto entityIDs = std::vector<TEntityID>();
      auto const entityNo = std::min(neighborNo, neighborEntities.size());
      if (entityNo == 0)
        return entityIDs;

      auto lastIt = std::next(neighborEntities.begin(), entityNo);
      if (std::prev(lastIt)->Distance >= maxDistance)
        lastIt = std::partition_point(neighborEntities.begin(), lastIt, [maxDistance](auto const& ed) { return ed.Distance < maxDistance; });

      entityIDs.reserve(entityNo);
      std::transform(neighborEntities.begin(), lastIt, std::back_inserter(entityIDs), [](auto const& ed) { return ed.EntityID; });
      return entityIDs;
    }

  public:
    // K Nearest Neighbor
    std::vector<TEntityID> GetNearestNeighbors(TVector const& searchPoint, std::size_t neighborNo, TGeometry maxDistance, TContainer const& points) const noexcept
    {
      auto neighborEntities = std::multiset<EntityDistance>();
      auto const [smallestNodeKey, smallesDepthID] = this->FindSmallestNodeKeyWithDepth(this->template GetNodeID<true>(searchPoint));
      if (!SI::IsValidKey(smallestNodeKey))
        return {};

      auto const& smallestNode = this->GetNode(smallestNodeKey);

      auto const& halfSize = this->GetNodeSize(smallesDepthID + 1);
      auto const& centerPoint = GetNodeCenterMacro(this, smallestNodeKey, smallestNode);

      auto const distance = IGM::GetBoxWallDistanceAD(searchPoint, centerPoint, halfSize);
      auto const wallDistance = *std::min(distance.begin(), distance.end());
      CreateEntityDistance(smallestNode, searchPoint, points, neighborEntities);
      if (!smallestNode.IsAnyChildExist())
      {
        if (GetFarestDistance(neighborEntities, neighborNo) < wallDistance)
        {
          return ConvertEntityDistanceToList(neighborEntities, neighborNo, maxDistance);
        }
      }

      // BFS search in iterative way
      using VisitedNodesMap =
        std::conditional_t<SI::IS_LINEAR_TREE, std::unordered_set<MortonChildID>, std::set<MortonNodeID, bitset_arithmetic_compare>>;
      VisitedNodesMap visitedNodes = { smallestNodeKey };
      auto traversedChildNodeKey = smallestNodeKey;
      auto parentDepthID = smallesDepthID - 1;
      for (auto parentNodeKey = SI::GetParentKey(smallestNodeKey); SI::IsValidKey(parentNodeKey);
           traversedChildNodeKey = parentNodeKey, parentNodeKey = SI::GetParentKey(parentNodeKey), --parentDepthID)
      {
        auto nodeMinDistances = std::multiset<BoxDistance>();
        auto const addNodeWallDistance = [&](MortonLocationIDCR key, Node const& node) {
          auto const depthID = SI::GetDepthID(key);
          auto const& halfSize = this->GetNodeSize(depthID + 1);
          auto const& centerPoint = GetNodeCenterMacro(this, key, node);
          nodeMinDistances.insert({ { IGM::Size(IGM::GetBoxWallDistanceAD(searchPoint, centerPoint, halfSize)) }, key, node });
        };

        // Search siblings

        auto nodesToVisit = std::queue<MortonNodeID>{};
        for (nodesToVisit.push(parentNodeKey); !nodesToVisit.empty(); nodesToVisit.pop())
        {
          MortonLocationIDCR nodeKey = nodesToVisit.front();
          if (nodeKey == traversedChildNodeKey)
          {
            continue;
          }

          auto const& node = this->GetNode(nodeKey);
          for (MortonLocationIDCR child : node.GetChildren())
          {
            nodesToVisit.push(child);
          }

          if (!visitedNodes.contains(nodeKey))
          {
            addNodeWallDistance(nodeKey, node);
            visitedNodes.insert(nodeKey);
          }
        }

        // Search non-siblings

        auto const& parentSize = this->GetNodeSize(parentDepthID);
        auto searchBox = typename IGM::Box{};
        for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
        {
          auto const size = parentSize[dimensionID] * 2;
          searchBox.Min[dimensionID] = AD::GetPointC(searchPoint, dimensionID) - size;
          searchBox.Max[dimensionID] = AD::GetPointC(searchPoint, dimensionID) + size;
        }

        auto const nonSiblingNeighbours = this->RangeSearchNodes(searchBox, parentNodeKey);
        for (MortonLocationIDCR nonSiblingKey : nonSiblingNeighbours)
        {
          if (!visitedNodes.contains(nonSiblingKey))
          {
            addNodeWallDistance(nonSiblingKey, this->GetNode(nonSiblingKey));
          }
        }
        visitedNodes.insert(nonSiblingNeighbours.begin(), nonSiblingNeighbours.end());

        // Evaluate the nodes

        auto farestEntityDistance = GetFarestDistance(neighborEntities, neighborNo);
        for (auto const& nodeDistance : nodeMinDistances)
        {
          if (maxDistance <= nodeDistance.Distance)
          {
            break;
          }

          if (neighborNo <= neighborEntities.size() && farestEntityDistance < nodeDistance.Distance)
          {
            break;
          }

          CreateEntityDistance(nodeDistance.NodeReference, searchPoint, points, neighborEntities);
          farestEntityDistance = GetFarestDistance(neighborEntities, neighborNo);
        }
      }

      return ConvertEntityDistanceToList(neighborEntities, neighborNo, maxDistance);
    }

    inline std::vector<TEntityID> GetNearestNeighbors(TVector const& searchPoint, std::size_t neighborNo, TContainer const& points) const noexcept
    {
      return this->GetNearestNeighbors(searchPoint, neighborNo, std::numeric_limits<TGeometry>::max(), points);
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
    using IGM = typename Base::IGM;
    using IGM_Geometry = typename IGM::Geometry;

  public:
    using AD = typename Base::AD;
    using SI = typename Base::SI;
    using MortonLocationID = typename Base::MortonLocationID;
    using MortonLocationIDCR = typename Base::MortonLocationIDCR;
    using MortonNodeID = typename Base::MortonNodeID;
    using MortonNodeIDCR = typename Base::MortonNodeIDCR;
    using MortonChildID = typename Base::MortonChildID;

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

  public: // Ctors
    OrthoTreeBoundingBox() = default;
    explicit OrthoTreeBoundingBox(
      TContainer const& boxes,
      std::optional<depth_t> maxDepthNo = std::nullopt,
      std::optional<TBox> boxSpaceOptional = std::nullopt,
      std::size_t nElementMaxInNode = DEFAULT_MAX_ELEMENT) noexcept
    {
      Create(*this, boxes, maxDepthNo, std::move(boxSpaceOptional), nElementMaxInNode);
    }

  private: // Aid functions
    struct Location
    {
      TEntityID EntityID;
      SI::DepthAndLocationID DepthAndLocation;

      constexpr auto operator<(Location const& rightLocation) const { return SI::IsLess(DepthAndLocation, rightLocation.DepthAndLocation); }
    };

    using LocationContainer = std::vector<Location>;
    using LocationIterator = typename LocationContainer::iterator;

    void CreateChildNodes(
      Node& parentNode,
      MortonNodeIDCR parentKey,
      LocationIterator& beginLocationIterator,
      LocationIterator const& endLocationIterator,
      MortonLocationIDCR startLocationID,
      depth_t remainingDepthNo) noexcept
    {
      std::size_t const elementNo = std::distance(beginLocationIterator, endLocationIterator);
      if (elementNo < this->m_maxElementNo || remainingDepthNo == 0)
      {
        if (elementNo == 0)
          return;

        parentNode.Entities.resize(elementNo);
        std::transform(beginLocationIterator, endLocationIterator, parentNode.Entities.begin(), [](auto const& item) { return item.EntityID; });
        beginLocationIterator = endLocationIterator;
        return;
      }

      depth_t currentDepthID = this->m_maxDepthNo - remainingDepthNo;
      if (beginLocationIterator->DepthAndLocation.DepthID == currentDepthID)
      {
        auto it = beginLocationIterator;
        beginLocationIterator = std::partition_point(it, endLocationIterator, [&](auto const& location) {
          return location.DepthAndLocation.DepthID == it->DepthAndLocation.DepthID;
        });
        auto const nElementCur = static_cast<int>(std::distance(it, beginLocationIterator));

        parentNode.Entities.resize(nElementCur);
        std::transform(it, beginLocationIterator, parentNode.Entities.begin(), [](auto const& item) { return item.EntityID; });
      }

      ++currentDepthID;
      --remainingDepthNo;

      auto const keyGenerator = typename SI::ChildKeyGenerator(parentKey);
      auto const locationGenerator = typename SI::ChildLocationGenerator(startLocationID, remainingDepthNo);
      while (beginLocationIterator != endLocationIterator)
      {
        auto const actualChildID = locationGenerator.GetChildID(beginLocationIterator->DepthAndLocation, currentDepthID);
        auto const actualEndLocationIterator = std::partition_point(beginLocationIterator, endLocationIterator, [&](auto const& location) {
          auto const childID = locationGenerator.GetChildID(location.DepthAndLocation, currentDepthID);
          return actualChildID == childID;
        });

        auto const childKey = keyGenerator.GetChildNodeKey(actualChildID);

        parentNode.AddChild(childKey);
        auto& nodeChild = this->CreateChild(parentNode, actualChildID, childKey);
        this->CreateChildNodes(
          nodeChild, childKey, beginLocationIterator, actualEndLocationIterator, locationGenerator.GetStartLocationID(actualChildID), remainingDepthNo);
      }
    }


    void SplitEntityLocation(std::array<DimArray<GridID>, 2> const& boxMinMaxGridID, Location& location, LocationContainer& additionalLocations) const noexcept
    {
      depth_t depthID = location.DepthAndLocation.DepthID + SPLIT_DEPTH_INCREASEMENT;
      if (depthID > this->m_maxDepthNo)
        depthID = this->m_maxDepthNo;

      auto const remainingDepthNo = static_cast<depth_t>(this->m_maxDepthNo - depthID);
      auto const gridStepNo = detail::pow2<depth_t, GridID>(remainingDepthNo);

      auto gridBoundaries = DimArray<GridBoundary>{};
      std::size_t boxNoByGrid = 1;
      for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
      {
        GridID const firstGridSplit = (boxMinMaxGridID[0][dimensionID] / gridStepNo) + GridID{ 1 };
        GridID const lastGridSplit = (boxMinMaxGridID[1][dimensionID] / gridStepNo);
        GridID const gridIDNo = (lastGridSplit < firstGridSplit ? 0 : (lastGridSplit - firstGridSplit + 1)) + 1;
        boxNoByGrid *= gridIDNo;
        if (boxNoByGrid >= SI::CHILD_NO)
          return;

        gridBoundaries[dimensionID] = { boxMinMaxGridID[0][dimensionID], firstGridSplit, lastGridSplit + 1 };
      }

      auto gridIDs = std::vector<DimArray<GridID>>{};
      gridIDs.reserve(boxNoByGrid);
      auto temporaryGridID = DimArray<GridID>{};
      Base::template ConstructGridIDListRecursively<DIMENSION_NO>(gridStepNo, gridBoundaries, temporaryGridID, gridIDs);

      auto const boxNo = gridIDs.size();
      auto const gridGenerator = typename SI::GridConverter(remainingDepthNo);

      // First element into locationID
      location.DepthAndLocation.DepthID = depthID;
      location.DepthAndLocation.LocID = gridGenerator.GetLocationID(gridIDs[0]);
      auto const entityID = location.EntityID;

      auto const additionalBoxNo = boxNo - 1;
      auto const locationNo = additionalLocations.size();
      additionalLocations.resize(locationNo + additionalBoxNo);

      LOOPIVDEP
      for (std::size_t iBox = 0; iBox < additionalBoxNo; ++iBox)
      {
        auto& location = additionalLocations.at(locationNo + iBox);
        location.EntityID = entityID;
        location.DepthAndLocation.DepthID = depthID;
        location.DepthAndLocation.LocID = gridGenerator.GetLocationID(gridIDs[iBox + 1]);
      }
    }


    Location GetEntityLocation(TEntityID entityID, TBox const& box, LocationContainer* additionalLocations) const noexcept
    {
      auto const boxMinMaxGridID = this->m_grid.GetBoxGridID(box);
      auto const boxLocationID = SI::GetRangeLocationID(boxMinMaxGridID);
      auto location = Location{ .EntityID = entityID, .DepthAndLocation = SI::GetDepthAndLocationID(this->m_maxDepthNo, boxLocationID) };

      if constexpr (SPLIT_DEPTH_INCREASEMENT > 0)
      {
        auto const examinationLevel = this->m_maxDepthNo - location.DepthAndLocation.DepthID;

        // if not all nodes are touched, we split
        if (!SI::IsAllChildTouched(boxLocationID, examinationLevel))
          this->SplitEntityLocation(boxMinMaxGridID, location, *additionalLocations);
      }

      return location;
    }

  public: // Create
    // Create
    template<bool IS_PARALLEL_EXEC = false>
    static void Create(
      OrthoTreeBoundingBox& tree,
      TContainer const& boxes,
      std::optional<depth_t> maxDepthIn = std::nullopt,
      std::optional<TBox> boxSpaceOptional = std::nullopt,
      std::size_t maxElementNoInNode = DEFAULT_MAX_ELEMENT) noexcept
    {
      auto const boxSpace = boxSpaceOptional.has_value() ? IGM::GetBoxAD(*boxSpaceOptional) : IGM::GetBoxOfBoxesAD(boxes);
      auto const entityNo = boxes.size();
      auto const maxDepthNo = (!maxDepthIn || maxDepthIn == 0) ? Base::EstimateMaxDepth(entityNo, maxElementNoInNode) : *maxDepthIn;
      tree.InitBase(boxSpace, maxDepthNo, maxElementNoInNode);

      detail::reserve(tree.m_nodes, Base::EstimateNodeNumber(entityNo, maxDepthNo, maxElementNoInNode));
      if (entityNo == 0)
        return;

      auto constexpr rootKey = SI::GetRootKey();
      auto& nodeRoot = detail::at(tree.m_nodes, rootKey);

      auto locations = LocationContainer(entityNo);
      auto constexpr NON_SPLITTED = SPLIT_DEPTH_INCREASEMENT == 0;
      if constexpr (NON_SPLITTED)
      {
        EXEC_POL_DEF(epf); // GCC 11.3
        std::transform(EXEC_POL_ADD(epf) boxes.begin(), boxes.end(), locations.begin(), [&tree, &boxes](auto const& box) {
          return tree.GetEntityLocation(detail::getKeyPart(boxes, box), detail::getValuePart(box), nullptr);
        });
      }
      else if constexpr (!IS_PARALLEL_EXEC)
      {
        locations.reserve(entityNo * std::min<std::size_t>(10, std::size_t{ SI::CHILD_NO } * std::size_t{ SPLIT_DEPTH_INCREASEMENT }));

        std::size_t locationID = 0;
        EXEC_POL_DEF(epf); // GCC 11.3
        std::for_each(EXEC_POL_ADD(epf) boxes.begin(), boxes.end(), [&tree, &boxes, &locations, &locationID](auto const& box) {
          locations[locationID] = tree.GetEntityLocation(detail::getKeyPart(boxes, box), detail::getValuePart(box), &locations);
          ++locationID;
        });
      }
      else // Splitted with parallel execution
      {
        auto additionalLocations = std::unordered_map<TEntityID, LocationContainer>{};
        for (auto const& entity : boxes)
          additionalLocations[detail::getKeyPart(boxes, entity)];

        locations.reserve(entityNo * std::min<std::size_t>(10, SI::CHILD_NO * std::size_t{ SPLIT_DEPTH_INCREASEMENT }));
        EXEC_POL_DEF(epf); // GCC 11.3
        std::transform(EXEC_POL_ADD(epf) boxes.begin(), boxes.end(), locations.begin(), [&tree, &boxes, &additionalLocations](auto const& box) {
          auto const entityID = detail::getKeyPart(boxes, box);
          return tree.GetEntityLocation(entityID, detail::getValuePart(box), &additionalLocations.at(entityID));
        });

        auto additionalLocationPositions = std::unordered_map<TEntityID, std::size_t>(entityNo);
        std::size_t position = entityNo;
        for (auto const& [entityID, adds] : additionalLocations)
        {
          additionalLocationPositions[entityID] = position;
          position += adds.size();
        }

        locations.resize(position);
        EXEC_POL_DEF(epf2); // GCC 11.3
        std::for_each(
          EXEC_POL_ADD(epf2) additionalLocations.begin(), additionalLocations.end(), [&locations, &additionalLocationPositions](auto& additionalLocation) {
            if (additionalLocation.second.empty())
              return;

            std::copy(
              additionalLocation.second.begin(),
              additionalLocation.second.end(),
              std::next(locations.begin(), additionalLocationPositions.at(additionalLocation.first)));
          });
      }

      EXEC_POL_DEF(eps); // GCC 11.3
      std::sort(EXEC_POL_ADD(eps) locations.begin(), locations.end());

      auto beginLocationIterator = locations.begin();
      tree.CreateChildNodes(nodeRoot, rootKey, beginLocationIterator, locations.end(), MortonNodeID{ 0 }, maxDepthNo);
      if constexpr (SPLIT_DEPTH_INCREASEMENT > 0)
      {
        // Eliminate duplicates. Not all sub-nodes will be created due to the maxElementNoInNode, which cause duplicates in the parent nodes.
        EXEC_POL_DEF(epsp); // GCC 11.3
        std::for_each(EXEC_POL_ADD(epsp) tree.m_nodes.begin(), tree.m_nodes.end(), [](auto& pairOfKeyAndNode) {
          detail::sortAndUnique(pairOfKeyAndNode.second.Entities);
        });
      }
    }

  public: // Edit functions
    bool InsertWithRebalancing(TEntityID newEntityID, TBox const& newBox, TContainer const& boxes) noexcept
    {
      if (!IGM::DoesRangeContainBoxAD(this->m_grid.GetBoxSpace(), newBox))
        return false;

      auto locations = std::vector<Location>(1);
      locations[0] = this->GetEntityLocation(newEntityID, newBox, &locations);

      for (auto const& location : locations)
      {
        auto const entityNodeKey = SI::GetHash(location.DepthAndLocation);
        auto const parentNodeKey = this->FindSmallestNodeKey(entityNodeKey);

        if (!this->template InsertWithRebalancingBase<SPLIT_DEPTH_INCREASEMENT == 0>(
              parentNodeKey, SI::GetDepthID(parentNodeKey), entityNodeKey, location.DepthAndLocation.DepthID, newEntityID, boxes))
          return false;
      }

      return true;
    }


    // Insert item into a node. If doInsertToLeaf is true: The smallest node will be chosen by the max depth. If doInsertToLeaf is false: The smallest existing level on the branch will be chosen.
    bool Insert(TEntityID newEntityID, TBox const& newBox, bool doInsertToLeaf = false) noexcept
    {
      if (!IGM::DoesRangeContainBoxAD(this->m_grid.GetBoxSpace(), newBox))
        return false;

      auto const smallestNodeKey = this->FindSmallestNode(newBox);
      if (!SI::IsValidKey(smallestNodeKey))
        return false; // new box is not in the handled space domain

      auto locations = std::vector<Location>(1);
      locations[0] = this->GetEntityLocation(newEntityID, newBox, &locations);

      for (auto const& location : locations)
      {
        auto const entityNodeKey = SI::GetHash(location.DepthAndLocation);
        if (!this->template InsertWithoutRebalancingBase<SPLIT_DEPTH_INCREASEMENT == 0>(smallestNodeKey, entityNodeKey, newEntityID, doInsertToLeaf))
          return false;
      }

      return true;
    }


  private:
    bool DoErase(Node& node, TEntityID entityID) noexcept
    {
      auto& entities = node.Entities;
      auto const endIteratorAfterRemove = std::remove(entities.begin(), entities.end(), entityID);
      if (endIteratorAfterRemove == entities.end())
        return false; // id was not registered previously.

      entities.erase(endIteratorAfterRemove, entities.end());
      return true;
    }


    template<depth_t REMAINING_DEPTH>
    bool DoEraseRec(MortonNodeIDCR nodeKey, TEntityID entityID) noexcept
    {
      auto& node = detail::at(this->m_nodes, nodeKey);
      auto isThereAnyErased = this->DoErase(node, entityID);
      if constexpr (REMAINING_DEPTH > 0)
      {
        auto const childKeys = node.GetChildren(); // Copy required because of RemoveNodeIfPossible()
        for (MortonNodeIDCR childKey : childKeys)
          isThereAnyErased |= DoEraseRec<REMAINING_DEPTH - 1>(childKey, entityID);
      }
      this->RemoveNodeIfPossible(nodeKey, node);

      return isThereAnyErased;
    }


  public:
    // Erase id, aided with the original bounding box
    template<bool DO_UPDATE_ENTITY_IDS = true>
    bool Erase(TEntityID entityIDToErase, TBox const& box) noexcept
    {
      auto const smallestNodeKey = this->FindSmallestNode(box);
      if (!SI::IsValidKey(smallestNodeKey))
        return false; // old box is not in the handled space domain

      if (DoEraseRec<SPLIT_DEPTH_INCREASEMENT>(smallestNodeKey, entityIDToErase))
      {
        if constexpr (DO_UPDATE_ENTITY_IDS)
        {
          for (auto& [key, node] : this->m_nodes)
            for (auto& entityID : node.Entities)
              entityID -= entityIDToErase < entityID;
        }
        return true;
      }
      else
        return false;
    }


    // Erase an id. Traverse all node if it is needed, which has major performance penalty.
    template<bool DO_UPDATE_ENTITY_IDS = true>
    constexpr bool EraseEntity(TEntityID idErase) noexcept
    {
      bool isErased = false;
      if constexpr (SPLIT_DEPTH_INCREASEMENT == 0)
      {
        for (auto& [nodeKey, node] : this->m_nodes)
        {
          if (std::erase(node.Entities, idErase) > 0)
          {
            this->RemoveNodeIfPossible(nodeKey, node);
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
          auto const isErasedInCurrent = std::erase(node.Entities, idErase) > 0;
          if (isErasedInCurrent)
            erasableNodes.emplace_back(nodeKey);

          isErased |= isErasedInCurrent;
        }

        for (MortonNodeIDCR nodeKey : erasableNodes)
          this->RemoveNodeIfPossible(nodeKey, this->GetNode(nodeKey));
      }

      if (!isErased)
        return false;

      if constexpr (DO_UPDATE_ENTITY_IDS)
      {
        for (auto& [key, node] : this->m_nodes)
          for (auto& id : node.Entities)
            id -= idErase < id;
      }

      return true;
    }


    // Update id by the new bounding box information
    bool Update(TEntityID entityID, TBox const& boxNew, bool doInsertToLeaf = false) noexcept
    {
      if (!IGM::DoesRangeContainBoxAD(this->m_grid.GetBoxSpace(), boxNew))
        return false;

      if (!this->EraseEntity<false>(entityID))
        return false;

      return this->Insert(entityID, boxNew, doInsertToLeaf);
    }


    // Update id by the new bounding box information and the erase part is aided by the old bounding box geometry data
    bool Update(TEntityID entityID, TBox const& oldBox, TBox const& newBox, bool doInsertToLeaf = false) noexcept
    {
      if (!IGM::DoesRangeContainBoxAD(this->m_grid.GetBoxSpace(), newBox))
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
      if (!IGM::DoesRangeContainBoxAD(this->m_grid.GetBoxSpace(), boxNew))
        return false;

      if (!this->EraseEntity<false>(entityID))
        return false;

      return this->InsertWithRebalancing(entityID, boxNew, boxes);
    }


    // Update id with rebalancing by the new bounding box information and the erase part is aided by the old bounding box geometry data
    bool Update(TEntityID entityID, TBox const& oldBox, TBox const& newBox, TContainer const& boxes) noexcept
    {
      if (!IGM::DoesRangeContainBoxAD(this->m_grid.GetBoxSpace(), newBox))
        return false;

      if constexpr (SPLIT_DEPTH_INCREASEMENT == 0)
        if (this->FindSmallestNode(oldBox) == this->FindSmallestNode(newBox))
          return true;

      if (!this->Erase<false>(entityID, oldBox))
        return false; // entityID was not registered previously.

      return this->InsertWithRebalancing(entityID, newBox, boxes);
    }


  private:
    void PickSearchRecursive(TVector const& pickPoint, TContainer const& boxes, MortonNodeIDCR parentKey, std::vector<TEntityID>& foundEntitiyIDs) const noexcept
    {
      auto const& parentNode = this->GetNode(parentKey);
      std::copy_if(parentNode.Entities.begin(), parentNode.Entities.end(), std::back_inserter(foundEntitiyIDs), [&](auto const entityID) {
        return AD::DoesBoxContainPoint(detail::at(boxes, entityID), pickPoint);
      });

      auto const& centerPoint = GetNodeCenterMacro(this, parentKey, parentNode);
      bool isPickPointInCenter = true;
      bool isPickPointInCenterOngoing = true;
      for (MortonNodeIDCR keyChild : parentNode.GetChildren())
      {
        if (!isPickPointInCenter || isPickPointInCenterOngoing)
        {
          auto mask = MortonNodeID{ 1 };
          for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID, mask <<= 1)
          {
            auto const lower = (keyChild & mask) == MortonNodeID{};
            auto const center = TGeometry(centerPoint[dimensionID]);
            if (lower)
            {
              if (center < AD::GetPointC(pickPoint, dimensionID))
                continue;
            }
            else
            {
              if (center > AD::GetPointC(pickPoint, dimensionID))
                continue;
            }

            if (isPickPointInCenter)
              isPickPointInCenter &= center == AD::GetPointC(pickPoint, dimensionID);
          }
        }
        isPickPointInCenterOngoing = false;

        PickSearchRecursive(pickPoint, boxes, keyChild, foundEntitiyIDs);
        if (!isPickPointInCenter)
          break;
      }
    }


  public: // Search functions
    // Pick search
    std::vector<TEntityID> PickSearch(TVector const& pickPoint, TContainer const& boxes) const noexcept
    {
      auto foundEntitiyIDs = std::vector<TEntityID>();
      if (!IGM::DoesBoxContainPointAD(this->m_grid.GetBoxSpace(), pickPoint))
        return foundEntitiyIDs;

      foundEntitiyIDs.reserve(100);

      auto const endIteratorOfNodes = this->m_nodes.end();
      auto const gridIDRange = this->m_grid.GetEdgePointGridID(pickPoint);
      auto rangeLocationID = SI::GetRangeLocationID(gridIDRange);

      auto nodeKey = SI::GetHash(this->m_maxDepthNo, rangeLocationID[0]);
      if (rangeLocationID[0] != rangeLocationID[1]) // Pick point is on the nodes edge. It must check more nodes downward.
      {
        auto const rangeKey = SI::GetNodeID(this->m_maxDepthNo, rangeLocationID);
        nodeKey = this->FindSmallestNodeKey(rangeKey);
        auto const nodeIterator = this->m_nodes.find(nodeKey);
        if (nodeIterator != endIteratorOfNodes)
          PickSearchRecursive(pickPoint, boxes, nodeIterator->first, foundEntitiyIDs);

        nodeKey = SI::GetParentKey(nodeKey);
      }

      for (; SI::IsValidKey(nodeKey); nodeKey = SI::GetParentKey(nodeKey))
      {
        auto const nodeIterator = this->m_nodes.find(nodeKey);
        if (nodeIterator == endIteratorOfNodes)
          continue;

        std::copy_if(nodeIterator->second.Entities.begin(), nodeIterator->second.Entities.end(), std::back_inserter(foundEntitiyIDs), [&](auto const entityID) {
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
      this->template RangeSearchBaseRoot<DO_MUST_FULLY_CONTAIN, false>(range, boxes, foundEntities);

      if constexpr (SPLIT_DEPTH_INCREASEMENT > 0)
        detail::sortAndUnique(foundEntities);

      return foundEntities;
    }

    // Hyperplane intersection (Plane equation: dotProduct(planeNormal, point) = distanceOfOrigo)
    inline std::vector<TEntityID> PlaneIntersection(
      TGeometry const& distanceOfOrigo, TVector const& planeNormal, TGeometry tolerance, TContainer const& boxes) const noexcept
    {
      return this->PlaneIntersectionBase(distanceOfOrigo, planeNormal, tolerance, boxes);
    }

    // Hyperplane intersection using built-in plane
    inline std::vector<TEntityID> PlaneIntersection(TPlane const& plane, TGeometry tolerance, TContainer const& boxes) const noexcept
    {
      return this->PlaneIntersectionBase(AD::GetPlaneOrigoDistance(plane), AD::GetPlaneNormal(plane), tolerance, boxes);
    }

    // Hyperplane segmentation, get all elements in positive side (Plane equation: dotProduct(planeNormal, point) = distanceOfOrigo)
    inline std::vector<TEntityID> PlanePositiveSegmentation(
      TGeometry distanceOfOrigo, TVector const& planeNormal, TGeometry tolerance, TContainer const& boxes) const noexcept
    {
      return this->PlanePositiveSegmentationBase(distanceOfOrigo, planeNormal, tolerance, boxes);
    }

    // Hyperplane segmentation, get all elements in positive side (Plane equation: dotProduct(planeNormal, point) = distanceOfOrigo)
    inline std::vector<TEntityID> PlanePositiveSegmentation(TPlane const& plane, TGeometry tolerance, TContainer const& boxes) const noexcept
    {
      return this->PlanePositiveSegmentationBase(AD::GetPlaneOrigoDistance(plane), AD::GetPlaneNormal(plane), tolerance, boxes);
    }

    // Hyperplane segmentation, get all elements in positive side (Plane equation: dotProduct(planeNormal, point) = distanceOfOrigo)
    inline std::vector<TEntityID> FrustumCulling(std::span<TPlane const> const& boundaryPlanes, TGeometry tolerance, TContainer const& boxes) const noexcept
    {
      return this->FrustumCullingBase(boundaryPlanes, tolerance, boxes);
    }


    // Client-defined Collision detector based on indices. AABB intersection is executed independently from this checker.
    using FCollisionDetector = std::function<bool(TEntityID, TEntityID)>;

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

      auto constexpr rootKey = SI::GetRootKey();
      auto const trees = std::array{ &leftTree, &rightTree };

      [[maybe_unused]] auto const pLeftTree = &leftTree;
      [[maybe_unused]] auto const pRightTree = &rightTree;
      auto nodePairToProceed = std::queue<ParentIteratorArray>{};
      nodePairToProceed.push({
        NodeIteratorAndStatus{  leftTree.m_nodes.find(rootKey), false },
        NodeIteratorAndStatus{ rightTree.m_nodes.find(rootKey), false }
      });
      for (; !nodePairToProceed.empty(); nodePairToProceed.pop())
      {
        auto const& parentNodePair = nodePairToProceed.front();

        // Check the current ascendant content
        {
          for (auto const leftEntityID : parentNodePair[Left].Iterator->second.Entities)
            for (auto const rightEntityID : parentNodePair[Right].Iterator->second.Entities)
              if (AD::AreBoxesOverlapped(detail::at(leftBoxes, leftEntityID), detail::at(rightBoxes, rightEntityID), false))
                results.emplace_back(leftEntityID, rightEntityID);
        }

        // Collect children
        auto childNodes = std::array<std::vector<NodeIteratorAndStatus>, 2>{};
        for (auto const sideID : { Left, Right })
        {
          auto const& [nodeIterator, fTraversed] = parentNodePair[sideID];
          if (fTraversed)
            continue;

          auto const& childIDs = nodeIterator->second.GetChildren();
          childNodes[sideID].resize(childIDs.size());
          std::transform(childIDs.begin(), childIDs.end(), childNodes[sideID].begin(), [&](MortonNodeIDCR childKey) -> NodeIteratorAndStatus {
            return { trees[sideID]->m_nodes.find(childKey), false };
          });
        }

        // Stop condition
        if (childNodes[Left].empty() && childNodes[Right].empty())
          continue;

        // Add parent if it has any element
        for (auto const sideID : { Left, Right })
          if (!parentNodePair[sideID].Iterator->second.Entities.empty())
            childNodes[sideID].push_back({ parentNodePair[sideID].Iterator, true });


        // Cartesian product of childNodes left and right
        for (auto const& leftChildNode : childNodes[Left])
          for (auto const& rightChildNode : childNodes[Right])
            if (!(leftChildNode.Iterator == parentNodePair[Left].Iterator && rightChildNode.Iterator == parentNodePair[Right].Iterator))
              if (IGM::AreBoxesOverlappingByCenter(
                    GetNodeCenterMacro(pLeftTree, leftChildNode.Iterator->first, leftChildNode.Iterator->second),
                    GetNodeCenterMacro(pRightTree, rightChildNode.Iterator->first, rightChildNode.Iterator->second),
                    leftTree.GetNodeSizeByKey(leftChildNode.Iterator->first),
                    rightTree.GetNodeSizeByKey(rightChildNode.Iterator->first)))
                nodePairToProceed.emplace(std::array{ leftChildNode, rightChildNode });
      }

      if constexpr (SPLIT_DEPTH_INCREASEMENT > 0)
        detail::sortAndUnique(results);

      return results;
    }


    // Collision detection: Returns all overlapping boxes from the source trees.
    inline std::vector<std::pair<TEntityID, TEntityID>> CollisionDetection(
      TContainer const& boxes, OrthoTreeBoundingBox const& otherTree, TContainer const& otherBoxes) const noexcept
    {
      return CollisionDetection(*this, boxes, otherTree, otherBoxes);
    }

  private:
    struct SweepAndPruneDatabase
    {
      constexpr SweepAndPruneDatabase(TContainer const& boxes, std::vector<TEntityID> const& entityIDs) noexcept
      : m_sortedEntityIDs(entityIDs)
      {
        std::sort(m_sortedEntityIDs.begin(), m_sortedEntityIDs.end(), [&](auto const entityIDL, auto const entityIDR) {
          return AD::GetBoxMinC(detail::at(boxes, entityIDL), 0) < AD::GetBoxMinC(detail::at(boxes, entityIDR), 0);
        });
      }

      constexpr std::vector<TEntityID> const& GetEntities() const noexcept { return m_sortedEntityIDs; }

    private:
      std::vector<TEntityID> m_sortedEntityIDs;
    };

    template<typename TCollisionDetectionContainer>
    void InsertCollidedEntities(
      TContainer const& boxes, std::pair<MortonNodeID, Node> const& pairKeyNode, TCollisionDetectionContainer& collidedEntityPairsInsideNode) const noexcept
    {
      auto constexpr DOES_ORDER_MATTER = SPLIT_DEPTH_INCREASEMENT > 0;

      auto const& [nodeKey, node] = pairKeyNode;

      auto const nodeSPD = SweepAndPruneDatabase(boxes, node.Entities);
      auto const& entityIDs = nodeSPD.GetEntities();
      auto const noEntity = entityIDs.size();

      for (auto parentKey = SI::GetParentKey(nodeKey); SI::IsValidKey(parentKey); parentKey = SI::GetParentKey(parentKey))
      {
        auto const parentSPD = SweepAndPruneDatabase(boxes, this->GetNode(parentKey).Entities);
        auto const& parentEntityIDs = parentSPD.GetEntities();

        std::size_t iEntityBegin = 0;
        for (auto const entityIDParent : parentEntityIDs)
        {
          for (; iEntityBegin < noEntity; ++iEntityBegin)
            if (AD::GetBoxMaxC(detail::at(boxes, entityIDs[iEntityBegin]), 0) >= AD::GetBoxMinC(detail::at(boxes, entityIDParent), 0))
              break; // sweep and prune optimization

          for (std::size_t iEntity = iEntityBegin; iEntity < noEntity; ++iEntity)
          {
            auto const entityID = entityIDs[iEntity];

            if (AD::GetBoxMaxC(detail::at(boxes, entityIDParent), 0) < AD::GetBoxMinC(detail::at(boxes, entityID), 0))
              break; // sweep and prune optimization

            if (AD::AreBoxesOverlappedStrict(detail::at(boxes, entityID), detail::at(boxes, entityIDParent)))
              detail::emplace(collidedEntityPairsInsideNode, detail::makePair<TEntityID, DOES_ORDER_MATTER>(entityID, entityIDParent));
          }
        }
      }

      for (std::size_t i = 0; i < noEntity; ++i)
      {
        auto const entityIDI = entityIDs[i];

        for (std::size_t j = i + 1; j < noEntity; ++j)
        {
          auto const entityIDJ = entityIDs[j];
          if (AD::GetBoxMaxC(detail::at(boxes, entityIDI), 0) < AD::GetBoxMinC(detail::at(boxes, entityIDJ), 0))
            break; // sweep and prune optimization

          if (AD::AreBoxesOverlappedStrict(detail::at(boxes, entityIDI), detail::at(boxes, entityIDJ)))
            detail::emplace(collidedEntityPairsInsideNode, detail::makePair<TEntityID, DOES_ORDER_MATTER>(entityIDI, entityIDJ));
        }
      }
    }

    // Collision detection between the stored elements from bottom to top logic
    template<bool IS_PARALLEL_EXEC = false>
    std::vector<std::pair<TEntityID, TEntityID>> CollectCollidedEntities(
      TContainer const& boxes, std::optional<FCollisionDetector> const& collisionDetector = std::nullopt) const noexcept
    {
      using CollisionDetectionContainer = std::vector<std::pair<TEntityID, TEntityID>>;
      using CollisionDetectionContainerMap = std::unordered_set<std::pair<TEntityID, TEntityID>, detail::pair_hash>;

      auto const entityNo = boxes.size();
      auto collidedEntityPairs = CollisionDetectionContainer{};
      collidedEntityPairs.reserve(boxes.size());
      auto collidedEntityPairsMap = CollisionDetectionContainerMap{};
      if constexpr (!IS_PARALLEL_EXEC)
      {
        if constexpr (SPLIT_DEPTH_INCREASEMENT > 0)
        {
          collidedEntityPairsMap.reserve(entityNo);
        }

        EXEC_POL_DEF(epcd); // GCC 11.3
        std::for_each(EXEC_POL_ADD(epcd) this->m_nodes.begin(), this->m_nodes.end(), [&](auto const& pairKeyNode) {
          if constexpr (SPLIT_DEPTH_INCREASEMENT == 0)
            InsertCollidedEntities(boxes, pairKeyNode, collidedEntityPairs);
          else
            InsertCollidedEntities(boxes, pairKeyNode, collidedEntityPairsMap);
        });
      }
      else
      {
        auto collidedEntityPairsInsideNodes = std::vector<CollisionDetectionContainer>(this->m_nodes.size());
        EXEC_POL_DEF(epcd); // GCC 11.3
        std::transform(EXEC_POL_ADD(epcd) this->m_nodes.begin(), this->m_nodes.end(), collidedEntityPairsInsideNodes.begin(), [&](auto const& pairKeyNode) {
          auto collidedEntityPairsInsideNode = CollisionDetectionContainer{};
          InsertCollidedEntities(boxes, pairKeyNode, collidedEntityPairsInsideNode);
          return collidedEntityPairsInsideNode;
        });

        EXEC_POL_DEF(eps); // GCC 11.3
        auto const noCollisions = std::transform_reduce(
          EXEC_POL_ADD(eps) collidedEntityPairsInsideNodes.begin(),
          collidedEntityPairsInsideNodes.end(),
          size_t{},
          std::plus{},
          [](auto const& collidedEntityPairsInsideNode) { return collidedEntityPairsInsideNode.size(); });

        if constexpr (SPLIT_DEPTH_INCREASEMENT == 0)
        {
          collidedEntityPairs.reserve(noCollisions);
          for (auto const& collidedEntityPairsInsideNode : collidedEntityPairsInsideNodes)
            collidedEntityPairs.insert(collidedEntityPairs.end(), collidedEntityPairsInsideNode.begin(), collidedEntityPairsInsideNode.end());
        }
        else
        {
          collidedEntityPairsMap.reserve(noCollisions);
          for (auto& collidedEntityPairsInsideNode : collidedEntityPairsInsideNodes)
            collidedEntityPairsMap.insert(collidedEntityPairsInsideNode.begin(), collidedEntityPairsInsideNode.end());
        }
      }

      if constexpr (SPLIT_DEPTH_INCREASEMENT > 0)
      {
        collidedEntityPairs.reserve(collidedEntityPairsMap.size());
        collidedEntityPairs.insert(collidedEntityPairs.end(), collidedEntityPairsMap.begin(), collidedEntityPairsMap.end());
      }

      if (collisionDetector)
      {
        auto const it = std::remove_if(collidedEntityPairs.begin(), collidedEntityPairs.end(), [&](auto const& pair) {
          return (*collisionDetector)(pair.first, pair.second);
        });
        collidedEntityPairs.erase(it, collidedEntityPairs.end());
      }

      return collidedEntityPairs;
    }

  public:
    // Collision detection between the stored elements from bottom to top logic
    template<bool IS_PARALLEL_EXEC = false>
    inline std::vector<std::pair<TEntityID, TEntityID>> CollisionDetection(TContainer const& boxes) const noexcept
    {
      return CollectCollidedEntities<IS_PARALLEL_EXEC>(boxes, std::nullopt);
    }


    // Collision detection between the stored elements from bottom to top logic
    template<bool IS_PARALLEL_EXEC = false>
    inline std::vector<std::pair<TEntityID, TEntityID>> CollisionDetection(TContainer const& boxes, FCollisionDetector&& collisionDetector) const noexcept
    {
      return CollectCollidedEntities<IS_PARALLEL_EXEC>(boxes, collisionDetector);
    }

  private:
    void GetRayIntersectedAllRecursive(
      depth_t depthID,
      MortonNodeIDCR parentKey,
      TContainer const& boxes,
      TVector const& rayBasePoint,
      TVector const& rayHeading,
      TGeometry tolerance,
      TGeometry maxExaminationDistance,
      std::vector<EntityDistance>& foundEntities) const noexcept
    {
      auto const& node = this->GetNode(parentKey);

      auto const isNodeHit =
        IGM::GetRayBoxDistanceAD(GetNodeCenterMacro(this, parentKey, node), this->GetNodeSize(depthID + 1), rayBasePoint, rayHeading, tolerance);
      if (!isNodeHit)
        return;

      for (auto const entityID : node.Entities)
      {
        auto const entityDistance = AD::GetRayBoxDistance(detail::at(boxes, entityID), rayBasePoint, rayHeading, tolerance);
        if (entityDistance && (maxExaminationDistance == 0 || entityDistance.value() <= maxExaminationDistance))
          foundEntities.push_back({ { IGM_Geometry(entityDistance.value()) }, entityID });
      }

      ++depthID;
      for (MortonNodeIDCR childKey : node.GetChildren())
        GetRayIntersectedAllRecursive(depthID, childKey, boxes, rayBasePoint, rayHeading, tolerance, maxExaminationDistance, foundEntities);
    }


    void GetRayIntersectedFirstRecursive(
      depth_t depthID,
      MortonNodeIDCR parentKey,
      TContainer const& boxes,
      TVector const& rayBasePoint,
      TVector const& rayHeading,
      TGeometry tolerance,
      std::multiset<EntityDistance>& foundEntities) const noexcept
    {
      auto const& node = this->GetNode(parentKey);

      auto const maxExaminationDistance =
        foundEntities.empty() ? std::numeric_limits<typename IGM::Geometry>::infinity() : foundEntities.rbegin()->Distance;
      for (auto const entityID : node.Entities)
      {
        auto const distance = AD::GetRayBoxDistance(detail::at(boxes, entityID), rayBasePoint, rayHeading, tolerance);
        if (!distance)
          continue;

        auto const distance_ = IGM_Geometry(*distance);
        if (distance_ > maxExaminationDistance)
          continue;

        foundEntities.insert({ { distance_ }, entityID });
      }

      ++depthID;
      auto nodeDistances = std::multiset<BoxDistance>();
      auto const& halfSize = this->GetNodeSize(depthID + 1);
      for (MortonNodeIDCR childKey : node.GetChildren())
      {
        auto const& nodeChild = this->GetNode(childKey);
        auto const distance = IGM::GetRayBoxDistanceAD(GetNodeCenterMacro(this, childKey, nodeChild), halfSize, rayBasePoint, rayHeading, tolerance);
        if (!distance)
          continue;

        if (*distance > maxExaminationDistance)
          continue;

        nodeDistances.insert({ { IGM_Geometry(distance.value()) }, childKey, nodeChild });
      }

      for (auto const& nodeDistance : nodeDistances)
        GetRayIntersectedFirstRecursive(depthID, nodeDistance.NodeKey, boxes, rayBasePoint, rayHeading, tolerance, foundEntities);
    }


  public:
    // Get all box which is intersected by the ray in order
    std::vector<TEntityID> RayIntersectedAll(
      TVector const& rayBasePointPoint, TVector const& rayHeading, TContainer const& boxes, TGeometry tolerance, TGeometry maxExaminationDistance = 0) const noexcept
    {
      auto foundEntities = std::vector<EntityDistance>();
      foundEntities.reserve(20);
      GetRayIntersectedAllRecursive(0, SI::GetRootKey(), boxes, rayBasePointPoint, rayHeading, tolerance, maxExaminationDistance, foundEntities);

      auto const beginIteratorOfEntities = foundEntities.begin();
      auto endIteratorOfEntities = foundEntities.end();
      std::sort(beginIteratorOfEntities, endIteratorOfEntities);
      if constexpr (SPLIT_DEPTH_INCREASEMENT > 0)
        endIteratorOfEntities =
          std::unique(beginIteratorOfEntities, endIteratorOfEntities, [](auto const& lhs, auto const& rhs) { return lhs.EntityID == rhs.EntityID; });

      auto foundEntityIDs = std::vector<TEntityID>(std::distance(beginIteratorOfEntities, endIteratorOfEntities));
      std::transform(beginIteratorOfEntities, endIteratorOfEntities, foundEntityIDs.begin(), [](auto const& entityDistance) {
        return entityDistance.EntityID;
      });
      return foundEntityIDs;
    }


    // Get first box which is intersected by the ray
    std::optional<TEntityID> RayIntersectedFirst(TVector const& rayBasePoint, TVector const& rayHeading, TContainer const& boxes, TGeometry tolerance) const noexcept
    {
      auto const distance = IGM::GetRayBoxDistanceAD(
        GetNodeCenterMacro(this, SI::GetRootKey(), this->GetNode(SI::GetRootKey())), this->GetNodeSize(1), rayBasePoint, rayHeading, tolerance);
      if (!distance)
        return std::nullopt;

      auto foundEntities = std::multiset<EntityDistance>();
      GetRayIntersectedFirstRecursive(0, SI::GetRootKey(), boxes, rayBasePoint, rayHeading, tolerance, foundEntities);
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

#undef LOOPIVDEP

#endif // ORTHOTREE_GUARD
