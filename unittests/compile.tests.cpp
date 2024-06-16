// pch.cpp: source file corresponding to the pre-compiled header

#include "pch.h"
#include "compile_test.h"

// When you are using pre-compiled headers, this source file is necessary for compilation to succeed.

namespace CompileTest
{
  TEST_CLASS(CompileTest)
  { 
    TEST_METHOD(AllFunc)
    { 
      testCompileBoxBatchSplitStrategy();
    }
  };
}