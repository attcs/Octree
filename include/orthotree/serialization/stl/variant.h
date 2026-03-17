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
#include "common.h"
#include <variant>
#include <utility>
#include <type_traits>

namespace OrthoTree
{
  // std::variant
  template<typename TArchive, typename... Types>
  auto serialize(TArchive& ar, std::variant<Types...>& val)
    -> std::enable_if_t<is_stl_serialization_enabled_v<TArchive>>
  {
    std::size_t index = val.index();
    ar& make_nvp("index", index);

    if (ar.is_loading())
    {
      auto load_variant_index = [&]<std::size_t I>(std::integral_constant<std::size_t, I>) {
        if (I == index)
        {
          using T = std::variant_alternative_t<I, std::variant<Types...>>;
          T item;
          ar & make_nvp("value", item);
          val = std::move(item);
          return true;
        }
        return false;
      };

      auto visitor = [&]<std::size_t... Is>(std::index_sequence<Is...>) {
        (load_variant_index(std::integral_constant<std::size_t, Is>{}) || ...);
      };

      visitor(std::index_sequence_for<Types...>{});
    }
    else
    {
      std::visit([&ar](auto& item) { ar & make_nvp("value", item); }, val);
    }
  }

} // namespace OrthoTree
