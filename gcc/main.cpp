#include <iostream>

#include "../octree.h"
#include "../unittests/compile_test.h"     

using namespace OrthoTree;

int main()
{
  testCompileBoxBatchSplitStrategy();
  std::cout << "Octree compile test was ran successfully.\n";
  return 0;
}