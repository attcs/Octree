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
  namespace UnrealAdaptor
  {
    template<typename FVector2D_>
    struct FRay2DTemplate
    {
      FVector2D_ Direction;
      FVector2D_ Origin;
    };

    template<typename FGeometry_, typename FVector2D_, typename FBox2D_>
    struct UnrealAdaptorBasics2D
    {
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
    struct UnrealAdaptorBasics3D
    {
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

      static constexpr FGeometry_ GetBoxMinC(FBox_ const& box, dim_t dimensionID) { return GetPointC(box.Min, dimensionID); }
      static constexpr FGeometry_ GetBoxMaxC(FBox_ const& box, dim_t dimensionID) { return GetPointC(box.Max, dimensionID); }
      static constexpr void SetBoxMinC(FBox_& box, dim_t dimensionID, FGeometry_ value) { SetPointC(box.Min, dimensionID, value); }
      static constexpr void SetBoxMaxC(FBox_& box, dim_t dimensionID, FGeometry_ value) { SetPointC(box.Max, dimensionID, value); }

      static constexpr FVector_ const& GetRayDirection(FRay_ const& ray) noexcept { return ray.Direction; }
      static constexpr FVector_ const& GetRayOrigin(FRay_ const& ray) noexcept { return ray.Origin; }

      static constexpr FVector_ const& GetPlaneNormal(FPlane_ const& plane) noexcept { return plane.GetNormal(); }
      static constexpr FGeometry_ GetPlaneOrigoDistance(FPlane_ const& plane) noexcept { return FGeometry_(plane.W); }
    };

    template<int AmbientDim_, typename FGeometry_, typename FVector_, typename FBox_, typename FRay_, typename FPlane_, typename UnrealAdaptorBasics_>
    struct UnrealAdaptorGeneral : UnrealAdaptorBasics_
    {
      using Base = UnrealAdaptorBasics_;

      static_assert(AdaptorBasicsConcept<Base, FVector_, FBox_, FRay_, FPlane_, FGeometry_>);

      static constexpr FGeometry_ Size2(FVector_ const& v) noexcept { return v.SizeSquared(); }

      static constexpr FGeometry_ Size(FVector_ const& point) noexcept { return point.Size(); }

      static constexpr FVector_ Add(FVector_ const& v1, FVector_ const& v2) noexcept { return v1 + v2; }

      static constexpr FVector_ Subtract(FVector_ const& v1, FVector_ const& v2) noexcept { return v1 - v2; }

      static constexpr FVector_ Multiply(FVector_ const& v, double scalarFactor) noexcept { return v * float(scalarFactor); }

      static constexpr FGeometry_ Dot(FVector_ const& v1, FVector_ const& v2) noexcept { return FVector_::DotProduct(v1, v2); }

      static constexpr FGeometry_ Distance(FVector_ const& v1, FVector_ const& v2) noexcept { return FVector_::Dist(v1, v2); }

      static constexpr FGeometry_ Distance2(FVector_ const& v1, FVector_ const& v2) noexcept { return FVector_::DistSquared(v1, v2); }

      static constexpr bool ArePointsEqual(FVector_ const& v1, FVector_ const& v2, FGeometry_ tolerance) noexcept
      {
        return Distance2(v1, v2) <= tolerance * tolerance;
      }

      static constexpr bool IsNormalizedVector(FVector_ const& normal) noexcept { return normal.IsUnit(0.000001f); }

      static constexpr bool DoesBoxContainPoint(FBox_ const& box, FVector_ const& point) noexcept { return box.IsInsideOrOn(point); }

      static constexpr bool DoesBoxContainPointStrict(FBox_ const& box, FVector_ const& point) noexcept { return box.IsInside(point); }

      static constexpr bool AreBoxesOverlappedStrict(FBox_ const& e1, FBox_ const& e2) noexcept { return e1.Intersect(e2); }

      enum class EBoxRelation
      {
        Overlapped = -1,
        Adjecent = 0,
        Separated = 1
      };
      static constexpr EBoxRelation GetBoxRelation(FBox_ const& e1, FBox_ const& e2) noexcept
      {
        enum EBoxRelationCandidate : uint8_t
        {
          OverlappedC = 0x1,
          AdjecentC = 0x2,
          SeparatedC = 0x4
        };
        uint8_t rel = 0;
        for (dim_t dimensionID = 0; dimensionID < AmbientDim_; ++dimensionID)
        {
          if (Base::GetBoxMinC(e1, dimensionID) < Base::GetBoxMaxC(e2, dimensionID) && Base::GetBoxMaxC(e1, dimensionID) > Base::GetBoxMinC(e2, dimensionID))
            rel |= EBoxRelationCandidate::OverlappedC;
          else if (Base::GetBoxMinC(e1, dimensionID) == Base::GetBoxMaxC(e2, dimensionID) || Base::GetBoxMaxC(e1, dimensionID) == Base::GetBoxMinC(e2, dimensionID))
            rel |= EBoxRelationCandidate::AdjecentC;
          else if (Base::GetBoxMinC(e1, dimensionID) > Base::GetBoxMaxC(e2, dimensionID) || Base::GetBoxMaxC(e1, dimensionID) < Base::GetBoxMinC(e2, dimensionID))
            return EBoxRelation::Separated;
        }
        return (rel & EBoxRelationCandidate::AdjecentC) == EBoxRelationCandidate::AdjecentC ? EBoxRelation::Adjecent : EBoxRelation::Overlapped;
      }

      static constexpr bool AreBoxesOverlapped(FBox_ const& e1, FBox_ const& e2, bool e1_must_contain_e2 = true, bool fOverlapPtTouchAllowed = false) noexcept
      {
        if (e1_must_contain_e2)
        {
          for (dim_t dimensionID = 0; dimensionID < AmbientDim_; ++dimensionID)
          {
            if (Base::GetBoxMinC(e1, dimensionID) > Base::GetBoxMinC(e2, dimensionID) || Base::GetBoxMinC(e2, dimensionID) > Base::GetBoxMaxC(e1, dimensionID))
              return false;

            if (Base::GetBoxMinC(e1, dimensionID) > Base::GetBoxMaxC(e2, dimensionID) || Base::GetBoxMaxC(e2, dimensionID) > Base::GetBoxMaxC(e1, dimensionID))
              return false;
          }
          return true;
        }
        else
        {
          auto const rel = GetBoxRelation(e1, e2);
          if (fOverlapPtTouchAllowed)
            return rel == EBoxRelation::Adjecent || rel == EBoxRelation::Overlapped;
          else
            return rel == EBoxRelation::Overlapped;
        }
      }

      static FBox_ GetBoxOfPoints(std::span<FVector_ const> const& points) noexcept
      {
        auto ext = points.size() == 0 ? FBox_{} : FBox_(points[0], points[0]);
        for (auto const& point : points)
        {
          for (dim_t dimensionID = 0; dimensionID < AmbientDim_; ++dimensionID)
          {
            if (Base::GetBoxMinC(ext, dimensionID) > Base::GetPointC(point, dimensionID))
              Base::SetBoxMinC(ext, dimensionID, Base::GetPointC(point, dimensionID));

            if (Base::GetBoxMaxC(ext, dimensionID) < Base::GetPointC(point, dimensionID))
              Base::SetBoxMaxC(ext, dimensionID, Base::GetPointC(point, dimensionID));
          }
        }
        return ext;
      }

      static FBox_ GetBoxOfBoxes(std::span<FBox_ const> const& extents) noexcept
      {
        if (extents.size() == 0)
          return {};

        auto ext = extents[0];
        for (auto const& e : extents)
        {
          for (dim_t dimensionID = 0; dimensionID < AmbientDim_; ++dimensionID)
          {
            if (Base::GetBoxMinC(ext, dimensionID) > Base::GetBoxMinC(e, dimensionID))
              Base::SetBoxMinC(ext, dimensionID, Base::GetBoxMinC(e, dimensionID));

            if (Base::GetBoxMaxC(ext, dimensionID) < Base::GetBoxMaxC(e, dimensionID))
              Base::SetBoxMaxC(ext, dimensionID, Base::GetBoxMaxC(e, dimensionID));
          }
        }

        return ext;
      }

      static void MoveBox(FBox_& box, FVector_ const& moveVector) noexcept { box = box.ShiftBy(moveVector); }

      static constexpr std::optional<double> GetRayBoxDistance(
        FBox_ const& box, FVector_ const& rayBasePoint, FVector_ const& rayHeading, FGeometry_ tolerance) noexcept
      {
        auto rayBasePointBox = FBox_();
        for (dim_t dimensionID = 0; dimensionID < AmbientDim_; ++dimensionID)
        {
          Base::SetBoxMinC(rayBasePointBox, dimensionID, Base::GetPointC(rayBasePoint, dimensionID) - tolerance);
          Base::SetBoxMaxC(rayBasePointBox, dimensionID, Base::GetPointC(rayBasePoint, dimensionID) + tolerance);
        }

        if (box.Intersect(rayBasePointBox))
          return 0.0;

        auto constexpr inf = std::numeric_limits<double>::max();

        auto minBoxDistances = std::array<double, AmbientDim_>{};
        auto maxBoxDistances = std::array<double, AmbientDim_>{};
        for (dim_t dimensionID = 0; dimensionID < AmbientDim_; ++dimensionID)
        {
          auto const dirComp = Base::GetPointC(rayHeading, dimensionID);
          if (dirComp == 0)
          {
            if (tolerance != 0.0)
            {
              // Box should be within tolerance (<, not <=)

              assert(tolerance > 0);
              if (Base::GetBoxMaxC(box, dimensionID) + tolerance <= Base::GetPointC(rayBasePoint, dimensionID))
                return std::nullopt;

              if (Base::GetBoxMinC(box, dimensionID) - tolerance >= Base::GetPointC(rayBasePoint, dimensionID))
                return std::nullopt;
            }
            else
            {
              if (Base::GetBoxMaxC(box, dimensionID) < Base::GetPointC(rayBasePoint, dimensionID))
                return std::nullopt;

              if (Base::GetBoxMinC(box, dimensionID) > Base::GetPointC(rayBasePoint, dimensionID))
                return std::nullopt;
            }

            minBoxDistances[dimensionID] = -inf;
            maxBoxDistances[dimensionID] = +inf;
          }
          else
          {
            auto const minBox = Base::GetBoxMinC(box, dimensionID) - tolerance;
            auto const maxBox = Base::GetBoxMaxC(box, dimensionID) + tolerance;
            auto const pointComp = Base::GetPointC(rayBasePoint, dimensionID);
            auto const dirCompRecip = 1.0 / dirComp;
            if (dirComp < 0.0)
            {
              minBoxDistances[dimensionID] = (maxBox - pointComp) * dirCompRecip;
              maxBoxDistances[dimensionID] = (minBox - pointComp) * dirCompRecip;
            }
            else
            {
              minBoxDistances[dimensionID] = (minBox - pointComp) * dirCompRecip;
              maxBoxDistances[dimensionID] = (maxBox - pointComp) * dirCompRecip;
            }
          }
        }

        auto const minBoxDistance = *std::ranges::max_element(minBoxDistances);
        auto const maxBoxDistance = *std::ranges::min_element(maxBoxDistances);
        if (minBoxDistance > maxBoxDistance || maxBoxDistance < 0.0)
          return std::nullopt;
        else
          return minBoxDistance < 0 ? maxBoxDistance : minBoxDistance;
      }

      static constexpr std::optional<double> GetRayBoxDistance(FBox_ const& box, FRay_ const& ray, FGeometry_ tolerance) noexcept
      {
        return GetRayBoxDistance(box, Base::GetRayOrigin(ray), Base::GetRayDirection(ray), tolerance);
      }

      // Get point-Hyperplane relation (Plane equation: dotProduct(planeNormal, point) = distanceOfOrigo)
      static constexpr PlaneRelation GetPointPlaneRelation(
        FVector_ const& point, FGeometry_ distanceOfOrigo, FVector_ const& planeNormal, FGeometry_ tolerance) noexcept
      {
        assert(IsNormalizedVector(planeNormal));

        auto const pointProjected = Dot(planeNormal, point);

        if (pointProjected < distanceOfOrigo - tolerance)
          return PlaneRelation::Negative;

        if (pointProjected > distanceOfOrigo + tolerance)
          return PlaneRelation::Positive;

        return PlaneRelation::Hit;
      }

      // Get box-Hyperplane relation (Plane equation: dotProduct(planeNormal, point) = distanceOfOrigo)
      static constexpr PlaneRelation GetBoxPlaneRelation(FBox_ const& box, FGeometry_ distanceOfOrigo, FVector_ const& planeNormal, FGeometry_ tolerance) noexcept
      {
        assert(IsNormalizedVector(planeNormal));

        FVector_ center, radius;
        for (dim_t dimensionID = 0; dimensionID < AmbientDim_; ++dimensionID)
        {
          auto const minComponent = Base::GetBoxMinC(box, dimensionID);
          auto const maxComponent = Base::GetBoxMaxC(box, dimensionID);
          auto const centerComponent = static_cast<FGeometry_>((minComponent + maxComponent) * 0.5);
          Base::SetPointC(center, dimensionID, centerComponent);
          Base::SetPointC(radius, dimensionID, centerComponent - minComponent);
        }

        double radiusProjected = 0.0;
        for (dim_t dimensionID = 0; dimensionID < AmbientDim_; ++dimensionID)
          radiusProjected += Base::GetPointC(radius, dimensionID) * std::abs(Base::GetPointC(planeNormal, dimensionID));

        auto const centerProjected = Dot(planeNormal, center);

        if (centerProjected - radiusProjected < distanceOfOrigo - tolerance)
          return PlaneRelation::Negative;

        if (centerProjected + radiusProjected > distanceOfOrigo + tolerance)
          return PlaneRelation::Positive;

        return PlaneRelation::Hit;
      }
    };


    template<typename FGeometry_, typename FVector2D_, typename FBox2D_>
    using UnrealAdaptorGeneral2D = UnrealAdaptorGeneral<
      2,
      FGeometry_,
      FVector2D_,
      FBox2D_,
      typename UnrealAdaptorBasics2D<FGeometry_, FVector2D_, FBox2D_>::FRay2D_,
      typename UnrealAdaptorBasics2D<FGeometry_, FVector2D_, FBox2D_>::FPlane2D_,
      UnrealAdaptorBasics2D<FGeometry_, FVector2D_, FBox2D_>>;

    template<typename FGeometry_, typename FVector_, typename FBox_, typename FRay_, typename FPlane_>
    using UnrealAdaptorGeneral3D =
      UnrealAdaptorGeneral<3, FGeometry_, FVector_, FBox_, FRay_, FPlane_, UnrealAdaptorBasics3D<FGeometry_, FVector_, FBox_, FRay_, FPlane_>>;


    // Templates for point types

    template<typename FGeometry_, typename FVector_, typename FBox_, typename TContainer_ = std::span<FVector_ const>>
    using QuadtreePointTemplate = OrthoTreePoint<
      2,
      FVector_,
      FBox_,
      typename UnrealAdaptorBasics2D<FGeometry_, FVector_, FBox_>::FRay2D_,
      typename UnrealAdaptorBasics2D<FGeometry_, FVector_, FBox_>::FPlane2D_,
      FGeometry_,
      UnrealAdaptorGeneral2D<FGeometry_, FVector_, FBox_>,
      TContainer_>;

    template<typename FGeometry_, typename FVector_, typename FBox_, typename FRay_, typename FPlane_, typename TContainer_ = std::span<FVector_ const>>
    using OctreePointTemplate =
      OrthoTreePoint<3, FVector_, FBox_, FRay_, FPlane_, FGeometry_, UnrealAdaptorGeneral3D<FGeometry_, FVector_, FBox_, FRay_, FPlane_>, TContainer_>;


    // Templates for box types

    template<typename FGeometry_, typename FVector_, typename FBox_, bool DO_SPLIT_PARENT_ENTITIES = true, typename TContainer_ = std::span<FBox_ const>>
    using QuadtreeBoxTemplate = OrthoTreeBoundingBox<
      2,
      FVector_,
      FBox_,
      typename UnrealAdaptorBasics2D<FGeometry_, FVector_, FBox_>::FRay2D_,
      typename UnrealAdaptorBasics2D<FGeometry_, FVector_, FBox_>::FPlane2D_,
      FGeometry_,
      DO_SPLIT_PARENT_ENTITIES,
      UnrealAdaptorGeneral2D<FGeometry_, FVector_, FBox_>,
      TContainer_>;

    template<typename FGeometry_, typename FVector_, typename FBox_, typename FRay_, typename FPlane_, bool DO_SPLIT_PARENT_ENTITIES = true, typename TContainer_ = std::span<FBox_ const>>
    using OctreeBoxTemplate =
      OrthoTreeBoundingBox<3, FVector_, FBox_, FRay_, FPlane_, FGeometry_, DO_SPLIT_PARENT_ENTITIES, UnrealAdaptorGeneral3D<FGeometry_, FVector_, FBox_, FPlane_, FGeometry_>, TContainer_>;
  } // namespace UnrealAdaptor
} // namespace OrthoTree


// Orthotree Core Types
using FLargeWorldCoordinatesReal = double;

using FQuadtreePoint = OrthoTree::UnrealAdaptor::QuadtreePointTemplate<FLargeWorldCoordinatesReal, FVector2D, FBox2D>;
using FQuadtreePoint2D = FQuadtreePoint;
using FQuadtreePoint2f = OrthoTree::UnrealAdaptor::QuadtreePointTemplate<float, FVector2f, FBox2f>;

using FOctreePoint = OrthoTree::UnrealAdaptor::OctreePointTemplate<FLargeWorldCoordinatesReal, FVector, FBox, FRay, FPlane>;
using FOctreePoint3d = OrthoTree::UnrealAdaptor::OctreePointTemplate<double, FVector3d, FBox3d, FRay, FPlane>;
using FOctreePoint3f = OrthoTree::UnrealAdaptor::OctreePointTemplate<float, FVector3f, FBox3f, FRay, FPlane>;


template<bool DO_SPLIT_PARENT_ENTITIES = true>
using FQuadtreeBoxs = OrthoTree::UnrealAdaptor::QuadtreeBoxTemplate<FLargeWorldCoordinatesReal, FVector2D, FBox2D, DO_SPLIT_PARENT_ENTITIES>;
using FQuadtreeBox = FQuadtreeBoxs<true>;

template<bool DO_SPLIT_PARENT_ENTITIES = true>
using FQuadtreeBox2Ds = FQuadtreeBoxs<DO_SPLIT_PARENT_ENTITIES>;
using FQuadtreeBox2D = FQuadtreeBox;

template<bool DO_SPLIT_PARENT_ENTITIES = true>
using FQuadtreeBox2fs = OrthoTree::UnrealAdaptor::QuadtreeBoxTemplate<float, FVector2f, FBox2f, DO_SPLIT_PARENT_ENTITIES>;
using FQuadtreeBox2f = OrthoTree::UnrealAdaptor::QuadtreeBoxTemplate<float, FVector2f, FBox2f, true>;

template<bool DO_SPLIT_PARENT_ENTITIES = true>
using FOctreeBoxs = OrthoTree::UnrealAdaptor::OctreeBoxTemplate<FLargeWorldCoordinatesReal, FVector, FBox, FRay, FPlane, DO_SPLIT_PARENT_ENTITIES>;
using FOctreeBox = OrthoTree::UnrealAdaptor::OctreeBoxTemplate<FLargeWorldCoordinatesReal, FVector, FBox, FRay, FPlane, true>;

template<bool DO_SPLIT_PARENT_ENTITIES = true>
using FOctreeBox3ds = OrthoTree::UnrealAdaptor::OctreeBoxTemplate<double, FVector3d, FBox3d, FRay, FPlane, DO_SPLIT_PARENT_ENTITIES>;
using FOctreeBox3d = OrthoTree::UnrealAdaptor::OctreeBoxTemplate<double, FVector3d, FBox3d, FRay, FPlane, true>;

template<bool DO_SPLIT_PARENT_ENTITIES = true>
using FOctreeBox3fs = OrthoTree::UnrealAdaptor::OctreeBoxTemplate<float, FVector3f, FBox3f, FRay, FPlane, DO_SPLIT_PARENT_ENTITIES>;
using FOctreeBox3f = OrthoTree::UnrealAdaptor::OctreeBoxTemplate<float, FVector3f, FBox3f, FRay, FPlane, true>;


// Orthotree Container Types

using FQuadtreePointC = OrthoTree::OrthoTreeContainerPoint<FQuadtreePoint>;
using FQuadtreePoint2DC = FQuadtreePointC;
using FQuadtreePoint2fC = OrthoTree::OrthoTreeContainerPoint<FQuadtreePoint2f>;

using FOctreePointC = OrthoTree::OrthoTreeContainerPoint<FOctreePoint>;
using FOctreePoint3dC = OrthoTree::OrthoTreeContainerPoint<FOctreePoint3d>;
using FOctreePoint3fC = OrthoTree::OrthoTreeContainerPoint<FOctreePoint3f>;

template<bool DO_SPLIT_PARENT_ENTITIES = true>
using FQuadtreeBoxCs = OrthoTree::OrthoTreeContainerBox<FQuadtreeBoxs<DO_SPLIT_PARENT_ENTITIES>>;
using FQuadtreeBoxC = FQuadtreeBoxCs<true>;

template<bool DO_SPLIT_PARENT_ENTITIES = true>
using FQuadtreeBox2DCs = OrthoTree::OrthoTreeContainerBox<FQuadtreeBox2Ds<DO_SPLIT_PARENT_ENTITIES>>;
using FQuadtreeBox2DC = FQuadtreeBox2DCs<true>;

template<bool DO_SPLIT_PARENT_ENTITIES = true>
using FQuadtreeBox2fCs = OrthoTree::OrthoTreeContainerBox<FQuadtreeBox2fs<DO_SPLIT_PARENT_ENTITIES>>;
using FQuadtreeBox2fC = FQuadtreeBox2fCs<true>;

template<bool DO_SPLIT_PARENT_ENTITIES = true>
using FOctreeBoxCs = OrthoTree::OrthoTreeContainerBox<FOctreeBoxs<DO_SPLIT_PARENT_ENTITIES>>;
using FOctreeBoxC = FOctreeBoxCs<true>;

template<bool DO_SPLIT_PARENT_ENTITIES = true>
using FOctreeBox3dCs = OrthoTree::OrthoTreeContainerBox<FOctreeBox3ds<DO_SPLIT_PARENT_ENTITIES>>;
using FOctreeBox3dC = FOctreeBox3dCs<true>;

template<bool DO_SPLIT_PARENT_ENTITIES = true>
using FOctreeBox3fCs = OrthoTree::OrthoTreeContainerBox<FOctreeBox3fs<DO_SPLIT_PARENT_ENTITIES>>;
using FOctreeBox3fC = FOctreeBox3fCs<true>;


template<typename T>
using FContainer = std::unordered_map<int, T>;

using FQuadtreePointMap = OrthoTree::UnrealAdaptor::QuadtreePointTemplate<FLargeWorldCoordinatesReal, FVector2D, FBox2D, FContainer<FVector2D>>;
using FQuadtreePointMap2D = FQuadtreePointMap;
using FQuadtreePointMap2f = OrthoTree::UnrealAdaptor::QuadtreePointTemplate<float, FVector2f, FBox2f, FContainer<FVector2f>>;

using FOctreePointMap = OrthoTree::UnrealAdaptor::OctreePointTemplate<FLargeWorldCoordinatesReal, FVector, FBox, FRay, FPlane, FContainer<FVector>>;
using FOctreePointMap3d = OrthoTree::UnrealAdaptor::OctreePointTemplate<double, FVector3d, FBox3d, FRay, FPlane, FContainer<FVector3d>>;
using FOctreePointMap3f = OrthoTree::UnrealAdaptor::OctreePointTemplate<float, FVector3f, FBox3f, FRay, FPlane, FContainer<FVector3f>>;


template<bool DO_SPLIT_PARENT_ENTITIES = true>
using FQuadtreeBoxsMap =
  OrthoTree::UnrealAdaptor::QuadtreeBoxTemplate<FLargeWorldCoordinatesReal, FVector2D, FBox2D, DO_SPLIT_PARENT_ENTITIES, FContainer<FBox2D>>;
using FQuadtreeBoxMap = FQuadtreeBoxsMap<true>;

template<bool DO_SPLIT_PARENT_ENTITIES = true>
using FQuadtreeBoxMap2Ds = FQuadtreeBoxsMap<DO_SPLIT_PARENT_ENTITIES>;
using FQuadtreeBoxMap2D = FQuadtreeBoxMap;

template<bool DO_SPLIT_PARENT_ENTITIES = true>
using FQuadtreeBoxMap2fs = OrthoTree::UnrealAdaptor::QuadtreeBoxTemplate<float, FVector2f, FBox2f, DO_SPLIT_PARENT_ENTITIES, FContainer<FBox2f>>;
using FQuadtreeBoxMap2f = OrthoTree::UnrealAdaptor::QuadtreeBoxTemplate<float, FVector2f, FBox2f, true, FContainer<FBox2f>>;

template<bool DO_SPLIT_PARENT_ENTITIES = true>
using FOctreeBoxsMap =
  OrthoTree::UnrealAdaptor::OctreeBoxTemplate<FLargeWorldCoordinatesReal, FVector, FBox, FRay, FPlane, DO_SPLIT_PARENT_ENTITIES, FContainer<FBox>>;
using FOctreeBoxMap = OrthoTree::UnrealAdaptor::OctreeBoxTemplate<FLargeWorldCoordinatesReal, FVector, FBox, FRay, FPlane, true, FContainer<FBox>>;

template<bool DO_SPLIT_PARENT_ENTITIES = true>
using FOctreeBoxMap3ds =
  OrthoTree::UnrealAdaptor::OctreeBoxTemplate<double, FVector3d, FBox3d, FRay, FPlane, DO_SPLIT_PARENT_ENTITIES, FContainer<FBox3d>>;
using FOctreeBoxMap3d = OrthoTree::UnrealAdaptor::OctreeBoxTemplate<double, FVector3d, FBox3d, FRay, FPlane, true, FContainer<FBox3d>>;

template<bool DO_SPLIT_PARENT_ENTITIES = true>
using FOctreeBoxMap3fs =
  OrthoTree::UnrealAdaptor::OctreeBoxTemplate<float, FVector3f, FBox3f, FRay, FPlane, DO_SPLIT_PARENT_ENTITIES, FContainer<FBox3f>>;
using FOctreeBoxMap3f = OrthoTree::UnrealAdaptor::OctreeBoxTemplate<float, FVector3f, FBox3f, FRay, FPlane, true, FContainer<FBox3f>>;


using FQuadtreePointMapC = OrthoTree::OrthoTreeContainerPoint<FQuadtreePointMap>;
using FQuadtreePointMap2DC = FQuadtreePointMapC;
using FQuadtreePointMap2fC = OrthoTree::OrthoTreeContainerPoint<FQuadtreePointMap2f>;

using FOctreePointMapC = OrthoTree::OrthoTreeContainerPoint<FOctreePointMap>;
using FOctreePointMap3dC = OrthoTree::OrthoTreeContainerPoint<FOctreePointMap3d>;
using FOctreePointMap3fC = OrthoTree::OrthoTreeContainerPoint<FOctreePointMap3f>;

template<bool DO_SPLIT_PARENT_ENTITIES = true>
using FQuadtreeBoxMapCs = OrthoTree::OrthoTreeContainerBox<FQuadtreeBoxsMap<DO_SPLIT_PARENT_ENTITIES>>;
using FQuadtreeBoxMapC = FQuadtreeBoxMapCs<true>;

template<bool DO_SPLIT_PARENT_ENTITIES = true>
using FQuadtreeBoxMap2DCs = OrthoTree::OrthoTreeContainerBox<FQuadtreeBoxMap2Ds<DO_SPLIT_PARENT_ENTITIES>>;
using FQuadtreeBoxMap2DC = FQuadtreeBoxMap2DCs<true>;

template<bool DO_SPLIT_PARENT_ENTITIES = true>
using FQuadtreeBoxMap2fCs = OrthoTree::OrthoTreeContainerBox<FQuadtreeBoxMap2fs<DO_SPLIT_PARENT_ENTITIES>>;
using FQuadtreeBoxMap2fC = FQuadtreeBoxMap2fCs<true>;

template<bool DO_SPLIT_PARENT_ENTITIES = true>
using FOctreeBoxMapCs = OrthoTree::OrthoTreeContainerBox<FOctreeBoxsMap<DO_SPLIT_PARENT_ENTITIES>>;
using FOctreeBoxMapC = FOctreeBoxMapCs<true>;

template<bool DO_SPLIT_PARENT_ENTITIES = true>
using FOctreeBoxMap3dCs = OrthoTree::OrthoTreeContainerBox<FOctreeBoxMap3ds<DO_SPLIT_PARENT_ENTITIES>>;
using FOctreeBoxMap3dC = FOctreeBoxMap3dCs<true>;

template<bool DO_SPLIT_PARENT_ENTITIES = true>
using FOctreeBoxMap3fCs = OrthoTree::OrthoTreeContainerBox<FOctreeBoxMap3fs<DO_SPLIT_PARENT_ENTITIES>>;
using FOctreeBoxMap3fC = FOctreeBoxMap3fCs<true>;
