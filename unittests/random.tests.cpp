#include "pch.h"

using namespace OrthoTree;

using std::array;
using std::vector;

namespace Microsoft
{
  namespace VisualStudio
  {
    namespace CppUnitTestFramework
    {
      template<>
      [[maybe_unused]] inline std::wstring ToString<std::bitset<65>>(const std::bitset<65>& t)
      {
        RETURN_WIDE_STRING(t);
      }

    } // namespace CppUnitTestFramework
  }   // namespace VisualStudio
} // namespace Microsoft

namespace RandomTests
{
  template<int DIMENSION_NO, typename TGeometry>
  static std::vector<PointND<DIMENSION_NO, TGeometry>> CreatePoints_Random(size_t nNumber, TGeometry rMin, TGeometry rMax)
  {
    auto aPoint = vector<PointND<DIMENSION_NO, TGeometry>>(nNumber);
    if (nNumber <= 1)
      return aPoint;

    size_t iNumber = 2;

    auto const length = rMax - rMin;
    auto ptMin = PointND<DIMENSION_NO, TGeometry>{};
    for (int iDim = 0; iDim < DIMENSION_NO; ++iDim)
      ptMin[iDim] = rMin;


    auto ptMax = PointND<DIMENSION_NO, TGeometry>{};
    for (int iDim = 0; iDim < DIMENSION_NO; ++iDim)
      ptMax[iDim] = rMax;


    // Corner points
    {
      aPoint[0] = ptMin;
      aPoint[1] = ptMax;
      ++iNumber;

      for (int iDim = 0; iDim < DIMENSION_NO && iNumber < nNumber; ++iDim, ++iNumber)
      {
        aPoint[iNumber].fill(rMin);
        aPoint[iNumber][iDim] = rMax;
      }
    }

    {
      for (; iNumber < nNumber; ++iNumber)
        for (int iDim = 0; iDim < DIMENSION_NO; ++iDim)
          aPoint[iNumber][iDim] = rMin + TGeometry(double(rand() % 1000) * (double(length) * 0.001));
    }
    /*
    [[maybe_unused]] auto const box = Adaptor::GetBoxOfPoints(aPoint);
    assert(Adaptor::ArePointsEqual(box.Max, ptMax, 0.0001));
    assert(Adaptor::ArePointsEqual(box.Min, ptMin, 0.0001));
    */
    return aPoint;
  }


  template<int DIMENSION_NO, typename TGeometry>
  static vector<BoundingBoxND<DIMENSION_NO, TGeometry>> CreateBoxes_Random(size_t nNumber, TGeometry rMin, TGeometry rMax)
  {
    if (nNumber == 0)
      return {};

    auto const rSize = rMax - rMin;
    auto const rUnit = std::min(TGeometry(1), rSize);
    auto aBox = vector<BoundingBoxND<DIMENSION_NO, TGeometry>>(nNumber);

    for (int iDim = 0; iDim < DIMENSION_NO; ++iDim)
    {
      aBox[0].Min[iDim] = rMin;
      aBox[0].Max[iDim] = rMax;
    }
    if (nNumber == 1)
      return aBox;

    size_t iNumber = 1;

    // Corner points
    {
      for (int iDim = 0; iDim < DIMENSION_NO && iNumber < nNumber; ++iDim, ++iNumber)
      {
        aBox[iNumber].Min.fill(rMin);
        aBox[iNumber].Min[iDim] = rMax - rUnit;
        aBox[iNumber].Max.fill(rMin + rUnit);
        aBox[iNumber].Max[iDim] = rMax;
      }
      if (iNumber == nNumber)
        return aBox;


      for (int iDim = 0; iDim < DIMENSION_NO; ++iDim)
        aBox[iNumber].Min[iDim] = rMax - rUnit;

      aBox[iNumber].Max.fill(rMax);

      ++iNumber;
    }

    {
      auto const getRandom = [&] {
        auto const randomInt = rand() % 1000;
        auto const scaledRandom = TGeometry(double(randomInt) * double(rSize) * 0.001);
        return scaledRandom;
      };

      auto const getRandomSize = [&] {
        return std::max<TGeometry>(rUnit, getRandom());
      };

      auto const getRandomPos = [&] {
        return rMin + getRandom();
      };

      for (; iNumber < nNumber; ++iNumber)
      {
        struct EType
        {
          enum : int
          {
            Free,
            Cubic,
            Limit__1_2,
            Limit__1_5,
            Limit__2_0,
            Limit__4_0,
            Max
          };
        };
        auto const type = (rand() % EType::Max);

        auto sizes = std::array<TGeometry, DIMENSION_NO>{};
        sizes[0] = getRandomSize();
        for (int iDim = 1; iDim < DIMENSION_NO; ++iDim)
        {
          if (type == EType::Cubic)
          {
            sizes[iDim] = sizes[0];
            continue;
          }
          auto const rSizeDim = getRandomSize();
          auto const getClampedSize = [&](double factor) {
            return TGeometry(std::clamp(double(rSizeDim), double(sizes[0]) / factor, double(sizes[0]) * factor));
          };
          switch (type)
          {
          case EType::Free: sizes[iDim] = rSizeDim; break;
          case EType::Cubic: sizes[iDim] = sizes[0]; break;
          case EType::Limit__1_2: sizes[iDim] = getClampedSize(1.2); break;
          case EType::Limit__1_5: sizes[iDim] = getClampedSize(1.5); break;
          case EType::Limit__2_0: sizes[iDim] = getClampedSize(2.0); break;
          case EType::Limit__4_0: sizes[iDim] = getClampedSize(4.0); break;
          }
        }

        auto& box = aBox[iNumber];
        for (int iDim = 0; iDim < DIMENSION_NO; ++iDim)
        {
          box.Min[iDim] = std::min(getRandomPos(), rMax - sizes[iDim]);
          box.Max[iDim] = std::min(box.Min[iDim] + sizes[iDim], rMax);
        }
      }
    }

    return aBox;
  }

  /*
  template<typename TVector, typename TBox, typename TRay, typename TPlane, typename execution_policy_type = std::execution::unsequenced_policy>
  std::vector<std::pair<std::size_t, std::size_t>> SelfConflicthNaive(std::span<TBox const> const& vBox)
  {
    auto const nEntity = vBox.size();

    auto vidCheck = vector<std::size_t>(nEntity);
    std::iota(std::begin(vidCheck), std::end(vidCheck), 0);

    auto vvidCollision = vector<vector<std::size_t>>(vidCheck.size());
    auto ep = execution_policy_type{};
    std::transform(ep, std::begin(vidCheck), std::end(vidCheck), std::begin(vvidCollision), [&](auto const idCheck) -> vector<std::size_t> {
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
  */

  template<int DIMENSION_NO, typename TVector, typename TBox, typename TRay, typename TPlane, typename TGeometry>
  std::vector<std::size_t> RangeSearchNaive(TBox const& boxSearch, std::span<TBox const> const& vBox)
  {
    auto const n = vBox.size();
    auto vElementFound = vector<std::size_t>();
    vElementFound.reserve(n);

    for (size_t i = 0; i < n; ++i)
      if (AdaptorGeneral<DIMENSION_NO, TVector, TBox, TRay, TPlane, TGeometry>::AreBoxesOverlapped(boxSearch, vBox[i], false))
        vElementFound.emplace_back(i);

    return vElementFound;
  }


  template<int DIMENSION_NO, typename TVector, typename TBox, typename TRay, typename TPlane, typename TGeometry>
  std::vector<std::size_t> RangeSearchNaive(TBox const& searchBox, std::span<TVector const> const& points)
  {
    auto const n = points.size();
    auto vElementFound = vector<std::size_t>();
    vElementFound.reserve(n);

    for (size_t i = 0; i < n; ++i)
      if (AdaptorGeneral<DIMENSION_NO, TVector, TBox, TRay, TPlane, TGeometry>::DoesBoxContainPoint(searchBox, points[i]))
        vElementFound.emplace_back(i);

    return vElementFound;
  }

  TEST_CLASS(RandomTests){
    private: template<int DIMENSION_NO, typename TGeometry>
    void PointND_RangeSearch(size_t pointsNo, size_t boxesNo, TGeometry rMin, TGeometry rMax){ using TVector = VectorND<DIMENSION_NO, TGeometry>;
  using TBox = BoundingBoxND<DIMENSION_NO, TGeometry>;
  using TRay = RayND<DIMENSION_NO, TGeometry>;
  using TPlane = PlaneND<DIMENSION_NO, TGeometry>;

  auto const points = CreatePoints_Random<DIMENSION_NO, TGeometry>(pointsNo, rMin, rMax);
  auto const boxes = CreateBoxes_Random<DIMENSION_NO, TGeometry>(boxesNo, rMin, rMax);

  auto const treeCore = OrthoTree::TreePointND<DIMENSION_NO, TGeometry>(points);
  auto const treeContainer = OrthoTree::TreePointContainerND<DIMENSION_NO, TGeometry>(points);

  for (auto const& box : boxes)
  {
    auto elementIDsTreeCore = treeCore.RangeSearch(box, points);
    auto const elementIDsTreeContainer = treeContainer.RangeSearch(box);
    Assert::IsTrue(elementIDsTreeCore == elementIDsTreeContainer);

    auto const elementIDsNaive = RangeSearchNaive<DIMENSION_NO, TVector, TBox, TRay, TPlane, TGeometry>(box, points);
    std::ranges::sort(elementIDsTreeCore);
    assert(elementIDsNaive == elementIDsTreeCore);
    Assert::IsTrue(elementIDsNaive == elementIDsTreeCore);
  }
} // namespace RandomTests

template<typename TGeometry>
void PointT_RangeSearch()
{
  auto constexpr isPlatform64 = sizeof(std::size_t) == 8;

  for (auto const pointsNo : { 32, 100, 1000 })
  {
    auto const boxNo = std::min(300, std::max(4, pointsNo / 10));

    auto const rMin = TGeometry(rand() % 110 - 50);
    auto const rLength = TGeometry(20.0 + double(rand() % 100));
    auto const rMax = TGeometry(rMin + rLength);

    PointND_RangeSearch<1, TGeometry>(pointsNo, boxNo, rMin, rMax);
    PointND_RangeSearch<2, TGeometry>(pointsNo, boxNo, rMin, rMax);
    PointND_RangeSearch<3, TGeometry>(pointsNo, boxNo, rMin, rMax);
    PointND_RangeSearch<4, TGeometry>(pointsNo, boxNo, rMin, rMax);
    PointND_RangeSearch<6, TGeometry>(pointsNo, boxNo, rMin, rMax);
    PointND_RangeSearch<8, TGeometry>(pointsNo, boxNo, rMin, rMax);
    PointND_RangeSearch<16, TGeometry>(pointsNo, boxNo, rMin, rMax);
    PointND_RangeSearch<31, TGeometry>(pointsNo, boxNo, rMin, rMax);
    if constexpr (isPlatform64)
    {
      PointND_RangeSearch<32, TGeometry>(pointsNo, boxNo, rMin, rMax);
      PointND_RangeSearch<63, TGeometry>(pointsNo, boxNo, rMin, rMax);
    }
  }
}

template<int DIMENSION_NO, uint32_t SPLIT_DEPTH_INCREASEMENT, typename TGeometry>
void BoxND_RangeSearch(size_t boxNo, size_t searchboxNo, TGeometry rMin, TGeometry rMax)
{
  using TVector = VectorND<DIMENSION_NO, TGeometry>;
  using TBox = BoundingBoxND<DIMENSION_NO, TGeometry>;
  using TRay = RayND<DIMENSION_NO, TGeometry>;
  using TPlane = PlaneND<DIMENSION_NO, TGeometry>;

  constexpr bool DO_SPLIT_PARENT_ENTITIES = SPLIT_DEPTH_INCREASEMENT > 0;

  auto const entityBoxes = CreateBoxes_Random<DIMENSION_NO, TGeometry>(boxNo, rMin, rMax);
  auto const searchBoxes = CreateBoxes_Random<DIMENSION_NO, TGeometry>(searchboxNo, rMin, rMax);

  auto const treeCore = OrthoTree::TreeBoxND<DIMENSION_NO, DO_SPLIT_PARENT_ENTITIES, TGeometry>(entityBoxes);
  auto const treeContainer = OrthoTree::TreeBoxContainerND<DIMENSION_NO, DO_SPLIT_PARENT_ENTITIES, TGeometry>(entityBoxes);
  for (auto const& searchBox : searchBoxes)
  {
    auto elementIDsTreeCore = treeCore.template RangeSearch<false>(searchBox, entityBoxes);
    auto const elementIDsTreeContainer = treeContainer.template RangeSearch<false>(searchBox);
    Assert::IsTrue(elementIDsTreeCore == elementIDsTreeContainer);

    auto const elementIDsNaive = RangeSearchNaive<DIMENSION_NO, TVector, TBox, TRay, TPlane, TGeometry>(searchBox, entityBoxes);
    std::ranges::sort(elementIDsTreeCore);
    assert(elementIDsNaive == elementIDsTreeCore);
    Assert::IsTrue(elementIDsNaive == elementIDsTreeCore);
  }
}

template<typename TGeometry>
void BoxT_RangeSearch()
{
  auto constexpr isPlatform64 = sizeof(std::size_t) == 8;

  for (auto const pointsNo : { 32, 100, 1000 })
  {
    auto const boxNo = std::min(300, std::max(4, pointsNo / 10));

    auto const rMin = TGeometry(rand() % 110 - 50);
    auto const rLength = TGeometry(20.0 + double(rand() % 100));
    auto const rMax = TGeometry(rMin + rLength);

    BoxND_RangeSearch<1, 0, TGeometry>(pointsNo, boxNo, rMin, rMax);
    BoxND_RangeSearch<2, 0, TGeometry>(pointsNo, boxNo, rMin, rMax);
    BoxND_RangeSearch<3, 0, TGeometry>(pointsNo, boxNo, rMin, rMax);
    BoxND_RangeSearch<4, 0, TGeometry>(pointsNo, boxNo, rMin, rMax);
    BoxND_RangeSearch<6, 0, TGeometry>(pointsNo, boxNo, rMin, rMax);
    BoxND_RangeSearch<8, 0, TGeometry>(pointsNo, boxNo, rMin, rMax);
    BoxND_RangeSearch<16, 0, TGeometry>(pointsNo, boxNo, rMin, rMax);
    BoxND_RangeSearch<31, 0, TGeometry>(pointsNo, boxNo, rMin, rMax);
    if constexpr (isPlatform64)
    {
      BoxND_RangeSearch<32, 0, TGeometry>(pointsNo, boxNo, rMin, rMax);
      BoxND_RangeSearch<63, 0, TGeometry>(pointsNo, boxNo, rMin, rMax);
    }

    BoxND_RangeSearch<1, 1, TGeometry>(pointsNo, boxNo, rMin, rMax);
    BoxND_RangeSearch<2, 1, TGeometry>(pointsNo, boxNo, rMin, rMax);
    BoxND_RangeSearch<3, 1, TGeometry>(pointsNo, boxNo, rMin, rMax);
    BoxND_RangeSearch<4, 1, TGeometry>(pointsNo, boxNo, rMin, rMax);
    BoxND_RangeSearch<6, 1, TGeometry>(pointsNo, boxNo, rMin, rMax);
    BoxND_RangeSearch<8, 1, TGeometry>(pointsNo, boxNo, rMin, rMax);
    BoxND_RangeSearch<16, 1, TGeometry>(pointsNo, boxNo, rMin, rMax);
    BoxND_RangeSearch<31, 1, TGeometry>(pointsNo, boxNo, rMin, rMax);
    if constexpr (isPlatform64)
    {
      BoxND_RangeSearch<32, 1, TGeometry>(pointsNo, boxNo, rMin, rMax);
      BoxND_RangeSearch<63, 1, TGeometry>(pointsNo, boxNo, rMin, rMax);
    }
  }
}

public:

BEGIN_TEST_METHOD_ATTRIBUTE(Point_RangeSearch)
TEST_IGNORE()
END_TEST_METHOD_ATTRIBUTE()
TEST_METHOD(Point_RangeSearch)
{
  srand(0);
  /*
  PointT_RangeSearch<double>();
  PointT_RangeSearch<float>();
  PointT_RangeSearch<int>();
  */
}
BEGIN_TEST_METHOD_ATTRIBUTE(Box_RangeSearch)
TEST_IGNORE()
END_TEST_METHOD_ATTRIBUTE()
TEST_METHOD(Box_RangeSearch)
{
  srand(0);
  /*
  BoxT_RangeSearch<double>();
  BoxT_RangeSearch<float>();
  BoxT_RangeSearch<int>();
  */
}
}
;
}