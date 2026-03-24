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
#include <optional>

namespace OrthoTree
{
  // std::optional
  template<typename TArchive, typename T>
  auto serialize(TArchive& ar, std::optional<T>& val)
    -> std::enable_if_t<is_stl_serialization_enabled_v<TArchive>>
  {
    bool has_value = val.has_value();
    ar& make_nvp("has_value", has_value);

    if (ar.is_loading())
    {
      if (has_value)
      {
        T item;
        ar & make_nvp("value", item);
        val = std::move(item);
      }
      else
        val = std::nullopt;
    }
    else
    {
      if (has_value)
        ar & make_nvp("value", *val);
    }
  }

} // namespace OrthoTree
