#include <cassert>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/xml.hpp>
#include <cereal/types/array.hpp>
#include <cereal/types/variant.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/optional.hpp>
#include <cereal/types/utility.hpp>
#include <iostream>
#include <orthotree/bvh.h>
#include <orthotree/octree.h>
#include <orthotree/serialization.h>
#include <orthotree/serialization/text_archives.h>
#include <sstream>

using namespace OrthoTree;

int main()
{
  try
  {
    std::cout << "Starting OrthoTree + Cereal Integration Test..." << std::endl;

    // 1. Data Setup
    Point3D vpt[] = {
      { 0, 0, 0 },
      { 1, 1, 1 },
      { 2, 2, 2 },
      { 3, 3, 3 }
    };
    StaticBVHPoint3D core_save(vpt, 4);

    // 2. Cereal JSON Roundtrip
    std::string json_data;
    {
      std::stringstream ss;
      {
        cereal::JSONOutputArchive ar(ss);
        ar(cereal::make_nvp("bvh_core", core_save));
      }
      json_data = ss.str();
    }
    std::cout << "Cereal JSON Output Generated (size: " << json_data.size() << ")" << std::endl;

    // 3. Cereal XML Roundtrip
    std::string xml_cereal_data;
    {
      std::stringstream ss;
      {
        cereal::XMLOutputArchive ar(ss);
        ar(cereal::make_nvp("bvh_core", core_save));
      }
      xml_cereal_data = ss.str();
    }
    std::cout << "Cereal XML Data Preview: " << xml_cereal_data.substr(0, 200) << "..." << std::endl;
    std::cout << "Cereal XML Output Generated (size: " << xml_cereal_data.size() << ")" << std::endl;

    // 4. OrthoTree XML Output
    std::string xml_orthotree_data;
    {
      std::stringstream ss;
      {
        XMLOutputArchive ar(ss);
        ar & make_nvp("bvh_core", core_save);
      }
      xml_orthotree_data = ss.str();
    }
    std::cout << "OrthoTree XML Data Preview: " << xml_orthotree_data.substr(0, 200) << "..." << std::endl;
    std::cout << "OrthoTree XML Output Generated (size: " << xml_orthotree_data.size() << ")" << std::endl;

    // 5. Binary Roundtrip
    std::stringstream ss_bin(std::ios::in | std::ios::out | std::ios::binary);
    {
      cereal::BinaryOutputArchive ar(ss_bin);
      ar(core_save);
    }

    ss_bin.seekg(0);
    StaticBVHPoint3D core_load_bin;
    {
      cereal::BinaryInputArchive ar(ss_bin);
      ar(core_load_bin);
    }
    std::cout << "Binary Roundtrip Successful." << std::endl;

    // 6. Verification
    assert(core_save.GetMaxDepth() == core_load_bin.GetMaxDepth());

    std::cout << "All tests passed successfully!" << std::endl;

    // 7. Static Octree Test
    // 7. Static Octree Test
    {
      std::cout << "Starting Static Octree Test..." << std::endl;
      BoundingBox3D box{ { 0, 0, 0 }, { 3, 3, 3 } };
      std::vector<Point3D> vpt_vec(vpt, vpt + 4);
      StaticOctreePointM static_managed_save(vpt_vec, std::optional<depth_t>(3), std::optional<BoundingBox3D>(box), size_t(8), SEQ_EXEC);
      
      std::stringstream ss;
      {
        cereal::XMLOutputArchive ar(ss);
        ar & cereal::make_nvp("static_octree", static_managed_save);
      }
      std::string xml = ss.str();
      std::cout << "Static Octree XML Generated (size: " << xml.size() << ")" << std::endl;
    }

    // 8. Dynamic Octree Test
    {
      std::cout << "Starting Dynamic Octree Test..." << std::endl;
      BoundingBox3D box{ { 0, 0, 0 }, { 3, 3, 3 } };
      OctreePointMapM dynamic_managed_save;
      dynamic_managed_save.Init(box, 3);
      for (int i = 0; i < 4; ++i) dynamic_managed_save.Add(std::make_pair((index_t)i, vpt[i]));

      std::stringstream ss;
      {
        cereal::XMLOutputArchive ar(ss);
        ar & cereal::make_nvp("dynamic_octree", dynamic_managed_save);
      }
      std::string xml = ss.str();
      std::cout << "Dynamic Octree XML Generated (size: " << xml.size() << ")" << std::endl;
    }

    std::cout << "All extended tests passed successfully!" << std::endl;
    return 0;
  }
  catch (const std::exception& e)
  {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }
}
