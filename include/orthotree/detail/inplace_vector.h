/*
MIT License

Copyright (c) 2021 Attila Csik�s

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

#include <cassert>
#include <type_traits>
#include <algorithm>
#include <memory>
#include <iterator>


#ifdef __cpp_lib_constexpr_inplace_vector
#include <inplace_vector>
#else
#include <array>
#endif

namespace OrthoTree::detail
{
#ifdef __cpp_lib_constexpr_inplace_vector
  template<typename T, std::size_t N>
  using inplace_vector = std::inplace_vector<T, N>;

#else

  template<typename T, std::size_t N>
  class inplace_vector
  {
  private:
    using container = std::array<T, N>;

  public:
    using value_type = typename container::value_type;
    using reference = T&;
    using const_reference = const T&;
    using size_type = typename std::size_t;
    using iterator = container::iterator;
    using const_iterator = container::const_iterator;

  public:
    constexpr inplace_vector() = default;

    template<typename... TVals>
    constexpr T& emplace_back(TVals&&... value) noexcept
    {
      assert(m_size < N);
      m_stack[m_size] = T(std::forward<TVals>(value)...);
      return m_stack[m_size++];
    }


    constexpr void push_back(const T& value) noexcept
    {
      assert(m_size < N);
      m_stack[m_size] = value;
      ++m_size;
    }

    constexpr void push_back(T&& value) noexcept
    {
      assert(m_size < N);
      m_stack[m_size] = std::move(value);
      ++m_size;
    }

    constexpr T& operator[](std::size_t index) noexcept { return m_stack[index]; }

    constexpr T const& operator[](std::size_t index) const noexcept { return m_stack[index]; }

    constexpr std::size_t size() const noexcept { return m_size; }

    constexpr bool empty() const noexcept { return m_size == 0; }

    constexpr iterator insert(iterator whereIt, T const& val) noexcept
      requires(std::is_trivially_copyable_v<T>)
    {
      assert(m_size < N);

      auto* dst = std::to_address(whereIt);
      auto* src = dst;
      std::size_t count = m_size - (whereIt - begin());

      std::memmove(dst + 1, src, count * sizeof(T));
      *dst = val;
      ++m_size;
      return whereIt;
    }

    constexpr iterator insert(iterator whereIt, T const& val) noexcept
      requires(!std::is_trivially_copyable_v<T>)
    {
      assert(m_size < N);
      std::move_backward(whereIt, end(), end() + 1);
      *whereIt = val;
      ++m_size;
      return whereIt;
    }

    constexpr bool erase(iterator it) noexcept
      requires(std::is_trivially_copyable_v<T>)
    {
      if (it < begin() || it >= end())
        return false;

      auto* dst = std::to_address(it);
      std::size_t count = end() - it - 1;

      std::memmove(dst, dst + 1, count * sizeof(T));
      --m_size;
      return true;
    }

    constexpr bool erase(iterator it) noexcept
      requires(!std::is_trivially_copyable_v<T>)
    {
      assert(m_size > 0);

      if (it < begin() || it >= end())
      {
        return false;
      }

      std::move(it + 1, end(), it);
      --m_size;
      return true;
    }

    constexpr void clear() noexcept { m_size = 0; }

    constexpr iterator begin() noexcept { return m_stack.begin(); }

    constexpr iterator end() noexcept { return m_stack.begin() + m_size; }

    constexpr const_iterator begin() const noexcept { return m_stack.begin(); }

    constexpr const_iterator end() const noexcept { return m_stack.begin() + m_size; }

  private:
    container m_stack;
    std::size_t m_size = 0;
  };
#endif
} // namespace OrthoTree::detail
