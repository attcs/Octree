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
#if __has_include(<cereal/cereal.hpp>)
  #include <cereal/cereal.hpp>
  #define ORTHOTREE_SERIALIZATION_CEREAL_ENABLED
#endif

#if __has_include(<boost/serialization/nvp.hpp>)
  #include <boost/serialization/nvp.hpp>
  #define ORTHOTREE_SERIALIZATION_BOOST_ENABLED
#endif


namespace OrthoTree
{
  template<typename T>
  struct NameValuePair
  {
    const char* name;
    T& value;

    constexpr NameValuePair(const char* n, T& v) noexcept : name(n), value(v) {}

    // Support for cereal NVP detection traits
    using cereal_nvp_tag = std::true_type;
  };

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

#ifndef ORTHOTREE_NVP
#define ORTHOTREE_NVP(member) ::OrthoTree::make_nvp(#member, member)
#endif

#ifndef ORTHOTREE_NVP_M
#define ORTHOTREE_NVP_M(obj, member) ::OrthoTree::make_nvp(#member, obj.member)
#endif

  // Generic bridge for other archivers
  template<typename TArchive, typename T>
  auto operator&(TArchive& ar, NameValuePair<T> nvp) -> decltype(ar(nvp.value), ar)
  {
#if defined(ORTHOTREE_SERIALIZATION_CEREAL_ENABLED)
    // Bridge to cereal NVP
    ar(::cereal::make_nvp<TArchive>(nvp.name, nvp.value));
    return ar;
#elif defined(ORTHOTREE_SERIALIZATION_BOOST_ENABLED)
    // Bridge to boost NVP
    ar(::boost::serialization::make_nvp(nvp.name, nvp.value));
    return ar;
#else
    // Fallback
    ar(nvp.value);
    return ar;
#endif
  }

} // namespace OrthoTree
