#pragma once

#include <vector>
#include <memory>
#include <span>

#include "../../octree.h"

using namespace std;
using namespace OrthoTree;


template<
  size_t DIMENSION_NO,
  typename TVector,
  typename TBox,
  typename TRay,
  typename TPlane, 
  typename TGeometry = double,
  typename adaptor_type = OrthoTree::AdaptorGeneral<DIMENSION_NO, TVector, TBox, TRay, TPlane, TGeometry>>
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
    auto const nid = aid.size();
    if (nDepthRemain == 0 || nid < nElementMax)
    {
      vid.reserve(aid.size());
      for (auto const& [id, idNode] : aid)
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
    std::sort(begin(aid), end(aid), [&](auto const& idL, auto const& idR) { return idL.idNode < idR.idNode; });

    auto const itidNodeEnd = end(aid);
    for (auto itidNodeLast = begin(aid); itidNodeLast != itidNodeEnd;)
    {
      vNode[itidNodeLast->idNode] = make_unique<OrthoTreePointDynamicGeneral>();

      auto& node = *vNode[itidNodeLast->idNode].get();
      for (dim_t iDimension = 0; iDimension < DIMENSION_NO; ++iDimension)
      {
        auto const fGreater = ((itidNodeLast->idNode >> iDimension) & 1);
        AD::SetBoxMinC(node.box, iDimension, fGreater * AD::GetPointC(ptMiddle, iDimension) + !fGreater * AD::GetBoxMinC(box, iDimension));
        AD::SetBoxMaxC(node.box, iDimension, !fGreater * AD::GetPointC(ptMiddle, iDimension) + fGreater * AD::GetBoxMaxC(box, iDimension));
      }

      auto const itidNode = partition_point(itidNodeLast, itidNodeEnd, [&](auto const& p) { return p.idNode == itidNodeLast->idNode; });

      node._process({ itidNodeLast, itidNode }, vpt, nDepthRemain - 1, nElementMax);

      itidNodeLast = itidNode;
    }

  }

  static inline TBox BoxInvertedInit() noexcept
  {
    auto ext = TBox{};

    for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
    {
      AD::SetBoxMinC(ext, dimensionID, std::numeric_limits<TGeometry>::max());
      AD::SetBoxMaxC(ext, dimensionID, std::numeric_limits<TGeometry>::lowest());
    }

    return ext;
  }

  static TBox GetBoxOfPoints(std::span<TVector const> const& points) noexcept
  {
    auto ext = BoxInvertedInit();
    for (auto const& point : points)
      for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
      {
        if (AD::GetBoxMinC(ext, dimensionID) > AD::GetPointC(point, dimensionID))
          AD::SetBoxMinC(ext, dimensionID, AD::GetPointC(point, dimensionID));

        if (AD::GetBoxMaxC(ext, dimensionID) < AD::GetPointC(point, dimensionID))
          AD::SetBoxMaxC(ext, dimensionID, AD::GetPointC(point, dimensionID));
      }

    return ext;
  }

public:
  static OrthoTreePointDynamicGeneral Create(span<TVector const> const& vpt, size_t nDepthMax, std::optional<TBox> const& obox, size_t nElementMax)
  {
    auto const box = obox.has_value() ? *obox : GetBoxOfPoints(vpt);

    auto const npt = vpt.size();
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
    return std::ranges::count_if(vNode, [](auto const& node) -> bool { return node.get(); });
  }
};




template<
  size_t DIMENSION_NO,
  typename TVector,
  typename TBox,
  typename TRay,
  typename TPlane,
  typename TGeometry = double,
  typename adaptor_type = OrthoTree::AdaptorGeneral<DIMENSION_NO, TVector, TBox, TRay, TPlane, TGeometry>>
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
    auto const nid = aid.size();
    if (nDepthRemain == 0 || nid < nElementMax)
    {
      vid.reserve(aid.size());
      for (auto const& [id, idNode] : aid)
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

    sort(begin(aid), end(aid), [&](auto const& idL, auto const& idR) { return idL.idNode < idR.idNode; });

    auto const itidNodeEnd = end(aid);
    for (auto itidNodeLast = begin(aid); itidNodeLast != itidNodeEnd;)
    {
      if (itidNodeLast->idNode == _nChild)
      {
        auto const nElement = std::distance(itidNodeLast, itidNodeEnd);
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
          auto const fGreater = ((itidNodeLast->idNode >> iDimension) & 1);
          AD::SetBoxMinC(node.box, iDimension, fGreater * AD::GetPointC(ptMiddle, iDimension) + !fGreater * AD::GetBoxMinC(box, iDimension));
          AD::SetBoxMaxC(node.box, iDimension, !fGreater * AD::GetPointC(ptMiddle, iDimension) + fGreater * AD::GetBoxMaxC(box, iDimension));
        }

        auto const itidNode = partition_point(itidNodeLast, itidNodeEnd, [&](auto const& p) { return p.idNode == itidNodeLast->idNode; });

        node._process({ itidNodeLast, itidNode }, vBox, nDepthRemain - 1, nElementMax);

        itidNodeLast = itidNode;
      }
    }

  }

  static inline TBox BoxInvertedInit() noexcept
  {
    auto ext = TBox{};

    for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
    {
      AD::SetBoxMinC(ext, dimensionID, std::numeric_limits<TGeometry>::max());
      AD::SetBoxMaxC(ext, dimensionID, std::numeric_limits<TGeometry>::lowest());
    }

    return ext;
  }

  static TBox GetBoxOfBoxes(std::span<TBox const> const& boxes) noexcept
  {
    auto ext = BoxInvertedInit();
    for (auto const& e : boxes)
    {
      for (dim_t dimensionID = 0; dimensionID < DIMENSION_NO; ++dimensionID)
      {
        if (AD::GetBoxMinC(ext, dimensionID) > AD::GetBoxMinC(e, dimensionID))
          AD::SetBoxMinC(ext, dimensionID, AD::GetBoxMinC(e, dimensionID));

        if (AD::GetBoxMaxC(ext, dimensionID) < AD::GetBoxMaxC(e, dimensionID))
          AD::SetBoxMaxC(ext, dimensionID, AD::GetBoxMaxC(e, dimensionID));
      }
    }
    return ext;
  }

public:
  static OrthoTreeBoxDynamicGeneral Create(span<TBox const> const& vBox, size_t nDepthMax, std::optional<TBox> const& obox = std::nullopt, size_t nElementMax = 20)
  {
    auto const box = obox.has_value() ? *obox : GetBoxOfBoxes(vBox);

    auto const nEnt = vBox.size();
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
    return std::ranges::count_if(vNode, [](auto const& node) -> bool { return node.get(); });
  }
};


template<size_t N>
using OrthoTreePointDynamicND = OrthoTreePointDynamicGeneral<N, OrthoTree::VectorND<N>, OrthoTree::BoundingBoxND<N>, OrthoTree::RayND<N>, OrthoTree::PlaneND<N>>;
using OctreePointDynamic = OrthoTreePointDynamicND<3>;

template<size_t N>
using OrthoTreeBoxDynamicND = OrthoTreeBoxDynamicGeneral<N, OrthoTree::VectorND<N>, OrthoTree::BoundingBoxND<N>, OrthoTree::RayND<N>, OrthoTree::PlaneND<N>>;
using OctreeBoxDynamic = OrthoTreeBoxDynamicND<3>;
