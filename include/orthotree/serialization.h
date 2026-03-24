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
#ifndef ORTHOTREE_SERIALIZATION_H
#define ORTHOTREE_SERIALIZATION_H
#endif

#include "adapters/general.h"
#include "core/bvh_static_linear_core.h"
#include "core/ot_dynamic_hash_core.h"
#include "core/ot_managed.h"
#include "core/ot_query.h"
#include "core/ot_static_linear_core.h"

// Components
#include "detail/internal_geometry_module.h"
#include "serialization/nvp.h"
#include "serialization/traits.h"
#include <span>
#include <string>
#include <vector>

namespace OrthoTree
{
  // --- Non-Intrusive Serialization Functions for Geometry Types ---

  // BoundingBoxND
  template<typename TArchive, dim_t DIM, typename T>
  void serialize(TArchive& ar, BoundingBoxND<DIM, T>& box)
  {
    ar& ORTHOTREE_NVP_M(box, Min);
    ar& ORTHOTREE_NVP_M(box, Max);
  }

  // RayND
  template<typename TArchive, dim_t DIM, typename T>
  void serialize(TArchive& ar, RayND<DIM, T>& ray)
  {
    ar& ORTHOTREE_NVP_M(ray, Origin);
    ar& ORTHOTREE_NVP_M(ray, Direction);
  }

  // PlaneND
  template<typename TArchive, dim_t DIM, typename T>
  void serialize(TArchive& ar, PlaneND<DIM, T>& plane)
  {
    ar& ORTHOTREE_NVP_M(plane, OrigoDistance);
    ar& ORTHOTREE_NVP_M(plane, Normal);
  }


  // --- Internal Type Serialization ---
  namespace detail
  {
    template<typename TArchive, typename TVector>
    void serialize(TArchive& ar, BoundingBoxMinPointAndSize<TVector>& box)
    {
      ar& ORTHOTREE_NVP_M(box, minPoint);
      ar& ORTHOTREE_NVP_M(box, size);
    }


    template<typename TArchive, typename TBegin, typename TLength>
    void serialize(TArchive& ar, Segment<TBegin, TLength>& segment)
    {
      ar& ORTHOTREE_NVP_M(segment, begin);
      ar& ORTHOTREE_NVP_M(segment, length);
    }

    template<typename TArchive>
    void serialize(TArchive& ar, NodeStorage256& storage)
    {
      ar& ORTHOTREE_NVP_M(storage, nodeChildSegmentBegins);
      ar& ORTHOTREE_NVP_M(storage, nodeEntitySegment);
    }

    template<typename TArchive>
    void serialize(TArchive& ar, NodeStorage65536& storage)
    {
      ar& ORTHOTREE_NVP_M(storage, nodeChildSegmentBegins);
      ar& ORTHOTREE_NVP_M(storage, nodeEntitySegment);
    }

    template<typename TArchive>
    void serialize(TArchive& ar, NodeStorageGeneral& storage)
    {
      ar& ORTHOTREE_NVP_M(storage, nodeChildSegmentBegins);
      ar& ORTHOTREE_NVP_M(storage, nodeEntitySegment);
    }

    template<typename TArchive, typename TVector>
    void serialize(TArchive& ar, NodeGeometryData<TVector>& data)
    {
      ar& ORTHOTREE_NVP_M(data, minPoint);
      ar& ORTHOTREE_NVP_M(data, size);
    }

    template<typename TArchive, typename T, std::size_t N>
    void serialize(TArchive& ar, detail::inplace_vector<T, N>& val)
    {
      serialized_size_t size = static_cast<serialized_size_t>(val.size());
      ar& OrthoTree::make_size_tag(size);
      if (OrthoTree::is_loading_archive(ar))
      {
        val.clear();
        for (std::size_t i = 0; i < size; ++i)
        {
          val.push_back(load_construct<TArchive, T>::load(ar));
        }
      }
      else
      {
        for (auto& item : val)
          ar & item;
      }
    }


    template<typename TArchive, typename GA, bool ALLOW_OUT, double LOOSE, depth_t MAX_DP>
    void serialize(TArchive& ar, MortonGridSpaceIndexing<GA, ALLOW_OUT, LOOSE, MAX_DP>& si)
    {
      ar& ORTHOTREE_NVP_M(si, m_grid);
      ar& ORTHOTREE_NVP_M(si, m_maxDepthID);
    }

    template<typename TArchive, typename TGeometryAdapter>
    void serialize(TArchive& ar, GridSpaceIndexing<TGeometryAdapter>& grid)
    {
      ar& ORTHOTREE_NVP_M(grid, m_maxRasterResolution);
      ar& ORTHOTREE_NVP_M(grid, m_maxRasterID);
      ar& ORTHOTREE_NVP_M(grid, m_boxSpace);
      ar& ORTHOTREE_NVP_M(grid, m_sizeInDimensions);
      ar& ORTHOTREE_NVP_M(grid, m_volumeOfOverallSpace);
      ar& ORTHOTREE_NVP_M(grid, m_rasterization);
    }

    template<typename TArchive, typename GA>
    void serialize(TArchive& ar, typename GridSpaceIndexing<GA>::NativeRasterizationFactors& factors)
    {
      ar& ORTHOTREE_NVP_M(factors, sizeInDimensions);
      ar& ORTHOTREE_NVP_M(factors, rasterizerFactors);
      ar& ORTHOTREE_NVP_M(factors, derasterizerFactors);
    }

    template<typename TArchive, typename GA>
    void serialize(TArchive& ar, typename GridSpaceIndexing<GA>::ForcedDoubleRasterizationFactors& factors)
    {
      ar& ORTHOTREE_NVP_M(factors, sizeInDimensions);
      ar& ORTHOTREE_NVP_M(factors, rasterizerFactors);
      ar& ORTHOTREE_NVP_M(factors, derasterizerFactors);
    }

    template<typename TArchive, std::size_t C, typename NID, typename CID, typename EID, typename NG>
    void serialize(TArchive& ar, OrthoTreeNodeData<C, NID, CID, EID, NG>& node)
    {
      using Node = OrthoTreeNodeData<C, NID, CID, EID, NG>;

      auto serializedVersionID = Node::SERIALIZED_VERSION_ID;
      ar& ORTHOTREE_NVP(serializedVersionID);

      ar& ORTHOTREE_NVP_M(node, m_children);
      ar& ORTHOTREE_NVP_M(node, m_childIndex);
      if constexpr (!std::is_same_v<NG, std::monostate>)
        ar& ORTHOTREE_NVP_M(node, m_geometry);
    }


    template<typename T>
    class StdSpanSerializerProxy
    {
    public:
      StdSpanSerializerProxy(const std::span<T>& span, std::function<std::span<T>(std::size_t)>&& construct)
      : m_span(span)
      , m_construct(std::move(construct))
      {}

      template<typename TArchive>
      void serialize(TArchive& ar)
      {
        serialized_size_t size = static_cast<serialized_size_t>(m_span.size());
        ar& make_size_tag(size);

        if (OrthoTree::is_loading_archive(ar))
          m_span = m_construct(size);

        for (auto& item : m_span)
          ar & item;
      }

    private:
      std::span<T> m_span;
      std::function<std::span<T>(std::size_t)> m_construct;
    };

    template<typename NodeID, typename T>
    class NodeSegmentSerializerProxy
    {
    public:
      constexpr NodeSegmentSerializerProxy(NodeID& nodeID, typename MemoryResource<T>::MemorySegment& segment, MemoryResource<T>& memoryResource) noexcept
      : m_nodeID(nodeID)
      , m_segment(segment)
      , m_memoryResource(memoryResource)
      {}

      template<typename TArchive>
      void serialize(TArchive& ar)
      {
        ar& ORTHOTREE_NVP(m_nodeID);
        auto m_entityStorage = StdSpanSerializerProxy<T>(m_segment.segment, [&](std::size_t size) {
          m_segment = m_memoryResource.Allocate(size);
          return m_segment.segment;
        });
        ar& ORTHOTREE_NVP(m_entityStorage);
      }

    private:
      NodeID& m_nodeID;
      MemoryResource<T>::MemorySegment& m_segment;
      MemoryResource<T>& m_memoryResource;
    };

    template<typename TArchive, typename T, typename TNodes>
    void serializeMemoryResource(TArchive& ar, MemoryResource<T>& memoryResource, TNodes& nodes)
    {
      using NodeID = typename std::decay_t<TNodes>::key_type;
      using NodeSegment = NodeSegmentSerializerProxy<NodeID, T>;

      serialized_size_t nodeCount = 0;
      if (OrthoTree::is_loading_archive(ar))
      {
        ar& make_size_tag(nodeCount);
        for (std::size_t i = 0; i < nodeCount; ++i)
        {
          NodeID nodeID = {};
          typename MemoryResource<T>::MemorySegment segment = {};
          auto m_segment = NodeSegment(nodeID, segment, memoryResource);

          ar & m_segment;

          nodes.at(nodeID).GetEntitySegment() = std::move(segment);
        }
      }
      else
      {
        for (auto& [nodeID, node] : nodes)
          nodeCount += !node.IsEntitiesEmpty();

        ar& make_size_tag(nodeCount);

        for (auto& [nodeID, node] : nodes)
        {
          if (node.IsEntitiesEmpty())
            continue;

          auto nonConstNodeID = const_cast<NodeID&>(nodeID);
          auto m_segment = NodeSegment(nonConstNodeID, node.GetEntitySegment(), memoryResource);
          ar & m_segment;
        }
      }
    }


    template<typename TData, typename TNodeMap>
    class MemoryResourceContentSerializerProxy
    {
    public:
      MemoryResourceContentSerializerProxy(MemoryResource<TData>& memoryResource, TNodeMap& nodes)
      : m_memoryResource(memoryResource)
      , m_nodes(nodes)
      {}

      template<typename TArchive>
      void serialize(TArchive& ar)
      {
        serializeMemoryResource(ar, m_memoryResource, m_nodes);
      }

    private:
      MemoryResource<TData>& m_memoryResource;
      TNodeMap& m_nodes;
    };

    template<typename TData, typename TNodeMap>
    class MemoryResourceSerializerProxy
    {
    public:
      MemoryResourceSerializerProxy(MemoryResource<TData>& memoryResource, TNodeMap& nodes)
      : m_memoryResource(memoryResource)
      , m_nodes(nodes)
      {}

      template<typename TArchive>
      void serialize(TArchive& ar)
      {
        auto serializedVersionID = MemoryResource<TData>::SERIALIZED_VERSION_ID;
        ar& ORTHOTREE_NVP(serializedVersionID);

        auto m_memorySize = m_memoryResource.GetSize();
        ar& ORTHOTREE_NVP(m_memorySize);

        if (OrthoTree::is_loading_archive(ar))
          m_memoryResource.Init(m_memorySize);

        auto m_content = MemoryResourceContentSerializerProxy(m_memoryResource, m_nodes);
        ar& ORTHOTREE_NVP(m_content);
      }

    private:
      MemoryResource<TData>& m_memoryResource;
      TNodeMap& m_nodes;
    };

  } // namespace detail


  // --- Core Serialization ---

  // OrthoTreeCoreBase
  template<typename TArchive, typename TEntityAdapter, typename TGeometryAdapter, typename TConfiguration>
  void serialize(TArchive& ar, OrthoTreeCoreBase<TEntityAdapter, TGeometryAdapter, TConfiguration>& core)
  {
    using TCore = OrthoTreeCoreBase<TEntityAdapter, TGeometryAdapter, TConfiguration>;
    auto serializedVersionID = TCore::SERIALIZED_VERSION_ID;
    ar& ORTHOTREE_NVP(serializedVersionID);
    ar& ORTHOTREE_NVP_M(core, m_maxElementNum);
    ar& ORTHOTREE_NVP_M(core, m_maxDepthID);
    ar& ORTHOTREE_NVP_M(core, m_nominalTreeBox);
    ar& ORTHOTREE_NVP_M(core, m_realTreeBox);
    ar& ORTHOTREE_NVP_M(core, m_nodeSize);
  }

  // StaticLinearOrthoTreeCore
  template<typename TArchive, typename TEntityAdapter, typename TGeometryAdapter, typename TConfiguration>
  void serialize(TArchive& ar, StaticLinearOrthoTreeCore<TEntityAdapter, TGeometryAdapter, TConfiguration>& core)
  {
    using TCore = StaticLinearOrthoTreeCore<TEntityAdapter, TGeometryAdapter, TConfiguration>;
    auto serializedVersionID = TCore::SERIALIZED_VERSION_ID;
    ar& ORTHOTREE_NVP(serializedVersionID);

    ar& make_nvp("base", static_cast<OrthoTreeCoreBase<TEntityAdapter, TGeometryAdapter, TConfiguration>&>(core));
    ar& ORTHOTREE_NVP_M(core, m_nodes);
    ar& ORTHOTREE_NVP_M(core, m_nodeDepthIDs);
    ar& ORTHOTREE_NVP_M(core, m_entityStorage);
    ar& ORTHOTREE_NVP_M(core, m_nodeGeometry);
  }


  // DynamicHashOrthoTreeCore
  template<typename TArchive, typename TEntityAdapter, typename TGeometryAdapter, typename TConfiguration>
  void serialize(TArchive& ar, DynamicHashOrthoTreeCore<TEntityAdapter, TGeometryAdapter, TConfiguration>& core)
  {
    using TCore = DynamicHashOrthoTreeCore<TEntityAdapter, TGeometryAdapter, TConfiguration>;

    ar& make_nvp("base", static_cast<TCore::Base&>(core));

    auto serializedVersionID = TCore::SERIALIZED_VERSION_ID;
    ar& ORTHOTREE_NVP(serializedVersionID);

    ar& ORTHOTREE_NVP_M(core, m_nodes);
    ar& ORTHOTREE_NVP_M(core, m_reverseMap);
    ar& ORTHOTREE_NVP_M(core, m_spaceIndexing);

    auto m_memoryResource = detail::MemoryResourceSerializerProxy(core.m_memoryResource, core.m_nodes);
    ar& ORTHOTREE_NVP(m_memoryResource);
  }


  // StaticBVHLinearCore
  template<typename TArchive, typename TEntityAdapter, typename TGeometryAdapter, typename TConfiguration>
  void serialize(TArchive& ar, StaticBVHLinearCore<TEntityAdapter, TGeometryAdapter, TConfiguration>& core)
  {
    using TCore = StaticBVHLinearCore<TEntityAdapter, TGeometryAdapter, TConfiguration>;

    auto serializedVersionID = TCore::SERIALIZED_VERSION_ID;
    ar& ORTHOTREE_NVP(serializedVersionID);

    ar& ORTHOTREE_NVP_M(core, m_maxDepthNo);
    ar& ORTHOTREE_NVP_M(core, m_maxElementNum);
    ar& ORTHOTREE_NVP_M(core, m_entityStorage);
    ar& ORTHOTREE_NVP_M(core, m_nodeGeometry);
    ar& ORTHOTREE_NVP_M(core, m_nodes);
  }


  // OrthoTreeQueryBase
  template<typename TArchive, typename TCore>
  void serialize(TArchive& ar, OrthoTreeQueryBase<TCore>& query)
  {
    ar& make_nvp("base", static_cast<TCore&>(query));
  }

  // OrthoTreeManaged
  template<typename TArchive, typename TCore>
  void serialize(TArchive& ar, OrthoTreeManaged<TCore>& managed)
  {
    ar& ORTHOTREE_NVP_M(managed, m_tree);
    ar& ORTHOTREE_NVP_M(managed, m_entities);
  }


  // EmbeddedResourcePmrMap
  template<typename TArchive, typename PmrMap>
  void serialize(TArchive& ar, detail::EmbeddedResourcePmrMap<PmrMap>& pmrMap)
  {
    auto& m_data = pmrMap.Underlying();
    ar& ORTHOTREE_NVP(m_data);
  }


#if defined(ORTHOTREE_SERIALIZATION_BOOST_ARCHIVE_AVAILABLE)
  // --- Boost Compatibility Bridge ---
  // Boost.Serialization looks here via ADL for a 3-parameter version.
  // We provide it here but hide it from Cereal archives using SFINAE,
  // because Cereal's versioning system can cause unwanted wrappers in JSON/XML.
  template<typename TArchive, typename T>
  inline auto serialize(TArchive& ar, T& t, const unsigned int /*version*/) -> std::enable_if_t<!detail::is_cereal_archive_v<TArchive>, void>
  {
    serialize(ar, t);
  }
#endif
} // namespace OrthoTree
