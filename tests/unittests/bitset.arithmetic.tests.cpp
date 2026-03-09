#include "pch.h"

using namespace OrthoTree;

using std::array;
using std::vector;

namespace BitsetArithmeticTest
{
  TEST_CLASS(BitsetArithmeticTest)
  {
    TEST_METHOD(Less_0_1__true)
    {
      Assert::IsTrue(bitset_arithmetic<10>(0) < bitset_arithmetic<10>(1));
    }


    TEST_METHOD(Less_12_12__false)
    {
      Assert::IsFalse(bitset_arithmetic<10>(12) < bitset_arithmetic<10>(12));
    }


    TEST_METHOD(Less_126_254__true)
    {
      Assert::IsTrue(bitset_arithmetic<30>(126) < bitset_arithmetic<30>(254));
    }


    TEST_METHOD(Greater_126_254__false)
    {
      Assert::IsFalse(bitset_arithmetic<30>(126) > bitset_arithmetic<30>(254));
    }


    TEST_METHOD(Plus_0_0__0)
    {
      auto const bs = bitset_arithmetic<10>(0) + bitset_arithmetic<10>(0);
      auto const n = bs.to_ulong();
      Assert::AreEqual<uint32_t>(0, n);
    }


    TEST_METHOD(Plus_0_1__1)
    {
      auto const bs = bitset_arithmetic<10>(0) + bitset_arithmetic<10>(1);
      auto const n = bs.to_ulong();
      Assert::AreEqual<uint32_t>(1, n);
    }


    TEST_METHOD(Plus_1_1__2)
    {
      auto const bs = bitset_arithmetic<10>(1) + bitset_arithmetic<10>(1);
      auto const n = bs.to_ulong();
      Assert::AreEqual<uint32_t>(2, n);
    }


    TEST_METHOD(Plus_16_16__32)
    {
      auto const bs = bitset_arithmetic<10>(16) + bitset_arithmetic<10>(16);
      auto const n = bs.to_ulong();
      Assert::AreEqual<uint32_t>(32, n);
    }


    TEST_METHOD(Plus_17_16__33)
    {
      auto const bs = bitset_arithmetic<10>(17) + bitset_arithmetic<10>(16);
      auto const n = bs.to_ulong();
      Assert::AreEqual<uint32_t>(33, n);
    }


    TEST_METHOD(Minus_17_16__1)
    {
      auto const bs = bitset_arithmetic<10>(17) - bitset_arithmetic<10>(16);
      auto const n = bs.to_ulong();
      Assert::AreEqual<uint32_t>(1, n);
    }


    TEST_METHOD(Mult_17_16__272)
    {
      auto const bs = bitset_arithmetic<10>(17) * bitset_arithmetic<10>(16);
      auto const n = bs.to_ulong();
      Assert::AreEqual<uint32_t>(272, n);
    }


    TEST_METHOD(Mult_127_255__32385)
    {
      auto const bs = bitset_arithmetic<18>(127) * bitset_arithmetic<18>(255);
      auto const n = bs.to_ulong();
      Assert::AreEqual<uint32_t>(32385, n);
    }


    TEST_METHOD(Mult_2_3__6)
    {
      auto const bs = bitset_arithmetic<10>(2) * bitset_arithmetic<10>(3);
      auto const n = bs.to_ulong();
      Assert::AreEqual<uint32_t>(6, n);
    }


    TEST_METHOD(Mult_5_5__25)
    {
      auto const bs = bitset_arithmetic<10>(5) * bitset_arithmetic<10>(5);
      auto const n = bs.to_ulong();
      Assert::AreEqual<uint32_t>(25, n);
    }


    TEST_METHOD(Mult_10_10__100)
    {
      auto const bs = bitset_arithmetic<10>(10) * bitset_arithmetic<10>(10);
      auto const n = bs.to_ulong();
      Assert::AreEqual<uint32_t>(100, n);
    }


    TEST_METHOD(Mult_15_15__225)
    {
      auto const bs = bitset_arithmetic<10>(15) * bitset_arithmetic<10>(15);
      auto const n = bs.to_ulong();
      Assert::AreEqual<uint32_t>(225, n);
    }


    TEST_METHOD(Mult_20_20__400)
    {
      auto const bs = bitset_arithmetic<10>(20) * bitset_arithmetic<10>(20);
      auto const n = bs.to_ulong();
      Assert::AreEqual<uint32_t>(400, n);
    }
  };
}
