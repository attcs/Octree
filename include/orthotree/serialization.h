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

#include "adapters/general.h"
#include "core/bvh_static_linear_core.h"
#include "core/ot_dynamic_hash_core.h"
#include "core/ot_managed.h"
#include "core/ot_query.h"
#include "core/ot_static_linear_core.h"

// Components
#include "serialization/nvp.h"
#include "serialization/traits.h"

namespace OrthoTree
{
  // --- Non-Intrusive Serialization Functions for Geometry Types ---

  // BoundingBoxND
  template<typename TArchive, dim_t DIM, typename T>
  void serialize(TArchive& ar, BoundingBoxND<DIM, T>& box, [[maybe_unused]] const unsigned int version)
  {
    ar& ORTHOTREE_NVP_M(box, Min);
    ar& ORTHOTREE_NVP_M(box, Max);
  }

  // RayND
  template<typename TArchive, dim_t DIM, typename T>
  void serialize(TArchive& ar, RayND<DIM, T>& ray, [[maybe_unused]] const unsigned int version)
  {
    ar& ORTHOTREE_NVP_M(ray, Origin);
    ar& ORTHOTREE_NVP_M(ray, Direction);
  }

  // PlaneND
  template<typename TArchive, dim_t DIM, typename T>
  void serialize(TArchive& ar, PlaneND<DIM, T>& plane, [[maybe_unused]] const unsigned int version)
  {
    ar& ORTHOTREE_NVP_M(plane, OrigoDistance);
    ar& ORTHOTREE_NVP_M(plane, Normal);
  }


  // --- Internal Type Serialization ---
  namespace detail
  {
    template<typename TArchive, typename TVector>
    void serialize(TArchive& ar, BoundingBoxMinPointAndSize<TVector>& box, [[maybe_unused]] const unsigned int version)
    {
      ar& ORTHOTREE_NVP_M(box, minPoint);
      ar& ORTHOTREE_NVP_M(box, size);
    }


    template<typename TArchive, typename TBegin, typename TLength>
    void serialize(TArchive& ar, Segment<TBegin, TLength>& segment, [[maybe_unused]] const unsigned int version)
    {
      ar& ORTHOTREE_NVP_M(segment, begin);
      ar& ORTHOTREE_NVP_M(segment, length);
    }

    template<typename TArchive>
    void serialize(TArchive& ar, NodeStorage256& storage, [[maybe_unused]] const unsigned int version)
    {
      ar& ORTHOTREE_NVP_M(storage, nodeChildSegmentBegins);
      ar& ORTHOTREE_NVP_M(storage, nodeEntitySegment);
    }

    template<typename TArchive>
    void serialize(TArchive& ar, NodeStorage65536& storage, [[maybe_unused]] const unsigned int version)
    {
      ar& ORTHOTREE_NVP_M(storage, nodeChildSegmentBegins);
      ar& ORTHOTREE_NVP_M(storage, nodeEntitySegment);
    }

    template<typename TArchive>
    void serialize(TArchive& ar, NodeStorageGeneral& storage, [[maybe_unused]] const unsigned int version)
    {
      ar& ORTHOTREE_NVP_M(storage, nodeChildSegmentBegins);
      ar& ORTHOTREE_NVP_M(storage, nodeEntitySegment);
    }

    template<typename TArchive, typename TVector>
    void serialize(TArchive& ar, NodeGeometryData<TVector>& data, [[maybe_unused]] const unsigned int version)
    {
      ar& ORTHOTREE_NVP_M(data, minPoint);
      ar& ORTHOTREE_NVP_M(data, size);
    }

    template<typename TArchive, typename T, std::size_t N>
    auto serialize(TArchive& ar, detail::inplace_vector<T, N>& val, [[maybe_unused]] const unsigned int version)
      -> std::enable_if_t<is_stl_serialization_enabled_v<TArchive>>
    {
      std::size_t size = val.size();
      ar& make_nvp("size", size);
      if (ar.is_loading())
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
    void serialize(TArchive& ar, MortonGridSpaceIndexing<GA, ALLOW_OUT, LOOSE, MAX_DP>& si, [[maybe_unused]] const unsigned int version)
    {
      ar& ORTHOTREE_NVP_M(si, m_grid);
      ar& ORTHOTREE_NVP_M(si, m_maxDepthID);
    }

    template<typename TArchive, typename TGeometryAdapter>
    void serialize(TArchive& ar, GridSpaceIndexing<TGeometryAdapter>& grid, [[maybe_unused]] const unsigned int version)
    {
      ar& ORTHOTREE_NVP_M(grid, m_maxRasterResolution);
      ar& ORTHOTREE_NVP_M(grid, m_maxRasterID);
      ar& ORTHOTREE_NVP_M(grid, m_boxSpace);
      ar& ORTHOTREE_NVP_M(grid, m_sizeInDimensions);
      ar& ORTHOTREE_NVP_M(grid, m_volumeOfOverallSpace);
      ar& ORTHOTREE_NVP_M(grid, m_rasterization);
    }

    template<typename TArchive, typename GA>
    void serialize(TArchive& ar, typename GridSpaceIndexing<GA>::NativeRasterizationFactors& factors, [[maybe_unused]] const unsigned int version)
    {
      ar& ORTHOTREE_NVP_M(factors, sizeInDimensions);
      ar& ORTHOTREE_NVP_M(factors, rasterizerFactors);
      ar& ORTHOTREE_NVP_M(factors, derasterizerFactors);
    }

    template<typename TArchive, typename GA>
    void serialize(TArchive& ar, typename GridSpaceIndexing<GA>::ForcedDoubleRasterizationFactors& factors, [[maybe_unused]] const unsigned int version)
    {
      ar& ORTHOTREE_NVP_M(factors, sizeInDimensions);
      ar& ORTHOTREE_NVP_M(factors, rasterizerFactors);
      ar& ORTHOTREE_NVP_M(factors, derasterizerFactors);
    }

    template<typename TArchive, typename T>
    void serialize(TArchive& ar, MemoryResource<T>& res, [[maybe_unused]] const unsigned int version)
    {
      auto size = res.GetSize();
      ar& ORTHOTREE_NVP(size);

      if (ar.is_loading())
      {
        res.Init(size);
      }
    }

    template<typename TArchive, std::size_t C, typename NID, typename CID, typename EID, typename NG>
    void serialize(TArchive& ar, typename OrthoTreeNodeData<C, NID, CID, EID, NG>::EntityContainer& ec, [[maybe_unused]] const unsigned int version)
    {
      std::size_t count = ec.segment.size();
      ar& ORTHOTREE_NVP(count);

      if (ar.is_loading())
      {
        std::size_t count = 0;
        ar& make_nvp("count", count);
        ec.segment = std::span<EID>{}; // Will be fixed by parent
      }
      else
      {
        for (auto& id : ec.segment)
          ar& ORTHOTREE_NVP(id);
      }
    }

    template<typename TArchive, std::size_t C, typename NID, typename CID, typename EID, typename NG>
    void serialize(TArchive& ar, OrthoTreeNodeData<C, NID, CID, EID, NG>& node, [[maybe_unused]] const unsigned int version)
    {
      ar& ORTHOTREE_NVP_M(node, m_entities);
      ar& ORTHOTREE_NVP_M(node, m_children);
      ar& ORTHOTREE_NVP_M(node, m_childIndex);
      if constexpr (!std::is_same_v<NG, std::monostate>)
        ar& ORTHOTREE_NVP_M(node, m_geometry);
    }
  } // namespace detail


  // --- Core Serialization ---

  // OrthoTreeCoreBase
  template<typename TArchive, typename TEntityAdapter, typename TGeometryAdapter, typename TConfiguration>
  void serialize(TArchive& ar, OrthoTreeCoreBase<TEntityAdapter, TGeometryAdapter, TConfiguration>& core, [[maybe_unused]] const unsigned int version)
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
  void serialize(TArchive& ar, StaticLinearOrthoTreeCore<TEntityAdapter, TGeometryAdapter, TConfiguration>& core, [[maybe_unused]] const unsigned int version)
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
  void serialize(TArchive& ar, DynamicHashOrthoTreeCore<TEntityAdapter, TGeometryAdapter, TConfiguration>& core, [[maybe_unused]] const unsigned int version)
  {
    ar& ORTHOTREE_NVP_M(core, m_nodes);
    ar& ORTHOTREE_NVP_M(core, m_reverseMap);
    ar& ORTHOTREE_NVP_M(core, m_spaceIndexing);
    ar& ORTHOTREE_NVP_M(core, m_memoryResource);

    if constexpr (TArchive::is_loading::value)
    {
      for (auto& [id, node] : core.m_nodes)
        node.Rebridge(core.m_memoryResource);
    }
  }

  // StaticBVHLinearCore
  template<typename TArchive, typename TEntityAdapter, typename TGeometryAdapter, typename TConfiguration>
  void serialize(TArchive& ar, StaticBVHLinearCore<TEntityAdapter, TGeometryAdapter, TConfiguration>& core, [[maybe_unused]] const unsigned int version)
  {
    using TCore = StaticBVHLinearCore<TEntityAdapter, TGeometryAdapter, TConfiguration>;
    auto serializedVersionID = TCore::SERIALIZED_VERSION_ID;
    ar& ORTHOTREE_NVP(serializedVersionID);
    ar& ORTHOTREE_NVP_M(core, m_maxDepthNo);
    ar& ORTHOTREE_NVP_M(core, m_maxElementNum);
    ar& ORTHOTREE_NVP_M(core, m_entityStorage);
    ar& ORTHOTREE_NVP_M(core, m_nodeDepthIDs);
    ar& ORTHOTREE_NVP_M(core, m_nodeGeometry);
    ar& ORTHOTREE_NVP_M(core, m_nodes);
  }

  // OrthoTreeQueryBase
  template<typename TArchive, typename TCore>
  void serialize(TArchive& ar, OrthoTreeQueryBase<TCore>& query, [[maybe_unused]] const unsigned int version)
  {
    ar& make_nvp("core", static_cast<TCore&>(query));
  }

  // OrthoTreeManaged
  template<typename TArchive, typename TCore>
  void serialize(TArchive& ar, OrthoTreeManaged<TCore>& managed, [[maybe_unused]] const unsigned int version)
  {
    ar& make_nvp("tree", managed.GetCore());
    ar& make_nvp("data", managed.GetData());
  }


  // EmbeddedResourcePmrMap
  template<typename TArchive, typename PmrMap>
  void serialize(TArchive& ar, detail::EmbeddedResourcePmrMap<PmrMap>& pmrMap, [[maybe_unused]] const unsigned int version)
  {
    if constexpr (TArchive::is_loading::value)
    {
      using value_type = typename PmrMap::value_type;
      std::vector<value_type> vec;
      ar& make_nvp("data", vec);
      pmrMap.clear();
      for (auto&& item : vec)
        pmrMap.Underlying().insert(std::move(item));
    }
    else
    {
      ar& make_nvp("data", pmrMap.Underlying());
    }
  }


  // --- Core Serialization ---


} // namespace OrthoTree
