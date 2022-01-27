/*
MIT License

Copyright (c) 2021 Attila Csikós

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef ORTHOTREE_CONTAINER_GUARD
#define ORTHOTREE_CONTAINER_GUARD

//////////////////////////////////////////////////////////////////////////
// Container type to make easier the OrthoTree usage
//////////////////////////////////////////////////////////////////////////



namespace OrthoTree
{
  template<typename OrthoTree, typename data_type>
  class OrthoTreeContainerBase
  {
  public:
    using _Ad = typename OrthoTree::_Ad;
    using vector_type = OrthoTree::vector_type;
    using box_type = OrthoTree::box_type;

  protected:
    OrthoTree _tree;
    vector<data_type> _vData;

  public: // Constructors
    OrthoTreeContainerBase() = default;
    OrthoTreeContainerBase(span<data_type const> const& vData, OrthoTree::depth_type nDepthMax, std::optional<box_type> const& oBoxSpace = std::nullopt, OrthoTree::max_element_type nElementMaxInNode = OrthoTree::max_element_default, bool fParallelCreate = false)
      : _vData(vData.begin(), vData.end())
    {
      if (fParallelCreate)
        OrthoTree::template Create<std::execution::parallel_unsequenced_policy>(_tree, vData, nDepthMax, oBoxSpace, nElementMaxInNode);
      else
        OrthoTree::Create(_tree, vData, nDepthMax, oBoxSpace, nElementMaxInNode);
    }

    OrthoTreeContainerBase(vector<data_type>&& vData, OrthoTree::depth_type nDepthMax, std::optional<box_type> const& oBoxSpace = std::nullopt, OrthoTree::max_element_type nElementMaxInNode = OrthoTree::max_element_default, bool fParallelCreate = false) 
      : _vData(vData)
    {
      if (fParallelCreate)
        OrthoTree::template Create<std::execution::parallel_unsequenced_policy>(_tree, _vData, nDepthMax, oBoxSpace, nElementMaxInNode);
      else
        OrthoTree::Create(_tree, _vData, nDepthMax, oBoxSpace, nElementMaxInNode);
    }


  public: // Member functions

    constexpr OrthoTree const& GetCore() const { return _tree; }
    constexpr vector<data_type> const& GetData() const { return _vData; }

    constexpr void Init(box_type const& boxSpace, OrthoTree::depth_type nDepthMax, OrthoTree::max_element_type nElementMaxInNode = OrthoTree::max_element_default)
    {
      _tree.Init(boxSpace, nDepthMax, nElementMaxInNode);
    }

    bool Add(data_type const& data, bool fInsertToLeaf = false)
    {
      autoc id = _vData.size();
      if (_tree.Insert(id, data, fInsertToLeaf))
      {
        _vData.emplace_back(data);
        return true;
      }

      return false;
    }

    bool Update(entity_id_type id, data_type const& dataNew, bool fInsertToLeaf = false)
    {
      if (_tree.Update(id, _vData[id], dataNew, fInsertToLeaf))
      {
        _vData[id] = dataNew;
        return true;
      }

      return false;
    }

    bool Erase(entity_id_type id)
    {
      if (_tree.Erase(id, _vData[id]))
      {
        _vData.erase(std::next(std::begin(_vData), id));
        return true;
      }

      return false;
    }

    inline void Clear() { _tree.Clear(); _vData.clear(); }

  };


  // General OrthoTree container for point types
  template<typename OrthoTree, typename data_type>
  class OrthoTreeContainerPoint : public OrthoTreeContainerBase<OrthoTree, data_type>
  {
  public:
    using base = OrthoTreeContainerBase<OrthoTree, data_type>;
    using _Ad = typename base::_Ad;
    using vector_type = base::vector_type;
    using box_type = base::box_type;

    using base::base; // inherits all constructors

  public: // Edit functions

    template<typename execution_policy_type = std::execution::unsequenced_policy>
    static OrthoTreeContainerPoint Create(span<data_type const> const& vData, OrthoTree::depth_type nDepthMax, std::optional<box_type> const& oBoxSpace = std::nullopt, OrthoTree::max_element_type nElementMaxInNode = OrthoTree::max_element_default)
    {
      auto otc = OrthoTreeContainerPoint();
      otc._vData(vData.begin(), vData.end());
      OrthoTree::template Create<execution_policy_type>(otc._tree, vData, nDepthMax, oBoxSpace, nElementMaxInNode);
      return otc;
    }

    template<typename execution_policy_type = std::execution::unsequenced_policy>
    static OrthoTreeContainerPoint Create(vector<data_type>&& vData, OrthoTree::depth_type nDepthMax, std::optional<box_type> const& oBoxSpace = std::nullopt, OrthoTree::max_element_type nElementMaxInNode = OrthoTree::max_element_default)
    {
      auto otc = OrthoTreeContainerPoint();
      otc._vData = vData;
      OrthoTree::template Create<execution_policy_type>(otc._tree, otc._vData, nDepthMax, oBoxSpace, nElementMaxInNode);
      return otc;
    }

    template<typename execution_policy_type = std::execution::unsequenced_policy>
    void Move(vector_type const& vMove)
    {
      this->_tree.Move<execution_policy_type>(vMove);
      std::for_each(execution_policy_type{}, std::begin(this->_vData), std::end(this->_vData), [&vMove](auto& pt) { pt = _Ad::add(pt, vMove); });
    }


  public:

    // Range search
    template<bool fLeafNodeContainsElementOnly = false>
    inline vector<entity_id_type> RangeSearch(box_type const& range) const
    {
      return this->_tree.RangeSearch(range, this->_vData);
    }

    // K Nearest Neighbor
    inline vector<entity_id_type> GetNearestNeighbors(vector_type const& pt, size_t k) const
    {
      return this->_tree.GetNearestNeighbors(pt, k, this->_vData);
    }
  };


  // General OrthoTree container for box types
  template<typename OrthoTree, typename data_type>
  class OrthoTreeContainerBox : public OrthoTreeContainerBase<OrthoTree, data_type>
  {
  public:
    using base = OrthoTreeContainerBase<OrthoTree, data_type>;
    using _Ad = typename base::_Ad;
    using vector_type = base::vector_type;
    using box_type = base::box_type;

    using base::base; // inherits all constructors

  public: // Edit functions

    template<typename execution_policy_type = std::execution::unsequenced_policy>
    static OrthoTreeContainerBox Create(span<data_type const> const& vData, OrthoTree::depth_type nDepthMax, std::optional<box_type> const& oBoxSpace = std::nullopt, OrthoTree::max_element_type nElementMaxInNode = OrthoTree::max_element_default)
    {
      auto otc = OrthoTreeContainerBox();
      otc._vData = vector(vData.begin(), vData.end());
      OrthoTree::template Create<execution_policy_type>(otc._tree, vData, nDepthMax, oBoxSpace, nElementMaxInNode);
      return otc;
    }

    template<typename execution_policy_type = std::execution::unsequenced_policy>
    static OrthoTreeContainerBox Create(vector<data_type>&& vData, OrthoTree::depth_type nDepthMax, std::optional<box_type> const& oBoxSpace = std::nullopt, OrthoTree::max_element_type nElementMaxInNode = OrthoTree::max_element_default)
    {
      auto otc = OrthoTreeContainerBox();
      otc._vData = vData;
      OrthoTree::template Create<execution_policy_type>(otc._tree, otc._vData, nDepthMax, oBoxSpace, nElementMaxInNode);
      return otc;
    }

    template<typename execution_policy_type = std::execution::unsequenced_policy>
    void Move(vector_type const& vMove)
    {
      this->_tree.Move<execution_policy_type>(vMove);

      std::for_each(execution_policy_type{}, std::begin(this->_vData), std::end(this->_vData), [&vMove](auto& box)
      {
        _Ad::move_box(box, vMove);
      });
    }


  public: // Search functions

    // Pick search
    inline vector<entity_id_type> PickSearch(vector_type const& ptPick) const
    {
      return this->_tree.PickSearch(ptPick, this->_vData);
    }

    // Range search
    template<bool fFullyContained = true>
    inline vector<entity_id_type> RangeSearch(box_type const& range) const
    {
      return this->_tree.RangeSearch<fFullyContained>(range, this->_vData);
    }


  public: // Collision detection

    // Collision detection between the contained elements
    template<typename execution_policy_type = std::execution::unsequenced_policy>
    inline vector<std::pair<entity_id_type, entity_id_type>> CollisionDetection() const
    {
      return this->_tree.template CollisionDetection<execution_policy_type>(this->_vData);
    }

    // Collision detection with another tree
    inline vector<std::pair<entity_id_type, entity_id_type>> CollisionDetection(OrthoTreeContainerBox const& treeOther) const
    {
      return this->_tree.CollisionDetection(this->_tree, this->_vData, treeOther->_tree, treeOther->_vData);
    }

    // Collision detection between trees
    static inline vector<std::pair<entity_id_type, entity_id_type>> CollisionDetection(OrthoTreeContainerBox const& treeL, OrthoTreeContainerBox const& treeR)
    {
      return treeL.CollisionDetection(treeR);
    }


  public: // Ray intersection

    // Get all box which is intersected by the ray in order
    inline vector<entity_id_type> RayIntersectedAll(vector_type const& rayBasePoint, vector_type const& rayHeading) const
    {
      return this->_tree.RayIntersectedAll(rayBasePoint, rayHeading, this->_vData);
    }

    // Get first box which is intersected by the ray
    inline std::optional<entity_id_type> RayIntersectedFirst(vector_type const& rayBasePoint, vector_type const& rayHeading) const
    {
      return this->_tree.RayIntersectedFirst(rayBasePoint, rayHeading, this->_vData);
    }
  };

  template<dim_type N>
  using TreePointContainerND = OrthoTreeContainerPoint<TreePointND<N>, PointND<N>>;

  template<dim_type N>
  using TreeBoxContainerND = OrthoTreeContainerBox<TreeBoxND<N>, BoundingBoxND<N>>;

  // Dualtree for points
  using DualtreePointC = TreePointContainerND<1>;

  // Dualtree for bounding boxes
  using DualtreeBoxC = TreeBoxContainerND<1>;

  // Quadtree for points
  using QuadtreePointC = TreePointContainerND<2>;

  // Quadtree for bounding boxes
  using QuadtreeBoxC = TreeBoxContainerND<2>;

  // Octree for points
  using OctreePointC = TreePointContainerND<3>;

  // Octree for bounding boxes
  using OctreeBoxC = TreeBoxContainerND<3>;

}
#endif // ORTHOTREE_CONTAINER_GUARD
