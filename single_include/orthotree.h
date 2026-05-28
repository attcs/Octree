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

/*
 * OrthoTree - Single Header Edition
 * Amalgamated version containing Octree, BVH, and Serialization modules.
 * Generated automatically.
 */

#pragma once

// --- Standard Library Dependencies ---
#include <algorithm>
#include <array>
#include <bit>
#include <bitset>
#include <cassert>
#include <climits>
#include <cmath>
#include <compare>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <execution>
#include <forward_list>
#include <functional>
#include <iostream>
#include <iterator>
#include <limits>
#include <list>
#include <map>
#include <memory>
#include <memory_resource>
#include <numeric>
#include <optional>
#include <queue>
#include <ranges>
#include <set>
#include <span>
#include <stack>
#include <stdexcept>
#include <string>
#include <thread>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <variant>
#include <vector>
#include <version>


#ifndef ORTHOTREE__OCTREE_H_INCLUDED
#define ORTHOTREE__OCTREE_H_INCLUDED
#endif

#if defined(ORTHOTREE__USE_PMR) || defined(_MSC_VER)
#ifndef ORTHOTREE__DISABLE_PMR
#define ORTHOTREE_IS_PMR_USED
#endif // !ORTHOTREE__DISABLE_PMR
#endif


namespace OrthoTree
{
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
    template<std::size_t N>
    int bit_width(bitset_arithmetic<N> const& val) noexcept
    {
      for (uint32_t i = N; i > 0; --i)
        if (val[i - 1])
          return i;

      return 0;
    }

    template<std::unsigned_integral T>
    constexpr int bit_width(T val) noexcept
    {
      return std::bit_width(val);
    }
  } // namespace detail

} // namespace OrthoTree


namespace OrthoTree
{
#ifdef ORTHOTREE_INDEX_T__SIZE_T
  using index_t = std::size_t;
#else
#ifdef ORTHOTREE_INDEX_T__UINT_FAST32_T
  using index_t = std::uint_fast32_t;
#else
#ifdef ORTHOTREE_INDEX_T__INT
  using index_t = int;
#else
  using index_t = std::uint32_t;
#endif // ORTHOTREE_INDEX_INT
#endif // ORTHOTREE_INDEX_T__UINT_FAST32_T
#endif // ORTHOTREE_INDEX_T__SIZE_T

#ifdef ORTHOTREE__LARGE_DATASET
  using serialized_size_t = std::uint64_t;
#else
  using serialized_size_t = std::uint32_t;
#endif

  // Type of the dimension
  using dim_t = uint32_t;

  // Type of depth
  using depth_t = uint32_t;
  constexpr depth_t INVALID_DEPTH = 31;
  constexpr depth_t MAX_DEPTH_ID = 30;

  // Grid id
  using GridID = uint32_t;
  constexpr GridID INVALID_GRIDID = std::numeric_limits<GridID>::max();

  enum class LogicalOperator
  {
    And,
    Or
  };

  // Enum of entity geometry type
  enum class GeometryType
  {
    Point,
    Box,
    // Mixed // Not supported yet
  };

  enum class RangeSearchMode
  {
    Inside,
    Overlap
  };


  // Enum of relation with Planes
  enum class PlaneRelation : char
  {
    Negative,
    Hit,
    Positive
  };

  enum class InsertionMode
  {
    Balanced,
    LowestLeaf,
    ExistingLeaf
  };

  namespace ExecutionTags
  {
    // Sequential execution tag
    struct Sequential
    {};

    // Parallel execution tag
    struct Parallel
    {};
  } // namespace ExecutionTags

  using SeqExec = ExecutionTags::Sequential;
  using ParExec = ExecutionTags::Parallel;
  auto constexpr SEQ_EXEC = SeqExec{};
  auto constexpr PAR_EXEC = ParExec{};


  namespace detail
  {
    template<typename TVector>
    struct BoundingBoxMinPointAndSize
    {
      TVector minPoint;
      TVector size;
    };

    template<typename TBegin, typename TLength>
    struct Segment
    {
      using Begin = TBegin;
      using Length = TLength;

      TBegin begin;
      TLength length;

      constexpr Segment() noexcept
      : begin(0)
      , length(0)
      {}
      constexpr Segment(TBegin b, TLength l) noexcept
      : begin(b)
      , length(l)
      {}
    };

    struct NodeStorage256
    {
      using NodeSegmentIndex = uint8_t;
      using EntitySegment = Segment<uint8_t, uint8_t>;

      std::vector<NodeSegmentIndex> nodeChildSegmentBegins;
      std::vector<EntitySegment> nodeEntitySegment;
    };

    struct NodeStorage65536
    {
      using NodeSegmentIndex = uint16_t;
      using EntitySegment = Segment<uint16_t, uint16_t>;

      std::vector<NodeSegmentIndex> nodeChildSegmentBegins;
      std::vector<EntitySegment> nodeEntitySegment;
    };

    struct NodeStorageGeneral
    {
      using NodeSegmentIndex = uint32_t;
#ifdef ORTHOTREE__LARGE_DATASET
      using EntitySegment = Segment<uint64_t, uint64_t>;
#else
      using EntitySegment = Segment<uint32_t, uint32_t>;
#endif
      std::vector<NodeSegmentIndex> nodeChildSegmentBegins;
      std::vector<EntitySegment> nodeEntitySegment;
    };

    template<typename TVector>
    struct NodeGeometryData
    {
      TVector minPoint;
      TVector size;
    };
  } // namespace detail
} // namespace OrthoTree


#if defined(__cpp_lib_unreachable) && (__cpp_lib_unreachable >= 202202L)
#define ORTHOTREE_UNREACHABLE() std::unreachable()
#elif defined(_MSC_VER)
#define ORTHOTREE_UNREACHABLE() __assume(0)
#elif defined(__GNUC__) || defined(__clang__)
#define ORTHOTREE_UNREACHABLE() __builtin_unreachable()
#else
#define ORTHOTREE_UNREACHABLE() std::abort()
#endif


#if (defined(__BMI2__) || defined(__AVX2__)) && (defined(_M_X64) || defined(__x86_64__) || defined(__amd64__))
#ifdef __has_include
#if __has_include(<immintrin.h>)
#include <immintrin.h>
#define ORTHOTREE_BMI2_PDEP_AVAILABLE 1
#endif
#endif
#endif

#if defined(__clang__)
#define ORTHOTREE_LOOPIVDEP
#elif defined(__INTEL_COMPILER)
#define ORTHOTREE_LOOPIVDEP _Pragma("ivdep")
#elif defined(__GNUC__)
#define ORTHOTREE_LOOPIVDEP _Pragma("GCC ivdep")
#elif defined(_MSC_VER)
#define ORTHOTREE_LOOPIVDEP _Pragma("loop(ivdep)")
#else
#define ORTHOTREE_LOOPIVDEP
#endif

#ifndef CRASH
#define CRASH_UNDEF
#define CRASH(errorMessage)        \
  do                               \
  {                                \
    assert(false && errorMessage); \
    std::terminate();              \
  } while (0)
#endif // !CRASH

#ifndef ORTHOTREE_CRASH_IF
#define CRASH_IF_UNDEF
#define ORTHOTREE_CRASH_IF(cond, errorMessage) \
  do                                           \
  {                                            \
    if (cond)                                  \
    {                                          \
      CRASH(errorMessage);                     \
    }                                          \
  } while (0)
#endif // !ORTHOTREE_CRASH_IF

#ifdef __cpp_lib_execution
#define EXEC_POL_DEF(e) \
  (void)execMode;       \
  std::conditional_t<std::is_same_v<TExecMode, ExecutionTags::Parallel>, std::execution::parallel_unsequenced_policy, std::execution::unsequenced_policy> constexpr e
#define EXEC_POL_ADD(e) e,
#else
#define EXEC_POL_DEF(e)
#define EXEC_POL_ADD(e)
#endif


#ifdef __cpp_lib_constexpr_inplace_vector
#include <inplace_vector>
#else
#endif

namespace OrthoTree::detail
{
#ifdef __cpp_lib_constexpr_inplace_vector
  template<typename T, std::size_t N>
  using inplace_vector = std::inplace_vector<T, N>;

#else

  template<typename T, std::size_t N>
  class inplace_vector
  {
  private:
    using container = std::array<T, N>;

  public:
    using value_type = typename container::value_type;
    using reference = T&;
    using const_reference = const T&;
    using size_type = typename std::size_t;
    using iterator = container::iterator;
    using const_iterator = container::const_iterator;

  public:
    constexpr inplace_vector() = default;

    template<typename... TVals>
    constexpr T& emplace_back(TVals&&... value) noexcept
    {
      assert(m_size < N);
      m_stack[m_size] = T(std::forward<TVals>(value)...);
      return m_stack[m_size++];
    }


    constexpr void push_back(const T& value) noexcept
    {
      assert(m_size < N);
      m_stack[m_size] = value;
      ++m_size;
    }

    constexpr void push_back(T&& value) noexcept
    {
      assert(m_size < N);
      m_stack[m_size] = std::move(value);
      ++m_size;
    }

    constexpr void pop_back() noexcept
    {
      assert(m_size > 0);
      --m_size;
    }

    constexpr reference operator[](std::size_t index) noexcept { return m_stack[index]; }
    constexpr const_reference operator[](std::size_t index) const noexcept { return m_stack[index]; }

    constexpr reference front() noexcept { return m_stack.front(); }
    constexpr const_reference front() const noexcept { return m_stack.front(); }

    constexpr reference back() noexcept { return m_stack[m_size - 1]; }
    constexpr const_reference back() const noexcept { return m_stack[m_size - 1]; }

    constexpr T* data() noexcept { return m_stack.data(); }
    constexpr const T* data() const noexcept { return m_stack.data(); }

    constexpr std::size_t size() const noexcept { return m_size; }
    constexpr bool empty() const noexcept { return m_size == 0; }
    constexpr std::size_t capacity() const noexcept { return N; }

    constexpr iterator insert(const_iterator whereIt, T const& val) noexcept
    {
      assert(m_size < N);
      auto non_const_it = begin() + (whereIt - begin());
      std::copy_backward(non_const_it, end(), end() + 1);
      *non_const_it = val;
      ++m_size;
      return non_const_it;
    }

    constexpr iterator insert(const_iterator whereIt, T&& val) noexcept
    {
      assert(m_size < N);
      auto non_const_it = begin() + (whereIt - begin());
      std::copy_backward(non_const_it, end(), end() + 1);
      *non_const_it = std::move(val);
      ++m_size;
      return non_const_it;
    }

    constexpr iterator erase(const_iterator it) noexcept
    {
      assert(m_size > 0);
      auto non_const_it = begin() + (it - begin());
      if (non_const_it >= begin() && non_const_it < end())
      {
        std::copy(non_const_it + 1, end(), non_const_it);
        --m_size;
        return non_const_it;
      }
      return end();
    }

    constexpr void clear() noexcept { m_size = 0; }

    constexpr iterator begin() noexcept { return m_stack.begin(); }
    constexpr iterator end() noexcept { return m_stack.begin() + m_size; }

    constexpr const_iterator begin() const noexcept { return m_stack.begin(); }
    constexpr const_iterator end() const noexcept { return m_stack.begin() + m_size; }

    constexpr const_iterator cbegin() const noexcept { return m_stack.begin(); }
    constexpr const_iterator cend() const noexcept { return m_stack.begin() + m_size; }

  private:
    container m_stack;
    std::size_t m_size = 0;
  };
#endif
} // namespace OrthoTree::detail


namespace OrthoTree
{
  // Adapter concepts

  template<class TAdapter, dim_t DIMENSION_NO_, typename TVector, typename TBox, typename TRay, typename TPlane, typename TScalar, typename TFloatScalar>
  concept BaseGeometryAdapterConcept =
    // --- required nested types ---
    requires {
      typename TAdapter::Scalar;
      typename TAdapter::FloatScalar;
      typename TAdapter::Vector;
      typename TAdapter::Box;
      typename TAdapter::Ray;
      typename TAdapter::Plane;
    }

    // --- exact type bindings ---
    && std::same_as<typename TAdapter::Scalar, TScalar> && std::same_as<typename TAdapter::FloatScalar, TFloatScalar> &&
    std::same_as<typename TAdapter::Vector, TVector> && std::same_as<typename TAdapter::Box, TBox> && std::same_as<typename TAdapter::Ray, TRay> &&
    std::same_as<typename TAdapter::Plane, TPlane> &&
    std::is_floating_point_v<typename TAdapter::FloatScalar>

    // --- required static constexpr values ---
    &&
    requires {
      { TAdapter::DIMENSION_NO } -> std::convertible_to<dim_t>;
      { TAdapter::BASE_TOLERANCE } -> std::convertible_to<TFloatScalar>;
    } &&
    (TAdapter::DIMENSION_NO == DIMENSION_NO_)

    // --- point access ---
    &&
    requires(TVector& p, TVector const& cp, dim_t d, TScalar v) {
      { TAdapter::GetPointC(cp, d) } -> std::convertible_to<TScalar>;
      { TAdapter::SetPointC(p, d, v) } -> std::same_as<void>;
    }

    // --- box access ---
    &&
    requires(TBox& b, TBox const& cb, dim_t d, TScalar v) {
      { TAdapter::GetBoxMinC(cb, d) } -> std::convertible_to<TScalar>;
      { TAdapter::GetBoxMaxC(cb, d) } -> std::convertible_to<TScalar>;
      { TAdapter::SetBoxMinC(b, d, v) } -> std::same_as<void>;
      { TAdapter::SetBoxMaxC(b, d, v) } -> std::same_as<void>;
    }

    // --- ray access ---
    &&
    requires(TRay const& r) {
      { TAdapter::GetRayOrigin(r) } -> std::convertible_to<TVector>;
      { TAdapter::GetRayDirection(r) } -> std::convertible_to<TVector>;
    }

    // --- plane access ---
    && requires(TPlane const& p) {
         { TAdapter::GetPlaneNormal(p) } -> std::convertible_to<TVector>;
         { TAdapter::GetPlaneOrigoDistance(p) } -> std::convertible_to<TScalar>;
       };


  template<class TAdapter, dim_t DIMENSION_NO, typename TVector, typename TBox, typename TRay, typename TPlane, typename TScalar, typename TFloatScalar>
  concept GeometryAdapterConcept = requires {
    requires BaseGeometryAdapterConcept<TAdapter, DIMENSION_NO, TVector, TBox, TRay, TPlane, TScalar, TFloatScalar>;
  } && requires(TBox const& box, TVector const& point, TFloatScalar tolerance) {
    { TAdapter::DoesBoxContainPoint(box, point, tolerance) } -> std::convertible_to<bool>;
  } && requires(TBox const& e1, TBox const& e2, bool e1_must_contain_e2, bool fOverlapPtTouchAllowed, TFloatScalar tolerance) {
    { TAdapter::AreBoxesOverlapped(e1, e2, e1_must_contain_e2, fOverlapPtTouchAllowed, tolerance) } -> std::convertible_to<bool>;
  } && requires(TBox const& e1, TBox const& e2, TFloatScalar tolerance) {
    { TAdapter::AreBoxesOverlappedStrict(e1, e2, tolerance) } -> std::convertible_to<bool>;
  } && requires(TVector const& box, TScalar distanceOfOrigo, TVector const& planeNormal, TFloatScalar tolerance) {
    { TAdapter::GetPointPlaneRelation(box, distanceOfOrigo, planeNormal, tolerance) } -> std::convertible_to<PlaneRelation>;
  };
} // namespace OrthoTree


// Optional third-party support
#if defined(CEREAL_CEREAL_HPP_)
#define ORTHOTREE_SERIALIZATION_CEREAL_ENABLED
#endif

#if defined(BOOST_SERIALIZATION_NVP_HPP)
#include <boost/serialization/nvp.hpp>
#define ORTHOTREE_SERIALIZATION_BOOST_ENABLED
#endif


#if defined(BOOST_ARCHIVE_DETAIL_INTERFACE_IARCHIVE_HPP) || defined(BOOST_ARCHIVE_DETAIL_INTERFACE_OARCHIVE_HPP)
#define ORTHOTREE_SERIALIZATION_BOOST_ARCHIVE_AVAILABLE
#endif

namespace cereal
{
  namespace detail
  {
    class OutputArchiveBase;
    class InputArchiveBase;
  } // namespace detail
} // namespace cereal

namespace boost
{
  namespace archive
  {
    namespace detail
    {
      template<class Archive>
      class interface_iarchive;

      template<class Archive>
      class interface_oarchive;
    } // namespace detail
  } // namespace archive
} // namespace boost

namespace OrthoTree
{
  // --- Versioning ---
  // Default version is 0.
  // Can be specialized for specific types or picked up from class members.
  template<typename T, typename = void>
  struct version
  {
    static constexpr uint32_t value = 0;
  };

  // Trait to pick up version from class if it exists
  template<typename T>
  struct version<T, std::void_t<decltype(T::SERIALIZED_VERSION_ID)>>
  {
    static constexpr uint32_t value = T::SERIALIZED_VERSION_ID;
  };

  template<typename T>
  inline constexpr uint32_t version_v = version<T>::value;


  // --- Archive Identification ---
  template<typename T>
  struct is_orthotree_archive : std::false_type
  {};

  template<typename T>
  inline constexpr bool is_orthotree_archive_v = is_orthotree_archive<T>::value;

  template<typename T>
  struct is_stl_serialization_enabled : std::false_type
  {};

  template<typename T>
  inline constexpr bool is_stl_serialization_enabled_v = is_stl_serialization_enabled<T>::value;


  namespace detail
  {
    template<typename T>
    struct is_cereal_archive
    : std::bool_constant<
        std::is_base_of_v<::cereal::detail::InputArchiveBase, std::decay_t<T>> || std::is_base_of_v<::cereal::detail::OutputArchiveBase, std::decay_t<T>>>
    {};

    template<typename T>
    inline constexpr bool is_cereal_archive_v = is_cereal_archive<T>::value;

#ifdef ORTHOTREE_SERIALIZATION_BOOST_ARCHIVE_AVAILABLE
    template<typename T>
    struct is_boost_archive : std::bool_constant<
                                std::is_base_of_v<::boost::archive::detail::interface_iarchive<std::decay_t<T>>, std::decay_t<T>> ||
                                std::is_base_of_v<::boost::archive::detail::interface_oarchive<std::decay_t<T>>, std::decay_t<T>>>
    {};
#else
    template<typename T>
    struct is_boost_archive : std::false_type
    {};
#endif

    template<typename T>
    inline constexpr bool is_boost_archive_v = is_boost_archive<T>::value;
  } // namespace detail


  // --- Factory for non-default constructible types ---
  template<typename TArchive, typename T>
  struct load_construct
  {
    static T load(TArchive& ar)
    {
      if constexpr (std::is_default_constructible_v<T>)
      {
        T val;
        ar & val;
        return val;
      }
      else
      {
        static_assert(std::is_default_constructible_v<T>, "Type T is not default constructible. Please specialize OrthoTree::load_construct for your type.");
        return T{};
      }
    }
  };

  // --- Archive Traits Helpers ---
  template<typename TArchive, typename = void>
  struct archive_traits
  {
    static bool is_loading(TArchive const& ar) { return ar.is_loading(); }
  };

  template<typename TArchive>
  struct archive_traits<TArchive, std::void_t<typename TArchive::is_loading>>
  {
    static bool is_loading(TArchive const&) { return TArchive::is_loading::value; }
  };

  template<typename TArchive>
  bool is_loading_archive(TArchive const& ar)
  {
    return archive_traits<TArchive>::is_loading(ar);
  }


  // --- NVP Identification ---
  template<typename T>
  struct is_nvp : std::false_type
  {};

  template<typename T>
  inline constexpr bool is_nvp_v = is_nvp<std::remove_cvref_t<T>>::value;


} // namespace OrthoTree


// Forward declarations to help the compiler parse the bridge with if constexpr
#ifndef ORTHOTREE_SERIALIZATION_CEREAL_ENABLED
namespace cereal
{
  template<class T>
  struct NameValuePair;
  template<class T>
  NameValuePair<T> make_nvp(const char*, T&);
  template<class T>
  struct SizeTag;
  template<class T>
  SizeTag<T> make_size_tag(T&);
} // namespace cereal
#endif

#ifndef ORTHOTREE_SERIALIZATION_BOOST_ENABLED
namespace boost
{
  namespace serialization
  {
    template<class T>
    class nvp;
    template<class T>
    const nvp<T> make_nvp(const char*, T&) noexcept;
  } // namespace serialization
} // namespace boost
#endif

namespace OrthoTree
{
  template<typename T>
  struct NameValuePair
  {
    const char* name;
    T& value;

    constexpr NameValuePair(const char* n, T& v) noexcept
    : name(n)
    , value(v)
    {}

    // Support for cereal NVP detection traits
    using cereal_nvp_tag = std::true_type;
  };

  template<typename T>
  struct is_nvp<NameValuePair<T>> : std::true_type
  {};

  template<typename T>
  struct is_nvp<::cereal::NameValuePair<T>> : std::true_type
  {};

  template<typename T>
  struct is_nvp<::boost::serialization::nvp<T>> : std::true_type
  {};

  template<typename T>
  constexpr NameValuePair<T> make_nvp(const char* name, T& value) noexcept
  {
    return NameValuePair<T>(name, value);
  }

  template<typename T>
  constexpr NameValuePair<const T> make_nvp(const char* name, const T& value) noexcept
  {
    return NameValuePair<const T>(name, value);
  }

  // --- Size Tag (signals archiver to enter array/sequence mode) ---

  template<typename T>
  struct SizeTag
  {
    T& value;
    constexpr SizeTag(T& v) noexcept
    : value(v)
    {}
  };

  template<typename T>
  constexpr SizeTag<T> make_size_tag(T& value) noexcept
  {
    return SizeTag<T>(value);
  }

  template<typename T>
  struct is_size_tag : std::false_type
  {};

  template<typename U>
  struct is_size_tag<SizeTag<U>> : std::true_type
  {};

  template<typename T>
  inline constexpr bool is_size_tag_v = is_size_tag<T>::value;

  namespace detail
  {
    template<typename T>
    constexpr auto get_nvp_name(T const& nvp) noexcept
    {
      if constexpr (is_nvp_v<T>)
      {
        if constexpr (requires { nvp.name(); })
          return nvp.name();
        else if constexpr (requires { nvp.name; })
          return nvp.name;
      }
    }

    template<typename T>
    constexpr decltype(auto) get_nvp_value(T& nvp) noexcept
    {
      if constexpr (is_nvp_v<T>)
      {
        if constexpr (requires { nvp.value(); })
          return nvp.value();
        else if constexpr (requires { nvp.value; })
          return (nvp.value);
      }
    }
  } // namespace detail


#ifndef ORTHOTREE_NVP
#define ORTHOTREE_NVP(member) ::OrthoTree::make_nvp(#member, member)
#endif

#ifndef ORTHOTREE_NVP_M
#define ORTHOTREE_NVP_M(obj, member) ::OrthoTree::make_nvp(#member, obj.member)
#endif

#ifndef ORTHOTREE_NVP_INT
#define ORTHOTREE_NVP_INT(member) ::OrthoTree::make_nvp(#member, this->member)
#endif


  // Generic bridge for other archivers (not for OrthoTree archives — they have member operator&)
  template<typename TArchive, typename T>
    requires(!is_orthotree_archive_v<TArchive>)
  auto operator&(TArchive& ar, NameValuePair<T> nvp) -> decltype(ar(detail::get_nvp_value(nvp)), ar)
  {
    if constexpr (detail::is_cereal_archive_v<TArchive>)
    {
      // Bridge to cereal NVP
      auto&& val = detail::get_nvp_value(nvp);
      if constexpr (is_size_tag_v<std::decay_t<decltype(val)>>)
      {
        ar(::cereal::make_nvp(detail::get_nvp_name(nvp), ::cereal::make_size_tag(val.value)));
      }
      else
      {
        ar(::cereal::make_nvp(detail::get_nvp_name(nvp), val));
      }
    }
    else if constexpr (detail::is_boost_archive_v<TArchive>)
    {
      // Bridge to boost NVP
      ar(::boost::serialization::make_nvp(detail::get_nvp_name(nvp), detail::get_nvp_value(nvp)));
    }
    else
    {
      // Fallback for any other archive that supports operator()
      ar(detail::get_nvp_value(nvp));
    }
    return ar;
  }

  template<typename TArchive, typename T>
    requires(!is_orthotree_archive_v<TArchive>)
  auto operator&(TArchive& ar, SizeTag<T> tag) -> decltype(ar(tag.value), ar)
  {
    if constexpr (detail::is_cereal_archive_v<TArchive>)
    {
      std::size_t cerealSize = static_cast<std::size_t>(tag.value);
      ar(::cereal::make_size_tag(cerealSize));
      tag.value = static_cast<T>(cerealSize);
    }
    else
    {
      ar(tag.value);
    }
    return ar;
  }

  // Generic bridge for archives that use operator() (like cereal/boost)
  // Note: !requires(ar & t) would cause recursive constraint satisfaction on GCC,
  // so we use !is_orthotree_archive_v which is equivalent (orthotree archives
  // have member operator& so they never need this bridge).
  template<typename TArchive, typename T>
    requires(!is_orthotree_archive_v<TArchive> && requires(TArchive& ar, T& t) { ar(t); })
  TArchive& operator&(TArchive& ar, T& t)
  {
    ar(t);
    return ar;
  }

} // namespace OrthoTree


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

namespace OrthoTree::detail
{
  template<typename T>
  struct IsStdOptional : std::false_type
  {
    using BaseType = T;
  };

  template<typename U>
  struct IsStdOptional<std::optional<U>> : std::true_type
  {
    using BaseType = U;
  };

  template<typename T>
  constexpr bool IsStdOptionalV = IsStdOptional<T>::value;


  template<size_t... Is, class F>
  constexpr void static_for_impl(std::index_sequence<Is...>, F&& f) noexcept
  {
    (f(static_cast<uint32_t>(std::integral_constant<size_t, Is>{})), ...);
  }

  template<size_t N, class F>
  constexpr void static_for(F&& f) noexcept
  {
    if constexpr (N < 16)
    {
      static_for_impl(std::make_index_sequence<N>{}, std::forward<F>(f));
    }
    else
    {
      constexpr uint32_t uintN = static_cast<uint32_t>(N);
      ORTHOTREE_LOOPIVDEP
      for (uint32_t i = 0; i < uintN; ++i)
        f(i);
    }
  }

  template<typename T>
  struct is_reference_stable : std::false_type
  {};

  // Standard pointer-stable containers
  template<typename T, std::size_t N>
  struct is_reference_stable<std::array<T, N>> : std::true_type
  {};
  template<typename... T>
  struct is_reference_stable<std::list<T...>> : std::true_type
  {};
  template<typename... T>
  struct is_reference_stable<std::forward_list<T...>> : std::true_type
  {};
  template<typename... T>
  struct is_reference_stable<std::map<T...>> : std::true_type
  {};
  template<typename... T>
  struct is_reference_stable<std::set<T...>> : std::true_type
  {};
  template<typename... T>
  struct is_reference_stable<std::multimap<T...>> : std::true_type
  {};
  template<typename... T>
  struct is_reference_stable<std::multiset<T...>> : std::true_type
  {};
  template<typename... T>
  struct is_reference_stable<std::unordered_map<T...>> : std::true_type
  {};
  template<typename... T>
  struct is_reference_stable<std::unordered_set<T...>> : std::true_type
  {};
  template<typename... T>
  struct is_reference_stable<std::unordered_multimap<T...>> : std::true_type
  {};
  template<typename... T>
  struct is_reference_stable<std::unordered_multiset<T...>> : std::true_type
  {};

  template<typename T>
  constexpr bool is_reference_stable_v = is_reference_stable<T>::value;

  template<typename... Handlers>
  struct Overloaded : Handlers...
  {
    using Handlers::operator()...;
  };

  template<typename... Handlers>
  Overloaded(Handlers...) -> Overloaded<Handlers...>;

  template<typename VariantT, typename... Handlers>
  decltype(auto) VisitVariant(VariantT&& variant, Handlers&&... handlers)
  {
    static_assert(std::variant_size_v<std::remove_reference_t<VariantT>> > 0, "VisitVariant requires a std::variant");

    auto overloaded = Overloaded<std::decay_t<Handlers>...>{ std::forward<Handlers>(handlers)... };

    return std::visit(overloaded, std::forward<VariantT>(variant));
  }

  template<typename VariantT, typename... Handlers>
  decltype(auto) VisitVariant(VariantT const& variant, Handlers&&... handlers)
  {
    static_assert(std::variant_size_v<std::remove_reference_t<VariantT>> > 0, "VisitVariant requires a std::variant");

    auto overloaded = Overloaded<std::decay_t<Handlers>...>{ std::forward<Handlers>(handlers)... };

    return std::visit(overloaded, variant);
  }


  // Trait to check if TOrthoTreeCore::Create() has maxDepthID and boxSpace parameters
  template<typename TCore>
  concept HasCreateWithBoxSpace = requires(
    TCore& tree,
    typename TCore::EA::EntityContainerView entities,
    std::optional<depth_t> maxDepthID,
    std::optional<typename TCore::GA::Box> boxSpace,
    std::size_t maxElementNoInNode,
    SeqExec execMode) { TCore::Create(tree, entities, maxDepthID, boxSpace, maxElementNoInNode, execMode); };

  template<typename TCore>
  inline constexpr bool HasCreateWithBoxSpaceV = HasCreateWithBoxSpace<TCore>;


  // Trait to check if TOrthoTreeCore::Create() has NO maxDepthID and boxSpace parameters (typical for BVH)
  template<typename TCore>
  concept HasCreateSimple = requires(TCore& tree, typename TCore::EA::EntityContainerView entities, std::size_t maxElementNoInNode, SeqExec execMode) {
    TCore::Create(tree, entities, maxElementNoInNode, execMode);
  };

  template<typename TCore>
  inline constexpr bool HasCreateSimpleV = HasCreateSimple<TCore>;


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
  constexpr typename TContainer::key_type getKeyPart(TContainer const&, typename TContainer::value_type const& value) noexcept
    requires(HasFirst<typename TContainer::value_type>)
  {
    return value.first;
  }

  template<typename TValue>
  constexpr auto getKeyPart(TValue const& value) noexcept
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
  constexpr void set(TContainer& continer, TKey key, TValue&& value) noexcept
  {
    continer[key] = std::forward<TValue>(value);
  }

  template<typename TContainer, typename TValue>
  constexpr void insert(TContainer& container, TValue&& value)
  {
    if constexpr (requires { container.push_back(std::forward<TValue>(value)); })
    {
      container.push_back(std::forward<TValue>(value));
    }
    else if constexpr (requires { container.insert(std::forward<TValue>(value)); })
    {
      container.insert(std::forward<TValue>(value));
    }
    else
    {
      static_assert(sizeof(TContainer) == 0, "Insert: unsupported container type");
    }
  }

  template<typename TReturn = std::size_t>
  constexpr TReturn size(auto const& container) noexcept
  {
    return static_cast<TReturn>(container.size());
  }


  template<typename TReturn = std::size_t>
  constexpr TReturn size(auto beginIt, auto endIt) noexcept
  {
    return static_cast<TReturn>(std::distance(beginIt, endIt));
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

  template<HasEmplaceBack TContainer, typename... TElement>
  constexpr void emplace(TContainer& container, TElement&&... element) noexcept
  {
    container.emplace_back(std::forward<TElement>(element)...);
  }

  template<typename TContainer, typename... TElement>
  concept HasEmplace = requires(TContainer container, TElement&&... elements) { container.emplace(std::forward<TElement>(elements)...); };

  template<HasEmplace TContainer, typename... TElement>
  constexpr void emplace(TContainer& container, TElement&&... element) noexcept
  {
    container.emplace(std::forward<TElement>(element)...);
  }


  template<HasEmplaceBack TContainer, typename TKey, typename TMappedType>
  constexpr void add(TContainer& container, TKey&& key, TMappedType&& mappedType) noexcept
  {
    container.resize(key + 1);
    container[key] = std::forward<TMappedType>(mappedType);
  }

  template<HasEmplace TContainer, typename TKey, typename TMappedType>
  constexpr void add(TContainer& container, TKey&& key, TMappedType&& mappedType) noexcept
  {
    container[std::forward<TKey>(key)] = std::forward<TMappedType>(mappedType);
  }


  constexpr bool contains(auto const& container, auto const& element) noexcept
  {
    return container.contains(element);
  }

  constexpr void clear(auto& container) noexcept
  {
    container.clear();
  }

  template<typename T, std::integral TIndex>
  constexpr auto erase(std::vector<T>& container, TIndex index) noexcept
  {
    return container.erase(container.begin() + static_cast<std::size_t>(index));
  }

  template<typename TContainer, typename TKey>
    requires(requires(TContainer& c, TKey const& k) { c.erase(k); })
  constexpr auto erase(TContainer& container, TKey const& element) noexcept
  {
    return container.erase(element);
  }

  template<typename TContainer, typename TValue>
    requires(!requires(TContainer& c, TValue const& v) { c.extract(v); })
  constexpr void decrementKeys(TContainer& container, TValue const& element) noexcept
  {
    detail::erase(container, element);
  }

  template<typename TContainer, typename TValue>
    requires(requires(TContainer& c, TValue const& v) { c.extract(v); })
  constexpr void decrementKeys(TContainer& container, TValue const& element) noexcept
  {
    auto reverseMap = std::move(container);
    for (auto it = reverseMap.begin(); it != reverseMap.end();)
    {
      auto node = reverseMap.extract(it++);
      node.key() -= (element <= node.key());
      container.insert(std::move(node));
    }
  }

  // Indexable containers (std::array, std::vector, std::span)
  template<typename Container, typename Key>
    requires(
      requires(Container& c, Key k) { c[k]; } && !requires(Container& c, Key k) { c.find(k); })
  constexpr decltype(auto) get(Container& container, Key id) noexcept
  {
    return container[id];
  }

  template<typename Container, typename Key>
    requires(
      requires(const Container& c, Key k) { c[k]; } && !requires(const Container& c, Key k) { c.find(k); })
  constexpr decltype(auto) get(const Container& container, Key id) noexcept
  {
    return container[id];
  }

  template<typename Container, typename Key>
    requires requires(Container& c, Key k) { c.find(k); }
  constexpr decltype(auto) get(Container& container, const Key& id) noexcept
  {
    return *container.find(id);
  }

  template<typename Container, typename Key>
    requires requires(const Container& c, Key k) { c.find(k); }
  constexpr decltype(auto) get(const Container& container, const Key& id) noexcept
  {
    return *container.find(id);
  }


  template<typename Container, typename Key>
    requires(
      requires(Container& c, Key k) { c[k]; } && !requires(Container& c, Key k) { c.find(k); })
  constexpr decltype(auto) get_if(Container& container, Key id) noexcept
  {
    return std::size_t(id) < container.size() ? &container[id] : nullptr;
  }

  template<typename Container, typename Key>
    requires(
      requires(const Container& c, Key k) { c[k]; } && !requires(const Container& c, Key k) { c.find(k); })
  constexpr decltype(auto) get_if(const Container& container, Key id) noexcept
  {
    return std::size_t(id) < container.size() ? &container[id] : nullptr;
  }

  template<typename Container, typename Key>
    requires requires(Container& c, Key k) { c.find(k); }
  constexpr decltype(auto) get_if(Container& container, const Key& id) noexcept
  {
    auto it = container.find(id);
    return it != container.end() ? &it->second : nullptr;
  }

  template<typename Container, typename Key>
    requires requires(const Container& c, Key k) { c.find(k); }
  constexpr decltype(auto) get_if(const Container& container, const Key& id) noexcept
  {
    auto it = container.find(id);
    return it != container.end() ? &it->second : nullptr;
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

  template<typename TContainer>
  concept HasReserve = requires(TContainer container) { container.reserve(0); };

  template<HasReserve TContainer>
  constexpr void reserve(TContainer& c, std::size_t n) noexcept
  {
    c.reserve(n);
  };

  template<typename TContainer>
  constexpr void reserve(TContainer&, std::size_t) noexcept {};

  template<typename TContainer>
  concept HasResize = requires(TContainer container) { container.resize(0); };

  template<HasResize TContainer>
  constexpr void resize(TContainer& c, std::size_t n) noexcept
  {
    c.resize(n);
  };

  template<typename TContainer>
  constexpr void resize(TContainer&, std::size_t) noexcept {};

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

  constexpr void inplaceMerge(auto const& comparator, auto& entityIDs, std::size_t middleIndex) noexcept
  {
    auto const beginIt = entityIDs.begin();
    auto const middleIt = beginIt + middleIndex;
    auto const endIt = entityIDs.end();
    std::sort(middleIt, endIt, comparator);
    std::inplace_merge(beginIt, middleIt, endIt, comparator);
  }

  template<dim_t DIMENSION_NO, depth_t MAX_THEORETICAL_DEPTH_ID>
  static constexpr std::size_t EstimateNodeNumber(std::size_t elementNo, depth_t maxDepthID, std::size_t maxElementNo) noexcept
  {
    assert(maxElementNo > 0);
    assert(maxDepthID > 0);

    if (elementNo < 10)
      return 10;

    auto constexpr rMult = 1.5;
    constexpr depth_t bitSize = sizeof(std::size_t) * CHAR_BIT;
    if ((maxDepthID + 1) * DIMENSION_NO < bitSize)
    {
      auto const nMaxChild = detail::pow2(maxDepthID * DIMENSION_NO);
      auto const nElementInNode = elementNo / nMaxChild;
      if (nElementInNode > maxElementNo / 2)
        return nMaxChild;
    }

    auto const nElementInNodeAvg = static_cast<float>(elementNo) / static_cast<float>(maxElementNo);
    auto const nDepthEstimated = std::min(maxDepthID, static_cast<depth_t>(ceil((log2f(nElementInNodeAvg) + 1.0) / static_cast<float>(DIMENSION_NO))));
    if (nDepthEstimated * DIMENSION_NO < 64)
      return static_cast<std::size_t>(1.05 * detail::pow2(nDepthEstimated * std::min<depth_t>(6, DIMENSION_NO)));

    return static_cast<std::size_t>(rMult * nElementInNodeAvg);
  }

  template<dim_t DIMENSION_NO, depth_t MAX_THEORETICAL_DEPTH_ID>
  static depth_t EstimateMaxDepth(std::size_t elementNo, std::size_t maxElementNo) noexcept
  {
    if (elementNo <= maxElementNo)
      return 2;

    auto const nLeaf = elementNo / maxElementNo;
    // nLeaf = (2^nDepth)^DIMENSION_NO
    return std::clamp(static_cast<depth_t>(std::log2(nLeaf) / static_cast<double>(DIMENSION_NO)), depth_t(2), MAX_THEORETICAL_DEPTH_ID);
  }

} // namespace OrthoTree::detail


#ifdef _MSC_VER
#pragma warning(default : 4715)
#endif

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif


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

      template<typename TArchive>
      void serialize(TArchive& ar)
      {
        ar& ORTHOTREE_NVP_INT(Min);
        ar& ORTHOTREE_NVP_INT(Max);
      }
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

    static constexpr Vector Multiply(Vector const& v, Geometry scalar) noexcept
    {
      Vector result;
      static_for<DIMENSION_NO>([&](auto dimensionID) { result[dimensionID] = v[dimensionID] * scalar; });
      return result;
    }

    static constexpr Vector Sub(Vector const& v1, Vector const& v2) noexcept
    {
      Vector v;
      static_for<DIMENSION_NO>([&](auto dimensionID) { v[dimensionID] = v1[dimensionID] - v2[dimensionID]; });
      return v;
    }

    static constexpr Vector Add(Vector const& v1, Vector const& v2) noexcept
    {
      Vector v;
      static_for<DIMENSION_NO>([&](auto dimensionID) { v[dimensionID] = v1[dimensionID] + v2[dimensionID]; });
      return v;
    }

    static constexpr Vector GetBoxMinPointAD(TBox const& box) noexcept
    {
      Vector minPoint;
      static_for<DIMENSION_NO>([&](auto dimensionID) { minPoint[dimensionID] = Geometry(GA::GetBoxMinC(box, dimensionID)); });
      return minPoint;
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
      static_for<DIMENSION_NO>(
        [&](auto dimensionID) { sizes[dimensionID] = Geometry((GA::GetBoxMaxC(box, dimensionID) - GA::GetBoxMinC(box, dimensionID))); });
      return sizes;
    }

    static bool AreBoxesOverlappingByMinPoint(
      Vector const& minPointLhs, Vector const& sizeLhs, Vector const& minPointRhs, Vector const& sizeRhs, Geometry tolerance) noexcept
    {
      for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
      {
        auto const maxLhs = minPointLhs[dimensionID] + sizeLhs[dimensionID];
        auto const maxRhs = minPointRhs[dimensionID] + sizeRhs[dimensionID];

        if (maxLhs <= minPointRhs[dimensionID] - tolerance || maxRhs <= minPointLhs[dimensionID] - tolerance)
          return false;
      }

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

    static constexpr bool DoesRangeContainBox(Geometry rangeMin, Geometry rangeMax, Geometry boxMin, Geometry boxMax, Geometry tolerance) noexcept
    {
      if (rangeMin - tolerance > boxMin || boxMin > rangeMax + tolerance)
        return false;

      if (rangeMin - tolerance > boxMax || boxMax > rangeMax + tolerance)
        return false;

      return true;
    }

    static constexpr bool DoesRangeContainBoxAD(TBox const& range, Box const& box, GA::FloatScalar tolerance) noexcept
    {
      for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
      {
        if (!DoesRangeContainBox(
              Geometry(GA::GetBoxMinC(range, dimensionID)), Geometry(GA::GetBoxMaxC(range, dimensionID)), box.Min[dimensionID], box.Max[dimensionID], tolerance))
        {
          return false;
        }
      }
      return true;
    }

    static constexpr bool DoesRangeContainBoxAD(Box const& range, TBox const& box, GA::FloatScalar tolerance) noexcept
    {
      for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
      {
        if (!DoesRangeContainBox(
              range.Min[dimensionID], range.Max[dimensionID], Geometry(GA::GetBoxMinC(box, dimensionID)), Geometry(GA::GetBoxMaxC(box, dimensionID)), tolerance))
        {
          return false;
        }
      }
      return true;
    }

    static constexpr bool DoesRangeContainBoxAD(Box const& range, Box const& box, GA::FloatScalar tolerance) noexcept
    {
      for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
      {
        if (!DoesRangeContainBox(range.Min[dimensionID], range.Max[dimensionID], box.Min[dimensionID], box.Max[dimensionID], tolerance))
        {
          return false;
        }
      }
      return true;
    }

    static constexpr bool DoesRangeContainBoxAD(TBox const& range, Vector const& minPoint, Vector const& size, GA::FloatScalar tolerance) noexcept
    {
      for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
      {
        const auto boxMax = minPoint[dimensionID] + size[dimensionID];

        if (!DoesRangeContainBox(
              Geometry(GA::GetBoxMinC(range, dimensionID)), Geometry(GA::GetBoxMaxC(range, dimensionID)), minPoint[dimensionID], boxMax, tolerance))
        {
          return false;
        }
      }
      return true;
    }

    static constexpr PlaneRelation GetBoxPlaneRelationAD(
      Vector const& minPoint, Vector const& size, TScalar distanceOfOrigo, TVector const& planeNormal, TFloatScalar tolerance) noexcept
    {
      assert(GA::IsNormalizedVector(planeNormal));

      Vector center;
      Vector halfSize;
      for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
      {
        halfSize[dimensionID] = size[dimensionID] * Geometry(0.5);
        center[dimensionID] = minPoint[dimensionID] + halfSize[dimensionID];
      }

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
      detail::static_for<DIMENSION_NO>([&](auto dimensionID) {
        boxIGM.Min[dimensionID] = Geometry(GA::GetBoxMinC(box, dimensionID));
        boxIGM.Max[dimensionID] = Geometry(GA::GetBoxMaxC(box, dimensionID));
      });

      return boxIGM;
    }

    static constexpr Box GetBoxAD(TVector const& point) noexcept
    {
      Box boxIGM;
      detail::static_for<DIMENSION_NO>([&](auto dimensionID) {
        auto const pointValue = Geometry(GA::GetPointC(point, dimensionID));
        boxIGM.Min[dimensionID] = pointValue;
        boxIGM.Max[dimensionID] = pointValue;
      });

      return boxIGM;
    }

    template<typename TEntityGeometry>
    static constexpr void UniteInBoxAD(Box& box, TEntityGeometry const& entityGeometry) noexcept
    {
      using EntityGeometry = std::remove_cv_t<TEntityGeometry>;

      detail::static_for<DIMENSION_NO>([&](auto dimensionID) {
        if constexpr (std::is_same_v<TVector, EntityGeometry>)
        {
          auto const point = Geometry(GA::GetPointC(entityGeometry, dimensionID));
          box.Min[dimensionID] = std::min(box.Min[dimensionID], point);
          box.Max[dimensionID] = std::max(box.Max[dimensionID], point);
        }
        else if constexpr (std::is_same_v<TBox, EntityGeometry>)
        {
          box.Min[dimensionID] = std::min(box.Min[dimensionID], Geometry(GA::GetBoxMinC(entityGeometry, dimensionID)));
          box.Max[dimensionID] = std::max(box.Max[dimensionID], Geometry(GA::GetBoxMaxC(entityGeometry, dimensionID)));
        }
        else if constexpr (std::is_same_v<Box, EntityGeometry>)
        {
          box.Min[dimensionID] = std::min(box.Min[dimensionID], entityGeometry.Min[dimensionID]);
          box.Max[dimensionID] = std::max(box.Max[dimensionID], entityGeometry.Max[dimensionID]);
        }
        else
        {
          static_assert(false);
        }
      });
    }

    template<typename EA>
    static constexpr Box GetBoundingBoxAD(typename EA::EntityContainerView entities) noexcept
    {
      auto ext = BoxInvertedInit();
      for (auto const& entity : entities)
      {
        auto const& entityGeometry = EA::GetGeometry(entity);
        if constexpr (EA::GEOMETRY_TYPE == GeometryType::Point)
        {
          detail::static_for<GA::DIMENSION_NO>([&](auto dimensionID) {
            auto const pointValue = Geometry(GA::GetPointC(entityGeometry, dimensionID));
            ext.Min[dimensionID] = std::min(ext.Min[dimensionID], pointValue);
            ext.Max[dimensionID] = std::max(ext.Max[dimensionID], pointValue);
          });
        }
        else if constexpr (EA::GEOMETRY_TYPE == GeometryType::Box)
        {
          detail::static_for<GA::DIMENSION_NO>([&](auto dimensionID) {
            ext.Min[dimensionID] = std::min(ext.Min[dimensionID], Geometry(GA::GetBoxMinC(entityGeometry, dimensionID)));
            ext.Max[dimensionID] = std::max(ext.Max[dimensionID], Geometry(GA::GetBoxMaxC(entityGeometry, dimensionID)));
          });
        }
        else
        {
          static_assert(false);
        }
      }
      return ext;
    }

    static constexpr bool DoesBoxContainPointAD(Box const& box, TVector const& point, TFloatScalar tolerance = GA::Tolerance) noexcept
    {
      if (tolerance != 0.0)
      {
        assert(tolerance > 0);
        for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
        {
          auto const pointValue = Geometry(GA::GetPointC(point, dimensionID));

          if (!(box.Min[dimensionID] - tolerance < pointValue && pointValue < box.Max[dimensionID] + tolerance))
            return false;
        }
      }
      else
      {
        for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
        {
          auto const pointValue = Geometry(GA::GetPointC(point, dimensionID));

          if (!(box.Min[dimensionID] <= pointValue && pointValue <= box.Max[dimensionID]))
            return false;
        }
      }
      return true;
    }

    static constexpr bool DoesBoxContainPointAD(Vector const& minPoint, Vector const& size, TVector const& point, TFloatScalar tolerance = GA::Tolerance) noexcept
    {
      if (tolerance != 0.0)
      {
        assert(tolerance > 0);
        for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
        {
          auto const halfSize = size[dimensionID] * Geometry(0.5);
          auto const pointDistance = std::abs(Geometry(GA::GetPointC(point, dimensionID)) - minPoint[dimensionID] - halfSize);
          if (pointDistance >= halfSize + tolerance)
            return false;
        }
      }
      else
      {
        for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
        {
          auto const halfSize = size[dimensionID] * Geometry(0.5);
          auto const pointDistance = std::abs(Geometry(GA::GetPointC(point, dimensionID)) - minPoint[dimensionID] - halfSize);
          if (pointDistance > halfSize)
            return false;
        }
      }
      return true;
    }

    static Geometry GetBoxWallDistanceAD(TVector const& searchPoint, Vector const& minPoint, Vector const& size, bool isInsideConsideredAsZero) noexcept
    {
      Vector centerDistance;
      Vector halfSize;
      bool isInside = true;
      for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
      {
        halfSize[dimensionID] = size[dimensionID] * Geometry(0.5);
        centerDistance[dimensionID] = std::abs(minPoint[dimensionID] + halfSize[dimensionID] - Geometry(GA::GetPointC(searchPoint, dimensionID)));
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
          minWallDistance = std::min(minWallDistance, wallDistance);
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

    static constexpr Geometry GetSurfaceAreaAD(Box const& range) noexcept
    {
      if constexpr (DIMENSION_NO == 2)
      {
        return ((range.Max[0] - range.Min[0]) + (range.Max[1] - range.Min[1])) * 2.0;
      }
      else if constexpr (DIMENSION_NO == 3)
      {
        auto const dx = range.Max[0] - range.Min[0];
        auto const dy = range.Max[1] - range.Min[1];
        auto const dz = range.Max[2] - range.Min[2];
        return (dx * dy + dy * dz + dz * dx) * 2.0;
      }
      else
      {
        Geometry surfaceArea = 0.0;
        static_for<DIMENSION_NO>([&](auto dimensionID) {
          Geometry faceArea = 1.0;
          static_for<DIMENSION_NO>([&](auto otherDimID) {
            if (otherDimID != dimensionID)
              faceArea *= (range.Max[otherDimID] - range.Min[otherDimID]);
          });
          surfaceArea += faceArea;
        });
        return surfaceArea * 2.0;
      }
    }

    static constexpr Geometry GetSurfaceAreaAD(TBox const& range) noexcept
    {
      if constexpr (DIMENSION_NO == 2)
      {
        return (Geometry(GA::GetBoxMaxC(range, 0) - GA::GetBoxMinC(range, 0)) + Geometry(GA::GetBoxMaxC(range, 1) - GA::GetBoxMinC(range, 1))) * 2.0;
      }
      else if constexpr (DIMENSION_NO == 3)
      {
        auto const dx = Geometry(GA::GetBoxMaxC(range, 0) - GA::GetBoxMinC(range, 0));
        auto const dy = Geometry(GA::GetBoxMaxC(range, 1) - GA::GetBoxMinC(range, 1));
        auto const dz = Geometry(GA::GetBoxMaxC(range, 2) - GA::GetBoxMinC(range, 2));
        return (dx * dy + dy * dz + dz * dx) * 2.0;
      }
      else
      {
        Geometry surfaceArea = 0.0;
        static_for<DIMENSION_NO>([&](auto dimensionID) {
          Geometry faceArea = 1.0;
          static_for<DIMENSION_NO>([&](auto otherDimID) {
            if (otherDimID != dimensionID)
              faceArea *= Geometry(GA::GetBoxMaxC(range, otherDimID) - GA::GetBoxMinC(range, otherDimID));
          });
          surfaceArea += faceArea;
        });
        return surfaceArea * 2.0;
      }
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
          boxPickTester->m_isZeroDirection.set(dimensionID, isNanCompnent);
          boxPickTester->m_hasNaNComponent |= isNanCompnent;
        }

        return boxPickTester;
      }

      struct PickResult
      {
        Geometry enterDistance, exitDistance;
      };

      template<bool isConeToleranceConsidered = true>
      constexpr std::optional<PickResult> Hit(const Vector& minPoint, const Vector& size) const noexcept
      {
        Vector minDifference, maxDifference;
        detail::static_for<DIMENSION_NO>(
          [&](dim_t dimensionID) noexcept { minDifference[dimensionID] = minPoint[dimensionID] - m_origin[dimensionID]; });
        detail::static_for<DIMENSION_NO>(
          [&](dim_t dimensionID) noexcept { maxDifference[dimensionID] = minPoint[dimensionID] + size[dimensionID] - m_origin[dimensionID]; });

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
        std::optional pickResult =
          PickResult{ .enterDistance = -std::numeric_limits<Geometry>::max(), .exitDistance = std::numeric_limits<Geometry>::max() };

        // plane distances
        std::array<Vector, 2> pd;
        detail::static_for<DIMENSION_NO>(
          [&](dim_t dimensionID) noexcept { pd[0][dimensionID] = minDifference[dimensionID] * m_inverseDirection[dimensionID]; });
        detail::static_for<DIMENSION_NO>(
          [&](dim_t dimensionID) noexcept { pd[1][dimensionID] = maxDifference[dimensionID] * m_inverseDirection[dimensionID]; });

        // Find the largest entering distance and the smallest exiting distance. fmax/fmin handles NaN correctly.
        if (m_hasNaNComponent)
        {
          detail::static_for<DIMENSION_NO>([&](dim_t dimensionID) noexcept {
            if (!m_isZeroDirection.test(dimensionID))
              pickResult->enterDistance = std::fmax(pickResult->enterDistance, pd[m_signInfo[dimensionID]][dimensionID]);
          });
          detail::static_for<DIMENSION_NO>([&](dim_t dimensionID) noexcept {
            if (!m_isZeroDirection.test(dimensionID))
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
            if (!m_isZeroDirection.test(dimensionID))
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
      std::bitset<DIMENSION_NO> m_signInfo = {};
      std::bitset<DIMENSION_NO> m_isZeroDirection = {};
      bool m_hasNaNComponent = false;
    };
  };
} // namespace OrthoTree::detail


namespace OrthoTree::detail
{
  using PageID = std::uint32_t;
  static constexpr PageID PRIMARY_PAGEID = 0;
  static constexpr PageID MAIN_PAGEID = PRIMARY_PAGEID; // legacy alias for serialization
  static constexpr PageID INVALID_PAGEID = std::numeric_limits<PageID>::max();

  template<typename T>
  using MemoryBlock = std::span<T>;

  template<typename T>
  struct MemorySegment
  {
    PageID pageID = PRIMARY_PAGEID;
    MemoryBlock<T> segment;
  };

  // RawPage: std::vector<T> like container without initialization
  template<typename T, typename Allocator = std::allocator<T>>
  class RawPage
  {
  public:
    constexpr RawPage() noexcept = default;
    constexpr RawPage(RawPage const&) = delete;
    constexpr RawPage& operator=(RawPage const&) = delete;

    constexpr RawPage(RawPage&& o) noexcept
    : m_data(o.m_data)
    , m_size(o.m_size)
    , m_capacity(o.m_capacity)
    {
      o.m_data = nullptr;
      o.m_size = o.m_capacity = 0;
    }

    constexpr RawPage& operator=(RawPage&& o) noexcept
    {
      if (this != &o)
      {
        Free();
        m_data = o.m_data;
        m_size = o.m_size;
        m_capacity = o.m_capacity;
        o.m_data = nullptr;
        o.m_size = o.m_capacity = 0;
      }
      return *this;
    }

    constexpr ~RawPage() noexcept { Free(); }

    constexpr void Allocate(std::size_t size, std::size_t capacity = -1) noexcept
    {
      Free();
      if (size == 0)
        return;

      m_capacity = capacity == -1 ? CalcCapacity(size) : capacity;
      m_data = Allocator{}.allocate(m_capacity);
      m_size = size;
      if constexpr (!std::is_trivially_copyable_v<T> && std::is_default_constructible_v<T>)
        std::uninitialized_default_construct_n(m_data, size);
    }

    constexpr void Free() noexcept
    {
      if (!m_data)
        return;

      if constexpr (!std::is_trivially_copyable_v<T> && std::is_default_constructible_v<T> && !std::is_trivially_destructible_v<T>)
        std::destroy_n(m_data, m_size);

      Allocator{}.deallocate(m_data, m_capacity);
      m_data = nullptr;
      m_size = m_capacity = 0;
    }

    // Resize: allocates more if needed (doubling capacity), default-constructs new elements.
    constexpr void Resize(std::size_t newSize) noexcept
    {
      if (newSize > m_capacity)
      {
        std::size_t newCapacity = CalcCapacity(newSize);
        T* nd = Allocator{}.allocate(newCapacity);
        if (m_data && m_size > 0)
        {
          if constexpr (std::is_trivially_copyable_v<T>)
            std::memcpy(nd, m_data, m_size * sizeof(T));
          else
            std::uninitialized_move_n(m_data, m_size, nd);
        }
        if constexpr (!std::is_trivially_copyable_v<T> && std::is_default_constructible_v<T>)
          std::uninitialized_default_construct_n(nd + m_size, newSize - m_size);
        Free();
        m_data = nd;
        m_capacity = newCapacity;
      }
      else
      {
        if (newSize > m_size)
        {
          if constexpr (!std::is_trivially_copyable_v<T> && std::is_default_constructible_v<T>)
            std::uninitialized_default_construct_n(m_data + m_size, newSize - m_size);
        }
        else if (newSize < m_size)
        {
          if constexpr (!std::is_trivially_destructible_v<T>)
            std::destroy_n(m_data + newSize, m_size - newSize);
        }
      }
      m_size = newSize;
    }

    constexpr T* Data() const noexcept { return m_data; }
    constexpr std::size_t Size() const noexcept { return m_size; }
    constexpr std::size_t Capacity() const noexcept { return m_capacity; }

  private:
    static constexpr std::size_t CalcCapacity(std::size_t size) noexcept
    {
      return std::clamp(std::bit_ceil(size), static_cast<std::size_t>(1.10 * size), static_cast<std::size_t>(1.30 * size));
    }

  private:
    T* m_data = nullptr;
    std::size_t m_size = 0;     // logical used size
    std::size_t m_capacity = 0; // physical allocation size
  };


  template<typename T>
  class AllocatorInterface
  {
  public:
    virtual ~AllocatorInterface() = default;
    virtual MemorySegment<T> Allocate(std::size_t capacity) noexcept = 0;
    virtual void Deallocate(MemorySegment<T>& segment) noexcept = 0;
    virtual bool TryToExtend(PageID pageID, MemoryBlock<T>& segment, std::size_t sizeIncrease) noexcept = 0;
    virtual bool TryToShrink(PageID pageID, MemoryBlock<T>& segment, std::size_t sizeDecrease) noexcept = 0;
    virtual void Reset() noexcept = 0;
  };


  template<typename T, std::size_t BUCKET_SIZE>
  class BucketAllocator : public AllocatorInterface<T>
  {
  public:
    static constexpr std::size_t POOL_SIZE = std::max<std::size_t>(64, 16 * 1024 / sizeof(T) / BUCKET_SIZE);
    struct Page
    {
    private:
      alignas(T) std::byte m_data[POOL_SIZE * BUCKET_SIZE * sizeof(T)];

    public:
      std::uint16_t pageOccupancy = 0;
      constexpr T* data() noexcept { return reinterpret_cast<T*>(m_data); }
    };

  public:
    constexpr explicit BucketAllocator() noexcept = default;
    constexpr explicit BucketAllocator(BucketAllocator&&) noexcept = default;
    constexpr BucketAllocator& operator=(BucketAllocator&&) noexcept = default;

    constexpr MemorySegment<T> Allocate(std::size_t size) noexcept override
    {
      assert(size <= BUCKET_SIZE);

      T* bucketPtr = nullptr;
      PageID pageID = 0;

      if (m_freeList.empty())
      {
        if (m_pages.empty() || m_cursor == POOL_SIZE * BUCKET_SIZE)
        {
          m_pages.emplace_back(new Page);
          m_cursor = 0;
        }

        pageID = PageID(m_pages.size() - 1);
        bucketPtr = m_pages.back()->data() + m_cursor;
        m_cursor += BUCKET_SIZE;
      }
      else
      {
        auto item = m_freeList.back();
        m_freeList.pop_back();
        pageID = item.first;
        bucketPtr = m_pages[pageID]->data() + item.second;
      }

      ++m_pages[pageID]->pageOccupancy;

      if constexpr (!std::is_trivially_copyable_v<T> && std::is_default_constructible_v<T>)
        std::uninitialized_default_construct_n(bucketPtr, size);

      return { pageID, std::span<T>(bucketPtr, size) };
    }

    constexpr void Deallocate(MemorySegment<T>& segment) noexcept override
    {
      assert(segment.pageID < m_pages.size());
      if (segment.pageID >= m_pages.size())
        return;

      if constexpr (!std::is_trivially_copyable_v<T> && std::is_default_constructible_v<T> && !std::is_trivially_destructible_v<T>)
        std::destroy_n(segment.segment.data(), segment.segment.size());

      assert(m_pages[segment.pageID]);
      assert(m_pages[segment.pageID]->pageOccupancy > 0);
      --m_pages[segment.pageID]->pageOccupancy;

      // If it matches the current cursor, just move cursor back
      if (segment.pageID == m_pages.size() - 1 && segment.segment.data() == (m_pages.back()->data() + (m_cursor - BUCKET_SIZE)))
      {
        m_cursor -= BUCKET_SIZE;
      }
      else
      {
        m_freeList.push_back({ segment.pageID, static_cast<std::uint16_t>(segment.segment.data() - m_pages[segment.pageID]->data()) });
      }

      // Recursive reclamation of terminal empty pages
      while (!m_pages.empty() && m_pages.back()->pageOccupancy == 0)
      {
        PageID const lastPageID = PageID(m_pages.size() - 1);
        std::erase_if(m_freeList, [lastPageID](auto const& p) { return p.first == lastPageID; });
        m_pages.pop_back();
        m_cursor = POOL_SIZE * BUCKET_SIZE;
      }
    }

    constexpr bool TryToExtend(PageID pageID, MemoryBlock<T>& segment, std::size_t sizeIncrease) noexcept override
    {
      assert(pageID < m_pages.size());
      if (pageID >= m_pages.size())
        return false;

      assert(m_pages[pageID]);
      auto const requestedSize = segment.size() + sizeIncrease;
      if (requestedSize > BUCKET_SIZE)
        return false;

      if constexpr (!std::is_trivially_copyable_v<T> && std::is_default_constructible_v<T>)
        std::uninitialized_default_construct_n(segment.data() + segment.size(), sizeIncrease);

      segment = std::span<T>(segment.data(), requestedSize);
      return true;
    }

    constexpr bool TryToShrink(PageID pageID, MemoryBlock<T>& segment, std::size_t sizeDecrease) noexcept override
    {
      assert(pageID < m_pages.size());
      if (pageID >= m_pages.size())
        return false;

      assert(m_pages[pageID]);
      auto const requestedSize = segment.size() - sizeDecrease;
      if (requestedSize < 1)
        return false;

      if constexpr (!std::is_trivially_destructible_v<T>)
        std::destroy_n(segment.data() + requestedSize, sizeDecrease);

      segment = std::span<T>(segment.data(), requestedSize);
      return true;
    }

    constexpr void Reset() noexcept override
    {
      m_pages.clear();
      m_freeList.clear();
      m_cursor = 0;
    }

  private:
    std::vector<std::unique_ptr<Page>> m_pages;
    std::vector<std::pair<PageID, std::uint16_t>> m_freeList;
    std::uint16_t m_cursor = 0;

    static_assert(std::numeric_limits<decltype(m_cursor)>::max() > POOL_SIZE * BUCKET_SIZE);
  };


  template<typename T>
  class PagedHeapAllocator : public AllocatorInterface<T>
  {
  public:
    constexpr explicit PagedHeapAllocator() noexcept = default;

    constexpr MemorySegment<T> Allocate(std::size_t capacity) noexcept override
    {
      if (capacity == 0)
        return { INVALID_PAGEID, {} };

      PageID id = 0;
      if (!m_freeList.empty())
      {
        id = m_freeList.back();
        m_freeList.pop_back();
      }
      else
      {
        id = static_cast<PageID>(m_pages.size());
        m_pages.emplace_back();
      }

      m_pages[id].Allocate(capacity);
      return { id, std::span<T>(m_pages[id].Data(), capacity) };
    }

    constexpr void Deallocate(MemorySegment<T>& segment) noexcept override
    {
      if (segment.segment.empty() || segment.pageID == INVALID_PAGEID)
        return;

      PageID id = segment.pageID;
      assert(id < m_pages.size());
      m_pages[id].Free();

      if (id < m_pages.size() - 1)
      {
        m_freeList.push_back(id);
      }
      else
      {
        m_pages.pop_back();
        /*
        // Fully reclaim any trailing free pages
        while (!m_pages.empty())
        {
          auto lastId = static_cast<PageID>(m_pages.size() - 1);
          auto it = std::find(m_freeList.begin(), m_freeList.end(), lastId);
          if (it == m_freeList.end())
            break;

          m_freeList.erase(it);
          m_pages.pop_back();
        }
        */
      }
    }

    constexpr bool TryToExtend(PageID pageID, MemoryBlock<T>& segment, std::size_t sizeIncrease) noexcept override
    {
      assert(pageID < m_pages.size());
      auto const requestedSize = segment.size() + sizeIncrease;

      m_pages[pageID].Resize(requestedSize);
      segment = std::span<T>(m_pages[pageID].Data(), requestedSize);
      return true;
    }

    constexpr bool TryToShrink(PageID pageID, MemoryBlock<T>& segment, std::size_t sizeDecrease) noexcept override
    {
      assert(pageID < m_pages.size());
      auto const requestedSize = segment.size() - sizeDecrease;
      if (requestedSize < 1)
        return false;

      m_pages[pageID].Resize(requestedSize);
      segment = std::span<T>(m_pages[pageID].Data(), requestedSize);
      return true;
    }

    constexpr void Reset() noexcept override
    {
      m_pages.clear();
      m_freeList.clear();
    }

  private:
    std::vector<RawPage<T>> m_pages;
    std::vector<PageID> m_freeList;
  };

  // ============================================================================
  // FixedBufferAllocator
  // Single pre-allocated buffer + address-sorted free list.
  //   • TryAllocate  : O(k) first-fit linear scan (k = free-segment count, typically < 32)
  //   • TryExtend    : O(log k) binary search
  //   • DeallocateRange : O(log k) binary search + O(k) insert/erase (rare)
  // Free list is kept sorted by begin address, enabling O(log k) neighbour
  // lookup during coalescing (vs. the old capacity-sorted scheme which needed
  // O(k) linear scans for FindConnecting/PrecedingFreeSegment).
  // ============================================================================
  template<typename T>
  class FixedBufferAllocator : public AllocatorInterface<T>
  {
  public:
#ifdef ORTHOTREE__LARGE_DATASET
    using Index = std::uint64_t;
#else
    using Index = std::uint32_t;
#endif

    static constexpr std::size_t MIN_SEGMENT_SIZE = 4;

  private:
    struct FreeSegment
    {
      Index begin = 0;
      Index capacity = 0;
    };

  public:
    constexpr void Init(std::size_t pageSize) noexcept
    {
      m_page.Allocate(pageSize + MIN_SEGMENT_SIZE, pageSize + MIN_SEGMENT_SIZE);
      m_free.clear();
      m_free.reserve(16);
      m_free.push_back({ 0, Index(m_page.Size()) });
    }

    // First-fit allocation: O(k) scan. Small leftover fragments (< MIN_SEGMENT_SIZE)
    // are consumed whole to avoid unusable slivers.
    constexpr MemorySegment<T> Allocate(std::size_t n) noexcept override
    {
      auto const requiredSize = Index(n);
      for (auto freeIt = m_free.begin(); freeIt != m_free.end(); ++freeIt)
      {
        if (freeIt->capacity < requiredSize)
          continue;

        T* ptr = m_page.Data() + freeIt->begin;
        auto const remaining = freeIt->capacity - requiredSize;

        if (remaining < MIN_SEGMENT_SIZE)
        {
          m_free.erase(freeIt);
        }
        else
        {
          freeIt->begin += requiredSize;
          freeIt->capacity = remaining;
        }
        return {
          PRIMARY_PAGEID, { ptr, n }
        };
      }
      return { PRIMARY_PAGEID, {} };
    }

    // O(log k): extend seg into the immediately following free segment.
    constexpr bool TryToExtend(PageID /*pageID*/, MemoryBlock<T>& seg, std::size_t increase) noexcept override
    {
      if (seg.empty())
        return false;
      auto const endIdx = IndexOf(seg.data()) + Index(seg.size());
      auto const increase_ = Index(increase);

      auto it = LowerBoundBegin(endIdx);
      if (it == m_free.end() || it->begin != endIdx || it->capacity < increase_)
        return false;

      it->begin += increase_;
      it->capacity -= increase_;
      if (it->capacity == 0)
        m_free.erase(it);
      seg = std::span<T>(seg.data(), seg.size() + increase);
      return true;
    }

    // Return the tail `sizeDecrease` elements back to the free list and shrink seg.
    constexpr bool TryToShrink(PageID /*pageID*/, MemoryBlock<T>& seg, std::size_t sizeDecrease) noexcept override
    {
      if (seg.size() <= sizeDecrease)
        return false;
      DeallocateRange(std::span<T>(seg.data() + seg.size() - sizeDecrease, sizeDecrease));
      seg = seg.first(seg.size() - sizeDecrease);
      return true;
    }

    constexpr void Deallocate(MemorySegment<T>& seg) noexcept override
    {
      if (!seg.segment.empty())
        DeallocateRange(seg.segment);
    }

    constexpr void Reset() noexcept override
    {
      m_page.Free();
      m_free.clear();
    }

    // Copy-compact all non-empty spans into a single contiguous block at the
    // start of this page. Updates each span pointer in-place.
    // Precondition: this allocator is fresh / reset.
    constexpr void CompactInto(std::vector<std::span<T>*>& spans, std::size_t totalSize)
    {
      m_page.Resize(totalSize);
      T* dest = m_page.Data();
      for (auto* s : spans)
      {
        if (s->empty())
          continue;
        auto const n = s->size();
        if constexpr (std::is_trivially_copyable_v<T>)
          std::memcpy(dest, s->data(), n * sizeof(T));
        else
          std::uninitialized_copy_n(s->data(), n, dest);
        *s = std::span<T>(dest, n);
        dest += n;
      }
      m_free.clear();
      std::size_t remaining = m_page.Size() - totalSize;
      if (remaining > MIN_SEGMENT_SIZE)
        m_free.push_back({ Index(totalSize), Index(remaining) });
    }

  private:
    constexpr Index IndexOf(T const* ptr) const noexcept { return Index(ptr - m_page.Data()); }

    // First free segment with begin >= val.
    constexpr auto LowerBoundBegin(Index val) noexcept
    {
      return std::lower_bound(m_free.begin(), m_free.end(), val, [](FreeSegment const& f, Index v) { return f.begin < v; });
    }

    // O(log k): return freed region to the free list, coalescing with neighbours.
    constexpr void DeallocateRange(std::span<T> seg) noexcept
    {
      auto const beginIdx = IndexOf(seg.data());
      auto const segSize = Index(seg.size());
      auto const endIdx = beginIdx + segSize;

      // First free segment that starts at or after the freed block's end.
      auto nextIt = LowerBoundBegin(endIdx);
      bool hasNext = (nextIt != m_free.end()) && (nextIt->begin == endIdx);

      // The potential predecessor is the entry immediately before nextIt.
      auto prevIt = nextIt;
      bool hasPrev = false;
      if (nextIt != m_free.begin())
      {
        --prevIt;
        hasPrev = (prevIt->begin + prevIt->capacity == beginIdx);
      }

      if (hasPrev && hasNext)
      {
        prevIt->capacity += segSize + nextIt->capacity; // merge all three
        m_free.erase(nextIt);
      }
      else if (hasPrev)
      {
        prevIt->capacity += segSize;
      }
      else if (hasNext)
      {
        nextIt->begin = beginIdx; // extend backwards
        nextIt->capacity += segSize;
      }
      else
      {
        m_free.insert(nextIt, FreeSegment{ beginIdx, segSize }); // new isolated block
      }

#ifdef _DEBUG
      if constexpr (std::is_integral_v<T>)
        std::fill(seg.begin(), seg.end(), std::numeric_limits<T>::max());
      assert(CheckFreeList());
#endif
    }

    [[maybe_unused]] constexpr bool CheckFreeList() const noexcept
    {
      for (std::size_t i = 1; i < m_free.size(); ++i)
        if (m_free[i - 1].begin + m_free[i - 1].capacity > m_free[i].begin)
          return false;
      return true;
    }

    template<typename T2>
    friend class MemoryResource;

  private:
    RawPage<T> m_page;
    std::vector<FreeSegment> m_free; // sorted ascending by begin address
  };

  template<typename T>
  class MemoryResource
  {
  public:
#ifdef ORTHOTREE__LARGE_DATASET
    using Index = std::uint64_t;
#else
    using Index = std::uint32_t;
#endif

    using MemorySegment = detail::MemorySegment<T>;

    static constexpr std::size_t MIN_SEGMENT_SIZE = FixedBufferAllocator<T>::MIN_SEGMENT_SIZE;
    static constexpr std::size_t DEFAULT_PAGE_SIZE = 4096 / sizeof(T);

  public:
    MemoryResource() = default;
    MemoryResource(MemoryResource const&) = delete;
    MemoryResource(MemoryResource&&) = default;
    MemoryResource& operator=(MemoryResource&&) = default;

    void Init(std::size_t firstPageSize = DEFAULT_PAGE_SIZE) noexcept { m_primary.Init(firstPageSize); }

    void Reset() noexcept
    {
      m_primary.Reset();
      m_fallback.Reset();
    }


    MemorySegment Allocate(std::size_t capacity) noexcept
    {
      ORTHOTREE_CRASH_IF(capacity > std::numeric_limits<Index>::max(), "Too many elements. Use ORTHOTREE__LARGE_DATASET!");
      assert(capacity > 0);

      // Hot path: fixed primary buffer (cache-friendly, no heap round-trip)
      if (capacity > 32)
      {
        if (auto ms = m_primary.Allocate(capacity); !ms.segment.empty())
          return ms;
      }

      // Bucket allocators for small, frequent allocations
      auto bucketIndex = (capacity - 1) / 4;
      switch (bucketIndex)
      {
      case 0: {
        auto ms = m_bucket4.Allocate(capacity);
        ms.pageID = GetGlobalPageID(ms.pageID, 0);
        return ms;
      }
      case 1: {
        auto ms = m_bucket8.Allocate(capacity);
        ms.pageID = GetGlobalPageID(ms.pageID, 1);
        return ms;
      }
      case 2:
      case 3: {
        auto ms = m_bucket16.Allocate(capacity);
        ms.pageID = GetGlobalPageID(ms.pageID, 2);
        return ms;
      }
      case 4:
      case 5: {
        auto ms = m_bucket24.Allocate(capacity);
        ms.pageID = GetGlobalPageID(ms.pageID, 3);
        return ms;
      }
      case 6:
      case 7: {
        auto ms = m_bucket32.Allocate(capacity);
        ms.pageID = GetGlobalPageID(ms.pageID, 4);
        return ms;
      }
      default: {
        auto ms = m_fallback.Allocate(capacity);
        ++ms.pageID;
        return ms;
      }
      }
    }

    void Deallocate(MemorySegment const& ms) noexcept
    {
      if (ms.segment.empty())
        return;

      auto localMs = ms;

      if (ms.pageID == PRIMARY_PAGEID)
      {
        m_primary.Deallocate(localMs);
      }
      else if (IsBucketPageID(ms.pageID))
      {
        localMs.pageID = GetLocalPageID(ms.pageID);
        switch (GetBucketIndex(ms.pageID))
        {
        case 0: m_bucket4.Deallocate(localMs); break;
        case 1: m_bucket8.Deallocate(localMs); break;
        case 2: m_bucket16.Deallocate(localMs); break;
        case 3: m_bucket24.Deallocate(localMs); break;
        case 4: m_bucket32.Deallocate(localMs); break;
        default: break;
        }
      }
      else
      {
        localMs.pageID = GetLocalPageID(ms.pageID);
        m_fallback.Deallocate(localMs);
      }
    }

    AllocatorInterface<T>& GetAllocator(PageID pageID) noexcept
    {
      if (pageID == PRIMARY_PAGEID)
      {
        return m_primary;
      }
      else if (IsBucketPageID(pageID))
      {
        switch (GetBucketIndex(pageID))
        {
        case 0: return m_bucket4;
        case 1: return m_bucket8;
        case 2: return m_bucket16;
        case 3: return m_bucket24;
        case 4: return m_bucket32;
        default: return m_fallback;
        }
      }
      else
      {
        return m_fallback;
      }
    }

    void IncreaseSegment(MemorySegment& ms, std::size_t sizeIncrease) noexcept
    {
      if (ms.segment.empty())
      {
        ms = Allocate(sizeIncrease);
        return;
      }

      auto& allocator = GetAllocator(ms.pageID);
      if (allocator.TryToExtend(GetLocalPageID(ms.pageID), ms.segment, sizeIncrease))
        return;

      auto newMs = Allocate(ms.segment.size() + sizeIncrease);
      CopyOrMove(ms.segment, newMs.segment);
      Deallocate(ms);
      ms = newMs;
    }

    void DecreaseSegment(MemorySegment& ms, std::size_t sizeDecrease)
    {
      if (ms.segment.empty())
        return;
      assert(ms.segment.size() >= sizeDecrease);

      auto& allocator = GetAllocator(ms.pageID);
      if (allocator.TryToShrink(GetLocalPageID(ms.pageID), ms.segment, sizeDecrease))
        return;

      if (sizeDecrease < ms.segment.size())
      {
        auto newMs = Allocate(ms.segment.size() - sizeDecrease);
        CopyOrMove(ms.segment.first(ms.segment.size() - sizeDecrease), newMs.segment);
        Deallocate(ms);
        ms = newMs;
      }
      else
      {
        Deallocate(ms);
        ms = {};
      }
    }

    // Copy-compact all segments into a single contiguous block in dst.
    // Updates each MemorySegment pointer to refer to the new location.
    // After this call every segment has pageID == PRIMARY_PAGEID.
    void Clone(MemoryResource& dst, std::vector<MemorySegment*> memorySegments) const noexcept
    {
      std::size_t totalSize = 0;
      for (auto* pms : memorySegments)
        totalSize += pms->segment.size();

      std::vector<std::span<T>*> spans;
      spans.reserve(memorySegments.size());
      for (auto* pms : memorySegments)
        spans.push_back(&pms->segment);

      dst.m_primary.CompactInto(spans, totalSize);
      dst.m_bucket4.Reset();
      dst.m_bucket8.Reset();
      dst.m_bucket16.Reset();
      dst.m_bucket24.Reset();
      dst.m_bucket32.Reset();
      dst.m_fallback.Reset();

      for (auto* pms : memorySegments)
        pms->pageID = PRIMARY_PAGEID;
    }

  private:
    static constexpr bool IsBucketPageID(PageID pageID) noexcept { return pageID >= (1u << 31); }
    static constexpr int GetBucketIndex(PageID pageID) noexcept { return static_cast<int>((pageID >> 28) & 0x7); }

    static constexpr PageID GetLocalPageID(PageID pageID) noexcept
    {
      if (IsBucketPageID(pageID))
        return pageID & 0x0FFFFFFF;

      if (pageID == PRIMARY_PAGEID)
        return PRIMARY_PAGEID;

      return pageID - 1;
    }

    static constexpr PageID GetGlobalPageID(PageID localPageID, int bucketIndex) noexcept
    {
      return (1u << 31) | (static_cast<uint32_t>(bucketIndex) << 28) | localPageID;
    }

    static void CopyOrMove(MemoryBlock<T> const& src, MemoryBlock<T>& dst) noexcept
    {
      if constexpr (std::is_trivially_copyable_v<T>)
        std::memcpy(dst.data(), src.data(), src.size() * sizeof(T));
      else
        std::uninitialized_move_n(src.data(), src.size(), dst.data());
    }

  private:
    /*
      Version 0 -> 1: m_memorySize from std::size_t to serialized_size_t
    */
    static constexpr uint32_t SERIALIZED_VERSION_ID = 1;

    template<typename TData, typename TNodeMap>
    friend class MemoryResourceSerializerProxy;

    template<typename TArchive, typename TData, typename TNodes>
    friend void serialize(TArchive& ar, MemoryResource<TData>& memoryResource, TNodes& nodes);

  private:
    FixedBufferAllocator<T> m_primary;
    BucketAllocator<T, 4> m_bucket4;
    BucketAllocator<T, 8> m_bucket8;
    BucketAllocator<T, 16> m_bucket16;
    BucketAllocator<T, 24> m_bucket24;
    BucketAllocator<T, 32> m_bucket32;
    PagedHeapAllocator<T> m_fallback;
  };
} // namespace OrthoTree::detail


namespace OrthoTree::Partitioning
{
  template<uint32_t kChunkBitSize>
  class flagset
  {
  private:
    using BucketIndexType = uint64_t;
    static constexpr std::size_t kBucketIndexBitSize = sizeof(BucketIndexType) * 8;
    static constexpr std::size_t kChunkNum = std::max<std::size_t>(1, kChunkBitSize / kBucketIndexBitSize);
    using Container = std::array<BucketIndexType, kChunkNum>;

    static_assert(kChunkBitSize > 0);

  public:
    class Iterator
    {
    public:
      constexpr Iterator(BucketIndexType const* chunks, uint32_t currentChunkIndex, BucketIndexType currentChunkState) noexcept
      : m_chunks(chunks)
      , m_currentChunkIndex(currentChunkIndex)
      , m_currentChunkState(currentChunkState)
      {
        AdvanceToValid();
      }

      constexpr uint32_t operator*() const noexcept
      {
        return m_currentChunkIndex * kBucketIndexBitSize + static_cast<uint32_t>(std::countr_zero(m_currentChunkState));
      }

      constexpr Iterator& operator++() noexcept
      {
        m_currentChunkState &= m_currentChunkState - 1; // clear lowest bit
        AdvanceToValid();
        return *this;
      }

      constexpr bool operator==(std::default_sentinel_t) const noexcept { return m_currentChunkIndex >= kChunkNum; }

    private:
      constexpr void AdvanceToValid() noexcept
      {
        while (!m_currentChunkState && ++m_currentChunkIndex < kChunkNum)
          m_currentChunkState = m_chunks[m_currentChunkIndex];
      }

      BucketIndexType const* m_chunks;
      uint32_t m_currentChunkIndex;
      BucketIndexType m_currentChunkState;
    };

    constexpr flagset() = default;

    constexpr Iterator begin() const noexcept { return Iterator(m_chunks.data(), 0, m_chunks[0]); }

    constexpr std::default_sentinel_t end() const noexcept { return {}; }

    constexpr void set(uint32_t bucketID) noexcept
    {
      auto const indexID = bucketID / kBucketIndexBitSize;
      auto const indexBit = bucketID - indexID * kBucketIndexBitSize;
      m_chunks[indexID] |= BucketIndexType(1) << indexBit;
    }

    constexpr std::size_t size() const noexcept
    {
      std::size_t bucketNum = 0;
      for (const auto bucketIndexChunk : m_chunks)
        bucketNum += std::popcount(bucketIndexChunk);

      return bucketNum;
    }

    constexpr void for_each(auto&& fn) const noexcept
    {
      for (int bucketIndexID = 0; bucketIndexID < kChunkNum; ++bucketIndexID)
      {
        auto bucketIndexChunk = m_chunks[bucketIndexID];
        while (bucketIndexChunk)
        {
          auto const bucketID = bucketIndexID * kBucketIndexBitSize + std::countr_zero(bucketIndexChunk);
          bucketIndexChunk &= bucketIndexChunk - 1;
          fn(bucketID);
        }
      }
    }

  private:
    Container m_chunks = {};
  };


  constexpr auto Quicksort3WayPartitionImpl(auto beginIt, auto endIt, auto const& accessor) noexcept
  {
    assert(detail::size(beginIt, endIt) > 1);

    auto lastIt = std::prev(endIt);
    auto const v = accessor(*lastIt);

    auto i = beginIt;
    auto j = lastIt;

    // equal elements cursors
    auto p = beginIt; // elements equal to pivot from left
    auto q = lastIt;  // elements equal to pivot from right

    for (;;)
    {
      while (accessor(*i) < v)
        ++i;

      while (v < accessor(*(--j)))
        if (j == beginIt)
          break;

      if (i >= j)
        break;

      std::iter_swap(i, j);

      if (accessor(*i) == v)
      {
        std::iter_swap(p, i);
        ++p;
      }

      if (accessor(*j) == v)
      {
        --q;
        std::iter_swap(j, q);
      }

      ++i;
    }

    // Moving equal elements to center
    std::iter_swap(i, lastIt);

    auto left_ptr = i;
    for (auto k = beginIt; k < p; ++k)
    {
      if (left_ptr != beginIt)
      {
        --left_ptr;
        std::iter_swap(k, left_ptr);
      }
    }

    auto right_ptr = i;
    for (auto k = std::prev(lastIt); k >= q; --k)
    {
      ++right_ptr;
      std::iter_swap(k, right_ptr);
    }

    return std::pair{ left_ptr, std::next(right_ptr) };
  }

  // Bentley-McIlroy 3-way quicksort-based partitioning
  void Quicksort3WayPartition(auto beginIt, auto endIt, auto const& accessor, std::size_t maxClusterSize, auto& partititionEnds) noexcept
  {
    auto const elementNum = detail::size(beginIt, endIt);
    if (elementNum <= maxClusterSize)
    {
      partititionEnds.push_back(endIt);
      return;
    }

    auto const [midBeginIt, midEndIt] = Quicksort3WayPartitionImpl(beginIt, endIt, accessor);
    Quicksort3WayPartition(beginIt, midBeginIt, accessor, maxClusterSize, partititionEnds);
    Quicksort3WayPartition(midEndIt, endIt, accessor, maxClusterSize, partititionEnds);
  }

  constexpr auto partition(auto first, auto last, auto&& p, auto&& spectate) noexcept
  {
    while (true)
    {
      while (first != last && p(*first))
      {
        spectate(*first);
        ++first;
      }

      if (first == last)
        return first;

      while (true)
      {
        if (first == --last)
          return first;

        if (p(*last))
          break;
      }

      spectate(*last);
      std::iter_swap(first, last);
      ++first;
    }
  }


  constexpr auto partition(auto first, auto last, auto&& p, auto&& spectateTrue, auto&& spectateFalse) noexcept
  {
    while (true)
    {
      while (first != last && p(*first))
      {
        spectateTrue(*first);
        ++first;
      }

      if (first == last)
        return first;

      while (true)
      {
        if (first == --last)
        {
          spectateFalse(*first);
          return first;
        }

        if (p(*last))
          break;

        spectateFalse(*last);
      }

      spectateTrue(*last);
      spectateFalse(*first);
      std::iter_swap(first, last);
      ++first;
    }
  }
} // namespace OrthoTree::Partitioning


// Generates a contiguous sequence of integral values:
// [beginValue, beginValue + length)
template<typename TValue>
class SequenceView
{
public:
  using ValueType = TValue;
  using SizeType = std::size_t;

  class Iterator
  {
  public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = ValueType;
    using difference_type = std::ptrdiff_t;
    using pointer = void;
    using reference = ValueType;

    constexpr Iterator() = default;
    constexpr explicit Iterator(ValueType value)
    : m_value(value)
    {}

    constexpr ValueType operator*() const noexcept { return m_value; }

    constexpr Iterator& operator++() noexcept
    {
      ++m_value;
      return *this;
    }

    constexpr Iterator operator++(int) noexcept
    {
      Iterator tmp(*this);
      ++(*this);
      return tmp;
    }

    constexpr Iterator& operator--() noexcept
    {
      --m_value;
      return *this;
    }

    constexpr Iterator operator--(int) noexcept
    {
      Iterator tmp(*this);
      --(*this);
      return tmp;
    }

    constexpr Iterator& operator+=(difference_type n) noexcept
    {
      m_value += static_cast<ValueType>(n);
      return *this;
    }

    constexpr Iterator& operator-=(difference_type n) noexcept
    {
      m_value -= static_cast<ValueType>(n);
      return *this;
    }

    constexpr Iterator operator+(difference_type n) const noexcept { return Iterator(m_value + static_cast<ValueType>(n)); }

    constexpr Iterator operator-(difference_type n) const noexcept { return Iterator(m_value - static_cast<ValueType>(n)); }

    constexpr difference_type operator-(Iterator const& other) const noexcept { return static_cast<difference_type>(m_value - other.m_value); }

    constexpr ValueType operator[](difference_type n) const noexcept { return m_value + static_cast<ValueType>(n); }

    constexpr bool operator==(Iterator const& other) const noexcept { return m_value == other.m_value; }
    constexpr bool operator!=(Iterator const& other) const noexcept { return m_value != other.m_value; }
    constexpr bool operator<(Iterator const& other) const noexcept { return m_value < other.m_value; }
    constexpr bool operator>(Iterator const& other) const noexcept { return m_value > other.m_value; }
    constexpr bool operator<=(Iterator const& other) const noexcept { return m_value <= other.m_value; }
    constexpr bool operator>=(Iterator const& other) const noexcept { return m_value >= other.m_value; }

  private:
    ValueType m_value = {};
  };

  constexpr SequenceView(ValueType beginValue, ValueType endValue) noexcept
  : m_beginValue(beginValue)
  , m_endValue(endValue)
  {}

  constexpr Iterator begin() const noexcept { return Iterator(m_beginValue); }
  constexpr Iterator end() const noexcept { return Iterator(m_endValue); }
  constexpr SizeType size() const noexcept { return m_endValue - m_beginValue; }
  constexpr bool empty() const noexcept { return size() == 0; }

private:
  ValueType m_beginValue = {};
  ValueType m_endValue = {};
};


namespace OrthoTree::detail
{

  template<typename TGeometryAdapter>
  class GridSpaceIndexing
  {
  public:
    using GA = TGeometryAdapter;
    using IGM = InternalGeometryModule<GA>;
    using IGM_Geometry = typename IGM::Geometry;

    static constexpr dim_t DIMENSION_NO = GA::DIMENSION_NO;

    template<typename T>
    using DimArray = std::array<T, DIMENSION_NO>;

    // Grid cell identification type
    using GridPosition = DimArray<GridID>;

    // Min-max grid ID pairs
    using GridRange = std::array<GridPosition, 2>;

  public:
    constexpr GridSpaceIndexing() = default;

    constexpr GridSpaceIndexing(depth_t maxDepthID, typename IGM::Box const& boxSpace) noexcept
    : m_maxRasterResolution(detail::pow2<depth_t, GridID>(maxDepthID))
    , m_maxRasterID(m_maxRasterResolution - 1)
    , m_boxSpace(boxSpace)
    {
      if constexpr (std::is_same_v<IGM_Geometry, float>)
      {
        if (maxDepthID >= 16)
          m_rasterization = ForcedDoubleRasterizationFactors{};
        else
          m_rasterization = NativeRasterizationFactors{};
      }
      else
      {
        m_rasterization = NativeRasterizationFactors{};
      }

      std::visit(
        [&](auto& rasterization) {
          using RasterizationType = std::decay_t<decltype(rasterization)>;
          using RasterizationFloat = typename RasterizationType::Float;

          auto& [sizeInDimensions, rasterizerFactors, derasterizerFactors] = rasterization;

          auto const subDivisionNoFactor = RasterizationFloat(m_maxRasterResolution);
          for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
          {
            m_sizeInDimensions[dimensionID] = m_boxSpace.Max[dimensionID] - m_boxSpace.Min[dimensionID];
            sizeInDimensions[dimensionID] = RasterizationFloat(m_sizeInDimensions[dimensionID]);

            derasterizerFactors[dimensionID] = sizeInDimensions[dimensionID] / subDivisionNoFactor;
            auto const isFlat = sizeInDimensions[dimensionID] == 0;
            rasterizerFactors[dimensionID] = isFlat ? RasterizationFloat(1) : (subDivisionNoFactor / sizeInDimensions[dimensionID]);
          }
        },
        m_rasterization);

      m_volumeOfOverallSpace = IGM::GetVolumeAD(m_boxSpace);
    }

  private:
    static constexpr uint32_t SERIALIZED_VERSION_ID = 0;

    template<typename TArchive, typename TGA>
    friend void serialize(TArchive& ar, GridSpaceIndexing<TGA>& grid);

  public:
    constexpr IGM::Vector const& GetMinPoint() const noexcept { return m_boxSpace.Min; }

    constexpr IGM::Vector const& GetSize() const noexcept { return m_sizeInDimensions; }

    constexpr IGM::Geometry GetVolume() const noexcept { return m_volumeOfOverallSpace; }

    constexpr IGM::Box const& GetBoxSpace() const noexcept { return m_boxSpace; }

    constexpr void Move(IGM::Vector const& moveVector) noexcept { IGM::MoveAD(m_boxSpace, moveVector); }

    constexpr GridID GetResolution() const noexcept { return m_maxRasterResolution; }

    constexpr IGM::Vector CalculateGridCellCenter(DimArray<GridID>&& gridID, depth_t&& centerLevel) const noexcept
    {
      return std::visit(
        [&](auto& rasterization) {
          using RasterizationType = std::decay_t<decltype(rasterization)>;
          using RasterizationFloat = typename RasterizationType::Float;
          auto& [sizeInDimensions, rasterizerFactors, derasterizerFactors] = rasterization;

          auto const halfGrid = RasterizationFloat(detail::pow2(centerLevel)) * RasterizationFloat(0.5);

          typename IGM::Vector center;
          detail::static_for<GA::DIMENSION_NO>([&](auto dimensionID) {
            center[dimensionID] =
              IGM_Geometry((RasterizationFloat(gridID[dimensionID]) + halfGrid) * derasterizerFactors[dimensionID]) + m_boxSpace.Min[dimensionID];
          });

          return center;
        },
        m_rasterization);
    }

    constexpr DimArray<GridID> GetPointGridID(GA::Vector const& point) const noexcept
    {
      return std::visit(
        [&](auto& rasterization) {
          using RasterizationType = std::decay_t<decltype(rasterization)>;
          using RasterizationFloat = typename RasterizationType::Float;

          auto& [sizeInDimensions, rasterizerFactors, derasterizerFactors] = rasterization;

          auto gridIDs = DimArray<GridID>{};
          for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
          {
            auto pointComponent = IGM_Geometry(GA::GetPointC(point, dimensionID)) - m_boxSpace.Min[dimensionID];
            if (pointComponent < -GA::BASE_TOLERANCE || pointComponent > sizeInDimensions[dimensionID] + GA::BASE_TOLERANCE)
            {
              gridIDs[0] = INVALID_GRIDID;
              return gridIDs;
            }

            if (pointComponent <= 0)
              gridIDs[dimensionID] = 0;
            else
            {
              auto const rasterID = GridID(RasterizationFloat(pointComponent) * rasterizerFactors[dimensionID]);
              gridIDs[dimensionID] = std::min<GridID>(m_maxRasterID, rasterID);
            }
          }
          return gridIDs;
        },
        m_rasterization);
    }

    constexpr std::array<DimArray<GridID>, 2> GetBoxGridID(GA::Box const& box) const noexcept
    {
      return std::visit(
        [&](auto& rasterization) {
          using RasterizationType = std::decay_t<decltype(rasterization)>;
          using RasterizationFloat = typename RasterizationType::Float;

          auto constexpr zero = RasterizationFloat{};
          auto const maxRasterID = RasterizationFloat(m_maxRasterResolution);

          auto& [sizeInDimensions, rasterizerFactors, derasterizerFactors] = rasterization;

          std::array<DimArray<GridID>, 2> gridID;
          for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
          {
            auto const boxMin = IGM_Geometry(GA::GetBoxMinC(box, dimensionID)) - m_boxSpace.Min[dimensionID];
            auto const boxMax = IGM_Geometry(GA::GetBoxMaxC(box, dimensionID)) - m_boxSpace.Min[dimensionID];

            if (boxMin < -GA::BASE_TOLERANCE || boxMax > m_sizeInDimensions[dimensionID] + GA::BASE_TOLERANCE)
            {
              gridID[0][0] = INVALID_GRIDID;
              return gridID;
            }

            assert(boxMin <= boxMax && "Wrong bounding box. Input error.");
            auto const minComponentRasterID = RasterizationFloat(boxMin) * rasterizerFactors[dimensionID];
            auto const maxComponentRasterID = RasterizationFloat(boxMax) * rasterizerFactors[dimensionID];

            gridID[0][dimensionID] = static_cast<GridID>(std::clamp(minComponentRasterID, zero, maxRasterID));
            gridID[1][dimensionID] = static_cast<GridID>(std::clamp(maxComponentRasterID, zero, maxRasterID));

            if ((gridID[0][dimensionID] != gridID[1][dimensionID] && std::floor(maxComponentRasterID) == maxComponentRasterID) || gridID[1][dimensionID] >= m_maxRasterResolution)
            {
              --gridID[1][dimensionID];
            }

            assert(gridID[0][dimensionID] < m_maxRasterResolution);
            assert(gridID[1][dimensionID] < m_maxRasterResolution);
          }
          return gridID;
        },
        m_rasterization);
    }

    template<double LOOSE_FACTOR>
    constexpr std::pair<GridPosition, depth_t> GetLooseBoxGridData(GA::Box const& box) const noexcept
    {
      if (!IGM::DoesRangeContainBoxAD(m_boxSpace, box, GA::BASE_TOLERANCE))
        return { GridPosition{ INVALID_GRIDID }, 0 };

      auto const boxCenter = IGM::GetBoxCenterAD(box);
      auto const boxSize = IGM::GetBoxSizeAD(box);
      return std::visit(
        [&](auto& rasterization) -> std::pair<GridPosition, depth_t> {
          using RasterizationType = std::decay_t<decltype(rasterization)>;
          using RasterizationFloat = typename RasterizationType::Float;

          auto& [sizeInDimensions, rasterizerFactors, derasterizerFactors] = rasterization;

          GridPosition boxCenterGrid;
          static_for<DIMENSION_NO>([&](auto dimensionID) noexcept {
            // box is withing the space, so center must be also within, no need to clamp
            boxCenterGrid[dimensionID] =
              GridID(RasterizationFloat(boxCenter[dimensionID] - m_boxSpace.Min[dimensionID]) * rasterizerFactors[dimensionID]);
          });

          auto maxRelativeSize = RasterizationFloat{};
          static_for<DIMENSION_NO>([&](auto dimensionID) noexcept {
            maxRelativeSize = std::max(maxRelativeSize, RasterizationFloat(boxSize[dimensionID]) * rasterizerFactors[dimensionID]);
          });

          GridID maxRelativeGridSize = GridID(std::ceil(maxRelativeSize));
          if (maxRelativeGridSize == 0)
            return { boxCenterGrid, 0 };

          auto levelID = std::bit_width(maxRelativeGridSize - 1);
          assert(levelID == std::ceil(std::log2(maxRelativeGridSize)));

          // depth calculation
          if constexpr (LOOSE_FACTOR != 2.0)
          {
            while (true)
            {
              typename IGM::Box looseCellBox;

              auto const cellGridSize = GridID(1) << levelID;
              static_for<DIMENSION_NO>([&](auto dimensionID) noexcept {
                auto const cellGridMin = (boxCenterGrid[dimensionID] / cellGridSize) * cellGridSize;

                auto const cellGridCenter = cellGridMin + cellGridSize / GridID(2);
                auto const cellCenterWorld = RasterizationFloat(cellGridCenter) / rasterizerFactors[dimensionID];
                auto const halfLooseCellSizeWorld =
                  RasterizationFloat(cellGridSize) * RasterizationFloat(LOOSE_FACTOR * 0.5) / rasterizerFactors[dimensionID];

                looseCellBox.Min[dimensionID] = IGM_Geometry(cellCenterWorld - halfLooseCellSizeWorld);
                looseCellBox.Max[dimensionID] = IGM_Geometry(cellCenterWorld + halfLooseCellSizeWorld);
              });

              if (IGM::DoesRangeContainBoxAD(looseCellBox, box))
                break;

              ++levelID;
            }
          }

          return { boxCenterGrid, levelID };
        },
        m_rasterization);
    }

  private:
    struct NativeRasterizationFactors
    {
      using Float = IGM::Geometry;

      IGM::Vector sizeInDimensions = {};
      IGM::Vector rasterizerFactors = {};
      IGM::Vector derasterizerFactors = {};

      template<typename TArchive>
      void serialize(TArchive& ar)
      {
        ar& ORTHOTREE_NVP_INT(sizeInDimensions);
        ar& ORTHOTREE_NVP_INT(rasterizerFactors);
        ar& ORTHOTREE_NVP_INT(derasterizerFactors);
      }
    };

    // float32_t with large maxDepthID would lead to precision issues, increase double precision is required.
    struct ForcedDoubleRasterizationFactors
    {
      using Float = double;

      std::array<double, GA::DIMENSION_NO> sizeInDimensions = {};
      std::array<double, GA::DIMENSION_NO> rasterizerFactors = {};
      std::array<double, GA::DIMENSION_NO> derasterizerFactors = {};

      template<typename TArchive>
      void serialize(TArchive& ar)
      {
        ar& ORTHOTREE_NVP_INT(sizeInDimensions);
        ar& ORTHOTREE_NVP_INT(rasterizerFactors);
        ar& ORTHOTREE_NVP_INT(derasterizerFactors);
      }
    };

  private:
    GridID m_maxRasterResolution = {};
    GridID m_maxRasterID = {};

    IGM::Geometry m_volumeOfOverallSpace = {};
    IGM::Box m_boxSpace = {};
    IGM::Vector m_sizeInDimensions = {};

    std::variant<NativeRasterizationFactors, ForcedDoubleRasterizationFactors> m_rasterization = {};
  };
} // namespace OrthoTree::detail


namespace OrthoTree::detail
{

  template<dim_t DIMENSION_NO, depth_t MAX_ALLOWED_DEPTH_ID>
  struct MortonSpaceIndexing
  {
#ifdef ORTHOTREE__LOCATIONCODE_32
    static auto constexpr IS_32BIT_LOCATION = DIMENSION_NO <= 3;
#else
    static auto constexpr IS_32BIT_LOCATION = DIMENSION_NO <= 2;
#endif
    static auto constexpr IS_64BIT_LOCATION = !IS_32BIT_LOCATION && DIMENSION_NO < 15;

    // Indexing can be solved with integral types (above this, internal container will be changed to std::map)
    static auto constexpr IS_LINEAR_TREE = IS_32BIT_LOCATION || IS_64BIT_LOCATION;

    static auto constexpr MAX_NONLINEAR_DEPTH_ID = depth_t{ 4 };

    using UnderlyingInt = std::conditional_t<IS_32BIT_LOCATION, uint32_t, uint64_t>;
    using ChildID = UnderlyingInt;

    // Max number of children
    static auto constexpr CHILD_NO = detail::pow2_ce<DIMENSION_NO, ChildID>();

    // Mask for child bit part
    static auto constexpr CHILD_MASK = detail::pow2_ce<DIMENSION_NO, ChildID>() - 1;

    // Max value: 2 ^ nDepth ^ DIMENSION_NO * 2 (signal bit)
    using LinearLocationID = UnderlyingInt;
    using NonLinearLocationID = bitset_arithmetic<DIMENSION_NO * MAX_NONLINEAR_DEPTH_ID + 1>;
    using LocationID = typename std::conditional_t<IS_LINEAR_TREE, LinearLocationID, NonLinearLocationID>;
    using NodeID = LocationID; // same as the LocationID, but depth is signed by a sentinel bit.
    using LocationIDCR = typename std::conditional_t<IS_LINEAR_TREE, LocationID const, LocationID const&>;
    using NodeIDCR = LocationIDCR;
    template<typename T>
    using DimArray = std::array<T, DIMENSION_NO>;

    // Type system determined maximal depth id due to the resolution.
    static auto constexpr MAX_THEORETICAL_DEPTH_ID =
      IS_LINEAR_TREE ? static_cast<depth_t>((CHAR_BIT * sizeof(NodeID) - 1 /*sentinel bit*/)) / DIMENSION_NO : MAX_NONLINEAR_DEPTH_ID;

  private:
    struct GeneralLocationData
    {
      LocationID locationID;
      depth_t depthID;

      constexpr GeneralLocationData() noexcept = default;
      constexpr GeneralLocationData(LocationID locationID, depth_t depthID) noexcept
      : locationID(locationID)
      , depthID(depthID)
      {}

      constexpr depth_t GetDepthID() const noexcept { return depthID; }
      constexpr LocationID GetLocationID() const noexcept { return locationID; }

      template<bool IS_ELEMENT_DEPTH_SPECIFIC = true>
      static constexpr bool IsLess(GeneralLocationData const& leftLocation, GeneralLocationData const& rightLocation) noexcept
      {
        if constexpr (IS_ELEMENT_DEPTH_SPECIFIC)
        {
          auto const leftLocationID = leftLocation.locationID;
          auto const rightLocationID = rightLocation.locationID;
          return (leftLocationID < rightLocationID) || ((leftLocationID == rightLocationID) && (leftLocation.depthID < rightLocation.depthID));
        }
        else
        {
          return leftLocation.locationID < rightLocation.locationID;
        }
      }

      constexpr bool operator==(GeneralLocationData rhs) const noexcept { return locationID == rhs.locationID && depthID == rhs.depthID; }
    };

    struct CompactLocationData
    {
    private:
      static constexpr std::size_t DEPTH_ID_BIT_COUNT = 5;
      static constexpr uint64_t DEPTH_ID_MASK = ((1ULL << DEPTH_ID_BIT_COUNT) - 1);

      // upper 59 bits are reserved for location id -> 19 levels for 3D
      // lower 5 bits are reserved for depth id
      uint64_t locationAndDepthID;

    public:
      constexpr CompactLocationData() noexcept = default;
      constexpr CompactLocationData(LocationID locationID, uint64_t depthID) noexcept
      : locationAndDepthID((locationID << DEPTH_ID_BIT_COUNT) | depthID)
      {}

      constexpr depth_t GetDepthID() const noexcept { return (locationAndDepthID & DEPTH_ID_MASK); }
      constexpr LocationID GetLocationID() const noexcept { return locationAndDepthID >> DEPTH_ID_BIT_COUNT; }

      template<bool IS_ELEMENT_DEPTH_SPECIFIC = true>
      static constexpr bool IsLess(CompactLocationData const& leftLocation, CompactLocationData const& rightLocation) noexcept
      {
        return leftLocation.locationAndDepthID < rightLocation.locationAndDepthID;
      }

      constexpr bool operator==(CompactLocationData rhs) const noexcept { return locationAndDepthID == rhs.locationAndDepthID; }
    };

  public:
    struct Location
    {
    private:
      static constexpr bool IS_COMPACT_STORAGE = MAX_ALLOWED_DEPTH_ID <= 19 && DIMENSION_NO == 3;
      using LocationData = std::conditional_t<IS_COMPACT_STORAGE, CompactLocationData, GeneralLocationData>;

      LocationData locationData;

    public:
      constexpr Location() noexcept = default;
      constexpr Location(LocationID locationID, depth_t depthID) noexcept
      : locationData(locationID, depthID)
      {}

      constexpr depth_t GetDepthID() const noexcept { return locationData.GetDepthID(); }
      constexpr LocationID GetLocationID() const noexcept { return locationData.GetLocationID(); }

      constexpr bool operator<(Location const& rightLocation) const noexcept { return IsLess<true>(*this, rightLocation); }

      constexpr bool operator==(Location const& rightLocation) const noexcept { return locationData == rightLocation.locationData; }

      template<bool IS_ELEMENT_DEPTH_SPECIFIC = true>
      static constexpr bool IsLess(Location const& leftLocation, Location const& rightLocation) noexcept
      {
        return LocationData::template IsLess<IS_ELEMENT_DEPTH_SPECIFIC>(leftLocation.locationData, rightLocation.locationData);
      }
    };

    static constexpr Location GetRootLocation() noexcept { return Location(LocationID{}, 0); }

    class ChildCheckerFixedDepth
    {
    public:
      constexpr ChildCheckerFixedDepth(depth_t examinedLevel, LocationIDCR locationID) noexcept
      : m_mask((LocationID(CHILD_MASK)) << (examinedLevel * DIMENSION_NO))
      , m_childFlag(locationID & m_mask)
      {}

      ChildID GetChildID(depth_t examinedLevel) const noexcept { return CastMortonIDToChildID(m_childFlag >> (examinedLevel * DIMENSION_NO)); }

      constexpr bool Test(LocationIDCR locationID) const noexcept { return (locationID & m_mask) == m_childFlag; }

    private:
      LocationID m_mask;
      LocationID m_childFlag;
    };

    class ChildKeyGenerator
    {
    public:
      constexpr ChildKeyGenerator() noexcept = default;
      explicit constexpr ChildKeyGenerator(NodeIDCR parentNodeKey) noexcept
      : m_parentFlag(parentNodeKey << DIMENSION_NO)
      {}

      constexpr ChildKeyGenerator(ChildKeyGenerator const&) noexcept = default;
      constexpr ChildKeyGenerator(ChildKeyGenerator&&) noexcept = default;

      constexpr NodeID GetChildNodeKey(ChildID childID) const noexcept { return m_parentFlag | NodeID(childID); }

    private:
      NodeID m_parentFlag = {};
    };

    static constexpr NodeID GetNodeID(LocationIDCR locationIDOnDepth, depth_t depthID) noexcept
    {
      assert(locationIDOnDepth < (NodeID(1) << (depthID * DIMENSION_NO)));
      return (NodeID{ 1 } << (depthID * DIMENSION_NO)) | locationIDOnDepth;
    }

    template<typename T>
    static constexpr NodeID GetNodeID(T&& location, depth_t maxDepthID) noexcept
      requires requires(T t) {
        t.GetDepthID();
        t.GetLocationID();
      }
    {
      auto depthID = location.GetDepthID();
      auto locationID = location.GetLocationID();
      return (NodeID{ 1 } << (depthID * DIMENSION_NO)) | (locationID >> ((maxDepthID - depthID) * DIMENSION_NO));
    }


    static constexpr NodeID GetNodeID(LocationIDCR locationID, depth_t depthID, depth_t maxDepthID) noexcept
    {
      assert((locationID >> (maxDepthID * DIMENSION_NO)) == 0);
      return (NodeID{ 1 } << (depthID * DIMENSION_NO)) | (locationID >> ((maxDepthID - depthID) * DIMENSION_NO));
    }

    static constexpr NodeID GetRootKey() noexcept { return NodeID{ 1 }; }

    static constexpr NodeID GetNoneKey() noexcept { return NodeID{ 0 }; }

    static constexpr bool IsValidKey(LinearLocationID key) noexcept { return key > 0; }

    static bool IsValidKey(NonLinearLocationID const& key) noexcept { return key.any(); }

    static constexpr NodeID GetParentKey(NodeIDCR key) noexcept { return key >> DIMENSION_NO; }
    static constexpr bool IsParentKey(NodeIDCR childKey, NodeIDCR possibleAncestorKey) noexcept
    {
      auto const childDepth = GetDepthID(childKey);
      auto const ancestorDepth = GetDepthID(possibleAncestorKey);

      if (ancestorDepth >= childDepth)
        return false;

      const auto shift = (childDepth - ancestorDepth) * DIMENSION_NO;

      return (childKey >> shift) == possibleAncestorKey;
    }

    static constexpr depth_t GetDepthID(NodeID key) noexcept
    {
      if constexpr (IS_LINEAR_TREE)
      {
        depth_t const usedBitNo = std::bit_width(key) - 1;
        return usedBitNo / DIMENSION_NO;
      }
      else
      {
        for (depth_t d = 0; IsValidKey(key); ++d, key = GetParentKey(key))
          if (key == 1) // If only sentinel bit remains, exit with node depth
            return d;

        CRASH("Bad key! Internal error!"); // Bad key
      }
    }

    static constexpr NodeID RemoveSentinelBit(NodeIDCR key, std::optional<depth_t> depthIDOptional = std::nullopt) noexcept
    {
      if constexpr (IS_LINEAR_TREE)
      {
        auto const sentinelBitPosition = std::bit_width(key) - 1;
        return key - (NodeID{ 1 } << sentinelBitPosition);
      }
      else
      {
        auto const depthID = depthIDOptional.has_value() ? depthIDOptional.value() : GetDepthID(key);
        auto const sentinelBitPosition = depthID * DIMENSION_NO;
        return key - (NodeID{ 1 } << sentinelBitPosition);
      }
    }

    static constexpr LocationID GetLocationIDOnExaminedLevel(LocationIDCR locationID, depth_t examinationLevel) noexcept
    {
      return locationID >> (examinationLevel * DIMENSION_NO);
    }

    static constexpr bool IsAllChildTouched(std::array<LocationID, 2> const& locationIDRange, depth_t examinationLevel) noexcept
    {
      return IsValidKey((locationIDRange[1] - locationIDRange[0]) >> (examinationLevel * DIMENSION_NO - 1));
    }


    static constexpr bool IsAllChildTouched(LocationIDCR locationDifference, depth_t examinationLevel) noexcept
    {
      return (CastMortonIDToChildID(locationDifference >> ((examinationLevel - 1) * DIMENSION_NO)) ^ CHILD_MASK) == 0;
    }

    static constexpr bool IsAllChildTouched(ChildID touchedDimensionsFlag) noexcept { return touchedDimensionsFlag == CHILD_MASK; }

    static constexpr NodeID GetLowestCommonAncestor(NodeID nodeID1, NodeID nodeID2) noexcept
    {
      if (nodeID1 == nodeID2)
        return nodeID1;

      auto const depthID1 = GetDepthID(nodeID1);
      auto const depthID2 = GetDepthID(nodeID2);
      if (depthID1 > depthID2)
        nodeID1 >>= (depthID1 - depthID2) * DIMENSION_NO;
      else if (depthID2 > depthID1)
        nodeID2 >>= (depthID2 - depthID1) * DIMENSION_NO;

      NodeID const differentNodeBits = nodeID1 ^ nodeID2;
      auto const differentBitWidth = detail::bit_width(differentNodeBits);

      // ceil
      auto const differentLevelNum = (differentBitWidth + DIMENSION_NO - 1) / DIMENSION_NO;
      return nodeID1 >> (differentLevelNum * DIMENSION_NO);
    }

    static constexpr NodeID GetLowestCommonAncestor(Location location1, Location location2, depth_t maxDepthID) noexcept
    {
      if (location1 == location2)
        return GetNodeID(location1, maxDepthID);

      auto const depthID1 = location1.GetDepthID();
      auto const depthID2 = location2.GetDepthID();

      auto locationID = location1.GetLocationID();
      LocationID const differentNodeBits = locationID ^ location2.GetLocationID();
      auto const differentBitWidth = detail::bit_width(differentNodeBits);

      // ceil
      auto const differentLevelNum = depth_t((differentBitWidth + DIMENSION_NO - 1) / DIMENSION_NO);
      auto depthID = std::min(std::min(depthID1, depthID2), maxDepthID - differentLevelNum);
      auto levelID = maxDepthID - depthID;
      return GetNodeID(locationID >> (levelID * DIMENSION_NO), depthID);
    }

    template<bool IS_ELEMENT_DEPTH_SPECIFIC = true>
    class LowestCommonAncestorCalculator
    {
    public:
      constexpr LowestCommonAncestorCalculator() noexcept = default;

      constexpr explicit LowestCommonAncestorCalculator(Location location) noexcept
      : m_minDepthID(location.GetDepthID())
      , m_base(location.GetLocationID())
      {}

      constexpr void Add(Location location) noexcept
      {
        if constexpr (IS_ELEMENT_DEPTH_SPECIFIC)
          m_minDepthID = std::min(m_minDepthID, location.GetDepthID());

        m_diff |= (m_base ^ location.GetLocationID());
      }

      constexpr Location GetLocation(depth_t maxDepthID) const noexcept
      {
        auto const levelIDDiff = (detail::bit_width(m_diff) + DIMENSION_NO - 1) / DIMENSION_NO;
        auto const depthID = std::min(m_minDepthID, maxDepthID - levelIDDiff);

        return Location(m_base, depthID);
      }

      constexpr NodeID GetNodeID(depth_t maxDepthID) const noexcept
      {
        auto const levelIDDiff = (detail::bit_width(m_diff) + DIMENSION_NO - 1) / DIMENSION_NO;
        auto const depthID = std::min(m_minDepthID, maxDepthID - levelIDDiff);
        assert(levelIDDiff <= maxDepthID);
        assert(depthID <= m_minDepthID);

        return (NodeID{ 1 } << (depthID * DIMENSION_NO)) | (m_base >> ((maxDepthID - depthID) * DIMENSION_NO));
      }

    private:
      depth_t m_minDepthID = std::numeric_limits<depth_t>::max();
      LocationID m_base = {};
      LocationID m_diff = {};
    };

  private: // Morton aid functions
    // Separates low 16/32 bits of input by 1 bit
    static constexpr LocationID Part1By1(GridID gridID) noexcept
    {
      static_assert(sizeof(GridID) == 4);

      auto locationID = LocationID{ gridID };
      if constexpr (sizeof(LocationID) == 4)
      {
        // 15 bits can be used
        // n = ----------------fedcba9876543210 : Bits initially
        // n = --------fedcba98--------76543210 : After (1)
        // n = ----fedc----ba98----7654----3210 : After (2)
        // n = --fe--dc--ba--98--76--54--32--10 : After (3)
        // n = -f-e-d-c-b-a-9-8-7-6-5-4-3-2-1-0 : After (4)
        locationID = (locationID ^ (locationID << 8)) & LocationID{ 0x00ff00ff }; // (1)
        locationID = (locationID ^ (locationID << 4)) & LocationID{ 0x0f0f0f0f }; // (2)
        locationID = (locationID ^ (locationID << 2)) & LocationID{ 0x33333333 }; // (3)
        locationID = (locationID ^ (locationID << 1)) & LocationID{ 0x55555555 }; // (4)
      }
      else if constexpr (sizeof(LocationID) == 8)
      {
        // 31 bits can be used
        // n = --------------------------------xytsrqponmlkjihgfedcba9876543210 : Bits initially
        // n = ----------------xytsrqponmlkjihg----------------fedcba9876543210 : After (1)
        // n = ----xyts----rqpo----nmlk----jihg----fedc----ba98----7654----3210 : After (2)
        // n = --xy--ts--rq--po--nm--lk--ji--hg--fe--dc--ba--98--76--54--32--10 : After (3)
        // n = -x-y-t-s-r-q-p-o-n-m-l-k-j-i-h-g-f-e-d-c-b-a-9-8-7-6-5-4-3-2-1-0 : After (4)
        locationID = (locationID ^ (locationID << 16)) & LocationID{ 0x0000ffff0000ffff }; // (1)
        locationID = (locationID ^ (locationID << 8)) & LocationID{ 0x00ff00ff00ff00ff };  // (2)
        locationID = (locationID ^ (locationID << 4)) & LocationID{ 0x0f0f0f0f0f0f0f0f };  // (3)
        locationID = (locationID ^ (locationID << 2)) & LocationID{ 0x3333333333333333 };  // (4)
        locationID = (locationID ^ (locationID << 1)) & LocationID{ 0x5555555555555555 };  // (5)
      }
      else
      {
        static_assert(sizeof(LocationID) == 4 || sizeof(LocationID) == 8, "Unsupported LocationID size");
      }

      return locationID;
    }

    // Separates low 16/32 bits of input by 2 bit
    static constexpr LocationID Part1By2(GridID gridID) noexcept
    {
      static_assert(sizeof(GridID) == 4);

      auto locationID = LocationID{ gridID };
      if constexpr (sizeof(LocationID) == 4)
      {
        // 10 bits can be used
        // n = ----------------------9876543210 : Bits initially
        // n = ------98----------------76543210 : After (1)
        // n = ------98--------7654--------3210 : After (2)
        // n = ------98----76----54----32----10 : After (3)
        // n = ----9--8--7--6--5--4--3--2--1--0 : After (4)
        locationID = (locationID ^ (locationID << 16)) & LocationID{ 0xff0000ff }; // (1)
        locationID = (locationID ^ (locationID << 8)) & LocationID{ 0x0300f00f };  // (2)
        locationID = (locationID ^ (locationID << 4)) & LocationID{ 0x030c30c3 };  // (3)
        locationID = (locationID ^ (locationID << 2)) & LocationID{ 0x09249249 };  // (4)
      }
      else if constexpr (sizeof(LocationID) == 8)
      {
        // 21 bits can be used
        // n = -------------------------------------------lkjhgfedcba9876543210 : Bits initially
        // n = -----------lkjhg--------------------------------fedcba9876543210 : After (1)
        // n = -----------lkjhg----------------fedcba98----------------76543210 : After (2)
        // n = ---l--------kjhg--------fedc--------ba98--------7654--------3210 : After (3)
        // n = ---l----kj----hg----fe----dc----ba----98----76----54----32----10 : After (4)
        // n = ---l--k--j--i--h--g--f--e--d--c--b--a--9--7--6--5--4--3--2--1--0 : After (5)
        locationID = (locationID ^ (locationID << 32)) & LocationID{ 0xffff00000000ffff }; // (1)
        locationID = (locationID ^ (locationID << 16)) & LocationID{ 0x00ff0000ff0000ff }; // (2)
        locationID = (locationID ^ (locationID << 8)) & LocationID{ 0xf00f00f00f00f00f };  // (3)
        locationID = (locationID ^ (locationID << 4)) & LocationID{ 0x30c30c30c30c30c3 };  // (4)
        locationID = (locationID ^ (locationID << 2)) & LocationID{ 0x9249249249249249 };  // (5)
      }
      else
      {
        static_assert(sizeof(LocationID) == 4 || sizeof(LocationID) == 8, "Unsupported LocationID size");
      }

      return locationID;
    }

    static consteval LocationID GetBitPattern()
    {
      constexpr auto size = sizeof(LocationID) * CHAR_BIT;
      constexpr auto maxDepth = (size - 1) / DIMENSION_NO;

      auto bitPattern = LocationID{ 0 };
      auto shift = LocationID{ 0 };
      for (dim_t depthID = 0; depthID < maxDepth; ++depthID, shift += DIMENSION_NO)
        bitPattern |= LocationID{ 1 } << shift;

      return bitPattern;
    }

    static consteval std::array<LocationID, DIMENSION_NO> GetBitPatterns()
    {
      constexpr auto bitPattern = GetBitPattern();

      std::array<LocationID, DIMENSION_NO> bitPatterns;
      for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
        bitPatterns[dimensionID] = bitPattern << dimensionID;

      return bitPatterns;
    }

  public:
    static LocationID Encode(DimArray<GridID> const& gridID) noexcept
    {
      if constexpr (DIMENSION_NO == 1)
      {
        return LocationID(gridID[0]);
      }
      else if constexpr (DIMENSION_NO == 2)
      {
#ifdef ORTHOTREE_BMI2_PDEP_AVAILABLE
        if constexpr (IS_32BIT_LOCATION)
        {
          return _pdep_u32(gridID[1], 0b10101010'10101010'10101010'10101010) | _pdep_u32(gridID[0], 0b01010101'01010101'01010101'01010101);
        }
        else
        {
          return (Part1By1(gridID[1]) << 1) + Part1By1(gridID[0]);
        }
#else
        return (Part1By1(gridID[1]) << 1) + Part1By1(gridID[0]);
#endif
      }
      else if constexpr (DIMENSION_NO == 3)
      {
#ifdef ORTHOTREE_BMI2_PDEP_AVAILABLE
        if constexpr (IS_32BIT_LOCATION)
        {
          return _pdep_u32(gridID[2], 0b00100100'10010010'01001001'00100100) | _pdep_u32(gridID[1], 0b10010010'01001001'00100100'10010010) |
                 _pdep_u32(gridID[0], 0b01001001'00100100'10010010'01001001);
        }
        else
        {
          static constexpr auto bitPatterns = GetBitPatterns();
          return _pdep_u64(gridID[2], bitPatterns[2]) | _pdep_u64(gridID[1], bitPatterns[1]) | _pdep_u64(gridID[0], bitPatterns[0]);
        }
#else
        return (Part1By2(gridID[2]) << 2) + (Part1By2(gridID[1]) << 1) + Part1By2(gridID[0]);
#endif
      }
#ifdef ORTHOTREE_BMI2_PDEP_AVAILABLE
      else if constexpr (IS_64BIT_LOCATION)
      {
        static constexpr auto bitPatterns = GetBitPatterns();

        auto locationID = LocationID{};
        for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
          locationID |= _pdep_u64(gridID[dimensionID], bitPatterns[dimensionID]);

        return locationID;
      }
#endif
      else
      {
        auto msb = gridID[0];
        for (dim_t dimensionID = 1; dimensionID < DIMENSION_NO; ++dimensionID)
          msb |= gridID[dimensionID];

        LocationID locationID = 0;
        GridID mask = 1;
        for (dim_t i = 0; msb; mask <<= 1, msb >>= 1, ++i)
        {
          ORTHOTREE_LOOPIVDEP
          for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
          {
            auto const shift = dimensionID + i * DIMENSION_NO;
            if constexpr (IS_LINEAR_TREE)
            {
              locationID |= static_cast<LocationID>(gridID[dimensionID] & mask) << (shift - i);
            }
            else
            {
              locationID.set(shift, gridID[dimensionID] & mask);
            }
          }
        }
        return locationID;
      }
    }

    static DimArray<GridID> Decode(NodeIDCR nodeKey, depth_t maxDepthID) noexcept
    {
      auto const depthID = GetDepthID(nodeKey);
      auto gridID = DimArray<GridID>{};
      if constexpr (DIMENSION_NO == 1)
      {
        auto const examinationLevelID = maxDepthID - depthID;
        gridID[0] = GridID(RemoveSentinelBit(nodeKey) << examinationLevelID);
      }
#ifdef ORTHOTREE_BMI2_PDEP_AVAILABLE
      else if constexpr (IS_LINEAR_TREE)
      {
        static constexpr auto bitPatterns = GetBitPatterns();

        auto const examinationLevelID = maxDepthID - depthID;
        const auto locationID = RemoveSentinelBit(nodeKey) << examinationLevelID * DIMENSION_NO;
        for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
        {
          if constexpr (IS_32BIT_LOCATION)
          {
            gridID[dimensionID] = GridID(_pext_u32(locationID, bitPatterns[dimensionID]));
          }
          else
          {
            gridID[dimensionID] = GridID(_pext_u64(locationID, bitPatterns[dimensionID]));
          }
        }
      }
#endif
      else
      {
        auto constexpr mask = LocationID{ 1 };
        for (depth_t examinationLevelID = maxDepthID - depthID, shift = 0; examinationLevelID < maxDepthID; ++examinationLevelID)
          for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID, ++shift)
          {
            if constexpr (IS_LINEAR_TREE)
            {
              gridID[dimensionID] |= ((nodeKey >> shift) & mask) << examinationLevelID;
            }
            else
            {
              gridID[dimensionID] |= GridID{ nodeKey[shift] } << examinationLevelID;
            }
          }
      }
      return gridID;
    }

    static ChildID CastMortonIDToChildID(NonLinearLocationID const& bs) noexcept
    {
      assert(bs <= NonLinearLocationID(std::numeric_limits<ChildID>::max()));
      return bs.to_ullong();
    }

    static constexpr ChildID CastMortonIDToChildID(LinearLocationID morton) noexcept { return morton; }

    static ChildID GetChildID(NodeIDCR key) noexcept
    {
      if constexpr (IS_LINEAR_TREE)
      {
        auto constexpr childMask = LocationID(CHILD_MASK);
        return CastMortonIDToChildID(key & childMask);
      }
      else
      {
        auto childID = NodeID{};
        for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
        {
          childID.set(dimensionID, key[dimensionID]);
        }

        return CastMortonIDToChildID(childID);
      }
    }

    static ChildID GetNonDirectChildID(NodeIDCR parentNodeID, NodeID childNodeID) noexcept
    {
      auto const parentDepthID = GetDepthID(parentNodeID);
      auto const childDepthID = GetDepthID(childNodeID);
      assert(childDepthID > parentDepthID);
      childNodeID >>= (childDepthID - parentDepthID - 1) * DIMENSION_NO;
      return GetChildID(childNodeID);
    }

    static constexpr ChildID GetChildID(LocationIDCR childNodeKey, depth_t examinationLevelID)
    {
      return GetChildID(childNodeKey >> (DIMENSION_NO * (examinationLevelID - 1)));
    }

    static constexpr NodeID GetDirectChildNodeID(NodeID parentNodeID, NodeID childNodeID) noexcept
    {
      auto const parentDepthID = GetDepthID(parentNodeID);
      auto const childDepthID = GetDepthID(childNodeID);
      assert(parentDepthID < childDepthID);
      auto const depthDiff = childDepthID - parentDepthID;
      if (depthDiff == 1)
        return childNodeID;
      else
        return childNodeID >> (DIMENSION_NO * (depthDiff - 1));
    }

    static constexpr ChildID GetChildIDByDepth(depth_t parentDepth, depth_t childDepth, LocationIDCR childNodeKey)
    {
      auto const depthDifference = childDepth - parentDepth;
      assert(depthDifference > 0);
      return GetChildID(childNodeKey, depthDifference);
    }

    static constexpr bool IsChildInGreaterSegment(ChildID childID, dim_t dimensionID) noexcept { return (ChildID{ 1 } << dimensionID) & childID; }

    static constexpr bool IsChildInGreaterSegment(NonLinearLocationID const& locationID, dim_t dimensionID) noexcept
    {
      return locationID[dimensionID];
    }

    static constexpr std::array<LocationID, 2> GetRangeLocationID(std::array<DimArray<GridID>, 2> const& gridIDRange) noexcept
    {
      return { Encode(gridIDRange[0]), Encode(gridIDRange[1]) };
    }

    static constexpr Location GetLocation(auto&& locationID, depth_t maxDepthID) noexcept { return Location(locationID, maxDepthID); }

    static constexpr Location GetRangeLocation(auto&& gridIDRange, depth_t maxDepthID) noexcept
    {
      if (gridIDRange[0] == gridIDRange[1])
        return Location(Encode(gridIDRange[0]), maxDepthID);

      auto locationIDRange = std::array<LocationID, 2>{ Encode(gridIDRange[0]), Encode(gridIDRange[1]) };
      auto const locationDifference = locationIDRange[0] ^ locationIDRange[1];

      depth_t levelID = 0;
      if constexpr (IS_LINEAR_TREE)
      {
        auto const differentBitNo = std::bit_width(locationDifference);
        levelID = (differentBitNo + DIMENSION_NO - 1) / DIMENSION_NO;
      }
      else
      {
        for (auto diffLocationFlag = locationDifference; diffLocationFlag != 0; diffLocationFlag >>= DIMENSION_NO)
          ++levelID;
      }

      assert(0 < levelID && levelID <= maxDepthID);

      auto const shift = levelID * DIMENSION_NO;
      return Location((locationIDRange[0] >> shift) << shift, maxDepthID - levelID);
    }
  };
} // namespace OrthoTree::detail

namespace OrthoTree::detail
{
  template<typename TGeometryAdapter, bool ALLOW_OUT_OF_SPACE_INSERTION, double LOOSE_FACTOR, depth_t MAX_ALLOWED_DEPTH_ID>
  class MortonGridSpaceIndexing : public MortonSpaceIndexing<TGeometryAdapter::DIMENSION_NO, MAX_ALLOWED_DEPTH_ID>
  {
    static constexpr dim_t DIMENSION_NO = TGeometryAdapter::DIMENSION_NO;
    using SI = MortonSpaceIndexing<DIMENSION_NO, MAX_ALLOWED_DEPTH_ID>;

  public:
    using TVector = typename TGeometryAdapter::Vector;
    using TBox = typename TGeometryAdapter::Box;
    using IGM = InternalGeometryModule<TGeometryAdapter>;

  public:
    constexpr MortonGridSpaceIndexing() = default;

    template<typename TArchive, typename TGA, bool ALLOW_OUT_SPACE, double LOOSE, depth_t MAX_DP>
    friend void serialize(TArchive& ar, MortonGridSpaceIndexing<TGA, ALLOW_OUT_SPACE, LOOSE, MAX_DP>& si);

    constexpr MortonGridSpaceIndexing(depth_t maxDepthID, typename IGM::Box const& boxSpace) noexcept
    : m_grid(GridSpaceIndexing<TGeometryAdapter>(maxDepthID, boxSpace))
    , m_maxDepthID(maxDepthID)
    {}

  public:
    constexpr typename SI::Location GetInvalidLocation() const noexcept
    {
      if constexpr (ALLOW_OUT_OF_SPACE_INSERTION)
      {
        return typename SI::Location(typename SI::LocationID(), 0);
      }
      else
      {
        return typename SI::Location(typename SI::LocationID(), INVALID_DEPTH);
      }
    }

    constexpr typename SI::Location GetLocation(TVector const& point) const noexcept
    {
      auto gridID = m_grid.GetPointGridID(point);
      if (gridID[0] == INVALID_GRIDID) [[unlikely]]
        return GetInvalidLocation();

      return typename SI::Location(SI::Encode(gridID), m_maxDepthID);
    }

    constexpr typename SI::Location GetLocation(TBox const& box) const noexcept
    {
      if constexpr (LOOSE_FACTOR > 1.0)
      {
        auto [centerGridID, levelID] = m_grid.template GetLooseBoxGridData<LOOSE_FACTOR>(box);
        if (centerGridID[0] == INVALID_GRIDID) [[unlikely]]
          return GetInvalidLocation();

        auto const shift = levelID * DIMENSION_NO;
        return typename SI::Location(SI::Encode(centerGridID) >> shift << shift, m_maxDepthID - levelID);
      }
      else
      {
        auto boxGrid = m_grid.GetBoxGridID(box);
        if (boxGrid[0][0] == INVALID_GRIDID) [[unlikely]]
          return GetInvalidLocation();

        return SI::GetRangeLocation(boxGrid, m_maxDepthID);
      }
    }

    constexpr typename SI::NodeID GetNodeID(TVector const& point) const noexcept
    {
      auto gridID = m_grid.GetPointGridID(point);
      if (gridID[0] == INVALID_GRIDID) [[unlikely]]
        return {};

      return SI::GetNodeID(SI::Encode(gridID), m_maxDepthID);
    }

    constexpr typename SI::NodeID GetNodeID(TBox const& box) const noexcept
    {
      auto location = GetLocation(box);
      if (location.GetDepthID() == INVALID_DEPTH) [[unlikely]]
        return {};

      return SI::GetNodeID(location, m_maxDepthID);
    }

    constexpr typename SI::NodeID GetNodeID(SI::Location const& location) const noexcept { return SI::GetNodeID(location, m_maxDepthID); }

    constexpr decltype(auto) GetMinPoint() const noexcept { return m_grid.GetMinPoint(); }

    constexpr decltype(auto) GetSize() const noexcept { return m_grid.GetSize(); }

    constexpr IGM::Vector CalculateNodeCenter(SI::NodeIDCR nodeID) const noexcept
    {
      return m_grid.CalculateGridCellCenter(SI::Decode(nodeID, m_maxDepthID), m_maxDepthID - SI::GetDepthID(nodeID));
    }

    constexpr void Move(TVector const& move) noexcept { m_grid.Move(move); }

  private:
    GridSpaceIndexing<TGeometryAdapter> m_grid;
    depth_t m_maxDepthID;
  };

} // namespace OrthoTree::detail


namespace OrthoTree::detail
{
  template<typename It1, typename It2>
  class proxy_reference
  {
  private:
    using T1 = typename std::iterator_traits<It1>::value_type;
    using T2 = typename std::iterator_traits<It2>::value_type;
    using R1 = typename std::iterator_traits<It1>::reference;
    using R2 = typename std::iterator_traits<It2>::reference;
    using value_type = std::pair<T1, T2>;
    using reference = std::pair<R1, R2>;

  private:
    constexpr proxy_reference() = default;

  public:
    constexpr proxy_reference(It1 it1, It2 it2) noexcept
    : m_it1(it1)
    , m_it2(it2)
    {}

    constexpr proxy_reference(const proxy_reference&) noexcept = default;
    constexpr proxy_reference(proxy_reference&&) noexcept = default;

    constexpr proxy_reference& operator=(const proxy_reference& right) noexcept
    {
      *m_it1 = *right.m_it1;
      *m_it2 = *right.m_it2;
      return *this;
    }

    constexpr proxy_reference& operator=(proxy_reference&& right) noexcept
    {
      *m_it1 = std::move(*right.m_it1);
      *m_it2 = std::move(*right.m_it2);
      return *this;
    }

    constexpr proxy_reference& operator=(const value_type& val) noexcept
    {
      *m_it1 = val.first;
      *m_it2 = val.second;
      return *this;
    }

    constexpr proxy_reference& operator=(value_type&& val) noexcept
    {
      *m_it1 = std::move(val.first);
      *m_it2 = std::move(val.second);
      return *this;
    }

    constexpr operator value_type() const& noexcept { return { *m_it1, *m_it2 }; }
    constexpr operator value_type() && noexcept { return { std::move(*m_it1), std::move(*m_it2) }; }

    constexpr R1 const GetFirst() const noexcept { return *m_it1; }
    constexpr R1 GetFirst() noexcept { return *m_it1; }
    constexpr R2 const GetSecond() const noexcept { return *m_it2; }
    constexpr R2 GetSecond() noexcept { return *m_it2; }

    friend constexpr void swap(proxy_reference left, proxy_reference right) noexcept
    {
      auto val1 = std::move(*left.m_it1);
      *left.m_it1 = std::move(*right.m_it1);
      *right.m_it1 = std::move(val1);

      auto val2 = std::move(*left.m_it2);
      *left.m_it2 = std::move(*right.m_it2);
      *right.m_it2 = std::move(val2);
    }

    // Tuple protocol for std::get compatibility
    template<std::size_t I>
    friend constexpr auto get(proxy_reference const& p) noexcept -> std::conditional_t<I == 0, R1, R2>
    {
      if constexpr (I == 0)
        return p.GetFirst();
      else
        return p.GetSecond();
    }
    template<std::size_t I>
    friend constexpr auto get(proxy_reference& p) noexcept -> std::conditional_t<I == 0, R1, R2>
    {
      if constexpr (I == 0)
        return p.GetFirst();
      else
        return p.GetSecond();
    }
    template<std::size_t I>
    friend constexpr auto get(proxy_reference&& p) noexcept -> std::conditional_t<I == 0, R1, R2>
    {
      if constexpr (I == 0)
        return p.GetFirst();
      else
        return p.GetSecond();
    }

  private:
    It1 m_it1;
    It2 m_it2;
  };

  template<typename It1, typename It2>
  class zip_iterator
  {
  public:
    using iterator_concept = std::random_access_iterator_tag;
    using iterator_category = std::random_access_iterator_tag;

    using difference_type = std::common_type_t<typename std::iterator_traits<It1>::difference_type, typename std::iterator_traits<It2>::difference_type>;
    using pointer = void;

    using reference = proxy_reference<It1, It2>;
    using value_type = std::pair<typename std::iterator_traits<It1>::value_type, typename std::iterator_traits<It2>::value_type>;

    constexpr zip_iterator() noexcept = default;
    constexpr zip_iterator(It1 it1, It2 it2) noexcept
    : it1_(it1)
    , it2_(it2)
    {}

    // constexpr reference operator*() noexcept { return reference(it1_, it2_); }
    constexpr auto operator*() const noexcept { return proxy_reference<It1, It2>{ it1_, it2_ }; }
    // constexpr reference operator*() const noexcept { return reference(it1_, it2_); }
    constexpr It1 GetFirst() const noexcept { return it1_; }
    constexpr It2 GetSecond() const noexcept { return it2_; }

    constexpr zip_iterator& operator++() noexcept
    {
      ++it1_;
      ++it2_;
      return *this;
    }

    constexpr zip_iterator operator++(int) noexcept
    {
      auto tmp = *this;
      ++(*this);
      return tmp;
    }

    constexpr zip_iterator& operator--() noexcept
    {
      --it1_;
      --it2_;
      return *this;
    }
    constexpr zip_iterator operator--(int) noexcept
    {
      auto tmp = *this;
      --(*this);
      return tmp;
    }

    constexpr zip_iterator& operator+=(difference_type n) noexcept
    {
      it1_ += n;
      it2_ += n;
      return *this;
    }
    constexpr zip_iterator& operator-=(difference_type n) noexcept
    {
      it1_ -= n;
      it2_ -= n;
      return *this;
    }

    constexpr zip_iterator operator+(difference_type n) const noexcept { return zip_iterator(it1_ + n, it2_ + n); }
    constexpr zip_iterator operator-(difference_type n) const noexcept { return zip_iterator(it1_ - n, it2_ - n); }
    constexpr difference_type operator-(const zip_iterator& other) const noexcept { return it1_ - other.it1_; }
    friend constexpr zip_iterator operator+(difference_type n, zip_iterator it) noexcept { return it + n; }

    constexpr reference operator[](difference_type n) noexcept { return *(*this + n); }
    constexpr reference operator[](difference_type n) const noexcept { return *(*this + n); }

    constexpr bool operator==(const zip_iterator& other) const noexcept { return it1_ == other.it1_; }
    constexpr bool operator!=(const zip_iterator& other) const noexcept { return !(*this == other); }
    constexpr bool operator<(const zip_iterator& other) const noexcept { return it1_ < other.it1_; }
    constexpr bool operator>(const zip_iterator& other) const noexcept { return it1_ > other.it1_; }
    constexpr bool operator<=(const zip_iterator& other) const noexcept { return it1_ <= other.it1_; }
    constexpr bool operator>=(const zip_iterator& other) const noexcept { return it1_ >= other.it1_; }
    // Required for std::iter_swap to work correctly with proxy_reference Without this, algorithms using move-based swap corrupt data because
    // proxy_reference is a reference type (points to data) not a value type.
    friend constexpr void iter_swap(zip_iterator const& left, zip_iterator const& right) noexcept
    {
      using std::swap;
      swap(*left.it1_, *right.it1_);
      swap(*left.it2_, *right.it2_);
    }
    // Required for algorithms that use iter_move (C++20 ranges)
    friend constexpr auto iter_move(zip_iterator const& it) noexcept
    {
      using T1 = typename std::iterator_traits<It1>::value_type;
      using T2 = typename std::iterator_traits<It2>::value_type;
      return std::pair<T1, T2>{ std::move(*it.it1_), std::move(*it.it2_) };
    }

  private:
    It1 it1_;
    It2 it2_;
  };


  template<typename T1, typename T2>
  class zip_view : public std::ranges::view_interface<zip_view<T1, T2>>
  {
  public:
    using It1 = typename T1::iterator;
    using It2 = typename T2::iterator;

    using iterator = zip_iterator<It1, It2>;
    using const_iterator = iterator;

    constexpr zip_view(T1& data1, T2& data2) noexcept
      requires(std::ranges::contiguous_range<T1> && std::ranges::contiguous_range<T2>)
    : m_data1(data1)
    , m_data2(data2)
    {
      assert(m_data1.size() == m_data2.size());
      assert((void*)m_data1.data() != (void*)m_data2.data());

      // overlap is not allowed
      assert((void*)(m_data1.data() + m_data1.size()) <= (void*)m_data2.data() || (void*)(m_data2.data() + m_data2.size()) <= (void*)m_data1.data());
    }

    zip_view(const zip_view& other) noexcept
    : m_data1(other.m_data1)
    , m_data2(other.m_data2)
    {
      assert(m_data1.size() == m_data2.size());
    }

    zip_view(zip_view&& other) noexcept
    : m_data1(other.m_data1)
    , m_data2(other.m_data2)
    {
      assert(m_data1.size() == m_data2.size());
    }

    zip_view& operator=(const zip_view& other) noexcept
    {
      if (this != &other)
      {
        m_data1 = other.m_data1;
        m_data2 = other.m_data2;
        assert(m_data1.size() == m_data2.size());
      }
      return *this;
    }

    zip_view& operator=(zip_view&& other) noexcept
    {
      if (this != &other)
      {
        m_data1 = other.m_data1;
        m_data2 = other.m_data2;
        assert(m_data1.size() == m_data2.size());
      }
      return *this;
    }

    constexpr iterator begin() noexcept { return iterator(m_data1.begin(), m_data2.begin()); }
    constexpr iterator begin() const noexcept { return const_iterator(m_data1.begin(), m_data2.begin()); }

    constexpr iterator end() noexcept { return iterator(m_data1.end(), m_data2.end()); }
    constexpr iterator end() const noexcept { return const_iterator(m_data1.end(), m_data2.end()); }

    constexpr std::size_t size() const noexcept { return m_data1.size(); }

  private:
    T1& m_data1;
    T2& m_data2;
  };

  static_assert(std::ranges::random_access_range<zip_view<std::vector<int>, std::vector<int>>>);
  static_assert(std::ranges::sized_range<zip_view<std::vector<int>, std::vector<int>>>);

} // namespace OrthoTree::detail


/* Settings
* Use the following define-s before the header include

// PMR is used with MSVC only by default. To use PMR anyway
#define ORTHOTREE__USE_PMR

// To disable PMR on all platforms use:
#define ORTHOTREE__DISABLE_PMR

// If the depth is less than 10, 32bit location code is enough (otherwise 64bit will be used)
#define ORTHOTREE__LOCATIONCODE_32

// Contiguous container of geometry data does not have specified index type. Octree lib uses index_t for it, it can specified to int or std::size_t.
ORTHOTREE_INDEX_T__INT / ORTHOTREE_INDEX_T__SIZE_T / ORTHOTREE_INDEX_T__UINT_FAST32_T

// By default, the node interface is not available. Use this macro to make node-related functions public.
#define ORTHOTREE__PUBLIC_NODE_INTERFACE

// By default, the linear data interface is not available. Use this macro to make linear data-related functions public on Static Orthotrees.
#define ORTHOTREE__PUBLIC_LINEARDATA_INTERFACE

// Over 4 billion elements in the tree use 64bit index type for memory resource
#define ORTHOTREE__LARGE_DATASET

// Enable debug checks
#define ORTHOTREE__DEBUG_CHECKS
*/


namespace OrthoTree::detail
{
  template<typename PmrMap>
  class EmbeddedResourcePmrMap
  {
  public:
    using MapType = PmrMap;
    using key_type = typename MapType::key_type;
    using mapped_type = typename MapType::mapped_type;
    using value_type = typename MapType::value_type;
    using size_type = typename MapType::size_type;
    using iterator = typename MapType::iterator;
    using const_iterator = typename MapType::const_iterator;
    using node_type = typename MapType::node_type;

    EmbeddedResourcePmrMap()
    : m_data(&m_resource)
    {}

    // Copy ctor: new resource, copy elements into it
    EmbeddedResourcePmrMap(const EmbeddedResourcePmrMap& other)
    : m_data(&m_resource)
    {
      m_data.insert(other.m_data.begin(), other.m_data.end());
    }

    // Copy assignment: keep own resource, copy elements
    EmbeddedResourcePmrMap& operator=(const EmbeddedResourcePmrMap& other)
    {
      if (this == &other)
        return *this;

      m_data.clear();
      m_data.insert(other.m_data.begin(), other.m_data.end());
      return *this;
    }

    // Move ctor: keep own resource, move elements into it
    EmbeddedResourcePmrMap(EmbeddedResourcePmrMap&& other) noexcept
    : m_data(&m_resource)
    {
      m_data.insert(std::make_move_iterator(other.m_data.begin()), std::make_move_iterator(other.m_data.end()));
      other.m_data.clear();
    }

    // Move assignment
    EmbeddedResourcePmrMap& operator=(EmbeddedResourcePmrMap&& other) noexcept
    {
      if (this == &other)
        return *this;

      m_data.clear();
      m_data.insert(std::make_move_iterator(other.m_data.begin()), std::make_move_iterator(other.m_data.end()));
      other.m_data.clear();
      return *this;
    }

    // Iterators
    iterator begin() noexcept { return m_data.begin(); }
    const_iterator begin() const noexcept { return m_data.begin(); }
    const_iterator cbegin() const noexcept { return m_data.cbegin(); }

    iterator end() noexcept { return m_data.end(); }
    const_iterator end() const noexcept { return m_data.end(); }
    const_iterator cend() const noexcept { return m_data.cend(); }

    // Capacity
    bool empty() const noexcept { return m_data.empty(); }
    size_type size() const noexcept { return m_data.size(); }

    // Lookup
    iterator find(const key_type& key) { return m_data.find(key); }
    const_iterator find(const key_type& key) const { return m_data.find(key); }

    mapped_type& at(const key_type& key) { return m_data.at(key); }
    const mapped_type& at(const key_type& key) const { return m_data.at(key); }

#if (defined(_MSC_VER) && _MSVC_LANG >= 202002L) || (__cplusplus >= 202002L)
    bool contains(const key_type& key) const { return m_data.contains(key); }
#endif

    // Element access
    mapped_type& operator[](const key_type& key) { return m_data[key]; }

    mapped_type& operator[](key_type&& key) { return m_data[std::move(key)]; }

    // Modifiers
    template<typename... Args>
    std::pair<iterator, bool> emplace(Args&&... args)
    {
      return m_data.emplace(std::forward<Args>(args)...);
    }

    template<typename... Args>
    std::pair<iterator, bool> try_emplace(const key_type& key, Args&&... args)
    {
      return m_data.try_emplace(key, std::forward<Args>(args)...);
    }

    template<typename... Args>
    std::pair<iterator, bool> try_emplace(key_type&& key, Args&&... args)
    {
      return m_data.try_emplace(std::move(key), std::forward<Args>(args)...);
    }

    template<typename... Args>
    iterator emplace_hint(const_iterator hint, Args&&... args)
    {
      return m_data.emplace_hint(hint, std::forward<Args>(args)...);
    }

    template<typename... Args>
    iterator try_emplace(const_iterator hint, const key_type& key, Args&&... args)
    {
      return m_data.try_emplace(hint, key, std::forward<Args>(args)...);
    }

    template<typename... Args>
    iterator try_emplace(const_iterator hint, key_type&& key, Args&&... args)
    {
      return m_data.try_emplace(hint, std::move(key), std::forward<Args>(args)...);
    }

    void erase(iterator it) { m_data.erase(it); }
    size_type erase(const key_type& key) { return m_data.erase(key); }

    void clear() noexcept { m_data.clear(); }

    // Direct access if full map API needed
    MapType& Underlying() noexcept { return m_data; }
    const MapType& Underlying() const noexcept { return m_data; }

  private:
    // Each instance owns its allocator resource
    std::pmr::unsynchronized_pool_resource m_resource;

    // Container always bound to the local resource
    MapType m_data;
  };

} // namespace OrthoTree::detail


namespace OrthoTree::detail
{
#ifdef ORTHOTREE_IS_PMR_USED
  constexpr bool USE_PMR = true;
#else
  constexpr bool USE_PMR = false;
#endif
} // namespace OrthoTree::detail


namespace OrthoTree
{
  enum class NodeGeometryStorage
  {
    None,     // Node does not contain geometry information, every geometry queries requires its calculation.
    MinPoint, // Node contains only the min-point. Node sizes are stored level-wise.
    MBR,      // Node contains the min-point and the size of the minimal bounding rectangle. It requires calculation on every insertion.
              //   Entity removal does not effect. Recommended for Loose trees, where the node overlapping would be large.
  };

  template<double IS_LOOSE_FACTOR_ = 1.0, NodeGeometryStorage NODE_GEOMETRY_STORAGE_ = NodeGeometryStorage::MinPoint, bool USE_REVERSE_MAPPING_ = false, bool USE_PMR = detail::USE_PMR>
  struct Configuration
  {
    // Geometry type cannot be mixed within the same octree.
    static constexpr bool IS_HOMOGENEOUS_GEOMETRY = true;

    // Loose tree's looseness factor. Recommended value for Box trees with ray-picking: 2.0
    static constexpr double LOOSE_FACTOR = IS_LOOSE_FACTOR_;

    // If true, out-of-handled space element will be stored in the root node. Otherwise, insertion will fail.
    static constexpr bool ALLOW_OUT_OF_SPACE_INSERTION = true;

    // If tree, Reverse mapping (entityID -> nodeID) will be stored to speed up removal and update operations. Maintaining reverse mapping has significant overhead. (Dynamic datasets)
    static constexpr bool USE_REVERSE_MAPPING = USE_REVERSE_MAPPING_;

    // It determines the internal location data type sizes.
    // In 3D, 21: (maximum allowed depth for 3D), LocationID is an uint64_t, Location's size is over 64bit. Resolution: for 1km model-space is 0.5mm
    // In 3D, 19: Location's size is 57bit. Compact location storage is used for 3D. Resolution: for 1km model-space is 20mm
    // In 3D, 10: LocationID is an uint32_t, Location's size is over 32bit. Resolution: for 1km model-space is 1m
    // In 3D, 8: LocationID is an uint32_t, Location's size is 32bit. Resolution: for 1km model-space is 4m
    static constexpr depth_t MAX_ALLOWED_DEPTH_ID = depth_t{ 19 };

    static constexpr NodeGeometryStorage NODE_GEOMETRY_STORAGE = NODE_GEOMETRY_STORAGE_;

    // Target number of elements in nodes
    static constexpr std::size_t DEFAULT_TARGET_ELEMENT_NUM_IN_NODES = 16;

    // Associative container used for node storage (default: std::unordered_map)
    // Pointer-stability is checked via detail::is_reference_stable(), non-std containers are handled as non-pointer-stable.
    template<typename TKey, typename TValue, typename THash>
    using UMapNodeContainer =
      std::conditional_t<USE_PMR, detail::EmbeddedResourcePmrMap<std::pmr::unordered_map<TKey, TValue, THash>>, std::unordered_map<TKey, TValue, THash>>;

    template<typename TKey, typename TValue, typename TComp>
    using MapNodeContainer =
      std::conditional_t<USE_PMR, detail::EmbeddedResourcePmrMap<std::pmr::map<TKey, TValue, TComp>>, std::map<TKey, TValue, TComp>>;

    // Associative container used for reverse mapping storage (default: std::unordered_map) for keyed entities
    template<typename TKey, typename TValue, typename THash>
    using ReverseMapKeyed = std::unordered_map<TKey, TValue, THash>;

    // Associative container used for reverse mapping storage (default: std::vector) for indexed entities
    template<typename TValue>
    using ReverseMapIndexed = std::vector<TValue>;
  };

  template<NodeGeometryStorage NODE_GEOMETRY_STORAGE = NodeGeometryStorage::MinPoint, bool USE_REVERSE_MAPPING = true>
  using PointConfiguration = Configuration<1.0, NODE_GEOMETRY_STORAGE, USE_REVERSE_MAPPING>;

  template<bool IS_LOOSE_TREE, NodeGeometryStorage NODE_GEOMETRY_STORAGE = NodeGeometryStorage::MBR, bool USE_REVERSE_MAPPING = true>
  using BoxConfiguration = Configuration<IS_LOOSE_TREE ? 2.0 : 1.0, NODE_GEOMETRY_STORAGE, USE_REVERSE_MAPPING>;

} // namespace OrthoTree


namespace OrthoTree
{
  enum class EntityIdStrategy
  {
    ContiguousIndex, // Index-based ID, and the tree actively maintains the continuity of indexes (e.g. by swapping elements on removal, so indexes might change).
    StableIndex, // Index-based ID, contiguous upon creation, but removals may create "holes", keeping remaining indexes stable.
    EntityKeyed  // The entity itself contains the ID.
  };

  template<GeometryType GEOMETRY_TYPE_, typename TEntity, typename TEntityID, typename TEntityContainer, typename TEntityContainerView, typename TGeometry, typename TEntityIDHash = std::hash<TEntityID>>
  struct EntityAdapterDefault
  {
  public:
    // OctreeContainer requires the TEntityContainer to eliminate data conversion
    using EntityContainer = TEntityContainer;

    // Generally, EntityContainerView is used in the Core type
    using EntityContainerView = TEntityContainerView;

    // Underlying data, that could be anything
    using Entity = TEntity;

    // Trivially copyable type, that is stored in the nodes
    using EntityID = TEntityID;

    // Geometric representation of the Entity: Point/Box
    using Geometry = TGeometry;

    // Hash to store EntityID in hashmaps
    using Hash = TEntityIDHash;

    static constexpr GeometryType GEOMETRY_TYPE = GEOMETRY_TYPE_;

  private:
    using EntityContainerViewType = std::remove_cvref_t<EntityContainerView>;

    // Whether the Entity contains its ID or not. If IS_ENTITY_KEYED = false, integer-based EntityID-s is required, because continuous indexing should be maintained in the tree.
    static constexpr bool IS_ENTITY_KEYED =
      !(std::contiguous_iterator<typename EntityContainerViewType::iterator> && std::is_same_v<typename EntityContainerViewType::value_type, Geometry> &&
        std::is_integral_v<EntityID> && std::is_default_constructible_v<EntityID>);

  public:
    static constexpr EntityIdStrategy ENTITY_ID_STRATEGY = IS_ENTITY_KEYED ? EntityIdStrategy::EntityKeyed : EntityIdStrategy::ContiguousIndex;

  public: // General accessors
    static constexpr Entity GetEntity(EntityContainerView entities, EntityID const& entityID) noexcept { return detail::get(entities, entityID); }

    static constexpr EntityID GetEntityID(EntityContainerView entities, Entity const& entity) noexcept
    {
      return detail::getKeyPart(entities, entity);
    }

    static constexpr Geometry const& GetGeometry(EntityContainerView entities, EntityID const& entityID) noexcept
    {
      return detail::at(entities, entityID);
    }

    static constexpr Geometry const& GetGeometry(Entity const& entity) noexcept { return detail::getValuePart(entity); }

    static constexpr void SetGeometry(Entity& entity, Geometry const& geometry) noexcept { return detail::setValuePart(entity, geometry); }

    static constexpr std::size_t GetEntityCount(EntityContainerView entities) noexcept
    {
      if constexpr (std::ranges::sized_range<EntityContainerViewType>)
        return std::ranges::size(entities);
      else
        return std::distance(std::ranges::begin(entities), std::ranges::end(entities));
    }

  public: // ENTITY_ID_STRATEGY == EntityKeyed required overload
    static constexpr EntityID GetEntityID(Entity const& entity) noexcept { return detail::getKeyPart(entity); }

  public: // Container mutation
    // Insert entity into the container. Returns the EntityID of the newly inserted entity.
    static constexpr EntityID Insert(EntityContainer& entities, auto&& entity) noexcept
    {
      if constexpr (ENTITY_ID_STRATEGY == EntityIdStrategy::EntityKeyed)
      {
        detail::emplace(entities, std::forward<decltype(entity)>(entity));
        return detail::getKeyPart(entities, entity);
      }
      else
      {
        auto const id = EntityID(entities.size());
        detail::emplace(entities, std::forward<decltype(entity)>(entity));
        return id;
      }
    }

    // Erase entity from the container by EntityID.
    static constexpr void Erase(EntityContainer& entities, EntityID entityID) noexcept
    {
      if constexpr (ENTITY_ID_STRATEGY == EntityIdStrategy::EntityKeyed)
        detail::erase(entities, entityID);
      else
      {
        if (entityID >= entities.size())
          return;

        if constexpr (ENTITY_ID_STRATEGY == EntityIdStrategy::ContiguousIndex)
          detail::erase(entities, std::next(entities.begin(), entityID));
      }
    }

    // Exchange (replace) the entity at the given EntityID with a new value. The EntityID does not change.
    static constexpr auto Exchange(EntityContainer& entities, EntityID entityID, auto&& entity) noexcept
    {
      if constexpr (ENTITY_ID_STRATEGY == EntityIdStrategy::EntityKeyed)
      {
        return Entity{ entityID, std::exchange(entities.find(entityID)->second, std::forward<decltype(entity.second)>(entity.second)) };
      }
      else
      {
        return std::exchange(detail::at(entities, entityID), std::forward<decltype(entity)>(entity));
      }
    }

    // Clear all entities from the container.
    static constexpr void Clear(EntityContainer& entities) noexcept { detail::clear(entities); }

    // Check entity containment in the container
    static constexpr bool Contains(EntityContainer& entities, EntityID entityID) noexcept
    {
      if constexpr (ENTITY_ID_STRATEGY == EntityIdStrategy::EntityKeyed)
        return detail::contains(entities, entityID);
      else
        return entityID < entities.size();
    }
  };

  template<typename TPoint>
  using PointEntitySpanAdapter = EntityAdapterDefault<GeometryType::Point, TPoint, index_t, std::vector<TPoint>, std::span<TPoint const>, TPoint>;

  template<typename TBox>
  using BoxEntitySpanAdapter = EntityAdapterDefault<GeometryType::Box, TBox, index_t, std::vector<TBox>, std::span<TBox const>, TBox>;

  template<
    typename TPoint,
    typename TEntityContainer = std::unordered_map<index_t, TPoint>,
    typename TEntityContainerView = TEntityContainer const&,
    typename THash = std::hash<typename TEntityContainer::key_type>>
  using PointEntityMapAdapter =
    EntityAdapterDefault<GeometryType::Point, typename TEntityContainer::value_type, typename TEntityContainer::key_type, TEntityContainer, TEntityContainerView, TPoint, THash>;

  template<
    typename TBox,
    typename TEntityContainer = std::unordered_map<index_t, TBox>,
    typename TEntityContainerView = TEntityContainer const&,
    typename THash = std::hash<typename TEntityContainer::key_type>>
  using BoxEntityMapAdapter =
    EntityAdapterDefault<GeometryType::Box, typename TEntityContainer::value_type, typename TEntityContainer::key_type, TEntityContainer, TEntityContainerView, TBox, THash>;
} // namespace OrthoTree


namespace OrthoTree
{

  template<typename TEntityAdapter, typename TGeometryAdapter, typename TConfiguration>
  class OrthoTreeCoreBase
  {
  protected:
    using EA = TEntityAdapter;
    using GA = TGeometryAdapter;
    using CONFIG = TConfiguration;
    using IGM = detail::InternalGeometryModule<GA>;
    using SI = detail::MortonSpaceIndexing<GA::DIMENSION_NO, CONFIG::MAX_ALLOWED_DEPTH_ID>;

    using IGM_Geometry = typename IGM::Geometry;

    using TScalar = typename GA::Scalar;
    using TFloatScalar = typename GA::FloatScalar;
    using TVector = typename GA::Vector;
    using TBox = typename GA::Box;
    using TRay = typename GA::Ray;
    using TPlane = typename GA::Plane;

    using IGM_Vector = typename IGM::Vector;

    using TreeBox = detail::BoundingBoxMinPointAndSize<IGM_Vector>;
    using NodeBox = detail::BoundingBoxMinPointAndSize<IGM_Vector>;

    using NodeGeometry = std::conditional_t<
      CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MinPoint,
      IGM_Vector,
      std::conditional_t<CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MBR, NodeBox, std::monostate>>;

  private:
    static_assert(CONFIG::MAX_ALLOWED_DEPTH_ID <= MAX_DEPTH_ID, "MAX_ALLOWED_DEPTH_ID of Configuration is too large.");
    static_assert(CONFIG::LOOSE_FACTOR >= 1.0 && CONFIG::LOOSE_FACTOR <= 2.0, "Wrong loose factor for Loose trees.");

  private:
    std::size_t m_maxElementNum = CONFIG::DEFAULT_TARGET_ELEMENT_NUM_IN_NODES;
    depth_t m_maxDepthID = INVALID_DEPTH;

  private: // Serialization
    static constexpr uint32_t SERIALIZED_VERSION_ID = 0;

    template<typename TArchive, typename TEntityAdapter_, typename TGeometryAdapter_, typename TConfiguration_>
    friend void serialize(TArchive& ar, OrthoTreeCoreBase<TEntityAdapter_, TGeometryAdapter_, TConfiguration_>& core);

    std::conditional_t<CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::None || CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MBR, TreeBox, std::monostate>
      m_nominalTreeBox;
    std::conditional_t<CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::None && CONFIG::ALLOW_OUT_OF_SPACE_INSERTION, TreeBox, std::monostate> m_realTreeBox;
    std::conditional_t<CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MinPoint, std::vector<typename IGM::Vector>, std::monostate> m_nodeSize;

  public:
    using EntityContainerView = EA::EntityContainerView;
    using EntityID = EA::EntityID;
    using Entity = EA::Entity;
    using EntityGeometry = EA::Geometry;
    using ChildID = SI::ChildID;

  protected:
    constexpr OrthoTreeCoreBase() = default;

  protected:
    constexpr void InitBase(IGM::Vector const& treeMinPoint, IGM::Vector const& treeSize, depth_t maxDepthID, std::size_t maxElementNo) noexcept
    {
      ORTHOTREE_CRASH_IF(maxDepthID < 1, "maxDepthID must be larger than 0!");
      ORTHOTREE_CRASH_IF(maxDepthID > SI::MAX_THEORETICAL_DEPTH_ID, "maxDepthID is larger than the applicable with the current GA::DIMENSION_NO!");
      ORTHOTREE_CRASH_IF(maxDepthID >= std::numeric_limits<uint8_t>::max(), "maxDepthID is too large.");
      ORTHOTREE_CRASH_IF(maxElementNo == 0, "maxElementNo must be larger than 0. It is allowed max entity number for one node.");
      ORTHOTREE_CRASH_IF(CHAR_BIT * sizeof(GridID) < maxDepthID, "GridID and maxDepthID are not compatible.");

      m_maxDepthID = maxDepthID;
      m_maxElementNum = maxElementNo;

      if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::None)
      {
        m_nominalTreeBox = TreeBox{ .minPoint = treeMinPoint, .size = treeSize };
        if constexpr (CONFIG::ALLOW_OUT_OF_SPACE_INSERTION)
        {
          m_realTreeBox = TreeBox{ .minPoint = treeMinPoint, .size = treeSize };
        }
      }
      else if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MinPoint)
      {
        // the 0-based depth size of the tree is m_maxDepthID+1, and a fictive childnode halfsize (+1) could be asked prematurely.
        depth_t constexpr additionalDepth = 2;
        auto const examinedDepthSize = m_maxDepthID + additionalDepth;

        auto constexpr multiplier = IGM_Geometry(0.5);
        auto looseFactor = IGM_Geometry(CONFIG::LOOSE_FACTOR);

        m_nodeSize.resize(examinedDepthSize);
        for (depth_t depthID = 0; depthID < examinedDepthSize; ++depthID, looseFactor *= multiplier)
          detail::static_for<GA::DIMENSION_NO>(
            [&](auto dimensionID) noexcept { m_nodeSize[depthID][dimensionID] = treeSize[dimensionID] * looseFactor; });
      }
      else if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MBR)
      {
        m_nominalTreeBox = TreeBox{ .minPoint = treeMinPoint, .size = treeSize };
      }
      else
      {
        static_assert(false);
      }
    }

    constexpr IGM::Box GetNodeBox(depth_t depthID, auto const& nodeGeometry) const noexcept
    {
      if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MBR)
      {
        return { nodeGeometry.minPoint, IGM::Add(nodeGeometry.minPoint, nodeGeometry.size) };
      }
      else
      {
        auto const& size = GetNodeSize(depthID);
        return { .Min = nodeGeometry, .Max = IGM::Add(nodeGeometry, size) };
      }
    }

  public:
    constexpr depth_t GetExaminationLevelID(depth_t depthID) const { return m_maxDepthID - depthID; }


    constexpr typename IGM::Vector CalculateNodeSize(depth_t depthID) const noexcept
      requires(CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::None)
    {
      return IGM::Multiply(m_nominalTreeBox.size, IGM_Geometry(1) / static_cast<IGM_Geometry>(detail::pow2(depthID)));
    }

  public:
    constexpr auto GetMaxDepthID() const noexcept { return m_maxDepthID; }
    constexpr auto GetMaxElementNum() const noexcept { return m_maxElementNum; }
    constexpr auto GetDepthNo() const noexcept { return m_maxDepthID + 1; }
    static constexpr bool AreChildNodesOverlapping() noexcept { return CONFIG::LOOSE_FACTOR > 1.0; }

  protected:
    constexpr decltype(auto) GetNodeSize(depth_t depthID) const
    {
      if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::None)
      {
        return CalculateNodeSize(depthID);
      }
      else
      {
        return m_nodeSize[depthID];
      }
    }

    constexpr typename IGM::Vector const& GetTreeBoxNominalMinPoint() const noexcept
      requires(CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::None || CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MBR)
    {
      return m_nominalTreeBox.minPoint;
    }

    constexpr typename IGM::Vector const& GetTreeBoxNominalSize() const noexcept
      requires(CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::None || CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MBR)
    {
      return m_nominalTreeBox.size;
    }

    constexpr typename IGM::Vector const& GetTreeBoxRealMinPoint() const noexcept
      requires(CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::None && CONFIG::ALLOW_OUT_OF_SPACE_INSERTION)
    {
      return m_realTreeBox.minPoint;
    }

    constexpr typename IGM::Vector const& GetTreeBoxRealSize() const noexcept
      requires(CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::None && CONFIG::ALLOW_OUT_OF_SPACE_INSERTION)
    {
      return m_realTreeBox.size;
    }

    constexpr void SetRealTreeMinPoint(IGM::Vector const& minPoint) noexcept
      requires(CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::None && CONFIG::ALLOW_OUT_OF_SPACE_INSERTION)
    {
      m_realTreeBox.minPoint = minPoint;
    }

    constexpr void SetRealTreeSize(IGM::Vector const& size) noexcept
      requires(CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::None && CONFIG::ALLOW_OUT_OF_SPACE_INSERTION)
    {
      m_realTreeBox.size = size;
    }

    constexpr void SetTreeSize(IGM::Vector const& treeSize) noexcept
      requires(CONFIG::ALLOW_OUT_OF_SPACE_INSERTION)
    {
      if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::None)
      {
        m_realTreeBox.size = treeSize;
      }
      else if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MinPoint)
      {
        m_nodeSize[0] = treeSize;
      }
    }
  };

#define ORTHOTREE_DEPENDENT_TYPES(Base)                         \
  using EA = Base::EA;                                          \
  using GA = Base::GA;                                          \
  using CONFIG = Base::CONFIG;                                  \
  using IGM = Base::IGM;                                        \
  using IGM_Geometry = IGM::Geometry;                           \
                                                                \
  using TScalar = typename GA::Scalar;                          \
  using TFloatScalar = typename GA::FloatScalar;                \
  using TVector = typename GA::Vector;                          \
  using TBox = typename GA::Box;                                \
  using TRay = typename GA::Ray;                                \
  using TPlane = typename GA::Plane;                            \
                                                                \
  using EntityContainerView = typename EA::EntityContainerView; \
  using EntityID = typename EA::EntityID;                       \
  using Entity = typename EA::Entity;                           \
  using EntityGeometry = typename EA::Geometry;                 \
                                                                \
  using ChildID = Base::ChildID
} // namespace OrthoTree


namespace OrthoTree
{
  namespace detail
  {
    template<typename T>
    struct SentinelMortonHash
    {
      std::size_t operator()(T code) const noexcept
      {
#if defined(_MSC_VER)
        if constexpr (sizeof(T) == 8)
        {
          if constexpr (sizeof(std::size_t) == 4)
          {
            uint32_t lo = (uint32_t)(code);
            uint32_t hi = (uint32_t)(code >> 32);
            uint32_t folded = lo ^ hi;
            folded ^= folded >> 16;
            folded *= 0x45d9f3bU;
            folded ^= folded >> 16;
            return static_cast<std::size_t>(folded);
          }
          else
          {
            code ^= code >> 30;
            code *= 0xbf58476d1ce4e5b9ULL;
            code ^= code >> 27;
            code *= 0x94d049bb133111ebULL;
            return static_cast<std::size_t>(code ^ (code >> 31));
          }
        }
        else
        {
          return std::hash<T>{}(code);
        }
#else
        return std::hash<uint64_t>{}(code);
#endif
      }
    };

    template<std::size_t CHILD_NO, typename NodeID, typename ChildID, typename EntityID, typename Geometry>
    class OrthoTreeNodeData
    {
    private:
      constexpr static auto IS_SPARSE_CHILDREN_CONTAINER = CHILD_NO > 8;

      constexpr static bool IS_BITSET_BASED_FLAGS = CHILD_NO > 64;
      using ChildFlags = std::conditional_t<(CHILD_NO > 32), uint64_t, std::conditional_t<(CHILD_NO > 8), uint32_t, uint8_t>>;
      using ChildIndex = std::conditional_t<IS_BITSET_BASED_FLAGS, std::vector<std::size_t>, ChildFlags>;

    public:
      using ChildContainer = std::conditional_t<IS_SPARSE_CHILDREN_CONTAINER, typename std::vector<NodeID>, detail::inplace_vector<NodeID, CHILD_NO>>;
      using EntityContainer = detail::MemoryResource<EntityID>::MemorySegment;

    private:
      ChildIndex m_childIndex = {};
      ChildContainer m_children = {};
      EntityContainer m_entities = {};

      Geometry m_geometry;

    private:
      static constexpr uint32_t SERIALIZED_VERSION_ID = 0;

      template<typename TArchive, std::size_t CHILD_NO2, typename NodeID2, typename ChildID2, typename EntityID2, typename Geometry2>
      friend void serialize(TArchive& ar, OrthoTreeNodeData<CHILD_NO2, NodeID2, ChildID2, EntityID2, Geometry2>& node);

      template<typename TArchive, typename T, typename TNodes>
      friend void serialize(TArchive& ar, MemoryResource<T>& memoryResource, TNodes& nodes);

    public:
      explicit constexpr OrthoTreeNodeData() noexcept = default;

      constexpr Geometry const& GetGeometry() const noexcept { return m_geometry; }
      constexpr Geometry& GetGeometry() noexcept { return m_geometry; }

      void Clear() noexcept
      {
        m_entities = {};
        m_children = {};
      }

    public: // Entity handling
      constexpr auto const& GetEntities() const noexcept { return m_entities.segment; }

      constexpr auto& GetEntities() noexcept { return m_entities.segment; }

      constexpr std::size_t GetEntitiesSize() const noexcept { return m_entities.segment.size(); }

      constexpr bool IsEntitiesEmpty() const noexcept { return m_entities.segment.empty(); }

      constexpr bool ContainsEntity(EntityID entityID) const noexcept
      {
        return std::find(m_entities.segment.begin(), m_entities.segment.end(), entityID) != m_entities.segment.end();
      }

      constexpr void ReplaceEntities(std::span<EntityID> entities) noexcept { m_entities.segment = std::move(entities); }

      constexpr bool RemoveEntity(EntityID entityID) noexcept
      {
        auto const endIteratorAfterRemove = std::remove(m_entities.segment.begin(), m_entities.segment.end(), entityID);
        if (endIteratorAfterRemove == m_entities.segment.end())
          return false; // id was not registered previously.

        // segment size decrease is handled by the memory resource
        return true;
      }

      constexpr void DecreaseEntityIDs([[maybe_unused]] EntityID removedEntityID) noexcept
      {
        if constexpr (std::is_integral_v<EntityID>)
        {
          for (auto& id : m_entities.segment)
            id -= removedEntityID < id;
        }
      }

      EntityContainer& GetEntitySegment() noexcept { return m_entities; }
      EntityContainer const& GetEntitySegment() const noexcept { return m_entities; }

    public: // Child handling
      constexpr void LinkChild(ChildID childID, NodeID nodeID) noexcept
      {
        std::size_t elementID = 0;
        bool shouldOverwrite = false;
        if constexpr (IS_BITSET_BASED_FLAGS)
        {
          auto const it = std::ranges::lower_bound(m_childIndex, static_cast<std::size_t>(childID));
          elementID = std::distance(m_childIndex.begin(), it);
          shouldOverwrite = elementID < m_childIndex.size() && m_childIndex[elementID] == childID;
          if (!shouldOverwrite)
            m_childIndex.insert(it, static_cast<std::size_t>(childID));
        }
        else
        {
          ChildFlags const childFlag = (ChildFlags{ 1 } << childID);
          ChildFlags const childMask = childFlag - ChildFlags{ 1 };
          elementID = std::popcount(ChildFlags(m_childIndex & childMask));
          shouldOverwrite = m_childIndex & childFlag;
          m_childIndex |= childFlag;
        }

        if (shouldOverwrite)
          m_children[elementID] = nodeID;
        else
          m_children.insert(m_children.begin() + elementID, nodeID);
      }

      constexpr NodeID GetChild(ChildID childID) noexcept
      {
        std::size_t elementID = 0;
        if constexpr (IS_BITSET_BASED_FLAGS)
        {
          auto const it = std::ranges::lower_bound(m_childIndex, static_cast<std::size_t>(childID));
          elementID = std::distance(m_childIndex.begin(), it);
        }
        else
        {
          ChildFlags const childFlag = (ChildFlags{ 1 } << childID);
          ChildFlags const childMask = childFlag - ChildFlags{ 1 };
          elementID = std::popcount(ChildFlags(m_childIndex & childMask));
        }

        return m_children[elementID];
      }

      constexpr bool HasChild(ChildID childID) const noexcept
      {
        if constexpr (IS_BITSET_BASED_FLAGS)
        {
          return std::ranges::binary_search(m_childIndex, static_cast<std::size_t>(childID));
        }
        else
        {
          return m_childIndex & (ChildFlags{ 1 } << childID);
        }
      }

      constexpr void UnlinkChild(ChildID childID) noexcept
      {
        std::size_t elementID = 0;
        if constexpr (IS_BITSET_BASED_FLAGS)
        {
          auto const it = std::ranges::lower_bound(m_childIndex, static_cast<std::size_t>(childID));
          elementID = std::distance(m_childIndex.begin(), it);
          m_childIndex.erase(it);
        }
        else
        {
          static_assert(std::unsigned_integral<ChildFlags>, "Incompatible type!");

          ChildFlags const childFlag = (ChildFlags{ 1 } << childID);
          ChildFlags const childMask = childFlag - ChildFlags{ 1 };
          elementID = std::popcount(ChildFlags(m_childIndex & childMask));
          m_childIndex &= ~childFlag;
        }

        m_children.erase(m_children.begin() + elementID);
      }

      constexpr bool IsAnyChildExist() const noexcept { return !m_children.empty(); }

      constexpr auto const& GetChildren() const noexcept { return m_children; }
    };
  } // namespace detail


  // Non-owning ortho-tree with dynamic storage for changing datasets.
  template<typename TEntityAdapter, typename TGeometryAdapter, typename TConfiguration>
  class DynamicHashOrthoTreeCore : public OrthoTreeCoreBase<TEntityAdapter, TGeometryAdapter, TConfiguration>
  {
  public:
    using Base = OrthoTreeCoreBase<TEntityAdapter, TGeometryAdapter, TConfiguration>;
    ORTHOTREE_DEPENDENT_TYPES(Base);

  public:
    using SI = typename Base::SI;

    using NodeID = typename SI::NodeID;
    using NodeIDCR = typename SI::NodeIDCR;

  private:
    static_assert(std::is_trivially_copyable_v<EntityID>, "EntityID must be trivially copyable!");

    using NodeGeometry = std::conditional_t<CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::None, std::monostate, typename Base::NodeGeometry>;

    using Node = detail::OrthoTreeNodeData<SI::CHILD_NO, NodeID, typename SI::ChildID, EntityID, NodeGeometry>;

    template<typename TData>
    using LinearNodeContainer = typename CONFIG::template UMapNodeContainer<NodeID, TData, detail::SentinelMortonHash<NodeID>>;

    template<typename TData>
    using NonLinearNodeContainer = typename CONFIG::template MapNodeContainer<NodeID, TData, bitset_arithmetic_compare>;

    template<typename TData>
    using NodeContainer = typename std::conditional_t<SI::IS_LINEAR_TREE, LinearNodeContainer<TData>, NonLinearNodeContainer<TData>>;

    using ReverseMapType = std::conditional_t<
      EA::ENTITY_ID_STRATEGY == EntityIdStrategy::EntityKeyed,
      typename CONFIG::template ReverseMapKeyed<EntityID, NodeID, typename EA::Hash>,
      typename CONFIG::template ReverseMapIndexed<NodeID>>;
    using ReverseMap = std::conditional_t<CONFIG::USE_REVERSE_MAPPING, ReverseMapType, std::monostate>;

    using NodeValueType = std::pair<NodeID, Node>;
    using MutableNodeValue = std::decay_t<typename NodeContainer<Node>::value_type>*;

  public:
    using NodeValue = std::decay_t<typename NodeContainer<Node>::value_type> const*;

  private: // Member variables
    NodeContainer<Node> m_nodes;
    ReverseMap m_reverseMap;

    detail::MortonGridSpaceIndexing<GA, CONFIG::ALLOW_OUT_OF_SPACE_INSERTION, CONFIG::LOOSE_FACTOR, CONFIG::MAX_ALLOWED_DEPTH_ID> m_spaceIndexing;
    detail::MemoryResource<EntityID> m_memoryResource;

  private: // Serialization
    static constexpr uint32_t SERIALIZED_VERSION_ID = 0;

    template<typename TArchive, typename TEntityAdapter_, typename TGeometryAdapter_, typename TConfiguration_>
    friend void serialize(TArchive& ar, DynamicHashOrthoTreeCore<TEntityAdapter_, TGeometryAdapter_, TConfiguration_>& core);

  public: // Constructors
    // Default constructor. Requires InitBase call before usage.
    constexpr DynamicHashOrthoTreeCore() = default;

    constexpr DynamicHashOrthoTreeCore(DynamicHashOrthoTreeCore&&) = default;

    // Initialize the base octree structure
    constexpr DynamicHashOrthoTreeCore(
      TBox const& boxSpace,
      depth_t maxDepthID,
      std::size_t maxElementNo = CONFIG::DEFAULT_TARGET_ELEMENT_NUM_IN_NODES,
      std::size_t estimatedEntityNo = detail::MemoryResource<EntityID>::DEFAULT_PAGE_SIZE) noexcept
    {
      InitBase(IGM::GetBoxAD(boxSpace), maxDepthID, maxElementNo, estimatedEntityNo);
    }

    // Initialize the base octree structure with entity collection
    template<typename TExecMode = SeqExec>
    explicit DynamicHashOrthoTreeCore(
      EntityContainerView entities,
      std::optional<depth_t> maxDepthIDIn = std::nullopt,
      std::optional<TBox> boxSpaceOptional = std::nullopt,
      std::size_t maxElementNoInNode = CONFIG::DEFAULT_TARGET_ELEMENT_NUM_IN_NODES,
      TExecMode execMode = {}) noexcept
    {
      [[maybe_unused]] auto isSuccessfullyInsertedAllElements =
        Create(entities, maxDepthIDIn, std::move(boxSpaceOptional), maxElementNoInNode, execMode);
      assert(isSuccessfullyInsertedAllElements);
    }

    // Initialize the base octree structure with entity collection and parallel tree-building option
    template<typename TExecMode>
    DynamicHashOrthoTreeCore(
      TExecMode execMode,
      EntityContainerView entities,
      std::optional<depth_t> maxDepthIDIn = std::nullopt,
      std::optional<TBox> boxSpaceOptional = std::nullopt,
      std::size_t maxElementNoInNode = CONFIG::DEFAULT_TARGET_ELEMENT_NUM_IN_NODES) noexcept
    {
      [[maybe_unused]] auto isSuccessfullyInsertedAllElements =
        Create(entities, maxDepthIDIn, std::move(boxSpaceOptional), maxElementNoInNode, execMode);
      assert(isSuccessfullyInsertedAllElements);
    }

    DynamicHashOrthoTreeCore(DynamicHashOrthoTreeCore const& other)
    : Base(other)
    , m_nodes(other.m_nodes)
    , m_reverseMap(other.m_reverseMap)
    , m_spaceIndexing(other.m_spaceIndexing)
    {
      auto segments = std::vector<typename detail::MemoryResource<EntityID>::MemorySegment*>(m_nodes.size());
      int i = 0;
      for (auto& [key, node] : m_nodes)
      {
        segments[i] = &node.GetEntitySegment();
        ++i;
      }
      other.m_memoryResource.Clone(m_memoryResource, segments);
    }

    DynamicHashOrthoTreeCore& operator=(DynamicHashOrthoTreeCore const& other)
    {
      if (this == &other)
        return *this;

      Base::operator=(other);

      m_nodes = other.m_nodes;
      m_reverseMap = other.m_reverseMap;
      m_spaceIndexing = other.m_spaceIndexing;

      // using MR = detail::MemoryResource<EntityID>;
      auto segments = std::vector<typename detail::MemoryResource<EntityID>::MemorySegment*>(m_nodes.size());
      int i = 0;
      for (auto& [key, node] : m_nodes)
      {
        segments[i] = &node.GetEntitySegment();
        ++i;
      }
      other.m_memoryResource.Clone(m_memoryResource, segments);
      return *this;
    }

  public:
    constexpr TBox GetBox() const noexcept
    {
      auto box = GA::MakeBox();

      auto const& minPoint = [&]() -> decltype(auto) {
        if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MBR)
          return Base::GetTreeBoxNominalMinPoint();
        else if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MinPoint)
          return m_nodes.at(GetRootNodeID()).GetGeometry();
        else if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::None)
        {
          if constexpr (CONFIG::ALLOW_OUT_OF_SPACE_INSERTION)
            return Base::GetTreeBoxRealMinPoint();
          else
            return Base::GetTreeBoxNominalMinPoint();
        }
      }();

      auto const& size = [&]() -> decltype(auto) {
        if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MBR)
          return Base::GetTreeBoxNominalSize();
        else if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MinPoint)
          return Base::GetNodeSize(0);
        else if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::None)
        {
          if constexpr (CONFIG::ALLOW_OUT_OF_SPACE_INSERTION)
            return Base::GetTreeBoxRealSize();
          else
            return Base::GetTreeBoxNominalSize();
        }
      }();

      detail::static_for<GA::DIMENSION_NO>([&](auto dimensionID) {
        GA::SetBoxMinC(box, dimensionID, minPoint[dimensionID]);
        GA::SetBoxMaxC(box, dimensionID, minPoint[dimensionID] + size[dimensionID]);
      });

      return box;
    }

  private:
    MutableNodeValue GetMutableNodeValue(NodeIDCR nodeID) noexcept { return &(*m_nodes.find(nodeID)); }
    MutableNodeValue GetMutableRootNodeValue() noexcept { return GetMutableNodeValue(GetRootNodeID()); }

    // Node interface
#ifdef ORTHOTREE__PUBLIC_NODE_INTERFACE
  public:
#else
  protected:
#endif

    static NodeID GetRootNodeID() noexcept { return SI::GetRootKey(); }
    static NodeID GetNoneNodeID() noexcept { return SI::GetNoneKey(); }

    constexpr auto const& GetNodes() const noexcept { return m_nodes; }
    constexpr std::size_t GetNodeCount() const noexcept { return m_nodes.size(); }
    bool HasNode(NodeIDCR key) const noexcept { return m_nodes.contains(key); }
    auto const& GetNode(NodeIDCR key) const noexcept { return m_nodes.at(key); }

    NodeValue GetNodeValue(NodeIDCR nodeID) const noexcept { return &(*m_nodes.find(nodeID)); }
    NodeValue GetRootNodeValue() const noexcept
    {
      assert(
        m_nodes.contains(GetRootNodeID()) &&
        "Root node does not exist! Non-default Constructor, Init() or Create() must be called first to create root node.");
      return GetNodeValue(GetRootNodeID());
    }


    // Get EntityIDs of the node
    constexpr auto const& GetNodeEntities(NodeValue nodeValue) const noexcept { return nodeValue->second.GetEntities(); }

    // Get EntityIDs number of the node
    constexpr std::size_t GetNodeEntityCount(NodeValue nodeValue) const noexcept { return nodeValue->second.GetEntitiesSize(); }

    // Get Node min-point
    constexpr decltype(auto) GetNodeMinPoint(NodeValue nodeValue) const noexcept
    {
      if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::None)
      {
        auto const nodeCenter = m_spaceIndexing.CalculateNodeCenter(nodeValue->first);
        auto const depthID = SI::GetDepthID(nodeValue->first);
        auto const& nodeHalfSize = Base::GetNodeSize(depthID + 1);

        return IGM::Sub(nodeCenter, nodeHalfSize);
      }
      else if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MinPoint)
        return nodeValue->second.GetGeometry();
      else if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MBR)
        return nodeValue->second.GetGeometry().minPoint;
      else
        static_assert(false, "Unsupported node geometry storage!");
    }

    // Get Node size
    constexpr decltype(auto) GetNodeSize(NodeValue nodeValue) const noexcept
    {
      auto const depthID = SI::GetDepthID(nodeValue->first);
      if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::None || CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MinPoint)
        return Base::GetNodeSize(depthID);
      else if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MBR)
        return nodeValue->second.GetGeometry().size;
      else
        static_assert(false, "Unsupported node geometry storage!");
    }

    // Get Child Node IDs
    constexpr decltype(auto) GetNodeChildren(NodeValue nodeValue) const noexcept { return nodeValue->second.GetChildren(); }

    // Get Node box
    constexpr IGM::Box GetNodeBox(NodeValue nodeValue) const noexcept
    {
      auto const& minPoint = GetNodeMinPoint(nodeValue);
      return { .Min = minPoint, .Max = IGM::Add(minPoint, GetNodeSize(nodeValue)) };
    }

    // Is the node has any entity
    constexpr bool IsNodeEntitiesEmpty(NodeValue nodeValue) const noexcept { return nodeValue->second.IsEntitiesEmpty(); }

  protected:
    constexpr void AddNodeEntity(MutableNodeValue nodeValue, EntityID newEntity) noexcept
    {
      m_memoryResource.IncreaseSegment(nodeValue->second.GetEntitySegment(), 1);
      nodeValue->second.GetEntities().back() = std::move(newEntity);
      UpdateReverseMap(newEntity, nodeValue->first);
    }

    constexpr bool RemoveNodeEntity(Node& node, EntityID entity) noexcept
    {
      auto const isRemoved = node.RemoveEntity(entity);
      if (isRemoved)
        m_memoryResource.DecreaseSegment(node.GetEntitySegment(), 1);

      return isRemoved;
    }

    constexpr void ClearNodeEntities(Node& node) noexcept
    {
      auto& ms = node.GetEntitySegment();
      m_memoryResource.DecreaseSegment(ms, ms.segment.size());
    }

    constexpr void ResizeNodeEntities(Node& node, std::size_t size) noexcept
    {
      auto& ms = node.GetEntitySegment();
      m_memoryResource.DecreaseSegment(ms, ms.segment.size() - size);
    }

    constexpr void InitNodeGeometry([[maybe_unused]] auto* nodeValue) const noexcept
    {
      if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MinPoint)
      {
        auto const nodeCenter = m_spaceIndexing.CalculateNodeCenter(nodeValue->first);
        auto const depthID = SI::GetDepthID(nodeValue->first);
        auto const& nodeHalfSize = Base::GetNodeSize(depthID + 1);

        nodeValue->second.GetGeometry() = IGM::Sub(nodeCenter, nodeHalfSize);
      }
      else if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MBR)
      {
        nodeValue->second.GetGeometry() = NodeGeometry{ .minPoint = { std::numeric_limits<IGM_Geometry>::max() }, .size = {} };
      }
    }

    bool IsNodeGeometryInitialized(NodeGeometry const& nodeGeometry)
    {
      if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MBR)
      {
        auto& [minPoint, _] = nodeGeometry;
        return minPoint[0] != std::numeric_limits<IGM_Geometry>::max();
      }
      else
      {
        return true;
      }
    }

    constexpr NodeValueType CreateNode(NodeIDCR nodeID) const noexcept
    {
      if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::None)
      {
        return NodeValueType{ nodeID, Node() };
      }
      else if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MinPoint || CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MBR)
      {
        auto nodeValue = NodeValueType{ nodeID, Node() };
        InitNodeGeometry(&nodeValue);
        return nodeValue;
      }
      else
      {
        static_assert(false, "Unsupported node geometry storage!");
      }
    }

  private:
    constexpr void InitBase(
      IGM::Box const& boxSpace,
      depth_t maxDepthID,
      std::size_t maxElementNo,
      std::size_t estimatedEntityNo = detail::MemoryResource<EntityID>::DEFAULT_PAGE_SIZE) noexcept
    {
      m_spaceIndexing =
        detail::MortonGridSpaceIndexing<GA, CONFIG::ALLOW_OUT_OF_SPACE_INSERTION, CONFIG::LOOSE_FACTOR, CONFIG::MAX_ALLOWED_DEPTH_ID>(maxDepthID, boxSpace);
      Base::InitBase(m_spaceIndexing.GetMinPoint(), m_spaceIndexing.GetSize(), maxDepthID, maxElementNo);

      ORTHOTREE_CRASH_IF(
        !m_nodes.empty(), "To build/setup/create the tree, use the Create() [recommended] or Init() function. If an already built tree is wanted to be reset, use the Reset() function before Init().");

      m_nodes.emplace(CreateNode(GetRootNodeID()));
      m_memoryResource.Init(estimatedEntityNo);
    }

  public: // Main service functions
    // Alternative creation mode (instead of Create), Init then Insert items into leafs one by one. NOT RECOMMENDED.
    constexpr void Init(
      TBox const& box,
      depth_t maxDepthID,
      std::size_t maxElementNo = 11,
      std::size_t estimatedEntityNo = detail::MemoryResource<EntityID>::DEFAULT_PAGE_SIZE) noexcept
    {
      InitBase(IGM::GetBoxAD(box), maxDepthID, maxElementNo, estimatedEntityNo);
    }

  private: // Build
    using LocationIterator = typename detail::zip_view<std::vector<typename SI::Location>, std::span<EntityID>>::iterator;

    struct NodeProcessingData
    {
      NodeValueType NodeInstance;
      LocationIterator EndLocationIt;
    };
    template<bool ARE_LOCATIONS_SORTED>
    constexpr void ProcessNode(depth_t depthID, LocationIterator& locationIt, NodeProcessingData& nodeProcessingData)
    {
      auto& [node, endLocationIt] = nodeProcessingData;

      auto const subtreeEntityNo = detail::size(locationIt, endLocationIt);
      if (subtreeEntityNo == 0)
        return;

      auto nodeEntityNo = subtreeEntityNo;
      auto stuckedEndLocationIt = endLocationIt;
      if (subtreeEntityNo > Base::GetMaxElementNum() && depthID < Base::GetMaxDepthID())
      {
        auto const isNonRefinable = [depthID](auto const& location) {
          auto const d = location.GetFirst().GetDepthID();
          return d == depthID;
        };

        if constexpr (ARE_LOCATIONS_SORTED)
          stuckedEndLocationIt = std::partition_point(locationIt, endLocationIt, isNonRefinable);
        else
          stuckedEndLocationIt = std::partition(locationIt, endLocationIt, isNonRefinable);

        nodeEntityNo = std::size_t(std::distance(locationIt, stuckedEndLocationIt));
      }

      if (nodeEntityNo == 0)
        return;

      node.second.ReplaceEntities(std::span(locationIt.GetSecond(), stuckedEndLocationIt.GetSecond()));

      if constexpr (CONFIG::USE_REVERSE_MAPPING)
      {
        for (auto it = locationIt.GetSecond(); it != stuckedEndLocationIt.GetSecond(); ++it)
          detail::set(m_reverseMap, *it, node.first);
      }

      locationIt += nodeEntityNo;
    }

    template<bool ARE_LOCATIONS_SORTED>
    constexpr void CreateProcessingData(
      depth_t examinedLevelID,
      SI::ChildKeyGenerator const& keyGenerator,
      LocationIterator const& locationIt,
      NodeProcessingData& parentNodeProcessingData,
      NodeProcessingData& nodeProcessingData) const noexcept
    {
      auto const childChecker = typename SI::ChildCheckerFixedDepth(examinedLevelID, (*locationIt).GetFirst().GetLocationID());
      auto const childID = childChecker.GetChildID(examinedLevelID);
      auto childKey = keyGenerator.GetChildNodeKey(childID);

      parentNodeProcessingData.NodeInstance.second.LinkChild(childID, childKey);
      if constexpr (ARE_LOCATIONS_SORTED)
      {
        nodeProcessingData.EndLocationIt = std::partition_point(locationIt, parentNodeProcessingData.EndLocationIt, [&](auto const& location) {
          return childChecker.Test(location.GetFirst().GetLocationID());
        });
      }
      else
      {
        nodeProcessingData.EndLocationIt = std::partition(locationIt, parentNodeProcessingData.EndLocationIt, [&](auto const& location) {
          return childChecker.Test(location.GetFirst().GetLocationID());
        });
      }

      nodeProcessingData.NodeInstance = CreateNode(childKey);
    }

    // Build the tree in depth-first order
    template<bool ARE_LOCATIONS_SORTED, typename TResulEntityContainerView>
    constexpr void BuildSubtree(
      LocationIterator const& rootBeginLocationIt, LocationIterator const& rootEndLocationIt, NodeValueType&& rootNode, TResulEntityContainerView& nodes) noexcept
    {
      auto nodeStack = std::vector<NodeProcessingData>(Base::GetDepthNo());
      nodeStack[0] = NodeProcessingData{ std::move(rootNode), rootEndLocationIt };

      auto locationIt = rootBeginLocationIt;
      auto constexpr exitDepthID = depth_t(-1);
      for (depth_t depthID = 0; depthID != exitDepthID;)
      {
        if (!nodeStack[depthID].NodeInstance.second.IsAnyChildExist())
        {
          ProcessNode<ARE_LOCATIONS_SORTED>(depthID, locationIt, nodeStack[depthID]);
        }

        auto const isEntitesProcessed = locationIt == nodeStack[depthID].EndLocationIt;

        if (isEntitesProcessed || depthID == Base::GetMaxDepthID())
        {
          detail::emplace(nodes, std::move(nodeStack[depthID].NodeInstance));
          --depthID;
          continue;
        }

        ++depthID;
        auto const examinedLevelID = this->GetExaminationLevelID(depthID);
        auto const keyGenerator = typename SI::ChildKeyGenerator(nodeStack[depthID - 1].NodeInstance.first);
        CreateProcessingData<ARE_LOCATIONS_SORTED>(examinedLevelID, keyGenerator, locationIt, nodeStack[depthID - 1], nodeStack[depthID]);
      }
    }

  public: // Create
    // Create
    template<typename TExecMode = SeqExec, bool ARE_ENTITIES_SURELY_IN_MODELSPACE = false>
    bool Create(
      EntityContainerView entities,
      std::optional<depth_t> maxDepthIn = std::nullopt,
      std::optional<TBox> boxSpaceOptional = std::nullopt,
      std::size_t maxElementNoInNode = CONFIG::DEFAULT_TARGET_ELEMENT_NUM_IN_NODES,
      TExecMode execMode = {}) noexcept
    {
      auto const boxSpace = boxSpaceOptional ? IGM::GetBoxAD(*boxSpaceOptional) : IGM::template GetBoundingBoxAD<EA>(entities);

      auto const entityNo = entities.size();
      auto const maxDepthID = (!maxDepthIn || maxDepthIn == depth_t{})
                              ? detail::EstimateMaxDepth<GA::DIMENSION_NO, SI::MAX_THEORETICAL_DEPTH_ID>(entityNo, maxElementNoInNode)
                              : *maxDepthIn;
      InitBase(boxSpace, maxDepthID, maxElementNoInNode, entityNo);

      if (entityNo == 0)
        return true;


      if constexpr (CONFIG::USE_REVERSE_MAPPING)
        detail::resize(m_reverseMap, entityNo);

      auto mortonIDs = std::vector<typename SI::Location>(entityNo);
      auto entityIDsView = m_memoryResource.Allocate(entityNo).segment;

      auto locationsZip = detail::zip_view(mortonIDs, entityIDsView);
      using Location = decltype(locationsZip)::iterator::value_type;

      EXEC_POL_DEF(epf); // GCC 11.3
      std::transform(EXEC_POL_ADD(epf) entities.begin(), entities.end(), locationsZip.begin(), [&](auto const& entity) -> Location {
        return { m_spaceIndexing.GetLocation(EA::GetGeometry(entity)), EA::GetEntityID(entities, entity) };
      });

      auto endIt = locationsZip.end();
      if constexpr (!ARE_ENTITIES_SURELY_IN_MODELSPACE && !CONFIG::ALLOW_OUT_OF_SPACE_INSERTION)
      {
        endIt = std::partition(locationsZip.begin(), endIt, [](auto const& element) { return element.GetFirst().GetDepthID() != INVALID_DEPTH; });
      }

      constexpr bool ARE_LOCATIONS_SORTED = std::is_same_v<TExecMode, ExecutionTags::Parallel>;
      if constexpr (ARE_LOCATIONS_SORTED)
      {
        EXEC_POL_DEF(eps); // GCC 11.3
        std::sort(EXEC_POL_ADD(eps) locationsZip.begin(), endIt, [](Location const& l, Location const& r) { return l.first < r.first; });
      }

      auto rootNode = *m_nodes.begin();
      m_nodes.clear();
      detail::reserve(m_nodes, detail::EstimateNodeNumber<GA::DIMENSION_NO, SI::MAX_THEORETICAL_DEPTH_ID>(entityNo, maxDepthID, maxElementNoInNode));
      BuildSubtree<ARE_LOCATIONS_SORTED>(locationsZip.begin(), endIt, std::move(rootNode), m_nodes);

      if (boxSpaceOptional && !ARE_ENTITIES_SURELY_IN_MODELSPACE)
        InitializeRootNodeBox(entities);
      else
        InitializeSubtreeMinimalNodeGeometry(GetMutableRootNodeValue(), entities);

      return true;
    }

    // Create
    template<typename TExecMode = SeqExec, bool ARE_ENTITIES_SURELY_IN_MODELSPACE = false>
    static bool Create(
      DynamicHashOrthoTreeCore& tree,
      EntityContainerView entities,
      std::optional<depth_t> maxDepthIn = std::nullopt,
      std::optional<TBox> boxSpaceOptional = std::nullopt,
      std::size_t maxElementNoInNode = CONFIG::DEFAULT_TARGET_ELEMENT_NUM_IN_NODES,
      TExecMode execMode = {}) noexcept
    {
      return tree.Create<TExecMode, ARE_ENTITIES_SURELY_IN_MODELSPACE>(entities, maxDepthIn, std::move(boxSpaceOptional), maxElementNoInNode, execMode);
    }


  private:
    void LinkOrphanNodes(std::vector<NodeID>&& orphanNodes) noexcept
    {
      for (std::size_t i = 0; i < orphanNodes.size(); ++i)
      {
        auto const orphanNodeID = orphanNodes[i];
        auto& [parentNodeID, parentNode] = *GetParentIt(orphanNodeID);
        auto const childID = SI::GetNonDirectChildID(parentNodeID, orphanNodeID);

        if (parentNode.HasChild(childID))
        {
          auto const childNodeID = parentNode.GetChild(childID);
          auto const lcaNodeID = SI::GetLowestCommonAncestor(childNodeID, orphanNodeID);
          parentNode.LinkChild(childID, lcaNodeID);

          if (orphanNodeID == lcaNodeID)
          {
            auto& orphanNode = m_nodes.at(orphanNodeID);
            auto const childIDOfOrphanNode = SI::GetNonDirectChildID(orphanNodeID, childNodeID);
            if (orphanNode.HasChild(childIDOfOrphanNode))
              orphanNodes.push_back(orphanNode.GetChild(childIDOfOrphanNode));

            orphanNode.LinkChild(childIDOfOrphanNode, childNodeID);
          }
          else
          {
            auto [lcaIt, _] = m_nodes.try_emplace(lcaNodeID);
            InitNodeGeometry(&*lcaIt);
            auto& lcaNode = lcaIt->second;
            lcaNode.LinkChild(SI::GetNonDirectChildID(lcaNodeID, childNodeID), childNodeID);
            lcaNode.LinkChild(SI::GetNonDirectChildID(lcaNodeID, orphanNodeID), orphanNodeID);
          }
        }
        else
        {
          parentNode.LinkChild(childID, orphanNodeID);
        }
      }
    }

    template<typename TNewEntities, typename TExecMode = SeqExec>
    constexpr void UpdateNodeGeometry(
      [[maybe_unused]] std::vector<std::pair<typename SI::Location, std::size_t>> changedNodes,
      [[maybe_unused]] TNewEntities const& newEntities,
      [[maybe_unused]] std::size_t existingEntityNum,
      [[maybe_unused]] TExecMode execMode = {}) noexcept
    {
      if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MBR)
      {
        if (changedNodes.empty())
          return;

        std::sort(changedNodes.begin(), changedNodes.end(), [](auto const& lhs, auto const& rhs) {
          return SI::Location::template IsLess<true>(lhs.first, rhs.first);
        });
        auto const& changedNodesInDFS = changedNodes;

        auto entityGeometries =
          std::conditional_t<EA::ENTITY_ID_STRATEGY == EntityIdStrategy::EntityKeyed, std::unordered_map<EntityID, EntityGeometry>, std::vector<EntityGeometry>>{};
        detail::reserve(entityGeometries, detail::size(newEntities));

        EXEC_POL_DEF(epu);
        if constexpr (EA::ENTITY_ID_STRATEGY == EntityIdStrategy::EntityKeyed)
        {
          std::for_each(newEntities.begin(), newEntities.end(), [&](auto const& item) {
            using T = std::remove_cvref_t<decltype(item)>;
            if constexpr (std::is_convertible_v<T, typename EA::Entity>)
            {
              entityGeometries.try_emplace(EA::GetEntityID(item), EA::GetGeometry(item));
            }
            else if constexpr (requires(T v) {
                                 []<typename U>(U&& u) {
                                   auto [a, b] = u;
                                 }(v);
                               })
            {
              auto const& [entityID, entityGeometry] = item;
              entityGeometries.try_emplace(static_cast<EntityID>(entityID), entityGeometry);
            }
            else
            {
              static_assert(false, "EntityID cannot be determined for non-keyed entities in a non-contiguous container.");
            }
          });
        }
        else
        {
          entityGeometries.resize(detail::size(newEntities));
          std::for_each(EXEC_POL_ADD(epu) newEntities.begin(), newEntities.end(), [&](auto const& item) {
            using T = std::remove_cvref_t<decltype(item)>;

            if constexpr (std::is_convertible_v<T, typename EA::Geometry>)
            {
              entityGeometries[detail::getKeyPart(newEntities, item)] = item;
            }
            else if constexpr (requires(T v) {
                                 []<typename U>(U&& u) {
                                   auto [a, b] = u;
                                 }(v);
                               })
            {
              auto const& [entityID, entityGeometry] = item;
              assert(entityID >= existingEntityNum);
              entityGeometries[entityID - existingEntityNum] = entityGeometry;
            }
            else
            {
              static_assert(false, "EntityID cannot be determined for non-keyed entities in a non-contiguous container.");
            }
          });
        }

        auto getNewEntityGeometry = [&](EntityID id) -> EntityGeometry const& {
          if constexpr (EA::ENTITY_ID_STRATEGY == EntityIdStrategy::EntityKeyed)
            return entityGeometries.at(id);
          else
            return entityGeometries[id - existingEntityNum];
        };

        // Reverse DFS order
        int changedNodesInDFSIndex = int(changedNodesInDFS.size() - 1);
        for (int i = changedNodesInDFSIndex; i >= 0; --i)
        {
          auto const& [location, beginID] = changedNodesInDFS[i];
          auto const nodeID = m_spaceIndexing.GetNodeID(location);
          auto lcaNodeID = i == 0 ? GetRootNodeID() : SI::GetLowestCommonAncestor(changedNodesInDFS[i - 1].first, location, Base::GetMaxDepthID());

          auto nodeIt = m_nodes.find(nodeID);

          auto const& entityIDs = GetNodeEntities(&*nodeIt);
          auto& nodeGeometry = nodeIt->second.GetGeometry();
          bool const isInitialized = IsNodeGeometryInitialized(nodeGeometry);

          bool nodeChanged = false;
          typename IGM::Box nodeBox;
          if (isInitialized)
          {
            auto const& [minPoint, size] = nodeGeometry;
            nodeBox = typename IGM::Box{ minPoint, IGM::Add(minPoint, size) };
          }
          else
          {
            nodeBox = IGM::BoxInvertedInit();
            nodeChanged = true;
          }

          auto const nodeBoxOriginal = nodeBox;
          for (auto it = std::next(entityIDs.begin(), beginID); it != entityIDs.end(); ++it)
            IGM::UniteInBoxAD(nodeBox, getNewEntityGeometry(*it));

          // it is possible that it is a new Parent node (LCA), it should update by all children
          for (auto const& childNodeID : GetNodeChildren(&*nodeIt))
            IGM::UniteInBoxAD(nodeBox, GetNodeBox(GetNodeValue(childNodeID)));

          nodeChanged = nodeChanged || (nodeBoxOriginal.Min != nodeBox.Min || nodeBoxOriginal.Max != nodeBox.Max);
          if (nodeChanged)
            nodeIt->second.GetGeometry() = { nodeBox.Min, IGM::Sub(nodeBox.Max, nodeBox.Min) };

          NodeID workingNodeID = nodeID;
          typename IGM::Box workingBox = nodeBox;
          while (nodeChanged && workingNodeID != GetRootNodeID())
          {
            auto parentNodeIt = GetParentIt(m_nodes, workingNodeID);
            auto const parentNodeID = parentNodeIt->first;
            auto parentNodeBox = GetNodeBox(&*parentNodeIt);
            auto const parentNodeBoxOriginal = parentNodeBox;

            IGM::UniteInBoxAD(parentNodeBox, workingBox);
            nodeChanged = (parentNodeBoxOriginal.Min != parentNodeBox.Min || parentNodeBoxOriginal.Max != parentNodeBox.Max);

            if (nodeChanged)
              parentNodeIt->second.GetGeometry() = { parentNodeBox.Min, IGM::Sub(parentNodeBox.Max, parentNodeBox.Min) };

            if (parentNodeID == lcaNodeID || SI::GetDepthID(parentNodeID) < SI::GetDepthID(lcaNodeID))
              break;

            workingNodeID = parentNodeID;
            workingBox = parentNodeBox;
          }
        }
      }
    }

  public:
    // Insert entities into the octree. Returns true if all entities were inserted successfully, false otherwise.
    // Input: Accepted newEntities containers: std::span<EA::Geometry>, TContainer<[EA::EntityID, EA::Geometry]>, TContainer<EA:Entity>
    // Output: failedEntities contains the IDs of entities that were not inserted if there were any. (Only if CONFIG::ALLOW_OUT_OF_SPACE_INSERTION == false)
    template<typename TExecMode = SeqExec>
    bool Insert(auto&& newEntities, EA::EntityContainerView existingEntities, TExecMode execMode = {}, std::unordered_set<EntityID>* failedEntities = nullptr) noexcept
      requires std::ranges::input_range<decltype(newEntities)> &&
               (std::convertible_to<std::ranges::range_value_t<decltype(newEntities)>, typename EA::Geometry> ||
                std::convertible_to<std::ranges::range_value_t<decltype(newEntities)>, typename EA::Entity> ||
                requires(std::ranges::range_value_t<decltype(newEntities)> v) { v.first; v.second; } ||
                requires(std::ranges::range_value_t<decltype(newEntities)> v) {
                  // Structured binding check that is less likely to trigger hard errors on arrays
                  []<typename U>(U&& u) { [[maybe_unused]]auto [a, b] = u; }(v);
                })
    {
      constexpr bool IS_ELEMENT_DEPTH_SPECIFIC = (EA::GEOMETRY_TYPE != GeometryType::Point);
      constexpr dim_t DIMENSION_NO = GA::DIMENSION_NO;
      constexpr uint32_t kSortThreshold = 256;

      using Location = typename SI::Location;
      using LowestCommonAncestorCalculator = typename SI::template LowestCommonAncestorCalculator<IS_ELEMENT_DEPTH_SPECIFIC>;

      struct EntityData
      {
        Location location;
        EntityID id;
      };

      struct WorkItem
      {
        uint32_t begin;
        uint32_t end;
        uint32_t bitsRemaining;
        Location location;
      };

      auto newEntityCount = newEntities.size();
      if (newEntityCount == 0)
        return true;

      auto const maxDepthID = Base::GetMaxDepthID();
      auto const maxElementNo = Base::GetMaxElementNum();
      auto const existingEntityNum = EA::GetEntityCount(existingEntities);

      // Morton code creation
      auto buffer = std::vector<EntityData>(newEntityCount);
      EXEC_POL_DEF(ept); // GCC 11.3
      std::transform(EXEC_POL_ADD(ept) newEntities.begin(), newEntities.end(), buffer.begin(), [&](auto const& item) -> EntityData {
        using T = std::remove_cvref_t<decltype(item)>;
        if constexpr (EA::ENTITY_ID_STRATEGY == EntityIdStrategy::EntityKeyed && std::is_convertible_v<T, typename EA::Entity>)
        {
          return { m_spaceIndexing.GetLocation(EA::GetGeometry(item)), EA::GetEntityID(item) };
        }
        else if constexpr (std::is_convertible_v<T, typename EA::Geometry> && std::ranges::contiguous_range<decltype(newEntities)>)
        {
          auto const entityID = static_cast<EntityID>(existingEntityNum + detail::getID(newEntities, item));
          return { m_spaceIndexing.GetLocation(item), entityID };
        }
        else if constexpr (requires(T v) {
                             []<typename U>(U&& u) {
                               auto [a, b] = u;
                             }(v);
                           })
        {
          auto const& [entityID, entityGeometry] = item;
          return { m_spaceIndexing.GetLocation(entityGeometry), static_cast<EntityID>(entityID) };
        }
        else
        {
          static_assert(false, "EntityID cannot be determined for non-keyed entities in a non-contiguous container.");
        }
      });

      // Check if all entities are within the tree bounds
      bool isAllEntitiesInserted = true;
      if constexpr (!CONFIG::ALLOW_OUT_OF_SPACE_INSERTION)
      {
        auto const endIt =
          std::partition(buffer.begin(), buffer.end(), [](auto const& element) { return element.location.GetDepthID() != INVALID_DEPTH; });
        newEntityCount = std::distance(buffer.begin(), endIt);
        isAllEntitiesInserted = endIt == buffer.end();

        if (!isAllEntitiesInserted && failedEntities)
        {
          failedEntities->reserve(std::distance(endIt, buffer.end()));
          std::transform(endIt, buffer.end(), std::back_inserter(*failedEntities), [](auto const& element) { return element.id; });
        }

        if constexpr (EA::ENTITY_ID_STRATEGY != EntityIdStrategy::EntityKeyed)
        {
          if (!isAllEntitiesInserted)
            return false;
        }
      }

      if constexpr (CONFIG::USE_REVERSE_MAPPING)
        detail::resize(m_reverseMap, existingEntityNum + newEntityCount);

      // Configurable digit width
      // Considering 32kB L1 cache with 64B width cache lines: 512 cache lines total is the limit.
      // 3D: 6 bits/64 buckets with 2 cache lines per bucket: 128 cache lines. Other stack variabbles (e.g. histogram / offsets) are also cached.
      constexpr uint32_t kRadixBits = DIMENSION_NO > 8 ? std::min<uint32_t>(12, DIMENSION_NO) : (9 / DIMENSION_NO) * DIMENSION_NO;
      constexpr uint32_t kRadixMaxSize = 1u << kRadixBits;

      auto const totalBits = static_cast<uint32_t>(maxDepthID * DIMENSION_NO);

      // In-place partition of buffer[begin..end) using bits [shift, shift+numBits).
      auto bucketSort = [&](auto& activeBuckets, auto& histogram, auto& offsets, auto& buffer, auto const& GetBucketID) {
        auto leaderBucketIt = activeBuckets.begin();
        uint32_t leaderBucketID = 0;
        auto leaderBucketEndIdx = 0;
        int fromIdx = 0;
        for (;;)
        {
          if (fromIdx == leaderBucketEndIdx)
          {
            leaderBucketID = *leaderBucketIt;
            ++leaderBucketIt;
            if (leaderBucketIt == activeBuckets.end())
              break;

            if (histogram[leaderBucketID] == 0)
              continue;

            fromIdx = offsets[leaderBucketID];
            leaderBucketEndIdx = fromIdx + histogram[leaderBucketID];
            histogram[leaderBucketID] = 0;
          }

          auto bucketID = GetBucketID(buffer[fromIdx].location.GetLocationID());
          if (leaderBucketID == bucketID)
          {
            ++fromIdx;
          }
          else
          {
            std::swap(buffer[fromIdx], buffer[offsets[bucketID]]);

            ++offsets[bucketID];
            --histogram[bucketID];
          }
        }
      };

      // Computes per-bucket LCA fold during histogram pass (zero extra scan).
      auto const radixPartition = [&](uint32_t begin, uint32_t end, uint32_t bitsToTest, auto& workStack) {
        uint32_t const numBits = std::min(kRadixBits, bitsToTest);
        uint32_t const radixShift = bitsToTest - numBits;
        uint32_t const radixMask = (1u << numBits) - 1;

        auto const GetBucketID = [radixShift, radixMask](SI::LocationIDCR key) noexcept -> uint32_t {
          return static_cast<uint32_t>(key >> radixShift) & radixMask;
        };

        // Histogram + active bucket tracking + per-bucket LCA fold
        auto histogram = std::array<uint32_t, kRadixMaxSize>{};
        auto activeBuckets = Partitioning::flagset<kRadixMaxSize>{};
        auto bucketFolds = std::array<LowestCommonAncestorCalculator, kRadixMaxSize>{};

        for (uint32_t i = begin; i < end; ++i)
        {
          auto const& loc = buffer[i].location;
          auto const b = GetBucketID(loc.GetLocationID());
          if (histogram[b] == 0)
            bucketFolds[b] = LowestCommonAncestorCalculator(loc);
          else
            bucketFolds[b].Add(loc);

          ++histogram[b];
          activeBuckets.set(b);
        }

        // Prefix sum (only active buckets)
        auto offsets = std::array<uint32_t, kRadixMaxSize>{};
        uint32_t sum = begin;
        for (auto b : activeBuckets)
        {
          offsets[b] = sum;
          sum += histogram[b];

          auto location = bucketFolds[b].GetLocation(Base::GetMaxDepthID());

          auto const levelID = Base::GetMaxDepthID() - location.GetDepthID();
          workStack.push_back({ offsets[b], sum, std::min(levelID * DIMENSION_NO, radixShift), location });
        }

        if (activeBuckets.size() == 1)
          return;

        bucketSort(activeBuckets, histogram, offsets, buffer, GetBucketID);
      };

      // Emit range as a single node using precomputed LCA fold. Zero scan.
      auto orphanNodes = std::vector<NodeID>{};
      auto changedNodes = std::vector<std::pair<Location, std::size_t>>{};

      auto const emitCluster = [&](uint32_t begin, uint32_t end, Location const& location) {
        auto const elementNum = end - begin;
        if (elementNum == 0)
          return;

        auto nodeID = m_spaceIndexing.GetNodeID(location);
        auto [it, isInserted] = m_nodes.try_emplace(nodeID);
        auto& entitySegment = it->second.GetEntitySegment();
        auto existingElementNum = entitySegment.segment.size();
        changedNodes.emplace_back(location, existingElementNum);

        if (isInserted)
        {
          orphanNodes.push_back(nodeID);
          InitNodeGeometry(&*it);
          entitySegment = m_memoryResource.Allocate(elementNum);
        }
        else
          m_memoryResource.IncreaseSegment(entitySegment, elementNum);

        for (uint32_t i = 0; i < elementNum; ++i)
          entitySegment.segment[existingElementNum + i] = buffer[begin + i].id;

        // Update reverse map
        if constexpr (CONFIG::USE_REVERSE_MAPPING)
        {
          for (uint32_t i = 0; i < elementNum; ++i)
            UpdateReverseMap(buffer[begin + i].id, nodeID);
        }
      };

      // Linear walk over pre-sorted buffer[begin..end), emits tree nodes.
      auto const emitSortedRange = [&](uint32_t begin, uint32_t end) {
        uint32_t clusterBegin = begin;
        while (clusterBegin < end)
        {
          auto lcah = LowestCommonAncestorCalculator(buffer[clusterBegin].location);

          uint32_t clusterEnd = clusterBegin + 1;
          while (clusterEnd < end)
          {
            auto newLcah = lcah;
            newLcah.Add(buffer[clusterEnd].location);

            auto const newDepthID = newLcah.GetLocation(maxDepthID).GetDepthID();
            auto const clusterSize = clusterEnd - clusterBegin + 1;

            if (clusterSize > maxElementNo && newDepthID < maxDepthID)
            {
              auto const curDepthID = lcah.GetLocation(maxDepthID).GetDepthID();
              if (newDepthID < curDepthID)
                break;
            }

            lcah = newLcah;
            ++clusterEnd;
          }
          auto const clusterLocation = lcah.GetLocation(maxDepthID);

          if constexpr (IS_ELEMENT_DEPTH_SPECIFIC)
          {
            auto const clusterDepthID = clusterLocation.GetDepthID();
            auto const clusterSize = clusterEnd - clusterBegin;

            if (clusterSize > maxElementNo && clusterDepthID < maxDepthID)
            {
              uint32_t stuckEnd = clusterBegin;
              while (stuckEnd < clusterEnd && buffer[stuckEnd].location.GetDepthID() <= clusterDepthID)
              {
                ++stuckEnd;
              }

              if (stuckEnd < clusterEnd)
              {
                if (stuckEnd > clusterBegin)
                {
                  emitCluster(clusterBegin, stuckEnd, clusterLocation);
                  clusterBegin = stuckEnd;
                  continue;
                }
              }
            }
          }

          emitCluster(clusterBegin, clusterEnd, clusterLocation);
          clusterBegin = clusterEnd;
        }
      };


      auto workStack = std::vector<WorkItem>{};
      workStack.reserve(64);
      workStack.push_back({ 0, static_cast<uint32_t>(newEntityCount), totalBits, SI::GetRootLocation() });

      while (!workStack.empty())
      {
        auto const w = workStack.back();
        workStack.pop_back();

        auto const elementNum = w.end - w.begin;
        if (elementNum == 0)
          continue;

        // Terminal: fits in one node → emit directly using precomputed fold (zero scan)
        if (elementNum <= maxElementNo)
        {
          emitCluster(w.begin, w.end, w.location);
          continue;
        }

        // Size-based strategy: sort+walk if fits cache, else radix partition.
        // If bitsRemaining == 0, we can't partition further, so we MUST sort and walk.
        if (elementNum <= kSortThreshold || w.bitsRemaining == 0)
        {
          std::sort(buffer.begin() + w.begin, buffer.begin() + w.end, [](EntityData const& a, EntityData const& b) {
            return Location::template IsLess<IS_ELEMENT_DEPTH_SPECIFIC>(a.location, b.location);
          });
          emitSortedRange(w.begin, w.end);
        }
        else
        {
          radixPartition(w.begin, w.end, w.bitsRemaining, workStack);
        }
      }

      LinkOrphanNodes(std::move(orphanNodes));
      UpdateNodeGeometry(changedNodes, newEntities, existingEntityNum, execMode);

      return isAllEntitiesInserted;
    }


  private:
    auto AddNode(NodeIDCR nodeID) noexcept
    {
      auto [nodeIt, isNew] = m_nodes.try_emplace(nodeID);
      assert(isNew);

      InitNodeGeometry(&*nodeIt);

      auto& [parentNodeID, parentNode] = *GetParentIt(nodeID);
      auto const childID = SI::GetNonDirectChildID(parentNodeID, nodeID);

      if (!parentNode.HasChild(childID))
      {
        parentNode.LinkChild(childID, nodeID);
        // leaf node does not have MBR geometry yet
        return nodeIt;
      }

      auto const childNodeID = parentNode.GetChild(childID);
      auto const lcaNodeID = SI::GetLowestCommonAncestor(childNodeID, nodeID);
      parentNode.LinkChild(childID, lcaNodeID);

      if (nodeID == lcaNodeID)
      {
        nodeIt->second.LinkChild(SI::GetNonDirectChildID(lcaNodeID, childNodeID), childNodeID);
        return nodeIt;
      }
      else
      {
        auto [lcaIt, _] = m_nodes.emplace(lcaNodeID, Node{});
        InitNodeGeometry(&*lcaIt);

        auto& lcaNode = lcaIt->second;
        lcaNode.LinkChild(SI::GetNonDirectChildID(lcaNodeID, childNodeID), childNodeID);
        lcaNode.LinkChild(SI::GetNonDirectChildID(lcaNodeID, nodeID), nodeID);

        if constexpr (detail::is_reference_stable_v<decltype(m_nodes)>)
          return nodeIt;
        else
          return m_nodes.find(nodeID);
      }
    }

    static auto GetParentIt(auto& nodes, NodeID nodeID) noexcept
    {
      auto endIt = nodes.end();
      for (nodeID = SI::GetParentKey(nodeID); SI::IsValidKey(nodeID); nodeID = SI::GetParentKey(nodeID))
      {
        auto nodeIt = nodes.find(nodeID);
        if (nodeIt != endIt)
          return nodeIt;
      }

      return endIt;
    }

    static auto GetNodeIt(auto& nodes, auto const& searchNodeID) noexcept
    {
      auto const endIt = nodes.end();
      if (!SI::IsValidKey(searchNodeID))
        return endIt;

      auto const nodeIt = nodes.find(searchNodeID);
      if (nodeIt == endIt)
        return GetParentIt(nodes, searchNodeID);

      return nodeIt;
    }

    auto GetParentIt(NodeID nodeID) const noexcept { return GetParentIt(m_nodes, nodeID); }
    auto GetParentIt(NodeID nodeID) noexcept { return GetParentIt(m_nodes, nodeID); }
    auto GetNodeIt(auto const& searchGeometry) const noexcept { return GetNodeIt(m_nodes, m_spaceIndexing.GetNodeID(searchGeometry)); }
    auto GetNodeIt(auto const& searchGeometry) { return GetNodeIt(m_nodes, m_spaceIndexing.GetNodeID(searchGeometry)); }

    constexpr void UpdateReverseMap(EntityID entityID, NodeIDCR nodeID) noexcept
    {
      if constexpr (CONFIG::USE_REVERSE_MAPPING)
      {
        if constexpr (EA::ENTITY_ID_STRATEGY != EntityIdStrategy::EntityKeyed)
        {
          auto size = detail::size(m_reverseMap);
          if (entityID >= size)
            detail::resize(m_reverseMap, std::size_t(entityID + 1));
        }

        detail::set(m_reverseMap, entityID, nodeID);
      }
    }

    IGM::Box InitializeSubtreeMinimalNodeGeometry(MutableNodeValue nodeValue, EntityContainerView entities)
    {
      if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MBR)
      {
        auto nodeBox = IGM::BoxInvertedInit();

        // Union with all children
        for (auto const& childNodeID : GetNodeChildren(nodeValue))
        {
          auto childNodeValue = GetMutableNodeValue(childNodeID);
          auto childNodeBox = InitializeSubtreeMinimalNodeGeometry(childNodeValue, entities);
          IGM::UniteInBoxAD(nodeBox, childNodeBox);
        }

        // Union with all entities
        for (auto const& entityID : GetNodeEntities(nodeValue))
          IGM::UniteInBoxAD(nodeBox, EA::GetGeometry(entities, entityID));

        nodeValue->second.GetGeometry() = { nodeBox.Min, IGM::Sub(nodeBox.Max, nodeBox.Min) };
        return nodeBox;
      }
      else
      {
        return {};
      }
    }

    // Cascade update of node geometry by a new geometry
    template<bool IS_ROOT_UPDATE = false>
    void UpdateMinimalNodeGeometry(MutableNodeValue nodeValue, IGM::Box const& newGeometryBox)
    {
      if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MBR)
      {
        auto& nodeGeometry = nodeValue->second.GetGeometry();
        bool const isInitialized = IsNodeGeometryInitialized(nodeGeometry);
        bool isChanged = false;
        auto& [minPoint, size] = nodeGeometry;
        if (!isInitialized)
        {
          isChanged = true;

          auto nodeBox = IGM::BoxInvertedInit();
          IGM::UniteInBoxAD(nodeBox, newGeometryBox);

          // it is possible that it is a new Parent node (LCA), it should update by all children
          for (auto const& childNodeID : GetNodeChildren(nodeValue))
            IGM::UniteInBoxAD(nodeBox, GetNodeBox(GetNodeValue(childNodeID)));

          minPoint = nodeBox.Min;
          size = IGM::Sub(nodeBox.Max, nodeBox.Min);
        }
        else
        {
          for (dim_t d = 0; d < GA::DIMENSION_NO; ++d)
          {
            auto maxPoint = minPoint[d] + size[d];

            if (newGeometryBox.Min[d] < minPoint[d])
            {
              minPoint[d] = newGeometryBox.Min[d];
              isChanged = true;
            }

            if (newGeometryBox.Max[d] > maxPoint)
            {
              maxPoint = newGeometryBox.Max[d];
              isChanged = true;
            }

            size[d] = maxPoint - minPoint[d];
          }
        }

        if (isChanged)
        {
          auto parentNodeIt = GetParentIt(nodeValue->first);
          if (parentNodeIt != m_nodes.end())
            UpdateMinimalNodeGeometry(&*parentNodeIt, newGeometryBox);
        }
      }
      else if constexpr (IS_ROOT_UPDATE && (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MinPoint || CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::None))
      {
        assert(nodeValue == GetMutableRootNodeValue());
        auto nodeBox = GetNodeBox(nodeValue);
        IGM::UniteInBoxAD(nodeBox, newGeometryBox);

        Base::SetTreeSize(IGM::Sub(nodeBox.Max, nodeBox.Min));
        if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MinPoint)
          nodeValue->second.GetGeometry() = nodeBox.Min;
        else
          Base::SetRealTreeMinPoint(nodeBox.Min);
      }
    }

    // Cascade initialization of node geometry
    void InitializeMinimalNodeGeometry(MutableNodeValue nodeValue, bool isCascadeMode = false, std::optional<typename IGM::Box> entityBox = std::nullopt)
    {
      if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MBR)
      {
        auto nodeBox = IGM::BoxInvertedInit();
        if (entityBox)
          IGM::UniteInBoxAD(nodeBox, *entityBox);

        // Union with all children
        auto const& children = GetNodeChildren(nodeValue);
        for (auto const& childNodeID : children)
        {
          auto const childNodeValue = GetNodeValue(childNodeID);
          if (!IsNodeGeometryInitialized(childNodeValue->second.GetGeometry()))
          {
            assert(false);
            continue;
          }

          const auto childBox = GetNodeBox(childNodeValue);
          IGM::UniteInBoxAD(nodeBox, childBox);
        }

        // Initialize geometry
        nodeValue->second.GetGeometry() = { nodeBox.Min, IGM::Sub(nodeBox.Max, nodeBox.Min) };

        if (isCascadeMode)
        {
          auto parentNodeIt = GetParentIt(nodeValue->first);
          if (parentNodeIt != m_nodes.end())
          {
            UpdateMinimalNodeGeometry(&*parentNodeIt, nodeBox);
          }
        }
      }
    }

    // Cascade initialization of node geometry
    void InitializeMinimalNodeGeometry(MutableNodeValue nodeValue, EntityContainerView entities, bool isCascadeMode = false)
    {
      if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MBR)
      {
        auto nodeBox = IGM::BoxInvertedInit();

        // Union with all entities
        auto const& entityIDs = GetNodeEntities(nodeValue);
        for (auto const& entityID : entityIDs)
          IGM::UniteInBoxAD(nodeBox, EA::GetGeometry(entities, entityID));

        InitializeMinimalNodeGeometry(nodeValue, isCascadeMode, nodeBox);
      }
    }

    void InitializeRootNodeBox(EntityContainerView entities)
    {
      auto rootNodeValue = GetMutableRootNodeValue();
      if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MBR)
      {
        InitializeSubtreeMinimalNodeGeometry(rootNodeValue, entities);
      }
      else if constexpr (CONFIG::ALLOW_OUT_OF_SPACE_INSERTION)
      {
        auto nodeBox = GetNodeBox(rootNodeValue);
        if (!IsNodeGeometryInitialized(rootNodeValue->second.GetGeometry()))
          nodeBox = IGM::BoxInvertedInit();

        auto const& entityIDs = GetNodeEntities(rootNodeValue);
        for (auto const& entityID : entityIDs)
          IGM::UniteInBoxAD(nodeBox, EA::GetGeometry(entities, entityID));

        if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::None)
        {
          Base::SetRealTreeMinPoint(nodeBox.Min);
          Base::SetTreeSize(IGM::Sub(nodeBox.Max, nodeBox.Min));
        }
        else if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MinPoint)
        {
          rootNodeValue->second.GetGeometry() = nodeBox.Min;
          Base::SetTreeSize(IGM::Sub(nodeBox.Max, nodeBox.Min));
        }
      }
    }

#ifdef ORTHOTREE__PUBLIC_NODE_INTERFACE
  public:
#else
  private:
#endif // ORTHOTREE__PUBLIC_NODE_INTERFACE

    constexpr NodeID Find(EntityID entityID) const noexcept
    {
      if constexpr (CONFIG::USE_REVERSE_MAPPING)
      {
        auto const pNodeID = detail::get_if(m_reverseMap, entityID);
        if (!pNodeID)
          return GetNoneNodeID();

        return *pNodeID;
      }
      else
      {
        auto const it = std::find_if(m_nodes.begin(), m_nodes.end(), [&](auto const& keyAndValue) {
          auto const& entities = GetNodeEntities(&keyAndValue);
          return std::ranges::find(entities, entityID) != entities.end();
        });
        return it == m_nodes.end() ? GetNoneNodeID() : it->first;
      }
    }

    bool IsEveryEntityUnique() const noexcept
    {
#ifdef ORTHOTREE__DEBUG_CHECKS
      auto ids = std::vector<EntityID>();
      ids.reserve(100);
      std::for_each(m_nodes.begin(), m_nodes.end(), [&](auto& nodeValue) {
        auto const& entities = GetNodeEntities(&nodeValue);
        ids.insert(ids.end(), entities.begin(), entities.end());
      });

      auto const idsSizeBeforeUnique = ids.size();
      detail::sortAndUnique(ids);
      return idsSizeBeforeUnique == ids.size();
#else
      return true;
#endif
    }

  private:
    bool IsGeometryInTree(EA::Geometry const& entityGeometry, TFloatScalar tolerance) const noexcept
    {
      const auto treeBox = GetBox();
      if constexpr (EA::GEOMETRY_TYPE == GeometryType::Point)
      {
        return GA::DoesBoxContainPoint(treeBox, entityGeometry, tolerance);
      }
      else if constexpr (EA::GEOMETRY_TYPE == GeometryType::Box)
      {
        return GA::AreBoxesOverlapped(treeBox, entityGeometry, true, false, tolerance);
      }
      else
      {
        static_assert(false, "Unsupported geometry type!");
      }
    }

    enum class OutOfSpaceInsertionResult
    {
      NotHandled,
      InsertedIntoRoot,
      Denied
    };
    OutOfSpaceInsertionResult HandleOutOfSpaceInsertion(EntityID entityID, EA::Geometry const& entityGeometry)
    {
      if (!IsGeometryInTree(entityGeometry, GA::BASE_TOLERANCE))
      {
        if constexpr (CONFIG::ALLOW_OUT_OF_SPACE_INSERTION)
        {
          auto nodeValue = GetMutableNodeValue(GetRootNodeID());
          AddNodeEntity(nodeValue, entityID);
          UpdateMinimalNodeGeometry<true>(nodeValue, IGM::GetBoxAD(entityGeometry));
          return OutOfSpaceInsertionResult::InsertedIntoRoot;
        }
        else
        {
          return OutOfSpaceInsertionResult::Denied;
        }
      }

      return OutOfSpaceInsertionResult::NotHandled;
    }

  public:
    // Insert entity into the tree. If allowLeafCreation is true: The smallest node will be chosen by the max depth. If allowLeafCreation is false: The smallest existing level on the branch will be chosen.
    bool InsertIntoLeaf(EntityID entityID, EA::Geometry const& entityGeometry, InsertionMode insertionMode = InsertionMode::ExistingLeaf) noexcept
    {
      assert([insertionMode] {
        switch (insertionMode)
        {
        case InsertionMode::Balanced: return false;
        case InsertionMode::LowestLeaf:
        case InsertionMode::ExistingLeaf: return true;
        }
        return false;
      }());

      auto const oosResult = HandleOutOfSpaceInsertion(entityID, entityGeometry);
      if (oosResult != OutOfSpaceInsertionResult::NotHandled)
        return oosResult != OutOfSpaceInsertionResult::Denied;

      auto const entityNodeID = m_spaceIndexing.GetNodeID(entityGeometry);
      auto nodeIt = m_nodes.find(entityNodeID);
      if (nodeIt == m_nodes.end())
      {
        if (insertionMode == InsertionMode::LowestLeaf)
        {
          nodeIt = AddNode(entityNodeID);
        }
        else
        {
          nodeIt = GetParentIt(entityNodeID);
          if (nodeIt->second.IsAnyChildExist())
          {
            auto const childID = SI::GetNonDirectChildID(nodeIt->first, entityNodeID);
            auto const childGenerator = typename SI::ChildKeyGenerator(nodeIt->first);
            auto const childNodeID = childGenerator.GetChildNodeKey(childID);

            nodeIt = AddNode(childNodeID);
          }
        }
      }

      AddNodeEntity(&*nodeIt, entityID);
      UpdateMinimalNodeGeometry(&*nodeIt, IGM::GetBoxAD(entityGeometry));

#ifdef ORTHOTREE__DEBUG_CHECKS
      assert(IsEveryEntityUnique()); // Assert means: index is already added. Wrong input!
#endif

      return true;
    }

    // Insert entity into the tree with node rebalancing
    bool Insert(EntityID entityID, EA::Geometry const& entityGeometry, EntityContainerView entities) noexcept
    {
      auto const oosResult = HandleOutOfSpaceInsertion(entityID, entityGeometry);
      if (oosResult != OutOfSpaceInsertionResult::NotHandled)
        return oosResult != OutOfSpaceInsertionResult::Denied;


      // Rules:
      // 1. If the node has a child, it is already split
      // 2. Split nodes contain stucked boxes only, In other words: new elements that would be inserted in the node (but not stucked), if the node
      // has any child node, the new element should be inserted into a child.
      // 3. Rebalance push elements downward only once.

      auto const entityNodeID = m_spaceIndexing.GetNodeID(entityGeometry);
      auto nodeIt = m_nodes.find(entityNodeID);
      auto const isNonRefinable = nodeIt != m_nodes.end();
      if (nodeIt == m_nodes.end())
        nodeIt = GetParentIt(entityNodeID);

      auto const nodeValue = &*nodeIt;
      auto const entityCount = GetNodeEntityCount(nodeValue) + 1;
      auto const isOverflowed = entityCount > Base::GetMaxElementNum();
      auto const isPreviouslyOverflowed = nodeValue->second.IsAnyChildExist();
      auto const depthID = SI::GetDepthID(nodeValue->first);

      if (isNonRefinable)
        UpdateMinimalNodeGeometry(nodeValue, IGM::GetBoxAD(entityGeometry));

      // No overflow or non-refinable or max depth reached
      if ((!isPreviouslyOverflowed && !isOverflowed) || (isPreviouslyOverflowed && isNonRefinable) || depthID == Base::GetMaxDepthID())
      {
        AddNodeEntity(nodeValue, entityID);
        return true;
      }

      // Previously overflowed but now non-overflowed. Just a direct child node creation is required.
      if (isPreviouslyOverflowed)
      {
        assert(!isNonRefinable);

        nodeIt = AddNode(SI::GetDirectChildNodeID(nodeValue->first, entityNodeID));
        AddNodeEntity(&*nodeIt, entityID);
        UpdateMinimalNodeGeometry(&*nodeIt, IGM::GetBoxAD(entityGeometry));

        return true;
      }

      // Handling overflow

      assert(isOverflowed);

      auto const& originalEntityIDs = GetNodeEntities(nodeValue);

      auto mortonIDs = std::vector<typename SI::Location>(entityCount);
      auto entityIDs = std::vector<EntityID>{};
      entityIDs.reserve(entityCount);
      entityIDs.assign(originalEntityIDs.begin(), originalEntityIDs.end());
      entityIDs.emplace_back(entityID);

      auto locationsZip = detail::zip_view(mortonIDs, entityIDs);
      using ZipValue = decltype(locationsZip)::iterator::value_type;
      std::transform(entityIDs.begin(), entityIDs.end() - 1, locationsZip.begin(), [&](auto const& storedEntityID) -> ZipValue {
        return { m_spaceIndexing.GetLocation(EA::GetGeometry(entities, storedEntityID)), storedEntityID };
      });

      // new entity is may not added to the geometry container
      auto lastIt = locationsZip.end() - 1;
      *lastIt = { m_spaceIndexing.GetLocation(entityGeometry), entityID };

      auto beginIt = locationsZip.begin();

      // Non-refinable box entities
      if constexpr (EA::GEOMETRY_TYPE == GeometryType::Point)
      {
        ClearNodeEntities(nodeValue->second);
      }
      else if constexpr (EA::GEOMETRY_TYPE == GeometryType::Box)
      {
        beginIt = std::partition(beginIt, locationsZip.end(), [&](auto const& element) { return element.GetFirst().GetDepthID() == depthID; });

        auto const isAllNonRefinable = beginIt == locationsZip.end();

        if (isAllNonRefinable && isNonRefinable)
        {
          AddNodeEntity(nodeValue, entityID);
          return true;
        }

        ClearNodeEntities(nodeValue->second);
        for (auto it = locationsZip.begin().GetSecond(); it != beginIt.GetSecond(); ++it)
          AddNodeEntity(nodeValue, *it);

        // Update parent node geometry once to avoid multiple updates
        UpdateMinimalNodeGeometry(nodeValue, IGM::GetBoxAD(entityGeometry));
      }
      else
      {
        static_assert(false);
      }

      constexpr bool IS_ELEMENT_DEPTH_SPECIFIC = (EA::GEOMETRY_TYPE != GeometryType::Point);
      using LCAC = typename SI::template LowestCommonAncestorCalculator<IS_ELEMENT_DEPTH_SPECIFIC>;

      // Child nodes
      auto const childNodeLevelID = Base::GetMaxDepthID() - depthID - 1;
      while (beginIt != locationsZip.end())
      {
        auto const nodeChecker = typename SI::ChildCheckerFixedDepth(childNodeLevelID, beginIt.GetFirst()->GetLocationID());
        auto const nextIt =
          std::partition(beginIt, locationsZip.end(), [&](auto const& element) { return nodeChecker.Test(element.GetFirst().GetLocationID()); });

        // Create child node deeper if possible
        auto lcaHelper = LCAC(*beginIt.GetFirst());
        for (auto it = std::next(beginIt.GetFirst()); it != nextIt.GetFirst(); ++it)
          lcaHelper.Add(*it);

        auto lcaNodeID = lcaHelper.GetNodeID(Base::GetMaxDepthID());
        assert(
          lcaHelper.GetLocation(Base::GetMaxDepthID()).GetDepthID() >= depthID &&
          "Existing entities are out-of-sync with the tree. Client code may have modified the geometry of the existing entities without updating "
          "the tree.");

        auto childNodeIt = AddNode(lcaNodeID);
        auto childNodeValue = &*childNodeIt;

        bool isNewEntityContained = false;

        // Insert entities to the child node
        for (auto it = beginIt.GetSecond(); it != nextIt.GetSecond(); ++it)
        {
          // New entity will be added separately, to avoid non-containment issues in InitializeMinimalNodeGeometry
          if (entityID == *it)
          {
            isNewEntityContained = true;
            continue;
          }

          AddNodeEntity(childNodeValue, *it);
        }

        // Update child node geometry. Parent node geometry is already updated, cascading is not required.
        InitializeMinimalNodeGeometry(childNodeValue, entities, false);
        if (isNewEntityContained)
        {
          AddNodeEntity(childNodeValue, entityID);
          UpdateMinimalNodeGeometry(childNodeValue, IGM::GetBoxAD(entityGeometry));
        }

        beginIt = nextIt;
      }

#ifdef ORTHOTREE__DEBUG_CHECKS
      assert(IsEveryEntityUnique()); // Assert means: index is already added. Wrong input!
#endif

      return true;
    }


    // Update id by the new bounding box information
    bool Update(EntityID entityID, EA::Geometry const& newEntityGeometry, InsertionMode insertionMode = InsertionMode::ExistingLeaf) noexcept
    {
      if constexpr (!CONFIG::ALLOW_OUT_OF_SPACE_INSERTION)
      {
        if (!IsGeometryInTree(newEntityGeometry, GA::BASE_TOLERANCE))
          return false;
      }

      if (!EraseBase<true>(entityID))
        return false;

      bool isInserted = InsertIntoLeaf(entityID, newEntityGeometry, insertionMode);
      if (!isInserted)
        ErasePostUpdate<false>(entityID); // Finish the erase if the update is failed due to out-of-space insertion.

      return isInserted;
    }

    // Update id by the new bounding box information and the erase part is aided by the old bounding box geometry data
    bool Update(
      EntityID entityID,
      EA::Geometry const& oldEntityGeometry,
      EA::Geometry const& newEntityGeometry,
      InsertionMode insertionMode = InsertionMode::ExistingLeaf) noexcept
    {
      if constexpr (CONFIG::USE_REVERSE_MAPPING)
      {
        return Update(entityID, newEntityGeometry, insertionMode);
      }
      else
      {
        if (!EraseBase<true>(entityID, oldEntityGeometry))
          return false; // entityID was not registered previously.

        bool isInserted = InsertIntoLeaf(entityID, newEntityGeometry, insertionMode);
        if (!isInserted)
          ErasePostUpdate<false>(entityID); // Finish the erase if the update is failed due to out-of-space insertion.

        return isInserted;
      }
    }

    // Update id with rebalancing by the new bounding box information
    bool Update(EntityID entityID, EA::Geometry const& newEntityGeometry, EntityContainerView entities) noexcept
    {
      if (!EraseBase<true>(entityID))
        return false;

      bool isInserted = Insert(entityID, newEntityGeometry, entities);
      if (!isInserted)
        ErasePostUpdate<false>(entityID); // Finish the erase if the update is failed due to out-of-space insertion.

      return isInserted;
    }

    // Update id with rebalancing by the new bounding box information and the erase part is aided by the old bounding box geometry data
    bool Update(EntityID entityID, EA::Geometry const& oldEntityGeometry, EA::Geometry const& newEntityGeometry, EntityContainerView const& entities) noexcept
    {
      if constexpr (CONFIG::USE_REVERSE_MAPPING)
      {
        return Update(entityID, newEntityGeometry, entities);
      }
      else
      {
        if (!EraseBase<true>(entityID, oldEntityGeometry))
          return false; // entityID was not registered previously.

        auto isInserted = Insert(entityID, newEntityGeometry, entities);
        if (!isInserted)
          ErasePostUpdate<false>(entityID); // Finish the erase if the update is failed due to out-of-space insertion.

        return isInserted;
      }
    }

    // Update all element which are in the given hash-table. Use with std::move
    template<typename TExecMode = SeqExec>
    void UpdateIndexes(
      std::unordered_map<EntityID, std::conditional_t<EA::ENTITY_ID_STRATEGY == EntityIdStrategy::ContiguousIndex, EntityID, std::optional<EntityID>>> updateMap,
      TExecMode execMode = {}) noexcept
    {
      auto const updateMapEndIterator = updateMap.end();

      auto const UpdateNodes = [&](auto& node) {
        auto& entityIDs = node.second.GetEntities();
        auto entityNo = entityIDs.size();
        for (std::size_t i = 0; i < entityNo; ++i)
        {
          auto it = updateMap.find(entityIDs[i]);
          if (it == updateMapEndIterator)
            continue;

          if constexpr (EA::ENTITY_ID_STRATEGY == EntityIdStrategy::ContiguousIndex)
          {
            entityIDs[i] = it->second;
          }
          else
          {
            if (it->second)
              entityIDs[i] = *it->second;
            else
            {
              --entityNo;
              entityIDs[i] = entityIDs[entityNo];
              --i;
            }
          }
          if constexpr (std::is_same_v<TExecMode, SeqExec>)
            updateMap.erase(it);
        }

        ResizeNodeEntities(node.second, entityNo);
      };

      if constexpr (CONFIG::USE_REVERSE_MAPPING)
      {
        auto nodes = std::unordered_set<NodeID>{};
        for (auto const [oldEntityID, newEntityID] : updateMap)
        {
          nodes.emplace(detail::at(m_reverseMap, oldEntityID));

          if (!newEntityID && EA::ENTITY_ID_STRATEGY == EntityIdStrategy::EntityKeyed)
            detail::erase(m_reverseMap, oldEntityID);
        }

        EXEC_POL_DEF(ep);
        std::for_each(EXEC_POL_ADD(ep) nodes.begin(), nodes.end(), [&](auto nodeID) { UpdateNodes(*m_nodes.find(nodeID)); });
      }
      else
      {
        EXEC_POL_DEF(ep);
        std::for_each(EXEC_POL_ADD(ep) m_nodes.begin(), m_nodes.end(), UpdateNodes);
      }

#ifdef ORTHOTREE__DEBUG_CHECKS
      assert(IsEveryEntityUnique()); // Assert means: index replacements causes that multiple object has the same id. Wrong input!
#endif
    }

    // Reset the tree
    void Reset() noexcept
    {
      m_nodes.clear();
      if constexpr (CONFIG::USE_REVERSE_MAPPING)
      {
        detail::clear(m_reverseMap);
      }
      m_memoryResource.Reset();
      m_spaceIndexing = {};
    }


    // Remove all elements and ids, except Root
    void Clear() noexcept
    {
      auto& rootNode = m_nodes.at(GetRootNodeID());
      rootNode.Clear();
      auto rootNodeCopy = rootNode;

      m_nodes.clear();
      m_nodes.emplace(GetRootNodeID(), std::move(rootNodeCopy));
    }


    // Move the whole tree with a std::vector of the movement
    template<typename TExecMode = SeqExec>
    void Move(TVector const& moveVector, TExecMode execMode = {}) noexcept
    {
      if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::None)
      {
        return;
      }
      else if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MinPoint || CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MBR)
      {
        EXEC_POL_DEF(ep); // GCC 11.3
        std::for_each(EXEC_POL_ADD(ep) m_nodes.begin(), m_nodes.end(), [&moveVector](auto& pairKeyNode) {
          if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MinPoint)
          {
            auto& minPoint = pairKeyNode.second.GetGeometry();
            IGM::MoveAD(minPoint, moveVector);
          }
          else if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MBR)
          {
            auto& box = pairKeyNode.second.GetGeometry();
            IGM::MoveAD(box.minPoint, moveVector);
          }
        });
        m_spaceIndexing.Move(moveVector);
      }
      else
      {
        static_assert(false, "Unsupported NODE_GEOMETRY_STORAGE value!");
      }
    }

  private:
    void RemoveNodeIfPossible(auto nodeIt) noexcept
    {
      while (true)
      {
        auto& [nodeID, node] = *nodeIt;
        if (nodeID == GetRootNodeID())
          return;

        if (node.IsAnyChildExist() || !IsNodeEntitiesEmpty(&*nodeIt))
          return;

        m_memoryResource.Deallocate(node.GetEntitySegment());
        auto parentNodeIt = GetParentIt(nodeID);

        auto& [parentNodeID, parentNode] = *parentNodeIt;
        parentNode.UnlinkChild(SI::GetNonDirectChildID(parentNodeID, nodeID));
        m_nodes.erase(nodeIt);

        nodeIt = parentNodeIt;
      }
    }

    void DecreaseEntityIDs([[maybe_unused]] EntityID entityID) noexcept
    {
      if constexpr (EA::ENTITY_ID_STRATEGY == EntityIdStrategy::ContiguousIndex)
      {
        for (auto& [_, node] : m_nodes)
          node.DecreaseEntityIDs(entityID);

        if constexpr (CONFIG::USE_REVERSE_MAPPING && EA::ENTITY_ID_STRATEGY == EntityIdStrategy::ContiguousIndex)
          detail::decrementKeys(m_reverseMap, entityID);
      }
    }

    template<bool IS_PART_OF_UPDATE = false>
    constexpr void ErasePostUpdate(EntityID entityID) noexcept
    {
      if constexpr (IS_PART_OF_UPDATE)
      {
        if constexpr (CONFIG::USE_REVERSE_MAPPING)
        {
          if constexpr (EA::ENTITY_ID_STRATEGY == EntityIdStrategy::EntityKeyed)
            detail::erase(m_reverseMap, entityID);
        }
      }
      else
      {
        if constexpr (EA::ENTITY_ID_STRATEGY == EntityIdStrategy::ContiguousIndex)
          DecreaseEntityIDs(entityID);
        else if constexpr (CONFIG::USE_REVERSE_MAPPING && EA::ENTITY_ID_STRATEGY == EntityIdStrategy::EntityKeyed)
          detail::erase(m_reverseMap, entityID);
      }
    }

    template<bool IS_PART_OF_UPDATE = false>
    constexpr bool EraseBase(EntityID entityID) noexcept
    {
      bool isErased = false;

      if constexpr (CONFIG::USE_REVERSE_MAPPING)
      {
        auto pNodeID = detail::get_if(m_reverseMap, entityID);
        if (!pNodeID)
          return false;

        auto nodeIt = m_nodes.find(*pNodeID);
        if (!RemoveNodeEntity(nodeIt->second, entityID))
          return false;

        RemoveNodeIfPossible(nodeIt);
        isErased = true;
      }
      else
      {
        for (auto nodeIt = m_nodes.begin(); nodeIt != m_nodes.end(); ++nodeIt)
        {
          if (!RemoveNodeEntity(nodeIt->second, entityID))
            continue;

          isErased = true;
          RemoveNodeIfPossible(nodeIt);
          break;
        }
      }

      if (!isErased)
        return false;

      ErasePostUpdate<IS_PART_OF_UPDATE>(entityID);
      return true;
    }

    template<bool IS_PART_OF_UPDATE = false>
    constexpr bool EraseBase(EntityID entitiyID, EA::Geometry const& entityGeometry) noexcept
    {
      auto nodeIt = GetNodeIt(entityGeometry);
      if (nodeIt == m_nodes.end())
        return false; // entity's geometry is not in the handled space

      // entityID may be in a parent node, so we need to check all the way up to the root
      while (!RemoveNodeEntity(nodeIt->second, entitiyID))
      {
        nodeIt = GetParentIt(nodeIt->first);
        if (nodeIt == m_nodes.end())
          return false; // entity is not in the tree
      }

      RemoveNodeIfPossible(nodeIt);
      ErasePostUpdate<IS_PART_OF_UPDATE>(entitiyID);
      return true;
    }

  public: // Entity handling
    // Erase entity via reverse mapping or brute force search. Reverse mapping is recommended.
    constexpr bool Erase(EntityID entityID) noexcept { return EraseBase(entityID); }

    // Erase id, aided with the original geometry. Reverse mapping is not used in this function, consider its usage, with the alternative Erase().
    constexpr bool Erase(EntityID entityID, EA::Geometry const& entityGeometry) noexcept { return EraseBase(entityID, entityGeometry); }

  public: // Search functions
    bool Contains(EA::Geometry const& geometry, EntityContainerView entities, TFloatScalar tolerance = GA::BASE_TOLERANCE) const noexcept
    {
      auto const nodeIt = GetNodeIt(geometry);
      if (nodeIt == m_nodes.end())
        return false;

      auto const& nodeEntityIDs = GetNodeEntities(&*nodeIt);
      return std::any_of(nodeEntityIDs.begin(), nodeEntityIDs.end(), [&](auto const& entityID) {
        if constexpr (EA::GEOMETRY_TYPE == GeometryType::Point)
        {
          return GA::ArePointsEqual(geometry, EA::GetGeometry(entities, entityID), tolerance);
        }
        else if constexpr (EA::GEOMETRY_TYPE == GeometryType::Box)
        {
          return GA::AreBoxesEqual(geometry, EA::GetGeometry(entities, entityID), tolerance);
        }
        else
        {
          static_assert(false, "Unsupported geometry type!");
        }
        ORTHOTREE_UNREACHABLE();
      });
    }

    bool Contains(EntityID const& entityID, EntityContainerView entities, TFloatScalar tolerance = GA::BASE_TOLERANCE) const noexcept
    {
      if constexpr (CONFIG::USE_REVERSE_MAPPING)
      {
        return detail::get_if(m_reverseMap, entityID) != nullptr;
      }
      else
      {
        if (entities.size() == 0)
          return false;

        auto const& entityGeometry = EA::GetGeometry(entities, entityID);
        return Contains(entityGeometry, entities, tolerance);
      }
    }
  };
} // namespace OrthoTree


namespace OrthoTree
{
  // Non-owning ortho-tree with compact SoA storage for not changing datasets.
  template<typename TEntityAdapter, typename TGeometryAdapter, typename TConfiguration>
  class StaticLinearOrthoTreeCore : public OrthoTreeCoreBase<TEntityAdapter, TGeometryAdapter, TConfiguration>
  {
  public:
    using Base = OrthoTreeCoreBase<TEntityAdapter, TGeometryAdapter, TConfiguration>;
    ORTHOTREE_DEPENDENT_TYPES(Base);

  public:
    using SI = typename Base::SI;

    using NodeID = uint32_t;
    using NodeIDCR = NodeID;

    using NodeValue = NodeID;

  private:
    static_assert(GA::DIMENSION_NO <= 16);
    static_assert(SI::IS_LINEAR_TREE);

  private:
    using MGSI = typename detail::MortonGridSpaceIndexing<GA, CONFIG::ALLOW_OUT_OF_SPACE_INSERTION, CONFIG::LOOSE_FACTOR, CONFIG::MAX_ALLOWED_DEPTH_ID>;

    using Segment256 = detail::NodeStorage256::EntitySegment;
    using Segment65536 = detail::NodeStorage65536::EntitySegment;
    using SegmentGeneral = detail::NodeStorageGeneral::EntitySegment;

    using NodeStorage256 = detail::NodeStorage256;
    using NodeStorage65536 = detail::NodeStorage65536;
    using NodeStorageGeneral = detail::NodeStorageGeneral;

    using NodeGeometry =
      std::conditional_t<CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::None, std::monostate, std::vector<typename Base::NodeGeometry>>;

  private: // Data
    std::variant<NodeStorage256, NodeStorage65536, NodeStorageGeneral> m_nodes;
    std::vector<uint8_t> m_nodeDepthIDs;
    std::vector<EntityID> m_entityStorage;
    NodeGeometry m_nodeGeometry;

  private: // Serialization
    static constexpr uint32_t SERIALIZED_VERSION_ID = 0;

    template<typename TArchive, typename TEntityAdapter_, typename TGeometryAdapter_, typename TConfiguration_>
    friend void serialize(TArchive& ar, StaticLinearOrthoTreeCore<TEntityAdapter_, TGeometryAdapter_, TConfiguration_>& core);

  public: // Constructors
    // Default constructor. Requires Create call before usage.
    constexpr StaticLinearOrthoTreeCore() = default;

    constexpr StaticLinearOrthoTreeCore(StaticLinearOrthoTreeCore&&) = default;

    // Initialize the base octree structure with entity collection
    template<typename TExecMode = SeqExec>
    explicit StaticLinearOrthoTreeCore(
      EA::EntityContainerView entities,
      std::optional<depth_t> maxDepthIDIn = std::nullopt,
      std::optional<TBox> boxSpaceOptional = std::nullopt,
      std::size_t maxElementNoInNode = CONFIG::DEFAULT_TARGET_ELEMENT_NUM_IN_NODES,
      TExecMode execMode = {}) noexcept
    {
      [[maybe_unused]] auto isSuccessfullyInsertedAllElements =
        Create(entities, maxDepthIDIn, std::move(boxSpaceOptional), maxElementNoInNode, execMode);
      assert(isSuccessfullyInsertedAllElements);
    }

    // Initialize the base octree structure with entity collection and parallel tree-building option
    template<typename TExecMode>
    StaticLinearOrthoTreeCore(
      TExecMode execMode,
      EntityContainerView entities,
      std::optional<depth_t> maxDepthIDIn = std::nullopt,
      std::optional<TBox> boxSpaceOptional = std::nullopt,
      std::size_t maxElementNoInNode = CONFIG::DEFAULT_TARGET_ELEMENT_NUM_IN_NODES) noexcept
    {
      [[maybe_unused]] auto isSuccessfullyInsertedAllElements =
        Create(entities, maxDepthIDIn, std::move(boxSpaceOptional), maxElementNoInNode, execMode);
      assert(isSuccessfullyInsertedAllElements);
    }

  public:
    constexpr TBox GetBox() const noexcept
    {
      auto box = GA::MakeBox();
      auto const& minPoint = [&]() -> decltype(auto) {
        if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MBR)
          return Base::GetTreeBoxNominalMinPoint();
        else if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MinPoint)
          return m_nodeGeometry[GetRootNodeID()];
        else if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::None)
        {
          if constexpr (CONFIG::ALLOW_OUT_OF_SPACE_INSERTION)
            return Base::GetTreeBoxRealMinPoint();
          else
            return Base::GetTreeBoxNominalMinPoint();
        }
      }();

      auto const& size = [&]() -> decltype(auto) {
        if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MBR)
          return Base::GetTreeBoxNominalSize();
        else if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MinPoint)
          return Base::GetNodeSize(0);
        else if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::None)
        {
          if constexpr (CONFIG::ALLOW_OUT_OF_SPACE_INSERTION)
            return Base::GetTreeBoxRealSize();
          else
            return Base::GetTreeBoxNominalSize();
        }
      }();

      detail::static_for<GA::DIMENSION_NO>([&](auto dimensionID) {
        GA::SetBoxMinC(box, dimensionID, minPoint[dimensionID]);
        GA::SetBoxMaxC(box, dimensionID, minPoint[dimensionID] + size[dimensionID]);
      });

      return box;
    }


    // Node API
#ifdef ORTHOTREE__PUBLIC_NODE_INTERFACE
  public:
#else
  protected:
#endif
    static NodeID GetRootNodeID() noexcept { return 0; }

    constexpr NodeID GetRootNodeValue() const noexcept { return GetRootNodeID(); }

    constexpr NodeID GetNodeValue(NodeID nodeID) const noexcept { return nodeID; }

    constexpr std::size_t GetNodeCount() const noexcept { return m_nodeDepthIDs.size(); }

    constexpr std::size_t GetNodeEntityCount(NodeValue nodeID) const noexcept
    {
      return std::visit([&](auto const& nodes) { return nodes.nodeEntitySegment[nodeID].length; }, m_nodes);
    }

    constexpr SequenceView<NodeID> GetNodeChildren(NodeValue nodeID) const noexcept
    {
      return std::visit(
        [nodeID](auto const& nodes) {
          if (nodeID >= nodes.nodeChildSegmentBegins.size() - 1)
            return SequenceView<NodeID>(static_cast<NodeID>(0), static_cast<NodeID>(0));

          return SequenceView<NodeID>(
            static_cast<NodeID>(nodes.nodeChildSegmentBegins[nodeID]), static_cast<NodeID>(nodes.nodeChildSegmentBegins[nodeID + 1]));
        },
        m_nodes);
    }

    constexpr std::span<EntityID const> GetNodeEntities(NodeValue nodeID) const noexcept
    {
      return std::visit(
        [this, nodeID](auto const& nodes) {
          auto const [beginID, length] = nodes.nodeEntitySegment[nodeID];
          return std::span<EntityID const>(&m_entityStorage[beginID], length);
        },
        m_nodes);
    }

    constexpr bool IsNodeEntitiesEmpty(NodeValue nodeID) const noexcept
    {
      return std::visit([nodeID](auto const& nodes) { return nodes.nodeEntitySegment[nodeID].length == 0; }, m_nodes);
    }

    constexpr decltype(auto) GetNodeMinPoint(NodeValue nodeID) const noexcept
    {
      static_assert(CONFIG::NODE_GEOMETRY_STORAGE != NodeGeometryStorage::None, "Real-time min-point calculation is not supported!");

      if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MinPoint)
      {
        return m_nodeGeometry[nodeID];
      }
      else if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MBR)
      {
        return m_nodeGeometry[nodeID].minPoint;
      }
      else
      {
        static_assert(false, "Unsupported node geometry storage!");
      }
    }

    constexpr decltype(auto) GetNodeSize(NodeValue nodeID) const noexcept
    {
      if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MinPoint || CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::None)
      {
        return Base::GetNodeSize(m_nodeDepthIDs[nodeID]);
      }
      else if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MBR)
      {
        return m_nodeGeometry[nodeID].size;
      }
      else
      {
        static_assert(false, "Unsupported node geometry storage!");
      }
    }

    constexpr IGM::Box GetNodeBox(NodeValue nodeID) const noexcept
    {
      if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MinPoint)
      {
        return Base::GetNodeBox(m_nodeDepthIDs[nodeID], m_nodeGeometry[nodeID]);
      }
      else if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MBR)
      {
        auto const& [minPoint, size] = m_nodeGeometry[nodeID];
        return { .Min = minPoint, .Max = IGM::Add(minPoint, size) };
      }
      else
      {
        static_assert(false, "Unsupported node geometry storage!");
      }
    }

  private: // Build
    constexpr void InitBase(IGM::Vector const& boxSpaceMinPoint, IGM::Vector const& boxSpaceSize, depth_t maxDepthID, std::size_t maxElementNo) noexcept
    {
      Base::InitBase(boxSpaceMinPoint, boxSpaceSize, maxDepthID, maxElementNo);

      ORTHOTREE_CRASH_IF(
        GetNodeCount() > 0, "To build/setup/create the tree, use the Create() [recommended] or Init() function. If an already built tree is wanted to be reset, use the Reset() function before Init().");
    }

  private:
    template<typename TExecMode = SeqExec>
    void InitializeMinimalNodeGeometry(EntityContainerView entities, TExecMode execMode = {})
    {
      if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MBR)
      {
        auto const nodeCount = static_cast<uint32_t>(m_nodeDepthIDs.size());
        if (nodeCount == 0)
          return;

        // Group nodes by depth for safe parallel/iterative bottom-up processing
        std::vector<std::vector<uint32_t>> nodesAtDepth(Base::GetMaxDepthID() + 1);
        for (uint32_t i = 0; i < nodeCount; ++i)
          nodesAtDepth[m_nodeDepthIDs[i]].push_back(i);

        auto const processNode = [&](uint32_t nodeID) {
          auto nodeBox = IGM::BoxInvertedInit();

          std::visit(
            [&](auto const& nodes) {
              auto const [beginID, length] = nodes.nodeEntitySegment[nodeID];
              for (uint32_t j = beginID, endID = beginID + length; j < endID; ++j)
                IGM::UniteInBoxAD(nodeBox, EA::GetGeometry(entities, m_entityStorage[j]));

              if (nodes.nodeChildSegmentBegins.size() <= nodeID + 1)
                return;

              auto const childBeginNodeID = nodes.nodeChildSegmentBegins[nodeID];
              auto const childEndNodeID = nodes.nodeChildSegmentBegins[nodeID + 1];
              for (auto childID = childBeginNodeID; childID < childEndNodeID; ++childID)
                IGM::UniteInBoxAD(nodeBox, GetNodeBox(childID));
            },
            m_nodes);

          m_nodeGeometry[nodeID] = { nodeBox.Min, IGM::Sub(nodeBox.Max, nodeBox.Min) };
        };

        // Process depths bottom-up
        for (int d = static_cast<int>(Base::GetMaxDepthID()); d >= 0; --d)
        {
          auto const& currentLevelNodes = nodesAtDepth[d];
          if (currentLevelNodes.empty())
            continue;

          EXEC_POL_DEF(ep);
          std::for_each(EXEC_POL_ADD(ep) currentLevelNodes.begin(), currentLevelNodes.end(), processNode);
        }
      }
    }

  private:
    constexpr std::size_t GetMaxPossibleNodeCount(std::size_t entityCount) const noexcept
    {
      // Assuming perfect tree
      // - All entities are in a leaf node as a single entity

      auto const calculateNodeNum = [](depth_t depthID) {
        return (detail::pow2(GA::DIMENSION_NO * depthID) - 1) / (SI::CHILD_NO - 1);
      };

      auto const maxNodeNumByMaxDepth = calculateNodeNum(Base::GetMaxDepthID());
      auto const possiblyFullLevels = std::min(Base::GetMaxDepthID(), std::bit_width(entityCount - 1) / GA::DIMENSION_NO);
      auto const maxNodeNumByEntities = calculateNodeNum(possiblyFullLevels) + (Base::GetMaxDepthID() - possiblyFullLevels) * entityCount;

      return static_cast<std::size_t>(std::min(maxNodeNumByEntities, maxNodeNumByMaxDepth));
    }

    constexpr void InitNodeGeometry(NodeID nodeID, SI::Location location, MGSI const& spaceIndexing) noexcept
    {
      if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::None)
      {
        return;
      }
      else if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MinPoint)
      {
        auto const mortonNodeID = spaceIndexing.GetNodeID(location);
        auto const nodeCenter = spaceIndexing.CalculateNodeCenter(mortonNodeID);
        auto const& nodeHalfSize = Base::GetNodeSize(location.GetDepthID() + 1);

        m_nodeGeometry[nodeID] = IGM::Sub(nodeCenter, nodeHalfSize);
      }
      else if constexpr (CONFIG::NODE_GEOMETRY_STORAGE == NodeGeometryStorage::MBR)
      {
        return;
      }
      else
      {
        static_assert(false, "Unsupported node geometry storage!");
      }
    }

    template<typename TNodeStorage>
    static constexpr bool IsFit(std::size_t entityCount, std::size_t maxNodeCount) noexcept
    {
      return maxNodeCount < std::numeric_limits<typename TNodeStorage::NodeSegmentIndex>::max() &&
             entityCount < std::numeric_limits<typename TNodeStorage::EntitySegment::Begin>::max() &&
             entityCount < std::numeric_limits<typename TNodeStorage::EntitySegment::Length>::max();
    }

#if defined(__GNUC__) && !defined(__clang__) && __GNUC__ >= 11
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstringop-overflow"
#pragma GCC diagnostic ignored "-Warray-bounds"
#endif
    constexpr NodeID CreateNode(SI::Location location, uint32_t nodeEntityBeginID, uint32_t nodeEntityCount, uint32_t childNodeCount, MGSI const& spaceIndexing)
    {
      auto const nodeID = static_cast<NodeID>(m_nodeDepthIDs.size());

      m_nodeDepthIDs.push_back(static_cast<uint8_t>(location.GetDepthID()));
      std::visit(
        [&](auto& nodes) {
          using NodeStorage = std::decay_t<decltype(nodes)>;
          using EntitySegment = typename NodeStorage::EntitySegment;
          using NodeSegmentIndex = typename NodeStorage::NodeSegmentIndex;

          nodes.nodeEntitySegment.push_back(EntitySegment{ static_cast<typename EntitySegment::Begin>(nodeEntityBeginID),
                                                           static_cast<typename EntitySegment::Length>(nodeEntityCount) });

          // Child node segment is filled if there are child nodes. Leaf nodes' segments are not created to save space, and GetNodeChildren() returns empty view for them.
          if (childNodeCount > 0)
          {
            nodes.nodeChildSegmentBegins.resize(nodeID + 2, nodes.nodeChildSegmentBegins.back());
            nodes.nodeChildSegmentBegins.back() += static_cast<NodeSegmentIndex>(childNodeCount);
          }
        },
        m_nodes);

      if constexpr (!std::is_same_v<decltype(m_nodeGeometry), std::monostate>)
      {
        m_nodeGeometry.push_back({});
      }
      InitNodeGeometry(nodeID, location, spaceIndexing);
      return nodeID;
    }
#if defined(__GNUC__) && !defined(__clang__) && __GNUC__ >= 11
#pragma GCC diagnostic pop
#endif

    // Build the tree in depth-first order
    template<bool ARE_LOCATIONS_SORTED>
    constexpr void Build(auto const& rootBeginLocationIt, auto const& rootEndLocationIt, MGSI const& spaceIndexing) noexcept
    {
      // Breadth-first build is required to ensure continuous addition of child nodes in nodeChildIDs

      struct NodeProcessingData
      {
        typename SI::Location location = {};
        uint32_t beginID = 0, length = 0;
      };

      using LCAC = typename SI::template LowestCommonAncestorCalculator<EA::GEOMETRY_TYPE != OrthoTree::GeometryType::Point>;

      auto nodeQueue = std::queue<NodeProcessingData>();
      nodeQueue.push(NodeProcessingData{ {}, 0, detail::size<uint32_t>(rootBeginLocationIt, rootEndLocationIt) });

      while (!nodeQueue.empty())
      {
        auto [location, nodeEntityBeginID, nodeEntityCount] = nodeQueue.front();
        nodeQueue.pop();

        auto depthID = location.GetDepthID();

        if (nodeEntityCount <= Base::GetMaxElementNum() || depthID >= Base::GetMaxDepthID())
        {
          CreateNode(location, nodeEntityBeginID, nodeEntityCount, 0, spaceIndexing);
          continue;
        }

        auto beginIt = rootBeginLocationIt + nodeEntityBeginID;
        auto endIt = beginIt + nodeEntityCount;
        auto const endID = nodeEntityBeginID + nodeEntityCount;

        uint32_t nonRefinableEntityCount = 0;
        if constexpr (EA::GEOMETRY_TYPE == OrthoTree::GeometryType::Box)
        {
          auto nonRefinableEndIt = beginIt;
          if constexpr (ARE_LOCATIONS_SORTED)
          {
            nonRefinableEndIt =
              std::partition_point(beginIt, endIt, [depthID](auto const& location) { return location.GetFirst().GetDepthID() == depthID; });
          }
          else
          {
            nonRefinableEndIt = std::partition(beginIt, endIt, [depthID](auto const& location) { return location.GetFirst().GetDepthID() == depthID; });
          }

          if (beginIt != nonRefinableEndIt)
          {
            nonRefinableEntityCount = detail::size<uint32_t>(beginIt, nonRefinableEndIt);
            if (nonRefinableEntityCount == nodeEntityCount)
            {
              CreateNode(location, nodeEntityBeginID, nodeEntityCount, 0, spaceIndexing);
              continue;
            }

            beginIt = nonRefinableEndIt;
          }
        }
        auto beginID = nodeEntityBeginID + nonRefinableEntityCount;

        ++depthID;
        auto const examinedLevelID = Base::GetExaminationLevelID(depthID);

        uint32_t childNodeCount = 0;
        while (beginID < endID)
        {
          auto const& pivot = (*beginIt).GetFirst();
          auto const childChecker = typename SI::ChildCheckerFixedDepth(examinedLevelID, pivot.GetLocationID());

          auto lcah = LCAC(pivot);
          auto childEndIt = [&] {
            if constexpr (ARE_LOCATIONS_SORTED)
            {
              auto childEndIt =
                std::partition_point(beginIt, endIt, [&](auto const& location) { return childChecker.Test(location.GetFirst().GetLocationID()); });
              for (auto it = beginIt; it != childEndIt; ++it)
                lcah.Add((*it).GetFirst());

              return childEndIt;
            }
            else
            {
              return Partitioning::partition(
                beginIt,
                endIt,
                [&](auto const& location) { return childChecker.Test(location.GetFirst().GetLocationID()); },
                [&](auto const& loc) { lcah.Add(loc.GetFirst()); });
            }
          }();

          auto childEntityCount = detail::size<uint32_t>(beginIt, childEndIt);
          nodeQueue.push(NodeProcessingData{ lcah.GetLocation(Base::GetMaxDepthID()), beginID, childEntityCount });

          ++childNodeCount;
          beginID += childEntityCount;
          beginIt = childEndIt;
        }

        CreateNode(location, nodeEntityBeginID, nonRefinableEntityCount, childNodeCount, spaceIndexing);
      }
    }

  public: // Create
    // Create
    template<typename TExecMode = SeqExec, bool ARE_ENTITIES_SURELY_IN_MODELSPACE = false>
    bool Create(
      EntityContainerView entities,
      std::optional<depth_t> maxDepthIn = std::nullopt,
      std::optional<TBox> boxSpaceOptional = std::nullopt,
      std::size_t maxElementNumInNode = CONFIG::DEFAULT_TARGET_ELEMENT_NUM_IN_NODES,
      TExecMode execMode = {}) noexcept
    {
      auto const boxSpace = boxSpaceOptional ? IGM::GetBoxAD(*boxSpaceOptional) : IGM::template GetBoundingBoxAD<EA>(entities);
      auto const entityCount = entities.size();
      auto const maxDepthID = (!maxDepthIn || maxDepthIn == depth_t{})
                              ? detail::EstimateMaxDepth<GA::DIMENSION_NO, SI::MAX_THEORETICAL_DEPTH_ID>(entityCount, maxElementNumInNode)
                              : *maxDepthIn;
      auto const spaceIndexing = MGSI(maxDepthID, boxSpace);
      InitBase(spaceIndexing.GetMinPoint(), spaceIndexing.GetSize(), maxDepthID, maxElementNumInNode);

      if (entityCount == 0)
        return true;


      auto mortonIDs = std::vector<typename SI::Location>(entityCount);
      m_entityStorage.resize(entityCount);

      auto locationsZip = detail::zip_view(mortonIDs, m_entityStorage);
      using Location = decltype(locationsZip)::iterator::value_type;

      EXEC_POL_DEF(epf); // GCC 11.3
      std::transform(EXEC_POL_ADD(epf) entities.begin(), entities.end(), locationsZip.begin(), [&](auto const& entity) -> Location {
        return { spaceIndexing.GetLocation(EA::GetGeometry(entity)), EA::GetEntityID(entities, entity) };
      });

      bool outOfSpace = false;
      auto endIt = locationsZip.end();
      if constexpr (!ARE_ENTITIES_SURELY_IN_MODELSPACE && !CONFIG::ALLOW_OUT_OF_SPACE_INSERTION)
      {
        endIt = std::partition(locationsZip.begin(), endIt, [](auto const& element) { return element.GetFirst().GetDepthID() != INVALID_DEPTH; });
        outOfSpace = endIt != locationsZip.end();
        if (outOfSpace)
          m_entityStorage.resize(detail::size(locationsZip.begin(), endIt));
      }

      constexpr bool ARE_LOCATIONS_SORTED = std::is_same_v<TExecMode, ExecutionTags::Parallel>;
      if constexpr (ARE_LOCATIONS_SORTED)
      {
        EXEC_POL_DEF(eps); // GCC 11.3
        std::sort(EXEC_POL_ADD(eps) locationsZip.begin(), endIt, [](auto const& l, auto const& r) {
          using std::get;
          return get<0>(l) < get<0>(r);
        });
      }

      auto const maxNodeCount = GetMaxPossibleNodeCount(entityCount);
      if (IsFit<NodeStorage256>(entityCount, maxNodeCount))
        m_nodes = NodeStorage256{ .nodeChildSegmentBegins = { 1 }, .nodeEntitySegment = {} };
      else if (IsFit<NodeStorage65536>(entityCount, maxNodeCount))
        m_nodes = NodeStorage65536{ .nodeChildSegmentBegins = { 1 }, .nodeEntitySegment = {} };
      else
        m_nodes = NodeStorageGeneral{ .nodeChildSegmentBegins = { 1 }, .nodeEntitySegment = {} };

      auto const estimatedNodeNum =
        detail::EstimateNodeNumber<GA::DIMENSION_NO, SI::MAX_THEORETICAL_DEPTH_ID>(entityCount, maxDepthID, maxElementNumInNode);

      std::visit(
        [&](auto& nodes) {
          nodes.nodeChildSegmentBegins.reserve(estimatedNodeNum);
          nodes.nodeEntitySegment.reserve(estimatedNodeNum);
        },
        m_nodes);

      m_nodeDepthIDs.reserve(estimatedNodeNum);
      if constexpr (!std::is_same_v<decltype(m_nodeGeometry), std::monostate>)
        m_nodeGeometry.reserve(estimatedNodeNum);

      Build<ARE_LOCATIONS_SORTED>(locationsZip.begin(), endIt, spaceIndexing);
      InitializeMinimalNodeGeometry(entities, execMode);
      return !outOfSpace;
    }

  public: // Create
    // Create
    template<typename TExecMode = SeqExec>
    static bool Create(
      StaticLinearOrthoTreeCore& tree,
      EntityContainerView entities,
      std::optional<depth_t> maxDepthIn = std::nullopt,
      std::optional<TBox> boxSpaceOptional = std::nullopt,
      std::size_t maxElementNoInNode = CONFIG::DEFAULT_TARGET_ELEMENT_NUM_IN_NODES,
      TExecMode execMode = {}) noexcept
    {
      return tree.Create<TExecMode>(entities, maxDepthIn, std::move(boxSpaceOptional), maxElementNoInNode, execMode);
    }

#ifdef ORTHOTREE__PUBLIC_LINEARDATA_INTERFACE
  public:
#else
  protected:
#endif
    auto const& GetNodes() const noexcept { return m_nodes; }
    auto const& GetNodeGeometries() const noexcept { return m_nodeGeometry; }
    auto const& GetNodeDepthIDs() const noexcept { return m_nodeDepthIDs; }
    auto const& GetEntityStorage() const noexcept { return m_entityStorage; }
  };
} // namespace OrthoTree


namespace OrthoTree
{

  // TraverseControl is the result type of node-visitor functions' procedure.
  enum class TraverseControl
  {
    Terminate,    // Terminates the traverse
    SkipChildren, // Skips children nodes
    Continue      // Continues the traverse
  };

  // OrthoTree: Non-owning Base container which spatially organize data ids in N dimension space into a hash-table by Morton Z order.
  template<typename T>
  concept OrthoTreeCoreView = requires(
    const T ct,
    typename T::NodeIDCR nodeID,
    typename T::NodeValue nodeValue,
    typename T::EA::Geometry const& geometry,
    typename T::EA::EntityContainerView entities,
    typename T::EntityID entityID) {
    typename T::NodeID;
    typename T::NodeIDCR;
    typename T::NodeValue;
    typename T::EA;
    typename T::GA;
    typename T::CONFIG;
    typename T::IGM;

    { T::GetRootNodeID() } -> std::convertible_to<typename T::NodeID>;
    { ct.GetRootNodeValue() } -> std::convertible_to<typename T::NodeValue>;
    { ct.GetNodeValue(nodeID) } -> std::convertible_to<typename T::NodeValue>;

    { ct.GetNodeChildren(nodeValue) };
    { ct.GetNodeEntities(nodeValue) };

    { ct.GetNodeBox(nodeValue) };
    { ct.GetNodeMinPoint(nodeValue) };
    { ct.GetNodeSize(nodeValue) };
    { ct.AreChildNodesOverlapping() } -> std::convertible_to<bool>;

    { ct.IsNodeEntitiesEmpty(nodeValue) } -> std::convertible_to<bool>;

    { ct.GetNodeCount() } -> std::convertible_to<std::size_t>;
  };

  template<typename TOrthoTreeCore>
  class OrthoTreeQueryBase : public TOrthoTreeCore
  {
  public:
    using Core = TOrthoTreeCore;
    ORTHOTREE_DEPENDENT_TYPES(Core);

  public:
#ifdef ORTHOTREE__PUBLIC_NODE_INTERFACE
    static_assert(OrthoTreeCoreView<TOrthoTreeCore>);
#endif
    static_assert(GeometryAdapterConcept<GA, GA::DIMENSION_NO, TVector, TBox, TRay, TPlane, TScalar, TFloatScalar>);
    static_assert(0 < GA::DIMENSION_NO && GA::DIMENSION_NO < 64);

    static_assert(std::is_trivially_copyable_v<EntityID>, "Only trivially copyable EntityID types are supported!");
    static_assert(CONFIG::IS_HOMOGENEOUS_GEOMETRY, "Mixed geometry types are not supported yet!");
    static_assert(EA::GEOMETRY_TYPE == GeometryType::Point || EA::GEOMETRY_TYPE == GeometryType::Box, "Entity geometry type is not supported!");

    using TOrthoTreeCore::TOrthoTreeCore;

    using NodeID = typename Core::NodeID;
    using NodeIDCR = typename Core::NodeIDCR;
    using NodeValue = Core::NodeValue;

  protected: // Aid struct to partitioning and distance ordering
    template<typename T>
    using DimArray = std::array<T, GA::DIMENSION_NO>;

  public:
    // Insert entity into the tree, if there is no entity within the same location by tolerance.
    bool InsertUnique(
      EA::Geometry const& entityGeometry,
      EntityContainerView entities,
      TFloatScalar tolerance = GA::BASE_TOLERANCE,
      InsertionMode insertionMode = InsertionMode::Balanced)
      requires(EA::ENTITY_ID_STRATEGY != EntityIdStrategy::EntityKeyed)
    {
      auto const nearestEntityList = GetNearestNeighbors(entityGeometry, 1, 0.0, entities, tolerance);
      if (!nearestEntityList.empty())
        return false;

      auto const entityID = EntityID(entities.size());
      return insertionMode == InsertionMode::Balanced ? Core::Insert(entityID, entityGeometry, entities)
                                                      : Core::InsertIntoLeaf(entityID, entityGeometry, insertionMode);
    }

    // Insert entity into the tree, if there is no entity within the same location by tolerance.
    bool InsertUnique(
      EntityID entityID,
      EA::Geometry const& entityGeometry,
      EntityContainerView entities,
      TFloatScalar tolerance = GA::BASE_TOLERANCE,
      bool doInsertToLeaf = false)
      requires(EA::ENTITY_ID_STRATEGY == EntityIdStrategy::EntityKeyed)
    {
      auto const nearestEntityList = GetNearestNeighbors(entityGeometry, 1, 0.0, entities, tolerance);
      if (!nearestEntityList.empty())
        return false;

      return doInsertToLeaf ? Core::InsertIntoLeaf(entityID, entityGeometry, true) : Core::Insert(entityID, entityGeometry, entities);
    }


  public:
    constexpr TBox GetNodeBox(NodeValue nodeValue) const noexcept
    {
      auto const box = Core::GetNodeBox(nodeValue);
      TBox outBox = {};
      detail::static_for<GA::DIMENSION_NO>([&, this](auto dimensionID) {
        GA::SetBoxMinC(outBox, dimensionID, box.Min[dimensionID]);
        GA::SetBoxMaxC(outBox, dimensionID, box.Max[dimensionID]);
      });

      return outBox;
    }


    // Node interface
#ifdef ORTHOTREE__PUBLIC_NODE_INTERFACE
  public:
#else
  protected:
#endif

    // Visit nodes in breadth first order
    constexpr void TraverseNodesBreadthFirst(auto&& procedure, NodeValue rootNodeValue) const noexcept
    {
      auto queue = std::queue<NodeValue>();
      queue.push(rootNodeValue);
      while (!queue.empty())
      {
        auto const nodeValue = queue.front();
        queue.pop();

        auto const traverseControl = procedure(nodeValue);
        switch (traverseControl)
        {
        case TraverseControl::Terminate: return;
        case TraverseControl::SkipChildren: continue;
        case TraverseControl::Continue:
          for (NodeIDCR childNodeID : Core::GetNodeChildren(nodeValue))
            queue.push(Core::GetNodeValue(childNodeID));

          break;
        }
      }
    }

    // Visit nodes in depth first order
    constexpr void TraverseNodesDepthFirst(auto&& procedure, NodeValue pRootValue) const noexcept
    {
      auto nodeStack = std::vector<NodeValue>{};
      nodeStack.reserve(64);
      nodeStack.push_back(pRootValue);

      while (!nodeStack.empty())
      {
        auto const currentNodeValue = nodeStack.back();
        nodeStack.pop_back();

        auto const control = procedure(currentNodeValue);
        switch (control)
        {
        case TraverseControl::Terminate: return;
        case TraverseControl::SkipChildren: continue;
        case TraverseControl::Continue: {
          for (auto const& childNodeID : Core::GetNodeChildren(currentNodeValue))
          {
            nodeStack.push_back(Core::GetNodeValue(childNodeID));
          }
          break;
        }
        }
      }
    }

    // Visit nodes in priority order
    constexpr void TraverseNodesByPriority(auto&& procedure, auto&& priorityCalculator, NodeValue rootNodeValue) const noexcept
    {
      using TPriorityResult = std::invoke_result_t<decltype(priorityCalculator), NodeValue>;
      using TPriority = typename detail::IsStdOptional<TPriorityResult>::BaseType;

      auto constexpr GetValue = [](auto const& pr) noexcept -> TPriority {
        if constexpr (detail::IsStdOptionalV<TPriorityResult>)
          return *pr;
        else
          return pr;
      };

      struct PrioritizedNode
      {
        NodeValue nodeValue;
        TPriority priority;

        constexpr auto operator<=>(PrioritizedNode const& other) const noexcept { return priority <=> other.priority; }
      };

      auto nodePriority = priorityCalculator(rootNodeValue);
      if constexpr (detail::IsStdOptionalV<TPriorityResult>)
      {
        if (!nodePriority)
          return;
      }

      auto nodesToProceed = std::priority_queue<PrioritizedNode, std::vector<PrioritizedNode>, std::greater<PrioritizedNode>>();
      nodesToProceed.push({ rootNodeValue, GetValue(nodePriority) });
      while (!nodesToProceed.empty())
      {
        auto const [nodeValue, priority] = nodesToProceed.top();
        nodesToProceed.pop();

        auto const control = procedure(nodeValue, priority);
        switch (control)
        {
        case TraverseControl::Terminate: return;
        case TraverseControl::SkipChildren: continue;
        case TraverseControl::Continue: break;
        }

        for (NodeIDCR childNodeID : Core::GetNodeChildren(nodeValue))
        {
          auto const& childNodeValue = Core::GetNodeValue(childNodeID);
          auto childNodePriority = priorityCalculator(childNodeValue);

          if constexpr (detail::IsStdOptionalV<TPriorityResult>)
          {
            if (!childNodePriority)
              continue;
          }

          nodesToProceed.push({ childNodeValue, GetValue(childNodePriority) });
        }
      }
    }

    // Visit nodes in breadth first order (procedure requires node interface)
    constexpr void TraverseNodesBreadthFirst(auto&& procedure) const noexcept
    {
      TraverseNodesBreadthFirst(std::forward<decltype(procedure)>(procedure), Core::GetRootNodeValue());
    }

    // Visit nodes in depth first order (procedure requires node interface)
    constexpr void TraverseNodesDepthFirst(auto&& procedure) const noexcept
    {
      TraverseNodesDepthFirst(std::forward<decltype(procedure)>(procedure), Core::GetRootNodeValue());
    }

    // Visit nodes in priority order (procedure and priorityCalculator require node interface)
    constexpr void TraverseNodesByPriority(auto&& procedure, auto&& priorityCalculator) const noexcept
    {
      TraverseNodesByPriority(
        std::forward<decltype(procedure)>(procedure), std::forward<decltype(priorityCalculator)>(priorityCalculator), Core::GetRootNodeValue());
    }

  public:
    // Visit entities in breadth first order
    // Accepted procedure signature: TraverseControl(auto const& entitiesInNode, TBox const& nodeBox). Where entitiesInNode is iterable of EntityID.
    constexpr void TraverseEntitiesBreadthFirst(auto&& procedure) const noexcept
    {
      TraverseNodesBreadthFirst([&, this](auto const nodeValue) { return procedure(Core::GetNodeEntities(nodeValue), Core::GetNodeBox(nodeValue)); });
    }

    // Visit entities in depth first order
    // Accepted procedure signature: TraverseControl(auto const& entitiesInNode, TBox const& nodeBox). Where entitiesInNode is iterable of EntityID.
    constexpr void TraverseEntitiesDepthFirst(auto&& procedure) const noexcept
    {
      TraverseNodesDepthFirst([&, this](auto const nodeValue) { return procedure(Core::GetNodeEntities(nodeValue), Core::GetNodeBox(nodeValue)); });
    }

    // Visit entities in priority order
    // Accepted procedure signature: TraverseControl(auto const& entitiesInNode, TBox const& nodeBox, TPriorityValue priority). Where entitiesInNode
    // is iterable of EntityID. Accepted priorityCalculator signature: TPriority(TBox const& nodeBox).
    //   Where TPriority is TPriorityValue or std::optional<TPriorityValue>. TPriorityValue is any type that is comparable by operator<. If it is
    //   wrapped in std::optional, nodes with std::nullopt priority will be skipped.
    constexpr void TraverseEntitiesByPriority(auto&& procedure, auto&& priorityCalculator) const noexcept
    {
      TraverseNodesByPriority(
        [&, this](auto const nodeValue, auto const& priority) {
          return procedure(Core::GetNodeEntities(nodeValue), Core::GetNodeBox(nodeValue), priority);
        },
        [&, this](auto const nodeValue) { return priorityCalculator(Core::GetNodeBox(nodeValue)); });
    }

    // Collect all item id, traversing the tree in breadth-first search order
    std::vector<EntityID> GetEntitiesBreadthFirst(NodeIDCR rootNodeID = Core::GetRootNodeID(), bool shouldSortInsideNodes = false) const noexcept
    {
      auto entityIDs = std::vector<EntityID>();
      TraverseNodesBreadthFirst(
        [&, this](auto const nodeValue) {
          auto const& entities = Core::GetNodeEntities(nodeValue);
          auto const entityIDsSize = entityIDs.size();
          entityIDs.insert(entityIDs.end(), entities.begin(), entities.end());
          if (shouldSortInsideNodes)
            std::sort(entityIDs.begin() + entityIDsSize, entityIDs.end());

          return TraverseControl::Continue;
        },
        Core::GetNodeValue(rootNodeID));
      return entityIDs;
    }

  private:
    void CollectAllEntitiesInDFSRecursive(NodeValue parentNode, std::vector<EntityID>& foundEntities, bool shouldSortInsideNodes) const noexcept
    {
      auto const& entities = Core::GetNodeEntities(parentNode);
      auto const entityIDsSize = foundEntities.size();
      foundEntities.insert(foundEntities.end(), entities.begin(), entities.end());
      if (shouldSortInsideNodes)
        std::sort(foundEntities.begin() + entityIDsSize, foundEntities.end());

      for (NodeIDCR childKey : Core::GetNodeChildren(parentNode))
        CollectAllEntitiesInDFSRecursive(Core::GetNodeValue(childKey), foundEntities, shouldSortInsideNodes);
    }

  public:
    // Collect all entity id, traversing the tree in depth-first search pre-order
    std::vector<EntityID> GetEntitiesDepthFirst(NodeID parentKey = Core::GetRootNodeID(), bool shouldSortInsideNodes = false) const noexcept
    {
      auto entityIDs = std::vector<EntityID>{};
      CollectAllEntitiesInDFSRecursive(Core::GetNodeValue(parentKey), entityIDs, shouldSortInsideNodes);
      return entityIDs;
    }


    template<typename TTester, typename... TAgainst>
    static constexpr auto TestEntity(TTester const& tester, EntityID entityID, EntityContainerView entities, [[maybe_unused]] TAgainst&&... against) noexcept
    {
      if constexpr (std::is_same_v<TTester, std::monostate>)
      {
        return true;
      }
      else if constexpr (sizeof...(against) > 0 && std::is_invocable_v<TTester, EntityID, TAgainst...>)
      {
        return tester(entityID, std::forward<TAgainst>(against)...);
      }
      else if constexpr (sizeof...(against) > 0 && std::is_invocable_v<TTester, typename EA::Entity, TAgainst...>)
      {
        return tester(EA::GetEntity(entities, entityID), std::forward<TAgainst>(against)...);
      }
      else if constexpr (sizeof...(against) == 1 && std::is_invocable_v<TTester, TAgainst..., EntityID>)
      {
        return tester(std::forward<TAgainst>(against)..., entityID);
      }
      else if constexpr (sizeof...(against) == 1 && std::is_invocable_v<TTester, TAgainst..., typename EA::Entity>)
      {
        return tester(std::forward<TAgainst>(against)..., EA::GetEntity(entities, entityID));
      }
      else if constexpr (std::is_invocable_v<TTester, EntityID>)
      {
        return tester(entityID);
      }
      else if constexpr (std::is_invocable_v<TTester, typename EA::Entity>)
      {
        return tester(EA::GetEntity(entities, entityID));
      }
      else
      {
        static_assert(sizeof(TTester) == 0, "Unsupported tester signature");
      }
    }

  public: // Search functions
    // Pick search
    //
    // Accepted tester signatures:
    // * bool(EntityID) / bool(EntityID, TVector&)
    // * bool(Entity) / bool(Entity, TVector&)
    template<typename TTester = std::monostate>
    std::vector<EntityID> PickSearch(
      TVector const& pickPoint, EntityContainerView entities, TFloatScalar tolerance = GA::BASE_TOLERANCE, TTester&& tester = {}) const noexcept
    {
      auto foundEntitiyIDs = std::vector<EntityID>();
      if (!IGM::DoesBoxContainPointAD(Core::GetNodeBox(Core::GetRootNodeValue()), pickPoint, tolerance))
        return foundEntitiyIDs;

      foundEntitiyIDs.reserve(100);

      TraverseNodesDepthFirst([&, this](auto const nodeValue) {
        if (!IGM::DoesBoxContainPointAD(Core::GetNodeMinPoint(nodeValue), Core::GetNodeSize(nodeValue), pickPoint, tolerance))
          return TraverseControl::SkipChildren;

        auto const pointTest = [&, this](auto const entityID) {
          if constexpr (EA::GEOMETRY_TYPE == GeometryType::Point)
          {
            return GA::ArePointsEqual(EA::GetGeometry(entities, entityID), pickPoint, tolerance);
          }
          else if constexpr (EA::GEOMETRY_TYPE == GeometryType::Box)
          {
            return GA::DoesBoxContainPoint(EA::GetGeometry(entities, entityID), pickPoint, tolerance);
          }
          else
          {
            static_assert(false, "Unsupported geometry type!");
          }
          ORTHOTREE_UNREACHABLE();
        };

        std::ranges::copy_if(Core::GetNodeEntities(nodeValue), std::back_inserter(foundEntitiyIDs), [&, this](auto const entityID) {
          if (!pointTest(entityID))
            return false;

          return TestEntity(tester, entityID, entities, pickPoint);
        });

        return TraverseControl::Continue;
      });

      return foundEntitiyIDs;
    }

  public:
    // Range search
    //
    // Accepted tester signatures:
    // * bool(EntityID)
    // * bool(EntityID, TVector&)
    // * bool(Entity)
    // * bool(Entity, TVector&)
    template<typename TTester = std::monostate>
    std::vector<EntityID> RangeSearch(
      TBox const& range,
      EntityContainerView entities,
      RangeSearchMode rangeSearchMode = RangeSearchMode::Inside,
      TFloatScalar tolerance = GA::BASE_TOLERANCE,
      TTester&& tester = {}) const noexcept
    {
      auto foundEntities = std::vector<EntityID>{};

      auto const entityNo = entities.size();

      if constexpr (EA::ENTITY_ID_STRATEGY != EntityIdStrategy::StableIndex)
      {
        if (IGM::DoesRangeContainBoxAD(range, Core::GetNodeBox(Core::GetRootNodeValue()), tolerance))
        {
          foundEntities.reserve(entityNo);

          if constexpr (EA::ENTITY_ID_STRATEGY == EntityIdStrategy::ContiguousIndex)
          {
            foundEntities.resize(entityNo);
            std::iota(foundEntities.begin(), foundEntities.end(), 0);
          }
          else if constexpr (EA::ENTITY_ID_STRATEGY == EntityIdStrategy::EntityKeyed)
          {
            std::transform(entities.begin(), entities.end(), std::back_inserter(foundEntities), [&entities](auto const& item) {
              return EA::GetEntityID(entities, item);
            });
          }
          else
          {
            static_assert(false, "Unsupported entity ID strategy!");
          }

          return foundEntities;
        }
      }

      // If the range has zero volume, it could stuck at any node comparison with point/side touch. It is eliminated to work node bounding box independently.
      const auto rangeVolume = IGM::GetVolumeAD(range);
      if (rangeVolume <= 0.0)
      {
        return foundEntities;
      }

      auto const boxTest = [&, this](auto const entityID) {
        if constexpr (EA::GEOMETRY_TYPE == GeometryType::Point)
        {
          return GA::DoesBoxContainPoint(range, EA::GetGeometry(entities, entityID), tolerance);
        }
        else if constexpr (EA::GEOMETRY_TYPE == GeometryType::Box)
        {
          switch (rangeSearchMode)
          {
          case RangeSearchMode::Overlap: return GA::AreBoxesOverlappedStrict(range, EA::GetGeometry(entities, entityID), tolerance);
          case RangeSearchMode::Inside: return GA::AreBoxesOverlapped(range, EA::GetGeometry(entities, entityID), true, false, tolerance);
          }
        }
        else
        {
          static_assert(false, "Unsupported geometry type!");
        }
        ORTHOTREE_UNREACHABLE();
      };

      auto const searchRangeMinPoint = IGM::GetBoxMinPointAD(range);
      auto const searchRangeSize = IGM::GetBoxSizeAD(range);
      TraverseNodesDepthFirst([&, this](auto const nodeValue) {
        auto const& nodeMinPoint = Core::GetNodeMinPoint(nodeValue);
        auto const& nodeSize = Core::GetNodeSize(nodeValue);
        if (!IGM::AreBoxesOverlappingByMinPoint(searchRangeMinPoint, searchRangeSize, nodeMinPoint, nodeSize, tolerance))
          return TraverseControl::SkipChildren;

        if (IGM::DoesRangeContainBoxAD(range, nodeMinPoint, nodeSize, tolerance))
        {
          TraverseNodesDepthFirst(
            [&, this](auto const childNodeValue) {
              if constexpr (std::is_same_v<TTester, std::monostate>)
              {
                std::ranges::copy(Core::GetNodeEntities(childNodeValue), std::back_inserter(foundEntities));
              }
              else
              {
                std::ranges::copy_if(Core::GetNodeEntities(childNodeValue), std::back_inserter(foundEntities), [&, this](auto const entityID) {
                  return TestEntity(tester, entityID, entities, range);
                });
              }
              return TraverseControl::Continue;
            },
            nodeValue);

          return TraverseControl::SkipChildren;
        }
        else
        {
          std::ranges::copy_if(Core::GetNodeEntities(nodeValue), std::back_inserter(foundEntities), [&, this](auto const entityID) {
            if (!boxTest(entityID))
              return false;

            return TestEntity(tester, entityID, entities, range);
          });
        }

        return TraverseControl::Continue;
      });

      return foundEntities;
    }

  private:
    static constexpr PlaneRelation GetEntityPlaneRelation(
      EA::Geometry const& entityGeometry, TScalar distanceOfOrigo, TVector const& planeNormal, TFloatScalar tolerance = GA::BASE_TOLERANCE) noexcept
    {
      if constexpr (EA::GEOMETRY_TYPE == GeometryType::Point)
      {
        return GA::GetPointPlaneRelation(entityGeometry, distanceOfOrigo, planeNormal, tolerance);
      }
      else if constexpr (EA::GEOMETRY_TYPE == GeometryType::Box)
      {
        return IGM::GetBoxPlaneRelationAD(IGM::GetBoxMinPointAD(entityGeometry), IGM::GetBoxSizeAD(entityGeometry), distanceOfOrigo, planeNormal, tolerance);
      }
      else
      {
        static_assert(false, "Unsupported geometry type for kNN!");
      }
      ORTHOTREE_UNREACHABLE();
    }

  public:
    // Hyperplane intersection
    //
    // Accepted tester signatures:
    // * bool(EntityID)
    // * bool(EntityID, TScalar, TVector)
    // * bool(Entity)
    // * bool(Entity, TScalar, TVector)
    template<typename TTester = std::monostate>
    std::vector<EntityID> PlaneSearch(
      TScalar distanceOfOrigo,
      TVector const& planeNormal,
      EntityContainerView entities,
      TFloatScalar tolerance = GA::BASE_TOLERANCE,
      TTester&& tester = {}) const noexcept
    {
      assert(GA::IsNormalizedVector(planeNormal));

      auto results = std::vector<EntityID>{};
      TraverseNodesDepthFirst([&, this](auto const nodeValue) {
        if (IGM::GetBoxPlaneRelationAD(Core::GetNodeMinPoint(nodeValue), Core::GetNodeSize(nodeValue), distanceOfOrigo, planeNormal, tolerance) != PlaneRelation::Hit)
          return TraverseControl::SkipChildren;

        for (auto const entityID : Core::GetNodeEntities(nodeValue))
          if (GetEntityPlaneRelation(EA::GetGeometry(entities, entityID), distanceOfOrigo, planeNormal, tolerance) == PlaneRelation::Hit)
            if (TestEntity(tester, entityID, entities, distanceOfOrigo, planeNormal))
              results.emplace_back(entityID);

        return TraverseControl::Continue;
      });

      return results;
    }

    // Hyperplane intersection using built-in plane
    //
    // Accepted tester signatures:
    // * bool(EntityID)
    // * bool(EntityID, TScalar, TVector)
    // * bool(Entity)
    // * bool(Entity, TScalar, TVector)
    template<typename TTester = std::monostate>
    std::vector<EntityID> PlaneSearch(
      TPlane const& plane, EntityContainerView entities, TFloatScalar tolerance = GA::BASE_TOLERANCE, TTester&& tester = {}) const noexcept
    {
      return PlaneSearch(GA::GetPlaneOrigoDistance(plane), GA::GetPlaneNormal(plane), entities, tolerance, std::forward<TTester>(tester));
    }

    // Hyperplane intersection using built-in plane (same as PlaneSearch())
    //
    // Accepted tester signatures:
    // * bool(EntityID)
    // * bool(EntityID, TScalar, TVector)
    // * bool(Entity)
    // * bool(Entity, TScalar, TVector)
    template<typename TTester = std::monostate>
    std::vector<EntityID> PlaneIntersection(
      TScalar distanceOfOrigo,
      TVector const& planeNormal,
      EntityContainerView entities,
      TFloatScalar tolerance = GA::BASE_TOLERANCE,
      TTester&& tester = {}) const noexcept
    {
      return PlaneSearch(distanceOfOrigo, planeNormal, entities, tolerance, std::forward<TTester>(tester));
    }

    // Hyperplane intersection using built-in plane (same as PlaneSearch())
    //
    // Accepted tester signatures:
    // * bool(EntityID)
    // * bool(EntityID, TScalar, TVector)
    // * bool(Entity)
    // * bool(Entity, TScalar, TVector)
    template<typename TTester = std::monostate>
    std::vector<EntityID> PlaneIntersection(
      TPlane const& plane, EntityContainerView entities, TFloatScalar tolerance = GA::BASE_TOLERANCE, TTester&& tester = {}) const noexcept
    {
      return PlaneSearch(plane, entities, tolerance, std::forward<TTester>(tester));
    }

    // Hyperplane segmentation, get all elements in positive side (Plane equation: dotProduct(planeNormal, point) = distanceOfOrigo)
    //
    // Accepted tester signatures:
    // * bool(EntityID)
    // * bool(EntityID, TScalar, TVector)
    // * bool(Entity)
    // * bool(Entity, TScalar, TVector)
    template<typename TTester = std::monostate>
    std::vector<EntityID> PlanePositiveSegmentation(
      TScalar distanceOfOrigo,
      TVector const& planeNormal,
      EntityContainerView entities,
      TFloatScalar tolerance = GA::BASE_TOLERANCE,
      TTester&& tester = {}) const noexcept
    {
      assert(GA::IsNormalizedVector(planeNormal));

      auto results = std::vector<EntityID>{};
      TraverseNodesDepthFirst([&, this](auto const nodeValue) {
        auto const nodeRelation =
          IGM::GetBoxPlaneRelationAD(Core::GetNodeMinPoint(nodeValue), Core::GetNodeSize(nodeValue), distanceOfOrigo, planeNormal, tolerance);

        switch (nodeRelation)
        {
        case PlaneRelation::Negative: return TraverseControl::SkipChildren;
        case PlaneRelation::Positive:
          TraverseNodesDepthFirst(
            [&, this](auto const childNodeValue) {
              if constexpr (std::is_same_v<std::monostate, TTester>)
              {
                std::ranges::copy(Core::GetNodeEntities(childNodeValue), std::back_inserter(results));
              }
              else
              {
                std::ranges::copy_if(Core::GetNodeEntities(childNodeValue), std::back_inserter(results), [&, this](auto const entityID) {
                  return TestEntity(tester, entityID, entities, distanceOfOrigo, planeNormal);
                });
              }
              return TraverseControl::Continue;
            },
            nodeValue);
          return TraverseControl::SkipChildren;

        case PlaneRelation::Hit:
          for (auto const entityID : Core::GetNodeEntities(nodeValue))
          {
            auto const entityRelation = GetEntityPlaneRelation(EA::GetGeometry(entities, entityID), distanceOfOrigo, planeNormal, tolerance);
            if (entityRelation == PlaneRelation::Negative)
              continue;

            if (TestEntity(tester, entityID, entities, distanceOfOrigo, planeNormal))
              results.emplace_back(entityID);
          }
          return TraverseControl::Continue;
        }

        ORTHOTREE_UNREACHABLE();
      });

      return results;
    }

    // Hyperplane segmentation, get all elements in positive side (Plane equation: dotProduct(planeNormal, point) = distanceOfOrigo)
    //
    // Accepted tester signatures:
    // * bool(EntityID)
    // * bool(EntityID, TScalar, TVector)
    // * bool(Entity)
    // * bool(Entity, TScalar, TVector)
    template<typename TTester = std::monostate>
    std::vector<EntityID> PlanePositiveSegmentation(
      TPlane const& plane, EntityContainerView entities, TFloatScalar tolerance = GA::BASE_TOLERANCE, TTester&& tester = {}) const noexcept
    {
      return PlanePositiveSegmentation(GA::GetPlaneOrigoDistance(plane), GA::GetPlaneNormal(plane), entities, tolerance, std::forward<TTester>(tester));
    }

    // Get all entities which relation is positive or intersected by the given space boundary planes
    //
    // Accepted tester signatures:
    // * bool(EntityID)
    // * bool(EntityID, TPlane)
    // * bool(Entity)
    // * bool(Entity, TPlane)
    template<typename TTester = std::monostate>
    std::vector<EntityID> FrustumCulling(
      std::span<TPlane const> const& boundaryPlanes, EntityContainerView entities, TFloatScalar tolerance = GA::BASE_TOLERANCE, TTester&& tester = {}) const noexcept
    {
      auto results = std::vector<EntityID>{};
      if (boundaryPlanes.empty())
        return results;

      assert(std::all_of(boundaryPlanes.begin(), boundaryPlanes.end(), [](auto const& plane) -> bool {
        return GA::IsNormalizedVector(GA::GetPlaneNormal(plane));
      }));

      auto const selector = [&, this](auto const nodeValue) -> bool {
        auto const& nodeMinPoint = Core::GetNodeMinPoint(nodeValue);
        auto const& nodeSize = Core::GetNodeSize(nodeValue);

        for (auto const& plane : boundaryPlanes)
        {
          auto const relation =
            IGM::GetBoxPlaneRelationAD(nodeMinPoint, nodeSize, GA::GetPlaneOrigoDistance(plane), GA::GetPlaneNormal(plane), tolerance);
          switch (relation)
          {
          case PlaneRelation::Hit: return true;
          case PlaneRelation::Negative: return false;
          case PlaneRelation::Positive: break;
          }
        }
        return true;
      };

      auto const procedure = [&, this](auto const nodeValue) {
        if (!selector(nodeValue))
          return TraverseControl::SkipChildren;

        for (auto const entityID : Core::GetNodeEntities(nodeValue))
        {
          bool isAnyOnNegativeSide = false;
          for (auto const& plane : boundaryPlanes)
          {
            auto const relation =
              GetEntityPlaneRelation(EA::GetGeometry(entities, entityID), GA::GetPlaneOrigoDistance(plane), GA::GetPlaneNormal(plane), tolerance);
            auto const isOnPositiveSideOrHit = [&, this](auto const planeRelation) {
              switch (planeRelation)
              {
              case PlaneRelation::Hit: return TestEntity(tester, entityID, entities, plane);
              case PlaneRelation::Negative: return false;
              case PlaneRelation::Positive: return true;
              }
              return false;
            }(relation);

            isAnyOnNegativeSide |= !isOnPositiveSideOrHit;
            if (isAnyOnNegativeSide)
              break;
          }

          if (!isAnyOnNegativeSide)
            results.emplace_back(entityID);
        }

        return TraverseControl::Continue;
      };

      TraverseNodesDepthFirst(procedure);

      return results;
    }

    struct FrustumCondition
    {
      std::vector<std::pair<TPlane, bool>> boundaryPlanes; // pair<plane, isCrossingAllowed>
    };

    struct RangeCondition
    {
      TBox range;
      RangeSearchMode rangeSearchMode = RangeSearchMode::Inside;
    };

    struct PlaneIntersectionCondition
    {
      TPlane plane;
      TFloatScalar tolerance = GA::BASE_TOLERANCE;
    };
    using EntityIDCondition = std::function<bool(EntityID)>;
    using EntityCondition = std::function<bool(typename EA::Entity)>;
    using QueryCondition = std::variant<FrustumCondition, RangeCondition, PlaneIntersectionCondition, EntityIDCondition, EntityCondition>;

    static constexpr QueryCondition ByWithin(TBox const& range) noexcept { return RangeCondition{ range, RangeSearchMode::Inside }; }
    static constexpr QueryCondition ByOverlaps(TBox const& range) noexcept { return RangeCondition{ range, RangeSearchMode::Overlap }; }
    static constexpr QueryCondition ByInFrustum(std::vector<std::pair<TPlane, bool>> boundaryPlanes) noexcept
    {
      return FrustumCondition{ std::move(boundaryPlanes) };
    }
    static constexpr QueryCondition ByIntersecting(TPlane const& plane, TFloatScalar tolerance = GA::BASE_TOLERANCE) noexcept
    {
      return PlaneIntersectionCondition{ plane, tolerance };
    }
    static constexpr QueryCondition BySatisfies(EntityIDCondition condition) noexcept { return condition; }
    static constexpr QueryCondition BySatisfies(EntityCondition condition) noexcept { return condition; }

    // Complex query with multiple conditions. The conditions are combined with logical AND by default, but can be switched to OR by template parameter.
    template<LogicalOperator OP = LogicalOperator::And>
    constexpr std::vector<EntityID> Query(auto const& conditions, EntityContainerView entities, TFloatScalar tolerance = GA::BASE_TOLERANCE) const noexcept
    {
      auto results = std::vector<EntityID>{};

      TraverseNodesDepthFirst([&, this](auto const nodeValue) {
        // Node selection

        auto const& nodeMinPoint = Core::GetNodeMinPoint(nodeValue);
        auto const& nodeSize = Core::GetNodeSize(nodeValue);

        bool isNodePassed = false;
        for (auto const& condition : conditions)
        {
          auto const isNodePassedCondition = detail::VisitVariant(
            condition,
            [&](FrustumCondition const& frustumCondition) {
              for (auto const& [plane, isCrossingAllowed] : frustumCondition.boundaryPlanes)
              {
                auto const relation =
                  IGM::GetBoxPlaneRelationAD(nodeMinPoint, nodeSize, GA::GetPlaneOrigoDistance(plane), GA::GetPlaneNormal(plane), tolerance);
                switch (relation)
                {
                case PlaneRelation::Hit: return true;
                case PlaneRelation::Negative: return false;
                case PlaneRelation::Positive: break;
                }
              }
              return true;
            },
            [&](RangeCondition const& rangeCondition) {
              auto const& [range, rangeSearchMode] = rangeCondition;

              auto const searchRangeMinPoint = IGM::GetBoxMinPointAD(range);
              auto const searchRangeSize = IGM::GetBoxSizeAD(range);

              return IGM::AreBoxesOverlappingByMinPoint(searchRangeMinPoint, searchRangeSize, nodeMinPoint, nodeSize, tolerance);
            },
            [&](PlaneIntersectionCondition const& planeCondition) {
              auto const& [plane, planeTolerance] = planeCondition;

              return IGM::GetBoxPlaneRelationAD(nodeMinPoint, nodeSize, GA::GetPlaneOrigoDistance(plane), GA::GetPlaneNormal(plane), planeTolerance) ==
                     PlaneRelation::Hit;
            },
            [&](EntityIDCondition const&) { return true; },
            [&](EntityCondition const&) { return true; });

          if constexpr (OP == LogicalOperator::Or)
          {
            isNodePassed |= isNodePassedCondition;
            if (isNodePassed)
              break;
          }
          else if constexpr (OP == LogicalOperator::And)
          {
            if (!isNodePassedCondition)
              return TraverseControl::SkipChildren;
          }
          else
          {
            static_assert(false, "Invalid logical operator");
          }
        }

        if constexpr (OP == LogicalOperator::Or)
        {
          if (!isNodePassed)
            return TraverseControl::SkipChildren;
        }


        // Entity selection

        for (auto const entityID : Core::GetNodeEntities(nodeValue))
        {
          bool isPassed = OP == LogicalOperator::And;
          for (auto const& condition : conditions)
          {
            auto const isConditionPassed = detail::VisitVariant(
              condition,
              [&](FrustumCondition const& frustumCondition) {
                for (auto const& [plane, isCrossingAllowed] : frustumCondition.boundaryPlanes)
                {
                  auto relation =
                    GetEntityPlaneRelation(EA::GetGeometry(entities, entityID), GA::GetPlaneOrigoDistance(plane), GA::GetPlaneNormal(plane), tolerance);
                  switch (relation)
                  {
                  case PlaneRelation::Hit:
                    if (!isCrossingAllowed)
                      return false;
                    break;
                  case PlaneRelation::Negative: return false;
                  case PlaneRelation::Positive: break;
                  }
                }
                return true;
              },
              [&](RangeCondition const& rangeCondition) {
                auto const& [range, rangeSearchMode] = rangeCondition;

                if constexpr (EA::GEOMETRY_TYPE == GeometryType::Point)
                {
                  return GA::DoesBoxContainPoint(range, EA::GetGeometry(entities, entityID), tolerance);
                }
                else if constexpr (EA::GEOMETRY_TYPE == GeometryType::Box)
                {
                  switch (rangeSearchMode)
                  {
                  case RangeSearchMode::Inside: return GA::AreBoxesOverlapped(range, EA::GetGeometry(entities, entityID), true, false, tolerance);
                  case RangeSearchMode::Overlap: return GA::AreBoxesOverlappedStrict(range, EA::GetGeometry(entities, entityID), tolerance);
                  }
                  assert(false && "Unsupported range search mode!");
                  return false;
                }
                else
                {
                  static_assert(false, "Unsupported geometry type!");
                  return false;
                }
              },
              [&](PlaneIntersectionCondition const& planeCondition) {
                auto const& [plane, planeTolerance] = planeCondition;

                return GetEntityPlaneRelation(EA::GetGeometry(entities, entityID), GA::GetPlaneOrigoDistance(plane), GA::GetPlaneNormal(plane), planeTolerance) ==
                       PlaneRelation::Hit;
              },
              [&](EntityIDCondition const& entityIDTester) { return entityIDTester(entityID); },
              [&](EntityCondition const& entityTester) { return entityTester(EA::GetEntity(entities, entityID)); });

            if constexpr (OP == LogicalOperator::Or)
            {
              isPassed |= isConditionPassed;
              if (isPassed)
                break;
            }
            else if constexpr (OP == LogicalOperator::And)
            {
              if (!isConditionPassed)
              {
                isPassed = false;
                break;
              }
            }
            else
            {
              static_assert(false, "Invalid logical operator");
            }
          }

          if (isPassed)
            results.emplace_back(entityID);
        }

        return TraverseControl::Continue;
      });

      return results;
    }

  public: // K Nearest Neighbor
    // Get the precise distance between the entity and kNN's search point. Floating-point return value is required.
    using EntityDistanceFn = std::function<TFloatScalar(TVector const&, EntityID)>;

  private: // K Nearest Neighbor helpers
    static constexpr TFloatScalar GetValueWithToleranceUpper(TFloatScalar value, TFloatScalar tolerance = GA::BASE_TOLERANCE) noexcept
    {
      return std::fmax(tolerance, value * (TFloatScalar(1.0) + tolerance));
    }

    static constexpr TFloatScalar GetValueWithToleranceLower(TFloatScalar value, TFloatScalar tolerance = GA::BASE_TOLERANCE) noexcept
    {
      return value == 0 ? -tolerance : (value * (TFloatScalar(1.0) - tolerance));
    }

    struct EntityDistance
    {
      EntityID entityID;
      TFloatScalar distance;

      constexpr auto operator<=>(EntityDistance const& other) const noexcept { return distance <=> other.distance; }
      constexpr bool operator==(EntityDistance const& other) const noexcept { return distance == other.distance; }
    };

    struct EntityDistances
    {
      std::vector<EntityDistance> optimistic;
      std::vector<EntityDistance> pessimistic;
    };

    template<typename TEntityDistanceFn = std::monostate>
    static void AddEntityDistance(
      std::size_t neighborCount,
      TVector const& searchPoint,
      TEntityDistanceFn const& entityDistanceFn,
      auto const& nodeEntityIDs,
      EntityContainerView entities,
      TFloatScalar tolerance,
      EntityDistances& neighborEntities,
      TFloatScalar& farthestEntityDistance) noexcept
    {
      auto neighborEntitiesPessimisticMaxHeap = std::vector<TFloatScalar>{};

      for (auto const entityID : nodeEntityIDs)
      {
        typename GA::PointBoxMinMaxDistance pbd;
        if constexpr (EA::GEOMETRY_TYPE == GeometryType::Point)
        {
          const auto distance = GA::Distance(searchPoint, EA::GetGeometry(entities, entityID));
          pbd = { distance, distance };
        }
        else if constexpr (EA::GEOMETRY_TYPE == GeometryType::Box)
        {
          pbd = GA::MinMaxDistance(searchPoint, EA::GetGeometry(entities, entityID), tolerance);
        }
        else
        {
          static_assert(false, "Unsupported geometry type for kNN!");
        }

        if (pbd.min >= farthestEntityDistance)
          continue;

        if constexpr (!std::is_same_v<TEntityDistanceFn, std::monostate>)
        {
          auto const entityDistanceResult = TestEntity(entityDistanceFn, entityID, entities, searchPoint);

          using ResultType = std::decay_t<decltype(entityDistanceResult)>;
          using OptionalTrait = typename detail::IsStdOptional<ResultType>;
          if constexpr (std::is_same_v<typename OptionalTrait::BaseType, bool> || OptionalTrait::value)
          {
            if (!entityDistanceResult)
              continue;
          }

          if constexpr (std::is_same_v<typename OptionalTrait::BaseType, TScalar> || std::is_same_v<typename OptionalTrait::BaseType, TFloatScalar>)
          {
            if constexpr (OptionalTrait::value)
              pbd.min = *entityDistanceResult;
            else
              pbd.min = entityDistanceResult;

            if (pbd.min >= farthestEntityDistance)
              continue;

            pbd.minMax = pbd.min;
          }
        }

        auto const shouldHeapify = neighborEntities.optimistic.size() == neighborCount - 1;
        neighborEntities.optimistic.push_back({ entityID, pbd.min });
        neighborEntities.pessimistic.push_back({ entityID, pbd.minMax });
        if (neighborEntities.pessimistic.size() < neighborCount)
          continue;

        if (shouldHeapify)
        {
          std::ranges::make_heap(neighborEntities.optimistic);
          std::ranges::make_heap(neighborEntities.pessimistic);

          farthestEntityDistance = GetValueWithToleranceUpper(neighborEntities.pessimistic[0].distance, tolerance);
        }
        else
        {
          std::ranges::push_heap(neighborEntities.optimistic);

          std::ranges::push_heap(neighborEntities.pessimistic);
          std::ranges::pop_heap(neighborEntities.pessimistic);
          neighborEntities.pessimistic.pop_back();

          auto nthFarthestEntityDistanceLimit = GetValueWithToleranceUpper(neighborEntities.pessimistic.front().distance, tolerance);

          if (nthFarthestEntityDistanceLimit < farthestEntityDistance)
          {
            farthestEntityDistance = nthFarthestEntityDistanceLimit;

            bool pessimisticRemoved = false;
            while (neighborEntities.optimistic.front().distance > farthestEntityDistance)
            {
              auto const removedEntityID = neighborEntities.optimistic.front().entityID;
              auto const it =
                std::ranges::find_if(neighborEntities.pessimistic, [removedEntityID](auto const& ed) { return ed.entityID == removedEntityID; });
              if (it != neighborEntities.pessimistic.end())
              {
                neighborEntities.pessimistic.erase(it);
                pessimisticRemoved = true;
              }
              std::ranges::pop_heap(neighborEntities.optimistic);
              neighborEntities.optimistic.pop_back();
            }

            if (pessimisticRemoved)
              std::ranges::make_heap(neighborEntities.pessimistic);
          }
        }
      }
    }

    template<bool SHOULD_SORT_ENTITIES_BY_DISTANCE = true>
    static constexpr std::vector<EntityID> ConvertEntityDistanceToList(std::vector<EntityDistance>& neighborEntities, std::size_t neighborNo) noexcept
    {
      auto entityIDs = std::vector<EntityID>();
      if (neighborEntities.empty())
        return entityIDs;

      if (neighborEntities.size() < neighborNo)
      {
        if constexpr (SHOULD_SORT_ENTITIES_BY_DISTANCE)
        {
          std::sort(neighborEntities.begin(), neighborEntities.end());
        }
      }
      else
      {
        std::sort_heap(neighborEntities.begin(), neighborEntities.end());
      }

      auto const entityNo = neighborEntities.size();
      entityIDs.resize(entityNo);
      for (std::size_t i = 0; i < entityNo; ++i)
        entityIDs[i] = neighborEntities[i].entityID;

      return entityIDs;
    }

    constexpr IGM::Geometry GetNodeWallDistance(TVector const& searchPoint, NodeValue nodeValue, bool isInsideConsideredAsZero) const noexcept
    {
      return IGM::GetBoxWallDistanceAD(searchPoint, Core::GetNodeMinPoint(nodeValue), Core::GetNodeSize(nodeValue), isInsideConsideredAsZero);
    }

  public:
    // Get K Nearest Neighbor sorted by distance (point distance should be less than maxDistanceWithin, it is used as a Tolerance check). It may
    // results more element than neighborCount, if those are in equal distance (point-like) or possible hit (box-like).
    //
    // Accepted entityDistanceFn signatures:
    // * ReturnType(EntityID)
    // * ReturnType(EntityID, TVector)
    // * ReturnType(Entity)
    // * ReturnType(Entity, TVector)
    // where ReturnType is bool | TScalar | TFloatScalar | std::optional<TScalar> | std::optional<TFloatScalar>
    template<bool SHOULD_SORT_ENTITIES_BY_DISTANCE = true, typename TEntityDistanceFn = std::monostate>
    std::vector<EntityID> GetNearestNeighbors(
      TVector const& searchPoint,
      std::size_t neighborCount,
      TScalar maxDistanceWithin,
      EntityContainerView entities,
      TFloatScalar tolerance = GA::BASE_TOLERANCE,
      TEntityDistanceFn&& entityDistanceFn = {}) const noexcept
    {
      assert(neighborCount > 0 && "At least one neighbor must be requested!");
      if (neighborCount == 0)
        return {};

      auto neighborEntities = EntityDistances{};
      neighborEntities.optimistic.reserve(neighborCount + 4);
      neighborEntities.pessimistic.reserve(neighborCount);

      // farthestEntityDistance already contains the numerical tolerance
      auto farthestEntityDistance = maxDistanceWithin == std::numeric_limits<TScalar>::max() ? TFloatScalar(std::numeric_limits<TScalar>::max())
                                                                                             : GetValueWithToleranceUpper(maxDistanceWithin, tolerance);
      TraverseNodesByPriority(
        [&, this](auto const nodeValue, TFloatScalar nodeDistance) -> TraverseControl {
          if (nodeDistance >= farthestEntityDistance)
            return TraverseControl::Terminate;

          AddEntityDistance(
            neighborCount, searchPoint, entityDistanceFn, Core::GetNodeEntities(nodeValue), entities, tolerance, neighborEntities, farthestEntityDistance);

          return TraverseControl::Continue;
        },
        [&, this](auto const nodeValue) -> std::optional<TFloatScalar> {
          auto wallDistance = GetNodeWallDistance(searchPoint, nodeValue, true);
          if (wallDistance >= farthestEntityDistance)
            return std::nullopt;

          return wallDistance;
        });

      return ConvertEntityDistanceToList<SHOULD_SORT_ENTITIES_BY_DISTANCE>(neighborEntities.optimistic, neighborCount);
    }

    // Get K Nearest Neighbor sorted by distance
    //
    // Accepted entityDistanceFn signatures:
    // * bool(EntityID)
    // * bool(EntityID, TVector)
    // * bool(Entity)
    // * bool(Entity, TVector)
    // * std::optional<TScalar>(EntityID)
    // * std::optional<TScalar>(EntityID, TVector)
    // * std::optional<TScalar>(Entity)
    // * std::optional<TScalar>(Entity, TVector)
    template<bool SHOULD_SORT_ENTITIES_BY_DISTANCE = true, typename TEntityDistanceFn = std::monostate>
    std::vector<EntityID> GetNearestNeighbors(
      TVector const& searchPoint,
      std::size_t neighborNo,
      EntityContainerView entities,
      TFloatScalar tolerance = GA::BASE_TOLERANCE,
      TEntityDistanceFn&& entityDistanceFn = {}) const noexcept
    {
      return GetNearestNeighbors<SHOULD_SORT_ENTITIES_BY_DISTANCE>(
        searchPoint, neighborNo, std::numeric_limits<TScalar>::max(), entities, tolerance, std::forward<TEntityDistanceFn>(entityDistanceFn));
    }


  private:
    struct SweepAndPruneDatabase
    {
      constexpr SweepAndPruneDatabase(EntityContainerView entities, auto const& entityIDs) noexcept
      : m_sortedEntityIDs(entityIDs.begin(), entityIDs.end())
      {
        std::sort(m_sortedEntityIDs.begin(), m_sortedEntityIDs.end(), [&, this](auto const entityIDL, auto const entityIDR) {
          if constexpr (EA::GEOMETRY_TYPE == GeometryType::Point)
          {
            return GA::GetPointC(EA::GetGeometry(entities, entityIDL), 0) < GA::GetPointC(EA::GetGeometry(entities, entityIDR), 0);
          }
          else if constexpr (EA::GEOMETRY_TYPE == GeometryType::Box)
          {
            return GA::GetBoxMinC(EA::GetGeometry(entities, entityIDL), 0) < GA::GetBoxMinC(EA::GetGeometry(entities, entityIDR), 0);
          }
          else
          {
            static_assert(false, "Unsupported geometry type for collision detection!");
          }
          ORTHOTREE_UNREACHABLE();
        });
      }

      constexpr std::vector<EntityID> const& GetEntities() const noexcept { return m_sortedEntityIDs; }

    private:
      std::vector<EntityID> m_sortedEntityIDs;
    };

  public:
    // Client-defined Collision detector based on indexes. AABB intersection is executed independently from this checker.
    using FCollisionDetector = std::function<bool(EntityID, EntityID)>;

  private:
    // Collision detection: Returns all overlapping entities from the source trees.
    static void CollisionDetection(
      OrthoTreeQueryBase const& leftTree,
      EntityContainerView leftEntities,
      NodeValue leftNodeValue,
      OrthoTreeQueryBase const& rightTree,
      EntityContainerView rightEntities,
      NodeValue rightNodeValue,
      std::vector<std::pair<EntityID, EntityID>>& collidedEntities,
      TFloatScalar tolerance,
      std::optional<FCollisionDetector> const& collisionDetector) noexcept
    {
      struct NodeValueAndStatus
      {
        NodeValue nodeValue;
        bool IsTraversed;
      };

      enum : bool
      {
        Left,
        Right
      };

      auto const isSameTree = &leftTree == &rightTree;

      auto const trees = std::array{ &leftTree, &rightTree };

      auto entitiesInOrderCache = std::array<std::unordered_map<NodeValue, SweepAndPruneDatabase>, 2>{};
      auto const GetOrCreateEntitiesInOrder = [&](bool side, NodeValue nodeValue, EntityContainerView entities) -> std::vector<EntityID> const& {
        if (isSameTree)
          side = Left;

        auto itKeyAndSPD = entitiesInOrderCache[side].find(nodeValue);
        if (itKeyAndSPD == entitiesInOrderCache[side].end())
        {
          bool isInserted = false;
          std::tie(itKeyAndSPD, isInserted) =
            entitiesInOrderCache[side].emplace(nodeValue, SweepAndPruneDatabase(entities, trees[side]->GetNodeEntities(nodeValue)));
        }

        return itKeyAndSPD->second.GetEntities();
      };

      auto nodePairToProceed = std::queue<std::array<NodeValueAndStatus, 2>>{};
      nodePairToProceed.push({
        NodeValueAndStatus{  leftNodeValue, false },
        NodeValueAndStatus{ rightNodeValue, false }
      });
      while (!nodePairToProceed.empty())
      {
        auto const parentNodePair = nodePairToProceed.front();
        nodePairToProceed.pop();

        // Check the current ascendant content

        auto const& leftEntitiesInOrder = GetOrCreateEntitiesInOrder(Left, parentNodePair[Left].nodeValue, leftEntities);
        auto const& rightEntitiesInOrder = GetOrCreateEntitiesInOrder(Right, parentNodePair[Right].nodeValue, rightEntities);

        auto const rightEntityNo = rightEntitiesInOrder.size();
        std::size_t iRightEntityBegin = 0;
        for (auto const leftEntityID : leftEntitiesInOrder)
        {
          auto const& leftEntityGeometry = EA::GetGeometry(leftEntities, leftEntityID);
          for (; iRightEntityBegin < rightEntityNo; ++iRightEntityBegin)
          {
            auto const& rightEntityGeometry = EA::GetGeometry(rightEntities, rightEntitiesInOrder[iRightEntityBegin]);
            if constexpr (EA::GEOMETRY_TYPE == GeometryType::Point)
            {
              if (GA::GetPointC(rightEntityGeometry, 0) >= GA::GetPointC(leftEntityGeometry, 0))
                break; // sweep and prune optimization
            }
            else if constexpr (EA::GEOMETRY_TYPE == GeometryType::Box)
            {
              if (GA::GetBoxMaxC(rightEntityGeometry, 0) >= GA::GetBoxMinC(leftEntityGeometry, 0))
                break; // sweep and prune optimization
            }
            else
            {
              static_assert(false, "Unsupported geometry type for collision detection!");
            }
          }

          for (std::size_t iRightEntity = iRightEntityBegin; iRightEntity < rightEntityNo; ++iRightEntity)
          {
            auto const rightEntityID = rightEntitiesInOrder[iRightEntity];

            auto const& rightEntityGeometry = EA::GetGeometry(rightEntities, rightEntityID);
            if constexpr (EA::GEOMETRY_TYPE == GeometryType::Point)
            {
              if (GA::GetPointC(leftEntityGeometry, 0) < GA::GetPointC(rightEntityGeometry, 0))
                break; // sweep and prune optimization

              if (GA::ArePointEqual(leftEntityGeometry, rightEntityGeometry, tolerance))
                if (!collisionDetector || (*collisionDetector)(leftEntityID, rightEntityID))
                  collidedEntities.emplace_back(leftEntityID, rightEntityID);
            }
            else if constexpr (EA::GEOMETRY_TYPE == GeometryType::Box)
            {
              if (GA::GetBoxMaxC(leftEntityGeometry, 0) < GA::GetBoxMinC(rightEntityGeometry, 0))
                break; // sweep and prune optimization

              if (GA::AreBoxesOverlapped(leftEntityGeometry, rightEntityGeometry, false, false, tolerance))
                if (!collisionDetector || (*collisionDetector)(leftEntityID, rightEntityID))
                  collidedEntities.emplace_back(leftEntityID, rightEntityID);
            }
            else
            {
              static_assert(false, "Unsupported geometry type for collision detection!");
            }
          }
        }

        // Collect children
        auto childNodes = std::array<std::vector<NodeValueAndStatus>, 2>{};
        for (auto const sideID : { Left, Right })
        {
          auto const& [nodeValue, isTraversed] = parentNodePair[sideID];
          if (isTraversed)
            continue;

          auto const& childIDs = trees[sideID]->Core::GetNodeChildren(nodeValue);
          childNodes[sideID].resize(childIDs.size());
          std::transform(childIDs.begin(), childIDs.end(), childNodes[sideID].begin(), [&](NodeIDCR childNodeID) -> NodeValueAndStatus {
            return { trees[sideID]->Core::GetNodeValue(childNodeID), false };
          });
        }

        // Stop condition
        if (childNodes[Left].empty() && childNodes[Right].empty())
          continue;

        // Add parent if it has any element
        for (auto const sideID : { Left, Right })
          if (!trees[sideID]->IsNodeEntitiesEmpty(parentNodePair[sideID].nodeValue))
            childNodes[sideID].push_back({ parentNodePair[sideID].nodeValue, true });


        // Cartesian product of childNodes left and right
        for (auto const& leftChildNode : childNodes[Left])
          for (auto const& rightChildNode : childNodes[Right])
            if (!(leftChildNode.nodeValue == parentNodePair[Left].nodeValue && rightChildNode.nodeValue == parentNodePair[Right].nodeValue))
              if (IGM::AreBoxesOverlappingByMinPoint(
                    trees[Left]->GetNodeMinPoint(leftChildNode.nodeValue),
                    trees[Left]->GetNodeSize(leftChildNode.nodeValue),
                    trees[Right]->GetNodeMinPoint(rightChildNode.nodeValue),
                    trees[Right]->GetNodeSize(rightChildNode.nodeValue),
                    tolerance))
                nodePairToProceed.emplace(std::array{ leftChildNode, rightChildNode });
      }
    }

  public:
    // Collision detection: Returns all overlapping entities from the source trees.
    static std::vector<std::pair<EntityID, EntityID>> CollisionDetection(
      OrthoTreeQueryBase const& leftTree,
      EntityContainerView leftEntities,
      OrthoTreeQueryBase const& rightTree,
      EntityContainerView rightEntities,
      TFloatScalar tolerance = GA::BASE_TOLERANCE,
      std::optional<FCollisionDetector> const& collisionDetector = std::nullopt) noexcept
    {
      std::vector<std::pair<EntityID, EntityID>> collidedEntities;
      collidedEntities.reserve(leftEntities.size() / 10);

      CollisionDetection(
        leftTree,
        leftEntities,
        leftTree.GetNodeValue(Core::GetRootNodeID()),
        rightTree,
        rightEntities,
        rightTree.GetNodeValue(Core::GetRootNodeID()),
        collidedEntities,
        tolerance,
        collisionDetector);

      return collidedEntities;
    }


    // Collision detection: Returns all overlapping boxes from the source trees.
    std::vector<std::pair<EntityID, EntityID>> CollisionDetection(
      EntityContainerView const& boxes,
      OrthoTreeQueryBase const& otherTree,
      EntityContainerView const& otherBoxes,
      TFloatScalar tolerance = GA::BASE_TOLERANCE,
      std::optional<FCollisionDetector> const& collisionDetector = std::nullopt) const noexcept
    {
      return CollisionDetection(*this, boxes, otherTree, otherBoxes, tolerance, collisionDetector);
    }

  private:
    struct NodeCollisionContext
    {
      NodeValue nodeValue;
      IGM::Box Box;
      std::vector<EntityID> EntityIDs;
    };

    constexpr void FillNodeCollisionContext(NodeValue nodeValue, NodeCollisionContext& nodeContext, auto const& comparator) const noexcept
    {
      auto const& nodeEntities = Core::GetNodeEntities(nodeValue);

      nodeContext.nodeValue = nodeValue;
      nodeContext.EntityIDs.clear();
      nodeContext.EntityIDs.assign(nodeEntities.begin(), nodeEntities.end());
      std::ranges::sort(nodeContext.EntityIDs, comparator);
      nodeContext.Box = Core::GetNodeBox(nodeValue);
    }

    constexpr void InsertCollidedEntitiesInsideNode(
      EntityContainerView entities,
      NodeCollisionContext const& context,
      std::vector<std::pair<EntityID, EntityID>>& collidedEntities,
      TFloatScalar tolerance,
      std::optional<FCollisionDetector> const& collisionDetector) const noexcept
    {
      auto const& entityIDs = context.EntityIDs;
      auto const entityNo = entityIDs.size();
      for (std::size_t i = 0; i < entityNo; ++i)
      {
        auto const entityIDI = entityIDs[i];
        auto const& entityBoxI = EA::GetGeometry(entities, entityIDI);

        for (std::size_t j = i + 1; j < entityNo; ++j)
        {
          auto const entityIDJ = entityIDs[j];
          auto const& entityBoxJ = EA::GetGeometry(entities, entityIDJ);

          if constexpr (EA::GEOMETRY_TYPE == GeometryType::Point)
          {
            if (GA::GetPointC(entityBoxI, 0) < GA::GetPointC(entityBoxJ, 0))
              break; // sweep and prune optimization

            if (GA::ArePointsEqual(entityBoxI, entityBoxJ, tolerance))
              if (!collisionDetector || (*collisionDetector)(entityIDI, entityIDJ))
                collidedEntities.emplace_back(entityIDI, entityIDJ);
          }
          else if constexpr (EA::GEOMETRY_TYPE == GeometryType::Box)
          {
            if (GA::GetBoxMaxC(entityBoxI, 0) < GA::GetBoxMinC(entityBoxJ, 0))
              break; // sweep and prune optimization

            if (GA::AreBoxesOverlappedStrict(entityBoxI, entityBoxJ, tolerance))
              if (!collisionDetector || (*collisionDetector)(entityIDI, entityIDJ))
                collidedEntities.emplace_back(entityIDI, entityIDJ);
          }
          else
          {
            static_assert(false, "Unsupported geometry type for collision detection!");
          }
        }
      }
    }

    constexpr void InsertCollidedEntitiesWithParents(
      EntityContainerView entities,
      std::vector<NodeCollisionContext> const& nodeContextStack,
      std::vector<std::pair<EntityID, EntityID>>& collidedEntities,
      TFloatScalar tolerance,
      std::optional<FCollisionDetector> const& collisionDetector) const noexcept
    {
      auto const& nodeContext = nodeContextStack.back();
      auto const& nodeMinPoint = nodeContext.Box.Min;
      auto const& nodeSize = Core::GetNodeSize(nodeContext.nodeValue);
      auto const& entityIDs = nodeContext.EntityIDs;

      auto const entityNo = entityIDs.size();
      auto const depthNo = nodeContextStack.size();
      for (depth_t parentDepthID = 0; parentDepthID < depthNo - 1; ++parentDepthID)
      {
        auto const& [pParentNodeValue, parentBox, parenEntityIDs] = nodeContextStack[parentDepthID];

        auto iEntityBegin = std::size_t{};
        for (auto const parenEntityID : parenEntityIDs)
        {
          auto const& parentEntityGeometry = EA::GetGeometry(entities, parenEntityID);
          if constexpr (EA::GEOMETRY_TYPE == GeometryType::Point)
          {
            if (GA::GetPointC(parentEntityGeometry, 0) > nodeContext.Box.Max[0])
              break;
          }
          else if constexpr (EA::GEOMETRY_TYPE == GeometryType::Box)
          {
            if (GA::GetBoxMinC(parentEntityGeometry, 0) > nodeContext.Box.Max[0])
              break;
          }
          else
          {
            static_assert(false, "Unsupported geometry type for collision detection!");
          }

          auto const parentEntityMinPoint = IGM::GetBoxMinPointAD(parentEntityGeometry);
          auto const parentEntitySize = IGM::GetBoxSizeAD(parentEntityGeometry);
          if (!IGM::AreBoxesOverlappingByMinPoint(nodeMinPoint, nodeSize, parentEntityMinPoint, parentEntitySize, tolerance))
            continue;

          for (; iEntityBegin < entityNo; ++iEntityBegin)
          {
            if constexpr (EA::GEOMETRY_TYPE == GeometryType::Point)
            {
              if (GA::GetPointC(EA::GetGeometry(entities, entityIDs[iEntityBegin]), 0) >= GA::GetPointC(parentEntityGeometry, 0))
                break; // sweep and prune optimization
            }
            else if constexpr (EA::GEOMETRY_TYPE == GeometryType::Box)
            {
              if (GA::GetBoxMaxC(EA::GetGeometry(entities, entityIDs[iEntityBegin]), 0) >= GA::GetBoxMinC(parentEntityGeometry, 0))
                break; // sweep and prune optimization
            }
            else
            {
              static_assert(false, "Unsupported geometry type for collision detection!");
            }
          }

          for (std::size_t iEntity = iEntityBegin; iEntity < entityNo; ++iEntity)
          {
            auto const entityID = entityIDs[iEntity];
            auto const& entityGeometry = EA::GetGeometry(entities, entityID);

            if constexpr (EA::GEOMETRY_TYPE == GeometryType::Point)
            {
              if (GA::GetPointC(parentEntityGeometry, 0) < GA::GetPointC(entityGeometry, 0))
                break; // sweep and prune optimization

              if (GA::ArePointsEqual(parentEntityGeometry, entityGeometry, tolerance))
                if (!collisionDetector || (*collisionDetector)(entityID, parenEntityID))
                  collidedEntities.emplace_back(entityID, parenEntityID);
            }
            else if constexpr (EA::GEOMETRY_TYPE == GeometryType::Box)
            {
              if (GA::GetBoxMaxC(parentEntityGeometry, 0) < GA::GetBoxMinC(entityGeometry, 0))
                break; // sweep and prune optimization

              if (GA::AreBoxesOverlappedStrict(entityGeometry, parentEntityGeometry, tolerance))
                if (!collisionDetector || (*collisionDetector)(entityID, parenEntityID))
                  collidedEntities.emplace_back(entityID, parenEntityID);
            }
            else
            {
              static_assert(false, "Unsupported geometry type for collision detection!");
            }
          }
        }
      }
    }

    void InsertCollidedEntitiesInSubtree(
      EntityContainerView entities,
      auto const& comparator,
      NodeValue nodeValue,
      std::vector<NodeCollisionContext>& nodeContextStack,
      std::vector<std::pair<EntityID, EntityID>>& collidedEntities,
      TFloatScalar tolerance,
      std::optional<FCollisionDetector> const& collisionDetector) const noexcept
    {
      auto& nodeContext = nodeContextStack.emplace_back();
      FillNodeCollisionContext(nodeValue, nodeContext, comparator);
      InsertCollidedEntitiesInsideNode(entities, nodeContext, collidedEntities, tolerance, collisionDetector);
      InsertCollidedEntitiesWithParents(entities, nodeContextStack, collidedEntities, tolerance, collisionDetector);

      for (NodeIDCR childKey : Core::GetNodeChildren(nodeValue))
        InsertCollidedEntitiesInSubtree(entities, comparator, Core::GetNodeValue(childKey), nodeContextStack, collidedEntities, tolerance, collisionDetector);

      // Pairwise sub-tree collision detection for tree with overlapping child nodes
      if constexpr (Core::AreChildNodesOverlapping())
      {
        auto const& childNodeIDs = Core::GetNodeChildren(nodeValue);
        for (auto it1 = childNodeIDs.begin(); it1 != childNodeIDs.end(); ++it1)
        {
          for (auto it2 = it1 + 1; it2 != childNodeIDs.end(); ++it2)
          {
            CollisionDetection(
              *this, entities, Core::GetNodeValue(*it1), *this, entities, Core::GetNodeValue(*it2), collidedEntities, tolerance, collisionDetector);
          }
        }
      }

      nodeContextStack.pop_back();
    }

    // Collision detection between the stored entities from bottom to top logic
    template<typename TExecMode = SeqExec>
    std::vector<std::pair<EntityID, EntityID>> CollectCollidedEntities(
      EntityContainerView entities,
      TFloatScalar tolerance = EA::GEOMETRY_TYPE == GeometryType::Point ? GA::BASE_TOLERANCE : 0,
      std::optional<FCollisionDetector> const& collisionDetector = std::nullopt,
      TExecMode execMode = {}) const noexcept
    {
      static_assert(std::is_same_v<TExecMode, SeqExec> || std::is_same_v<TExecMode, ExecutionTags::Parallel>, "Invalid execution tag!");

      auto const comparator = [&entities](EntityID entityID1, EntityID entityID2) {
        if constexpr (EA::GEOMETRY_TYPE == GeometryType::Point)
        {
          auto const x1 = GA::GetPointC(EA::GetGeometry(entities, entityID1), 0);
          auto const x2 = GA::GetPointC(EA::GetGeometry(entities, entityID2), 0);
          return x1 < x2 || (x1 == x2 && entityID1 < entityID2);
        }
        else if constexpr (EA::GEOMETRY_TYPE == GeometryType::Box)
        {
          auto const x1 = GA::GetBoxMinC(EA::GetGeometry(entities, entityID1), 0);
          auto const x2 = GA::GetBoxMinC(EA::GetGeometry(entities, entityID2), 0);
          return x1 < x2 || (x1 == x2 && entityID1 < entityID2);
        }
        else
        {
          static_assert(false, "Unsupported geometry type for collision detection!");
        }
      };

      auto const entityNo = entities.size();
      auto collidedEntities = std::vector<std::pair<EntityID, EntityID>>{};
      collidedEntities.reserve(std::max<std::size_t>(100, entityNo / 10));

      // non-parallel execution
      if constexpr (std::is_same_v<TExecMode, SeqExec>)
      {
        auto nodeContextStack = std::vector<NodeCollisionContext>{};
        nodeContextStack.reserve(Core::GetDepthNo());
        InsertCollidedEntitiesInSubtree(
          entities, comparator, Core::GetNodeValue(Core::GetRootNodeID()), nodeContextStack, collidedEntities, tolerance, collisionDetector);

        return collidedEntities;
      }
      else
      {
        // maybe parallel execution

        auto const threadNum = uint32_t(std::thread::hardware_concurrency());
        auto const isSingleThreadMoreEffective = Core::GetNodeCount() < threadNum * 3;
        if (isSingleThreadMoreEffective)
        {
          auto nodeContextStack = std::vector<NodeCollisionContext>();
          nodeContextStack.reserve(Core::GetDepthNo());
          InsertCollidedEntitiesInSubtree(
            entities, comparator, Core::GetNodeValue(Core::GetRootNodeID()), nodeContextStack, collidedEntities, tolerance, collisionDetector);

          return collidedEntities;
        }

        constexpr uint32_t INVALID_INDEX = std::numeric_limits<uint32_t>::max();
        struct NodeData
        {
          NodeValue nodeValue;
          uint32_t parentID = INVALID_INDEX;
          uint32_t contextID = INVALID_INDEX;
        };

        auto nodeQueue = std::vector<NodeData>{};
        nodeQueue.reserve(threadNum * 2);
        nodeQueue.push_back(NodeData{ Core::GetNodeValue(Core::GetRootNodeID()), INVALID_INDEX, INVALID_INDEX });

        auto nodeContextMap = std::vector<NodeCollisionContext>{};

        uint32_t nodeQueueNum = 1;
        uint32_t childNodeAddedParentNum = 0;
        for (uint32_t i = 0; 0 < nodeQueueNum && nodeQueueNum < threadNum - 2; --nodeQueueNum, ++i)
        {
          ++childNodeAddedParentNum;
          auto& [nodeValue, parentID, contextID] = nodeQueue[i];
          contextID = static_cast<uint32_t>(nodeContextMap.size());
          auto& nodeContext = nodeContextMap.emplace_back();
          FillNodeCollisionContext(nodeValue, nodeContext, comparator);

          for (NodeIDCR childNodeID : Core::GetNodeChildren(nodeValue))
          {
            nodeQueue.push_back(NodeData{ Core::GetNodeValue(childNodeID), i, INVALID_INDEX });
            ++nodeQueueNum;
          }
        }

        if (nodeQueueNum < threadNum)
        {
          auto nodeContextStack = std::vector<NodeCollisionContext>();
          nodeContextStack.reserve(Core::GetDepthNo());
          InsertCollidedEntitiesInSubtree(
            entities, comparator, Core::GetNodeValue(Core::GetRootNodeID()), nodeContextStack, collidedEntities, tolerance, collisionDetector);

          return collidedEntities;
        }

        // parallel execution

        struct TaskContext
        {
          NodeValue nodeValue;
          NodeValue nodeValueSecondary;
          uint32_t parentID;
          uint32_t contextID;
          std::vector<std::pair<EntityID, EntityID>> collidedEntities;
        };

        auto const nodeQueueAllNo = nodeQueue.size();
        auto const nodeQueueBegin = nodeQueueAllNo - nodeQueueNum;
        auto taskContexts = std::vector<TaskContext>(nodeQueueNum);
        for (std::size_t taskID = 0; taskID < nodeQueueNum; ++taskID)
        {
          auto const& [nodeValue, parentID, contextID] = nodeQueue[nodeQueueBegin + taskID];
          taskContexts[taskID].nodeValue = nodeValue;
          taskContexts[taskID].parentID = parentID;
          taskContexts[taskID].contextID = contextID;
        }

        EXEC_POL_DEF(epcd); // GCC 11.3
        std::for_each(EXEC_POL_ADD(epcd) taskContexts.begin(), taskContexts.end(), [&, this](auto& taskContext) {
          auto const& [nodeValue, nvs_, parentID, contextID, ce_] = taskContext;

          auto nodeContextStack = std::vector<NodeCollisionContext>();
          nodeContextStack.reserve(Core::GetDepthNo());

          auto parentID_ = parentID;
          while (parentID_ != INVALID_INDEX)
          {
            auto const& parentNodeData = nodeQueue[parentID_];
            nodeContextStack.push_back(nodeContextMap[parentNodeData.contextID]);
            parentID_ = parentNodeData.parentID;
          }

          std::ranges::reverse(nodeContextStack);
          InsertCollidedEntitiesInSubtree(entities, comparator, nodeValue, nodeContextStack, taskContext.collidedEntities, tolerance, collisionDetector);
        });

        if constexpr (Core::AreChildNodesOverlapping())
        {
          for (std::size_t i = 0; i < childNodeAddedParentNum; ++i)
          {
            auto const& childNodeIDs = Core::GetNodeChildren(nodeQueue[i].nodeValue);
            for (auto it1 = childNodeIDs.begin(); it1 != childNodeIDs.end(); ++it1)
            {
              for (auto it2 = it1 + 1; it2 != childNodeIDs.end(); ++it2)
              {
                taskContexts.push_back({ .nodeValue = Core::GetNodeValue(*it1),
                                         .nodeValueSecondary = Core::GetNodeValue(*it2),
                                         .parentID = INVALID_INDEX,
                                         .contextID = INVALID_INDEX,
                                         .collidedEntities = {} });
              }
            }
          }

          EXEC_POL_DEF(epst); // GCC 11.3
          std::for_each(EXEC_POL_ADD(epst) taskContexts.begin() + nodeQueueNum, taskContexts.end(), [&, this](auto& taskContext) {
            CollisionDetection(
              *this, entities, taskContext.nodeValue, *this, entities, taskContext.nodeValueSecondary, taskContext.collidedEntities, tolerance, collisionDetector);
          });
        }
        auto collidedEntitiesInParents = std::vector<std::pair<EntityID, EntityID>>{};
        auto nodeContextStack = std::vector<NodeCollisionContext>();
        auto usedContextsStack = std::vector<NodeCollisionContext*>{};
        std::for_each(nodeQueue.begin(), nodeQueue.end() - nodeQueueNum, [&, this](auto const& nodeData) {
          {
            usedContextsStack.emplace_back(&nodeContextMap[nodeData.contextID]);
            auto parentID = nodeData.parentID;
            while (parentID != INVALID_INDEX)
            {
              auto const& parentData = nodeQueue[parentID];
              usedContextsStack.emplace_back(&nodeContextMap[parentData.contextID]);
              parentID = parentData.parentID;
            }

            for (auto it = usedContextsStack.rbegin(); it != usedContextsStack.rend(); ++it)
              nodeContextStack.emplace_back(std::move(*(*it)));
          }

          InsertCollidedEntitiesInsideNode(entities, nodeContextStack.back(), collidedEntitiesInParents, tolerance, collisionDetector);
          InsertCollidedEntitiesWithParents(entities, nodeContextStack, collidedEntitiesInParents, tolerance, collisionDetector);

          {
            auto i = 0;
            for (auto it = usedContextsStack.rbegin(); it != usedContextsStack.rend(); ++it, ++i)
              *(*it) = std::move(nodeContextStack[i]);
            usedContextsStack.clear();
            nodeContextStack.clear();
          }
        });

        auto const collisionNo =
          std::transform_reduce(taskContexts.begin(), taskContexts.end(), collidedEntitiesInParents.size(), std::plus{}, [](auto const& taskContext) {
            return taskContext.collidedEntities.size();
          });

        collidedEntities.reserve(collisionNo);
        collidedEntities.insert(collidedEntities.end(), collidedEntitiesInParents.begin(), collidedEntitiesInParents.end());
        for (auto const& taskContext : taskContexts)
          collidedEntities.insert(collidedEntities.end(), taskContext.collidedEntities.begin(), taskContext.collidedEntities.end());

        return collidedEntities;
      }
    }

  public:
    // Collision detection between the stored entities from bottom to top logic
    template<typename TExecMode = SeqExec>
    std::vector<std::pair<EntityID, EntityID>> CollisionDetection(
      EntityContainerView entities, TFloatScalar tolerance = GA::BASE_TOLERANCE, TExecMode execMode = {}) const noexcept
    {
      return CollectCollidedEntities(entities, tolerance, std::nullopt, execMode);
    }

    // Collision detection between the stored entities from bottom to top logic
    template<typename TExecMode = SeqExec>
    std::vector<std::pair<EntityID, EntityID>> CollisionDetection(
      EntityContainerView entities, FCollisionDetector&& collisionDetector, TFloatScalar tolerance = GA::BASE_TOLERANCE, TExecMode execMode = {}) const noexcept
    {
      return CollectCollidedEntities(entities, tolerance, std::forward<FCollisionDetector>(collisionDetector), execMode);
    }

  public:
    // Get all entities that are intersected by the ray in order
    //
    // Accepted entityDistanceFn signatures:
    // * bool(EntityID)
    // * bool(EntityID, TVector rayBasePoint, TVector rayHeading)
    // * bool(Entity)
    // * bool(Entity, TVector rayBasePoint, TVector rayHeading)
    // * std::optional<TFloatScalar>(EntityID)
    // * std::optional<TFloatScalar>(EntityID, TVector rayBasePoint, TVector rayHeading)
    // * std::optional<TFloatScalar>(Entity)
    // * std::optional<TFloatScalar>(Entity, TVector rayBasePoint, TVector rayHeading)
    template<bool SHOULD_SORT_ENTITIES_BY_DISTANCE = true, typename TEntityRayHitTester = std::monostate>
    std::vector<EntityID> RayIntersectedAll(
      TVector const& rayBasePoint,
      TVector const& rayHeading,
      EntityContainerView entities,
      TFloatScalar tolerance = GA::BASE_TOLERANCE,
      TFloatScalar toleranceIncrement = {},
      TScalar maxExaminationDistance = std::numeric_limits<TScalar>::max(),
      TEntityRayHitTester&& entityRayHitTester = {}) const noexcept
    {
      assert(
        toleranceIncrement <= std::max<TFloatScalar>(tolerance * 10, TFloatScalar(0.1)) &&
        "toleranceIncrement seems to be large. Probably maxExaminationDistance is used.");

      const auto rayHitTester = IGM::RayHitTester::Make(rayBasePoint, rayHeading, tolerance, toleranceIncrement);
      if (!rayHitTester)
        return {};

      struct ItemDistance
      {
        IGM_Geometry distance;
        auto operator<=>(ItemDistance const& rhs) const = default;
      };

      struct EntityDistance : ItemDistance
      {
        EntityID entityID;
        auto operator<=>(EntityDistance const& rhs) const { return ItemDistance::operator<=>(rhs); }
      };

      using EntityDistanceContainer = std::conditional_t<SHOULD_SORT_ENTITIES_BY_DISTANCE, std::vector<EntityDistance>, std::vector<EntityID>>;

      auto foundEntities = EntityDistanceContainer{};
      foundEntities.reserve(20);

      TraverseNodesDepthFirst([&, this](auto const nodeValue) {
        auto const nodeHit = rayHitTester->Hit(Core::GetNodeMinPoint(nodeValue), Core::GetNodeSize(nodeValue));
        if (!nodeHit)
          return TraverseControl::SkipChildren;

        for (auto const entityID : Core::GetNodeEntities(nodeValue))
        {
          auto const entityDistance = rayHitTester->Hit(EA::GetGeometry(entities, entityID));
          if (!entityDistance)
            continue;

          if (!((maxExaminationDistance == 0 || entityDistance->enterDistance <= maxExaminationDistance)))
            continue;

          auto closestEntityDistance = entityDistance->enterDistance;
          if constexpr (!std::is_same_v<std::monostate, TEntityRayHitTester>)
          {
            auto result = TestEntity(entityRayHitTester, entityID, entities, rayBasePoint, rayHeading);
            if (!result)
              continue;

            if constexpr (SHOULD_SORT_ENTITIES_BY_DISTANCE && detail::IsStdOptionalV<decltype(result)>)
            {
              closestEntityDistance = *result;
            }
          }

          if constexpr (SHOULD_SORT_ENTITIES_BY_DISTANCE)
            detail::insert(foundEntities, EntityDistance{ { closestEntityDistance }, entityID });
          else
            detail::insert(foundEntities, entityID);
        }

        return TraverseControl::Continue;
      });

      if constexpr (!SHOULD_SORT_ENTITIES_BY_DISTANCE)
      {
        return foundEntities;
      }
      else
      {
        std::ranges::sort(foundEntities);

        auto foundEntityIDs = std::vector<EntityID>(foundEntities.size());
        std::ranges::transform(foundEntities, foundEntityIDs.begin(), [](auto const& entityDistance) { return entityDistance.entityID; });
        return foundEntityIDs;
      }
    }

    // Get all box which is intersected by the ray in order
    std::vector<EntityID> RayIntersectedAll(
      TRay const& ray,
      EntityContainerView entities,
      TFloatScalar tolerance = GA::BASE_TOLERANCE,
      TFloatScalar toleranceIncrement = {},
      TScalar maxExaminationDistance = std::numeric_limits<TScalar>::max()) const noexcept
    {
      return RayIntersectedAll(GA::GetRayOrigin(ray), GA::GetRayDirection(ray), entities, tolerance, toleranceIncrement, maxExaminationDistance);
    }

    // Get first entities that hit by the ray
    //
    // Accepted entityDistanceFn signatures:
    // * bool(EntityID)
    // * bool(EntityID, TVector rayBasePoint, TVector rayHeading)
    // * bool(Entity)
    // * bool(Entity, TVector rayBasePoint, TVector rayHeading)
    // * std::optional<TFloatScalar>(EntityID)
    // * std::optional<TFloatScalar>(EntityID, TVector rayBasePoint, TVector rayHeading)
    // * std::optional<TFloatScalar>(Entity)
    // * std::optional<TFloatScalar>(Entity, TVector rayBasePoint, TVector rayHeading)
    template<typename TEntityRayHitTester = std::monostate>
    std::vector<EntityID> RayIntersectedFirst(
      TVector const& rayBasePoint,
      TVector const& rayHeading,
      EntityContainerView entities,
      TFloatScalar tolerance = GA::BASE_TOLERANCE,
      TFloatScalar toleranceIncrement = {},
      TScalar maxDistance = std::numeric_limits<TScalar>::max(),
      TEntityRayHitTester&& entityRayHitTester = {}) const noexcept
    {
      const auto rayHitTester = IGM::RayHitTester::Make(rayBasePoint, rayHeading, tolerance, toleranceIncrement);
      if (!rayHitTester)
        return {};

      struct Candidate
      {
        EntityID entityID;
        TFloatScalar enterDistance;

        constexpr auto operator<=>(Candidate const& other) const noexcept { return enterDistance <=> other.enterDistance; }
      };

      const auto appliedTolerance = tolerance == 0 ? std::numeric_limits<TFloatScalar>::epsilon() : tolerance;

      // max-heap by enterDistance (largest enterDistance at top)
      auto maxDistanceHeap = std::priority_queue<Candidate, std::vector<Candidate>>{};
      auto maxExaminationDistance = TFloatScalar(maxDistance);
      TraverseNodesByPriority(
        [&, this, rayHitTester = *rayHitTester](auto const nodeValue, TFloatScalar nodeEnterDistance) {
          if (nodeEnterDistance > maxExaminationDistance)
            return TraverseControl::Terminate;

          for (auto const entityID : Core::GetNodeEntities(nodeValue))
          {
            auto pickDomain = rayHitTester.Hit(EA::GetGeometry(entities, entityID));
            if (!pickDomain)
              continue;

            if (pickDomain->enterDistance > maxExaminationDistance)
              continue;

            // furthest possible hit distance
            auto& [closestHitDistance, furthestPossibleHitDistance] = *pickDomain;
            if constexpr (!std::is_same_v<std::monostate, TEntityRayHitTester>)
            {
              auto result = TestEntity(entityRayHitTester, entityID, entities, rayBasePoint, rayHeading);
              if (!result)
                continue;

              if constexpr (detail::IsStdOptionalV<decltype(result)>)
              {
                assert(*result <= furthestPossibleHitDistance && "entityRayHitTester returned out of box result.");
                assert(*result >= closestHitDistance && "entityRayHitTester returned out of box result.");
                closestHitDistance = furthestPossibleHitDistance = *result;
              }
            }

            // push candidate
            maxDistanceHeap.push(Candidate{ entityID, closestHitDistance });

            // update bestExitDistance if this entity's exit is closer
            if (furthestPossibleHitDistance >= maxExaminationDistance)
              continue;

            maxExaminationDistance = (TScalar(1) + toleranceIncrement) * furthestPossibleHitDistance + appliedTolerance;

            // drop any candidates whose enterDistance > new maxExaminationDistance
            while (!maxDistanceHeap.empty() && maxDistanceHeap.top().enterDistance > maxExaminationDistance)
              maxDistanceHeap.pop();
          }

          return TraverseControl::Continue;
        },
        [&, this, rayHitTester = *rayHitTester](auto const nodeValue) -> std::optional<TFloatScalar> {
          auto result = rayHitTester.Hit(Core::GetNodeMinPoint(nodeValue), Core::GetNodeSize(nodeValue));
          if (!result)
            return std::nullopt;

          return result->enterDistance;
        });

      std::vector<EntityID> resulEntityIDs;
      resulEntityIDs.reserve(maxDistanceHeap.size());
      for (; !maxDistanceHeap.empty(); maxDistanceHeap.pop())
        resulEntityIDs.push_back(maxDistanceHeap.top().entityID);

      return resulEntityIDs;
    }

    // Get first entities that hit by the ray
    //
    // Accepted entityDistanceFn signatures:
    // * bool(EntityID)
    // * bool(EntityID, TVector)
    // * bool(Entity)
    // * bool(Entity, TVector)
    // * std::optional<TScalar>(EntityID)
    // * std::optional<TScalar>(EntityID, TVector)
    // * std::optional<TScalar>(Entity)
    // * std::optional<TScalar>(Entity, TVector)
    template<typename TEntityRayHitTester = std::monostate>
    std::vector<EntityID> RayIntersectedFirst(
      TRay const& ray,
      EntityContainerView entities,
      TFloatScalar tolerance = GA::BASE_TOLERANCE,
      TFloatScalar toleranceIncrement = {},
      TScalar maxDistance = std::numeric_limits<TScalar>::max(),
      TEntityRayHitTester&& entityHitTester = {}) const noexcept
    {
      return RayIntersectedFirst(
        GA::GetRayOrigin(ray), GA::GetRayDirection(ray), entities, tolerance, toleranceIncrement, maxDistance, std::forward<TEntityRayHitTester>(entityHitTester));
    }

    // Compare tree equality
    template<typename TOtherCore>
    bool IsEqualTo(OrthoTreeQueryBase<TOtherCore> const& otherTree, TFloatScalar tolerance = GA::BASE_TOLERANCE) const noexcept
    {
      if (Core::GetMaxDepthID() != otherTree.GetMaxDepthID() || Core::GetMaxElementNum() != otherTree.GetMaxElementNum())
        return false;

      if (Core::GetNodeCount() != otherTree.GetNodeCount())
        return false;

      return AreNodesEqual(otherTree, Core::GetRootNodeID(), otherTree.GetRootNodeID(), tolerance);
    }

  private:
    template<typename TOtherCore>
    bool AreNodesEqual(OrthoTreeQueryBase<TOtherCore> const& otherTree, NodeID keyL, typename TOtherCore::NodeID keyR, TFloatScalar tolerance) const noexcept
    {
      auto const nodeL = Core::GetNodeValue(keyL);
      auto const nodeR = otherTree.GetNodeValue(keyR);

      // 1. Entities Check
      auto const entitiesL = Core::GetNodeEntities(nodeL);
      auto const entitiesR = otherTree.GetNodeEntities(nodeR);
      if (entitiesL.size() != entitiesR.size())
        return false;

      if (entitiesL.size() > 0)
      {
        auto setL = std::vector<EntityID>(entitiesL.begin(), entitiesL.end());
        auto setR = std::vector<typename TOtherCore::EntityID>(entitiesR.begin(), entitiesR.end());
        std::sort(setL.begin(), setL.end());
        std::sort(setR.begin(), setR.end());
        for (size_t i = 0; i < setL.size(); ++i)
          if (setL[i] != setR[i])
            return false;
      }

      // 2. Geometry Check: node boxes must be equal (not just overlapping)
      auto const boxL = Core::GetNodeBox(nodeL);
      auto const boxR = otherTree.GetNodeBox(nodeR);
      for (dim_t d = 0; d < GA::DIMENSION_NO; ++d)
      {
        if (std::abs(boxL.Min[d] - boxR.Min[d]) > tolerance || std::abs(boxL.Max[d] - boxR.Max[d]) > tolerance)
          return false;
      }

      // 3. Children Check
      auto const& childrenL = Core::GetNodeChildren(nodeL);
      auto const& childrenR = otherTree.GetNodeChildren(nodeR);
      if (childrenL.size() != childrenR.size())
        return false;

      if (childrenL.size() == 0)
        return true;

      // Map octants to children via Geometry
      auto mapL = std::map<ChildID, NodeID>();
      auto mapR = std::map<ChildID, typename TOtherCore::NodeID>();

      auto const parentMinL = Core::GetNodeMinPoint(nodeL);
      auto const parentSizeL = Core::GetNodeSize(nodeL);
      auto parentCenterL = parentMinL;
      for (dim_t d = 0; d < GA::DIMENSION_NO; ++d)
        parentCenterL[d] += parentSizeL[d] * 0.5;

      auto const parentMinR = otherTree.GetNodeMinPoint(nodeR);
      auto const parentSizeR = otherTree.GetNodeSize(nodeR);
      auto parentCenterR = parentMinR;
      for (dim_t d = 0; d < GA::DIMENSION_NO; ++d)
        parentCenterR[d] += parentSizeR[d] * 0.5;

      for (auto const childNodeID : childrenL)
      {
        auto const childNode = Core::GetNodeValue(childNodeID);
        auto const childMin = Core::GetNodeMinPoint(childNode);
        auto const childSize = Core::GetNodeSize(childNode);
        auto childCenter = childMin;
        for (dim_t d = 0; d < GA::DIMENSION_NO; ++d)
          childCenter[d] += childSize[d] * 0.5;

        ChildID octant = 0;
        for (dim_t d = 0; d < GA::DIMENSION_NO; ++d)
          if (childCenter[d] > parentCenterL[d])
            octant |= (ChildID(1) << d);
        mapL[octant] = childNodeID;
      }

      for (auto const childNodeID : childrenR)
      {
        auto const childNode = otherTree.GetNodeValue(childNodeID);
        auto const childMin = otherTree.GetNodeMinPoint(childNode);
        auto const childSize = otherTree.GetNodeSize(childNode);
        auto childCenter = childMin;
        for (dim_t d = 0; d < GA::DIMENSION_NO; ++d)
          childCenter[d] += childSize[d] * 0.5;

        ChildID octant = 0;
        for (dim_t d = 0; d < GA::DIMENSION_NO; ++d)
          if (childCenter[d] > parentCenterR[d]) // Use parentCenterR for the right tree
            octant |= (ChildID(1) << d);
        mapR[octant] = childNodeID;
      }

      if (mapL.size() != mapR.size())
        return false;

      for (auto const& [octant, keyChildL] : mapL)
      {
        auto itR = mapR.find(octant);
        if (itR == mapR.end())
          return false;

        auto const keyChildR = itR->second;
        if (!AreNodesEqual(otherTree, keyChildL, keyChildR, tolerance))
          return false;
      }

      return true;
    }
  };

} // namespace OrthoTree


//////////////////////////////////////////////////////////////////////////
// Container type to make easier the OrthoTree usage
//////////////////////////////////////////////////////////////////////////


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

  // Provides general vector/box/ray/plane operations based on a basic adapter interface. If the geometric types are connected to a BLAS, it is
  // recommended to implement a custom GeneralGeometryAdapterTemplate.
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

    static constexpr bool DoesBoxContainPoint(Box const& box, Vector const& point, FloatScalar tolerance) noexcept
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
    static constexpr EBoxRelation GetBoxRelation(Box const& e1, Box const& e2, FloatScalar tolerance) noexcept
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

    static constexpr bool AreBoxesOverlappedStrict(Box const& e1, Box const& e2, FloatScalar tolerance) noexcept
    {
      return GetBoxRelation(e1, e2, tolerance) == EBoxRelation::Overlapped;
    }

    static constexpr bool AreBoxesOverlapped(
      Box const& e1, Box const& e2, bool e1_must_contain_e2 = true, bool fOverlapPtTouchAllowed = false, FloatScalar tolerance = Base::BASE_TOLERANCE) noexcept
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
  using GeneralGeometryAdapterTemplate = GeneralGeometryAdapter<
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


  // Geometry aliases
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
  using GeneralGeometryAdapterND = GeneralGeometryAdapterTemplate<
    DIMENSION_NO,
    OrthoTree::VectorND<DIMENSION_NO, TScalar>,
    OrthoTree::BoundingBoxND<DIMENSION_NO, TScalar>,
    OrthoTree::RayND<DIMENSION_NO, TScalar>,
    OrthoTree::PlaneND<DIMENSION_NO, TScalar>,
    TScalar>;
} // namespace OrthoTree


namespace OrthoTree
{
  template<typename TOrthoTreeCore>
  class OrthoTreeManaged
  {
  public:
    using CONFIG = typename TOrthoTreeCore::CONFIG;

    using GA = typename TOrthoTreeCore::GA;

    using TScalar = typename GA::Scalar;
    using TFloatScalar = typename GA::FloatScalar;
    using TVector = typename GA::Vector;
    using TBox = typename GA::Box;
    using TRay = typename GA::Ray;
    using TPlane = typename GA::Plane;

    using EA = typename TOrthoTreeCore::EA;
    using Entity = typename EA::Entity;
    using EntityID = typename EA::EntityID;
    using EntityContainer = EA::EntityContainer;
    using EntityContainerView = typename EA::EntityContainerView;

    using QueryCondition = typename TOrthoTreeCore::QueryCondition;

  protected:
    TOrthoTreeCore m_tree;
    EntityContainer m_entities;

  private:
    static constexpr uint32_t SERIALIZED_VERSION_ID = 0;

    template<typename TArchive, typename TOrthoTreeCore_>
    friend void serialize(TArchive& ar, OrthoTreeManaged<TOrthoTreeCore_>& core);

  public: // Constructors
    constexpr explicit OrthoTreeManaged() noexcept = default;

  public: // Constructors without box space
    // Constructor for any contiguous container with runtime parallel parameter
    template<typename TExecMode = SeqExec>
    explicit OrthoTreeManaged(
      std::span<Entity const> const& geometryCollection,
      std::size_t maxElementNoInNode = CONFIG::DEFAULT_TARGET_ELEMENT_NUM_IN_NODES,
      TExecMode execMode = {}) noexcept
      requires(EA::ENTITY_ID_STRATEGY != EntityIdStrategy::EntityKeyed && detail::HasCreateSimple<TOrthoTreeCore>)
    : m_entities(geometryCollection.begin(), geometryCollection.end())
    {
#ifndef __cpp_lib_execution
      static_assert(!std::is_same_v<TExecMode, ExecutionTags::Parallel>, "Parallel creation is based on execution policies. __cpp_lib_execution is required.");
#endif
      TOrthoTreeCore::Create(m_tree, m_entities, maxElementNoInNode, execMode);
    }

    // Constructor for any copyable container with runtime parallel parameter
    template<typename TExecMode = SeqExec>
    explicit OrthoTreeManaged(
      EntityContainer const& geometryCollection, std::size_t maxElementNoInNode = CONFIG::DEFAULT_TARGET_ELEMENT_NUM_IN_NODES, TExecMode execMode = {}) noexcept
      requires(detail::HasCreateSimple<TOrthoTreeCore>)
    : m_entities(geometryCollection)
    {
#ifndef __cpp_lib_execution
      static_assert(!std::is_same_v<TExecMode, ExecutionTags::Parallel>, "Parallel creation is based on execution policies. __cpp_lib_execution is required.");
#endif
      TOrthoTreeCore::Create(m_tree, m_entities, maxElementNoInNode, execMode);
    }

    // Constructor for any movable container with runtime parallel parameter
    template<typename TExecMode = SeqExec>
    explicit OrthoTreeManaged(
      EntityContainer&& geometryCollection, std::size_t maxElementNoInNode = CONFIG::DEFAULT_TARGET_ELEMENT_NUM_IN_NODES, TExecMode execMode = {}) noexcept
      requires(detail::HasCreateSimple<TOrthoTreeCore>)
    : m_entities(std::move(geometryCollection))
    {
#ifndef __cpp_lib_execution
      static_assert(!std::is_same_v<TExecMode, ExecutionTags::Parallel>, "Parallel creation is based on execution policies. __cpp_lib_execution is required.");
#endif
      TOrthoTreeCore::Create(m_tree, m_entities, maxElementNoInNode, execMode);
    }

    // Constructor for any contiguous container with compile-time parallel parameter
    template<typename TExecMode>
    explicit OrthoTreeManaged(
      TExecMode execMode,
      std::span<Entity const> const& geometryCollection,
      std::size_t maxElementNoInNode = CONFIG::DEFAULT_TARGET_ELEMENT_NUM_IN_NODES) noexcept
      requires(EA::ENTITY_ID_STRATEGY != EntityIdStrategy::EntityKeyed && detail::HasCreateSimple<TOrthoTreeCore>)
    : m_entities(geometryCollection.begin(), geometryCollection.end())
    {
#ifdef __cpp_lib_execution
#else
      static_assert(!std::is_same_v<TExecMode, ExecutionTags::Parallel>, "Parallel creation is based on execution policies. __cpp_lib_execution is required.");
#endif
      TOrthoTreeCore::Create(m_tree, m_entities, maxElementNoInNode, execMode);
    }

    // Constructor for any copyable container compile-time parallel parameter
    template<typename TExecMode>
    explicit OrthoTreeManaged(
      TExecMode execMode, EntityContainer const& geometryCollection, std::size_t maxElementNoInNode = CONFIG::DEFAULT_TARGET_ELEMENT_NUM_IN_NODES) noexcept
      requires(detail::HasCreateSimple<TOrthoTreeCore>)
    : m_entities(geometryCollection)
    {
#ifdef __cpp_lib_execution
#else
      static_assert(!std::is_same_v<TExecMode, ExecutionTags::Parallel>, "Parallel creation is based on execution policies. __cpp_lib_execution is required.");
#endif
      TOrthoTreeCore::Create(m_tree, m_entities, maxElementNoInNode, execMode);
    }

    // Constructor for any movable container with compile-time parallel parameter
    template<typename TExecMode>
    explicit OrthoTreeManaged(
      TExecMode execMode, EntityContainer&& geometryCollection, std::size_t maxElementNoInNode = CONFIG::DEFAULT_TARGET_ELEMENT_NUM_IN_NODES) noexcept
      requires(detail::HasCreateSimple<TOrthoTreeCore>)
    : m_entities(std::move(geometryCollection))
    {
#ifdef __cpp_lib_execution
#else
      static_assert(!std::is_same_v<TExecMode, ExecutionTags::Parallel>, "Parallel creation is based on execution policies. __cpp_lib_execution is required.");
#endif
      TOrthoTreeCore::Create(m_tree, m_entities, maxElementNoInNode, execMode);
    }

    template<typename TExecMode = SeqExec>
    static OrthoTreeManaged Create(
      std::span<Entity const> const& entities, std::size_t maxElementNoInNode = CONFIG::DEFAULT_TARGET_ELEMENT_NUM_IN_NODES, TExecMode execMode = {}) noexcept
      requires(EA::ENTITY_ID_STRATEGY != EntityIdStrategy::EntityKeyed && detail::HasCreateSimple<TOrthoTreeCore>)
    {
      auto otc = OrthoTreeManaged();
      otc.m_entities = std::vector(entities.begin(), entities.end());
      TOrthoTreeCore::Create(otc.m_tree, otc.m_entities, maxElementNoInNode, execMode);
      return otc;
    }

    template<typename TExecMode = SeqExec>
    static OrthoTreeManaged Create(
      EntityContainer const& entities, std::size_t maxElementNoInNode = CONFIG::DEFAULT_TARGET_ELEMENT_NUM_IN_NODES, TExecMode execMode = {}) noexcept
      requires(detail::HasCreateSimple<TOrthoTreeCore>)
    {
      auto otc = OrthoTreeManaged();
      otc.m_entities = entities;
      TOrthoTreeCore::Create(otc.m_tree, otc.m_entities, maxElementNoInNode, execMode);
      return otc;
    }

    template<typename TExecMode = SeqExec>
    static OrthoTreeManaged Create(
      EntityContainer&& entities, std::size_t maxElementNoInNode = CONFIG::DEFAULT_TARGET_ELEMENT_NUM_IN_NODES, TExecMode execMode = {}) noexcept
      requires(detail::HasCreateSimple<TOrthoTreeCore>)
    {
      auto otc = OrthoTreeManaged();
      otc.m_entities = std::move(entities);
      TOrthoTreeCore::Create(otc.m_tree, otc.m_entities, maxElementNoInNode, execMode);
      return otc;
    }


  public: // Constructors with box space
    explicit OrthoTreeManaged(
      TBox const& boxSpace,
      std::optional<depth_t> maxDepthID = std::nullopt,
      std::size_t maxElementNoInNode = CONFIG::DEFAULT_TARGET_ELEMENT_NUM_IN_NODES) noexcept
      requires(detail::HasCreateWithBoxSpaceV<TOrthoTreeCore>)
    {
      m_tree.Init(boxSpace, maxDepthID.value_or(CONFIG::MAX_ALLOWED_DEPTH_ID), maxElementNoInNode);
    }

    // Constructor for any contiguous container with runtime parallel parameter
    template<typename TExecMode = SeqExec>
    explicit OrthoTreeManaged(
      std::span<Entity const> const& geometryCollection,
      std::optional<depth_t> maxDepthID = std::nullopt,
      std::optional<TBox> boxSpace = std::nullopt,
      std::size_t maxElementNoInNode = CONFIG::DEFAULT_TARGET_ELEMENT_NUM_IN_NODES,
      TExecMode execMode = {}) noexcept
      requires(EA::ENTITY_ID_STRATEGY != EntityIdStrategy::EntityKeyed && detail::HasCreateWithBoxSpaceV<TOrthoTreeCore>)
    : m_entities(geometryCollection.begin(), geometryCollection.end())
    {
#ifndef __cpp_lib_execution
      static_assert(!std::is_same_v<TExecMode, ExecutionTags::Parallel>, "Parallel creation is based on execution policies. __cpp_lib_execution is required.");
#endif
      TOrthoTreeCore::Create(m_tree, m_entities, maxDepthID, std::move(boxSpace), maxElementNoInNode, execMode);
    }

    // Constructor for any copyable container with runtime parallel parameter
    template<typename TExecMode = SeqExec>
    explicit OrthoTreeManaged(
      EntityContainer const& geometryCollection,
      std::optional<depth_t> maxDepthID = std::nullopt,
      std::optional<TBox> boxSpace = std::nullopt,
      std::size_t maxElementNoInNode = CONFIG::DEFAULT_TARGET_ELEMENT_NUM_IN_NODES,
      TExecMode execMode = {}) noexcept
      requires(detail::HasCreateWithBoxSpaceV<TOrthoTreeCore>)
    : m_entities(geometryCollection)
    {
#ifndef __cpp_lib_execution
      static_assert(!std::is_same_v<TExecMode, ExecutionTags::Parallel>, "Parallel creation is based on execution policies. __cpp_lib_execution is required.");
#endif
      TOrthoTreeCore::Create(m_tree, m_entities, maxDepthID, std::move(boxSpace), maxElementNoInNode, execMode);
    }

    // Constructor for any movable container with runtime parallel parameter
    template<typename TExecMode = SeqExec>
    explicit OrthoTreeManaged(
      EntityContainer&& geometryCollection,
      std::optional<depth_t> maxDepthID = std::nullopt,
      std::optional<TBox> boxSpace = std::nullopt,
      std::size_t maxElementNoInNode = CONFIG::DEFAULT_TARGET_ELEMENT_NUM_IN_NODES,
      TExecMode execMode = {}) noexcept
      requires(detail::HasCreateWithBoxSpaceV<TOrthoTreeCore>)
    : m_entities(std::move(geometryCollection))
    {
#ifndef __cpp_lib_execution
      static_assert(!std::is_same_v<TExecMode, ExecutionTags::Parallel>, "Parallel creation is based on execution policies. __cpp_lib_execution is required.");
#endif
      TOrthoTreeCore::Create(m_tree, m_entities, maxDepthID, std::move(boxSpace), maxElementNoInNode, execMode);
    }

    // Constructor for any contiguous container with compile-time parallel parameter
    template<typename TExecMode>
    explicit OrthoTreeManaged(
      TExecMode execMode,
      std::span<Entity const> const& geometryCollection,
      std::optional<depth_t> maxDepthID = std::nullopt,
      std::optional<TBox> boxSpace = std::nullopt,
      std::size_t maxElementNoInNode = CONFIG::DEFAULT_TARGET_ELEMENT_NUM_IN_NODES) noexcept
      requires(EA::ENTITY_ID_STRATEGY != EntityIdStrategy::EntityKeyed && detail::HasCreateWithBoxSpaceV<TOrthoTreeCore>)
    : m_entities(geometryCollection.begin(), geometryCollection.end())
    {
#ifdef __cpp_lib_execution
#else
      static_assert(!std::is_same_v<TExecMode, ExecutionTags::Parallel>, "Parallel creation is based on execution policies. __cpp_lib_execution is required.");
#endif
      TOrthoTreeCore::Create(m_tree, m_entities, maxDepthID, std::move(boxSpace), maxElementNoInNode, execMode);
    }

    // Constructor for any copyable container compile-time parallel parameter
    template<typename TExecMode>
    explicit OrthoTreeManaged(
      TExecMode execMode,
      EntityContainer const& geometryCollection,
      std::optional<depth_t> maxDepthID = std::nullopt,
      std::optional<TBox> boxSpace = std::nullopt,
      std::size_t maxElementNoInNode = CONFIG::DEFAULT_TARGET_ELEMENT_NUM_IN_NODES) noexcept
      requires(detail::HasCreateWithBoxSpaceV<TOrthoTreeCore>)
    : m_entities(geometryCollection)
    {
#ifdef __cpp_lib_execution
#else
      static_assert(!std::is_same_v<TExecMode, ExecutionTags::Parallel>, "Parallel creation is based on execution policies. __cpp_lib_execution is required.");
#endif
      TOrthoTreeCore::Create(m_tree, m_entities, maxDepthID, std::move(boxSpace), maxElementNoInNode, execMode);
    }

    // Constructor for any movable container with compile-time parallel parameter
    template<typename TExecMode>
    explicit OrthoTreeManaged(
      TExecMode execMode,
      EntityContainer&& geometryCollection,
      std::optional<depth_t> maxDepthID = std::nullopt,
      std::optional<TBox> boxSpace = std::nullopt,
      std::size_t maxElementNoInNode = CONFIG::DEFAULT_TARGET_ELEMENT_NUM_IN_NODES) noexcept
      requires(detail::HasCreateWithBoxSpaceV<TOrthoTreeCore>)
    : m_entities(std::move(geometryCollection))
    {
#ifdef __cpp_lib_execution
#else
      static_assert(!std::is_same_v<TExecMode, ExecutionTags::Parallel>, "Parallel creation is based on execution policies. __cpp_lib_execution is required.");
#endif
      TOrthoTreeCore::Create(m_tree, m_entities, maxDepthID, std::move(boxSpace), maxElementNoInNode, execMode);
    }

    template<typename TExecMode = SeqExec>
    static OrthoTreeManaged Create(
      std::span<Entity const> const& entities,
      std::optional<depth_t> maxDepthID = std::nullopt,
      std::optional<TBox> boxSpace = std::nullopt,
      std::size_t maxElementNoInNode = CONFIG::DEFAULT_TARGET_ELEMENT_NUM_IN_NODES,
      TExecMode execMode = {}) noexcept
      requires(EA::ENTITY_ID_STRATEGY != EntityIdStrategy::EntityKeyed && detail::HasCreateWithBoxSpaceV<TOrthoTreeCore>)
    {
      auto otc = OrthoTreeManaged();
      otc.m_entities = std::vector(entities.begin(), entities.end());
      TOrthoTreeCore::Create(otc.m_tree, otc.m_entities, maxDepthID, std::move(boxSpace), maxElementNoInNode, execMode);
      return otc;
    }

    template<typename TExecMode = SeqExec>
    static OrthoTreeManaged Create(
      EntityContainer const& entities,
      std::optional<depth_t> maxDepthID = std::nullopt,
      std::optional<TBox> boxSpace = std::nullopt,
      std::size_t maxElementNoInNode = CONFIG::DEFAULT_TARGET_ELEMENT_NUM_IN_NODES,
      TExecMode execMode = {}) noexcept
      requires(detail::HasCreateWithBoxSpaceV<TOrthoTreeCore>)
    {
      auto otc = OrthoTreeManaged();
      otc.m_entities = entities;
      TOrthoTreeCore::Create(otc.m_tree, otc.m_entities, maxDepthID, std::move(boxSpace), maxElementNoInNode, execMode);
      return otc;
    }

    template<typename TExecMode = SeqExec>
    static OrthoTreeManaged Create(
      EntityContainer&& entities,
      std::optional<depth_t> maxDepthID = std::nullopt,
      std::optional<TBox> boxSpace = std::nullopt,
      std::size_t maxElementNoInNode = CONFIG::DEFAULT_TARGET_ELEMENT_NUM_IN_NODES,
      TExecMode execMode = {}) noexcept
      requires(detail::HasCreateWithBoxSpaceV<TOrthoTreeCore>)
    {
      auto otc = OrthoTreeManaged();
      otc.m_entities = std::move(entities);
      TOrthoTreeCore::Create(otc.m_tree, otc.m_entities, maxDepthID, std::move(boxSpace), maxElementNoInNode, execMode);
      return otc;
    }

  public: // Member functions
    constexpr TOrthoTreeCore const& GetCore() const noexcept { return m_tree; }
    constexpr EntityContainer const& GetData() const noexcept { return m_entities; }

    constexpr TBox GetBox() const noexcept { return m_tree.GetBox(); }

    constexpr void Init(TBox const& boxSpace, depth_t maxDepthID, std::size_t maxElementNoInNode = CONFIG::DEFAULT_TARGET_ELEMENT_NUM_IN_NODES) noexcept
    {
      m_tree.Init(boxSpace, maxDepthID, maxElementNoInNode);
    }

    // Get entity by ID
    constexpr Entity const& Get(EntityID entityID) const noexcept { return EA::GetEntity(m_entities, entityID); }

    // Add entity with tree rebalancing
    template<typename TEntity = Entity>
    constexpr std::optional<EntityID> Add(TEntity&& newEntity, InsertionMode insertionMode = InsertionMode::Balanced) noexcept
      requires(std::is_convertible_v<TEntity, Entity>)
    {
      auto const newEntityID = EA::Insert(m_entities, std::forward<decltype(newEntity)>(newEntity));
      bool isInserted = false;
      switch (insertionMode)
      {
      case InsertionMode::Balanced: isInserted = m_tree.Insert(newEntityID, EA::GetGeometry(m_entities, newEntityID), m_entities); break;
      case InsertionMode::LowestLeaf:
      case InsertionMode::ExistingLeaf:
        isInserted = m_tree.InsertIntoLeaf(newEntityID, EA::GetGeometry(m_entities, newEntityID), insertionMode);
        break;
      }

      if (!isInserted)
      {
        EA::Erase(m_entities, newEntityID);
        return std::nullopt;
      }

      return newEntityID;
    }

    // Add entity with tree rebalancing if it is not yet added
    template<typename TEntity = Entity>
    constexpr std::optional<EntityID> AddUnique(
      TEntity&& newEntity, TFloatScalar tolerance = GA::BASE_TOLERANCE, InsertionMode insertionMode = InsertionMode::Balanced) noexcept
    {
      auto const newEntityID = EA::Insert(m_entities, std::forward<decltype(newEntity)>(newEntity));
      if (!m_tree.InsertUnique(newEntityID, EA::GetGeometry(m_entities, newEntityID), m_entities, tolerance, insertionMode))
      {
        EA::Erase(m_entities, newEntityID);
        return std::nullopt;
      }

      return newEntityID;
    }

    // Add multiple entities (Bulk Insertion)
    // Accepted entity ranges:
    // - TContainer<Entity>
    // Note:
    // - If the container is not keyed, if any entity is failed to insert, nothing will be inserted.
    // - The tree will not be rebalanced after insertion.
    template<typename TEntityRange, typename TExecMode = SeqExec>
    constexpr bool Add(TEntityRange&& newEntities, TExecMode execMode = {}) noexcept
      requires(requires { newEntities.size(); } && !std::is_same_v<std::remove_cvref_t<TEntityRange>, Entity>)
    {
      if (newEntities.empty())
        return true;

      auto failedEntities = std::unordered_set<EntityID>{};
      auto const isAllEntitiesInserted = m_tree.Insert(newEntities, m_entities, execMode, &failedEntities);
      if constexpr (EA::ENTITY_ID_STRATEGY != EntityIdStrategy::EntityKeyed)
      {
        // For non-keyed entities, we MUST insert everything to keep indices sync with tree IDs
        if (!isAllEntitiesInserted)
          return false;
      }

      // Now add to the physical container
      if constexpr (requires { m_entities.reserve(0); })
        m_entities.reserve(m_entities.size() + newEntities.size());

      for (auto&& entity : newEntities)
      {
        if constexpr (EA::ENTITY_ID_STRATEGY == EntityIdStrategy::EntityKeyed)
        {
          bool isFailed = std::erase(failedEntities, EA::GetEntityID(entity));
          if (isFailed)
            continue;
        }

        EA::Insert(m_entities, std::forward<decltype(entity)>(entity));
      }

      return isAllEntitiesInserted;
    }

    // Replace entity to the changedEntity
    template<typename TEntity = Entity>
    constexpr bool Update(EntityID entityID, TEntity&& changedEntity, InsertionMode insertionMode = InsertionMode::Balanced) noexcept
      requires(EA::ENTITY_ID_STRATEGY != EntityIdStrategy::EntityKeyed && std::is_convertible_v<TEntity, Entity>)
    {
      auto oldEntity = EA::Exchange(m_entities, entityID, std::forward<decltype(changedEntity)>(changedEntity));
      bool isInserted = false;
      switch (insertionMode)
      {
      case InsertionMode::Balanced:
        isInserted = m_tree.Update(entityID, EA::GetGeometry(oldEntity), EA::GetGeometry(m_entities, entityID), m_entities);
        break;

      case InsertionMode::ExistingLeaf:
      case InsertionMode::LowestLeaf:
        isInserted = m_tree.Update(entityID, EA::GetGeometry(oldEntity), EA::GetGeometry(m_entities, entityID), insertionMode);
      }

      return isInserted;
    }

    // Replace entity to the changedEntity
    template<typename TEntity = Entity>
    constexpr bool Update(TEntity&& changedEntity, InsertionMode insertionMode = InsertionMode::Balanced) noexcept
      requires(EA::ENTITY_ID_STRATEGY == EntityIdStrategy::EntityKeyed)
    {
      auto const entityID = EA::GetEntityID(changedEntity);
      auto oldEntity = EA::Exchange(m_entities, entityID, std::forward<decltype(changedEntity)>(changedEntity));

      bool isInserted = false;
      switch (insertionMode)
      {
      case InsertionMode::Balanced:
        isInserted = m_tree.Update(entityID, EA::GetGeometry(oldEntity), EA::GetGeometry(m_entities, entityID), m_entities);
        break;

      case InsertionMode::ExistingLeaf:
      case InsertionMode::LowestLeaf:
        isInserted = m_tree.Update(entityID, EA::GetGeometry(oldEntity), EA::GetGeometry(m_entities, entityID), insertionMode);
      }

      // failed core update: the original element is not in the tree anymore, therefore restoration would cause data inconsistency between the tree and m_entities.

      return isInserted;
    }

    // Erase entity by ID
    bool Erase(EntityID entityID) noexcept
    {
      if (!EA::Contains(m_entities, entityID))
        return false;

      if (!m_tree.Erase(entityID, EA::GetGeometry(m_entities, entityID)))
        return false;

      EA::Erase(m_entities, entityID);
      return true;
    }

    // Clear the tree: remove all node and entity.
    void Clear() noexcept
    {
      m_tree.Clear();
      EA::Clear(m_entities);
    }

    // Reset the tree: Same as clear but also reset the handled domain
    void Reset() noexcept
    {
      m_tree.Reset();
      EA::Clear(m_entities);
    }

    template<typename TExecMode = SeqExec>
    void Move(TVector const& moveVector, TExecMode execMode = SEQ_EXEC) noexcept
    {
      m_tree.Move(moveVector, execMode);

      EXEC_POL_DEF(ep); // GCC 11.3
      std::for_each(EXEC_POL_ADD(ep) m_entities.begin(), m_entities.end(), [&moveVector](EA::Entity& entity) {
        if constexpr (EA::GEOMETRY_TYPE == GeometryType::Point)
        {
          EA::SetGeometry(entity, GA::Add(EA::GetGeometry(entity), moveVector));
        }
        else
        {
          auto box = EA::GetGeometry(entity);
          GA::MoveBox(box, moveVector);
          EA::SetGeometry(entity, box);
        }
      });
    }

    // Collect all entity ID in breadth-first traverse order
    std::vector<EntityID> GetEntitiesBreadthFirst() const noexcept { return m_tree.GetEntitiesBreadthFirst(); }

    // Collect all entity ID in depth-first traverse order
    std::vector<EntityID> GetEntitiesDepthFirst() const noexcept { return m_tree.GetEntitiesDepthFirst(); }

    // Visit entities in breadth first order
    template<typename TProcedure>
    constexpr void TraverseEntitiesBreadthFirst(TProcedure&& procedure) const noexcept
    {
      m_tree.TraverseEntitiesBreadthFirst(std::forward<TProcedure>(procedure));
    }

    // Visit entities in depth first order
    template<typename TProcedure>
    constexpr void TraverseEntitiesDepthFirst(TProcedure&& procedure) const noexcept
    {
      m_tree.TraverseEntitiesDepthFirst(std::forward<TProcedure>(procedure));
    }

    // Visit entities in priority order
    template<typename TProcedure, typename TPriorityCalculator>
    constexpr void TraverseEntitiesByPriority(TProcedure&& procedure, TPriorityCalculator&& priorityCalculator) const noexcept
    {
      m_tree.TraverseEntitiesByPriority(std::forward<TProcedure>(procedure), std::forward<TPriorityCalculator>(priorityCalculator));
    }

    // Pick search
    //
    // Accepted tester signatures (same as OrthoTreeQueryBase::PickSearch):
    // * bool(EntityID)
    // * bool(EntityID, TVector&)
    // * bool(Entity)
    // * bool(Entity, TVector&)
    template<typename TTester = std::monostate>
    std::vector<EntityID> PickSearch(TVector const& pickPoint, TFloatScalar tolerance = GA::BASE_TOLERANCE, TTester&& tester = {}) const noexcept
    {
      return m_tree.PickSearch(pickPoint, m_entities, tolerance, std::forward<TTester>(tester));
    }

    // Range search
    //
    // Accepted tester signatures (same as OrthoTreeQueryBase::RangeSearch):
    // * bool(EntityID)
    // * bool(EntityID, TVector&)
    // * bool(Entity)
    // * bool(Entity, TVector&)
    template<typename TTester = std::monostate>
    std::vector<EntityID> RangeSearch(
      TBox const& range, RangeSearchMode rangeSearchMode = RangeSearchMode::Inside, TFloatScalar tolerance = GA::BASE_TOLERANCE, TTester&& tester = {}) const noexcept
    {
      return m_tree.RangeSearch(range, m_entities, rangeSearchMode, tolerance, std::forward<TTester>(tester));
    }

    // Hyperplane segmentation, get all elements in positive side (Plane equation: dotProduct(planeNormal, point) = distanceOfOrigo)
    //
    // Accepted tester signatures (same as OrthoTreeQueryBase::PlanePositiveSegmentation):
    // * bool(EntityID)
    // * bool(EntityID, TScalar, TVector)
    // * bool(Entity)
    // * bool(Entity, TScalar, TVector)
    template<typename TTester = std::monostate>
    std::vector<EntityID> PlanePositiveSegmentation(
      TScalar distanceOfOrigo, TVector const& planeNormal, TFloatScalar tolerance = GA::BASE_TOLERANCE, TTester&& tester = {}) const noexcept
    {
      return m_tree.PlanePositiveSegmentation(distanceOfOrigo, planeNormal, m_entities, tolerance, std::forward<TTester>(tester));
    }

    // Hyperplane segmentation, get all elements in positive side (Plane equation: dotProduct(planeNormal, point) = distanceOfOrigo)
    //
    // Accepted tester signatures (same as OrthoTreeQueryBase::PlanePositiveSegmentation):
    // * bool(EntityID)
    // * bool(EntityID, TScalar, TVector)
    // * bool(Entity)
    // * bool(Entity, TScalar, TVector)
    template<typename TTester = std::monostate>
    std::vector<EntityID> PlanePositiveSegmentation(TPlane const& plane, TFloatScalar tolerance = GA::BASE_TOLERANCE, TTester&& tester = {}) const noexcept
    {
      return m_tree.PlanePositiveSegmentation(plane, m_entities, tolerance, std::forward<TTester>(tester));
    }

    // Hyperplane segmentation, get all elements in positive side (Plane equation: dotProduct(planeNormal, point) = distanceOfOrigo)
    //
    // Accepted tester signatures (same as OrthoTreeQueryBase::FrustumCulling):
    // * bool(EntityID)
    // * bool(EntityID, TScalar, TVector)
    // * bool(Entity)
    // * bool(Entity, TScalar, TVector)
    template<typename TTester = std::monostate>
    std::vector<EntityID> FrustumCulling(
      std::span<TPlane const> const& boundaryPlanes, TFloatScalar tolerance = GA::BASE_TOLERANCE, TTester&& tester = {}) const noexcept
    {
      return m_tree.FrustumCulling(boundaryPlanes, m_entities, tolerance, std::forward<TTester>(tester));
    }

    using FrustumCondition = TOrthoTreeCore::FrustumCondition;
    using RangeCondition = TOrthoTreeCore::RangeCondition;
    using PlaneIntersectionCondition = TOrthoTreeCore::PlaneIntersectionCondition;
    using EntityIDCondition = TOrthoTreeCore::EntityIDCondition;
    using EntityCondition = TOrthoTreeCore::EntityCondition;

    static constexpr QueryCondition ByWithin(TBox const& range) noexcept { return TOrthoTreeCore::ByWithin(range); }
    static constexpr QueryCondition ByOverlaps(TBox const& range) noexcept { return TOrthoTreeCore::ByOverlaps(range); }
    static constexpr QueryCondition ByInFrustum(std::vector<std::pair<TPlane, bool>> boundaryPlanes) noexcept
    {
      return TOrthoTreeCore::ByInFrustum(std::move(boundaryPlanes));
    }
    static constexpr QueryCondition ByIntersecting(TPlane const& plane, TFloatScalar tolerance = GA::BASE_TOLERANCE) noexcept
    {
      return TOrthoTreeCore::ByIntersecting(plane, tolerance);
    }
    static constexpr QueryCondition BySatisfies(EntityIDCondition condition) noexcept { return TOrthoTreeCore::BySatisfies(std::move(condition)); }
    static constexpr QueryCondition BySatisfies(EntityCondition condition) noexcept { return TOrthoTreeCore::BySatisfies(std::move(condition)); }

    // Complex query with multiple conditions. The conditions are combined with logical AND by default, but can be switched to OR by template parameter.
    // See the By* functions and the `FrustumCondition`/ `RangeCondition` / `PlaneIntersectionCondition`/ `EntityIDCondition`/ `EntityCondition` for the accepted condition signatures.
    template<LogicalOperator OP = LogicalOperator::And>
    std::vector<EntityID> Query(auto const& conditions, TFloatScalar tolerance = GA::BASE_TOLERANCE) const noexcept
    {
      return m_tree.template Query<OP>(conditions, m_entities, tolerance);
    }

    // K Nearest Neighbor
    //
    // Accepted entityDistanceFn signatures (same as OrthoTreeQueryBase::GetNearestNeighbors):
    // * bool(EntityID)
    // * bool(EntityID, TVector)
    // * bool(Entity)
    // * bool(Entity, TVector)
    // * std::optional<TScalar>(EntityID)
    // * std::optional<TScalar>(EntityID, TVector)
    // * std::optional<TScalar>(Entity)
    // * std::optional<TScalar>(Entity, TVector)
    template<bool SHOULD_SORT_ENTITIES_BY_DISTANCE = true, typename TTester = std::monostate>
    std::vector<EntityID> GetNearestNeighbors(
      TVector const& pt,
      std::size_t k,
      TScalar maxDistanceWithin = std::numeric_limits<TScalar>::max(),
      TFloatScalar tolerance = GA::BASE_TOLERANCE,
      TTester&& entityDistanceFn = {}) const noexcept
    {
      return m_tree.template GetNearestNeighbors<SHOULD_SORT_ENTITIES_BY_DISTANCE>(
        pt, k, maxDistanceWithin, m_entities, tolerance, std::forward<TTester>(entityDistanceFn));
    }

  public: // Collision detection
    using FCollisionDetector = typename TOrthoTreeCore::FCollisionDetector;

    // Collision detection between the contained elements
    template<typename TExecMode = SeqExec>
    std::vector<std::pair<EntityID, EntityID>> CollisionDetection(
      TFloatScalar tolerance = GA::BASE_TOLERANCE,
      std::optional<FCollisionDetector> const& collisionDetector = std::nullopt,
      TExecMode execMode = {}) const noexcept
    {
      if (collisionDetector)
        return m_tree.CollisionDetection(m_entities, FCollisionDetector(*collisionDetector), tolerance, execMode);
      else
        return m_tree.template CollisionDetection<TExecMode>(m_entities, tolerance);
    }

    // Collision detection with another tree
    std::vector<std::pair<EntityID, EntityID>> CollisionDetection(
      OrthoTreeManaged const& otherTree,
      TFloatScalar tolerance = GA::BASE_TOLERANCE,
      std::optional<FCollisionDetector> const& collisionDetector = std::nullopt) const noexcept
    {
      return m_tree.CollisionDetection(m_tree, m_entities, otherTree.m_tree, otherTree.m_entities, tolerance, collisionDetector);
    }

    // Collision detection between trees
    static std::vector<std::pair<EntityID, EntityID>> CollisionDetection(
      OrthoTreeManaged const& leftTree,
      OrthoTreeManaged const& rightTree,
      TFloatScalar tolerance = GA::BASE_TOLERANCE,
      std::optional<FCollisionDetector> const& collisionDetector = std::nullopt) noexcept
    {
      return leftTree.CollisionDetection(rightTree, tolerance, collisionDetector);
    }


  public: // Ray intersection
    // Get all entities that are intersected by the ray in order
    //
    // Accepted entity ray-hit tester signatures (same as OrthoTreeQueryBase::RayIntersectedAll/First):
    // * bool(EntityID)
    // * bool(EntityID, TVector rayBasePoint, TVector rayHeading)
    // * bool(Entity)
    // * bool(Entity, TVector rayBasePoint, TVector rayHeading)
    // * std::optional<TFloatScalar>(EntityID)
    // * std::optional<TFloatScalar>(EntityID, TVector rayBasePoint, TVector rayHeading)
    // * std::optional<TFloatScalar>(Entity)
    // * std::optional<TFloatScalar>(Entity, TVector rayBasePoint, TVector rayHeading)
    template<bool SHOULD_SORT_ENTITIES_BY_DISTANCE = true, typename TEntityRayHitTester = std::monostate>
    std::vector<EntityID> RayIntersectedAll(
      TVector const& rayBasePoint,
      TVector const& rayHeading,
      TFloatScalar tolerance = GA::BASE_TOLERANCE,
      TFloatScalar toleranceIncrement = {},
      TScalar maxDistance = std::numeric_limits<TScalar>::max(),
      TEntityRayHitTester&& entityHitTester = {}) const noexcept
    {
      return m_tree.template RayIntersectedAll<SHOULD_SORT_ENTITIES_BY_DISTANCE>(
        rayBasePoint, rayHeading, m_entities, tolerance, toleranceIncrement, maxDistance, std::forward<TEntityRayHitTester>(entityHitTester));
    }

    // Get all entities that are intersected by the ray in order
    //
    // Accepted entity ray-hit tester signatures (same as OrthoTreeQueryBase::RayIntersectedAll/First):
    // * bool(EntityID)
    // * bool(EntityID, TVector rayBasePoint, TVector rayHeading)
    // * bool(Entity)
    // * bool(Entity, TVector rayBasePoint, TVector rayHeading)
    // * std::optional<TFloatScalar>(EntityID)
    // * std::optional<TFloatScalar>(EntityID, TVector rayBasePoint, TVector rayHeading)
    // * std::optional<TFloatScalar>(Entity)
    // * std::optional<TFloatScalar>(Entity, TVector rayBasePoint, TVector rayHeading)
    template<bool SHOULD_SORT_ENTITIES_BY_DISTANCE = true, typename TEntityRayHitTester = std::monostate>
    std::vector<EntityID> RayIntersectedAll(
      TRay const& ray,
      TFloatScalar tolerance = GA::BASE_TOLERANCE,
      TFloatScalar toleranceIncrement = {},
      TScalar maxDistance = std::numeric_limits<TScalar>::max(),
      TEntityRayHitTester&& entityHitTester = {}) const noexcept
    {
      return m_tree.template RayIntersectedAll<SHOULD_SORT_ENTITIES_BY_DISTANCE>(
        GA::GetRayOrigin(ray),
        GA::GetRayDirection(ray),
        m_entities,
        tolerance,
        toleranceIncrement,
        maxDistance,
        std::forward<TEntityRayHitTester>(entityHitTester));
    }

    // Get first entities that hit by the ray
    //
    // Accepted entity ray-hit tester signatures (same as OrthoTreeQueryBase::RayIntersectedAll/First):
    // * bool(EntityID)
    // * bool(EntityID, TVector rayBasePoint, TVector rayHeading)
    // * bool(Entity)
    // * bool(Entity, TVector rayBasePoint, TVector rayHeading)
    // * std::optional<TFloatScalar>(EntityID)
    // * std::optional<TFloatScalar>(EntityID, TVector rayBasePoint, TVector rayHeading)
    // * std::optional<TFloatScalar>(Entity)
    // * std::optional<TFloatScalar>(Entity, TVector rayBasePoint, TVector rayHeading)
    template<typename TEntityRayHitTester = std::monostate>
    std::vector<EntityID> RayIntersectedFirst(
      TVector const& rayBasePoint,
      TVector const& rayHeading,
      TFloatScalar tolerance = GA::BASE_TOLERANCE,
      TFloatScalar toleranceIncrement = {},
      TScalar maxDistance = std::numeric_limits<TScalar>::max(),
      TEntityRayHitTester&& entityHitTester = {}) const noexcept
    {
      return m_tree.RayIntersectedFirst(
        rayBasePoint, rayHeading, m_entities, tolerance, toleranceIncrement, maxDistance, std::forward<TEntityRayHitTester>(entityHitTester));
    }

    // Get first entities that hit by the ray
    //
    // Accepted entity ray-hit tester signatures (same as OrthoTreeQueryBase::RayIntersectedAll/First):
    // * bool(EntityID)
    // * bool(EntityID, TVector rayBasePoint, TVector rayHeading)
    // * bool(Entity)
    // * bool(Entity, TVector rayBasePoint, TVector rayHeading)
    // * std::optional<TFloatScalar>(EntityID)
    // * std::optional<TFloatScalar>(EntityID, TVector rayBasePoint, TVector rayHeading)
    // * std::optional<TFloatScalar>(Entity)
    // * std::optional<TFloatScalar>(Entity, TVector rayBasePoint, TVector rayHeading)
    template<typename TEntityRayHitTester = std::monostate>
    std::vector<EntityID> RayIntersectedFirst(
      TRay const& ray,
      TFloatScalar tolerance = GA::BASE_TOLERANCE,
      TFloatScalar toleranceIncrement = {},
      TScalar maxDistance = std::numeric_limits<TScalar>::max(),
      TEntityRayHitTester&& entityHitTester = {}) const noexcept
    {
      return m_tree.RayIntersectedFirst(
        GA::GetRayOrigin(ray),
        GA::GetRayDirection(ray),
        m_entities,
        tolerance,
        toleranceIncrement,
        maxDistance,
        std::forward<TEntityRayHitTester>(entityHitTester));
    }

  public: // Plane
    // Hyperplane intersection (Plane equation: dotProduct(planeNormal, point) = distanceOfOrigo)
    //
    // Accepted tester signatures (same as OrthoTreeQueryBase::PlaneSearch):
    // * bool(EntityID)
    // * bool(EntityID, TScalar, TVector)
    // * bool(Entity)
    // * bool(Entity, TScalar, TVector)
    template<typename TTester = std::monostate>
    std::vector<EntityID> PlaneSearch(
      TScalar distanceOfOrigo, TVector const& planeNormal, TFloatScalar tolerance = GA::BASE_TOLERANCE, TTester&& tester = {}) const noexcept
    {
      return m_tree.PlaneSearch(distanceOfOrigo, planeNormal, m_entities, tolerance, std::forward<TTester>(tester));
    }

    // Hyperplane intersection using built-in plane
    //
    // Accepted tester signatures (same as OrthoTreeQueryBase::PlaneSearch):
    // * bool(EntityID)
    // * bool(EntityID, TScalar, TVector)
    // * bool(Entity)
    // * bool(Entity, TScalar, TVector)
    template<typename TTester = std::monostate>
    std::vector<EntityID> PlaneSearch(TPlane const& plane, TFloatScalar tolerance = GA::BASE_TOLERANCE, TTester&& tester = {}) const noexcept
    {
      return m_tree.PlaneSearch(plane, m_entities, tolerance, std::forward<TTester>(tester));
    }

    // Hyperplane intersection using built-in plane
    //
    // Accepted tester signatures (same as OrthoTreeQueryBase::PlaneSearch):
    // * bool(EntityID)
    // * bool(EntityID, TScalar, TVector)
    // * bool(Entity)
    // * bool(Entity, TScalar, TVector)
    template<typename TTester = std::monostate>
    std::vector<EntityID> PlaneIntersection(
      TScalar distanceOfOrigo, TVector const& planeNormal, TFloatScalar tolerance = GA::BASE_TOLERANCE, TTester&& tester = {}) const noexcept
    {
      return m_tree.PlaneSearch(distanceOfOrigo, planeNormal, m_entities, tolerance, std::forward<TTester>(tester));
    }

    // Hyperplane intersection using built-in plane
    //
    // Accepted tester signatures (same as OrthoTreeQueryBase::PlaneSearch):
    // * bool(EntityID)
    // * bool(EntityID, TScalar, TVector)
    // * bool(Entity)
    // * bool(Entity, TScalar, TVector)
    template<typename TTester = std::monostate>
    std::vector<EntityID> PlaneIntersection(TPlane const& plane, TFloatScalar tolerance = GA::BASE_TOLERANCE, TTester&& tester = {}) const noexcept
    {
      return m_tree.PlaneSearch(plane, m_entities, tolerance, std::forward<TTester>(tester));
    }
  };
} // namespace OrthoTree


namespace OrthoTree
{
  template<typename TEntityAdapter, typename TGeometryAdapter, typename TConfiguration>
  using DynamicOrthoTreeBase = OrthoTreeQueryBase<DynamicHashOrthoTreeCore<TEntityAdapter, TGeometryAdapter, TConfiguration>>;

  template<typename TEntityAdapter, typename TGeometryAdapter, typename TConfiguration>
  using StaticOrthoTreeBase = OrthoTreeQueryBase<StaticLinearOrthoTreeCore<TEntityAdapter, TGeometryAdapter, TConfiguration>>;

  template<typename TEntityAdapter, typename TGeometryAdapter, typename TConfiguration>
  using OrthoTreeBase = DynamicOrthoTreeBase<TEntityAdapter, TGeometryAdapter, TConfiguration>;

  // Tree aliases

  template<dim_t DIMENSION_NO, typename TScalar = BaseGeometryType, bool IS_CONTIOGUOS_CONTAINER = true, NodeGeometryStorage NODE_GEOMETRY_STORAGE = NodeGeometryStorage::MinPoint>
  using OrthoTreePointND = OrthoTree::OrthoTreeBase<
    std::conditional_t<IS_CONTIOGUOS_CONTAINER, PointEntitySpanAdapter<PointND<DIMENSION_NO, TScalar>>, PointEntityMapAdapter<PointND<DIMENSION_NO, TScalar>>>,
    GeneralGeometryAdapterND<DIMENSION_NO, TScalar>,
    PointConfiguration<NODE_GEOMETRY_STORAGE>>;

  template<dim_t DIMENSION_NO, bool IS_LOOSE_TREE = true, typename TScalar = BaseGeometryType, bool IS_CONTIOGUOS_CONTAINER = true, NodeGeometryStorage NODE_GEOMETRY_STORAGE = NodeGeometryStorage::MBR>
  using OrthoTreeBoxND = OrthoTree::OrthoTreeBase<
    std::conditional_t<IS_CONTIOGUOS_CONTAINER, BoxEntitySpanAdapter<BoundingBoxND<DIMENSION_NO, TScalar>>, BoxEntityMapAdapter<BoundingBoxND<DIMENSION_NO, TScalar>>>,
    GeneralGeometryAdapterND<DIMENSION_NO, TScalar>,
    BoxConfiguration<IS_LOOSE_TREE, NODE_GEOMETRY_STORAGE>>;

  template<dim_t DIMENSION_NO, typename TScalar, typename TEntityContainer, NodeGeometryStorage NODE_GEOMETRY_STORAGE = NodeGeometryStorage::MinPoint>
  using OrthoTreePointNDUD = OrthoTree::OrthoTreeBase<
    PointEntityMapAdapter<PointND<DIMENSION_NO, TScalar>, TEntityContainer>,
    GeneralGeometryAdapterND<DIMENSION_NO, TScalar>,
    PointConfiguration<NODE_GEOMETRY_STORAGE>>;

  template<dim_t DIMENSION_NO, bool IS_LOOSE_TREE, typename TScalar, typename TEntityContainer, NodeGeometryStorage NODE_GEOMETRY_STORAGE = NodeGeometryStorage::MBR>
  using OrthoTreeBoxNDUD = OrthoTree::OrthoTreeBase<
    BoxEntityMapAdapter<BoundingBoxND<DIMENSION_NO, TScalar>, TEntityContainer>,
    GeneralGeometryAdapterND<DIMENSION_NO, TScalar>,
    BoxConfiguration<IS_LOOSE_TREE, NODE_GEOMETRY_STORAGE>>;

  template<dim_t DIMENSION_NO, typename TScalar = BaseGeometryType, bool IS_CONTIOGUOS_CONTAINER = true, NodeGeometryStorage NODE_GEOMETRY_STORAGE = NodeGeometryStorage::MinPoint>
  using StaticOrthoTreePointND = OrthoTree::StaticOrthoTreeBase<
    std::conditional_t<IS_CONTIOGUOS_CONTAINER, PointEntitySpanAdapter<PointND<DIMENSION_NO, TScalar>>, PointEntityMapAdapter<PointND<DIMENSION_NO, TScalar>>>,
    GeneralGeometryAdapterND<DIMENSION_NO, TScalar>,
    PointConfiguration<NODE_GEOMETRY_STORAGE>>;

  template<dim_t DIMENSION_NO, bool IS_LOOSE_TREE = true, typename TScalar = BaseGeometryType, bool IS_CONTIOGUOS_CONTAINER = true, NodeGeometryStorage NODE_GEOMETRY_STORAGE = NodeGeometryStorage::MBR>
  using StaticOrthoTreeBoxND = OrthoTree::StaticOrthoTreeBase<
    std::conditional_t<IS_CONTIOGUOS_CONTAINER, BoxEntitySpanAdapter<BoundingBoxND<DIMENSION_NO, TScalar>>, BoxEntityMapAdapter<BoundingBoxND<DIMENSION_NO, TScalar>>>,
    GeneralGeometryAdapterND<DIMENSION_NO, TScalar>,
    BoxConfiguration<IS_LOOSE_TREE, NODE_GEOMETRY_STORAGE>>;

  template<dim_t DIMENSION_NO, typename TScalar, typename TEntityContainer, NodeGeometryStorage NODE_GEOMETRY_STORAGE = NodeGeometryStorage::MinPoint>
  using StaticOrthoTreePointNDUD = OrthoTree::StaticOrthoTreeBase<
    PointEntityMapAdapter<PointND<DIMENSION_NO, TScalar>, TEntityContainer>,
    GeneralGeometryAdapterND<DIMENSION_NO, TScalar>,
    PointConfiguration<NODE_GEOMETRY_STORAGE>>;

  template<dim_t DIMENSION_NO, bool IS_LOOSE_TREE, typename TScalar, typename TEntityContainer, NodeGeometryStorage NODE_GEOMETRY_STORAGE = NodeGeometryStorage::MBR>
  using StaticOrthoTreeBoxNDUD = OrthoTree::StaticOrthoTreeBase<
    BoxEntityMapAdapter<BoundingBoxND<DIMENSION_NO, TScalar>, TEntityContainer>,
    GeneralGeometryAdapterND<DIMENSION_NO, TScalar>,
    BoxConfiguration<IS_LOOSE_TREE, NODE_GEOMETRY_STORAGE>>;


  // Dualtree for points
  using DualtreePoint = OrthoTreePointND<1, BaseGeometryType>;

  // Dualtree for bounding boxes
  using DualtreeBox = OrthoTreeBoxND<1, true, BaseGeometryType>;

  // Quadtree for points
  using QuadtreePoint = OrthoTreePointND<2, BaseGeometryType>;

  // Quadtree for bounding boxes
  using QuadtreeBox = OrthoTreeBoxND<2, true, BaseGeometryType>;

  // Octree for points
  using OctreePoint = OrthoTreePointND<3, BaseGeometryType>;

  // Octree for bounding boxes
  using OctreeBox = OrthoTreeBoxND<3, true, BaseGeometryType>;

  // Hexatree for points
  using HexatreePoint = OrthoTreePointND<4, BaseGeometryType>;

  // Hexatree for bounding boxes
  using HexatreeBox = OrthoTreeBoxND<4, true, BaseGeometryType>;

  // OrthoTrees for higher dimensions
  using TreePoint16D = OrthoTreePointND<16, BaseGeometryType>;
  using TreeBox16D = OrthoTreeBoxND<16, true, BaseGeometryType>;


  // Dualtree for bounding boxes with split-depth settings
  template<bool IS_LOOSE_TREE>
  using DualtreeBoxs = OrthoTreeBoxND<1, IS_LOOSE_TREE, BaseGeometryType>;

  // Quadtree for bounding boxes with split-depth settings
  template<bool IS_LOOSE_TREE>
  using QuadtreeBoxs = OrthoTreeBoxND<2, IS_LOOSE_TREE, BaseGeometryType>;

  // Octree for bounding boxes with split-depth settings
  template<bool IS_LOOSE_TREE>
  using OctreeBoxs = OrthoTreeBoxND<3, IS_LOOSE_TREE, BaseGeometryType>;

  // Hexatree for bounding boxes with split-depth settings
  template<bool IS_LOOSE_TREE>
  using HexatreeBoxs = OrthoTreeBoxND<4, IS_LOOSE_TREE, BaseGeometryType>;

  // OrthoTrees for higher dimensions with split-depth settings
  template<bool IS_LOOSE_TREE>
  using TreeBox16Ds = OrthoTreeBoxND<16, IS_LOOSE_TREE, BaseGeometryType>;


  // OrthoTrees with std::unordered_map

  // std::unordered_map-based Quadtree for points
  using QuadtreePointMap = OrthoTreePointND<2, BaseGeometryType, false>;

  // std::unordered_map-based Octree for points
  using OctreePointMap = OrthoTreePointND<3, BaseGeometryType, false>;

  // std::unordered_map-based Octree for bounding boxes
  using QuadreeBoxMap = OrthoTreeBoxND<2, true, BaseGeometryType, false>;

  // std::unordered_map-based Octree for bounding boxes
  using OctreeBoxMap = OrthoTreeBoxND<3, true, BaseGeometryType, false>;

  // std::unordered_map-based Quadtree for bounding boxes with split-depth settings
  template<bool IS_LOOSE_TREE>
  using QuadtreeBoxsMap = OrthoTreeBoxND<2, IS_LOOSE_TREE, BaseGeometryType, false>;
  using QuadtreeBoxMap = OrthoTreeBoxND<2, true, BaseGeometryType, false>;

  // std::unordered_map-based Octree for bounding boxes with split-depth settings
  template<bool IS_LOOSE_TREE>
  using OctreeBoxsMap = OrthoTreeBoxND<3, IS_LOOSE_TREE, BaseGeometryType, false>;
  using OctreeBoxMap = OrthoTreeBoxND<3, true, BaseGeometryType, false>;


  template<
    dim_t DIMENSION_NO,
    typename TScalar = BaseGeometryType,
    typename TEntityAdapter = PointEntitySpanAdapter<PointND<DIMENSION_NO, TScalar>>,
    NodeGeometryStorage NODE_GEOMETRY_STORAGE = NodeGeometryStorage::MinPoint>
  using TreePointEAND =
    OrthoTree::OrthoTreeBase<TEntityAdapter, GeneralGeometryAdapterND<DIMENSION_NO, TScalar>, PointConfiguration<NODE_GEOMETRY_STORAGE>>;

  template<
    dim_t DIMENSION_NO,
    bool IS_LOOSE_TREE = true,
    typename TScalar = BaseGeometryType,
    typename TEntityAdapter = BoxEntitySpanAdapter<BoundingBoxND<DIMENSION_NO, TScalar>>,
    NodeGeometryStorage NODE_GEOMETRY_STORAGE = NodeGeometryStorage::MBR>
  using TreeBoxEAND =
    OrthoTree::OrthoTreeBase<TEntityAdapter, GeneralGeometryAdapterND<DIMENSION_NO, TScalar>, BoxConfiguration<IS_LOOSE_TREE, NODE_GEOMETRY_STORAGE>>;


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


  // Dualtree for points
  using StaticDualtreePoint = StaticOrthoTreePointND<1, BaseGeometryType>;

  // Dualtree for bounding boxes
  using StaticDualtreeBox = StaticOrthoTreeBoxND<1, true, BaseGeometryType>;

  // Quadtree for points
  using StaticQuadtreePoint = StaticOrthoTreePointND<2, BaseGeometryType>;

  // Quadtree for bounding boxes
  using StaticQuadtreeBox = StaticOrthoTreeBoxND<2, true, BaseGeometryType>;

  // Octree for points
  using StaticOctreePoint = StaticOrthoTreePointND<3, BaseGeometryType>;

  // Octree for bounding boxes
  using StaticOctreeBox = StaticOrthoTreeBoxND<3, true, BaseGeometryType>;

  // Hexatree for points
  using StaticHexatreePoint = StaticOrthoTreePointND<4, BaseGeometryType>;

  // Hexatree for bounding boxes
  using StaticHexatreeBox = StaticOrthoTreeBoxND<4, true, BaseGeometryType>;


  // Dualtree for bounding boxes with split-depth settings
  template<bool IS_LOOSE_TREE>
  using StaticDualtreeBoxs = StaticOrthoTreeBoxND<1, IS_LOOSE_TREE, BaseGeometryType>;

  // Quadtree for bounding boxes with split-depth settings
  template<bool IS_LOOSE_TREE>
  using StaticQuadtreeBoxs = StaticOrthoTreeBoxND<2, IS_LOOSE_TREE, BaseGeometryType>;

  // Octree for bounding boxes with split-depth settings
  template<bool IS_LOOSE_TREE>
  using StaticOctreeBoxs = StaticOrthoTreeBoxND<3, IS_LOOSE_TREE, BaseGeometryType>;

  // Hexatree for bounding boxes with split-depth settings
  template<bool IS_LOOSE_TREE>
  using StaticHexatreeBoxs = StaticOrthoTreeBoxND<4, IS_LOOSE_TREE, BaseGeometryType>;


  // OrthoTrees with std::unordered_map

  // std::unordered_map-based Quadtree for points
  using StaticQuadtreePointMap = StaticOrthoTreePointND<2, BaseGeometryType, false>;

  // std::unordered_map-based Octree for points
  using StaticOctreePointMap = StaticOrthoTreePointND<3, BaseGeometryType, false>;

  // std::unordered_map-based Octree for bounding boxes
  using StaticQuadreeBoxMap = StaticOrthoTreeBoxND<2, true, BaseGeometryType, false>;

  // std::unordered_map-based Octree for bounding boxes
  using StaticOctreeBoxMap = StaticOrthoTreeBoxND<3, true, BaseGeometryType, false>;

  // std::unordered_map-based Quadtree for bounding boxes with split-depth settings
  template<bool IS_LOOSE_TREE>
  using StaticQuadtreeBoxsMap = StaticOrthoTreeBoxND<2, IS_LOOSE_TREE, BaseGeometryType, false>;
  using StaticQuadtreeBoxMap = StaticOrthoTreeBoxND<2, true, BaseGeometryType, false>;

  // std::unordered_map-based Octree for bounding boxes with split-depth settings
  template<bool IS_LOOSE_TREE>
  using StaticOctreeBoxsMap = StaticOrthoTreeBoxND<3, IS_LOOSE_TREE, BaseGeometryType, false>;
  using StaticOctreeBoxMap = StaticOrthoTreeBoxND<3, true, BaseGeometryType, false>;


  template<
    dim_t DIMENSION_NO,
    typename TScalar = BaseGeometryType,
    typename TEntityAdapter = PointEntitySpanAdapter<PointND<DIMENSION_NO, TScalar>>,
    NodeGeometryStorage NODE_GEOMETRY_STORAGE = NodeGeometryStorage::MinPoint>
  using StaticTreePointEAND =
    OrthoTree::StaticOrthoTreeBase<TEntityAdapter, GeneralGeometryAdapterND<DIMENSION_NO, TScalar>, PointConfiguration<NODE_GEOMETRY_STORAGE>>;

  template<
    dim_t DIMENSION_NO,
    bool IS_LOOSE_TREE = true,
    typename TScalar = BaseGeometryType,
    typename TEntityAdapter = BoxEntitySpanAdapter<BoundingBoxND<DIMENSION_NO, TScalar>>,
    NodeGeometryStorage NODE_GEOMETRY_STORAGE = NodeGeometryStorage::MBR>
  using StaticTreeBoxEAND =
    OrthoTree::StaticOrthoTreeBase<TEntityAdapter, GeneralGeometryAdapterND<DIMENSION_NO, TScalar>, BoxConfiguration<IS_LOOSE_TREE, NODE_GEOMETRY_STORAGE>>;


  // User-defined container-based Quadtree for points
  template<typename EntityAdapter>
  using StaticQuadtreePointUDMap = StaticTreePointEAND<2, BaseGeometryType, EntityAdapter>;

  // User-defined container-based Octree for points
  template<typename EntityAdapter>
  using StaticOctreePointUDMap = StaticTreePointEAND<3, BaseGeometryType, EntityAdapter>;

  // User-defined container-based Octree for bounding boxes
  template<typename EntityAdapter>
  using StaticQuadreeBoxUDMap = StaticTreeBoxEAND<2, true, BaseGeometryType, EntityAdapter>;

  // User-defined container-based Octree for bounding boxes
  template<typename EntityAdapter>
  using StaticOctreeBoxUDMap = StaticTreeBoxEAND<3, true, BaseGeometryType, EntityAdapter>;

  // User-defined container-based Quadtree for bounding boxes with split-depth settings
  template<bool IS_LOOSE_TREE, typename EntityAdapter>
  using StaticQuadtreeBoxsUDMap = StaticTreeBoxEAND<2, IS_LOOSE_TREE, BaseGeometryType, EntityAdapter>;

  // User-defined container-based Octree for bounding boxes with split-depth settings
  template<bool IS_LOOSE_TREE, typename EntityAdapter>
  using StaticOctreeBoxsUDMap = StaticTreeBoxEAND<3, IS_LOOSE_TREE, BaseGeometryType, EntityAdapter>;

  // Managed types

  template<dim_t DIMENSION_NO, typename TScalar = BaseGeometryType, bool IS_CONTIOGUOS_CONTAINER = true>
  using OrthoTreePointManagedND = OrthoTreeManaged<OrthoTreePointND<DIMENSION_NO, TScalar, IS_CONTIOGUOS_CONTAINER>>;

  template<dim_t DIMENSION_NO, bool IS_LOOSE_TREE = true, typename TScalar = BaseGeometryType, bool IS_CONTIOGUOS_CONTAINER = true>
  using OrthoTreeBoxManagedND = OrthoTreeManaged<OrthoTreeBoxND<DIMENSION_NO, IS_LOOSE_TREE, TScalar, IS_CONTIOGUOS_CONTAINER>>;

  template<dim_t DIMENSION_NO, typename TScalar, typename TEntityContainer>
  using OrthoTreePointManagedNDUD = OrthoTreeManaged<OrthoTreePointNDUD<DIMENSION_NO, TScalar, TEntityContainer>>;

  template<dim_t DIMENSION_NO, bool IS_LOOSE_TREE, typename TScalar, typename TEntityContainer>
  using OrthoTreeBoxManagedNDUD = OrthoTreeManaged<OrthoTreeBoxNDUD<DIMENSION_NO, IS_LOOSE_TREE, TScalar, TEntityContainer>>;

  // Dualtree for points
  using DualtreePointM = OrthoTreePointManagedND<1, BaseGeometryType>;

  // Dualtree for bounding boxes
  template<bool IS_LOOSE_TREE = true>
  using DualtreeBoxMs = OrthoTreeBoxManagedND<1, IS_LOOSE_TREE, BaseGeometryType>;
  using DualtreeBoxM = OrthoTreeBoxManagedND<1, true, BaseGeometryType>;

  // Quadtree for points
  using QuadtreePointM = OrthoTreePointManagedND<2, BaseGeometryType>;

  // Quadtree for bounding boxes
  template<bool IS_LOOSE_TREE = true>
  using QuadtreeBoxMs = OrthoTreeBoxManagedND<2, IS_LOOSE_TREE, BaseGeometryType>;
  using QuadtreeBoxM = OrthoTreeBoxManagedND<2, true, BaseGeometryType>;

  // Octree for points
  using OctreePointM = OrthoTreePointManagedND<3, BaseGeometryType>;

  // Octree for bounding boxes
  template<bool IS_LOOSE_TREE = true>
  using OctreeBoxMs = OrthoTreeBoxManagedND<3, IS_LOOSE_TREE, BaseGeometryType>;
  using OctreeBoxM = OrthoTreeBoxManagedND<3, true, BaseGeometryType>;


  // std::unordered_map-based Dualtree for points
  using DualtreePointMapM = OrthoTreePointManagedND<1, BaseGeometryType, false>;

  // std::unordered_map-based Dualtree for bounding boxes
  template<bool IS_LOOSE_TREE = true>
  using DualtreeBoxMapMs = OrthoTreeBoxManagedND<1, IS_LOOSE_TREE, BaseGeometryType, false>;
  using DualtreeBoxMapM = OrthoTreeBoxManagedND<1, true, BaseGeometryType, false>;

  // std::unordered_map-based Quadtree for points
  using QuadtreePointMapM = OrthoTreePointManagedND<2, BaseGeometryType, false>;

  // std::unordered_map-based Quadtree for bounding boxes
  template<bool IS_LOOSE_TREE = true>
  using QuadtreeBoxMapMs = OrthoTreeBoxManagedND<2, IS_LOOSE_TREE, BaseGeometryType, false>;
  using QuadtreeBoxMapM = OrthoTreeBoxManagedND<2, true, BaseGeometryType, false>;

  // std::unordered_map-based Octree for points
  using OctreePointMapM = OrthoTreePointManagedND<3, BaseGeometryType, false>;

  // std::unordered_map-based Octree for bounding boxes
  template<bool IS_LOOSE_TREE = true>
  using OctreeBoxMapMs = OrthoTreeBoxManagedND<3, IS_LOOSE_TREE, BaseGeometryType, false>;
  using OctreeBoxMapM = OrthoTreeBoxManagedND<3, true, BaseGeometryType, false>;

  // User-defined container-based Quadtree for points
  template<typename EntityContainer>
  using QuadtreePointUDMapM = OrthoTreePointManagedNDUD<2, BaseGeometryType, EntityContainer>;

  // User-defined container-based Quadtree for bounding boxes
  template<typename EntityContainer, bool IS_LOOSE_TREE = true>
  using QuadtreeBoxUDMapMs = OrthoTreeBoxManagedNDUD<2, IS_LOOSE_TREE, BaseGeometryType, EntityContainer>;
  template<typename EntityContainer>
  using QuadtreeBoxUDMapM = OrthoTreeBoxManagedNDUD<2, true, BaseGeometryType, EntityContainer>;

  // User-defined container-based Octree for points
  template<typename EntityContainer>
  using OctreePointUDMapM = OrthoTreePointManagedNDUD<3, BaseGeometryType, EntityContainer>;

  // User-defined container-based Octree for bounding boxes
  template<typename EntityContainer, bool IS_LOOSE_TREE = true>
  using OctreeBoxUDMapMs = OrthoTreeBoxManagedNDUD<3, IS_LOOSE_TREE, BaseGeometryType, EntityContainer>;
  template<typename EntityContainer>
  using OctreeBoxUDMapM = OrthoTreeBoxManagedNDUD<3, true, BaseGeometryType, EntityContainer>;


  // Static Managed types

  template<dim_t DIMENSION_NO, typename TScalar = BaseGeometryType, bool IS_CONTIOGUOS_CONTAINER = true>
  using StaticTreePointManagedND = OrthoTreeManaged<StaticOrthoTreePointND<DIMENSION_NO, TScalar, IS_CONTIOGUOS_CONTAINER>>;

  template<dim_t DIMENSION_NO, bool IS_LOOSE_TREE = true, typename TScalar = BaseGeometryType, bool IS_CONTIOGUOS_CONTAINER = true>
  using StaticTreeBoxManagedND = OrthoTreeManaged<StaticOrthoTreeBoxND<DIMENSION_NO, IS_LOOSE_TREE, TScalar, IS_CONTIOGUOS_CONTAINER>>;


  // Static Managed Dualtree for points
  using StaticDualtreePointM = StaticTreePointManagedND<1, BaseGeometryType>;

  // Static Managed Dualtree for bounding boxes
  using StaticDualtreeBoxM = StaticTreeBoxManagedND<1, true, BaseGeometryType>;

  // Static Managed Quadtree for points
  using StaticQuadtreePointM = StaticTreePointManagedND<2, BaseGeometryType>;

  // Static Managed Quadtree for bounding boxes
  using StaticQuadtreeBoxM = StaticTreeBoxManagedND<2, true, BaseGeometryType>;

  // Static Managed Octree for points
  using StaticOctreePointM = StaticTreePointManagedND<3, BaseGeometryType>;

  // Static Managed Octree for bounding boxes
  using StaticOctreeBoxM = StaticTreeBoxManagedND<3, true, BaseGeometryType>;


  // Static Managed std::unordered_map-based Dualtree for points
  using StaticDualtreePointMapM = StaticTreePointManagedND<1, BaseGeometryType, false>;

  // Static Managed std::unordered_map-based Dualtree for bounding boxes
  using StaticDualtreeBoxMapM = StaticTreeBoxManagedND<1, true, BaseGeometryType, false>;

  // Static Managed std::unordered_map-based Quadtree for points
  using StaticQuadtreePointMapM = StaticTreePointManagedND<2, BaseGeometryType, false>;

  // Static Managed std::unordered_map-based Quadtree for bounding boxes
  using StaticQuadtreeBoxMapM = StaticTreeBoxManagedND<2, true, BaseGeometryType, false>;

  // Static Managed std::unordered_map-based Octree for points
  using StaticOctreePointMapM = StaticTreePointManagedND<3, BaseGeometryType, false>;

  // Static Managed std::unordered_map-based Octree for bounding boxes
  using StaticOctreeBoxMapM = StaticTreeBoxManagedND<3, true, BaseGeometryType, false>;

} // namespace OrthoTree


#ifndef ORTHOTREE__BVH_H_INCLUDED
#define ORTHOTREE__BVH_H_INCLUDED
#endif

#if defined(ORTHOTREE__USE_PMR) || defined(_MSC_VER)
#ifndef ORTHOTREE__DISABLE_PMR
#define ORTHOTREE_IS_PMR_USED
#endif // !ORTHOTREE__DISABLE_PMR
#endif


namespace OrthoTree
{
  template<int CHILD_NUM_ = 8>
  struct BVHConfiguration
  {
    static constexpr int CHILD_NUM = CHILD_NUM_;
    static constexpr std::size_t DEFAULT_TARGET_ELEMENT_NUM_IN_NODES = 16;

    // SAH bin count. Increasing this value will improve the quality of the BVH but will also increase the creation time.
    static constexpr int SAH_BIN_COUNT = 16;

    // Required by OrthoTreeQueryBase
    static constexpr bool IS_HOMOGENEOUS_GEOMETRY = true;
    static constexpr bool USE_REVERSE_MAPPING = false;
    static constexpr depth_t MAX_ALLOWED_DEPTH_ID = depth_t{ 32 };

    template<typename TKey, typename TValue, typename THash>
    using UMapNodeContainer = std::unordered_map<TKey, TValue, THash>;
    template<typename TKey, typename TValue, typename TComp>
    using MapNodeContainer = std::map<TKey, TValue, TComp>;
  };

  template<typename TEntityAdapter, typename TGeometryAdapter, typename TConfiguration>
  class StaticBVHLinearCore
  {
  public: // Public type aliases
    using EA = TEntityAdapter;
    using GA = TGeometryAdapter;
    using CONFIG = TConfiguration;
    using IGM = detail::InternalGeometryModule<TGeometryAdapter>;

    using NodeID = int;
    using NodeIDCR = NodeID;
    using NodeValue = NodeID;
    using ChildID = NodeID;

    using EntityID = EA::EntityID;
    using EntityGeometry = EA::Geometry;

    using TScalar = typename GA::Scalar;
    using TFloatScalar = typename GA::FloatScalar;
    using TVector = typename GA::Vector;
    using TBox = typename GA::Box;
    using TRay = typename GA::Ray;
    using TPlane = typename GA::Plane;

  private: // Internal type aliases
    using NodeStorage256 = detail::NodeStorage256;
    using NodeStorage65536 = detail::NodeStorage65536;
    using NodeStorageGeneral = detail::NodeStorageGeneral;
    using NodeGeometryData = detail::NodeGeometryData<typename IGM::Vector>;

  private: // Data members
    std::variant<NodeStorage256, NodeStorage65536, NodeStorageGeneral> m_nodes;
    std::vector<EntityID> m_entityStorage;
    std::vector<NodeGeometryData> m_nodeGeometry;
    depth_t m_maxDepthNo = 0;
    std::size_t m_maxElementNum = CONFIG::DEFAULT_TARGET_ELEMENT_NUM_IN_NODES;

  private: // Serialization
    static constexpr uint32_t SERIALIZED_VERSION_ID = 0;

    template<typename TArchive, typename TEntityAdapter_, typename TGeometryAdapter_, typename TConfiguration_>
    friend void serialize(TArchive& ar, StaticBVHLinearCore<TEntityAdapter_, TGeometryAdapter_, TConfiguration_>& core);

  public:
    // Default constructor. Requires Create call before usage.
    constexpr StaticBVHLinearCore() = default;

    // Initialize the base octree structure with entity collection
    template<typename TExecMode = SeqExec>
    explicit StaticBVHLinearCore(
      EA::EntityContainerView entities, std::size_t maxElementNoInNode = CONFIG::DEFAULT_TARGET_ELEMENT_NUM_IN_NODES, TExecMode execMode = {}) noexcept
    {
      [[maybe_unused]] auto isSuccessfullyInsertedAllElements = Create(entities, maxElementNoInNode, execMode);
      assert(isSuccessfullyInsertedAllElements);
    }

    template<typename TExecMode = SeqExec>
    explicit StaticBVHLinearCore(
      TExecMode execMode, EA::EntityContainerView entities, std::size_t maxElementNoInNode = CONFIG::DEFAULT_TARGET_ELEMENT_NUM_IN_NODES) noexcept
    {
      [[maybe_unused]] auto isSuccessfullyInsertedAllElements = Create(entities, maxElementNoInNode, execMode);
      assert(isSuccessfullyInsertedAllElements);
    }

  public:
    constexpr depth_t GetMaxDepthID() const { return m_maxDepthNo - 1; }
    constexpr depth_t GetDepthNo() const { return m_maxDepthNo; }
    constexpr std::size_t GetMaxElementNum() const noexcept { return m_maxElementNum; }

    constexpr TBox GetBox() const noexcept
    {
      if (GetNodeCount() == 0)
        return GA::MakeBox();

      auto const rootBox = GetNodeBox(GetRootNodeID());
      auto box = GA::MakeBox();
      detail::static_for<GA::DIMENSION_NO>([&](auto d) {
        GA::SetBoxMinC(box, d, rootBox.Min[d]);
        GA::SetBoxMaxC(box, d, rootBox.Max[d]);
      });
      return box;
    }

    // Node API
#ifdef ORTHOTREE__PUBLIC_NODE_INTERFACE
  public:
#else
  protected:
#endif
    static NodeID GetRootNodeID() noexcept { return 0; }

    constexpr NodeID GetRootNodeValue() const noexcept { return GetRootNodeID(); }

    constexpr NodeID GetNodeValue(NodeID nodeID) const noexcept { return nodeID; }

    constexpr std::size_t GetNodeCount() const noexcept
    {
      return std::visit([&](auto const& nodes) -> std::size_t { return nodes.nodeEntitySegment.size(); }, m_nodes);
    }

    constexpr std::size_t GetNodeEntityCount(NodeValue nodeID) const noexcept
    {
      return std::visit([&](auto const& nodes) -> std::size_t { return static_cast<std::size_t>(nodes.nodeEntitySegment[nodeID].length); }, m_nodes);
    }

    constexpr SequenceView<NodeID> GetNodeChildren(NodeValue nodeID) const noexcept
    {
      return std::visit(
        [nodeID](auto const& nodes) {
          if (nodeID >= NodeValue(nodes.nodeChildSegmentBegins.size()) - 1)
            return SequenceView<NodeID>(static_cast<NodeID>(0), static_cast<NodeID>(0));

          return SequenceView<NodeID>(
            static_cast<NodeID>(nodes.nodeChildSegmentBegins[nodeID]), static_cast<NodeID>(nodes.nodeChildSegmentBegins[nodeID + 1]));
        },
        m_nodes);
    }

    constexpr std::span<EntityID const> GetNodeEntities(NodeValue nodeID) const noexcept
    {
      return std::visit(
        [this, nodeID](auto const& nodes) {
          auto const [beginID, length] = nodes.nodeEntitySegment[nodeID];
          return std::span<EntityID const>(&m_entityStorage[beginID], length);
        },
        m_nodes);
    }

    constexpr bool IsNodeEntitiesEmpty(NodeValue nodeID) const noexcept
    {
      return std::visit([nodeID](auto const& nodes) { return nodes.nodeEntitySegment[nodeID].length == 0; }, m_nodes);
    }

    constexpr decltype(auto) GetNodeMinPoint(NodeValue nodeID) const noexcept { return m_nodeGeometry[nodeID].minPoint; }

    constexpr decltype(auto) GetNodeSize(NodeValue nodeID) const noexcept { return m_nodeGeometry[nodeID].size; }

    constexpr typename IGM::Box GetNodeBox(NodeValue nodeID) const noexcept
    {
      auto const& [minPoint, size] = m_nodeGeometry[nodeID];
      return { .Min = minPoint, .Max = IGM::Add(minPoint, size) };
    }

  private:
    static constexpr int CHILD_NUM = CONFIG::CHILD_NUM;

    struct EntityBuildData
    {
      EntityID entityID;
      IGM::Vector center;
      typename IGM::Box box;
    };

    // Evaluate SAH cost for splitting along a given axis at a given position
    // Returns the cost = leftArea * leftCount + rightArea * rightCount
    struct SplitCandidate
    {
      typename IGM::Geometry cost = std::numeric_limits<typename IGM::Geometry>::max();
      typename IGM::Geometry splitPos = {};
      dim_t axis = 0;
    };

    template<typename TExecMode = SeqExec>
    static SplitCandidate FindBestSplitAxis(std::span<EntityBuildData const> entities, [[maybe_unused]] TExecMode execMode = {}) noexcept
    {
      using Geometry = typename IGM::Geometry;

      struct Bin
      {
        uint32_t count = 0;
        typename IGM::Box bounds;
      };

      SplitCandidate best;

      for (dim_t dimID = 0; dimID < GA::DIMENSION_NO; ++dimID)
      {
        // Find bounds of centers along this axis
        Geometry boundsMin = std::numeric_limits<Geometry>::max();
        Geometry boundsMax = std::numeric_limits<Geometry>::lowest();
        for (auto const& ed : entities)
        {
          boundsMin = std::min(boundsMin, ed.center[dimID]);
          boundsMax = std::max(boundsMax, ed.center[dimID]);
        }

        if (boundsMax - boundsMin < std::numeric_limits<Geometry>::epsilon())
          continue;

        // Populate bins
        std::array<Bin, CONFIG::SAH_BIN_COUNT> bins;
        for (auto& b : bins)
        {
          b.count = 0;
          b.bounds = IGM::BoxInvertedInit();
        }

        Geometry const scale = Geometry(CONFIG::SAH_BIN_COUNT) / (boundsMax - boundsMin);
        for (auto const& ed : entities)
        {
          int binIdx = std::min(CONFIG::SAH_BIN_COUNT - 1, static_cast<int>((ed.center[dimID] - boundsMin) * scale));
          ++bins[binIdx].count;
          IGM::UniteInBoxAD(bins[binIdx].bounds, ed.box);
        }

        // Sweep from left and right to compute prefix areas and counts
        std::array<Geometry, CONFIG::SAH_BIN_COUNT - 1> leftArea, rightArea;
        std::array<uint32_t, CONFIG::SAH_BIN_COUNT - 1> leftCount, rightCount;

        {
          typename IGM::Box leftBox = IGM::BoxInvertedInit();
          uint32_t leftSum = 0;
          for (int i = 0; i < CONFIG::SAH_BIN_COUNT - 1; ++i)
          {
            leftSum += bins[i].count;
            leftCount[i] = leftSum;
            if (bins[i].count > 0)
              IGM::UniteInBoxAD(leftBox, bins[i].bounds);
            leftArea[i] = IGM::GetSurfaceAreaAD(leftBox);
          }
        }
        {
          typename IGM::Box rightBox = IGM::BoxInvertedInit();
          uint32_t rightSum = 0;
          for (int i = CONFIG::SAH_BIN_COUNT - 1; i > 0; --i)
          {
            rightSum += bins[i].count;
            rightCount[i - 1] = rightSum;
            if (bins[i].count > 0)
              IGM::UniteInBoxAD(rightBox, bins[i].bounds);
            rightArea[i - 1] = IGM::GetSurfaceAreaAD(rightBox);
          }
        }

        // Evaluate SAH cost for each split plane
        Geometry const binWidth = (boundsMax - boundsMin) / Geometry(CONFIG::SAH_BIN_COUNT);
        for (int i = 0; i < CONFIG::SAH_BIN_COUNT - 1; ++i)
        {
          if (leftCount[i] == 0 || rightCount[i] == 0)
            continue;

          Geometry cost = leftCount[i] * leftArea[i] + rightCount[i] * rightArea[i];
          if (cost < best.cost)
          {
            best.cost = cost;
            best.splitPos = boundsMin + binWidth * Geometry(i + 1);
            best.axis = dimID;
          }
        }
      }

      return best;
    }


    constexpr NodeID CreateBVHNode(uint8_t depthID, uint32_t nodeEntityBeginID, uint32_t nodeEntityCount, uint32_t childNodeCount) noexcept
    {
      auto const nodeID = static_cast<NodeID>(GetNodeCount());

      m_maxDepthNo = std::max<depth_t>(m_maxDepthNo, depthID + 1);

      std::visit(
        [&](auto& nodes) {
          using NodeStorage = std::decay_t<decltype(nodes)>;
          using EntitySegment = typename NodeStorage::EntitySegment;

          nodes.nodeEntitySegment.emplace_back(
            static_cast<typename EntitySegment::Begin>(nodeEntityBeginID), static_cast<typename EntitySegment::Length>(nodeEntityCount));

          // If there are no child nodes, we can skip storing child segment begin index for this node.
          if (childNodeCount == 0)
            return;

          nodes.nodeChildSegmentBegins.resize(nodeID + 2, nodes.nodeChildSegmentBegins.back());
          nodes.nodeChildSegmentBegins.back() += static_cast<typename NodeStorage::NodeSegmentIndex>(childNodeCount);
        },
        m_nodes);

      m_nodeGeometry.push_back({});

      return nodeID;
    }


    template<typename TExecMode = SeqExec>
    void BuildBVH(
      std::vector<EntityBuildData>& buildData,
      [[maybe_unused]] EA::EntityContainerView entities,
      uint8_t maxDepthID,
      std::size_t maxElementNoInNode,
      std::vector<uint8_t>& nodeDepthIDs,
      TExecMode execMode = {}) noexcept
    {
      struct NodeProcessingData
      {
        uint32_t beginID;
        uint32_t length;
        uint8_t depthID;
      };

      if constexpr (std::is_same_v<TExecMode, SeqExec>)
      {
        std::queue<NodeProcessingData> q;
        q.push({ 0, static_cast<uint32_t>(buildData.size()), 0 });

        while (!q.empty())
        {
          auto const r = q.front();
          q.pop();

          if (r.length <= maxElementNoInNode || r.depthID >= maxDepthID)
          {
            nodeDepthIDs.push_back(r.depthID);
            CreateBVHNode(r.depthID, r.beginID, r.length, 0);
            continue;
          }

          struct SubRange
          {
            uint32_t beginID;
            uint32_t length;
          };
          std::array<SubRange, CHILD_NUM> subRanges;
          subRanges[0] = { r.beginID, r.length };
          int rangeCount = 1;

          while (rangeCount < CHILD_NUM)
          {
            int bestIdx = 0;
            for (int k = 1; k < rangeCount; ++k)
              if (subRanges[k].length > subRanges[bestIdx].length)
                bestIdx = k;

            auto& sub = subRanges[bestIdx];
            if (sub.length <= 1)
              break;

            auto subSpan = std::span<EntityBuildData>(buildData.data() + sub.beginID, sub.length);
            auto split = FindBestSplitAxis(subSpan, SeqExec{});
            if (split.cost >= std::numeric_limits<typename IGM::Geometry>::max())
              break;

            auto partIt = std::partition(subSpan.begin(), subSpan.end(), [&](auto const& ed) { return ed.center[split.axis] < split.splitPos; });
            uint32_t leftCount = static_cast<uint32_t>(partIt - subSpan.begin());
            leftCount = std::clamp(leftCount, uint32_t(1), sub.length - 1);

            uint32_t origBegin = sub.beginID;
            uint32_t origLength = sub.length;
            sub = { origBegin, leftCount };
            subRanges[rangeCount] = { origBegin + leftCount, origLength - leftCount };
            ++rangeCount;
          }

          if (rangeCount == 1)
          {
            nodeDepthIDs.push_back(r.depthID);
            CreateBVHNode(r.depthID, r.beginID, r.length, 0);
          }
          else
          {
            std::sort(subRanges.begin(), subRanges.begin() + rangeCount, [](auto const& a, auto const& b) { return a.beginID < b.beginID; });
            nodeDepthIDs.push_back(r.depthID);
            CreateBVHNode(r.depthID, r.beginID, 0, static_cast<uint32_t>(rangeCount));
            for (int i = 0; i < rangeCount; ++i)
              q.push({ subRanges[i].beginID, subRanges[i].length, static_cast<uint8_t>(r.depthID + 1) });
          }
        }
      }
      else
      {
        std::vector<NodeProcessingData> currentLevel;
        currentLevel.push_back({ 0, static_cast<uint32_t>(buildData.size()), 0 });

        while (!currentLevel.empty())
        {
          struct SubRange
          {
            uint32_t beginID;
            uint32_t length;
          };
          struct SplitResult
          {
            std::array<SubRange, CHILD_NUM> subRanges;
            int rangeCount = 1;
            bool isLeaf = false;
          };

          std::vector<SplitResult> results(currentLevel.size());

          EXEC_POL_DEF(epl);
          auto const indices = SequenceView<std::size_t>(0, currentLevel.size());
          std::for_each(EXEC_POL_ADD(epl) indices.begin(), indices.end(), [&](std::size_t i) {
            auto const& r = currentLevel[i];
            auto& res = results[i];

            if (r.length <= maxElementNoInNode || r.depthID >= maxDepthID)
            {
              res.isLeaf = true;
              return;
            }

            res.subRanges[0] = { r.beginID, r.length };
            while (res.rangeCount < CHILD_NUM)
            {
              int bestIdx = 0;
              for (int k = 1; k < res.rangeCount; ++k)
                if (res.subRanges[k].length > res.subRanges[bestIdx].length)
                  bestIdx = k;

              auto& sub = res.subRanges[bestIdx];
              if (sub.length <= 1)
                break;

              auto subSpan = std::span<EntityBuildData>(buildData.data() + sub.beginID, sub.length);
              auto split = FindBestSplitAxis(subSpan, execMode);
              if (split.cost >= std::numeric_limits<typename IGM::Geometry>::max())
                break;

              auto partIt = std::partition(subSpan.begin(), subSpan.end(), [&](auto const& ed) { return ed.center[split.axis] < split.splitPos; });
              uint32_t leftCount = static_cast<uint32_t>(partIt - subSpan.begin());
              leftCount = std::clamp(leftCount, uint32_t(1), sub.length - 1);

              uint32_t origBegin = sub.beginID;
              uint32_t origLength = sub.length;
              sub = { origBegin, leftCount };
              res.subRanges[res.rangeCount] = { origBegin + leftCount, origLength - leftCount };
              ++res.rangeCount;
            }

            if (res.rangeCount == 1)
            {
              res.isLeaf = true;
            }
            else
            {
              std::sort(res.subRanges.begin(), res.subRanges.begin() + res.rangeCount, [](auto const& a, auto const& b) {
                return a.beginID < b.beginID;
              });
            }
          });

          std::vector<NodeProcessingData> nextLevel;
          for (std::size_t i = 0; i < currentLevel.size(); ++i)
          {
            auto const& r = currentLevel[i];
            auto const& res = results[i];
            if (res.isLeaf)
            {
              nodeDepthIDs.push_back(r.depthID);
              CreateBVHNode(r.depthID, r.beginID, r.length, 0);
            }
            else
            {
              nodeDepthIDs.push_back(r.depthID);
              CreateBVHNode(r.depthID, r.beginID, 0, static_cast<uint32_t>(res.rangeCount));
              for (int j = 0; j < res.rangeCount; ++j)
                nextLevel.push_back({ res.subRanges[j].beginID, res.subRanges[j].length, static_cast<uint8_t>(r.depthID + 1) });
            }
          }
          currentLevel = std::move(nextLevel);
        }
      }
    }


    template<typename TExecMode = SeqExec>
    void InitializeNodeGeometryBVH(EA::EntityContainerView entities, std::vector<uint8_t>& nodeDepthIDs, TExecMode execMode = {}) noexcept
    {
      auto const nodeCount = static_cast<uint32_t>(GetNodeCount());
      if (nodeCount == 0)
        return;

      // Group nodes by depth for safe parallel/iterative bottom-up processing
      auto const maxDepth = GetMaxDepthID();
      std::vector<std::vector<uint32_t>> nodesAtDepth(maxDepth + 1);
      for (uint32_t i = 0; i < nodeCount; ++i)
        nodesAtDepth[nodeDepthIDs[i]].push_back(i);

      auto const processNode = [&](uint32_t nodeID) {
        auto nodeBox = IGM::BoxInvertedInit();

        std::visit(
          [&](auto const& nodes) {
            auto const [entityBeginID, entityLength] = nodes.nodeEntitySegment[nodeID];
            for (uint32_t j = entityBeginID, endID = entityBeginID + entityLength; j < endID; ++j)
              IGM::UniteInBoxAD(nodeBox, EA::GetGeometry(entities, m_entityStorage[j]));

            if (nodes.nodeChildSegmentBegins.size() <= static_cast<size_t>(nodeID) + 1)
              return;

            auto const childBeginNodeID = nodes.nodeChildSegmentBegins[nodeID];
            auto const childEndNodeID = nodes.nodeChildSegmentBegins[nodeID + 1];
            for (auto childID = childBeginNodeID; childID < childEndNodeID; ++childID)
              IGM::UniteInBoxAD(nodeBox, GetNodeBox(childID));
          },
          m_nodes);

        m_nodeGeometry[nodeID] = { nodeBox.Min, IGM::Sub(nodeBox.Max, nodeBox.Min) };
      };

      // Process depths bottom-up
      for (int d = static_cast<int>(maxDepth); d >= 0; --d)
      {
        auto const& currentLevelNodes = nodesAtDepth[d];
        if (currentLevelNodes.empty())
          continue;

        EXEC_POL_DEF(ep);
        std::for_each(EXEC_POL_ADD(ep) currentLevelNodes.begin(), currentLevelNodes.end(), processNode);
      }
    }

    template<typename TNodeStorage>
    static constexpr bool IsFit(std::size_t entityCount, std::size_t maxNodeCount) noexcept
    {
      return maxNodeCount < std::numeric_limits<typename TNodeStorage::NodeSegmentIndex>::max() &&
             entityCount < std::numeric_limits<typename TNodeStorage::EntitySegment::Begin>::max() &&
             entityCount < std::numeric_limits<typename TNodeStorage::EntitySegment::Length>::max();
    }

  public:
    template<typename TExecMode = SeqExec>
    bool Create(EA::EntityContainerView entities, std::size_t maxElementNoInNode = CONFIG::DEFAULT_TARGET_ELEMENT_NUM_IN_NODES, TExecMode execMode = {}) noexcept
    {
      auto const entityCount = entities.size();
      if (entityCount == 0)
        return true;

      m_maxElementNum = maxElementNoInNode;
      m_maxDepthNo = 0;

      // Estimate max depth: log_{CHILD_NUM}(entityCount / maxElementNoInNode)
      auto const maxDepthID = [&]() -> uint8_t {
        if (entityCount <= maxElementNoInNode)
          return 1;
        auto const nLeaf = static_cast<double>(entityCount) / static_cast<double>(maxElementNoInNode);
        auto const logBase = std::log2(nLeaf) / std::log2(static_cast<double>(CHILD_NUM));
        return static_cast<uint8_t>(std::clamp(static_cast<int>(std::ceil(logBase)), 2, static_cast<int>(CONFIG::MAX_ALLOWED_DEPTH_ID)));
      }();

      // Build entity center data and populate entity storage
      auto buildData = std::vector<EntityBuildData>(entityCount);
      m_entityStorage.resize(entityCount);

      EXEC_POL_DEF(epb);
      detail::zip_view buildDataAndStorageZip(buildData, m_entityStorage);
      std::transform(EXEC_POL_ADD(epb) entities.begin(), entities.end(), buildDataAndStorageZip.begin(), [&](auto const& entity) {
        auto const entityID = EA::GetEntityID(entities, entity);
        auto const& geometry = EA::GetGeometry(entity);

        typename IGM::Vector center;
        auto const entityBox = IGM::GetBoxAD(geometry);

        if constexpr (EA::GEOMETRY_TYPE == GeometryType::Box)
        {
          for (dim_t d = 0; d < GA::DIMENSION_NO; ++d)
            center[d] = (typename IGM::Geometry(GA::GetBoxMinC(geometry, d)) + typename IGM::Geometry(GA::GetBoxMaxC(geometry, d))) *
                        typename IGM::Geometry(0.5);
        }
        else // Point
        {
          for (dim_t d = 0; d < GA::DIMENSION_NO; ++d)
            center[d] = typename IGM::Geometry(GA::GetPointC(geometry, d));
        }

        return std::make_pair(EntityBuildData{ entityID, center, entityBox }, entityID);
      });

      // Initialize node storage variant
      auto const estimatedNodeCount = std::max<std::size_t>(16, entityCount / std::max<std::size_t>(1, maxElementNoInNode) * 4);
      if (IsFit<NodeStorage256>(entityCount, estimatedNodeCount))
        m_nodes = NodeStorage256{ .nodeChildSegmentBegins = { 1 }, .nodeEntitySegment = {} };
      else if (IsFit<NodeStorage65536>(entityCount, estimatedNodeCount))
        m_nodes = NodeStorage65536{ .nodeChildSegmentBegins = { 1 }, .nodeEntitySegment = {} };
      else
        m_nodes = NodeStorageGeneral{ .nodeChildSegmentBegins = { 1 }, .nodeEntitySegment = {} };

      // Reserve space to avoid reallocations and appease compiler warnings
      std::vector<uint8_t> nodeDepthIDs;
      nodeDepthIDs.reserve(estimatedNodeCount);
      m_nodeGeometry.reserve(estimatedNodeCount);

      std::visit(
        [&](auto& nodes) {
          nodes.nodeEntitySegment.reserve(estimatedNodeCount);
          nodes.nodeChildSegmentBegins.reserve(estimatedNodeCount + 1);
        },
        m_nodes);

      // Build the BVH
      BuildBVH(buildData, entities, maxDepthID, maxElementNoInNode, nodeDepthIDs, execMode);

      // Write back entity IDs in the final sorted order
      EXEC_POL_DEF(epw);
      std::transform(EXEC_POL_ADD(epw) buildData.begin(), buildData.end(), m_entityStorage.begin(), [](auto const& ed) { return ed.entityID; });

      // Initialize node geometry (MBR) bottom-up
      InitializeNodeGeometryBVH(entities, nodeDepthIDs, execMode);

      return true;
    }

    template<typename TExecMode = SeqExec>
    static bool Create(
      StaticBVHLinearCore& tree,
      EA::EntityContainerView entities,
      std::size_t maxElementNoInNode = CONFIG::DEFAULT_TARGET_ELEMENT_NUM_IN_NODES,
      TExecMode execMode = {}) noexcept
    {
      return tree.Create<TExecMode>(entities, maxElementNoInNode, execMode);
    }


    static constexpr bool AreChildNodesOverlapping() noexcept { return true; }
  };

  template<typename TEntityAdapter, typename TGeometryAdapter, typename TConfiguration>
  using StaticBVHLinearBase = OrthoTreeQueryBase<StaticBVHLinearCore<TEntityAdapter, TGeometryAdapter, TConfiguration>>;
} // namespace OrthoTree


namespace OrthoTree
{
  template<dim_t DIMENSION_NO, typename TScalar = BaseGeometryType, bool IS_CONTIOGUOS_CONTAINER = true, int CHILD_NUM = 2>
  using StaticBVHPointND = StaticBVHLinearBase<
    std::conditional_t<IS_CONTIOGUOS_CONTAINER, PointEntitySpanAdapter<PointND<DIMENSION_NO, TScalar>>, PointEntityMapAdapter<PointND<DIMENSION_NO, TScalar>>>,
    GeneralGeometryAdapterND<DIMENSION_NO, TScalar>,
    BVHConfiguration<CHILD_NUM>>;

  template<dim_t DIMENSION_NO, typename TScalar = BaseGeometryType, bool IS_CONTIOGUOS_CONTAINER = true, int CHILD_NUM = 2>
  using StaticBVHBoxND = StaticBVHLinearBase<
    std::conditional_t<IS_CONTIOGUOS_CONTAINER, BoxEntitySpanAdapter<BoundingBoxND<DIMENSION_NO, TScalar>>, BoxEntityMapAdapter<BoundingBoxND<DIMENSION_NO, TScalar>>>,
    GeneralGeometryAdapterND<DIMENSION_NO, TScalar>,
    BVHConfiguration<CHILD_NUM>>;

  // BVH for points
  using StaticBVHPoint1D = StaticBVHPointND<1, BaseGeometryType>;
  using StaticBVHPoint2D = StaticBVHPointND<2, BaseGeometryType>;
  using StaticBVHPoint3D = StaticBVHPointND<3, BaseGeometryType>;
  using StaticBVHPoint4D = StaticBVHPointND<4, BaseGeometryType>;

  // BVH for bounding boxes
  using StaticBVHBox1D = StaticBVHBoxND<1, BaseGeometryType>;
  using StaticBVHBox2D = StaticBVHBoxND<2, BaseGeometryType>;
  using StaticBVHBox3D = StaticBVHBoxND<3, BaseGeometryType>;
  using StaticBVHBox4D = StaticBVHBoxND<4, BaseGeometryType>;

  // BVH with unordered_map entities
  using StaticBVHPointMap1D = StaticBVHPointND<1, BaseGeometryType, false>;
  using StaticBVHPointMap2D = StaticBVHPointND<2, BaseGeometryType, false>;
  using StaticBVHPointMap3D = StaticBVHPointND<3, BaseGeometryType, false>;
  using StaticBVHPointMap4D = StaticBVHPointND<4, BaseGeometryType, false>;
  using StaticBVHBoxMap1D = StaticBVHBoxND<1, BaseGeometryType, false>;
  using StaticBVHBoxMap2D = StaticBVHBoxND<2, BaseGeometryType, false>;
  using StaticBVHBoxMap3D = StaticBVHBoxND<3, BaseGeometryType, false>;
  using StaticBVHBoxMap4D = StaticBVHBoxND<4, BaseGeometryType, false>;

  template<dim_t DIMENSION_NO, int CHILD_NUM = 8>
  using StaticBVHNPointND = StaticBVHPointND<DIMENSION_NO, BaseGeometryType, true, CHILD_NUM>;
  template<dim_t DIMENSION_NO, int CHILD_NUM = 8>
  using StaticBVHNBoxND = StaticBVHBoxND<DIMENSION_NO, BaseGeometryType, true, CHILD_NUM>;

  template<dim_t DIMENSION_NO, int CHILD_NUM = 8>
  using StaticBVHNPointMapND = StaticBVHPointND<DIMENSION_NO, BaseGeometryType, false, CHILD_NUM>;
  template<dim_t DIMENSION_NO, int CHILD_NUM = 8>
  using StaticBVHNBoxMapND = StaticBVHBoxND<DIMENSION_NO, BaseGeometryType, false, CHILD_NUM>;

  // Managed types

  template<dim_t DIMENSION_NO, typename TScalar = BaseGeometryType, bool IS_CONTIOGUOS_CONTAINER = true, int CHILD_NUM = 2>
  using StaticBVHPointManagedND = OrthoTreeManaged<StaticBVHPointND<DIMENSION_NO, TScalar, IS_CONTIOGUOS_CONTAINER, CHILD_NUM>>;

  template<dim_t DIMENSION_NO, typename TScalar = BaseGeometryType, bool IS_CONTIOGUOS_CONTAINER = true, int CHILD_NUM = 2>
  using StaticBVHBoxManagedND = OrthoTreeManaged<StaticBVHBoxND<DIMENSION_NO, TScalar, IS_CONTIOGUOS_CONTAINER, CHILD_NUM>>;

  // BVH for points
  using StaticBVHPoint1DM = StaticBVHPointManagedND<1, BaseGeometryType>;
  using StaticBVHPoint2DM = StaticBVHPointManagedND<2, BaseGeometryType>;
  using StaticBVHPoint3DM = StaticBVHPointManagedND<3, BaseGeometryType>;
  using StaticBVHPoint4DM = StaticBVHPointManagedND<4, BaseGeometryType>;

  // BVH for bounding boxes
  using StaticBVHBox1DM = StaticBVHBoxManagedND<1, BaseGeometryType>;
  using StaticBVHBox2DM = StaticBVHBoxManagedND<2, BaseGeometryType>;
  using StaticBVHBox3DM = StaticBVHBoxManagedND<3, BaseGeometryType>;
  using StaticBVHBox4DM = StaticBVHBoxManagedND<4, BaseGeometryType>;

  // BVH with unordered_map entities
  using StaticBVHPointMap1DM = StaticBVHPointManagedND<1, BaseGeometryType, false>;
  using StaticBVHPointMap2DM = StaticBVHPointManagedND<2, BaseGeometryType, false>;
  using StaticBVHPointMap3DM = StaticBVHPointManagedND<3, BaseGeometryType, false>;
  using StaticBVHPointMap4DM = StaticBVHPointManagedND<4, BaseGeometryType, false>;
  using StaticBVHBoxMap1DM = StaticBVHBoxManagedND<1, BaseGeometryType, false>;
  using StaticBVHBoxMap2DM = StaticBVHBoxManagedND<2, BaseGeometryType, false>;
  using StaticBVHBoxMap3DM = StaticBVHBoxManagedND<3, BaseGeometryType, false>;
  using StaticBVHBoxMap4DM = StaticBVHBoxManagedND<4, BaseGeometryType, false>;

  // N-ary BVH Managed
  template<dim_t DIMENSION_NO, int CHILD_NUM = 8>
  using StaticBVHNPointManagedND = StaticBVHPointManagedND<DIMENSION_NO, BaseGeometryType, true, CHILD_NUM>;
  template<dim_t DIMENSION_NO, int CHILD_NUM = 8>
  using StaticBVHNBoxManagedND = StaticBVHBoxManagedND<DIMENSION_NO, BaseGeometryType, true, CHILD_NUM>;

  template<dim_t DIMENSION_NO, int CHILD_NUM = 8>
  using StaticBVHNPointMapManagedND = StaticBVHPointManagedND<DIMENSION_NO, BaseGeometryType, false, CHILD_NUM>;
  template<dim_t DIMENSION_NO, int CHILD_NUM = 8>
  using StaticBVHNBoxMapManagedND = StaticBVHBoxManagedND<DIMENSION_NO, BaseGeometryType, false, CHILD_NUM>;
} // namespace OrthoTree


#ifndef ORTHOTREE_SERIALIZATION_H
#define ORTHOTREE_SERIALIZATION_H
#endif


// Components

namespace OrthoTree
{
  // --- Non-Intrusive Serialization Functions for Geometry Types ---

  // BoundingBoxND
  template<typename TArchive, dim_t DIM, typename T>
  void serialize(TArchive& ar, BoundingBoxND<DIM, T>& box)
  {
    ar& ORTHOTREE_NVP_M(box, Min);
    ar& ORTHOTREE_NVP_M(box, Max);
  }

  // RayND
  template<typename TArchive, dim_t DIM, typename T>
  void serialize(TArchive& ar, RayND<DIM, T>& ray)
  {
    ar& ORTHOTREE_NVP_M(ray, Origin);
    ar& ORTHOTREE_NVP_M(ray, Direction);
  }

  // PlaneND
  template<typename TArchive, dim_t DIM, typename T>
  void serialize(TArchive& ar, PlaneND<DIM, T>& plane)
  {
    ar& ORTHOTREE_NVP_M(plane, OrigoDistance);
    ar& ORTHOTREE_NVP_M(plane, Normal);
  }


  // --- Internal Type Serialization ---
  namespace detail
  {
    template<typename TArchive, typename TVector>
    void serialize(TArchive& ar, BoundingBoxMinPointAndSize<TVector>& box)
    {
      ar& ORTHOTREE_NVP_M(box, minPoint);
      ar& ORTHOTREE_NVP_M(box, size);
    }


    template<typename TArchive, typename TBegin, typename TLength>
    void serialize(TArchive& ar, Segment<TBegin, TLength>& segment)
    {
      ar& ORTHOTREE_NVP_M(segment, begin);
      ar& ORTHOTREE_NVP_M(segment, length);
    }

    template<typename TArchive>
    void serialize(TArchive& ar, NodeStorage256& storage)
    {
      ar& ORTHOTREE_NVP_M(storage, nodeChildSegmentBegins);
      ar& ORTHOTREE_NVP_M(storage, nodeEntitySegment);
    }

    template<typename TArchive>
    void serialize(TArchive& ar, NodeStorage65536& storage)
    {
      ar& ORTHOTREE_NVP_M(storage, nodeChildSegmentBegins);
      ar& ORTHOTREE_NVP_M(storage, nodeEntitySegment);
    }

    template<typename TArchive>
    void serialize(TArchive& ar, NodeStorageGeneral& storage)
    {
      ar& ORTHOTREE_NVP_M(storage, nodeChildSegmentBegins);
      ar& ORTHOTREE_NVP_M(storage, nodeEntitySegment);
    }

    template<typename TArchive, typename TVector>
    void serialize(TArchive& ar, NodeGeometryData<TVector>& data)
    {
      ar& ORTHOTREE_NVP_M(data, minPoint);
      ar& ORTHOTREE_NVP_M(data, size);
    }

    template<typename TArchive, typename T, std::size_t N>
    void serialize(TArchive& ar, detail::inplace_vector<T, N>& val)
    {
      serialized_size_t size = static_cast<serialized_size_t>(val.size());
      ar& OrthoTree::make_size_tag(size);
      if (OrthoTree::is_loading_archive(ar))
      {
        val.clear();
        for (std::size_t i = 0; i < size; ++i)
        {
          val.push_back(load_construct<TArchive, T>::load(ar));
        }
      }
      else
      {
        for (auto& item : val)
          ar & item;
      }
    }


    template<typename TArchive, typename GA, bool ALLOW_OUT, double LOOSE, depth_t MAX_DP>
    void serialize(TArchive& ar, MortonGridSpaceIndexing<GA, ALLOW_OUT, LOOSE, MAX_DP>& si)
    {
      ar& ORTHOTREE_NVP_M(si, m_grid);
      ar& ORTHOTREE_NVP_M(si, m_maxDepthID);
    }

    template<typename TArchive, typename TGeometryAdapter>
    void serialize(TArchive& ar, GridSpaceIndexing<TGeometryAdapter>& grid)
    {
      ar& ORTHOTREE_NVP_M(grid, m_maxRasterResolution);
      ar& ORTHOTREE_NVP_M(grid, m_maxRasterID);
      ar& ORTHOTREE_NVP_M(grid, m_boxSpace);
      ar& ORTHOTREE_NVP_M(grid, m_sizeInDimensions);
      ar& ORTHOTREE_NVP_M(grid, m_volumeOfOverallSpace);
      ar& ORTHOTREE_NVP_M(grid, m_rasterization);
    }

    template<typename TArchive, typename GA>
    void serialize(TArchive& ar, typename GridSpaceIndexing<GA>::NativeRasterizationFactors& factors)
    {
      ar& ORTHOTREE_NVP_M(factors, sizeInDimensions);
      ar& ORTHOTREE_NVP_M(factors, rasterizerFactors);
      ar& ORTHOTREE_NVP_M(factors, derasterizerFactors);
    }

    template<typename TArchive, typename GA>
    void serialize(TArchive& ar, typename GridSpaceIndexing<GA>::ForcedDoubleRasterizationFactors& factors)
    {
      ar& ORTHOTREE_NVP_M(factors, sizeInDimensions);
      ar& ORTHOTREE_NVP_M(factors, rasterizerFactors);
      ar& ORTHOTREE_NVP_M(factors, derasterizerFactors);
    }

    template<typename TArchive, std::size_t C, typename NID, typename CID, typename EID, typename NG>
    void serialize(TArchive& ar, OrthoTreeNodeData<C, NID, CID, EID, NG>& node)
    {
      using Node = OrthoTreeNodeData<C, NID, CID, EID, NG>;

      auto serializedVersionID = Node::SERIALIZED_VERSION_ID;
      ar& ORTHOTREE_NVP(serializedVersionID);

      ar& ORTHOTREE_NVP_M(node, m_children);
      ar& ORTHOTREE_NVP_M(node, m_childIndex);
      if constexpr (!std::is_same_v<NG, std::monostate>)
        ar& ORTHOTREE_NVP_M(node, m_geometry);
    }


    template<typename T>
    class StdSpanSerializerProxy
    {
    public:
      StdSpanSerializerProxy(const std::span<T>& span, std::function<std::span<T>(std::size_t)>&& construct)
      : m_span(span)
      , m_construct(std::move(construct))
      {}

      template<typename TArchive>
      void serialize(TArchive& ar)
      {
        serialized_size_t size = static_cast<serialized_size_t>(m_span.size());
        ar& make_size_tag(size);

        if (OrthoTree::is_loading_archive(ar))
          m_span = m_construct(size);

        for (auto& item : m_span)
          ar & item;
      }

    private:
      std::span<T> m_span;
      std::function<std::span<T>(std::size_t)> m_construct;
    };

    template<typename NodeID, typename T>
    class NodeSegmentSerializerProxy
    {
    public:
      constexpr NodeSegmentSerializerProxy(NodeID& nodeID, typename MemoryResource<T>::MemorySegment& segment, MemoryResource<T>& memoryResource) noexcept
      : m_nodeID(nodeID)
      , m_segment(segment)
      , m_memoryResource(memoryResource)
      {}

      template<typename TArchive>
      void serialize(TArchive& ar)
      {
        ar& ORTHOTREE_NVP(m_nodeID);
        auto m_entityStorage = StdSpanSerializerProxy<T>(m_segment.segment, [&](std::size_t size) {
          m_segment = m_memoryResource.Allocate(size);
          return m_segment.segment;
        });
        ar& ORTHOTREE_NVP(m_entityStorage);
      }

    private:
      NodeID& m_nodeID;
      MemoryResource<T>::MemorySegment& m_segment;
      MemoryResource<T>& m_memoryResource;
    };

    template<typename TArchive, typename T, typename TNodes>
    void serializeMemoryResource(TArchive& ar, MemoryResource<T>& memoryResource, TNodes& nodes)
    {
      using NodeID = typename std::decay_t<TNodes>::key_type;
      using NodeSegment = NodeSegmentSerializerProxy<NodeID, T>;

      serialized_size_t nodeCount = 0;
      if (OrthoTree::is_loading_archive(ar))
      {
        ar& make_size_tag(nodeCount);
        for (std::size_t i = 0; i < nodeCount; ++i)
        {
          NodeID nodeID = {};
          typename MemoryResource<T>::MemorySegment segment = {};
          auto m_segment = NodeSegment(nodeID, segment, memoryResource);

          ar & m_segment;

          nodes.at(nodeID).GetEntitySegment() = std::move(segment);
        }
      }
      else
      {
        for (auto& [nodeID, node] : nodes)
          nodeCount += !node.IsEntitiesEmpty();

        ar& make_size_tag(nodeCount);

        for (auto& [nodeID, node] : nodes)
        {
          if (node.IsEntitiesEmpty())
            continue;

          auto nonConstNodeID = const_cast<NodeID&>(nodeID);
          auto m_segment = NodeSegment(nonConstNodeID, node.GetEntitySegment(), memoryResource);
          ar & m_segment;
        }
      }
    }


    template<typename TData, typename TNodeMap>
    class MemoryResourceContentSerializerProxy
    {
    public:
      MemoryResourceContentSerializerProxy(MemoryResource<TData>& memoryResource, TNodeMap& nodes)
      : m_memoryResource(memoryResource)
      , m_nodes(nodes)
      {}

      template<typename TArchive>
      void serialize(TArchive& ar)
      {
        serializeMemoryResource(ar, m_memoryResource, m_nodes);
      }

    private:
      MemoryResource<TData>& m_memoryResource;
      TNodeMap& m_nodes;
    };

    template<typename TData, typename TNodeMap>
    class MemoryResourceSerializerProxy
    {
    public:
      MemoryResourceSerializerProxy(MemoryResource<TData>& memoryResource, TNodeMap& nodes)
      : m_memoryResource(memoryResource)
      , m_nodes(nodes)
      {}

      template<typename TArchive>
      void serialize(TArchive& ar)
      {
        auto serializedVersionID = MemoryResource<TData>::SERIALIZED_VERSION_ID;
        ar& ORTHOTREE_NVP(serializedVersionID);

        serialized_size_t m_memorySize = 0;
        if (!OrthoTree::is_loading_archive(ar))
        {
          for (auto& [nodeID, node] : m_nodes)
            m_memorySize += node.GetEntitySegment().segment.size();

          ar& ORTHOTREE_NVP(m_memorySize);
        }
        else
        {
          // Version handling
          if (serializedVersionID < 1)
          {
            std::size_t m_memorySize_old = static_cast<std::size_t>(m_memorySize);
            ar& ORTHOTREE_NVP(m_memorySize_old);
            m_memorySize = static_cast<serialized_size_t>(m_memorySize_old);
          }
          else
          {
            ar& ORTHOTREE_NVP(m_memorySize);
          }

          m_memoryResource.Init(m_memorySize);
        }

        auto m_content = MemoryResourceContentSerializerProxy(m_memoryResource, m_nodes);
        ar& ORTHOTREE_NVP(m_content);
      }

    private:
      MemoryResource<TData>& m_memoryResource;
      TNodeMap& m_nodes;
    };

  } // namespace detail


  // --- Core Serialization ---

  // OrthoTreeCoreBase
  template<typename TArchive, typename TEntityAdapter, typename TGeometryAdapter, typename TConfiguration>
  void serialize(TArchive& ar, OrthoTreeCoreBase<TEntityAdapter, TGeometryAdapter, TConfiguration>& core)
  {
    using TCore = OrthoTreeCoreBase<TEntityAdapter, TGeometryAdapter, TConfiguration>;
    auto serializedVersionID = TCore::SERIALIZED_VERSION_ID;
    ar& ORTHOTREE_NVP(serializedVersionID);
    ar& ORTHOTREE_NVP_M(core, m_maxElementNum);
    ar& ORTHOTREE_NVP_M(core, m_maxDepthID);
    ar& ORTHOTREE_NVP_M(core, m_nominalTreeBox);
    ar& ORTHOTREE_NVP_M(core, m_realTreeBox);
    ar& ORTHOTREE_NVP_M(core, m_nodeSize);
  }

  // StaticLinearOrthoTreeCore
  template<typename TArchive, typename TEntityAdapter, typename TGeometryAdapter, typename TConfiguration>
  void serialize(TArchive& ar, StaticLinearOrthoTreeCore<TEntityAdapter, TGeometryAdapter, TConfiguration>& core)
  {
    using TCore = StaticLinearOrthoTreeCore<TEntityAdapter, TGeometryAdapter, TConfiguration>;
    auto serializedVersionID = TCore::SERIALIZED_VERSION_ID;
    ar& ORTHOTREE_NVP(serializedVersionID);

    ar& make_nvp("base", static_cast<OrthoTreeCoreBase<TEntityAdapter, TGeometryAdapter, TConfiguration>&>(core));
    ar& ORTHOTREE_NVP_M(core, m_nodes);
    ar& ORTHOTREE_NVP_M(core, m_nodeDepthIDs);
    ar& ORTHOTREE_NVP_M(core, m_entityStorage);
    ar& ORTHOTREE_NVP_M(core, m_nodeGeometry);
  }


  // DynamicHashOrthoTreeCore
  template<typename TArchive, typename TEntityAdapter, typename TGeometryAdapter, typename TConfiguration>
  void serialize(TArchive& ar, DynamicHashOrthoTreeCore<TEntityAdapter, TGeometryAdapter, TConfiguration>& core)
  {
    using TCore = DynamicHashOrthoTreeCore<TEntityAdapter, TGeometryAdapter, TConfiguration>;

    ar& make_nvp("base", static_cast<TCore::Base&>(core));

    auto serializedVersionID = TCore::SERIALIZED_VERSION_ID;
    ar& ORTHOTREE_NVP(serializedVersionID);

    ar& ORTHOTREE_NVP_M(core, m_nodes);
    ar& ORTHOTREE_NVP_M(core, m_reverseMap);
    ar& ORTHOTREE_NVP_M(core, m_spaceIndexing);

    auto m_memoryResource = detail::MemoryResourceSerializerProxy(core.m_memoryResource, core.m_nodes);
    ar& ORTHOTREE_NVP(m_memoryResource);
  }


  // StaticBVHLinearCore
  template<typename TArchive, typename TEntityAdapter, typename TGeometryAdapter, typename TConfiguration>
  void serialize(TArchive& ar, StaticBVHLinearCore<TEntityAdapter, TGeometryAdapter, TConfiguration>& core)
  {
    using TCore = StaticBVHLinearCore<TEntityAdapter, TGeometryAdapter, TConfiguration>;

    auto serializedVersionID = TCore::SERIALIZED_VERSION_ID;
    ar& ORTHOTREE_NVP(serializedVersionID);

    ar& ORTHOTREE_NVP_M(core, m_maxDepthNo);
    ar& ORTHOTREE_NVP_M(core, m_maxElementNum);
    ar& ORTHOTREE_NVP_M(core, m_entityStorage);
    ar& ORTHOTREE_NVP_M(core, m_nodeGeometry);
    ar& ORTHOTREE_NVP_M(core, m_nodes);
  }


  // OrthoTreeQueryBase
  template<typename TArchive, typename TCore>
  void serialize(TArchive& ar, OrthoTreeQueryBase<TCore>& query)
  {
    ar& make_nvp("base", static_cast<TCore&>(query));
  }

  // OrthoTreeManaged
  template<typename TArchive, typename TCore>
  void serialize(TArchive& ar, OrthoTreeManaged<TCore>& managed)
  {
    ar& ORTHOTREE_NVP_M(managed, m_tree);
    ar& ORTHOTREE_NVP_M(managed, m_entities);
  }


  // EmbeddedResourcePmrMap
  template<typename TArchive, typename PmrMap>
  void serialize(TArchive& ar, detail::EmbeddedResourcePmrMap<PmrMap>& pmrMap)
  {
    auto& m_data = pmrMap.Underlying();
    ar& ORTHOTREE_NVP(m_data);
  }


#if defined(ORTHOTREE_SERIALIZATION_BOOST_ARCHIVE_AVAILABLE)
  // --- Boost Compatibility Bridge ---
  // Boost.Serialization looks here via ADL for a 3-parameter version.
  // We provide it here but hide it from Cereal archives using SFINAE,
  // because Cereal's versioning system can cause unwanted wrappers in JSON/XML.
  template<typename TArchive, typename T>
  inline auto serialize(TArchive& ar, T& t, const unsigned int /*version*/) -> std::enable_if_t<!detail::is_cereal_archive_v<TArchive>, void>
  {
    serialize(ar, t);
  }
#endif
} // namespace OrthoTree


namespace OrthoTree
{
  template<typename TArchive, typename T1, typename T2>
  auto serialize(TArchive& ar, std::pair<T1, T2>& val) -> std::enable_if_t<is_stl_serialization_enabled_v<TArchive>>
  {
    ar& make_nvp("key", const_cast<std::remove_const_t<T1>&>(val.first));
    ar& make_nvp("value", val.second);
  }

  template<typename TArchive>
  auto serialize(TArchive&, std::monostate&) -> std::enable_if_t<is_stl_serialization_enabled_v<TArchive>>
  {}

  template<typename TArchive>
  auto serialize(TArchive&, std::nullptr_t&) -> std::enable_if_t<is_stl_serialization_enabled_v<TArchive>>
  {}
} // namespace OrthoTree


namespace OrthoTree
{
  // std::array
  template<typename TArchive, typename T, std::size_t N>
  auto serialize(TArchive& ar, std::array<T, N>& val) -> std::enable_if_t<is_stl_serialization_enabled_v<TArchive>>
  {
    serialized_size_t size = static_cast<serialized_size_t>(N);
    ar& make_size_tag(size);
    for (auto& item : val)
      ar & item;
  }

} // namespace OrthoTree


namespace OrthoTree
{
  // std::map
  template<typename TArchive, typename K, typename V, typename Compare, typename Alloc>
  auto serialize(TArchive& ar, std::map<K, V, Compare, Alloc>& val) -> std::enable_if_t<is_stl_serialization_enabled_v<TArchive>>
  {
    serialized_size_t size = static_cast<serialized_size_t>(val.size());
    ar& make_size_tag(size);

    if (ar.is_loading())
    {
      val.clear();
      for (std::size_t i = 0; i < size; ++i)
      {
        std::pair<K, V> item;
        ar & item;
        val.emplace(std::move(item.first), std::move(item.second));
      }
    }
    else
    {
      for (auto& item : val)
      {
        ar& const_cast<std::pair<K const, V>&>(item);
      }
    }
  }

} // namespace OrthoTree


namespace OrthoTree
{
  // std::unordered_map
  template<typename TArchive, typename K, typename V, typename Hash, typename KeyEqual, typename Alloc>
  auto serialize(TArchive& ar, std::unordered_map<K, V, Hash, KeyEqual, Alloc>& val) -> std::enable_if_t<is_stl_serialization_enabled_v<TArchive>>
  {
    serialized_size_t size = static_cast<serialized_size_t>(val.size());
    ar& make_size_tag(size);

    if (ar.is_loading())
    {
      val.clear();
      for (std::size_t i = 0; i < size; ++i)
      {
        std::pair<K, V> item;
        ar & item;
        val.emplace(std::move(item.first), std::move(item.second));
      }
    }
    else
    {
      for (auto& item : val)
      {
        ar& const_cast<std::pair<K const, V>&>(item);
      }
    }
  }

} // namespace OrthoTree


namespace OrthoTree
{
  // std::optional
  template<typename TArchive, typename T>
  auto serialize(TArchive& ar, std::optional<T>& val) -> std::enable_if_t<is_stl_serialization_enabled_v<TArchive>>
  {
    bool has_value = val.has_value();
    ar& make_nvp("has_value", has_value);

    if (ar.is_loading())
    {
      if (has_value)
      {
        T item;
        ar& make_nvp("value", item);
        val = std::move(item);
      }
      else
        val = std::nullopt;
    }
    else
    {
      if (has_value)
        ar& make_nvp("value", *val);
    }
  }

} // namespace OrthoTree


namespace OrthoTree
{
  // std::unique_ptr
  template<typename TArchive, typename T, typename D>
  auto serialize(TArchive& ar, std::unique_ptr<T, D>& val) -> std::enable_if_t<is_stl_serialization_enabled_v<TArchive>>
  {
    bool has_value = (val != nullptr);
    ar& make_nvp("has_value", has_value);

    if (ar.is_loading())
    {
      if (has_value)
      {
        T item;
        ar& make_nvp("value", item);
        val = std::unique_ptr<T, D>(new T(std::move(item)));
      }
      else
        val.reset();
    }
    else
    {
      if (has_value)
        ar& make_nvp("value", *val);
    }
  }

  // std::shared_ptr
  template<typename TArchive, typename T>
  auto serialize(TArchive& ar, std::shared_ptr<T>& val) -> std::enable_if_t<is_stl_serialization_enabled_v<TArchive>>
  {
    bool has_value = (val != nullptr);
    ar& make_nvp("has_value", has_value);

    if (ar.is_loading())
    {
      if (has_value)
      {
        T item;
        ar& make_nvp("value", item);
        val = std::make_shared<T>(std::move(item));
      }
      else
        val.reset();
    }
    else
    {
      if (has_value)
        ar& make_nvp("value", *val);
    }
  }

} // namespace OrthoTree


namespace OrthoTree
{
  // std::set
  template<typename TArchive, typename K, typename Compare, typename Alloc>
  auto serialize(TArchive& ar, std::set<K, Compare, Alloc>& val) -> std::enable_if_t<is_stl_serialization_enabled_v<TArchive>>
  {
    serialized_size_t size = static_cast<serialized_size_t>(val.size());
    ar& make_size_tag(size);

    if (ar.is_loading())
    {
      val.clear();
      for (std::size_t i = 0; i < size; ++i)
      {
        K key;
        ar & key;
        val.emplace(std::move(key));
      }
    }
    else
    {
      for (auto& item : val)
        ar& const_cast<K&>(item);
    }
  }

} // namespace OrthoTree


namespace OrthoTree
{
  // std::variant
  template<typename TArchive, typename... Types>
  auto serialize(TArchive& ar, std::variant<Types...>& val) -> std::enable_if_t<is_stl_serialization_enabled_v<TArchive>>
  {
    serialized_size_t index = static_cast<serialized_size_t>(val.index());
    ar& make_nvp("index", index);

    if (ar.is_loading())
    {
      auto load_variant_index = [&]<std::size_t I>(std::integral_constant<std::size_t, I>) {
        if (I == index)
        {
          using T = std::variant_alternative_t<I, std::variant<Types...>>;
          T item;
          ar& make_nvp("value", item);
          val = std::move(item);
          return true;
        }
        return false;
      };

      auto visitor = [&]<std::size_t... Is>(std::index_sequence<Is...>) {
        (load_variant_index(std::integral_constant<std::size_t, Is>{}) || ...);
      };

      visitor(std::index_sequence_for<Types...>{});
    }
    else
    {
      std::visit([&ar](auto& item) { ar& make_nvp("value", item); }, val);
    }
  }

} // namespace OrthoTree


#ifdef __cpp_lib_constexpr_inplace_vector
#include <inplace_vector>
#endif

namespace OrthoTree
{
  // std::vector
  template<typename TArchive, typename T, typename Alloc>
  auto serialize(TArchive& ar, std::vector<T, Alloc>& val) -> std::enable_if_t<is_stl_serialization_enabled_v<TArchive>>
  {
    serialized_size_t size = static_cast<serialized_size_t>(val.size());
    ar& make_size_tag(size);
    if (ar.is_loading())
    {
      val.resize(size);
      for (auto& item : val)
        ar & item;
    }
    else
    {
      for (auto& item : val)
        ar & item;
    }
  }

#ifdef __cpp_lib_constexpr_inplace_vector
  // std::inplace_vector (C++26)
  template<typename TArchive, typename T, std::size_t N>
  auto serialize(TArchive& ar, std::inplace_vector<T, N>& val) -> std::enable_if_t<is_stl_serialization_enabled_v<TArchive>>
  {
    serialized_size_t size = static_cast<serialized_size_t>(val.size());
    ar& make_size_tag(size);
    if (ar.is_loading())
    {
      val.clear();
      for (std::size_t i = 0; i < size; ++i)
      {
        val.push_back(load_construct<TArchive, T>::load(ar));
      }
    }
    else
    {
      for (auto& item : val)
        ar & item;
    }
  }
#endif

} // namespace OrthoTree


#ifndef ORTHOTREE_SERIALIZATION_BINARY_ARCHIVE_H
#define ORTHOTREE_SERIALIZATION_BINARY_ARCHIVE_H
#endif


namespace OrthoTree
{
  // Endianness handling
  enum class Endianness
  {
    Little = 0,
    Big = 1
  };

  inline constexpr Endianness native_endian = (std::endian::native == std::endian::little) ? Endianness::Little : Endianness::Big;

  // Swap bytes for endian conversion
  template<typename T>
  T swap_endian(T val)
  {
    static_assert(std::is_arithmetic_v<T>, "swap_endian only works for arithmetic types");
    T res;
    auto* src = reinterpret_cast<const unsigned char*>(&val);
    auto* dst = reinterpret_cast<unsigned char*>(&res);
    for (std::size_t i = 0; i < sizeof(T); ++i)
    {
      dst[i] = src[sizeof(T) - 1 - i];
    }
    return res;
  }

  class BinaryArchive
  {
  protected:
    std::iostream& m_stream;
    bool m_is_loading;
    Endianness m_target_endian;

  public:
    BinaryArchive(std::iostream& stream, bool is_loading, Endianness target_endian = Endianness::Little)
    : m_stream(stream)
    , m_is_loading(is_loading)
    , m_target_endian(target_endian)
    {}

    constexpr bool is_loading() const { return m_is_loading; }
    constexpr bool is_saving() const { return !m_is_loading; }

    template<typename T>
    BinaryArchive& operator&(T& val)
    {
      if constexpr (std::is_arithmetic_v<T>)
      {
        if (m_is_loading)
        {
          m_stream.read(reinterpret_cast<char*>(&val), sizeof(T));
          if (m_target_endian != native_endian)
            val = swap_endian(val);
        }
        else
        {
          T out = val;
          if (m_target_endian != native_endian)
            out = swap_endian(val);
          m_stream.write(reinterpret_cast<const char*>(&out), sizeof(T));
        }
      }
      else if constexpr (std::is_same_v<T, std::string>)
      {
        if (m_is_loading)
        {
          serialized_size_t size;
          (*this)(size);
          val.resize(size);
          m_stream.read(val.data(), size);
        }
        else
        {
          serialized_size_t size = static_cast<serialized_size_t>(val.size());
          (*this)(size);
          m_stream.write(val.data(), size);
        }
      }
      else if constexpr (requires { val.serialize(*this); })
      {
        val.serialize(*this);
      }
      else
      {
        serialize(*this, val);
      }
      return *this;
    }

    template<typename T>
    BinaryArchive& operator&(const T& val)
    {
      if (m_is_loading)
      {
        // This should never happen if used correctly via make_nvp for const objects
        // but we need it for compilation of saving paths.
        throw std::runtime_error("Cannot load into const object");
      }

      if constexpr (std::is_arithmetic_v<T>)
      {
        T out = val;
        if (m_target_endian != native_endian)
          out = swap_endian(val);
        m_stream.write(reinterpret_cast<const char*>(&out), sizeof(T));
      }
      else if constexpr (std::is_same_v<T, std::string>)
      {
        serialized_size_t size = static_cast<serialized_size_t>(val.size());
        (*this)(size);
        m_stream.write(val.data(), size);
      }
      else if constexpr (requires { val.serialize(*this); })
      {
        const_cast<T&>(val).serialize(*this);
      }
      else
      {
        serialize(*this, const_cast<T&>(val));
      }
      return *this;
    }

    // SizeTag: write/read the size value
    template<typename T>
    BinaryArchive& operator&(SizeTag<T> tag)
    {
      return (*this & tag.value);
    }

    // Support any type that is an NVP
    template<typename TNVP>
      requires(is_nvp_v<TNVP>)
    auto operator&(TNVP&& nvp) -> BinaryArchive&
    {
      return (*this & detail::get_nvp_value(nvp));
    }

    template<typename... Args>
    BinaryArchive& operator()(Args&&... args)
    {
      ((*this & std::forward<Args>(args)), ...);
      return *this;
    }
  };

  class BinaryOutputArchive : public BinaryArchive
  {
  public:
    BinaryOutputArchive(std::iostream& stream, Endianness target_endian = Endianness::Little)
    : BinaryArchive(stream, false, target_endian)
    {}
  };

  class BinaryInputArchive : public BinaryArchive
  {
  public:
    BinaryInputArchive(std::iostream& stream, Endianness target_endian = Endianness::Little)
    : BinaryArchive(stream, true, target_endian)
    {}
  };

  // Specialization for traits
  template<>
  struct is_orthotree_archive<BinaryArchive> : std::true_type
  {};
  template<>
  struct is_orthotree_archive<BinaryOutputArchive> : std::true_type
  {};
  template<>
  struct is_orthotree_archive<BinaryInputArchive> : std::true_type
  {};

  template<>
  struct is_stl_serialization_enabled<BinaryArchive> : std::true_type
  {};
  template<>
  struct is_stl_serialization_enabled<BinaryOutputArchive> : std::true_type
  {};
  template<>
  struct is_stl_serialization_enabled<BinaryInputArchive> : std::true_type
  {};

} // namespace OrthoTree

// --- Start of Guarded Adapter: MSGPACK ---
#if defined(ORTHOTREE_SERIALIZATION_MSGPACK_SUPPORT) || defined(MSGPACK_HPP)


#ifndef ORTHOTREE_SERIALIZATION_MSGPACK_ARCHIVE_H
#define ORTHOTREE_SERIALIZATION_MSGPACK_ARCHIVE_H
#endif


#ifndef ORTHOTREE_SERIALIZATION_MSGPACK_INCLUDE
#define ORTHOTREE_SERIALIZATION_MSGPACK_INCLUDE <msgpack.hpp>
#endif

#include ORTHOTREE_SERIALIZATION_MSGPACK_INCLUDE


namespace OrthoTree
{
  class MsgPackArchive;

  // Specialization for traits
  template<>
  struct is_orthotree_archive<MsgPackArchive> : std::true_type
  {};

  template<>
  struct is_stl_serialization_enabled<MsgPackArchive> : std::true_type
  {};

  // A simple MsgPack Output Archive that works with our serialize functions
  class MsgPackArchive
  {
  protected:
    msgpack::sbuffer m_buffer;
    msgpack::packer<msgpack::sbuffer> m_packer;
    std::ostream& m_os;

  public:
    MsgPackArchive(std::ostream& os)
    : m_packer(m_buffer)
    , m_os(os)
    {}

    ~MsgPackArchive() { m_os.write(m_buffer.data(), m_buffer.size()); }

    constexpr bool is_loading() const { return false; }
    constexpr bool is_saving() const { return true; }

    // SizeTag: MsgPack uses array headers
    template<typename T>
    MsgPackArchive& operator&(SizeTag<T> tag)
    {
      m_packer.pack_array(static_cast<serialized_size_t>(tag.value));
      return *this;
    }

    // NameValuePair: We can pack as a map or just the value.
    // To stay consistent with JSON/XML, we'll use maps if it's a named object.
    template<typename TNVP>
      requires(requires(TNVP nvp) {
        detail::get_nvp_name(nvp);
        detail::get_nvp_value(nvp);
      })
    MsgPackArchive& operator&(TNVP&& nvp)
    {
      // For simplicity in this bridge, we pack the value directly.
      // If full Map support is needed, we'd need to track scope.
      serialize_value(detail::get_nvp_value(nvp));
      return *this;
    }

    template<typename T>
    auto operator&(T& val) -> std::enable_if_t<!is_size_tag_v<std::remove_cv_t<T>>, MsgPackArchive&>
    {
      serialize_value(val);
      return *this;
    }

    template<typename... Args>
    MsgPackArchive& operator()(Args&&... args)
    {
      ((*this & std::forward<Args>(args)), ...);
      return *this;
    }

  protected:
    template<typename T>
    void serialize_value(T& val)
    {
      if constexpr (std::is_arithmetic_v<T>)
      {
        m_packer.pack(val);
      }
      else if constexpr (std::is_same_v<T, std::string>)
      {
        m_packer.pack(val);
      }
      else if constexpr (std::is_same_v<T, std::monostate>)
      {
        m_packer.pack_nil();
      }
      else if constexpr (std::is_same_v<T, std::nullptr_t>)
      {
        m_packer.pack_nil();
      }
      else if constexpr (requires { val.serialize(*this); })
      {
        val.serialize(*this);
      }
      else if constexpr (is_stl_serialization_enabled_v<MsgPackArchive>)
      {
        serialize(*this, val);
      }
      else
      {
        static_assert(false, "Type not serializable");
      }
    }
  };
} // namespace OrthoTree

#endif
// --- End of Guarded Adapter: MSGPACK ---


namespace BasicTypesXYZ // Replaceable with similarly build geometry types
{
  using float_t = float;

  struct Point2D
  {
    float_t x;
    float_t y;
  };

  struct Point3D
  {
    float_t x;
    float_t y;
    float_t z;
  };

  struct BoundingBox2D
  {
    Point2D Min;
    Point2D Max;
  };

  struct BoundingBox3D
  {
    Point3D Min;
    Point3D Max;
  };

  struct Ray2D
  {
    Point2D BasePoint;
    Point2D Heading;
  };

  struct Ray3D
  {
    Point3D BasePoint;
    Point3D Heading;
  };

  struct Plane2D
  {
    float_t OrigoDistance;
    Point2D Normal;
  };

  struct Plane3D
  {
    float_t OrigoDistance;
    Point3D Normal;
  };
} // namespace BasicTypesXYZ


namespace OrthoTree
{
  namespace XYAdaptor2D
  {
    using xy_geometry_type = BasicTypesXYZ::float_t;
    using XYPoint2D = BasicTypesXYZ::Point2D;
    using XYBoundingBox2D = BasicTypesXYZ::BoundingBox2D;
    using XYRay2D = BasicTypesXYZ::Ray2D;
    using XYPlane2D = BasicTypesXYZ::Plane2D;

    struct XYBaseGeometryAdapter
    {
      using Scalar = xy_geometry_type;
      using FloatScalar = xy_geometry_type;
      using Vector = XYPoint2D;
      using Box = XYBoundingBox2D;
      using Ray = XYRay2D;
      using Plane = XYPlane2D;

      static constexpr dim_t DIMENSION_NO = 2;
      static constexpr FloatScalar BASE_TOLERANCE = std::numeric_limits<FloatScalar>::epsilon() * FloatScalar(10);

      static constexpr XYPoint2D MakePoint() noexcept { return {}; };
      static constexpr XYBoundingBox2D MakeBox() noexcept { return {}; };

      static constexpr xy_geometry_type GetPointC(XYPoint2D const& pt, dim_t dimensionID)
      {
        switch (dimensionID)
        {
        case 0: return pt.x;
        case 1: return pt.y;
        default: assert(false); std::terminate();
        }
      }

      static constexpr void SetPointC(XYPoint2D& pt, dim_t dimensionID, xy_geometry_type value)
      {
        switch (dimensionID)
        {
        case 0: pt.x = value; break;
        case 1: pt.y = value; break;
        default: assert(false); std::terminate();
        }
      }


      static constexpr void SetBoxMinC(XYBoundingBox2D& box, dim_t dimensionID, xy_geometry_type value) { SetPointC(box.Min, dimensionID, value); }
      static constexpr void SetBoxMaxC(XYBoundingBox2D& box, dim_t dimensionID, xy_geometry_type value) { SetPointC(box.Max, dimensionID, value); }
      static constexpr xy_geometry_type GetBoxMinC(XYBoundingBox2D const& box, dim_t dimensionID) { return GetPointC(box.Min, dimensionID); }
      static constexpr xy_geometry_type GetBoxMaxC(XYBoundingBox2D const& box, dim_t dimensionID) { return GetPointC(box.Max, dimensionID); }

      static constexpr XYPoint2D const& GetRayDirection(XYRay2D const& ray) noexcept { return ray.Heading; }
      static constexpr XYPoint2D const& GetRayOrigin(XYRay2D const& ray) noexcept { return ray.BasePoint; }

      static constexpr XYPoint2D const& GetPlaneNormal(XYPlane2D const& plane) noexcept { return plane.Normal; }
      static constexpr xy_geometry_type GetPlaneOrigoDistance(XYPlane2D const& plane) noexcept { return plane.OrigoDistance; }
    };

    using XYGeometryAdapter = GeneralGeometryAdapter<XYBaseGeometryAdapter>;
  } // namespace XYAdaptor2D


  namespace XYZAdaptor3D
  {
    using xyz_geometry_type = BasicTypesXYZ::float_t;
    using XYZPoint3D = BasicTypesXYZ::Point3D;
    using XYZBoundingBox3D = BasicTypesXYZ::BoundingBox3D;
    using XYZRay3D = BasicTypesXYZ::Ray3D;
    using XYZPlane3D = BasicTypesXYZ::Plane3D;

    struct XYZAdaptorBasics
    {
      using Scalar = xyz_geometry_type;
      using FloatScalar = xyz_geometry_type;
      using Vector = XYZPoint3D;
      using Box = XYZBoundingBox3D;
      using Ray = XYZRay3D;
      using Plane = XYZPlane3D;

      static constexpr dim_t DIMENSION_NO = 3;
      static constexpr FloatScalar BASE_TOLERANCE = std::numeric_limits<FloatScalar>::epsilon() * FloatScalar(10);

      static constexpr XYZPoint3D MakePoint() noexcept { return {}; };
      static constexpr XYZBoundingBox3D MakeBox() noexcept { return {}; };

      static constexpr xyz_geometry_type GetPointC(XYZPoint3D const& pt, dim_t dimensionID) noexcept
      {
        switch (dimensionID)
        {
        case 0: return pt.x;
        case 1: return pt.y;
        case 2: return pt.z;
        default: assert(false); std::terminate();
        }
      }

      static constexpr void SetPointC(XYZPoint3D& pt, dim_t dimensionID, xyz_geometry_type value) noexcept
      {
        switch (dimensionID)
        {
        case 0: pt.x = value; break;
        case 1: pt.y = value; break;
        case 2: pt.z = value; break;
        default: assert(false); std::terminate();
        }
      }

      static constexpr void SetBoxMinC(XYZBoundingBox3D& box, dim_t dimensionID, xyz_geometry_type value) { SetPointC(box.Min, dimensionID, value); }
      static constexpr void SetBoxMaxC(XYZBoundingBox3D& box, dim_t dimensionID, xyz_geometry_type value) { SetPointC(box.Max, dimensionID, value); }
      static constexpr xyz_geometry_type GetBoxMinC(XYZBoundingBox3D const& box, dim_t dimensionID) { return GetPointC(box.Min, dimensionID); }
      static constexpr xyz_geometry_type GetBoxMaxC(XYZBoundingBox3D const& box, dim_t dimensionID) { return GetPointC(box.Max, dimensionID); }

      static constexpr XYZPoint3D const& GetRayDirection(XYZRay3D const& ray) noexcept { return ray.Heading; }
      static constexpr XYZPoint3D const& GetRayOrigin(XYZRay3D const& ray) noexcept { return ray.BasePoint; }

      static constexpr XYZPoint3D const& GetPlaneNormal(XYZPlane3D const& plane) noexcept { return plane.Normal; }
      static constexpr xyz_geometry_type GetPlaneOrigoDistance(XYZPlane3D const& plane) noexcept { return plane.OrigoDistance; }
    };

    using XYZGeometryAdapter = GeneralGeometryAdapter<XYZAdaptorBasics>;


  } // namespace XYZAdaptor3D
} // namespace OrthoTree

namespace XYZ
{
  using namespace OrthoTree;
  using namespace OrthoTree::XYAdaptor2D;
  using namespace OrthoTree::XYZAdaptor3D;

  using QuadtreePoint = OrthoTreeBase<PointEntitySpanAdapter<XYPoint2D>, XYGeometryAdapter, PointConfiguration<>>;

  template<bool IS_LOOSE_TREE = true>
  using QuadtreeBox = OrthoTreeBase<BoxEntitySpanAdapter<XYBoundingBox2D>, XYGeometryAdapter, BoxConfiguration<IS_LOOSE_TREE>>;

  using QuadtreePointM = OrthoTreeManaged<QuadtreePoint>;

  template<bool IS_LOOSE_TREE = true>
  using QuadtreeBoxCs = OrthoTreeManaged<QuadtreeBox<IS_LOOSE_TREE>>;
  using QuadtreeBoxM = QuadtreeBoxCs<true>;

  using OctreePoint = OrthoTreeBase<PointEntitySpanAdapter<XYZPoint3D>, XYZGeometryAdapter, PointConfiguration<>>;

  template<bool IS_LOOSE_TREE = true>
  using OctreeBox = OrthoTreeBase<BoxEntitySpanAdapter<XYZBoundingBox3D>, XYZGeometryAdapter, BoxConfiguration<IS_LOOSE_TREE>>;


  using OcreePointC = OrthoTreeManaged<OctreePoint>;

  template<bool IS_LOOSE_TREE = true>
  using OctreeBoxCs = OrthoTreeManaged<OctreeBox<IS_LOOSE_TREE>>;
  using OctreeBoxM = OctreeBoxCs<true>;


  // Map types
  template<typename Entity>
  using Map = std::unordered_map<int, Entity>;

  using QuadtreePointMap = OrthoTreeBase<PointEntityMapAdapter<XYPoint2D>, XYGeometryAdapter, PointConfiguration<>>;

  template<bool IS_LOOSE_TREE = true>
  using QuadtreeBoxMap = OrthoTreeBase<BoxEntityMapAdapter<XYBoundingBox2D>, XYGeometryAdapter, BoxConfiguration<IS_LOOSE_TREE>>;

  using QuadtreePointMapM = OrthoTreeManaged<QuadtreePointMap>;

  template<bool IS_LOOSE_TREE = true>
  using QuadtreeBoxMapCs = OrthoTreeManaged<QuadtreeBoxMap<IS_LOOSE_TREE>>;
  using QuadtreeBoxMapM = QuadtreeBoxMapCs<true>;

  using OctreePointMap = OrthoTreeBase<PointEntityMapAdapter<XYZPoint3D>, XYZGeometryAdapter, PointConfiguration<>>;

  template<bool IS_LOOSE_TREE = true>
  using OctreeBoxMap = OrthoTreeBase<BoxEntityMapAdapter<XYZBoundingBox3D>, XYZGeometryAdapter, BoxConfiguration<IS_LOOSE_TREE>>;

  using OcreePointMapC = OrthoTreeManaged<OctreePointMap>;

  template<bool IS_LOOSE_TREE = true>
  using OctreeBoxMapCs = OrthoTreeManaged<OctreeBoxMap<IS_LOOSE_TREE>>;
  using OctreeBoxMapM = OctreeBoxMapCs<true>;
} // namespace XYZ

namespace OrthoTree
{
  // BasicTypesXYZ::Point2D
  template<typename TArchive>
  void serialize(TArchive& ar, BasicTypesXYZ::Point2D& pt)
  {
    ar& ORTHOTREE_NVP_M(pt, x);
    ar& ORTHOTREE_NVP_M(pt, y);
  }

  // BasicTypesXYZ::Point3D
  template<typename TArchive>
  void serialize(TArchive& ar, BasicTypesXYZ::Point3D& pt)
  {
    ar& ORTHOTREE_NVP_M(pt, x);
    ar& ORTHOTREE_NVP_M(pt, y);
    ar& ORTHOTREE_NVP_M(pt, z);
  }

  // BasicTypesXYZ::BoundingBox2D
  template<typename TArchive>
  void serialize(TArchive& ar, BasicTypesXYZ::BoundingBox2D& box)
  {
    ar& ORTHOTREE_NVP_M(box, Min);
    ar& ORTHOTREE_NVP_M(box, Max);
  }

  // BasicTypesXYZ::BoundingBox3D
  template<typename TArchive>
  void serialize(TArchive& ar, BasicTypesXYZ::BoundingBox3D& box)
  {
    ar& ORTHOTREE_NVP_M(box, Min);
    ar& ORTHOTREE_NVP_M(box, Max);
  }

  // BasicTypesXYZ::Ray2D
  template<typename TArchive>
  void serialize(TArchive& ar, BasicTypesXYZ::Ray2D& ray)
  {
    ar& ORTHOTREE_NVP_M(ray, BasePoint);
    ar& ORTHOTREE_NVP_M(ray, Heading);
  }

  // BasicTypesXYZ::Ray3D
  template<typename TArchive>
  void serialize(TArchive& ar, BasicTypesXYZ::Ray3D& ray)
  {
    ar& ORTHOTREE_NVP_M(ray, BasePoint);
    ar& ORTHOTREE_NVP_M(ray, Heading);
  }

  // BasicTypesXYZ::Plane2D
  template<typename TArchive>
  void serialize(TArchive& ar, BasicTypesXYZ::Plane2D& plane)
  {
    ar& ORTHOTREE_NVP_M(plane, OrigoDistance);
    ar& ORTHOTREE_NVP_M(plane, Normal);
  }

  // BasicTypesXYZ::Plane3D
  template<typename TArchive>
  void serialize(TArchive& ar, BasicTypesXYZ::Plane3D& plane)
  {
    ar& ORTHOTREE_NVP_M(plane, OrigoDistance);
    ar& ORTHOTREE_NVP_M(plane, Normal);
  }
} // namespace OrthoTree

// --- Start of Guarded Adapter: EIGEN ---
#if defined(ORTHOTREE_EIGEN_SUPPORT) || defined(EIGEN_GEOMETRY_MODULE_H)


// --- Start of Guarded Adapter: EIGEN ---
#if defined(ORTHOTREE_EIGEN_SUPPORT) || defined(EIGEN_GEOMETRY_MODULE_H)


#ifdef _MSC_VER
// Warning: std::optional + Eigen::AlignedBox: "structure was padded due to alignment specifier"
// Disable: This value will be immediately converted to another type, there is no alignment relevant instructions.
#pragma warning(disable : 4324)
#endif


// Override the Eigen include if your Eigen installation is in a non-standard location.
// Example: -DORTHOTREE_EIGEN_INCLUDE="<my_vendor/Eigen/Geometry>"
#ifndef ORTHOTREE_EIGEN_INCLUDE
#define ORTHOTREE_EIGEN_INCLUDE <Eigen/Geometry>
#endif

#include ORTHOTREE_EIGEN_INCLUDE

namespace OrthoTree
{
  namespace EigenAdapter
  {
    using namespace Eigen;

    template<typename TScalar, int AmbientDim>
    struct EigenBaseGeometryAdapter
    {
      using Scalar = TScalar;
      using FloatScalar = std::conditional_t<std::is_floating_point_v<Scalar>, Scalar, float>;
      using Vector = Matrix<Scalar, AmbientDim, 1>;
      using Box = AlignedBox<Scalar, AmbientDim>;
      using Ray = ParametrizedLine<Scalar, AmbientDim>;
      using Plane = Hyperplane<Scalar, AmbientDim>;

      static constexpr dim_t DIMENSION_NO = AmbientDim;
      static constexpr FloatScalar BASE_TOLERANCE = std::numeric_limits<FloatScalar>::epsilon() * FloatScalar(10);

      static constexpr Vector MakePoint() noexcept { return {}; };
      static constexpr Box MakeBox() noexcept { return {}; };

      static constexpr Scalar GetPointC(Vector const& point, dim_t dimensionID) noexcept { return point(dimensionID); }
      static constexpr void SetPointC(Vector& point, dim_t dimensionID, Scalar value) noexcept { point(dimensionID) = value; }

      static constexpr Scalar GetBoxMinC(Box const& box, dim_t dimensionID) { return box.min()(dimensionID); }
      static constexpr Scalar GetBoxMaxC(Box const& box, dim_t dimensionID) { return box.max()(dimensionID); }
      static constexpr void SetBoxMinC(Box& box, dim_t dimensionID, Scalar value) { box.min()(dimensionID) = value; }
      static constexpr void SetBoxMaxC(Box& box, dim_t dimensionID, Scalar value) { box.max()(dimensionID) = value; }

      static constexpr Vector const& GetRayDirection(Ray const& ray) noexcept { return ray.direction(); }
      static constexpr Vector const& GetRayOrigin(Ray const& ray) noexcept { return ray.origin(); }

      static constexpr Vector GetPlaneNormal(Plane const& plane) noexcept { return plane.normal(); }
      static constexpr Scalar GetPlaneOrigoDistance(Plane const& plane) noexcept { return -plane.offset(); }
    };

    template<typename TScalar, int AmbientDim>
    struct EigenGeometryAdapter : GeneralGeometryAdapter<EigenBaseGeometryAdapter<TScalar, AmbientDim>>
    {
      using Base = EigenBaseGeometryAdapter<TScalar, AmbientDim>;

      using Scalar = Base::Scalar;
      using FloatScalar = Base::FloatScalar;
      using Vector = Base::Vector;
      using Box = Base::Box;
      using Ray = Base::Ray;
      using Plane = Base::Plane;

      static_assert(BaseGeometryAdapterConcept<Base, AmbientDim, Vector, Box, Ray, Plane, Scalar, FloatScalar>);

      static constexpr Scalar Size2(Vector const& v) noexcept { return v.squaredNorm(); }

      static constexpr Scalar Size(Vector const& v) noexcept { return v.norm(); }

      static constexpr Vector Add(Vector const& v1, Vector const& v2) noexcept { return v1 + v2; }

      static constexpr Vector Subtract(Vector const& v1, Vector const& v2) noexcept { return v1 - v2; }

      static constexpr Vector Multiply(Vector const& v, FloatScalar scalarFactor) noexcept { return v * scalarFactor; }

      static constexpr Scalar Dot(Vector const& v1, Vector const& v2) noexcept { return v1.dot(v2); }

      static constexpr Scalar Distance(Vector const& v1, Vector const& v2) noexcept { return Size(v1 - v2); }

      static constexpr Scalar Distance2(Vector const& v1, Vector const& v2) noexcept { return Size2(v1 - v2); }

      static constexpr bool ArePointsEqual(Vector const& v1, Vector const& v2, FloatScalar tolerance) noexcept
      {
        return Distance2(v1, v2) <= tolerance * tolerance;
      }

      static constexpr bool IsNormalizedVector(Vector const& normal) noexcept { return std::abs(Size2(normal) - 1.0) < 0.000001; }

      static Box GetBoxOfPoints(std::span<Vector const> const& points) noexcept
      {
        auto ext = points.size() == 0 ? Box{} : Box(points[0]);
        for (auto const& point : points)
          ext.extend(point);

        return ext;
      }

      static Box GetBoxOfBoxes(std::span<Box const> const& extents) noexcept
      {
        auto ext = extents.size() == 0 ? Box{} : extents[0];
        for (auto const& extent : extents)
          ext.extend(extent);

        return ext;
      }

      static void MoveBox(Box& box, Vector const& moveVector) noexcept { box.translate(moveVector); }

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

  } // namespace EigenAdapter
} // namespace OrthoTree


namespace Eigen
{
  using namespace OrthoTree::EigenAdapter;

  // Basic OrthoTree types
  template<typename TScalar, int AmbientDim>
  using EigenOrthoTreePointSpan =
    OrthoTree::OrthoTreeBase<OrthoTree::PointEntitySpanAdapter<Matrix<TScalar, AmbientDim, 1>>, EigenGeometryAdapter<TScalar, AmbientDim>, OrthoTree::PointConfiguration<>>;

  template<typename TScalar, int AmbientDim, bool IS_LOOSE_TREE = true>
  using EigenOrthoTreeBoxSpan =
    OrthoTree::OrthoTreeBase<OrthoTree::BoxEntitySpanAdapter<AlignedBox<TScalar, AmbientDim>>, EigenGeometryAdapter<TScalar, AmbientDim>, OrthoTree::BoxConfiguration<IS_LOOSE_TREE>>;

  template<typename TScalar, int AmbientDim, typename TContainer = std::unordered_map<OrthoTree::index_t, AlignedBox<TScalar, AmbientDim>>>
  using EigenOrthoTreePointMap =
    OrthoTree::OrthoTreeBase<OrthoTree::PointEntityMapAdapter<Matrix<TScalar, AmbientDim, 1>, TContainer>, EigenGeometryAdapter<TScalar, AmbientDim>, OrthoTree::PointConfiguration<>>;

  template<typename TScalar, int AmbientDim, bool IS_LOOSE_TREE = true, typename TContainer = std::unordered_map<OrthoTree::index_t, AlignedBox<TScalar, AmbientDim>>>
  using EigenOrthoTreeBoxMap = OrthoTree::OrthoTreeBase<
    OrthoTree::BoxEntityMapAdapter<AlignedBox<TScalar, AmbientDim>, TContainer>,
    EigenGeometryAdapter<TScalar, AmbientDim>,
    OrthoTree::BoxConfiguration<IS_LOOSE_TREE>>;


  template<typename TScalar, int AmbientDim>
  using OrthoTreeManagedPoint = OrthoTree::OrthoTreeManaged<EigenOrthoTreePointSpan<TScalar, AmbientDim>>;

  template<typename TScalar, int AmbientDim, bool IS_LOOSE_TREE = true>
  using OrthoTreeManagedBox = OrthoTree::OrthoTreeManaged<EigenOrthoTreeBoxSpan<TScalar, AmbientDim, IS_LOOSE_TREE>>;

  template<typename TScalar, int AmbientDim, bool IS_LOOSE_TREE = true, typename TContainer = std::unordered_map<OrthoTree::index_t, AlignedBox<TScalar, AmbientDim>>>
  using OrthoTreeManagedPointMap = OrthoTree::OrthoTreeManaged<EigenOrthoTreePointMap<TScalar, AmbientDim, TContainer>>;

  template<typename TScalar, int AmbientDim, bool IS_LOOSE_TREE = true, typename TContainer = std::unordered_map<OrthoTree::index_t, AlignedBox<TScalar, AmbientDim>>>
  using OrthoTreeManagedBoxMap = OrthoTree::OrthoTreeManaged<EigenOrthoTreeBoxMap<TScalar, AmbientDim, IS_LOOSE_TREE, TContainer>>;

  // Non-owning types
  using QuadtreePoint2f = EigenOrthoTreePointSpan<float, 2>;

  using QuadtreePoint2d = EigenOrthoTreePointSpan<double, 2>;

  using OctreePoint3f = EigenOrthoTreePointSpan<float, 3>;

  using OctreePoint3d = EigenOrthoTreePointSpan<double, 3>;

  template<bool IS_LOOSE_TREE = true>
  using QuadtreeBox2fs = EigenOrthoTreeBoxSpan<float, 2, IS_LOOSE_TREE>;
  using QuadtreeBox2f = QuadtreeBox2fs<true>;

  template<bool IS_LOOSE_TREE = true>
  using QuadtreeBox2ds = EigenOrthoTreeBoxSpan<double, 2, IS_LOOSE_TREE>;
  using QuadtreeBox2d = QuadtreeBox2ds<true>;

  template<bool IS_LOOSE_TREE = true>
  using OctreeBox3fs = EigenOrthoTreeBoxSpan<float, 3, IS_LOOSE_TREE>;
  using OctreeBox3f = OctreeBox3fs<true>;

  template<bool IS_LOOSE_TREE = true>
  using OctreeBox3ds = EigenOrthoTreeBoxSpan<double, 3, IS_LOOSE_TREE>;
  using OctreeBox3d = OctreeBox3ds<true>;

  // Container types
  using QuadtreePointMd2f = OrthoTreeManagedPoint<float, 2>;

  using QuadtreePointM2d = OrthoTreeManagedPoint<double, 2>;

  using OctreePointM3f = OrthoTreeManagedPoint<float, 3>;

  using OctreePointM3d = OrthoTreeManagedPoint<double, 3>;

  template<bool IS_LOOSE_TREE = true>
  using QuadtreeBoxM2fs = OrthoTreeManagedBox<float, 2, IS_LOOSE_TREE>;
  using QuadtreeBoxM2f = QuadtreeBoxM2fs<true>;

  template<bool IS_LOOSE_TREE = true>
  using QuadtreeBoxM2ds = OrthoTreeManagedBox<double, 2, IS_LOOSE_TREE>;
  using QuadtreeBoxM2d = QuadtreeBoxM2ds<true>;

  template<bool IS_LOOSE_TREE = true>
  using OctreeBoxM3fs = OrthoTreeManagedBox<float, 3, IS_LOOSE_TREE>;
  using OctreeBoxM3f = OctreeBoxM3fs<true>;

  template<bool IS_LOOSE_TREE = true>
  using OctreeBoxM3ds = OrthoTreeManagedBox<double, 3, IS_LOOSE_TREE>;
  using OctreeBoxM3d = OctreeBoxM3ds<true>;


  // Map types

  // Non-owning types
  using QuadtreePointMap2f = EigenOrthoTreePointMap<float, 2>;

  using QuadtreePointMap2d = EigenOrthoTreePointMap<double, 2>;

  using OctreePointMap3f = EigenOrthoTreePointMap<float, 3>;

  using OctreePointMap3d = EigenOrthoTreePointMap<double, 3>;

  template<bool IS_LOOSE_TREE = true>
  using QuadtreeBox2Mapfs = EigenOrthoTreeBoxMap<float, 2, IS_LOOSE_TREE>;
  using QuadtreeBox2Mapf = QuadtreeBox2Mapfs<true>;

  template<bool IS_LOOSE_TREE = true>
  using QuadtreeBox2Mapds = EigenOrthoTreeBoxMap<double, 2, IS_LOOSE_TREE>;
  using QuadtreeBox2Mapd = QuadtreeBox2Mapds<true>;

  template<bool IS_LOOSE_TREE = true>
  using OctreeBox3Mapfs = EigenOrthoTreeBoxMap<float, 3, IS_LOOSE_TREE>;
  using OctreeBox3Mapf = OctreeBox3Mapfs<true>;

  template<bool IS_LOOSE_TREE = true>
  using OctreeBox3Mapds = EigenOrthoTreeBoxMap<double, 3, IS_LOOSE_TREE>;
  using OctreeBox3Mapd = OctreeBox3Mapds<true>;

  // Container types
  using QuadtreePointManagedMap2f = OrthoTreeManagedPointMap<float, 2>;

  using QuadtreePointManagedMap2d = OrthoTreeManagedPointMap<double, 2>;

  using OctreePointManagedMap3f = OrthoTreeManagedPointMap<float, 3>;

  using OctreePointManagedMap3d = OrthoTreeManagedPointMap<double, 3>;

  template<bool IS_LOOSE_TREE = true>
  using QuadtreeBoxManagedMap2fs = OrthoTreeManagedBoxMap<float, 2, IS_LOOSE_TREE>;
  using QuadtreeBoxManagedMap2f = QuadtreeBoxManagedMap2fs<true>;

  template<bool IS_LOOSE_TREE = true>
  using QuadtreeBoxManagedMap2ds = OrthoTreeManagedBoxMap<double, 2, IS_LOOSE_TREE>;
  using QuadtreeBoxManagedMap2d = QuadtreeBoxManagedMap2ds<true>;

  template<bool IS_LOOSE_TREE = true>
  using OctreeBoxManagedMap3fs = OrthoTreeManagedBoxMap<float, 3, IS_LOOSE_TREE>;
  using OctreeBoxManagedMap3f = OctreeBoxManagedMap3fs<true>;

  template<bool IS_LOOSE_TREE = true>
  using OctreeBoxManagedMap3ds = OrthoTreeManagedBoxMap<double, 3, IS_LOOSE_TREE>;
  using OctreeBoxManagedMap3d = OctreeBoxManagedMap3ds<true>;
} // namespace Eigen

#endif
// --- End of Guarded Adapter: EIGEN ---


namespace OrthoTree
{
  // Eigen::Matrix (Vector2d, Vector3d, etc.)
  template<typename TArchive, typename Scalar, int Rows, int Cols, int Options, int MaxRows, int MaxCols>
  void serialize(TArchive& ar, Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols>& m)
  {
    for (int i = 0; i < m.size(); ++i)
      ar & m.data()[i];
  }

  // Eigen::AlignedBox
  template<typename TArchive, typename Scalar, int Dim>
  void serialize(TArchive& ar, Eigen::AlignedBox<Scalar, Dim>& box)
  {
    ar& make_nvp("min", box.min());
    ar& make_nvp("max", box.max());
  }
} // namespace OrthoTree

#endif
// --- End of Guarded Adapter: EIGEN ---


// --- Start of Guarded Adapter: GLM ---
#if defined(ORTHOTREE_GLM_SUPPORT) || defined(GLM_VERSION)


// --- Start of Guarded Adapter: GLM ---
#if defined(ORTHOTREE_GLM_SUPPORT) || defined(GLM_VERSION)


// Override the GLM include if your GLM installation is in a non-standard location.
// Example: -DORTHOTREE_GLM_INCLUDE="<my_vendor/glm/glm.hpp>"
#ifndef ORTHOTREE_GLM_INCLUDE
#define ORTHOTREE_GLM_INCLUDE <glm/glm.hpp>
#endif

#include ORTHOTREE_GLM_INCLUDE

namespace glm
{
  // Define related elements

  template<OrthoTree::dim_t DIMENSION_NO, typename TScalar>
  struct boxNd_t
  {
    vec<DIMENSION_NO, TScalar> min;
    vec<DIMENSION_NO, TScalar> max;
  };

  template<OrthoTree::dim_t DIMENSION_NO, typename TScalar>
  struct rayNd_t
  {
    vec<DIMENSION_NO, TScalar> origin;
    vec<DIMENSION_NO, TScalar> direction;
  };

  template<OrthoTree::dim_t DIMENSION_NO, typename TScalar>
  struct planeNd_t
  {
    TScalar origo_distance;            // origo_distance = dot_product(normal, any_point)
    vec<DIMENSION_NO, TScalar> normal; // should be normalized
  };

  using box2 = boxNd_t<2, float>;
  using ray2 = rayNd_t<2, float>;
  using plane2 = planeNd_t<2, float>;

  using box3 = boxNd_t<3, float>;
  using ray3 = rayNd_t<3, float>;
  using plane3 = planeNd_t<3, float>;

  using box4 = boxNd_t<4, float>;
  using ray4 = rayNd_t<4, float>;
  using plane4 = planeNd_t<4, float>;
} // namespace glm


namespace OrthoTree
{
  namespace GlmAdaptor
  {

    template<dim_t DIMENSION_NO_, typename TScalar = double>
    struct GlmBaseGeometryAdapter
    {
      using Scalar = TScalar;
      using FloatScalar = std::conditional_t<std::is_floating_point_v<TScalar>, TScalar, float>;
      using Vector = glm::vec<DIMENSION_NO_, TScalar>;
      using Box = glm::boxNd_t<DIMENSION_NO_, TScalar>;
      using Ray = glm::rayNd_t<DIMENSION_NO_, TScalar>;
      using Plane = glm::planeNd_t<DIMENSION_NO_, TScalar>;

      static constexpr dim_t DIMENSION_NO = DIMENSION_NO_;
      static constexpr FloatScalar BASE_TOLERANCE = std::numeric_limits<FloatScalar>::epsilon() * FloatScalar(10);

      static constexpr Vector MakePoint() noexcept { return {}; };
      static constexpr Box MakeBox() noexcept { return {}; };

      static constexpr TScalar GetPointC(Vector const& point, dim_t dimensionID) noexcept { return point[dimensionID]; }
      static constexpr void SetPointC(Vector& point, dim_t dimensionID, TScalar value) noexcept { point[dimensionID] = value; }

      static constexpr TScalar GetBoxMinC(Box const& box, dim_t dimensionID) noexcept { return GetPointC(box.min, dimensionID); }
      static constexpr TScalar GetBoxMaxC(Box const& box, dim_t dimensionID) noexcept { return GetPointC(box.max, dimensionID); }
      static constexpr void SetBoxMinC(Box& box, dim_t dimensionID, TScalar value) noexcept { SetPointC(box.min, dimensionID, value); }
      static constexpr void SetBoxMaxC(Box& box, dim_t dimensionID, TScalar value) noexcept { SetPointC(box.max, dimensionID, value); }

      static constexpr Vector const& GetRayDirection(Ray const& ray) noexcept { return ray.direction; }
      static constexpr Vector const& GetRayOrigin(Ray const& ray) noexcept { return ray.origin; }

      static constexpr Vector const& GetPlaneNormal(Plane const& plane) noexcept { return plane.normal; }
      static constexpr TScalar GetPlaneOrigoDistance(Plane const& plane) noexcept { return plane.origo_distance; }
    };

    template<dim_t DIMENSION_NO, typename TScalar>
    using GlmAdaptorGeneral = GeneralGeometryAdapter<GlmBaseGeometryAdapter<DIMENSION_NO, TScalar>>;

    template<dim_t DIMENSION_NO, typename TScalar>
    using GlmOrthoTreePoint =
      OrthoTreeBase<PointEntitySpanAdapter<glm::vec<DIMENSION_NO, TScalar>>, GlmAdaptorGeneral<DIMENSION_NO, TScalar>, PointConfiguration<>>;

    template<dim_t DIMENSION_NO, bool IS_LOOSE_TREE, typename TScalar>
    using GlmOrthoTreeBoundingBox =
      OrthoTreeBase<BoxEntitySpanAdapter<glm::boxNd_t<DIMENSION_NO, TScalar>>, GlmAdaptorGeneral<DIMENSION_NO, TScalar>, BoxConfiguration<IS_LOOSE_TREE>>;

    template<dim_t DIMENSION_NO, typename TScalar, typename TContainer = std::unordered_map<index_t, glm::vec<DIMENSION_NO, TScalar>>>
    using GlmOrthoTreePointMap =
      OrthoTreeBase<PointEntityMapAdapter<glm::vec<DIMENSION_NO, TScalar>, TContainer>, GlmAdaptorGeneral<DIMENSION_NO, TScalar>, PointConfiguration<>>;

    template<dim_t DIMENSION_NO, bool IS_LOOSE_TREE, typename TScalar, typename TContainer = std::unordered_map<index_t, glm::boxNd_t<DIMENSION_NO, TScalar>>>
    using GlmOrthoTreeBoundingBoxMap =
      OrthoTreeBase<BoxEntityMapAdapter<glm::boxNd_t<DIMENSION_NO, TScalar>, TContainer>, GlmAdaptorGeneral<DIMENSION_NO, TScalar>, BoxConfiguration<IS_LOOSE_TREE>>;


  } // namespace GlmAdaptor
} // namespace OrthoTree

namespace glm
{
  using namespace OrthoTree::GlmAdaptor;

  // Core types

  template<int DIMENSION_NO, typename TScalar = double>
  using orthotree_point_t = GlmOrthoTreePoint<DIMENSION_NO, TScalar>;

  using quadtree_point_d = GlmOrthoTreePoint<2, double>;
  using quadtree_point_f = GlmOrthoTreePoint<2, float>;
  using quadtree_point_i = GlmOrthoTreePoint<2, int>;
  using quadtree_point = quadtree_point_f;

  using octree_point_d = GlmOrthoTreePoint<3, double>;
  using octree_point_f = GlmOrthoTreePoint<3, float>;
  using octree_point_i = GlmOrthoTreePoint<3, int>;
  using octree_point = octree_point_f;

  using hextree_point_d = GlmOrthoTreePoint<4, double>;
  using hextree_point_f = GlmOrthoTreePoint<4, float>;
  using hextree_point_i = GlmOrthoTreePoint<4, int>;
  using hextree_point = hextree_point_f;

  template<int DIMENSION_NO, bool IS_LOOSE_TREE = true, typename TScalar = double>
  using orthotree_box_t = GlmOrthoTreeBoundingBox<DIMENSION_NO, IS_LOOSE_TREE, TScalar>;

  template<bool IS_LOOSE_TREE = true>
  using quadtree_box_ds = GlmOrthoTreeBoundingBox<2, IS_LOOSE_TREE, double>;
  using quadtree_box_d = GlmOrthoTreeBoundingBox<2, true, double>;
  using quadtree_box_f = GlmOrthoTreeBoundingBox<2, true, float>;
  using quadtree_box_i = GlmOrthoTreeBoundingBox<2, true, int>;
  using quadtree_box = quadtree_box_f;

  template<bool IS_LOOSE_TREE = true>
  using octree_box_ds = GlmOrthoTreeBoundingBox<3, IS_LOOSE_TREE, double>;
  using octree_box_d = GlmOrthoTreeBoundingBox<3, true, double>;
  using octree_box_f = GlmOrthoTreeBoundingBox<3, true, float>;
  using octree_box_i = GlmOrthoTreeBoundingBox<3, true, int>;
  using octree_box = octree_box_f;

  template<bool IS_LOOSE_TREE = true>
  using hextree_box_ds = GlmOrthoTreeBoundingBox<4, IS_LOOSE_TREE, double>;
  using hextree_box_d = GlmOrthoTreeBoundingBox<4, true, double>;
  using hextree_box_f = GlmOrthoTreeBoundingBox<4, true, float>;
  using hextree_box_i = GlmOrthoTreeBoundingBox<4, true, int>;
  using hextree_box = hextree_box_f;

  // Container types

  template<int DIMENSION_NO, typename TScalar = double>
  using orthotree_point_c_t = OrthoTree::OrthoTreeManaged<orthotree_point_t<DIMENSION_NO, TScalar>>;

  using quadtree_point_c_d = orthotree_point_c_t<2, double>;
  using quadtree_point_c_f = orthotree_point_c_t<2, float>;
  using quadtree_point_c_i = orthotree_point_c_t<2, int>;
  using quadtree_point_c = quadtree_point_c_d;

  using octree_point_c_d = orthotree_point_c_t<3, double>;
  using octree_point_c_f = orthotree_point_c_t<3, float>;
  using octree_point_c_i = orthotree_point_c_t<3, int>;
  using octree_point_c = octree_point_c_f;

  using hextree_point_c_d = orthotree_point_c_t<4, double>;
  using hextree_point_c_f = orthotree_point_c_t<4, float>;
  using hextree_point_c_i = orthotree_point_c_t<4, int>;
  using hextree_point_c = hextree_point_c_f;


  template<int DIMENSION_NO, bool IS_LOOSE_TREE = true, typename TScalar = double>
  using orthotree_box_c_t = OrthoTree::OrthoTreeManaged<orthotree_box_t<DIMENSION_NO, IS_LOOSE_TREE, TScalar>>;

  template<bool IS_LOOSE_TREE = true>
  using quadtree_box_c_ds = orthotree_box_c_t<2, IS_LOOSE_TREE, double>;
  using quadtree_box_c_d = orthotree_box_c_t<2, true, double>;
  using quadtree_box_c_f = orthotree_box_c_t<2, true, float>;
  using quadtree_box_c_i = orthotree_box_c_t<2, true, int>;
  using quadtree_box_c = quadtree_box_c_f;

  template<bool IS_LOOSE_TREE = true>
  using octree_box_c_ds = orthotree_box_c_t<3, IS_LOOSE_TREE, double>;
  using octree_box_c_d = orthotree_box_c_t<3, true, double>;
  using octree_box_c_f = orthotree_box_c_t<3, true, float>;
  using octree_box_c_i = orthotree_box_c_t<3, true, int>;
  using octree_box_c = octree_box_c_f;

  template<bool IS_LOOSE_TREE = true>
  using hextree_box_c_ds = orthotree_box_c_t<4, IS_LOOSE_TREE, double>;
  using hextree_box_c_d = orthotree_box_c_t<4, true, double>;
  using hextree_box_c_f = orthotree_box_c_t<4, true, float>;
  using hextree_box_c_i = orthotree_box_c_t<4, true, int>;
  using hextree_box_c = hextree_box_c_f;


  // Map types

  template<typename T>
  using GlmContainer = std::unordered_map<int, T>;

  //  Core types

  template<int DIMENSION_NO, typename TScalar = double>
  using orthotree_point_map_t = GlmOrthoTreePointMap<DIMENSION_NO, TScalar, GlmContainer<glm::vec<DIMENSION_NO, TScalar>>>;

  using quadtree_point_map_d = GlmOrthoTreePointMap<2, double, GlmContainer<glm::vec<2, double>>>;
  using quadtree_point_map_f = GlmOrthoTreePointMap<2, float, GlmContainer<glm::vec<2, float>>>;
  using quadtree_point_map_i = GlmOrthoTreePointMap<2, int, GlmContainer<glm::vec<2, int>>>;
  using quadtree_point_map = quadtree_point_map_f;

  using octree_point_map_d = GlmOrthoTreePointMap<3, double, GlmContainer<glm::vec<3, double>>>;
  using octree_point_map_f = GlmOrthoTreePointMap<3, float, GlmContainer<glm::vec<3, float>>>;
  using octree_point_map_i = GlmOrthoTreePointMap<3, int, GlmContainer<glm::vec<3, int>>>;
  using octree_point_map = octree_point_map_f;

  using hextree_point_map_d = GlmOrthoTreePointMap<4, double, GlmContainer<glm::vec<4, double>>>;
  using hextree_point_map_f = GlmOrthoTreePointMap<4, float, GlmContainer<glm::vec<4, float>>>;
  using hextree_point_map_i = GlmOrthoTreePointMap<4, int, GlmContainer<glm::vec<4, int>>>;
  using hextree_point_map = hextree_point_map_f;

  template<int DIMENSION_NO, bool IS_LOOSE_TREE = true, typename TScalar = double, typename TContainer = GlmContainer<glm::boxNd_t<DIMENSION_NO, TScalar>>>
  using orthotree_box_map_t = GlmOrthoTreeBoundingBoxMap<DIMENSION_NO, IS_LOOSE_TREE, TScalar, TContainer>;

  template<bool IS_LOOSE_TREE = true>
  using quadtree_box_map_ds = GlmOrthoTreeBoundingBoxMap<2, IS_LOOSE_TREE, double, GlmContainer<glm::boxNd_t<2, double>>>;
  using quadtree_box_map_d = GlmOrthoTreeBoundingBoxMap<2, true, double, GlmContainer<glm::boxNd_t<2, double>>>;
  using quadtree_box_map_f = GlmOrthoTreeBoundingBoxMap<2, true, float, GlmContainer<glm::boxNd_t<2, float>>>;
  using quadtree_box_map_i = GlmOrthoTreeBoundingBoxMap<2, true, int, GlmContainer<glm::boxNd_t<2, int>>>;
  using quadtree_box_map = quadtree_box_map_f;

  template<bool IS_LOOSE_TREE = true>
  using octree_box_map_ds = GlmOrthoTreeBoundingBoxMap<3, IS_LOOSE_TREE, double, GlmContainer<glm::boxNd_t<3, double>>>;
  using octree_box_map_d = GlmOrthoTreeBoundingBoxMap<3, true, double, GlmContainer<glm::boxNd_t<3, double>>>;
  using octree_box_map_f = GlmOrthoTreeBoundingBoxMap<3, true, float, GlmContainer<glm::boxNd_t<3, float>>>;
  using octree_box_map_i = GlmOrthoTreeBoundingBoxMap<3, true, int, GlmContainer<glm::boxNd_t<3, int>>>;
  using octree_box_map = octree_box_map_f;

  template<bool IS_LOOSE_TREE = true>
  using hextree_box_map_ds = GlmOrthoTreeBoundingBoxMap<4, IS_LOOSE_TREE, double, GlmContainer<glm::boxNd_t<4, double>>>;
  using hextree_box_map_d = GlmOrthoTreeBoundingBoxMap<4, true, double, GlmContainer<glm::boxNd_t<4, double>>>;
  using hextree_box_map_f = GlmOrthoTreeBoundingBoxMap<4, true, float, GlmContainer<glm::boxNd_t<4, float>>>;
  using hextree_box_map_i = GlmOrthoTreeBoundingBoxMap<4, true, int, GlmContainer<glm::boxNd_t<4, int>>>;
  using hextree_box_map = hextree_box_map_f;

  // Container types

  template<int DIMENSION_NO, typename TScalar = double>
  using orthotree_point_map_c_t =
    OrthoTree::OrthoTreeManaged<GlmOrthoTreePointMap<DIMENSION_NO, TScalar, GlmContainer<glm::vec<DIMENSION_NO, TScalar>>>>;

  using quadtree_point_map_c_d = orthotree_point_map_c_t<2, double>;
  using quadtree_point_map_c_f = orthotree_point_map_c_t<2, float>;
  using quadtree_point_map_c_i = orthotree_point_map_c_t<2, int>;
  using quadtree_point_map_c = quadtree_point_map_c_d;

  using octree_point_map_c_d = orthotree_point_map_c_t<3, double>;
  using octree_point_map_c_f = orthotree_point_map_c_t<3, float>;
  using octree_point_map_c_i = orthotree_point_map_c_t<3, int>;
  using octree_point_map_c = octree_point_map_c_f;

  using hextree_point_map_c_d = orthotree_point_map_c_t<4, double>;
  using hextree_point_map_c_f = orthotree_point_map_c_t<4, float>;
  using hextree_point_map_c_i = orthotree_point_map_c_t<4, int>;
  using hextree_point_map_c = hextree_point_map_c_f;


  template<int DIMENSION_NO, bool IS_LOOSE_TREE = true, typename TScalar = double>
  using orthotree_box_map_c_t =
    OrthoTree::OrthoTreeManaged<orthotree_box_map_t<DIMENSION_NO, IS_LOOSE_TREE, TScalar, GlmContainer<glm::boxNd_t<DIMENSION_NO, TScalar>>>>;

  template<bool IS_LOOSE_TREE = true>
  using quadtree_box_map_c_ds = orthotree_box_map_c_t<2, IS_LOOSE_TREE, double>;
  using quadtree_box_map_c_d = orthotree_box_map_c_t<2, true, double>;
  using quadtree_box_map_c_f = orthotree_box_map_c_t<2, true, float>;
  using quadtree_box_map_c_i = orthotree_box_map_c_t<2, true, int>;
  using quadtree_box_map_c = quadtree_box_map_c_f;

  template<bool IS_LOOSE_TREE = true>
  using octree_box_map_c_ds = orthotree_box_map_c_t<3, IS_LOOSE_TREE, double>;
  using octree_box_map_c_d = orthotree_box_map_c_t<3, true, double>;
  using octree_box_map_c_f = orthotree_box_map_c_t<3, true, float>;
  using octree_box_map_c_i = orthotree_box_map_c_t<3, true, int>;
  using octree_box_map_c = octree_box_map_c_f;

  template<bool IS_LOOSE_TREE = true>
  using hextree_box_map_c_ds = orthotree_box_map_c_t<4, IS_LOOSE_TREE, double>;
  using hextree_box_map_c_d = orthotree_box_map_c_t<4, true, double>;
  using hextree_box_map_c_f = orthotree_box_map_c_t<4, true, float>;
  using hextree_box_map_c_i = orthotree_box_map_c_t<4, true, int>;
  using hextree_box_map_c = hextree_box_map_c_f;
} // namespace glm


#endif
// --- End of Guarded Adapter: GLM ---


namespace OrthoTree
{
  // glm::vec
  template<typename TArchive, glm::length_t L, typename T, glm::qualifier Q>
  void serialize(TArchive& ar, glm::vec<L, T, Q>& v)
  {
    for (glm::length_t i = 0; i < L; ++i)
      ar& v[i];
  }

  // glm::boxNd_t
  template<typename TArchive, dim_t DIM, typename T>
  void serialize(TArchive& ar, glm::boxNd_t<DIM, T>& box)
  {
    ar& ORTHOTREE_NVP_M(box, min);
    ar& ORTHOTREE_NVP_M(box, max);
  }

  // glm::rayNd_t
  template<typename TArchive, dim_t DIM, typename T>
  void serialize(TArchive& ar, glm::rayNd_t<DIM, T>& ray)
  {
    ar& ORTHOTREE_NVP_M(ray, origin);
    ar& ORTHOTREE_NVP_M(ray, direction);
  }

  // glm::planeNd_t
  template<typename TArchive, dim_t DIM, typename T>
  void serialize(TArchive& ar, glm::planeNd_t<DIM, T>& plane)
  {
    ar& ORTHOTREE_NVP_M(plane, origo_distance);
    ar& ORTHOTREE_NVP_M(plane, normal);
  }
} // namespace OrthoTree

#endif
// --- End of Guarded Adapter: GLM ---


// --- Start of Guarded Adapter: BOOST ---
#if defined(ORTHOTREE_BOOST_SUPPORT) || defined(BOOST_GEOMETRY_HPP) || defined(BOOST_VERSION)


// --- Start of Guarded Adapter: BOOST ---
#if defined(ORTHOTREE_BOOST_SUPPORT) || defined(BOOST_GEOMETRY_HPP) || defined(BOOST_VERSION)


// Override the Boost Geometry include if your Boost installation is in a non-standard location.
// Example: -DORTHOTREE_BOOST_GEOMETRY_INCLUDE="<my_vendor/boost/geometry.hpp>"
#ifndef ORTHOTREE_BOOST_GEOMETRY_INCLUDE
#define ORTHOTREE_BOOST_GEOMETRY_INCLUDE <boost/geometry.hpp>
#endif

#include ORTHOTREE_BOOST_GEOMETRY_INCLUDE


namespace boost::geometry::model
{
  // Define related elements

  template<OrthoTree::dim_t DIMENSION_NO, typename TScalar>
  using pointNd_t = point<TScalar, DIMENSION_NO, boost::geometry::cs::cartesian>;

  template<OrthoTree::dim_t DIMENSION_NO, typename TScalar>
  using boxNd_t = box<pointNd_t<DIMENSION_NO, TScalar>>;

  template<OrthoTree::dim_t DIMENSION_NO, typename TScalar>
  struct rayNd_t
  {
    pointNd_t<DIMENSION_NO, TScalar> origin;
    pointNd_t<DIMENSION_NO, TScalar> direction;
  };

  template<OrthoTree::dim_t DIMENSION_NO, typename TScalar>
  struct planeNd_t
  {
    TScalar origo_distance;                  // origo_distance = dot_product(normal, any_point)
    pointNd_t<DIMENSION_NO, TScalar> normal; // should be normalized
  };
} // namespace boost::geometry::model


namespace OrthoTree
{
  namespace BoostAdaptor
  {
    template<dim_t DIMENSION_NO_, typename TScalar = double>
    struct BoostBaseGeometryAdapter
    {
      using Scalar = TScalar;
      using FloatScalar = std::conditional_t<std::is_floating_point_v<TScalar>, TScalar, float>;
      using Vector = boost::geometry::model::pointNd_t<DIMENSION_NO_, TScalar>;
      using Box = boost::geometry::model::boxNd_t<DIMENSION_NO_, TScalar>;
      using Ray = boost::geometry::model::rayNd_t<DIMENSION_NO_, TScalar>;
      using Plane = boost::geometry::model::planeNd_t<DIMENSION_NO_, TScalar>;

      static constexpr dim_t DIMENSION_NO = DIMENSION_NO_;
      static constexpr FloatScalar BASE_TOLERANCE = std::numeric_limits<FloatScalar>::epsilon() * FloatScalar(10.0);

      static constexpr Vector MakePoint() noexcept { return {}; };
      static constexpr Box MakeBox() noexcept { return {}; };

      static constexpr Scalar GetPointC(Vector const& point, dim_t dimensionID) noexcept
      {
        static_assert(DIMENSION_NO < 6, "For this dimension, the adaptor is not implemented. Supplement the function!");

        if constexpr (DIMENSION_NO == 1)
        {
          return point.template get<0>();
        }
        else if constexpr (DIMENSION_NO == 2)
        {
          switch (dimensionID)
          {
          case 0: return point.template get<0>();
          case 1: return point.template get<1>();
          }
        }
        else if constexpr (DIMENSION_NO == 3)
        {
          switch (dimensionID)
          {
          case 0: return point.template get<0>();
          case 1: return point.template get<1>();
          case 2: return point.template get<2>();
          }
        }
        else if constexpr (DIMENSION_NO == 4)
        {
          switch (dimensionID)
          {
          case 0: return point.template get<0>();
          case 1: return point.template get<1>();
          case 2: return point.template get<2>();
          case 3: return point.template get<3>();
          }
        }
        else
        {
          switch (dimensionID)
          {
          case 0: return point.template get<0>();
          case 1: return point.template get<1>();
          case 2: return point.template get<2>();
          case 3: return point.template get<3>();
          case 4: return point.template get<4>();
          }
        }

        assert(false);
        std::terminate();
      }


      static constexpr void SetPointC(Vector& point, dim_t dimensionID, TScalar value) noexcept
      {
        static_assert(DIMENSION_NO < 6, "For this dimension, the adaptor is not implemented. Supplement the function!");

        if constexpr (DIMENSION_NO == 1)
        {
          point.set<0>(value);
        }
        else if constexpr (DIMENSION_NO == 2)
        {
          switch (dimensionID)
          {
          case 0: point.template set<0>(value); return;
          case 1: point.template set<1>(value); return;
          }
        }
        else if constexpr (DIMENSION_NO == 3)
        {
          switch (dimensionID)
          {
          case 0: point.template set<0>(value); return;
          case 1: point.template set<1>(value); return;
          case 2: point.template set<2>(value); return;
          }
        }
        else if constexpr (DIMENSION_NO == 4)
        {
          switch (dimensionID)
          {
          case 0: point.template set<0>(value); return;
          case 1: point.template set<1>(value); return;
          case 2: point.template set<2>(value); return;
          case 3: point.template set<3>(value); return;
          }
        }
        else
        {
          switch (dimensionID)
          {
          case 0: point.template set<0>(value); return;
          case 1: point.template set<1>(value); return;
          case 2: point.template set<2>(value); return;
          case 3: point.template set<3>(value); return;
          case 4: point.template set<4>(value); return;
          }
        }

        assert(false);
        std::terminate();
      }

      static constexpr Scalar GetBoxMinC(Box const& box, dim_t dimensionID) noexcept { return GetPointC(box.min_corner(), dimensionID); }
      static constexpr Scalar GetBoxMaxC(Box const& box, dim_t dimensionID) noexcept { return GetPointC(box.max_corner(), dimensionID); }
      static constexpr void SetBoxMinC(Box& box, dim_t dimensionID, Scalar value) noexcept { SetPointC(box.min_corner(), dimensionID, value); }
      static constexpr void SetBoxMaxC(Box& box, dim_t dimensionID, Scalar value) noexcept { SetPointC(box.max_corner(), dimensionID, value); }

      static constexpr Vector const& GetRayDirection(Ray const& ray) noexcept { return ray.direction; }
      static constexpr Vector const& GetRayOrigin(Ray const& ray) noexcept { return ray.origin; }

      static constexpr Vector const& GetPlaneNormal(Plane const& plane) noexcept { return plane.normal; }
      static constexpr Scalar GetPlaneOrigoDistance(Plane const& plane) noexcept { return plane.origo_distance; }
    };

    template<dim_t DIMENSION_NO, typename TScalar>
    using BoostGeometryAdapter = GeneralGeometryAdapter<BoostBaseGeometryAdapter<DIMENSION_NO, TScalar>>;

    template<dim_t DIMENSION_NO, typename TScalar, typename TEntityAdapter = PointEntitySpanAdapter<boost::geometry::model::pointNd_t<DIMENSION_NO, TScalar>>>
    using BoostOrthoTreePoint = OrthoTreeBase<TEntityAdapter, BoostGeometryAdapter<DIMENSION_NO, TScalar>, PointConfiguration<>>;

    template<dim_t DIMENSION_NO, bool IS_LOOSE_TREE, typename TScalar, typename TEntityAdapter = BoxEntitySpanAdapter<boost::geometry::model::boxNd_t<DIMENSION_NO, TScalar>>>
    using BoostOrthoTreeBoundingBox = OrthoTreeBase<TEntityAdapter, BoostGeometryAdapter<DIMENSION_NO, TScalar>, BoxConfiguration<IS_LOOSE_TREE>>;
  } // namespace BoostAdaptor
} // namespace OrthoTree

namespace boost::geometry
{
  using namespace OrthoTree::BoostAdaptor;

  // Core types

  template<int DIMENSION_NO, typename TScalar = double, typename TEntityAdapter = OrthoTree::PointEntitySpanAdapter<boost::geometry::model::pointNd_t<DIMENSION_NO, TScalar>>>
  using orthotree_point_t = BoostOrthoTreePoint<DIMENSION_NO, TScalar, TEntityAdapter>;

  template<
    int DIMENSION_NO,
    bool IS_LOOSE_TREE = true,
    typename TScalar = double,
    typename TEntityAdapter = OrthoTree::BoxEntitySpanAdapter<boost::geometry::model::boxNd_t<DIMENSION_NO, TScalar>>>
  using orthotree_box_t = BoostOrthoTreeBoundingBox<DIMENSION_NO, IS_LOOSE_TREE, TScalar, TEntityAdapter>;


  using quadtree_point_d = BoostOrthoTreePoint<2, double>;
  using quadtree_point_f = BoostOrthoTreePoint<2, float>;
  using quadtree_point_i = BoostOrthoTreePoint<2, int>;
  using quadtree_point = quadtree_point_d;

  using octree_point_d = BoostOrthoTreePoint<3, double>;
  using octree_point_f = BoostOrthoTreePoint<3, float>;
  using octree_point_i = BoostOrthoTreePoint<3, int>;
  using octree_point = octree_point_d;


  template<bool IS_LOOSE_TREE = true>
  using quadtree_box_ds = BoostOrthoTreeBoundingBox<2, IS_LOOSE_TREE, double>;
  using quadtree_box_d = BoostOrthoTreeBoundingBox<2, true, double>;
  using quadtree_box_f = BoostOrthoTreeBoundingBox<2, true, float>;
  using quadtree_box_i = BoostOrthoTreeBoundingBox<2, true, int>;
  using quadtree_box = quadtree_box_d;

  template<bool IS_LOOSE_TREE = true>
  using octree_box_ds = BoostOrthoTreeBoundingBox<3, IS_LOOSE_TREE, double>;
  using octree_box_d = BoostOrthoTreeBoundingBox<3, true, double>;
  using octree_box_f = BoostOrthoTreeBoundingBox<3, true, float>;
  using octree_box_i = BoostOrthoTreeBoundingBox<3, true, int>;
  using octree_box = octree_box_d;

  // Container types

  template<int DIMENSION_NO, typename TScalar = double>
  using orthotree_point_m_t = OrthoTree::OrthoTreeManaged<orthotree_point_t<DIMENSION_NO, TScalar>>;

  using quadtree_point_m_d = orthotree_point_m_t<2, double>;
  using quadtree_point_m_f = orthotree_point_m_t<2, float>;
  using quadtree_point_m_i = orthotree_point_m_t<2, int>;
  using quadtree_point_m = quadtree_point_m_d;

  using octree_point_m_d = orthotree_point_m_t<3, double>;
  using octree_point_m_f = orthotree_point_m_t<3, float>;
  using octree_point_m_i = orthotree_point_m_t<3, int>;
  using octree_point_m = octree_point_m_d;


  template<int DIMENSION_NO, bool IS_LOOSE_TREE = true, typename TScalar = double>
  using orthotree_box_m_t = OrthoTree::OrthoTreeManaged<orthotree_box_t<DIMENSION_NO, IS_LOOSE_TREE, TScalar>>;

  template<bool IS_LOOSE_TREE = true>
  using quadtree_box_m_ds = orthotree_box_m_t<2, IS_LOOSE_TREE, double>;
  using quadtree_box_m_d = orthotree_box_m_t<2, true, double>;
  using quadtree_box_m_f = orthotree_box_m_t<2, true, float>;
  using quadtree_box_m_i = orthotree_box_m_t<2, true, int>;
  using quadtree_box_m = quadtree_box_m_d;

  template<bool IS_LOOSE_TREE = true>
  using octree_box_m_ds = orthotree_box_m_t<3, IS_LOOSE_TREE, double>;
  using octree_box_m_d = orthotree_box_m_t<3, true, double>;
  using octree_box_m_f = orthotree_box_m_t<3, true, float>;
  using octree_box_m_i = orthotree_box_m_t<3, true, int>;
  using octree_box_m = octree_box_m_d;


  // Map types

  template<int DIMENSION_NO, typename TScalar>
  using pointmap_container = OrthoTree::PointEntityMapAdapter<boost::geometry::model::pointNd_t<DIMENSION_NO, TScalar>>;
  template<int DIMENSION_NO, typename TScalar>
  using boxmap_container = OrthoTree::BoxEntityMapAdapter<boost::geometry::model::boxNd_t<DIMENSION_NO, TScalar>>;

  using quadtree_point_map_d = BoostOrthoTreePoint<2, double, pointmap_container<2, double>>;
  using quadtree_point_map_f = BoostOrthoTreePoint<2, float, pointmap_container<2, float>>;
  using quadtree_point_map_i = BoostOrthoTreePoint<2, int, pointmap_container<2, int>>;
  using quadtree_point_map = quadtree_point_map_d;

  using octree_point_map_d = BoostOrthoTreePoint<3, double, pointmap_container<3, double>>;
  using octree_point_map_f = BoostOrthoTreePoint<3, float, pointmap_container<3, float>>;
  using octree_point_map_i = BoostOrthoTreePoint<3, int, pointmap_container<3, int>>;
  using octree_point_map = octree_point_map_d;


  template<bool IS_LOOSE_TREE = true>
  using quadtree_box_map_ds = BoostOrthoTreeBoundingBox<2, IS_LOOSE_TREE, double, boxmap_container<2, double>>;
  using quadtree_box_map_d = BoostOrthoTreeBoundingBox<2, true, double, boxmap_container<2, double>>;
  using quadtree_box_map_f = BoostOrthoTreeBoundingBox<2, true, float, boxmap_container<2, float>>;
  using quadtree_box_map_i = BoostOrthoTreeBoundingBox<2, true, int, boxmap_container<2, int>>;
  using quadtree_box_map = quadtree_box_map_d;

  template<bool IS_LOOSE_TREE = true>
  using octree_box_map_ds = BoostOrthoTreeBoundingBox<3, IS_LOOSE_TREE, double, boxmap_container<3, double>>;
  using octree_box_map_d = BoostOrthoTreeBoundingBox<3, true, double, boxmap_container<3, double>>;
  using octree_box_map_f = BoostOrthoTreeBoundingBox<3, true, float, boxmap_container<3, float>>;
  using octree_box_map_i = BoostOrthoTreeBoundingBox<3, true, int, boxmap_container<3, int>>;
  using octree_box_map = octree_box_map_d;

  // Container types

  template<int DIMENSION_NO, typename TScalar = double>
  using orthotree_point_map_m_t = OrthoTree::OrthoTreeManaged<orthotree_point_t<DIMENSION_NO, TScalar, pointmap_container<DIMENSION_NO, TScalar>>>;

  using quadtree_point_map_m_d = orthotree_point_map_m_t<2, double>;
  using quadtree_point_map_m_f = orthotree_point_map_m_t<2, float>;
  using quadtree_point_map_m_i = orthotree_point_map_m_t<2, int>;
  using quadtree_point_map_m = quadtree_point_map_m_d;

  using octree_point_map_m_d = orthotree_point_map_m_t<3, double>;
  using octree_point_map_m_f = orthotree_point_map_m_t<3, float>;
  using octree_point_map_m_i = orthotree_point_map_m_t<3, int>;
  using octree_point_map_m = octree_point_map_m_d;


  template<int DIMENSION_NO, bool IS_LOOSE_TREE = true, typename TScalar = double>
  using orthotree_box_map_m_t =
    OrthoTree::OrthoTreeManaged<orthotree_box_t<DIMENSION_NO, IS_LOOSE_TREE, TScalar, boxmap_container<DIMENSION_NO, TScalar>>>;

  template<bool IS_LOOSE_TREE = true>
  using quadtree_box_map_m_ds = orthotree_box_map_m_t<2, IS_LOOSE_TREE, double>;
  using quadtree_box_map_m_d = orthotree_box_map_m_t<2, true, double>;
  using quadtree_box_map_m_f = orthotree_box_map_m_t<2, true, float>;
  using quadtree_box_map_m_i = orthotree_box_map_m_t<2, true, int>;
  using quadtree_box_map_m = quadtree_box_map_m_d;

  template<bool IS_LOOSE_TREE = true>
  using octree_box_map_m_ds = orthotree_box_map_m_t<3, IS_LOOSE_TREE, double>;
  using octree_box_map_m_d = orthotree_box_map_m_t<3, true, double>;
  using octree_box_map_m_f = orthotree_box_map_m_t<3, true, float>;
  using octree_box_map_m_i = orthotree_box_map_m_t<3, true, int>;
  using octree_box_map_m = octree_box_map_m_d;
} // namespace boost::geometry

#endif
// --- End of Guarded Adapter: BOOST ---


namespace OrthoTree
{
  namespace detail
  {
    static constexpr const char* point_dimension_names[] = { "0",  "1",  "2",  "3",  "4",  "5",  "6",  "7",  "8",  "9",  "10", "11", "12",
                                                             "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25",
                                                             "26", "27", "28", "29", "30", "31", "32", "33", "34", "35", "36", "37", "38",
                                                             "39", "40", "41", "42", "43", "44", "45", "46", "47", "48", "49", "50", "51",
                                                             "52", "53", "54", "55", "56", "57", "58", "59", "60", "61", "62", "63" };

    template<std::size_t I, typename TArchive, typename T, std::size_t D, typename CS>
    void serialize_point_dimension(TArchive& ar, boost::geometry::model::point<T, D, CS>& pt)
    {
      T val = boost::geometry::get<I>(pt);

      const char* name = "unknown";
      if constexpr (0 <= I && I < 64)
        name = point_dimension_names[I];

      ar& make_nvp(name, val);

      if (OrthoTree::is_loading_archive(ar))
        boost::geometry::set<I>(pt, val);
    }

    template<typename TArchive, typename T, std::size_t D, typename CS, std::size_t... Is>
    void serialize_point_internal(TArchive& ar, boost::geometry::model::point<T, D, CS>& pt, std::index_sequence<Is...>)
    {
      (serialize_point_dimension<Is>(ar, pt), ...);
    }
  } // namespace detail

  // boost::geometry::model::point
  template<typename TArchive, typename T, std::size_t D, typename CS>
  void serialize(TArchive& ar, boost::geometry::model::point<T, D, CS>& pt)
  {
    detail::serialize_point_internal(ar, pt, std::make_index_sequence<D>{});
  }

  // boost::geometry::model::box
  template<typename TArchive, typename Point>
  void serialize(TArchive& ar, boost::geometry::model::box<Point>& box)
  {
    ar& make_nvp("min_corner", box.min_corner());
    ar& make_nvp("max_corner", box.max_corner());
  }

  // boost::geometry::model::rayNd_t
  template<typename TArchive, dim_t DIM, typename T>
  void serialize(TArchive& ar, boost::geometry::model::rayNd_t<DIM, T>& ray)
  {
    ar& ORTHOTREE_NVP_M(ray, origin);
    ar& ORTHOTREE_NVP_M(ray, direction);
  }

  // boost::geometry::model::planeNd_t
  template<typename TArchive, dim_t DIM, typename T>
  void serialize(TArchive& ar, boost::geometry::model::planeNd_t<DIM, T>& plane)
  {
    ar& ORTHOTREE_NVP_M(plane, origo_distance);
    ar& ORTHOTREE_NVP_M(plane, normal);
  }
} // namespace OrthoTree

#endif
// --- End of Guarded Adapter: BOOST ---


// --- Start of Guarded Adapter: CGAL ---
#if defined(ORTHOTREE_CGAL_SUPPORT) || defined(CGAL_VERSION) || defined(CGAL_OCTREE_H)


// --- Start of Guarded Adapter: CGAL ---
#if defined(ORTHOTREE_CGAL_SUPPORT) || defined(CGAL_VERSION) || defined(CGAL_OCTREE_H)


// Define ORTHOTREE_CGAL_INCLUDE if your CGAL installation is in a non-standard location and include the following headers manually.
// Example: -DORTHOTREE_CGAL_INCLUDE
#ifndef ORTHOTREE_CGAL_INCLUDE
#include <CGAL/Bbox_2.h>
#include <CGAL/Bbox_3.h>
#include <CGAL/Cartesian.h>
#include <CGAL/Origin.h>
#include <CGAL/Plane_3.h>
#include <CGAL/Point_2.h>
#include <CGAL/Point_3.h>
#include <CGAL/Ray_2.h>
#include <CGAL/Ray_3.h>
#include <CGAL/basic.h>
#include <CGAL/cartesian.h>
#endif

namespace CGAL
{
  // 2d Hyperplane
  struct Plane_2
  {
    double offset; // n1*x + n2*y + offset = 0
    Point_2<Cartesian<double>> normal;
  };
} // namespace CGAL


namespace OrthoTree
{
  namespace CGALAdapter
  {
    struct BaseGeometryAdapter2D
    {
      using Scalar = double;
      using FloatScalar = double;
      using Vector = CGAL::Point_2<CGAL::Cartesian<Scalar>>;
      using Box = CGAL::Bbox_2;
      using Ray = CGAL::Ray_2<CGAL::Cartesian<Scalar>>;
      using Plane = CGAL::Plane_2;

      static constexpr dim_t DIMENSION_NO = 2;
      static constexpr FloatScalar BASE_TOLERANCE = std::numeric_limits<FloatScalar>::epsilon() * FloatScalar(10);

      static Vector MakePoint() noexcept { return {}; };
      static Box MakeBox() noexcept { return {}; };

      static Scalar GetPointC(Vector const& point, dim_t dimensionID) noexcept { return point[dimensionID]; }
      static void SetPointC(Vector& point, dim_t dimensionID, Scalar value) noexcept
      {
        if (dimensionID == 0)
          point = Vector(value, point.y());
        else
          point = Vector(point.x(), value);
      }

      static Scalar GetBoxMinC(Box const& box, dim_t dimensionID) noexcept { return box.min(dimensionID); }
      static Scalar GetBoxMaxC(Box const& box, dim_t dimensionID) noexcept { return box.max(dimensionID); }
      static void SetBoxMinC(Box& box, dim_t dimensionID, Scalar value) noexcept
      {
        if (dimensionID == 0)
          box = Box(value, box.ymin(), box.xmax(), box.ymax());
        else
          box = Box(box.xmin(), value, box.xmax(), box.ymax());
      }
      static void SetBoxMaxC(Box& box, dim_t dimensionID, Scalar value) noexcept
      {
        if (dimensionID == 0)
          box = Box(box.xmin(), box.ymin(), value, box.ymax());
        else
          box = Box(box.xmin(), box.ymin(), box.xmax(), value);
      }

      static Vector GetRayDirection(Ray const& ray) noexcept
      {
        auto const direction = ray.to_vector();
        return Vector(direction.x(), direction.y());
      }
      static Vector GetRayOrigin(Ray const& ray) noexcept { return ray.source(); }

      static Vector const& GetPlaneNormal(Plane const& plane) noexcept { return plane.normal; }
      static Scalar GetPlaneOrigoDistance(Plane const& plane) noexcept { return -plane.offset; }
    };

    using CGALGeometryAdapter2D = GeneralGeometryAdapter<BaseGeometryAdapter2D>;


    struct BaseGeometryAdapter3D
    {
      using Scalar = double;
      using FloatScalar = double;
      using Vector = CGAL::Point_3<CGAL::Cartesian<Scalar>>;
      using Box = CGAL::Bbox_3;
      using Ray = CGAL::Ray_3<CGAL::Cartesian<Scalar>>;
      using Plane = CGAL::Plane_3<CGAL::Cartesian<Scalar>>;

      static constexpr dim_t DIMENSION_NO = 3;
      static constexpr FloatScalar BASE_TOLERANCE = std::numeric_limits<FloatScalar>::epsilon() * FloatScalar(10);

      static Vector MakePoint() noexcept { return {}; };
      static Box MakeBox() noexcept { return {}; };

      static Scalar GetPointC(Vector const& point, dim_t dimensionID) noexcept { return point[dimensionID]; }
      static void SetPointC(Vector& point, dim_t dimensionID, Scalar value) noexcept
      {
        switch (dimensionID)
        {
        case 0: point = Vector(value, point.y(), point.z()); return;
        case 1: point = Vector(point.x(), value, point.z()); return;
        case 2: point = Vector(point.x(), point.y(), value); return;
        }
        assert(false);
        std::terminate();
      }

      static Scalar GetBoxMinC(Box const& box, dim_t dimensionID) noexcept { return box.min(dimensionID); }
      static Scalar GetBoxMaxC(Box const& box, dim_t dimensionID) noexcept { return box.max(dimensionID); }
      static void SetBoxMinC(Box& box, dim_t dimensionID, Scalar value) noexcept
      {
        switch (dimensionID)
        {
        case 0: box = Box(value, box.ymin(), box.zmin(), box.xmax(), box.ymax(), box.zmax()); return;
        case 1: box = Box(box.xmin(), value, box.zmin(), box.xmax(), box.ymax(), box.zmax()); return;
        case 2: box = Box(box.xmin(), box.ymin(), value, box.xmax(), box.ymax(), box.zmax()); return;
        }
        assert(false);
        std::terminate();
      }

      static void SetBoxMaxC(Box& box, dim_t dimensionID, Scalar value) noexcept
      {
        switch (dimensionID)
        {
        case 0: box = Box(box.xmin(), box.ymin(), box.zmin(), value, box.ymax(), box.zmax()); return;
        case 1: box = Box(box.xmin(), box.ymin(), box.zmin(), box.xmax(), value, box.zmax()); return;
        case 2: box = Box(box.xmin(), box.ymin(), box.zmin(), box.xmax(), box.ymax(), value); return;
        }
        assert(false);
        std::terminate();
      }

      static Vector GetRayDirection(Ray const& ray) noexcept
      {
        auto const direction = ray.to_vector();
        return Vector(direction.x(), direction.y(), direction.z());
      }
      static Vector GetRayOrigin(Ray const& ray) noexcept { return ray.source(); }

      static Vector GetPlaneNormal(Plane const& plane) noexcept { return Vector(plane.a(), plane.b(), plane.c()); }
      static Scalar GetPlaneOrigoDistance(Plane const& plane) noexcept { return -plane.d(); }
    };

    using CGALGeometryAdapter3D = GeneralGeometryAdapter<BaseGeometryAdapter3D>;
  } // namespace CGALAdapter
} // namespace OrthoTree

namespace CGAL
{
  using namespace OrthoTree::CGALAdapter;

  // Core types
  using QuadtreePoint =
    OrthoTree::OrthoTreeBase<OrthoTree::PointEntitySpanAdapter<CGAL::Point_2<CGAL::Cartesian<double>>>, CGALGeometryAdapter2D, OrthoTree::PointConfiguration<>>;

  using OctreePoint =
    OrthoTree::OrthoTreeBase<OrthoTree::PointEntitySpanAdapter<CGAL::Point_3<CGAL::Cartesian<double>>>, CGALGeometryAdapter3D, OrthoTree::PointConfiguration<>>;

  template<bool IS_LOOSE_TREE>
  using QuadtreeBoxs =
    OrthoTree::OrthoTreeBase<OrthoTree::BoxEntitySpanAdapter<CGAL::Bbox_2>, CGALGeometryAdapter2D, OrthoTree::BoxConfiguration<IS_LOOSE_TREE>>;

  using QuadtreeBox = QuadtreeBoxs<true>;

  template<bool IS_LOOSE_TREE>
  using OctreeBoxs =
    OrthoTree::OrthoTreeBase<OrthoTree::BoxEntitySpanAdapter<CGAL::Bbox_3>, CGALGeometryAdapter3D, OrthoTree::BoxConfiguration<IS_LOOSE_TREE>>;

  using OctreeBox = OctreeBoxs<true>;


  // Container types

  using QuadtreePointM = OrthoTree::OrthoTreeManaged<QuadtreePoint>;
  using OctreePointM = OrthoTree::OrthoTreeManaged<OctreePoint>;

  template<bool IS_LOOSE_TREE>
  using QuadtreeBoxCs = OrthoTree::OrthoTreeManaged<QuadtreeBoxs<IS_LOOSE_TREE>>;
  using QuadtreeBoxM = QuadtreeBoxCs<true>;
  template<bool IS_LOOSE_TREE>
  using OctreeBoxCs = OrthoTree::OrthoTreeManaged<OctreeBoxs<IS_LOOSE_TREE>>;
  using OctreeBoxM = OctreeBoxCs<true>;


  // Map types

  template<typename T>
  using CGALContainer = std::unordered_map<std::size_t, T>;

  // Core types
  using QuadtreePointMap =
    OrthoTree::OrthoTreeBase<OrthoTree::PointEntityMapAdapter<CGAL::Point_2<CGAL::Cartesian<double>>>, CGALGeometryAdapter2D, OrthoTree::PointConfiguration<>>;


  using OctreePointMap =
    OrthoTree::OrthoTreeBase<OrthoTree::PointEntityMapAdapter<CGAL::Point_3<CGAL::Cartesian<double>>>, CGALGeometryAdapter3D, OrthoTree::PointConfiguration<>>;


  template<bool IS_LOOSE_TREE>
  using QuadtreeBoxsMap =
    OrthoTree::OrthoTreeBase<OrthoTree::BoxEntityMapAdapter<CGAL::Bbox_2>, CGALGeometryAdapter2D, OrthoTree::BoxConfiguration<IS_LOOSE_TREE>>;

  using QuadtreeBoxMap = QuadtreeBoxs<true>;

  template<bool IS_LOOSE_TREE>
  using OctreeBoxsMap =
    OrthoTree::OrthoTreeBase<OrthoTree::BoxEntityMapAdapter<CGAL::Bbox_3>, CGALGeometryAdapter3D, OrthoTree::BoxConfiguration<IS_LOOSE_TREE>>;

  using OctreeBoxMap = OctreeBoxsMap<true>;


  // Container types

  using QuadtreePointMapM = OrthoTree::OrthoTreeManaged<QuadtreePointMap>;
  using OctreePointMapM = OrthoTree::OrthoTreeManaged<OctreePointMap>;

  template<bool IS_LOOSE_TREE>
  using QuadtreeBoxMapCs = OrthoTree::OrthoTreeManaged<QuadtreeBoxsMap<IS_LOOSE_TREE>>;
  using QuadtreeBoxMapM = QuadtreeBoxMapCs<true>;
  template<bool IS_LOOSE_TREE>
  using OctreeBoxMapCs = OrthoTree::OrthoTreeManaged<OctreeBoxsMap<IS_LOOSE_TREE>>;
  using OctreeBoxMapM = OctreeBoxMapCs<true>;
} // namespace CGAL

#endif
// --- End of Guarded Adapter: CGAL ---


namespace OrthoTree
{
  // CGAL Point_2
  template<typename TArchive, typename R>
  void serialize(TArchive& ar, CGAL::Point_2<R>& pt)
  {
    auto x = pt.x();
    auto y = pt.y();
    ar& make_nvp("x", x);
    ar& make_nvp("y", y);
    if (OrthoTree::is_loading_archive(ar))
      pt = CGAL::Point_2<R>(x, y);
  }

  // CGAL Point_3
  template<typename TArchive, typename R>
  void serialize(TArchive& ar, CGAL::Point_3<R>& pt)
  {
    auto x = pt.x();
    auto y = pt.y();
    auto z = pt.z();
    ar& make_nvp("x", x);
    ar& make_nvp("y", y);
    ar& make_nvp("z", z);
    if (OrthoTree::is_loading_archive(ar))
      pt = CGAL::Point_3<R>(x, y, z);
  }

  // CGAL Bbox_2
  template<typename TArchive>
  void serialize(TArchive& ar, CGAL::Bbox_2& box)
  {
    auto xmin = box.xmin();
    auto ymin = box.ymin();
    auto xmax = box.xmax();
    auto ymax = box.ymax();
    ar& make_nvp("xmin", xmin);
    ar& make_nvp("ymin", ymin);
    ar& make_nvp("xmax", xmax);
    ar& make_nvp("ymax", ymax);
    if (OrthoTree::is_loading_archive(ar))
      box = CGAL::Bbox_2(xmin, ymin, xmax, ymax);
  }

  // CGAL Bbox_3
  template<typename TArchive>
  void serialize(TArchive& ar, CGAL::Bbox_3& box)
  {
    auto xmin = box.xmin();
    auto ymin = box.ymin();
    auto zmin = box.zmin();
    auto xmax = box.xmax();
    auto ymax = box.ymax();
    auto zmax = box.zmax();
    ar& make_nvp("xmin", xmin);
    ar& make_nvp("ymin", ymin);
    ar& make_nvp("zmin", zmin);
    ar& make_nvp("xmax", xmax);
    ar& make_nvp("ymax", ymax);
    ar& make_nvp("zmax", zmax);
    if (OrthoTree::is_loading_archive(ar))
      box = CGAL::Bbox_3(xmin, ymin, zmin, xmax, ymax, zmax);
  }

  // CGAL Ray_2
  template<typename TArchive, typename R>
  void serialize(TArchive& ar, CGAL::Ray_2<R>& ray)
  {
    auto source = ray.source();
    auto direction = ray.to_vector();
    ar& make_nvp("source", source);
    ar& make_nvp("direction", direction);
    if (OrthoTree::is_loading_archive(ar))
      ray = CGAL::Ray_2<R>(source, direction);
  }

  // CGAL Ray_3
  template<typename TArchive, typename R>
  void serialize(TArchive& ar, CGAL::Ray_3<R>& ray)
  {
    auto source = ray.source();
    auto direction = ray.to_vector();
    ar& make_nvp("source", source);
    ar& make_nvp("direction", direction);
    if (OrthoTree::is_loading_archive(ar))
      ray = CGAL::Ray_3<R>(source, direction);
  }

  // CGAL Plane_2 (Custom)
  template<typename TArchive>
  void serialize(TArchive& ar, CGAL::Plane_2& plane)
  {
    ar& ORTHOTREE_NVP_M(plane, offset);
    ar& ORTHOTREE_NVP_M(plane, normal);
  }

  // CGAL Plane_3
  template<typename TArchive, typename R>
  void serialize(TArchive& ar, CGAL::Plane_3<R>& plane)
  {
    auto a = plane.a();
    auto b = plane.b();
    auto c = plane.c();
    auto d = plane.d();
    ar& make_nvp("a", a);
    ar& make_nvp("b", b);
    ar& make_nvp("c", c);
    ar& make_nvp("d", d);
    if (OrthoTree::is_loading_archive(ar))
      plane = CGAL::Plane_3<R>(a, b, c, d);
  }
} // namespace OrthoTree

#endif
// --- End of Guarded Adapter: CGAL ---


// --- Start of Guarded Adapter: UNREAL ---
#if defined(ORTHOTREE_UNREAL_SUPPORT)


// #define UNREAL_DUMMY_TYPES
// #define UNREAL_DUMMY_TYPES__SUPPRESS_ASSERTS

#ifdef UNREAL_DUMMY_TYPES

#ifdef UNREAL_DUMMY_TYPES__SUPPRESS_ASSERTS
#define NOT_IMPLEMENTED
#else
#define NOT_IMPLEMENTED assert(false)
#endif

namespace UnrealDummyTypes
{
  // Helper templates

  template<typename FVector_>
  struct FVectorTemplate
  {
    float Size() const
    {
      NOT_IMPLEMENTED;
      return {};
    }

    float SizeSquared() const
    {
      NOT_IMPLEMENTED;
      return {};
    }

    bool IsUnit(float) const
    {
      NOT_IMPLEMENTED;
      return true;
    }

    static float Dist(FVector_ const&, FVector_ const&)
    {
      NOT_IMPLEMENTED;
      return {};
    }

    static float DistSquared(FVector_ const&, FVector_ const&)
    {
      NOT_IMPLEMENTED;
      return {};
    }

    static float DotProduct(FVector_ const&, FVector_ const&)
    {
      NOT_IMPLEMENTED;
      return {};
    }

    FVector_ operator+=(FVector_ const&) const
    {
      NOT_IMPLEMENTED;
      return {};
    }

    FVector_ operator+(FVector_ const&) const
    {
      NOT_IMPLEMENTED;
      return {};
    }

    FVector_ operator-=(FVector_ const&) const
    {
      NOT_IMPLEMENTED;
      return {};
    }

    FVector_ operator-(FVector_ const&) const
    {
      NOT_IMPLEMENTED;
      return {};
    }

    FVector_ operator*=(float) const
    {
      NOT_IMPLEMENTED;
      return {};
    }

    FVector_ operator*(float) const
    {
      NOT_IMPLEMENTED;
      return {};
    }
  };

  template<typename geometry_type>
  struct FVectorTemplate2 : FVectorTemplate<FVectorTemplate2<geometry_type>>
  {
    geometry_type X;
    geometry_type Y;

    FVectorTemplate2() = default;
    FVectorTemplate2(geometry_type X, geometry_type Y)
    : X(X)
    , Y(Y)
    {}
  };

  template<typename geometry_type>
  struct FVectorTemplate3 : FVectorTemplate<FVectorTemplate3<geometry_type>>
  {
    geometry_type X;
    geometry_type Y;
    geometry_type Z;

    FVectorTemplate3() = default;
    FVectorTemplate3(geometry_type X, geometry_type Y, geometry_type Z)
    : X(X)
    , Y(Y)
    , Z(Z)
    {}
  };

  template<typename FVector_>
  struct FBoxTemplate
  {
    FVector_ Min;
    FVector_ Max;
    bool IsValid;

    FBoxTemplate() = default;
    FBoxTemplate(FVector_&& Min, FVector_&& Max)
    : Min(Min)
    , Max(Max)
    {}

    FBoxTemplate(FVector_ const& Min, FVector_ const& Max)
    : Min(Min)
    , Max(Max)
    {}

    FVector_ GetCenter() const
    {
      NOT_IMPLEMENTED;
      return FVector_{};
    }

    FVector_ GetSize() const
    {
      NOT_IMPLEMENTED;
      return FVector_{};
    }

    float GetVolume() const
    {
      NOT_IMPLEMENTED;
      return 0.0f;
    }

    bool Intersect(FBoxTemplate const&) const
    {
      NOT_IMPLEMENTED;
      return false;
    }

    bool IsInside(FBoxTemplate const&) const
    {
      NOT_IMPLEMENTED;
      return false;
    }

    bool IsInside(FVector_ const&) const
    {
      NOT_IMPLEMENTED;
      return false;
    }

    bool IsInsideOrOn(FVector_ const&) const
    {
      NOT_IMPLEMENTED;
      return false;
    }

    FBoxTemplate Overlap(FBoxTemplate const&)
    {
      NOT_IMPLEMENTED;
      return {};
    }

    FBoxTemplate ExpandBy(FVector_ const& v) const
    {
      NOT_IMPLEMENTED;
      return {};
    }

    FBoxTemplate ShiftBy(FVector_ const& v) const
    {
      NOT_IMPLEMENTED;
      return {};
    }
  };

  template<typename FVector_>
  struct FRayTemplate
  {
    FVector_ Direction;
    FVector_ Origin;
  };

  template<typename FVector_>
  struct FPlaneTemplate : public FVector_
  {
    float W;

    const FVector_& GetNormal() const { return *this; }
  };


  // Unreal types
  using int32 = int32_t;
  using int64 = int64_t;
  using FLargeWorldCoordinatesReal = double;

  using FVector2D = FVectorTemplate2<FLargeWorldCoordinatesReal>;
  using FVector2f = FVectorTemplate2<float>;

  using FVector = FVectorTemplate3<FLargeWorldCoordinatesReal>;
  using FVector3f = FVectorTemplate3<float>;
  using FVector3d = FVectorTemplate3<double>;

  using FBox = FBoxTemplate<FVector>;
  using FBox2f = FBoxTemplate<FVector2f>;
  using FBox2D = FBoxTemplate<FVector2D>;
  using FBox3f = FBoxTemplate<FVector3f>;
  using FBox3d = FBoxTemplate<FVector3d>;

  using FRay2D = FRayTemplate<FBox2D>;
  using FRay = FRayTemplate<FVector>;

  using FPlane2D = FPlaneTemplate<FVector2D>;
  using FPlane = FPlaneTemplate<FVector>;
} // namespace UnrealDummyTypes
#endif

#ifdef UNREAL_DUMMY_TYPES
using namespace UnrealDummyTypes;
#endif

namespace OrthoTree
{
  namespace UnrealAdapter
  {
    template<typename FVector2D_>
    struct FRay2DTemplate
    {
      FVector2D_ Direction;
      FVector2D_ Origin;
    };

    template<typename FGeometry_, typename FVector2D_, typename FBox2D_>
    struct UnrealBaseGeometryAdapter2D
    {
      using Scalar = FGeometry_;
      using FloatScalar = FGeometry_;
      using Vector = FVector2D_;
      using Box = FBox2D_;
      using Ray = FRay2DTemplate<FVector2D_>;
      using Plane = FRay2DTemplate<FVector2D_>;

      static constexpr dim_t DIMENSION_NO = 2;
      static constexpr FloatScalar BASE_TOLERANCE = std::numeric_limits<FloatScalar>::epsilon() * FloatScalar(10);

      // There is no 2d ray and plane in Unreal
      using FRay2D_ = FRay2DTemplate<FVector2D_>;
      using FPlane2D_ = FRay2DTemplate<FVector2D_>;

      static constexpr FGeometry_ GetPointC(FVector2D_ const& pt, dim_t dimensionID)
      {
        switch (dimensionID)
        {
        case 0: return pt.X;
        case 1: return pt.Y;
        default: std::terminate();
        }
      }

      static constexpr void SetPointC(FVector2D_& pt, dim_t dimensionID, FGeometry_ value)
      {
        switch (dimensionID)
        {
        case 0: pt.X = value; break;
        case 1: pt.Y = value; break;
        default: std::terminate();
        }
      }

      static constexpr FGeometry_ GetBoxMinC(FBox2D_ const& box, dim_t dimensionID) { return GetPointC(box.Min, dimensionID); }
      static constexpr FGeometry_ GetBoxMaxC(FBox2D_ const& box, dim_t dimensionID) { return GetPointC(box.Max, dimensionID); }
      static constexpr void SetBoxMinC(FBox2D_& box, dim_t dimensionID, FGeometry_ value) { SetPointC(box.Min, dimensionID, value); }
      static constexpr void SetBoxMaxC(FBox2D_& box, dim_t dimensionID, FGeometry_ value) { SetPointC(box.Max, dimensionID, value); }

      static constexpr FVector2D_ const& GetRayDirection(FRay2D_ const& ray) noexcept { return ray.Direction; }
      static constexpr FVector2D_ const& GetRayOrigin(FRay2D_ const& ray) noexcept { return ray.Origin; }

      static constexpr FVector2D_ const& GetPlaneNormal(FPlane2D_ const& plane) noexcept { return plane.Direction; }
      static constexpr FGeometry_ GetPlaneOrigoDistance(FPlane2D_ const& plane) noexcept
      {
        return FVector2D_::DotProduct(plane.Origin, plane.Direction);
      }
    };


    template<typename FGeometry_, typename FVector_, typename FBox_, typename FRay_, typename FPlane_>
    struct UnrealBaseGeometryAdapter3D
    {
      using Scalar = FGeometry_;
      using FloatScalar = FGeometry_;
      using Vector = FVector_;
      using Box = FBox_;
      using Ray = FRay_;
      using Plane = FPlane_;

      static constexpr dim_t DIMENSION_NO = 3;
      static constexpr FloatScalar BASE_TOLERANCE = std::numeric_limits<FloatScalar>::epsilon() * FloatScalar(10);

      static constexpr FVector MakePoint() { return {}; }
      static constexpr FGeometry_ GetPointC(FVector_ const& v, dim_t dimensionID)
      {
        switch (dimensionID)
        {
        case 0: return v.X;
        case 1: return v.Y;
        case 2: return v.Z;
        default: assert(false); std::terminate();
        }
      }

      static constexpr void SetPointC(FVector_& v, dim_t dimensionID, FGeometry_ value)
      {
        switch (dimensionID)
        {
        case 0: v.X = value; break;
        case 1: v.Y = value; break;
        case 2: v.Z = value; break;
        default: assert(false); std::terminate();
        }
      }

      static constexpr FBox_ MakeBox() { return {}; }
      static constexpr FGeometry_ GetBoxMinC(FBox_ const& box, dim_t dimensionID) { return GetPointC(box.Min, dimensionID); }
      static constexpr FGeometry_ GetBoxMaxC(FBox_ const& box, dim_t dimensionID) { return GetPointC(box.Max, dimensionID); }
      static constexpr void SetBoxMinC(FBox_& box, dim_t dimensionID, FGeometry_ value) { SetPointC(box.Min, dimensionID, value); }
      static constexpr void SetBoxMaxC(FBox_& box, dim_t dimensionID, FGeometry_ value) { SetPointC(box.Max, dimensionID, value); }

      static constexpr FVector_ const& GetRayDirection(FRay_ const& ray) noexcept { return ray.Direction; }
      static constexpr FVector_ const& GetRayOrigin(FRay_ const& ray) noexcept { return ray.Origin; }

      static constexpr FVector_ const& GetPlaneNormal(FPlane_ const& plane) noexcept { return plane.GetNormal(); }
      static constexpr FGeometry_ GetPlaneOrigoDistance(FPlane_ const& plane) noexcept { return FGeometry_(plane.W); }
    };

    template<typename TUnrealBaseGeometryAdapter>
    struct UnrealGeometryAdapter : GeneralGeometryAdapter<TUnrealBaseGeometryAdapter>
    {
      using Base = TUnrealBaseGeometryAdapter;

      using Scalar = Base::Scalar;
      using FloatScalar = Base::FloatScalar;
      using Vector = Base::Vector;
      using Box = Base::Box;
      using Ray = Base::Ray;
      using Plane = Base::Plane;

      static constexpr dim_t DIMENSION_NO = Base::DIMENSION_NO;

      static_assert(BaseGeometryAdapterConcept<Base, DIMENSION_NO, Vector, Box, Ray, Plane, Scalar, FloatScalar>);

      static constexpr Scalar Size2(Vector const& v) noexcept { return v.SizeSquared(); }

      static constexpr Scalar Size(Vector const& point) noexcept { return point.Size(); }

      static constexpr Vector Add(Vector const& v1, Vector const& v2) noexcept { return v1 + v2; }

      static constexpr Vector Subtract(Vector const& v1, Vector const& v2) noexcept { return v1 - v2; }

      static constexpr Vector Multiply(Vector const& v, FloatScalar scalarFactor) noexcept { return v * float(scalarFactor); }

      static constexpr Scalar Dot(Vector const& v1, Vector const& v2) noexcept { return Vector::DotProduct(v1, v2); }

      static constexpr Scalar Distance(Vector const& v1, Vector const& v2) noexcept { return Vector::Dist(v1, v2); }

      static constexpr Scalar Distance2(Vector const& v1, Vector const& v2) noexcept { return Vector::DistSquared(v1, v2); }

      static constexpr bool ArePointsEqual(Vector const& v1, Vector const& v2, FloatScalar tolerance) noexcept
      {
        return Distance2(v1, v2) <= tolerance * tolerance;
      }

      static constexpr bool IsNormalizedVector(Vector const& normal) noexcept { return normal.IsUnit(0.000001f); }

      static constexpr bool IsAlmostEqual(FloatScalar lhs, FloatScalar rhs, FloatScalar tolerance) noexcept
      {
        return std::abs(lhs - rhs) <= tolerance;
      }

      static Box GetBoxOfPoints(std::span<Vector const> const& points) noexcept
      {
        auto ext = points.size() == 0 ? Box{} : Box(points[0], points[0]);
        for (auto const& point : points)
        {
          for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
          {
            if (Base::GetBoxMinC(ext, dimensionID) > Base::GetPointC(point, dimensionID))
              Base::SetBoxMinC(ext, dimensionID, Base::GetPointC(point, dimensionID));

            if (Base::GetBoxMaxC(ext, dimensionID) < Base::GetPointC(point, dimensionID))
              Base::SetBoxMaxC(ext, dimensionID, Base::GetPointC(point, dimensionID));
          }
        }
        return ext;
      }

      static Box GetBoxOfBoxes(std::span<Box const> const& extents) noexcept
      {
        if (extents.size() == 0)
          return {};

        auto ext = extents[0];
        for (auto const& e : extents)
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

      static void MoveBox(Box& box, Vector const& moveVector) noexcept { box = box.ShiftBy(moveVector); }
    };


    template<typename FGeometry_, typename FVector2D_, typename FBox2D_>
    using UnrealGeometryAdapter2D = UnrealGeometryAdapter<UnrealBaseGeometryAdapter2D<FGeometry_, FVector2D_, FBox2D_>>;

    template<typename FGeometry_, typename FVector_, typename FBox_, typename FRay_, typename FPlane_>
    using UnrealGeometryAdapter3D = UnrealGeometryAdapter<UnrealBaseGeometryAdapter3D<FGeometry_, FVector_, FBox_, FRay_, FPlane_>>;


    // Templates for point types

    template<typename FGeometry_, typename FVector_, typename FBox_>
    using QuadtreePointTemplate =
      OrthoTreeBase<PointEntitySpanAdapter<FVector_>, UnrealGeometryAdapter2D<FGeometry_, FVector_, FBox_>, PointConfiguration<>>;

    template<typename FGeometry_, typename FVector_, typename FBox_, typename FRay_, typename FPlane_>
    using OctreePointTemplate =
      OrthoTreeBase<PointEntitySpanAdapter<FVector_>, UnrealGeometryAdapter3D<FGeometry_, FVector_, FBox_, FRay_, FPlane_>, PointConfiguration<>>;

    template<typename FGeometry_, typename FVector_, typename FBox_, typename TContainer_ = std::unordered_map<index_t, FVector_>>
    using QuadtreePointMapTemplate =
      OrthoTreeBase<PointEntityMapAdapter<FVector_, TContainer_>, UnrealGeometryAdapter2D<FGeometry_, FVector_, FBox_>, PointConfiguration<>>;

    template<typename FGeometry_, typename FVector_, typename FBox_, typename FRay_, typename FPlane_, typename TContainer_ = std::unordered_map<index_t, FVector_>>
    using OctreePointMapTemplate =
      OrthoTreeBase<PointEntityMapAdapter<FVector_, TContainer_>, UnrealGeometryAdapter3D<FGeometry_, FVector_, FBox_, FRay_, FPlane_>, PointConfiguration<>>;

    // Templates for box types

    template<typename FGeometry_, typename FVector_, typename FBox_, bool IS_LOOSE_TREE = true>
    using QuadtreeBoxTemplate =
      OrthoTreeBase<BoxEntitySpanAdapter<FBox_>, UnrealGeometryAdapter2D<FGeometry_, FVector_, FBox_>, BoxConfiguration<IS_LOOSE_TREE>>;

    template<typename FGeometry_, typename FVector_, typename FBox_, typename FRay_, typename FPlane_, bool IS_LOOSE_TREE = true>
    using OctreeBoxTemplate =
      OrthoTreeBase<BoxEntitySpanAdapter<FBox_>, UnrealGeometryAdapter3D<FGeometry_, FVector_, FBox_, FRay_, FPlane_>, BoxConfiguration<IS_LOOSE_TREE>>;

    template<typename FGeometry_, typename FVector_, typename FBox_, bool IS_LOOSE_TREE = true, typename TContainer_ = std::unordered_map<index_t, FVector_>>
    using QuadtreeBoxMapTemplate =
      OrthoTreeBase<BoxEntityMapAdapter<FBox_, TContainer_>, UnrealGeometryAdapter2D<FGeometry_, FVector_, FBox_>, BoxConfiguration<IS_LOOSE_TREE>>;

    template<typename FGeometry_, typename FVector_, typename FBox_, typename FRay_, typename FPlane_, bool IS_LOOSE_TREE = true, typename TContainer_ = std::unordered_map<index_t, FVector_>>
    using OctreeBoxMapTemplate =
      OrthoTreeBase<BoxEntityMapAdapter<FBox_, TContainer_>, UnrealGeometryAdapter3D<FGeometry_, FVector_, FBox_, FRay_, FPlane_>, BoxConfiguration<IS_LOOSE_TREE>>;

  } // namespace UnrealAdapter
} // namespace OrthoTree


// Orthotree Core Types
using FLargeWorldCoordinatesReal = double;

using FQuadtreePoint = OrthoTree::UnrealAdapter::QuadtreePointTemplate<FLargeWorldCoordinatesReal, FVector2D, FBox2D>;
using FQuadtreePoint2D = FQuadtreePoint;
using FQuadtreePoint2f = OrthoTree::UnrealAdapter::QuadtreePointTemplate<float, FVector2f, FBox2f>;

using FOctreePoint = OrthoTree::UnrealAdapter::OctreePointTemplate<FLargeWorldCoordinatesReal, FVector, FBox, FRay, FPlane>;
using FOctreePoint3d = OrthoTree::UnrealAdapter::OctreePointTemplate<double, FVector3d, FBox3d, FRay, FPlane>;
using FOctreePoint3f = OrthoTree::UnrealAdapter::OctreePointTemplate<float, FVector3f, FBox3f, FRay, FPlane>;


template<bool IS_LOOSE_TREE = true>
using FQuadtreeBoxs = OrthoTree::UnrealAdapter::QuadtreeBoxTemplate<FLargeWorldCoordinatesReal, FVector2D, FBox2D, IS_LOOSE_TREE>;
using FQuadtreeBox = FQuadtreeBoxs<true>;

template<bool IS_LOOSE_TREE = true>
using FQuadtreeBox2Ds = FQuadtreeBoxs<IS_LOOSE_TREE>;
using FQuadtreeBox2D = FQuadtreeBox;

template<bool IS_LOOSE_TREE = true>
using FQuadtreeBox2fs = OrthoTree::UnrealAdapter::QuadtreeBoxTemplate<float, FVector2f, FBox2f, IS_LOOSE_TREE>;
using FQuadtreeBox2f = OrthoTree::UnrealAdapter::QuadtreeBoxTemplate<float, FVector2f, FBox2f, true>;

template<bool IS_LOOSE_TREE = true>
using FOctreeBoxs = OrthoTree::UnrealAdapter::OctreeBoxTemplate<FLargeWorldCoordinatesReal, FVector, FBox, FRay, FPlane, IS_LOOSE_TREE>;
using FOctreeBox = OrthoTree::UnrealAdapter::OctreeBoxTemplate<FLargeWorldCoordinatesReal, FVector, FBox, FRay, FPlane, true>;

template<bool IS_LOOSE_TREE = true>
using FOctreeBox3ds = OrthoTree::UnrealAdapter::OctreeBoxTemplate<double, FVector3d, FBox3d, FRay, FPlane, IS_LOOSE_TREE>;
using FOctreeBox3d = OrthoTree::UnrealAdapter::OctreeBoxTemplate<double, FVector3d, FBox3d, FRay, FPlane, true>;

template<bool IS_LOOSE_TREE = true>
using FOctreeBox3fs = OrthoTree::UnrealAdapter::OctreeBoxTemplate<float, FVector3f, FBox3f, FRay, FPlane, IS_LOOSE_TREE>;
using FOctreeBox3f = OrthoTree::UnrealAdapter::OctreeBoxTemplate<float, FVector3f, FBox3f, FRay, FPlane, true>;


// Orthotree Container Types

using FQuadtreePointC = OrthoTree::OrthoTreeManaged<FQuadtreePoint>;
using FQuadtreePoint2DC = FQuadtreePointC;
using FQuadtreePoint2fC = OrthoTree::OrthoTreeManaged<FQuadtreePoint2f>;

using FOctreePointC = OrthoTree::OrthoTreeManaged<FOctreePoint>;
using FOctreePoint3dC = OrthoTree::OrthoTreeManaged<FOctreePoint3d>;
using FOctreePoint3fC = OrthoTree::OrthoTreeManaged<FOctreePoint3f>;

template<bool IS_LOOSE_TREE = true>
using FQuadtreeBoxCs = OrthoTree::OrthoTreeManaged<FQuadtreeBoxs<IS_LOOSE_TREE>>;
using FQuadtreeBoxC = FQuadtreeBoxCs<true>;

template<bool IS_LOOSE_TREE = true>
using FQuadtreeBox2DCs = OrthoTree::OrthoTreeManaged<FQuadtreeBox2Ds<IS_LOOSE_TREE>>;
using FQuadtreeBox2DC = FQuadtreeBox2DCs<true>;

template<bool IS_LOOSE_TREE = true>
using FQuadtreeBox2fCs = OrthoTree::OrthoTreeManaged<FQuadtreeBox2fs<IS_LOOSE_TREE>>;
using FQuadtreeBox2fC = FQuadtreeBox2fCs<true>;

template<bool IS_LOOSE_TREE = true>
using FOctreeBoxCs = OrthoTree::OrthoTreeManaged<FOctreeBoxs<IS_LOOSE_TREE>>;
using FOctreeBoxC = FOctreeBoxCs<true>;

template<bool IS_LOOSE_TREE = true>
using FOctreeBox3dCs = OrthoTree::OrthoTreeManaged<FOctreeBox3ds<IS_LOOSE_TREE>>;
using FOctreeBox3dC = FOctreeBox3dCs<true>;

template<bool IS_LOOSE_TREE = true>
using FOctreeBox3fCs = OrthoTree::OrthoTreeManaged<FOctreeBox3fs<IS_LOOSE_TREE>>;
using FOctreeBox3fC = FOctreeBox3fCs<true>;


template<typename T>
using FContainer = std::unordered_map<int, T>;

using FQuadtreePointMap = OrthoTree::UnrealAdapter::QuadtreePointMapTemplate<FLargeWorldCoordinatesReal, FVector2D, FBox2D, FContainer<FVector2D>>;
using FQuadtreePointMap2D = FQuadtreePointMap;
using FQuadtreePointMap2f = OrthoTree::UnrealAdapter::QuadtreePointMapTemplate<float, FVector2f, FBox2f, FContainer<FVector2f>>;

using FOctreePointMap = OrthoTree::UnrealAdapter::OctreePointMapTemplate<FLargeWorldCoordinatesReal, FVector, FBox, FRay, FPlane, FContainer<FVector>>;
using FOctreePointMap3d = OrthoTree::UnrealAdapter::OctreePointMapTemplate<double, FVector3d, FBox3d, FRay, FPlane, FContainer<FVector3d>>;
using FOctreePointMap3f = OrthoTree::UnrealAdapter::OctreePointMapTemplate<float, FVector3f, FBox3f, FRay, FPlane, FContainer<FVector3f>>;


template<bool IS_LOOSE_TREE = true>
using FQuadtreeBoxsMap =
  OrthoTree::UnrealAdapter::QuadtreeBoxMapTemplate<FLargeWorldCoordinatesReal, FVector2D, FBox2D, IS_LOOSE_TREE, FContainer<FBox2D>>;
using FQuadtreeBoxMap = FQuadtreeBoxsMap<true>;

template<bool IS_LOOSE_TREE = true>
using FQuadtreeBoxMap2Ds = FQuadtreeBoxsMap<IS_LOOSE_TREE>;
using FQuadtreeBoxMap2D = FQuadtreeBoxMap;

template<bool IS_LOOSE_TREE = true>
using FQuadtreeBoxMap2fs = OrthoTree::UnrealAdapter::QuadtreeBoxMapTemplate<float, FVector2f, FBox2f, IS_LOOSE_TREE, FContainer<FBox2f>>;
using FQuadtreeBoxMap2f = OrthoTree::UnrealAdapter::QuadtreeBoxMapTemplate<float, FVector2f, FBox2f, true, FContainer<FBox2f>>;

template<bool IS_LOOSE_TREE = true>
using FOctreeBoxsMap =
  OrthoTree::UnrealAdapter::OctreeBoxMapTemplate<FLargeWorldCoordinatesReal, FVector, FBox, FRay, FPlane, IS_LOOSE_TREE, FContainer<FBox>>;
using FOctreeBoxMap = OrthoTree::UnrealAdapter::OctreeBoxMapTemplate<FLargeWorldCoordinatesReal, FVector, FBox, FRay, FPlane, true, FContainer<FBox>>;

template<bool IS_LOOSE_TREE = true>
using FOctreeBoxMap3ds = OrthoTree::UnrealAdapter::OctreeBoxMapTemplate<double, FVector3d, FBox3d, FRay, FPlane, IS_LOOSE_TREE, FContainer<FBox3d>>;
using FOctreeBoxMap3d = OrthoTree::UnrealAdapter::OctreeBoxMapTemplate<double, FVector3d, FBox3d, FRay, FPlane, true, FContainer<FBox3d>>;

template<bool IS_LOOSE_TREE = true>
using FOctreeBoxMap3fs = OrthoTree::UnrealAdapter::OctreeBoxMapTemplate<float, FVector3f, FBox3f, FRay, FPlane, IS_LOOSE_TREE, FContainer<FBox3f>>;
using FOctreeBoxMap3f = OrthoTree::UnrealAdapter::OctreeBoxMapTemplate<float, FVector3f, FBox3f, FRay, FPlane, true, FContainer<FBox3f>>;


using FQuadtreePointMapC = OrthoTree::OrthoTreeManaged<FQuadtreePointMap>;
using FQuadtreePointMap2DC = FQuadtreePointMapC;
using FQuadtreePointMap2fC = OrthoTree::OrthoTreeManaged<FQuadtreePointMap2f>;

using FOctreePointMapC = OrthoTree::OrthoTreeManaged<FOctreePointMap>;
using FOctreePointMap3dC = OrthoTree::OrthoTreeManaged<FOctreePointMap3d>;
using FOctreePointMap3fC = OrthoTree::OrthoTreeManaged<FOctreePointMap3f>;

template<bool IS_LOOSE_TREE = true>
using FQuadtreeBoxMapCs = OrthoTree::OrthoTreeManaged<FQuadtreeBoxsMap<IS_LOOSE_TREE>>;
using FQuadtreeBoxMapC = FQuadtreeBoxMapCs<true>;

template<bool IS_LOOSE_TREE = true>
using FQuadtreeBoxMap2DCs = OrthoTree::OrthoTreeManaged<FQuadtreeBoxMap2Ds<IS_LOOSE_TREE>>;
using FQuadtreeBoxMap2DC = FQuadtreeBoxMap2DCs<true>;

template<bool IS_LOOSE_TREE = true>
using FQuadtreeBoxMap2fCs = OrthoTree::OrthoTreeManaged<FQuadtreeBoxMap2fs<IS_LOOSE_TREE>>;
using FQuadtreeBoxMap2fC = FQuadtreeBoxMap2fCs<true>;

template<bool IS_LOOSE_TREE = true>
using FOctreeBoxMapCs = OrthoTree::OrthoTreeManaged<FOctreeBoxsMap<IS_LOOSE_TREE>>;
using FOctreeBoxMapC = FOctreeBoxMapCs<true>;

template<bool IS_LOOSE_TREE = true>
using FOctreeBoxMap3dCs = OrthoTree::OrthoTreeManaged<FOctreeBoxMap3ds<IS_LOOSE_TREE>>;
using FOctreeBoxMap3dC = FOctreeBoxMap3dCs<true>;

template<bool IS_LOOSE_TREE = true>
using FOctreeBoxMap3fCs = OrthoTree::OrthoTreeManaged<FOctreeBoxMap3fs<IS_LOOSE_TREE>>;
using FOctreeBoxMap3fC = FOctreeBoxMap3fCs<true>;

#endif
// --- End of Guarded Adapter: UNREAL ---
