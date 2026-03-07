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

#include "../core/types.h"

#include "../core/configuration.h"
#include "../core/entity_adapter.h"

#include "../core/ot_dynamic_hash_core.h"
#include "../core/ot_managed.h"
#include "../core/ot_query.h"
#include "../core/ot_static_linear_core.h"


#include "../adapters/general.h"

#include <type_traits>


namespace OrthoTree
{
  template<typename TEntityAdapter, typename TGeometryAdapter, typename TConfiguration>
  using DynamicOrthoTreeBase = OrthoTreeQueryBase<DynamicHashOrthoTreeCore<TEntityAdapter, TGeometryAdapter, TConfiguration>>;

  template<typename TEntityAdapter, typename TGeometryAdapter, typename TConfiguration>
  using StaticOrthoTreeBase = OrthoTreeQueryBase<StaticLinearOrthoTreeCore<TEntityAdapter, TGeometryAdapter, TConfiguration>>;

  template<typename TEntityAdapter, typename TGeometryAdapter, typename TConfiguration>
  using OrthoTreeBase = DynamicOrthoTreeBase<TEntityAdapter, TGeometryAdapter, TConfiguration>;

  // Tree aliases

  template<dim_t DIMENSION_NO, typename TScalar = BaseGeometryType, bool IS_CONTIOGUOS_CONTAINER = true, NodeGeometryStorage NODE_GEOMETRY_STORAGE = NodeGeometryStorage::MinPoint>
  using OrthoTreePointND = OrthoTree::OrthoTreeBase<
    std::conditional_t<IS_CONTIOGUOS_CONTAINER, PointEntitySpanAdapter<PointND<DIMENSION_NO, TScalar>>, PointEntityMapAdapter<PointND<DIMENSION_NO, TScalar>>>,
    GeneralGeometryAdapterND<DIMENSION_NO, TScalar>,
    PointConfiguration<NODE_GEOMETRY_STORAGE>>;

  template<dim_t DIMENSION_NO, bool IS_LOOSE_TREE = true, typename TScalar = BaseGeometryType, bool IS_CONTIOGUOS_CONTAINER = true, NodeGeometryStorage NODE_GEOMETRY_STORAGE = NodeGeometryStorage::MBR>
  using OrthoTreeBoxND = OrthoTree::OrthoTreeBase<
    std::conditional_t<IS_CONTIOGUOS_CONTAINER, BoxEntitySpanAdapter<BoundingBoxND<DIMENSION_NO, TScalar>>, BoxEntityMapAdapter<BoundingBoxND<DIMENSION_NO, TScalar>>>,
    GeneralGeometryAdapterND<DIMENSION_NO, TScalar>,
    BoxConfiguration<IS_LOOSE_TREE, NODE_GEOMETRY_STORAGE>>;

  template<dim_t DIMENSION_NO, typename TScalar, typename TEntityContainer, NodeGeometryStorage NODE_GEOMETRY_STORAGE = NodeGeometryStorage::MinPoint>
  using OrthoTreePointNDUD = OrthoTree::OrthoTreeBase<
    PointEntityMapAdapter<PointND<DIMENSION_NO, TScalar>, TEntityContainer>,
    GeneralGeometryAdapterND<DIMENSION_NO, TScalar>,
    PointConfiguration<NODE_GEOMETRY_STORAGE>>;

  template<dim_t DIMENSION_NO, bool IS_LOOSE_TREE, typename TScalar, typename TEntityContainer, NodeGeometryStorage NODE_GEOMETRY_STORAGE = NodeGeometryStorage::MBR>
  using OrthoTreeBoxNDUD = OrthoTree::OrthoTreeBase<
    BoxEntityMapAdapter<BoundingBoxND<DIMENSION_NO, TScalar>, TEntityContainer>,
    GeneralGeometryAdapterND<DIMENSION_NO, TScalar>,
    BoxConfiguration<IS_LOOSE_TREE, NODE_GEOMETRY_STORAGE>>;

  template<dim_t DIMENSION_NO, typename TScalar = BaseGeometryType, bool IS_CONTIOGUOS_CONTAINER = true, NodeGeometryStorage NODE_GEOMETRY_STORAGE = NodeGeometryStorage::MinPoint>
  using StaticOrthoTreePointND = OrthoTree::StaticOrthoTreeBase<
    std::conditional_t<IS_CONTIOGUOS_CONTAINER, PointEntitySpanAdapter<PointND<DIMENSION_NO, TScalar>>, PointEntityMapAdapter<PointND<DIMENSION_NO, TScalar>>>,
    GeneralGeometryAdapterND<DIMENSION_NO, TScalar>,
    PointConfiguration<NODE_GEOMETRY_STORAGE>>;

  template<dim_t DIMENSION_NO, bool IS_LOOSE_TREE = true, typename TScalar = BaseGeometryType, bool IS_CONTIOGUOS_CONTAINER = true, NodeGeometryStorage NODE_GEOMETRY_STORAGE = NodeGeometryStorage::MBR>
  using StaticOrthoTreeBoxND = OrthoTree::StaticOrthoTreeBase<
    std::conditional_t<IS_CONTIOGUOS_CONTAINER, BoxEntitySpanAdapter<BoundingBoxND<DIMENSION_NO, TScalar>>, BoxEntityMapAdapter<BoundingBoxND<DIMENSION_NO, TScalar>>>,
    GeneralGeometryAdapterND<DIMENSION_NO, TScalar>,
    BoxConfiguration<IS_LOOSE_TREE, NODE_GEOMETRY_STORAGE>>;

  template<dim_t DIMENSION_NO, typename TScalar, typename TEntityContainer, NodeGeometryStorage NODE_GEOMETRY_STORAGE = NodeGeometryStorage::MinPoint>
  using StaticOrthoTreePointNDUD = OrthoTree::StaticOrthoTreeBase<
    PointEntityMapAdapter<PointND<DIMENSION_NO, TScalar>, TEntityContainer>,
    GeneralGeometryAdapterND<DIMENSION_NO, TScalar>,
    PointConfiguration<NODE_GEOMETRY_STORAGE>>;

  template<dim_t DIMENSION_NO, bool IS_LOOSE_TREE, typename TScalar, typename TEntityContainer, NodeGeometryStorage NODE_GEOMETRY_STORAGE = NodeGeometryStorage::MBR>
  using StaticOrthoTreeBoxNDUD = OrthoTree::StaticOrthoTreeBase<
    BoxEntityMapAdapter<BoundingBoxND<DIMENSION_NO, TScalar>, TEntityContainer>,
    GeneralGeometryAdapterND<DIMENSION_NO, TScalar>,
    BoxConfiguration<IS_LOOSE_TREE, NODE_GEOMETRY_STORAGE>>;


  // Dualtree for points
  using DualtreePoint = OrthoTreePointND<1, BaseGeometryType>;

  // Dualtree for bounding boxes
  using DualtreeBox = OrthoTreeBoxND<1, true, BaseGeometryType>;

  // Quadtree for points
  using QuadtreePoint = OrthoTreePointND<2, BaseGeometryType>;

  // Quadtree for bounding boxes
  using QuadtreeBox = OrthoTreeBoxND<2, true, BaseGeometryType>;

  // Octree for points
  using OctreePoint = OrthoTreePointND<3, BaseGeometryType>;

  // Octree for bounding boxes
  using OctreeBox = OrthoTreeBoxND<3, true, BaseGeometryType>;

  // Hexatree for points
  using HexatreePoint = OrthoTreePointND<4, BaseGeometryType>;

  // Hexatree for bounding boxes
  using HexatreeBox = OrthoTreeBoxND<4, true, BaseGeometryType>;

  // OrthoTrees for higher dimensions
  using TreePoint16D = OrthoTreePointND<16, BaseGeometryType>;
  using TreeBox16D = OrthoTreeBoxND<16, true, BaseGeometryType>;


  // Dualtree for bounding boxes with split-depth settings
  template<bool IS_LOOSE_TREE>
  using DualtreeBoxs = OrthoTreeBoxND<1, IS_LOOSE_TREE, BaseGeometryType>;

  // Quadtree for bounding boxes with split-depth settings
  template<bool IS_LOOSE_TREE>
  using QuadtreeBoxs = OrthoTreeBoxND<2, IS_LOOSE_TREE, BaseGeometryType>;

  // Octree for bounding boxes with split-depth settings
  template<bool IS_LOOSE_TREE>
  using OctreeBoxs = OrthoTreeBoxND<3, IS_LOOSE_TREE, BaseGeometryType>;

  // Hexatree for bounding boxes with split-depth settings
  template<bool IS_LOOSE_TREE>
  using HexatreeBoxs = OrthoTreeBoxND<4, IS_LOOSE_TREE, BaseGeometryType>;

  // OrthoTrees for higher dimensions with split-depth settings
  template<bool IS_LOOSE_TREE>
  using TreeBox16Ds = OrthoTreeBoxND<16, IS_LOOSE_TREE, BaseGeometryType>;


  // OrthoTrees with std::unordered_map

  // std::unordered_map-based Quadtree for points
  using QuadtreePointMap = OrthoTreePointND<2, BaseGeometryType, false>;

  // std::unordered_map-based Octree for points
  using OctreePointMap = OrthoTreePointND<3, BaseGeometryType, false>;

  // std::unordered_map-based Octree for bounding boxes
  using QuadreeBoxMap = OrthoTreeBoxND<2, true, BaseGeometryType, false>;

  // std::unordered_map-based Octree for bounding boxes
  using OctreeBoxMap = OrthoTreeBoxND<3, true, BaseGeometryType, false>;

  // std::unordered_map-based Quadtree for bounding boxes with split-depth settings
  template<bool IS_LOOSE_TREE>
  using QuadtreeBoxsMap = OrthoTreeBoxND<2, IS_LOOSE_TREE, BaseGeometryType, false>;
  using QuadtreeBoxMap = OrthoTreeBoxND<2, true, BaseGeometryType, false>;

  // std::unordered_map-based Octree for bounding boxes with split-depth settings
  template<bool IS_LOOSE_TREE>
  using OctreeBoxsMap = OrthoTreeBoxND<3, IS_LOOSE_TREE, BaseGeometryType, false>;
  using OctreeBoxMap = OrthoTreeBoxND<3, true, BaseGeometryType, false>;


  template<
    dim_t DIMENSION_NO,
    typename TScalar = BaseGeometryType,
    typename TEntityAdapter = PointEntitySpanAdapter<PointND<DIMENSION_NO, TScalar>>,
    NodeGeometryStorage NODE_GEOMETRY_STORAGE = NodeGeometryStorage::MinPoint>
  using TreePointEAND =
    OrthoTree::OrthoTreeBase<TEntityAdapter, GeneralGeometryAdapterND<DIMENSION_NO, TScalar>, PointConfiguration<NODE_GEOMETRY_STORAGE>>;

  template<
    dim_t DIMENSION_NO,
    bool IS_LOOSE_TREE = true,
    typename TScalar = BaseGeometryType,
    typename TEntityAdapter = BoxEntitySpanAdapter<BoundingBoxND<DIMENSION_NO, TScalar>>,
    NodeGeometryStorage NODE_GEOMETRY_STORAGE = NodeGeometryStorage::MBR>
  using TreeBoxEAND =
    OrthoTree::OrthoTreeBase<TEntityAdapter, GeneralGeometryAdapterND<DIMENSION_NO, TScalar>, BoxConfiguration<IS_LOOSE_TREE, NODE_GEOMETRY_STORAGE>>;


  // User-defined container-based Quadtree for points
  template<typename EntityAdapter>
  using QuadtreePointUDMap = TreePointEAND<2, BaseGeometryType, EntityAdapter>;

  // User-defined container-based Octree for points
  template<typename EntityAdapter>
  using OctreePointUDMap = TreePointEAND<3, BaseGeometryType, EntityAdapter>;

  // User-defined container-based Octree for bounding boxes
  template<typename EntityAdapter>
  using QuadreeBoxUDMap = TreeBoxEAND<2, true, BaseGeometryType, EntityAdapter>;

  // User-defined container-based Octree for bounding boxes
  template<typename EntityAdapter>
  using OctreeBoxUDMap = TreeBoxEAND<3, true, BaseGeometryType, EntityAdapter>;

  // User-defined container-based Quadtree for bounding boxes with split-depth settings
  template<bool IS_LOOSE_TREE, typename EntityAdapter>
  using QuadtreeBoxsUDMap = TreeBoxEAND<2, IS_LOOSE_TREE, BaseGeometryType, EntityAdapter>;

  // User-defined container-based Octree for bounding boxes with split-depth settings
  template<bool IS_LOOSE_TREE, typename EntityAdapter>
  using OctreeBoxsUDMap = TreeBoxEAND<3, IS_LOOSE_TREE, BaseGeometryType, EntityAdapter>;


  // Dualtree for points
  using StaticDualtreePoint = StaticOrthoTreePointND<1, BaseGeometryType>;

  // Dualtree for bounding boxes
  using StaticDualtreeBox = StaticOrthoTreeBoxND<1, true, BaseGeometryType>;

  // Quadtree for points
  using StaticQuadtreePoint = StaticOrthoTreePointND<2, BaseGeometryType>;

  // Quadtree for bounding boxes
  using StaticQuadtreeBox = StaticOrthoTreeBoxND<2, true, BaseGeometryType>;

  // Octree for points
  using StaticOctreePoint = StaticOrthoTreePointND<3, BaseGeometryType>;

  // Octree for bounding boxes
  using StaticOctreeBox = StaticOrthoTreeBoxND<3, true, BaseGeometryType>;

  // Hexatree for points
  using StaticHexatreePoint = StaticOrthoTreePointND<4, BaseGeometryType>;

  // Hexatree for bounding boxes
  using StaticHexatreeBox = StaticOrthoTreeBoxND<4, true, BaseGeometryType>;


  // Dualtree for bounding boxes with split-depth settings
  template<bool IS_LOOSE_TREE>
  using StaticDualtreeBoxs = StaticOrthoTreeBoxND<1, IS_LOOSE_TREE, BaseGeometryType>;

  // Quadtree for bounding boxes with split-depth settings
  template<bool IS_LOOSE_TREE>
  using StaticQuadtreeBoxs = StaticOrthoTreeBoxND<2, IS_LOOSE_TREE, BaseGeometryType>;

  // Octree for bounding boxes with split-depth settings
  template<bool IS_LOOSE_TREE>
  using StaticOctreeBoxs = StaticOrthoTreeBoxND<3, IS_LOOSE_TREE, BaseGeometryType>;

  // Hexatree for bounding boxes with split-depth settings
  template<bool IS_LOOSE_TREE>
  using StaticHexatreeBoxs = StaticOrthoTreeBoxND<4, IS_LOOSE_TREE, BaseGeometryType>;


  // OrthoTrees with std::unordered_map

  // std::unordered_map-based Quadtree for points
  using StaticQuadtreePointMap = StaticOrthoTreePointND<2, BaseGeometryType, false>;

  // std::unordered_map-based Octree for points
  using StaticOctreePointMap = StaticOrthoTreePointND<3, BaseGeometryType, false>;

  // std::unordered_map-based Octree for bounding boxes
  using StaticQuadreeBoxMap = StaticOrthoTreeBoxND<2, true, BaseGeometryType, false>;

  // std::unordered_map-based Octree for bounding boxes
  using StaticOctreeBoxMap = StaticOrthoTreeBoxND<3, true, BaseGeometryType, false>;

  // std::unordered_map-based Quadtree for bounding boxes with split-depth settings
  template<bool IS_LOOSE_TREE>
  using StaticQuadtreeBoxsMap = StaticOrthoTreeBoxND<2, IS_LOOSE_TREE, BaseGeometryType, false>;
  using StaticQuadtreeBoxMap = StaticOrthoTreeBoxND<2, true, BaseGeometryType, false>;

  // std::unordered_map-based Octree for bounding boxes with split-depth settings
  template<bool IS_LOOSE_TREE>
  using StaticOctreeBoxsMap = StaticOrthoTreeBoxND<3, IS_LOOSE_TREE, BaseGeometryType, false>;
  using StaticOctreeBoxMap = StaticOrthoTreeBoxND<3, true, BaseGeometryType, false>;


  template<
    dim_t DIMENSION_NO,
    typename TScalar = BaseGeometryType,
    typename TEntityAdapter = PointEntitySpanAdapter<PointND<DIMENSION_NO, TScalar>>,
    NodeGeometryStorage NODE_GEOMETRY_STORAGE = NodeGeometryStorage::MinPoint>
  using StaticTreePointEAND =
    OrthoTree::StaticOrthoTreeBase<TEntityAdapter, GeneralGeometryAdapterND<DIMENSION_NO, TScalar>, PointConfiguration<NODE_GEOMETRY_STORAGE>>;

  template<
    dim_t DIMENSION_NO,
    bool IS_LOOSE_TREE = true,
    typename TScalar = BaseGeometryType,
    typename TEntityAdapter = BoxEntitySpanAdapter<BoundingBoxND<DIMENSION_NO, TScalar>>,
    NodeGeometryStorage NODE_GEOMETRY_STORAGE = NodeGeometryStorage::MBR>
  using StaticTreeBoxEAND =
    OrthoTree::StaticOrthoTreeBase<TEntityAdapter, GeneralGeometryAdapterND<DIMENSION_NO, TScalar>, BoxConfiguration<IS_LOOSE_TREE, NODE_GEOMETRY_STORAGE>>;


  // User-defined container-based Quadtree for points
  template<typename EntityAdapter>
  using StaticQuadtreePointUDMap = StaticTreePointEAND<2, BaseGeometryType, EntityAdapter>;

  // User-defined container-based Octree for points
  template<typename EntityAdapter>
  using StaticOctreePointUDMap = StaticTreePointEAND<3, BaseGeometryType, EntityAdapter>;

  // User-defined container-based Octree for bounding boxes
  template<typename EntityAdapter>
  using StaticQuadreeBoxUDMap = StaticTreeBoxEAND<2, true, BaseGeometryType, EntityAdapter>;

  // User-defined container-based Octree for bounding boxes
  template<typename EntityAdapter>
  using StaticOctreeBoxUDMap = StaticTreeBoxEAND<3, true, BaseGeometryType, EntityAdapter>;

  // User-defined container-based Quadtree for bounding boxes with split-depth settings
  template<bool IS_LOOSE_TREE, typename EntityAdapter>
  using StaticQuadtreeBoxsUDMap = StaticTreeBoxEAND<2, IS_LOOSE_TREE, BaseGeometryType, EntityAdapter>;

  // User-defined container-based Octree for bounding boxes with split-depth settings
  template<bool IS_LOOSE_TREE, typename EntityAdapter>
  using StaticOctreeBoxsUDMap = StaticTreeBoxEAND<3, IS_LOOSE_TREE, BaseGeometryType, EntityAdapter>;

  // Managed types

  template<dim_t DIMENSION_NO, typename TScalar = BaseGeometryType, bool IS_CONTIOGUOS_CONTAINER = true>
  using TreePointManagedND = OrthoTreeManaged<OrthoTreePointND<DIMENSION_NO, TScalar, IS_CONTIOGUOS_CONTAINER>>;

  template<dim_t DIMENSION_NO, bool IS_LOOSE_TREE = true, typename TScalar = BaseGeometryType, bool IS_CONTIOGUOS_CONTAINER = true>
  using TreeBoxManagedND = OrthoTreeManaged<OrthoTreeBoxND<DIMENSION_NO, IS_LOOSE_TREE, TScalar, IS_CONTIOGUOS_CONTAINER>>;

  template<dim_t DIMENSION_NO, typename TScalar, typename TEntityContainer>
  using TreePointContainerNDUD = OrthoTreeManaged<OrthoTreePointNDUD<DIMENSION_NO, TScalar, TEntityContainer>>;

  template<dim_t DIMENSION_NO, bool IS_LOOSE_TREE, typename TScalar, typename TEntityContainer>
  using TreeBoxContainerNDUD = OrthoTreeManaged<OrthoTreeBoxNDUD<DIMENSION_NO, IS_LOOSE_TREE, TScalar, TEntityContainer>>;

  // Dualtree for points
  using DualtreePointM = TreePointManagedND<1, BaseGeometryType>;

  // Dualtree for bounding boxes
  template<bool IS_LOOSE_TREE = true>
  using DualtreeBoxCs = TreeBoxManagedND<1, IS_LOOSE_TREE, BaseGeometryType>;
  using DualtreeBoxM = TreeBoxManagedND<1, true, BaseGeometryType>;

  // Quadtree for points
  using QuadtreePointM = TreePointManagedND<2, BaseGeometryType>;

  // Quadtree for bounding boxes
  template<bool IS_LOOSE_TREE = true>
  using QuadtreeBoxCs = TreeBoxManagedND<2, IS_LOOSE_TREE, BaseGeometryType>;
  using QuadtreeBoxM = TreeBoxManagedND<2, true, BaseGeometryType>;

  // Octree for points
  using OctreePointM = TreePointManagedND<3, BaseGeometryType>;

  // Octree for bounding boxes
  template<bool IS_LOOSE_TREE = true>
  using OctreeBoxCs = TreeBoxManagedND<3, IS_LOOSE_TREE, BaseGeometryType>;
  using OctreeBoxM = TreeBoxManagedND<3, true, BaseGeometryType>;


  // std::unordered_map-based Dualtree for points
  using DualtreePointMapM = TreePointManagedND<1, BaseGeometryType, false>;

  // std::unordered_map-based Dualtree for bounding boxes
  template<bool IS_LOOSE_TREE = true>
  using DualtreeBoxMapCs = TreeBoxManagedND<1, IS_LOOSE_TREE, BaseGeometryType, false>;
  using DualtreeBoxMapM = TreeBoxManagedND<1, true, BaseGeometryType, false>;

  // std::unordered_map-based Quadtree for points
  using QuadtreePointMapM = TreePointManagedND<2, BaseGeometryType, false>;

  // std::unordered_map-based Quadtree for bounding boxes
  template<bool IS_LOOSE_TREE = true>
  using QuadtreeBoxMapCs = TreeBoxManagedND<2, IS_LOOSE_TREE, BaseGeometryType, false>;
  using QuadtreeBoxMapM = TreeBoxManagedND<2, true, BaseGeometryType, false>;

  // std::unordered_map-based Octree for points
  using OctreePointMapM = TreePointManagedND<3, BaseGeometryType, false>;

  // std::unordered_map-based Octree for bounding boxes
  template<bool IS_LOOSE_TREE = true>
  using OctreeBoxMapCs = TreeBoxManagedND<3, IS_LOOSE_TREE, BaseGeometryType, false>;
  using OctreeBoxMapM = TreeBoxManagedND<3, true, BaseGeometryType, false>;

  // User-defined container-based Quadtree for points
  template<typename EntityContainer>
  using QuadtreePointUDMapC = TreePointContainerNDUD<2, BaseGeometryType, EntityContainer>;

  // User-defined container-based Quadtree for bounding boxes
  template<typename EntityContainer, bool IS_LOOSE_TREE = true>
  using QuadtreeBoxUDMapCs = TreeBoxContainerNDUD<2, IS_LOOSE_TREE, BaseGeometryType, EntityContainer>;
  template<typename EntityContainer>
  using QuadtreeBoxUDMapC = TreeBoxContainerNDUD<2, true, BaseGeometryType, EntityContainer>;

  // User-defined container-based Octree for points
  template<typename EntityContainer>
  using OctreePointUDMapC = TreePointContainerNDUD<3, BaseGeometryType, EntityContainer>;

  // User-defined container-based Octree for bounding boxes
  template<typename EntityContainer, bool IS_LOOSE_TREE = true>
  using OctreeBoxUDMapCs = TreeBoxContainerNDUD<3, IS_LOOSE_TREE, BaseGeometryType, EntityContainer>;
  template<typename EntityContainer>
  using OctreeBoxUDMapC = TreeBoxContainerNDUD<3, true, BaseGeometryType, EntityContainer>;


  // Static Managed types

  template<dim_t DIMENSION_NO, typename TScalar = BaseGeometryType, bool IS_CONTIOGUOS_CONTAINER = true>
  using StaticTreePointManagedND = OrthoTreeManaged<StaticOrthoTreePointND<DIMENSION_NO, TScalar, IS_CONTIOGUOS_CONTAINER>>;

  template<dim_t DIMENSION_NO, bool IS_LOOSE_TREE = true, typename TScalar = BaseGeometryType, bool IS_CONTIOGUOS_CONTAINER = true>
  using StaticTreeBoxManagedND = OrthoTreeManaged<StaticOrthoTreeBoxND<DIMENSION_NO, IS_LOOSE_TREE, TScalar, IS_CONTIOGUOS_CONTAINER>>;


  // Static Managed Dualtree for points
  using StaticDualtreePointM = StaticTreePointManagedND<1, BaseGeometryType>;

  // Static Managed Dualtree for bounding boxes
  using StaticDualtreeBoxM = StaticTreeBoxManagedND<1, true, BaseGeometryType>;

  // Static Managed Quadtree for points
  using StaticQuadtreePointM = StaticTreePointManagedND<2, BaseGeometryType>;

  // Static Managed Quadtree for bounding boxes
  using StaticQuadtreeBoxM = StaticTreeBoxManagedND<2, true, BaseGeometryType>;

  // Static Managed Octree for points
  using StaticOctreePointM = StaticTreePointManagedND<3, BaseGeometryType>;

  // Static Managed Octree for bounding boxes
  using StaticOctreeBoxM = StaticTreeBoxManagedND<3, true, BaseGeometryType>;


  // Static Managed std::unordered_map-based Dualtree for points
  using StaticDualtreePointMapM = StaticTreePointManagedND<1, BaseGeometryType, false>;

  // Static Managed std::unordered_map-based Dualtree for bounding boxes
  using StaticDualtreeBoxMapM = StaticTreeBoxManagedND<1, true, BaseGeometryType, false>;

  // Static Managed std::unordered_map-based Quadtree for points
  using StaticQuadtreePointMapM = StaticTreePointManagedND<2, BaseGeometryType, false>;

  // Static Managed std::unordered_map-based Quadtree for bounding boxes
  using StaticQuadtreeBoxMapM = StaticTreeBoxManagedND<2, true, BaseGeometryType, false>;

  // Static Managed std::unordered_map-based Octree for points
  using StaticOctreePointMapM = StaticTreePointManagedND<3, BaseGeometryType, false>;

  // Static Managed std::unordered_map-based Octree for bounding boxes
  using StaticOctreeBoxMapM = StaticTreeBoxManagedND<3, true, BaseGeometryType, false>;

} // namespace OrthoTree
