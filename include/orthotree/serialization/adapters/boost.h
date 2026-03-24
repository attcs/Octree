#pragma once

#include "../../adapters/boost.h"
#include "../../serialization.h"
#include <utility>

namespace OrthoTree
{
  namespace detail
  {
    static constexpr const char* point_dimension_names[] = { "0",  "1",  "2",  "3",  "4",  "5",  "6",  "7",  "8",  "9",  "10", "11", "12",
                                                             "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25",
                                                             "26", "27", "28", "29", "30", "31", "32", "33", "34", "35", "36", "37", "38",
                                                             "39", "40", "41", "42", "43", "44", "45", "46", "47", "48", "49", "50", "51",
                                                             "52", "53", "54", "55", "56", "57", "58", "59", "60", "61", "62", "63" };

    template<std::size_t I, typename TArchive, typename T, std::size_t D, typename CS>
    void serialize_point_dimension(TArchive& ar, boost::geometry::model::point<T, D, CS>& pt)
    {
      T val = boost::geometry::get<I>(pt);

      const char* name = "unknown";
      if constexpr (0 <= I && I < 64)
        name = point_dimension_names[I];

      ar& make_nvp(name, val);

      if (OrthoTree::is_loading_archive(ar))
        boost::geometry::set<I>(pt, val);
    }

    template<typename TArchive, typename T, std::size_t D, typename CS, std::size_t... Is>
    void serialize_point_internal(TArchive& ar, boost::geometry::model::point<T, D, CS>& pt, std::index_sequence<Is...>)
    {
      (serialize_point_dimension<Is>(ar, pt), ...);
    }
  } // namespace detail

  // boost::geometry::model::point
  template<typename TArchive, typename T, std::size_t D, typename CS>
  void serialize(TArchive& ar, boost::geometry::model::point<T, D, CS>& pt)
  {
    detail::serialize_point_internal(ar, pt, std::make_index_sequence<D>{});
  }

  // boost::geometry::model::box
  template<typename TArchive, typename Point>
  void serialize(TArchive& ar, boost::geometry::model::box<Point>& box)
  {
    ar& make_nvp("min_corner", box.min_corner());
    ar& make_nvp("max_corner", box.max_corner());
  }

  // boost::geometry::model::rayNd_t
  template<typename TArchive, dim_t DIM, typename T>
  void serialize(TArchive& ar, boost::geometry::model::rayNd_t<DIM, T>& ray)
  {
    ar& ORTHOTREE_NVP_M(ray, origin);
    ar& ORTHOTREE_NVP_M(ray, direction);
  }

  // boost::geometry::model::planeNd_t
  template<typename TArchive, dim_t DIM, typename T>
  void serialize(TArchive& ar, boost::geometry::model::planeNd_t<DIM, T>& plane)
  {
    ar& ORTHOTREE_NVP_M(plane, origo_distance);
    ar& ORTHOTREE_NVP_M(plane, normal);
  }
} // namespace OrthoTree
