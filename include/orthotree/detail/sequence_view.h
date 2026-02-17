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

  constexpr SequenceView(ValueType beginValue, SizeType length) noexcept
  : m_beginValue(beginValue)
  , m_length(length)
  {}

  constexpr Iterator begin() const noexcept { return Iterator(m_beginValue); }

  // end iterator points one past the last generated value
  constexpr Iterator end() const noexcept { return Iterator(m_beginValue + static_cast<ValueType>(m_length)); }

  constexpr SizeType size() const noexcept { return m_length; }
  constexpr bool empty() const noexcept { return m_length == 0; }

private:
  ValueType m_beginValue = {};
  SizeType m_length = 0;
};
