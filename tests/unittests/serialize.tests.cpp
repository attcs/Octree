#include "pch.h"

#include "orthotree/bvh.h"
#include "orthotree/serialization.h"
#include "orthotree/serialization/binary_archive.h"
#include "orthotree/serialization/text_archives.h"

#include <array>
#include <iostream>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>

// --- Test Implementation ---
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace OrthoTree;

namespace OrthoTree
{
  struct TestObject
  {
    int id = 1;
    double value = 3.14;
    std::vector<int> data = { 1, 2, 3 };
  };

  template<typename TArchive>
  void serialize(TArchive& ar, TestObject& obj, [[maybe_unused]] const unsigned int version)
  {
    ar& ORTHOTREE_NVP_M(obj, id);
    ar& ORTHOTREE_NVP_M(obj, value);
    ar& ORTHOTREE_NVP_M(obj, data);
  }
} // namespace OrthoTree

namespace UnitTests
{
  TEST_CLASS(SerializeTests)
  {
  public:
    TEST_METHOD(TestJSONOutputArchive)
    {
      OrthoTree::TestObject obj;
      std::stringstream ss;
      {
        OrthoTree::JSONOutputArchive ar(ss);
        ar& make_nvp("test_obj", obj);
      }

      std::string json = ss.str();
      Assert::AreNotEqual(std::string::npos, json.find("\"id\": 1"));
      Assert::AreNotEqual(std::string::npos, json.find("\"value\": 3.14"));
    }

    TEST_METHOD(TestXMLOutputArchive)
    {
      OrthoTree::Point3D pt{ 1.0, 2.0, 3.0 };
      std::stringstream ss;
      {
        OrthoTree::XMLOutputArchive ar(ss);
        ar& make_nvp("point", pt);
      }

      std::string xml = ss.str();
      Assert::AreNotEqual(std::string::npos, xml.find("<point>"));
      Assert::AreNotEqual(std::string::npos, xml.find("1"));
    }

    TEST_METHOD(TestBinaryArchive)
    {
      OrthoTree::BoundingBox3D box_save{
        { 1.0, 2.0, 3.0 },
        { 4.0, 5.0, 6.0 }
      };
      std::stringstream ss(std::ios::in | std::ios::out | std::ios::binary);

      {
        OrthoTree::BinaryOutputArchive ar(ss);
        ar & box_save;
      }

      ss.seekg(0);

      OrthoTree::BoundingBox3D box_load;
      {
        OrthoTree::BinaryInputArchive ar(ss);
        ar & box_load;
      }

      Assert::AreEqual(box_save.Min[0], box_load.Min[0]);
      Assert::AreEqual(box_save.Max[2], box_load.Max[2]);
    }

    TEST_METHOD(TestCoreBinarySerialization)
    {
      OrthoTree::Point3D vpt[] = {
        { 0, 0, 0 },
        { 1, 1, 1 }
      };
      OrthoTree::StaticBVHPoint3D core_save(vpt, 1);

      std::stringstream ss(std::ios::in | std::ios::out | std::ios::binary);
      {
        OrthoTree::BinaryOutputArchive ar(ss);
        ar & core_save;
      }

      ss.seekg(0);

      OrthoTree::StaticBVHPoint3D core_load;
      {
        OrthoTree::BinaryInputArchive ar(ss);
        ar & core_load;
      }
    }
  };
} // namespace UnitTests
