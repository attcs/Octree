#pragma once
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

#include "../octree.h"
#include "general.h"

// #define UNREAL_DUMMY_TYPES
// #define UNREAL_DUMMY_TYPES__SUPPRESS_ASSERTS

#ifdef UNREAL_DUMMY_TYPES

#ifdef UNREAL_DUMMY_TYPES__SUPPRESS_ASSERTS
#define NOT_IMPLEMENTED
#else
#define NOT_IMPLEMENTED assert(false)
#endif

namespace UnrealDummyTypes
{
  // Helper templates

  template<typename FVector_>
  struct FVectorTemplate
  {
    float Size() const
    {
      NOT_IMPLEMENTED;
      return {};
    }

    float SizeSquared() const
    {
      NOT_IMPLEMENTED;
      return {};
    }

    bool IsUnit(float) const
    {
      NOT_IMPLEMENTED;
      return true;
    }

    static float Dist(FVector_ const&, FVector_ const&)
    {
      NOT_IMPLEMENTED;
      return {};
    }

    static float DistSquared(FVector_ const&, FVector_ const&)
    {
      NOT_IMPLEMENTED;
      return {};
    }

    static float DotProduct(FVector_ const&, FVector_ const&)
    {
      NOT_IMPLEMENTED;
      return {};
    }

    FVector_ operator+=(FVector_ const&) const
    {
      NOT_IMPLEMENTED;
      return {};
    }

    FVector_ operator+(FVector_ const&) const
    {
      NOT_IMPLEMENTED;
      return {};
    }

    FVector_ operator-=(FVector_ const&) const
    {
      NOT_IMPLEMENTED;
      return {};
    }

    FVector_ operator-(FVector_ const&) const
    {
      NOT_IMPLEMENTED;
      return {};
    }

    FVector_ operator*=(float) const
    {
      NOT_IMPLEMENTED;
      return {};
    }

    FVector_ operator*(float) const
    {
      NOT_IMPLEMENTED;
      return {};
    }
  };

  template<typename geometry_type>
  struct FVectorTemplate2 : FVectorTemplate<FVectorTemplate2<geometry_type>>
  {
    geometry_type X;
    geometry_type Y;

    FVectorTemplate2() = default;
    FVectorTemplate2(geometry_type X, geometry_type Y)
    : X(X)
    , Y(Y)
    {}
  };

  template<typename geometry_type>
  struct FVectorTemplate3 : FVectorTemplate<FVectorTemplate3<geometry_type>>
  {
    geometry_type X;
    geometry_type Y;
    geometry_type Z;

    FVectorTemplate3() = default;
    FVectorTemplate3(geometry_type X, geometry_type Y, geometry_type Z)
    : X(X)
    , Y(Y)
    , Z(Z)
    {}
  };

  template<typename FVector_>
  struct FBoxTemplate
  {
    FVector_ Min;
    FVector_ Max;
    bool IsValid;

    FBoxTemplate() = default;
    FBoxTemplate(FVector_&& Min, FVector_&& Max)
    : Min(Min)
    , Max(Max)
    {}

    FBoxTemplate(FVector_ const& Min, FVector_ const& Max)
    : Min(Min)
    , Max(Max)
    {}

    FVector_ GetCenter() const
    {
      NOT_IMPLEMENTED;
      return FVector_{};
    }

    FVector_ GetSize() const
    {
      NOT_IMPLEMENTED;
      return FVector_{};
    }

    float GetVolume() const
    {
      NOT_IMPLEMENTED;
      return 0.0f;
    }

    bool Intersect(FBoxTemplate const&) const
    {
      NOT_IMPLEMENTED;
      return false;
    }

    bool IsInside(FBoxTemplate const&) const
    {
      NOT_IMPLEMENTED;
      return false;
    }

    bool IsInside(FVector_ const&) const
    {
      NOT_IMPLEMENTED;
      return false;
    }

    bool IsInsideOrOn(FVector_ const&) const
    {
      NOT_IMPLEMENTED;
      return false;
    }

    FBoxTemplate Overlap(FBoxTemplate const&)
    {
      NOT_IMPLEMENTED;
      return {};
    }

    FBoxTemplate ExpandBy(FVector_ const& v) const
    {
      NOT_IMPLEMENTED;
      return {};
    }

    FBoxTemplate ShiftBy(FVector_ const& v) const
    {
      NOT_IMPLEMENTED;
      return {};
    }
  };

  template<typename FVector_>
  struct FRayTemplate
  {
    FVector_ Direction;
    FVector_ Origin;
  };

  template<typename FVector_>
  struct FPlaneTemplate : public FVector_
  {
    float W;

    const FVector_& GetNormal() const { return *this; }
  };


  // Unreal types
  using int32 = int32_t;
  using int64 = int64_t;
  using FLargeWorldCoordinatesReal = double;

  using FVector2D = FVectorTemplate2<FLargeWorldCoordinatesReal>;
  using FVector2f = FVectorTemplate2<float>;

  using FVector = FVectorTemplate3<FLargeWorldCoordinatesReal>;
  using FVector3f = FVectorTemplate3<float>;
  using FVector3d = FVectorTemplate3<double>;

  using FBox = FBoxTemplate<FVector>;
  using FBox2f = FBoxTemplate<FVector2f>;
  using FBox2D = FBoxTemplate<FVector2D>;
  using FBox3f = FBoxTemplate<FVector3f>;
  using FBox3d = FBoxTemplate<FVector3d>;

  using FRay2D = FRayTemplate<FBox2D>;
  using FRay = FRayTemplate<FVector>;

  using FPlane2D = FPlaneTemplate<FVector2D>;
  using FPlane = FPlaneTemplate<FVector>;
} // namespace UnrealDummyTypes
#endif

#ifdef UNREAL_DUMMY_TYPES
using namespace UnrealDummyTypes;
#endif

namespace OrthoTree
{
  namespace UnrealAdapter
  {
    template<typename FVector2D_>
    struct FRay2DTemplate
    {
      FVector2D_ Direction;
      FVector2D_ Origin;
    };

    template<typename FGeometry_, typename FVector2D_, typename FBox2D_>
    struct UnrealBaseGeometryAdapter2D
    {
      using Scalar = FGeometry_;
      using FloatScalar = FGeometry_;
      using Vector = FVector2D_;
      using Box = FBox2D_;
      using Ray = FRay2DTemplate<FVector2D_>;
      using Plane = FRay2DTemplate<FVector2D_>;

      static constexpr dim_t DIMENSION_NO = 2;
      static constexpr FloatScalar BASE_TOLERANCE = std::numeric_limits<FloatScalar>::epsilon() * FloatScalar(10);

      // There is no 2d ray and plane in Unreal
      using FRay2D_ = FRay2DTemplate<FVector2D_>;
      using FPlane2D_ = FRay2DTemplate<FVector2D_>;

      static constexpr FGeometry_ GetPointC(FVector2D_ const& pt, dim_t dimensionID)
      {
        switch (dimensionID)
        {
        case 0: return pt.X;
        case 1: return pt.Y;
        default: std::terminate();
        }
      }

      static constexpr void SetPointC(FVector2D_& pt, dim_t dimensionID, FGeometry_ value)
      {
        switch (dimensionID)
        {
        case 0: pt.X = value; break;
        case 1: pt.Y = value; break;
        default: std::terminate();
        }
      }

      static constexpr FGeometry_ GetBoxMinC(FBox2D_ const& box, dim_t dimensionID) { return GetPointC(box.Min, dimensionID); }
      static constexpr FGeometry_ GetBoxMaxC(FBox2D_ const& box, dim_t dimensionID) { return GetPointC(box.Max, dimensionID); }
      static constexpr void SetBoxMinC(FBox2D_& box, dim_t dimensionID, FGeometry_ value) { SetPointC(box.Min, dimensionID, value); }
      static constexpr void SetBoxMaxC(FBox2D_& box, dim_t dimensionID, FGeometry_ value) { SetPointC(box.Max, dimensionID, value); }

      static constexpr FVector2D_ const& GetRayDirection(FRay2D_ const& ray) noexcept { return ray.Direction; }
      static constexpr FVector2D_ const& GetRayOrigin(FRay2D_ const& ray) noexcept { return ray.Origin; }

      static constexpr FVector2D_ const& GetPlaneNormal(FPlane2D_ const& plane) noexcept { return plane.Direction; }
      static constexpr FGeometry_ GetPlaneOrigoDistance(FPlane2D_ const& plane) noexcept
      {
        return FVector2D_::DotProduct(plane.Origin, plane.Direction);
      }
    };


    template<typename FGeometry_, typename FVector_, typename FBox_, typename FRay_, typename FPlane_>
    struct UnrealBaseGeometryAdapter3D
    {
      using Scalar = FGeometry_;
      using FloatScalar = FGeometry_;
      using Vector = FVector_;
      using Box = FBox_;
      using Ray = FRay_;
      using Plane = FPlane_;

      static constexpr dim_t DIMENSION_NO = 3;
      static constexpr FloatScalar BASE_TOLERANCE = std::numeric_limits<FloatScalar>::epsilon() * FloatScalar(10);

      static constexpr FVector MakePoint() { return {}; }
      static constexpr FGeometry_ GetPointC(FVector_ const& v, dim_t dimensionID)
      {
        switch (dimensionID)
        {
        case 0: return v.X;
        case 1: return v.Y;
        case 2: return v.Z;
        default: assert(false); std::terminate();
        }
      }

      static constexpr void SetPointC(FVector_& v, dim_t dimensionID, FGeometry_ value)
      {
        switch (dimensionID)
        {
        case 0: v.X = value; break;
        case 1: v.Y = value; break;
        case 2: v.Z = value; break;
        default: assert(false); std::terminate();
        }
      }

      static constexpr FBox_ MakeBox() { return {}; }
      static constexpr FGeometry_ GetBoxMinC(FBox_ const& box, dim_t dimensionID) { return GetPointC(box.Min, dimensionID); }
      static constexpr FGeometry_ GetBoxMaxC(FBox_ const& box, dim_t dimensionID) { return GetPointC(box.Max, dimensionID); }
      static constexpr void SetBoxMinC(FBox_& box, dim_t dimensionID, FGeometry_ value) { SetPointC(box.Min, dimensionID, value); }
      static constexpr void SetBoxMaxC(FBox_& box, dim_t dimensionID, FGeometry_ value) { SetPointC(box.Max, dimensionID, value); }

      static constexpr FVector_ const& GetRayDirection(FRay_ const& ray) noexcept { return ray.Direction; }
      static constexpr FVector_ const& GetRayOrigin(FRay_ const& ray) noexcept { return ray.Origin; }

      static constexpr FVector_ const& GetPlaneNormal(FPlane_ const& plane) noexcept { return plane.GetNormal(); }
      static constexpr FGeometry_ GetPlaneOrigoDistance(FPlane_ const& plane) noexcept { return FGeometry_(plane.W); }
    };

    template<typename TUnrealBaseGeometryAdapter>
    struct UnrealGeometryAdapter : GeneralGeometryAdapter<TUnrealBaseGeometryAdapter>
    {
      using Base = TUnrealBaseGeometryAdapter;

      using Scalar = Base::Scalar;
      using FloatScalar = Base::FloatScalar;
      using Vector = Base::Vector;
      using Box = Base::Box;
      using Ray = Base::Ray;
      using Plane = Base::Plane;

      static constexpr dim_t DIMENSION_NO = Base::DIMENSION_NO;

      static_assert(BaseGeometryAdapterConcept<Base, DIMENSION_NO, Vector, Box, Ray, Plane, Scalar, FloatScalar>);

      static constexpr Scalar Size2(Vector const& v) noexcept { return v.SizeSquared(); }

      static constexpr Scalar Size(Vector const& point) noexcept { return point.Size(); }

      static constexpr Vector Add(Vector const& v1, Vector const& v2) noexcept { return v1 + v2; }

      static constexpr Vector Subtract(Vector const& v1, Vector const& v2) noexcept { return v1 - v2; }

      static constexpr Vector Multiply(Vector const& v, FloatScalar scalarFactor) noexcept { return v * float(scalarFactor); }

      static constexpr Scalar Dot(Vector const& v1, Vector const& v2) noexcept { return Vector::DotProduct(v1, v2); }

      static constexpr Scalar Distance(Vector const& v1, Vector const& v2) noexcept { return Vector::Dist(v1, v2); }

      static constexpr Scalar Distance2(Vector const& v1, Vector const& v2) noexcept { return Vector::DistSquared(v1, v2); }

      static constexpr bool ArePointsEqual(Vector const& v1, Vector const& v2, FloatScalar tolerance) noexcept
      {
        return Distance2(v1, v2) <= tolerance * tolerance;
      }

      static constexpr bool IsNormalizedVector(Vector const& normal) noexcept { return normal.IsUnit(0.000001f); }

      static constexpr bool IsAlmostEqual(FloatScalar lhs, FloatScalar rhs, FloatScalar tolerance) noexcept
      {
        return std::abs(lhs - rhs) <= tolerance;
      }

      static Box GetBoxOfPoints(std::span<Vector const> const& points) noexcept
      {
        auto ext = points.size() == 0 ? Box{} : Box(points[0], points[0]);
        for (auto const& point : points)
        {
          for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
          {
            if (Base::GetBoxMinC(ext, dimensionID) > Base::GetPointC(point, dimensionID))
              Base::SetBoxMinC(ext, dimensionID, Base::GetPointC(point, dimensionID));

            if (Base::GetBoxMaxC(ext, dimensionID) < Base::GetPointC(point, dimensionID))
              Base::SetBoxMaxC(ext, dimensionID, Base::GetPointC(point, dimensionID));
          }
        }
        return ext;
      }

      static Box GetBoxOfBoxes(std::span<Box const> const& extents) noexcept
      {
        if (extents.size() == 0)
          return {};

        auto ext = extents[0];
        for (auto const& e : extents)
        {
          for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
          {
            if (Base::GetBoxMinC(ext, dimensionID) > Base::GetBoxMinC(e, dimensionID))
              Base::SetBoxMinC(ext, dimensionID, Base::GetBoxMinC(e, dimensionID));

            if (Base::GetBoxMaxC(ext, dimensionID) < Base::GetBoxMaxC(e, dimensionID))
              Base::SetBoxMaxC(ext, dimensionID, Base::GetBoxMaxC(e, dimensionID));
          }
        }

        return ext;
      }

      static void MoveBox(Box& box, Vector const& moveVector) noexcept { box = box.ShiftBy(moveVector); }
    };


    template<typename FGeometry_, typename FVector2D_, typename FBox2D_>
    using UnrealGeometryAdapter2D = UnrealGeometryAdapter<UnrealBaseGeometryAdapter2D<FGeometry_, FVector2D_, FBox2D_>>;

    template<typename FGeometry_, typename FVector_, typename FBox_, typename FRay_, typename FPlane_>
    using UnrealGeometryAdapter3D = UnrealGeometryAdapter<UnrealBaseGeometryAdapter3D<FGeometry_, FVector_, FBox_, FRay_, FPlane_>>;


    // Templates for point types

    template<typename FGeometry_, typename FVector_, typename FBox_>
    using QuadtreePointTemplate =
      OrthoTreeBase<PointEntitySpanAdapter<FVector_>, UnrealGeometryAdapter2D<FGeometry_, FVector_, FBox_>, PointConfiguration<>>;

    template<typename FGeometry_, typename FVector_, typename FBox_, typename FRay_, typename FPlane_>
    using OctreePointTemplate =
      OrthoTreeBase<PointEntitySpanAdapter<FVector_>, UnrealGeometryAdapter3D<FGeometry_, FVector_, FBox_, FRay_, FPlane_>, PointConfiguration<>>;

    template<typename FGeometry_, typename FVector_, typename FBox_, typename TContainer_ = std::unordered_map<index_t, FVector_>>
    using QuadtreePointMapTemplate =
      OrthoTreeBase<PointEntityMapAdapter<FVector_, TContainer_>, UnrealGeometryAdapter2D<FGeometry_, FVector_, FBox_>, PointConfiguration<>>;

    template<typename FGeometry_, typename FVector_, typename FBox_, typename FRay_, typename FPlane_, typename TContainer_ = std::unordered_map<index_t, FVector_>>
    using OctreePointMapTemplate =
      OrthoTreeBase<PointEntityMapAdapter<FVector_, TContainer_>, UnrealGeometryAdapter3D<FGeometry_, FVector_, FBox_, FRay_, FPlane_>, PointConfiguration<>>;

    // Templates for box types

    template<typename FGeometry_, typename FVector_, typename FBox_, bool IS_LOOSE_TREE = true>
    using QuadtreeBoxTemplate =
      OrthoTreeBase<BoxEntitySpanAdapter<FBox_>, UnrealGeometryAdapter2D<FGeometry_, FVector_, FBox_>, BoxConfiguration<IS_LOOSE_TREE>>;

    template<typename FGeometry_, typename FVector_, typename FBox_, typename FRay_, typename FPlane_, bool IS_LOOSE_TREE = true>
    using OctreeBoxTemplate =
      OrthoTreeBase<BoxEntitySpanAdapter<FBox_>, UnrealGeometryAdapter3D<FGeometry_, FVector_, FBox_, FPlane_, FGeometry_>, BoxConfiguration<IS_LOOSE_TREE>>;

    template<typename FGeometry_, typename FVector_, typename FBox_, bool IS_LOOSE_TREE = true, typename TContainer_ = std::unordered_map<index_t, FVector_>>
    using QuadtreeBoxMapTemplate =
      OrthoTreeBase<BoxEntityMapAdapter<FBox_, TContainer_>, UnrealGeometryAdapter2D<FGeometry_, FVector_, FBox_>, BoxConfiguration<IS_LOOSE_TREE>>;

    template<typename FGeometry_, typename FVector_, typename FBox_, typename FRay_, typename FPlane_, bool IS_LOOSE_TREE = true, typename TContainer_ = std::unordered_map<index_t, FVector_>>
    using OctreeBoxMapTemplate =
      OrthoTreeBase<BoxEntityMapAdapter<FBox_, TContainer_>, UnrealGeometryAdapter3D<FGeometry_, FVector_, FBox_, FRay_, FPlane_>, BoxConfiguration<IS_LOOSE_TREE>>;

  } // namespace UnrealAdapter
} // namespace OrthoTree


// Orthotree Core Types
using FLargeWorldCoordinatesReal = double;

using FQuadtreePoint = OrthoTree::UnrealAdapter::QuadtreePointTemplate<FLargeWorldCoordinatesReal, FVector2D, FBox2D>;
using FQuadtreePoint2D = FQuadtreePoint;
using FQuadtreePoint2f = OrthoTree::UnrealAdapter::QuadtreePointTemplate<float, FVector2f, FBox2f>;

using FOctreePoint = OrthoTree::UnrealAdapter::OctreePointTemplate<FLargeWorldCoordinatesReal, FVector, FBox, FRay, FPlane>;
using FOctreePoint3d = OrthoTree::UnrealAdapter::OctreePointTemplate<double, FVector3d, FBox3d, FRay, FPlane>;
using FOctreePoint3f = OrthoTree::UnrealAdapter::OctreePointTemplate<float, FVector3f, FBox3f, FRay, FPlane>;


template<bool IS_LOOSE_TREE = true>
using FQuadtreeBoxs = OrthoTree::UnrealAdapter::QuadtreeBoxTemplate<FLargeWorldCoordinatesReal, FVector2D, FBox2D, IS_LOOSE_TREE>;
using FQuadtreeBox = FQuadtreeBoxs<true>;

template<bool IS_LOOSE_TREE = true>
using FQuadtreeBox2Ds = FQuadtreeBoxs<IS_LOOSE_TREE>;
using FQuadtreeBox2D = FQuadtreeBox;

template<bool IS_LOOSE_TREE = true>
using FQuadtreeBox2fs = OrthoTree::UnrealAdapter::QuadtreeBoxTemplate<float, FVector2f, FBox2f, IS_LOOSE_TREE>;
using FQuadtreeBox2f = OrthoTree::UnrealAdapter::QuadtreeBoxTemplate<float, FVector2f, FBox2f, true>;

template<bool IS_LOOSE_TREE = true>
using FOctreeBoxs = OrthoTree::UnrealAdapter::OctreeBoxTemplate<FLargeWorldCoordinatesReal, FVector, FBox, FRay, FPlane, IS_LOOSE_TREE>;
using FOctreeBox = OrthoTree::UnrealAdapter::OctreeBoxTemplate<FLargeWorldCoordinatesReal, FVector, FBox, FRay, FPlane, true>;

template<bool IS_LOOSE_TREE = true>
using FOctreeBox3ds = OrthoTree::UnrealAdapter::OctreeBoxTemplate<double, FVector3d, FBox3d, FRay, FPlane, IS_LOOSE_TREE>;
using FOctreeBox3d = OrthoTree::UnrealAdapter::OctreeBoxTemplate<double, FVector3d, FBox3d, FRay, FPlane, true>;

template<bool IS_LOOSE_TREE = true>
using FOctreeBox3fs = OrthoTree::UnrealAdapter::OctreeBoxTemplate<float, FVector3f, FBox3f, FRay, FPlane, IS_LOOSE_TREE>;
using FOctreeBox3f = OrthoTree::UnrealAdapter::OctreeBoxTemplate<float, FVector3f, FBox3f, FRay, FPlane, true>;


// Orthotree Container Types

using FQuadtreePointC = OrthoTree::OrthoTreeContainer<FQuadtreePoint>;
using FQuadtreePoint2DC = FQuadtreePointC;
using FQuadtreePoint2fC = OrthoTree::OrthoTreeContainer<FQuadtreePoint2f>;

using FOctreePointC = OrthoTree::OrthoTreeContainer<FOctreePoint>;
using FOctreePoint3dC = OrthoTree::OrthoTreeContainer<FOctreePoint3d>;
using FOctreePoint3fC = OrthoTree::OrthoTreeContainer<FOctreePoint3f>;

template<bool IS_LOOSE_TREE = true>
using FQuadtreeBoxCs = OrthoTree::OrthoTreeContainer<FQuadtreeBoxs<IS_LOOSE_TREE>>;
using FQuadtreeBoxC = FQuadtreeBoxCs<true>;

template<bool IS_LOOSE_TREE = true>
using FQuadtreeBox2DCs = OrthoTree::OrthoTreeContainer<FQuadtreeBox2Ds<IS_LOOSE_TREE>>;
using FQuadtreeBox2DC = FQuadtreeBox2DCs<true>;

template<bool IS_LOOSE_TREE = true>
using FQuadtreeBox2fCs = OrthoTree::OrthoTreeContainer<FQuadtreeBox2fs<IS_LOOSE_TREE>>;
using FQuadtreeBox2fC = FQuadtreeBox2fCs<true>;

template<bool IS_LOOSE_TREE = true>
using FOctreeBoxCs = OrthoTree::OrthoTreeContainer<FOctreeBoxs<IS_LOOSE_TREE>>;
using FOctreeBoxC = FOctreeBoxCs<true>;

template<bool IS_LOOSE_TREE = true>
using FOctreeBox3dCs = OrthoTree::OrthoTreeContainer<FOctreeBox3ds<IS_LOOSE_TREE>>;
using FOctreeBox3dC = FOctreeBox3dCs<true>;

template<bool IS_LOOSE_TREE = true>
using FOctreeBox3fCs = OrthoTree::OrthoTreeContainer<FOctreeBox3fs<IS_LOOSE_TREE>>;
using FOctreeBox3fC = FOctreeBox3fCs<true>;


template<typename T>
using FContainer = std::unordered_map<int, T>;

using FQuadtreePointMap = OrthoTree::UnrealAdapter::QuadtreePointMapTemplate<FLargeWorldCoordinatesReal, FVector2D, FBox2D, FContainer<FVector2D>>;
using FQuadtreePointMap2D = FQuadtreePointMap;
using FQuadtreePointMap2f = OrthoTree::UnrealAdapter::QuadtreePointMapTemplate<float, FVector2f, FBox2f, FContainer<FVector2f>>;

using FOctreePointMap = OrthoTree::UnrealAdapter::OctreePointMapTemplate<FLargeWorldCoordinatesReal, FVector, FBox, FRay, FPlane, FContainer<FVector>>;
using FOctreePointMap3d = OrthoTree::UnrealAdapter::OctreePointMapTemplate<double, FVector3d, FBox3d, FRay, FPlane, FContainer<FVector3d>>;
using FOctreePointMap3f = OrthoTree::UnrealAdapter::OctreePointMapTemplate<float, FVector3f, FBox3f, FRay, FPlane, FContainer<FVector3f>>;


template<bool IS_LOOSE_TREE = true>
using FQuadtreeBoxsMap =
  OrthoTree::UnrealAdapter::QuadtreeBoxMapTemplate<FLargeWorldCoordinatesReal, FVector2D, FBox2D, IS_LOOSE_TREE, FContainer<FBox2D>>;
using FQuadtreeBoxMap = FQuadtreeBoxsMap<true>;

template<bool IS_LOOSE_TREE = true>
using FQuadtreeBoxMap2Ds = FQuadtreeBoxsMap<IS_LOOSE_TREE>;
using FQuadtreeBoxMap2D = FQuadtreeBoxMap;

template<bool IS_LOOSE_TREE = true>
using FQuadtreeBoxMap2fs = OrthoTree::UnrealAdapter::QuadtreeBoxMapTemplate<float, FVector2f, FBox2f, IS_LOOSE_TREE, FContainer<FBox2f>>;
using FQuadtreeBoxMap2f = OrthoTree::UnrealAdapter::QuadtreeBoxMapTemplate<float, FVector2f, FBox2f, true, FContainer<FBox2f>>;

template<bool IS_LOOSE_TREE = true>
using FOctreeBoxsMap =
  OrthoTree::UnrealAdapter::OctreeBoxMapTemplate<FLargeWorldCoordinatesReal, FVector, FBox, FRay, FPlane, IS_LOOSE_TREE, FContainer<FBox>>;
using FOctreeBoxMap = OrthoTree::UnrealAdapter::OctreeBoxMapTemplate<FLargeWorldCoordinatesReal, FVector, FBox, FRay, FPlane, true, FContainer<FBox>>;

template<bool IS_LOOSE_TREE = true>
using FOctreeBoxMap3ds = OrthoTree::UnrealAdapter::OctreeBoxMapTemplate<double, FVector3d, FBox3d, FRay, FPlane, IS_LOOSE_TREE, FContainer<FBox3d>>;
using FOctreeBoxMap3d = OrthoTree::UnrealAdapter::OctreeBoxMapTemplate<double, FVector3d, FBox3d, FRay, FPlane, true, FContainer<FBox3d>>;

template<bool IS_LOOSE_TREE = true>
using FOctreeBoxMap3fs = OrthoTree::UnrealAdapter::OctreeBoxMapTemplate<float, FVector3f, FBox3f, FRay, FPlane, IS_LOOSE_TREE, FContainer<FBox3f>>;
using FOctreeBoxMap3f = OrthoTree::UnrealAdapter::OctreeBoxMapTemplate<float, FVector3f, FBox3f, FRay, FPlane, true, FContainer<FBox3f>>;


using FQuadtreePointMapC = OrthoTree::OrthoTreeContainer<FQuadtreePointMap>;
using FQuadtreePointMap2DC = FQuadtreePointMapC;
using FQuadtreePointMap2fC = OrthoTree::OrthoTreeContainer<FQuadtreePointMap2f>;

using FOctreePointMapC = OrthoTree::OrthoTreeContainer<FOctreePointMap>;
using FOctreePointMap3dC = OrthoTree::OrthoTreeContainer<FOctreePointMap3d>;
using FOctreePointMap3fC = OrthoTree::OrthoTreeContainer<FOctreePointMap3f>;

template<bool IS_LOOSE_TREE = true>
using FQuadtreeBoxMapCs = OrthoTree::OrthoTreeContainer<FQuadtreeBoxsMap<IS_LOOSE_TREE>>;
using FQuadtreeBoxMapC = FQuadtreeBoxMapCs<true>;

template<bool IS_LOOSE_TREE = true>
using FQuadtreeBoxMap2DCs = OrthoTree::OrthoTreeContainer<FQuadtreeBoxMap2Ds<IS_LOOSE_TREE>>;
using FQuadtreeBoxMap2DC = FQuadtreeBoxMap2DCs<true>;

template<bool IS_LOOSE_TREE = true>
using FQuadtreeBoxMap2fCs = OrthoTree::OrthoTreeContainer<FQuadtreeBoxMap2fs<IS_LOOSE_TREE>>;
using FQuadtreeBoxMap2fC = FQuadtreeBoxMap2fCs<true>;

template<bool IS_LOOSE_TREE = true>
using FOctreeBoxMapCs = OrthoTree::OrthoTreeContainer<FOctreeBoxsMap<IS_LOOSE_TREE>>;
using FOctreeBoxMapC = FOctreeBoxMapCs<true>;

template<bool IS_LOOSE_TREE = true>
using FOctreeBoxMap3dCs = OrthoTree::OrthoTreeContainer<FOctreeBoxMap3ds<IS_LOOSE_TREE>>;
using FOctreeBoxMap3dC = FOctreeBoxMap3dCs<true>;

template<bool IS_LOOSE_TREE = true>
using FOctreeBoxMap3fCs = OrthoTree::OrthoTreeContainer<FOctreeBoxMap3fs<IS_LOOSE_TREE>>;
using FOctreeBoxMap3fC = FOctreeBoxMap3fCs<true>;
