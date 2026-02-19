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

#include <cstddef>
#include <iterator>

// Generates a contiguous sequence of integral values:
// [beginValue, beginValue + length)
template<typename TValue>
class SequenceView
{
public:
  using ValueType = TValue;
  using SizeType = std::size_t;

  class Iterator
  {
  public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = ValueType;
    using difference_type = std::ptrdiff_t;
    using pointer = void;
    using reference = ValueType;

    constexpr Iterator() = default;
    constexpr explicit Iterator(ValueType value)
    : m_value(value)
    {}

    constexpr ValueType operator*() const noexcept { return m_value; }

    constexpr Iterator& operator++() noexcept
    {
      ++m_value;
      return *this;
    }

    constexpr Iterator operator++(int) noexcept
    {
      Iterator tmp(*this);
      ++(*this);
      return tmp;
    }

    constexpr Iterator& operator--() noexcept
    {
      --m_value;
      return *this;
    }

    constexpr Iterator operator--(int) noexcept
    {
      Iterator tmp(*this);
      --(*this);
      return tmp;
    }

    constexpr Iterator& operator+=(difference_type n) noexcept
    {
      m_value += static_cast<ValueType>(n);
      return *this;
    }

    constexpr Iterator& operator-=(difference_type n) noexcept
    {
      m_value -= static_cast<ValueType>(n);
      return *this;
    }

    constexpr Iterator operator+(difference_type n) const noexcept { return Iterator(m_value + static_cast<ValueType>(n)); }

    constexpr Iterator operator-(difference_type n) const noexcept { return Iterator(m_value - static_cast<ValueType>(n)); }

    constexpr difference_type operator-(Iterator const& other) const noexcept { return static_cast<difference_type>(m_value - other.m_value); }

    constexpr ValueType operator[](difference_type n) const noexcept { return m_value + static_cast<ValueType>(n); }

    constexpr bool operator==(Iterator const& other) const noexcept { return m_value == other.m_value; }
    constexpr bool operator!=(Iterator const& other) const noexcept { return m_value != other.m_value; }
    constexpr bool operator<(Iterator const& other) const noexcept { return m_value < other.m_value; }
    constexpr bool operator>(Iterator const& other) const noexcept { return m_value > other.m_value; }
    constexpr bool operator<=(Iterator const& other) const noexcept { return m_value <= other.m_value; }
    constexpr bool operator>=(Iterator const& other) const noexcept { return m_value >= other.m_value; }

  private:
    ValueType m_value = {};
  };

  constexpr SequenceView(ValueType beginValue, ValueType endValue) noexcept
  : m_beginValue(beginValue)
  , m_endValue(endValue)
  {}

  constexpr Iterator begin() const noexcept { return Iterator(m_beginValue); }
  constexpr Iterator end() const noexcept { return Iterator(m_endValue); }
  constexpr SizeType size() const noexcept { return m_endValue - m_beginValue; }
  constexpr bool empty() const noexcept { return size() == 0; }

private:
  ValueType m_beginValue = {};
  ValueType m_endValue = {};
};
