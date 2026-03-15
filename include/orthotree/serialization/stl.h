/*
MIT License

Copyright (c) 2026 Attila Csikós

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

#include "nvp.h"
#include "traits.h"
#include <array>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <variant>
#include <string>
#include <optional>
#include <memory>

// Header for future STL containers if available
#if __has_include(<inplace_vector>)
#include <inplace_vector>
#endif

namespace OrthoTree
{
  // --- Factory for non-default constructible types ---
  template<typename TArchive, typename T>
  struct load_construct
  {
    static T load(TArchive& ar)
    {
      if constexpr (std::is_default_constructible_v<T>)
      {
        T val;
        ar& val;
        return val;
      }
      else
      {
        static_assert(std::is_default_constructible_v<T>, 
          "Type T is not default constructible. Please specialize OrthoTree::load_construct for your type.");
        return T{}; // Should not be reached
      }
    }
  };


  // --- STL Container Serialization ---

  // std::vector
  template<typename TArchive, typename T, typename Alloc>
  void serialize(TArchive& ar, std::vector<T, Alloc>& val, [[maybe_unused]] const unsigned int version)
  {
    std::size_t size = val.size();
    ar& make_nvp("size", size);
    if (ar.is_loading())
      val.resize(size);

    for (auto& item : val)
      ar& item;
  }

  // std::array
  template<typename TArchive, typename T, std::size_t N>
  void serialize(TArchive& ar, std::array<T, N>& val, [[maybe_unused]] const unsigned int version)
  {
    for (auto& item : val)
      ar& item;
  }

  // std::map
  template<typename TArchive, typename K, typename V, typename Compare, typename Alloc>
  void serialize(TArchive& ar, std::map<K, V, Compare, Alloc>& val, [[maybe_unused]] const unsigned int version)
  {
    std::size_t size = val.size();
    ar& make_nvp("size", size);

    if (ar.is_loading())
    {
      val.clear();
      for (std::size_t i = 0; i < size; ++i)
      {
        K key = load_construct<TArchive, K>::load(ar);
        V value = load_construct<TArchive, V>::load(ar);
        val.emplace(std::move(key), std::move(value));
      }
    }
    else
    {
      for (auto& [key, value] : val)
      {
        ar& const_cast<K&>(key);
        ar& value;
      }
    }
  }

  // std::unordered_map
  template<typename TArchive, typename K, typename V, typename Hash, typename KeyEqual, typename Alloc>
  void serialize(TArchive& ar, std::unordered_map<K, V, Hash, KeyEqual, Alloc>& val, [[maybe_unused]] const unsigned int version)
  {
    std::size_t size = val.size();
    ar& make_nvp("size", size);

    if (ar.is_loading())
    {
      val.clear();
      for (std::size_t i = 0; i < size; ++i)
      {
        K key = load_construct<TArchive, K>::load(ar);
        V value = load_construct<TArchive, V>::load(ar);
        val.emplace(std::move(key), std::move(value));
      }
    }
    else
    {
      for (auto& [key, value] : val)
      {
        ar& const_cast<K&>(key);
        ar& value;
      }
    }
  }

  // std::set
  template<typename TArchive, typename K, typename Compare, typename Alloc>
  void serialize(TArchive& ar, std::set<K, Compare, Alloc>& val, [[maybe_unused]] const unsigned int version)
  {
    std::size_t size = val.size();
    ar& make_nvp("size", size);

    if (ar.is_loading())
    {
      val.clear();
      for (std::size_t i = 0; i < size; ++i)
      {
        K key = load_construct<TArchive, K>::load(ar);
        val.emplace(std::move(key));
      }
    }
    else
    {
      for (auto& item : val)
        ar& const_cast<K&>(item);
    }
  }

  // std::optional
  template<typename TArchive, typename T>
  void serialize(TArchive& ar, std::optional<T>& val, [[maybe_unused]] const unsigned int version)
  {
    bool has_value = val.has_value();
    ar& make_nvp("has_value", has_value);

    if (ar.is_loading())
    {
      if (has_value)
        val = load_construct<TArchive, T>::load(ar);
      else
        val = std::nullopt;
    }
    else
    {
      if (has_value)
        ar& *val;
    }
  }

  // std::unique_ptr
  template<typename TArchive, typename T, typename D>
  void serialize(TArchive& ar, std::unique_ptr<T, D>& val, [[maybe_unused]] const unsigned int version)
  {
    bool has_value = (val != nullptr);
    ar& make_nvp("has_value", has_value);

    if (ar.is_loading())
    {
      if (has_value)
        val = std::unique_ptr<T, D>(new T(load_construct<TArchive, T>::load(ar)));
      else
        val.reset();
    }
    else
    {
      if (has_value)
        ar& *val;
    }
  }

  // std::shared_ptr
  template<typename TArchive, typename T>
  void serialize(TArchive& ar, std::shared_ptr<T>& val, [[maybe_unused]] const unsigned int version)
  {
    bool has_value = (val != nullptr);
    ar& make_nvp("has_value", has_value);

    if (ar.is_loading())
    {
      if (has_value)
        val = std::make_shared<T>(load_construct<TArchive, T>::load(ar));
      else
        val.reset();
    }
    else
    {
      if (has_value)
        ar& *val;
    }
  }

  // std::variant
  template<typename TArchive, typename... Types>
  void serialize(TArchive& ar, std::variant<Types...>& val, [[maybe_unused]] const unsigned int version)
  {
    std::size_t index = val.index();
    ar& make_nvp("index", index);

    if (ar.is_loading())
    {
      auto load_variant_index = [&]<std::size_t I>(std::integral_constant<std::size_t, I>) {
        if (I == index) {
          val = load_construct<TArchive, std::variant_alternative_t<I, std::variant<Types...>>>::load(ar);
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
      std::visit([&ar](auto& item) {
        ar& item;
      }, val);
    }
  }

#if __has_include(<inplace_vector>)
  // std::inplace_vector (C++26)
  template<typename TArchive, typename T, std::size_t N>
  void serialize(TArchive& ar, std::inplace_vector<T, N>& val, [[maybe_unused]] const unsigned int version)
  {
    std::size_t size = val.size();
    ar& make_nvp("size", size);
    if (ar.is_loading())
    {
        val.clear();
        for(std::size_t i = 0; i < size; ++i)
        {
           val.push_back(load_construct<TArchive, T>::load(ar));
        }
    }
    else
    {
        for (auto& item : val)
            ar& item;
    }
  }
#endif

} // namespace OrthoTree
