#pragma once

#include <vector>
#include <memory>
#include <span>

#include "../octree.h"

using namespace std;


template<size_t nDimension, typename point_type, typename box_type, typename adaptor_type = OrthoTree::AdaptorGeneral<nDimension, point_type, box_type, double>, typename geometry_type = double>
class OrthoTreePointDynamicGeneral
{
  static size_t constexpr _nChild = 1 << nDimension;

public:
  box_type box;
  array<unique_ptr<OrthoTreePointDynamicGeneral>, _nChild> vNode;
  vector<size_t> vid;

private:
  using _Ad = typename adaptor_type;

  struct IdEntityNode { size_t id, idNode; };

  void _process(span<IdEntityNode> aid, span<point_type const> const& vpt, size_t nDepthRemain, size_t nElementMax)
  {
    autoc nid = aid.size();
    if (nDepthRemain == 0 || nid < nElementMax)
    {
      vid.reserve(aid.size());
      for (autoc& [id, idNode] : aid)
        vid.emplace_back(id);

      return;
    }

    autoc ptMiddle = _Ad::div(_Ad::add(_Ad::box_max_c(box), _Ad::box_min_c(box)), 2.0);
    for (auto& [id, idNode] : aid)
    {
      idNode = 0;
      for (size_t iDimension = 0; iDimension < nDimension; ++iDimension)
        idNode |= (_Ad::point_comp_c(ptMiddle, iDimension) < _Ad::point_comp_c(vpt[id], iDimension)) << iDimension;
    }
    sort(begin(aid), end(aid), [&](autoc& idL, autoc& idR) { return idL.idNode < idR.idNode; });

    autoc itidNodeEnd = end(aid);
    for (auto itidNodeLast = begin(aid); itidNodeLast != itidNodeEnd;)
    {
      vNode[itidNodeLast->idNode] = make_unique<OrthoTreePointDynamicGeneral>();

      auto& node = *vNode[itidNodeLast->idNode].get();
      for (size_t iDimension = 0; iDimension < nDimension; ++iDimension)
      {
        autoc fGreater = ((itidNodeLast->idNode >> iDimension) & 1);
        _Ad::point_comp(_Ad::box_min(node.box), iDimension) = fGreater * _Ad::point_comp_c(ptMiddle, iDimension) + !fGreater * _Ad::point_comp_c(_Ad::box_min_c(box), iDimension);
        _Ad::point_comp(_Ad::box_max(node.box), iDimension) = !fGreater * _Ad::point_comp_c(ptMiddle, iDimension) + fGreater * _Ad::point_comp_c(_Ad::box_max_c(box), iDimension);
      }

      autoc itidNode = partition_point(itidNodeLast, itidNodeEnd, [&](autoc& p) { return p.idNode == itidNodeLast->idNode; });

      node._process({ itidNodeLast, itidNode }, vpt, nDepthRemain - 1, nElementMax);

      itidNodeLast = itidNode;
    }

  }


public:
  static OrthoTreePointDynamicGeneral Create(span<point_type const> const& vpt, size_t nDepthMax, std::optional<box_type> const& obox, size_t nElementMax)
  {
    autoc box = obox.has_value() ? *obox : _Ad::box_of_points(vpt);

    autoc npt = vpt.size();
    auto aid = vector<IdEntityNode>(npt, IdEntityNode{});
    for (size_t i = 0; i < npt; ++i)
      aid[i].id = i;

    auto tree = OrthoTreePointDynamicGeneral();
    tree.box = box;
    tree._process(aid, vpt, nDepthMax, nElementMax);
    return tree;
  }

  size_t GetNodeSize() const
  {
    return std::ranges::count_if(vNode, [](autoc& node) -> bool { return node.get(); });
  }
};




template<size_t nDimension, typename point_type, typename box_type, typename adaptor_type = OrthoTree::AdaptorGeneral<nDimension, point_type, box_type, double>, typename geometry_type = double>
class OrthoTreeBoxDynamicGeneral
{
  static size_t constexpr _nChild = 1 << nDimension;

public:
  box_type box;
  array<unique_ptr<OrthoTreeBoxDynamicGeneral>, _nChild> vNode;
  vector<size_t> vid;

private:
  using _Ad = typename adaptor_type;

  struct IdEntityNode { size_t id, idNode; };

  void _process(span<IdEntityNode> aid, span<box_type const> const& vBox, size_t nDepthRemain, size_t nElementMax)
  {
    autoc nid = aid.size();
    if (nDepthRemain == 0 || nid < nElementMax)
    {
      vid.reserve(aid.size());
      for (autoc& [id, idNode] : aid)
        vid.emplace_back(id);

      return;
    }

    autoc ptMiddle = _Ad::div(_Ad::add(_Ad::box_max_c(box), _Ad::box_min_c(box)), 2.0);
    for (auto& [id, idNode] : aid)
    {
      auto idNode1 = 0;
      auto idNode2 = 0;
      for (size_t iDimension = 0; iDimension < nDimension; ++iDimension)
      {
        idNode1 |= (_Ad::point_comp_c(ptMiddle, iDimension) < _Ad::point_comp_c(_Ad::box_min_c(vBox[id]), iDimension)) << iDimension;
        idNode2 |= (_Ad::point_comp_c(ptMiddle, iDimension) < _Ad::point_comp_c(_Ad::box_max_c(vBox[id]), iDimension)) << iDimension;
      }

      idNode = idNode1 == idNode2 ? idNode1 : _nChild;
    }

    sort(begin(aid), end(aid), [&](autoc& idL, autoc& idR) { return idL.idNode < idR.idNode; });

    autoc itidNodeEnd = end(aid);
    for (auto itidNodeLast = begin(aid); itidNodeLast != itidNodeEnd;)
    {
      if (itidNodeLast->idNode == _nChild)
      {
        autoc nElement = std::distance(itidNodeLast, itidNodeEnd);
        vid.reserve(nElement);
        for (; itidNodeLast != itidNodeEnd; ++itidNodeLast)
          vid.emplace_back(itidNodeLast->id);

        break;
      }

      {
        vNode[itidNodeLast->idNode] = make_unique<OrthoTreeBoxDynamicGeneral>();

        auto& node = *vNode[itidNodeLast->idNode].get();

        for (size_t iDimension = 0; iDimension < nDimension; ++iDimension)
        {
          autoc fGreater = ((itidNodeLast->idNode >> iDimension) & 1);
          _Ad::point_comp(_Ad::box_min(node.box), iDimension) = fGreater * _Ad::point_comp_c(ptMiddle, iDimension) + !fGreater * _Ad::point_comp_c(_Ad::box_min_c(box), iDimension);
          _Ad::point_comp(_Ad::box_max(node.box), iDimension) = !fGreater * _Ad::point_comp_c(ptMiddle, iDimension) + fGreater * _Ad::point_comp_c(_Ad::box_max_c(box), iDimension);
        }

        autoc itidNode = partition_point(itidNodeLast, itidNodeEnd, [&](autoc& p) { return p.idNode == itidNodeLast->idNode; });

        node._process({ itidNodeLast, itidNode }, vBox, nDepthRemain - 1, nElementMax);

        itidNodeLast = itidNode;
      }
    }

  }


public:
  static OrthoTreeBoxDynamicGeneral Create(span<box_type const> const& vBox, size_t nDepthMax, std::optional<box_type> const& obox, size_t nElementMax)
  {
    autoc box = obox.has_value() ? *obox : _Ad::box_of_boxes(vBox);

    autoc nEnt = vBox.size();
    auto aid = vector<IdEntityNode>(nEnt, IdEntityNode{});
    for (size_t i = 0; i < nEnt; ++i)
      aid[i].id = i;

    auto tree = OrthoTreeBoxDynamicGeneral();
    tree.box = box;
    tree._process(aid, vBox, nDepthMax, nElementMax);
    return tree;
  }

  size_t GetNodeSize() const
  {
    return std::ranges::count_if(vNode, [](autoc& node) -> bool { return node.get(); });
  }
};


template<size_t N>
using OrthoTreePointDynamicND = OrthoTreePointDynamicGeneral<N, OrthoTree::PointND<N>, OrthoTree::BoundingBoxND<N>>;
using OctreePointDynamic = OrthoTreePointDynamicND<3>;

template<size_t N>
using OrthoTreeBoxDynamicND = OrthoTreeBoxDynamicGeneral<N, OrthoTree::PointND<N>, OrthoTree::BoundingBoxND<N>>;
using OctreeBoxDynamic = OrthoTreeBoxDynamicND<3>;
