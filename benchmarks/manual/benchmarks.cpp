// benchmarks.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <chrono>
#include <execution>
#include <span>
#include <functional>
#include <numbers>
#include <vector>
#include <random>

#include "../../octree.h"
#include "generators.h"
#include "OrthoTreePointDynamicGeneral.h"

using namespace std::chrono;
using namespace std;

using namespace OrthoTree;


namespace
{
  auto constexpr N = dim_t{ 3 };

  auto constexpr szSeparator = "; ";
  auto constexpr szNewLine = "\n";


#ifdef _DEBUG
  auto constexpr N1M = 100000;
  auto constexpr NR = 1;
#else
  auto constexpr N1M = 1000000;
  auto constexpr NR = 100;
#endif // _DEBUG

  auto boxMax = BoundingBoxND<N>{};

  using time_unit = milliseconds;

  using Adaptor = AdaptorGeneral<N, PointND<N>, BoundingBoxND<N>, RayND<N>, PlaneND<N>>;
  auto constexpr degree_to_rad(double degree) { return degree / 180.0 * std::numbers::pi; }


  template<size_t DIMENSION_NO>
  static constexpr PointND<DIMENSION_NO> CreateBoxMax(PointND<DIMENSION_NO> const& pt, double size)
  {
    auto ptMax = pt;
    for (size_t iDim = 0; iDim < DIMENSION_NO; ++iDim)
      Adaptor::SetPointC(ptMax, static_cast<dim_t>(iDim), Adaptor::GetPointC(pt, static_cast<dim_t>(iDim)) + size);

    return ptMax;
  }


  template<dim_t DIMENSION_NO>
  static BoundingBoxND<DIMENSION_NO> CreateSearcBox(double rBegin, double rSize)
  {
    auto box = BoundingBoxND<DIMENSION_NO>{};
    for (dim_t iDim = 0; iDim < DIMENSION_NO; ++iDim)
      box.Min[iDim] = rBegin;

    box.Max = CreateBoxMax<DIMENSION_NO>(box.Min, rSize);
    return box;
  }


  template<dim_t DIMENSION_NO, size_t nNumber>
  static vector<PointND<DIMENSION_NO>> CreatePoints_Diagonal()
  {
    auto aPoint = vector<PointND<DIMENSION_NO>>(nNumber);
    if (nNumber <= 1)
      return aPoint;

    size_t iNumber = 1;

    auto ptMax = PointND<DIMENSION_NO>{};
    for (dim_t iDim = 0; iDim < DIMENSION_NO; ++iDim)
      ptMax[iDim] = rMax;

    // Corner points
    {
      aPoint[1] = ptMax;
      ++iNumber;

      for (dim_t iDim = 0; iDim < DIMENSION_NO && iNumber < nNumber; ++iDim, ++iNumber)
        aPoint[iNumber][iDim] = rMax;
    }

    auto const nNumberPre = iNumber;
    // Angle points
    {
      auto const nRemain = nNumber - iNumber;
      auto const rStep = rMax / static_cast<double>(nRemain + 2);
      for (size_t iRemain = 1; iNumber < nNumber; ++iNumber, ++iRemain)
        for (dim_t iDim = 0; iDim < DIMENSION_NO; ++iDim)
          aPoint[iNumber][iDim] = rMax - iRemain * rStep;

    }

    std::random_device rd;
    std::mt19937 g(rd());
    g.seed();
    std::shuffle(next(begin(aPoint), nNumberPre), end(aPoint), g);
    return aPoint;
  }

  template<dim_t DIMENSION_NO, size_t nNumber>
  static vector<PointND<DIMENSION_NO>> CreatePoints_CylindricalSemiRandom()
  {
    auto aPoint = vector<PointND<DIMENSION_NO>>(nNumber);
    if (nNumber <= 1)
      return aPoint;

    size_t iNumber = 1;

    auto ptMax = PointND<DIMENSION_NO>{};
    for (dim_t iDim = 0; iDim < DIMENSION_NO; ++iDim)
      ptMax[iDim] = rMax;

    // Corner points
    {
      aPoint[1] = ptMax;
      ++iNumber;

      for (dim_t iDim = 0; iDim < DIMENSION_NO && iNumber < nNumber; ++iDim, ++iNumber)
        aPoint[iNumber][iDim] = rMax;
    }

    srand(0);
    {
      for (; iNumber < nNumber; ++iNumber)
      {
        auto const rAngle = degree_to_rad(static_cast<double>(rand() % 360));
        auto const rRadius = rMax / 4.0 + double(rand() % 100) * 0.01;
        aPoint[iNumber][0] = cos(rAngle) * rRadius + rMax / 2.0;
        aPoint[iNumber][1] = sin(rAngle) * rRadius + rMax / 2.0;

        for (dim_t iDim = 2; iDim < DIMENSION_NO; ++iDim)
          aPoint[iNumber][iDim] = double(rand() % 100) * rMax / 100.0;
      }
    }

    return aPoint;
  }



  template<dim_t DIMENSION_NO, size_t nNumber>
  static vector<BoundingBoxND<DIMENSION_NO>> CreateBoxes_Diagonal()
  {
    if (nNumber == 0)
      return {};

    auto constexpr rMax = 8.0;
    auto constexpr rUnit = 1.0;
    auto aBox = vector<BoundingBoxND<DIMENSION_NO>>(nNumber);
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

    // Angle points
    {
      auto const nRemain = nNumber - iNumber;
      auto const rStep = (rMax - rUnit) / (nRemain + 2);
      for (size_t iRemain = 1; iNumber < nNumber; ++iNumber, ++iRemain)
      {
        for (dim_t iDim = 0; iDim < DIMENSION_NO && iNumber < nNumber; ++iDim)
          aBox[iNumber].Min[iDim] = rMax - rUnit - iRemain * rStep;

        aBox[iNumber].Max = CreateBoxMax(aBox[iNumber].Min, rStep);
      }
    }

    return aBox;
  }

  template<dim_t DIMENSION_NO, size_t nNumber>
  static vector<BoundingBoxND<DIMENSION_NO>> CreateBoxes_Random()
  {
    if (nNumber == 0)
      return {};

    auto constexpr rMax = 8.0;
    auto constexpr rUnit = 1.0;
    auto aBox = vector<BoundingBoxND<DIMENSION_NO>>(nNumber);
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

    srand(0);

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

  template<dim_t DIMENSION_NO, size_t nNumber>
  static vector<BoundingBoxND<DIMENSION_NO>> CreateBoxes_CylindricalSemiRandom()
  {
    if (nNumber == 0)
      return {};

    auto constexpr rUnit = 1.0;
    auto aBox = vector<BoundingBoxND<DIMENSION_NO>>(nNumber);
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

    srand(0);

    {
      for (; iNumber < nNumber; ++iNumber)
      {
        auto const rAngle = degree_to_rad(static_cast<double>(rand() % 180) * 2);
        auto const rRadius = 3.0 + (rand() % 100) * 0.01;
        auto const rSize = 0.0005 + static_cast<double>(rand() % 1000) * 0.0005;
        aBox[iNumber].Min[0] = cos(rAngle) * rRadius + rMax / 2.0 - rSize / 2.0;
        aBox[iNumber].Min[1] = sin(rAngle) * rRadius + rMax / 2.0 - rSize / 2.0;

        for (dim_t iDim = 2; iDim < DIMENSION_NO; ++iDim)
          aBox[iNumber].Min[iDim] = double(rand() % 100) * 0.01 * (rMax - 2.0 * rUnit) + rUnit - rSize / 2.0;

        aBox[iNumber].Max = CreateBoxMax(aBox[iNumber].Min, rSize);

      }
    }

    return aBox;
  }

  template <typename TVector, typename TBox, typename TRay, typename TPlane>
  vector<vector<std::size_t>> RangeSearchNaive(span<TBox const> const& vSearchBox, span<TBox const> const& vBox)
  {
    auto const n = vBox.size();
    auto vElementFound = vector<vector<std::size_t>>();
    vElementFound.reserve(n);
    for (auto const& boxSearch : vSearchBox)
    {
      auto& vElementByBox = vElementFound.emplace_back();
      vElementByBox.reserve(50);

      for (size_t i = 0; i < n; ++i)
        if (AdaptorGeneral<N, TVector, TBox, TRay, TPlane>::AreBoxesOverlapped(boxSearch, vBox[i], false))
          vElementByBox.emplace_back(i);
    }
    return vElementFound;
  }

  template<typename TVector, typename TBox, typename TRay, typename TPlane, typename execution_policy_type = std::execution::unsequenced_policy>
  vector<std::pair<std::size_t, std::size_t>> SelfConflicthNaive(span<TBox const> const& vBox)
  {
    auto const nEntity = vBox.size();

    auto vidCheck = vector<std::size_t>(nEntity);
    std::iota(std::begin(vidCheck), std::end(vidCheck), 0);

    auto vvidCollision = vector<vector<std::size_t>>(vidCheck.size());
    auto ep = execution_policy_type{};
    std::transform(ep, std::begin(vidCheck), std::end(vidCheck), std::begin(vvidCollision), [&](auto const idCheck) -> vector<std::size_t>
    {
      auto sidFound = vector<std::size_t>();
      for (size_t i = idCheck + 1; i < nEntity; ++i)
        if (AdaptorGeneral<N, TVector, TBox, TRay, TPlane>::AreBoxesOverlapped(vBox[idCheck], vBox[i], false))
          sidFound.emplace_back(i);

      return sidFound;
    });

    auto vPair = vector<std::pair<std::size_t, std::size_t>>{};
    if (nEntity > 10)
      vPair.reserve(nEntity / 10);

    for (auto const idCheck : vidCheck)
      for (auto const idCollide : vvidCollision[idCheck])
        vPair.emplace_back(idCheck, idCollide);

    return vPair;
  }


  template<typename TVector, typename TBox, typename TRay, typename TPlane>
  vector<vector<std::size_t>> RangeSearchNaive(span<TBox const> const& vSearchBox, span<TVector const> const& vPoint)
  {
    auto const n = vPoint.size();
    auto vElementFound = vector<vector<std::size_t>>();
    vElementFound.reserve(n);
    for (auto const& boxSearch : vSearchBox)
    {
      auto& vElementByBox = vElementFound.emplace_back();
      vElementByBox.reserve(50);

      for (size_t i = 0; i < n; ++i)
        if (AdaptorGeneral<N, TVector, TBox, TRay, TPlane>::DoesBoxContainPoint(boxSearch, vPoint[i]))
          vElementByBox.emplace_back(i);
    }
    return vElementFound;
  }


  template<dim_t DIMENSION_NO>
  static size_t TreePointNaiveCreate(depth_t depth, std::span<PointND<DIMENSION_NO> const> const& aPoint, bool)
  {
    auto box = BoundingBoxND<DIMENSION_NO>{};
    box.Max.fill(rMax);

    auto const nt = OrthoTreePointDynamicND<DIMENSION_NO>::Create(aPoint, depth, box, 11);
    return nt.GetNodeSize();
  }


  template<dim_t DIMENSION_NO>
  static size_t TreeBoxCreate(depth_t depth, std::span<BoundingBoxND<DIMENSION_NO> const> const& aBox, bool fPar = false)
  {
    auto constexpr nSplit = 0;
    using TreeBox = TreeBoxND<DIMENSION_NO, nSplit>;

    auto box = BoundingBoxND<DIMENSION_NO>{};
    box.Max.fill(rMax);

    auto nt = TreeBox{};
    if (fPar)
      TreeBox::template Create<true>(nt, aBox, depth, box);
    else
      TreeBox::Create(nt, aBox, depth, box);

    return nt.GetNodes().size();
  }

  template<dim_t DIMENSION_NO>
  static size_t TreeBoxDynCreate(depth_t depth, std::span<BoundingBoxND<DIMENSION_NO> const> const& aBox, bool)
  {
    auto box = BoundingBoxND<DIMENSION_NO>{};
    box.Max.fill(rMax);

    auto nt = OrthoTreeBoxDynamicND<DIMENSION_NO>::Create(aBox, depth, box, 11);
    return nt.GetNodeSize();
  }
  

  void display_time(microseconds const& time)
  {
    if (time.count() < 10000)
      std::cout << time.count() << "us";
    else
      std::cout << duration_cast<milliseconds>(time).count() << "ms";
  }

  inline double microseconds_to_double(microseconds const& time)
  {
    return static_cast<double>(time.count()) / 1000.0;
  }

  template<typename entity_type>
  struct MeasurementTask
  {
    string szDisplay;
    int nDataSize;
    int nRepeat = 10;
    depth_t nDepth = 5;
    bool fParallel = false;
    span<entity_type const> sEntity;
    function<size_t(depth_t, span<entity_type const>, bool)> func;

    size_t Run() const { return func(nDepth, sEntity, fParallel); }
  };


  template<size_t N, typename product_type>
  product_type GenerateGeometry(std::function<product_type(void)> fn, string const& szName, double M, ofstream& report)
  {
    std::cout << "Generate " << szName << "...";

    auto const t0 = high_resolution_clock::now();
    product_type const product = fn();
    auto const t1 = high_resolution_clock::now();
    auto const tDur = duration_cast<microseconds>(t1 - t0);

    std::cout << " Finished. ";
    display_time(tDur);
    std::cout << "\n";

    report
      << szName << szSeparator
      << 1 << szSeparator
      << string("seq") << szSeparator
      << M * N1M << szSeparator
      << microseconds_to_double(tDur) << szSeparator
      << szNewLine
      ;

    return product;
  }


  auto constexpr aSizeNonLog = array{ 50, 100, 200, 500, 1000, 1500, 2000, 2500, 3000, 3500, 4000, 5000, 6000, 7000, 8000, 10000 };
  //auto constexpr aSizeNonLog = array{ 10000 };

  auto constexpr nSizeNonLog = aSizeNonLog.size();
  auto constexpr aRepeatNonLog = array{ 100 * NR, 100 * NR, 50*NR, 10 * NR, 10 * NR, 5 * NR, 5 * NR, 5 * NR, 1 * NR, 1 * NR, 1 * NR, 1 * NR, 1 * NR, 1 * NR, 1 * NR, 1 * NR };
  //auto constexpr aRepeatNonLog = array{ 1 * NR };

  static_assert(nSizeNonLog == aRepeatNonLog.size());

  auto constexpr aSizeLog = array{ 10, 50, 100, 1000, 2500, 5000, 7500, 10000, 100000, N1M, 10 * N1M, 100 * N1M };
  auto constexpr nSizeLog = aSizeLog.size();

  auto constexpr aRepeatLog = array{ 100000, 100000, 10000, 2000, 1000, 500, 500, 100, 100, 10, 10, 2 };
  static_assert(nSizeLog == aRepeatLog.size());


  template<dim_t N>
  vector<MeasurementTask<PointND<N>>> GeneratePointTasks(depth_t nDepth, string const& szName, span<PointND<N> const> const& sPoint)
  {
    auto vTask = vector<MeasurementTask<PointND<N>>>();
    for (auto const fPar : { false, true })
      for (size_t iSize = 0; iSize < nSizeLog; ++iSize)
        vTask.push_back(MeasurementTask<PointND<N>>{ szName, aSizeLog[iSize], aRepeatLog[iSize], nDepth, fPar, sPoint.subspan(0, aSizeLog[iSize]), CreateTreePoint<N> });

    return vTask;
  }

  template<dim_t N>
  vector<MeasurementTask<PointND<N>>> GeneratePointTasks_NonLog(depth_t nDepth, string const& szName, span<PointND<N> const> const& sPoint)
  {
    auto vTask = vector<MeasurementTask<PointND<N>>>();
    for (size_t iSize = 0; iSize < nSizeNonLog; ++iSize)
      vTask.push_back(MeasurementTask<PointND<N>>{ szName, aSizeNonLog[iSize], aRepeatNonLog[iSize], nDepth, false, sPoint.subspan(0, aSizeNonLog[iSize]), CreateTreePoint<N> });

    return vTask;
  }


  template<dim_t N>
  vector<MeasurementTask<PointND<N>>> GeneratePointDynTasks_NonLog(depth_t nDepth, string const& szName, span<PointND<N> const> const& sPoint)
  {
    auto vTask = vector<MeasurementTask<PointND<N>>>();
    for (size_t iSize = 0; iSize < nSizeLog; ++iSize)
      vTask.push_back(MeasurementTask<PointND<N>>{ szName, aSizeLog[iSize], aRepeatLog[iSize], nDepth, false, sPoint.subspan(0, aSizeLog[iSize]), TreePointNaiveCreate<N> });

    return vTask;
  }

  template<dim_t N>
  vector<MeasurementTask<BoundingBoxND<N>>> GenerateBoxDynTasks_NonLog(depth_t nDepth, string const& szName, span<BoundingBoxND<N> const> const& sBox)
  {
    auto vTask = vector<MeasurementTask<BoundingBoxND<N>>>();
    for (size_t iSize = 0; iSize < nSizeLog; ++iSize)
      vTask.push_back(MeasurementTask<BoundingBoxND<N>>{ szName, aSizeLog[iSize], aRepeatLog[iSize], nDepth, false, sBox.subspan(0, aSizeLog[iSize]), TreeBoxDynCreate<N> });

    return vTask;
  }


  template<dim_t N>
  vector<MeasurementTask<BoundingBoxND<N>>> CollisionDetectionBoxTasks(depth_t nDepth, string const& szName, span<BoundingBoxND<N> const> const& aBox_)
  {
    auto vTask = vector<MeasurementTask<BoundingBoxND<N>>>();
    for (auto const fPar : { false, true })
      for (size_t iSize = 0; iSize < nSizeNonLog; ++iSize)
      {
        auto nDepthActual = nDepth;
        if (nDepth == -1) // Adapt to the size
        {
          if (aSizeNonLog[iSize] <= 100)
            nDepthActual = 3;
          else if (aSizeNonLog[iSize] <= 500)
            nDepthActual = 3;
          else if (aSizeNonLog[iSize] <= 1000)
            nDepthActual = 4;
          else if (aSizeNonLog[iSize] <= 10000)
            nDepthActual = 5;
          else 
            nDepthActual = 6;
        }
        vTask.push_back(MeasurementTask<BoundingBoxND<N>>{ szName, aSizeNonLog[iSize], aRepeatNonLog[iSize], nDepthActual, fPar, aBox_.subspan(0, aSizeNonLog[iSize]), [](depth_t nDepth_, span<BoundingBoxND<N> const> const& aBox, bool fPar)
        {
          auto constexpr nSplit = 2;
          using TreeBox = TreeBoxND<N, nSplit>;
          auto nt = TreeBox{};
          if (fPar)
          {
            TreeBox::template Create<true>(nt, aBox, nDepth_, boxMax);
            auto const vPair = nt.template CollisionDetection<true>(aBox);
            return vPair.size();
          }
          else
          {
            TreeBox::Create(nt, aBox, nDepth_, boxMax);
            auto const vPair = nt.CollisionDetection(aBox);
            return vPair.size();
          }          
        } 
        });
      }
    return vTask;
  }

  template<dim_t N>
  vector<MeasurementTask<BoundingBoxND<N>>> SearchTreeBoxTasks(depth_t nDepth, string const& szName, span<BoundingBoxND<N> const> const& aBox_)
  {
    auto vTask = vector<MeasurementTask<BoundingBoxND<N>>>();
    for (size_t iSize = 0; iSize < nSizeNonLog; ++iSize)
    {
      auto nDepthActual = nDepth;
      if (nDepth == -1) // Adapt to the size
      {
        if (aSizeNonLog[iSize] <= 100)
          nDepthActual = 3;
        else if (aSizeNonLog[iSize] <= 500)
          nDepthActual = 3;
        else if (aSizeNonLog[iSize] <= 1000)
          nDepthActual = 4;
        else if (aSizeNonLog[iSize] <= 10000)
          nDepthActual = 5;
        else
          nDepthActual = 6;
      }
      vTask.push_back(MeasurementTask<BoundingBoxND<N>>{ szName,
                                                         aSizeNonLog[iSize],
                                                         aRepeatNonLog[iSize],
                                                         nDepthActual,
                                                         false,
                                                         aBox_.subspan(0, aSizeNonLog[iSize]),
                                                         [](depth_t nDepth, span<BoundingBoxND<N> const> const& aBox, bool fPar) {
                                                           auto constexpr nSplit = 2;
                                                           auto nt = TreeBoxND<N, nSplit>{};
                                                           TreeBoxND<N, nSplit>::Create(nt, aBox, nDepth, boxMax);
                                                           size_t nFound = 0;
                                                           auto const n = aBox.size();
                                                           for (size_t i = 0; i < n; ++i)
                                                           {
                                                             auto const vElem = nt.template RangeSearch<false>(aBox[i], aBox);
                                                             nFound += vElem.size();
                                                             assert(vElem.size());
                                                           }
                                                           return nFound;
                                                         } });
    }
    return vTask;
  }

  template<dim_t N>
  vector<MeasurementTask<BoundingBoxND<N>>> SearchBruteForceBoxTasks(depth_t nDepth, string const& szName, span<BoundingBoxND<N> const> const& aBox_)
  {
    auto vTask = vector<MeasurementTask<BoundingBoxND<N>>>();
    for (size_t iSize = 0; iSize < nSizeNonLog; ++iSize)
      vTask.push_back(MeasurementTask<BoundingBoxND<N>>
        { szName, aSizeNonLog[iSize], aRepeatNonLog[iSize] / 50, nDepth, false, aBox_.subspan(0, aSizeNonLog[iSize])
        , [](depth_t nDepth, span<BoundingBoxND<N> const> const& aBox, bool fPar)
          {
            auto const vvElem = RangeSearchNaive<PointND<N>, BoundingBoxND<N>, RayND<N>, PlaneND<N>>(aBox, aBox);

            size_t nFound = 0;
            for (auto const& vElem : vvElem)
              nFound += vElem.size();

            return nFound;
          }
        }
      );

    return vTask;
  }

  template<dim_t N>
  vector<MeasurementTask<BoundingBoxND<N>>> SelfConflictBruteForceBoxTasks(string const& szName, span<BoundingBoxND<N> const> const& aBox_)
  {
    auto vTask = vector<MeasurementTask<BoundingBoxND<N>>>();
    for (size_t iSize = 0; iSize < nSizeNonLog; ++iSize)
      vTask.push_back(MeasurementTask<BoundingBoxND<N>>
    { szName, aSizeNonLog[iSize], aRepeatNonLog[iSize] / 50, 0, false, aBox_.subspan(0, aSizeNonLog[iSize])
        , [](depth_t nDepth, span<BoundingBoxND<N> const> const& aBox, bool fPar)
    {
      if (!fPar)
      {
        auto const vvElem = SelfConflicthNaive<PointND<N>, BoundingBoxND<N>, RayND<N>, PlaneND<N>>(aBox);
        return vvElem.size();
      }
      else
      {
        auto const vvElem = SelfConflicthNaive<PointND<N>, BoundingBoxND<N>, RayND<N>, PlaneND<N>, std::execution::parallel_unsequenced_policy>(aBox);
        return vvElem.size();
      }
    }
    }
    );

    return vTask;
  }


  template<size_t N>
  vector<MeasurementTask<BoundingBoxND<N>>> GenerateBoxTasks(depth_t nDepth, string szName, span<BoundingBoxND<N> const> const& sBox)
  {
    auto vTask = vector<MeasurementTask<BoundingBoxND<N>>>();
    for (auto const fPar : { false, true })
      for (size_t iSize = 0; iSize < nSizeLog; ++iSize)
        vTask.push_back(MeasurementTask<BoundingBoxND<N>>{ szName, aSizeLog[iSize], aRepeatLog[iSize], nDepth, fPar, sBox.subspan(0, aSizeLog[iSize]), TreeBoxCreate<N> });

    return vTask;
  }


  tuple<microseconds, size_t> Measure(int nRepeat, function<size_t(void)> const& fn)
  {
    auto const t0 = high_resolution_clock::now();
    auto v = vector<size_t>(nRepeat);
    for (int i = 0; i < nRepeat; ++i)
      v[i] = fn();

    auto const t1 = high_resolution_clock::now();
    return { duration_cast<microseconds>((t1 - t0) / nRepeat), v[0] };
  }


  template<typename entity_type>
  void RunTasks(vector<MeasurementTask<entity_type>> const& vTask, ofstream& report)
  {
    for (auto const& task : vTask)
    {
      auto const szPar = (task.fParallel ? string("par") : string("unseq"));
      std::cout << "Create tree for: " << task.szDisplay << " " << task.nDataSize << " " << szPar << " Repeat: " << task.nRepeat << "...";
      auto const[tDur, nNode] = Measure(task.nRepeat, [&task]{ return task.Run(); });

      std::cout << " Finished. ";
      display_time(tDur);
      std::cout << szNewLine;

      report
        << task.szDisplay << szSeparator
        << task.nRepeat << szSeparator
        << szPar << szSeparator
        << task.nDataSize << szSeparator
        << microseconds_to_double(tDur) << szSeparator
        << nNode << szSeparator
        << szNewLine
        ;
    }
  }
}


int main()
{
  boxMax.Max.fill(rMax);

  ofstream report;
  report.open("report.csv");
  
  auto constexpr nDepth = depth_t{ 5 }; 
  {
    auto const szName = string("Diagonally placed points");
    auto const aPointDiag_100M = GenerateGeometry<N, vector<PointND<N>>>([&] { return CreatePoints_Diagonal<N, 100 * N1M>(); }, szName, 100, report);
    auto const vTask = GeneratePointTasks<N>(nDepth, szName, aPointDiag_100M);
    RunTasks(vTask, report);
  }

  {
    auto const szName = string("Random placed points");
    auto const aPointDiag_100M = GenerateGeometry<N, vector<PointND<N>>>([&] { return GeneratePointsRandom<N>(100 * N1M); }, szName, 100, report);
    auto const vTask = GeneratePointTasks<N>(nDepth, szName, aPointDiag_100M);
    RunTasks(vTask, report);
  }

  {
    auto const szName = string("Cylindrical semi-random placed points");
    auto const aPointDiag_100M = GenerateGeometry<N, vector<PointND<N>>>([&] { return CreatePoints_CylindricalSemiRandom<N, 100 * N1M>(); }, szName, 100, report);
    auto const vTask = GeneratePointTasks<N>(nDepth, szName, aPointDiag_100M);
    RunTasks(vTask, report);
  }
  
  {
    auto const szName = string("Diagonally placed boxes");
    auto const aPointDiag_100M = GenerateGeometry<N, vector<BoundingBoxND<N>>>([&] { return CreateBoxes_Diagonal<N, 100 * N1M>(); }, szName, 100, report);
    auto const vTask = GenerateBoxTasks<N>(nDepth, szName, aPointDiag_100M);
    RunTasks(vTask, report);
  }

  {
    auto const szName = string("Random placed boxes");
    auto const aPointDiag_100M = GenerateGeometry<N, vector<BoundingBoxND<N>>>([&] { return CreateBoxes_Random<N, 100 * N1M>(); }, szName, 100, report);
    auto const vTask = GenerateBoxTasks<N>(nDepth, szName, aPointDiag_100M);
    RunTasks(vTask, report);
  }

  {
    auto const szName = string("Cylindrical semi-random placed boxes");
    auto const aPointDiag_100M = GenerateGeometry<N, vector<BoundingBoxND<N>>>([&] { return CreateBoxes_CylindricalSemiRandom<N, 100 * N1M>(); }, szName, 100, report);
    auto const vTask = GenerateBoxTasks<N>(nDepth, szName, aPointDiag_100M);
    RunTasks(vTask, report);
  }
  
  // Morton vs Dynamic
  {
    auto const szName = string("Cylindrical semi-random placed points Morton vs Dynamic");
    auto const aPoint = GenerateGeometry<N, vector<PointND<N>>>([&] { return GeneratePointsRandom<N, N1M>(); }, szName, 100, report);
    auto const aBox = GenerateGeometry<N, vector<BoundingBoxND<N>>>([&] { return CreateBoxes_Random<N, N1M>(); }, szName, 100, report);

    auto const vTaskMortonP = GeneratePointTasks<N>(nDepth, "Morton point", aPoint);
    auto const vTaskDynP = GeneratePointDynTasks_NonLog<N>(nDepth, "Dynamic point", aPoint);
    
    auto const vTaskMortonB = GenerateBoxTasks<N>(nDepth, "Morton box", aBox);
    auto const vTaskDynB = GenerateBoxDynTasks_NonLog<N>(nDepth, "Dynamic box", aBox);

    RunTasks(vTaskMortonP, report);
    RunTasks(vTaskDynP, report);
    RunTasks(vTaskMortonB, report);
    RunTasks(vTaskDynB, report);

  }
  
  // Collision detection
  {
    auto const szName = string("Search: Cylindrical semi-random placed point NoPt/NoBox:100%");
    auto const aBox = GenerateGeometry<N, vector<BoundingBoxND<N>>>([&] { return CreateBoxes_CylindricalSemiRandom<N, static_cast<size_t>(aSizeNonLog.back())>(); }, szName, aSizeNonLog.back(), report);
    auto const vTaskBruteForce = SelfConflictBruteForceBoxTasks<N>("Box self conflict by brute force", aBox);
    auto const vTaskTreeCollisionDetection = CollisionDetectionBoxTasks<N>(3, "Box self conflict by octree", aBox);
    auto const vTaskTreeSearch = SearchTreeBoxTasks<N>(5, "Box search by octree", aBox);

    RunTasks(vTaskBruteForce, report);
    RunTasks(vTaskTreeCollisionDetection, report);
    RunTasks(vTaskTreeSearch, report);
  }
  
  report.close();
}
