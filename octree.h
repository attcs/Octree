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

#ifndef NTREE_GUARD
#define NTREE_GUARD

#include <array>
#include <bitset>
#include <optional>
#include <span>
#include <vector>
#include <set>
#include <map>
#include <unordered_map>
#include <queue>
#include <tuple>
#include <algorithm>
#include <numeric>
#include <concepts>
#include <type_traits>
#include <functional>
#include <assert.h>


#ifndef autoc
#define autoc auto const
#define undef_autoc
#endif

#ifndef autoce
#define autoce auto constexpr
#define undef_autoce
#endif

#ifndef HASSERT
#define HASSERT(isOk) assert(isOk); if (!isOk) { exit(1); }
#define UNDEF_HASSERT
#endif


namespace NTree
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
#pragma warning(disable: 4715)
#endif
  // Crash the program if out_of_range exception is raised
  template<typename var_type, typename index_type, typename container_type>
  inline auto const& cont_at(container_type const& container, typename std::remove_reference_t<container_type>::key_type const& id)
  {
    try { return container.at(id); }
    catch (std::out_of_range const&) { HASSERT(false); }
  }

  // Crash the program if out_of_range exception is raised
  template<typename container_type>
  inline auto& cont_at(container_type& container, typename std::remove_reference_t<container_type>::key_type const& id)
  {
    try { return container.at(id); }
    catch (std::out_of_range const&) { HASSERT(false); }
  }


#ifdef _MSC_VER
#pragma warning(default: 4715)
#endif

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

  constexpr uint64_t pow_ce(uint64_t a, unsigned char e) { return e == 0 ? 1 : a * pow_ce(a, e - 1); }
  namespace
  {
    using std::array;
    using std::bitset;
    using std::span;
    using std::vector;
    using std::unordered_map;
    using std::map;
    using std::queue;
    using std::multiset;
  }


  // Grid id
  using grid_id_type = uint64_t;

  // Type of the dimension
  using dim_type = uint8_t;

  // Content id type
  using entity_id_type = size_t;


  template<dim_type nDimension, typename geometry_type = double>
  using Point = array<geometry_type, nDimension>;

  template <dim_type nDimension, typename geometry_type = double>
  struct BoundingBox
  {
    Point<nDimension, geometry_type> Min;
    Point<nDimension, geometry_type> Max;
  };


  // Adaptor concepts

  template <class adaptor_type, typename point_type, typename box_type, typename geometry_type = double>
  concept AdaptorBasicsConcept =
    requires (point_type & pt, dim_type iDimension) { {adaptor_type::point_comp(pt, iDimension)}->std::convertible_to<geometry_type&>; }
  && requires (point_type const& pt, dim_type iDimension) { {adaptor_type::point_comp_c(pt, iDimension)}->std::convertible_to<geometry_type>; }
  && requires (box_type& box) { { adaptor_type::box_min(box) }->std::convertible_to<point_type&>; }
  && requires (box_type& box) { { adaptor_type::box_max(box) }->std::convertible_to<point_type&>; }
  && requires (box_type const& box) { { adaptor_type::box_min_c(box) }->std::convertible_to<point_type>; }
  && requires (box_type const& box) { { adaptor_type::box_max_c(box) }->std::convertible_to<point_type>; }
  ;

  template <class adaptor_type, typename point_type, typename box_type, typename geometry_type = double>
  concept AdaptorConcept =
    requires { AdaptorBasicsConcept<adaptor_type, point_type, box_type, geometry_type>; }
  && requires (box_type const& box, point_type const& pt) { { adaptor_type::does_box_contain_point(box, pt)}->std::convertible_to<bool>; }
  && requires (box_type const& e1, box_type const& e2, bool e1_must_contain_e2) { { adaptor_type::are_boxes_overlapped(e1, e2, e1_must_contain_e2)}->std::convertible_to<bool>; }
  && requires (span<point_type const> const& vPoint) { { adaptor_type::box_of_points(vPoint)}->std::convertible_to<box_type>; }
  && requires (span<box_type const> const& vBox) { { adaptor_type::box_of_boxes(vBox)}->std::convertible_to<box_type>; }
  ;


  // Adaptors

  template <dim_type nDimension, typename point_type, typename box_type, typename geometry_type = double>
  struct AdaptorGeneralBasics
  {
    static constexpr geometry_type& point_comp(point_type& pt, dim_type iDimension) { return pt[iDimension]; }
    static constexpr geometry_type const& point_comp_c(point_type const& pt, dim_type iDimension) { return pt[iDimension]; }

    static constexpr point_type& box_min(box_type& box) { return box.Min; }
    static constexpr point_type& box_max(box_type& box) { return box.Max; }
    static constexpr point_type const& box_min_c(box_type const& box) { return box.Min; }
    static constexpr point_type const& box_max_c(box_type const& box) { return box.Max; }
  };


  template <dim_type nDimension, typename point_type, typename box_type, typename adaptor_basics_type, typename geometry_type = double>
  struct AdaptorGeneralBase : adaptor_basics_type
  {
    using base = adaptor_basics_type;
    static_assert(AdaptorBasicsConcept<base, point_type, box_type, geometry_type>);

    static constexpr geometry_type size2(point_type const& pt)
    {
      auto d2 = geometry_type(0);
      for (dim_type iDim = 0; iDim < nDimension; ++iDim)
      {
        autoc d = base::point_comp_c(pt, iDim);
        d2 += d * d;
      }
      return d2;
    }

    static constexpr geometry_type size(point_type const& pt)
    {
      return sqrt(size2(pt));
    }

    static constexpr point_type substract(point_type const& ptL, point_type const& ptR)
    {
      auto pt = point_type{};
      for (dim_type iDim = 0; iDim < nDimension; ++iDim)
        base::point_comp(pt, iDim) = base::point_comp_c(ptL, iDim) - base::point_comp_c(ptR, iDim);

      return pt;

    }

    static constexpr geometry_type distance(point_type const& ptL, point_type const& ptR)
    {
      return size(substract(ptL, ptR));
    }

    static constexpr geometry_type distance2(point_type const& ptL, point_type const& ptR)
    {
      return size2(substract(ptL, ptR));
    }

    static constexpr bool are_points_equal(point_type const& ptL, point_type const& ptR, geometry_type rAccuracy)
    {
      return distance2(ptL, ptR) <= rAccuracy * rAccuracy;
    }

    static constexpr bool does_box_contain_point(box_type const& box, point_type const& pt)
    {
      for (dim_type iDimension = 0; iDimension < nDimension; ++iDimension)
        if (!(base::point_comp_c(base::box_min_c(box), iDimension) <= base::point_comp_c(pt, iDimension) && base::point_comp_c(pt, iDimension) <= base::point_comp_c(base::box_max_c(box), iDimension)))
          return false;

      return true;
    }

    static constexpr bool does_box_contain_point_strict(box_type const& box, point_type const& pt)
    {
      for (dim_type iDimension = 0; iDimension < nDimension; ++iDimension)
        if (!(base::point_comp_c(base::box_min_c(box), iDimension) < base::point_comp_c(pt, iDimension) && base::point_comp_c(pt, iDimension) < base::point_comp_c(base::box_max_c(box), iDimension)))
          return false;

      return true;
    }


    static constexpr bool does_point_touch_box(box_type const& box, point_type const& pt)
    {
      for (dim_type iDimension = 0; iDimension < nDimension; ++iDimension)
        if ((base::point_comp_c(base::box_min_c(box), iDimension) == base::point_comp_c(pt, iDimension)))
          return false;

      return true;
    }

    enum EBoxRelation : int8_t { Overlapped = -1, Adjecent = 0, Separated = 1 };
    static constexpr EBoxRelation box_relation(box_type const& e1, box_type const& e2)
    {
      enum EBoxRelationCandidate : uint8_t { OverlappedC = 0x1, AdjecentC = 0x2, SeparatedC = 0x4 };
      int8_t rel = 0;
      for (dim_type iDimension = 0; iDimension < nDimension; ++iDimension)
      {
        if (base::point_comp_c(base::box_min_c(e1), iDimension) < base::point_comp_c(base::box_max_c(e2), iDimension) && base::point_comp_c(base::box_max_c(e1), iDimension) > base::point_comp_c(base::box_min_c(e2), iDimension))
          rel |= EBoxRelationCandidate::OverlappedC;
        else if (base::point_comp_c(base::box_min_c(e1), iDimension) == base::point_comp_c(base::box_max_c(e2), iDimension) || base::point_comp_c(base::box_max_c(e1), iDimension) == base::point_comp_c(base::box_min_c(e2), iDimension))
          rel |= EBoxRelationCandidate::AdjecentC;
        else if (base::point_comp_c(base::box_min_c(e1), iDimension) > base::point_comp_c(base::box_max_c(e2), iDimension) || base::point_comp_c(base::box_max_c(e1), iDimension) < base::point_comp_c(base::box_min_c(e2), iDimension))
          return EBoxRelation::Separated;
      }
      return (rel & EBoxRelationCandidate::AdjecentC) == EBoxRelationCandidate::AdjecentC ? EBoxRelation::Adjecent : EBoxRelation::Overlapped;
    }

    static constexpr bool are_boxes_overlapped(box_type const& e1, box_type const& e2, bool e1_must_contain_e2 = true, bool fOverlapPtTouchAllowed = false)
    {
      autoc e1_contains_e2min = does_box_contain_point(e1, base::box_min_c(e2)); //! ->ATT neve used

      return e1_must_contain_e2
        ? does_box_contain_point(e1, base::box_min_c(e2)) && does_box_contain_point(e1, base::box_max_c(e2))
        : fOverlapPtTouchAllowed
        ? does_box_contain_point(e1, base::box_min_c(e2)) || does_box_contain_point(e1, base::box_max_c(e2)) || does_box_contain_point(e2, base::box_max_c(e1))
        : box_relation(e1, e2) == EBoxRelation::Overlapped
        ;
    }

    static inline box_type box_inverted_init()
    {
      auto ext = box_type{};
      for (dim_type iDimension = 0; iDimension < nDimension; ++iDimension)
      {
        base::point_comp(base::box_min(ext), iDimension) = +std::numeric_limits<geometry_type>::infinity();
        base::point_comp(base::box_max(ext), iDimension) = -std::numeric_limits<geometry_type>::infinity();
      }

      return ext;
    }

    static box_type box_of_points(span<point_type const> const& vPoint)
    {
      auto ext = box_inverted_init();
      for (autoc& pt : vPoint)
        for (dim_type iDimension = 0; iDimension < nDimension; ++iDimension)
        {
          if (base::point_comp_c(base::box_min_c(ext), iDimension) > base::point_comp_c(pt, iDimension))
            base::point_comp(base::box_min(ext), iDimension) = base::point_comp_c(pt, iDimension);

          if (base::point_comp_c(base::box_max_c(ext), iDimension) < base::point_comp_c(pt, iDimension))
            base::point_comp(base::box_max(ext), iDimension) = base::point_comp_c(pt, iDimension);
        }

      return ext;
    }

    static box_type box_of_boxes(span<box_type const> const& vExtent)
    {
      auto ext = box_inverted_init();
      for (autoc& e : vExtent)
        for (dim_type iDimension = 0; iDimension < nDimension; ++iDimension)
        {
          if (base::point_comp_c(base::box_min_c(ext), iDimension) > base::point_comp_c(base::box_min_c(e), iDimension))
            base::point_comp(base::box_min(ext), iDimension) = base::point_comp_c(base::box_min_c(e), iDimension);

          if (base::point_comp_c(base::box_max_c(ext), iDimension) < base::point_comp_c(base::box_max_c(e), iDimension))
            base::point_comp(base::box_max(ext), iDimension) = base::point_comp_c(base::box_max_c(e), iDimension);
        }

      return ext;
    }
  };

  template<dim_type nDimension, typename point_type, typename box_type, typename geometry_type = double>
  using AdaptorGeneral = AdaptorGeneralBase<nDimension, point_type, box_type, AdaptorGeneralBasics<nDimension, point_type, box_type, geometry_type>, geometry_type>;


  // Grid

  template <dim_type nDimension, typename point_type, typename adaptor_type>
  constexpr array<double, nDimension> resolve_grid_get_rasterizer(point_type const& p0, point_type const& p1, grid_id_type n_divide)
  {
    auto aRasterizer = array<double, nDimension>{};
    for (dim_type iDimension = 0; iDimension < nDimension; ++iDimension)
    {
      autoc rExt = adaptor_type::point_comp_c(p1, iDimension) - adaptor_type::point_comp_c(p0, iDimension);
      aRasterizer[iDimension] = rExt == 0 ? 1.0 : (static_cast<double>(n_divide) / rExt);
    }
    return aRasterizer;
  }

  template <dim_type nDimension, typename point_type, typename adaptor_type>
  constexpr array<grid_id_type, nDimension> resolve_grid_id_point(point_type const& pe, point_type const& p0, array<double, nDimension> const& aRasterizer, grid_id_type maxSlot)
  {
    auto aid = array<grid_id_type, nDimension>{};
    for (dim_type iDimension = 0; iDimension < nDimension; ++iDimension)
      aid[iDimension] = std::min<grid_id_type>(maxSlot, static_cast<grid_id_type>(static_cast<double>(adaptor_type::point_comp_c(pe, iDimension) - adaptor_type::point_comp_c(p0, iDimension)) * aRasterizer[iDimension]));

    return aid;
  }


  template <dim_type nDimension, typename point_type, typename adaptor_type>
  auto resolve_grid_id(span<point_type const> const& points, point_type const& p0, point_type const& p1, grid_id_type n_divide)
  {
    autoc aRasterizer = resolve_grid_get_rasterizer<nDimension, point_type, adaptor_type>(p0, p1, n_divide);

    autoc maxSlot = n_divide - 1;

    autoc n = points.size();
    auto aid = vector<array<grid_id_type, nDimension>>(n);
    for (size_t i = 0; i < n; ++i)
      aid[i] = resolve_grid_id_point<nDimension, point_type, adaptor_type>(points[i], p0, aRasterizer, maxSlot);

    return aid;
  }


  template <dim_type nDimension, typename point_type, typename box_type, typename adaptor_type>
  constexpr array<array<grid_id_type, nDimension>, 2> resolve_grid_id_box(box_type const& extent, point_type const& p0, array<double, nDimension> const& aRasterizer, grid_id_type maxSlot)
  {
    autoc aMinMax = array<point_type const*, 2>{ &adaptor_type::box_min_c(extent), & adaptor_type::box_max_c(extent) };

    auto aid = array<array<grid_id_type, nDimension>, 2>{};
    for (dim_type iDimension = 0; iDimension < nDimension; ++iDimension)
    {
      auto aEdge = array<bool, 2>{};
      for (int i = 0; i < 2; ++i)
      {
        autoc& pe = *aMinMax[i];
        autoc id = static_cast<double>(adaptor_type::point_comp_c(pe, iDimension) - adaptor_type::point_comp_c(p0, iDimension)) * aRasterizer[iDimension];
        if (id < 1)
          aid[i][iDimension] = 0;
        else if (id > maxSlot)
          aid[i][iDimension] = maxSlot;
        else if (id == floor(id))
        {
          aid[i][iDimension] = static_cast<grid_id_type>(id - 1);
          aEdge[i] = true;
        }
        else
          aid[i][iDimension] = static_cast<grid_id_type>(id);
      }

      // If the first value is on edge, the lower id was chosen, but maybe the bigger grid would be the appropriate.
      if (aEdge[0] && aid[0][iDimension] < maxSlot && aid[0][iDimension] < aid[1][iDimension])
        ++aid[0][iDimension];
    }
    return aid;
  }


  template <dim_type nDimension, typename box_type, typename point_type, typename adaptor_type>
  auto resolve_grid_id(span<box_type const> const& extents, point_type const& p0, point_type const& p1, grid_id_type n_divide)
  {
    autoc aRasterizer = resolve_grid_get_rasterizer<nDimension, point_type, adaptor_type>(p0, p1, n_divide);

    autoc maxSlot = n_divide - 1;

    autoc n = extents.size();
    auto aid = vector<array<array<grid_id_type, nDimension>, 2>>(n);
    for (size_t i = 0; i < n; ++i)
      aid[i] = resolve_grid_id_box<nDimension, point_type, box_type, adaptor_type>(extents[i], p0, aRasterizer, maxSlot);

    return aid;
  }


  template<size_t N> using bitset_arithmetic = bitset<N>;

  template<size_t N>
  bitset_arithmetic<N> operator+ (bitset_arithmetic<N> const& lhs, bitset_arithmetic<N> const& rhs) noexcept
  {
    bool carry = false;
    auto ans = bitset_arithmetic<N>();
    for (size_t i = 0; i < N; i++)
    {
      autoc sum = (lhs[i] ^ rhs[i]) ^ carry;
      carry = (lhs[i] && rhs[i]) || (lhs[i] && carry) || (rhs[i] && carry);
      ans[i] = sum;
    }

    assert(!carry); // unhandled overflow
    return ans;
  }

  template<size_t N>
  bitset_arithmetic<N> operator+ (bitset_arithmetic<N> const& lhs, size_t rhs) noexcept
  {
    return lhs + bitset_arithmetic<N>(rhs);
  }

  template<size_t N>
  bitset_arithmetic<N> operator- (bitset_arithmetic<N> const& lhs, bitset_arithmetic<N> const& rhs) noexcept
  {
    assert(lhs >= rhs);

    auto ret = lhs;
    bool borrow = false;
    for (size_t i = 0; i < N; ++i)
    {
      if (borrow)
      {
        if (ret[i]) { ret[i] = rhs[i];  borrow = rhs[i]; }
        else { ret[i] = !rhs[i]; borrow = true; }
      }
      else
      {
        if (ret[i]) { ret[i] = !rhs[i]; borrow = false; }
        else { ret[i] = rhs[i];  borrow = rhs[i]; }
      }
    }

    return ret;
  }

  template<size_t N>
  bitset_arithmetic<N> operator- (bitset_arithmetic<N> const& lhs, size_t rhs) noexcept
  {
    return lhs - bitset_arithmetic<N>(rhs);
  }


  template<size_t N>
  bitset_arithmetic<N> operator* (bitset_arithmetic<N> const& lhs, bitset_arithmetic<N> const& rhs) noexcept
  {
    auto ret = bitset_arithmetic<N>{};

    if (lhs.count() < rhs.count())
    {
      for (size_t i = 0; i < N; i++)
        if (lhs[i])
          ret = ret + (rhs << i);
    }
    else
    {
      for (size_t i = 0; i < N; i++)
        if (rhs[i])
          ret = ret + (lhs << i);
    }

    return ret;
  }

  template<size_t N>
  bitset_arithmetic<N> operator* (bitset_arithmetic<N> const& lhs, size_t rhs) noexcept
  {
    return lhs * bitset_arithmetic<N>(rhs);
  }
  template<size_t N>
  bitset_arithmetic<N> operator* (size_t rhs, bitset_arithmetic<N> const& lhs) noexcept
  {
    return lhs * bitset_arithmetic<N>(rhs);
  }


  template<size_t N>
  static std::tuple<bitset_arithmetic<N>, bitset_arithmetic<N>> gf2_div(bitset_arithmetic<N> const& dividend, bitset_arithmetic<N> divisor)
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
      if (dividend[id]) { sig_dividend = id; break; }

    size_t sig_divisor = 0;
    for (size_t i = 0, id = N - 1; i < N; ++i, --id)
      if (divisor[id]) { sig_divisor = id; break; }

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
  bitset_arithmetic<N> operator / (bitset_arithmetic<N> const& dividend, bitset_arithmetic<N> const& divisor)
  {
    return std::get<0>(gf2_div(dividend, divisor));
  }


  template<size_t N>
  auto operator<=> (bitset_arithmetic<N> const& lhs, bitset_arithmetic<N> const& rhs) noexcept
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
  template<dim_type nDimension, typename point_type, typename box_type, typename adaptor_type = AdaptorGeneral<nDimension, point_type, box_type, double>, typename geometry_type = double>
  class NTreeLinear
  {
    static_assert(0 < nDimension && nDimension < 64);

  protected:
    // Max number of children
    static autoce _nChild = pow_ce(2, nDimension);

  public:
    enum UpdateId { ERASE = std::numeric_limits<entity_id_type>::max() };
    static autoce is_linear_tree = nDimension < 15;
    
    // Max value: 2 ^ nDimension
    using child_id_type = uint64_t;

    // Max value: 2 ^ nDepth ^ nDimension * 2 (signal bit)
    using morton_grid_id_type = std::conditional<nDimension < 4
      , uint16_t
      , typename std::conditional<is_linear_tree
        , uint64_t
        , bitset_arithmetic<nDimension * 4 + 1>
      >::type
    >::type;
    
    using morton_node_id_type = morton_grid_id_type; // same as the morton_grid_id_type, but depth is signed by a sentinel bit.
    using morton_grid_id_type_cref = std::conditional<is_linear_tree, morton_node_id_type, morton_node_id_type const&>::type;
    using morton_node_id_type_cref = morton_grid_id_type_cref;
    using max_element_type = uint32_t;
    using depth_type = uint32_t;


  protected:

    // Type system determined maximal depth.
    static autoce _nDepthMaxTheoretical = depth_type((CHAR_BIT * sizeof(morton_node_id_type) - 1/*sentinal bit*/) / nDimension);

    using _Ad = adaptor_type;
    static_assert(AdaptorConcept<_Ad, point_type, box_type, geometry_type>);


  public:
    struct Node
    {
    public:
      static constexpr bool is_bitset = _nChild > 64;

    private:

      // Max value: 2^(2^nDimension)
      using child_exist_flag_type = std::conditional<_nChild <= 8
        , uint8_t
        , typename std::conditional<is_bitset
          , bitset_arithmetic<_nChild>
          , uint64_t
        >::type
      >::type;

      child_exist_flag_type _hasChildK = {};

      struct unused_container_type {};
      using child_container_type = std::conditional<is_bitset, vector<child_id_type>, unused_container_type>::type;
      child_container_type _children;

    public:
      vector<entity_id_type> vid;
      box_type box;

      constexpr void EnableChild(child_id_type iChild) 
      { 
        if constexpr (is_bitset)
        {
          if (!_hasChildK[iChild])
          {
            _hasChildK[iChild] = true;
            _children.push_back(iChild);
          }
        }
        else
          _hasChildK |= (child_exist_flag_type(1) << iChild);
      }

      constexpr void DisableChild(child_id_type iChild)
      {
        if constexpr (is_bitset)
        {
          _hasChildK[iChild] = false;
          std::erase_if(_children, [](autoc iChildContained) { return iChild = iChildContained; });
        }
        else
          _hasChildK &= ~(child_exist_flag_type(1) << iChild);
      }

      constexpr bool HasChild(child_id_type iChild) const 
      {
        if constexpr (is_bitset)
          return _hasChildK[iChild];
        else
          return _hasChildK & (child_exist_flag_type(1) << iChild);
      }

      inline bool IsAnyChildExist() const { return _isAnyChildExist(_hasChildK); }

      vector<child_id_type> GetChildren() const
      {
        if constexpr (is_bitset)
          return _children;
        else
          return _getChildren();
      }

    private:
      template<size_t N> 
      static inline bool _isAnyChildExist(bitset_arithmetic<N> const& hasChildK) { return !hasChildK.none(); }
      static constexpr bool _isAnyChildExist(size_t hasChildK) { return hasChildK > 0; }

      inline vector<child_id_type> _getChildren() const
      {
        if (!_hasChildK)
          return {};

        auto vChild = vector<child_id_type>();
        vChild.reserve(std::max<child_id_type>(4, _nChild / 8));

        auto mask = child_exist_flag_type{ 1 };
        for (child_id_type iChild = 0; iChild < _nChild; ++iChild, mask <<= 1)
          if (_hasChildK & mask)
            vChild.emplace_back(iChild);
        
        return vChild;
      }
    };


  protected: // member variables
    using container_type = std::conditional<is_linear_tree, unordered_map<morton_node_id_type, Node>, map<morton_node_id_type, Node, bitset_arithmetic_compare>>::type;
    container_type _nodes;
    box_type _box = {};
    depth_type _nDepthMax = 0;
    grid_id_type _nRasterResolutionMax = 0;
    max_element_type _nElementMax = 11;

    static_assert(sizeof(_nRasterResolutionMax) <= sizeof(grid_id_type));

  protected: // aid struct to partitioning
    struct _NodePartitioner
    {
      morton_node_id_type kNode = 0;
      Node* pNode = nullptr;
      depth_type nDepth;
      morton_node_id_type idLocationBegin;
      vector<entity_id_type>::iterator itBegin;
      vector<entity_id_type>::iterator itEnd;
    };


  protected: // Aid functions
    static void _reserveContainer(map<morton_node_id_type, Node, bitset_arithmetic_compare>& m, size_t n) {};
    static void _reserveContainer(unordered_map<morton_node_id_type, Node>& m, size_t n) { m.reserve(n); };
    template<size_t N>
    static inline size_t _mortonIdToChildId(bitset_arithmetic<N> const& bs)
    {
      assert(bs <= bitset_arithmetic<N>(std::numeric_limits<size_t>::max()));
      return bs.to_ullong();
    }
    static constexpr size_t _mortonIdToChildId(size_t morton) { return morton; }


    static inline vector<entity_id_type> _generatePointId(size_t n)
    {
      auto vidPoint = vector<entity_id_type>(n);
      std::iota(begin(vidPoint), end(vidPoint), 0);
      return vidPoint;
    }

    inline Node& _createChild(Node& nodeParent, child_id_type iChild, morton_node_id_type_cref kChild)
    {
      nodeParent.EnableChild(iChild);

      auto& nodeChild = _nodes[kChild];

      {
        auto flagChild = iChild;
        for (dim_type iDimension = 0; iDimension < nDimension; ++iDimension)
        {
          autoc fGreater = (flagChild & child_id_type{1});
          _Ad::point_comp(_Ad::box_min(nodeChild.box), iDimension) = fGreater ? (_Ad::point_comp_c(_Ad::box_max_c(nodeParent.box), iDimension) + _Ad::point_comp_c(_Ad::box_min_c(nodeParent.box), iDimension)) / geometry_type(2) : _Ad::point_comp_c(_Ad::box_min_c(nodeParent.box), iDimension);
          _Ad::point_comp(_Ad::box_max(nodeChild.box), iDimension) = fGreater ? _Ad::point_comp_c(_Ad::box_max_c(nodeParent.box), iDimension) : ((_Ad::point_comp_c(_Ad::box_max_c(nodeParent.box), iDimension) + _Ad::point_comp_c(_Ad::box_min_c(nodeParent.box), iDimension)) / geometry_type(2));
          flagChild >>= 1;
        }
      }
      return nodeChild;
    }

    bool _isEveryItemIdUnique() const
    {
      auto ids = vector<entity_id_type>();
      ids.reserve(100);
      std::ranges::for_each(_nodes, [&](auto& node)
      {
        ids.insert(end(ids), begin(node.second.vid), end(node.second.vid));
      });

      std::ranges::sort(ids);
      autoc itEndUnique = std::unique(begin(ids), end(ids));
      return itEndUnique == end(ids);
    }

    bool _insert(morton_node_id_type_cref kNode, morton_node_id_type_cref kNodeSmallest, entity_id_type id, bool fInsertToLeaf)
    {
      if (kNode == kNodeSmallest)
      {
        cont_at(this->_nodes, kNode).vid.emplace_back(id);
        assert(this->_isEveryItemIdUnique()); // Assert means: index is already added. Wrong input!
        return true;
      }

      if (fInsertToLeaf)
      {
        auto& nodeNew = this->_nodes[kNode];
        nodeNew.vid.emplace_back(id);
        nodeNew.box = this->CalculateExtent(kNode);

        // Create all child between the new (kNode) and the smallest existing one (kNodeSmallest)
        auto kNodeParent = kNode;
        do
        {
          autoc iChild = _getChildPartOfLocation(kNodeParent);
          kNodeParent = kNodeParent >>= nDimension;
          assert(IsValidKey(kNodeParent));
          auto& nodeParent = this->_nodes[kNodeParent];
          nodeParent.EnableChild(iChild);
          nodeParent.box = this->CalculateExtent(kNodeParent);
        } while (kNodeParent != kNodeSmallest);
      }
      else
      {
        autoc itNode = this->_nodes.find(kNodeSmallest);
        if (itNode->second.IsAnyChildExist())
        {
          autoc nDepth = this->GetDepth(kNodeSmallest);
          autoc kNodeChild = kNode << (nDimension * (this->_nDepthMax - nDepth - 1));
          autoc iChild = _getChildPartOfLocation(kNodeChild);
          auto& nodeChild = this->_createChild(itNode->second, iChild, kNodeChild);
          nodeChild.vid.emplace_back(id);
        }
        else
          itNode->second.vid.emplace_back(id);
      }

      assert(this->_isEveryItemIdUnique()); // Assert means: index is already added. Wrong input!
      return true;
    }


  public: // Static aid functions

    static constexpr size_t EstimateNodeNumber(size_t n, depth_type nDepthMax, max_element_type nElementMax)
    {
      assert(nElementMax > 0);
      assert(nDepthMax > 0);

      autoc nDepthEstimated = std::min(nDepthMax, static_cast<depth_type>(ceil((log2(static_cast<float>(n) / static_cast<float>(nElementMax)) + 1) / static_cast<float>(nDimension))));
      autoc nNodeEstimated = (pow_ce(_nChild, nDepthEstimated + 1) - 1) / (_nChild - 1);
      return static_cast<size_t>(1.5 * nNodeEstimated);
    }

    static inline morton_node_id_type GetHash(depth_type depth, morton_node_id_type_cref key)
    {
      assert(key < (morton_node_id_type(1) << (depth * nDimension)));
      return (morton_node_id_type(1) << (depth * nDimension)) | key;
    }

    static inline morton_node_id_type GetRootKey()
    { 
      return GetHash(0, 0);
    }

    static constexpr bool IsValidKey(uint64_t key) { return key; }

    template<size_t N>
    static inline bool IsValidKey(bitset_arithmetic<N> const& key) { return !key.none(); }

    static depth_type GetDepth(morton_node_id_type key)
    {
      // Keep shifting off three bits at a time, increasing depth counter
      for (depth_type d = 0; IsValidKey(key); ++d, key >>= nDimension)
        if (key == 1) // If only sentinel bit remains, exit with node depth
          return d;

      assert(false); // Bad key
      return 0;
    }

    static inline morton_node_id_type RemoveSentinelBit(morton_node_id_type_cref key, std::optional<depth_type> const& onDepth = std::nullopt)
    {
      autoc nDepth = onDepth.value_or(GetDepth(key));
      return key - (1 << nDepth);
    }


  private: // Morton aid functions

    static inline child_id_type _getChildPartOfLocation(morton_node_id_type_cref key)
    {
      autoc maskLastBits1 = (morton_node_id_type(1) << nDimension) - 1;

      return _mortonIdToChildId(key & maskLastBits1);
    }

    static constexpr morton_grid_id_type Part1By2(grid_id_type n)
    {
      // n = ----------------------9876543210 : Bits initially
      // n = ------98----------------76543210 : After (1)
      // n = ------98--------7654--------3210 : After (2)
      // n = ------98----76----54----32----10 : After (3)
      // n = ----9--8--7--6--5--4--3--2--1--0 : After (4)
      n = (n ^ (n << 16)) & 0xff0000ff; // (1)
      n = (n ^ (n << 8)) & 0x0300f00f; // (2)
      n = (n ^ (n << 4)) & 0x030c30c3; // (3)
      n = (n ^ (n << 2)) & 0x09249249; // (4)
      return std::is_same<morton_grid_id_type, bitset_arithmetic<nDimension>>::value ? morton_grid_id_type(n) : static_cast<morton_grid_id_type>(n);
    }

    // Separates low 16 bits of input by one bit
    static constexpr morton_grid_id_type Part1By1(grid_id_type n)
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
      return std::is_same<morton_grid_id_type, bitset_arithmetic<nDimension>>::value ? morton_grid_id_type(n) : static_cast<morton_grid_id_type>(n);
    }

  public:

    static constexpr morton_grid_id_type Morton(array<grid_id_type, 1> const& x)
    {
      return morton_grid_id_type(x[0]);
    }

    static constexpr morton_grid_id_type Morton(array<grid_id_type, 2> const& xy)
    {
      return (Part1By1(xy[1]) << 1) + Part1By1(xy[0]);
    }

    static constexpr morton_grid_id_type Morton(array<grid_id_type, 3> const& xyz)
    {
      // z--z--z--z--z--z--z--z--z--z-- : Part1By2(z) << 2
      // -y--y--y--y--y--y--y--y--y--y- : Part1By2(y) << 1
      // --x--x--x--x--x--x--x--x--x--x : Part1By2(x)
      // zyxzyxzyxzyxzyxzyxzyxzyxzyxzyx : Final result
      return (Part1By2(xyz[2]) << 2) + (Part1By2(xyz[1]) << 1) + Part1By2(xyz[0]);
    }

    template<dim_type nDimension>
    static morton_grid_id_type Morton(array<grid_id_type, nDimension> const& aidGrid)
    {
      static_assert(nDimension > 3);

      auto msb = aidGrid[0];
      for (dim_type iDimension = 1; iDimension < nDimension; ++iDimension)
        msb |= aidGrid[iDimension];

      morton_grid_id_type id = 0;
      grid_id_type mask = 1;
      for (dim_type i = 0, shift = 0; msb; mask <<= 1, msb >>= 1, ++i)
        for (dim_type iDimension = 0; iDimension < nDimension; ++iDimension, ++shift)
          if constexpr (Node::is_bitset)
            id[shift] = aidGrid[iDimension] & mask;
          else
            id |= (aidGrid[iDimension] & mask) << (shift - i);

      return id;
    }


  public: // Getters

    size_t GetNodeSize() const { return _nodes.size(); }
    auto const& GetBox() const { return _box; }
    auto const& Get() const { return _nodes; }
    auto const& Get(morton_node_id_type_cref key) const { return cont_at(_nodes, key); }
    auto GetDepthMax() const noexcept { return _nDepthMax; }
    auto GetResolutionMax() const noexcept { return _nRasterResolutionMax; }


  public: // Main service functions

    // Alternative creation mode (instead of Create), Init then Insert items into leafs one by one. NOT RECOMMENDED.
    void Init(box_type const& box, depth_type nDepthMax, max_element_type nElementMax = 11)
    {
      assert(this->_nodes.empty()); // To build/setup/create the tree, use the Create() [recommended] or Init() function. If an already builded tree is wanted to be reset, use the Clear() function before init.
      assert(nDepthMax > 1);
      assert(nDepthMax <= _nDepthMaxTheoretical);
      assert(nElementMax > 1);
      assert(CHAR_BIT * sizeof(grid_id_type) >= _nDepthMax);

      this->_box = box;
      this->_nDepthMax = nDepthMax;
      this->_nRasterResolutionMax = static_cast<grid_id_type>(pow_ce(2, nDepthMax));
      this->_nElementMax = nElementMax;

      auto& nodeRoot = this->_nodes[GetHash(0, 0)];
      nodeRoot.box = box;
    }


    using fnProcedure = std::function<void(morton_node_id_type_cref, Node const&)>;
    using fnProcedureUnconditional = std::function<void(morton_node_id_type_cref, Node const&, bool)>;
    using fnSelector = std::function<bool(morton_node_id_type_cref, Node const&)>;
    using fnSelectorUnconditional = std::function<bool(morton_node_id_type_cref, Node const&)>;


    // Visit nodes with special selection and procedure in breadth-first search order
    void VisitNodes(morton_node_id_type_cref kRoot, fnProcedure const& procedure, fnSelector const& selector) const
    {
      auto q = queue<morton_node_id_type>();
      for (q.push(kRoot); !q.empty(); q.pop())
      {
        autoc& key = q.front();
        autoc& node = cont_at(_nodes, key);
        procedure(key, node);

        autoc flagPrefix = key << nDimension;
        for (autoc iChild : node.GetChildren())
        {
          autoc kChild = flagPrefix | morton_node_id_type(iChild);
          if (selector(kChild, cont_at(_nodes, kChild)))
            q.push(kChild);
        }
      }
    }


    // Visit nodes with special selection and procedure in breadth-first search order
    inline void VisitNodes(morton_node_id_type_cref kRoot, fnProcedure const& procedure) const
    {
      VisitNodes(kRoot, procedure, [](morton_node_id_type_cref key, Node const& node) -> bool { return true; });
    }


    // Visit nodes with special selection and procedure and if unconditional selection is fulfilled descendants will not be test with selector
    void VisitNodes(morton_node_id_type_cref kRoot, fnProcedureUnconditional const& procedure, fnSelector const& selector, fnSelectorUnconditional const& selectorUnconditional) const
    {
      struct _search
      {
        morton_node_id_type key;
        Node const& pNode;
        depth_type nDepth;
        bool fUnconditional;
      };

      autoc nDepthRoot = GetDepth(kRoot);
      auto q = queue<_search>();
      for (q.push({ kRoot, cont_at(_nodes, kRoot), nDepthRoot, false }); !q.empty(); q.pop())
      {
        autoc& item = q.front();
        procedure(item.key, item.pNode, item.fUnconditional);

        autoc nDepthChild = depth_type(item.nDepth + 1);
        autoc flagPrefix = morton_node_id_type(item.key << nDimension);
        for (autoc iChild : item.pNode.GetChildren())
        {
          autoc kChild = morton_node_id_type(flagPrefix | iChild);
          autoc& pNodeChild = cont_at(_nodes, kChild);
          if (item.fUnconditional)
            q.push({ kChild, pNodeChild, nDepthChild, true });
          else if (selector(kChild, pNodeChild))
            q.push({ kChild, pNodeChild, nDepthChild, selectorUnconditional(kChild, pNodeChild) });
        }
      }
    }


    // Collect all item id, traversing the tree in breadth-first search order
    vector<entity_id_type> CollectAllIdInBFS() const
    {
      auto ids = vector<entity_id_type>();
      ids.reserve(GetNodeSize() * std::max<size_t>(2, _nElementMax / 2));

      VisitNodes(GetRootKey(), [&ids](morton_node_id_type_cref key, autoc& node)
      { 
        ids.insert(end(ids), begin(node.vid), end(node.vid));
      });
      return ids;
    }


    // Erase an id. Traverse all node if it is needed, which has major performance penalty.
    bool EraseId(entity_id_type id)
    {
      return std::ranges::any_of(_nodes, [&](auto& pairNode) { return erase(pairNode.second.vid, id); });
    }


    // Update all element which are in the given hash-table. Elements will be erased if the replacement id is std::numeric_limits<entity_id_type>::max().
    void UpdateIndexes(unordered_map<entity_id_type, entity_id_type> const& vIndexOldNew)
    {
      autoc itEnd = std::end(vIndexOldNew);
      std::ranges::for_each(_nodes, [&](auto& node)
      {
        auto vid = vector<entity_id_type>(node.second.vid.size());
        std::ranges::transform(node.second.vid, begin(vid), [&](autoc& id)
        {
          autoc it = vIndexOldNew.find(id);
          return it == itEnd ? id : it->second;
        });

        std::erase_if(vid, [](autoc id) { return id == UpdateId::ERASE; });
        node.second.vid.swap(vid);
      });

      assert(_isEveryItemIdUnique()); // Assert means: index replacements causes that multiple object has the same id. Wrong input!
    }


    // Calculate extent by box of the tree and the key of the node 
    box_type CalculateExtent(morton_node_id_type_cref keyNode) const
    {
      auto boxNode = box_type();
      auto& ptMinBoxNode = _Ad::box_min(boxNode);
      auto& ptMaxBoxNode = _Ad::box_max(boxNode);
      autoc& ptMinBoxRoot = _Ad::box_min_c(_box);
      autoc& ptMaxBoxRoot = _Ad::box_max_c(_box);

      ptMinBoxNode = ptMinBoxRoot;

      auto aSize = array<geometry_type, nDimension>();
      for (dim_type iDimension = 0; iDimension < nDimension; ++iDimension)
        aSize[iDimension] = _Ad::point_comp_c(ptMaxBoxRoot, iDimension) - _Ad::point_comp_c(ptMinBoxRoot, iDimension);

      autoc nDepth = GetDepth(keyNode);
      autoc nRasterResolution = pow_ce(2, nDepth);
      autoc rMax = 1.0 / static_cast<double>(nRasterResolution);

      autoce one = morton_grid_id_type(1);
      auto keyShifted = keyNode;// RemoveSentinelBit(key, nDepth);
      for (depth_type iDepth = 0; iDepth < nDepth; ++iDepth)
      {
        autoc r = rMax * (1 << iDepth);
        for (dim_type iDimension = 0; iDimension < nDimension; ++iDimension, keyShifted >>= 1)
          _Ad::point_comp(ptMinBoxNode, iDimension) += static_cast<geometry_type>((aSize[iDimension] * r)) * ((keyShifted & one) > morton_grid_id_type{});
      }

      for (dim_type iDimension = 0; iDimension < nDimension; ++iDimension)
        _Ad::point_comp(ptMaxBoxNode, iDimension) = _Ad::point_comp_c(ptMinBoxNode, iDimension) + static_cast<geometry_type>(aSize[iDimension] * rMax);

      return boxNode;
    }


    // Remove all elements and ids, except Root
    void Clear()
    {
      erase_if(_nodes, [](autoc& p) { return p.first != GetRootKey(); });
      cont_at(_nodes, GetRootKey()).vid.clear();
    }


    // Doubles the handled space relative to the root. iRootNew defines the relative location in the new space
    //!IMPLEMENT void Extend(morton_node_id_type_cref iRootNew = 0) {}
  };


  // NTreePoint: Non-owning container which spatially organize point ids in N dimension space into a hash-table by Morton Z order.
  template<dim_type nDimension, typename point_type, typename box_type, typename adaptor_type = AdaptorGeneral<nDimension, point_type, box_type, double>, typename geometry_type = double>
  class NTreePoint : public NTreeLinear<nDimension, point_type, box_type, adaptor_type, geometry_type>
  {
  private:
    using base = NTreeLinear<nDimension, point_type, box_type, adaptor_type, geometry_type>;
    using base::_NodePartitioner;
    using base::_Ad;

  public:
    using base::depth_type;
    using base::morton_grid_id_type;
    using base::morton_grid_id_type_cref;
    using base::morton_node_id_type;
    using base::morton_node_id_type_cref;
    using base::max_element_type;
    using base::child_id_type;

    using base::Node;

  private: // Aid functions
    static constexpr max_element_type max_element_default = 11;


    static inline vector<morton_grid_id_type> _getLocationId(vector<array<grid_id_type, nDimension>> const& vidGrid)
    {
      auto vidLocation = vector<morton_grid_id_type>(vidGrid.size());
      std::ranges::transform(vidGrid, begin(vidLocation), [](autoc& aGrid) { return base::Morton(aGrid); });
      return vidLocation;
    }

    void _addNode(_NodePartitioner& ns, child_id_type iChild, morton_grid_id_type_cref nLocationStep, vector<entity_id_type>::iterator const& itBegin, vector<entity_id_type>::iterator const& itEnd, queue<_NodePartitioner>& q)
    {
      if (itBegin == itEnd)
        return;

      autoc kChild = morton_grid_id_type((ns.kNode << nDimension) | morton_grid_id_type(iChild));
      auto& nodeChild = this->_createChild(*ns.pNode, iChild, kChild);

      autoc nElement = distance(itBegin, itEnd);
      if (nElement < base::_nElementMax || ns.nDepth == this->_nDepthMax)
        nodeChild.vid.insert(end(nodeChild.vid), itBegin, itEnd);
      else
      {
        autoc idLocation = morton_grid_id_type(ns.idLocationBegin + iChild * nLocationStep);
        q.emplace(_NodePartitioner{ kChild, &nodeChild, depth_type(ns.nDepth + 1), idLocation, itBegin, itEnd });
      }
    }


  public: // Create

    // Ctors
    NTreePoint() = default;
    NTreePoint(span<point_type const> const& vpt, depth_type nDepthMax, std::optional<box_type> const& oextent = std::nullopt, max_element_type nElementMaxInNode = max_element_default)
    {
      *this = Create(vpt, nDepthMax, oextent, nElementMaxInNode);
    }


    // Create
    static NTreePoint Create(span<point_type const> const& vpt, depth_type nDepthMax, std::optional<box_type> const& oextent = std::nullopt, max_element_type nElementMaxInNode = max_element_default)
    {
      autoc n = vpt.size();

      auto tree = NTreePoint{};
      tree.Init(oextent.value_or(_Ad::box_of_points(vpt)), nDepthMax, nElementMaxInNode);
      base::_reserveContainer(tree._nodes, base::EstimateNodeNumber(n, nDepthMax, nElementMaxInNode));
      if (vpt.empty())
        return tree;

      autoc kRoot = base::GetHash(0, 0);
      auto& nodeRoot = cont_at(tree._nodes, kRoot);

      autoc idLocationMax = morton_grid_id_type(1) << (nDimension * nDepthMax);
      autoc aidGrid = resolve_grid_id<nDimension, point_type, _Ad>(vpt, _Ad::box_min_c(tree._box), _Ad::box_max_c(tree._box), tree._nRasterResolutionMax);
      autoc aidLocation = _getLocationId(aidGrid);
      
      auto vidPoint = base::_generatePointId(n);
      auto q = std::queue<_NodePartitioner>();
      auto nsRoot = _NodePartitioner{ kRoot, &nodeRoot, depth_type(1), morton_node_id_type(0), std::begin(vidPoint), std::end(vidPoint) };
      for (q.push(nsRoot); !q.empty(); q.pop())
      {
        auto& ns = q.front();
        autoc nPart = morton_grid_id_type(1) << (nDimension * ns.nDepth);
        autoc nLocationStep = idLocationMax / nPart;

        autoc nElem = distance(ns.itBegin, ns.itEnd);
        if (nElem < base::_nChild)
        {
          sort(ns.itBegin, ns.itEnd, [&](autoc idPointL, autoc idPointR) { return aidLocation[idPointL] < aidLocation[idPointR]; });
          auto itEndPrev = ns.itBegin;
          auto iChild = base::_mortonIdToChildId((aidLocation[*itEndPrev] - ns.idLocationBegin) / nLocationStep);
          for (auto itSplit = itEndPrev + 1; itSplit != ns.itEnd && iChild < base::_nChild - 1; ++itSplit)
          {
            autoc iChildActual = base::_mortonIdToChildId((aidLocation[*itSplit] - ns.idLocationBegin) / nLocationStep);
            if (iChild == iChildActual)
              continue;
            
            tree._addNode(ns, iChild, nLocationStep, itEndPrev, itSplit, q);
            itEndPrev = itSplit;
            iChild = iChildActual;
          }

          tree._addNode(ns, iChild, nLocationStep, itEndPrev, ns.itEnd, q);
        }
        else
        {
          auto itEndPrev = ns.itBegin;
          for (child_id_type iChild = 1; iChild < base::_nChild; ++iChild)
          {
            autoc idLocationSplit = ns.idLocationBegin + iChild * nLocationStep;
            auto itSplit = std::partition(itEndPrev, ns.itEnd, [&](autoc idPoint) { return aidLocation[idPoint] < idLocationSplit; });
            tree._addNode(ns, iChild - 1, nLocationStep, itEndPrev, itSplit, q);
            itEndPrev = itSplit;
          }

          tree._addNode(ns, base::_nChild - 1, nLocationStep, itEndPrev, ns.itEnd, q);
        }
      }

      return tree;
    }


  public: // Edit functions


    // Insert item into a node. If fInsertToLeaf is true: The smallest node will be chosen by the max depth. If fInsertToLeaf is false: The smallest existing level on the branch will be chosen.
    bool Insert(entity_id_type id, point_type const& pt, bool fInsertToLeaf = false)
    {
      if (!_Ad::does_box_contain_point(this->_box, pt))
        return false;

      autoc kNodeSmallest = FindSmallestNode(pt);
      if (!base::IsValidKey(kNodeSmallest))
        return false;

      autoc aGrid = resolve_grid_id<nDimension, point_type, _Ad>(vector{ pt }, _Ad::box_min_c(this->_box), _Ad::box_max_c(this->_box), this->GetResolutionMax())[0];
      autoc idLocation = base::Morton(aGrid);
      autoc kNode = this->GetHash(this->_nDepthMax, idLocation);

      return this->_insert(kNode, kNodeSmallest, id, fInsertToLeaf);
    }


    // Erase id, aided with the original point
    bool Erase(entity_id_type id, point_type const& pt)
    {
      autoc kOld = FindSmallestNode(pt);
      if (!kOld)
        return false; // old box is not in the handled space domain

      auto& vid = cont_at(this->_nodes, kOld).vid;
      autoc itRemove = std::remove(begin(vid), end(vid), id);
      if (itRemove == end(vid))
        return false; // id was not registered previously.

      vid.erase(itRemove, vid.end());
      return true;
    }


    // Update id by the new point information
    bool Update(entity_id_type id, point_type const& ptNew)
    {
      if (!_Ad::does_box_contain_point(this->_box, ptNew))
        return false;

      if (!this->EraseId(id))
        return false;

      return this->Insert(id, ptNew);
    }


    // Update id by the new point information and the erase part is aided by the old point geometry data
    void Update(entity_id_type id, point_type const& ptOld, point_type const& ptNew)
    {
      if (!_Ad::does_box_contain_point(this->_box, ptNew))
        return false;

      if (!this->EraseId(id, ptOld))
        return false;

      return this->Insert(id, ptNew);
    }


  public: // Search functions

    // Find smallest node which contains the box
    morton_node_id_type FindSmallestNode(point_type const& pt) const
    {
      if (!_Ad::does_box_contain_point(this->_box, pt))
        return morton_node_id_type{};

      autoc aGrid = resolve_grid_id<nDimension, point_type, _Ad>(vector{ pt }, _Ad::box_min_c(this->_box), _Ad::box_max_c(this->_box), this->_nRasterResolutionMax)[0];
      autoc idLocation = base::Morton(aGrid);

      for (auto kSmallestNode = this->GetHash(this->_nDepthMax, idLocation); base::IsValidKey(kSmallestNode); kSmallestNode >>= nDimension)
        if (this->_nodes.contains(kSmallestNode))
          return kSmallestNode;

      return morton_node_id_type{}; // Not found
    }
 
    bool Contains(point_type const& pt, span<point_type const> const& vpt, geometry_type rAccuracy) const
    {
      autoc aGrid = resolve_grid_id<nDimension, point_type, _Ad>(vector{ pt }, _Ad::box_min_c(this->_box), _Ad::box_max_c(this->_box), this->_nRasterResolutionMax)[0];
      autoc idLocation = Morton(aGrid);
      autoc kSmallestNode = this->GetHash(this->_nDepthMax, idLocation);
      if (!this->_nodes.contains(kSmallestNode))
        return false;

      autoc& node = cont_at(this->_nodes, kSmallestNode);
      return std::ranges::any_of(node.vid, [&](autoc& id) { return _Ad::are_points_equal(pt, vpt[id], rAccuracy); });
    }

    // Range search
    vector<entity_id_type> RangeSearch(box_type const& range, span<point_type const> const& vpt) const
    {
      // If the range has zero volume, it could stuck at any node comparison with point/side touch. It is eliminated to work node bounding box independently.
      for (dim_type iDimension = 0; iDimension < nDimension; ++iDimension)
        if (_Ad::point_comp_c(_Ad::box_min_c(range), iDimension) == _Ad::point_comp_c(_Ad::box_max_c(range), iDimension))
          return {};

      auto vidFound = vector<entity_id_type>();
      this->VisitNodes(base::GetHash(0, 0)
        , [&](morton_node_id_type_cref, Node const& pNode, bool fUnconditional)
        {
          if (fUnconditional)
            vidFound.insert(end(vidFound), begin(pNode.vid), end(pNode.vid));
          else
            std::ranges::copy_if(pNode.vid, back_inserter(vidFound), [&](autoc id) { return _Ad::does_box_contain_point(range, vpt[id]); });
        }
        , [&](morton_node_id_type_cref, autoc& pNode)
          {
            return _Ad::are_boxes_overlapped(range, pNode.box, false);
          }
        , [&](morton_node_id_type_cref, autoc& pNode)
          {
            return _Ad::are_boxes_overlapped(range, pNode.box, true);
          }
      );

      return vidFound;
    }


  private: // K Nearest Neighbor helpers

    static geometry_type _getBoxWallDistanceMax(point_type const& pt, box_type const& box)
    {
      autoc& ptMin = _Ad::box_min_c(box);
      autoc& ptMax = _Ad::box_max_c(box);

      auto vDist = vector<geometry_type>();
      vDist.reserve(nDimension);
      for (dim_type iDim = 0; iDim < nDimension; ++iDim)
      {
        autoc rDistActual = vDist.emplace_back(std::min(
          abs(_Ad::point_comp_c(pt, iDim) - _Ad::point_comp_c(ptMin, iDim)),
          abs(_Ad::point_comp_c(pt, iDim) - _Ad::point_comp_c(ptMax, iDim))
        ));

        if (rDistActual == 0)
          return 0.0;
      }

      return *std::min_element(begin(vDist), end(vDist));
    }


    struct _ItemDistance
    {
      geometry_type distance;
      auto operator <=> (_ItemDistance const& rhs) const = default;
    };

    struct _EntityDistance : _ItemDistance { entity_id_type id; };
    struct _BoxDistance : _ItemDistance { Node const& node; };

    static void _createEntityDistance(Node const& node, point_type const& pt, span<point_type const> const& vpt, multiset<_EntityDistance>& setEntity)
    {
      for (autoc id : node.vid)
        setEntity.insert({ _Ad::distance(pt, vpt[id]), id });
    }

    static geometry_type _getFarestDistance(multiset<_EntityDistance>& setEntity, size_t k)
    {
      if (setEntity.size() < k)
        return std::numeric_limits<geometry_type>::infinity();

      return next(begin(setEntity), k - 1)->distance;
    }

    static vector<entity_id_type> _convertEntityDistanceToList(multiset<_EntityDistance>& setEntity, size_t k)
    {
      autoc nEntity = std::min(k, setEntity.size());
      auto vidEntity = vector<entity_id_type>(nEntity);
      transform(begin(setEntity), next(begin(setEntity), nEntity), begin(vidEntity), [](autoc& ed) { return ed.id; });
      return vidEntity;
    }

   public:

    // K Nearest Neighbor
    vector<entity_id_type> GetNearestNeighbors(point_type const& pt, size_t k, span<point_type const> const& vpt) const
    {
      auto setEntity = multiset<_EntityDistance>();
      autoc kSmallestNode = FindSmallestNode(pt);
      if (base::IsValidKey(kSmallestNode))
      {
        autoc& nodeSmallest = cont_at(this->_nodes, kSmallestNode);
        _createEntityDistance(nodeSmallest, pt, vpt, setEntity);
        if (!nodeSmallest.IsAnyChildExist())
          if (_getFarestDistance(setEntity, k) < _getBoxWallDistanceMax(pt, nodeSmallest.box))
            return _convertEntityDistanceToList(setEntity, k);
      }

      auto setNodeDist = multiset<_BoxDistance>();
      std::ranges::for_each(this->_nodes, [&](autoc& pairKeyNode)
      {
        autoc& [key, node] = pairKeyNode;
        if (node.vid.empty() || key == kSmallestNode)
          return;

        autoc& ptMin = _Ad::box_min_c(node.box);
        autoc& ptMax = _Ad::box_max_c(node.box);

        auto aDist = point_type{};
        for (dim_type iDim = 0; iDim < nDimension; ++iDim)
        {
          autoc dMin = _Ad::point_comp_c(ptMin, iDim) - _Ad::point_comp_c(pt, iDim);
          autoc dMax = _Ad::point_comp_c(ptMax, iDim) - _Ad::point_comp_c(pt, iDim);

          // If pt projection in iDim is within min and max the wall distance should be calculated.
          _Ad::point_comp(aDist, iDim) = dMin * dMax < 0 ? 0 : std::min(abs(dMin), abs(dMax));
        }
        setNodeDist.insert({ _Ad::size(aDist), node });
      });

      auto rLatestNodeDist = begin(setNodeDist)->distance;
      for (autoc& nodeDist : setNodeDist)
      {
        autoc n = setEntity.size();
        if (k <= n && rLatestNodeDist < nodeDist.distance)
          break;

        _createEntityDistance(nodeDist.node, pt, vpt, setEntity);
        rLatestNodeDist = nodeDist.distance;
      }

      return _convertEntityDistanceToList(setEntity, k);
    }
  };


  // NTreeBoundingBox: Non-owning container which spatially organize bounding box ids in N dimension space into a hash-table by Morton Z order.
  template<dim_type nDimension, typename point_type, typename box_type, typename adaptor_type = AdaptorGeneral<nDimension, point_type, box_type, double>, typename geometry_type = double>
  class NTreeBoundingBox : public NTreeLinear<nDimension, point_type, box_type, adaptor_type, geometry_type>
  {
  private:
    using base = NTreeLinear<nDimension, point_type, box_type, adaptor_type, geometry_type>;
    using base::_NodePartitioner;
    using base::_Ad;

  public:
    using base::depth_type;
    using base::morton_grid_id_type;
    using base::morton_grid_id_type_cref;
    using base::morton_node_id_type;
    using base::morton_node_id_type_cref;
    using base::max_element_type;
    using base::child_id_type;

    using base::Node;

  private: // Aid functions
    
    static constexpr max_element_type max_element_default = 11;


    static inline vector<array<morton_grid_id_type, 2>> _getLocationId(vector<array<array<grid_id_type, nDimension>, 2>> const& vidGrid)
    {
      auto vidLocation = vector<array<morton_grid_id_type, 2>>(vidGrid.size());
      std::ranges::transform(vidGrid, begin(vidLocation), [](autoc& aGrid)->array<morton_grid_id_type, 2> { return { base::Morton(aGrid[0]), base::Morton(aGrid[1]) }; });
      return vidLocation;
    }


    void _addNode(_NodePartitioner& ns, child_id_type iChild, morton_grid_id_type_cref nLocationStep, vector<entity_id_type>::iterator itBegin, vector<entity_id_type>::iterator itIntersected, vector<entity_id_type>::iterator itEnd, queue<_NodePartitioner>& q)
    {
      if (itBegin == itEnd)
        return;

      ns.pNode->vid.insert(end(ns.pNode->vid), itBegin, itIntersected);
      autoc nElement = distance(itIntersected, itEnd);
      if (nElement == 0)
        return;

      ns.pNode->EnableChild(iChild);
      morton_node_id_type const kChild = (ns.kNode << nDimension) | morton_node_id_type(iChild);
      auto& nodeChild = this->_createChild(*ns.pNode, iChild, kChild);

      if (nElement < base::_nElementMax || ns.nDepth == this->_nDepthMax)
        nodeChild.vid.insert(end(nodeChild.vid), itIntersected, itEnd);
      else
      {
        autoc idLocation = morton_grid_id_type(ns.idLocationBegin + iChild * nLocationStep);
        q.emplace(_NodePartitioner{ kChild, &nodeChild, ns.nDepth + 1, idLocation, itIntersected, itEnd });
      }
    }


    void _rangeSearchRec(vector<entity_id_type>& vidFound, morton_node_id_type_cref kCurrent, Node const& pNode, depth_type nDepth, box_type const& range, span<box_type const> const& vExtent, bool fFullyContained, bool fUnconditional) const
    {
      ++nDepth;
      morton_node_id_type const flagPrefix = kCurrent << nDimension;

      if (fUnconditional)
      {
        vidFound.insert(end(vidFound), begin(pNode.vid), end(pNode.vid));

        for (autoc iChild : pNode.GetChildren())
        {
          autoc kChild = flagPrefix | morton_node_id_type(iChild);
          _rangeSearchRec(vidFound, kChild, cont_at(this->_nodes, kChild), nDepth, range, vExtent, fFullyContained, true);
        }
      }
      else
      {
        std::ranges::copy_if(pNode.vid, std::back_inserter(vidFound), [&](autoc id) { return _Ad::are_boxes_overlapped(range, vExtent[id], fFullyContained); });
        for (autoc iChild : pNode.GetChildren())
        {
          autoc kChild = flagPrefix | morton_node_id_type(iChild);
          autoc& pNodeChild = cont_at(this->_nodes, kChild);
          if (_Ad::are_boxes_overlapped(range, pNodeChild.box, false))
            _rangeSearchRec(vidFound, kChild, pNodeChild, nDepth, range, vExtent, fFullyContained, _Ad::are_boxes_overlapped(range, pNodeChild.box, true));
        }
      }
    }


  public: // Create
    
    // Ctors
    NTreeBoundingBox() = default;
    NTreeBoundingBox(span<box_type const> const& vExtent, depth_type nDepthMax, std::optional<box_type> const& oextent = std::nullopt, max_element_type nElementMaxInNode = max_element_default)
    {
      *this = Create(vExtent, nDepthMax, oextent, nElementMaxInNode);
    }


    // Create
    static NTreeBoundingBox Create(span<box_type const> const& vExtent, depth_type nDepthMax, std::optional<box_type> const& oextent = std::nullopt, max_element_type nElementMaxInNode = max_element_default)
    {
      auto tree = NTreeBoundingBox{};
      tree.Init(oextent.value_or(_Ad::box_of_boxes(vExtent))
        , nDepthMax
        , nElementMaxInNode
      );
     
      autoc n = vExtent.size();

      base::_reserveContainer(tree._nodes, base::EstimateNodeNumber(n, nDepthMax, nElementMaxInNode));
      if (n == 0)
        return tree;

      autoc kRoot = base::GetHash(0, 0);
      auto& nodeRoot = cont_at(tree._nodes, kRoot);

      autoc idLocationMax = morton_grid_id_type(1) << (tree._nDepthMax * nDimension);

      autoc aidGrid = resolve_grid_id<nDimension, box_type, point_type, _Ad>(vExtent, _Ad::box_min(tree._box), _Ad::box_max(tree._box), tree.GetResolutionMax());
      autoc aidLocation = _getLocationId(aidGrid);

      auto vidPoint = base::_generatePointId(n);
      auto q = std::queue<_NodePartitioner>();
      auto nsRoot = _NodePartitioner{ kRoot, &nodeRoot, 1, morton_node_id_type(0), std::begin(vidPoint), std::end(vidPoint) };
      for (q.push(nsRoot); !q.empty(); q.pop())
      {
        auto& ns = q.front();
        autoc nPart = morton_grid_id_type(1) << (ns.nDepth * nDimension);
        autoc nLocationStep = morton_grid_id_type(idLocationMax / nPart);

        autoc nElem = distance(ns.itBegin, ns.itEnd);
        if (nElem < base::_nChild)
        {
          auto aidLocationActualResolution = unordered_map<entity_id_type, array<child_id_type, 2>>();
          for_each(ns.itBegin, ns.itEnd, [&](autoc idExt)
          {
            aidLocationActualResolution.emplace(idExt,
              array
              { 
                base::_mortonIdToChildId((aidLocation[idExt][0] - ns.idLocationBegin) / nLocationStep), 
                base::_mortonIdToChildId((aidLocation[idExt][1] - ns.idLocationBegin) / nLocationStep) 
              }
            );
          });

          auto itSplitIntersected = std::partition(ns.itBegin, ns.itEnd, [&](autoc idExt)
          { 
            autoc& aid = cont_at(aidLocationActualResolution, idExt);
            return aid[0] != aid[1];
          });

          tree._addNode(ns, 0, nLocationStep, ns.itBegin, itSplitIntersected, itSplitIntersected, q);

          if (itSplitIntersected != ns.itEnd)
          {
            sort(itSplitIntersected, ns.itEnd, [&](autoc idPointL, autoc idPointR)
            { 
              return cont_at(aidLocationActualResolution, idPointL)[0] < cont_at(aidLocationActualResolution, idPointR)[0];
            });

            auto itEndPrev = itSplitIntersected;
            auto iChild = cont_at(aidLocationActualResolution, *itEndPrev)[0];
            for (auto itSplit = itEndPrev + 1; itSplit != ns.itEnd && iChild < base::_nChild - 1; ++itSplit)
            {
              autoc iChildActual = cont_at(aidLocationActualResolution, *itSplit)[0];
              if (iChild == iChildActual)
                continue;

              tree._addNode(ns, iChild, nLocationStep, itEndPrev, itEndPrev, itSplit, q);
              itEndPrev = itSplit;
              iChild = iChildActual;
            }

            tree._addNode(ns, iChild, nLocationStep, itEndPrev, itEndPrev, ns.itEnd, q);
          }
        }
        else
        {

          auto itEndPrev = ns.itBegin;
          for (child_id_type iChild = 1; iChild < base::_nChild; ++iChild)
          {
            autoc idLocationSplit = ns.idLocationBegin + iChild * nLocationStep;
            auto itSplitIntersected = std::partition(itEndPrev, ns.itEnd, [&](autoc idExt) { return aidLocation[idExt][0] < idLocationSplit && idLocationSplit <= aidLocation[idExt][1]; });
            auto itSplit = std::partition(itSplitIntersected, ns.itEnd, [&](autoc idExt) { return aidLocation[idExt][0] < idLocationSplit; });
            tree._addNode(ns, iChild - 1, nLocationStep, itEndPrev, itSplitIntersected, itSplit, q);
            itEndPrev = itSplit;
          }

          // Last item
          tree._addNode(ns, base::_nChild - 1, nLocationStep, itEndPrev, itEndPrev, ns.itEnd, q);
        }
      }

      return tree;
    }


  public: // Edit functions

    // Find smallest node which contains the box by grid id description
    morton_node_id_type FindSmallestNode(array<array<grid_id_type, nDimension>, 2> const& aidGrid) const
    {
      auto idLocationMin = base::Morton(aidGrid[0]);
      auto idLocationMax = base::Morton(aidGrid[1]);

      auto nDepth = this->_nDepthMax;
      for (auto flagDiffOfLocation = idLocationMin ^ idLocationMax; base::IsValidKey(flagDiffOfLocation); flagDiffOfLocation >>= nDimension, --nDepth)
        idLocationMin >>= nDimension;

      autoc itEnd = std::end(this->_nodes);
      for (auto kSmallestNode = this->GetHash(nDepth, idLocationMin); base::IsValidKey(kSmallestNode); kSmallestNode >>= nDimension)
        if (this->_nodes.find(kSmallestNode) != itEnd)
          return kSmallestNode;

      return morton_node_id_type{}; // Not found
    }

    // Find smallest node which contains the box
    morton_node_id_type FindSmallestNode(box_type const& box) const
    {
      if (!_Ad::are_boxes_overlapped(this->_box, box))
        return morton_node_id_type{};

      autoc aidGrid = resolve_grid_id<nDimension, box_type, point_type, _Ad>(vector{ box }, _Ad::box_min_c(this->_box), _Ad::box_max_c(this->_box), this->GetResolutionMax())[0];
      return FindSmallestNode(aidGrid);
    }


    // Insert item into a node. If fInsertToLeaf is true: The smallest node will be chosen by the max depth. If fInsertToLeaf is false: The smallest existing level on the branch will be chosen.
    bool Insert(entity_id_type id, box_type const& box, bool fInsertToLeaf = false)
    {
      if (!_Ad::are_boxes_overlapped(this->_box, box))
        return false;

      autoc kNodeSmallest = FindSmallestNode(box);
      if (!base::IsValidKey(kNodeSmallest))
        return false; // new box is not in the handled space domain

      autoc aGrid = resolve_grid_id<nDimension, box_type, point_type, _Ad>(vector{ box }, _Ad::box_min_c(this->_box), _Ad::box_max_c(this->_box), this->GetResolutionMax())[0];
      autoc idLocationMin = base::Morton(aGrid[0]);
      autoc idLocationMax = base::Morton(aGrid[1]);

      auto nDepthCommon = 0;
      for (auto idLocationDiff = idLocationMin ^ idLocationMax; base::IsValidKey(idLocationDiff); idLocationDiff >>= nDimension)
        ++nDepthCommon;

      autoc nDepth = this->_nDepthMax - nDepthCommon;
      autoc idLocation = (idLocationMin & idLocationMax) >> nDepthCommon;
      
      autoc kNode = this->GetHash(nDepth, idLocation);
      return this->_insert(kNode, kNodeSmallest, id, fInsertToLeaf);
    }


    // Erase id, aided with the original bounding box
    bool Erase(entity_id_type id, box_type const& ext)
    {
      autoc kOld = FindSmallestNode(ext);
      if (!kOld)
        return false; // old box is not in the handled space domain

      auto& vid = cont_at(this->_nodes, kOld).vid;
      autoc itRemove = std::remove(begin(vid), end(vid), id);
      if (itRemove == end(vid))
        return false; // id was not registered previously.

      vid.erase(itRemove, vid.end());
      return true;
    }


    // Update id by the new bounding box information
    bool Update(entity_id_type id, box_type const& extNew)
    {
      if (!_Ad::are_boxes_overlapped(this->_box, extNew))
        return false;

      if (!this->EraseId(id))
        return false;

      return this->Insert(id, extNew);
    }


    // Update id by the new point information and the erase part is aided by the old bounding box geometry data
    bool Update(entity_id_type id, box_type const& extOld, box_type const& extNew)
    {
      if (!_Ad::are_boxes_overlapped(this->_box, extNew))
        return false;

      if (!this->Erase(id, extOld))
        return false; // id was not registered previously.

      return this->Insert(id, extNew);
    }


  public: // Search functions
    
    // Pick search
    vector<entity_id_type> PickSearch(point_type const& ptPick, span<box_type const> const& vExtent) const
    {
      autoc aGrid = resolve_grid_id<nDimension, point_type, _Ad>(vector{ ptPick }, _Ad::box_min_c(this->_box), _Ad::box_max_c(this->_box), this->GetResolutionMax())[0];
      autoc idLocation = base::Morton(aGrid);

      auto vidFound = vector<entity_id_type>();
      vidFound.reserve(100);
      autoc itEnd = std::end(this->_nodes);
      for (auto kNode = this->GetHash(this->_nDepthMax, idLocation); base::IsValidKey(kNode); kNode >>= nDimension)
      {
        autoc itNode = this->_nodes.find(kNode);
        if (itNode == itEnd)
          continue;

        std::ranges::copy_if(itNode->second.vid, back_inserter(vidFound), [&](autoc id) { return _Ad::does_box_contain_point(vExtent[id], ptPick); });
      }

      return vidFound;
    }


    // Range search
    vector<entity_id_type> RangeSearch(box_type const& range, span<box_type const> const& vExtent, bool fFullyContained = true) const
    {
      // If the range has zero volume, it could stuck at any node comparison with point/side touch. It is eliminated to work node bounding box independently.
      for (dim_type iDimension = 0; iDimension < nDimension; ++iDimension)
        if (_Ad::point_comp_c(_Ad::box_min_c(range), iDimension) == _Ad::point_comp_c(_Ad::box_max_c(range), iDimension))
          return {};

      auto vidFound = vector<entity_id_type>();
      vidFound.reserve(100);
      autoc kRoot = base::GetHash(0, 0);
      _rangeSearchRec(vidFound, kRoot, cont_at(this->_nodes, kRoot), 0, range, vExtent, fFullyContained, false);

      return vidFound;
    }


    // Collision detection: Returns all overlapping boxes from the source trees.
    static vector<std::pair<entity_id_type, entity_id_type>> CollisionDetection(NTreeBoundingBox const& tL, span<box_type const> const& vExtentL, NTreeBoundingBox const& tR, span<box_type const> const& vExtentR)
    {
      using NodeIterator = decltype(tL._nodes)::const_iterator;
      struct NodeIteratorAndStatus { NodeIterator it; bool fTraversed; };
      using ParentIteratorArray = array<NodeIteratorAndStatus, 2>;

      enum : bool { Left, Right };

      auto vResult = vector<std::pair<entity_id_type, entity_id_type>>{};
      vResult.reserve(vExtentL.size() / 10);

      autoc kRoot = base::GetRootKey();
      autoc aTree = array{ &tL, &tR };

      auto q = queue<ParentIteratorArray>{};
      for (q.push({ NodeIteratorAndStatus{ tL._nodes.find(kRoot), false }, NodeIteratorAndStatus{ tR._nodes.find(kRoot), false } }); !q.empty(); q.pop())
      {
        autoc& aitNodeParent = q.front();

        // Check the current ascendant content 
        {
          for (autoc idL : aitNodeParent[Left].it->second.vid)
            for (autoc idR : aitNodeParent[Right].it->second.vid)
              if (_Ad::are_boxes_overlapped(vExtentL[idL], vExtentR[idR], false))
                vResult.emplace_back(idL, idR);
        }

        // Collect children
        auto avitChildNode = array<vector<NodeIteratorAndStatus>, 2>{};
        for (autoc id : { Left, Right })
        {
          autoc& [itNode, fTraversed] = aitNodeParent[id];
          if (fTraversed)
            continue;
          
          autoc flagPrefix = itNode->first << nDimension;
          autoc vidChild = itNode->second.GetChildren();
          avitChildNode[id].resize(vidChild.size());
          std::ranges::transform(vidChild, begin(avitChildNode[id]), [&](autoc iChild) -> NodeIteratorAndStatus
          {
            return { aTree[id]->_nodes.find(flagPrefix | morton_node_id_type(iChild)), false };
          });
        }

        // Stop condition
        if (avitChildNode[0].empty() && avitChildNode[1].empty())
          continue;

        // Add parent if it has any element
        for (autoc id : { Left, Right })
          if (!aitNodeParent[id].it->second.vid.empty())
            avitChildNode[id].push_back({ aitNodeParent[id].it, true });

        // Cartesian product of avitChildNode left and right
        for (autoc& itNodeChildL : avitChildNode[Left])
          for (autoc& itNodeChildR : avitChildNode[Right])
            if (!(itNodeChildL.it == aitNodeParent[Left].it && itNodeChildR.it == aitNodeParent[Right].it))
              if (_Ad::are_boxes_overlapped(itNodeChildL.it->second.box, itNodeChildR.it->second.box, false))
                q.emplace(array{ itNodeChildL, itNodeChildR });
      }
      return vResult;
    }


    // Collision detection: Returns all overlapping boxes from the source trees.
    vector<std::pair<entity_id_type, entity_id_type>> CollisionDetection(span<box_type const> const& vExtentL, NTreeBoundingBox const& tR, span<box_type const> const& vExtentR) const
    {
      return CollisionDetection(*this, vExtentL, tR, vExtentR);
    }
  };


  // Aliases
  using Point1D = NTree::Point<1>;
  using Point2D = NTree::Point<2>;
  using Point3D = NTree::Point<3>;
  using BoundingBox1D = NTree::BoundingBox<1>;
  using BoundingBox2D = NTree::BoundingBox<2>;
  using BoundingBox3D = NTree::BoundingBox<3>;

  template<size_t nDimension> using TreePointXD = NTree::NTreePoint<nDimension, NTree::Point<nDimension>, NTree::BoundingBox<nDimension>>;
  template<size_t nDimension> using TreeBoxXD = NTree::NTreeBoundingBox<nDimension, NTree::Point<nDimension>, NTree::BoundingBox<nDimension>>;

  // Dualtree for points
  using DualtreePoint = TreePointXD<1>;

  // Dualtree for bounding boxes
  using DualtreeBox = TreeBoxXD<1>;

  // Quadtree for points
  using QuadtreePoint = TreePointXD<2>;

  // Quadtree for bounding boxes
  using QuadtreeBox = TreeBoxXD<2>;

  // Octree for points
  using OctreePoint = TreePointXD<3>;

  // Octree for bounding boxes
  using OctreeBox = TreeBoxXD<3>;

  // Hexatree for points
  using HexatreePoint = TreePointXD<4>;

  // Hexatree for bounding boxes
  using HexatreeBox = TreeBoxXD<4>;

  // NTrees for higher dimensions
  using TreePoint16D = TreePointXD<16>;
  using TreeBox16D = TreeBoxXD<16>;
}


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

#endif // NTREE_GUARD
