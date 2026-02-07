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

#include <concepts>
#include <optional>
#include <type_traits>

namespace OrthoTree
{

  // Enum of relation with Planes
  enum class PlaneRelation : char
  {
    Negative,
    Hit,
    Positive
  };

  // Adapter concepts

  template<class TAdapter, dim_t DIMENSION_NO_, typename TVector, typename TBox, typename TRay, typename TPlane, typename TScalar, typename TFloatScalar>
  concept BaseGeometryAdapterConcept =
    // --- required nested types ---
    requires {
      typename TAdapter::Scalar;
      typename TAdapter::FloatScalar;
      typename TAdapter::Vector;
      typename TAdapter::Box;
      typename TAdapter::Ray;
      typename TAdapter::Plane;
    }

    // --- exact type bindings ---
    && std::same_as<typename TAdapter::Scalar, TScalar> && std::same_as<typename TAdapter::FloatScalar, TFloatScalar> &&
    std::same_as<typename TAdapter::Vector, TVector> && std::same_as<typename TAdapter::Box, TBox> && std::same_as<typename TAdapter::Ray, TRay> &&
    std::same_as<typename TAdapter::Plane, TPlane> &&
    std::is_floating_point_v<typename TAdapter::FloatScalar>

    // --- required static constexpr values ---
    &&
    requires {
      { TAdapter::DIMENSION_NO } -> std::convertible_to<dim_t>;
      { TAdapter::BASE_TOLERANCE } -> std::convertible_to<TFloatScalar>;
    } &&
    (TAdapter::DIMENSION_NO == DIMENSION_NO_)

    // --- point access ---
    &&
    requires(TVector& p, TVector const& cp, dim_t d, TScalar v) {
      { TAdapter::GetPointC(cp, d) } -> std::convertible_to<TScalar>;
      { TAdapter::SetPointC(p, d, v) } -> std::same_as<void>;
    }

    // --- box access ---
    &&
    requires(TBox& b, TBox const& cb, dim_t d, TScalar v) {
      { TAdapter::GetBoxMinC(cb, d) } -> std::convertible_to<TScalar>;
      { TAdapter::GetBoxMaxC(cb, d) } -> std::convertible_to<TScalar>;
      { TAdapter::SetBoxMinC(b, d, v) } -> std::same_as<void>;
      { TAdapter::SetBoxMaxC(b, d, v) } -> std::same_as<void>;
    }

    // --- ray access ---
    &&
    requires(TRay const& r) {
      { TAdapter::GetRayOrigin(r) } -> std::convertible_to<TVector>;
      { TAdapter::GetRayDirection(r) } -> std::convertible_to<TVector>;
    }

    // --- plane access ---
    && requires(TPlane const& p) {
         { TAdapter::GetPlaneNormal(p) } -> std::convertible_to<TVector>;
         { TAdapter::GetPlaneOrigoDistance(p) } -> std::convertible_to<TScalar>;
       };


  template<class TAdapter, dim_t DIMENSION_NO, typename TVector, typename TBox, typename TRay, typename TPlane, typename TScalar, typename TFloatScalar>
  concept GeometryAdapterConcept = requires {
    requires BaseGeometryAdapterConcept<TAdapter, DIMENSION_NO, TVector, TBox, TRay, TPlane, TScalar, TFloatScalar>;
  } && requires(TBox const& box, TVector const& point, TFloatScalar tolerance) {
    { TAdapter::DoesBoxContainPoint(box, point, tolerance) } -> std::convertible_to<bool>;
  } && requires(TBox const& e1, TBox const& e2, bool e1_must_contain_e2, bool fOverlapPtTouchAllowed, TFloatScalar tolerance) {
    { TAdapter::AreBoxesOverlapped(e1, e2, e1_must_contain_e2, fOverlapPtTouchAllowed, tolerance) } -> std::convertible_to<bool>;
  } && requires(TBox const& e1, TBox const& e2, TFloatScalar tolerance) {
    { TAdapter::AreBoxesOverlappedStrict(e1, e2, tolerance) } -> std::convertible_to<bool>;
  } && requires(TVector const& box, TScalar distanceOfOrigo, TVector const& planeNormal, TFloatScalar tolerance) {
    { TAdapter::GetPointPlaneRelation(box, distanceOfOrigo, planeNormal, tolerance) } -> std::convertible_to<PlaneRelation>;
  };
} // namespace OrthoTree
