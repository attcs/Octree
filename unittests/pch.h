// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here

#include "../octree.h"


#include "CppUnitTest.h"

#endif //PCH_H

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

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
