#include "pch.h"


namespace PerformaceTest
{

  TEST_CLASS(PointTest)
  {
    template<dim_type nDim, size_t nNumber>
    static vector<PointND<nDim>> CreatePoints()
    {
      auto aPoint = vector<PointND<nDim>>(nNumber);
      if (nNumber <= 1)
        return aPoint;

      size_t iNumber = 1;
      autoce rMax = 8.0;

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

    template<dim_type nDim, class execution_policy_type = std::execution::unsequenced_policy>
    static TreePointND<nDim> CreateTest(unsigned depth, vector<PointND<nDim>> const& aPoint)
    {
      auto nt = TreePointND<nDim>::template Create<execution_policy_type>(aPoint, depth);
      return nt;
    }

  private:
    vector<PointND<2>> _aPoint2D_1000000;
    vector<PointND<3>> _aPoint3D_1000000;
    vector<PointND<3>> _aPoint3D_10000000;
    vector<PointND<4>> _aPoint4D_1000000;
  public:
    PointTest()
      : _aPoint2D_1000000(CreatePoints<2, 1000000>())
      , _aPoint3D_1000000(CreatePoints<3, 1000000>())
      , _aPoint3D_10000000(CreatePoints<3, 10000000>())
      , _aPoint4D_1000000(CreatePoints<4, 1000000>())
    {}

    TEST_METHOD(Create_2D_1000000_depth3) { CreateTest(3, _aPoint2D_1000000); }
    TEST_METHOD(Create_2D_1000000_depth4) { CreateTest(4, _aPoint2D_1000000); }
    TEST_METHOD(Create_3D_1000000_depth3) { CreateTest(3, _aPoint3D_1000000); }
    TEST_METHOD(Create_3D_1000000_depth4) { CreateTest(4, _aPoint3D_1000000); }
    TEST_METHOD(Create_3D_10000000_depth4_unseq) { CreateTest(4, _aPoint3D_10000000); }
    TEST_METHOD(Create_3D_10000000_depth4_par_unseq) { CreateTest<3, std::execution::parallel_unsequenced_policy>(4, _aPoint3D_10000000); }

    TEST_METHOD(Create_4D_1000000_depth2) { CreateTest(2, _aPoint4D_1000000); }
  };


  TEST_CLASS(BoxTest)
  {
    template<dim_type nDim>
    static BoundingBoxND<nDim> CreateBox(PointND<nDim> const& pt, double size)
    {
      auto box = BoundingBoxND<nDim>{ pt, pt };
      for (dim_type iDim = 0; iDim < nDim; ++iDim)
        box.Max[iDim] += size;

      return box;
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

    template<dim_type nDim, class execution_policy_type = std::execution::unsequenced_policy>
    static auto CreateTest(unsigned depth, vector<BoundingBoxND<nDim>> const& aBox)
    {
      auto n2t = OctreeBox::Create<execution_policy_type>({}, depth);

      auto nt = TreeBoxND<nDim>::template Create<execution_policy_type>(aBox, depth);
      return nt;
    }

  private:
    vector<BoundingBoxND<2>> _aBox2D_1000000;
    vector<BoundingBoxND<3>> _aBox3D_1000000;
    vector<BoundingBoxND<3>> _aBox3D_10000000;
    vector<BoundingBoxND<4>> _aBox4D_1000000;

  public:
    BoxTest()
      : _aBox2D_1000000(CreateBoxes<2, 1000000>())
      , _aBox3D_1000000(CreateBoxes<3, 1000000>())
      , _aBox3D_10000000(CreateBoxes<3, 10000000>())
      , _aBox4D_1000000(CreateBoxes<4, 1000000>())
    {}
    TEST_METHOD(Create_2D_1000000_depth3) { CreateTest(3, _aBox2D_1000000); }
    TEST_METHOD(Create_2D_1000000_depth4) { CreateTest(4, _aBox2D_1000000); }

    TEST_METHOD(Create_3D_1000000_depth3) { CreateTest(3, _aBox3D_1000000); }
    TEST_METHOD(Create_3D_1000000_depth4) { CreateTest(4, _aBox3D_1000000); }
    TEST_METHOD(Create_3D_10000000_depth4) { CreateTest(4, _aBox3D_10000000); }
    TEST_METHOD(Create_3D_10000000_depth4_par_unseq) { CreateTest<3, std::execution::parallel_unsequenced_policy>(4, _aBox3D_10000000); }

    TEST_METHOD(Create_4D_1000000_depth4) { CreateTest(4, _aBox4D_1000000); }
  };
}
