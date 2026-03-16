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
#include "stl.h"
#include "traits.h"
#include <iomanip>
#include <iostream>
#include <string>
#include <type_traits>
#include <vector>

namespace OrthoTree
{
  class JSONArchive;
  class XMLArchive;
  class JSONOutputArchive;
  class XMLOutputArchive;

  // Specialization for traits
  template<>
  struct is_stl_serialization_enabled<JSONArchive> : std::true_type
  {};
  template<>
  struct is_stl_serialization_enabled<JSONOutputArchive> : std::true_type
  {};
  template<>
  struct is_stl_serialization_enabled<XMLArchive> : std::true_type
  {};
  template<>
  struct is_stl_serialization_enabled<XMLOutputArchive> : std::true_type
  {};

  class JSONArchive
  {
  protected:
    std::ostream& m_os;
    int m_indent_level = 0;
    bool m_first_item = true;

    void indent()
    {
      for (int i = 0; i < m_indent_level; ++i)
        m_os << "  ";
    }

  public:
    JSONArchive(std::ostream& os)
    : m_os(os)
    {}

    constexpr bool is_loading() const { return false; }
    constexpr bool is_saving() const { return true; }

    // Support any type that looks like an NVP (has .name and .value)
    template<typename TNVP>
    auto operator&(TNVP&& nvp) -> decltype(nvp.name, nvp.value, *this)
    {
      if (!m_first_item)
        m_os << ",";
      m_first_item = false;
      m_os << "\n";
      indent();
      m_os << "\"" << nvp.name << "\": ";
      serialize_value(nvp.value);
      return *this;
    }

    template<typename T>
    JSONArchive& operator&(T& val)
    {
      if (!m_first_item)
        m_os << ",";
      m_first_item = false;
      m_os << "\n";
      indent();
      serialize_value(val);
      return *this;
    }

    template<typename... Args>
    JSONArchive& operator()(Args&&... args)
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
        if constexpr (std::is_same_v<T, bool>)
          m_os << (val ? "true" : "false");
        else
          m_os << val;
      }
      else if constexpr (std::is_same_v<T, std::string>)
      {
        m_os << "\"" << val << "\"";
      }
      else if constexpr (requires { val.serialize(*this); })
      {
        m_os << "{";
        m_indent_level++;
        bool old_first = m_first_item;
        m_first_item = true;

        val.serialize(*this);

        m_indent_level--;
        m_os << "\n";
        indent();
        m_os << "}";
        m_first_item = old_first;
      }
      else if constexpr (is_stl_serialization_enabled_v<JSONArchive>)
      {
        serialize(*this, val, version_v<T>);
      }
      else if constexpr (requires {
                           val.begin();
                           val.end();
                         })
      {
        m_os << "[";
        m_indent_level++;
        bool first = true;
        for (auto& item : val)
        {
          if (!first)
            m_os << ", ";
          serialize_value(item);
          first = false;
        }
        m_indent_level--;
        m_os << "]";
      }
      else
      {
        // Fallback for objects without member serialize and when STL headers are not included
        m_os << "{";
        m_indent_level++;
        bool old_first = m_first_item;
        m_first_item = true;

        serialize(*this, val, version_v<T>);

        m_indent_level--;
        m_os << "\n";
        indent();
        m_os << "}";
        m_first_item = old_first;
      }
    }
  };

  class JSONOutputArchive : public JSONArchive
  {
  public:
    JSONOutputArchive(std::ostream& os)
    : JSONArchive(os)
    {
      m_os << "{";
    }
    ~JSONOutputArchive() { m_os << "\n}\n"; }
  };


  class XMLArchive
  {
  protected:
    std::ostream& m_os;
    int m_indent_level = 0;

    void indent()
    {
      for (int i = 0; i < m_indent_level; ++i)
        m_os << "  ";
    }

  public:
    XMLArchive(std::ostream& os)
    : m_os(os)
    {}

    constexpr bool is_loading() const { return false; }
    constexpr bool is_saving() const { return true; }

    // Support any type that looks like an NVP (has .name and .value)
    template<typename TNVP>
    auto operator&(TNVP&& nvp) -> decltype(nvp.name, nvp.value, *this)
    {
      indent();
      m_os << "<" << nvp.name << ">";
      serialize_value(nvp.value);
      m_os << "</" << nvp.name << ">\n";
      return *this;
    }

    template<typename T>
    XMLArchive& operator&(T& val)
    {
      indent();
      m_os << "<value>";
      serialize_value(val);
      m_os << "</value>\n";
      return *this;
    }

    template<typename... Args>
    XMLArchive& operator()(Args&&... args)
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
        m_os << val;
      }
      else if constexpr (std::is_same_v<T, std::string>)
      {
        m_os << val;
      }
      else if constexpr (requires { val.serialize(*this); })
      {
        val.serialize(*this);
      }
      else if constexpr (is_stl_serialization_enabled_v<XMLArchive>)
      {
        m_os << "\n";
        m_indent_level++;
        serialize(*this, val, version_v<T>);
        m_indent_level--;
        indent();
      }
      else if constexpr (requires {
                           val.begin();
                           val.end();
                         })
      {
        m_os << "\n";
        m_indent_level++;
        for (auto& item : val)
        {
          indent();
          m_os << "<item>";
          serialize_value(item);
          m_os << "</item>\n";
        }
        m_indent_level--;
        indent();
      }
      else
      {
        // No serialization found
      }
    }
  };

  class XMLOutputArchive : public XMLArchive
  {
  public:
    XMLOutputArchive(std::ostream& os)
    : XMLArchive(os)
    {}
  };


} // namespace OrthoTree
