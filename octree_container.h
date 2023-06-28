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
    using AD = typename OrthoTree::AD;
    using vector_type = typename OrthoTree::vector_type;
    using box_type = typename OrthoTree::box_type;
    using max_element_type = typename OrthoTree::max_element_type;
    using geometry_type = typename OrthoTree::geometry_type;

  protected:
    OrthoTree m_tree;
    vector<data_type> m_vData;

  public: // Constructors
    OrthoTreeContainerBase() noexcept = default;
    OrthoTreeContainerBase(span<data_type const> const& vData, depth_type nDepthMax = 0, std::optional<box_type> const& oBoxSpace = std::nullopt, max_element_type nElementMaxInNode = OrthoTree::max_element_default, bool fParallelCreate = false) noexcept
      : m_vData(vData.begin(), vData.end())
    {
      if (fParallelCreate)
        OrthoTree::template Create<std::execution::parallel_unsequenced_policy>(m_tree, vData, nDepthMax, oBoxSpace, nElementMaxInNode);
      else
        OrthoTree::Create(m_tree, vData, nDepthMax, oBoxSpace, nElementMaxInNode);
    }

    OrthoTreeContainerBase(vector<data_type>&& vData, depth_type nDepthMax = 0, std::optional<box_type> const& oBoxSpace = std::nullopt, max_element_type nElementMaxInNode = OrthoTree::max_element_default, bool fParallelCreate = false) noexcept
      : m_vData(vData)
    {
      if (fParallelCreate)
        OrthoTree::template Create<std::execution::parallel_unsequenced_policy>(m_tree, m_vData, nDepthMax, oBoxSpace, nElementMaxInNode);
      else
        OrthoTree::Create(m_tree, m_vData, nDepthMax, oBoxSpace, nElementMaxInNode);
    }


  public: // Member functions

    constexpr OrthoTree const& GetCore() const noexcept { return m_tree; }
    constexpr vector<data_type> const& GetData() const noexcept { return m_vData; }

    constexpr void Init(box_type const& boxSpace, depth_type nDepthMax, max_element_type nElementMaxInNode = OrthoTree::max_element_default) noexcept
    {
      m_tree.Init(boxSpace, nDepthMax, nElementMaxInNode);
    }

    bool Add(data_type const& data, bool fInsertToLeaf = false) noexcept
    {
      autoc id = m_vData.size();
      if (m_tree.Insert(id, data, fInsertToLeaf))
      {
        m_vData.emplace_back(data);
        return true;
      }

      return false;
    }

    bool Update(entity_id_type id, data_type const& dataNew, bool fInsertToLeaf = false) noexcept
    {
      if (m_tree.Update(id, m_vData[id], dataNew, fInsertToLeaf))
      {
        m_vData[id] = dataNew;
        return true;
      }

      return false;
    }

    bool Erase(entity_id_type id) noexcept
    {
      if (m_vData.size() <= id)
        return false;

      if (m_tree.Erase(id, m_vData[id]))
      {
        m_vData.erase(std::next(std::begin(m_vData), id));
        return true;
      }

      return false;
    }

    inline void Clear() noexcept { m_tree.Clear(); m_vData.clear(); }
    inline void Reset() noexcept { m_tree.Reset(); m_vData.clear(); }
  };


  // General OrthoTree container for point types
  template<typename OrthoTree, typename data_type>
  class OrthoTreeContainerPoint : public OrthoTreeContainerBase<OrthoTree, data_type>
  {
  public:
    using base = OrthoTreeContainerBase<OrthoTree, data_type>;
    using AD = typename base::AD;
    using vector_type = typename base::vector_type;
    using box_type = typename base::box_type;
    using max_element_type = typename OrthoTree::max_element_type;

    using base::base; // inherits all constructors

  public: // Edit functions

    template<typename execution_policy_type = std::execution::unsequenced_policy>
    static OrthoTreeContainerPoint Create(span<data_type const> const& vData, depth_type nDepthMax = 0, std::optional<box_type> const& oBoxSpace = std::nullopt, max_element_type nElementMaxInNode = OrthoTree::max_element_default) noexcept
    {
      auto otc = OrthoTreeContainerPoint();
      otc.m_vData = vector(vData.begin(), vData.end());
      OrthoTree::template Create<execution_policy_type>(otc.m_tree, vData, nDepthMax, oBoxSpace, nElementMaxInNode);
      return otc;
    }

    template<typename execution_policy_type = std::execution::unsequenced_policy>
    static OrthoTreeContainerPoint Create(vector<data_type>&& vData, depth_type nDepthMax = 0, std::optional<box_type> const& oBoxSpace = std::nullopt, max_element_type nElementMaxInNode = OrthoTree::max_element_default) noexcept
    {
      auto otc = OrthoTreeContainerPoint();
      otc.m_vData = std::move(vData);
      OrthoTree::template Create<execution_policy_type>(otc.m_tree, otc.m_vData, nDepthMax, oBoxSpace, nElementMaxInNode);
      return otc;
    }

    template<typename execution_policy_type = std::execution::unsequenced_policy>
    void Move(vector_type const& vMove) noexcept
    {
      this->m_tree.template Move<execution_policy_type>(vMove);
      auto ep = execution_policy_type{}; // GCC 11.3
      std::for_each(ep, std::begin(this->m_vData), std::end(this->m_vData), [&vMove](auto& pt) { pt = AD::add(pt, vMove); });
    }


  public:

    // Range search
    template<bool fLeafNodeContainsElementOnly = false>
    inline vector<entity_id_type> RangeSearch(box_type const& range) const noexcept
    {
      return this->m_tree.RangeSearch(range, this->m_vData);
    }

    // K Nearest Neighbor
    inline vector<entity_id_type> GetNearestNeighbors(vector_type const& pt, size_t k) const noexcept
    {
      return this->m_tree.GetNearestNeighbors(pt, k, this->m_vData);
    }
  };


  // General OrthoTree container for box types
  template<typename OrthoTree, typename data_type>
  class OrthoTreeContainerBox : public OrthoTreeContainerBase<OrthoTree, data_type>
  {
  public:
    using base = OrthoTreeContainerBase<OrthoTree, data_type>;
    using AD = typename base::AD;
    using geometry_type = typename base::geometry_type;
    using vector_type = typename base::vector_type;
    using box_type = typename base::box_type;
    using max_element_type = typename OrthoTree::max_element_type;

    using base::base; // inherits all constructors

  public: // Edit functions

    template<typename execution_policy_type = std::execution::unsequenced_policy>
    static OrthoTreeContainerBox Create(span<data_type const> const& vData, depth_type nDepthMax = 0, std::optional<box_type> const& oBoxSpace = std::nullopt, max_element_type nElementMaxInNode = OrthoTree::max_element_default) noexcept
    {
      auto otc = OrthoTreeContainerBox();
      otc.m_vData = vector(vData.begin(), vData.end());
      OrthoTree::template Create<execution_policy_type>(otc.m_tree, vData, nDepthMax, oBoxSpace, nElementMaxInNode);
      return otc;
    }

    template<typename execution_policy_type = std::execution::unsequenced_policy>
    static OrthoTreeContainerBox Create(vector<data_type>&& vData, depth_type nDepthMax = 0, std::optional<box_type> const& oBoxSpace = std::nullopt, max_element_type nElementMaxInNode = OrthoTree::max_element_default) noexcept
    {
      auto otc = OrthoTreeContainerBox();
      otc.m_vData = std::move(vData);
      OrthoTree::template Create<execution_policy_type>(otc.m_tree, otc.m_vData, nDepthMax, oBoxSpace, nElementMaxInNode);
      return otc;
    }

    template<typename execution_policy_type = std::execution::unsequenced_policy>
    void Move(vector_type const& vMove) noexcept
    {
      this->m_tree.template Move<execution_policy_type>(vMove);
      auto ep = execution_policy_type{}; // GCC 11.3
      std::for_each(ep, std::begin(this->m_vData), std::end(this->m_vData), [&vMove](auto& box)
      {
        AD::move_box(box, vMove);
      });
    }


  public: // Search functions

    // Pick search
    inline vector<entity_id_type> PickSearch(vector_type const& ptPick) const noexcept
    {
      return this->m_tree.PickSearch(ptPick, this->m_vData);
    }

    // Range search
    template<bool isFullyContained = true>
    inline vector<entity_id_type> RangeSearch(box_type const& range) const noexcept
    {
      return this->m_tree.template RangeSearch<isFullyContained>(range, this->m_vData);
    }


  public: // Collision detection

    // Collision detection between the contained elements
    template<typename execution_policy_type = std::execution::unsequenced_policy>
    inline vector<std::pair<entity_id_type, entity_id_type>> CollisionDetection() const noexcept
    {
      return this->m_tree.template CollisionDetection<execution_policy_type>(this->m_vData);
    }

    // Collision detection with another tree
    inline vector<std::pair<entity_id_type, entity_id_type>> CollisionDetection(OrthoTreeContainerBox const& treeOther) const noexcept
    {
      return this->m_tree.CollisionDetection(this->m_tree, this->m_vData, treeOther.m_tree, treeOther.m_vData);
    }

    // Collision detection between trees
    static inline vector<std::pair<entity_id_type, entity_id_type>> CollisionDetection(OrthoTreeContainerBox const& treeL, OrthoTreeContainerBox const& treeR) noexcept
    {
      return treeL.CollisionDetection(treeR);
    }


  public: // Ray intersection

    // Get all box which is intersected by the ray in order
    inline vector<entity_id_type> RayIntersectedAll(vector_type const& rayBasePoint, vector_type const& rayHeading, geometry_type rMaxDistance = std::numeric_limits<geometry_type>::max()) const noexcept
    {
      return this->m_tree.RayIntersectedAll(rayBasePoint, rayHeading, this->m_vData, rMaxDistance);
    }

    // Get first box which is intersected by the ray
    inline std::optional<entity_id_type> RayIntersectedFirst(vector_type const& rayBasePoint, vector_type const& rayHeading) const noexcept
    {
      return this->m_tree.RayIntersectedFirst(rayBasePoint, rayHeading, this->m_vData);
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
