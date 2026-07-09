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


using namespace OrthoTree;


namespace SerializeTests
{
  namespace SerializeTests

  {
  // public:
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
        std::cout << "\n--- Serialized Content ---\n" << std::endl;
        std::cout << ss.str().c_str() << std::endl;
        std::cout << "\n--- End of content ---\n" << std::endl;
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

      EXPECT_EQ(entities_save.size(), entities_load.size()) << "Entity count mismatch";

      if constexpr (std::is_same_v<std::decay_t<decltype(entities_save)>, std::vector<typename TTree::Entity>>)
      {
        for (size_t i = 0; i < entities_save.size(); ++i)
        {
          if constexpr (std::is_same_v<typename TTree::Entity, BoundingBox3D>)
          {
            EXPECT_TRUE((entities_save[i].Min == entities_load[i].Min)) << "Min point mismatch";
            EXPECT_TRUE((entities_save[i].Max == entities_load[i].Max)) << "Max point mismatch";
          }
          else
          {
            EXPECT_TRUE((entities_save[i] == entities_load[i])) << "Entity data mismatch at index";
          }
        }
      }

      // Perform a small query to ensure the tree structure is also functional
      auto box = tree_save.GetBox();
      auto results_save = tree_save.RangeSearch(box);
      auto results_load = tree_load.RangeSearch(box);
      EXPECT_EQ(results_save.size(), results_load.size()) << "Query results count mismatch";
    }

    // --- Octree Tests ---

    TEST(SerializeTests, TestOctreePointBinary)
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

    TEST(SerializeTests, TestStaticOctreeBinary)
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

    TEST(SerializeTests, TestOctreeBoxBinary)
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

    TEST(SerializeTests, TestBVHPointBinary)
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

      EXPECT_EQ(bvh.GetNodeCount(), bvh_load.GetNodeCount());
    }
  }
} // namespace SerializeTests
