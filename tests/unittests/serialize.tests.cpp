#include "pch.h"

#define ORTHOTREE__PUBLIC_NODE_INTERFACE

#include "orthotree/bvh.h"
#include "orthotree/octree.h"
#include "orthotree/serialization.h"
#include "orthotree/serialization/binary_archive.h"

#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

namespace OrthoTree
{
  struct TestObject
  {
    int Tag = 1;
    double Value = 3.14;
    std::vector<int> Data = { 1, 2, 3 };

    bool operator==(const TestObject& other) const { return Tag == other.Tag && Value == other.Value && Data == other.Data; }
  };

  template<typename TArchive>
  void serialize(TArchive& ar, TestObject& obj)
  {
    ar& make_nvp("Tag", obj.Tag);
    ar& make_nvp("Value", obj.Value);
    ar& make_nvp("Data", obj.Data);
  }
} // namespace OrthoTree

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace OrthoTree;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
  // Specializations for OrthoTree types to integrate with Assert::AreEqual
  template<>
  inline std::wstring ToString<OrthoTree::Point3D>(const OrthoTree::Point3D& p)
  {
    std::wstringstream ss;
    ss << L"[" << p[0] << L", " << p[1] << L", " << p[2] << L"]";
    return ss.str();
  }

  template<>
  inline std::wstring ToString<OrthoTree::BoundingBox3D>(const OrthoTree::BoundingBox3D& b)
  {
    std::wstringstream ss;
    ss << L"{Min: " << ToString(b.Min).c_str() << L", Max: " << ToString(b.Max).c_str() << L"}";
    return ss.str();
  }
} // namespace Microsoft::VisualStudio::CppUnitTestFramework

namespace SerializeTests
{
  TEST_CLASS(SerializeTests)
  {
  public:
    template<typename TOutputArchive, typename TInputArchive, typename TTree>
    void RoundTripTest(TTree const& tree_save, std::ios::openmode mode)
    {
      std::stringstream ss(mode);
      {
        TOutputArchive ar_out(ss);
        ar_out& make_nvp("tree", tree_save);
      }

      // Display serialized content for debugging purposes (skipping binary)
      if constexpr (!std::is_same_v<TOutputArchive, BinaryOutputArchive>)
      {
        Logger::WriteMessage("\n--- Serialized Content ---\n");
        Logger::WriteMessage(ss.str().c_str());
        Logger::WriteMessage("\n--- End of content ---\n");
      }

      ss.seekg(0);

      TTree tree_load;
      {
        TInputArchive ar_in(ss);
        ar_in& make_nvp("tree", tree_load);
      }

      // Integrity checks
      auto const& entities_save = tree_save.GetData();
      auto const& entities_load = tree_load.GetData();

      Assert::AreEqual(entities_save.size(), entities_load.size(), L"Entity count mismatch");

      if constexpr (std::is_same_v<std::decay_t<decltype(entities_save)>, std::vector<typename TTree::Entity>>)
      {
        for (size_t i = 0; i < entities_save.size(); ++i)
        {
          if constexpr (std::is_same_v<typename TTree::Entity, BoundingBox3D>)
          {
            Assert::IsTrue(entities_save[i].Min == entities_load[i].Min, L"Min point mismatch");
            Assert::IsTrue(entities_save[i].Max == entities_load[i].Max, L"Max point mismatch");
          }
          else
          {
            Assert::IsTrue(entities_save[i] == entities_load[i], L"Entity data mismatch at index");
          }
        }
      }

      // Perform a small query to ensure the tree structure is also functional
      auto box = tree_save.GetBox();
      auto results_save = tree_save.RangeSearch(box);
      auto results_load = tree_load.RangeSearch(box);
      Assert::AreEqual(results_save.size(), results_load.size(), L"Query results count mismatch");
    }

    // --- Octree Tests ---

    TEST_METHOD(TestOctreePointBinary)
    {
      OctreePointM tree(
        {
          { 0, 0, 0 },
          { 1, 1, 1 }
      },
        3);
      tree.Add({ 0.1, 0.1, 0.1 });
      tree.Add({ 0.9, 0.9, 0.9 });
      RoundTripTest<BinaryOutputArchive, BinaryInputArchive>(tree, std::ios::in | std::ios::out | std::ios::binary);
    }

    TEST_METHOD(TestStaticOctreeBinary)
    {
      std::vector<Point3D> points = {
        { 0.1, 0.1, 0.1 },
        { 0.9, 0.9, 0.9 }
      };
      StaticOctreePointM tree(
        points,
        {
          { 0, 0, 0 },
          { 1, 1, 1 }
      },
        3);
      RoundTripTest<BinaryOutputArchive, BinaryInputArchive>(tree, std::ios::in | std::ios::out | std::ios::binary);
    }

    TEST_METHOD(TestOctreeBoxBinary)
    {
      OctreeBoxM tree(
        {
          { 0, 0, 0 },
          { 1, 1, 1 }
      },
        3);
      tree.Add(
        {
          { 0.1, 0.1, 0.1 },
          { 0.2, 0.2, 0.2 }
      });
      RoundTripTest<BinaryOutputArchive, BinaryInputArchive>(tree, std::ios::in | std::ios::out | std::ios::binary);
    }

    // --- BVH Tests ---

    TEST_METHOD(TestBVHPointBinary)
    {
      std::vector<Point3D> points = {
        { 0.1, 0.1, 0.1 },
        { 0.9, 0.9, 0.9 }
      };
      StaticBVHPoint3D bvh(points, 2);

      std::stringstream ss(std::ios::in | std::ios::out | std::ios::binary);
      {
        BinaryOutputArchive ar_out(ss);
        ar_out& make_nvp("bvh", bvh);
      }

      ss.seekg(0);
      StaticBVHPoint3D bvh_load;
      {
        BinaryInputArchive ar_in(ss);
        ar_in& make_nvp("bvh", bvh_load);
      }

      Assert::AreEqual(bvh.GetNodeCount(), bvh_load.GetNodeCount());
    }
  };
} // namespace SerializeTests
