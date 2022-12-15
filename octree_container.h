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
    using vector_type = typename OrthoTree::vector_type;
    using box_type = typename OrthoTree::box_type;
    using max_element_type = typename OrthoTree::max_element_type;

  protected:
    OrthoTree _tree;
    vector<data_type> _vData;

  public: // Constructors
    OrthoTreeContainerBase() noexcept = default;
    OrthoTreeContainerBase(span<data_type const> const& vData, depth_type nDepthMax, std::optional<box_type> const& oBoxSpace = std::nullopt, max_element_type nElementMaxInNode = OrthoTree::max_element_default, bool fParallelCreate = false) noexcept
      : _vData(vData.begin(), vData.end())
    {
      if (fParallelCreate)
        OrthoTree::template Create<std::execution::parallel_unsequenced_policy>(_tree, vData, nDepthMax, oBoxSpace, nElementMaxInNode);
      else
        OrthoTree::Create(_tree, vData, nDepthMax, oBoxSpace, nElementMaxInNode);
    }

    OrthoTreeContainerBase(vector<data_type>&& vData, depth_type nDepthMax, std::optional<box_type> const& oBoxSpace = std::nullopt, max_element_type nElementMaxInNode = OrthoTree::max_element_default, bool fParallelCreate = false) noexcept
      : _vData(vData)
    {
      if (fParallelCreate)
        OrthoTree::template Create<std::execution::parallel_unsequenced_policy>(_tree, _vData, nDepthMax, oBoxSpace, nElementMaxInNode);
      else
        OrthoTree::Create(_tree, _vData, nDepthMax, oBoxSpace, nElementMaxInNode);
    }


  public: // Member functions

    constexpr OrthoTree const& GetCore() const noexcept { return _tree; }
    constexpr vector<data_type> const& GetData() const noexcept { return _vData; }

    constexpr void Init(box_type const& boxSpace, depth_type nDepthMax, max_element_type nElementMaxInNode = OrthoTree::max_element_default) noexcept
    {
      _tree.Init(boxSpace, nDepthMax, nElementMaxInNode);
    }

    bool Add(data_type const& data, bool fInsertToLeaf = false) noexcept
    {
      autoc id = _vData.size();
      if (_tree.Insert(id, data, fInsertToLeaf))
      {
        _vData.emplace_back(data);
        return true;
      }

      return false;
    }

    bool Update(entity_id_type id, data_type const& dataNew, bool fInsertToLeaf = false) noexcept
    {
      if (_tree.Update(id, _vData[id], dataNew, fInsertToLeaf))
      {
        _vData[id] = dataNew;
        return true;
      }

      return false;
    }

    bool Erase(entity_id_type id) noexcept
    {
      if (_tree.Erase(id, _vData[id]))
      {
        _vData.erase(std::next(std::begin(_vData), id));
        return true;
      }

      return false;
    }

    inline void Clear() noexcept { _tree.Clear(); _vData.clear(); }
    inline void Reset() noexcept { _tree.Reset(); _vData.clear(); }
  };


  // General OrthoTree container for point types
  template<typename OrthoTree, typename data_type>
  class OrthoTreeContainerPoint : public OrthoTreeContainerBase<OrthoTree, data_type>
  {
  public:
    using base = OrthoTreeContainerBase<OrthoTree, data_type>;
    using _Ad = typename base::_Ad;
    using vector_type = typename base::vector_type;
    using box_type = typename base::box_type;
    using max_element_type = typename OrthoTree::max_element_type;

    using base::base; // inherits all constructors

  public: // Edit functions

    template<typename execution_policy_type = std::execution::unsequenced_policy>
    static OrthoTreeContainerPoint Create(span<data_type const> const& vData, depth_type nDepthMax, std::optional<box_type> const& oBoxSpace = std::nullopt, max_element_type nElementMaxInNode = OrthoTree::max_element_default) noexcept
    {
      auto otc = OrthoTreeContainerPoint();
      otc._vData = vector(vData.begin(), vData.end());
      OrthoTree::template Create<execution_policy_type>(otc._tree, vData, nDepthMax, oBoxSpace, nElementMaxInNode);
      return otc;
    }

    template<typename execution_policy_type = std::execution::unsequenced_policy>
    static OrthoTreeContainerPoint Create(vector<data_type>&& vData, depth_type nDepthMax, std::optional<box_type> const& oBoxSpace = std::nullopt, max_element_type nElementMaxInNode = OrthoTree::max_element_default) noexcept
    {
      auto otc = OrthoTreeContainerPoint();
      otc._vData = std::move(vData);
      OrthoTree::template Create<execution_policy_type>(otc._tree, otc._vData, nDepthMax, oBoxSpace, nElementMaxInNode);
      return otc;
    }

    template<typename execution_policy_type = std::execution::unsequenced_policy>
    void Move(vector_type const& vMove) noexcept
    {
      this->_tree.template Move<execution_policy_type>(vMove);
      auto ep = execution_policy_type{}; // GCC 11.3
      std::for_each(ep, std::begin(this->_vData), std::end(this->_vData), [&vMove](auto& pt) { pt = _Ad::add(pt, vMove); });
    }


  public:

    // Range search
    template<bool fLeafNodeContainsElementOnly = false>
    inline vector<entity_id_type> RangeSearch(box_type const& range) const noexcept
    {
      return this->_tree.RangeSearch(range, this->_vData);
    }

    // K Nearest Neighbor
    inline vector<entity_id_type> GetNearestNeighbors(vector_type const& pt, size_t k) const noexcept
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
    using vector_type = typename base::vector_type;
    using box_type = typename base::box_type;
    using max_element_type = typename OrthoTree::max_element_type;

    using base::base; // inherits all constructors

  public: // Edit functions

    template<typename execution_policy_type = std::execution::unsequenced_policy>
    static OrthoTreeContainerBox Create(span<data_type const> const& vData, depth_type nDepthMax, std::optional<box_type> const& oBoxSpace = std::nullopt, max_element_type nElementMaxInNode = OrthoTree::max_element_default) noexcept
    {
      auto otc = OrthoTreeContainerBox();
      otc._vData = vector(vData.begin(), vData.end());
      OrthoTree::template Create<execution_policy_type>(otc._tree, vData, nDepthMax, oBoxSpace, nElementMaxInNode);
      return otc;
    }

    template<typename execution_policy_type = std::execution::unsequenced_policy>
    static OrthoTreeContainerBox Create(vector<data_type>&& vData, depth_type nDepthMax, std::optional<box_type> const& oBoxSpace = std::nullopt, max_element_type nElementMaxInNode = OrthoTree::max_element_default) noexcept
    {
      auto otc = OrthoTreeContainerBox();
      otc._vData = std::move(vData);
      OrthoTree::template Create<execution_policy_type>(otc._tree, otc._vData, nDepthMax, oBoxSpace, nElementMaxInNode);
      return otc;
    }

    template<typename execution_policy_type = std::execution::unsequenced_policy>
    void Move(vector_type const& vMove) noexcept
    {
      this->_tree.template Move<execution_policy_type>(vMove);
      auto ep = execution_policy_type{}; // GCC 11.3
      std::for_each(ep, std::begin(this->_vData), std::end(this->_vData), [&vMove](auto& box)
      {
        _Ad::move_box(box, vMove);
      });
    }


  public: // Search functions

    // Pick search
    inline vector<entity_id_type> PickSearch(vector_type const& ptPick) const noexcept
    {
      return this->_tree.PickSearch(ptPick, this->_vData);
    }

    // Range search
    template<bool fFullyContained = true>
    inline vector<entity_id_type> RangeSearch(box_type const& range) const noexcept
    {
      return this->_tree.template RangeSearch<fFullyContained>(range, this->_vData);
    }


  public: // Collision detection

    // Collision detection between the contained elements
    template<typename execution_policy_type = std::execution::unsequenced_policy>
    inline vector<std::pair<entity_id_type, entity_id_type>> CollisionDetection() const noexcept
    {
      return this->_tree.template CollisionDetection<execution_policy_type>(this->_vData);
    }

    // Collision detection with another tree
    inline vector<std::pair<entity_id_type, entity_id_type>> CollisionDetection(OrthoTreeContainerBox const& treeOther) const noexcept
    {
      return this->_tree.CollisionDetection(this->_tree, this->_vData, treeOther._tree, treeOther._vData);
    }

    // Collision detection between trees
    static inline vector<std::pair<entity_id_type, entity_id_type>> CollisionDetection(OrthoTreeContainerBox const& treeL, OrthoTreeContainerBox const& treeR) noexcept
    {
      return treeL.CollisionDetection(treeR);
    }


  public: // Ray intersection

    // Get all box which is intersected by the ray in order
    inline vector<entity_id_type> RayIntersectedAll(vector_type const& rayBasePoint, vector_type const& rayHeading) const noexcept
    {
      return this->_tree.RayIntersectedAll(rayBasePoint, rayHeading, this->_vData);
    }

    // Get first box which is intersected by the ray
    inline std::optional<entity_id_type> RayIntersectedFirst(vector_type const& rayBasePoint, vector_type const& rayHeading) const noexcept
    {
      return this->_tree.RayIntersectedFirst(rayBasePoint, rayHeading, this->_vData);
    }
  };

  template<dim_type N>
  using TreePointContainerND = OrthoTreeContainerPoint<TreePointND<N>, PointND<N>>;

  template<dim_type N, uint32_t nSplitStrategyAdditionalDepth = 2>
  using TreeBoxContainerND = OrthoTreeContainerBox<TreeBoxND<N, nSplitStrategyAdditionalDepth>, BoundingBoxND<N>>;

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
