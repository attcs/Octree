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

namespace OrthoTree
{
  template<typename TEntityAdapter, typename TGeometryAdapter, typename TConfiguration>
  using DynamicOrthoTreeBase = OrthoTreeQueryBase<DynamicHashOrthoTreeCore<TEntityAdapter, TGeometryAdapter, TConfiguration>>;

  template<typename TEntityAdapter, typename TGeometryAdapter, typename TConfiguration>
  using StaticOrthoTreeBase = OrthoTreeQueryBase<StaticLinearOrthoTreeCore<TEntityAdapter, TGeometryAdapter, TConfiguration>>;

  template<typename TEntityAdapter, typename TGeometryAdapter, typename TConfiguration>
  using OrthoTreeBase = DynamicOrthoTreeBase<TEntityAdapter, TGeometryAdapter, TConfiguration>;

  // Tree aliases

  template<dim_t DIMENSION_NO, typename TScalar = BaseGeometryType, bool IS_CONTIOGUOS_CONTAINER = true>
  using TreePointND = OrthoTree::OrthoTreeBase<
    std::conditional_t<IS_CONTIOGUOS_CONTAINER, PointEntitySpanAdapter<PointND<DIMENSION_NO, TScalar>>, PointEntityMapAdapter<PointND<DIMENSION_NO, TScalar>>>,
    GeneralGeometryAdapterND<DIMENSION_NO, TScalar>,
    PointConfiguration>;

  template<dim_t DIMENSION_NO, bool IS_LOOSE_TREE = true, typename TScalar = BaseGeometryType, bool IS_CONTIOGUOS_CONTAINER = true>
  using TreeBoxND = OrthoTree::OrthoTreeBase<
    std::conditional_t<IS_CONTIOGUOS_CONTAINER, BoxEntitySpanAdapter<BoundingBoxND<DIMENSION_NO, TScalar>>, BoxEntityMapAdapter<BoundingBoxND<DIMENSION_NO, TScalar>>>,
    GeneralGeometryAdapterND<DIMENSION_NO, TScalar>,
    BoxConfiguration<IS_LOOSE_TREE>>;

  template<dim_t DIMENSION_NO, typename TScalar, typename TEntityContainer>
  using TreePointNDUD =
    OrthoTree::OrthoTreeBase<PointEntityMapAdapter<PointND<DIMENSION_NO, TScalar>, TEntityContainer>, GeneralGeometryAdapterND<DIMENSION_NO, TScalar>, PointConfiguration>;

  template<dim_t DIMENSION_NO, bool IS_LOOSE_TREE, typename TScalar, typename TEntityContainer>
  using TreeBoxNDUD = OrthoTree::OrthoTreeBase<
    BoxEntityMapAdapter<BoundingBoxND<DIMENSION_NO, TScalar>, TEntityContainer>,
    GeneralGeometryAdapterND<DIMENSION_NO, TScalar>,
    BoxConfiguration<IS_LOOSE_TREE>>;

  template<dim_t DIMENSION_NO, typename TScalar = BaseGeometryType, bool IS_CONTIOGUOS_CONTAINER = true>
  using StaticTreePointND = OrthoTree::StaticOrthoTreeBase<
    std::conditional_t<IS_CONTIOGUOS_CONTAINER, PointEntitySpanAdapter<PointND<DIMENSION_NO, TScalar>>, PointEntityMapAdapter<PointND<DIMENSION_NO, TScalar>>>,
    GeneralGeometryAdapterND<DIMENSION_NO, TScalar>,
    PointConfiguration>;

  template<dim_t DIMENSION_NO, bool IS_LOOSE_TREE = true, typename TScalar = BaseGeometryType, bool IS_CONTIOGUOS_CONTAINER = true>
  using StaticTreeBoxND = OrthoTree::StaticOrthoTreeBase<
    std::conditional_t<IS_CONTIOGUOS_CONTAINER, BoxEntitySpanAdapter<BoundingBoxND<DIMENSION_NO, TScalar>>, BoxEntityMapAdapter<BoundingBoxND<DIMENSION_NO, TScalar>>>,
    GeneralGeometryAdapterND<DIMENSION_NO, TScalar>,
    BoxConfiguration<IS_LOOSE_TREE>>;

  template<dim_t DIMENSION_NO, typename TScalar, typename TEntityContainer>
  using StaticTreePointNDUD =
    OrthoTree::StaticOrthoTreeBase<PointEntityMapAdapter<PointND<DIMENSION_NO, TScalar>, TEntityContainer>, GeneralGeometryAdapterND<DIMENSION_NO, TScalar>, PointConfiguration>;

  template<dim_t DIMENSION_NO, bool IS_LOOSE_TREE, typename TScalar, typename TEntityContainer>
  using StaticTreeBoxNDUD = OrthoTree::StaticOrthoTreeBase<
    BoxEntityMapAdapter<BoundingBoxND<DIMENSION_NO, TScalar>, TEntityContainer>,
    GeneralGeometryAdapterND<DIMENSION_NO, TScalar>,
    BoxConfiguration<IS_LOOSE_TREE>>;


  // Dualtree for points
  using DualtreePoint = TreePointND<1, BaseGeometryType>;

  // Dualtree for bounding boxes
  using DualtreeBox = TreeBoxND<1, true, BaseGeometryType>;

  // Quadtree for points
  using QuadtreePoint = TreePointND<2, BaseGeometryType>;

  // Quadtree for bounding boxes
  using QuadtreeBox = TreeBoxND<2, true, BaseGeometryType>;

  // Octree for points
  using OctreePoint = TreePointND<3, BaseGeometryType>;

  // Octree for bounding boxes
  using OctreeBox = TreeBoxND<3, true, BaseGeometryType>;

  // Hexatree for points
  using HexatreePoint = TreePointND<4, BaseGeometryType>;

  // Hexatree for bounding boxes
  using HexatreeBox = TreeBoxND<4, true, BaseGeometryType>;

  // OrthoTrees for higher dimensions
  using TreePoint16D = TreePointND<16, BaseGeometryType>;
  using TreeBox16D = TreeBoxND<16, true, BaseGeometryType>;


  // Dualtree for bounding boxes with split-depth settings
  template<bool IS_LOOSE_TREE>
  using DualtreeBoxs = TreeBoxND<1, IS_LOOSE_TREE, BaseGeometryType>;

  // Quadtree for bounding boxes with split-depth settings
  template<bool IS_LOOSE_TREE>
  using QuadtreeBoxs = TreeBoxND<2, IS_LOOSE_TREE, BaseGeometryType>;

  // Octree for bounding boxes with split-depth settings
  template<bool IS_LOOSE_TREE>
  using OctreeBoxs = TreeBoxND<3, IS_LOOSE_TREE, BaseGeometryType>;

  // Hexatree for bounding boxes with split-depth settings
  template<bool IS_LOOSE_TREE>
  using HexatreeBoxs = TreeBoxND<4, IS_LOOSE_TREE, BaseGeometryType>;

  // OrthoTrees for higher dimensions with split-depth settings
  template<bool IS_LOOSE_TREE>
  using TreeBox16Ds = TreeBoxND<16, IS_LOOSE_TREE, BaseGeometryType>;


  // OrthoTrees with std::unordered_map

  // std::unordered_map-based Quadtree for points
  using QuadtreePointMap = TreePointND<2, BaseGeometryType, false>;

  // std::unordered_map-based Octree for points
  using OctreePointMap = TreePointND<3, BaseGeometryType, false>;

  // std::unordered_map-based Octree for bounding boxes
  using QuadreeBoxMap = TreeBoxND<2, true, BaseGeometryType, false>;

  // std::unordered_map-based Octree for bounding boxes
  using OctreeBoxMap = TreeBoxND<3, true, BaseGeometryType, false>;

  // std::unordered_map-based Quadtree for bounding boxes with split-depth settings
  template<bool IS_LOOSE_TREE>
  using QuadtreeBoxsMap = TreeBoxND<2, IS_LOOSE_TREE, BaseGeometryType, false>;
  using QuadtreeBoxMap = TreeBoxND<2, true, BaseGeometryType, false>;

  // std::unordered_map-based Octree for bounding boxes with split-depth settings
  template<bool IS_LOOSE_TREE>
  using OctreeBoxsMap = TreeBoxND<3, IS_LOOSE_TREE, BaseGeometryType, false>;
  using OctreeBoxMap = TreeBoxND<3, true, BaseGeometryType, false>;


  template<dim_t DIMENSION_NO, typename TScalar = BaseGeometryType, typename TEntityAdapter = PointEntitySpanAdapter<PointND<DIMENSION_NO, TScalar>>>
  using TreePointEAND = OrthoTree::OrthoTreeBase<TEntityAdapter, GeneralGeometryAdapterND<DIMENSION_NO, TScalar>, PointConfiguration>;

  template<dim_t DIMENSION_NO, bool IS_LOOSE_TREE = true, typename TScalar = BaseGeometryType, typename TEntityAdapter = BoxEntitySpanAdapter<BoundingBoxND<DIMENSION_NO, TScalar>>>
  using TreeBoxEAND = OrthoTree::OrthoTreeBase<TEntityAdapter, GeneralGeometryAdapterND<DIMENSION_NO, TScalar>, BoxConfiguration<IS_LOOSE_TREE>>;


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
  using StaticDualtreePoint = StaticTreePointND<1, BaseGeometryType>;

  // Dualtree for bounding boxes
  using StaticDualtreeBox = StaticTreeBoxND<1, true, BaseGeometryType>;

  // Quadtree for points
  using StaticQuadtreePoint = StaticTreePointND<2, BaseGeometryType>;

  // Quadtree for bounding boxes
  using StaticQuadtreeBox = StaticTreeBoxND<2, true, BaseGeometryType>;

  // Octree for points
  using StaticOctreePoint = StaticTreePointND<3, BaseGeometryType>;

  // Octree for bounding boxes
  using StaticOctreeBox = StaticTreeBoxND<3, true, BaseGeometryType>;

  // Hexatree for points
  using StaticHexatreePoint = StaticTreePointND<4, BaseGeometryType>;

  // Hexatree for bounding boxes
  using StaticHexatreeBox = StaticTreeBoxND<4, true, BaseGeometryType>;


  // Dualtree for bounding boxes with split-depth settings
  template<bool IS_LOOSE_TREE>
  using StaticDualtreeBoxs = StaticTreeBoxND<1, IS_LOOSE_TREE, BaseGeometryType>;

  // Quadtree for bounding boxes with split-depth settings
  template<bool IS_LOOSE_TREE>
  using StaticQuadtreeBoxs = StaticTreeBoxND<2, IS_LOOSE_TREE, BaseGeometryType>;

  // Octree for bounding boxes with split-depth settings
  template<bool IS_LOOSE_TREE>
  using StaticOctreeBoxs = StaticTreeBoxND<3, IS_LOOSE_TREE, BaseGeometryType>;

  // Hexatree for bounding boxes with split-depth settings
  template<bool IS_LOOSE_TREE>
  using StaticHexatreeBoxs = StaticTreeBoxND<4, IS_LOOSE_TREE, BaseGeometryType>;


  // OrthoTrees with std::unordered_map

  // std::unordered_map-based Quadtree for points
  using StaticQuadtreePointMap = StaticTreePointND<2, BaseGeometryType, false>;

  // std::unordered_map-based Octree for points
  using StaticOctreePointMap = StaticTreePointND<3, BaseGeometryType, false>;

  // std::unordered_map-based Octree for bounding boxes
  using StaticQuadreeBoxMap = StaticTreeBoxND<2, true, BaseGeometryType, false>;

  // std::unordered_map-based Octree for bounding boxes
  using StaticOctreeBoxMap = StaticTreeBoxND<3, true, BaseGeometryType, false>;

  // std::unordered_map-based Quadtree for bounding boxes with split-depth settings
  template<bool IS_LOOSE_TREE>
  using StaticQuadtreeBoxsMap = StaticTreeBoxND<2, IS_LOOSE_TREE, BaseGeometryType, false>;
  using StaticQuadtreeBoxMap = StaticTreeBoxND<2, true, BaseGeometryType, false>;

  // std::unordered_map-based Octree for bounding boxes with split-depth settings
  template<bool IS_LOOSE_TREE>
  using StaticOctreeBoxsMap = StaticTreeBoxND<3, IS_LOOSE_TREE, BaseGeometryType, false>;
  using StaticOctreeBoxMap = StaticTreeBoxND<3, true, BaseGeometryType, false>;


  template<dim_t DIMENSION_NO, typename TScalar = BaseGeometryType, typename TEntityAdapter = PointEntitySpanAdapter<PointND<DIMENSION_NO, TScalar>>>
  using StaticTreePointEAND = OrthoTree::StaticOrthoTreeBase<TEntityAdapter, GeneralGeometryAdapterND<DIMENSION_NO, TScalar>, PointConfiguration>;

  template<dim_t DIMENSION_NO, bool IS_LOOSE_TREE = true, typename TScalar = BaseGeometryType, typename TEntityAdapter = BoxEntitySpanAdapter<BoundingBoxND<DIMENSION_NO, TScalar>>>
  using StaticTreeBoxEAND =
    OrthoTree::StaticOrthoTreeBase<TEntityAdapter, GeneralGeometryAdapterND<DIMENSION_NO, TScalar>, BoxConfiguration<IS_LOOSE_TREE>>;


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

} // namespace OrthoTree
