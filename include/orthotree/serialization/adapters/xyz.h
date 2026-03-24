#pragma once

#include "../../adapters/xyz.h"
#include "../../serialization.h"

namespace OrthoTree
{
  // BasicTypesXYZ::Point2D
  template<typename TArchive>
  void serialize(TArchive& ar, BasicTypesXYZ::Point2D& pt)
  {
    ar& ORTHOTREE_NVP_M(pt, x);
    ar& ORTHOTREE_NVP_M(pt, y);
  }

  // BasicTypesXYZ::Point3D
  template<typename TArchive>
  void serialize(TArchive& ar, BasicTypesXYZ::Point3D& pt)
  {
    ar& ORTHOTREE_NVP_M(pt, x);
    ar& ORTHOTREE_NVP_M(pt, y);
    ar& ORTHOTREE_NVP_M(pt, z);
  }

  // BasicTypesXYZ::BoundingBox2D
  template<typename TArchive>
  void serialize(TArchive& ar, BasicTypesXYZ::BoundingBox2D& box)
  {
    ar& ORTHOTREE_NVP_M(box, Min);
    ar& ORTHOTREE_NVP_M(box, Max);
  }

  // BasicTypesXYZ::BoundingBox3D
  template<typename TArchive>
  void serialize(TArchive& ar, BasicTypesXYZ::BoundingBox3D& box)
  {
    ar& ORTHOTREE_NVP_M(box, Min);
    ar& ORTHOTREE_NVP_M(box, Max);
  }

  // BasicTypesXYZ::Ray2D
  template<typename TArchive>
  void serialize(TArchive& ar, BasicTypesXYZ::Ray2D& ray)
  {
    ar& ORTHOTREE_NVP_M(ray, BasePoint);
    ar& ORTHOTREE_NVP_M(ray, Heading);
  }

  // BasicTypesXYZ::Ray3D
  template<typename TArchive>
  void serialize(TArchive& ar, BasicTypesXYZ::Ray3D& ray)
  {
    ar& ORTHOTREE_NVP_M(ray, BasePoint);
    ar& ORTHOTREE_NVP_M(ray, Heading);
  }

  // BasicTypesXYZ::Plane2D
  template<typename TArchive>
  void serialize(TArchive& ar, BasicTypesXYZ::Plane2D& plane)
  {
    ar& ORTHOTREE_NVP_M(plane, OrigoDistance);
    ar& ORTHOTREE_NVP_M(plane, Normal);
  }

  // BasicTypesXYZ::Plane3D
  template<typename TArchive>
  void serialize(TArchive& ar, BasicTypesXYZ::Plane3D& plane)
  {
    ar& ORTHOTREE_NVP_M(plane, OrigoDistance);
    ar& ORTHOTREE_NVP_M(plane, Normal);
  }
} // namespace OrthoTree
