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

#include "common.h"

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
  {};

  template<typename U>
  struct IsStdOptional<std::optional<U>> : std::true_type
  {};

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
  constexpr auto exchange(TContainer& container, TKey key, TValue&& value) noexcept
  {
    if constexpr (requires { container.extract(key); } && requires { value.second; })
    {
      return std::pair{ key, std::exchange(container.at(key), value.second) };
    }
    else
    {
      return std::exchange(detail::at(container, key), std::forward<TValue>(value));
    }
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
  constexpr void reserve(TContainer& c, std::size_t n) noexcept
  {
    c.reserve(n);
  };

  template<typename TContainer>
  constexpr void reserve(TContainer&, std::size_t) noexcept {};

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
