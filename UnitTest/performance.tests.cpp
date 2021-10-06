#include "pch.h"


namespace PerformaceTest
{
  autoce N1M = 1000000;
  autoce rMax = 8.0;


  template<dim_type nDim>
  static BoundingBoxND<nDim> CreateBox(PointND<nDim> const& pt, double size)
  {
    auto box = BoundingBoxND<nDim>{ pt, pt };
    for (dim_type iDim = 0; iDim < nDim; ++iDim)
      box.Max[iDim] += size;

    return box;
  }


  template<dim_type nDim>
  static BoundingBoxND<nDim> CreateSearcBox(double rBegin, double rSize)
  {
    auto pt = PointND<nDim>{};
    for (dim_type iDim = 0; iDim < nDim; ++iDim)
      pt[iDim] = rBegin;

    return CreateBox<nDim>(pt, rSize);
  }


  template<dim_type nDim, size_t nNumber>
  static vector<PointND<nDim>> CreatePoints()
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

    // Angle points
    {
      autoc nRemain = nNumber - iNumber;
      autoc rStep = rMax / (nRemain + 2);
      for (size_t iRemain = 1; iNumber < nNumber; ++iNumber, ++iRemain)
        for (dim_type iDim = 0; iDim < nDim && iNumber < nNumber; ++iDim)
          aPoint[nNumber - iNumber - 1][iDim] = iRemain * rStep;

    }

    return aPoint;
  }


  template<dim_type nDim, size_t nNumber>
  static vector<BoundingBoxND<nDim>> CreateBoxes()
  {
    if (nNumber == 0)
      return {};

    autoce rMax = 8.0;
    autoce rUnit = 1.0;
    auto aBox = vector<BoundingBoxND<nDim>>(nNumber);
    aBox[0] = CreateBox(PointND<nDim>(), rMax);
    if (nNumber == 1)
      return aBox;

    size_t iNumber = 1;

    // Corner points
    {
      for (dim_type iDim = 0; iDim < nDim && iNumber < nNumber; ++iDim, ++iNumber)
      {
        aBox[iNumber].Min[iDim] = rMax - rUnit;
        aBox[iNumber] = CreateBox(aBox[iNumber].Min, rUnit);
      }
      if (iNumber == nNumber)
        return aBox;


      for (dim_type iDim = 0; iDim < nDim; ++iDim)
        aBox[iNumber].Min[iDim] = rMax - rUnit;

      aBox[iNumber] = CreateBox(aBox[iNumber].Min, rUnit);

      ++iNumber;
    }

    // Angle points
    {
      autoc nRemain = nNumber - iNumber;
      autoc rStep = (rMax - rUnit) / (nRemain + 2);
      for (size_t iRemain = 1; iNumber < nNumber; ++iNumber, ++iRemain)
        for (dim_type iDim = 0; iDim < nDim && iNumber < nNumber; ++iDim)
        {
          autoc iNumberBox = nNumber - iNumber - 1;
          aBox[iNumberBox].Min[iDim] = iRemain * rStep;
          aBox[iNumberBox] = CreateBox(aBox[iNumberBox].Min, rUnit);
        }

    }

    return aBox;
  }


  namespace PreCalculated
  {
    autoc aPoint2D_1M = CreatePoints<2, N1M>();
    autoc aPoint2D_10M = CreatePoints<2, 10 * N1M>();
    autoc aPoint2D_100M = CreatePoints<2, 100 * N1M>();

    autoc aPoint3D_1M = CreatePoints<3, N1M>();
    autoc aPoint3D_10M = CreatePoints<3, 10*N1M>();
    autoc aPoint3D_100M = CreatePoints<3, 100*N1M>();

    autoc aPoint4D_1M = CreatePoints<4, N1M>();

    autoc aPoint16D_1M = CreatePoints<16, N1M>();

    autoc TreePoint2D = TreePointND<2>::template Create<std::execution::parallel_unsequenced_policy>(aPoint2D_100M, 4);
    autoc TreePoint3D = TreePointND<3>::template Create<std::execution::parallel_unsequenced_policy>(aPoint3D_100M, 4);

    autoc aBox2D_1M = CreateBoxes<2, N1M>();
    autoc aBox2D_10M = CreateBoxes<2, 10*N1M>();

    autoc aBox3D_1M = CreateBoxes<3, N1M>();
    autoc aBox3D_10M = CreateBoxes<3, 10*N1M>();
    autoc aBox3D_100M = CreateBoxes<3, 100*N1M>();
    autoc aBox4D_1M = CreateBoxes<4, N1M>();

    autoc TreeBox2D_10M = QuadtreeBox::Create<std::execution::parallel_unsequenced_policy>(aBox2D_10M, 4);
    autoc TreeBox3D_10M = OctreeBox::Create<std::execution::parallel_unsequenced_policy>(aBox3D_10M, 4);
    autoc TreeBox3D_100M = OctreeBox::Create<std::execution::parallel_unsequenced_policy>(aBox3D_100M, 4);
  }


  TEST_CLASS(PointTest)
  {
  private:

    template<dim_type nDim, class execution_policy_type = std::execution::unsequenced_policy>
    static TreePointND<nDim> CreateTest(unsigned depth, vector<PointND<nDim>> const& aPoint)
    {
      auto nt = TreePointND<nDim>::template Create<execution_policy_type>(aPoint, depth);
      return nt;
    }

  public:
    TEST_METHOD(Create_2D_1M_depth3) { CreateTest(3, PreCalculated::aPoint2D_1M); }
    TEST_METHOD(Create_2D_1M_depth4) { CreateTest(4, PreCalculated::aPoint2D_1M); }
    TEST_METHOD(Create_3D_1M_depth3) { CreateTest(3, PreCalculated::aPoint3D_1M); }
    TEST_METHOD(Create_3D_1M_depth4) { CreateTest(4, PreCalculated::aPoint3D_1M); }
    TEST_METHOD(Create_3D_10M_depth4_unseq) { CreateTest(4, PreCalculated::aPoint3D_10M); }
    TEST_METHOD(Create_3D_10M_depth4_par_unseq) { CreateTest<3, std::execution::parallel_unsequenced_policy>(4, PreCalculated::aPoint3D_10M); }

    TEST_METHOD(Create_4D_1M_depth2) { CreateTest(2, PreCalculated::aPoint4D_1M); }
    //TEST_METHOD(Create_16D_1M_depth2) { CreateTest(2, PreCalculated::aPoint16D_1M); }

    TEST_METHOD(RangeSearch_2D_100M)
    {
      autoc search_box = CreateSearcBox<2>(3.5, 1.0);
      autoc vid = PreCalculated::TreePoint2D.RangeSearch(search_box, PreCalculated::aPoint2D_100M);
    }
    TEST_METHOD(RangeSearch_3D_100M)
    {
      autoc search_box = CreateSearcBox<3>(3.5, 1.0);
      autoc vid = PreCalculated::TreePoint3D.RangeSearch(search_box, PreCalculated::aPoint3D_100M);
    }

  };


  TEST_CLASS(BoxTest)
  {
  private:
    template<dim_type nDim, class execution_policy_type = std::execution::unsequenced_policy>
    static auto CreateTest(unsigned depth, vector<BoundingBoxND<nDim>> const& aBox)
    {
      auto nt = TreeBoxND<nDim>::template Create<execution_policy_type>(aBox, depth);
      return nt;
    }


  public:
    TEST_METHOD(Create_2D_1M_depth3) { CreateTest(3, PreCalculated::aBox2D_1M); }
    TEST_METHOD(Create_2D_1M_depth4) { CreateTest(4, PreCalculated::aBox2D_1M); }

    TEST_METHOD(Create_3D_1M_depth3) { CreateTest(3, PreCalculated::aBox3D_1M); }
    TEST_METHOD(Create_3D_1M_depth4) { CreateTest(4, PreCalculated::aBox3D_1M); }
    TEST_METHOD(Create_3D_10M_depth4) { CreateTest(4, PreCalculated::aBox3D_10M); }
    TEST_METHOD(Create_3D_10M_depth4_par_unseq) { CreateTest<3, std::execution::parallel_unsequenced_policy>(4, PreCalculated::aBox3D_10M); }

    TEST_METHOD(Create_4D_1M_depth4) { CreateTest(4, PreCalculated::aBox4D_1M); }

    TEST_METHOD(RangeSearch_3D_10M)
    {
      autoce nDim = 3;
      autoc search_box = CreateSearcBox<nDim>(3.5, 1.0);
      autoc vid = PreCalculated::TreeBox3D_10M.RangeSearch(search_box, PreCalculated::aBox3D_10M);
    }

    TEST_METHOD(RangeSearch_3D_100M)
    {
      autoce nDim = 3;
      autoc search_box = CreateSearcBox<nDim>(3.5, 1.0);
      autoc vid = PreCalculated::TreeBox3D_100M.RangeSearch(search_box, PreCalculated::aBox3D_100M);
    }

  };
}
