#pragma once

#include <random>
#include <vector>


#include "orthotree/octree.h"

using namespace OrthoTree;

auto constexpr rMax = 8.0;

template<dim_t DIMENSION_NO>
std::vector<PointND<DIMENSION_NO>> GeneratePointsRandom(size_t pointsNo, int seed = 0)
{
  auto rng = std::mt19937(seed);
  std::uniform_real_distribution<double> dist(0.0, rMax);

  auto aPoint = std::vector<PointND<DIMENSION_NO>>(pointsNo);
  if (pointsNo <= 1)
    return aPoint;

  size_t iNumber = 1;

  auto ptMax = PointND<DIMENSION_NO>{};
  for (dim_t iDim = 0; iDim < DIMENSION_NO; ++iDim)
    ptMax[iDim] = rMax;


  // Corner points (Origin, Far Corner, and points on axes at rMax)
  {
    aPoint[1] = ptMax;
    ++iNumber;

    for (dim_t iDim = 0; iDim < DIMENSION_NO && iNumber < pointsNo; ++iDim, ++iNumber)
      aPoint[iNumber][iDim] = rMax;
  }

  {
    for (; iNumber < pointsNo; ++iNumber)
      for (dim_t iDim = 0; iDim < DIMENSION_NO; ++iDim)
        aPoint[iNumber][iDim] = dist(rng);
  }

  return aPoint;
}


template<dim_t DIMENSION_NO>
size_t CreateTreePoint(depth_t depth, std::span<PointND<DIMENSION_NO> const> const& aPoint, bool fPar = false)
{
  using TreePoint = OrthoTreePointND<DIMENSION_NO>;
  auto box = BoundingBoxND<DIMENSION_NO>{};
  box.Max.fill(rMax);

  auto nt = TreePoint{};
  if (fPar)
    TreePoint::template Create<ExecutionTags::Parallel>(nt, aPoint, depth, box);
  else
    TreePoint::Create(nt, aPoint, depth, box);

  return nt.GetNodeCount();
}


template<size_t DIMENSION_NO>
constexpr PointND<DIMENSION_NO> CreateBoxMax(PointND<DIMENSION_NO> const& pt, double size)
{
  auto ptMax = pt;
  for (size_t iDim = 0; iDim < DIMENSION_NO; ++iDim)
    ptMax[iDim] = pt[iDim] + size;

  return ptMax;
}


template<dim_t DIMENSION_NO>
BoundingBoxND<DIMENSION_NO> CreateSearcBox(double rBegin, double rSize)
{
  auto box = BoundingBoxND<DIMENSION_NO>{};
  for (dim_t iDim = 0; iDim < DIMENSION_NO; ++iDim)
    box.Min[iDim] = rBegin;

  box.Max = CreateBoxMax<DIMENSION_NO>(box.Min, rSize);
  return box;
}


template<dim_t DIMENSION_NO>
std::vector<BoundingBoxND<DIMENSION_NO>> GenerateBoxesRandom(size_t nNumber, int seed = 0, double boxSizeScale = 0.125)
{
  if (nNumber == 0)
    return {};

  auto rng = std::mt19937(seed);
  auto aBox = std::vector<BoundingBoxND<DIMENSION_NO>>(nNumber);

  // World boundary box [0, rMax]
  aBox[0].Min.fill(0.0);
  for (dim_t iDim = 0; iDim < DIMENSION_NO; ++iDim)
    aBox[0].Max[iDim] = rMax;

  if (nNumber == 1)
    return aBox;

  // 1. Position Strategy: Clustered (Hotspots)
  // Key for Sparse Octrees: creates deep isolated branches with skipped levels.
  size_t const nClusters = 3 + static_cast<size_t>(std::sqrt(nNumber) / 10.0);
  std::vector<PointND<DIMENSION_NO>> clusters(nClusters);
  std::uniform_real_distribution<double> distWorld(0.0, rMax);
  for (auto& c : clusters)
    for (dim_t d = 0; d < DIMENSION_NO; ++d)
      c[d] = distWorld(rng);

  std::uniform_real_distribution<double> distPos(0.0, 1.0);
  std::normal_distribution<double> distClusterShift(0.0, rMax * 0.02); // Standard deviation for tight clusters

  // 2. Size Distribution: Log-Normal
  // Focuses in the middle, few very large, few extremely small.
  // Centered around a "typical" depth (e.g. Level 8-10) to reach significant depths.
  double const meanDepth = 8.0;
  double const targetSize = rMax / std::pow(2.0, meanDepth);
  std::lognormal_distribution<double> distSize(std::log(targetSize), 1.0);

  size_t iNumber = 1;

  // Corner boxes (Boundary condition testing)
  {
    auto constexpr rUnit = 1.0;
    for (dim_t iDim = 0; iDim < DIMENSION_NO && iNumber < nNumber; ++iDim, ++iNumber)
    {
      aBox[iNumber].Min.fill(0.0);
      aBox[iNumber].Min[iDim] = rMax - rUnit;
      aBox[iNumber].Max = CreateBoxMax(aBox[iNumber].Min, rUnit);
    }
  }

  // Large scattered boxes (few and sparse)
  size_t const nLarge = std::max<size_t>(1, nNumber / 200);
  for (; iNumber <= nLarge && iNumber < nNumber; ++iNumber)
  {
    double rSize = (0.3 + distPos(rng) * 0.7) * (rMax * boxSizeScale * 2.0);
    for (dim_t iDim = 0; iDim < DIMENSION_NO; ++iDim)
    {
      aBox[iNumber].Min[iDim] = distPos(rng) * (rMax - rSize);
      aBox[iNumber].Max[iDim] = aBox[iNumber].Min[iDim] + rSize;
    }
  }

  // Clustered detailed boxes (the bulk of the data)
  for (; iNumber < nNumber; ++iNumber)
  {
    double const rSize = std::min(rMax * 0.4, distSize(rng));
    size_t const clusterIdx = rng() % nClusters;

    for (dim_t iDim = 0; iDim < DIMENSION_NO; ++iDim)
    {
      double pos = clusters[clusterIdx][iDim] + distClusterShift(rng);
      if (pos < 0)
        pos = 0;
      if (pos > rMax - rSize)
        pos = rMax - rSize;

      aBox[iNumber].Min[iDim] = pos;
      aBox[iNumber].Max[iDim] = pos + rSize;
    }
  }

  return aBox;
}
