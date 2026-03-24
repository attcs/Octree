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
#include <set>
#include <type_traits>

namespace OrthoTree
{
  // std::set
  template<typename TArchive, typename K, typename Compare, typename Alloc>
  auto serialize(TArchive& ar, std::set<K, Compare, Alloc>& val)
    -> std::enable_if_t<is_stl_serialization_enabled_v<TArchive>>
  {
    serialized_size_t size = static_cast<serialized_size_t>(val.size());
    ar& make_size_tag(size);

    if (ar.is_loading())
    {
      val.clear();
      for (std::size_t i = 0; i < size; ++i)
      {
        K key;
        ar& key;
        val.emplace(std::move(key));
      }
    }
    else
    {
      for (auto& item : val)
        ar& const_cast<K&>(item);
    }
  }

} // namespace OrthoTree
