//
// pch.h
//

#pragma once

#include <gtest/gtest.h>

#include <span>
#include <vector>

template<typename T1, typename T2>
static bool AreContainersItemsEqual(T1 const& l, T2 const& r)
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
