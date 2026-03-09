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

#include <algorithm>
#include <cassert>
#include <iterator>
#include <memory>
#include <type_traits>
#include <utility>


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

    constexpr void pop_back() noexcept
    {
      assert(m_size > 0);
      --m_size;
    }

    constexpr reference operator[](std::size_t index) noexcept { return m_stack[index]; }
    constexpr const_reference operator[](std::size_t index) const noexcept { return m_stack[index]; }

    constexpr reference front() noexcept { return m_stack.front(); }
    constexpr const_reference front() const noexcept { return m_stack.front(); }

    constexpr reference back() noexcept { return m_stack[m_size - 1]; }
    constexpr const_reference back() const noexcept { return m_stack[m_size - 1]; }

    constexpr T* data() noexcept { return m_stack.data(); }
    constexpr const T* data() const noexcept { return m_stack.data(); }

    constexpr std::size_t size() const noexcept { return m_size; }
    constexpr bool empty() const noexcept { return m_size == 0; }
    constexpr std::size_t capacity() const noexcept { return N; }

    constexpr iterator insert(const_iterator whereIt, T const& val) noexcept
    {
      assert(m_size < N);
      auto non_const_it = begin() + (whereIt - begin());
      std::copy_backward(non_const_it, end(), end() + 1);
      *non_const_it = val;
      ++m_size;
      return non_const_it;
    }

    constexpr iterator insert(const_iterator whereIt, T&& val) noexcept
    {
      assert(m_size < N);
      auto non_const_it = begin() + (whereIt - begin());
      std::copy_backward(non_const_it, end(), end() + 1);
      *non_const_it = std::move(val);
      ++m_size;
      return non_const_it;
    }

    constexpr iterator erase(const_iterator it) noexcept
    {
      assert(m_size > 0);
      auto non_const_it = begin() + (it - begin());
      if (non_const_it >= begin() && non_const_it < end())
      {
        std::copy(non_const_it + 1, end(), non_const_it);
        --m_size;
        return non_const_it;
      }
      return end();
    }

    constexpr void clear() noexcept { m_size = 0; }

    constexpr iterator begin() noexcept { return m_stack.begin(); }
    constexpr iterator end() noexcept { return m_stack.begin() + m_size; }

    constexpr const_iterator begin() const noexcept { return m_stack.begin(); }
    constexpr const_iterator end() const noexcept { return m_stack.begin() + m_size; }

    constexpr const_iterator cbegin() const noexcept { return m_stack.begin(); }
    constexpr const_iterator cend() const noexcept { return m_stack.begin() + m_size; }

  private:
    container m_stack;
    std::size_t m_size = 0;
  };
#endif
} // namespace OrthoTree::detail
