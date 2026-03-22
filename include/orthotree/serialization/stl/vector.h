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

#include "../nvp.h"
#include "../traits.h"
#include "common.h"
#include <type_traits>
#include <vector>

#ifdef __cpp_lib_constexpr_inplace_vector
#include <inplace_vector>
#endif

namespace OrthoTree
{
  // std::vector
  template<typename TArchive, typename T, typename Alloc>
  auto serialize(TArchive& ar, std::vector<T, Alloc>& val)
    -> std::enable_if_t<is_stl_serialization_enabled_v<TArchive>>
  {
    serialized_size_t size = static_cast<serialized_size_t>(val.size());
    ar& make_size_tag(size);
    if (ar.is_loading())
    {
      val.resize(size);
      for (auto& item : val)
        ar& item;
    }
    else
    {
      for (auto& item : val)
        ar& item;
    }
  }

#ifdef __cpp_lib_constexpr_inplace_vector
  // std::inplace_vector (C++26)
  template<typename TArchive, typename T, std::size_t N>
  auto serialize(TArchive& ar, std::inplace_vector<T, N>& val)
    -> std::enable_if_t<is_stl_serialization_enabled_v<TArchive>>
  {
    serialized_size_t size = static_cast<serialized_size_t>(val.size());
    ar& make_size_tag(size);
    if (ar.is_loading())
    {
      val.clear();
      for (std::size_t i = 0; i < size; ++i)
      {
        val.push_back(load_construct<TArchive, T>::load(ar));
      }
    }
    else
    {
      for (auto& item : val)
        ar& item;
    }
  }
#endif

} // namespace OrthoTree
