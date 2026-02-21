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

#include <concepts>
#include <iterator>
#include <ranges>
#include <type_traits>
#include <vector>

namespace OrthoTree::detail
{
  template<typename It1, typename It2>
  class proxy_reference
  {
  private:
    using T1 = typename std::iterator_traits<It1>::value_type;
    using T2 = typename std::iterator_traits<It2>::value_type;
    using R1 = typename std::iterator_traits<It1>::reference;
    using R2 = typename std::iterator_traits<It2>::reference;
    using value_type = std::pair<T1, T2>;
    using reference = std::pair<R1, R2>;

  private:
    constexpr proxy_reference() = default;

  public:
    constexpr proxy_reference(It1 it1, It2 it2) noexcept
    : m_it1(it1)
    , m_it2(it2)
    {}

    constexpr proxy_reference(const proxy_reference&) noexcept = default;
    constexpr proxy_reference(proxy_reference&&) noexcept = default;

    constexpr proxy_reference& operator=(const proxy_reference& right) noexcept
    {
      *m_it1 = *right.m_it1;
      *m_it2 = *right.m_it2;
      return *this;
    }

    constexpr proxy_reference& operator=(proxy_reference&& right) noexcept
    {
      *m_it1 = std::move(*right.m_it1);
      *m_it2 = std::move(*right.m_it2);
      return *this;
    }

    constexpr proxy_reference& operator=(const value_type& val) noexcept
    {
      *m_it1 = val.first;
      *m_it2 = val.second;
      return *this;
    }

    constexpr proxy_reference& operator=(value_type&& val) noexcept
    {
      *m_it1 = std::move(val.first);
      *m_it2 = std::move(val.second);
      return *this;
    }

    constexpr operator value_type() const& noexcept { return { *m_it1, *m_it2 }; }
    constexpr operator value_type() && noexcept { return { std::move(*m_it1), std::move(*m_it2) }; }

    constexpr R1 const GetFirst() const noexcept { return *m_it1; }
    constexpr R1 GetFirst() noexcept { return *m_it1; }
    constexpr R2 const GetSecond() const noexcept { return *m_it2; }
    constexpr R2 GetSecond() noexcept { return *m_it2; }

    friend constexpr void swap(proxy_reference left, proxy_reference right) noexcept
    {
      auto val1 = std::move(*left.m_it1);
      *left.m_it1 = std::move(*right.m_it1);
      *right.m_it1 = std::move(val1);

      auto val2 = std::move(*left.m_it2);
      *left.m_it2 = std::move(*right.m_it2);
      *right.m_it2 = std::move(val2);
    }

  private:
    It1 m_it1;
    It2 m_it2;
  };

  template<typename It1, typename It2>
  class zip_iterator
  {
  public:
    using iterator_concept = std::random_access_iterator_tag;
    using iterator_category = std::random_access_iterator_tag;

    // TODO: remove: using difference_type = typename std::iterator_traits<It1>::difference_type;
    using difference_type = std::common_type_t<typename std::iterator_traits<It1>::difference_type, typename std::iterator_traits<It2>::difference_type>;
    using pointer = void;

    using reference = proxy_reference<It1, It2>;
    using value_type = std::pair<typename std::iterator_traits<It1>::value_type, typename std::iterator_traits<It2>::value_type>;
    // using reference = std::tuple<std::iter_reference_t<It1>, std::iter_reference_t<It2>>;
    // using value_type = std::tuple<std::iter_value_t<It1>, std::iter_value_t<It2>>;

    constexpr zip_iterator() noexcept = default;
    constexpr zip_iterator(It1 it1, It2 it2) noexcept
    : it1_(it1)
    , it2_(it2)
    {}

    // constexpr reference operator*() noexcept { return reference(it1_, it2_); }
    constexpr auto operator*() const noexcept { return proxy_reference<It1, It2>{ it1_, it2_ }; }
    // constexpr reference operator*() const noexcept { return reference(it1_, it2_); }
    constexpr It1 GetFirst() const noexcept { return it1_; }
    constexpr It2 GetSecond() const noexcept { return it2_; }

    constexpr zip_iterator& operator++() noexcept
    {
      ++it1_;
      ++it2_;
      return *this;
    }

    constexpr zip_iterator operator++(int) noexcept
    {
      auto tmp = *this;
      ++(*this);
      return tmp;
    }

    constexpr zip_iterator& operator--() noexcept
    {
      --it1_;
      --it2_;
      return *this;
    }
    constexpr zip_iterator operator--(int) noexcept
    {
      auto tmp = *this;
      --(*this);
      return tmp;
    }

    constexpr zip_iterator& operator+=(difference_type n) noexcept
    {
      it1_ += n;
      it2_ += n;
      return *this;
    }
    constexpr zip_iterator& operator-=(difference_type n) noexcept
    {
      it1_ -= n;
      it2_ -= n;
      return *this;
    }

    constexpr zip_iterator operator+(difference_type n) const noexcept { return zip_iterator(it1_ + n, it2_ + n); }
    constexpr zip_iterator operator-(difference_type n) const noexcept { return zip_iterator(it1_ - n, it2_ - n); }
    constexpr difference_type operator-(const zip_iterator& other) const noexcept { return it1_ - other.it1_; }
    friend constexpr zip_iterator operator+(difference_type n, zip_iterator it) noexcept { return it + n; }

    constexpr reference operator[](difference_type n) noexcept { return *(*this + n); }
    constexpr reference operator[](difference_type n) const noexcept { return *(*this + n); }

    constexpr bool operator==(const zip_iterator& other) const noexcept { return it1_ == other.it1_; }
    constexpr bool operator!=(const zip_iterator& other) const noexcept { return !(*this == other); }
    constexpr bool operator<(const zip_iterator& other) const noexcept { return it1_ < other.it1_; }
    constexpr bool operator>(const zip_iterator& other) const noexcept { return it1_ > other.it1_; }
    constexpr bool operator<=(const zip_iterator& other) const noexcept { return it1_ <= other.it1_; }
    constexpr bool operator>=(const zip_iterator& other) const noexcept { return it1_ >= other.it1_; }

  private:
    It1 it1_;
    It2 it2_;
  };


  template<typename T1, typename T2>
  class zip_view : public std::ranges::view_interface<zip_view<T1, T2>>
  {
  public:
    using It1 = typename T1::iterator;
    using It2 = typename T2::iterator;

    using iterator = zip_iterator<It1, It2>;
    using const_iterator = iterator;

    constexpr zip_view(T1& data1, T2& data2) noexcept
      requires(std::ranges::contiguous_range<T1> && std::ranges::contiguous_range<T2>)
    : m_data1(data1)
    , m_data2(data2)
    {
      assert(m_data1.size() == m_data2.size());
      assert(m_data1.data() == m_data2.data());

      // overlap is not allowed
      assert(m_data1.data() + m_data1.size() <= m_data2.data() || m_data2.data() + m_data2.size() <= m_data1.data());
    }

    zip_view(const zip_view& other) noexcept
    : m_data1(other.m_data1)
    , m_data2(other.m_data2)
    {
      assert(m_data1.size() == m_data2.size());
    }

    zip_view(zip_view&& other) noexcept
    : m_data1(other.m_data1)
    , m_data2(other.m_data2)
    {
      assert(m_data1.size() == m_data2.size());
    }

    zip_view& operator=(const zip_view& other) noexcept
    {
      if (this != &other)
      {
        m_data1 = other.m_data1;
        m_data2 = other.m_data2;
        assert(m_data1.size() == m_data2.size());
      }
      return *this;
    }

    zip_view& operator=(zip_view&& other) noexcept
    {
      if (this != &other)
      {
        m_data1 = other.m_data1;
        m_data2 = other.m_data2;
        assert(m_data1.size() == m_data2.size());
      }
      return *this;
    }

    constexpr iterator begin() noexcept { return iterator(m_data1.begin(), m_data2.begin()); }
    constexpr iterator begin() const noexcept { return const_iterator(m_data1.begin(), m_data2.begin()); }

    constexpr iterator end() noexcept { return iterator(m_data1.end(), m_data2.end()); }
    constexpr iterator end() const noexcept { return const_iterator(m_data1.end(), m_data2.end()); }

    constexpr std::size_t size() const noexcept { return m_data1.size(); }

  private:
    T1& m_data1;
    T2& m_data2;
  };

  static_assert(std::ranges::random_access_range<zip_view<std::vector<int>, std::vector<int>>>);
  static_assert(std::ranges::sized_range<zip_view<std::vector<int>, std::vector<int>>>);

} // namespace OrthoTree::detail
