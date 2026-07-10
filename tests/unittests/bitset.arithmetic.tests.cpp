#include "pch.h"

using namespace OrthoTree;

using std::array;
using std::vector;

namespace BitsetArithmeticTest
{
  TEST(BitsetArithmeticTest, Less_0_1__true)
    {
      EXPECT_TRUE((bitset_arithmetic<10>(0) < bitset_arithmetic<10>(1)));
    }


    TEST(BitsetArithmeticTest, Less_12_12__false)
    {
      EXPECT_FALSE((bitset_arithmetic<10>(12) < bitset_arithmetic<10>(12)));
    }


    TEST(BitsetArithmeticTest, Less_126_254__true)
    {
      EXPECT_TRUE((bitset_arithmetic<30>(126) < bitset_arithmetic<30>(254)));
    }


    TEST(BitsetArithmeticTest, Greater_126_254__false)
    {
      EXPECT_FALSE((bitset_arithmetic<30>(126) > bitset_arithmetic<30>(254)));
    }


    TEST(BitsetArithmeticTest, Plus_0_0__0)
    {
      auto const bs = bitset_arithmetic<10>(0) + bitset_arithmetic<10>(0);
      auto const n = bs.to_ulong();
      EXPECT_EQ(0, n);
    }


    TEST(BitsetArithmeticTest, Plus_0_1__1)
    {
      auto const bs = bitset_arithmetic<10>(0) + bitset_arithmetic<10>(1);
      auto const n = bs.to_ulong();
      EXPECT_EQ(1, n);
    }


    TEST(BitsetArithmeticTest, Plus_1_1__2)
    {
      auto const bs = bitset_arithmetic<10>(1) + bitset_arithmetic<10>(1);
      auto const n = bs.to_ulong();
      EXPECT_EQ(2, n);
    }


    TEST(BitsetArithmeticTest, Plus_16_16__32)
    {
      auto const bs = bitset_arithmetic<10>(16) + bitset_arithmetic<10>(16);
      auto const n = bs.to_ulong();
      EXPECT_EQ(32, n);
    }


    TEST(BitsetArithmeticTest, Plus_17_16__33)
    {
      auto const bs = bitset_arithmetic<10>(17) + bitset_arithmetic<10>(16);
      auto const n = bs.to_ulong();
      EXPECT_EQ(33, n);
    }


    TEST(BitsetArithmeticTest, Minus_17_16__1)
    {
      auto const bs = bitset_arithmetic<10>(17) - bitset_arithmetic<10>(16);
      auto const n = bs.to_ulong();
      EXPECT_EQ(1, n);
    }


    TEST(BitsetArithmeticTest, Mult_17_16__272)
    {
      auto const bs = bitset_arithmetic<10>(17) * bitset_arithmetic<10>(16);
      auto const n = bs.to_ulong();
      EXPECT_EQ(272, n);
    }


    TEST(BitsetArithmeticTest, Mult_127_255__32385)
    {
      auto const bs = bitset_arithmetic<18>(127) * bitset_arithmetic<18>(255);
      auto const n = bs.to_ulong();
      EXPECT_EQ(32385, n);
    }


    TEST(BitsetArithmeticTest, Mult_2_3__6)
    {
      auto const bs = bitset_arithmetic<10>(2) * bitset_arithmetic<10>(3);
      auto const n = bs.to_ulong();
      EXPECT_EQ(6, n);
    }


    TEST(BitsetArithmeticTest, Mult_5_5__25)
    {
      auto const bs = bitset_arithmetic<10>(5) * bitset_arithmetic<10>(5);
      auto const n = bs.to_ulong();
      EXPECT_EQ(25, n);
    }


    TEST(BitsetArithmeticTest, Mult_10_10__100)
    {
      auto const bs = bitset_arithmetic<10>(10) * bitset_arithmetic<10>(10);
      auto const n = bs.to_ulong();
      EXPECT_EQ(100, n);
    }


    TEST(BitsetArithmeticTest, Mult_15_15__225)
    {
      auto const bs = bitset_arithmetic<10>(15) * bitset_arithmetic<10>(15);
      auto const n = bs.to_ulong();
      EXPECT_EQ(225, n);
    }


    TEST(BitsetArithmeticTest, Mult_20_20__400)
    {
      auto const bs = bitset_arithmetic<10>(20) * bitset_arithmetic<10>(20);
      auto const n = bs.to_ulong();
      EXPECT_EQ(400, n);
    }
} // namespace BitsetArithmeticTest
