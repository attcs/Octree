#pragma once

#include "../../adapters/glm.h"
#include "../../serialization.h"

namespace OrthoTree
{
  // glm::vec
  template<typename TArchive, glm::length_t L, typename T, glm::qualifier Q>
  void serialize(TArchive& ar, glm::vec<L, T, Q>& v)
  {
    for (glm::length_t i = 0; i < L; ++i)
      ar& v[i];
  }

  // glm::boxNd_t
  template<typename TArchive, dim_t DIM, typename T>
  void serialize(TArchive& ar, glm::boxNd_t<DIM, T>& box)
  {
    ar& ORTHOTREE_NVP_M(box, min);
    ar& ORTHOTREE_NVP_M(box, max);
  }

  // glm::rayNd_t
  template<typename TArchive, dim_t DIM, typename T>
  void serialize(TArchive& ar, glm::rayNd_t<DIM, T>& ray)
  {
    ar& ORTHOTREE_NVP_M(ray, origin);
    ar& ORTHOTREE_NVP_M(ray, direction);
  }

  // glm::planeNd_t
  template<typename TArchive, dim_t DIM, typename T>
  void serialize(TArchive& ar, glm::planeNd_t<DIM, T>& plane)
  {
    ar& ORTHOTREE_NVP_M(plane, origo_distance);
    ar& ORTHOTREE_NVP_M(plane, normal);
  }
} // namespace OrthoTree
