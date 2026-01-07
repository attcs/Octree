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

#include <assert.h>
#include <math.h>

#include <algorithm>
#include <array>
#include <bit>
#include <bitset>
#include <concepts>
#include <cstdint> // TODO: is needed?
#include <cstring>
#include <execution>
#include <functional>
#include <iterator>
#include <map>
#include <memory_resource>
#include <numeric>
#include <optional>
#include <queue>
#include <ranges>
#include <set>
#include <span>
#include <stack>
#include <stdexcept>
#include <thread>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <version>

#if defined(__cpp_lib_unreachable) && (__cpp_lib_unreachable >= 202202L)
#include <utility>
#define ORTHOTREE_UNREACHABLE() std::unreachable()
#elif defined(_MSC_VER)
#define ORTHOTREE_UNREACHABLE() __assume(0)
#elif defined(__GNUC__) || defined(__clang__)
#define ORTHOTREE_UNREACHABLE() __builtin_unreachable()
#else
#include <cstdlib>
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
  do                                 \
  {                                  \
    if (cond)                        \
    {                                \
      CRASH(errorMessage);           \
    }                                \
  } while (0)
#endif // !ORTHOTREE_CRASH_IF

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
    template<typename T>
    struct IsStdOptional : std::false_type
    {};

    template<typename U>
    struct IsStdOptional<std::optional<U>> : std::true_type
    {};

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
        for (uint32_t i = 0; i < uintN; ++i)
          f(i);
      }
    }

    template<typename T>
    inline constexpr bool IsStdOptionalV = IsStdOptional<T>::value;

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

    constexpr bool contains(auto const& container, auto const& element) noexcept
    {
      return container.contains(element);
    }

    constexpr void clear(auto& container) noexcept
    {
      container.clear();
    }

    constexpr auto erase(auto& container, auto const& element) noexcept
    {
      return container.erase(element);
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
    inline constexpr void reserve(TContainer& c, std::size_t n) noexcept
    {
      c.reserve(n);
    };

    template<typename TContainer>
    inline constexpr void reserve(TContainer&, std::size_t) noexcept {};

    template<uint8_t e, typename TOut = std::size_t>
    consteval TOut pow2_ce()
    {
      constexpr auto bitSize = sizeof(TOut) * CHAR_BIT;
      static_assert(e >= 0 && e < bitSize);
      return TOut{ 1 } << e;
    }

    template<typename TIn, typename TOut = std::size_t>
    inline constexpr TOut pow2(TIn e) noexcept
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
  } // namespace detail


  // Type of the dimension
  using dim_t = uint32_t;

  // Type of depth
  using depth_t = uint32_t;

  // Grid id
  using GridID = uint32_t;

  constexpr depth_t INVALID_DEPTH = 31;
  constexpr depth_t MAX_DEPTH_ID = 30;

#ifdef _MSC_VER
#pragma warning(default : 4715)
#endif

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif
} // namespace OrthoTree
