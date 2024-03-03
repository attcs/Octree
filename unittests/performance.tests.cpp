#include "pch.h"

using namespace OrthoTree;

using std::array;
using std::vector;

#ifdef _M_X64

namespace PerformaceTest
{
#ifdef _DEBUG
  autoce N1M = 1000;
#else
  autoce N1M = 1000000;
#endif // _DEBUG

  autoce rMax = 8.0;


  template<size_t nDim>
  static constexpr BoundingBoxND<nDim> CreateBox(PointND<nDim> const& pt, double size)
  {
    using Ad = AdaptorGeneral<nDim, PointND<nDim>, BoundingBoxND<nDim>, RayND<nDim>, PlaneND<nDim>>;
    auto Box = BoundingBoxND<nDim>{ pt, pt };
    for (size_t iDim = 0; iDim < nDim; ++iDim)
      Ad::SetBoxMaxC(Box, static_cast<dim_t>(iDim), Ad::GetBoxMinC(Box, static_cast<dim_t>(iDim)) + size);

    return Box;
  }


  template<dim_t nDim>
  static BoundingBoxND<nDim> CreateSearcBox(double rBegin, double rSize)
  {
    auto pt = PointND<nDim>{};
    for (dim_t iDim = 0; iDim < nDim; ++iDim)
      pt[iDim] = rBegin;

    return CreateBox<nDim>(pt, rSize);
  }


  template<dim_t nDim, size_t nNumber>
  static vector<PointND<nDim>> CreatePoints()
  {
    auto aPoint = vector<PointND<nDim>>(nNumber);
    if constexpr (nNumber <= 1)
      return aPoint;

    size_t iNumber = 1;

    // Corner points
    {
      for (dim_t iDim = 0; iDim < nDim && iNumber < nNumber; ++iDim, ++iNumber)
        aPoint[iNumber][iDim] = rMax;

      if (iNumber == nNumber)
        return aPoint;

      for (dim_t iDim = 0; iDim < nDim; ++iDim)
        aPoint[iNumber][iDim] = rMax;

      ++iNumber;
    }

    // Angle points
    {
      autoc nRemain = nNumber - iNumber;
      autoc rStep = rMax / (nRemain + 2);
      for (size_t iRemain = 1; iNumber < nNumber; ++iNumber, ++iRemain)
        for (dim_t iDim = 0; iDim < nDim; ++iDim)
          aPoint[nNumber - iNumber - 1][iDim] = iRemain * rStep;

    }

    return aPoint;
  }

  template<dim_t nDim, size_t nNumber>
  static vector<PointND<nDim>> CreateRandomPoints()
  {
    auto aPoint = vector<PointND<nDim>>(nNumber);
    if constexpr (nNumber <= 1)
      return aPoint;

    size_t iNumber = 1;

    // Corner points
    {
      for (dim_t iDim = 0; iDim < nDim && iNumber < nNumber; ++iDim, ++iNumber)
        aPoint[iNumber][iDim] = rMax;

      if (iNumber == nNumber)
        return aPoint;

      for (dim_t iDim = 0; iDim < nDim; ++iDim)
        aPoint[iNumber][iDim] = rMax;

      ++iNumber;
    }

    srand(0);
    {
      for (size_t iRemain = 1; iNumber < nNumber; ++iNumber, ++iRemain)
        for (dim_t iDim = 0; iDim < nDim && iNumber < nNumber; ++iDim)
          aPoint[nNumber - iNumber - 1][iDim] = (rand() % 100) * (rMax / 100.0);

    }

    return aPoint;
  }


  template<dim_t nDim, size_t nNumber>
  static vector<BoundingBoxND<nDim>> CreateBoxes()
  {
    if constexpr (nNumber == 0)
      return {};

    autoce rUnit = 1.0;
    auto aBox = vector<BoundingBoxND<nDim>>(nNumber);
    aBox[0] = CreateBox(PointND<nDim>(), rMax);
    if constexpr (nNumber == 1)
      return aBox;

    size_t iNumber = 1;

    // Corner points
    {
      for (dim_t iDim = 0; iDim < nDim && iNumber < nNumber; ++iDim, ++iNumber)
      {
        aBox[iNumber].Min[iDim] = rMax - rUnit;
        aBox[iNumber] = CreateBox(aBox[iNumber].Min, rUnit);
      }
      if (iNumber == nNumber)
        return aBox;


      for (dim_t iDim = 0; iDim < nDim; ++iDim)
        aBox[iNumber].Min[iDim] = rMax - rUnit;

      aBox[iNumber] = CreateBox(aBox[iNumber].Min, rUnit);

      ++iNumber;
    }

    // Angle points
    {
      autoc nRemain = nNumber - iNumber;
      autoc rStep = (rMax - rUnit) / (nRemain + 2);
      for (size_t iRemain = 1; iNumber < nNumber; ++iNumber, ++iRemain)
      {
        autoc iNumberBox = nNumber - iNumber - 1;
        for (dim_t iDim = 0; iDim < nDim && iNumber < nNumber; ++iDim)
          aBox[iNumberBox].Min[iDim] = iRemain * rStep;

        aBox[iNumberBox] = CreateBox(aBox[iNumberBox].Min, rUnit);
      }
    }

    return aBox;
  }

  template<dim_t nDim, size_t nNumber>
  static vector<BoundingBoxND<nDim>> CreateRandomBoxes()
  {
    if constexpr (nNumber == 0)
      return {};

    autoce rUnit = 1.0;
    auto aBox = vector<BoundingBoxND<nDim>>(nNumber);
    aBox[0] = CreateBox(PointND<nDim>(), rMax);
    if constexpr (nNumber == 1)
      return aBox;

    size_t iNumber = 1;

    // Corner points
    {
      for (dim_t iDim = 0; iDim < nDim && iNumber < nNumber; ++iDim, ++iNumber)
      {
        aBox[iNumber].Min[iDim] = rMax - rUnit;
        aBox[iNumber] = CreateBox(aBox[iNumber].Min, rUnit);
      }
      if (iNumber == nNumber)
        return aBox;


      for (dim_t iDim = 0; iDim < nDim; ++iDim)
        aBox[iNumber].Min[iDim] = rMax - rUnit;

      aBox[iNumber] = CreateBox(aBox[iNumber].Min, rUnit);

      ++iNumber;
    }

    srand(0);

    {
      for (size_t iRemain = 1; iNumber < nNumber; ++iNumber, ++iRemain)
      {
        autoc iNumberBox = nNumber - iNumber - 1;
        for (dim_t iDim = 0; iDim < nDim && iNumber < nNumber; ++iDim)
          aBox[iNumberBox].Min[iDim] = (rand() % 100) * (rMax / 100.0);

        aBox[iNumberBox] = CreateBox(aBox[iNumberBox].Min, (rand() % 100) * (4.0 * rUnit / 100.0));
      }
    }

    return aBox;
  }


  namespace PreCalculated
  {
    // Points

    autoc sPoint2D_10M = CreatePoints<2, 10 * N1M>();
    autoc aPoint2D_10M = std::span(sPoint2D_10M);
    autoc aPoint2D_1M = aPoint2D_10M.subspan(0, N1M);

    autoc sPoint3D_10M = CreatePoints<3, 10*N1M>();
    autoc aPoint3D_10M = std::span(sPoint3D_10M);
    autoc aPoint3D_1M = aPoint3D_10M.subspan(0, N1M);
    autoc aPoint3D_10MR = CreateRandomPoints<3, 10 * N1M>();

    autoc aPoint4D_1M = CreatePoints<4, N1M>();

    autoc aPoint16D_1M = CreatePoints<16, N1M>();
    autoc aPoint63D_1M = CreatePoints<63, N1M>();

    autoc TreePoint2D = QuadtreePoint(aPoint2D_10M, 4);
    autoc TreePoint3D = OctreePoint(aPoint3D_10M, 4);


    // Boxes

    autoc sBox2D_10M = CreateBoxes<2, 10*N1M>();
    autoc aBox2D_10M = std::span(sBox2D_10M);
    autoc aBox2D_1M = aBox2D_10M.subspan(0, N1M);

    autoc sBox3D_10M = CreateBoxes<3, 10*N1M>();
    autoc aBox3D_10M = std::span(sBox3D_10M);
    autoc aBox3D_1M = aBox3D_10M.subspan(0, N1M);
    autoc aBox3D_10MR = CreateRandomBoxes<3, 10 * N1M>();

    autoc aBox4D_1M = CreateBoxes<4, N1M>();
    autoc aBox63D_1M = CreateBoxes<63, N1M>();

    autoc box2D = BoundingBox2D{ Point2D{}, Point2D{ rMax, rMax} };
    autoc box3D = BoundingBox3D{ Point3D{}, Point3D{ rMax, rMax, rMax} };
    autoc TreeBox2D_10M = QuadtreeBox(aBox2D_10M, 4, box2D);
    autoc TreeBox3D_10M = OctreeBox(aBox3D_10M, 4, box3D);
  }


  TEST_CLASS(PointTest)
  {
  private:

    template<dim_t nDim>
    static TreePointND<nDim> CreateTest(depth_t depth, std::span<PointND<nDim> const> const& aPoint, bool fPar = false)
    {
      autoc Box = CreateSearcBox<nDim>(0.0, rMax);

      auto nt = TreePointND<nDim>{};
      if (fPar)
        TreePointND<nDim>::template Create<std::execution::parallel_unsequenced_policy>(nt, aPoint, depth, Box);
      else
        TreePointND<nDim>::Create(nt, aPoint, depth, Box);

      return nt;
    }

  public:
    TEST_METHOD(Create_2D_1M_d3) { CreateTest<2>(3, PreCalculated::aPoint2D_1M); }
    TEST_METHOD(Create_2D_1M_d4) { CreateTest<2>(4, PreCalculated::aPoint2D_1M); }
    TEST_METHOD(Create_3D_1M_d3) { CreateTest<3>(3, PreCalculated::aPoint3D_1M); }
    TEST_METHOD(Create_3D_1M_d4) { CreateTest<3>(4, PreCalculated::aPoint3D_1M); }
    TEST_METHOD(Create_3D_10M_d4_S) { CreateTest<3>(4, PreCalculated::sPoint3D_10M); }
    TEST_METHOD(Create_3D_10M_d4_S_PAR) { CreateTest<3>(4, PreCalculated::aPoint3D_10M, true); }
    TEST_METHOD(Create_3D_10M_d4_R) { CreateTest<3>(4, PreCalculated::aPoint3D_10MR); }
    TEST_METHOD(Create_3D_10MR_d4_R_PAR) { CreateTest<3>(4, PreCalculated::aPoint3D_10MR, true); }

    TEST_METHOD(Create_4D_1M_d2) { CreateTest<4>(2, PreCalculated::aPoint4D_1M); }
    TEST_METHOD(Create_16D_1M_d3) { CreateTest<16>(3, PreCalculated::aPoint16D_1M, true); }
    TEST_METHOD(Create_63D_1M_d3) { CreateTest<63>(3, PreCalculated::aPoint63D_1M); }

    TEST_METHOD(RangeSearch_2D_10M)
    {
      autoce n = 100;

      autoc search_box = CreateSearcBox<2>(3.5, 1.0);
      auto vvid = vector<vector<size_t>>(n);
      for (int i = 0; i < n; ++i)
        vvid[i] = PreCalculated::TreePoint2D.RangeSearch(search_box, PreCalculated::aPoint2D_10M);
    }
    TEST_METHOD(RangeSearch_3D_10M)
    {
      autoce n = 100;

      autoc search_box = CreateSearcBox<3>(3.5, 1.0);
      auto vvid = vector<vector<size_t>>(n);
      for (int i = 0; i < n; ++i)
        vvid[i] = PreCalculated::TreePoint3D.RangeSearch(search_box, PreCalculated::aPoint3D_10M);
    }

  };


  TEST_CLASS(BoxTest)
  {
  private:
    template<dim_t nDim>
    static auto CreateTest(depth_t depth, std::span<BoundingBoxND<nDim> const> const& aBox, bool fPar = false)
    {
      autoc Box = CreateSearcBox<nDim>(0.0, rMax);

      auto nt = TreeBoxND<nDim>{};
      if (fPar)
        TreeBoxND<nDim>::template Create<std::execution::parallel_unsequenced_policy>(nt, aBox, depth, Box);
      else
        TreeBoxND<nDim>::Create(nt, aBox, depth, Box);

      return nt;
    }


  public:
    TEST_METHOD(Create_2D_1M_d3) { CreateTest(3, PreCalculated::aBox2D_1M); }
    TEST_METHOD(Create_2D_1M_d4) { CreateTest(4, PreCalculated::aBox2D_1M); }

    TEST_METHOD(Create_3D_1M_d3) { CreateTest(3, PreCalculated::aBox3D_1M); }
    TEST_METHOD(Create_3D_1M_d4) { CreateTest(4, PreCalculated::aBox3D_1M); }
    TEST_METHOD(Create_3D_10M_d4_S) { CreateTest(4, PreCalculated::aBox3D_10M); }
    TEST_METHOD(Create_3D_10M_d4_S_PAR) { CreateTest<3>(4, PreCalculated::aBox3D_10M, true); }
    TEST_METHOD(Create_3D_10M_d4_R) { CreateTest<3>(4, PreCalculated::aBox3D_10MR); }
    TEST_METHOD(Create_3D_10M_d4_R_PAR) { CreateTest<3>(4, PreCalculated::aBox3D_10MR, true); }

    TEST_METHOD(Create_4D_1M_d4) { CreateTest<4>(4, PreCalculated::aBox4D_1M); }

    TEST_METHOD(Create_63D_1M_d4) { CreateTest<63>(4, PreCalculated::aBox63D_1M, true); }

    TEST_METHOD(RangeSearch_2D_10M)
    {
      autoce nDim = 2;
      autoce n = 10;

      autoc search_box = CreateSearcBox<nDim>(3.5, 1.0);
      auto vvid = vector<vector<size_t>>(n);
      for (int i = 0; i < n; ++i)
        vvid[i] = PreCalculated::TreeBox2D_10M.RangeSearch(search_box, PreCalculated::aBox2D_10M);
    }

    TEST_METHOD(RangeSearch_3D_10M)
    {
      autoce nDim = 3;
      autoce n = 10;

      autoc search_box = CreateSearcBox<nDim>(3.5, 1.0);
      auto vvid = vector<vector<size_t>>(n);
      for (int i = 0; i < n; ++i)
        vvid[i] = PreCalculated::TreeBox3D_10M.RangeSearch(search_box, PreCalculated::aBox3D_10MR);
    }
  };
}

#endif