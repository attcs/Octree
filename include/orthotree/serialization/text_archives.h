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

#include "../serialization/stl.h"

#include "nvp.h"
#include "traits.h"

#include <iostream>
#include <stack>
#include <string>
#include <type_traits>
#include <vector>

namespace OrthoTree
{
  class JSONOutputArchive
  {
    std::ostream& m_os;
    int m_depth = 0;
    bool m_first_item = true;

    void indent()
    {
      m_os << "\n";
      for (int i = 0; i < m_depth; ++i)
        m_os << "  ";
    }

  public:
    JSONOutputArchive(std::ostream& os)
    : m_os(os)
    {
      m_os << "{";
      m_depth++;
    }

    ~JSONOutputArchive()
    {
      m_depth--;
      indent();
      m_os << "}";
    }

    constexpr bool is_loading() const { return false; }
    constexpr bool is_saving() const { return true; }

    // Support any type that looks like an NVP (has .name and .value)
    template<typename TNVP>
    auto operator&(TNVP&& nvp) -> decltype(nvp.name, nvp.value, *this)
    {
      if (!m_first_item)
        m_os << ",";
      m_first_item = false;
      indent();
      m_os << "\"" << nvp.name << "\": ";
      serialize_value(nvp.value);
      return *this;
    }

    template<typename T>
    JSONOutputArchive& operator&(T& val)
    {
      serialize_value(val);
      return *this;
    }

    template<typename... Args>
    JSONOutputArchive& operator()(Args&&... args)
    {
      ((*this & std::forward<Args>(args)), ...);
      return *this;
    }

  private:
    template<typename T>
    void serialize_value(T& val)
    {
      if constexpr (std::is_arithmetic_v<T>)
      {
        if constexpr (std::is_same_v<T, bool>)
          m_os << (val ? "true" : "false");
        else
          m_os << val;
      }
      else if constexpr (std::is_same_v<T, std::string>)
      {
        m_os << "\"" << val << "\"";
      }
      else if constexpr (requires {
                           typename T::key_type;
                           typename T::mapped_type;
                         })
      {
        // Map serialization as JSON object (if key is string) or array of pairs
        bool keys_are_strings = std::is_base_of_v<std::string, typename T::key_type> || std::is_same_v<typename T::key_type, std::string>;
        m_os << (keys_are_strings ? "{" : "[");
        m_depth++;
        bool first = true;
        for (auto& [key, value] : val)
        {
          if (!first)
            m_os << ", ";
          if (keys_are_strings)
          {
            indent();
            m_os << "\"" << key << "\": ";
            serialize_value(value);
          }
          else
          {
            m_os << "{";
            m_depth++;
            m_first_item = true; // reset for the inner pair
            *this& make_nvp("key", const_cast<typename T::key_type&>(key));
            *this& make_nvp("value", value);
            m_depth--;
            indent();
            m_os << "}";
          }
          first = false;
        }
        m_depth--;
        indent();
        m_os << (keys_are_strings ? "}" : "]");
      }
      else if constexpr (requires {
                           val.begin();
                           val.end();
                         })
      {
        // Container serialization as JSON array
        m_os << "[";
        m_depth++;
        bool first = true;
        for (auto& item : val)
        {
          if (!first)
            m_os << ", ";
          serialize_value(item);
          first = false;
        }
        m_depth--;
        m_os << "]";
      }
      else
      {
        // Custom object serialization
        m_os << "{";
        m_depth++;
        bool old_first = m_first_item;
        m_first_item = true;

        serialize(*this, val, version_v<T>);

        m_depth--;
        indent();
        m_os << "}";
        m_first_item = old_first;
      }
    }
  };

  // Minimal implementation for XML saving
  class XMLOutputArchive
  {
    std::ostream& m_os;
    int m_depth = 0;
    std::stack<std::string> m_tags;

    void indent()
    {
      m_os << "\n";
      for (int i = 0; i < m_depth; ++i)
        m_os << "  ";
    }

  public:
    XMLOutputArchive(std::ostream& os)
    : m_os(os)
    {
      m_os << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
      m_os << "\n<root>";
      m_depth++;
    }

    ~XMLOutputArchive()
    {
      m_depth--;
      m_os << "\n</root>";
    }

    constexpr bool is_loading() const { return false; }
    constexpr bool is_saving() const { return true; }

    // Support any type that looks like an NVP (has .name and .value)
    template<typename TNVP>
    auto operator&(TNVP&& nvp) -> decltype(nvp.name, nvp.value, *this)
    {
      indent();
      m_os << "<" << nvp.name << ">";

      bool has_items = false;
      if constexpr (requires {
                      nvp.value.begin();
                      nvp.value.end();
                    })
      {
        if constexpr (!std::is_same_v<std::decay_t<decltype(nvp.value)>, std::string>)
          has_items = true;
      }

      if (has_items)
        m_depth++;

      serialize_value(nvp.value);

      if (has_items)
      {
        m_depth--;
        indent();
      }

      m_os << "</" << nvp.name << ">";
      return *this;
    }

    template<typename T>
    XMLOutputArchive& operator&(T& val)
    {
      serialize_value(val);
      return *this;
    }

    template<typename... Args>
    XMLOutputArchive& operator()(Args&&... args)
    {
      ((*this & std::forward<Args>(args)), ...);
      return *this;
    }

  private:
    template<typename T>
    void serialize_value(T& val)
    {
      if constexpr (std::is_arithmetic_v<T>)
      {
        if constexpr (std::is_same_v<T, bool>)
          m_os << (val ? "true" : "false");
        else
          m_os << val;
      }
      else if constexpr (std::is_same_v<T, std::string>)
      {
        m_os << val;
      }
      else if constexpr (requires {
                           val.begin();
                           val.end();
                         })
      {
        // Container serialization as XML items
        for (auto& item : val)
        {
          *this& make_nvp("item", item);
        }
      }
      else
      {
        serialize(*this, val, version_v<T>);
      }
    }
  };

} // namespace OrthoTree
