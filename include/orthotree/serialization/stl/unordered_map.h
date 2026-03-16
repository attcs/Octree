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
#include <unordered_map>
#include <type_traits>

namespace OrthoTree
{
  // std::unordered_map
  template<typename TArchive, typename K, typename V, typename Hash, typename KeyEqual, typename Alloc>
  auto serialize(TArchive& ar, std::unordered_map<K, V, Hash, KeyEqual, Alloc>& val, [[maybe_unused]] const unsigned int version)
    -> std::enable_if_t<is_stl_serialization_enabled_v<TArchive>>
  {
    std::size_t size = val.size();
    ar& make_size_tag(size);

    if (ar.is_loading())
    {
      val.clear();
      for (std::size_t i = 0; i < size; ++i)
      {
        std::pair<K, V> item;
        ar & make_nvp("item", item);
        val.emplace(std::move(item.first), std::move(item.second));
      }
    }
    else
    {
      for (auto& item : val)
      {
        ar& make_nvp("item", const_cast<std::pair<K const, V>&>(item));
      }
    }
  }

} // namespace OrthoTree
