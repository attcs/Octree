#pragma once

#include "../../adapters/eigen.h"
#include "../../serialization.h"

namespace OrthoTree
{
  // Eigen::Matrix (Vector2d, Vector3d, etc.)
  template<typename TArchive, typename Scalar, int Rows, int Cols, int Options, int MaxRows, int MaxCols>
  void serialize(TArchive& ar, Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols>& m)
  {
    for (int i = 0; i < m.size(); ++i)
      ar & m.data()[i];
  }

  // Eigen::AlignedBox
  template<typename TArchive, typename Scalar, int Dim>
  void serialize(TArchive& ar, Eigen::AlignedBox<Scalar, Dim>& box)
  {
    ar& make_nvp("min", box.min());
    ar& make_nvp("max", box.max());
  }
} // namespace OrthoTree
