#include <cassert>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/xml.hpp>
#include <cereal/types/array.hpp>
#include <cereal/types/variant.hpp>
#include <cereal/types/vector.hpp>
#include <iostream>
#include <orthotree/bvh.h>
#include <orthotree/serialization.h>
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

    // 2. JSON Roundtrip
    std::string json_data;
    {
      std::stringstream ss;
      {
        cereal::JSONOutputArchive ar(ss);
        ar(cereal::make_nvp("bvh_core", core_save));
      }
      json_data = ss.str();
    }
    std::cout << "JSON Data: " << json_data << std::endl;
    std::cout << "JSON Output Generated (size: " << json_data.size() << ")" << std::endl;

    StaticBVHPoint3D core_load_json;
    {
      std::stringstream ss(json_data);
      cereal::JSONInputArchive ar(ss);
      ar(cereal::make_nvp("bvh_core", core_load_json));
    }
    std::cout << "JSON Load Successful." << std::endl;

    // 3. Binary Roundtrip
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

    // 4. Verification
    assert(core_save.GetMaxDepth() == core_load_json.GetMaxDepth());
    assert(core_save.GetMaxDepth() == core_load_bin.GetMaxDepth());

    std::cout << "All tests passed successfully!" << std::endl;
    return 0;
  }
  catch (const std::exception& e)
  {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }
}
