#include "pch.h"

#include <charconv>
#include <filesystem>
#include <fstream>
#include <string>
#include <string_view>

using namespace OrthoTree;
using namespace OrthoTreeTestHelpers;
using std::array;
using std::vector;

namespace LongIntAdaptor
{
  using GeometryType = long int;
  template<dim_t N>
  using CustomVectorTypeND = std::array<GeometryType, N>;
  template<dim_t N>
  using CustomBoundingBoxND = std::array<CustomVectorTypeND<N>, 2>;
  template<dim_t N>
  using CustomRayND = std::array<CustomVectorTypeND<N>, 2>;
  template<dim_t N>
  using CustomPlaneND = std::tuple<CustomVectorTypeND<N>, GeometryType>;

  template<dim_t N>
  struct CustomBaseGeometryAdapter
  {
    using Scalar = GeometryType;
    using FloatScalar = float;
    using Vector = CustomVectorTypeND<N>;
    using Box = CustomBoundingBoxND<N>;
    using Ray = CustomRayND<N>;
    using Plane = CustomPlaneND<N>;

    static constexpr dim_t DIMENSION_NO = N;
    static constexpr FloatScalar BASE_TOLERANCE = std::numeric_limits<FloatScalar>::epsilon() * FloatScalar(10);

    static constexpr Scalar GetPointC(Vector const& pt, OrthoTree::dim_t iDimension) { return pt[iDimension]; }

    static constexpr void SetPointC(Vector& pt, OrthoTree::dim_t iDimension, Scalar value) { pt[iDimension] = value; }

    static constexpr void SetBoxMinC(Box& box, dim_t dimensionID, Scalar value) { SetPointC(box[0], dimensionID, value); }
    static constexpr void SetBoxMaxC(Box& box, dim_t dimensionID, Scalar value) { SetPointC(box[1], dimensionID, value); }
    static constexpr Scalar GetBoxMinC(Box const& box, dim_t dimensionID) { return GetPointC(box[0], dimensionID); }
    static constexpr Scalar GetBoxMaxC(Box const& box, dim_t dimensionID) { return GetPointC(box[1], dimensionID); }

    static constexpr Vector const& GetRayDirection(Ray const& ray) noexcept { return ray[1]; }
    static constexpr Vector const& GetRayOrigin(Ray const& ray) noexcept { return ray[0]; }

    static constexpr Vector const& GetPlaneNormal(Plane const& plane) noexcept { return std::get<0>(plane); }
    static constexpr Scalar GetPlaneOrigoDistance(Plane const& plane) noexcept { return std::get<1>(plane); }
  };

  template<dim_t N>
  using CustomGeometryAdapter = GeneralGeometryAdapter<CustomBaseGeometryAdapter<N>>;
  template<dim_t N>
  using OrthoTreePointCustom = OrthoTreeBase<PointEntitySpanAdapter<CustomVectorTypeND<N>>, CustomGeometryAdapter<N>, PointConfiguration<>>;
  template<dim_t N>
  using OrthoTreePointContainerCustom = OrthoTree::OrthoTreeManaged<OrthoTreePointCustom<N>>;
  template<dim_t N, bool IS_LOOSE_TREE = true>
  using OrthoTreeBoxCustom = OrthoTreeBase<BoxEntitySpanAdapter<CustomBoundingBoxND<N>>, CustomGeometryAdapter<N>, BoxConfiguration<IS_LOOSE_TREE>>;
  template<dim_t N>
  using OrthoTreeBoxContainerCustom = OrthoTree::OrthoTreeManaged<OrthoTreeBoxCustom<N>>;
} // namespace LongIntAdaptor

namespace LongIntTest
{
  using namespace LongIntAdaptor;

  TEST(LongIntTest, RangeSearchPointAtTheBorder)
  {
    auto constexpr nDim = 1;
    using Vector = CustomVectorTypeND<nDim>;
    using Box = CustomBoundingBoxND<nDim>;

    auto constexpr points = std::array{ Vector{ 0 }, Vector{ 4 }, Vector{ 5 }, Vector{ 8 } };

    auto const tree = OrthoTreePointContainerCustom<nDim>(points, 3, std::nullopt, 2);

    {
      auto const vidActual = tree.RangeSearch(Box{ points[1], points[2] }, RangeSearchMode::Inside, 0.0f);
      auto constexpr vidExpected = std::array<EntityID, 2>{ 1, 2 };
      EXPECT_TRUE((std::ranges::is_permutation(vidActual, vidExpected)));
    }

    {
      auto const vidActual = tree.RangeSearch(Box{ points[2], Vector{ 6 } }, RangeSearchMode::Inside, 0.0f);
      auto constexpr vidExpected = std::array<EntityID, 1>{ 2 };
      EXPECT_TRUE((std::ranges::is_permutation(vidActual, vidExpected)));
    }
  }

  TEST(LongIntTest, RangeSearchBoxAtTheBorder)
  {
    auto constexpr nDim = 1;
    using Box = CustomBoundingBoxND<nDim>;

    auto const boxes = array<Box, 5>{
      Box{ 0, 1 },
      Box{ 3, 4 },
      Box{ 4, 5 },
      Box{ 5, 6 },
      Box{ 7, 8 }
    };

    auto const tree = OrthoTreeBoxContainerCustom<nDim>(boxes, 3, std::nullopt, 2, false);
    {
      auto const vidActual = tree.RangeSearch(boxes[1], RangeSearchMode::Inside, 0.0f);
      auto constexpr vidExpected = array<EntityID, 1>{ 1 };
      EXPECT_TRUE((std::ranges::is_permutation(vidActual, vidExpected)));
    }

    {
      auto const vidActual = tree.RangeSearch(boxes[2], RangeSearchMode::Inside, 0.0f);
      auto constexpr vidExpected = array<EntityID, 1>{ 2 };
      EXPECT_TRUE((std::ranges::is_permutation(vidActual, vidExpected)));
    }

    {
      auto const vidActual = tree.RangeSearch(CustomBoundingBoxND<nDim>{ 4, 8 }, RangeSearchMode::Inside, 0.0f);
      auto constexpr vidExpected = array<EntityID, 3>{ 2, 3, 4 };
      EXPECT_TRUE((std::ranges::is_permutation(vidActual, vidExpected)));
    }
  }

  TEST(LongIntTest, PickSearchAtTheBorder)
  {
    auto constexpr nDim = 1;
    using Vector = CustomVectorTypeND<nDim>;
    using Box = CustomBoundingBoxND<nDim>;

    auto const boxes = array<Box, 5>{
      Box{ 0, 1 },
      Box{ 3, 4 },
      Box{ 4, 5 },
      Box{ 5, 6 },
      Box{ 7, 8 }
    };

    auto const tree = OrthoTreeBoxContainerCustom<nDim>(boxes, 3, std::nullopt, 2, false);
    {
      auto const vidActual = tree.PickSearch(Vector{ 4 });
      auto constexpr vidExpected = array<EntityID, 2>{ 1, 2 };
      EXPECT_TRUE((std::ranges::is_permutation(vidActual, vidExpected)));
    }
  }

  TEST(LongIntTest, PickSearchAtTheBorder3LevelPos)
  {
    auto constexpr nDim = 1;
    using Vector = CustomVectorTypeND<nDim>;
    using Box = CustomBoundingBoxND<nDim>;

    auto const boxes = array{
      Box{ 0, 16 },

      Box{ 0,  1 },
      Box{ 3,  4 },
      Box{ 4,  5 },
      Box{ 5,  6 },

      Box{ 2,  4 },
      Box{ 4,  6 },
      Box{ 2,  6 },
    };

    auto const tree = OrthoTreeBoxContainerCustom<nDim>(boxes, 4, std::nullopt, 2, false);
    {
      auto const vidActual = tree.PickSearch(Vector{ 4 });
      auto constexpr vidExpected = array<EntityID, 6>{ 0, 2, 3, 5, 6, 7 };
      EXPECT_TRUE((std::ranges::is_permutation(vidActual, vidExpected)));
    }
  }

  TEST(LongIntTest, PickSearchAtTheBorder3LevelPosNeg)
  {
    auto constexpr nDim = 1;
    using Vector = CustomVectorTypeND<nDim>;
    using Box = CustomBoundingBoxND<nDim>;

    auto const boxes = array{
      Box{ -8,  8 },

      Box{  0,  1 },
      Box{ -4, -3 },
      Box{  4,  5 },
      Box{ -6, -5 },

      Box{ -4,  2 },
      Box{  4,  6 },
      Box{ -6,  2 },
    };

    auto const tree = OrthoTreeBoxContainerCustom<nDim>(boxes, 4, std::nullopt, 2, false);
    {
      auto const vidActual = tree.PickSearch(Vector{ -4 });
      auto constexpr vidExpected = array<EntityID, 4>{ 0, 2, 5, 7 };
      EXPECT_TRUE((std::ranges::is_permutation(vidActual, vidExpected)));
    }
  }

  TEST(LongIntTest, PickSearchAtTheBorder3LevelNeg)
  {
    auto constexpr nDim = 1;
    using Vector = CustomVectorTypeND<nDim>;
    using Box = CustomBoundingBoxND<nDim>;

    auto const boxes = array{
      Box{ -17, -1 },

      Box{  -2, -1 },
      Box{  -4, -3 },
      Box{  -5, -4 },
      Box{  -6, -5 },

      Box{  -4, -2 },
      Box{  -6, -4 },
      Box{  -6, -2 },
    };

    auto const tree = OrthoTreeBoxContainerCustom<nDim>(boxes, 4, std::nullopt, 2, false);
    {
      auto const vidActual = tree.PickSearch(Vector{ -4 });
      auto constexpr vidExpected = array<EntityID, 6>{ 0, 2, 3, 5, 6, 7 };
      EXPECT_TRUE((std::ranges::is_permutation(vidActual, vidExpected)));
    }
  }

  TEST(LongIntTest, PickSearch_Issue8)
  {
    auto constexpr nDim = 3;

    using Vector = CustomVectorTypeND<nDim>;
    using Box = CustomBoundingBoxND<nDim>;

    auto mGridCells = array<Box, 144>{};
    mGridCells[0] = Box{
      Vector{ -1024, 0, 1024 },
      Vector{  -961, 0, 1087 }
    };
    mGridCells[1] = Box{
      Vector{ -1024, 0, 1088 },
      Vector{  -961, 0, 1151 }
    };
    mGridCells[2] = Box{
      Vector{ -1024, 0, 1152 },
      Vector{  -961, 0, 1215 }
    };
    mGridCells[3] = Box{
      Vector{ -1024, 0, 1216 },
      Vector{  -961, 0, 1279 }
    };
    mGridCells[4] = Box{
      Vector{ -1024, 0, 1280 },
      Vector{  -961, 0, 1343 }
    };
    mGridCells[5] = Box{
      Vector{ -1024, 0, 1344 },
      Vector{  -961, 0, 1407 }
    };
    mGridCells[6] = Box{
      Vector{ -1024, 0, 1408 },
      Vector{  -961, 0, 1471 }
    };
    mGridCells[7] = Box{
      Vector{ -1024, 0, 1472 },
      Vector{  -961, 0, 1535 }
    };
    mGridCells[8] = Box{
      Vector{ -960, 0, 1024 },
      Vector{ -897, 0, 1087 }
    };
    mGridCells[9] = Box{
      Vector{ -960, 0, 1088 },
      Vector{ -897, 0, 1151 }
    };
    mGridCells[10] = Box{
      Vector{ -960, 0, 1152 },
      Vector{ -897, 0, 1215 }
    };
    mGridCells[11] = Box{
      Vector{ -960, 0, 1216 },
      Vector{ -897, 0, 1279 }
    };
    mGridCells[12] = Box{
      Vector{ -960, 0, 1280 },
      Vector{ -897, 0, 1343 }
    };
    mGridCells[13] = Box{
      Vector{ -960, 0, 1344 },
      Vector{ -897, 0, 1407 }
    };
    mGridCells[14] = Box{
      Vector{ -960, 0, 1408 },
      Vector{ -897, 0, 1471 }
    };
    mGridCells[15] = Box{
      Vector{ -960, 0, 1472 },
      Vector{ -897, 0, 1535 }
    };
    mGridCells[16] = Box{
      Vector{ -896, 0, 1024 },
      Vector{ -833, 0, 1087 }
    };
    mGridCells[17] = Box{
      Vector{ -896, 0, 1088 },
      Vector{ -833, 0, 1151 }
    };
    mGridCells[18] = Box{
      Vector{ -896, 0, 1152 },
      Vector{ -833, 0, 1215 }
    };
    mGridCells[19] = Box{
      Vector{ -896, 0, 1216 },
      Vector{ -833, 0, 1279 }
    };
    mGridCells[20] = Box{
      Vector{ -896, 0, 1280 },
      Vector{ -833, 0, 1343 }
    };
    mGridCells[21] = Box{
      Vector{ -896, 0, 1344 },
      Vector{ -833, 0, 1407 }
    };
    mGridCells[22] = Box{
      Vector{ -896, 0, 1408 },
      Vector{ -833, 0, 1471 }
    };
    mGridCells[23] = Box{
      Vector{ -896, 0, 1472 },
      Vector{ -833, 0, 1535 }
    };
    mGridCells[24] = Box{
      Vector{ -832, 0, 1024 },
      Vector{ -769, 0, 1087 }
    };
    mGridCells[25] = Box{
      Vector{ -832, 0, 1088 },
      Vector{ -769, 0, 1151 }
    };
    mGridCells[26] = Box{
      Vector{ -832, 0, 1152 },
      Vector{ -769, 0, 1215 }
    };
    mGridCells[27] = Box{
      Vector{ -832, 0, 1216 },
      Vector{ -769, 0, 1279 }
    };
    mGridCells[28] = Box{
      Vector{ -832, 0, 1280 },
      Vector{ -769, 0, 1343 }
    };
    mGridCells[29] = Box{
      Vector{ -832, 0, 1344 },
      Vector{ -769, 0, 1407 }
    };
    mGridCells[30] = Box{
      Vector{ -832, 0, 1408 },
      Vector{ -769, 0, 1471 }
    };
    mGridCells[31] = Box{
      Vector{ -832, 0, 1472 },
      Vector{ -769, 0, 1535 }
    };
    mGridCells[32] = Box{
      Vector{ -768, 0, 1024 },
      Vector{ -705, 0, 1087 }
    };
    mGridCells[33] = Box{
      Vector{ -768, 0, 1088 },
      Vector{ -705, 0, 1151 }
    };
    mGridCells[34] = Box{
      Vector{ -768, 0, 1152 },
      Vector{ -705, 0, 1215 }
    };
    mGridCells[35] = Box{
      Vector{ -768, 0, 1216 },
      Vector{ -705, 0, 1279 }
    };
    mGridCells[36] = Box{
      Vector{ -768, 0, 1280 },
      Vector{ -705, 0, 1343 }
    };
    mGridCells[37] = Box{
      Vector{ -768, 0, 1344 },
      Vector{ -705, 0, 1407 }
    };
    mGridCells[38] = Box{
      Vector{ -768, 0, 1408 },
      Vector{ -705, 0, 1471 }
    };
    mGridCells[39] = Box{
      Vector{ -768, 0, 1472 },
      Vector{ -705, 0, 1535 }
    };
    mGridCells[40] = Box{
      Vector{ -704, 0, 1024 },
      Vector{ -641, 0, 1087 }
    };
    mGridCells[41] = Box{
      Vector{ -704, 0, 1088 },
      Vector{ -641, 0, 1151 }
    };
    mGridCells[42] = Box{
      Vector{ -704, 0, 1152 },
      Vector{ -641, 0, 1215 }
    };
    mGridCells[43] = Box{
      Vector{ -704, 0, 1216 },
      Vector{ -641, 0, 1279 }
    };
    mGridCells[44] = Box{
      Vector{ -704, 0, 1280 },
      Vector{ -641, 0, 1343 }
    };
    mGridCells[45] = Box{
      Vector{ -704, 0, 1344 },
      Vector{ -641, 0, 1407 }
    };
    mGridCells[46] = Box{
      Vector{ -704, 0, 1408 },
      Vector{ -641, 0, 1471 }
    };
    mGridCells[47] = Box{
      Vector{ -704, 0, 1472 },
      Vector{ -641, 0, 1535 }
    };
    mGridCells[48] = Box{
      Vector{ -640, 0, 1024 },
      Vector{ -577, 0, 1087 }
    };
    mGridCells[49] = Box{
      Vector{ -640, 0, 1088 },
      Vector{ -577, 0, 1151 }
    };
    mGridCells[50] = Box{
      Vector{ -640, 0, 1152 },
      Vector{ -577, 0, 1215 }
    };
    mGridCells[51] = Box{
      Vector{ -640, 0, 1216 },
      Vector{ -577, 0, 1279 }
    };
    mGridCells[52] = Box{
      Vector{ -640, 0, 1280 },
      Vector{ -577, 0, 1343 }
    };
    mGridCells[53] = Box{
      Vector{ -640, 0, 1344 },
      Vector{ -577, 0, 1407 }
    };
    mGridCells[54] = Box{
      Vector{ -640, 0, 1408 },
      Vector{ -577, 0, 1471 }
    };
    mGridCells[55] = Box{
      Vector{ -640, 0, 1472 },
      Vector{ -577, 0, 1535 }
    };
    mGridCells[56] = Box{
      Vector{ -576, 0, 1024 },
      Vector{ -513, 0, 1087 }
    };
    mGridCells[57] = Box{
      Vector{ -576, 0, 1088 },
      Vector{ -513, 0, 1151 }
    };
    mGridCells[58] = Box{
      Vector{ -576, 0, 1152 },
      Vector{ -513, 0, 1215 }
    };
    mGridCells[59] = Box{
      Vector{ -576, 0, 1216 },
      Vector{ -513, 0, 1279 }
    };
    mGridCells[60] = Box{
      Vector{ -576, 0, 1280 },
      Vector{ -513, 0, 1343 }
    };
    mGridCells[61] = Box{
      Vector{ -576, 0, 1344 },
      Vector{ -513, 0, 1407 }
    };
    mGridCells[62] = Box{
      Vector{ -576, 0, 1408 },
      Vector{ -513, 0, 1471 }
    };
    mGridCells[63] = Box{
      Vector{ -576, 0, 1472 },
      Vector{ -513, 0, 1535 }
    };
    mGridCells[64] = Box{
      Vector{ -1024, 0, 1536 },
      Vector{  -961, 0, 1599 }
    };
    mGridCells[65] = Box{
      Vector{ -1024, 0, 1600 },
      Vector{  -961, 0, 1663 }
    };
    mGridCells[66] = Box{
      Vector{ -1024, 0, 1664 },
      Vector{  -961, 0, 1727 }
    };
    mGridCells[67] = Box{
      Vector{ -1024, 0, 1728 },
      Vector{  -961, 0, 1791 }
    };
    mGridCells[68] = Box{
      Vector{ -1024, 0, 1792 },
      Vector{  -961, 0, 1855 }
    };
    mGridCells[69] = Box{
      Vector{ -1024, 0, 1856 },
      Vector{  -961, 0, 1919 }
    };
    mGridCells[70] = Box{
      Vector{ -1024, 0, 1920 },
      Vector{  -961, 0, 1983 }
    };
    mGridCells[71] = Box{
      Vector{ -1024, 0, 1984 },
      Vector{  -961, 0, 2047 }
    };
    mGridCells[72] = Box{
      Vector{ -960, 0, 1536 },
      Vector{ -897, 0, 1599 }
    };
    mGridCells[73] = Box{
      Vector{ -960, 0, 1600 },
      Vector{ -897, 0, 1663 }
    };
    mGridCells[74] = Box{
      Vector{ -960, 0, 1664 },
      Vector{ -897, 0, 1727 }
    };
    mGridCells[75] = Box{
      Vector{ -960, 0, 1728 },
      Vector{ -897, 0, 1791 }
    };
    mGridCells[76] = Box{
      Vector{ -960, 0, 1792 },
      Vector{ -897, 0, 1855 }
    };
    mGridCells[77] = Box{
      Vector{ -960, 0, 1856 },
      Vector{ -897, 0, 1919 }
    };
    mGridCells[78] = Box{
      Vector{ -960, 0, 1920 },
      Vector{ -897, 0, 1983 }
    };
    mGridCells[79] = Box{
      Vector{ -960, 0, 1984 },
      Vector{ -897, 0, 2047 }
    };
    mGridCells[80] = Box{
      Vector{ -896, 0, 1536 },
      Vector{ -833, 0, 1599 }
    };
    mGridCells[81] = Box{
      Vector{ -896, 0, 1600 },
      Vector{ -833, 0, 1663 }
    };
    mGridCells[82] = Box{
      Vector{ -896, 0, 1664 },
      Vector{ -833, 0, 1727 }
    };
    mGridCells[83] = Box{
      Vector{ -896, 0, 1728 },
      Vector{ -833, 0, 1791 }
    };
    mGridCells[84] = Box{
      Vector{ -896, 0, 1792 },
      Vector{ -833, 0, 1855 }
    };
    mGridCells[85] = Box{
      Vector{ -896, 0, 1856 },
      Vector{ -833, 0, 1919 }
    };
    mGridCells[86] = Box{
      Vector{ -896, 0, 1920 },
      Vector{ -833, 0, 1983 }
    };
    mGridCells[87] = Box{
      Vector{ -896, 0, 1984 },
      Vector{ -833, 0, 2047 }
    };
    mGridCells[88] = Box{
      Vector{ -832, 0, 1536 },
      Vector{ -769, 0, 1599 }
    };
    mGridCells[89] = Box{
      Vector{ -832, 0, 1600 },
      Vector{ -769, 0, 1663 }
    };
    mGridCells[90] = Box{
      Vector{ -832, 0, 1664 },
      Vector{ -769, 0, 1727 }
    };
    mGridCells[91] = Box{
      Vector{ -832, 0, 1728 },
      Vector{ -769, 0, 1791 }
    };
    mGridCells[92] = Box{
      Vector{ -832, 0, 1792 },
      Vector{ -769, 0, 1855 }
    };
    mGridCells[93] = Box{
      Vector{ -832, 0, 1856 },
      Vector{ -769, 0, 1919 }
    };
    mGridCells[94] = Box{
      Vector{ -832, 0, 1920 },
      Vector{ -769, 0, 1983 }
    };
    mGridCells[95] = Box{
      Vector{ -832, 0, 1984 },
      Vector{ -769, 0, 2047 }
    };
    mGridCells[96] = Box{
      Vector{ -768, 0, 1536 },
      Vector{ -705, 0, 1599 }
    };
    mGridCells[97] = Box{
      Vector{ -768, 0, 1600 },
      Vector{ -705, 0, 1663 }
    };
    mGridCells[98] = Box{
      Vector{ -768, 0, 1664 },
      Vector{ -705, 0, 1727 }
    };
    mGridCells[99] = Box{
      Vector{ -768, 0, 1728 },
      Vector{ -705, 0, 1791 }
    };
    mGridCells[100] = Box{
      Vector{ -768, 0, 1792 },
      Vector{ -705, 0, 1855 }
    };
    mGridCells[101] = Box{
      Vector{ -768, 0, 1856 },
      Vector{ -705, 0, 1919 }
    };
    mGridCells[102] = Box{
      Vector{ -768, 0, 1920 },
      Vector{ -705, 0, 1983 }
    };
    mGridCells[103] = Box{
      Vector{ -768, 0, 1984 },
      Vector{ -705, 0, 2047 }
    };
    mGridCells[104] = Box{
      Vector{ -704, 0, 1536 },
      Vector{ -641, 0, 1599 }
    };
    mGridCells[105] = Box{
      Vector{ -704, 0, 1600 },
      Vector{ -641, 0, 1663 }
    };
    mGridCells[106] = Box{
      Vector{ -704, 0, 1664 },
      Vector{ -641, 0, 1727 }
    };
    mGridCells[107] = Box{
      Vector{ -704, 0, 1728 },
      Vector{ -641, 0, 1791 }
    };
    mGridCells[108] = Box{
      Vector{ -704, 0, 1792 },
      Vector{ -641, 0, 1855 }
    };
    mGridCells[109] = Box{
      Vector{ -704, 0, 1856 },
      Vector{ -641, 0, 1919 }
    };
    mGridCells[110] = Box{
      Vector{ -704, 0, 1920 },
      Vector{ -641, 0, 1983 }
    };
    mGridCells[111] = Box{
      Vector{ -704, 0, 1984 },
      Vector{ -641, 0, 2047 }
    };
    mGridCells[112] = Box{
      Vector{ -640, 0, 1536 },
      Vector{ -577, 0, 1599 }
    };
    mGridCells[113] = Box{
      Vector{ -640, 0, 1600 },
      Vector{ -577, 0, 1663 }
    };
    mGridCells[114] = Box{
      Vector{ -640, 0, 1664 },
      Vector{ -577, 0, 1727 }
    };
    mGridCells[115] = Box{
      Vector{ -640, 0, 1728 },
      Vector{ -577, 0, 1791 }
    };
    mGridCells[116] = Box{
      Vector{ -640, 0, 1792 },
      Vector{ -577, 0, 1855 }
    };
    mGridCells[117] = Box{
      Vector{ -640, 0, 1856 },
      Vector{ -577, 0, 1919 }
    };
    mGridCells[118] = Box{
      Vector{ -640, 0, 1920 },
      Vector{ -577, 0, 1983 }
    };
    mGridCells[119] = Box{
      Vector{ -640, 0, 1984 },
      Vector{ -577, 0, 2047 }
    };
    mGridCells[120] = Box{
      Vector{ -576, 0, 1536 },
      Vector{ -513, 0, 1599 }
    };
    mGridCells[121] = Box{
      Vector{ -576, 0, 1600 },
      Vector{ -513, 0, 1663 }
    };
    mGridCells[122] = Box{
      Vector{ -576, 0, 1664 },
      Vector{ -513, 0, 1727 }
    };
    mGridCells[123] = Box{
      Vector{ -576, 0, 1728 },
      Vector{ -513, 0, 1791 }
    };
    mGridCells[124] = Box{
      Vector{ -576, 0, 1792 },
      Vector{ -513, 0, 1855 }
    };
    mGridCells[125] = Box{
      Vector{ -576, 0, 1856 },
      Vector{ -513, 0, 1919 }
    };
    mGridCells[126] = Box{
      Vector{ -576, 0, 1920 },
      Vector{ -513, 0, 1983 }
    };
    mGridCells[127] = Box{
      Vector{ -576, 0, 1984 },
      Vector{ -513, 0, 2047 }
    };
    mGridCells[128] = Box{
      Vector{ -1024, 0, 1600 },
      Vector{  -961, 0, 1663 }
    };
    mGridCells[129] = Box{
      Vector{ -960, 0, 1600 },
      Vector{ -897, 0, 1663 }
    };
    mGridCells[130] = Box{
      Vector{ -896, 0, 1600 },
      Vector{ -833, 0, 1663 }
    };
    mGridCells[131] = Box{
      Vector{ -832, 0, 1600 },
      Vector{ -769, 0, 1663 }
    };
    mGridCells[132] = Box{
      Vector{ -768, 0, 1600 },
      Vector{ -705, 0, 1663 }
    };
    mGridCells[133] = Box{
      Vector{ -704, 0, 1600 },
      Vector{ -641, 0, 1663 }
    };
    mGridCells[134] = Box{
      Vector{ -640, 0, 1600 },
      Vector{ -577, 0, 1663 }
    };
    mGridCells[135] = Box{
      Vector{ -576, 0, 1600 },
      Vector{ -513, 0, 1663 }
    };
    mGridCells[136] = Box{
      Vector{ -1024, 0, 1536 },
      Vector{  -961, 0, 1599 }
    };
    mGridCells[137] = Box{
      Vector{ -960, 0, 1536 },
      Vector{ -897, 0, 1599 }
    };
    mGridCells[138] = Box{
      Vector{ -896, 0, 1536 },
      Vector{ -833, 0, 1599 }
    };
    mGridCells[139] = Box{
      Vector{ -832, 0, 1536 },
      Vector{ -769, 0, 1599 }
    };
    mGridCells[140] = Box{
      Vector{ -768, 0, 1536 },
      Vector{ -705, 0, 1599 }
    };
    mGridCells[141] = Box{
      Vector{ -704, 0, 1536 },
      Vector{ -641, 0, 1599 }
    };
    mGridCells[142] = Box{
      Vector{ -576, 0, 1536 },
      Vector{ -513, 0, 1599 }
    };
    mGridCells[143] = Box{
      Vector{ -576, 0, 1600 },
      Vector{ -513, 0, 1663 }
    };

    auto const tree = OrthoTreeBoxContainerCustom<nDim>(mGridCells, 3, std::nullopt, 21, true);
    auto const vidActual = tree.PickSearch(Vector{ -864, 0, 1471 }, 0.0);
    auto constexpr vidExpected = array<EntityID, 1>{ 22 };
    EXPECT_TRUE((std::ranges::is_permutation(vidActual, vidExpected)));
  }

  template<int nDim>
  static vector<CustomVectorTypeND<nDim>> readPointCloud(std::filesystem::path const& path)
  {
    auto points = vector<CustomVectorTypeND<nDim>>{};
    auto file = std::ifstream(path, std::ios::in);
    if (file.fail())
      return points;

    auto line = std::string{};
    while (std::getline(file, line))
    {
      if (file.fail())
        return points;

      auto& point = points.emplace_back();
      auto sw = std::string_view(line);
      for (int iDim = 0; iDim < nDim; ++iDim)
      {
        auto const [ptr, ec] = std::from_chars(sw.data(), sw.data() + sw.length(), point[iDim]);
        if (ec != std::errc{})
          return points;

        sw.remove_prefix(ptr - sw.data());
        if (!sw.empty())
          sw.remove_prefix(1);
      }
    }

    return points;
  }

  template<EntityID nDim>
  static vector<EntityID> BruteForceRangeSearch(vector<CustomVectorTypeND<nDim>> const& points, CustomBoundingBoxND<nDim> const& searchbox)
  {
    auto Entities = vector<EntityID>{};
    auto const nid = points.size();
    for (EntityID id = 0; id < nid; ++id)
      if (CustomGeometryAdapter<nDim>::DoesBoxContainPoint(searchbox, points[id], CustomGeometryAdapter<nDim>::BASE_TOLERANCE))
        Entities.emplace_back(id);

    return Entities;
  }

  TEST(LongIntTest, BruteForceRangeSearch_UsingPredefinedData_IfAvailable)
  {
    auto constexpr nDim = 3;
    using Vector = CustomVectorTypeND<nDim>;
    using Box = CustomBoundingBoxND<nDim>;
    using Tree = OrthoTreePointContainerCustom<nDim>;

    auto const points = readPointCloud<nDim>("../../../octree_data.txt");
    if (points.empty())
      return;

    auto const searchbox = Box{
      Vector{ 39, 43, 72 },
      Vector{ 49, 53, 76 }
    };

    auto const tree = Tree(points, 3, std::nullopt, 2);
    auto vidActual = tree.RangeSearch(searchbox);
    auto vidExpected = BruteForceRangeSearch<nDim>(points, searchbox);

    std::ranges::sort(vidActual);
    std::ranges::sort(vidExpected);
    auto missing_ids = vector<EntityID>{};
    std::ranges::set_difference(vidExpected, vidActual, std::back_inserter(missing_ids));
    auto missing_nodes = vector<OrthoTreePointCustom<nDim>::NodeID>(missing_ids.size());
    std::ranges::transform(missing_ids, missing_nodes.begin(), [&](auto const id) { return tree.GetCore().Find(id); });

    EXPECT_TRUE((std::ranges::is_permutation(vidActual, vidExpected)));
  }
} // namespace LongIntTest
