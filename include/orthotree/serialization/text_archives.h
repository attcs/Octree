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
  class JSONInputArchive;
  class XMLInputArchive;

  // Specialization for traits
  template<>
  struct is_stl_serialization_enabled<JSONArchive> : std::true_type
  {};
  template<>
  struct is_stl_serialization_enabled<JSONOutputArchive> : std::true_type
  {};
  template<>
  struct is_stl_serialization_enabled<JSONInputArchive> : std::true_type
  {};
  template<>
  struct is_stl_serialization_enabled<XMLArchive> : std::true_type
  {};
  template<>
  struct is_stl_serialization_enabled<XMLOutputArchive> : std::true_type
  {};
  template<>
  struct is_stl_serialization_enabled<XMLInputArchive> : std::true_type
  {};

  class JSONArchive
  {
  protected:
    std::ostream& m_os;
    int m_indent_level = 0;
    bool m_first_item = true;
    bool m_deferred_scope = false;
    bool m_in_array = false;

    void write_indent()
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

    // SizeTag: signals array mode
    template<typename T>
    JSONArchive& operator&(SizeTag<T> tag)
    {
      m_os << "[";
      m_indent_level++;
      m_deferred_scope = false;
      m_in_array = true;
      m_first_item = true;
      return *this;
    }

    // Support any type that looks like an NVP (has .name and .value)
    template<typename TNVP>
    requires(requires(TNVP nvp) { detail::get_nvp_name(nvp); detail::get_nvp_value(nvp); })
    auto operator&(TNVP&& nvp) -> JSONArchive&
    {
      if (m_deferred_scope)
      {
        m_os << "{";
        m_indent_level++;
        m_deferred_scope = false;
      }

      if (!m_first_item)
        m_os << ",";
      m_first_item = false;
      m_os << "\n";
      write_indent();

      if (m_in_array)
        serialize_value(detail::get_nvp_value(nvp));
      else
      {
        m_os << "\"" << detail::get_nvp_name(nvp) << "\": ";
        serialize_value(detail::get_nvp_value(nvp));
      }
      return *this;
    }

    template<typename T>
    auto operator&(T& val) -> std::enable_if_t<!is_size_tag_v<std::remove_cv_t<T>>, JSONArchive&>
    {
      if (m_deferred_scope)
      {
        m_os << "{";
        m_indent_level++;
        m_deferred_scope = false;
      }

      if (!m_first_item)
        m_os << ",";
      m_first_item = false;
      m_os << "\n";
      write_indent();
      serialize_value(val);
      return *this;
    }

    template<typename T>
    auto operator&(const T& val) -> std::enable_if_t<!is_size_tag_v<std::remove_cv_t<T>>, JSONArchive&>
    {
      return (*this & const_cast<T&>(val));
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
        write_indent();
        m_os << "}";
        m_first_item = old_first;
      }
      else if constexpr (is_stl_serialization_enabled_v<JSONArchive>)
      {
        bool prev_deferred = m_deferred_scope;
        bool prev_in_array = m_in_array;
        bool prev_first = m_first_item;

        m_deferred_scope = true;
        m_in_array = false;
        m_first_item = true;

        serialize(*this, const_cast<std::remove_const_t<T>&>(val));

        if (m_in_array)
        {
          m_indent_level--;
          m_os << "\n";
          write_indent();
          m_os << "}";
        }

        m_deferred_scope = prev_deferred;
        m_in_array = prev_in_array;
        m_first_item = prev_first;
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
            m_os << ",";
          first = false;
          m_os << "\n";
          write_indent();
          serialize_value(item);
        }
        m_indent_level--;
        m_os << "\n";
        write_indent();
        m_os << "]";
      }
      else
      {
        // Fallback for objects without member serialize and when STL headers are not included
        m_os << "{";
        m_indent_level++;
        bool old_first = m_first_item;
        m_first_item = true;

        serialize(*this, const_cast<std::remove_const_t<T>&>(val));

        m_indent_level--;
        m_os << "\n";
        write_indent();
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

  class JSONInputArchive
  {
  protected:
    std::istream& m_is;

    void skip_ws()
    {
      while (m_is && std::isspace(m_is.peek()))
        m_is.ignore();
    }

    bool expect(char c)
    {
      skip_ws();
      if (m_is.peek() == c)
      {
        m_is.ignore();
        return true;
      }
      return false;
    }

    std::string read_string()
    {
      skip_ws();
      std::string s;
      if (expect('"'))
      {
        while (m_is && m_is.peek() != '"')
          s += (char)m_is.get();
        expect('"');
      }
      return s;
    }

  public:
    JSONInputArchive(std::istream& is)
    : m_is(is)
    {
      expect('{');
    }
    ~JSONInputArchive() { expect('}'); }

    constexpr bool is_loading() const { return true; }
    constexpr bool is_saving() const { return false; }

    template<typename T>
    JSONInputArchive& operator&(SizeTag<T>& tag)
    {
      expect('[');
      return *this;
    }

    template<typename TNVP>
    requires(requires(TNVP nvp) { detail::get_nvp_name(nvp); detail::get_nvp_value(nvp); })
    auto operator&(TNVP&& nvp) -> JSONInputArchive&
    {
      skip_ws();
      if (m_is.peek() == ',')
        m_is.ignore();

      std::string name = read_string();
      expect(':');
      serialize_value(detail::get_nvp_value(nvp));
      return *this;
    }

    template<typename T>
    auto operator&(T& val) -> std::enable_if_t<!is_size_tag_v<std::remove_cv_t<T>>, JSONInputArchive&>
    {
      skip_ws();
      if (m_is.peek() == ',')
        m_is.ignore();
      serialize_value(val);
      return *this;
    }

    template<typename... Args>
    JSONInputArchive& operator()(Args&&... args)
    {
      ((*this & std::forward<Args>(args)), ...);
      return *this;
    }

  protected:
    template<typename T>
    void serialize_value(T& val)
    {
      skip_ws();
      if constexpr (std::is_arithmetic_v<T>)
      {
        if constexpr (std::is_same_v<T, bool>)
        {
          std::string s;
          while (m_is && std::isalpha(m_is.peek()))
            s += (char)m_is.get();
          val = (s == "true");
        }
        else
        {
          m_is >> val;
        }
      }
      else if constexpr (std::is_same_v<T, std::string>)
      {
        val = read_string();
      }
      else if constexpr (requires { val.serialize(*this); })
      {
        expect('{');
        val.serialize(*this);
        expect('}');
      }
      else if constexpr (is_stl_serialization_enabled_v<JSONInputArchive>)
      {
        serialize(*this, val);
        // arrays and maps might need extra handling for closing brackets
        if constexpr (requires { val.resize(0); }) // Simple vector-like
        {
           expect(']');
        }
      }
      else if constexpr (requires {
                           val.begin();
                           val.end();
                         })
      {
        expect('[');
        val.clear();
        while (m_is)
        {
          skip_ws();
          if (m_is.peek() == ']')
            break;
          if (!val.empty())
            expect(',');
          
          typename std::remove_reference_t<decltype(*val.begin())> item;
          serialize_value(item);
          val.insert(val.end(), std::move(item));
        }
        expect(']');
      }
    }
  };

  class XMLInputArchive
  {
  protected:
    std::istream& m_is;

    void skip_ws()
    {
      while (m_is && std::isspace(m_is.peek()))
        m_is.ignore();
    }

    bool expect(char c)
    {
      skip_ws();
      if (m_is.peek() == c)
      {
        m_is.ignore();
        return true;
      }
      return false;
    }

    std::string read_tag()
    {
      skip_ws();
      std::string s;
      if (expect('<'))
      {
        while (m_is && m_is.peek() != '>')
          s += (char)m_is.get();
        expect('>');
      }
      return s;
    }

  public:
    XMLInputArchive(std::istream& is)
    : m_is(is)
    {}

    constexpr bool is_loading() const { return true; }
    constexpr bool is_saving() const { return false; }

    template<typename T>
    XMLInputArchive& operator&(SizeTag<T>& tag)
    {
      return *this;
    }

    template<typename TNVP>
    requires(requires(TNVP nvp) { detail::get_nvp_name(nvp); detail::get_nvp_value(nvp); })
    auto operator&(TNVP&& nvp) -> XMLInputArchive&
    {
      read_tag(); // <name>
      serialize_value(detail::get_nvp_value(nvp));
      read_tag(); // </name>
      return *this;
    }

    template<typename T>
    auto operator&(T& val) -> std::enable_if_t<!is_size_tag_v<std::remove_cv_t<T>>, XMLInputArchive&>
    {
      read_tag(); // <value>
      serialize_value(val);
      read_tag(); // </value>
      return *this;
    }

    template<typename... Args>
    XMLInputArchive& operator()(Args&&... args)
    {
      ((*this & std::forward<Args>(args)), ...);
      return *this;
    }

  protected:
    template<typename T>
    void serialize_value(T& val)
    {
      skip_ws();
      if constexpr (std::is_arithmetic_v<T>)
      {
        m_is >> val;
      }
      else if constexpr (std::is_same_v<T, std::string>)
      {
        while (m_is && m_is.peek() != '<')
          val += (char)m_is.get();
      }
      else if constexpr (requires { val.serialize(*this); })
      {
        val.serialize(*this);
      }
      else if constexpr (is_stl_serialization_enabled_v<XMLInputArchive>)
      {
        serialize(*this, val);
      }
    }
  };


  class XMLArchive
  {
  protected:
    std::ostream& m_os;
    int m_indent_level = 0;

    void write_indent()
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

    // SizeTag: array sizes are implicit in XML (derived from child count)
    template<typename T>
    XMLArchive& operator&(SizeTag<T>)
    {
      return *this;
    }

    // Support any type that looks like an NVP (has .name and .value)
    template<typename TNVP>
    requires(requires(TNVP nvp) { detail::get_nvp_name(nvp); detail::get_nvp_value(nvp); })
    auto operator&(TNVP&& nvp) -> XMLArchive&
    {
      write_indent();
      m_os << "<" << detail::get_nvp_name(nvp) << ">";
      serialize_value(detail::get_nvp_value(nvp));
      m_os << "</" << detail::get_nvp_name(nvp) << ">\n";
      return *this;
    }

    template<typename T>
    auto operator&(T& val) -> std::enable_if_t<!is_size_tag_v<std::remove_cv_t<T>>, XMLArchive&>
    {
      write_indent();
      m_os << "<value>";
      serialize_value(val);
      m_os << "</value>\n";
      return *this;
    }

    template<typename T>
    auto operator&(const T& val) -> std::enable_if_t<!is_size_tag_v<std::remove_cv_t<T>>, XMLArchive&>
    {
      return (*this & const_cast<T&>(val));
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
        serialize(*this, const_cast<std::remove_const_t<T>&>(val));
        m_indent_level--;
        write_indent();
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
          write_indent();
          m_os << "<item>";
          serialize_value(item);
          m_os << "</item>\n";
        }
        m_indent_level--;
        write_indent();
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
