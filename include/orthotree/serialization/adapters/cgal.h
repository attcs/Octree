#pragma once

#include "../../adapters/cgal.h"
#include "../../serialization.h"

namespace OrthoTree
{
  // CGAL Point_2
  template<typename TArchive, typename R>
  void serialize(TArchive& ar, CGAL::Point_2<R>& pt)
  {
    auto x = pt.x();
    auto y = pt.y();
    ar& make_nvp("x", x);
    ar& make_nvp("y", y);
    if (OrthoTree::is_loading_archive(ar))
      pt = CGAL::Point_2<R>(x, y);
  }

  // CGAL Point_3
  template<typename TArchive, typename R>
  void serialize(TArchive& ar, CGAL::Point_3<R>& pt)
  {
    auto x = pt.x();
    auto y = pt.y();
    auto z = pt.z();
    ar& make_nvp("x", x);
    ar& make_nvp("y", y);
    ar& make_nvp("z", z);
    if (OrthoTree::is_loading_archive(ar))
      pt = CGAL::Point_3<R>(x, y, z);
  }

  // CGAL Bbox_2
  template<typename TArchive>
  void serialize(TArchive& ar, CGAL::Bbox_2& box)
  {
    auto xmin = box.xmin();
    auto ymin = box.ymin();
    auto xmax = box.xmax();
    auto ymax = box.ymax();
    ar& make_nvp("xmin", xmin);
    ar& make_nvp("ymin", ymin);
    ar& make_nvp("xmax", xmax);
    ar& make_nvp("ymax", ymax);
    if (OrthoTree::is_loading_archive(ar))
      box = CGAL::Bbox_2(xmin, ymin, xmax, ymax);
  }

  // CGAL Bbox_3
  template<typename TArchive>
  void serialize(TArchive& ar, CGAL::Bbox_3& box)
  {
    auto xmin = box.xmin();
    auto ymin = box.ymin();
    auto zmin = box.zmin();
    auto xmax = box.xmax();
    auto ymax = box.ymax();
    auto zmax = box.zmax();
    ar& make_nvp("xmin", xmin);
    ar& make_nvp("ymin", ymin);
    ar& make_nvp("zmin", zmin);
    ar& make_nvp("xmax", xmax);
    ar& make_nvp("ymax", ymax);
    ar& make_nvp("zmax", zmax);
    if (OrthoTree::is_loading_archive(ar))
      box = CGAL::Bbox_3(xmin, ymin, zmin, xmax, ymax, zmax);
  }

  // CGAL Ray_2
  template<typename TArchive, typename R>
  void serialize(TArchive& ar, CGAL::Ray_2<R>& ray)
  {
    auto source = ray.source();
    auto direction = ray.to_vector();
    ar& make_nvp("source", source);
    ar& make_nvp("direction", direction);
    if (OrthoTree::is_loading_archive(ar))
      ray = CGAL::Ray_2<R>(source, direction);
  }

  // CGAL Ray_3
  template<typename TArchive, typename R>
  void serialize(TArchive& ar, CGAL::Ray_3<R>& ray)
  {
    auto source = ray.source();
    auto direction = ray.to_vector();
    ar& make_nvp("source", source);
    ar& make_nvp("direction", direction);
    if (OrthoTree::is_loading_archive(ar))
      ray = CGAL::Ray_3<R>(source, direction);
  }

  // CGAL Plane_2 (Custom)
  template<typename TArchive>
  void serialize(TArchive& ar, CGAL::Plane_2& plane)
  {
    ar& ORTHOTREE_NVP_M(plane, offset);
    ar& ORTHOTREE_NVP_M(plane, normal);
  }

  // CGAL Plane_3
  template<typename TArchive, typename R>
  void serialize(TArchive& ar, CGAL::Plane_3<R>& plane)
  {
    auto a = plane.a();
    auto b = plane.b();
    auto c = plane.c();
    auto d = plane.d();
    ar& make_nvp("a", a);
    ar& make_nvp("b", b);
    ar& make_nvp("c", c);
    ar& make_nvp("d", d);
    if (OrthoTree::is_loading_archive(ar))
      plane = CGAL::Plane_3<R>(a, b, c, d);
  }
} // namespace OrthoTree
