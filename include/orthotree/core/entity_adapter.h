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

#include "../detail/common.h"

namespace OrthoTree
{
  template<GeometryType GEOMETRY_TYPE_, typename TEntity, typename TEntityID, typename TEntityContainer, typename TEntityContainerView, typename TGeometry, typename TEntityIDHash = std::hash<TEntityID>>
  struct EntityAdapterDefault
  {
  public:
    // OctreeContainer requires the TEntityContainer to eliminate data conversion
    using EntityContainer = TEntityContainer;

    // Generally, EntityContainerView is used in the Core type
    using EntityContainerView = TEntityContainerView;

    // Underlying data, that could be anything
    using Entity = TEntity;

    // TODO: TEntityID is possible not default-constructible, it should be handled
    // Trivially copyable type, that is stored in the nodes
    using EntityID = TEntityID;

    // Geometric representation of the Entity: Point/Box
    using Geometry = TGeometry;

    // Hash to store EntityID in hashmaps
    using Hash = TEntityIDHash;

    static constexpr GeometryType GEOMETRY_TYPE = GEOMETRY_TYPE_;

  private:
    using EntityContainerViewType = std::remove_cvref_t<EntityContainerView>;

  public:
    static constexpr bool REQUIRES_CONTIGUOUS_ENTITY_IDS = std::contiguous_iterator<typename EntityContainerViewType::iterator> &&
                                                           std::is_same_v<typename EntityContainerViewType::value_type, Geometry> &&
                                                           std::is_integral_v<EntityID>;

  public:
    static constexpr EntityID GetEntityID(EntityContainerView entities, Entity const& entity) noexcept
    {
      return detail::getKeyPart(entities, entity);
    }

    static constexpr Geometry const& GetGeometry(EntityContainerView entities, EntityID const& entityID) noexcept
    {
      return detail::at(entities, entityID);
    }

    static constexpr Geometry const& GetGeometry(Entity const& entity) noexcept { return detail::getValuePart(entity); }
    static constexpr void SetGeometry(Entity& entity, Geometry const& geometry) noexcept { return detail::setValuePart(entity, geometry); }

    static constexpr Entity const& GetEntity(EntityContainerView entities, EntityID entityID) noexcept { return detail::get(entities, entityID); }
    static constexpr Entity& GetEntity(EntityContainer& entities, EntityID entityID) noexcept { return detail::get(entities, entityID); }
  };

  template<typename TPoint>
  using PointEntitySpanAdapter = EntityAdapterDefault<GeometryType::Point, TPoint, index_t, std::vector<TPoint>, std::span<TPoint const>, TPoint>;

  template<typename TBox>
  using BoxEntitySpanAdapter = EntityAdapterDefault<GeometryType::Box, TBox, index_t, std::vector<TBox>, std::span<TBox const>, TBox>;

  template<typename TPoint, typename TEntityContainer = std::unordered_map<index_t, TPoint>, typename TEntityContainerView = TEntityContainer const&>
  using PointEntityMapAdapter =
    EntityAdapterDefault<GeometryType::Point, typename TEntityContainer::value_type, typename TEntityContainer::key_type, TEntityContainer, TEntityContainerView, TPoint>;

  template<typename TBox, typename TEntityContainer = std::unordered_map<index_t, TBox>, typename TEntityContainerView = TEntityContainer const&>
  using BoxEntityMapAdapter =
    EntityAdapterDefault<GeometryType::Box, typename TEntityContainer::value_type, typename TEntityContainer::key_type, TEntityContainer, TEntityContainerView, TBox>;
} // namespace OrthoTree
