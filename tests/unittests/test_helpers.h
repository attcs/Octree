#ifndef ORTHOTREE_TEST_HELPERS_H
#define ORTHOTREE_TEST_HELPERS_H

#define ORTHOTREE__PUBLIC_NODE_INTERFACE
#define ORTHOTREE__DEBUG_CHECKS
#include "orthotree/bvh.h"
#include "orthotree/octree.h"

#include <array>
#include <cmath>
#include <limits>
#include <vector>

namespace OrthoTreeTestHelpers
{
  using namespace OrthoTree;
  using std::array;
  using std::vector;

  using SI1D = OrthoTree::detail::MortonSpaceIndexing<1, 20>;
  using SI2D = OrthoTree::detail::MortonSpaceIndexing<2, 20>;
  using SI3D = OrthoTree::detail::MortonSpaceIndexing<3, 20>;
  using SI4D = OrthoTree::detail::MortonSpaceIndexing<4, 20>;

  using EntityID = OrthoTree::index_t;

  inline constexpr auto BB1_INV = BoundingBox1D{ std::numeric_limits<double>::max(), std::numeric_limits<double>::lowest() };

  inline bool AreEqualAlmost(double l, double r) noexcept
  {
    return l == r || std::abs(l - r) < std::numeric_limits<double>::min() * 10;
  }

  template<dim_t nDimension, typename geometry_type = double>
  inline bool AreEqualAlmost(BoundingBoxND<nDimension, geometry_type> const& l, BoundingBoxND<nDimension, geometry_type> const& r)
  {
    for (dim_t iD = 0; iD < nDimension; ++iD)
    {
      if (!AreEqualAlmost(l.Min[iD], r.Min[iD]))
        return false;

      if (!AreEqualAlmost(l.Max[iD], r.Max[iD]))
        return false;
    }

    return true;
  }

  template<typename T1, typename T2>
  inline bool AreContainersItemsEqual(T1 const& l, T2 const& r)
  {
    if (l.size() != r.size())
      return false;

    auto itR = r.begin();
    for (auto itL = l.begin(); itL != l.end(); itL = std::next(itL), itR = std::next(itR))
    {
      if (*itL != *itR)
        return false;
    }
    return true;
  }

  template<dim_t N>
  constexpr auto getPointSetNo1()
  {
    using PointXD = PointND<N>;

    return array{
      PointXD{ 0.0, 0.0 },
      PointXD{ 3.0, 3.0 },

      PointXD{ 5.0, 3.0 },
      PointXD{ 5.2, 3.0 },

      PointXD{ 7.0, 1.0 },

      PointXD{ 0.0, 5.0 },
      PointXD{ 1.0, 5.0 },

      PointXD{ 0.0, 7.0 },
      PointXD{ 1.0, 7.0 },

      PointXD{ 3.0, 7.0 },
      PointXD{ 3.0, 7.0 },

      PointXD{ 2.2, 4.5 },
      PointXD{ 2.5, 4.5 },

      PointXD{ 3.4, 4.5 },
      PointXD{ 3.6, 4.6 },

      PointXD{ 2.4, 5.5 },
      PointXD{ 2.5, 5.5 },

      PointXD{ 3.5, 5.5 },
      PointXD{ 3.5, 5.5 },

      PointXD{ 5.0, 5.0 },
      PointXD{ 5.0, 5.0 },

      PointXD{ 7.0, 5.0 },
      PointXD{ 7.0, 5.0 },

      PointXD{ 6.5, 6.5 },
      PointXD{ 6.5, 6.5 },

      PointXD{ 7.5, 6.5 },
      PointXD{ 7.5, 6.5 },

      PointXD{ 6.5, 7.5 },
      PointXD{ 6.5, 7.5 },

      PointXD{ 7.5, 7.5 },
      PointXD{ 7.5, 7.5 },
      PointXD{ 8.0, 8.0 },
    };
  }
} // namespace OrthoTreeTestHelpers

#endif // ORTHOTREE_TEST_HELPERS_H
