/*
MIT License

Copyright (c) 2021 Attila Csikós

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

#include <array>

namespace OrthoTree::detail
{
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
    constexpr T& emplace_back(TVals&&... value)
    {
      assert(m_size < N);
      m_stack[m_size] = T(std::forward<TVals>(value)...);
      return m_stack[m_size++];
    }


    constexpr void push_back(const T& value)
    {
      assert(m_size < N);
      m_stack[m_size] = value;
      ++m_size;
    }

    constexpr T& operator[](std::size_t index) { return m_stack[index]; }

    constexpr T const& operator[](std::size_t index) const { return m_stack[index]; }

    constexpr std::size_t size() const { return m_size; }

    constexpr bool empty() const noexcept { return m_size == 0; }

    constexpr iterator insert(iterator whereIt, T const& val)
    {
      assert(m_size < N);
      for (auto it = m_stack.begin() + m_size; it != whereIt; --it)
      {
        *it = std::move(*(it - 1));
      }
      *whereIt = std::move(val);
      ++m_size;
      return whereIt;
    }

    constexpr bool erase(iterator it)
    {
      assert(m_size > 0);

      if (it < begin() || it >= end())
      {
        return false;
      }

      for (auto it2 = it; it2 != end() - 1; ++it2)
      {
        *it2 = std::move(*(it2 + 1));
      }

      --m_size;
      return true;
    }

    constexpr void clear() { m_size = 0; }

    constexpr iterator begin() { return m_stack.begin(); }

    constexpr iterator end() { return m_stack.begin() + m_size; }

    constexpr const_iterator begin() const { return m_stack.begin(); }

    constexpr const_iterator end() const { return m_stack.begin() + m_size; }

  private:
    container m_stack;
    std::size_t m_size = 0;
  };
} // namespace OrthoTree::detail