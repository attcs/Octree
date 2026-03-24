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

#include <type_traits>

// Optional third-party support
#if defined(CEREAL_CEREAL_HPP_)
#define ORTHOTREE_SERIALIZATION_CEREAL_ENABLED
#endif

#if defined(BOOST_SERIALIZATION_NVP_HPP)
#include <boost/serialization/nvp.hpp>
#define ORTHOTREE_SERIALIZATION_BOOST_ENABLED
#endif

#include "traits.h"


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
