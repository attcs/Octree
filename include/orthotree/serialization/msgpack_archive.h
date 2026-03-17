/*
MIT License
Copyright (c) 2026 Attila Csikós
*/

#pragma once

#include "nvp.h"
#include "traits.h"
#include <iostream>
#include <string>
#include <vector>

#if __has_include(<msgpack.hpp>)
#include <msgpack.hpp>
#define ORTHOTREE_SERIALIZATION_MSGPACK_ENABLED

namespace OrthoTree
{
  class MsgPackArchive;
  
  template<>
  struct is_stl_serialization_enabled<MsgPackArchive> : std::true_type {};

  // A simple MsgPack Output Archive that works with our serialize functions
  class MsgPackArchive
  {
  protected:
    msgpack::sbuffer m_buffer;
    msgpack::packer<msgpack::sbuffer> m_packer;
    std::ostream& m_os;

  public:
    MsgPackArchive(std::ostream& os) 
    : m_packer(m_buffer)
    , m_os(os) 
    {}

    ~MsgPackArchive()
    {
      m_os.write(m_buffer.data(), m_buffer.size());
    }

    constexpr bool is_loading() const { return false; }
    constexpr bool is_saving() const { return true; }

    // SizeTag: MsgPack uses array headers
    template<typename T>
    MsgPackArchive& operator&(SizeTag<T> tag)
    {
      m_packer.pack_array(static_cast<uint32_t>(tag.value));
      return *this;
    }

    // NameValuePair: We can pack as a map or just the value. 
    // To stay consistent with JSON/XML, we'll use maps if it's a named object.
    template<typename TNVP>
    requires(requires(TNVP nvp) { detail::get_nvp_name(nvp); detail::get_nvp_value(nvp); })
    MsgPackArchive& operator&(TNVP&& nvp)
    {
      // For simplicity in this bridge, we pack the value directly.
      // If full Map support is needed, we'd need to track scope.
      serialize_value(detail::get_nvp_value(nvp));
      return *this;
    }

    template<typename T>
    auto operator&(T& val) -> std::enable_if_t<!is_size_tag_v<std::remove_cv_t<T>>, MsgPackArchive&>
    {
      serialize_value(val);
      return *this;
    }

    template<typename... Args>
    MsgPackArchive& operator()(Args&&... args)
    {
      ((*this & std::forward<Args>(args)), ...);
      return *this;
    }

  protected:
    template<typename T>
    void serialize_value(T& val)
    {
      if constexpr (std::is_arithmetic_v<T>)
      {
        m_packer.pack(val);
      }
      else if constexpr (std::is_same_v<T, std::string>)
      {
        m_packer.pack(val);
      }
      else if constexpr (std::is_same_v<T, std::monostate>)
      {
        m_packer.pack_nil();
      }
      else if constexpr (std::is_same_v<T, std::nullptr_t>)
      {
        m_packer.pack_nil();
      }
      else if constexpr (requires { val.serialize(*this); })
      {
        val.serialize(*this);
      }
      else if constexpr (is_stl_serialization_enabled_v<MsgPackArchive>)
      {
        serialize(*this, val);
      }
      else
      {
        // Fallback or static_assert
      }
    }
  };
}
#endif
