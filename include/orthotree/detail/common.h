/*
MIT License

Copyright (c) 2021 Attila Csikós

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#pragma once

#include "../core/types.h"

#include <cassert>
#include <exception>
#include <execution>
#include <type_traits>
#include <utility>
#include <cstdlib>
#include <version>


#if defined(__cpp_lib_unreachable) && (__cpp_lib_unreachable >= 202202L)
#include <utility>
#define ORTHOTREE_UNREACHABLE() std::unreachable()
#elif defined(_MSC_VER)
#define ORTHOTREE_UNREACHABLE() __assume(0)
#elif defined(__GNUC__) || defined(__clang__)
#define ORTHOTREE_UNREACHABLE() __builtin_unreachable()
#else
#include <cstdlib>
#define ORTHOTREE_UNREACHABLE() std::abort()
#endif


#if (defined(__BMI2__) || defined(__AVX2__)) && (defined(_M_X64) || defined(__x86_64__) || defined(__amd64__))
#ifdef __has_include
#if __has_include(<immintrin.h>)
#include <immintrin.h>
#define ORTHOTREE_BMI2_PDEP_AVAILABLE 1
#endif
#endif
#endif

#if defined(__clang__)
#define ORTHOTREE_LOOPIVDEP
#elif defined(__INTEL_COMPILER)
#define ORTHOTREE_LOOPIVDEP _Pragma("ivdep")
#elif defined(__GNUC__)
#define ORTHOTREE_LOOPIVDEP _Pragma("GCC ivdep")
#elif defined(_MSC_VER)
#define ORTHOTREE_LOOPIVDEP _Pragma("loop(ivdep)")
#else
#define ORTHOTREE_LOOPIVDEP
#endif

#ifndef CRASH
#define CRASH_UNDEF
#define CRASH(errorMessage)        \
  do                               \
  {                                \
    assert(false && errorMessage); \
    std::terminate();              \
  } while (0)
#endif // !CRASH

#ifndef ORTHOTREE_CRASH_IF
#define CRASH_IF_UNDEF
#define ORTHOTREE_CRASH_IF(cond, errorMessage) \
  do                                           \
  {                                            \
    if (cond)                                  \
    {                                          \
      CRASH(errorMessage);                     \
    }                                          \
  } while (0)
#endif // !ORTHOTREE_CRASH_IF

#ifdef __cpp_lib_execution
#define EXEC_POL_DEF(e) \
  (void)execMode;       \
  std::conditional_t<std::is_same_v<TExecMode, ExecutionTags::Parallel>, std::execution::parallel_unsequenced_policy, std::execution::unsequenced_policy> constexpr e
#define EXEC_POL_ADD(e) e,
#else
#define EXEC_POL_DEF(e)
#define EXEC_POL_ADD(e)
#endif
