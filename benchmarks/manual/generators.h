#pragma once
#include <vector>

#include "../../octree.h"

using namespace OrthoTree;

auto constexpr rMax = 8.0;

template<dim_t DIMENSION_NO>
constexpr std::vector<PointND<DIMENSION_NO>> GeneratePointsRandom(size_t pointsNo, int seed = 0)
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

  srand(seed);
  {
    for (; iNumber < pointsNo; ++iNumber)
      for (dim_t iDim = 0; iDim < DIMENSION_NO; ++iDim)
        aPoint[iNumber][iDim] = double(rand() % 100) * (rMax / 100.0);
  }

  return aPoint;
}


template<dim_t DIMENSION_NO>
size_t CreateTreePoint(depth_t depth, std::span<PointND<DIMENSION_NO> const> const& aPoint, bool fPar = false)
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
constexpr std::vector<BoundingBoxND<DIMENSION_NO>> GenerateBoxesRandom(size_t nNumber, int seed = 0)
{
  if (nNumber == 0)
    return {};

  auto constexpr rMax = 8.0;
  auto constexpr rUnit = 1.0;
  auto aBox = std::vector<BoundingBoxND<DIMENSION_NO>>(nNumber);
  aBox[0].Max = CreateBoxMax(PointND<DIMENSION_NO>(), rMax);
  if (nNumber == 1)
    return aBox;

  size_t iNumber = 1;

  // Corner points
  {
    for (dim_t iDim = 0; iDim < DIMENSION_NO && iNumber < nNumber; ++iDim, ++iNumber)
    {
      aBox[iNumber].Min[iDim] = rMax - rUnit;
      aBox[iNumber].Max = CreateBoxMax(aBox[iNumber].Min, rUnit);
    }
    if (iNumber == nNumber)
      return aBox;


    for (dim_t iDim = 0; iDim < DIMENSION_NO; ++iDim)
      aBox[iNumber].Min[iDim] = rMax - rUnit;

    aBox[iNumber].Max = CreateBoxMax(aBox[iNumber].Min, rUnit);

    ++iNumber;
  }

  srand(seed);

  {
    for (size_t iRemain = 1; iNumber < nNumber; ++iNumber, ++iRemain)
    {
      auto const iNumberBox = nNumber - iNumber - 1;
      for (dim_t iDim = 0; iDim < DIMENSION_NO && iNumber < nNumber; ++iDim)
        aBox[iNumberBox].Min[iDim] = double(rand() % 100) * ((rMax - 1.0) / 100.0);

      aBox[iNumberBox].Max = CreateBoxMax(aBox[iNumberBox].Min, double(rand() % 100) * (1.0 * rUnit / 100.0));
    }
  }

  return aBox;
}