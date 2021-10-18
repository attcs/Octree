// benchmarks.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#define autoc auto const
#define autoce auto constexpr

#include <iostream>
#include <fstream>
#include <chrono>
#include <execution>
#include <span>
#include <functional>
#include <numbers>
#include <vector>
#include <random>

#include "../octree.h"
#include "OrthoTreePointDynamicGeneral.h"


using namespace std::chrono;
using namespace std;

using namespace NTree;


namespace
{
  autoce N = 3;

  autoce szSeparator = "; ";
  autoce szNewLine = "\n";

  autoce rMax = 8.0;

#ifdef _DEBUG
  autoce N1M = 100000;
  autoce NR = 1;
#else
  autoce N1M = 1000000;
  autoce NR = 100;
#endif // _DEBUG

  using time_unit = milliseconds;

  autoce degree_to_rad(double degree) { return degree / 180 * std::numbers::pi; }


  template<size_t nDim>
  static constexpr PointND<nDim> CreateBoxMax(PointND<nDim> const& pt, double size)
  {
    using Ad = AdaptorGeneral<nDim, PointND<nDim>, BoundingBoxND<nDim>>;
    auto ptMax = pt;
    for (size_t iDim = 0; iDim < nDim; ++iDim)
      Ad::point_comp(ptMax, static_cast<dim_type>(iDim)) += size;

    return ptMax;
  }


  template<dim_type nDim>
  static BoundingBoxND<nDim> CreateSearcBox(double rBegin, double rSize)
  {
    auto box = BoundingBoxND<nDim>{};
    for (dim_type iDim = 0; iDim < nDim; ++iDim)
      box.Min[iDim] = rBegin;

    box.Max = CreateBoxMax<nDim>(box.Min, rSize);
    return box;
  }


  template<dim_type nDim, size_t nNumber>
  static vector<PointND<nDim>> CreatePoints_Diagonal()
  {
    auto aPoint = vector<PointND<nDim>>(nNumber);
    if (nNumber <= 1)
      return aPoint;

    size_t iNumber = 1;

    // Corner points
    {
      for (dim_type iDim = 0; iDim < nDim && iNumber < nNumber; ++iDim, ++iNumber)
        aPoint[iNumber][iDim] = rMax;

      if (iNumber == nNumber)
        return aPoint;

      for (dim_type iDim = 0; iDim < nDim; ++iDim)
        aPoint[iNumber][iDim] = rMax;

      ++iNumber;
    }

    autoc nNumberPre = iNumber;
    // Angle points
    {
      autoc nRemain = nNumber - iNumber;
      autoc rStep = rMax / static_cast<double>(nRemain + 2);
      for (size_t iRemain = 1; iNumber < nNumber; ++iNumber, ++iRemain)
        for (dim_type iDim = 0; iDim < nDim; ++iDim)
          aPoint[iNumber][iDim] = rMax - iRemain * rStep;

    }

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(next(begin(aPoint), nNumberPre), end(aPoint), g);

    return aPoint;
  }

  template<dim_type nDim, size_t nNumber>
  static vector<PointND<nDim>> CreatePoints_Random()
  {
    auto aPoint = vector<PointND<nDim>>(nNumber);
    if (nNumber <= 1)
      return aPoint;

    size_t iNumber = 1;

    // Corner points
    {
      for (dim_type iDim = 0; iDim < nDim && iNumber < nNumber; ++iDim, ++iNumber)
        aPoint[iNumber][iDim] = rMax;

      if (iNumber == nNumber)
        return aPoint;

      for (dim_type iDim = 0; iDim < nDim; ++iDim)
        aPoint[iNumber][iDim] = rMax;

      ++iNumber;
    }

    srand(0);
    {
      for (; iNumber < nNumber; ++iNumber)
        for (dim_type iDim = 0; iDim < nDim; ++iDim)
          aPoint[iNumber][iDim] = (rand() % 100) * (rMax / 100.0);

    }

    return aPoint;
  }

  template<dim_type nDim, size_t nNumber>
  static vector<PointND<nDim>> CreatePoints_CylindricalSemiRandom()
  {
    auto aPoint = vector<PointND<nDim>>(nNumber);
    if (nNumber <= 1)
      return aPoint;

    size_t iNumber = 1;

    // Corner points
    {
      for (dim_type iDim = 0; iDim < nDim && iNumber < nNumber; ++iDim, ++iNumber)
        aPoint[iNumber][iDim] = rMax;

      if (iNumber == nNumber)
        return aPoint;

      for (dim_type iDim = 0; iDim < nDim; ++iDim)
        aPoint[iNumber][iDim] = rMax;

      ++iNumber;
    }

    srand(0);
    {
      for (; iNumber < nNumber; ++iNumber)
      {
        autoc rAngle = degree_to_rad(static_cast<double>(rand() % 360));
        autoc rRadius = rMax / 4.0 + (rand() % 100) * 0.01;
        aPoint[iNumber][0] = cos(rAngle) * rRadius + rMax / 2.0;
        aPoint[iNumber][1] = sin(rAngle) * rRadius + rMax / 2.0;

        for (dim_type iDim = 2; iDim < nDim; ++iDim)
          aPoint[iNumber][iDim] = (rand() % 100) * rMax / 100.0;
      }
    }

    return aPoint;
  }



  template<dim_type nDim, size_t nNumber>
  static vector<BoundingBoxND<nDim>> CreateBoxes_Diagonal()
  {
    if (nNumber == 0)
      return {};

    autoce rMax = 8.0;
    autoce rUnit = 1.0;
    auto aBox = vector<BoundingBoxND<nDim>>(nNumber);
    aBox[0].Max = CreateBoxMax(PointND<nDim>(), rMax);
    if (nNumber == 1)
      return aBox;

    size_t iNumber = 1;

    // Corner points
    {
      for (dim_type iDim = 0; iDim < nDim && iNumber < nNumber; ++iDim, ++iNumber)
      {
        aBox[iNumber].Min[iDim] = rMax - rUnit;
        aBox[iNumber].Max = CreateBoxMax(aBox[iNumber].Min, rUnit);
      }
      if (iNumber == nNumber)
        return aBox;


      for (dim_type iDim = 0; iDim < nDim; ++iDim)
        aBox[iNumber].Min[iDim] = rMax - rUnit;

      aBox[iNumber].Max = CreateBoxMax(aBox[iNumber].Min, rUnit);

      ++iNumber;
    }

    // Angle points
    {
      autoc nRemain = nNumber - iNumber;
      autoc rStep = (rMax - rUnit) / (nRemain + 2);
      for (size_t iRemain = 1; iNumber < nNumber; ++iNumber, ++iRemain)
      {
        for (dim_type iDim = 0; iDim < nDim && iNumber < nNumber; ++iDim)
          aBox[iNumber].Min[iDim] = rMax - rUnit - iRemain * rStep;

        aBox[iNumber].Max = CreateBoxMax(aBox[iNumber].Min, rStep);
      }
    }

    return aBox;
  }

  template<dim_type nDim, size_t nNumber>
  static vector<BoundingBoxND<nDim>> CreateBoxes_Random()
  {
    if (nNumber == 0)
      return {};

    autoce rMax = 8.0;
    autoce rUnit = 1.0;
    auto aBox = vector<BoundingBoxND<nDim>>(nNumber);
    aBox[0].Max = CreateBoxMax(PointND<nDim>(), rMax);
    if (nNumber == 1)
      return aBox;

    size_t iNumber = 1;

    // Corner points
    {
      for (dim_type iDim = 0; iDim < nDim && iNumber < nNumber; ++iDim, ++iNumber)
      {
        aBox[iNumber].Min[iDim] = rMax - rUnit;
        aBox[iNumber].Max = CreateBoxMax(aBox[iNumber].Min, rUnit);
      }
      if (iNumber == nNumber)
        return aBox;


      for (dim_type iDim = 0; iDim < nDim; ++iDim)
        aBox[iNumber].Min[iDim] = rMax - rUnit;

      aBox[iNumber].Max = CreateBoxMax(aBox[iNumber].Min, rUnit);

      ++iNumber;
    }

    srand(0);

    {
      for (size_t iRemain = 1; iNumber < nNumber; ++iNumber, ++iRemain)
      {
        autoc iNumberBox = nNumber - iNumber - 1;
        for (dim_type iDim = 0; iDim < nDim && iNumber < nNumber; ++iDim)
          aBox[iNumberBox].Min[iDim] = (rand() % 100) * ((rMax - 1.0) / 100.0);

        aBox[iNumberBox].Max = CreateBoxMax(aBox[iNumberBox].Min, (rand() % 100) * (1.0 * rUnit / 100.0));
      }
    }

    return aBox;
  }

  template<dim_type nDim, size_t nNumber>
  static vector<BoundingBoxND<nDim>> CreateBoxes_CylindricalSemiRandom()
  {
    if (nNumber == 0)
      return {};

    autoce rUnit = 1.0;
    auto aBox = vector<BoundingBoxND<nDim>>(nNumber);
    aBox[0].Max = CreateBoxMax(PointND<nDim>(), rMax);
    if (nNumber == 1)
      return aBox;

    size_t iNumber = 1;

    // Corner points
    {
      for (dim_type iDim = 0; iDim < nDim && iNumber < nNumber; ++iDim, ++iNumber)
      {
        aBox[iNumber].Min[iDim] = rMax - rUnit;
        aBox[iNumber].Max = CreateBoxMax(aBox[iNumber].Min, rUnit);
      }
      if (iNumber == nNumber)
        return aBox;


      for (dim_type iDim = 0; iDim < nDim; ++iDim)
        aBox[iNumber].Min[iDim] = rMax - rUnit;

      aBox[iNumber].Max = CreateBoxMax(aBox[iNumber].Min, rUnit);

      ++iNumber;
    }

    srand(0);

    {
      for (size_t iRemain = 1; iNumber < nNumber; ++iNumber, ++iRemain)
      {
        autoc id = nNumber - iNumber - 1;
        autoc rAngle = degree_to_rad(static_cast<double>(rand() % 360));
        autoc rRadius = rMax / 4.0 + (rand() % 100) * 0.01;
        aBox[id].Min[0] = cos(rAngle) * rRadius + rMax / 2.0 - rUnit;
        aBox[id].Min[1] = sin(rAngle) * rRadius + rMax / 2.0 - rUnit;

        for (dim_type iDim = 2; iDim < nDim; ++iDim)
          aBox[id].Min[iDim] = (rand() % 100) * 0.02;

        autoc rSize = (rand() % 100) * 0.01;
        aBox[id].Max = CreateBoxMax(aBox[id].Min, rSize);

      }
    }

    return aBox;
  }

  template <typename box_type>
  vector<vector<NTree::entity_id_type>> RangeSearchNaive(span<box_type> const& vSearchBox, span<box_type> const& vBox)
  {
    autoc n = vBox.size();
    auto vElementFound = vector<vector<NTree::entity_id_type>>();
    vElementFound.reserve(n);
    for (autoc& boxSearch : vSearchBox)
    {
      auto& vElementByBox = vElementFound.emplace_back();
      vElementByBox.reserve(50);

      for (size_t i = 0; i < n; ++i)
        if (AdaptorGeneral::are_boxes_overlapped(boxSearch, vBox[i], true))
          vElementByBox.emplace_back(i);
    }
    return vElementFound;
  }


  template <typename point_type, typename box_type>
  vector<vector<NTree::entity_id_type>> RangeSearchNaive(span<box_type> const& vSearchBox, span<point_type> const& vPoint)
  {
    autoc n = vPoint.size();
    auto vElementFound = vector<vector<NTree::entity_id_type>>();
    vElementFound.reserve(n);
    for (autoc& boxSearch : vSearchBox)
    {
      auto& vElementByBox = vElementFound.emplace_back();
      vElementByBox.reserve(50);

      for (size_t i = 0; i < n; ++i)
        if (AdaptorGeneral<N, point_type, box_type>::does_box_contain_point(boxSearch, vPoint[i]))
          vElementByBox.emplace_back(i);
    }
    return vElementFound;
  }


  template<size_t nDim>
  static size_t TreePointCreate(unsigned depth, std::span<PointND<nDim> const> const& aPoint, bool fPar = false)
  {
    auto nt = fPar
      ? TreePointND<nDim>::template Create<std::execution::parallel_unsequenced_policy>(aPoint, depth)
      : TreePointND<nDim>::Create(aPoint, depth)
      ;

    return nt.GetNodes().size();
  }

  template<size_t nDim>
  static size_t TreePointNaiveCreate(unsigned depth, std::span<PointND<nDim> const> const& aPoint)
  {
    auto nt = OrthoTreePointDynamicND<nDim>::Create(aPoint, depth, 11);
    return nt.GetNodeSize();
  }


  template<size_t nDim>
  static size_t TreeBoxCreate(unsigned depth, std::span<BoundingBoxND<nDim> const> const& aBox, bool fPar = false)
  {
    auto nt = fPar
      ? TreeBoxND<nDim>::template Create<std::execution::parallel_unsequenced_policy>(aBox, depth)
      : TreeBoxND<nDim>::Create(aBox, depth)
      ;

    return nt.GetNodes().size();
  }



 
  tuple<microseconds, size_t> Measure(int nRepeat, function<size_t(void)> const& fn)
  {
    autoc t0 = high_resolution_clock::now();
    auto v = vector<size_t>(nRepeat);
    for (int i = 0; i < nRepeat; ++i)
      v[i] = fn();

    autoc t1 = high_resolution_clock::now();
    return { duration_cast<microseconds>((t1 - t0) / nRepeat), v[0] };
  }

  void display_time(microseconds const& time)
  {
    if (time.count() < 10000)
      std::cout << time;
    else
      std::cout << duration_cast<milliseconds>(time);
  }

  inline double microseconds_to_double(microseconds const& time)
  {
    return static_cast<double>(time.count()) / 1000.0;
  }


  struct MeasurementTask
  {
    string szDisplay;
    int nDataSize;
    function<size_t()> func;
    int nRepeat = 10;
    bool fParallel = false;
  };


  template<size_t N, typename product_type>
  product_type GenerateGeometry(std::function<product_type(void)> fn, string const& szName, double M, ofstream& report)
  {
    std::cout << "Generate " << szName << "...";

    autoc t0 = high_resolution_clock::now();
    product_type const product = fn();
    autoc t1 = high_resolution_clock::now();
    autoc tDur = duration_cast<microseconds>(t1 - t0);

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


  autoce aSizeNonLog = array{ 100, 500, 1000, 1500, 2000, 2500, 3000, 3500, 4000, 5000, 6000, 7000, 8000, 10000, 12000, 15000 };
  autoce nSizeNonLog = aSizeNonLog.size();
  autoce aRepeatNonLog = array{ 1000 * NR, 1000 * NR, 1000 * NR, 100 * NR, 100 * NR, 10 * NR, 10 * NR, 10 * NR, 10 * NR, 10 * NR, 10 * NR, 10 * NR, 10 * NR, 10 * NR, 10 * NR, 10 * NR };
  static_assert(nSizeNonLog == aRepeatNonLog.size());

  autoce aSizeLog = array{ 10, 50, 100, 1000, 2500, 5000, 7500, 10000, 100000, N1M, 10 * N1M, 100 * N1M };
  autoce nSizeLog = aSizeLog.size();
  autoce aRepeatLog = array{ 100000, 100000, 10000, 2000, 1000, 500, 500, 100, 100, 10, 10, 5 };
  static_assert(nSizeLog == aRepeatLog.size());

  template<size_t N>
  vector<MeasurementTask> GeneratePointTasks(int nDepth, string const& szName, span<PointND<N> const> const& sPoint)
  {
    auto vTask = vector<MeasurementTask>();
    for (autoc fPar : { false, true })
      for (size_t iSize = 0; iSize < nSizeLog; ++iSize)
        vTask.push_back(MeasurementTask{ szName, aSizeLog[iSize], [nDepth, iSize, fPar, &sPoint] { return TreePointCreate(nDepth, sPoint.subspan(0, aSizeLog[iSize]), fPar); }, aRepeatLog[iSize], fPar });

    return vTask;
  }

  template<size_t N>
  vector<MeasurementTask> GeneratePointTasks_NonLog(int nDepth, string const& szName, span<PointND<N> const> const& sPoint)
  {
    auto vTask = vector<MeasurementTask>();
    for (size_t iSize = 0; iSize < nSizeNonLog; ++iSize)
      vTask.push_back(MeasurementTask{ szName, aSizeNonLog[iSize], [nDepth, iSize, &sPoint] { return TreePointCreate(nDepth, sPoint.subspan(0, aSizeNonLog[iSize]), false); }, aRepeatNonLog[iSize], false });

    return vTask;
  }


  template<size_t N>
  vector<MeasurementTask> GeneratePointDynTasks_NonLog(int nDepth, string const& szName, span<PointND<N> const> const& sPoint)
  {
    auto vTask = vector<MeasurementTask>();
    for (size_t iSize = 0; iSize < nSizeLog; ++iSize)
      vTask.push_back(MeasurementTask{ szName, aSizeLog[iSize], [nDepth, iSize, &sPoint] { return TreePointNaiveCreate(nDepth, sPoint.subspan(0, aSizeLog[iSize])); }, aRepeatLog[iSize], false });

    return vTask;
  }

  template<size_t N>
  vector<MeasurementTask> SearchTreePointTasks(int nDepth, string const& szName, span<PointND<N> const> const& sPoint, span<BoundingBoxND<N> const> const& vSearchBox, double rPercentage)
  {
    auto vTask = vector<MeasurementTask>();
    for (autoc fPar : { false, true })
      for (size_t iSize = 0; iSize < nSizeNonLog; ++iSize)
        vTask.push_back(MeasurementTask{ szName, aSizeNonLog[iSize], [nDepth, iSize, fPar, rPercentage, &sPoint, &vSearchBox]
        {
          autoc aPoint = sPoint.subspan(0, aSizeNonLog[iSize]);
          autoc aSearchBox = vSearchBox.subspan(0, static_cast<size_t>(aSizeNonLog[iSize] * rPercentage / 100.0));

          autoc nt = fPar
            ? TreePointND<N>::template Create<std::execution::parallel_unsequenced_policy>(aPoint, nDepth)
            : TreePointND<N>::Create(aPoint, nDepth)
            ;

          size_t nFound = 0;
          for (autoc& boxSearch : aSearchBox)
          {
            autoc vElem = nt.RangeSearch(boxSearch, aPoint);
            nFound += vElem.size();
          }

          return nFound;
        }, aRepeatNonLog[iSize], fPar });

    return vTask;
  }

  template<size_t N>
  vector<MeasurementTask> SearchBruteForcePointTasks(int nDepth, string const& szName, span<PointND<N> const> const& sPoint, span<BoundingBoxND<N> const> const& vSearchBox, double rPercentage)
  {
    auto vTask = vector<MeasurementTask>();
    for (size_t iSize = 0; iSize < nSizeNonLog; ++iSize)
      vTask.push_back(MeasurementTask{ szName, aSizeNonLog[iSize], [nDepth, iSize, rPercentage, &sPoint, &vSearchBox]
      {
        autoc aPoint = sPoint.subspan(0, aSizeNonLog[iSize]);
        autoc aSearchBox = vSearchBox.subspan(0, static_cast<size_t>(aSizeNonLog[iSize] * rPercentage / 100.0));
        autoc vvElem = RangeSearchNaive(aSearchBox, aPoint);

        size_t nFound = 0;
        for (autoc& vElem : vvElem)
          nFound += vElem.size();

        return nFound;
      }, aRepeatNonLog[iSize], false });

    return vTask;
  }



  template<size_t N>
  vector<MeasurementTask> GenerateBoxTasks(int nDepth, string szName, span<BoundingBoxND<N> const> const& sBox)
  {
    auto vTask = vector<MeasurementTask>();
    for (autoc fPar : { false, true })
      for (int iSize = 0; iSize < nSizeLog; ++iSize)
        vTask.push_back(MeasurementTask{ szName, aSizeLog[iSize],[nDepth, iSize, fPar, &sBox] { return TreeBoxCreate(nDepth, sBox.subspan(0, aSizeLog[iSize]), fPar); }, aRepeatLog[iSize], fPar });

    return vTask;
  }


  void RunTasks(vector<MeasurementTask> const& vTask, ofstream& report)
  {
    for (autoc& task : vTask)
    {
      autoc szPar = (task.fParallel ? string("par") : string("unseq"));
      std::cout << "Create tree for: " << task.szDisplay << " " << task.nDataSize << " " << szPar << " Repeat: " << task.nRepeat << "...";
      autoc [tDur, nNode] = Measure(task.nRepeat, task.func);

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

  ofstream report;
  report.open("report.csv");

  autoce nDepth = 5;
  {
    autoc szName = string("Diagonally placed points");
    autoc aPointDiag_100M = GenerateGeometry<N, vector<PointND<N>>>([&] { return CreatePoints_Diagonal<N, 100 * N1M>(); }, szName, 100, report);
    autoc vTask = GeneratePointTasks<N>(nDepth, szName, aPointDiag_100M);
    RunTasks(vTask, report);
  }

  {
    autoc szName = string("Random placed points");
    autoc aPointDiag_100M = GenerateGeometry<N, vector<PointND<N>>>([&] { return CreatePoints_Random<N, 100 * N1M>(); }, szName, 100, report);
    autoc vTask = GeneratePointTasks<N>(nDepth, szName, aPointDiag_100M);
    RunTasks(vTask, report);
  }

  {
    autoc szName = string("Cylindrical semi-random placed points");
    autoc aPointDiag_100M = GenerateGeometry<N, vector<PointND<N>>>([&] { return CreatePoints_CylindricalSemiRandom<N, 100 * N1M>(); }, szName, 100, report);
    autoc vTask = GeneratePointTasks<N>(nDepth, szName, aPointDiag_100M);
    RunTasks(vTask, report);
  }

  {
    autoc szName = string("Diagonally placed boxes");
    autoc aPointDiag_100M = GenerateGeometry<N, vector<BoundingBoxND<N>>>([&] { return CreateBoxes_Diagonal<N, 100 * N1M>(); }, szName, 100, report);
    autoc vTask = GenerateBoxTasks<N>(nDepth, szName, aPointDiag_100M);
    RunTasks(vTask, report);
  }

  {
    autoc szName = string("Random placed boxes");
    autoc aPointDiag_100M = GenerateGeometry<N, vector<BoundingBoxND<N>>>([&] { return CreateBoxes_Random<N, 100 * N1M>(); }, szName, 100, report);
    autoc vTask = GenerateBoxTasks<N>(nDepth, szName, aPointDiag_100M);
    RunTasks(vTask, report);
  }

  {
    autoc szName = string("Cylindrical semi-random placed boxes");
    autoc aPointDiag_100M = GenerateGeometry<N, vector<BoundingBoxND<N>>>([&] { return CreateBoxes_CylindricalSemiRandom<N, 100 * N1M>(); }, szName, 100, report);
    autoc vTask = GenerateBoxTasks<N>(nDepth, szName, aPointDiag_100M);
    RunTasks(vTask, report);
  }

  // Morton vs Dynamic
  {
    autoc szName = string("Cylindrical semi-random placed points Morton vs Dynamic");
    autoc aPoint = GenerateGeometry<N, vector<PointND<N>>>([&] { return CreatePoints_Random<N, 100*N1M>(); }, szName, 100, report);
    autoc vTaskMorton = GeneratePointTasks<N>(nDepth, "Morton", aPoint);
    autoc vTaskNaive = GeneratePointDynTasks_NonLog<N>(nDepth, "Dynamic", aPoint);
    
    RunTasks(vTaskMorton, report);
    RunTasks(vTaskNaive, report);
  }

  // Search

  // Range search: Brute force vs Octree
  {
    autoc szName = string("Search: Cylindrical semi-random placed point NoPt/NoBox:10%");
    autoc aPoint = GenerateGeometry<N, vector<PointND<N>>>([&] { return CreatePoints_CylindricalSemiRandom<N, N1M>(); }, szName, N1M, report);
    autoce M = 0.1;
    autoc aSearchBox = GenerateGeometry<N, vector<BoundingBoxND<N>>>([&] { return CreateBoxes_CylindricalSemiRandom<N, static_cast<size_t>(N1M * M)>(); }, szName, M, report);
    autoce rPercentage = 10.0;
    autoc vTaskBruteForce = SearchBruteForcePointTasks<N>(nDepth, "Point range-search naive", aPoint, aSearchBox, rPercentage);
    autoc vTaskTree = SearchTreePointTasks<N>(nDepth, "Point range-search by octree", aPoint, aSearchBox, rPercentage);

    RunTasks(vTaskBruteForce, report);
    RunTasks(vTaskTree, report);
  }

  report.close();
}
