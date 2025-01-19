#pragma once
#include <vector>

#include "../../octree.h"

using namespace OrthoTree;

auto constexpr rMax = 8.0;

template<dim_t DIMENSION_NO>
static constexpr std::vector<OrthoTree::PointND<DIMENSION_NO>> GeneratePointsRandom(size_t pointsNo)
{

  auto aPoint = std::vector<PointND<DIMENSION_NO>>(pointsNo);
  if (pointsNo <= 1)
    return aPoint;

  size_t iNumber = 1;

  auto ptMax = PointND<DIMENSION_NO>{};
  for (dim_t iDim = 0; iDim < DIMENSION_NO; ++iDim)
    ptMax[iDim] = rMax;


  // Corner points
  {
    aPoint[1] = ptMax;
    ++iNumber;

    for (dim_t iDim = 0; iDim < DIMENSION_NO && iNumber < pointsNo; ++iDim, ++iNumber)
      aPoint[iNumber][iDim] = rMax;
  }

  srand(0);
  {
    for (; iNumber < pointsNo; ++iNumber)
      for (dim_t iDim = 0; iDim < DIMENSION_NO; ++iDim)
        aPoint[iNumber][iDim] = double(rand() % 100) * (rMax / 100.0);
  }

  return aPoint;
}


template<dim_t DIMENSION_NO>
static size_t CreateTreePoint(depth_t depth, std::span<PointND<DIMENSION_NO> const> const& aPoint, bool fPar = false)
{
  using TreePoint = TreePointND<DIMENSION_NO>;
  auto box = BoundingBoxND<DIMENSION_NO>{};
  box.Max.fill(rMax);

  auto nt = TreePoint{};
  if (fPar)
    TreePoint::template Create<true>(nt, aPoint, depth, box);
  else
    TreePoint::Create(nt, aPoint, depth, box);

  return nt.GetNodes().size();
}
