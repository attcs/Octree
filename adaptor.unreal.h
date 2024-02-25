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

#include "octree.h"

//#define UNREAL_DUMMY_TYPES
//#define UNREAL_DUMMY_TYPES__SUPPRESS_ASSERTS

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

    bool IsUnit(float LengthSquaredTolerance) const
    {
      NOT_IMPLEMENTED;
      return true;
    }

    static float Dist(FVector_ const& v1, FVector_ const& v2)
    {
      NOT_IMPLEMENTED;
      return {};
    }

    static float DistSquared(FVector_ const& v1, FVector_ const& v2)
    {
      NOT_IMPLEMENTED;
      return {};
    }

    static float DotProduct(FVector_ const& v1, FVector_ const& v2)
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
  struct FVectorTemplate2 : FVectorTemplate < FVectorTemplate2<geometry_type>>
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
    FBoxTemplate(FVector_ && Min, FVector_ && Max)
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


} // namespace UnrealDummyTypes
#else
#include "Math/Box.h"
#include "Math/Box2D.h"
#include "Math/Vector.h"
#include "Math/Vector2D.h"
#include "Math/Vector4.h"
#endif

#ifdef UNREAL_DUMMY_TYPES
using namespace UnrealDummyTypes;
#endif

namespace OrthoTree
{
  namespace UnrealAdaptor
  {
    template<typename geometry_type_, typename FVector2D_, typename FBox2D_>
    struct UnrealAdaptorBasics2D
    {
      static constexpr geometry_type_& point_comp(FVector2D_& pt, dim_type dimensionID)
      {
        switch (dimensionID)
        {
        case 0: return pt.X;
        case 1: return pt.Y;
        default: std::terminate();
        }
      }

      static constexpr geometry_type_ point_comp_c(FVector2D_ const& pt, dim_type dimensionID)
      {
        switch (dimensionID)
        {
        case 0: return pt.X;
        case 1: return pt.Y;
        default: std::terminate();
        }
      }

      static constexpr FVector2D_& box_min(FBox2D_& box) { return box.Min; }
      static constexpr FVector2D_& box_max(FBox2D_& box) { return box.Max; }
      static constexpr FVector2D_ const& box_min_c(FBox2D_ const& box) { return box.Min; }
      static constexpr FVector2D_ const& box_max_c(FBox2D_ const& box) { return box.Max; }
    };


    template<typename geometry_type_, typename FVector_, typename FBox_>
    struct UnrealAdaptorBasics3D
    {
      static constexpr geometry_type_& point_comp(FVector_& v, dim_type dimensionID)
      {
        switch (dimensionID)
        {
        case 0: return v.X;
        case 1: return v.Y;
        case 2: return v.Z;
        default: std::terminate();
        }
      }

      static constexpr geometry_type_ point_comp_c(FVector_ const& v, dim_type dimensionID)
      {
        switch (dimensionID)
        {
        case 0: return v.X;
        case 1: return v.Y;
        case 2: return v.Z;
        default: std::terminate();
        }
      }

      static constexpr FVector_& box_min(FBox_& box) { return box.Min; }
      static constexpr FVector_& box_max(FBox_& box) { return box.Max; }
      static constexpr FVector_ const& box_min_c(FBox_ const& box) { return box.Min; }
      static constexpr FVector_ const& box_max_c(FBox_ const& box) { return box.Max; }
    };

    template<int AmbientDim_, typename geometry_type_, typename FVector_, typename FBox_, typename UnrealAdaptorBasics_>
    struct UnrealAdaptorGeneral : UnrealAdaptorBasics_
    {
      using base = UnrealAdaptorBasics_;

      static_assert(AdaptorBasicsConcept<base, FVector_, FBox_, geometry_type_>);

      static constexpr geometry_type_ size2(FVector_ const& v) noexcept { return v.SizeSquared(); }

      static constexpr geometry_type_ size(FVector_ const& point) noexcept { return point.Size(); }

      static constexpr FVector_ add(FVector_ const& v1, FVector_ const& v2) noexcept { return v1 + v2; }

      static constexpr FVector_ subtract(FVector_ const& v1, FVector_ const& v2) noexcept { return v1 - v2; }

      static constexpr FVector_ multiply(FVector_ const& v, double scalarFactor) noexcept { return v * float(scalarFactor); }

      static constexpr geometry_type_ dot(FVector_ const& v1, FVector_ const& v2) noexcept { return FVector_::DotProduct(v1, v2); }

      static constexpr geometry_type_ distance(FVector_ const& v1, FVector_ const& v2) noexcept { return FVector_::Dist(v1, v2); }

      static constexpr geometry_type_ distance2(FVector_ const& v1, FVector_ const& v2) noexcept { return FVector_::DistSquared(v1, v2); }

      static constexpr bool are_points_equal(FVector_ const& v1, FVector_ const& v2, geometry_type_ tolerance) noexcept
      {
        return distance2(v1, v2) <= tolerance * tolerance;
      }

      static constexpr bool is_normalized_vector(FVector_ const& normal) noexcept { return normal.IsUnit(0.000001f); }

      static constexpr bool does_box_contain_point(FBox_ const& box, FVector_ const& point) noexcept { return box.IsInsideOrOn(point); }

      static constexpr bool does_box_contain_point_strict(FBox_ const& box, FVector_ const& point) noexcept { return box.IsInside(point); }


      static constexpr bool does_point_touch_box(FBox_ const& box, FVector_ const& point) noexcept
      {
        for (dim_type dimensionID = 0; dimensionID < AmbientDim_; ++dimensionID)
          if ((base::point_comp_c(base::box_min_c(box), dimensionID) == base::point_comp_c(point, dimensionID)))
            return false;

        return true;
      }

      static constexpr bool are_boxes_overlapped_strict(FBox_ const& e1, FBox_ const& e2) noexcept { return e1.Intersect(e2); }

      enum class EBoxRelation
      {
        Overlapped = -1,
        Adjecent = 0,
        Separated = 1
      };
      static constexpr EBoxRelation box_relation(FBox_ const& e1, FBox_ const& e2) noexcept
      {
        enum EBoxRelationCandidate : uint8_t
        {
          OverlappedC = 0x1,
          AdjecentC = 0x2,
          SeparatedC = 0x4
        };
        uint8_t rel = 0;
        for (dim_type dimensionID = 0; dimensionID < AmbientDim_; ++dimensionID)
        {
          if (
            base::point_comp_c(base::box_min_c(e1), dimensionID) < base::point_comp_c(base::box_max_c(e2), dimensionID) &&
            base::point_comp_c(base::box_max_c(e1), dimensionID) > base::point_comp_c(base::box_min_c(e2), dimensionID))
            rel |= EBoxRelationCandidate::OverlappedC;
          else if (
            base::point_comp_c(base::box_min_c(e1), dimensionID) == base::point_comp_c(base::box_max_c(e2), dimensionID) ||
            base::point_comp_c(base::box_max_c(e1), dimensionID) == base::point_comp_c(base::box_min_c(e2), dimensionID))
            rel |= EBoxRelationCandidate::AdjecentC;
          else if (
            base::point_comp_c(base::box_min_c(e1), dimensionID) > base::point_comp_c(base::box_max_c(e2), dimensionID) ||
            base::point_comp_c(base::box_max_c(e1), dimensionID) < base::point_comp_c(base::box_min_c(e2), dimensionID))
            return EBoxRelation::Separated;
        }
        return (rel & EBoxRelationCandidate::AdjecentC) == EBoxRelationCandidate::AdjecentC ? EBoxRelation::Adjecent : EBoxRelation::Overlapped;
      }

      static constexpr bool are_boxes_overlapped(FBox_ const& e1, FBox_ const& e2, bool e1_must_contain_e2 = true, bool fOverlapPtTouchAllowed = false) noexcept
      {
        if (e1_must_contain_e2)
        {
          return does_box_contain_point(e1, base::box_min_c(e2)) && does_box_contain_point(e1, base::box_max_c(e2));
        }
        else
        {
          autoc rel = box_relation(e1, e2);
          if (fOverlapPtTouchAllowed)
            return rel == EBoxRelation::Adjecent || rel == EBoxRelation::Overlapped;
          else
            return rel == EBoxRelation::Overlapped;
        }
      }

      static FBox_ box_of_points(std::span<FVector_ const> const& points) noexcept
      {
        auto ext = points.size() == 0 ? FBox_{} : FBox_(points[0], points[0]);
        for (autoc& point : points)
        {
          for (dim_type dimensionID = 0; dimensionID < AmbientDim_; ++dimensionID)
          {
            if (base::point_comp_c(base::box_min_c(ext), dimensionID) > base::point_comp_c(point, dimensionID))
              base::point_comp(base::box_min(ext), dimensionID) = base::point_comp_c(point, dimensionID);

            if (base::point_comp_c(base::box_max_c(ext), dimensionID) < base::point_comp_c(point, dimensionID))
              base::point_comp(base::box_max(ext), dimensionID) = base::point_comp_c(point, dimensionID);
          }
        }
        return ext;
      }

      static FBox_ box_of_boxes(std::span<FBox_ const> const& extents) noexcept
      {
        if (extents.size() == 0)
          return {};

        auto ext = extents[0];
        for (autoc& e : extents)
        {
          for (dim_type dimensionID = 0; dimensionID < AmbientDim_; ++dimensionID)
          {
            if (base::point_comp_c(base::box_min_c(ext), dimensionID) > base::point_comp_c(base::box_min_c(e), dimensionID))
              base::point_comp(base::box_min(ext), dimensionID) = base::point_comp_c(base::box_min_c(e), dimensionID);

            if (base::point_comp_c(base::box_max_c(ext), dimensionID) < base::point_comp_c(base::box_max_c(e), dimensionID))
              base::point_comp(base::box_max(ext), dimensionID) = base::point_comp_c(base::box_max_c(e), dimensionID);
          }
        }

        return ext;
      }

      static void move_box(FBox_& box, FVector_ const& moveVector) noexcept { box = box.ShiftBy(moveVector); }

      static constexpr std::optional<double> is_ray_hit(FBox_ const& box, FVector_ const& rayBasePoint, FVector_ const& rayHeading, geometry_type_ tolerance) noexcept
      {
        auto rayBasePointBox = FBox_(rayBasePoint, rayBasePoint);
        for (dim_type dimensionID = 0; dimensionID < AmbientDim_; ++dimensionID)
        {
          base::point_comp(base::box_min(rayBasePointBox), dimensionID) -= tolerance;
          base::point_comp(base::box_max(rayBasePointBox), dimensionID) += tolerance;
        }

        if (box.Intersect(rayBasePointBox))
          return 0.0;

        autoc& minBoxPoint = base::box_min_c(box);
        autoc& maxBoxPoint = base::box_max_c(box);

        autoce inf = std::numeric_limits<double>::infinity();

        auto minDistances = std::array<double, AmbientDim_>{};
        auto maxDistances = std::array<double, AmbientDim_>{};
        for (dim_type dimensionID = 0; dimensionID < AmbientDim_; ++dimensionID)
        {
          autoc hComp = base::point_comp_c(rayHeading, dimensionID);
          if (hComp == 0)
          {
            if (base::point_comp_c(maxBoxPoint, dimensionID) + tolerance < base::point_comp_c(rayBasePoint, dimensionID))
              return std::nullopt;

            if (base::point_comp_c(minBoxPoint, dimensionID) - tolerance > base::point_comp_c(rayBasePoint, dimensionID))
              return std::nullopt;

            minDistances[dimensionID] = -inf;
            maxDistances[dimensionID] = +inf;
            continue;
          }

          minDistances[dimensionID] =
            (base::point_comp_c(hComp > 0.0 ? minBoxPoint : maxBoxPoint, dimensionID) - tolerance - base::point_comp_c(rayBasePoint, dimensionID)) / hComp;
          maxDistances[dimensionID] =
            (base::point_comp_c(hComp < 0.0 ? minBoxPoint : maxBoxPoint, dimensionID) + tolerance - base::point_comp_c(rayBasePoint, dimensionID)) / hComp;
        }

        autoc rMin = *std::ranges::max_element(minDistances);
        autoc rMax = *std::ranges::min_element(maxDistances);
        if (rMin > rMax || rMax < 0.0)
          return std::nullopt;

        return rMin < 0 ? rMax : rMin;
      }


      // Plane intersection (Plane equation: dotProduct(planeNormal, point) = distanceOfOrigo)
      static constexpr bool does_plane_intersect(FBox_ const& box, geometry_type_ distanceOfOrigo, FVector_ const& planeNormal, geometry_type_ tolerance) noexcept
      {
        assert(is_normalized_vector(planeNormal));

        autoc& minPoint = base::box_min_c(box);
        autoc& maxPoint = base::box_max_c(box);

        autoc center = multiply(add(minPoint, maxPoint), 0.5);
        autoc radius = subtract(maxPoint, center);

        double radiusProjected = 0.0;
        for (dim_type dimensionID = 0; dimensionID < AmbientDim_; ++dimensionID)
          radiusProjected += base::point_comp_c(radius, dimensionID) * abs(base::point_comp_c(planeNormal, dimensionID));

        autoc centerProjected = dot(planeNormal, center);

        return abs(centerProjected - distanceOfOrigo) <= radiusProjected + tolerance;
      }
    };


    template<typename geometry_type_, typename FVector2D_, typename FBox2D_>
    using UnrealAdaptorGeneral2D =
      UnrealAdaptorGeneral<2, geometry_type_, FVector2D_, FBox2D_, UnrealAdaptorBasics2D<geometry_type_, FVector2D_, FBox2D_>>;

    template<typename geometry_type_, typename FVector_, typename FBox_>
    using UnrealAdaptorGeneral3D = UnrealAdaptorGeneral<3, geometry_type_, FVector_, FBox_, UnrealAdaptorBasics3D<geometry_type_, FVector_, FBox_>>;


    // Templates for point types

    template<typename geometry_type_, typename FVector_, typename FBox_>
    using QuadtreePointTemplate = OrthoTreePoint<2, FVector_, FBox_, UnrealAdaptorGeneral2D<geometry_type_, FVector_, FBox_>, geometry_type_>;

    template<typename geometry_type_, typename FVector_, typename FBox_>
    using OctreePointTemplate = OrthoTreePoint<3, FVector_, FBox_, UnrealAdaptorGeneral3D<geometry_type_, FVector_, FBox_>, geometry_type_>;


    // Templates for box types

    template<typename geometry_type_, typename FVector_, typename FBox_, uint32_t t_AdditionalDepthOfSplitStrategy = 2>
    using QuadtreeBoxTemplate =
      OrthoTreeBoundingBox<2, FVector_, FBox_, UnrealAdaptorGeneral2D<geometry_type_, FVector_, FBox_>, geometry_type_, t_AdditionalDepthOfSplitStrategy>;

    template<typename geometry_type_, typename FVector_, typename FBox_, uint32_t t_AdditionalDepthOfSplitStrategy = 2>
    using OctreeBoxTemplate =
      OrthoTreeBoundingBox<3, FVector_, FBox_, UnrealAdaptorGeneral3D<geometry_type_, FVector_, FBox_>, geometry_type_, t_AdditionalDepthOfSplitStrategy>;
  } // namespace UnrealAdaptor
} // namespace OrthoTree


// Orthotree Core Types

using FQuadtreePoint = OrthoTree::UnrealAdaptor::QuadtreePointTemplate<FLargeWorldCoordinatesReal, FVector2D, FBox2D>;
using FQuadtreePoint2D = FQuadtreePoint;
using FQuadtreePoint2f = OrthoTree::UnrealAdaptor::QuadtreePointTemplate<float, FVector2f, FBox2f>;

using FOctreePoint = OrthoTree::UnrealAdaptor::OctreePointTemplate<FLargeWorldCoordinatesReal, FVector, FBox>;
using FOctreePoint3d = OrthoTree::UnrealAdaptor::OctreePointTemplate<double, FVector3d, FBox3d>;
using FOctreePoint3f = OrthoTree::UnrealAdaptor::OctreePointTemplate<float, FVector3f, FBox3f>;


template<uint32_t t_AdditionalDepthOfSplitStrategy = 2>
using FQuadtreeBox = OrthoTree::UnrealAdaptor::QuadtreeBoxTemplate<FLargeWorldCoordinatesReal, FVector2D, FBox2D, t_AdditionalDepthOfSplitStrategy>;
template<uint32_t t_AdditionalDepthOfSplitStrategy = 2>
using FQuadtreeBox2D = FQuadtreeBox<t_AdditionalDepthOfSplitStrategy>;
template<uint32_t t_AdditionalDepthOfSplitStrategy = 2>
using FQuadtreeBox2f = OrthoTree::UnrealAdaptor::QuadtreeBoxTemplate<float, FVector2f, FBox2f, t_AdditionalDepthOfSplitStrategy>;

template<uint32_t t_AdditionalDepthOfSplitStrategy = 2>
using FOctreeBox = OrthoTree::UnrealAdaptor::OctreeBoxTemplate<FLargeWorldCoordinatesReal, FVector, FBox, t_AdditionalDepthOfSplitStrategy>;
template<uint32_t t_AdditionalDepthOfSplitStrategy = 2>
using FOctreeBox3d = OrthoTree::UnrealAdaptor::OctreeBoxTemplate<double, FVector3d, FBox3d, t_AdditionalDepthOfSplitStrategy>;
template<uint32_t t_AdditionalDepthOfSplitStrategy = 2>
using FOctreeBox3f = OrthoTree::UnrealAdaptor::OctreeBoxTemplate<float, FVector3f, FBox3f, t_AdditionalDepthOfSplitStrategy>;


// Orthotree Container Types

using FQuadtreePointC = OrthoTree::OrthoTreeContainerPoint<FQuadtreePoint, FVector2D>;
using FQuadtreePoint2DC = FQuadtreePointC;
using FQuadtreePoint2fC = OrthoTree::OrthoTreeContainerPoint<FQuadtreePoint2f, FVector2f>;

using FOctreePointC = OrthoTree::OrthoTreeContainerPoint<FOctreePoint, FVector>;
using FOctreePoint3dC = OrthoTree::OrthoTreeContainerPoint<FOctreePoint3d, FVector3d>;
using FOctreePoint3fC = OrthoTree::OrthoTreeContainerPoint<FOctreePoint3f, FVector3f>;

template<uint32_t t_AdditionalDepthOfSplitStrategy = 2>
using FQuadtreeBoxC = OrthoTree::OrthoTreeContainerBox<FQuadtreeBox<t_AdditionalDepthOfSplitStrategy>, FBox2D>;
template<uint32_t t_AdditionalDepthOfSplitStrategy = 2>
using FQuadtreeBox2DC = OrthoTree::OrthoTreeContainerBox<FQuadtreeBox2D<t_AdditionalDepthOfSplitStrategy>, FBox2D>;
template<uint32_t t_AdditionalDepthOfSplitStrategy = 2>
using FQuadtreeBox2fC = OrthoTree::OrthoTreeContainerBox<FQuadtreeBox2f<t_AdditionalDepthOfSplitStrategy>, FBox2f>;

template<uint32_t t_AdditionalDepthOfSplitStrategy = 2>
using FOctreeBoxC = OrthoTree::OrthoTreeContainerBox<FOctreeBox<t_AdditionalDepthOfSplitStrategy>, FBox>;
template<uint32_t t_AdditionalDepthOfSplitStrategy = 2>
using FOctreeBox3dC = OrthoTree::OrthoTreeContainerBox<FOctreeBox3d<t_AdditionalDepthOfSplitStrategy>, FBox3d>;
template<uint32_t t_AdditionalDepthOfSplitStrategy = 2>
using FOctreeBox3fC = OrthoTree::OrthoTreeContainerBox<FOctreeBox3f<t_AdditionalDepthOfSplitStrategy>, FBox3f>;
