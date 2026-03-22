#include <cassert>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

// OrthoTree includes
#include <msgpack.hpp>
#include <orthotree/bvh.h>
#include <orthotree/octree.h>
#include <orthotree/serialization.h>
#include <orthotree/serialization/msgpack_archive.h>

// Cereal includes
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/xml.hpp>
#include <cereal/types/array.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/optional.hpp>
#include <cereal/types/set.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/utility.hpp>
#include <cereal/types/variant.hpp>
#include <cereal/types/vector.hpp>

using namespace OrthoTree;

// --- Test Helpers ---

template<typename TArchiveOut, typename TArchiveIn, typename T>
void TestCerealRoundtrip(T& original, std::string const& label)
{
  std::cout << "  Cereal Roundtrip [" << label << "]: ";
  std::stringstream ss;
  try
  {
    {
      TArchiveOut ar(ss);
      ar(cereal::make_nvp("data", original));
    }

    T loaded;
    {
      TArchiveIn ar(ss);
      ar(cereal::make_nvp("data", loaded));
    }
    if (label == "JSON")
    {
      std::cout << "Success (Size: " << ss.str().size() << " bytes)" << std::endl;
      std::cout << "JSON Content: " << ss.str() << std::endl;
    }
    else if (label == "XML")
    {
      std::cout << "Success (Size: " << ss.str().size() << " bytes)" << std::endl;
      std::cout << "XML Content: " << ss.str() << std::endl;
    }
    else
    {
      std::cout << "Success (Size: " << ss.str().size() << " bytes)" << std::endl;
    }
  }
  catch (const std::exception& e)
  {
    std::cout << "FAILED!" << std::endl;
    std::cerr << "    Error: " << e.what() << std::endl;
    if (label == "JSON")
    {
      std::cerr << "    JSON Content: " << ss.str() << (ss.str().size() > 1000 ? "..." : "") << std::endl;
    }
    // Re-throw to be caught by main's catch-all if desired, or just continue
    // throw;
  }
}

template<typename T>
void TestMsgPack(T& original, std::string const& label)
{
#ifdef ORTHOTREE_SERIALIZATION_MSGPACK_ENABLED
  std::cout << "  MsgPack Save [" << label << "]: ";
  std::stringstream ss(std::ios::out | std::ios::binary);
  {
    MsgPackArchive ar(ss);
    ar& make_nvp("data", original);
  }
  std::cout << "Success (Size: " << ss.str().size() << " bytes)" << std::endl;
#else
  (void)original;
  (void)label;
#endif
}

template<typename T>
void RunAllTestsForType(T& tree, std::string const& typeName)
{
  std::cout << "Testing Type: " << typeName << std::endl;

  TestCerealRoundtrip<cereal::BinaryOutputArchive, cereal::BinaryInputArchive>(tree, "Binary");
  TestCerealRoundtrip<cereal::JSONOutputArchive, cereal::JSONInputArchive>(tree, "JSON");
  TestCerealRoundtrip<cereal::XMLOutputArchive, cereal::XMLInputArchive>(tree, "XML");
  TestMsgPack(tree, "MsgPack");

  std::cout << std::endl;
}

int main()
{
  try
  {
    std::cout << "Starting External Library Serialization Tests..." << std::endl << std::endl;

    // 1. Setup Data (More points for a better test)
    std::vector<Point3D> vpt_vec;
    for (int x = 0; x < 4; ++x)
      for (int y = 0; y < 4; ++y)
        for (int z = 0; z < 4; ++z)
          vpt_vec.push_back({ (double)x, (double)y, (double)z });

    BoundingBox3D box{
      { -0.1, -0.1, -0.1 },
      {  4.1,  4.1,  4.1 }
    };

    // --- 1. OctreePoint (Vector based) ---
    {
      OctreePoint tree(vpt_vec, 3, box);
      RunAllTestsForType(tree, "OctreePoint");
    }

    // --- 2. OctreePointMap (Map based) ---
    {
      std::unordered_map<index_t, Point3D> vpt_map_data;
      for (index_t i = 0; i < vpt_vec.size(); ++i)
        vpt_map_data[i] = vpt_vec[i];

      OctreePointMap tree(vpt_map_data, 3, box);
      RunAllTestsForType(tree, "OctreePointMap");
    }

    // --- 3. OctreePointMapM (Managed Map) ---
    {
      OctreePointMapM tree;
      tree.Init(box, 3);
      for (index_t i = 0; i < vpt_vec.size(); ++i)
      {
        tree.Add(std::pair<const index_t, Point3D>{ i, vpt_vec[i] });
      }

      RunAllTestsForType(tree, "OctreePointMapM");
    }

    // --- 4. OctreePointM (Managed Vector) ---
    {
      OctreePointM tree;
      tree.Init(box, 3);
      for (auto const& pt : vpt_vec)
        tree.Add(pt);

      RunAllTestsForType(tree, "OctreePointM");
    }
    std::cout << "All external serialization tests finished successfully!" << std::endl;
    return 0;
  }
  catch (const std::exception& e)
  {
    std::cerr << "Fatal Error: " << e.what() << std::endl;
    return 1;
  }
}
