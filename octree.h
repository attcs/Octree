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

#ifndef HASSERT
#define HASSERT(isOk) \
  assert(isOk);       \
  if (!isOk)          \
  {                   \
    exit(1);          \
  }
#define UNDEF_HASSERT
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
  consteval uint64_t pow2_ce()
  {
    static_assert(e >= 0 && e < 64);
    return uint64_t(1) << e;
  }

  constexpr int32_t pow2(int32_t e)
  {
    assert(e >= 0 && e < 31);
    return int32_t(1) << e;
  }

  // Grid id
  using grid_id_type = uint32_t;

  // Type of the dimension
  using dim_type = int;

  // Type of depth
  using depth_type = int;

  // Content id type
  using entity_id_type = size_t;


  // Adaptor concepts

  template<class adaptor_type, typename vector_type, typename box_type, typename geometry_type = double>
  concept AdaptorBasicsConcept = requires(vector_type& point, dim_type dimensionID) {
    {
      adaptor_type::point_comp(point, dimensionID)
    } -> std::convertible_to<geometry_type&>;
  } && requires(vector_type const& point, dim_type dimensionID) {
    {
      adaptor_type::point_comp_c(point, dimensionID)
    } -> std::convertible_to<geometry_type>;
  } && requires(box_type& box) {
    {
      adaptor_type::box_min(box)
    } -> std::convertible_to<vector_type&>;
  } && requires(box_type& box) {
    {
      adaptor_type::box_max(box)
    } -> std::convertible_to<vector_type&>;
  } && requires(box_type const& box) {
    {
      adaptor_type::box_min_c(box)
    } -> std::convertible_to<vector_type const&>;
  } && requires(box_type const& box) {
    {
      adaptor_type::box_max_c(box)
    } -> std::convertible_to<vector_type const&>;
  };

  template<class adaptor_type, typename vector_type, typename box_type, typename geometry_type = double>
  concept AdaptorConcept =
    requires { AdaptorBasicsConcept<adaptor_type, vector_type, box_type, geometry_type>; } && requires(box_type const& box, vector_type const& point) {
      {
        adaptor_type::does_box_contain_point(box, point)
      } -> std::convertible_to<bool>;
    } && requires(box_type const& e1, box_type const& e2, bool e1_must_contain_e2) {
      {
        adaptor_type::are_boxes_overlapped(e1, e2, e1_must_contain_e2)
      } -> std::convertible_to<bool>;
    } && requires(std::span<vector_type const> const& points) {
      {
        adaptor_type::box_of_points(points)
      } -> std::convertible_to<box_type>;
    } && requires(std::span<box_type const> const& boxes) {
      {
        adaptor_type::box_of_boxes(boxes)
      } -> std::convertible_to<box_type>;
    };


  // Adaptors

  template<dim_type t_DimensionNo, typename vector_type, typename box_type, typename geometry_type = double>
  struct AdaptorGeneralBasics
  {
    static constexpr geometry_type& point_comp(vector_type& point, dim_type dimensionID) noexcept { return point[dimensionID]; }
    static constexpr geometry_type const& point_comp_c(vector_type const& point, dim_type dimensionID) noexcept { return point[dimensionID]; }

    static constexpr vector_type& box_min(box_type& box) noexcept { return box.Min; }
    static constexpr vector_type& box_max(box_type& box) noexcept { return box.Max; }
    static constexpr vector_type const& box_min_c(box_type const& box) noexcept { return box.Min; }
    static constexpr vector_type const& box_max_c(box_type const& box) noexcept { return box.Max; }
  };


  template<dim_type t_DimensionNo, typename vector_type, typename box_type, typename adaptor_basics_type, typename geometry_type = double>
  struct AdaptorGeneralBase : adaptor_basics_type
  {
    using base = adaptor_basics_type;
    static_assert(AdaptorBasicsConcept<base, vector_type, box_type, geometry_type>);

    static constexpr geometry_type size2(vector_type const& point) noexcept
    {
      auto d2 = geometry_type{ 0 };
      for (dim_type dimensionID = 0; dimensionID < t_DimensionNo; ++dimensionID)
      {
        autoc d = base::point_comp_c(point, dimensionID);
        d2 += d * d;
      }
      return d2;
    }

    static constexpr geometry_type size(vector_type const& point) noexcept { return sqrt(size2(point)); }

    static constexpr vector_type add(vector_type const& ptL, vector_type const& ptR) noexcept
    {
      auto point = vector_type{};
      for (dim_type dimensionID = 0; dimensionID < t_DimensionNo; ++dimensionID)
        base::point_comp(point, dimensionID) = base::point_comp_c(ptL, dimensionID) + base::point_comp_c(ptR, dimensionID);

      return point;
    }

    static constexpr vector_type subtract(vector_type const& ptL, vector_type const& ptR) noexcept
    {
      auto point = vector_type{};
      for (dim_type dimensionID = 0; dimensionID < t_DimensionNo; ++dimensionID)
        base::point_comp(point, dimensionID) = base::point_comp_c(ptL, dimensionID) - base::point_comp_c(ptR, dimensionID);

      return point;
    }

    static constexpr vector_type multiply(vector_type const& ptL, double rScalarR) noexcept
    {
      auto point = vector_type{};
      for (dim_type dimensionID = 0; dimensionID < t_DimensionNo; ++dimensionID)
        base::point_comp(point, dimensionID) = base::point_comp_c(ptL, dimensionID) * rScalarR;

      return point;
    }

    static constexpr geometry_type dot(vector_type const& ptL, vector_type const& ptR) noexcept
    {
      auto value = geometry_type{};
      for (dim_type dimensionID = 0; dimensionID < t_DimensionNo; ++dimensionID)
        value += base::point_comp_c(ptL, dimensionID) * base::point_comp_c(ptR, dimensionID);

      return value;
    }


    static constexpr geometry_type distance(vector_type const& ptL, vector_type const& ptR) noexcept { return size(subtract(ptL, ptR)); }

    static constexpr geometry_type distance2(vector_type const& ptL, vector_type const& ptR) noexcept { return size2(subtract(ptL, ptR)); }

    static constexpr bool are_points_equal(vector_type const& ptL, vector_type const& ptR, geometry_type rAccuracy) noexcept
    {
      return distance2(ptL, ptR) <= rAccuracy * rAccuracy;
    }

    static constexpr bool is_normalized_vector(vector_type const& normal) noexcept { return abs(size2(normal) - 1.0) < 0.000001; }

    static constexpr bool does_box_contain_point(box_type const& box, vector_type const& point, geometry_type tolerance = 0) noexcept
    {
      for (dim_type dimensionID = 0; dimensionID < t_DimensionNo; ++dimensionID)
        if (!(base::point_comp_c(base::box_min_c(box), dimensionID) - tolerance <= base::point_comp_c(point, dimensionID) &&
              base::point_comp_c(point, dimensionID) <= base::point_comp_c(base::box_max_c(box), dimensionID) + tolerance))
          return false;

      return true;
    }

    static constexpr bool does_box_contain_point_strict(box_type const& box, vector_type const& point) noexcept
    {
      for (dim_type dimensionID = 0; dimensionID < t_DimensionNo; ++dimensionID)
        if (!(base::point_comp_c(base::box_min_c(box), dimensionID) < base::point_comp_c(point, dimensionID) &&
              base::point_comp_c(point, dimensionID) < base::point_comp_c(base::box_max_c(box), dimensionID)))
          return false;

      return true;
    }


    static constexpr bool does_point_touch_box(box_type const& box, vector_type const& point) noexcept
    {
      for (dim_type dimensionID = 0; dimensionID < t_DimensionNo; ++dimensionID)
        if ((base::point_comp_c(base::box_min_c(box), dimensionID) == base::point_comp_c(point, dimensionID)))
          return false;

      return true;
    }

    enum class EBoxRelation
    {
      Overlapped = -1,
      Adjecent = 0,
      Separated = 1
    };
    static constexpr EBoxRelation box_relation(box_type const& e1, box_type const& e2) noexcept
    {
      enum EBoxRelationCandidate : uint8_t
      {
        OverlappedC = 0x1,
        AdjecentC = 0x2,
        SeparatedC = 0x4
      };
      uint8_t rel = 0;
      for (dim_type dimensionID = 0; dimensionID < t_DimensionNo; ++dimensionID)
      {
        if (
          base::point_comp_c(base::box_min_c(e1), dimensionID) < base::point_comp_c(base::box_max_c(e2), dimensionID) &&
          base::point_comp_c(base::box_max_c(e1), dimensionID) > base::point_comp_c(base::box_min_c(e2), dimensionID))
          rel |= EBoxRelationCandidate::OverlappedC;
        else if (
          base::point_comp_c(base::box_min_c(e1), dimensionID) == base::point_comp_c(base::box_max_c(e2), dimensionID) ||
          base::point_comp_c(base::box_max_c(e1), dimensionID) == base::point_comp_c(base::box_min_c(e2), dimensionID))
          rel |= EBoxRelationCandidate::AdjecentC;
        else if (
          base::point_comp_c(base::box_min_c(e1), dimensionID) > base::point_comp_c(base::box_max_c(e2), dimensionID) ||
          base::point_comp_c(base::box_max_c(e1), dimensionID) < base::point_comp_c(base::box_min_c(e2), dimensionID))
          return EBoxRelation::Separated;
      }
      return (rel & EBoxRelationCandidate::AdjecentC) == EBoxRelationCandidate::AdjecentC ? EBoxRelation::Adjecent : EBoxRelation::Overlapped;
    }

    static constexpr bool are_boxes_overlapped_strict(box_type const& e1, box_type const& e2) noexcept
    {
      return box_relation(e1, e2) == EBoxRelation::Overlapped;
    }

    static constexpr bool are_boxes_overlapped(box_type const& e1, box_type const& e2, bool e1_must_contain_e2 = true, bool fOverlapPtTouchAllowed = false) noexcept
    {
      autoc e1_contains_e2min = does_box_contain_point(e1, base::box_min_c(e2));

      return e1_must_contain_e2 ? e1_contains_e2min && does_box_contain_point(e1, base::box_max_c(e2))
           : fOverlapPtTouchAllowed
             ? e1_contains_e2min || does_box_contain_point(e1, base::box_max_c(e2)) || does_box_contain_point(e2, base::box_max_c(e1))
             : box_relation(e1, e2) == EBoxRelation::Overlapped;
    }

    static inline box_type box_inverted_init() noexcept
    {
      auto ext = box_type{};
      auto& minPoint = base::box_min(ext);
      auto& maxPoint = base::box_max(ext);

      LOOPIVDEP
      for (dim_type dimensionID = 0; dimensionID < t_DimensionNo; ++dimensionID)
      {
        base::point_comp(minPoint, dimensionID) = std::numeric_limits<geometry_type>::max();
        base::point_comp(maxPoint, dimensionID) = std::numeric_limits<geometry_type>::lowest();
      }

      return ext;
    }

    static box_type box_of_points(std::span<vector_type const> const& points) noexcept
    {
      auto ext = box_inverted_init();
      for (autoc& point : points)
        for (dim_type dimensionID = 0; dimensionID < t_DimensionNo; ++dimensionID)
        {
          if (base::point_comp_c(base::box_min_c(ext), dimensionID) > base::point_comp_c(point, dimensionID))
            base::point_comp(base::box_min(ext), dimensionID) = base::point_comp_c(point, dimensionID);

          if (base::point_comp_c(base::box_max_c(ext), dimensionID) < base::point_comp_c(point, dimensionID))
            base::point_comp(base::box_max(ext), dimensionID) = base::point_comp_c(point, dimensionID);
        }

      return ext;
    }

    static box_type box_of_boxes(std::span<box_type const> const& vExtent) noexcept
    {
      auto ext = box_inverted_init();
      for (autoc& e : vExtent)
        for (dim_type dimensionID = 0; dimensionID < t_DimensionNo; ++dimensionID)
        {
          if (base::point_comp_c(base::box_min_c(ext), dimensionID) > base::point_comp_c(base::box_min_c(e), dimensionID))
            base::point_comp(base::box_min(ext), dimensionID) = base::point_comp_c(base::box_min_c(e), dimensionID);

          if (base::point_comp_c(base::box_max_c(ext), dimensionID) < base::point_comp_c(base::box_max_c(e), dimensionID))
            base::point_comp(base::box_max(ext), dimensionID) = base::point_comp_c(base::box_max_c(e), dimensionID);
        }

      return ext;
    }

    static void move_box(box_type& box, vector_type const& moveVector) noexcept
    {
      LOOPIVDEP
      for (dim_type dimensionID = 0; dimensionID < t_DimensionNo; ++dimensionID)
      {
        base::point_comp(base::box_min(box), dimensionID) += base::point_comp_c(moveVector, dimensionID);
        base::point_comp(base::box_max(box), dimensionID) += base::point_comp_c(moveVector, dimensionID);
      }
    }

    static constexpr std::optional<double> is_ray_hit(
      box_type const& box, vector_type const& rayBasePoint, vector_type const& rayHeading, geometry_type tolerance) noexcept
    {
      if (does_box_contain_point(box, rayBasePoint, tolerance))
        return 0.0;

      autoc& minBoxPoint = base::box_min_c(box);
      autoc& maxBoxPoint = base::box_max_c(box);

      autoce inf = std::numeric_limits<double>::infinity();

      auto minDistances = std::array<double, t_DimensionNo>{};
      auto maxDistances = std::array<double, t_DimensionNo>{};
      for (dim_type dimensionID = 0; dimensionID < t_DimensionNo; ++dimensionID)
      {
        autoc hComp = base::point_comp_c(rayHeading, dimensionID);
        if (hComp == 0)
        {
          if (base::point_comp_c(maxBoxPoint, dimensionID) + tolerance < base::point_comp_c(rayBasePoint, dimensionID))
            return std::nullopt;

          if (base::point_comp_c(minBoxPoint, dimensionID) - tolerance > base::point_comp_c(rayBasePoint, dimensionID))
            return std::nullopt;

          minDistances[dimensionID] = -inf;
          maxDistances[dimensionID] = +inf;
          continue;
        }

        minDistances[dimensionID] =
          (base::point_comp_c(hComp > 0.0 ? minBoxPoint : maxBoxPoint, dimensionID) - tolerance - base::point_comp_c(rayBasePoint, dimensionID)) / hComp;
        maxDistances[dimensionID] =
          (base::point_comp_c(hComp < 0.0 ? minBoxPoint : maxBoxPoint, dimensionID) + tolerance - base::point_comp_c(rayBasePoint, dimensionID)) / hComp;
      }

      autoc rMin = *std::ranges::max_element(minDistances);
      autoc rMax = *std::ranges::min_element(maxDistances);
      if (rMin > rMax || rMax < 0.0)
        return std::nullopt;

      return rMin < 0 ? rMax : rMin;
    }


    // Plane intersection (Plane equation: dotProduct(planeNormal, point) = distanceOfOrigo)
    static constexpr bool does_plane_intersect(box_type const& box, geometry_type distanceOfOrigo, vector_type const& planeNormal, geometry_type tolerance) noexcept
    {
      assert(is_normalized_vector(planeNormal));

      autoc& minPoint = base::box_min_c(box);
      autoc& maxPoint = base::box_max_c(box);

      autoc center = multiply(add(minPoint, maxPoint), 0.5);
      autoc radius = subtract(maxPoint, center);

      double radiusProjected = 0.0;
      for (dim_type dimensionID = 0; dimensionID < t_DimensionNo; ++dimensionID)
        radiusProjected += base::point_comp_c(radius, dimensionID) * abs(base::point_comp_c(planeNormal, dimensionID));

      autoc centerProjected = dot(planeNormal, center);

      return abs(centerProjected - distanceOfOrigo) <= radiusProjected + tolerance;
    }
  };


  template<dim_type t_DimensionNo, typename vector_type, typename box_type, typename geometry_type = double>
  using AdaptorGeneral =
    AdaptorGeneralBase<t_DimensionNo, vector_type, box_type, AdaptorGeneralBasics<t_DimensionNo, vector_type, box_type, geometry_type>, geometry_type>;


  template<size_t N>
  using bitset_arithmetic = std::bitset<N>;

  template<size_t N>
  bitset_arithmetic<N> operator+(bitset_arithmetic<N> const& lhs, bitset_arithmetic<N> const& rhs) noexcept
  {
    bool carry = false;
    auto ans = bitset_arithmetic<N>();
    for (size_t i = 0; i < N; ++i)
    {
      autoc sum = (lhs[i] ^ rhs[i]) ^ carry;
      carry = (lhs[i] && rhs[i]) || (lhs[i] && carry) || (rhs[i] && carry);
      ans[i] = sum;
    }

    assert(!carry); // unhandled overflow
    return ans;
  }

  template<size_t N>
  bitset_arithmetic<N> operator+(bitset_arithmetic<N> const& lhs, size_t rhs) noexcept
  {
    return lhs + bitset_arithmetic<N>(rhs);
  }

  template<size_t N>
  bitset_arithmetic<N> operator-(bitset_arithmetic<N> const& lhs, bitset_arithmetic<N> const& rhs) noexcept
  {
    assert(lhs >= rhs);

    auto ret = lhs;
    bool borrow = false;
    for (size_t i = 0; i < N; ++i)
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

  template<size_t N>
  bitset_arithmetic<N> operator-(bitset_arithmetic<N> const& lhs, size_t rhs) noexcept
  {
    return lhs - bitset_arithmetic<N>(rhs);
  }


  template<size_t N>
  bitset_arithmetic<N> operator*(bitset_arithmetic<N> const& lhs, bitset_arithmetic<N> const& rhs) noexcept
  {
    auto ret = bitset_arithmetic<N>{};

    if (lhs.count() < rhs.count())
    {
      for (size_t i = 0; i < N; ++i)
        if (lhs[i])
          ret = ret + (rhs << i);
    }
    else
    {
      for (size_t i = 0; i < N; ++i)
        if (rhs[i])
          ret = ret + (lhs << i);
    }

    return ret;
  }

  template<size_t N>
  bitset_arithmetic<N> operator*(bitset_arithmetic<N> const& lhs, size_t rhs) noexcept
  {
    return lhs * bitset_arithmetic<N>(rhs);
  }
  template<size_t N>
  bitset_arithmetic<N> operator*(size_t rhs, bitset_arithmetic<N> const& lhs) noexcept
  {
    return lhs * bitset_arithmetic<N>(rhs);
  }


  template<size_t N>
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


    size_t sig_dividend = 0;
    for (size_t i = 0, id = N - 1; i < N; ++i, --id)
      if (dividend[id])
      {
        sig_dividend = id;
        break;
      }

    size_t sig_divisor = 0;
    for (size_t i = 0, id = N - 1; i < N; ++i, --id)
      if (divisor[id])
      {
        sig_divisor = id;
        break;
      }

    size_t nAlignment = (sig_dividend - sig_divisor);
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


  template<size_t N>
  bitset_arithmetic<N> operator/(bitset_arithmetic<N> const& dividend, bitset_arithmetic<N> const& divisor) noexcept
  {
    return std::get<0>(gf2_div(dividend, divisor));
  }


  template<size_t N>
  auto operator<=>(bitset_arithmetic<N> const& lhs, bitset_arithmetic<N> const& rhs) noexcept
  {
    using R = std::strong_ordering;
    for (size_t i = 0, id = N - 1; i < N; ++i, --id)
      if (lhs[id] ^ rhs[id])
        return lhs[id] ? R::greater : R::less;

    return R::equal;
  }


  struct bitset_arithmetic_compare final
  {
    template<size_t N>
    bool operator()(bitset_arithmetic<N> const& lhs, bitset_arithmetic<N> const& rhs) const noexcept
    {
      return lhs < rhs;
    }
  };


  // NTrees


  // NTreeLinear: Non-owning base container which spatially organize data ids in N dimension space into a hash-table by Morton Z order.
  template<dim_type t_DimensionNo, typename vector_type_, typename box_type_, typename adaptor_type = AdaptorGeneral<t_DimensionNo, vector_type_, box_type_, double>, typename geometry_type_ = double>
  class OrthoTreeBase
  {
  public:
    struct UpdateID
    {
      enum : entity_id_type
      {
        ERASE = std::numeric_limits<entity_id_type>::max()
      };
    };

    static autoce IS_LINEAR_TREE = t_DimensionNo < 15;

    // Max value: 2 ^ t_DimensionNo
    using child_id_type = uint64_t;

    // Max value: 2 ^ nDepth ^ t_DimensionNo * 2 (signal bit)
    using morton_grid_id_type =
      typename std::conditional <
      t_DimensionNo<4, uint32_t, typename std::conditional<IS_LINEAR_TREE, uint64_t, bitset_arithmetic<t_DimensionNo * 4 + 1>>::type>::type;

    using morton_node_id_type = morton_grid_id_type; // same as the morton_grid_id_type, but depth is signed by a sentinel bit.
    using morton_grid_id_type_cref = typename std::conditional<IS_LINEAR_TREE, morton_node_id_type, morton_node_id_type const&>::type;
    using morton_node_id_type_cref = morton_grid_id_type_cref;
    using max_element_type = uint32_t;
    using geometry_type = geometry_type_;
    using vector_type = vector_type_;
    using box_type = box_type_;

    using AD = adaptor_type;
    static_assert(AdaptorConcept<AD, vector_type, box_type, geometry_type>);
    static_assert(0 < t_DimensionNo && t_DimensionNo < 64);

  protected:
    // Max number of children
    static autoce CHILD_NO = pow2_ce<t_DimensionNo>();
    // Type system determined maximal depth.
    static autoce MAX_THEORETICAL_DEPTH = depth_type((CHAR_BIT * sizeof(morton_node_id_type) - 1 /*sentinal bit*/) / t_DimensionNo);

  public:
    class Node
    {
    private:
      std::vector<morton_node_id_type> m_children;

    public: // Public members
      std::vector<entity_id_type> Entities = {};
      box_type Box = {};

    public:
      constexpr void AddChild(morton_node_id_type_cref childKey) noexcept { m_children.emplace_back(childKey); }

      constexpr void AddChildInOrder(morton_node_id_type_cref childKey) noexcept
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

      constexpr bool HasChild(morton_node_id_type_cref childKey) const noexcept
      {
        if constexpr (IS_LINEAR_TREE)
          return std::ranges::binary_search(m_children, childKey);
        else
          return std::ranges::binary_search(m_children, childKey, bitset_arithmetic_compare{});
      }

      constexpr bool IsChildNodeEnabled(child_id_type childID) const noexcept
      {
        autoc childMortonID = morton_node_id_type(childID);
        return std::find_if(m_children.begin(), m_children.end(), [childMortonID](autoc& childKey) {
          return (childKey & childMortonID) == childMortonID;
        });
      }

      constexpr void DisableChild(morton_node_id_type_cref childKey) noexcept
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
      constexpr std::vector<morton_node_id_type> const& GetChildren() const noexcept { return m_children; }
    };


  protected: // Aid struct to partitioning and distance ordering
    struct ItemDistance
    {
      geometry_type Distance;
      auto operator<=>(ItemDistance const& rhs) const = default;
    };

    struct EntityDistance : ItemDistance
    {
      entity_id_type EntityID;
      auto operator<=>(EntityDistance const& rhs) const = default;
    };

    struct BoxDistance : ItemDistance
    {
      morton_node_id_type NodeKey;
      Node const& Node;
    };

    using DimensionArray = std::array<double, t_DimensionNo>;

    template<typename data_type>
    using UnderlyingContainer =
      typename std::conditional<IS_LINEAR_TREE, std::unordered_map<morton_node_id_type, data_type>, std::map<morton_node_id_type, data_type, bitset_arithmetic_compare>>::type;

  protected: // Member variables
    UnderlyingContainer<Node> m_nodes;
    box_type m_boxSpace = {};
    depth_type m_maxDepthNo = {};
    grid_id_type m_maxRasterResolution = {};
    grid_id_type m_maxRasterID = {};
    max_element_type m_maxElementNo = 11;
    double m_volumeOfOverallSpace = {};
    DimensionArray m_rasterizerFactors;
    DimensionArray m_sizeInDimensions;
    DimensionArray m_minInDimensions;


  protected: // Aid functions
    template<size_t N>
    static inline child_id_type convertMortonIdToChildId(bitset_arithmetic<N> const& bs) noexcept
    {
      assert(bs <= bitset_arithmetic<N>(std::numeric_limits<size_t>::max()));
      return bs.to_ullong();
    }
    static constexpr child_id_type convertMortonIdToChildId(uint64_t morton) noexcept { return morton; }

  protected: // Grid functions
    static constexpr std::tuple<DimensionArray, DimensionArray> getGridRasterizer(
      vector_type const& minPoint, vector_type const& maxPoint, grid_id_type subDivisionNo) noexcept
    {
      auto ret = std::tuple<DimensionArray, DimensionArray>{};
      auto& [rasterizerFactors, boxSizes] = ret;
      autoc subDivisionNoFactor = static_cast<double>(subDivisionNo);
      for (dim_type dimensionID = 0; dimensionID < t_DimensionNo; ++dimensionID)
      {
        boxSizes[dimensionID] =
          static_cast<double>(adaptor_type::point_comp_c(maxPoint, dimensionID) - adaptor_type::point_comp_c(minPoint, dimensionID));
        rasterizerFactors[dimensionID] = boxSizes[dimensionID] == 0 ? 1.0 : (subDivisionNoFactor / boxSizes[dimensionID]);
      }

      return ret;
    }


    constexpr std::array<grid_id_type, t_DimensionNo> getGridIdPoint(vector_type const& point) const noexcept
    {
      auto gridIDs = std::array<grid_id_type, t_DimensionNo>{};
      for (dim_type dimensionID = 0; dimensionID < t_DimensionNo; ++dimensionID)
      {
        autoc pointComponent =
          adaptor_type::point_comp_c(point, dimensionID) - adaptor_type::point_comp_c(adaptor_type::box_min_c(this->m_boxSpace), dimensionID);
        auto rasterID = static_cast<double>(pointComponent) * this->m_rasterizerFactors[dimensionID];
        gridIDs[dimensionID] = std::min<grid_id_type>(this->m_maxRasterID, static_cast<grid_id_type>(rasterID));
      }
      return gridIDs;
    }


    constexpr std::array<std::array<grid_id_type, t_DimensionNo>, 2> getGridIdBox(box_type const& box) const noexcept
    {
      autoc& minPointOfSpace = AD::box_min_c(m_boxSpace);

      auto aid = std::array<std::array<grid_id_type, t_DimensionNo>, 2>{};
      for (dim_type dimensionID = 0; dimensionID < t_DimensionNo; ++dimensionID)
      {
        autoc minComponentRasterID =
          static_cast<double>(
            adaptor_type::point_comp_c(adaptor_type::box_min_c(box), dimensionID) - adaptor_type::point_comp_c(minPointOfSpace, dimensionID)) *
          m_rasterizerFactors[dimensionID];
        autoc maxComponentRasterID =
          static_cast<double>(
            adaptor_type::point_comp_c(adaptor_type::box_max_c(box), dimensionID) - adaptor_type::point_comp_c(minPointOfSpace, dimensionID)) *
          m_rasterizerFactors[dimensionID];

        if (minComponentRasterID < 1.0)
          aid[0][dimensionID] = 0;
        else if (minComponentRasterID > m_maxRasterID)
          aid[0][dimensionID] = m_maxRasterID;
        else
          aid[0][dimensionID] = static_cast<grid_id_type>(minComponentRasterID);


        if (maxComponentRasterID < 1.0)
          aid[1][dimensionID] = 0;
        else if (maxComponentRasterID > m_maxRasterID)
          aid[1][dimensionID] = m_maxRasterID;
        else if (minComponentRasterID != maxComponentRasterID && std::floor(maxComponentRasterID) == maxComponentRasterID)
          aid[1][dimensionID] = static_cast<grid_id_type>(maxComponentRasterID) - 1;
        else
          aid[1][dimensionID] = static_cast<grid_id_type>(maxComponentRasterID);
      }
      return aid;
    }


    inline Node& createChild(Node& parentNode, child_id_type childID, morton_node_id_type_cref childKey) noexcept
    {
      assert(childID < this->CHILD_NO);
      parentNode.AddChild(childKey);

      auto& nodeChild = m_nodes[childKey];
      if constexpr (std::is_integral_v<geometry_type>)
      {
        DimensionArray minNodePoint = this->m_minInDimensions;
        DimensionArray maxNodePoint;

        autoc nDepth = this->GetDepthID(childKey);
        auto mask = morton_node_id_type{ 1 } << (nDepth * t_DimensionNo - 1);

        auto scaleFactor = 1.0;
        for (depth_type iDepth = 0; iDepth < nDepth; ++iDepth)
        {
          scaleFactor *= 0.5;
          for (dim_type dimensionID = t_DimensionNo; dimensionID > 0; --dimensionID)
          {
            bool const isGreater = (childKey & mask);
            minNodePoint[dimensionID - 1] += isGreater * this->m_sizeInDimensions[dimensionID - 1] * scaleFactor;
            mask >>= 1;
          }
        }

        LOOPIVDEP
        for (dim_type dimensionID = 0; dimensionID < t_DimensionNo; ++dimensionID)
          maxNodePoint[dimensionID] = minNodePoint[dimensionID] + this->m_sizeInDimensions[dimensionID] * scaleFactor;

        LOOPIVDEP
        for (dim_type dimensionID = 0; dimensionID < t_DimensionNo; ++dimensionID)
        {
          AD::point_comp(AD::box_min(nodeChild.Box), dimensionID) = static_cast<geometry_type>(minNodePoint[dimensionID]);
          AD::point_comp(AD::box_max(nodeChild.Box), dimensionID) = static_cast<geometry_type>(maxNodePoint[dimensionID]);
        }
      }
      else
      {
        LOOPIVDEP
        for (dim_type dimensionID = 0; dimensionID < t_DimensionNo; ++dimensionID)
        {
          autoc isGreater = ((child_id_type{ 1 } << dimensionID) & childID) > 0;
          AD::point_comp(AD::box_min(nodeChild.Box), dimensionID) =
            isGreater * (AD::point_comp_c(AD::box_max_c(parentNode.Box), dimensionID) + AD::point_comp_c(AD::box_min_c(parentNode.Box), dimensionID)) *
              geometry_type{ 0.5 } +
            (!isGreater) * AD::point_comp_c(AD::box_min_c(parentNode.Box), dimensionID);

          AD::point_comp(AD::box_max(nodeChild.Box), dimensionID) =
            isGreater * AD::point_comp_c(AD::box_max_c(parentNode.Box), dimensionID) +
            (!isGreater) * ((AD::point_comp_c(AD::box_max_c(parentNode.Box), dimensionID) + AD::point_comp_c(AD::box_min_c(parentNode.Box), dimensionID)) *
                           geometry_type{ 0.5 });
        }
      }
      return nodeChild;
    }


    constexpr morton_grid_id_type getLocationID(vector_type const& point) const noexcept { return MortonEncode(this->getGridIdPoint(point)); }


    bool isEveryItemIdUnique() const noexcept
    {
      auto ids = std::vector<entity_id_type>();
      ids.reserve(100);
      std::ranges::for_each(m_nodes, [&](auto& node) { ids.insert(ids.end(), node.second.Entities.begin(), node.second.Entities.end()); });

      std::ranges::sort(ids);
      autoc itEndUnique = std::unique(ids.begin(), ids.end());
      return itEndUnique == ids.end();
    }

    template<bool t_doUniqnessCheckToIndicies>
    bool insert(morton_node_id_type_cref nodeKey, morton_node_id_type_cref smallestNodeKey, entity_id_type entityID, bool doInsertToLeaf) noexcept
    {
      if (nodeKey == smallestNodeKey)
      {
        cont_at(this->m_nodes, nodeKey).Entities.emplace_back(entityID);
        if constexpr (t_doUniqnessCheckToIndicies)
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
          parentNodeKey >>= t_DimensionNo;
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
          autoc childNodeKey = nodeKey << (t_DimensionNo * (this->m_maxDepthNo - nDepth - 1));
          autoc childID = getChildPartOfLocation(childNodeKey);
          auto& nodeChild = this->createChild(nodeIterator->second, childID, childNodeKey);
          nodeChild.Entities.emplace_back(entityID);
        }
        else
          nodeIterator->second.Entities.emplace_back(entityID);
      }

      if constexpr (t_doUniqnessCheckToIndicies)
        assert(this->isEveryItemIdUnique()); // Assert means: index is already added. Wrong input!

      return true;
    }

    template<typename data_type = Node>
    static void reserveContainer(std::map<morton_node_id_type, data_type, bitset_arithmetic_compare>&, size_t) noexcept {};

    template<typename data_type = Node>
    static void reserveContainer(std::unordered_map<morton_node_id_type, data_type>& m, size_t n) noexcept
    {
      m.reserve(n);
    };

  public: // Static aid functions
    static constexpr size_t EstimateNodeNumber(size_t elementNo, depth_type maxDepthNo, max_element_type maxElementNo) noexcept
    {
      assert(maxElementNo > 0);
      assert(maxDepthNo > 0);

      if (elementNo < 10)
        return 10;

      autoce rMult = 1.5;
      if ((maxDepthNo + 1) * t_DimensionNo < 64)
      {
        size_t const nMaxChild = size_t{ 1 } << (maxDepthNo * t_DimensionNo);
        autoc nElementInNode = elementNo / nMaxChild;
        if (nElementInNode > maxElementNo / 2)
          return nMaxChild;
      }

      autoc nElementInNodeAvg = static_cast<float>(elementNo) / static_cast<float>(maxElementNo);
      autoc nDepthEstimated = std::min(maxDepthNo, static_cast<depth_type>(ceil((log2f(nElementInNodeAvg) + 1.0) / static_cast<float>(t_DimensionNo))));
      if (nDepthEstimated * t_DimensionNo < 64)
        return static_cast<size_t>(rMult * (1 << nDepthEstimated * t_DimensionNo));

      return static_cast<size_t>(rMult * nElementInNodeAvg);
    }


    static inline depth_type EstimateMaxDepth(size_t elementNo, max_element_type maxElementNo) noexcept
    {
      if (elementNo < maxElementNo)
        return 2;

      autoc nLeaf = elementNo / maxElementNo;
      // nLeaf = (2^nDepth)^t_DimensionNo
      return std::clamp(static_cast<depth_type>(std::log2(nLeaf) / static_cast<double>(t_DimensionNo)), depth_type(2), depth_type(10));
    }


    static inline morton_node_id_type GetHash(depth_type depth, morton_node_id_type_cref key) noexcept
    {
      assert(key < (morton_node_id_type(1) << (depth * t_DimensionNo)));
      return (morton_node_id_type{ 1 } << (depth * t_DimensionNo)) | key;
    }

    static constexpr morton_node_id_type GetRootKey() noexcept { return morton_node_id_type{ 1 }; }

    static constexpr bool IsValidKey(uint64_t key) noexcept { return key; }

    template<size_t N>
    static inline bool IsValidKey(bitset_arithmetic<N> const& key) noexcept
    {
      return !key.none();
    }

    static depth_type GetDepthID(morton_node_id_type key) noexcept
    {
      // Keep shifting off three bits at a time, increasing depth counter
      for (depth_type d = 0; IsValidKey(key); ++d, key >>= t_DimensionNo)
        if (key == 1) // If only sentinel bit remains, exit with node depth
          return d;

      assert(false); // Bad key
      return 0;
    }

    static inline morton_node_id_type RemoveSentinelBit(morton_node_id_type_cref key, std::optional<depth_type> const& depthIDOptional = std::nullopt) noexcept
    {
      autoc depthID = depthIDOptional.value_or(GetDepthID(key));
      return key - (morton_node_id_type{ 1 } << depthID);
    }


  private: // Morton aid functions
    static inline child_id_type getChildPartOfLocation(morton_node_id_type_cref key) noexcept
    {
      if constexpr (IS_LINEAR_TREE)
      {
        autoce maskLastBits1 = (morton_node_id_type{ 1 } << t_DimensionNo) - 1;
        return convertMortonIdToChildId(key & maskLastBits1);
      }
      else
      {
        auto childID = morton_node_id_type{};
        for (dim_type dimensionID = 0; dimensionID < t_DimensionNo; ++dimensionID)
          childID[dimensionID] = key[dimensionID];

        return convertMortonIdToChildId(childID);
      }
    }

    static constexpr morton_grid_id_type part1By2(grid_id_type n) noexcept
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
      return std::is_same<morton_grid_id_type, bitset_arithmetic<t_DimensionNo>>::value ? morton_grid_id_type(n) : static_cast<morton_grid_id_type>(n);
    }

    // Separates low 16 bits of input by one bit
    static constexpr morton_grid_id_type part1By1(grid_id_type n) noexcept
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
      return std::is_same<morton_grid_id_type, bitset_arithmetic<t_DimensionNo>>::value ? morton_grid_id_type(n) : static_cast<morton_grid_id_type>(n);
    }

  public:
    static inline morton_grid_id_type MortonEncode(std::array<grid_id_type, t_DimensionNo> const& gridID) noexcept
    {
      if constexpr (t_DimensionNo == 1)
        return morton_grid_id_type(gridID[0]);
      else if constexpr (t_DimensionNo == 2)
        return (part1By1(gridID[1]) << 1) + part1By1(gridID[0]);
      else if constexpr (t_DimensionNo == 3)
        return (part1By2(gridID[2]) << 2) + (part1By2(gridID[1]) << 1) + part1By2(gridID[0]);
      else
      {
        auto msb = gridID[0];
        for (dim_type dimensionID = 1; dimensionID < t_DimensionNo; ++dimensionID)
          msb |= gridID[dimensionID];

        morton_grid_id_type id = 0;
        grid_id_type mask = 1;
        for (dim_type i = 0; msb; mask <<= 1, msb >>= 1, ++i)
        {
          LOOPIVDEP
          for (dim_type dimensionID = 0; dimensionID < t_DimensionNo; ++dimensionID)
          {
            autoc shift = dimensionID + i * t_DimensionNo;
            if constexpr (IS_LINEAR_TREE)
              id |= static_cast<morton_grid_id_type>(gridID[dimensionID] & mask) << (shift - i);
            else
              id[shift] = gridID[dimensionID] & mask;
          }
        }
        return id;
      }
    }

    static std::array<grid_id_type, t_DimensionNo> MortonDecode(morton_node_id_type_cref nodeKey, depth_type maxDepthNo) noexcept
    {
      auto gridID = std::array<grid_id_type, t_DimensionNo>{};
      if constexpr (t_DimensionNo == 1)
        return { RemoveSentinelBit(nodeKey) };
      else
      {
        autoc depthID = GetDepthID(nodeKey);

        auto mask = morton_grid_id_type{ 1 };
        for (depth_type iDepth = maxDepthNo - depthID, shift = 0; iDepth < maxDepthNo; ++iDepth)
          for (dim_type dimensionID = 0; dimensionID < t_DimensionNo; ++dimensionID, ++shift)
            if constexpr (IS_LINEAR_TREE)
            {
              gridID[dimensionID] |= (nodeKey & mask) >> (shift - iDepth);
              mask <<= 1;
            }
            else
              gridID[dimensionID] |= grid_id_type{ nodeKey[shift] } << iDepth;
      }
      return gridID;
    }


  public: // Getters
    inline auto const& GetNodes() const noexcept { return m_nodes; }
    inline auto const& GetNode(morton_node_id_type_cref key) const noexcept { return m_nodes.at(key); }
    inline auto const& GetBox() const noexcept { return m_boxSpace; }
    inline auto GetDepthMax() const noexcept { return m_maxDepthNo; }
    inline auto GetResolutionMax() const noexcept { return m_maxRasterResolution; }


  public: // Main service functions
    // Alternative creation mode (instead of Create), Init then Insert items into leafs one by one. NOT RECOMMENDED.
    constexpr void Init(box_type const& box, depth_type maxDepthNo, max_element_type maxElementNo = 11) noexcept
    {
      assert(this->m_nodes.empty()); // To build/setup/create the tree, use the Create() [recommended] or Init() function. If an already builded tree
                                     // is wanted to be reset, use the Reset() function before init.
      assert(maxDepthNo > 1);
      assert(maxDepthNo <= MAX_THEORETICAL_DEPTH);
      assert(maxDepthNo < std::numeric_limits<uint8_t>::max());
      assert(maxElementNo > 1);
      assert(CHAR_BIT * sizeof(grid_id_type) >= m_maxDepthNo);

      this->m_boxSpace = box;
      this->m_volumeOfOverallSpace = 1.0;
      for (dim_type dimensionID = 0; dimensionID < t_DimensionNo; ++dimensionID)
        this->m_volumeOfOverallSpace *=
          AD::point_comp_c(AD::box_max_c(this->m_boxSpace), dimensionID) - AD::point_comp_c(AD::box_min_c(this->m_boxSpace), dimensionID);

      this->m_maxDepthNo = maxDepthNo;
      this->m_maxRasterResolution = static_cast<grid_id_type>(pow2(maxDepthNo));
      this->m_maxRasterID = this->m_maxRasterResolution - 1;
      this->m_maxElementNo = maxElementNo;

      auto& nodeRoot = this->m_nodes[GetRootKey()];
      nodeRoot.Box = box;
      tie(this->m_rasterizerFactors, this->m_sizeInDimensions) =
        this->getGridRasterizer(AD::box_min_c(this->m_boxSpace), AD::box_max_c(this->m_boxSpace), this->m_maxRasterResolution);

      LOOPIVDEP
      for (dim_type dimensionID = 0; dimensionID < t_DimensionNo; ++dimensionID)
        this->m_minInDimensions[dimensionID] = static_cast<double>(AD::point_comp_c(AD::box_min_c(this->m_boxSpace), dimensionID));
    }


    using fnProcedure = std::function<void(morton_node_id_type_cref, Node const&)>;
    using fnProcedureUnconditional = std::function<void(morton_node_id_type_cref, Node const&, bool)>;
    using fnSelector = std::function<bool(morton_node_id_type_cref, Node const&)>;
    using fnSelectorUnconditional = std::function<bool(morton_node_id_type_cref, Node const&)>;


    // Visit nodes with special selection and procedure in breadth-first search order
    void VisitNodes(morton_node_id_type_cref rootKey, fnProcedure const& procedure, fnSelector const& selector) const noexcept
    {
      auto nodeIDsToProceed = std::queue<morton_node_id_type>();
      for (nodeIDsToProceed.push(rootKey); !nodeIDsToProceed.empty(); nodeIDsToProceed.pop())
      {
        autoc& key = nodeIDsToProceed.front();
        autoc& node = GetNode(key);
        if (!selector(key, node))
          continue;

        procedure(key, node);

        for (morton_node_id_type_cref childKey : node.GetChildren())
          nodeIDsToProceed.push(childKey);
      }
    }


    // Visit nodes with special selection and procedure in breadth-first search order
    inline void VisitNodes(morton_node_id_type_cref rootKey, fnProcedure const& procedure) const noexcept
    {
      VisitNodes(rootKey, procedure, [](morton_node_id_type_cref, Node const&) { return true; });
    }


    // Visit nodes with special selection and procedure and if unconditional selection is fulfilled descendants will not be test with selector
    void VisitNodes(
      morton_node_id_type_cref rootKey,
      fnProcedureUnconditional const& procedure,
      fnSelector const& selector,
      fnSelectorUnconditional const& selectorUnconditional) const noexcept
    {
      struct Search
      {
        morton_node_id_type Key;
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

        for (morton_node_id_type childKey : node.GetChildren())
          nodesToProceed.push({ childKey, doAvoidSelection });
      }
    }


    // Visit nodes with special selection and procedure in depth-first search order
    void VisitNodesInDFS(morton_node_id_type_cref key, fnProcedure const& procedure, fnSelector const& selector) const noexcept
    {
      autoc& node = GetNode(key);
      if (!selector(key, node))
        return;

      procedure(key, node);
      for (autoc childKey : node.GetChildren())
        VisitNodesInDFS(childKey, procedure, selector);
    }


    // Collect all item id, traversing the tree in breadth-first search order
    std::vector<entity_id_type> CollectAllIdInBFS(morton_node_id_type_cref rootKey = GetRootKey()) const noexcept
    {
      auto entityIDs = std::vector<entity_id_type>();
      entityIDs.reserve(m_nodes.size() * std::max<size_t>(2, m_maxElementNo / 2));

      VisitNodes(rootKey, [&entityIDs](morton_node_id_type_cref, autoc& node) {
        entityIDs.insert(entityIDs.end(), node.Entities.begin(), node.Entities.end());
      });
      return entityIDs;
    }


    // Update all element which are in the given hash-table. Elements will be erased if the replacement id is std::numeric_limits<entity_id_type>::max().
    template<bool t_doUniqnessCheckToIndicies = false>
    void UpdateIndexes(std::unordered_map<entity_id_type, entity_id_type> const& indexUpdateMap) noexcept
    {
      autoc mapEndIterator = indexUpdateMap.end();
      std::ranges::for_each(m_nodes, [&](auto& node) {
        auto idList = std::vector<entity_id_type>(node.second.Entities.size());
        std::ranges::transform(node.second.Entities, idList.begin(), [&](autoc& id) {
          autoc it = indexUpdateMap.find(id);
          return it == mapEndIterator ? id : it->second;
        });

        std::erase_if(idList, [](autoc id) { return id == UpdateID::ERASE; });
        node.second.Entities.swap(idList);
      });

      if constexpr (t_doUniqnessCheckToIndicies)
        assert(isEveryItemIdUnique()); // Assert means: index replacements causes that multiple object has the same id. Wrong input!
    }


    // Calculate extent by box of the tree and the key of the node
    box_type CalculateExtent(morton_node_id_type_cref nodeKey) const noexcept
    {
      auto nodeBox = box_type();
      auto& minNodePoint = AD::box_min(nodeBox);
      auto& maxNodePoint = AD::box_max(nodeBox);
      autoc& minSpacePoint = AD::box_min_c(m_boxSpace);
      autoc& maxSpacePoint = AD::box_max_c(m_boxSpace);

      minNodePoint = minSpacePoint;

      auto spaceSizes = std::array<geometry_type, t_DimensionNo>();
      LOOPIVDEP
      for (dim_type dimensionID = 0; dimensionID < t_DimensionNo; ++dimensionID)
        spaceSizes[dimensionID] = AD::point_comp_c(maxSpacePoint, dimensionID) - AD::point_comp_c(minSpacePoint, dimensionID);

      autoc depthIDOfNode = GetDepthID(nodeKey);
      autoc rasterResolutionNoAtNodeDepth = pow2(depthIDOfNode);
      autoc rasterFactorAtNodeDepth = 1.0 / static_cast<double>(rasterResolutionNoAtNodeDepth);

      autoce one = morton_grid_id_type{ 1 };
      auto examinationKey = nodeKey;
      for (depth_type depthID = 0; depthID < depthIDOfNode; ++depthID)
      {
        autoc rasterFactorAtDepth = rasterFactorAtNodeDepth * (1 << depthID);

        LOOPIVDEP
        for (dim_type dimensionID = 0; dimensionID < t_DimensionNo; ++dimensionID)
        {
          autoc shouldApply = ((examinationKey >> dimensionID) & one) > morton_grid_id_type{};
          AD::point_comp(minNodePoint, dimensionID) += static_cast<geometry_type>((spaceSizes[dimensionID] * rasterFactorAtDepth)) * shouldApply;
        }
        examinationKey >>= t_DimensionNo;
      }

      LOOPIVDEP
      for (dim_type dimensionID = 0; dimensionID < t_DimensionNo; ++dimensionID)
        AD::point_comp(maxNodePoint, dimensionID) =
          AD::point_comp_c(minNodePoint, dimensionID) + static_cast<geometry_type>(spaceSizes[dimensionID] * rasterFactorAtNodeDepth);

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
    template<typename execution_policy_type = std::execution::unsequenced_policy>
    void Move(vector_type const& moveVector) noexcept
    {
      auto ep = execution_policy_type{}; // GCC 11.3
      std::for_each(ep, m_nodes.begin(), m_nodes.end(), [&moveVector](auto& pairKeyNode) { AD::move_box(pairKeyNode.second.Box, moveVector); });
      AD::move_box(this->m_boxSpace, moveVector);
    }


    morton_node_id_type FindSmallestNodeKey(morton_node_id_type searchKey) const noexcept
    {
      for (; IsValidKey(searchKey); searchKey >>= t_DimensionNo)
        if (this->m_nodes.contains(searchKey))
          return searchKey;

      return morton_node_id_type{}; // Not found
    }

    morton_node_id_type Find(entity_id_type entityID) const noexcept
    {
      autoc it = find_if(this->m_nodes.begin(), this->m_nodes.end(), [entityID](autoc& keyAndNode) {
        return std::ranges::find(keyAndNode.second.Entities, entityID) != end(keyAndNode.second.Entities);
      });

      return it == this->m_nodes.end() ? 0 : it->first;
    }

  protected:
    struct GridBoundary
    {
      grid_id_type MinGridID, BeginGridID, EndGridID;
    };
    template<dim_type t_dimensionID>
    static constexpr void constructGridIDListRecursively(
      grid_id_type gridStepNo,
      std::array<GridBoundary, t_DimensionNo> const& gridIDBoundaries,
      std::array<grid_id_type, t_DimensionNo>& currentGridID,
      std::vector<std::array<grid_id_type, t_DimensionNo>>& allGridID) noexcept
    {
      if constexpr (t_dimensionID == 0)
        allGridID.emplace_back(currentGridID);
      else
      {
        autoc & [minGridID, beginGridID, endGridID] = gridIDBoundaries[t_dimensionID - 1];
        currentGridID[t_dimensionID - 1] = minGridID;
        constructGridIDListRecursively<t_dimensionID - 1>(gridStepNo, gridIDBoundaries, currentGridID, allGridID);
        for (auto gridID = beginGridID; gridID < endGridID; ++gridID)
        {
          currentGridID[t_dimensionID - 1] = gridID * gridStepNo;
          constructGridIDListRecursively<t_dimensionID - 1>(gridStepNo, gridIDBoundaries, currentGridID, allGridID);
        }
      }
    }


    template<bool t_doCollectOnlyLargerThanMinEntityID = false>
    void collectAllIdInDFS(Node const& parentNode, std::vector<entity_id_type>& foundEntities, entity_id_type minEntityID = 0) const noexcept
    {
      if constexpr (t_doCollectOnlyLargerThanMinEntityID)
      {
        for (autoc entityID : parentNode.Entities)
          if (entityID > minEntityID)
            foundEntities.emplace_back(entityID);
      }
      else
        foundEntities.insert(foundEntities.end(), parentNode.Entities.begin(), parentNode.Entities.end());

      for (morton_node_id_type_cref childKey : parentNode.GetChildren())
        collectAllIdInDFS<t_doCollectOnlyLargerThanMinEntityID>(this->GetNode(childKey), foundEntities, minEntityID);
    }

    template<typename data_type, bool t_doRangeMustFullyContain = false, bool t_doCollectOnlyLargerThanMinEntityID = false>
    constexpr void rangeSearchCopy(
      box_type const& range,
      std::span<data_type const> const& geometryCollection,
      Node const& parentNode,
      std::vector<entity_id_type>& foundEntities,
      entity_id_type minEntityID = 0) const noexcept
    {
      for (autoc entityID : parentNode.Entities)
      {
        if constexpr (std::is_same<data_type, box_type>::value)
        {
          if constexpr (t_doCollectOnlyLargerThanMinEntityID)
          {
            if (entityID <= minEntityID)
              continue;

            bool fAdd = false;
            if constexpr (t_doRangeMustFullyContain)
              fAdd = AD::are_boxes_overlapped(range, geometryCollection[entityID], true);
            else
              fAdd = AD::are_boxes_overlapped_strict(range, geometryCollection[entityID]);

            if (fAdd)
              foundEntities.emplace_back(entityID);
          }
          else
          {
            bool fAdd = false;
            if constexpr (t_doRangeMustFullyContain)
              fAdd = AD::are_boxes_overlapped(range, geometryCollection[entityID], t_doRangeMustFullyContain);
            else
              fAdd = AD::are_boxes_overlapped_strict(range, geometryCollection[entityID]);

            if (fAdd)
              foundEntities.emplace_back(entityID);
          }
        }
        else
        {
          if (AD::does_box_contain_point(range, geometryCollection[entityID]))
            foundEntities.emplace_back(entityID);
        }
      }
    }


    template<typename data_type, bool t_doRangeMustFullyContain = false, bool t_doCollectOnlyLargerThanMinEntityID = false>
    void rangeSearch(
      box_type const& range,
      std::span<data_type const> const& geometryCollection,
      double rangeVolume,
      double parentNodeVolume,
      Node const& currentNode,
      std::vector<entity_id_type>& foundEntities,
      entity_id_type minEntityID = 0) const noexcept
    {
      rangeSearchCopy<data_type, t_doRangeMustFullyContain, t_doCollectOnlyLargerThanMinEntityID>(
        range, geometryCollection, currentNode, foundEntities, minEntityID);

      autoc currentNodeVolume = parentNodeVolume / this->CHILD_NO;
      for (morton_node_id_type_cref keyChild : currentNode.GetChildren())
      {
        autoc& childNode = this->GetNode(keyChild);

        auto isOverlapped = true;
        for (dim_type dimensionID = 0; dimensionID < t_DimensionNo && isOverlapped; ++dimensionID)
        {
          autoc isUpperNodeInTheDimension = IsValidKey(keyChild & (morton_node_id_type{ 1 } << dimensionID));
          if (isUpperNodeInTheDimension)
            isOverlapped &= AD::point_comp_c(AD::box_min_c(childNode.Box), dimensionID) <= AD::point_comp_c(AD::box_max_c(range), dimensionID);
          else
            isOverlapped &= AD::point_comp_c(AD::box_max_c(childNode.Box), dimensionID) >= AD::point_comp_c(AD::box_min_c(range), dimensionID);
        }
        if (!isOverlapped)
          continue;

        if (rangeVolume >= currentNodeVolume && AD::are_boxes_overlapped(range, childNode.Box))
          collectAllIdInDFS<t_doCollectOnlyLargerThanMinEntityID>(childNode, foundEntities, minEntityID);
        else
          rangeSearch<data_type, t_doRangeMustFullyContain, t_doCollectOnlyLargerThanMinEntityID>(
            range, geometryCollection, rangeVolume, currentNodeVolume, childNode, foundEntities, minEntityID);
      }
    }

    template<typename data_type, bool t_doRangeMustFullyContain = false, bool t_doCollectOnlyLargerThanMinEntityID = false, bool t_doesLeafNodeContainElementOnly = true, bool t_isBoxType = false>
    bool rangeSearchRoot(
      box_type const& range,
      std::span<data_type const> const& geometryCollection,
      std::vector<entity_id_type>& foundEntities,
      entity_id_type minEntityID = 0) const noexcept
    {
      autoc entityNo = geometryCollection.size();
      if (AD::are_boxes_overlapped(range, this->m_boxSpace))
      {
        foundEntities.resize(t_doCollectOnlyLargerThanMinEntityID ? entityNo - minEntityID - 1 : entityNo);
        std::iota(foundEntities.begin(), foundEntities.end(), t_doCollectOnlyLargerThanMinEntityID ? minEntityID + 1 : 0);
        return entityNo;
      }

      // If the range has zero volume, it could stuck at any node comparison with point/side touch. It is eliminated to work node bounding box independently.
      for (dim_type dimensionID = 0; dimensionID < t_DimensionNo; ++dimensionID)
        if (AD::point_comp_c(AD::box_min_c(range), dimensionID) >= AD::point_comp_c(AD::box_max_c(range), dimensionID))
          return false;

      auto minLocationID = morton_grid_id_type{};
      auto maxLocationID = morton_grid_id_type{};
      if constexpr (t_isBoxType)
      {
        autoc[minGridID, maxGridID] = this->getGridIdBox(range);
        minLocationID = MortonEncode(minGridID);
        maxLocationID = MortonEncode(maxGridID);
      }
      else
      {
        minLocationID = MortonEncode(getGridIdPoint(AD::box_min_c(range)));
        maxLocationID = MortonEncode(getGridIdPoint(AD::box_max_c(range)));
      }

      auto depthNo = this->m_maxDepthNo;
      for (auto diffLocationFlag = minLocationID ^ maxLocationID; IsValidKey(diffLocationFlag); diffLocationFlag >>= t_DimensionNo, --depthNo)
        minLocationID >>= t_DimensionNo;

      autoc rangeKey = this->GetHash(depthNo, minLocationID);
      auto smallestNodeKey = this->FindSmallestNodeKey(rangeKey);
      if (!IsValidKey(smallestNodeKey))
        return false;

      auto rangeVolume = 1.0;
      for (dim_type dimensionID = 0; dimensionID < t_DimensionNo; ++dimensionID)
        rangeVolume *= AD::point_comp_c(AD::box_max_c(range), dimensionID) - AD::point_comp_c(AD::box_min_c(range), dimensionID);

      autoc nodeVolume = this->m_volumeOfOverallSpace / static_cast<double>(1 << (t_DimensionNo * depthNo));

      autoc foundEntityNoEstimation =
        this->m_volumeOfOverallSpace < 0.01 ? 10 : static_cast<size_t>((rangeVolume * entityNo) / this->m_volumeOfOverallSpace);
      foundEntities.reserve(foundEntityNoEstimation);
      autoc& node = this->GetNode(smallestNodeKey);
      rangeSearch<data_type, t_doRangeMustFullyContain, t_doCollectOnlyLargerThanMinEntityID>(
        range, geometryCollection, rangeVolume, nodeVolume, node, foundEntities, minEntityID);

      if constexpr (!t_doesLeafNodeContainElementOnly)
      {
        for (smallestNodeKey >>= t_DimensionNo; IsValidKey(smallestNodeKey); smallestNodeKey >>= t_DimensionNo)
          rangeSearchCopy<data_type, t_doRangeMustFullyContain, t_doCollectOnlyLargerThanMinEntityID>(
            range, geometryCollection, this->GetNode(smallestNodeKey), foundEntities, minEntityID);
      }

      return true;
    }


  public:
    std::vector<entity_id_type> CollectAllIdInDFS(morton_grid_id_type_cref parentKey = GetRootKey()) const noexcept
    {
      auto entityIDs = std::vector<entity_id_type>{};
      collectAllIdInDFS(GetNode(parentKey), entityIDs);
      return entityIDs;
    }
  };


  // OrthoTreePoint: Non-owning container which spatially organize point ids in N dimension space into a hash-table by Morton Z order.
  template<dim_type t_DimensionNo, typename vector_type, typename box_type, typename adaptor_type = AdaptorGeneral<t_DimensionNo, vector_type, box_type, double>, typename geometry_type = double>
  class OrthoTreePoint final : public OrthoTreeBase<t_DimensionNo, vector_type, box_type, adaptor_type, geometry_type>
  {
  protected:
    using base = OrthoTreeBase<t_DimensionNo, vector_type, box_type, adaptor_type, geometry_type>;
    using EntityDistance = typename base::EntityDistance;
    using BoxDistance = typename base::BoxDistance;

  public:
    using AD = typename base::AD;
    using morton_grid_id_type = typename base::morton_grid_id_type;
    using morton_grid_id_type_cref = typename base::morton_grid_id_type_cref;
    using morton_node_id_type = typename base::morton_node_id_type;
    using morton_node_id_type_cref = typename base::morton_node_id_type_cref;
    using max_element_type = typename base::max_element_type;
    using child_id_type = typename base::child_id_type;

    using Node = typename base::Node;

    static constexpr max_element_type DEFAULT_MAX_ELEMENT = 21;

  private: // Aid functions
    struct Location
    {
      entity_id_type EntityID;
      morton_grid_id_type GridID;
    };

    using LocationIterator = typename std::vector<Location>::iterator;
    void addNodes(
      Node& parentNode,
      morton_node_id_type_cref parentKey,
      LocationIterator& locationBeginIterator,
      LocationIterator const& locationEndIterator,
      morton_grid_id_type_cref gridID,
      depth_type remainingDepth) noexcept
    {
      autoc elementNo = std::distance(locationBeginIterator, locationEndIterator);
      if (elementNo < this->m_maxElementNo || remainingDepth == 0)
      {
        parentNode.Entities.resize(elementNo);
        std::transform(locationBeginIterator, locationEndIterator, parentNode.Entities.begin(), [](autoc& item) { return item.EntityID; });
        locationBeginIterator = locationEndIterator;
        return;
      }

      --remainingDepth;
      autoc shift = remainingDepth * t_DimensionNo;
      autoc stepNo = morton_grid_id_type{ 1 } << shift;
      autoc parentKeyFlag = parentKey << t_DimensionNo;

      while (locationBeginIterator != locationEndIterator)
      {
        autoc actualChildID = base::convertMortonIdToChildId((locationBeginIterator->GridID - gridID) >> shift);
        autoc actualEndIterator = std::partition_point(locationBeginIterator, locationEndIterator, [&](autoc& location) {
          return actualChildID == base::convertMortonIdToChildId((location.GridID - gridID) >> shift);
        });

        autoc actualChildGridID = morton_grid_id_type(actualChildID);
        morton_grid_id_type const childKey = parentKeyFlag | actualChildGridID;
        morton_grid_id_type const beginChildGridID = gridID + actualChildGridID * stepNo;

        auto& childNode = this->createChild(parentNode, actualChildID, childKey);
        this->addNodes(childNode, childKey, locationBeginIterator, actualEndIterator, beginChildGridID, remainingDepth);
      }
    }


  public: // Create
    // Ctors
    OrthoTreePoint() = default;
    OrthoTreePoint(
      std::span<vector_type const> const& points,
      depth_type maxDepthNo,
      std::optional<box_type> const& boxSpaceOptional = std::nullopt,
      max_element_type maxElementNoInNode = DEFAULT_MAX_ELEMENT) noexcept
    {
      Create(*this, points, maxDepthNo, boxSpaceOptional, maxElementNoInNode);
    }

    // Create
    template<typename execution_policy_type = std::execution::unsequenced_policy>
    static void Create(
      OrthoTreePoint& tree,
      std::span<vector_type const> const& points,
      depth_type maxDepthNoIn = 0,
      std::optional<box_type> const& boxSpaceOptional = std::nullopt,
      max_element_type maxElementNoInNode = DEFAULT_MAX_ELEMENT) noexcept
    {
      autoc boxSpace = boxSpaceOptional.has_value() ? *boxSpaceOptional : AD::box_of_points(points);
      autoc pointNo = points.size();

      autoc maxDepthNo = maxDepthNoIn == 0 ? base::EstimateMaxDepth(pointNo, maxElementNoInNode) : maxDepthNoIn;
      tree.Init(boxSpace, maxDepthNo, maxElementNoInNode);
      base::reserveContainer(tree.m_nodes, base::EstimateNodeNumber(pointNo, maxDepthNo, maxElementNoInNode));
      if (points.empty())
        return;

      auto pointLocations = std::vector<Location>(pointNo);
      auto ept = execution_policy_type{}; // GCC 11.3 only accept in this form
      std::transform(ept, points.begin(), points.end(), pointLocations.begin(), [&](autoc& point) {
        entity_id_type const id = std::distance(&points[0], &point);
        return Location{ id, tree.getLocationID(point) };
      });

      auto eps = execution_policy_type{}; // GCC 11.3 only accept in this form
      std::sort(eps, pointLocations.begin(), pointLocations.end(), [&](autoc& leftLocation, autoc& rightLocation) {
        return leftLocation.GridID < rightLocation.GridID;
      });

      autoc rootKey = base::GetRootKey();
      auto& nodeRoot = cont_at(tree.m_nodes, rootKey);

      auto beginIterator = pointLocations.begin();
      tree.addNodes(nodeRoot, rootKey, beginIterator, pointLocations.end(), morton_node_id_type{ 0 }, maxDepthNo);
    }


  public: // Edit functions
    // Insert item into a node. If doInsertToLeaf is true: The smallest node will be chosen by the max depth. If doInsertToLeaf is false: The smallest existing level on the branch will be chosen.
    bool Insert(entity_id_type entityID, vector_type const& point, bool doInsertToLeaf = false) noexcept
    {
      if (!AD::does_box_contain_point(this->m_boxSpace, point))
        return false;

      autoc smallestNodeKey = FindSmallestNode(point);
      if (!base::IsValidKey(smallestNodeKey))
        return false;

      autoc idLocation = this->getLocationID(point);
      autoc nodeKey = this->GetHash(this->m_maxDepthNo, idLocation);

      return this->template insert<true>(nodeKey, smallestNodeKey, entityID, doInsertToLeaf);
    }

    // Erase an id. Traverse all node if it is needed, which has major performance penalty.
    template<bool t_doReduceAllID = true>
    constexpr bool EraseId(entity_id_type entityID) noexcept
    {
      autoc isErased = std::ranges::any_of(this->m_nodes, [&](auto& pairNode) { return erase(pairNode.second.Entities, entityID); });
      if (!isErased)
        return false;

      if constexpr (t_doReduceAllID)
      {
        std::ranges::for_each(this->m_nodes, [entityID](auto& pairNode) {
          for (auto& id : pairNode.second.Entities)
            id -= entityID < id;
        });
      }

      return true;
    }

    // Erase id, aided with the original point
    template<bool t_doReduceAllID = true>
    bool Erase(entity_id_type entitiyID, vector_type const& entityOriginalPoint) noexcept
    {
      autoc oldKey = FindSmallestNode(entityOriginalPoint);
      if (!base::IsValidKey(oldKey))
        return false; // old box is not in the handled space domain

      auto& idList = cont_at(this->m_nodes, oldKey).Entities;
      autoc endIteratorAfterRemove = std::remove(idList.begin(), idList.end(), entitiyID);
      if (endIteratorAfterRemove == idList.end())
        return false; // id was not registered previously.

      idList.erase(endIteratorAfterRemove, idList.end());

      if constexpr (t_doReduceAllID)
      {
        std::ranges::for_each(this->m_nodes, [entitiyID](auto& pairNode) {
          for (auto& id : pairNode.second.Entities)
            id -= entitiyID < id;
        });
      }

      return true;
    }


    // Update id by the new point information
    bool Update(entity_id_type entityID, vector_type const& newPoint, bool doesInsertToLeaf = false) noexcept
    {
      if (!AD::does_box_contain_point(this->m_boxSpace, newPoint))
        return false;

      if (!this->EraseId<false>(entityID))
        return false;

      return this->Insert(entityID, newPoint, doesInsertToLeaf);
    }


    // Update id by the new point information and the erase part is aided by the old point geometry data
    bool Update(entity_id_type entityID, vector_type const& oldPoint, vector_type const& newPoint, bool doesInsertToLeaf = false) noexcept
    {
      if (!AD::does_box_contain_point(this->m_boxSpace, newPoint))
        return false;

      if (!this->Erase<false>(entityID, oldPoint))
        return false;

      return this->Insert(entityID, newPoint, doesInsertToLeaf);
    }


  public: // Search functions
    // Find smallest node which contains the box
    morton_node_id_type FindSmallestNode(vector_type const& searchPoint) const noexcept
    {
      if (!AD::does_box_contain_point(this->m_boxSpace, searchPoint))
        return morton_node_id_type{};

      autoc locationID = this->getLocationID(searchPoint);
      return this->FindSmallestNodeKey(this->GetHash(this->m_maxDepthNo, locationID));
    }

    bool Contains(vector_type const& searchPoint, std::span<vector_type const> const& points, geometry_type tolerance) const noexcept
    {
      autoc smallestNodeKey = this->FindSmallestNode(searchPoint);
      if (!base::IsValidKey(smallestNodeKey))
        return false;

      autoc& node = this->GetNode(smallestNodeKey);
      return std::ranges::any_of(node.Entities, [&](autoc& entityID) { return AD::are_points_equal(searchPoint, points[entityID], tolerance); });
    }


    // Range search
    template<bool t_doesLeafNodeContainElementOnly = false>
    std::vector<entity_id_type> RangeSearch(box_type const& range, std::span<vector_type const> const& points) const noexcept
    {
      auto foundEntityIDs = std::vector<entity_id_type>();

      if (!this->template rangeSearchRoot<vector_type, false, false, t_doesLeafNodeContainElementOnly, false>(range, points, foundEntityIDs))
        return {};

      return foundEntityIDs;
    }


    // Plane search (Plane equation: dotProduct(planeNormal, point) = distanceOfOrigo)
    inline std::vector<entity_id_type> PlaneSearch(
      geometry_type distanceOfOrigo, vector_type const& planeNormal, geometry_type tolerance, std::span<vector_type const> const& points) const noexcept
    {
      assert(AD::is_normalized_vector(planeNormal));

      auto results = std::vector<entity_id_type>{};
      if constexpr (t_DimensionNo < 3) // under 3 dimension, every boxes will be intersected.
      {
        results.resize(points.size());
        std::iota(results.begin(), results.end(), 0);
        return results;
      }

      autoc selector = [&](morton_node_id_type id, Node const& node) -> bool {
        return AD::does_plane_intersect(node.Box, distanceOfOrigo, planeNormal, tolerance);
      };

      autoc procedure = [&](morton_node_id_type id, Node const& node) {
        for (autoc id : node.Entities)
          if (abs(AD::dot(points[id], planeNormal) - distanceOfOrigo) <= tolerance)
            if (std::find(results.begin(), results.end(), id) == results.end())
              results.emplace_back(id);
      };

      this->VisitNodesInDFS(base::GetRootKey(), procedure, selector);

      return results;
    }

  private: // K Nearest Neighbor helpers
    static geometry_type getMinBoxWallDistance(vector_type const& point, box_type const& box) noexcept
    {
      autoc& minPoint = AD::box_min_c(box);
      autoc& maxPoint = AD::box_max_c(box);

      auto distances = std::vector<geometry_type>();
      distances.reserve(t_DimensionNo);
      for (dim_type dimensionID = 0; dimensionID < t_DimensionNo; ++dimensionID)
      {
        autoc actualDistance = distances.emplace_back(std::min(
          abs(AD::point_comp_c(point, dimensionID) - AD::point_comp_c(minPoint, dimensionID)),
          abs(AD::point_comp_c(point, dimensionID) - AD::point_comp_c(maxPoint, dimensionID))));

        if (actualDistance == 0)
          return 0.0;
      }

      return *std::ranges::min_element(distances);
    }


    static void createEntityDistance(
      Node const& node, vector_type const& searchPoint, std::span<vector_type const> const& points, std::multiset<EntityDistance>& neighborEntities) noexcept
    {
      for (autoc id : node.Entities)
        neighborEntities.insert({ { AD::distance(searchPoint, points[id]) }, id });
    }

    static geometry_type getFarestDistance(std::multiset<EntityDistance>& neighborEntities, size_t neighborNo) noexcept
    {
      if (neighborEntities.size() < neighborNo)
        return std::numeric_limits<geometry_type>::max();

      return std::next(neighborEntities.begin(), neighborNo - 1)->Distance;
    }

    static std::vector<entity_id_type> convertEntityDistanceToList(std::multiset<EntityDistance>& neighborEntities, size_t neighborNo) noexcept
    {
      autoc entityNo = std::min(neighborNo, neighborEntities.size());
      auto entityIDs = std::vector<entity_id_type>(entityNo);
      std::transform(neighborEntities.begin(), std::next(neighborEntities.begin(), entityNo), entityIDs.begin(), [](autoc& ed) { return ed.EntityID; });
      return entityIDs;
    }

  public:
    // K Nearest Neighbor
    std::vector<entity_id_type> GetNearestNeighbors(vector_type const& searchPoint, size_t neighborNo, std::span<vector_type const> const& points) const noexcept
    {
      auto neighborEntities = std::multiset<EntityDistance>();
      autoc smallestNodeKey = FindSmallestNode(searchPoint);
      if (base::IsValidKey(smallestNodeKey))
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

        autoc& minPoint = AD::box_min_c(node.Box);
        autoc& maxPoint = AD::box_max_c(node.Box);

        auto aDist = vector_type{};
        for (dim_type dimensionID = 0; dimensionID < t_DimensionNo; ++dimensionID)
        {
          autoc dMin = AD::point_comp_c(minPoint, dimensionID) - AD::point_comp_c(searchPoint, dimensionID);
          autoc dMax = AD::point_comp_c(maxPoint, dimensionID) - AD::point_comp_c(searchPoint, dimensionID);

          // If point projection in dimensionID is within min and max the wall distance should be calculated.
          AD::point_comp(aDist, dimensionID) = dMin * dMax < 0 ? 0 : std::min(abs(dMin), abs(dMax));
        }
        nodeMinDistances.insert({ { AD::size(aDist) }, key, node });
      });

      if (!nodeMinDistances.empty())
      {
        auto rLatestNodeDist = getFarestDistance(neighborEntities, neighborNo);
        for (autoc& nodeDist : nodeMinDistances)
        {
          autoc n = neighborEntities.size();
          if (neighborNo <= n && rLatestNodeDist < nodeDist.Distance)
            break;

          createEntityDistance(nodeDist.Node, searchPoint, points, neighborEntities);
          rLatestNodeDist = getFarestDistance(neighborEntities, neighborNo);
        }
      }

      return convertEntityDistanceToList(neighborEntities, neighborNo);
    }
  };


  // OrthoTreeBoundingBox: Non-owning container which spatially organize bounding box ids in N dimension space into a hash-table by Morton Z order.
  // t_AdditionalDepthOfSplitStrategy: if (t_AdditionalDepthOfSplitStrategy > 0) Those items which are not fit in the child nodes may be stored in the children/grand-children instead of the parent.
  template<
    dim_type t_DimensionNo,
    typename vector_type,
    typename box_type,
    typename adaptor_type = AdaptorGeneral<t_DimensionNo, vector_type, box_type, double>,
    typename geometry_type = double,
    depth_type t_AdditionalDepthOfSplitStrategy = 2>
  class OrthoTreeBoundingBox final : public OrthoTreeBase<t_DimensionNo, vector_type, box_type, adaptor_type, geometry_type>
  {
  protected:
    using base = OrthoTreeBase<t_DimensionNo, vector_type, box_type, adaptor_type, geometry_type>;
    using EntityDistance = typename base::EntityDistance;
    using BoxDistance = typename base::BoxDistance;
    using GridBoundary = typename base::GridBoundary;

  public:
    using AD = typename base::AD;
    using morton_grid_id_type = typename base::morton_grid_id_type;
    using morton_grid_id_type_cref = typename base::morton_grid_id_type_cref;
    using morton_node_id_type = typename base::morton_node_id_type;
    using morton_node_id_type_cref = typename base::morton_node_id_type_cref;
    using max_element_type = typename base::max_element_type;
    using child_id_type = typename base::child_id_type;

    using Node = typename base::Node;

    static constexpr max_element_type DEFAULT_MAX_ELEMENT = 21;

  private: // Aid functions
    struct Location
    {
      entity_id_type EntityID;
      morton_grid_id_type MinGridID;
      depth_type DepthID;

      constexpr auto operator<(Location const& rightLocation) const
      {
        if (DepthID == rightLocation.DepthID)
          return MinGridID < rightLocation.MinGridID;
        else if (DepthID < rightLocation.DepthID)
        {
          autoc shift = (rightLocation.DepthID - DepthID) * t_DimensionNo;
          autoc minGridIDOfRightOnTheSameDepth = rightLocation.MinGridID >> shift;
          if (MinGridID == minGridIDOfRightOnTheSameDepth)
            return true;

          return MinGridID < minGridIDOfRightOnTheSameDepth;
        }
        else
        {
          autoc shift = (DepthID - rightLocation.DepthID) * t_DimensionNo;
          autoc minGridIDOfLeftOnTheSameDepth = MinGridID >> shift;
          if (rightLocation.MinGridID == minGridIDOfLeftOnTheSameDepth)
            return false;

          return minGridIDOfLeftOnTheSameDepth < rightLocation.MinGridID;
        }
      }
    };

    using LocationContainer = std::vector<Location>;
    using LocationIterator = typename LocationContainer::iterator;

    static constexpr child_id_type getChildIDAtDepth(Location const& location, depth_type depthID, morton_node_id_type_cref nodeIDOnCurrentDepth) noexcept
    {
      assert(depthID <= location.DepthID);
      autoc gridIDOnCurrentDepth = location.MinGridID >> ((location.DepthID - depthID) * t_DimensionNo);
      return base::convertMortonIdToChildId(gridIDOnCurrentDepth - nodeIDOnCurrentDepth);
    }


    void addNodes(
      Node& parentNode,
      morton_node_id_type_cref parentKey,
      LocationIterator& beginLocationIterator,
      LocationIterator const& endLocationIterator,
      morton_grid_id_type_cref firstLocationID,
      depth_type remainingDepthNo) noexcept
    {
      autoc elementNo = std::distance(beginLocationIterator, endLocationIterator);
      if (elementNo < this->m_maxElementNo || remainingDepthNo == 0)
      {
        if (elementNo == 0)
          return;

        parentNode.Entities.resize(elementNo);
        std::transform(beginLocationIterator, endLocationIterator, parentNode.Entities.begin(), [](autoc& item) { return item.EntityID; });
        beginLocationIterator = endLocationIterator;
        return;
      }

      depth_type currentDepthID = this->m_maxDepthNo - remainingDepthNo;
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

      autoc shift = remainingDepthNo * t_DimensionNo;
      autoc locationStepNo = morton_grid_id_type{ 1 } << shift;
      autoc parentFlag = parentKey << t_DimensionNo;
      autoc currentDepthLocationID = firstLocationID >> shift;

      while (beginLocationIterator != endLocationIterator)
      {
        autoc actualChildID = getChildIDAtDepth(*beginLocationIterator, currentDepthID, currentDepthLocationID);
        autoc actualEndLocationIterator = std::partition_point(beginLocationIterator, endLocationIterator, [&](autoc& location) {
          autoc childID = getChildIDAtDepth(location, currentDepthID, currentDepthLocationID);
          return actualChildID == childID;
        });

        autoc actualChildID_ = morton_grid_id_type(actualChildID);
        morton_grid_id_type const childKey = parentFlag | actualChildID_;
        morton_grid_id_type const firstChildLocationID = firstLocationID + actualChildID_ * locationStepNo;

        auto& nodeChild = this->createChild(parentNode, actualChildID, childKey);
        this->addNodes(nodeChild, childKey, beginLocationIterator, actualEndLocationIterator, firstChildLocationID, remainingDepthNo);
      }
    }


    void split(
      std::array<std::array<grid_id_type, t_DimensionNo>, 2> const& boxMinMaxGridID,
      size_t entityID,
      LocationContainer& locations,
      LocationContainer* additionalLocations) const noexcept
    {
      depth_type depthID = locations[entityID].DepthID + t_AdditionalDepthOfSplitStrategy;
      if (depthID > this->m_maxDepthNo)
        depthID = this->m_maxDepthNo;

      autoc remainingDepthNo = static_cast<depth_type>(this->m_maxDepthNo - depthID);
      autoc gridStepNo = static_cast<grid_id_type>(pow2(remainingDepthNo));

      auto gridBoundaries = std::array<GridBoundary, t_DimensionNo>{};
      uint64_t boxNoByGrid = 1;
      for (dim_type dimensionID = 0; dimensionID < t_DimensionNo; ++dimensionID)
      {
        grid_id_type const firstGridSplit = (boxMinMaxGridID[0][dimensionID] / gridStepNo) + grid_id_type{ 1 };
        grid_id_type const lastGridSplit = (boxMinMaxGridID[1][dimensionID] / gridStepNo);
        grid_id_type const gridIDNo = (lastGridSplit < firstGridSplit ? 0 : (lastGridSplit - firstGridSplit + 1)) + 1;
        boxNoByGrid *= gridIDNo;
        if (boxNoByGrid >= this->CHILD_NO)
          return;

        gridBoundaries[dimensionID] = { boxMinMaxGridID[0][dimensionID], firstGridSplit, lastGridSplit + 1 };
      }

      auto gridIDs = std::vector<std::array<grid_id_type, t_DimensionNo>>{};
      gridIDs.reserve(boxNoByGrid);
      auto temporaryGridID = std::array<grid_id_type, t_DimensionNo>{};
      base::template constructGridIDListRecursively<t_DimensionNo>(gridStepNo, gridBoundaries, temporaryGridID, gridIDs);

      autoc boxNo = gridIDs.size();
      autoc shift = remainingDepthNo * t_DimensionNo;


      // First element into entityID
      locations[entityID].DepthID = depthID;
      locations[entityID].MinGridID = base::MortonEncode(gridIDs[0]) >> shift;

      size_t locationNo = 0;
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
      for (size_t iBox = 0; iBox < additionalBoxNo; ++iBox)
      {
        auto& location = additionalLocations->at(locationNo + iBox);
        location.EntityID = entityID;
        location.DepthID = depthID;
        location.MinGridID = base::MortonEncode(gridIDs[iBox + 1]) >> shift;
      }
    }


    void setLocation(box_type const& box, size_t entityID, LocationContainer& locations, std::vector<LocationContainer>* additionalLocations = nullptr) const noexcept
    {
      autoc boxMinMaxGridID = this->getGridIdBox(box);

      auto& location = locations[entityID];
      location.EntityID = entityID;
      location.DepthID = this->m_maxDepthNo;

      location.MinGridID = base::MortonEncode(boxMinMaxGridID[0]);
      autoc maxGridID = base::MortonEncode(boxMinMaxGridID[1]);
      if (location.MinGridID == maxGridID)
        return;

      autoc minGridID = location.MinGridID;
      for (auto flagDiffOfLocation = location.MinGridID ^ maxGridID; base::IsValidKey(flagDiffOfLocation);
           flagDiffOfLocation >>= t_DimensionNo, --location.DepthID)
        location.MinGridID >>= t_DimensionNo;

      if constexpr (t_AdditionalDepthOfSplitStrategy > 0)
      {
        autoc remainingDepthNo = this->m_maxDepthNo - location.DepthID;
        if (base::IsValidKey((maxGridID - minGridID) >> (remainingDepthNo * t_DimensionNo - 1)))
          return; // all nodes are touched, it is leaved.

        this->split(boxMinMaxGridID, entityID, locations, additionalLocations ? &additionalLocations->at(entityID) : nullptr);
      }
    }

  public: // Create
    // Ctors
    OrthoTreeBoundingBox() = default;
    OrthoTreeBoundingBox(
      std::span<box_type const> const& boxes,
      depth_type maxDepthNo,
      std::optional<box_type> const& oBoxSpace = std::nullopt,
      max_element_type nElementMaxInNode = DEFAULT_MAX_ELEMENT) noexcept
    {
      Create(*this, boxes, maxDepthNo, oBoxSpace, nElementMaxInNode);
    }

    // Create
    template<typename execution_policy_type = std::execution::unsequenced_policy>
    static void Create(
      OrthoTreeBoundingBox& tree,
      std::span<box_type const> const& boxes,
      depth_type maxDepthIn = 0,
      std::optional<box_type> const& boxSpaceOptional = std::nullopt,
      max_element_type maxElementNoInNode = DEFAULT_MAX_ELEMENT) noexcept
    {
      autoc boxSpace = boxSpaceOptional.has_value() ? *boxSpaceOptional : AD::box_of_boxes(boxes);
      autoc entityNo = boxes.size();
      autoc maxDepthNo = maxDepthIn == 0 ? base::EstimateMaxDepth(entityNo, maxElementNoInNode) : maxDepthIn;
      tree.Init(boxSpace, maxDepthNo, maxElementNoInNode);

      base::reserveContainer(tree.m_nodes, base::EstimateNodeNumber(entityNo, maxDepthNo, maxElementNoInNode));
      if (entityNo == 0)
        return;

      autoc rootKey = base::GetRootKey();
      auto& nodeRoot = cont_at(tree.m_nodes, rootKey);

      autoce isNoSplit = t_AdditionalDepthOfSplitStrategy == 0;
      autoce isNonParallel = std::is_same<execution_policy_type, std::execution::unsequenced_policy>::value ||
        std::is_same<execution_policy_type, std::execution::sequenced_policy>::value;

      auto epf = execution_policy_type{}; // GCC 11.3
      auto locations = LocationContainer(entityNo);
      locations.reserve(isNoSplit ? entityNo : (entityNo * std::min<size_t>(10, base::CHILD_NO * t_AdditionalDepthOfSplitStrategy)));
      if constexpr (isNoSplit || isNonParallel)
      {
        std::for_each(epf, boxes.begin(), boxes.end(), [&tree, &boxes, &locations](autoc& box) {
          entity_id_type const entityID = std::distance(&boxes[0], &box);
          tree.setLocation(box, entityID, locations);
        });
      }
      else
      {
        auto additionalLocations = std::vector<LocationContainer>(entityNo);
        std::for_each(epf, boxes.begin(), boxes.end(), [&tree, &boxes, &locations, &additionalLocations](autoc& box) {
          entity_id_type const entityID = std::distance(&boxes[0], &box);
          tree.setLocation(box, entityID, locations, &additionalLocations);
        });

        auto additionalLocationSizes = std::vector<size_t>(entityNo);
        auto epe = execution_policy_type{};
        std::transform_exclusive_scan(
          epe, additionalLocations.begin(), additionalLocations.end(), additionalLocationSizes.begin(), entityNo, std::plus<size_t>(), [](autoc& adds) {
            return adds.size();
          });

        locations.resize(additionalLocationSizes.back() + additionalLocations.back().size());
        auto epf2 = execution_policy_type{}; // GCC 11.3
        std::for_each(epf2, additionalLocations.begin(), additionalLocations.end(), [&locations, &additionalLocationSizes, &additionalLocations](auto& additionalLocation) {
          if (additionalLocation.empty())
            return;

          entity_id_type const entityID = std::distance(&additionalLocations[0], &additionalLocation);
          std::copy(additionalLocation.begin(), additionalLocation.end(), std::next(locations.begin(), additionalLocationSizes[entityID]));
        });
      }

      auto eps = execution_policy_type{}; // GCC 11.3
      std::sort(eps, locations.begin(), locations.end());

      auto beginLocationIterator = locations.begin();
      tree.addNodes(nodeRoot, rootKey, beginLocationIterator, locations.end(), morton_node_id_type{ 0 }, maxDepthNo);
      if constexpr (t_AdditionalDepthOfSplitStrategy > 0)
      {
        // Eliminate duplicates. Not all sub-nodes will be created due to the maxElementNoInNode, which cause duplicates in the parent nodes.
        auto epsp = execution_policy_type{}; // GCC 11.3
        std::for_each(epsp, tree.m_nodes.begin(), tree.m_nodes.end(), [](auto& pairOfKeyAndNode) {
          auto& node = pairOfKeyAndNode.second;
          std::ranges::sort(node.Entities);
          node.Entities.erase(std::unique(node.Entities.begin(), node.Entities.end()), node.Entities.end());
        });
      }
    }


  public: // Edit functions
    // Find smallest node which contains the box by grid id description
    morton_node_id_type FindSmallestNode(std::array<std::array<grid_id_type, t_DimensionNo>, 2> const& entityMinMaxGridID) const noexcept
    {
      auto minLocationID = base::MortonEncode(entityMinMaxGridID[0]);
      auto maxLocationID = base::MortonEncode(entityMinMaxGridID[1]);

      auto depthNo = this->m_maxDepthNo;
      for (auto locationDiffFlag = minLocationID ^ maxLocationID; base::IsValidKey(locationDiffFlag); locationDiffFlag >>= t_DimensionNo, --depthNo)
        minLocationID >>= t_DimensionNo;

      autoc endIterator = this->m_nodes.end();
      for (auto smallestNodeKey = this->GetHash(depthNo, minLocationID); base::IsValidKey(smallestNodeKey); smallestNodeKey >>= t_DimensionNo)
        if (this->m_nodes.find(smallestNodeKey) != endIterator)
          return smallestNodeKey;

      return morton_node_id_type{}; // Not found
    }


    // Find smallest node which contains the box
    morton_node_id_type FindSmallestNode(box_type const& box) const noexcept
    {
      if (!AD::are_boxes_overlapped(this->m_boxSpace, box))
        return morton_node_id_type{};

      return FindSmallestNode(this->getGridIdBox(box));
    }


    // Insert item into a node. If doInsertToLeaf is true: The smallest node will be chosen by the max depth. If doInsertToLeaf is false: The smallest existing level on the branch will be chosen.
    bool Insert(entity_id_type entityID, box_type const& box, bool doInsertToLeaf = false) noexcept
    {
      if (!AD::are_boxes_overlapped(this->m_boxSpace, box))
        return false;

      autoc smallestNodeKey = FindSmallestNode(box);
      if (!base::IsValidKey(smallestNodeKey))
        return false; // new box is not in the handled space domain

      auto locations = std::vector<Location>(1);
      setLocation(box, 0, locations);

      for (autoc& location : locations)
      {
        autoc nodeKey = this->GetHash(location.DepthID, location.MinGridID);
        if (!this->template insert<t_AdditionalDepthOfSplitStrategy == 0>(nodeKey, smallestNodeKey, entityID, doInsertToLeaf))
          return false;
      }

      return true;
    }


  private:
    bool doErase(morton_node_id_type_cref nodeKey, entity_id_type entityID) noexcept
    {
      auto& idList = cont_at(this->m_nodes, nodeKey).Entities;
      autoc endIteratorAfterRemove = std::remove(idList.begin(), idList.end(), entityID);
      if (endIteratorAfterRemove == idList.end())
        return false; // id was not registered previously.

      idList.erase(endIteratorAfterRemove, idList.end());
      return true;
    }


    template<depth_type t_remainingDepth>
    bool doEraseRec(morton_node_id_type_cref nodeKey, entity_id_type entityID) noexcept
    {
      auto ret = this->doErase(nodeKey, entityID);
      if constexpr (t_remainingDepth > 0)
      {
        autoc& node = this->GetNode(nodeKey);
        for (morton_node_id_type_cref childKey : node.GetChildren())
          ret |= doEraseRec<t_remainingDepth - 1>(childKey, entityID);
      }
      return ret;
    }


  public:
    // Erase id, aided with the original bounding box
    template<bool t_doReduceAllID = true>
    bool Erase(entity_id_type entityIDToErase, box_type const& box) noexcept
    {
      autoc smallestNodeKey = FindSmallestNode(box);
      if (!base::IsValidKey(smallestNodeKey))
        return false; // old box is not in the handled space domain

      if (doEraseRec<t_AdditionalDepthOfSplitStrategy>(smallestNodeKey, entityIDToErase))
      {
        if constexpr (t_doReduceAllID)
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
    template<bool t_doReduceAllID = true>
    constexpr bool EraseId(entity_id_type idErase) noexcept
    {
      bool bErased = false;
      if constexpr (t_AdditionalDepthOfSplitStrategy == 0)
        bErased = std::ranges::any_of(this->m_nodes, [&](auto& pairNode) { return erase(pairNode.second.Entities, idErase); });
      else
        std::ranges::for_each(this->m_nodes, [&](auto& pairNode) { bErased |= erase(pairNode.second.Entities, idErase) > 0; });

      if (!bErased)
        return false;

      if constexpr (t_doReduceAllID)
        std::ranges::for_each(this->m_nodes, [&](auto& pairNode) {
          for (auto& id : pairNode.second.Entities)
            id -= idErase < id;
        });

      return true;
    }


    // Update id by the new bounding box information
    bool Update(entity_id_type entityID, box_type const& boxNew, bool doInsertToLeaf = false) noexcept
    {
      if (!AD::are_boxes_overlapped(this->m_boxSpace, boxNew))
        return false;

      if (!this->EraseId<false>(entityID))
        return false;

      return this->Insert(entityID, boxNew, doInsertToLeaf);
    }


    // Update id by the new point information and the erase part is aided by the old bounding box geometry data
    bool Update(entity_id_type entityID, box_type const& oldBox, box_type const& newBox, bool doInsertToLeaf = false) noexcept
    {
      if (!AD::are_boxes_overlapped(this->m_boxSpace, newBox))
        return false;

      if constexpr (t_AdditionalDepthOfSplitStrategy == 0)
        if (FindSmallestNode(oldBox) == FindSmallestNode(newBox))
          return true;

      if (!this->Erase<false>(entityID, oldBox))
        return false; // entityID was not registered previously.

      return this->Insert(entityID, newBox, doInsertToLeaf);
    }


  private:
    constexpr std::array<std::array<grid_id_type, t_DimensionNo>, 2> getGridIdPointEdge(vector_type const& point) const noexcept
    {
      autoc& minSpacePoint = AD::box_min_c(this->m_boxSpace);

      auto pointMinMaxGridID = std::array<std::array<grid_id_type, t_DimensionNo>, 2>{};
      for (dim_type dimensionID = 0; dimensionID < t_DimensionNo; ++dimensionID)
      {
        autoc rasterID = static_cast<double>(adaptor_type::point_comp_c(point, dimensionID) - adaptor_type::point_comp_c(minSpacePoint, dimensionID)) *
                         this->m_rasterizerFactors[dimensionID];
        pointMinMaxGridID[0][dimensionID] = pointMinMaxGridID[1][dimensionID] = static_cast<grid_id_type>(rasterID);
        pointMinMaxGridID[0][dimensionID] -= (pointMinMaxGridID[0][dimensionID] > 0) && (floor(rasterID) == rasterID);
      }
      return pointMinMaxGridID;
    }


    void pickSearch(
      vector_type const& pickPoint, std::span<box_type const> const& boxes, Node const& parentNode, std::vector<entity_id_type>& foundEntitiyIDs) const noexcept
    {
      std::ranges::copy_if(parentNode.Entities, back_inserter(foundEntitiyIDs), [&](autoc id) {
        return AD::does_box_contain_point(boxes[id], pickPoint);
      });

      for (morton_node_id_type_cref keyChild : parentNode.GetChildren())
      {
        autoc& childNode = this->GetNode(keyChild);

        if (!AD::does_box_contain_point(childNode.Box, pickPoint))
          continue;

        pickSearch(pickPoint, boxes, childNode, foundEntitiyIDs);
      }
    }


  public: // Search functions
    // Pick search
    std::vector<entity_id_type> PickSearch(vector_type const& pickPoint, std::span<box_type const> const& boxes) const noexcept
    {
      auto foundEntitiyIDs = std::vector<entity_id_type>();
      if (!AD::does_box_contain_point(this->m_boxSpace, pickPoint))
        return foundEntitiyIDs;

      foundEntitiyIDs.reserve(100);

      autoc endIteratorOfNodes = this->m_nodes.end();
      autoc[minGridID, maxGridID] = this->getGridIdPointEdge(pickPoint);
      auto locationID = base::MortonEncode(minGridID);

      auto nodeKey = this->GetHash(this->m_maxDepthNo, locationID);
      if (minGridID != maxGridID) // Pick point is on the nodes edge. It must check more nodes downward.
      {
        autoc maxLocationID = base::MortonEncode(maxGridID);
        auto depthID = this->m_maxDepthNo;
        for (auto locationDiffFlag = locationID ^ maxLocationID; base::IsValidKey(locationDiffFlag); locationDiffFlag >>= t_DimensionNo, --depthID)
          locationID >>= t_DimensionNo;

        autoc rangeKey = this->GetHash(depthID, locationID);
        nodeKey = this->FindSmallestNodeKey(rangeKey);
        autoc nodeIterator = this->m_nodes.find(nodeKey);
        if (nodeIterator != endIteratorOfNodes)
          pickSearch(pickPoint, boxes, nodeIterator->second, foundEntitiyIDs);

        nodeKey >>= t_DimensionNo;
      }

      for (; base::IsValidKey(nodeKey); nodeKey >>= t_DimensionNo)
      {
        autoc nodeIterator = this->m_nodes.find(nodeKey);
        if (nodeIterator == endIteratorOfNodes)
          continue;

        std::ranges::copy_if(nodeIterator->second.Entities, std::back_inserter(foundEntitiyIDs), [&](autoc entityID) {
          return AD::does_box_contain_point(boxes[entityID], pickPoint);
        });
      }

      return foundEntitiyIDs;
    }


    // Range search
    template<bool t_isFullyContained = true>
    std::vector<entity_id_type> RangeSearch(box_type const& range, std::span<box_type const> const& boxes) const noexcept
    {
      auto foundEntities = std::vector<entity_id_type>();

      if (!this->template rangeSearchRoot<box_type, t_isFullyContained, false, false, true>(range, boxes, foundEntities))
        return {};

      if constexpr (t_AdditionalDepthOfSplitStrategy > 0)
      {
        std::ranges::sort(foundEntities);
        autoc itEnd = std::unique(foundEntities.begin(), foundEntities.end());
        foundEntities.erase(itEnd, foundEntities.end());
      }

      return foundEntities;
    }


    // Plane intersection (Plane equation: dotProduct(planeNormal, point) = distanceOfOrigo)
    std::vector<entity_id_type> PlaneIntersection(
      geometry_type const& distanceOfOrigo, vector_type const& planeNormal, geometry_type tolerance, std::span<box_type const> const& boxes) const noexcept
    {
      assert(AD::is_normalized_vector(planeNormal));

      auto results = std::vector<entity_id_type>{};
      if constexpr (t_DimensionNo < 3) // under 3 dimension, every boxes will be intersected.
      {
        results.resize(boxes.size());
        std::iota(results.begin(), results.end(), 0);
        return results;
      }

      autoc selector = [&](morton_node_id_type_cref, Node const& node) -> bool {
        return AD::does_plane_intersect(node.Box, distanceOfOrigo, planeNormal, tolerance);
      };

      autoc procedure = [&](morton_node_id_type_cref, Node const& node) {
        for (autoc entityID : node.Entities)
          if (AD::does_plane_intersect(boxes[entityID], distanceOfOrigo, planeNormal, tolerance))
            if (std::find(results.begin(), results.end(), entityID) == results.end())
              results.emplace_back(entityID);
      };

      this->VisitNodesInDFS(base::GetRootKey(), procedure, selector);

      return results;
    }


    // Collision detection: Returns all overlapping boxes from the source trees.
    static std::vector<std::pair<entity_id_type, entity_id_type>> CollisionDetection(
      OrthoTreeBoundingBox const& leftTree,
      std::span<box_type const> const& leftBoxes,
      OrthoTreeBoundingBox const& rightTree,
      std::span<box_type const> const& rightBoxes) noexcept
    {
      using NodeIterator = typename base::template UnderlyingContainer<Node>::const_iterator;
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

      auto results = std::vector<std::pair<entity_id_type, entity_id_type>>{};
      results.reserve(leftBoxes.size() / 10);

      autoc rootKey = base::GetRootKey();
      autoc trees = std::array{ &leftTree, &rightTree };

      auto nodePairToProceed = std::queue<ParentIteratorArray>{};
      for (nodePairToProceed.push({
             NodeIteratorAndStatus{ leftTree.m_nodes.find(rootKey), false},
             NodeIteratorAndStatus{rightTree.m_nodes.find(rootKey), false}
      });
           !nodePairToProceed.empty();
           nodePairToProceed.pop())
      {
        autoc& parentNodePair = nodePairToProceed.front();

        // Check the current ascendant content
        {
          for (autoc leftEntityID : parentNodePair[Left].Iterator->second.Entities)
            for (autoc rightEntityID : parentNodePair[Right].Iterator->second.Entities)
              if (AD::are_boxes_overlapped(leftBoxes[leftEntityID], rightBoxes[rightEntityID], false))
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
          std::ranges::transform(childIDs, childNodes[sideID].begin(), [&](morton_node_id_type_cref childKey) -> NodeIteratorAndStatus {
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
              if (AD::are_boxes_overlapped(leftChildNode.Iterator->second.Box, rightChildNode.Iterator->second.Box, false))
                nodePairToProceed.emplace(std::array{ leftChildNode, rightChildNode });
      }

      if constexpr (t_AdditionalDepthOfSplitStrategy > 0)
      {
        std::ranges::sort(results);
        results.erase(std::unique(results.begin(), results.end()), results.end());
      }

      return results;
    }


    // Collision detection: Returns all overlapping boxes from the source trees.
    std::vector<std::pair<entity_id_type, entity_id_type>> CollisionDetection(
      std::span<box_type const> const& boxes, OrthoTreeBoundingBox const& otherTree, std::span<box_type const> const& otherBoxes) const noexcept
    {
      return CollisionDetection(*this, boxes, otherTree, otherBoxes);
    }

  public:
    // Collision detection between the stored elements from top to bottom logic
    template<typename execution_policy_type = std::execution::unsequenced_policy>
    std::vector<std::pair<entity_id_type, entity_id_type>> CollisionDetectionObsolete(std::span<box_type const> const& boxes) const noexcept
    {
      autoc entityNo = boxes.size();

      auto vidCheck = std::vector<entity_id_type>(entityNo);
      std::iota(vidCheck.begin(), vidCheck.end(), 0);

      auto vvidCollision = std::vector<std::vector<entity_id_type>>(vidCheck.size());
      auto ep = execution_policy_type{}; // GCC 11.3
      std::transform(ep, vidCheck.begin(), vidCheck.end(), vvidCollision.begin(), [&boxes, this](autoc idCheck) -> std::vector<entity_id_type> {
        auto foundEntities = std::vector<entity_id_type>();

        autoc entityNo = boxes.size();
        if (!this->template rangeSearchRoot<box_type, false, true, false, true>(boxes[idCheck], boxes, foundEntities, idCheck))
          return {};

        if constexpr (t_AdditionalDepthOfSplitStrategy > 0)
        {
          std::ranges::sort(foundEntities);
          autoc itEnd = std::unique(foundEntities.begin(), foundEntities.end());
          foundEntities.erase(itEnd, foundEntities.end());
        }

        return foundEntities;
      });

      auto vPair = std::vector<std::pair<entity_id_type, entity_id_type>>{};
      if (entityNo > 10)
        vPair.reserve(entityNo / 10);

      for (autoc idCheck : vidCheck)
        for (autoc idCollide : vvidCollision[idCheck])
          vPair.emplace_back(idCheck, idCollide);

      return vPair;
    }

  public:
    // Collision detection between the stored elements from bottom to top logic
    template<typename execution_policy_type = std::execution::unsequenced_policy>
    std::vector<std::pair<entity_id_type, entity_id_type>> CollisionDetection(std::span<box_type const> const& boxes) const noexcept
    {
      using CollisionDetectionContainer = std::vector<std::pair<entity_id_type, entity_id_type>>;

      autoc entityNo = boxes.size();
      auto collidedEntityPairs = CollisionDetectionContainer();
      collidedEntityPairs.reserve(boxes.size());


      // t_AdditionalDepthOfSplitStrategy version of this algorithm needs a reverse std::map
      auto entityIDNodeMap = std::vector<std::vector<morton_node_id_type>>(entityNo);
      if constexpr (t_AdditionalDepthOfSplitStrategy > 0)
      {
        std::for_each(this->m_nodes.begin(), this->m_nodes.end(), [&entityIDNodeMap](autoc& pairKeyNode) {
          autoc & [nodeKey, node] = pairKeyNode;
          for (autoc entityID : node.Entities)
            entityIDNodeMap[entityID].emplace_back(nodeKey);
        });

        auto ep = execution_policy_type{}; // GCC 11.3
        std::for_each(ep, entityIDNodeMap.begin(), entityIDNodeMap.end(), [](auto& keys) {
          if constexpr (base::IS_LINEAR_TREE)
            std::ranges::sort(keys);
          else
            std::ranges::sort(keys, bitset_arithmetic_compare{});
        });
      }


      // Entities which contain all of the tree could slow the algorithm, so these are eliminated
      auto entityIDsInRoot = std::vector<entity_id_type>();
      {
        autoc& nodeRoot = this->GetNode(this->GetRootKey());
        for (autoc entityID : nodeRoot.Entities)
        {
          if (AD::are_boxes_overlapped(boxes[entityID], this->m_boxSpace))
          {
            for (auto entityIDOther = entityID + 1; entityIDOther < entityNo; ++entityIDOther)
              collidedEntityPairs.emplace_back(entityID, entityIDOther);
          }
          else
            entityIDsInRoot.emplace_back(entityID);
        }
      }

      auto ep = execution_policy_type{}; // GCC 11.3

      // Collision detection node-by-node without duplication
      auto collidedEntityPairsInsideNodes = std::vector<CollisionDetectionContainer>(this->m_nodes.size());
      std::transform(
        ep,
        this->m_nodes.begin(),
        this->m_nodes.end(),
        collidedEntityPairsInsideNodes.begin(),
        [&boxes, &entityIDNodeMap, &entityIDsInRoot, this](autoc& pairKeyNode) -> CollisionDetectionContainer {
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
            auto depthDifference = depth_type(1);
            for (auto parentKey = nodeKey >> t_DimensionNo; base::IsValidKey(parentKey); parentKey >>= t_DimensionNo, --parentDepthID, ++depthDifference)
            {
              autoc& parentEntityIDs = *(parentDepthID == 0 ? &entityIDsInRoot : &this->GetNode(parentKey).Entities);

              if constexpr (t_AdditionalDepthOfSplitStrategy == 0)
              {
                for (autoc entityID : entityIDs)
                  for (autoc entityIDFromParent : parentEntityIDs)
                    if (AD::are_boxes_overlapped_strict(boxes[entityID], boxes[entityIDFromParent]))
                      collidedEntityPairsInsideNode.emplace_back(entityID, entityIDFromParent);
              }
              else
              {
                // t_AdditionalDepthOfSplitStrategy: entityID could occur in multiple node. This algorithm aims to check only the first occurrence's parents.

                auto entityIDsToCheckOnOtherBranch = std::vector<entity_id_type>{};
                entityIDsToCheckOnOtherBranch.reserve(entityNoInNode);
                auto entityIDPairsFromOtherBranch = std::unordered_map<entity_id_type, std::set<entity_id_type>>{};
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
                    else if (depthDifference <= t_AdditionalDepthOfSplitStrategy)
                    {
                      auto keysOfEntitysDepth = std::vector<morton_node_id_type>();
                      for (size_t entityKeyID = 1; entityKeyID < currentNodeKeysNo; ++entityKeyID)
                      {
                        // An earlier node is already check this level
                        {
                          auto& keyOfEntitysDepth = keysOfEntitysDepth.emplace_back(currentNodeKeys[entityKeyID - 1]);
                          autoc prevDepthID = this->GetDepthID(keyOfEntitysDepth);
                          if (prevDepthID > parentDepthID)
                            keyOfEntitysDepth >>= t_DimensionNo * (prevDepthID - parentDepthID);

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

                          for (size_t prevEntityKeyID = 0, parentEntityKeyID = 0; prevEntityKeyID < entityKeyID && parentEntityKeyID < parentNodeKeysNo;)
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
                      if (AD::are_boxes_overlapped_strict(boxes[entityID], boxes[entityIDFromParent]))
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
                      if (!isAlreadyContained && AD::are_boxes_overlapped_strict(boxes[entityID], boxes[entityIDFromParent]))
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
            for (size_t iEntity = 0; iEntity < entityNoInNode; ++iEntity)
            {
              autoc iEntityID = entityIDs[iEntity];
              autoc& entityKeys = entityIDNodeMap[iEntityID];
              autoc entityKeysNo = entityKeys.size();

              for (size_t jEntity = iEntity + 1; jEntity < entityNoInNode; ++jEntity)
              {
                autoc jEntityID = entityIDs[jEntity];
                if constexpr (t_AdditionalDepthOfSplitStrategy == 0)
                {
                  if (AD::are_boxes_overlapped_strict(boxes[iEntityID], boxes[jEntityID]))
                    collidedEntityPairsInsideNode.emplace_back(iEntityID, jEntityID);
                }
                else
                {
                  // Same entities could collide in other nodes, but only the first occurrence should be checked
                  autoc& entityKeysOfJ = entityIDNodeMap[jEntityID];
                  auto isFirstCollisionCheckHappening = entityKeysNo == 1 || entityKeysOfJ.size() == 1;
                  if (!isFirstCollisionCheckHappening)
                  {
                    for (size_t iEntityKey = 0, jEntityKey = 0; iEntityKey < entityKeysNo;)
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
                    if (AD::are_boxes_overlapped_strict(boxes[iEntityID], boxes[jEntityID]))
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


  private:
    void getRayIntersectedAll(
      Node const& node,
      std::span<box_type const> const& boxes,
      vector_type const& rayBasePoint,
      vector_type const& rayHeading,
      geometry_type tolerance,
      geometry_type maxExaminationDistance,
      std::vector<EntityDistance>& foundEntities) const noexcept
    {
      autoc isNodeHit = AD::is_ray_hit(node.Box, rayBasePoint, rayHeading, tolerance);
      if (!isNodeHit)
        return;

      for (autoc entityID : node.Entities)
      {
        autoc entityDistance = AD::is_ray_hit(boxes[entityID], rayBasePoint, rayHeading, tolerance);
        if (entityDistance && (maxExaminationDistance == 0 || entityDistance.value() <= maxExaminationDistance))
          foundEntities.push_back({ { entityDistance.value() }, entityID });
      }

      for (autoc childKey : node.GetChildren())
        getRayIntersectedAll(this->GetNode(childKey), boxes, rayBasePoint, rayHeading, tolerance, maxExaminationDistance, foundEntities);
    }


    void getRayIntersectedFirst(
      Node const& node,
      std::span<box_type const> const& boxes,
      vector_type const& rayBasePoint,
      vector_type const& rayHeading,
      geometry_type tolerance,
      std::multiset<EntityDistance>& foundEntities) const noexcept
    {
      autoc maxExaminationDistance =
        foundEntities.empty() ? std::numeric_limits<double>::infinity() : static_cast<double>(foundEntities.rbegin()->Distance);
      for (autoc entityID : node.Entities)
      {
        autoc distance = AD::is_ray_hit(boxes[entityID], rayBasePoint, rayHeading, tolerance);
        if (!distance)
          continue;

        if (*distance > maxExaminationDistance)
          continue;

        foundEntities.insert({ { *distance }, entityID });
      }

      auto nodeDistances = std::multiset<BoxDistance>();
      for (autoc childKey : node.GetChildren())
      {
        autoc& nodeChild = this->GetNode(childKey);
        autoc distance = AD::is_ray_hit(nodeChild.Box, rayBasePoint, rayHeading, tolerance);
        if (!distance)
          continue;

        if (*distance > maxExaminationDistance)
          continue;

        nodeDistances.insert({ { static_cast<geometry_type>(distance.value()) }, childKey, nodeChild });
      }

      for (autoc& nodeDistance : nodeDistances)
        getRayIntersectedFirst(nodeDistance.Node, boxes, rayBasePoint, rayHeading, tolerance, foundEntities);
    }


  public:
    // Get all box which is intersected by the ray in order
    std::vector<entity_id_type> RayIntersectedAll(
      vector_type const& rayBasePointPoint,
      vector_type const& rayHeading,
      std::span<box_type const> const& boxes,
      geometry_type tolerance,
      geometry_type maxExaminationDistance = 0) const noexcept
    {
      auto foundEntities = std::vector<EntityDistance>();
      foundEntities.reserve(20);
      getRayIntersectedAll(this->GetNode(this->GetRootKey()), boxes, rayBasePointPoint, rayHeading, tolerance, maxExaminationDistance, foundEntities);

      autoc beginIteratorOfEntities = foundEntities.begin();
      auto endIteratorOfEntities = foundEntities.end();
      std::sort(beginIteratorOfEntities, endIteratorOfEntities);
      if constexpr (t_AdditionalDepthOfSplitStrategy > 0)
        endIteratorOfEntities =
          std::unique(beginIteratorOfEntities, endIteratorOfEntities, [](autoc& lhs, autoc& rhs) { return lhs.EntityID == rhs.EntityID; });

      auto foundEntityIDs = std::vector<entity_id_type>(std::distance(beginIteratorOfEntities, endIteratorOfEntities));
      std::transform(beginIteratorOfEntities, endIteratorOfEntities, foundEntityIDs.begin(), [](autoc& entityDistance) {
        return entityDistance.EntityID;
      });
      return foundEntityIDs;
    }


    // Get first box which is intersected by the ray
    std::optional<entity_id_type> RayIntersectedFirst(
      vector_type const& rayBasePointPoint, vector_type const& rayHeading, std::span<box_type const> const& boxes, geometry_type tolerance) const noexcept
    {
      autoc& node = this->GetNode(this->GetRootKey());
      autoc distance = AD::is_ray_hit(node.Box, rayBasePointPoint, rayHeading, tolerance);
      if (!distance)
        return std::nullopt;

      auto foundEntities = std::multiset<EntityDistance>();
      getRayIntersectedFirst(node, boxes, rayBasePointPoint, rayHeading, tolerance, foundEntities);
      if (foundEntities.empty())
        return std::nullopt;

      return foundEntities.begin()->EntityID;
    }
  };


  template<dim_type t_DimensionNo, typename geometry_type = double>
  using PointND = std::array<geometry_type, t_DimensionNo>;


  template<dim_type t_DimensionNo, typename geometry_type = double>
  struct BoundingBoxND
  {
    PointND<t_DimensionNo, geometry_type> Min;
    PointND<t_DimensionNo, geometry_type> Max;
  };


  // Aliases
  using Point1D = OrthoTree::PointND<1>;
  using Point2D = OrthoTree::PointND<2>;
  using Point3D = OrthoTree::PointND<3>;
  using BoundingBox1D = OrthoTree::BoundingBoxND<1>;
  using BoundingBox2D = OrthoTree::BoundingBoxND<2>;
  using BoundingBox3D = OrthoTree::BoundingBoxND<3>;

  template<size_t t_DimensionNo>
  using TreePointND = OrthoTree::OrthoTreePoint<t_DimensionNo, OrthoTree::PointND<t_DimensionNo>, OrthoTree::BoundingBoxND<t_DimensionNo>>;
  template<size_t t_DimensionNo, uint32_t t_AdditionalDepthOfSplitStrategy = 2>
  using TreeBoxND = OrthoTree::OrthoTreeBoundingBox<
    t_DimensionNo,
    OrthoTree::PointND<t_DimensionNo>,
    OrthoTree::BoundingBoxND<t_DimensionNo>,
    AdaptorGeneral<t_DimensionNo, OrthoTree::PointND<t_DimensionNo>, OrthoTree::BoundingBoxND<t_DimensionNo>>,
    double,
    t_AdditionalDepthOfSplitStrategy>;

  // Dualtree for points
  using DualtreePoint = TreePointND<1>;

  // Dualtree for bounding boxes
  using DualtreeBox = TreeBoxND<1>;

  // Quadtree for points
  using QuadtreePoint = TreePointND<2>;

  // Quadtree for bounding boxes
  using QuadtreeBox = TreeBoxND<2>;

  // Octree for points
  using OctreePoint = TreePointND<3>;

  // Octree for bounding boxes
  using OctreeBox = TreeBoxND<3>;

  // Hexatree for points
  using HexatreePoint = TreePointND<4>;

  // Hexatree for bounding boxes
  using HexatreeBox = TreeBoxND<4>;

  // NTrees for higher dimensions
  using TreePoint16D = TreePointND<16>;
  using TreeBox16D = TreeBoxND<16>;
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

#ifdef UNDEF_HASSERT
#undef HASSERT
#undef UNDEF_HASSERT
#endif

#undef LOOPIVDEP

#endif // ORTHOTREE_GUARD
