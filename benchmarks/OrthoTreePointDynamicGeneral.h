#pragma once

#include <vector>
#include <memory>
#include <span>

#include "../octree.h"

using namespace std;


template<size_t nDimension, typename point_type, typename box_type, typename adaptor_type = NTree::AdaptorGeneral<nDimension, point_type, box_type, double>, typename geometry_type = double>
class OrthoTreePointDynamicGeneral
{
  static size_t constexpr _nChild = 1 << nDimension;

public:
  box_type box;
  array<unique_ptr<OrthoTreePointDynamicGeneral>, _nChild> vNode;
  vector<size_t> vid;

private:
  using _Ad = typename adaptor_type;

  struct IdPointNode { size_t idPt, idNode; };

  void _process(span<IdPointNode> aid, span<point_type const> const& vpt, size_t nDepthRemain, size_t nElementMax)
  {
    autoc nid = aid.size();
    if (nDepthRemain == 0 || nid < nElementMax)
    {
      vid.reserve(aid.size());
      for (autoc& [idPt, idNode] : aid)
        vid.emplace_back(idPt);

      return;
    }

    autoc ptMiddle = _Ad::div(_Ad::add(_Ad::box_max_c(box), _Ad::box_min_c(box)), 2.0);
    for (auto& [idPt, idNode] : aid)
    {
      idNode = 0;
      for (size_t iDimension = 0; iDimension < nDimension; ++iDimension)
        idNode |= (_Ad::point_comp_c(ptMiddle, iDimension) < _Ad::point_comp_c(vpt[idPt], iDimension)) << iDimension;
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
  static OrthoTreePointDynamicGeneral Create(span<point_type const> const& vpt, size_t nDepthMax, size_t nElementMax)
  {
    autoc box = _Ad::box_of_points(vpt);

    autoc npt = vpt.size();
    auto aid = vector<IdPointNode>(npt, IdPointNode{});
    for (size_t i = 0; i < npt; ++i)
      aid[i].idPt = i;

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

template<size_t N>
using OrthoTreePointDynamicND = OrthoTreePointDynamicGeneral<N, NTree::PointND<N>, NTree::BoundingBoxND<N>>;
using OctreePointDynamic = OrthoTreePointDynamicND<3>;