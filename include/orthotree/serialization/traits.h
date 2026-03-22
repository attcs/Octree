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

#include "../core/types.h"
#include <cstdint>
#include <type_traits>

#if defined(BOOST_ARCHIVE_DETAIL_INTERFACE_IARCHIVE_HPP) || defined(BOOST_ARCHIVE_DETAIL_INTERFACE_OARCHIVE_HPP)
#define ORTHOTREE_SERIALIZATION_BOOST_ARCHIVE_AVAILABLE
#endif

namespace cereal
{
  namespace detail
  {
    class OutputArchiveBase;
    class InputArchiveBase;
  } // namespace detail
} // namespace cereal

namespace boost
{
  namespace archive
  {
    namespace detail
    {
      template<class Archive>
      class interface_iarchive;

      template<class Archive>
      class interface_oarchive;
    } // namespace detail
  } // namespace archive
} // namespace boost

namespace OrthoTree
{
  // --- Versioning ---
  // Default version is 0.
  // Can be specialized for specific types or picked up from class members.
  template<typename T, typename = void>
  struct version
  {
    static constexpr uint32_t value = 0;
  };

  // Trait to pick up version from class if it exists
  template<typename T>
  struct version<T, std::void_t<decltype(T::SERIALIZED_VERSION_ID)>>
  {
    static constexpr uint32_t value = T::SERIALIZED_VERSION_ID;
  };

  template<typename T>
  inline constexpr uint32_t version_v = version<T>::value;


  // --- Archive Identification ---
  template<typename T>
  struct is_orthotree_archive : std::false_type
  {};

  template<typename T>
  inline constexpr bool is_orthotree_archive_v = is_orthotree_archive<T>::value;

  template<typename T>
  struct is_stl_serialization_enabled : std::false_type
  {};

  template<typename T>
  inline constexpr bool is_stl_serialization_enabled_v = is_stl_serialization_enabled<T>::value;


  namespace detail
  {
    template<typename T>
    struct is_cereal_archive
    : std::bool_constant<
        std::is_base_of_v<::cereal::detail::InputArchiveBase, std::decay_t<T>> || std::is_base_of_v<::cereal::detail::OutputArchiveBase, std::decay_t<T>>>
    {};

    template<typename T>
    inline constexpr bool is_cereal_archive_v = is_cereal_archive<T>::value;

#ifdef ORTHOTREE_SERIALIZATION_BOOST_ARCHIVE_AVAILABLE
    template<typename T>
    struct is_boost_archive : std::bool_constant<
                                std::is_base_of_v<::boost::archive::detail::interface_iarchive<std::decay_t<T>>, std::decay_t<T>> ||
                                std::is_base_of_v<::boost::archive::detail::interface_oarchive<std::decay_t<T>>, std::decay_t<T>>>
    {};
#else
    template<typename T>
    struct is_boost_archive : std::false_type
    {};
#endif

    template<typename T>
    inline constexpr bool is_boost_archive_v = is_boost_archive<T>::value;
  } // namespace detail


  // --- Factory for non-default constructible types ---
  template<typename TArchive, typename T>
  struct load_construct
  {
    static T load(TArchive& ar)
    {
      if constexpr (std::is_default_constructible_v<T>)
      {
        T val;
        ar & val;
        return val;
      }
      else
      {
        static_assert(std::is_default_constructible_v<T>, "Type T is not default constructible. Please specialize OrthoTree::load_construct for your type.");
        return T{};
      }
    }
  };

  // --- Archive Traits Helpers ---
  template<typename TArchive, typename = void>
  struct archive_traits
  {
    static bool is_loading(TArchive const& ar) { return ar.is_loading(); }
  };

  template<typename TArchive>
  struct archive_traits<TArchive, std::void_t<typename TArchive::is_loading>>
  {
    static bool is_loading(TArchive const&) { return TArchive::is_loading::value; }
  };

  template<typename TArchive>
  bool is_loading_archive(TArchive const& ar)
  {
    return archive_traits<TArchive>::is_loading(ar);
  }


  // --- NVP Identification ---
  template<typename T>
  struct is_nvp : std::false_type
  {};

  template<typename T>
  inline constexpr bool is_nvp_v = is_nvp<std::remove_cvref_t<T>>::value;


} // namespace OrthoTree
