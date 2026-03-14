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

#include <cstddef>
#include <cstdint>
#include <limits>
#include <optional>
#include <type_traits>


namespace OrthoTree
{
#ifdef ORTHOTREE_INDEX_T__SIZE_T
  using index_t = std::size_t;
#else
#ifdef ORTHOTREE_INDEX_T__UINT_FAST32_T
  using index_t = std::uint_fast32_t;
#else
#ifdef ORTHOTREE_INDEX_T__INT
  using index_t = int;
#else
  using index_t = std::uint32_t;
#endif // ORTHOTREE_INDEX_INT
#endif // ORTHOTREE_INDEX_T__UINT_FAST32_T
#endif // ORTHOTREE_INDEX_T__SIZE_T

  // Type of the dimension
  using dim_t = uint32_t;

  // Type of depth
  using depth_t = uint32_t;
  constexpr depth_t INVALID_DEPTH = 31;
  constexpr depth_t MAX_DEPTH_ID = 30;

  // Grid id
  using GridID = uint32_t;
  constexpr GridID INVALID_GRIDID = std::numeric_limits<GridID>::max();

  enum class LogicalOperator
  {
    And,
    Or
  };

  // Enum of entity geometry type
  enum class GeometryType
  {
    Point,
    Box,
    // Mixed // Not supported yet
  };

  enum class RangeSearchMode
  {
    Inside,
    Overlap
  };


  // Enum of relation with Planes
  enum class PlaneRelation : char
  {
    Negative,
    Hit,
    Positive
  };

  enum class InsertionMode
  {
    Balanced,
    LowestLeaf,
    ExistingLeaf
  };

  namespace ExecutionTags
  {
    // Sequential execution tag
    struct Sequential
    {};

    // Parallel execution tag
    struct Parallel
    {};
  } // namespace ExecutionTags

  using SeqExec = ExecutionTags::Sequential;
  using ParExec = ExecutionTags::Parallel;
  auto constexpr SEQ_EXEC = SeqExec{};
  auto constexpr PAR_EXEC = ParExec{};
} // namespace OrthoTree
