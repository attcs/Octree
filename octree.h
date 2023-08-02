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
#include <type_traits>
#include <stdexcept>

#include <array>
#include <bitset>
#include <map>
#include <unordered_map>
#include <queue>
#include <set>
#include <span>
#include <vector>
#include <tuple>

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
#define HASSERT(isOk) assert(isOk); if (!isOk) { exit(1); }
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
#pragma warning(disable: 4715)
#endif
  // Crash the program if out_of_range exception is raised
  template<typename var_type, typename index_type, typename container_type>
  inline auto const& cont_at(container_type const& container, typename std::remove_reference_t<container_type>::key_type const& id) noexcept
  {
    return container.at(id);
  }

  // Crash the program if out_of_range exception is raised
  template<typename container_type>
  inline auto& cont_at(container_type& container, typename std::remove_reference_t<container_type>::key_type const& id) noexcept
  {
    return container.at(id);
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

  // Type of depth
  using depth_type = uint8_t;

  // Content id type
  using entity_id_type = size_t;



  // Adaptor concepts

  template <class adaptor_type, typename vector_type, typename box_type, typename geometry_type = double>
  concept AdaptorBasicsConcept =
    requires (vector_type & pt, dim_type iDimension) { {adaptor_type::point_comp(pt, iDimension)}->std::convertible_to<geometry_type&>; }
  && requires (vector_type const& pt, dim_type iDimension) { {adaptor_type::point_comp_c(pt, iDimension)}->std::convertible_to<geometry_type>; }
  && requires (box_type& box) { { adaptor_type::box_min(box) }->std::convertible_to<vector_type&>; }
  && requires (box_type& box) { { adaptor_type::box_max(box) }->std::convertible_to<vector_type&>; }
  && requires (box_type const& box) { { adaptor_type::box_min_c(box) }->std::convertible_to<vector_type const&>; }
  && requires (box_type const& box) { { adaptor_type::box_max_c(box) }->std::convertible_to<vector_type const&>; }
  ;

  template <class adaptor_type, typename vector_type, typename box_type, typename geometry_type = double>
  concept AdaptorConcept =
    requires { AdaptorBasicsConcept<adaptor_type, vector_type, box_type, geometry_type>; }
  && requires (box_type const& box, vector_type const& pt) { { adaptor_type::does_box_contain_point(box, pt)}->std::convertible_to<bool>; }
  && requires (box_type const& e1, box_type const& e2, bool e1_must_contain_e2) { { adaptor_type::are_boxes_overlapped(e1, e2, e1_must_contain_e2)}->std::convertible_to<bool>; }
  && requires (span<vector_type const> const& vPoint) { { adaptor_type::box_of_points(vPoint)}->std::convertible_to<box_type>; }
  && requires (span<box_type const> const& vBox) { { adaptor_type::box_of_boxes(vBox)}->std::convertible_to<box_type>; }
  ;


  // Adaptors

  template <dim_type nDimension, typename vector_type, typename box_type, typename geometry_type = double>
  struct AdaptorGeneralBasics
  {
    static constexpr geometry_type& point_comp(vector_type& pt, dim_type iDimension) noexcept { return pt[iDimension]; }
    static constexpr geometry_type const& point_comp_c(vector_type const& pt, dim_type iDimension) noexcept { return pt[iDimension]; }

    static constexpr vector_type& box_min(box_type& box) noexcept { return box.Min; }
    static constexpr vector_type& box_max(box_type& box) noexcept { return box.Max; }
    static constexpr vector_type const& box_min_c(box_type const& box) noexcept { return box.Min; }
    static constexpr vector_type const& box_max_c(box_type const& box) noexcept { return box.Max; }
  };


  template <dim_type nDimension, typename vector_type, typename box_type, typename adaptor_basics_type, typename geometry_type = double>
  struct AdaptorGeneralBase : adaptor_basics_type
  {
    using base = adaptor_basics_type;
    static_assert(AdaptorBasicsConcept<base, vector_type, box_type, geometry_type>);

    static constexpr geometry_type size2(vector_type const& pt) noexcept
    {
      auto d2 = geometry_type{ 0 };
      for (dim_type iDim = 0; iDim < nDimension; ++iDim)
      {
        autoc d = base::point_comp_c(pt, iDim);
        d2 += d * d;
      }
      return d2;
    }

    static constexpr geometry_type size(vector_type const& pt) noexcept
    {
      return sqrt(size2(pt));
    }

    static constexpr vector_type add(vector_type const& ptL, vector_type const& ptR) noexcept
    {
      auto pt = vector_type{};
      for (dim_type iDim = 0; iDim < nDimension; ++iDim)
        base::point_comp(pt, iDim) = base::point_comp_c(ptL, iDim) + base::point_comp_c(ptR, iDim);

      return pt;
    }

    static constexpr vector_type subtract(vector_type const& ptL, vector_type const& ptR) noexcept
    {
      auto pt = vector_type{};
      for (dim_type iDim = 0; iDim < nDimension; ++iDim)
        base::point_comp(pt, iDim) = base::point_comp_c(ptL, iDim) - base::point_comp_c(ptR, iDim);

      return pt;
    }

    static constexpr vector_type div(vector_type const& ptL, geometry_type const& rScalarR) noexcept
    {
      auto pt = vector_type{};
      for (dim_type iDim = 0; iDim < nDimension; ++iDim)
        base::point_comp(pt, iDim) = base::point_comp_c(ptL, iDim) / rScalarR;

      return pt;
    }

    static constexpr geometry_type distance(vector_type const& ptL, vector_type const& ptR) noexcept
    {
      return size(subtract(ptL, ptR));
    }

    static constexpr geometry_type distance2(vector_type const& ptL, vector_type const& ptR) noexcept
    {
      return size2(subtract(ptL, ptR));
    }

    static constexpr bool are_points_equal(vector_type const& ptL, vector_type const& ptR, geometry_type rAccuracy) noexcept
    {
      return distance2(ptL, ptR) <= rAccuracy * rAccuracy;
    }

    static constexpr bool does_box_contain_point(box_type const& box, vector_type const& pt) noexcept
    {
      for (dim_type iDimension = 0; iDimension < nDimension; ++iDimension)
        if (!(base::point_comp_c(base::box_min_c(box), iDimension) <= base::point_comp_c(pt, iDimension) && base::point_comp_c(pt, iDimension) <= base::point_comp_c(base::box_max_c(box), iDimension)))
          return false;

      return true;
    }

    static constexpr bool does_box_contain_point_strict(box_type const& box, vector_type const& pt) noexcept
    {
      for (dim_type iDimension = 0; iDimension < nDimension; ++iDimension)
        if (!(base::point_comp_c(base::box_min_c(box), iDimension) < base::point_comp_c(pt, iDimension) && base::point_comp_c(pt, iDimension) < base::point_comp_c(base::box_max_c(box), iDimension)))
          return false;

      return true;
    }


    static constexpr bool does_point_touch_box(box_type const& box, vector_type const& pt) noexcept
    {
      for (dim_type iDimension = 0; iDimension < nDimension; ++iDimension)
        if ((base::point_comp_c(base::box_min_c(box), iDimension) == base::point_comp_c(pt, iDimension)))
          return false;

      return true;
    }

    enum EBoxRelation : int8_t { Overlapped = -1, Adjecent = 0, Separated = 1 };
    static constexpr EBoxRelation box_relation(box_type const& e1, box_type const& e2) noexcept
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

    static constexpr bool are_boxes_overlapped_strict(box_type const& e1, box_type const& e2) noexcept
    {
      return box_relation(e1, e2) == EBoxRelation::Overlapped;
    }

    static constexpr bool are_boxes_overlapped(box_type const& e1, box_type const& e2, bool e1_must_contain_e2 = true, bool fOverlapPtTouchAllowed = false) noexcept
    {
      autoc e1_contains_e2min = does_box_contain_point(e1, base::box_min_c(e2));

      return e1_must_contain_e2
        ? e1_contains_e2min && does_box_contain_point(e1, base::box_max_c(e2))
        : fOverlapPtTouchAllowed
          ? e1_contains_e2min || does_box_contain_point(e1, base::box_max_c(e2)) || does_box_contain_point(e2, base::box_max_c(e1))
          : box_relation(e1, e2) == EBoxRelation::Overlapped
        ;
    }

    static inline box_type box_inverted_init() noexcept
    {
      auto ext = box_type{};
      auto& ptMin = base::box_min(ext);
      auto& ptMax = base::box_max(ext);

      autoce inf = std::numeric_limits<geometry_type>::infinity();
      LOOPIVDEP
      for (dim_type iDimension = 0; iDimension < nDimension; ++iDimension)
      {
        base::point_comp(ptMin, iDimension) = +inf;
        base::point_comp(ptMax, iDimension) = -inf;
      }

      return ext;
    }

    static box_type box_of_points(span<vector_type const> const& vPoint) noexcept
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

    static box_type box_of_boxes(span<box_type const> const& vExtent) noexcept
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

    static void move_box(box_type& box, vector_type const& vMove) noexcept
    {
      LOOPIVDEP
      for (dim_type iDimension = 0; iDimension < nDimension; ++iDimension)
      {
        base::point_comp(base::box_min(box), iDimension) += base::point_comp_c(vMove, iDimension);
        base::point_comp(base::box_max(box), iDimension) += base::point_comp_c(vMove, iDimension);
      }
    }

    static constexpr std::optional<double> is_ray_hit(box_type const& box, vector_type const& rayBasePoint, vector_type const& rayHeading) noexcept
    {
      if (does_box_contain_point(box, rayBasePoint))
        return 0.0;

      autoc& ptBoxMin = base::box_min_c(box);
      autoc& ptBoxMax = base::box_max_c(box);

      autoce inf = std::numeric_limits<double>::infinity();

      auto aRMinMax = array<array<double, nDimension>, 2>();
      for (dim_type iDimension = 0; iDimension < nDimension; ++iDimension)
      {
        autoc hComp = base::point_comp_c(rayHeading, iDimension);
        if (hComp == 0)
        {
          if (base::point_comp_c(ptBoxMax, iDimension) < base::point_comp_c(rayBasePoint, iDimension))
            return std::nullopt;

          if (base::point_comp_c(ptBoxMin, iDimension) > base::point_comp_c(rayBasePoint, iDimension))
            return std::nullopt;

          aRMinMax[0][iDimension] = -inf;
          aRMinMax[1][iDimension] = +inf;
          continue;
        }

        aRMinMax[0][iDimension] = (base::point_comp_c(hComp > 0.0 ? ptBoxMin : ptBoxMax, iDimension) - base::point_comp_c(rayBasePoint, iDimension)) / hComp;
        aRMinMax[1][iDimension] = (base::point_comp_c(hComp < 0.0 ? ptBoxMin : ptBoxMax, iDimension) - base::point_comp_c(rayBasePoint, iDimension)) / hComp;
      }

      autoc rMin = *std::ranges::max_element(aRMinMax[0]);
      autoc rMax = *std::ranges::min_element(aRMinMax[1]);
      if (rMin > rMax || rMax < 0.0)
        return std::nullopt;

      return rMin < 0 ? rMax : rMin;
    }
  };


  template<dim_type nDimension, typename vector_type, typename box_type, typename geometry_type = double>
  using AdaptorGeneral = AdaptorGeneralBase<nDimension, vector_type, box_type, AdaptorGeneralBasics<nDimension, vector_type, box_type, geometry_type>, geometry_type>;


  template<size_t N> using bitset_arithmetic = bitset<N>;

  template<size_t N>
  bitset_arithmetic<N> operator+ (bitset_arithmetic<N> const& lhs, bitset_arithmetic<N> const& rhs) noexcept
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
  bitset_arithmetic<N> operator / (bitset_arithmetic<N> const& dividend, bitset_arithmetic<N> const& divisor) noexcept
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
  template<dim_type nDimension, typename vector_type_, typename box_type_, typename adaptor_type = AdaptorGeneral<nDimension, vector_type_, box_type_, double>, typename geometry_type_ = double>
  class OrthoTreeBase
  {
    static_assert(0 < nDimension && nDimension < 64);

  protected:
    // Max number of children
    static autoce m_nChild = pow_ce(2, nDimension);

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
    using geometry_type = geometry_type_;
    using vector_type = vector_type_;
    using box_type = box_type_;

    using AD = adaptor_type;
    static_assert(AdaptorConcept<AD, vector_type, box_type, geometry_type>);

  protected:

    // Type system determined maximal depth.
    static autoce m_nDepthMaxTheoretical = depth_type((CHAR_BIT * sizeof(morton_node_id_type) - 1/*sentinal bit*/) / nDimension);

  public:

    class Node
    {
    private:
      vector<morton_node_id_type> m_children;

    public: // Public members
      vector<entity_id_type> vid = {};
      box_type box = {};

    public:
      constexpr void AddChild(morton_node_id_type_cref kChild) noexcept { m_children.emplace_back(kChild); }
      constexpr void AddChildInOrder(morton_node_id_type_cref kChild) noexcept
      {
        auto it = std::end(m_children);
        if constexpr (is_linear_tree)
          it = std::lower_bound(m_children.begin(), m_children.end(), kChild);
        else
          it = std::lower_bound(m_children.begin(), m_children.end(), kChild, bitset_arithmetic_compare{});

        if (it != m_children.end() && *it == kChild)
          return;

        m_children.insert(it, kChild);
      }

      constexpr bool HasChild(morton_node_id_type_cref kChild) const  noexcept
      {
        if constexpr (is_linear_tree)
          return std::ranges::binary_search(m_children, kChild);
        else
          return std::ranges::binary_search(m_children, kChild, bitset_arithmetic_compare{});
      }

      constexpr bool IsChildNodeEnabled(child_id_type idChild) const noexcept
      { 
        autoc midChild = morton_node_id_type(idChild);
        return std::find_if(std::begin(m_children), std::end(m_children), [midChild](autoc& kChild) { return (kChild & midChild) == midChild; });
      }

      constexpr void DisableChild(morton_node_id_type_cref kChild) noexcept
      {
        auto it = std::end(m_children);
        if constexpr (is_linear_tree)
          it = std::lower_bound(m_children.begin(), m_children.end(), kChild);
        else
          it = std::lower_bound(m_children.begin(), m_children.end(), kChild, bitset_arithmetic_compare{});

        if (it == std::end(m_children))
          return;

        m_children.erase(it);
      }

      constexpr bool IsAnyChildExist() const noexcept { return !m_children.empty(); }
      constexpr vector<morton_node_id_type> const& GetChildren() const noexcept { return m_children; }
    };


  protected: // Aid struct to partitioning and distance ordering

    struct ItemDistance
    {
      geometry_type distance;
      auto operator <=> (ItemDistance const& rhs) const = default;
    };

    struct EntityDistance : ItemDistance 
    { 
      entity_id_type id;
      auto operator <=> (EntityDistance const& rhs) const = default;
    };
    struct BoxDistance : ItemDistance { morton_node_id_type kNode; Node const& node; };

    template<typename data_type>
    using container_type = typename std::conditional<is_linear_tree, unordered_map<morton_node_id_type, data_type>, map<morton_node_id_type, data_type, bitset_arithmetic_compare>>::type;

  protected: // Member variables
    container_type<Node> m_nodes;
    box_type m_box = {};
    depth_type m_nDepthMax = {};
    grid_id_type m_nRasterResolutionMax = {};
    grid_id_type m_idSlotMax = {};
    max_element_type m_nElementMax = 11;
    double m_rVolume = {};
    array<double, nDimension> m_aRasterizer;
    array<double, nDimension> m_aBoxSize;
    array<double, nDimension> m_aMinPoint;

    

  protected: // Aid functions

    template<size_t N>
    static inline child_id_type convertMortonIdToChildId(bitset_arithmetic<N> const& bs) noexcept
    {
      assert(bs <= bitset_arithmetic<N>(std::numeric_limits<size_t>::max()));
      return bs.to_ullong();
    }
    static constexpr child_id_type convertMortonIdToChildId(uint64_t morton) noexcept { return morton; }


    static constexpr vector<entity_id_type> generatePointId(size_t n) noexcept
    {
      auto vidPoint = vector<entity_id_type>(n);
      std::iota(std::begin(vidPoint), std::end(vidPoint), 0);
      return vidPoint;
    }


  protected: // Grid functions

    static constexpr std::tuple<array<double, nDimension>, array<double, nDimension>> getGridRasterizer(vector_type const& p0, vector_type const& p1, grid_id_type n_divide) noexcept
    {
      auto ret = std::tuple<array<double, nDimension>, array<double, nDimension>>{};
      auto& [aRasterizer, aBoxSize] = ret;
      autoc rn_divide = static_cast<double>(n_divide);
      for (dim_type iDimension = 0; iDimension < nDimension; ++iDimension)
      {
        aBoxSize[iDimension] = static_cast<double>(adaptor_type::point_comp_c(p1, iDimension) - adaptor_type::point_comp_c(p0, iDimension));
        aRasterizer[iDimension] = aBoxSize[iDimension] == 0 ? 1.0 : (rn_divide / aBoxSize[iDimension]);
      }

      return ret;
    }


    constexpr array<grid_id_type, nDimension> getGridIdPoint(vector_type const& pe) const noexcept
    {
      auto aid = array<grid_id_type, nDimension>{};
      for (dim_type iDimension = 0; iDimension < nDimension; ++iDimension)
      {
        autoc local_comp = adaptor_type::point_comp_c(pe, iDimension) - adaptor_type::point_comp_c(adaptor_type::box_min_c(this->m_box), iDimension);
        auto raster_id = static_cast<double>(local_comp) * this->m_aRasterizer[iDimension];
        aid[iDimension] = std::min<grid_id_type>(this->m_idSlotMax, static_cast<grid_id_type>(raster_id));
      }
      return aid;
    }


    constexpr array<array<grid_id_type, nDimension>, 2> getGridIdBox(box_type const& box) const noexcept
    {
      autoc& p0 = AD::box_min_c(m_box);

      auto aid = array<array<grid_id_type, nDimension>, 2>{};
      for (dim_type iDimension = 0; iDimension < nDimension; ++iDimension)
      {
        autoc ridMin = static_cast<double>(adaptor_type::point_comp_c(adaptor_type::box_min_c(box), iDimension) - adaptor_type::point_comp_c(p0, iDimension)) * m_aRasterizer[iDimension];
        autoc ridMax = static_cast<double>(adaptor_type::point_comp_c(adaptor_type::box_max_c(box), iDimension) - adaptor_type::point_comp_c(p0, iDimension)) * m_aRasterizer[iDimension];

        if (ridMin < 1.0)
          aid[0][iDimension] = 0;
        else if (ridMin > m_idSlotMax)
          aid[0][iDimension] = m_idSlotMax;
        else
          aid[0][iDimension] = static_cast<grid_id_type>(ridMin);


        if (ridMax < 1.0)
          aid[1][iDimension] = 0;
        else if (ridMax > m_idSlotMax)
          aid[1][iDimension] = m_idSlotMax;
        else if (ridMin != ridMax && floor(ridMax) == ridMax)
          aid[1][iDimension] = static_cast<grid_id_type>(ridMax) - 1;
        else
          aid[1][iDimension] = static_cast<grid_id_type>(ridMax);

      }
      return aid;
    }


    inline Node& createChild(Node& nodeParent, child_id_type iChild, morton_node_id_type_cref kChild) noexcept
    {
      assert(iChild < this->m_nChild);
      nodeParent.AddChild(kChild);

      auto& nodeChild = m_nodes[kChild];
      if constexpr (std::is_integral_v<geometry_type>)
      {
        std::array<double, nDimension> ptNodeMin = this->m_aMinPoint, ptNodeMax;

        autoc nDepth = this->GetDepth(kChild);
        auto mask = morton_node_id_type{ 1 } << (nDepth * nDimension - 1);

        auto rScale = 1.0;
        for (depth_type iDepth = 0; iDepth < nDepth; ++iDepth)
        {
          rScale *= 0.5;
          for (dim_type iDimension = nDimension; iDimension > 0; --iDimension)
          {
            bool const isGreater = (kChild & mask);
            ptNodeMin[iDimension - 1] += isGreater * this->m_aBoxSize[iDimension - 1] * rScale;
            mask >>= 1;
          }
        }

        LOOPIVDEP
        for (dim_type iDimension = 0; iDimension < nDimension; ++iDimension)
          ptNodeMax[iDimension] = ptNodeMin[iDimension] + this->m_aBoxSize[iDimension] * rScale;

        LOOPIVDEP
        for (dim_type iDimension = 0; iDimension < nDimension; ++iDimension)
        {
          AD::point_comp(AD::box_min(nodeChild.box), iDimension) = static_cast<geometry_type>(ptNodeMin[iDimension]);
          AD::point_comp(AD::box_max(nodeChild.box), iDimension) = static_cast<geometry_type>(ptNodeMax[iDimension]);
        }
      }
      else
      {
        LOOPIVDEP
        for (dim_type iDimension = 0; iDimension < nDimension; ++iDimension)
        {
          autoc fGreater = ((child_id_type{ 1 } << iDimension) & iChild) > 0;
          AD::point_comp(AD::box_min(nodeChild.box), iDimension) =
            fGreater * (AD::point_comp_c(AD::box_max_c(nodeParent.box), iDimension) + AD::point_comp_c(AD::box_min_c(nodeParent.box), iDimension)) * geometry_type{ 0.5 } +
            (!fGreater) * AD::point_comp_c(AD::box_min_c(nodeParent.box), iDimension);

          AD::point_comp(AD::box_max(nodeChild.box), iDimension) =
            fGreater * AD::point_comp_c(AD::box_max_c(nodeParent.box), iDimension) +
            (!fGreater) * ((AD::point_comp_c(AD::box_max_c(nodeParent.box), iDimension) + AD::point_comp_c(AD::box_min_c(nodeParent.box), iDimension)) * geometry_type{ 0.5 });
        }
      }
      return nodeChild;
    }


    constexpr morton_grid_id_type getLocationId(vector_type const& pt) const noexcept
    {
      return MortonEncode(this->getGridIdPoint(pt));
    }


    bool isEveryItemIdUnique() const noexcept
    {
      auto ids = vector<entity_id_type>();
      ids.reserve(100);
      std::ranges::for_each(m_nodes, [&](auto& node)
      {
        ids.insert(end(ids), begin(node.second.vid), end(node.second.vid));
      });

      std::ranges::sort(ids);
      autoc itEndUnique = std::unique(begin(ids), end(ids));
      return itEndUnique == end(ids);
    }

    template<bool bCheckUniqness>
    bool insert(morton_node_id_type_cref kNode, morton_node_id_type_cref kNodeSmallest, entity_id_type id, bool fInsertToLeaf) noexcept
    {
      if (kNode == kNodeSmallest)
      {
        cont_at(this->m_nodes, kNode).vid.emplace_back(id);
        if constexpr (bCheckUniqness)
          assert(this->isEveryItemIdUnique()); // Assert means: index is already added. Wrong input!
        return true;
      }

      if (fInsertToLeaf)
      {
        auto& nodeNew = this->m_nodes[kNode];
        nodeNew.vid.emplace_back(id);
        nodeNew.box = this->CalculateExtent(kNode);

        // Create all child between the new (kNode) and the smallest existing one (kNodeSmallest)
        auto kNodeParent = kNode;
        do
        {
          auto kNodeChild = kNodeParent;
          kNodeParent >>= nDimension;
          assert(IsValidKey(kNodeParent));
          auto& nodeParent = this->m_nodes[kNodeParent];
          nodeParent.AddChildInOrder(kNodeChild);
          nodeParent.box = this->CalculateExtent(kNodeParent);
        } while (kNodeParent != kNodeSmallest);
      }
      else
      {
        autoc itNode = this->m_nodes.find(kNodeSmallest);
        if (itNode->second.IsAnyChildExist())
        {
          autoc nDepth = this->GetDepth(kNodeSmallest);
          autoc kNodeChild = kNode << (nDimension * (this->m_nDepthMax - nDepth - 1));
          autoc iChild = getChildPartOfLocation(kNodeChild);
          auto& nodeChild = this->createChild(itNode->second, iChild, kNodeChild);
          nodeChild.vid.emplace_back(id);
        }
        else
          itNode->second.vid.emplace_back(id);
      }

      if constexpr (bCheckUniqness)
        assert(this->isEveryItemIdUnique()); // Assert means: index is already added. Wrong input!

      return true;
    }

    template<typename data_type = Node>
    static void reserveContainer(map<morton_node_id_type, data_type, bitset_arithmetic_compare>&, size_t) noexcept {};
    
    template<typename data_type = Node>
    static void reserveContainer(unordered_map<morton_node_id_type, data_type>& m, size_t n) noexcept { m.reserve(n); };

  public: // Static aid functions

    static constexpr size_t EstimateNodeNumber(size_t nElement, depth_type nDepthMax, max_element_type nElementMax) noexcept
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
      autoc nDepthEstimated = std::min(nDepthMax, static_cast<depth_type>(ceil((log2f(nElementInNodeAvg) + 1.0) / static_cast<float>(nDimension))));
      if (nDepthEstimated * nDimension < 64)
        return static_cast<size_t>(rMult * (1 << nDepthEstimated * nDimension));

      return static_cast<size_t>(rMult * nElementInNodeAvg);
    }


    static inline depth_type EstimateMaxDepth(size_t nElement, max_element_type nElementMax) noexcept
    {
      if (nElement < nElementMax)
        return 2;

      autoc nLeaf = nElement / nElementMax;
      // nLeaf = (2^nDepth)^nDimension
      return std::clamp(static_cast<depth_type>(std::log2(nLeaf) / static_cast<double>(nDimension)), depth_type(2), depth_type(10));
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

    static constexpr bool IsValidKey(uint64_t key) noexcept { return key; }

    template<size_t N>
    static inline bool IsValidKey(bitset_arithmetic<N> const& key) noexcept { return !key.none(); }

    static depth_type GetDepth(morton_node_id_type key) noexcept
    {
      // Keep shifting off three bits at a time, increasing depth counter
      for (depth_type d = 0; IsValidKey(key); ++d, key >>= nDimension)
        if (key == 1) // If only sentinel bit remains, exit with node depth
          return d;

      assert(false); // Bad key
      return 0;
    }

    static inline morton_node_id_type RemoveSentinelBit(morton_node_id_type_cref key, std::optional<depth_type> const& onDepth = std::nullopt) noexcept
    {
      autoc nDepth = onDepth.has_value() ? *onDepth : GetDepth(key);
      return key - (morton_node_id_type{ 1 } << nDepth);
    }


  private: // Morton aid functions

    static inline child_id_type getChildPartOfLocation(morton_node_id_type_cref key) noexcept
    {
      if constexpr (is_linear_tree)
      {
        autoce maskLastBits1 = (morton_node_id_type{ 1 } << nDimension) - 1;
        return convertMortonIdToChildId(key & maskLastBits1);
      }
      else
      {
        auto idChild = morton_node_id_type{};
        for (dim_type iDim = 0; iDim < nDimension; ++iDim)
          idChild[iDim] = key[iDim];

        return convertMortonIdToChildId(idChild);
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
      n = (n ^ (n << 8)) & 0x0300f00f; // (2)
      n = (n ^ (n << 4)) & 0x030c30c3; // (3)
      n = (n ^ (n << 2)) & 0x09249249; // (4)
      return std::is_same<morton_grid_id_type, bitset_arithmetic<nDimension>>::value ? morton_grid_id_type(n) : static_cast<morton_grid_id_type>(n);
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
      return std::is_same<morton_grid_id_type, bitset_arithmetic<nDimension>>::value ? morton_grid_id_type(n) : static_cast<morton_grid_id_type>(n);
    }

  public:
    static inline morton_grid_id_type MortonEncode(array<grid_id_type, nDimension> const& aidGrid) noexcept
    {
      if constexpr (nDimension == 1)
        return morton_grid_id_type(aidGrid[0]);
      else if constexpr (nDimension == 2)
        return (part1By1(aidGrid[1]) << 1) + part1By1(aidGrid[0]);
      else if constexpr (nDimension == 3)
        return (part1By2(aidGrid[2]) << 2) + (part1By2(aidGrid[1]) << 1) + part1By2(aidGrid[0]);
      else
      {
        auto msb = aidGrid[0];
        for (dim_type iDimension = 1; iDimension < nDimension; ++iDimension)
          msb |= aidGrid[iDimension];

        morton_grid_id_type id = 0;
        grid_id_type mask = 1;
        for (dim_type i = 0; msb; mask <<= 1, msb >>= 1, ++i)
        {
          LOOPIVDEP
          for (dim_type iDimension = 0; iDimension < nDimension; ++iDimension)
          {
            autoc shift = iDimension + i * nDimension;
            if constexpr (is_linear_tree)
              id |= (aidGrid[iDimension] & mask) << (shift - i);
            else
              id[shift] = aidGrid[iDimension] & mask;
          }
        }
        return id;
      }
    }

    static array<grid_id_type, nDimension> MortonDecode(morton_node_id_type_cref kNode, depth_type nDepthMax) noexcept
    {
      auto aidGrid = array<grid_id_type, nDimension>{};
      if constexpr (nDimension == 1)
        return { RemoveSentinelBit(kNode) };
      else
      {
        autoc nDepth = GetDepth(kNode);

        auto mask = morton_grid_id_type{ 1 };
        for (depth_type iDepth = nDepthMax - nDepth, shift = 0; iDepth < nDepthMax; ++iDepth)
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

    inline auto const& GetNodes() const noexcept { return m_nodes; }
    inline auto const& GetNode(morton_node_id_type_cref key) const noexcept { return cont_at(m_nodes, key); }
    inline auto const& GetBox() const noexcept { return m_box; }
    inline auto GetDepthMax() const noexcept { return m_nDepthMax; }
    inline auto GetResolutionMax() const noexcept { return m_nRasterResolutionMax; }


  public: // Main service functions

    // Alternative creation mode (instead of Create), Init then Insert items into leafs one by one. NOT RECOMMENDED.
    constexpr void Init(box_type const& box, depth_type nDepthMax, max_element_type nElementMax = 11) noexcept
    {
      assert(this->m_nodes.empty()); // To build/setup/create the tree, use the Create() [recommended] or Init() function. If an already builded tree is wanted to be reset, use the Reset() function before init.
      assert(nDepthMax > 1);
      assert(nDepthMax <= m_nDepthMaxTheoretical);
      assert(nDepthMax < std::numeric_limits<uint8_t>::max());
      assert(nElementMax > 1);
      assert(CHAR_BIT * sizeof(grid_id_type) >= m_nDepthMax);

      this->m_box = box;
      this->m_rVolume = 1.0;
      for (dim_type iDimension = 0; iDimension < nDimension; ++iDimension)
        this->m_rVolume *= AD::point_comp_c(AD::box_max_c(this->m_box), iDimension) - AD::point_comp_c(AD::box_min_c(this->m_box), iDimension);

      this->m_nDepthMax = nDepthMax;
      this->m_nRasterResolutionMax = static_cast<grid_id_type>(pow_ce(2, nDepthMax));
      this->m_idSlotMax = this->m_nRasterResolutionMax - 1;
      this->m_nElementMax = nElementMax;

      auto& nodeRoot = this->m_nodes[GetRootKey()];
      nodeRoot.box = box;
      tie(this->m_aRasterizer, this->m_aBoxSize) = this->getGridRasterizer(AD::box_min_c(this->m_box), AD::box_max_c(this->m_box), this->m_nRasterResolutionMax);

      LOOPIVDEP
      for (dim_type iDimension = 0; iDimension < nDimension; ++iDimension)
        this->m_aMinPoint[iDimension] = static_cast<double>(AD::point_comp_c(AD::box_min_c(this->m_box), iDimension));
    }


    using fnProcedure = std::function<void(morton_node_id_type_cref, Node const&)>;
    using fnProcedureUnconditional = std::function<void(morton_node_id_type_cref, Node const&, bool)>;
    using fnSelector = std::function<bool(morton_node_id_type_cref, Node const&)>;
    using fnSelectorUnconditional = std::function<bool(morton_node_id_type_cref, Node const&)>;


    // Visit nodes with special selection and procedure in breadth-first search order
    void VisitNodes(morton_node_id_type_cref kRoot, fnProcedure const& procedure, fnSelector const& selector) const noexcept
    {
      auto q = queue<morton_node_id_type>();
      for (q.push(kRoot); !q.empty(); q.pop())
      {
        autoc& key = q.front();
        autoc& node = cont_at(m_nodes, key);
        procedure(key, node);

        for (morton_node_id_type_cref kChild : node.GetChildren())
        {
          if (selector(kChild, cont_at(m_nodes, kChild)))
            q.push(kChild);
        }
      }
    }


    // Visit nodes with special selection and procedure in breadth-first search order
    inline void VisitNodes(morton_node_id_type_cref kRoot, fnProcedure const& procedure) const noexcept
    {
      VisitNodes(kRoot, procedure, [](morton_node_id_type_cref, Node const&){ return true; });
    }


    // Visit nodes with special selection and procedure and if unconditional selection is fulfilled descendants will not be test with selector
    void VisitNodes(morton_node_id_type_cref kRoot, fnProcedureUnconditional const& procedure, fnSelector const& selector, fnSelectorUnconditional const& selectorUnconditional) const noexcept
    {
      struct Search
      {
        morton_node_id_type key;
        Node const& pNode;
        depth_type nDepth;
        bool fUnconditional;
      };

      autoc nDepthRoot = GetDepth(kRoot);
      auto q = queue<Search>();
      for (q.push({ kRoot, cont_at(m_nodes, kRoot), nDepthRoot, false }); !q.empty(); q.pop())
      {
        autoc& item = q.front();
        procedure(item.key, item.pNode, item.fUnconditional);

        autoc nDepthChild = depth_type{ item.nDepth + 1 };
        for (morton_node_id_type kChild : item.pNode.GetChildren())
        {
          autoc& pNodeChild = cont_at(m_nodes, kChild);
          if (item.fUnconditional)
            q.push({ kChild, pNodeChild, nDepthChild, true });
          else if (selector(kChild, pNodeChild))
            q.push({ kChild, pNodeChild, nDepthChild, selectorUnconditional(kChild, pNodeChild) });
        }
      }
    }


    // Collect all item id, traversing the tree in breadth-first search order
    vector<entity_id_type> CollectAllIdInBFS(morton_node_id_type_cref kRoot = GetRootKey()) const noexcept
    {
      auto ids = vector<entity_id_type>();
      ids.reserve(m_nodes.size() * std::max<size_t>(2, m_nElementMax / 2));

      VisitNodes(kRoot, [&ids](morton_node_id_type_cref, autoc& node)
      { 
        ids.insert(std::end(ids), std::begin(node.vid), std::end(node.vid));
      });
      return ids;
    }


    // Update all element which are in the given hash-table. Elements will be erased if the replacement id is std::numeric_limits<entity_id_type>::max().
    template<bool bCheckUniqness = false>
    void UpdateIndexes(unordered_map<entity_id_type, entity_id_type> const& vIndexOldNew) noexcept
    {
      autoc itEnd = std::end(vIndexOldNew);
      std::ranges::for_each(m_nodes, [&](auto& node)
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

      if constexpr (bCheckUniqness)
        assert(isEveryItemIdUnique()); // Assert means: index replacements causes that multiple object has the same id. Wrong input!
    }


    // Calculate extent by box of the tree and the key of the node 
    box_type CalculateExtent(morton_node_id_type_cref keyNode) const noexcept
    {
      auto boxNode = box_type();
      auto& ptMinBoxNode = AD::box_min(boxNode);
      auto& ptMaxBoxNode = AD::box_max(boxNode);
      autoc& ptMinBoxRoot = AD::box_min_c(m_box);
      autoc& ptMaxBoxRoot = AD::box_max_c(m_box);

      ptMinBoxNode = ptMinBoxRoot;

      auto aSize = array<geometry_type, nDimension>();
      LOOPIVDEP
      for (dim_type iDimension = 0; iDimension < nDimension; ++iDimension)
        aSize[iDimension] = AD::point_comp_c(ptMaxBoxRoot, iDimension) - AD::point_comp_c(ptMinBoxRoot, iDimension);

      autoc nDepth = GetDepth(keyNode);
      autoc nRasterResolution = pow_ce(2, nDepth);
      autoc rMax = 1.0 / static_cast<double>(nRasterResolution);

      autoce one = morton_grid_id_type{ 1 };
      auto keyShifted = keyNode;// RemoveSentinelBit(key, nDepth);
      for (depth_type iDepth = 0; iDepth < nDepth; ++iDepth)
      {
        autoc r = rMax * (1 << iDepth);

        LOOPIVDEP
        for (dim_type iDimension = 0; iDimension < nDimension; ++iDimension)
        {
          autoc fApply = ((keyShifted >> iDimension) & one) > morton_grid_id_type{};
          AD::point_comp(ptMinBoxNode, iDimension) += static_cast<geometry_type>((aSize[iDimension] * r)) * fApply;
        }
        keyShifted >>= nDimension;
      }

      LOOPIVDEP
      for (dim_type iDimension = 0; iDimension < nDimension; ++iDimension)
        AD::point_comp(ptMaxBoxNode, iDimension) = AD::point_comp_c(ptMinBoxNode, iDimension) + static_cast<geometry_type>(aSize[iDimension] * rMax);

      return boxNode;
    }


    // Reset the tree
    void Reset() noexcept
    {
      m_nodes.clear();
      m_box = {};
      m_rVolume = 0.0;
      m_aRasterizer = {};
    }


    // Remove all elements and ids, except Root
    void Clear() noexcept
    {
      std::erase_if(m_nodes, [](autoc& p) { return p.first != GetRootKey(); });
      cont_at(m_nodes, GetRootKey()).vid.clear();
    }


    // Move the whole tree with a vector of the movement
    template<typename execution_policy_type = std::execution::unsequenced_policy>
    void Move(vector_type const& vMove) noexcept
    {
      auto ep = execution_policy_type{}; // GCC 11.3
      std::for_each(ep, std::begin(m_nodes), std::end(m_nodes), [&vMove](auto& pairKeyNode)
      {
        AD::move_box(pairKeyNode.second.box, vMove);
      });
      AD::move_box(this->m_box, vMove);
    }


    morton_node_id_type FindSmallestNodeKey(morton_node_id_type keySearch) const noexcept
    {
      for (; IsValidKey(keySearch); keySearch >>= nDimension)
        if (this->m_nodes.contains(keySearch))
          return keySearch;

      return morton_node_id_type{}; // Not found
    }

    morton_node_id_type Find(entity_id_type id) const noexcept
    {
      autoc it = find_if(this->m_nodes.begin(), this->m_nodes.end(), [id](autoc& keyAndNode)
      {
        return std::ranges::find(keyAndNode.second.vid, id) != end(keyAndNode.second.vid);
      });

      return it == this->m_nodes.end() ? 0 : it->first;
    }

  protected:

    template <dim_type iDimensionSet>
    static constexpr void constructGridIdRec(array<array<grid_id_type, 3>, nDimension> const& avidGridList, array<grid_id_type, nDimension>& aidGrid, vector<array<grid_id_type, nDimension>>& vidGrid, grid_id_type nStep) noexcept
    {
      if constexpr (iDimensionSet == 0)
        vidGrid.emplace_back(aidGrid);
      else
      {
        autoc& [nGridMin, nGridBegin, nGridEnd] = avidGridList[iDimensionSet - 1];
        aidGrid[iDimensionSet - 1] = nGridMin;
        constructGridIdRec<iDimensionSet - 1>(avidGridList, aidGrid, vidGrid, nStep);
        for (auto idGrid = nGridBegin; idGrid < nGridEnd; ++idGrid)
        {
          aidGrid[iDimensionSet - 1] = idGrid * nStep;
          constructGridIdRec<iDimensionSet - 1>(avidGridList, aidGrid, vidGrid, nStep);
        }
      }
    }


    template<bool fIdCheck = false>
    void collectAllIdInDFS(Node const& nodeParent, vector<entity_id_type>& sidFound, entity_id_type idMin = 0) const noexcept
    {
      if constexpr (fIdCheck)
      {
        for (autoc id : nodeParent.vid)
          if (id > idMin)
            sidFound.emplace_back(id);
      }
      else
        sidFound.insert(std::end(sidFound), std::begin(nodeParent.vid), std::end(nodeParent.vid));

      for (morton_node_id_type_cref kChild : nodeParent.GetChildren())
        collectAllIdInDFS<fIdCheck>(this->GetNode(kChild), sidFound, idMin);
    }

    template<typename data_type, bool fRangeMustContain = false, bool fIdCheck = false>
    constexpr void rangeSearchCopy(box_type const& range, span<data_type const> const& vData, Node const& nodeParent, vector<entity_id_type>& sidFound, entity_id_type idMin = 0) const noexcept
    {
      for (autoc id : nodeParent.vid)
      {
        if constexpr (std::is_same<data_type, box_type>::value)
        {
          if constexpr (fIdCheck)
          {
            if (id <= idMin)
              continue;

            bool fAdd = false;
            if constexpr (fRangeMustContain)
              fAdd = AD::are_boxes_overlapped(range, vData[id], fRangeMustContain);
            else
              fAdd = AD::are_boxes_overlapped_strict(range, vData[id]);

            if (fAdd)
              sidFound.emplace_back(id);
          }
          else
          {
            bool fAdd = false;
            if constexpr (fRangeMustContain)
              fAdd = AD::are_boxes_overlapped(range, vData[id], fRangeMustContain);
            else
              fAdd = AD::are_boxes_overlapped_strict(range, vData[id]);

            if (fAdd)
              sidFound.emplace_back(id);
          }
        }
        else
        {
          if (AD::does_box_contain_point(range, vData[id]))
            sidFound.emplace_back(id);
        }
      }
    }


    template<typename data_type, bool fRangeMustContain = false, bool fIdCheck = false>
    void rangeSearch(box_type const& range, span<data_type const> const& vData, double rVolumeRange, double rVolumeParent, Node const& nodeParent, vector<entity_id_type>& sidFound, entity_id_type idMin = 0) const noexcept
    {
      rangeSearchCopy<data_type, fRangeMustContain, fIdCheck>(range, vData, nodeParent, sidFound, idMin);
      
      autoc rVolumeNode = rVolumeParent / this->m_nChild;
      for (morton_node_id_type_cref keyChild : nodeParent.GetChildren())
      {
        autoc& nodeChild = this->GetNode(keyChild);

        auto bOverlap = true;
        for (dim_type iDim = 0; iDim < nDimension && bOverlap; ++iDim)
        {
          autoc isUpperNodeInTheDimension = IsValidKey(keyChild & (morton_node_id_type{ 1 } << iDim));
          if (isUpperNodeInTheDimension)
            bOverlap &= AD::point_comp_c(AD::box_min_c(nodeChild.box), iDim) <= AD::point_comp_c(AD::box_max_c(range), iDim);
          else
            bOverlap &= AD::point_comp_c(AD::box_max_c(nodeChild.box), iDim) >= AD::point_comp_c(AD::box_min_c(range), iDim);
        }
        if (!bOverlap)
          continue;

        if (rVolumeRange >= rVolumeNode && AD::are_boxes_overlapped(range, nodeChild.box))
          collectAllIdInDFS<fIdCheck>(nodeChild, sidFound, idMin);
        else
          rangeSearch<data_type, fRangeMustContain, fIdCheck>(range, vData, rVolumeRange, rVolumeNode, nodeChild, sidFound, idMin);
      }
    }

    template<typename data_type, bool fRangeMustContain = false, bool fIdCheck = false, bool fLeafNodeContainsElementOnly = true, bool isBoxType = false>
    bool rangeSearchRoot(box_type const& range, span<data_type const> const& vData, vector<entity_id_type>& sidFound, entity_id_type idMin = 0) const noexcept
    {
      autoc nEntity = vData.size();
      if (AD::are_boxes_overlapped(range, this->m_box))
      {
        sidFound.resize(fIdCheck ? nEntity - idMin - 1 : nEntity);
        std::iota(std::begin(sidFound), std::end(sidFound), fIdCheck ? idMin + 1 : 0);
        return nEntity;
      }

      // If the range has zero volume, it could stuck at any node comparison with point/side touch. It is eliminated to work node bounding box independently.
      for (dim_type iDimension = 0; iDimension < nDimension; ++iDimension)
        if (AD::point_comp_c(AD::box_min_c(range), iDimension) >= AD::point_comp_c(AD::box_max_c(range), iDimension))
          return false;

      auto idLocationMin = morton_grid_id_type{};
      auto idLocationMax = morton_grid_id_type{};
      if constexpr (isBoxType)
      {
        autoc aid = this->getGridIdBox(range);
        idLocationMin = MortonEncode(aid[0]);
        idLocationMax = MortonEncode(aid[1]);
      }
      else
      {
        idLocationMin = MortonEncode(getGridIdPoint(AD::box_min_c(range)));
        idLocationMax = MortonEncode(getGridIdPoint(AD::box_max_c(range)));
      }

      auto nDepth = this->m_nDepthMax;
      for (auto flagDiffOfLocation = idLocationMin ^ idLocationMax; IsValidKey(flagDiffOfLocation); flagDiffOfLocation >>= nDimension, --nDepth)
        idLocationMin >>= nDimension;

      autoc keyRange = this->GetHash(nDepth, idLocationMin);
      auto keyNodeSmallest = this->FindSmallestNodeKey(keyRange);
      if (!IsValidKey(keyNodeSmallest))
        return false;

      auto rVolumeRange = 1.0;
      for (dim_type iDimension = 0; iDimension < nDimension; ++iDimension)
        rVolumeRange *= AD::point_comp_c(AD::box_max_c(range), iDimension) - AD::point_comp_c(AD::box_min_c(range), iDimension);

      autoc rVolumeNode = this->m_rVolume / static_cast<double>(1 << (nDimension * nDepth));

      autoc nidFoundEstimation = this->m_rVolume < 0.01 ? 10 : static_cast<size_t>((rVolumeRange * nEntity) / this->m_rVolume);
      sidFound.reserve(nidFoundEstimation);
      autoc& node = this->GetNode(keyNodeSmallest);
      rangeSearch<data_type, fRangeMustContain, fIdCheck>(range, vData, rVolumeRange, rVolumeNode, node, sidFound, idMin);

      if constexpr (!fLeafNodeContainsElementOnly)
      {
        for (keyNodeSmallest >>= nDimension; IsValidKey(keyNodeSmallest); keyNodeSmallest >>= nDimension)
          rangeSearchCopy<data_type, fRangeMustContain, fIdCheck>(range, vData, this->GetNode(keyNodeSmallest), sidFound, idMin);
      }

      return true;
    }


  public:

    void CollectAllIdInDFS(morton_grid_id_type_cref keyParent, vector<entity_id_type>& vItem) const noexcept
    {
      autoc& node = cont_at(this->m_nodes, keyParent);
      collectAllIdInDFS(node, vItem);
    }


    // Doubles the handled space relative to the root. iRootNew defines the relative location in the new space
    //TODO IMPLEMENT void Extend(morton_node_id_type_cref iRootNew = 0) {}
  };



  // OrthoTreePoint: Non-owning container which spatially organize point ids in N dimension space into a hash-table by Morton Z order.
  template<dim_type nDimension, typename vector_type, typename box_type, typename adaptor_type = AdaptorGeneral<nDimension, vector_type, box_type, double>, typename geometry_type = double>
  class OrthoTreePoint : public OrthoTreeBase<nDimension, vector_type, box_type, adaptor_type, geometry_type>
  {
  protected:
    using base = OrthoTreeBase<nDimension, vector_type, box_type, adaptor_type, geometry_type>;
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

    static constexpr max_element_type max_element_default = 21;

  protected: // Aid functions

    using LocationIterator = typename vector<std::pair<entity_id_type, morton_grid_id_type>>::iterator;
    void addNodes(Node& nodeParent, morton_node_id_type_cref kParent, LocationIterator& itEndPrev, LocationIterator const& itEnd, morton_grid_id_type_cref idLocationBegin, depth_type nDepthRemain) noexcept
    {
      autoc nElement = std::distance(itEndPrev, itEnd);
      if (nElement < this->m_nElementMax || nDepthRemain == 0)
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
        autoc idChildActual = base::convertMortonIdToChildId((itEndPrev->second - idLocationBegin) >> shift);
        autoc itEndActual = std::partition_point(itEndPrev, itEnd, [&](autoc& idPoint)
        {
          return idChildActual == base::convertMortonIdToChildId((idPoint.second - idLocationBegin) >> shift);
        });

        autoc mChildActual = morton_grid_id_type(idChildActual);
        morton_grid_id_type const kChild = flagParent | mChildActual;
        morton_grid_id_type const idLocationBeginChild = idLocationBegin + mChildActual * nLocationStep;

        auto& nodeChild = this->createChild(nodeParent, idChildActual, kChild);
        this->addNodes(nodeChild, kChild, itEndPrev, itEndActual, idLocationBeginChild, nDepthRemain);
      }
    }


  public: // Create

    // Ctors
    OrthoTreePoint() = default;
    OrthoTreePoint(span<vector_type const> const& vpt, depth_type nDepthMax, std::optional<box_type> const& oBoxSpace = std::nullopt, max_element_type nElementMaxInNode = max_element_default) noexcept
    {
      Create(*this, vpt, nDepthMax, oBoxSpace, nElementMaxInNode);
    }

    // Create
    template<typename execution_policy_type = std::execution::unsequenced_policy>
    static void Create(OrthoTreePoint& tree, span<vector_type const> const& vpt, depth_type nDepthMaxIn = 0, std::optional<box_type> const& oBoxSpace = std::nullopt, max_element_type nElementMaxInNode = max_element_default) noexcept
    {
      autoc boxSpace = oBoxSpace.has_value() ? *oBoxSpace : AD::box_of_points(vpt);
      autoc n = vpt.size();

      autoc nDepthMax = nDepthMaxIn == 0 ? base::EstimateMaxDepth(n, nElementMaxInNode) : nDepthMaxIn;
      tree.Init(boxSpace, nDepthMax, nElementMaxInNode);
      base::reserveContainer(tree.m_nodes, base::EstimateNodeNumber(n, nDepthMax, nElementMaxInNode));
      if (vpt.empty())
        return;

      autoc kRoot = base::GetRootKey();
      auto& nodeRoot = cont_at(tree.m_nodes, kRoot);
      

      // Generate Morton location ids
      autoc vidPoint = base::generatePointId(n);
      auto aidLocation = vector<std::pair<entity_id_type, morton_grid_id_type>>(n);

      auto ept = execution_policy_type{}; // GCC 11.3 only accept in this form
      std::transform(ept, vpt.begin(), vpt.end(), vidPoint.begin(), aidLocation.begin(), [&](autoc& pt, autoc id) -> std::pair<entity_id_type, morton_grid_id_type>
      {
        return { id, tree.getLocationId(pt) };
      });

      auto eps = execution_policy_type{}; // GCC 11.3 only accept in this form
      std::sort(eps, std::begin(aidLocation), std::end(aidLocation), [&](autoc& idL, autoc& idR) { return idL.second < idR.second; });
      auto itBegin = std::begin(aidLocation);
      tree.addNodes(nodeRoot, kRoot, itBegin, std::end(aidLocation), morton_node_id_type{ 0 }, nDepthMax);
    }



  public: // Edit functions

    // Insert item into a node. If fInsertToLeaf is true: The smallest node will be chosen by the max depth. If fInsertToLeaf is false: The smallest existing level on the branch will be chosen.
    bool Insert(entity_id_type id, vector_type const& pt, bool fInsertToLeaf = false) noexcept
    {
      if (!AD::does_box_contain_point(this->m_box, pt))
        return false;

      autoc kNodeSmallest = FindSmallestNode(pt);
      if (!base::IsValidKey(kNodeSmallest))
        return false;

      autoc idLocation = this->getLocationId(pt);
      autoc kNode = this->GetHash(this->m_nDepthMax, idLocation);

      return this->template insert<true>(kNode, kNodeSmallest, id, fInsertToLeaf);
    }

    // Erase an id. Traverse all node if it is needed, which has major performance penalty.
    template<bool fReduceIds = true>
    constexpr bool EraseId(entity_id_type idErase) noexcept
    {
      autoc fErased = std::ranges::any_of(this->m_nodes, [&](auto& pairNode) { return erase(pairNode.second.vid, idErase); });
      if (!fErased)
        return false;

      if constexpr (fReduceIds)
      {
        std::ranges::for_each(this->m_nodes, [idErase](auto& pairNode)
        {
          for (auto& id : pairNode.second.vid)
            id -= idErase < id;
        });
      }

      return true;
    }

    // Erase id, aided with the original point
    template<bool fReduceIds = true>
    bool Erase(entity_id_type idErase, vector_type const& pt) noexcept
    {
      autoc kOld = FindSmallestNode(pt);
      if (!base::IsValidKey(kOld))
        return false; // old box is not in the handled space domain

      auto& vid = cont_at(this->m_nodes, kOld).vid;
      autoc itRemove = std::remove(std::begin(vid), std::end(vid), idErase);
      if (itRemove == end(vid))
        return false; // id was not registered previously.

      vid.erase(itRemove, vid.end());

      if constexpr (fReduceIds)
      {
        std::ranges::for_each(this->m_nodes, [idErase](auto& pairNode)
        {
          for (auto& id : pairNode.second.vid)
            id -= idErase < id;
        });
      }

      return true;
    }


    // Update id by the new point information
    bool Update(entity_id_type id, vector_type const& ptNew, bool fInsertToLeaf = false) noexcept
    {
      if (!AD::does_box_contain_point(this->m_box, ptNew))
        return false;

      if (!this->EraseId<false>(id))
        return false;

      return this->Insert(id, ptNew, fInsertToLeaf);
    }


    // Update id by the new point information and the erase part is aided by the old point geometry data
    bool Update(entity_id_type id, vector_type const& ptOld, vector_type const& ptNew, bool fInsertToLeaf = false) noexcept
    {
      if (!AD::does_box_contain_point(this->m_box, ptNew))
        return false;

      if (!this->Erase<false>(id, ptOld))
        return false;

      return this->Insert(id, ptNew, fInsertToLeaf);
    }


  public: // Search functions

    // Find smallest node which contains the box
    morton_node_id_type FindSmallestNode(vector_type const& pt) const noexcept
    {
      if (!AD::does_box_contain_point(this->m_box, pt))
        return morton_node_id_type{};

      autoc idLocation = this->getLocationId(pt);
      return this->FindSmallestNodeKey(this->GetHash(this->m_nDepthMax, idLocation));
    }
 
    bool Contains(vector_type const& pt, span<vector_type const> const& vpt, geometry_type rAccuracy) const noexcept
    {
      autoc kSmallestNode = this->FindSmallestNode(pt);
      if (!base::IsValidKey(kSmallestNode))
        return false;

      autoc& node = cont_at(this->m_nodes, kSmallestNode);
      return std::ranges::any_of(node.vid, [&](autoc& id) { return AD::are_points_equal(pt, vpt[id], rAccuracy); });
    }


    // Range search
    template<bool fLeafNodeContainsElementOnly = false>
    vector<entity_id_type> RangeSearch(box_type const& range, span<vector_type const> const& vpt) const noexcept
    {
      auto sidFound = vector<entity_id_type>();

      if (!this->template rangeSearchRoot<vector_type, false, false, fLeafNodeContainsElementOnly, false>(range, vpt, sidFound))
        return {};

      return sidFound;
    }


  private: // K Nearest Neighbor helpers

    static geometry_type getBoxWallDistanceMax(vector_type const& pt, box_type const& box) noexcept
    {
      autoc& ptMin = AD::box_min_c(box);
      autoc& ptMax = AD::box_max_c(box);

      auto vDist = vector<geometry_type>();
      vDist.reserve(nDimension);
      for (dim_type iDim = 0; iDim < nDimension; ++iDim)
      {
        autoc rDistActual = vDist.emplace_back(std::min(
          abs(AD::point_comp_c(pt, iDim) - AD::point_comp_c(ptMin, iDim)),
          abs(AD::point_comp_c(pt, iDim) - AD::point_comp_c(ptMax, iDim))
        ));

        if (rDistActual == 0)
          return 0.0;
      }

      return *std::min_element(begin(vDist), end(vDist));
    }


    static void createEntityDistance(Node const& node, vector_type const& pt, span<vector_type const> const& vpt, multiset<EntityDistance>& setEntity) noexcept
    {
      for (autoc id : node.vid)
        setEntity.insert({ { AD::distance(pt, vpt[id]) }, id });
    }

    static geometry_type getFarestDistance(multiset<EntityDistance>& setEntity, size_t k) noexcept
    {
      if (setEntity.size() < k)
        return std::numeric_limits<geometry_type>::infinity();

      return std::next(std::begin(setEntity), k - 1)->distance;
    }

    static vector<entity_id_type> convertEntityDistanceToList(multiset<EntityDistance>& setEntity, size_t k) noexcept
    {
      autoc nEntity = std::min(k, setEntity.size());
      auto vidEntity = vector<entity_id_type>(nEntity);
      std::transform(std::begin(setEntity), std::next(std::begin(setEntity), nEntity), std::begin(vidEntity), [](autoc& ed) { return ed.id; });
      return vidEntity;
    }

   public:

    // K Nearest Neighbor
    vector<entity_id_type> GetNearestNeighbors(vector_type const& pt, size_t k, span<vector_type const> const& vpt) const noexcept
    {
      auto setEntity = multiset<EntityDistance>();
      autoc kSmallestNode = FindSmallestNode(pt);
      if (base::IsValidKey(kSmallestNode))
      {
        autoc& nodeSmallest = cont_at(this->m_nodes, kSmallestNode);
        createEntityDistance(nodeSmallest, pt, vpt, setEntity);
        if (!nodeSmallest.IsAnyChildExist())
          if (getFarestDistance(setEntity, k) < getBoxWallDistanceMax(pt, nodeSmallest.box))
            return convertEntityDistanceToList(setEntity, k);
      }

      auto setNodeDist = multiset<BoxDistance>();
      std::ranges::for_each(this->m_nodes, [&](autoc& pairKeyNode)
      {
        autoc& [key, node] = pairKeyNode;
        if (node.vid.empty() || key == kSmallestNode)
          return;

        autoc& ptMin = AD::box_min_c(node.box);
        autoc& ptMax = AD::box_max_c(node.box);

        auto aDist = vector_type{};
        for (dim_type iDim = 0; iDim < nDimension; ++iDim)
        {
          autoc dMin = AD::point_comp_c(ptMin, iDim) - AD::point_comp_c(pt, iDim);
          autoc dMax = AD::point_comp_c(ptMax, iDim) - AD::point_comp_c(pt, iDim);

          // If pt projection in iDim is within min and max the wall distance should be calculated.
          AD::point_comp(aDist, iDim) = dMin * dMax < 0 ? 0 : std::min(abs(dMin), abs(dMax));
        }
        setNodeDist.insert({ { AD::size(aDist)}, key, node });
      });

      if (!setNodeDist.empty())
      {
        auto rLatestNodeDist = std::begin(setNodeDist)->distance;
        for (autoc& nodeDist : setNodeDist)
        {
          autoc n = setEntity.size();
          if (k <= n && rLatestNodeDist < nodeDist.distance)
            break;

          createEntityDistance(nodeDist.node, pt, vpt, setEntity);
          rLatestNodeDist = nodeDist.distance;
        }
      }

      return convertEntityDistanceToList(setEntity, k);
    }
  };



  // OrthoTreeBoundingBox: Non-owning container which spatially organize bounding box ids in N dimension space into a hash-table by Morton Z order. 
  // nSplitStrategyAdditionalDepth: if (nSplitStrategyAdditionalDepth > 0) Those items which are not fit in the child nodes may be stored in the children/grand-children instead of the parent.
  template<dim_type nDimension, typename vector_type, typename box_type, typename adaptor_type = AdaptorGeneral<nDimension, vector_type, box_type, double>, typename geometry_type = double, depth_type nSplitStrategyAdditionalDepth = 2>
  class OrthoTreeBoundingBox : public OrthoTreeBase<nDimension, vector_type, box_type, adaptor_type, geometry_type>
  {
  protected:
    using base = OrthoTreeBase<nDimension, vector_type, box_type, adaptor_type, geometry_type>;
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

    static constexpr max_element_type max_element_default = 21;

  private: // Aid functions

    struct Location
    {
      entity_id_type id;
      morton_grid_id_type idMin;
      depth_type depth;

      constexpr auto operator < (Location const& idLocationR) const
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

    using LocationContainer = vector<Location>;
    using LocationIterator = typename LocationContainer::iterator;

    static constexpr child_id_type getIdChildAtDepth(Location const& loc, depth_type depthCheck, morton_node_id_type_cref idLocationCurDepth) noexcept
    {
      assert(depthCheck <= loc.depth);
      autoc idGridCurDepth = loc.idMin >> ((loc.depth - depthCheck) * nDimension);
      return base::convertMortonIdToChildId(idGridCurDepth - idLocationCurDepth);
    }


    void addNodes(Node& nodeParent, morton_node_id_type_cref kParent, LocationIterator& itEndPrev, LocationIterator const& itEnd, morton_grid_id_type_cref idLocationBegin, depth_type nDepthRemain) noexcept
    {
      autoc nElement = std::distance(itEndPrev, itEnd);
      if (nElement < this->m_nElementMax || nDepthRemain == 0)
      {
        if (nElement == 0)
          return;

        nodeParent.vid.resize(nElement);
        std::transform(itEndPrev, itEnd, begin(nodeParent.vid), [](autoc& item) { return item.id; });
        itEndPrev = itEnd;
        return;
      }

      depth_type depthCheck = this->m_nDepthMax - nDepthRemain;
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
        autoc idChildActual = getIdChildAtDepth(*itEndPrev, depthCheck, idLocationCurDepth);
        autoc itEndActual = std::partition_point(itEndPrev, itEnd, [&](autoc& loc)
        {
          autoc idChild = getIdChildAtDepth(loc, depthCheck, idLocationCurDepth);
          return idChildActual == idChild;
        });

        autoc mChildActual = morton_grid_id_type(idChildActual);
        morton_grid_id_type const kChild = flagParent | mChildActual;
        morton_grid_id_type const idLocationBeginChild = idLocationBegin + mChildActual * nLocationStep;

        auto& nodeChild = this->createChild(nodeParent, idChildActual, kChild);
        this->addNodes(nodeChild, kChild, itEndPrev, itEndActual, idLocationBeginChild, nDepthRemain);
      }
    }


    void split(array<array<grid_id_type, nDimension>, 2> const& aidBoxGrid, size_t idLoc, LocationContainer& vLocation, LocationContainer * pvLocationAdditional) const noexcept
    {
      depth_type nDepth = vLocation[idLoc].depth + nSplitStrategyAdditionalDepth;
      if (nDepth > this->m_nDepthMax)
        nDepth = this->m_nDepthMax;

      autoc nDepthRemain = static_cast<depth_type>(this->m_nDepthMax - nDepth);
      autoc nStepGrid = static_cast<grid_id_type>(pow_ce(2, nDepthRemain));

      auto aMinGridList = array<array<grid_id_type, 3>, nDimension>{};
      uint64_t nBoxByGrid = 1;
      for (dim_type iDim = 0; iDim < nDimension; ++iDim)
      {
        grid_id_type const nGridSplitFirst = (aidBoxGrid[0][iDim] / nStepGrid) + grid_id_type{ 1 };
        grid_id_type const nGridSplitLast = (aidBoxGrid[1][iDim] / nStepGrid);
        grid_id_type const nMinGridList = (nGridSplitLast < nGridSplitFirst ? 0 : (nGridSplitLast - nGridSplitFirst + 1)) + 1;
        nBoxByGrid *= nMinGridList;
        if (nBoxByGrid >= this->m_nChild)
          return;

        aMinGridList[iDim] = { aidBoxGrid[0][iDim], nGridSplitFirst, nGridSplitLast + 1 };
      }

      auto vaidMinGrid = vector<array<grid_id_type, nDimension>>{};
      auto aidGrid = array<grid_id_type, nDimension>{};
      vaidMinGrid.reserve(nBoxByGrid);
      base::template constructGridIdRec<nDimension>(aMinGridList, aidGrid, vaidMinGrid, nStepGrid);
      
      autoc nBox = vaidMinGrid.size();
      autoc shift = nDepthRemain * nDimension;


      // First element into idLoc
      vLocation[idLoc].depth = nDepth;
      vLocation[idLoc].idMin = base::MortonEncode(vaidMinGrid[0]) >> shift;

      size_t nSize = 0;
      autoc nBoxAdd = nBox - 1;
      if (pvLocationAdditional)
      {
        pvLocationAdditional->resize(nBox - 1);
      }
      else
      {
        nSize = vLocation.size();
        vLocation.resize(nSize + nBoxAdd);
        pvLocationAdditional = &vLocation;
      }

      LOOPIVDEP
      for (size_t iBox = 0; iBox < nBoxAdd; ++iBox)
      {
        auto& loc = pvLocationAdditional->at(nSize + iBox);
        loc.id = idLoc;
        loc.depth = nDepth;
        loc.idMin = base::MortonEncode(vaidMinGrid[iBox + 1]) >> shift;
      }
    }


    void setLocation(box_type const& box, size_t idLoc, LocationContainer& vLocation, vector<LocationContainer>* pvvLocationAdditional = nullptr) const noexcept
    {
      autoc aidBoxGrid = this->getGridIdBox(box);

      auto& loc = vLocation[idLoc];
      loc.id = idLoc;
      loc.depth = this->m_nDepthMax;

      loc.idMin = base::MortonEncode(aidBoxGrid[0]);
      autoc idMax = base::MortonEncode(aidBoxGrid[1]);
      if (loc.idMin == idMax)
        return;

      autoc idMin = loc.idMin;
      for (auto flagDiffOfLocation = loc.idMin ^ idMax; base::IsValidKey(flagDiffOfLocation); flagDiffOfLocation >>= nDimension, --loc.depth)
        loc.idMin >>= nDimension;

      if constexpr (nSplitStrategyAdditionalDepth > 0)
      {
        autoc nDepthRemain = this->m_nDepthMax - loc.depth;
        if (base::IsValidKey((idMax - idMin) >> (nDepthRemain * nDimension - 1)))
          return; // all nodes are touched, it is leaved.

        this->split(aidBoxGrid, idLoc, vLocation, pvvLocationAdditional ? &pvvLocationAdditional->at(idLoc) : nullptr);
      }
    }

  public: // Create

    // Ctors
    OrthoTreeBoundingBox() = default;
    OrthoTreeBoundingBox(span<box_type const> const& vBox, depth_type nDepthMax, std::optional<box_type> const& oBoxSpace = std::nullopt, max_element_type nElementMaxInNode = max_element_default) noexcept
    {
      Create(*this, vBox, nDepthMax, oBoxSpace, nElementMaxInNode);
    }

    // Create
    template<typename execution_policy_type = std::execution::unsequenced_policy>
    static void Create(OrthoTreeBoundingBox& tree, span<box_type const> const& vBox, depth_type nDepthMaxIn = 0, std::optional<box_type> const& oBoxSpace = std::nullopt, max_element_type nElementMaxInNode = max_element_default) noexcept
    {
      autoc boxSpace = oBoxSpace.has_value() ? *oBoxSpace : AD::box_of_boxes(vBox);
      autoc n = vBox.size();
      autoc nDepthMax = nDepthMaxIn == 0 ? base::EstimateMaxDepth(n, nElementMaxInNode) : nDepthMaxIn;
      tree.Init(boxSpace, nDepthMax, nElementMaxInNode);

      base::reserveContainer(tree.m_nodes, base::EstimateNodeNumber(n, nDepthMax, nElementMaxInNode));
      if (n == 0)
        return;

      autoc kRoot = base::GetRootKey();
      auto& nodeRoot = cont_at(tree.m_nodes, kRoot);

      autoc vid = base::generatePointId(n);

      auto epf = execution_policy_type{}; // GCC 11.3
      auto aLocation = LocationContainer(n);
      aLocation.reserve(nSplitStrategyAdditionalDepth > 0 ? n * std::min<size_t>(10, base::m_nChild * nSplitStrategyAdditionalDepth) : n);
      if constexpr (nSplitStrategyAdditionalDepth == 0
        || std::is_same<execution_policy_type, std::execution::unsequenced_policy>::value
        || std::is_same<execution_policy_type, std::execution::sequenced_policy>::value
        )
      {
        std::for_each(epf, std::begin(vid), std::end(vid), [&tree, &vBox, &aLocation](autoc id)
        {
          tree.setLocation(vBox[id], id, aLocation);
        });
      }
      else
      {
        auto vAddtional = vector<LocationContainer>(n);
        std::for_each(epf, std::begin(vid), std::end(vid), [&tree, &vBox, &aLocation, &vAddtional](autoc id)
        {
          tree.setLocation(vBox[id], id, aLocation, &vAddtional);
        });

        auto vAddtionalSize = vector<size_t>(n);
        auto epe = execution_policy_type{};
        std::transform_exclusive_scan(epe, std::begin(vAddtional), std::end(vAddtional), std::begin(vAddtionalSize),
          n,
          std::plus<size_t>(),
          [](autoc& vAdd) { return vAdd.size(); }
        );

        aLocation.resize(vAddtionalSize.back() + vAddtional.back().size());
        auto epf2 = execution_policy_type{}; // GCC 11.3
        std::for_each(epf2, std::begin(vid), std::end(vid), [&aLocation, &vAddtionalSize, &vAddtional](autoc& id)
        {
          if (vAddtional[id].empty())
            return;

          std::copy(std::begin(vAddtional[id]), std::end(vAddtional[id]), std::next(std::begin(aLocation), vAddtionalSize[id]));
        });
      }

      auto eps = execution_policy_type{}; // GCC 11.3
      std::sort(eps, std::begin(aLocation), std::end(aLocation));

      auto itBegin = std::begin(aLocation);
      tree.addNodes(nodeRoot, kRoot, itBegin, std::end(aLocation), morton_node_id_type{ 0 }, nDepthMax);
      if constexpr (nSplitStrategyAdditionalDepth > 0)
      {
        // Eliminate duplicates. Not all sub-nodes will be created due to the nElementMaxInNode, which cause duplicates in the parent nodes.
        auto epsp = execution_policy_type{}; // GCC 11.3
        std::for_each(epsp, std::begin(tree.m_nodes), std::end(tree.m_nodes), [](auto& pairKeyNode)
        {
          auto& node = pairKeyNode.second;
          std::ranges::sort(node.vid);
          node.vid.erase(std::unique(std::begin(node.vid), std::end(node.vid)), std::end(node.vid));
        });
      }
    }


  public: // Edit functions

    // Find smallest node which contains the box by grid id description
    morton_node_id_type FindSmallestNode(array<array<grid_id_type, nDimension>, 2> const& aidGrid) const noexcept
    {
      auto idLocationMin = base::MortonEncode(aidGrid[0]);
      auto idLocationMax = base::MortonEncode(aidGrid[1]);

      auto nDepth = this->m_nDepthMax;
      for (auto flagDiffOfLocation = idLocationMin ^ idLocationMax; base::IsValidKey(flagDiffOfLocation); flagDiffOfLocation >>= nDimension, --nDepth)
        idLocationMin >>= nDimension;

      autoc itEnd = std::end(this->m_nodes);
      for (auto kSmallestNode = this->GetHash(nDepth, idLocationMin); base::IsValidKey(kSmallestNode); kSmallestNode >>= nDimension)
        if (this->m_nodes.find(kSmallestNode) != itEnd)
          return kSmallestNode;

      return morton_node_id_type{}; // Not found
    }


    // Find smallest node which contains the box
    morton_node_id_type FindSmallestNode(box_type const& box) const noexcept
    {
      if (!AD::are_boxes_overlapped(this->m_box, box))
        return morton_node_id_type{};

      autoc aidGrid = this->getGridIdBox(box);
      return FindSmallestNode(aidGrid);
    }


    // Insert item into a node. If fInsertToLeaf is true: The smallest node will be chosen by the max depth. If fInsertToLeaf is false: The smallest existing level on the branch will be chosen.
    bool Insert(entity_id_type id, box_type const& box, bool fInsertToLeaf = false) noexcept
    {
      if (!AD::are_boxes_overlapped(this->m_box, box))
        return false;

      autoc kNodeSmallest = FindSmallestNode(box);
      if (!base::IsValidKey(kNodeSmallest))
        return false; // new box is not in the handled space domain

      auto vLocation = vector<Location>(1);
      setLocation(box, 0, vLocation);

      for (autoc& loc : vLocation)
      {
        autoc kNode = this->GetHash(loc.depth, loc.idMin);
        if (!this->template insert<nSplitStrategyAdditionalDepth == 0>(kNode, kNodeSmallest, id, fInsertToLeaf))
          return false;
      }

      return true;
    }

  
  private: 
    bool doErase(morton_node_id_type_cref kNode, entity_id_type id) noexcept
    {
      auto& vid = cont_at(this->m_nodes, kNode).vid;
      autoc itRemove = std::remove(begin(vid), end(vid), id);
      if (itRemove == end(vid))
        return false; // id was not registered previously.

      vid.erase(itRemove, vid.end());
      return true;
    }


    template<depth_type nDepthRemainSet>
    bool doEraseRec(morton_node_id_type_cref kNode, entity_id_type id) noexcept
    {
      auto ret = this->doErase(kNode, id);
      if constexpr (nDepthRemainSet > 0)
      {
        autoc& node = cont_at(this->m_nodes, kNode);
        for (morton_node_id_type_cref kChild : node.GetChildren())
          ret |= doEraseRec<nDepthRemainSet - 1>(kChild, id);
      }
      return ret;
    }


  public:
    // Erase id, aided with the original bounding box
    template<bool fReduceIds = true>
    bool Erase(entity_id_type idErase, box_type const& box) noexcept
    {
      autoc kOld = FindSmallestNode(box);
      if (!base::IsValidKey(kOld))
        return false; // old box is not in the handled space domain

      if (doEraseRec<nSplitStrategyAdditionalDepth>(kOld, idErase))
      {
        if constexpr (fReduceIds)
          std::ranges::for_each(this->m_nodes, [&](auto& pairNode)
          { 
            for (auto& id : pairNode.second.vid)
              id -= idErase < id;
          });

        return true;
      }
      else
        return false;
    }


    // Erase an id. Traverse all node if it is needed, which has major performance penalty.
    template<bool fReduceIds = true>
    constexpr bool EraseId(entity_id_type idErase) noexcept
    {
      bool bErased = false;
      if constexpr (nSplitStrategyAdditionalDepth == 0)
        bErased = std::ranges::any_of(this->m_nodes, [&](auto& pairNode) { return erase(pairNode.second.vid, idErase); });
      else
        std::ranges::for_each(this->m_nodes, [&](auto& pairNode) { bErased |= erase(pairNode.second.vid, idErase) > 0; });

      if (!bErased)
        return false;

      if constexpr (fReduceIds)
        std::ranges::for_each(this->m_nodes, [&](auto& pairNode)
        {
          for (auto& id : pairNode.second.vid)
            id -= idErase < id;
        });

      return true;
    }


    // Update id by the new bounding box information
    bool Update(entity_id_type id, box_type const& boxNew, bool fInsertToLeaf = false) noexcept
    {
      if (!AD::are_boxes_overlapped(this->m_box, boxNew))
        return false;

      if (!this->EraseId<false>(id))
        return false;

      return this->Insert(id, boxNew, fInsertToLeaf);
    }


    // Update id by the new point information and the erase part is aided by the old bounding box geometry data
    bool Update(entity_id_type id, box_type const& boxOld, box_type const& boxNew, bool fInsertToLeaf = false) noexcept
    {
      if (!AD::are_boxes_overlapped(this->m_box, boxNew))
        return false;

      if constexpr (nSplitStrategyAdditionalDepth == 0)
        if (FindSmallestNode(boxOld) == FindSmallestNode(boxNew))
          return true;
 
      if (!this->Erase<false>(id, boxOld))
        return false; // id was not registered previously.

      return this->Insert(id, boxNew, fInsertToLeaf);
    }


  private:

    constexpr array<array<grid_id_type, nDimension>, 2> getGridIdPointEdge(vector_type const& point) const noexcept
    {
      autoc& p0 = AD::box_min_c(this->m_box);

      auto aid = array<array<grid_id_type, nDimension>, 2>{};
      for (dim_type iDimension = 0; iDimension < nDimension; ++iDimension)
      {
        autoc rid = static_cast<double>(adaptor_type::point_comp_c(point, iDimension) - adaptor_type::point_comp_c(p0, iDimension)) * this->m_aRasterizer[iDimension];
        aid[0][iDimension] = aid[1][iDimension] = static_cast<grid_id_type>(rid);
        aid[0][iDimension] -= (aid[0][iDimension] > 0) && (floor(rid) == rid);
      }
      return aid;
    }


    void pickSearch(vector_type const& ptPick, span<box_type const> const& vData, Node const& nodeParent, vector<entity_id_type>& vidFound) const noexcept
    {
      std::ranges::copy_if(nodeParent.vid, back_inserter(vidFound), [&](autoc id) { return AD::does_box_contain_point(vData[id], ptPick); });

      for (morton_node_id_type_cref keyChild : nodeParent.GetChildren())
      {
        autoc& nodeChild = this->GetNode(keyChild);

        if (!AD::does_box_contain_point(nodeChild.box, ptPick))
          continue;

        pickSearch(ptPick, vData, nodeChild, vidFound);
      }
    }


  public: // Search functions
    
    // Pick search
    vector<entity_id_type> PickSearch(vector_type const& ptPick, span<box_type const> const& vBox) const noexcept
    {
      auto vidFound = vector<entity_id_type>();
      if (!AD::does_box_contain_point(this->m_box, ptPick))
        return vidFound;

      vidFound.reserve(100);

      autoc itEnd = std::end(this->m_nodes);
      autoc aid = this->getGridIdPointEdge(ptPick);
      auto idLocation = base::MortonEncode(aid[0]);

      auto kNode = this->GetHash(this->m_nDepthMax, idLocation);
      if (aid[0] != aid[1]) // Pick point is on the nodes edge. It must check more nodes downward.
      {
        autoc idLocationMax = base::MortonEncode(aid[1]);
        auto nDepth = this->m_nDepthMax;
        for (auto flagDiffOfLocation = idLocation ^ idLocationMax; base::IsValidKey(flagDiffOfLocation); flagDiffOfLocation >>= nDimension, --nDepth)
          idLocation >>= nDimension;

        autoc keyRange = this->GetHash(nDepth, idLocation);
        kNode = this->FindSmallestNodeKey(keyRange);
        autoc itNode = this->m_nodes.find(kNode);
        if (itNode != itEnd)
          pickSearch(ptPick, vBox, itNode->second, vidFound);

        kNode >>= nDimension;
      }

      for (; base::IsValidKey(kNode); kNode >>= nDimension)
      {
        autoc itNode = this->m_nodes.find(kNode);
        if (itNode == itEnd)
          continue;

        std::ranges::copy_if(itNode->second.vid, back_inserter(vidFound), [&](autoc id) { return AD::does_box_contain_point(vBox[id], ptPick); });
      }
      
      return vidFound;
    }

    
    // Range search
    template<bool isFullyContained = true>
    vector<entity_id_type> RangeSearch(box_type const& range, span<box_type const> const& vBox) const noexcept
    {
      auto sidFound = vector<entity_id_type>();

      if (!this->template rangeSearchRoot<box_type, isFullyContained, false, false, true>(range, vBox, sidFound))
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
    static vector<std::pair<entity_id_type, entity_id_type>> CollisionDetection(OrthoTreeBoundingBox const& treeL, span<box_type const> const& vBoxL, OrthoTreeBoundingBox const& treeR, span<box_type const> const& vBoxR) noexcept
    {
      using NodeIterator = typename base::template container_type<Node>::const_iterator;
      struct NodeIteratorAndStatus { NodeIterator it; bool fTraversed; };
      using ParentIteratorArray = array<NodeIteratorAndStatus, 2>;

      enum : bool { Left, Right };

      auto vResult = vector<std::pair<entity_id_type, entity_id_type>>{};
      vResult.reserve(vBoxL.size() / 10);

      autoc kRoot = base::GetRootKey();
      autoc aTree = array{ &treeL, &treeR };

      auto q = queue<ParentIteratorArray>{};
      for (q.push({ NodeIteratorAndStatus{ treeL.m_nodes.find(kRoot), false }, NodeIteratorAndStatus{ treeR.m_nodes.find(kRoot), false } }); !q.empty(); q.pop())
      {
        autoc& aitNodeParent = q.front();

        // Check the current ascendant content 
        {
          for (autoc idL : aitNodeParent[Left].it->second.vid)
            for (autoc idR : aitNodeParent[Right].it->second.vid)
              if (AD::are_boxes_overlapped(vBoxL[idL], vBoxR[idR], false))
                vResult.emplace_back(idL, idR);
        }

        // Collect children
        auto avitChildNode = array<vector<NodeIteratorAndStatus>, 2>{};
        for (autoc id : { Left, Right })
        {
          autoc& [itNode, fTraversed] = aitNodeParent[id];
          if (fTraversed)
            continue;
          
          autoc vidChild = itNode->second.GetChildren();
          avitChildNode[id].resize(vidChild.size());
          std::ranges::transform(vidChild, begin(avitChildNode[id]), [&](morton_node_id_type_cref kChild) -> NodeIteratorAndStatus
          {
            return { aTree[id]->m_nodes.find(kChild), false };
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
              if (AD::are_boxes_overlapped(itNodeChildL.it->second.box, itNodeChildR.it->second.box, false))
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
    vector<std::pair<entity_id_type, entity_id_type>> CollisionDetection(span<box_type const> const& vBox, OrthoTreeBoundingBox const& treeOther, span<box_type const> const& vBoxOther) const noexcept
    {
      return CollisionDetection(*this, vBox, treeOther, vBoxOther);
    }

  public:
    // Collision detection between the stored elements from top to bottom logic
    template<typename execution_policy_type = std::execution::unsequenced_policy>
    vector<std::pair<entity_id_type, entity_id_type>> CollisionDetectionObsolete(span<box_type const> const& vBox) const noexcept
    {
      autoc nEntity = vBox.size();

      auto vidCheck = vector<entity_id_type>(nEntity);
      std::iota(std::begin(vidCheck), std::end(vidCheck), 0);

      auto vvidCollision = vector<vector<entity_id_type>>(vidCheck.size());
      auto ep = execution_policy_type{}; // GCC 11.3
      std::transform(ep, std::begin(vidCheck), std::end(vidCheck), std::begin(vvidCollision), [&vBox, this](autoc idCheck) -> vector<entity_id_type>
      {
        auto sidFound = vector<entity_id_type>();

        autoc nEntity = vBox.size();
        if (!this->template rangeSearchRoot<box_type, false, true, false, true>(vBox[idCheck], vBox, sidFound, idCheck))
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

  public:
    // Collision detection between the stored elements from bottom to top logic
    template<typename execution_policy_type = std::execution::unsequenced_policy>
    vector<std::pair<entity_id_type, entity_id_type>> CollisionDetection(span<box_type const> const& vBox) const noexcept
    {
      using CollisionDetectionContainer = vector<std::pair<entity_id_type, entity_id_type>>;

      autoc nEntity = vBox.size();
      auto vidCollision = CollisionDetectionContainer();
      vidCollision.reserve(vBox.size());


      // nSplitStrategyAdditionalDepth version of this algorithm needs a reverse map
      auto vReverseMap = vector<vector<morton_node_id_type>>(nEntity);
      if constexpr (nSplitStrategyAdditionalDepth > 0)
      {
        std::for_each(std::begin(this->m_nodes), std::end(this->m_nodes), [&vReverseMap](autoc& pairKeyNode)
        {
          autoc& [kNode, node] = pairKeyNode;
          for (autoc& id : node.vid)
            vReverseMap[id].emplace_back(kNode);
        });

        auto ep = execution_policy_type{}; // GCC 11.3
        std::for_each(ep, std::begin(vReverseMap), std::end(vReverseMap), [](auto& vKey)
        {
          if constexpr (base::is_linear_tree)
            std::ranges::sort(vKey);
          else
            std::ranges::sort(vKey, bitset_arithmetic_compare{});

        });
      }


      // Entities which contain all of the tree could slow the algorithm, so these are eliminated
      auto vidDepth0 = vector<entity_id_type>();
      {
        autoc& nodeRoot = this->GetNode(this->GetRootKey());
        for (autoc idEntity : nodeRoot.vid)
        {
          if (AD::are_boxes_overlapped(vBox[idEntity], this->m_box))
          {
            for (auto idEntityOther = idEntity + 1; idEntityOther < nEntity; ++idEntityOther)
              vidCollision.emplace_back(idEntity, idEntityOther);
          }
          else
            vidDepth0.emplace_back(idEntity);
        }
      }

      auto ep = execution_policy_type{}; // GCC 11.3

      // Collision detection node-by-node without duplication
      auto vvidCollisionByNode = vector<CollisionDetectionContainer>(this->m_nodes.size());
      std::transform(ep, std::begin(this->m_nodes), std::end(this->m_nodes), std::begin(vvidCollisionByNode), [&vBox, &vReverseMap, &vidDepth0, this](autoc& pairKeyNode) -> CollisionDetectionContainer
      {
        auto aidPair = CollisionDetectionContainer{};
        autoc& [keyNode, node] = pairKeyNode;

        autoc nDepthNode = this->GetDepth(keyNode);

        autoc pvid = nDepthNode == 0 ? &vidDepth0 : &node.vid;
        autoc& vid = *pvid;
        autoc nEntityNode = vid.size();

        aidPair.reserve(nEntityNode);

        // Collision detection with the parents
        if (nDepthNode > 0)
        {
          auto idDepthParent = nDepthNode - 1;
          auto idDepthDiff = depth_type(1);
          for (auto keyParent = keyNode >> nDimension; base::IsValidKey(keyParent); keyParent >>= nDimension, --idDepthParent, ++idDepthDiff)
          {
            autoc pvidParent = idDepthParent == 0 ? &vidDepth0 : &this->GetNode(keyParent).vid;

            if constexpr (nSplitStrategyAdditionalDepth == 0)
            {
              for (autoc iEntityNode : vid)
                for (autoc iEntityParent : *pvidParent)
                  if (AD::are_boxes_overlapped_strict(vBox[iEntityNode], vBox[iEntityParent]))
                    aidPair.emplace_back(iEntityNode, iEntityParent);
            }
            else
            {
              // nSplitStrategyAdditionalDepth: idEntity could occur in multiple node. This algorithm aims to check only the first occurrence's parents.

              auto vidCheckUp = vector<entity_id_type>{};
              vidCheckUp.reserve(nEntityNode);
              auto vidPairFromOtherBranch = unordered_map<entity_id_type, set<entity_id_type>>{};
              for (size_t iEntity = 0; iEntity < nEntityNode; ++iEntity)
              {
                autoc& vKeyEntity = vReverseMap[vid[iEntity]];
                autoc nKeyEntity = vKeyEntity.size();
                if (nKeyEntity == 1)
                  vidCheckUp.emplace_back(vid[iEntity]);
                else
                {
                  if (vKeyEntity[0] == keyNode)
                    vidCheckUp.emplace_back(vid[iEntity]);
                  else if (idDepthDiff <= nSplitStrategyAdditionalDepth)
                  {
                    auto vKeyEntitySameDepth = vector<morton_node_id_type>();
                    for (size_t iKeyEntity = 1; iKeyEntity < nKeyEntity; ++iKeyEntity)
                    {
                      // An earlier node is already check this level
                      {
                        auto& keyEntitySameDepth = vKeyEntitySameDepth.emplace_back(vKeyEntity[iKeyEntity - 1]);
                        autoc nDepthPrev = this->GetDepth(keyEntitySameDepth);
                        if (nDepthPrev > idDepthParent)
                          keyEntitySameDepth >>= nDimension * (nDepthPrev - idDepthParent);

                        if (keyEntitySameDepth == keyParent)
                          break;
                      }

                      if (vKeyEntity[iKeyEntity] != keyNode)
                        continue;

                      // On other branch splitted boxes could conflict already
                      for (autoc iEntityParent : *pvidParent)
                      {
                        autoc& vKeyEntityParent = vReverseMap[iEntityParent];
                        autoc nKeyEntityParent = vKeyEntityParent.size();

                        for (size_t iKeyEntityPrev = 0, iKeyEntityParent = 0; iKeyEntityPrev < iKeyEntity && iKeyEntityParent < nKeyEntityParent;)
                        {
                          if (vKeyEntityParent[iKeyEntityParent] == vKeyEntity[iKeyEntityPrev] || (vKeyEntityParent[iKeyEntityParent] == vKeyEntitySameDepth[iKeyEntityPrev]))
                          {
                            // Found a common an earlier common key
                            vidPairFromOtherBranch[vid[iEntity]].emplace(iEntityParent);
                            break;
                          }
                          else if (vKeyEntityParent[iKeyEntityParent] < vKeyEntity[iKeyEntityPrev])
                            ++iKeyEntityParent;
                          else
                            ++iKeyEntityPrev;
                        }
                      }

                      vidCheckUp.emplace_back(vid[iEntity]);
                      break;
                    }
                  }
                }
              }

              if (vidPairFromOtherBranch.empty())
              {
                for (autoc iEntityNode : vidCheckUp)
                  for (autoc iEntityParent : *pvidParent)
                    if (AD::are_boxes_overlapped_strict(vBox[iEntityNode], vBox[iEntityParent]))
                      aidPair.emplace_back(iEntityNode, iEntityParent);
              }
              else
              {
                for (autoc iEntityNode : vidCheckUp)
                {
                  autoc it = vidPairFromOtherBranch.find(iEntityNode);
                  autoc fThereAreFromOtherBranch = it != vidPairFromOtherBranch.end();

                  for (autoc iEntityParent : *pvidParent)
                  {
                    autoc fAlreadyContainted = fThereAreFromOtherBranch && it->second.contains(iEntityParent);
                    if (!fAlreadyContainted && AD::are_boxes_overlapped_strict(vBox[iEntityNode], vBox[iEntityParent]))
                      aidPair.emplace_back(iEntityNode, iEntityParent);
                  }
                }
              }
            }
          }
        }


        // Node inside collision detection
        if (nEntityNode > 1)
        {
          for (size_t iEntity = 0; iEntity < nEntityNode; ++iEntity)
          {
            autoc iEntityNode = vid[iEntity];
            autoc& vKeyEntity = vReverseMap[iEntityNode];
            autoc nKeyEntity = vKeyEntity.size();

            for (size_t jEntity = iEntity + 1; jEntity < nEntityNode; ++jEntity)
            {
              if constexpr (nSplitStrategyAdditionalDepth == 0)
              {
                if (AD::are_boxes_overlapped_strict(vBox[iEntityNode], vBox[vid[jEntity]]))
                  aidPair.emplace_back(iEntityNode, vid[jEntity]);
              }
              else
              {
                // Same entities could collide in other nodes, but only the first occurrence should be checked
                autoc& vKeyEntityJ = vReverseMap[vid[jEntity]];
                auto bIsTheFirstCollisionCheck = nKeyEntity == 1 || vKeyEntityJ.size() == 1;
                if (!bIsTheFirstCollisionCheck)
                {
                  for (size_t iKeyEntity = 0, jKeyEntity = 0; iKeyEntity < nKeyEntity; )
                  {
                    if      (vKeyEntityJ[jKeyEntity] == vKeyEntity[iKeyEntity]) { bIsTheFirstCollisionCheck = keyNode == vKeyEntity[iKeyEntity]; break; }
                    else if (vKeyEntityJ[jKeyEntity] < vKeyEntity[iKeyEntity])  ++jKeyEntity;
                    else                                                        ++iKeyEntity;
                  }
                }

                if (bIsTheFirstCollisionCheck)
                  if (AD::are_boxes_overlapped_strict(vBox[iEntityNode], vBox[vid[jEntity]]))
                    aidPair.emplace_back(iEntityNode, vid[jEntity]);
              }
            }
          }
        }

        return aidPair;
      });

      for (autoc& vidCollisionNode : vvidCollisionByNode)
        vidCollision.insert(vidCollision.end(), vidCollisionNode.begin(), vidCollisionNode.end());
      
      return vidCollision;
    }



  private:

    void getRayIntersectedAll(Node const& node, span<box_type const> const& vBox, vector_type const& rayBase, vector_type const& rayHeading, geometry_type rMaxDistance, vector<EntityDistance>& vdidOut) const noexcept
    {
      autoc oIsHit = AD::is_ray_hit(node.box, rayBase, rayHeading);
      if (!oIsHit)
        return;

      for (autoc id : node.vid)
      {
        autoc oDist = AD::is_ray_hit(vBox[id], rayBase, rayHeading);
        if (oDist && (rMaxDistance == 0 || oDist.value() <= rMaxDistance))
          vdidOut.push_back({ { oDist.value() }, id });
      }

      for (autoc kChild : node.GetChildren())
        getRayIntersectedAll(cont_at(this->m_nodes, kChild), vBox, rayBase, rayHeading, rMaxDistance, vdidOut);
    }


    void getRayIntersectedFirst(Node const& node, span<box_type const> const& vBox, vector_type const& rayBase, vector_type const& rayHeading, multiset<EntityDistance>& vidOut) const noexcept
    {
      autoc rLastDistance = vidOut.empty() ? std::numeric_limits<double>::infinity() : static_cast<double>(vidOut.rbegin()->distance);
      for (autoc id : node.vid)
      {
        autoc oDist = AD::is_ray_hit(vBox[id], rayBase, rayHeading);
        if (!oDist)
          continue;

        if (*oDist > rLastDistance)
          continue;

        vidOut.insert({ { *oDist }, id });
      }

      auto msNode = multiset<BoxDistance>();
      for (autoc kChild : node.GetChildren())
      {
        autoc& nodeChild = cont_at(this->m_nodes, kChild);
        autoc oDist = AD::is_ray_hit(nodeChild.box, rayBase, rayHeading);
        if (!oDist)
          continue;

        if (*oDist > rLastDistance)
          continue;

        msNode.insert({ { static_cast<geometry_type>(oDist.value()) }, kChild, nodeChild });
      }
      
      for (autoc& nodeData : msNode)
        getRayIntersectedFirst(nodeData.node, vBox, rayBase, rayHeading, vidOut);
      
    }


  public:

    // Get all box which is intersected by the ray in order
    vector<entity_id_type> RayIntersectedAll(vector_type const& rayBasePoint, vector_type const& rayHeading, span<box_type const> const& vBox, geometry_type rMaxDistance = 0) const noexcept
    {
      auto vdid = vector<EntityDistance>();
      vdid.reserve(20);
      getRayIntersectedAll(cont_at(this->m_nodes, base::GetRootKey()), vBox, rayBasePoint, rayHeading, rMaxDistance, vdid);

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
    std::optional<entity_id_type> RayIntersectedFirst(vector_type const& rayBase, vector_type const& rayHeading, span<box_type const> const& vBox) const noexcept
    {
      autoc& node = cont_at(this->m_nodes, base::GetRootKey());
      autoc oDist = AD::is_ray_hit(node.box, rayBase, rayHeading);
      if (!oDist)
        return std::nullopt;

      auto vid = multiset<EntityDistance>();
      getRayIntersectedFirst(node, vBox, rayBase, rayHeading, vid);
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
  template<size_t nDimension, uint32_t nSplitStrategyAdditionalDepth = 2> 
  using TreeBoxND = OrthoTree::OrthoTreeBoundingBox<nDimension, OrthoTree::PointND<nDimension>, OrthoTree::BoundingBoxND<nDimension>, AdaptorGeneral<nDimension, OrthoTree::PointND<nDimension>, OrthoTree::BoundingBoxND<nDimension>>, double, nSplitStrategyAdditionalDepth>;

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
