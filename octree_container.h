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
    std::vector<data_type> m_geometryCollection;

  public: // Constructors
    OrthoTreeContainerBase() noexcept = default;
    OrthoTreeContainerBase(
      std::span<data_type const> const& geometryCollection,
      depth_type maxDepthNo = 0,
      std::optional<box_type> const& boxSpace = std::nullopt,
      max_element_type maxElementNoInNode = OrthoTree::DEFAULT_MAX_ELEMENT,
      bool isParallelCreation = false) noexcept
    : m_geometryCollection(geometryCollection.begin(), geometryCollection.end())
    {
      if (isParallelCreation)
        OrthoTree::template Create<std::execution::parallel_unsequenced_policy>(m_tree, m_geometryCollection, maxDepthNo, boxSpace, maxElementNoInNode);
      else
        OrthoTree::Create(m_tree, m_geometryCollection, maxDepthNo, boxSpace, maxElementNoInNode);
    }

    OrthoTreeContainerBase(
      std::vector<data_type>&& geometryCollection,
      depth_type maxDepthNo = 0,
      std::optional<box_type> const& boxSpace = std::nullopt,
      max_element_type maxElementNoInNode = OrthoTree::DEFAULT_MAX_ELEMENT,
      bool isParallelCreation = false) noexcept
    : m_geometryCollection(geometryCollection)
    {
      if (isParallelCreation)
        OrthoTree::template Create<std::execution::parallel_unsequenced_policy>(m_tree, geometryCollection, maxDepthNo, boxSpace, maxElementNoInNode);
      else
        OrthoTree::Create(m_tree, geometryCollection, maxDepthNo, boxSpace, maxElementNoInNode);
    }


  public: // Member functions
    constexpr OrthoTree const& GetCore() const noexcept { return m_tree; }
    constexpr std::vector<data_type> const& GetData() const noexcept { return m_geometryCollection; }

    constexpr void Init(box_type const& boxSpace, depth_type maxDepthNo, max_element_type maxElementNoInNode = OrthoTree::DEFAULT_MAX_ELEMENT) noexcept
    {
      m_tree.Init(boxSpace, maxDepthNo, maxElementNoInNode);
    }

    bool Add(data_type const& data, bool doInsertToLeaf = false) noexcept
    {
      autoc id = m_geometryCollection.size();
      if (m_tree.Insert(id, data, doInsertToLeaf))
      {
        m_geometryCollection.emplace_back(data);
        return true;
      }

      return false;
    }

    bool Update(entity_id_type id, data_type const& newData, bool doInsertToLeaf = false) noexcept
    {
      if (m_tree.Update(id, m_geometryCollection[id], newData, doInsertToLeaf))
      {
        m_geometryCollection[id] = newData;
        return true;
      }

      return false;
    }

    bool Erase(entity_id_type id) noexcept
    {
      if (m_geometryCollection.size() <= id)
        return false;

      if (m_tree.Erase(id, m_geometryCollection[id]))
      {
        m_geometryCollection.erase(std::next(std::begin(m_geometryCollection), id));
        return true;
      }

      return false;
    }

    inline void Clear() noexcept
    {
      m_tree.Clear();
      m_geometryCollection.clear();
    }
    inline void Reset() noexcept
    {
      m_tree.Reset();
      m_geometryCollection.clear();
    }
  };


  // General OrthoTree container for point types
  template<typename OrthoTree, typename data_type>
  class OrthoTreeContainerPoint final : public OrthoTreeContainerBase<OrthoTree, data_type>
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
    static OrthoTreeContainerPoint Create(
      std::span<data_type const> const& geometryCollection,
      depth_type maxDepthNo = 0,
      std::optional<box_type> const& boxSpace = std::nullopt,
      max_element_type maxElementNoInNode = OrthoTree::DEFAULT_MAX_ELEMENT) noexcept
    {
      auto otc = OrthoTreeContainerPoint();
      otc.m_geometryCollection = std::vector(geometryCollection.begin(), geometryCollection.end());
      OrthoTree::template Create<execution_policy_type>(otc.m_tree, geometryCollection, maxDepthNo, boxSpace, maxElementNoInNode);
      return otc;
    }

    template<typename execution_policy_type = std::execution::unsequenced_policy>
    static OrthoTreeContainerPoint Create(
      std::vector<data_type>&& geometryCollection,
      depth_type maxDepthNo = 0,
      std::optional<box_type> const& boxSpace = std::nullopt,
      max_element_type maxElementNoInNode = OrthoTree::DEFAULT_MAX_ELEMENT) noexcept
    {
      auto otc = OrthoTreeContainerPoint();
      otc.m_geometryCollection = std::move(geometryCollection);
      OrthoTree::template Create<execution_policy_type>(otc.m_tree, geometryCollection, maxDepthNo, boxSpace, maxElementNoInNode);
      return otc;
    }

    template<typename execution_policy_type = std::execution::unsequenced_policy>
    void Move(vector_type const& vMove) noexcept
    {
      this->m_tree.template Move<execution_policy_type>(vMove);
      auto ep = execution_policy_type{}; // GCC 11.3
      std::for_each(ep, std::begin(this->m_geometryCollection), std::end(this->m_geometryCollection), [&vMove](auto& pt) { pt = AD::add(pt, vMove); });
    }


  public:
    // Range search
    template<bool t_doesLeafNodeContainElementOnly = false>
    inline std::vector<entity_id_type> RangeSearch(box_type const& range) const noexcept
    {
      return this->m_tree.RangeSearch(range, this->m_geometryCollection);
    }

    // Plane search (Plane equation: dotProduct(planeNormal, pt) = distanceOfOrigo)
    inline std::vector<entity_id_type> PlaneSearch(geometry_type distanceOfOrigo, vector_type const& planeNormal, geometry_type tolerance) const noexcept
    {
      return this->m_tree.PlaneSearch(distanceOfOrigo, planeNormal, tolerance, this->m_geometryCollection);
    }

    // K Nearest Neighbor
    inline std::vector<entity_id_type> GetNearestNeighbors(vector_type const& pt, size_t k) const noexcept
    {
      return this->m_tree.GetNearestNeighbors(pt, k, this->m_geometryCollection);
    }
  };


  // General OrthoTree container for Box types
  template<typename OrthoTree, typename data_type>
  class OrthoTreeContainerBox final : public OrthoTreeContainerBase<OrthoTree, data_type>
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
    static OrthoTreeContainerBox Create(
      std::span<data_type const> const& geometryCollection,
      depth_type maxDepthNo = 0,
      std::optional<box_type> const& boxSpace = std::nullopt,
      max_element_type maxElementNoInNode = OrthoTree::DEFAULT_MAX_ELEMENT) noexcept
    {
      auto otc = OrthoTreeContainerBox();
      otc.m_geometryCollection = std::vector(geometryCollection.begin(), geometryCollection.end());
      OrthoTree::template Create<execution_policy_type>(otc.m_tree, otc.m_geometryCollection, maxDepthNo, boxSpace, maxElementNoInNode);
      return otc;
    }

    template<typename execution_policy_type = std::execution::unsequenced_policy>
    static OrthoTreeContainerBox Create(
      std::vector<data_type>&& geometryCollection,
      depth_type maxDepthNo = 0,
      std::optional<box_type> const& boxSpace = std::nullopt,
      max_element_type maxElementNoInNode = OrthoTree::DEFAULT_MAX_ELEMENT) noexcept
    {
      auto otc = OrthoTreeContainerBox();
      otc.m_geometryCollection = std::move(geometryCollection);
      OrthoTree::template Create<execution_policy_type>(otc.m_tree, otc.m_geometryCollection, maxDepthNo, boxSpace, maxElementNoInNode);
      return otc;
    }

    template<typename execution_policy_type = std::execution::unsequenced_policy>
    void Move(vector_type const& moveVector) noexcept
    {
      this->m_tree.template Move<execution_policy_type>(moveVector);
      auto ep = execution_policy_type{}; // GCC 11.3
      std::for_each(ep, std::begin(this->m_geometryCollection), std::end(this->m_geometryCollection), [&moveVector](auto& Box) {
        AD::move_box(Box, moveVector);
      });
    }


  public: // Search functions
    // Pick search
    inline std::vector<entity_id_type> PickSearch(vector_type const& pickPoint) const noexcept
    {
      return this->m_tree.PickSearch(pickPoint, this->m_geometryCollection);
    }

    // Range search
    template<bool isFullyContained = true>
    inline std::vector<entity_id_type> RangeSearch(box_type const& range) const noexcept
    {
      return this->m_tree.template RangeSearch<isFullyContained>(range, this->m_geometryCollection);
    }

    // Plane intersection (Plane equation: dotProduct(planeNormal, pt) = distanceOfOrigo)
    inline std::vector<entity_id_type> PlaneIntersection(geometry_type distanceOfOrigo, vector_type const& planeNormal, geometry_type tolerance) const noexcept
    {
      return this->m_tree.PlaneIntersection(distanceOfOrigo, planeNormal, tolerance, this->m_geometryCollection);
    }

  public: // Collision detection
    // Collision detection between the contained elements
    template<typename execution_policy_type = std::execution::unsequenced_policy>
    inline std::vector<std::pair<entity_id_type, entity_id_type>> CollisionDetection() const noexcept
    {
      return this->m_tree.template CollisionDetection<execution_policy_type>(this->m_geometryCollection);
    }

    // Collision detection with another tree
    inline std::vector<std::pair<entity_id_type, entity_id_type>> CollisionDetection(OrthoTreeContainerBox const& otherTree) const noexcept
    {
      return this->m_tree.CollisionDetection(this->m_tree, this->m_geometryCollection, otherTree.m_tree, otherTree.m_geometryCollection);
    }

    // Collision detection between trees
    static inline std::vector<std::pair<entity_id_type, entity_id_type>> CollisionDetection(
      OrthoTreeContainerBox const& leftTree, OrthoTreeContainerBox const& rightTree) noexcept
    {
      return leftTree.CollisionDetection(rightTree);
    }


  public: // Ray intersection
    // Get all Box which is intersected by the ray in order
    inline std::vector<entity_id_type> RayIntersectedAll(
      vector_type const& rayBasePoint,
      vector_type const& rayHeading,
      geometry_type tolerance,
      geometry_type maxDistance = std::numeric_limits<geometry_type>::max()) const noexcept
    {
      return this->m_tree.RayIntersectedAll(rayBasePoint, rayHeading, this->m_geometryCollection, tolerance, maxDistance);
    }

    // Get first Box which is intersected by the ray
    inline std::optional<entity_id_type> RayIntersectedFirst(vector_type const& rayBasePoint, vector_type const& rayHeading, geometry_type tolerance) const noexcept
    {
      return this->m_tree.RayIntersectedFirst(rayBasePoint, rayHeading, this->m_geometryCollection, tolerance);
    }
  };

  template<dim_type N>
  using TreePointContainerND = OrthoTreeContainerPoint<TreePointND<N>, PointND<N>>;

  template<dim_type N, uint32_t additionalDepthOfSplitStrategy = 2>
  using TreeBoxContainerND = OrthoTreeContainerBox<TreeBoxND<N, additionalDepthOfSplitStrategy>, BoundingBoxND<N>>;

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

} // namespace OrthoTree
#endif // ORTHOTREE_CONTAINER_GUARD
