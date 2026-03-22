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

#ifndef ORTHOTREE_SERIALIZATION_BINARY_ARCHIVE_H
#define ORTHOTREE_SERIALIZATION_BINARY_ARCHIVE_H
#endif

#include "stl.h"
#include "nvp.h"
#include "traits.h"


#include <bit>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

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
