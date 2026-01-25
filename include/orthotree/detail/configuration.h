#pragma once

#include "common.h"

#include <unordered_map>

namespace OrthoTree
{
  enum class NodeGeometryStorage
  {
    None,
    MinPoint,
    MBR,
  };

  template<double IS_LOOSE_FACTOR_ = 1.0, bool USE_MBR_ = false>
  struct Configuration
  {
    // Geometry type cannot be mixed within the same octree.
    static constexpr bool IS_HOMOGENEOUS_GEOMETRY = true;

    // Loose tree's looseness factor. Recommended value for Box trees with ray-picking: 2.0
    static constexpr double LOOSE_FACTOR = IS_LOOSE_FACTOR_;

    // If true, out-of-handled space element will be stored in the root node. Otherwise, insertion will fail.
    static constexpr bool ALLOW_OUT_OF_SPACE_INSERTION = true;

    // If tree, Reverse mapping (entityID -> nodeID) will be stored to speed up removal and update operations. (Dynamic datasets)
    static constexpr bool USE_REVERSE_MAPPING = true;

    // TODO: rethink Location
    // It determines the internal location data type sizes.
    // In 3D, 21: (maximum allowed depth for 3D), LocationID is an uint64_t, Location's size is over 64bit. Resolution: for 1km model-space is 0.5mm
    // In 3D, 19: Location's size is 57bit. Resolution: for 1km model-space is 20mm
    // In 3D, 10: LocationID is an uint32_t, Location's size is over 32bit. Resolution: for 1km model-space is 1m
    // In 3D, 8: LocationID is an uint32_t, Location's size is 32bit. Resolution: for 1km model-space is 4m
    static constexpr depth_t MAX_ALLOWED_DEPTH_ID = depth_t{ 19 };

    // static constexpr bool USE_PMR = true;

    static constexpr NodeGeometryStorage NODE_GEOMETRY_STORAGE = USE_MBR_ ? NodeGeometryStorage::MBR : NodeGeometryStorage::MinPoint;

    // Target number of elements in nodes
    static constexpr std::size_t DEFAULT_TARGET_ELEMENT_NUM_IN_NODES = 20; // TODO: set to 16

    // Associative container used for node storage (default: std::unordered_map)
    template<typename TKey, typename TValue, typename THash>
    using LinearNodeContainer = std::unordered_map<TKey, TValue, THash>;

    // Associative container used for reverse mapping storage (default: std::unordered_map)
    template<typename TKey, typename TValue, typename THash>
    using ReverseMap = std::unordered_map<TKey, TValue, THash>;
  };

  using PointConfiguration = Configuration<1.0, false>;

  template<bool IS_LOOSE_TREE>
  using BoxConfiguration = Configuration<IS_LOOSE_TREE ? 2.0 : 1.0, false>;

} // namespace OrthoTree
