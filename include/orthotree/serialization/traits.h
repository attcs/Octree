#pragma once

#include <type_traits>
#include <cstdint>

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
  struct version<T, std::void_t<decltype(T::dataRepresentionVersion)>>
  {
    static constexpr uint32_t value = T::dataRepresentionVersion;
  };

  template<typename T>
  inline constexpr uint32_t version_v = version<T>::value;

} // namespace OrthoTree
