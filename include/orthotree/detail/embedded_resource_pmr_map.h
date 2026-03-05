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

#include <iterator>
#include <memory_resource>

#include <type_traits>
#include <utility>

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
