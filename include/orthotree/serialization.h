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
  } // namespace detail


  // --- Core Serialization ---

  // StaticBVHLinearCore
  template<typename TArchive, typename TCore, typename TGA, typename TCONFIG>
  void serialize(TArchive& ar, StaticBVHLinearCore<TCore, TGA, TCONFIG>& core, [[maybe_unused]] const unsigned int version)
  {
    ar& ORTHOTREE_NVP_M(core, m_maxDepthNo);
    ar& ORTHOTREE_NVP_M(core, m_maxElementNum);
    ar& ORTHOTREE_NVP_M(core, m_entityStorage);
    ar& ORTHOTREE_NVP_M(core, m_nodeDepthIDs);
    ar& ORTHOTREE_NVP_M(core, m_nodeGeometry);
    ar& ORTHOTREE_NVP_M(core, m_nodes);
  }

} // namespace OrthoTree
