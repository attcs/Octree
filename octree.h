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
#include <execution>
#include <stdexcept>

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

  constexpr uint64_t pow_ce(uint64_t a, uint8_t e) { return e == 0 ? 1 : a * pow_ce(a, e - 1); }
  namespace
  {
    using std::array;
    using std::bitset;
    using std::span;
    using std::vector;
    using std::unordered_map;
    using std::map;
    using std::queue;
    using std::set;
    using std::multiset;
  }


  // Grid id
  using grid_id_type = uint32_t;

  // Type of the dimension
  using dim_type = uint8_t;

  // Content id type
  using entity_id_type = size_t;



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
      auto d2 = geometry_type{ 0 };
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

    static constexpr point_type add(point_type const& ptL, point_type const& ptR)
    {
      auto pt = point_type{};
      for (dim_type iDim = 0; iDim < nDimension; ++iDim)
        base::point_comp(pt, iDim) = base::point_comp_c(ptL, iDim) + base::point_comp_c(ptR, iDim);

      return pt;
    }

    static constexpr point_type subtract(point_type const& ptL, point_type const& ptR)
    {
      auto pt = point_type{};
      for (dim_type iDim = 0; iDim < nDimension; ++iDim)
        base::point_comp(pt, iDim) = base::point_comp_c(ptL, iDim) - base::point_comp_c(ptR, iDim);

      return pt;
    }

    static constexpr point_type div(point_type const& ptL, geometry_type const& rScalarR)
    {
      auto pt = point_type{};
      for (dim_type iDim = 0; iDim < nDimension; ++iDim)
        base::point_comp(pt, iDim) = base::point_comp_c(ptL, iDim) / rScalarR;

      return pt;
    }

    static constexpr geometry_type distance(point_type const& ptL, point_type const& ptR)
    {
      return size(subtract(ptL, ptR));
    }

    static constexpr geometry_type distance2(point_type const& ptL, point_type const& ptR)
    {
      return size2(subtract(ptL, ptR));
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
      autoc e1_contains_e2min = does_box_contain_point(e1, base::box_min_c(e2));

      return e1_must_contain_e2
        ? e1_contains_e2min && does_box_contain_point(e1, base::box_max_c(e2))
        : fOverlapPtTouchAllowed
          ? e1_contains_e2min || does_box_contain_point(e1, base::box_max_c(e2)) || does_box_contain_point(e2, base::box_max_c(e1))
          : box_relation(e1, e2) == EBoxRelation::Overlapped
        ;
    }

    static inline box_type box_inverted_init()
    {
      auto ext = box_type{};
      auto& ptMin = base::box_min(ext);
      auto& ptMax = base::box_max(ext);

      autoce inf = std::numeric_limits<geometry_type>::infinity();
      for (dim_type iDimension = 0; iDimension < nDimension; ++iDimension)
      {
        base::point_comp(ptMin, iDimension) = +inf;
        base::point_comp(ptMax, iDimension) = -inf;
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

    static void move_box(box_type& box, point_type const& vMove)
    {
      for (dim_type iDimension = 0; iDimension < nDimension; ++iDimension)
      {
        base::point_comp(base::box_min(box), iDimension) += base::point_comp_c(vMove, iDimension);
        base::point_comp(base::box_max(box), iDimension) += base::point_comp_c(vMove, iDimension);
      }
    }

    static std::optional<double> is_ray_hit(box_type const& box, point_type const& rayBase, point_type const& rayHeading)
    {
      if (does_box_contain_point(box, rayBase))
        return 0.0;

      autoc& ptBoxMin = base::box_min_c(box);
      autoc& ptBoxMax = base::box_max_c(box);

      autoce inf = std::numeric_limits<double>::infinity();
      // ptHit = rayBase + rayHeading * r
      auto aRMinMax = array<array<double, nDimension>, 2>();
      for (dim_type iDimension = 0; iDimension < nDimension; ++iDimension)
      {
        autoc hComp = base::point_comp_c(rayHeading, iDimension);
        if (hComp == 0)
        {
          if (base::point_comp_c(ptBoxMax, iDimension) < base::point_comp_c(rayBase, iDimension))
            return std::nullopt;

          if (base::point_comp_c(ptBoxMin, iDimension) > base::point_comp_c(rayBase, iDimension))
            return std::nullopt;

          aRMinMax[0][iDimension] = -inf;
          aRMinMax[1][iDimension] = +inf;
          continue;
        }

        aRMinMax[0][iDimension] = (base::point_comp_c(hComp > 0.0 ? ptBoxMin : ptBoxMax, iDimension) - base::point_comp_c(rayBase, iDimension)) / hComp;
        aRMinMax[1][iDimension] = (base::point_comp_c(hComp < 0.0 ? ptBoxMin : ptBoxMax, iDimension) - base::point_comp_c(rayBase, iDimension)) / hComp;
      }

      autoc rMin = *std::ranges::max_element(aRMinMax[0]);
      autoc rMax = *std::ranges::min_element(aRMinMax[1]);
      if (rMin > rMax || rMax < 0.0)
        return std::nullopt;

      return rMin < 0 ? rMax : rMin;
    }
  };


  template<dim_type nDimension, typename point_type, typename box_type, typename geometry_type = double>
  using AdaptorGeneral = AdaptorGeneralBase<nDimension, point_type, box_type, AdaptorGeneralBasics<nDimension, point_type, box_type, geometry_type>, geometry_type>;


  template<size_t N> using bitset_arithmetic = bitset<N>;

  template<size_t N>
  bitset_arithmetic<N> operator+ (bitset_arithmetic<N> const& lhs, bitset_arithmetic<N> const& rhs)
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
  bitset_arithmetic<N> operator- (bitset_arithmetic<N> const& lhs, bitset_arithmetic<N> const& rhs)
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
    bool operator()(bitset_arithmetic<N> const& lhs, bitset_arithmetic<N> const& rhs) const noexcept { return lhs < rhs; }
  };



  // NTrees


  // NTreeLinear: Non-owning base container which spatially organize data ids in N dimension space into a hash-table by Morton Z order.
  template<dim_type nDimension, typename point_type, typename box_type, typename adaptor_type = AdaptorGeneral<nDimension, point_type, box_type, double>, typename geometry_type = double>
  class OrthoTreeBase
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
    using morton_grid_id_type = typename std::conditional<nDimension < 4
      , uint32_t
      , typename std::conditional<is_linear_tree
        , uint64_t
        , bitset_arithmetic<nDimension * 4 + 1>
      >::type
    >::type;
    
    using morton_node_id_type = morton_grid_id_type; // same as the morton_grid_id_type, but depth is signed by a sentinel bit.
    using morton_grid_id_type_cref = typename std::conditional<is_linear_tree, morton_node_id_type, morton_node_id_type const&>::type;
    using morton_node_id_type_cref = morton_grid_id_type_cref;
    using max_element_type = uint32_t;
    using depth_type = uint32_t;


  protected:

    // Type system determined maximal depth.
    static autoce _nDepthMaxTheoretical = depth_type((CHAR_BIT * sizeof(morton_node_id_type) - 1/*sentinal bit*/) / nDimension);

    using _Ad = adaptor_type;
    static_assert(AdaptorConcept<_Ad, point_type, box_type, geometry_type>);


  public:

    class Node
    {
    private:
      vector<morton_node_id_type> _children;

    public: // Public members
      vector<entity_id_type> vid = {};
      box_type box = {};

    public:
      constexpr void AddChild(morton_node_id_type_cref kChild) { _children.emplace_back(kChild); }
      constexpr void AddChildInOrder(morton_node_id_type_cref kChild)
      {
        auto it = std::end(_children);
        if constexpr (is_linear_tree)
          it = std::lower_bound(_children.begin(), _children.end(), kChild);
        else
          it = std::lower_bound(_children.begin(), _children.end(), kChild, bitset_arithmetic_compare{});

        if (it != _children.end() && *it == kChild)
          return;

        _children.insert(it, kChild);
      }

      constexpr bool HasChild(morton_node_id_type_cref kChild) const 
      {
        if constexpr (is_linear_tree)
          return std::ranges::binary_search(_children, kChild);
        else
          return std::ranges::binary_search(_children, kChild, bitset_arithmetic_compare{});
      }

      constexpr bool IsChildNodeEnabled(child_id_type idChild)
      { 
        autoc midChild = morton_node_id_type(idChild);
        return std::find_if(std::begin(_children), std::end(_children), [midChild](autoc& kChild) { return kChild & midChild == midChild; });
      }

      constexpr void DisableChild(morton_node_id_type_cref kChild)
      {
        auto it = std::end(_children);
        if constexpr (is_linear_tree)
          it = std::lower_bound(_children.begin(), _children.end(), kChild);
        else
          it = std::lower_bound(_children.begin(), _children.end(), kChild, bitset_arithmetic_compare{});

        if (it == std::end(_children))
          return;

        _children.erase(it);
      }

      constexpr bool IsAnyChildExist() const noexcept { return !_children.empty(); }
      constexpr vector<morton_node_id_type> const& GetChildren() const noexcept { return _children; }
    };


  protected: // Aid struct to partitioning and distance ordering

    struct _ItemDistance
    {
      geometry_type distance;
      auto operator <=> (_ItemDistance const& rhs) const = default;
    };

    struct _EntityDistance : _ItemDistance 
    { 
      entity_id_type id;
      auto operator <=> (_EntityDistance const& rhs) const = default;
    };
    struct _BoxDistance : _ItemDistance { morton_node_id_type kNode; Node const& node; };

    template<typename data_type>
    using container_type = typename std::conditional<is_linear_tree, unordered_map<morton_node_id_type, data_type>, map<morton_node_id_type, data_type, bitset_arithmetic_compare>>::type;

  protected: // Member variables
    container_type<Node> _nodes;
    box_type _box = {};
    depth_type _nDepthMax = 0;
    grid_id_type _nRasterResolutionMax = 0;
    grid_id_type _idSlotMax = 0;
    max_element_type _nElementMax = 11;
    double _rVolume = 0.0;
    array<double, nDimension> _aRasterizer;

  protected: // Aid functions

    template<size_t N>
    static inline child_id_type _mortonIdToChildId(bitset_arithmetic<N> const& bs)
    {
      assert(bs <= bitset_arithmetic<N>(std::numeric_limits<size_t>::max()));
      return bs.to_ullong();
    }
    static constexpr child_id_type _mortonIdToChildId(uint64_t morton) { return morton; }


    static inline vector<entity_id_type> _generatePointId(size_t n)
    {
      auto vidPoint = vector<entity_id_type>(n);
      std::iota(begin(vidPoint), end(vidPoint), 0);
      return vidPoint;
    }


  protected: // Grid functions

    static constexpr array<double, nDimension> _getGridRasterizer(point_type const& p0, point_type const& p1, grid_id_type n_divide)
    {
      auto aRasterizer = array<double, nDimension>{};
      
      for (dim_type iDimension = 0; iDimension < nDimension; ++iDimension)
      {
        autoc rExt = adaptor_type::point_comp_c(p1, iDimension) - adaptor_type::point_comp_c(p0, iDimension);
        aRasterizer[iDimension] = rExt == 0 ? 1.0 : (static_cast<double>(n_divide) / rExt);
      }
      return aRasterizer;
    }


    constexpr array<grid_id_type, nDimension> _getGridIdPoint(point_type const& pe) const
    {
      auto aid = array<grid_id_type, nDimension>{};
      for (dim_type iDimension = 0; iDimension < nDimension; ++iDimension)
        aid[iDimension] = std::min<grid_id_type>(this->_idSlotMax, static_cast<grid_id_type>(static_cast<double>(adaptor_type::point_comp_c(pe, iDimension) - adaptor_type::point_comp_c(adaptor_type::box_min_c(this->_box), iDimension)) * this->_aRasterizer[iDimension]));

      return aid;
    }


    constexpr array<array<grid_id_type, nDimension>, 2> _getGridIdBox(box_type const& box) const
    {
      autoc& p0 = _Ad::box_min_c(_box);

      auto aid = array<array<grid_id_type, nDimension>, 2>{};
      for (dim_type iDimension = 0; iDimension < nDimension; ++iDimension)
      {
        autoc ridMin = static_cast<double>(adaptor_type::point_comp_c(adaptor_type::box_min_c(box), iDimension) - adaptor_type::point_comp_c(p0, iDimension)) * _aRasterizer[iDimension];
        autoc ridMax = static_cast<double>(adaptor_type::point_comp_c(adaptor_type::box_max_c(box), iDimension) - adaptor_type::point_comp_c(p0, iDimension)) * _aRasterizer[iDimension];

        if (ridMin < 1.0)
          aid[0][iDimension] = 0;
        else if (ridMin > _idSlotMax)
          aid[0][iDimension] = _idSlotMax;
        else
          aid[0][iDimension] = static_cast<grid_id_type>(ridMin);


        if (ridMax < 1.0)
          aid[1][iDimension] = 0;
        else if (ridMax >= _idSlotMax)
          aid[1][iDimension] = _idSlotMax;
        else if (ridMin != ridMax && floor(ridMax) == ridMax)
          aid[1][iDimension] = static_cast<grid_id_type>(ridMax) - 1;
        else
          aid[1][iDimension] = static_cast<grid_id_type>(ridMax);

      }
      return aid;
    }


    inline Node& _createChild(Node& nodeParent, child_id_type iChild, morton_node_id_type_cref kChild)
    {
      assert(iChild < this->_nChild);
      nodeParent.AddChild(kChild);

      auto& nodeChild = _nodes[kChild];

      {
#pragma loop(ivdep)
        for (dim_type iDimension = 0; iDimension < nDimension; ++iDimension)
        {
          autoc fGreater = ((child_id_type{ 1 } << iDimension) & iChild) > 0;
          _Ad::point_comp(_Ad::box_min(nodeChild.box), iDimension) =
            fGreater * (_Ad::point_comp_c(_Ad::box_max_c(nodeParent.box), iDimension) + _Ad::point_comp_c(_Ad::box_min_c(nodeParent.box), iDimension)) / geometry_type{ 2 } +
            (!fGreater) * _Ad::point_comp_c(_Ad::box_min_c(nodeParent.box), iDimension);

          _Ad::point_comp(_Ad::box_max(nodeChild.box), iDimension) =
            fGreater * _Ad::point_comp_c(_Ad::box_max_c(nodeParent.box), iDimension) +
            (!fGreater) * ((_Ad::point_comp_c(_Ad::box_max_c(nodeParent.box), iDimension) + _Ad::point_comp_c(_Ad::box_min_c(nodeParent.box), iDimension)) / geometry_type{ 2 });
        }
      }
      return nodeChild;
    }


    constexpr morton_grid_id_type _getLocationId(point_type const& pt) const
    {
      return MortonEncode(this->_getGridIdPoint(pt));
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
          kNodeParent = kNodeParent >>= nDimension;
          assert(IsValidKey(kNodeParent));
          auto& nodeParent = this->_nodes[kNodeParent];
          nodeParent.AddChildInOrder(kNodeParent);
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

    template<typename data_type = Node>
    static void _reserveContainer(map<morton_node_id_type, data_type, bitset_arithmetic_compare>& m, size_t n) {};
    
    template<typename data_type = Node>
    static void _reserveContainer(unordered_map<morton_node_id_type, data_type>& m, size_t n) { m.reserve(n); };

  public: // Static aid functions

    static constexpr size_t EstimateNodeNumber(size_t nElement, depth_type nDepthMax, max_element_type nElementMax)
    {
      assert(nElementMax > 0);
      assert(nDepthMax > 0);
      
      if (nElement < 10)
        return 10;
      
      autoce rMult = 1.5;
      if ((nDepthMax + 1) * nDimension < 64)
      {
        size_t const nMaxChild = size_t{ 1 } << (nDepthMax * nDimension);
        autoc nElementInNode = nElement / nMaxChild;
        if (nElementInNode > nElementMax / 2)
          return nMaxChild;
      }

      autoc nElementInNodeAvg = static_cast<float>(nElement) / static_cast<float>(nElementMax);
      autoc nDepthEstimated = std::min(nDepthMax, static_cast<depth_type>(ceil((log2(nElementInNodeAvg) + 1) / static_cast<float>(nDimension))));
      if (nDepthEstimated * nDimension < 64)
        return static_cast<size_t>(rMult * (1 << nDepthEstimated * nDimension));

      return static_cast<size_t>(rMult * nElementInNodeAvg);
    }


    static inline morton_node_id_type GetHash(depth_type depth, morton_node_id_type_cref key) noexcept
    {
      assert(key < (morton_node_id_type(1) << (depth * nDimension)));
      return (morton_node_id_type{ 1 } << (depth * nDimension)) | key;
    }

    static constexpr morton_node_id_type GetRootKey() noexcept
    { 
      return morton_node_id_type{ 1 };
    }

    static constexpr bool IsValidKey(uint64_t key) { return key; }

    template<size_t N>
    static inline bool IsValidKey(bitset_arithmetic<N> const& key) noexcept { return !key.none(); }

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
      autoc nDepth = onDepth.has_value() ? *onDepth : GetDepth(key);
      return key - (1 << nDepth);
    }


  private: // Morton aid functions

    static inline child_id_type _getChildPartOfLocation(morton_node_id_type_cref key)
    {
      autoce maskLastBits1 = (morton_node_id_type{ 1 } << nDimension) - 1;

      return _mortonIdToChildId(key & maskLastBits1);
    }

    static constexpr morton_grid_id_type _part1By2(grid_id_type n)
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
    static constexpr morton_grid_id_type _part1By1(grid_id_type n)
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
    static inline morton_grid_id_type MortonEncode(array<grid_id_type, nDimension> const& aidGrid)
    {
      if constexpr (nDimension == 1)
        return morton_grid_id_type(aidGrid[0]);
      else if (nDimension == 2)
        return (_part1By1(aidGrid[1]) << 1) + _part1By1(aidGrid[0]);
      else if (nDimension == 3)
        return (_part1By2(aidGrid[2]) << 2) + (_part1By2(aidGrid[1]) << 1) + _part1By2(aidGrid[0]);
      else
      {
        auto msb = aidGrid[0];
        for (dim_type iDimension = 1; iDimension < nDimension; ++iDimension)
          msb |= aidGrid[iDimension];

        morton_grid_id_type id = 0;
        grid_id_type mask = 1;
        for (dim_type i = 0, shift = 0; msb; mask <<= 1, msb >>= 1, ++i)
          for (dim_type iDimension = 0; iDimension < nDimension; ++iDimension, ++shift)
            if constexpr (is_linear_tree)
              id |= (aidGrid[iDimension] & mask) << (shift - i);
            else
              id[shift] = aidGrid[iDimension] & mask;

        return id;
      }
    }

    static array<grid_id_type, nDimension> MortonDecode(morton_node_id_type_cref kNode, depth_type nDepthMax)
    {
      auto aidGrid = array<grid_id_type, nDimension>{};
      if constexpr (nDimension == 1)
        return { RemoveSentinelBit(kNode) };
      else
      {
        autoc nDepth = GetDepth(kNode);

        auto mask = morton_grid_id_type{ 1 };
        for (dim_type iDepth = nDepthMax - nDepth, shift = 0; iDepth < nDepthMax; ++iDepth)
          for (dim_type iDimension = 0; iDimension < nDimension; ++iDimension, ++shift)
            if constexpr (is_linear_tree)
            {
              aidGrid[iDimension] |= (kNode & mask) >> (shift - iDepth);
              mask <<= 1;
            }
            else
              aidGrid[iDimension] |= grid_id_type{ kNode[shift] } << iDepth;
      }
      return aidGrid;
    }


  public: // Getters

    auto const& GetNodes() const noexcept { return _nodes; }
    constexpr auto const& GetNode(morton_node_id_type_cref key) const { return cont_at(_nodes, key); }
    auto const& GetBox() const noexcept { return _box; }
    auto const& Get(morton_node_id_type_cref key) const { return cont_at(_nodes, key); }
    auto inline GetDepthMax() const noexcept { return _nDepthMax; }
    auto inline GetResolutionMax() const noexcept { return _nRasterResolutionMax; }


  public: // Main service functions

    // Alternative creation mode (instead of Create), Init then Insert items into leafs one by one. NOT RECOMMENDED.
    void constexpr Init(box_type const& box, depth_type nDepthMax, max_element_type nElementMax = 11)
    {
      assert(this->_nodes.empty()); // To build/setup/create the tree, use the Create() [recommended] or Init() function. If an already builded tree is wanted to be reset, use the Clear() function before init.
      assert(nDepthMax > 1);
      assert(nDepthMax <= _nDepthMaxTheoretical);
      assert(nDepthMax < std::numeric_limits<uint8_t>::max());
      assert(nElementMax > 1);
      assert(CHAR_BIT * sizeof(grid_id_type) >= _nDepthMax);

      this->_box = box;
      this->_rVolume = 1.0;
      for (dim_type iDimension = 0; iDimension < nDimension; ++iDimension)
        this->_rVolume *= _Ad::point_comp_c(_Ad::box_max_c(this->_box), iDimension) - _Ad::point_comp_c(_Ad::box_min_c(this->_box), iDimension);

      this->_nDepthMax = nDepthMax;
      this->_nRasterResolutionMax = static_cast<grid_id_type>(pow_ce(2, static_cast<uint8_t>(nDepthMax)));
      this->_idSlotMax = this->_nRasterResolutionMax - 1;
      this->_nElementMax = nElementMax;

      auto& nodeRoot = this->_nodes[GetRootKey()];
      nodeRoot.box = box;
      this->_aRasterizer = this->_getGridRasterizer(_Ad::box_min_c(this->_box), _Ad::box_max_c(this->_box), this->_nRasterResolutionMax);
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

        for (morton_node_id_type_cref kChild : node.GetChildren())
        {
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

        autoc nDepthChild = depth_type{ item.nDepth + 1 };
        for (morton_node_id_type kChild : item.pNode.GetChildren())
        {
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
      ids.reserve(_nodes.size() * std::max<size_t>(2, _nElementMax / 2));

      VisitNodes(GetRootKey(), [&ids](morton_node_id_type_cref key, autoc& node)
      { 
        ids.insert(std::end(ids), std::begin(node.vid), std::end(node.vid));
      });
      return ids;
    }


    // Erase an id. Traverse all node if it is needed, which has major performance penalty.
    constexpr virtual bool EraseId(entity_id_type id)
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

      autoce one = morton_grid_id_type{ 1 };
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
      std::erase_if(_nodes, [](autoc& p) { return p.first != GetRootKey(); });
      cont_at(_nodes, GetRootKey()).vid.clear();
    }


    template<typename execution_policy_type = std::execution::unsequenced_policy>
    void Move(point_type const& vMove)
    {
      std::for_each(execution_policy_type{}, std::begin(_nodes), std::end(_nodes), [&vMove](auto& pairKeyNode)
      {
        _Ad::move_box(pairKeyNode.second.box, vMove);
      });
      _Ad::move_box(this->_box, vMove);
    }


    morton_node_id_type FindSmallestNodeKey(morton_node_id_type keySearch) const
    {
      for (; IsValidKey(keySearch); keySearch >>= nDimension)
        if (this->_nodes.contains(keySearch))
          return keySearch;

      return morton_node_id_type{}; // Not found
    }


  protected:

    template <dim_type iDimensionSet>
    static constexpr void _constructGridIdRec(array<vector<grid_id_type>, nDimension> const& avidGridList, array<grid_id_type, nDimension>& aidGrid, vector<array<grid_id_type, nDimension>>& vidGrid)
    {
      if constexpr (iDimensionSet == 0)
        vidGrid.emplace_back(aidGrid);
      else
      {
        for (autoc idGrid : avidGridList[iDimensionSet - 1])
        {
          aidGrid[iDimensionSet - 1] = idGrid;
          _constructGridIdRec<iDimensionSet - 1>(avidGridList, aidGrid, vidGrid);
        }
      }
    }


    void _collectAllIdInDFS(Node const& nodeParent, vector<entity_id_type>& sidFound) const
    {
      std::copy(std::begin(nodeParent.vid), std::end(nodeParent.vid), std::back_inserter(sidFound));

      for (morton_node_id_type_cref kChild : nodeParent.GetChildren())
        _collectAllIdInDFS(this->GetNode(kChild), sidFound);
    }


    void _rangeSearch(box_type const& range, double rVolumeRange, double rVolumeParent, Node const& nodeParent, std::function<bool(entity_id_type)> const& fnPred, vector<entity_id_type>& sidFound) const
    {
      std::copy_if(std::begin(nodeParent.vid), std::end(nodeParent.vid), std::back_inserter(sidFound), fnPred);

      autoc rVolumeNode = rVolumeParent / this->_nChild;
      for (morton_node_id_type_cref keyChild : nodeParent.GetChildren())
      {
        autoc& nodeChild = this->GetNode(keyChild);

        auto bOverlap = true;
        for (dim_type iDim = 0; iDim < nDimension && bOverlap; ++iDim)
        {
          if (keyChild & (1 << iDim))
            bOverlap &= _Ad::point_comp_c(_Ad::box_min_c(nodeChild.box), iDim) < _Ad::point_comp_c(_Ad::box_max_c(range), iDim);
          else
            bOverlap &= _Ad::point_comp_c(_Ad::box_max_c(nodeChild.box), iDim) > _Ad::point_comp_c(_Ad::box_min_c(range), iDim);
        }
        if (!bOverlap)
          continue;

        if (rVolumeRange >= rVolumeNode && _Ad::are_boxes_overlapped(range, nodeChild.box))
          this->_collectAllIdInDFS(nodeChild, sidFound);
        else
          this->_rangeSearch(range, rVolumeRange, rVolumeNode, nodeChild, fnPred, sidFound);
      }
    }


    bool _rangeSearchRoot(box_type const& range, size_t nEntity, std::function<bool(entity_id_type)> const& fnPred, vector<entity_id_type>& sidFound, bool fLeafNodeContainsElementOnly = false) const
    {
      if (_Ad::are_boxes_overlapped(range, this->_box))
      {
        sidFound.resize(nEntity);
        std::iota(std::begin(sidFound), std::end(sidFound), 0);
        return nEntity;
      }
      
      // If the range has zero volume, it could stuck at any node comparison with point/side touch. It is eliminated to work node bounding box independently.
      for (dim_type iDimension = 0; iDimension < nDimension; ++iDimension)
        if (_Ad::point_comp_c(_Ad::box_min_c(range), iDimension) >= _Ad::point_comp_c(_Ad::box_max_c(range), iDimension))
          return false;

      autoc aid = this->_getGridIdBox(range);
      auto idLocationMin = MortonEncode(aid[0]);
      auto idLocationMax = MortonEncode(aid[1]);

      auto nDepth = this->_nDepthMax;
      for (auto flagDiffOfLocation = idLocationMin ^ idLocationMax; IsValidKey(flagDiffOfLocation); flagDiffOfLocation >>= nDimension, --nDepth)
        idLocationMin >>= nDimension;

      autoc keyRange = morton_node_id_type{ 1 } << (nDepth * nDimension) | idLocationMin;
      auto keyNodeSmallest = this->FindSmallestNodeKey(keyRange);
      if (!IsValidKey(keyNodeSmallest))
        return false;
     
      auto rVolumeRange = 1.0;
      for (dim_type iDimension = 0; iDimension < nDimension; ++iDimension)
        rVolumeRange *= _Ad::point_comp_c(_Ad::box_max_c(range), iDimension) - _Ad::point_comp_c(_Ad::box_min_c(range), iDimension);

      autoc rVolumeNode = this->_rVolume / (1 << (nDimension * nDepth));

      autoc nidFoundEstimation = this->_rVolume == 0 ? 10 : static_cast<size_t>((rVolumeRange * nEntity) / this->_rVolume);
      sidFound.reserve(nidFoundEstimation);
      autoc& node = this->GetNode(keyNodeSmallest);
      this->_rangeSearch(range, rVolumeRange, rVolumeNode, node, fnPred, sidFound);

      if (!fLeafNodeContainsElementOnly)
      {
        keyNodeSmallest >>= nDimension;
        for (; IsValidKey(keyNodeSmallest); keyNodeSmallest >>= nDimension)
        {
          autoc& node = this->GetNode(keyNodeSmallest);
          std::copy_if(std::begin(node.vid), std::end(node.vid), std::back_inserter(sidFound), fnPred);
        }
      }

      return true;
    }


  public:

    void CollectAllIdInDFS(morton_grid_id_type_cref keyParent, vector<entity_id_type>& vItem) const
    {
      autoc& node = cont_at(this->_nodes, keyParent);
      _collectAllIdInDFS(node, vItem);
    }


    // Doubles the handled space relative to the root. iRootNew defines the relative location in the new space
    //!IMPLEMENT void Extend(morton_node_id_type_cref iRootNew = 0) {}
  };



  // OrthoTreePoint: Non-owning container which spatially organize point ids in N dimension space into a hash-table by Morton Z order.
  template<dim_type nDimension, typename point_type, typename box_type, typename adaptor_type = AdaptorGeneral<nDimension, point_type, box_type, double>, typename geometry_type = double>
  class OrthoTreePoint : public OrthoTreeBase<nDimension, point_type, box_type, adaptor_type, geometry_type>
  {
  protected:
    using base = OrthoTreeBase<nDimension, point_type, box_type, adaptor_type, geometry_type>;
    using _EntityDistance = typename base::_EntityDistance;
    using _BoxDistance = typename base::_BoxDistance;
    using _Ad = typename base::_Ad;

  public:
    using depth_type = typename base::depth_type;
    using morton_grid_id_type = typename base::morton_grid_id_type;
    using morton_grid_id_type_cref = typename base::morton_grid_id_type_cref;
    using morton_node_id_type = typename base::morton_node_id_type;
    using morton_node_id_type_cref = typename base::morton_node_id_type_cref;
    using max_element_type = typename base::max_element_type;
    using child_id_type = typename base::child_id_type;

    using Node = typename base::Node;

  private: // Aid functions
    static constexpr max_element_type max_element_default = 21;


    using _LocationIterator = typename vector<std::pair<entity_id_type, morton_grid_id_type>>::const_iterator;
    void _addNodes(Node& nodeParent, morton_node_id_type_cref kParent, _LocationIterator& itEndPrev, _LocationIterator const& itEnd, morton_grid_id_type_cref idLocationBegin, depth_type nDepthRemain)
    {
      autoc nElement = std::distance(itEndPrev, itEnd);
      if (nElement < this->_nElementMax || nDepthRemain == 0)
      {
        nodeParent.vid.resize(nElement);
        std::transform(itEndPrev, itEnd, std::begin(nodeParent.vid), [](autoc& item) { return item.first; });
        itEndPrev = itEnd;
        return;
      }

      --nDepthRemain;
      autoc shift = nDepthRemain * nDimension;
      autoc nLocationStep = morton_grid_id_type{ 1 } << shift;
      autoc flagParent = kParent << nDimension;

      while (itEndPrev != itEnd)
      {
        autoc idChildActual = base::_mortonIdToChildId((itEndPrev->second - idLocationBegin) >> shift);
        autoc itEndActual = std::partition_point(itEndPrev, itEnd, [&](autoc& idPoint)
        {
          return idChildActual == base::_mortonIdToChildId((idPoint.second - idLocationBegin) >> shift);
        });

        autoc mChildActual = morton_grid_id_type(idChildActual);
        morton_grid_id_type const kChild = flagParent | mChildActual;
        morton_grid_id_type const idLocationBeginChild = idLocationBegin + mChildActual * nLocationStep;

        auto& nodeChild = this->_createChild(nodeParent, idChildActual, kChild);
        this->_addNodes(nodeChild, kChild, itEndPrev, itEndActual, idLocationBeginChild, nDepthRemain);
      }
    }


  public: // Create

    // Ctors
    OrthoTreePoint() = default;
    OrthoTreePoint(span<point_type const> const& vpt, depth_type nDepthMax, std::optional<box_type> const& oBoxSpace = std::nullopt, max_element_type nElementMaxInNode = max_element_default)
    {
      *this = Create(vpt, nDepthMax, oBoxSpace, nElementMaxInNode);
    }

    // Create
    template<typename execution_policy_type = std::execution::unsequenced_policy>
    static OrthoTreePoint Create(span<point_type const> const& vpt, depth_type nDepthMax, std::optional<box_type> const& oBoxSpace = std::nullopt, max_element_type nElementMaxInNode = max_element_default)
    {
      autoc boxSpace = oBoxSpace.has_value() ? *oBoxSpace : _Ad::box_of_points(vpt);
      autoc n = vpt.size();

      auto tree = OrthoTreePoint{};
      tree.Init(boxSpace, nDepthMax, nElementMaxInNode);
      base::_reserveContainer(tree._nodes, base::EstimateNodeNumber(n, nDepthMax, nElementMaxInNode));
      if (vpt.empty())
        return tree;

      autoc kRoot = base::GetRootKey();
      auto& nodeRoot = cont_at(tree._nodes, kRoot);
      

      // Generate Morton location ids
      autoc vidPoint = base::_generatePointId(n);
      auto aidLocation = vector<std::pair<entity_id_type, morton_grid_id_type>>(n);
      std::transform(execution_policy_type{}, std::begin(vpt), std::end(vpt), std::begin(vidPoint), std::begin(aidLocation), [&](autoc& pt, autoc id) -> std::pair<entity_id_type, morton_grid_id_type>
      {
        return { id, tree._getLocationId(pt) };
      });

      std::sort(execution_policy_type{}, std::begin(aidLocation), std::end(aidLocation), [&](autoc& idL, autoc& idR) { return idL.second < idR.second; });
      auto itBegin = std::begin(aidLocation);
      tree._addNodes(nodeRoot, kRoot, itBegin, std::end(aidLocation), morton_node_id_type{ 0 }, nDepthMax);
      //if constexpr (tree.is_linear_tree)
      //  tree._nodes.rehash(1);
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

      autoc idLocation = this->_getLocationId(pt);
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
      autoc itRemove = std::remove(std::begin(vid), std::end(vid), id);
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

      autoc idLocation = this->_getLocationId(pt);
      return this->FindSmallestNodeKey(this->GetHash(this->_nDepthMax, idLocation));
    }
 
    bool Contains(point_type const& pt, span<point_type const> const& vpt, geometry_type rAccuracy) const
    {
      autoc kSmallestNode = this->FindSmallestNode(pt);
      if (!base::IsValidKey(kSmallestNode))
        return false;

      autoc& node = cont_at(this->_nodes, kSmallestNode);
      return std::ranges::any_of(node.vid, [&](autoc& id) { return _Ad::are_points_equal(pt, vpt[id], rAccuracy); });
    }


    // Range search
    vector<entity_id_type> RangeSearch(box_type const& range, span<point_type const> const& vpt, bool fLeafNodeContainsElementOnly = false) const
    {
      auto sidFound = vector<entity_id_type>();

      autoc nEntity = vpt.size();
      if (!this->_rangeSearchRoot(range, nEntity, [&range, &vpt](autoc id) { return _Ad::does_box_contain_point(range, vpt[id]); }, sidFound, fLeafNodeContainsElementOnly))
        return {};

      return sidFound;
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


    static void _createEntityDistance(Node const& node, point_type const& pt, span<point_type const> const& vpt, multiset<_EntityDistance>& setEntity)
    {
      for (autoc id : node.vid)
        setEntity.insert({ { _Ad::distance(pt, vpt[id]) }, id });
    }

    static geometry_type _getFarestDistance(multiset<_EntityDistance>& setEntity, size_t k)
    {
      if (setEntity.size() < k)
        return std::numeric_limits<geometry_type>::infinity();

      return std::next(std::begin(setEntity), k - 1)->distance;
    }

    static vector<entity_id_type> _convertEntityDistanceToList(multiset<_EntityDistance>& setEntity, size_t k)
    {
      autoc nEntity = std::min(k, setEntity.size());
      auto vidEntity = vector<entity_id_type>(nEntity);
      std::transform(std::begin(setEntity), std::next(std::begin(setEntity), nEntity), std::begin(vidEntity), [](autoc& ed) { return ed.id; });
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
        setNodeDist.insert({ { _Ad::size(aDist)}, key, node });
      });

      if (!setNodeDist.empty())
      {
        auto rLatestNodeDist = std::begin(setNodeDist)->distance;
        for (autoc& nodeDist : setNodeDist)
        {
          autoc n = setEntity.size();
          if (k <= n && rLatestNodeDist < nodeDist.distance)
            break;

          _createEntityDistance(nodeDist.node, pt, vpt, setEntity);
          rLatestNodeDist = nodeDist.distance;
        }
      }

      return _convertEntityDistanceToList(setEntity, k);
    }
  };



  // OrthoTreeBoundingBox: Non-owning container which spatially organize bounding box ids in N dimension space into a hash-table by Morton Z order. 
  // nSplitStrategyAdditionalDepth: if (nSplitStrategyAdditionalDepth > 0) Those items which are not fit in the child nodes may be stored in the children/grand-children instead of the parent.
  template<dim_type nDimension, typename point_type, typename box_type, typename adaptor_type = AdaptorGeneral<nDimension, point_type, box_type, double>, typename geometry_type = double, uint32_t nSplitStrategyAdditionalDepth = 2>
  class OrthoTreeBoundingBox : public OrthoTreeBase<nDimension, point_type, box_type, adaptor_type, geometry_type>
  {
  private:
    using base = OrthoTreeBase<nDimension, point_type, box_type, adaptor_type, geometry_type>;
    using _EntityDistance = typename base::_EntityDistance;
    using _BoxDistance = typename base::_BoxDistance;
    using _Ad = typename base::_Ad;

  public:
    using depth_type = typename base::depth_type;
    using morton_grid_id_type = typename base::morton_grid_id_type;
    using morton_grid_id_type_cref = typename base::morton_grid_id_type_cref;
    using morton_node_id_type = typename base::morton_node_id_type;
    using morton_node_id_type_cref = typename base::morton_node_id_type_cref;
    using max_element_type = typename base::max_element_type;
    using child_id_type = typename base::child_id_type;

    using Node = typename base::Node;

  private: // Aid functions

    static constexpr max_element_type max_element_default = 21;
    struct _Location
    {
      entity_id_type id;
      morton_grid_id_type idMin;
      depth_type depth;

      auto operator < (_Location const& idLocationR) const
      {
        if (depth == idLocationR.depth)
          return idMin < idLocationR.idMin;
        else if (depth < idLocationR.depth)
        {
          autoc shift = (idLocationR.depth - depth) * nDimension;
          autoc aidRS = idLocationR.idMin >> shift;
          if (idMin == aidRS)
            return true;

          return idMin < aidRS;
        }
        else
        {
          autoc shift = (depth - idLocationR.depth) * nDimension;
          autoc aidLS = idMin >> shift;
          if (idLocationR.idMin == aidLS)
            return false;

          return aidLS < idLocationR.idMin;
        }
      }
    };

    using _LocationContainer = vector<_Location>;
    using _LocationIterator = typename _LocationContainer::const_iterator;

    static constexpr child_id_type _getIdChildAtDepth(_Location const& loc, depth_type depthCheck, morton_node_id_type_cref idLocationCurDepth)
    {
      assert(depthCheck <= loc.depth);
      autoc idGridCurDepth = loc.idMin >> ((loc.depth - depthCheck) * nDimension);
      return base::_mortonIdToChildId(idGridCurDepth - idLocationCurDepth);
    }


    void _addNodes(Node& nodeParent, morton_node_id_type_cref kParent, _LocationIterator& itEndPrev, _LocationIterator const& itEnd, morton_grid_id_type_cref idLocationBegin, depth_type nDepthRemain)
    {
      autoc nElement = std::distance(itEndPrev, itEnd);
      if (nElement < this->_nElementMax || nDepthRemain == 0)
      {
        nodeParent.vid.resize(nElement);
        std::transform(itEndPrev, itEnd, begin(nodeParent.vid), [](autoc& item) { return item.id; });
        itEndPrev = itEnd;
        return;
      }

      autoc nGridHalfSizeOfNode = static_cast<grid_id_type>(pow_ce(2, nDepthRemain - 1));

      auto depthCheck = this->_nDepthMax - nDepthRemain;
      if (itEndPrev->depth == depthCheck)
      {
        auto it = itEndPrev;
        itEndPrev = std::partition_point(it, itEnd, [&](autoc& idLocation) { return idLocation.depth == it->depth; });
        autoc nElementCur = static_cast<int>(std::distance(it, itEndPrev));

        nodeParent.vid.resize(nElementCur);
        std::transform(it, itEndPrev, std::begin(nodeParent.vid), [](autoc& item) { return item.id; });
      }

      ++depthCheck;
      --nDepthRemain;

      autoc shift = nDepthRemain * nDimension;
      autoc nLocationStep = morton_grid_id_type{ 1 } << shift;
      autoc flagParent = kParent << nDimension;
      autoc idLocationCurDepth = idLocationBegin >> shift;

      while (itEndPrev != itEnd)
      {
        autoc idChildActual = _getIdChildAtDepth(*itEndPrev, depthCheck, idLocationCurDepth);
        autoc itEndActual = std::partition_point(itEndPrev, itEnd, [&](autoc& loc)
        {
          autoc idChild = _getIdChildAtDepth(loc, depthCheck, idLocationCurDepth);
          return idChildActual == idChild;
        });

        autoc mChildActual = morton_grid_id_type(idChildActual);
        morton_grid_id_type const kChild = flagParent | mChildActual;
        morton_grid_id_type const idLocationBeginChild = idLocationBegin + mChildActual * nLocationStep;

        auto& nodeChild = this->_createChild(nodeParent, idChildActual, kChild);
        this->_addNodes(nodeChild, kChild, itEndPrev, itEndActual, idLocationBeginChild, nDepthRemain);
      }
    }


    constexpr void _split(array<array<grid_id_type, nDimension>, 2> const& aidBoxGrid, vector<_Location> & vLocation) const
    {
      auto nDepth = vLocation[0].depth + nSplitStrategyAdditionalDepth;
      if (nDepth > this->_nDepthMax)
        nDepth = this->_nDepthMax;

      autoc nDepthRemain = this->_nDepthMax - nDepth;
      autoc nStepGrid = pow_ce(2, nDepthRemain);

      auto aMinGridList = array<vector<grid_id_type>, nDimension>{};
      auto nBoxByGrid = 1.0;
      for (dim_type iDim = 0; iDim < nDimension; ++iDim)
      {
        autoc nGridSplitFirst = (aidBoxGrid[0][iDim] / nStepGrid) + 1;
        autoc nGridSplitLast = (aidBoxGrid[1][iDim] / nStepGrid);
        autoc nMinGridList = (nGridSplitLast < nGridSplitFirst ? 0 : (nGridSplitLast - nGridSplitFirst + 1)) + 1;
        aMinGridList[iDim].resize(nMinGridList);
        aMinGridList[iDim][0] = aidBoxGrid[0][iDim];
        size_t i = 1;
        for (auto nGridSplit = nGridSplitFirst; nGridSplit <= nGridSplitLast; ++nGridSplit, ++i)
          aMinGridList[iDim][i] = static_cast<grid_id_type>(nGridSplit * nStepGrid);

        nBoxByGrid *= nMinGridList;
      }

      if (nBoxByGrid >= this->_nChild)
        return;

      auto vaidMinGrid = vector<array<grid_id_type, nDimension>>{};
      auto aidGrid = array<grid_id_type, nDimension>{};
      base::template _constructGridIdRec<nDimension>(aMinGridList, aidGrid, vaidMinGrid);
      autoc nBox = vaidMinGrid.size();
      vLocation.resize(nBox);
      autoc id = vLocation[0].id;
      autoc shift = nDepthRemain * nDimension;

#pragma loop(ivdep)
      for (size_t iBox = 0; iBox < nBox; ++iBox)
      {
        vLocation[iBox].id = id;
        vLocation[iBox].depth = nDepth;
        vLocation[iBox].idMin = base::MortonEncode(vaidMinGrid[iBox]) >> shift;
      }
    }

    constexpr void _setLocation(box_type const& box, vector<_Location>& vLocation) const
    {
      autoc aidBoxGrid = this->_getGridIdBox(box);

      auto& loc = vLocation[0];
      loc.depth = this->_nDepthMax;

      loc.idMin = base::MortonEncode(aidBoxGrid[0]);
      autoc idMax = base::MortonEncode(aidBoxGrid[1]);
      if (loc.idMin == idMax)
        return;

      autoc idMin = loc.idMin;
      for (auto flagDiffOfLocation = loc.idMin ^ idMax; base::IsValidKey(flagDiffOfLocation); flagDiffOfLocation >>= nDimension, --loc.depth)
        loc.idMin >>= nDimension;

      if constexpr (nSplitStrategyAdditionalDepth > 0)
      {
        autoc nDepthRemain = this->_nDepthMax - loc.depth;
        if (base::IsValidKey((idMax - idMin) >> (nDepthRemain * nDimension - 1)))
          return; // all nodes are touched, it is leaved.

        this->_split(aidBoxGrid, vLocation);
      }
    }

  public: // Create

    // Ctors
    OrthoTreeBoundingBox() = default;
    OrthoTreeBoundingBox(span<box_type const> const& vBox, depth_type nDepthMax, std::optional<box_type> const& oBoxSpace = std::nullopt, max_element_type nElementMaxInNode = max_element_default)
    {
      *this = Create(vBox, nDepthMax, oBoxSpace, nElementMaxInNode);
    }



    // Create
    template<typename execution_policy_type = std::execution::unsequenced_policy>
    static OrthoTreeBoundingBox Create(span<box_type const> const& vBox, depth_type nDepthMax, std::optional<box_type> const& oBoxSpace = std::nullopt, max_element_type nElementMaxInNode = max_element_default)
    {
      autoc boxSpace = oBoxSpace.has_value() ? *oBoxSpace : _Ad::box_of_boxes(vBox);
      auto tree = OrthoTreeBoundingBox{};
      tree.Init(boxSpace, nDepthMax, nElementMaxInNode);

      autoc n = vBox.size();

      base::_reserveContainer(tree._nodes, base::EstimateNodeNumber(n, nDepthMax, nElementMaxInNode));
      if (n == 0)
        return tree;

      autoc kRoot = base::GetRootKey();
      auto& nodeRoot = cont_at(tree._nodes, kRoot);

      autoc vid = base::_generatePointId(n);
      auto aLocation = vector<_Location>();
      auto avLocation = vector<vector<_Location>>(n, { _Location{} });

      autoce nSplit = 2;
      std::for_each(execution_policy_type{}, std::begin(vid), std::end(vid), [&tree, &vBox, &avLocation](autoc id)
      {
        avLocation[id][0].id = id;
        tree._setLocation(vBox[id], avLocation[id]);
      });

      aLocation.reserve(n * 4);
      for (autoc& vLocation : avLocation)
        std::copy(std::begin(vLocation), std::end(vLocation), std::back_inserter(aLocation));

      std::sort(execution_policy_type{}, std::begin(aLocation), std::end(aLocation));
      auto itBegin = std::begin(aLocation);
      tree._addNodes(nodeRoot, kRoot, itBegin, std::end(aLocation), morton_node_id_type{ 0 }, nDepthMax);
      return tree;
    }


  public: // Edit functions

    // Find smallest node which contains the box by grid id description
    morton_node_id_type FindSmallestNode(array<array<grid_id_type, nDimension>, 2> const& aidGrid) const
    {
      auto idLocationMin = base::MortonEncode(aidGrid[0]);
      auto idLocationMax = base::MortonEncode(aidGrid[1]);

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

      autoc aidGrid = this->_getGridIdBox(box);
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

      auto vLocation = vector<_Location>(1);
      vLocation[0].id = id;
      _setLocation(box, vLocation);

      for (autoc& loc : vLocation)
      {
        autoc kNode = this->GetHash(loc.depth, loc.idMin);
        if (!this->_insert(kNode, kNodeSmallest, id, fInsertToLeaf))
          return false;
      }

      return true;
    }

  
  private: 
    bool _erase(morton_node_id_type_cref kNode, entity_id_type id)
    {
      auto& vid = cont_at(this->_nodes, kNode).vid;
      autoc itRemove = std::remove(begin(vid), end(vid), id);
      if (itRemove == end(vid))
        return false; // id was not registered previously.

      vid.erase(itRemove, vid.end());
      return true;
    }


    template<depth_type nDepthRemainSet>
    bool _eraseRec(morton_node_id_type_cref kNode, entity_id_type id)
    {
      auto ret = this->_erase(kNode, id);
      if constexpr (nDepthRemainSet > 0)
      {
        autoc& node = cont_at(this->_nodes, kNode);
        for (morton_node_id_type_cref kChild : node.GetChildren())
          ret |= _eraseRec<nDepthRemainSet - 1>(kChild, id);
      }
      return ret;
    }


  public:
    // Erase id, aided with the original bounding box
    bool Erase(entity_id_type id, box_type const& box)
    {
      autoc kOld = FindSmallestNode(box);
      if (!kOld)
        return false; // old box is not in the handled space domain

      return _eraseRec<nSplitStrategyAdditionalDepth>(kOld, id);
    }


    // Erase an id. Traverse all node if it is needed, which has major performance penalty.
    constexpr bool EraseId(entity_id_type id) override
    {
      if constexpr (nSplitStrategyAdditionalDepth == 0)
        return std::ranges::any_of(this->_nodes, [&](auto& pairNode) { return erase(pairNode.second.vid, id); });
      else
      {
        bool bErased = false;
#pragma warning( suppress : 4805 )
        std::ranges::for_each(this->_nodes, [&](auto& pairNode) { bErased |= erase(pairNode.second.vid, id); });
#pragma warning( default : 4805 )

        return bErased;
      }
    }


    // Update id by the new bounding box information
    bool Update(entity_id_type id, box_type const& boxNew)
    {
      if (!_Ad::are_boxes_overlapped(this->_box, boxNew))
        return false;

      if (!this->EraseId(id))
        return false;

      return this->Insert(id, boxNew);
    }


    // Update id by the new point information and the erase part is aided by the old bounding box geometry data
    bool Update(entity_id_type id, box_type const& boxOld, box_type const& boxNew)
    {
      if (!_Ad::are_boxes_overlapped(this->_box, boxNew))
        return false;

      if (!this->Erase(id, boxOld))
        return false; // id was not registered previously.

      return this->Insert(id, boxNew);
    }


  public: // Search functions
    
    // Pick search
    vector<entity_id_type> PickSearch(point_type const& ptPick, span<box_type const> const& vBox) const
    {
      autoc idLocation = this->_getLocationId(ptPick);

      auto vidFound = vector<entity_id_type>();
      vidFound.reserve(100);
      autoc itEnd = std::end(this->_nodes);
      for (auto kNode = this->GetHash(this->_nDepthMax, idLocation); base::IsValidKey(kNode); kNode >>= nDimension)
      {
        autoc itNode = this->_nodes.find(kNode);
        if (itNode == itEnd)
          continue;

        std::ranges::copy_if(itNode->second.vid, back_inserter(vidFound), [&](autoc id) { return _Ad::does_box_contain_point(vBox[id], ptPick); });
      }

      return vidFound;
    }

    
    // Range search
    vector<entity_id_type> RangeSearch(box_type const& range, span<box_type const> const& vBox, bool fFullyContained = true) const
    {
      auto sidFound = vector<entity_id_type>();

      autoc nEntity = vBox.size();
      if (!this->_rangeSearchRoot(range, nEntity, [&range, &vBox, fFullyContained](autoc id) { return _Ad::are_boxes_overlapped(range, vBox[id], fFullyContained); }, sidFound, false))
        return {};

      if constexpr (nSplitStrategyAdditionalDepth > 0)
      {
        std::ranges::sort(sidFound);
        autoc itEnd = std::unique(begin(sidFound), end(sidFound));
        sidFound.erase(itEnd, std::end(sidFound));
      }

      return sidFound;
    }


    // Collision detection: Returns all overlapping boxes from the source trees.
    static vector<std::pair<entity_id_type, entity_id_type>> CollisionDetection(OrthoTreeBoundingBox const& treeL, span<box_type const> const& vBoxL, OrthoTreeBoundingBox const& treeR, span<box_type const> const& vBoxR)
    {
      using NodeIterator = base::template container_type<Node>::const_iterator;
      struct NodeIteratorAndStatus { NodeIterator it; bool fTraversed; };
      using ParentIteratorArray = array<NodeIteratorAndStatus, 2>;

      enum : bool { Left, Right };

      auto vResult = vector<std::pair<entity_id_type, entity_id_type>>{};
      vResult.reserve(vBoxL.size() / 10);

      autoc kRoot = base::GetRootKey();
      autoc aTree = array{ &treeL, &treeR };

      auto q = queue<ParentIteratorArray>{};
      for (q.push({ NodeIteratorAndStatus{ treeL._nodes.find(kRoot), false }, NodeIteratorAndStatus{ treeR._nodes.find(kRoot), false } }); !q.empty(); q.pop())
      {
        autoc& aitNodeParent = q.front();

        // Check the current ascendant content 
        {
          for (autoc idL : aitNodeParent[Left].it->second.vid)
            for (autoc idR : aitNodeParent[Right].it->second.vid)
              if (_Ad::are_boxes_overlapped(vBoxL[idL], vBoxR[idR], false))
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
          std::ranges::transform(vidChild, begin(avitChildNode[id]), [&](morton_node_id_type_cref kChild) -> NodeIteratorAndStatus
          {
            return { aTree[id]->_nodes.find(kChild), false };
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

      if constexpr (nSplitStrategyAdditionalDepth > 0)
      {
        std::ranges::sort(vResult);
        vResult.erase(std::unique(std::begin(vResult), std::end(vResult)), end(vResult));
      }

      return vResult;
    }


    // Collision detection: Returns all overlapping boxes from the source trees.
    vector<std::pair<entity_id_type, entity_id_type>> CollisionDetection(span<box_type const> const& vBox, OrthoTreeBoundingBox const& treeOther, span<box_type const> const& vBoxOther) const
    {
      return CollisionDetection(*this, vBox, treeOther, vBoxOther);
    }

    // Collision detection between the stored elements
    template<typename execution_policy_type = std::execution::unsequenced_policy>
    vector<std::pair<entity_id_type, entity_id_type>> CollisionDetection(span<box_type const> const& vBox) const
    {
      autoc nEntity = vBox.size();

      auto vidCheck = vector<entity_id_type>(nEntity);
      std::iota(std::begin(vidCheck), std::end(vidCheck), 0);

      auto vvidCollision = vector<vector<entity_id_type>>(vidCheck.size());
      std::transform(execution_policy_type{}, std::begin(vidCheck), std::end(vidCheck), std::begin(vvidCollision), [&vBox, this](autoc idCheck) -> vector<entity_id_type>
      {
        auto sidFound = vector<entity_id_type>();

        autoc nEntity = vBox.size();
        if (!this->_rangeSearchRoot(vBox[idCheck], nEntity, [idCheck, &vBox](autoc id) { return idCheck < id && _Ad::are_boxes_overlapped(vBox[idCheck], vBox[id], false); }, sidFound, false))
          return {};

        if constexpr (nSplitStrategyAdditionalDepth > 0)
        {
          std::ranges::sort(sidFound);
          autoc itEnd = std::unique(std::begin(sidFound), std::end(sidFound));
          sidFound.erase(itEnd, std::end(sidFound));
        }

        return sidFound;
      });

      auto vPair = vector<std::pair<entity_id_type, entity_id_type>>{};
      if (nEntity > 10)
        vPair.reserve(nEntity / 10);

      for (autoc idCheck : vidCheck)
        for (autoc idCollide : vvidCollision[idCheck])
          vPair.emplace_back(idCheck, idCollide);

      return vPair;
    }

  private:
   
    void _rayIntersectedAll(morton_node_id_type_cref kNode, Node const& node, span<box_type const> const& vBox, point_type const& rayBase, point_type const& rayHeading, vector<_EntityDistance>& vdidOut) const
    {
      autoc oIsHit = _Ad::is_ray_hit(node.box, rayBase, rayHeading);
      if (!oIsHit)
        return;

      for (autoc id : node.vid)
      {
        autoc oDist = _Ad::is_ray_hit(vBox[id], rayBase, rayHeading);
        if (oDist)
          vdidOut.push_back({ *oDist, id });
      }

      autoc flagPrefix = kNode << nDimension;
      for (autoc kChild : node.GetChildren())
      {
        _rayIntersectedAll(kChild, cont_at(this->_nodes, kChild), vBox, rayBase, rayHeading, vdidOut);
      }
    }


    void _rayIntersectedFirst(morton_node_id_type_cref kNode, Node const& node, span<box_type const> const& vBox, point_type const& rayBase, point_type const& rayHeading, multiset<_EntityDistance>& vidOut) const
    {
      autoc rLastDistance = vidOut.empty() ? std::numeric_limits<double>::infinity() : static_cast<double>(vidOut.rbegin()->distance);
      for (autoc id : node.vid)
      {
        autoc oDist = _Ad::is_ray_hit(vBox[id], rayBase, rayHeading);
        if (!oDist)
          continue;

        if (*oDist > rLastDistance)
          continue;

        vidOut.insert({ { *oDist }, id });
      }

      autoc flagPrefix = kNode << nDimension;
      auto msNode = multiset<_BoxDistance>();
      for (autoc kChild : node.GetChildren())
      {
        autoc& nodeChild = cont_at(this->_nodes, kChild);
        autoc oDist = _Ad::is_ray_hit(nodeChild.box, rayBase, rayHeading);
        if (!oDist)
          continue;

        if (*oDist > rLastDistance)
          continue;

        msNode.insert({ { static_cast<geometry_type>(oDist.value()) }, kChild, nodeChild });
      }
      
      for (autoc& nodeData : msNode)
        _rayIntersectedFirst(nodeData.kNode, nodeData.node, vBox, rayBase, rayHeading, vidOut);
      
    }


  public:

    // Get all box which is intersected by the ray in order
    vector<entity_id_type> RayIntersectedAll(point_type const& rayBase, point_type const& rayHeading, span<box_type const> const& vBox) const
    {
      autoc kRoot = base::GetRootKey();

      auto vdid = vector<_EntityDistance>();
      vdid.reserve(20);
      _rayIntersectedAll(kRoot, cont_at(this->_nodes, kRoot), vBox, rayBase, rayHeading, vdid);

      autoc itBegin = std::begin(vdid);
      auto itEnd = std::end(vdid);
      std::sort(itBegin, itEnd);
      if constexpr (nSplitStrategyAdditionalDepth > 0)
        itEnd = std::unique(itBegin, itEnd, [](autoc& lhs, autoc& rhs) { return lhs.id == rhs.id; });

      auto vid = vector<entity_id_type>(std::distance(itBegin, itEnd));
      std::transform(itBegin, itEnd, std::begin(vid), [](autoc& did) { return did.id; });
      return vid;
    }
    

    // Get first box which is intersected by the ray
    std::optional<entity_id_type> RayIntersectedFirst(point_type const& rayBase, point_type const& rayHeading, span<box_type const> const& vBox) const
    {
      autoc kRoot = base::GetRootKey();

      autoc& node = cont_at(this->_nodes, kRoot);
      autoc oDist = _Ad::is_ray_hit(node.box, rayBase, rayHeading);
      if (!oDist)
        return std::nullopt;

      auto vid = multiset<_EntityDistance>();
      _rayIntersectedFirst(kRoot, node, vBox, rayBase, rayHeading, vid);
      if (vid.empty())
        return std::nullopt;
    
      return std::begin(vid)->id;
    }
  };


  template<dim_type nDimension, typename geometry_type = double>
  using PointND = array<geometry_type, nDimension>;


  template <dim_type nDimension, typename geometry_type = double>
  struct BoundingBoxND
  {
    PointND<nDimension, geometry_type> Min;
    PointND<nDimension, geometry_type> Max;
  };


  // Aliases
  using Point1D = OrthoTree::PointND<1>;
  using Point2D = OrthoTree::PointND<2>;
  using Point3D = OrthoTree::PointND<3>;
  using BoundingBox1D = OrthoTree::BoundingBoxND<1>;
  using BoundingBox2D = OrthoTree::BoundingBoxND<2>;
  using BoundingBox3D = OrthoTree::BoundingBoxND<3>;

  template<size_t nDimension> using TreePointND = OrthoTree::OrthoTreePoint<nDimension, OrthoTree::PointND<nDimension>, OrthoTree::BoundingBoxND<nDimension>>;
  template<size_t nDimension> using TreeBoxND = OrthoTree::OrthoTreeBoundingBox<nDimension, OrthoTree::PointND<nDimension>, OrthoTree::BoundingBoxND<nDimension>>;

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

#endif // ORTHOTREE_GUARD
