/*
MIT License

Copyright (c) 2026 Attila Csikós

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

#include "../core/bvh_static_linear_core.h"
#include "../core/ot_managed.h"
#include "../core/ot_query.h"

#include "../adapters/general.h"

#include <type_traits>

namespace OrthoTree
{
  template<dim_t DIMENSION_NO, typename TScalar = BaseGeometryType, bool IS_CONTIOGUOS_CONTAINER = true, int CHILD_NUM = 2>
  using StaticBVHPointND = StaticBVHLinearBase<
    std::conditional_t<IS_CONTIOGUOS_CONTAINER, PointEntitySpanAdapter<PointND<DIMENSION_NO, TScalar>>, PointEntityMapAdapter<PointND<DIMENSION_NO, TScalar>>>,
    GeneralGeometryAdapterND<DIMENSION_NO, TScalar>,
    BVHConfiguration<CHILD_NUM>>;

  template<dim_t DIMENSION_NO, typename TScalar = BaseGeometryType, bool IS_CONTIOGUOS_CONTAINER = true, int CHILD_NUM = 2>
  using StaticBVHBoxND = StaticBVHLinearBase<
    std::conditional_t<IS_CONTIOGUOS_CONTAINER, BoxEntitySpanAdapter<BoundingBoxND<DIMENSION_NO, TScalar>>, BoxEntityMapAdapter<BoundingBoxND<DIMENSION_NO, TScalar>>>,
    GeneralGeometryAdapterND<DIMENSION_NO, TScalar>,
    BVHConfiguration<CHILD_NUM>>;

  // BVH for points
  using StaticBVHPoint1D = StaticBVHPointND<1, BaseGeometryType>;
  using StaticBVHPoint2D = StaticBVHPointND<2, BaseGeometryType>;
  using StaticBVHPoint3D = StaticBVHPointND<3, BaseGeometryType>;
  using StaticBVHPoint4D = StaticBVHPointND<4, BaseGeometryType>;

  // BVH for bounding boxes
  using StaticBVHBox1D = StaticBVHBoxND<1, BaseGeometryType>;
  using StaticBVHBox2D = StaticBVHBoxND<2, BaseGeometryType>;
  using StaticBVHBox3D = StaticBVHBoxND<3, BaseGeometryType>;
  using StaticBVHBox4D = StaticBVHBoxND<4, BaseGeometryType>;

  // BVH with unordered_map entities
  using StaticBVHPointMap1D = StaticBVHPointND<1, BaseGeometryType, false>;
  using StaticBVHPointMap2D = StaticBVHPointND<2, BaseGeometryType, false>;
  using StaticBVHPointMap3D = StaticBVHPointND<3, BaseGeometryType, false>;
  using StaticBVHPointMap4D = StaticBVHPointND<4, BaseGeometryType, false>;
  using StaticBVHBoxMap1D = StaticBVHBoxND<1, BaseGeometryType, false>;
  using StaticBVHBoxMap2D = StaticBVHBoxND<2, BaseGeometryType, false>;
  using StaticBVHBoxMap3D = StaticBVHBoxND<3, BaseGeometryType, false>;
  using StaticBVHBoxMap4D = StaticBVHBoxND<4, BaseGeometryType, false>;

  template<dim_t DIMENSION_NO, int CHILD_NUM = 8>
  using StaticBVHNPointND = StaticBVHPointND<DIMENSION_NO, BaseGeometryType, true, CHILD_NUM>;
  template<dim_t DIMENSION_NO, int CHILD_NUM = 8>
  using StaticBVHNBoxND = StaticBVHBoxND<DIMENSION_NO, BaseGeometryType, true, CHILD_NUM>;

  template<dim_t DIMENSION_NO, int CHILD_NUM = 8>
  using StaticBVHNPointMapND = StaticBVHPointND<DIMENSION_NO, BaseGeometryType, false, CHILD_NUM>;
  template<dim_t DIMENSION_NO, int CHILD_NUM = 8>
  using StaticBVHNBoxMapND = StaticBVHBoxND<DIMENSION_NO, BaseGeometryType, false, CHILD_NUM>;

  // Managed types

  template<dim_t DIMENSION_NO, typename TScalar = BaseGeometryType, bool IS_CONTIOGUOS_CONTAINER = true, int CHILD_NUM = 2>
  using StaticBVHPointManagedND = OrthoTreeManaged<StaticBVHPointND<DIMENSION_NO, TScalar, IS_CONTIOGUOS_CONTAINER, CHILD_NUM>>;

  template<dim_t DIMENSION_NO, typename TScalar = BaseGeometryType, bool IS_CONTIOGUOS_CONTAINER = true, int CHILD_NUM = 2>
  using StaticBVHBoxManagedND = OrthoTreeManaged<StaticBVHBoxND<DIMENSION_NO, TScalar, IS_CONTIOGUOS_CONTAINER, CHILD_NUM>>;

  // BVH for points
  using StaticBVHPoint1DM = StaticBVHPointManagedND<1, BaseGeometryType>;
  using StaticBVHPoint2DM = StaticBVHPointManagedND<2, BaseGeometryType>;
  using StaticBVHPoint3DM = StaticBVHPointManagedND<3, BaseGeometryType>;
  using StaticBVHPoint4DM = StaticBVHPointManagedND<4, BaseGeometryType>;

  // BVH for bounding boxes
  using StaticBVHBox1DM = StaticBVHBoxManagedND<1, BaseGeometryType>;
  using StaticBVHBox2DM = StaticBVHBoxManagedND<2, BaseGeometryType>;
  using StaticBVHBox3DM = StaticBVHBoxManagedND<3, BaseGeometryType>;
  using StaticBVHBox4DM = StaticBVHBoxManagedND<4, BaseGeometryType>;

  // BVH with unordered_map entities
  using StaticBVHPointMap1DM = StaticBVHPointManagedND<1, BaseGeometryType, false>;
  using StaticBVHPointMap2DM = StaticBVHPointManagedND<2, BaseGeometryType, false>;
  using StaticBVHPointMap3DM = StaticBVHPointManagedND<3, BaseGeometryType, false>;
  using StaticBVHPointMap4DM = StaticBVHPointManagedND<4, BaseGeometryType, false>;
  using StaticBVHBoxMap1DM = StaticBVHBoxManagedND<1, BaseGeometryType, false>;
  using StaticBVHBoxMap2DM = StaticBVHBoxManagedND<2, BaseGeometryType, false>;
  using StaticBVHBoxMap3DM = StaticBVHBoxManagedND<3, BaseGeometryType, false>;
  using StaticBVHBoxMap4DM = StaticBVHBoxManagedND<4, BaseGeometryType, false>;

  // N-ary BVH Managed
  template<dim_t DIMENSION_NO, int CHILD_NUM = 8>
  using StaticBVHNPointManagedND = StaticBVHPointManagedND<DIMENSION_NO, BaseGeometryType, true, CHILD_NUM>;
  template<dim_t DIMENSION_NO, int CHILD_NUM = 8>
  using StaticBVHNBoxManagedND = StaticBVHBoxManagedND<DIMENSION_NO, BaseGeometryType, true, CHILD_NUM>;

  template<dim_t DIMENSION_NO, int CHILD_NUM = 8>
  using StaticBVHNPointMapManagedND = StaticBVHPointManagedND<DIMENSION_NO, BaseGeometryType, false, CHILD_NUM>;
  template<dim_t DIMENSION_NO, int CHILD_NUM = 8>
  using StaticBVHNBoxMapManagedND = StaticBVHBoxManagedND<DIMENSION_NO, BaseGeometryType, false, CHILD_NUM>;
} // namespace OrthoTree
