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

#include <cstdint>
#include <type_traits>

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
  struct is_stl_serialization_enabled : std::false_type
  {};

  template<typename T>
  inline constexpr bool is_stl_serialization_enabled_v = is_stl_serialization_enabled<T>::value;


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

} // namespace OrthoTree
