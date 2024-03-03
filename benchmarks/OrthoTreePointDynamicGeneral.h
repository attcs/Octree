#pragma once

#include <vector>
#include <memory>
#include <span>

#include "../octree.h"

using namespace std;
using namespace OrthoTree;


template<
  size_t DIMENSION_NO,
  typename TVector,
  typename TBox,
  typename TRay,
  typename TPlane, 
  typename TGeonetry = double,
  typename adaptor_type = OrthoTree::AdaptorGeneral<DIMENSION_NO, TVector, TBox, TRay, TPlane, TGeonetry>>
class OrthoTreePointDynamicGeneral
{
  static size_t constexpr _nChild = 1 << DIMENSION_NO;

public:
  TBox box;
  array<unique_ptr<OrthoTreePointDynamicGeneral>, _nChild> vNode;
  vector<size_t> vid;

private:
  using AD = adaptor_type;

  struct IdEntityNode { size_t id, idNode; };

  void _process(span<IdEntityNode> aid, span<TVector const> const& vpt, size_t nDepthRemain, size_t nElementMax)
  {
    autoc nid = aid.size();
    if (nDepthRemain == 0 || nid < nElementMax)
    {
      vid.reserve(aid.size());
      for (autoc& [id, idNode] : aid)
        vid.emplace_back(id);

      return;
    }

    auto ptMiddle = TVector{};
    for (dim_t iDimension = 0; iDimension < DIMENSION_NO; ++iDimension)
      AD::SetPointC(ptMiddle, iDimension, (AD::GetBoxMaxC(box, iDimension) + AD::GetBoxMinC(box, iDimension)) * 0.5);

    for (auto& [id, idNode] : aid)
    {
      idNode = 0;
      for (dim_t iDimension = 0; iDimension < DIMENSION_NO; ++iDimension)
        idNode |= static_cast<size_t>(AD::GetPointC(ptMiddle, iDimension) < AD::GetPointC(vpt[id], iDimension)) << static_cast<size_t>(iDimension);
    }
    std::sort(begin(aid), end(aid), [&](autoc& idL, autoc& idR) { return idL.idNode < idR.idNode; });

    autoc itidNodeEnd = end(aid);
    for (auto itidNodeLast = begin(aid); itidNodeLast != itidNodeEnd;)
    {
      vNode[itidNodeLast->idNode] = make_unique<OrthoTreePointDynamicGeneral>();

      auto& node = *vNode[itidNodeLast->idNode].get();
      for (dim_t iDimension = 0; iDimension < DIMENSION_NO; ++iDimension)
      {
        autoc fGreater = ((itidNodeLast->idNode >> iDimension) & 1);
        AD::SetBoxMinC(node.box, iDimension, fGreater * AD::GetPointC(ptMiddle, iDimension) + !fGreater * AD::GetBoxMinC(box, iDimension));
        AD::SetBoxMaxC(node.box, iDimension, !fGreater * AD::GetPointC(ptMiddle, iDimension) + fGreater * AD::GetBoxMaxC(box, iDimension));
      }

      autoc itidNode = partition_point(itidNodeLast, itidNodeEnd, [&](autoc& p) { return p.idNode == itidNodeLast->idNode; });

      node._process({ itidNodeLast, itidNode }, vpt, nDepthRemain - 1, nElementMax);

      itidNodeLast = itidNode;
    }

  }


public:
  static OrthoTreePointDynamicGeneral Create(span<TVector const> const& vpt, size_t nDepthMax, std::optional<TBox> const& obox, size_t nElementMax)
  {
    autoc box = obox.has_value() ? *obox : AD::GetBoxOfPoints(vpt);

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




template<
  size_t DIMENSION_NO,
  typename TVector,
  typename TBox,
  typename TRay,
  typename TPlane,
  typename TGeonetry = double,
  typename adaptor_type = OrthoTree::AdaptorGeneral<DIMENSION_NO, TVector, TBox, TRay, TPlane, TGeonetry>>
class OrthoTreeBoxDynamicGeneral
{
  static size_t constexpr _nChild = 1 << DIMENSION_NO;

public:
  TBox box;
  array<unique_ptr<OrthoTreeBoxDynamicGeneral>, _nChild> vNode;
  vector<size_t> vid;

private:
  using AD = adaptor_type;

  struct IdEntityNode { size_t id, idNode; };

  void _process(span<IdEntityNode> aid, span<TBox const> const& vBox, size_t nDepthRemain, size_t nElementMax)
  {
    autoc nid = aid.size();
    if (nDepthRemain == 0 || nid < nElementMax)
    {
      vid.reserve(aid.size());
      for (autoc& [id, idNode] : aid)
        vid.emplace_back(id);

      return;
    }

    auto ptMiddle = TVector{};
    for (dim_t iDimension = 0; iDimension < DIMENSION_NO; ++iDimension)
      AD::SetPointC(ptMiddle, iDimension, (AD::GetBoxMaxC(box, iDimension) + AD::GetBoxMinC(box, iDimension)) * 0.5);

    for (auto& [id, idNode] : aid)
    {
      size_t idNode1 = 0;
      size_t idNode2 = 0;
      for (dim_t iDimension = 0; iDimension < DIMENSION_NO; ++iDimension)
      {
        idNode1 |= (AD::GetPointC(ptMiddle, iDimension) < AD::GetBoxMinC(vBox[id], iDimension)) << iDimension;
        idNode2 |= (AD::GetPointC(ptMiddle, iDimension) < AD::GetBoxMaxC(vBox[id], iDimension)) << iDimension;
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

        for (dim_t iDimension = 0; iDimension < DIMENSION_NO; ++iDimension)
        {
          autoc fGreater = ((itidNodeLast->idNode >> iDimension) & 1);
          AD::SetBoxMinC(node.box, iDimension, fGreater * AD::GetPointC(ptMiddle, iDimension) + !fGreater * AD::GetBoxMinC(box, iDimension));
          AD::SetBoxMaxC(node.box, iDimension, !fGreater * AD::GetPointC(ptMiddle, iDimension) + fGreater * AD::GetBoxMaxC(box, iDimension));
        }

        autoc itidNode = partition_point(itidNodeLast, itidNodeEnd, [&](autoc& p) { return p.idNode == itidNodeLast->idNode; });

        node._process({ itidNodeLast, itidNode }, vBox, nDepthRemain - 1, nElementMax);

        itidNodeLast = itidNode;
      }
    }

  }


public:
  static OrthoTreeBoxDynamicGeneral Create(span<TBox const> const& vBox, size_t nDepthMax, std::optional<TBox> const& obox = std::nullopt, size_t nElementMax = 20)
  {
    autoc box = obox.has_value() ? *obox : AD::GetBoxOfBoxes(vBox);

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
using OrthoTreePointDynamicND = OrthoTreePointDynamicGeneral<N, OrthoTree::VectorND<N>, OrthoTree::BoundingBoxND<N>, OrthoTree::RayND<N>, OrthoTree::PlaneND<N>>;
using OctreePointDynamic = OrthoTreePointDynamicND<3>;

template<size_t N>
using OrthoTreeBoxDynamicND = OrthoTreeBoxDynamicGeneral<N, OrthoTree::VectorND<N>, OrthoTree::BoundingBoxND<N>, OrthoTree::RayND<N>, OrthoTree::PlaneND<N>>;
using OctreeBoxDynamic = OrthoTreeBoxDynamicND<3>;
