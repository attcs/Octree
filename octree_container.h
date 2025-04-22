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
  template<typename OrthoTreeCore>
  class OrthoTreeContainerBase
  {
  public:
    using AD = typename OrthoTreeCore::AD;

    using TGeometry = typename OrthoTreeCore::TGeometry;
    using TVector = typename OrthoTreeCore::TVector;
    using TBox = typename OrthoTreeCore::TBox;
    using TRay = typename OrthoTreeCore::TRay;
    using TPlane = typename OrthoTreeCore::TPlane;
    using TEntityID = typename OrthoTreeCore::TEntityID;
    using TEntity = typename OrthoTreeCore::TEntity;
    using TContainer =
      typename std::conditional_t<std::is_same_v<typename OrthoTreeCore::TContainer, std::span<TEntity const>>, std::vector<TEntity>, typename OrthoTreeCore::TContainer>;

  protected:
    OrthoTreeCore m_tree;
    TContainer m_geometryCollection;

  public: // Constructors
    OrthoTreeContainerBase() noexcept = default;

    // Constructor for any contiguous container with runtime parallel parameter
    explicit OrthoTreeContainerBase(
      std::span<TEntity const> const& geometryCollection,
      std::optional<depth_t> maxDepthID = std::nullopt,
      std::optional<TBox> boxSpace = std::nullopt,
      std::size_t maxElementNoInNode = OrthoTreeCore::DEFAULT_MAX_ELEMENT,
      bool isParallelCreation = false) noexcept
      requires(OrthoTreeCore::IS_CONTIGOUS_CONTAINER)
    : m_geometryCollection(geometryCollection.begin(), geometryCollection.end())
    {
#ifndef __cpp_lib_execution
      assert(!isParallelCreation); // Parallel creation is based on execution policies. __cpp_lib_execution is required.
#endif
      if (isParallelCreation)
        OrthoTreeCore::template Create<true>(m_tree, m_geometryCollection, maxDepthID, std::move(boxSpace), maxElementNoInNode);
      else
        OrthoTreeCore::Create(m_tree, m_geometryCollection, maxDepthID, boxSpace, maxElementNoInNode);
    }

    // Constructor for any copyable container with runtime parallel parameter
    explicit OrthoTreeContainerBase(
      TContainer const& geometryCollection,
      std::optional<depth_t> maxDepthID = std::nullopt,
      std::optional<TBox> boxSpace = std::nullopt,
      std::size_t maxElementNoInNode = OrthoTreeCore::DEFAULT_MAX_ELEMENT,
      bool isParallelCreation = false) noexcept
    : m_geometryCollection(geometryCollection)
    {
#ifndef __cpp_lib_execution
      assert(!isParallelCreation); // Parallel creation is based on execution policies. __cpp_lib_execution is required.
#endif
      if (isParallelCreation)
        OrthoTreeCore::template Create<true>(m_tree, m_geometryCollection, maxDepthID, std::move(boxSpace), maxElementNoInNode);
      else
        OrthoTreeCore::Create(m_tree, m_geometryCollection, maxDepthID, boxSpace, maxElementNoInNode);
    }

    // Constructor for any movable container with runtime parallel parameter
    explicit OrthoTreeContainerBase(
      TContainer&& geometryCollection,
      std::optional<depth_t> maxDepthID = std::nullopt,
      std::optional<TBox> boxSpace = std::nullopt,
      std::size_t maxElementNoInNode = OrthoTreeCore::DEFAULT_MAX_ELEMENT,
      bool isParallelCreation = false) noexcept
    : m_geometryCollection(std::move(geometryCollection))
    {
#ifndef __cpp_lib_execution
      assert(!isParallelCreation); // Parallel creation is based on execution policies. __cpp_lib_execution is required.
#endif
      if (isParallelCreation)
        OrthoTreeCore::template Create<true>(m_tree, m_geometryCollection, maxDepthID, std::move(boxSpace), maxElementNoInNode);
      else
        OrthoTreeCore::Create(m_tree, m_geometryCollection, maxDepthID, boxSpace, maxElementNoInNode);
    }

    // Constructor for any contiguous container with compile-time parallel parameter
    template<typename EXEC_TAG>
    explicit OrthoTreeContainerBase(
      EXEC_TAG,
      std::span<TEntity const> const& geometryCollection,
      std::optional<depth_t> maxDepthID = std::nullopt,
      std::optional<TBox> boxSpace = std::nullopt,
      std::size_t maxElementNoInNode = OrthoTreeCore::DEFAULT_MAX_ELEMENT) noexcept
      requires(OrthoTreeCore::IS_CONTIGOUS_CONTAINER)
    : m_geometryCollection(geometryCollection.begin(), geometryCollection.end())
    {
#ifdef __cpp_lib_execution
#else
      static_assert(!std::is_same_v<EXEC_TAG, ExecutionTags::Parallel>, "Parallel creation is based on execution policies. __cpp_lib_execution is required.");
#endif
      OrthoTreeCore::template Create<std::is_same_v<EXEC_TAG, ExecutionTags::Parallel>>(
        m_tree, m_geometryCollection, maxDepthID, std::move(boxSpace), maxElementNoInNode);
    }

    // Constructor for any copyable container compile-time parallel parameter
    template<typename EXEC_TAG>
    explicit OrthoTreeContainerBase(
      EXEC_TAG,
      TContainer const& geometryCollection,
      std::optional<depth_t> maxDepthID = std::nullopt,
      std::optional<TBox> boxSpace = std::nullopt,
      std::size_t maxElementNoInNode = OrthoTreeCore::DEFAULT_MAX_ELEMENT) noexcept
    : m_geometryCollection(geometryCollection)
    {
#ifdef __cpp_lib_execution
#else
      static_assert(!std::is_same_v<EXEC_TAG, ExecutionTags::Parallel>, "Parallel creation is based on execution policies. __cpp_lib_execution is required.");
#endif
      OrthoTreeCore::template Create<std::is_same_v<EXEC_TAG, ExecutionTags::Parallel>>(
        m_tree, m_geometryCollection, maxDepthID, std::move(boxSpace), maxElementNoInNode);
    }

    // Constructor for any movable container with compile-time parallel parameter
    template<typename EXEC_TAG>
    explicit OrthoTreeContainerBase(
      EXEC_TAG,
      TContainer&& geometryCollection,
      std::optional<depth_t> maxDepthID = std::nullopt,
      std::optional<TBox> boxSpace = std::nullopt,
      std::size_t maxElementNoInNode = OrthoTreeCore::DEFAULT_MAX_ELEMENT) noexcept
    : m_geometryCollection(std::move(geometryCollection))
    {
#ifdef __cpp_lib_execution
#else
      static_assert(!std::is_same_v<EXEC_TAG, ExecutionTags::Parallel>, "Parallel creation is based on execution policies. __cpp_lib_execution is required.");
#endif
      OrthoTreeCore::template Create<std::is_same_v<EXEC_TAG, ExecutionTags::Parallel>>(
        m_tree, m_geometryCollection, maxDepthID, std::move(boxSpace), maxElementNoInNode);
    }


  public: // Member functions
    constexpr OrthoTreeCore const& GetCore() const noexcept { return m_tree; }
    constexpr TContainer const& GetData() const noexcept { return m_geometryCollection; }

    constexpr void Init(TBox const& boxSpace, depth_t maxDepthID, std::size_t maxElementNoInNode = OrthoTreeCore::DEFAULT_MAX_ELEMENT) noexcept
    {
      m_tree.Init(boxSpace, maxDepthID, maxElementNoInNode);
    }

    // Get entity by ID
    TEntity const& Get(TEntityID entityID) const noexcept { return detail::at(m_geometryCollection, entityID); }

    // Add entity without tree rebalancing
    bool Add(TEntity const& newEntity, bool doInsertToLeaf = false) noexcept
      requires(OrthoTreeCore::IS_CONTIGOUS_CONTAINER)
    {
      auto const newEntityID = TEntityID(m_geometryCollection.size());
      if (!m_tree.Insert(newEntityID, newEntity, doInsertToLeaf))
        return false;

      m_geometryCollection.emplace_back(newEntity);
      return true;
    }

    // Add entity without tree rebalancing
    template<bool CHECK_ID_FOR_CONTAINMENT = false>
    bool Add(TEntityID newEntityID, TEntity const& newEntity, bool doInsertToLeaf = false) noexcept
      requires(!OrthoTreeCore::IS_CONTIGOUS_CONTAINER)
    {
      if constexpr (CHECK_ID_FOR_CONTAINMENT)
      {
        if (m_geometryCollection.contains(newEntityID))
          return false;
      }

      if (!m_tree.Insert(newEntityID, newEntity, doInsertToLeaf))
        return false;

      m_geometryCollection.emplace(newEntityID, newEntity);
      return true;
    }

    // Add entity with tree rebalancing
    bool AddAndRebalance(TEntity const& newEntity) noexcept
      requires(OrthoTreeCore::IS_CONTIGOUS_CONTAINER)
    {
      auto const newEntityID = TEntityID(m_geometryCollection.size());
      if (!m_tree.InsertWithRebalance(newEntityID, newEntity))
        return false;

      m_geometryCollection.emplace_back(newEntity);
      return true;
    }

    // Add entity with tree rebalancing
    template<bool CHECK_ID_FOR_CONTAINMENT = false>
    bool AddAndRebalance(TEntityID newEntityID, TEntity const& newEntity) noexcept
      requires(!OrthoTreeCore::IS_CONTIGOUS_CONTAINER)
    {
      if constexpr (CHECK_ID_FOR_CONTAINMENT)
      {
        if (m_geometryCollection.contains(newEntityID))
          return false;
      }

      if (!m_tree.InsertWithRebalance(newEntityID, newEntity))
        return false;

      m_geometryCollection.emplace(newEntityID, newEntity);
      return true;
    }

    // Update with tree rebalancing
    bool Update(TEntityID entityID, TEntity const& newEntity) noexcept
    {
      auto const oldEntity = detail::at(m_geometryCollection, entityID);
      detail::set(m_geometryCollection, entityID, newEntity);
      if (!m_tree.Update(entityID, oldEntity, newEntity, m_geometryCollection))
      {
        // restore the original state
        detail::set(m_geometryCollection, entityID, oldEntity);
        return false;
      }

      return true;
    }

    // Update without tree rebalancing
    bool Update(TEntityID entityID, TEntity const& newEntity, bool doInsertToLeaf) noexcept
    {
      auto const oldEntity = detail::at(m_geometryCollection, entityID);
      detail::set(m_geometryCollection, entityID, newEntity);
      if (!m_tree.Update(entityID, oldEntity, newEntity, doInsertToLeaf))
      {
        // restore the original state
        detail::set(m_geometryCollection, entityID, oldEntity);
        return false;
      }

      return true;
    }

    // Erase entity by ID
    bool Erase(TEntityID entityID) noexcept
    {
      if (OrthoTreeCore::IS_CONTIGOUS_CONTAINER)
      {
        if (TEntityID(m_geometryCollection.size()) <= entityID)
          return false;
      }

      if (!m_tree.Erase(entityID, detail::at(m_geometryCollection, entityID)))
        return false;

      if constexpr (OrthoTreeCore::IS_CONTIGOUS_CONTAINER)
        m_geometryCollection.erase(std::next(m_geometryCollection.begin(), entityID));
      else
        m_geometryCollection.erase(entityID);

      return true;
    }

    // Clear the tree: remove all node and entity.
    inline void Clear() noexcept
    {
      m_tree.Clear();
      m_geometryCollection.clear();
    }

    // Reset the tree: Same as clear but also reset the handled domain
    inline void Reset() noexcept
    {
      m_tree.Reset();
      m_geometryCollection.clear();
    }

    // Collect all entity ID in breadth-first traverse order
    inline std::vector<TEntityID> CollectAllEntitiesInBFS(OrthoTreeCore::MortonNodeIDCR parentKey = OrthoTreeCore::SI::GetRootKey()) const noexcept
    {
      return m_tree.CollectAllEntitiesInBFS(parentKey);
    }

    // Collect all entity ID in depth-first traverse order
    inline std::vector<TEntityID> CollectAllEntitiesInDFS(OrthoTreeCore::MortonNodeIDCR parentKey = OrthoTreeCore::SI::GetRootKey()) const noexcept
    {
      return m_tree.CollectAllEntitiesInDFS(parentKey);
    }

    // Hyperplane segmentation, get all elements in positive side (Plane equation: dotProduct(planeNormal, point) = distanceOfOrigo)
    inline std::vector<TEntityID> PlanePositiveSegmentation(TGeometry distanceOfOrigo, TVector const& planeNormal, TGeometry tolerance) const noexcept
    {
      return this->m_tree.PlanePositiveSegmentation(distanceOfOrigo, planeNormal, tolerance, this->m_geometryCollection);
    }

    // Hyperplane segmentation, get all elements in positive side (Plane equation: dotProduct(planeNormal, point) = distanceOfOrigo)
    inline std::vector<TEntityID> PlanePositiveSegmentation(TPlane const& plane, TGeometry tolerance) const noexcept
    {
      return this->m_tree.PlanePositiveSegmentation(plane, tolerance, this->m_geometryCollection);
    }

    // Hyperplane segmentation, get all elements in positive side (Plane equation: dotProduct(planeNormal, point) = distanceOfOrigo)
    inline std::vector<TEntityID> FrustumCulling(std::span<TPlane const> const& boundaryPlanes, TGeometry tolerance) const noexcept
    {
      return this->m_tree.FrustumCulling(boundaryPlanes, tolerance, this->m_geometryCollection);
    }
  };


  // General OrthoTree container for point types
  template<typename OrthoTreeCore>
  class OrthoTreeContainerPoint final : public OrthoTreeContainerBase<OrthoTreeCore>
  {
  public:
    using base = OrthoTreeContainerBase<OrthoTreeCore>;
    using AD = typename base::AD;

    using TGeometry = typename base::TGeometry;
    using TVector = typename base::TVector;
    using TBox = typename base::TBox;
    using TRay = typename base::TRay;
    using TPlane = typename base::TPlane;
    using TEntityID = typename OrthoTreeCore::TEntityID;
    using TEntity = typename OrthoTreeCore::TEntity;
    using TContainer = typename base::TContainer;

    using base::base; // inherits all constructors

  public: // Edit functions
    template<bool IS_PARALLEL_EXEC = false>
    static OrthoTreeContainerPoint Create(
      std::span<TEntity const> const& geometryCollectionSpan,
      depth_t maxDepthID = 0,
      std::optional<TBox> boxSpace = std::nullopt,
      std::size_t maxElementNoInNode = OrthoTreeCore::DEFAULT_MAX_ELEMENT) noexcept
      requires(OrthoTreeCore::IS_CONTIGOUS_CONTAINER)
    {
      auto otc = OrthoTreeContainerPoint();
      otc.m_geometryCollection = std::vector(geometryCollectionSpan.begin(), geometryCollectionSpan.end());
      OrthoTreeCore::template Create<IS_PARALLEL_EXEC>(otc.m_tree, otc.m_geometryCollection, maxDepthID, std::move(boxSpace), maxElementNoInNode);
      return otc;
    }

    template<bool IS_PARALLEL_EXEC = false>
    static OrthoTreeContainerPoint Create(
      TContainer const& geometryCollection,
      depth_t maxDepthID = 0,
      std::optional<TBox> boxSpace = std::nullopt,
      std::size_t maxElementNoInNode = OrthoTreeCore::DEFAULT_MAX_ELEMENT) noexcept
    {
      auto otc = OrthoTreeContainerPoint();
      otc.m_geometryCollection = geometryCollection;
      OrthoTreeCore::template Create<IS_PARALLEL_EXEC>(otc.m_tree, otc.m_geometryCollection, maxDepthID, std::move(boxSpace), maxElementNoInNode);
      return otc;
    }

    template<bool IS_PARALLEL_EXEC = false>
    static OrthoTreeContainerPoint Create(
      TContainer&& geometryCollection,
      depth_t maxDepthID = 0,
      std::optional<TBox> boxSpace = std::nullopt,
      std::size_t maxElementNoInNode = OrthoTreeCore::DEFAULT_MAX_ELEMENT) noexcept
    {
      auto otc = OrthoTreeContainerPoint();
      otc.m_geometryCollection = std::move(geometryCollection);
      OrthoTreeCore::template Create<IS_PARALLEL_EXEC>(otc.m_tree, otc.m_geometryCollection, maxDepthID, std::move(boxSpace), maxElementNoInNode);
      return otc;
    }

    template<bool IS_PARALLEL_EXEC = false>
    void Move(TVector const& moveVector) noexcept
    {
      this->m_tree.template Move<IS_PARALLEL_EXEC>(moveVector);
      EXEC_POL_DEF(ep); // GCC 11.3
      std::for_each(EXEC_POL_ADD(ep) this->m_geometryCollection.begin(), this->m_geometryCollection.end(), [&moveVector](auto& data) {
        detail::setValuePart(data, AD::Add(detail::getValuePart(data), moveVector));
      });
    }

  public:
    bool AddUnique(TEntity const& newEntity, TGeometry tolerance, bool doInsertToLeaf = false) noexcept
      requires(OrthoTreeCore::IS_CONTIGOUS_CONTAINER)
    {
      auto const newEntityID = TEntityID(this->m_geometryCollection.size());
      if (!this->m_tree.InsertUnique(newEntityID, newEntity, tolerance, this->m_geometryCollection, doInsertToLeaf))
        return false;

      this->m_geometryCollection.emplace_back(newEntity);
      return true;
    }

    template<bool CHECK_ID_FOR_CONTAINMENT = false>
    bool AddUnique(TEntityID newEntityID, TEntity const& newEntity, TGeometry tolerance, bool doInsertToLeaf = false) noexcept
      requires(!OrthoTreeCore::IS_CONTIGOUS_CONTAINER)
    {
      if constexpr (CHECK_ID_FOR_CONTAINMENT)
      {
        if (this->m_geometryCollection.contains(newEntityID))
          return false;
      }

      if (!this->m_tree.InsertUnique(newEntityID, newEntity, tolerance, this->m_geometryCollection, doInsertToLeaf))
        return false;

      this->m_geometryCollection.emplace(newEntityID, newEntity);
      return true;
    }

  public:
    // Range search
    template<bool DOES_LEAF_NODE_CONTAIN_ELEMENT_ONLY = false>
    inline std::vector<TEntityID> RangeSearch(TBox const& range) const noexcept
    {
      return this->m_tree.template RangeSearch<DOES_LEAF_NODE_CONTAIN_ELEMENT_ONLY>(range, this->m_geometryCollection);
    }

    // K Nearest Neighbor
    inline std::vector<TEntityID> GetNearestNeighbors(TVector const& pt, std::size_t k) const noexcept
    {
      return this->m_tree.GetNearestNeighbors(pt, k, this->m_geometryCollection);
    }

  public: // Plane
    // Hyperplane intersection (Plane equation: dotProduct(planeNormal, point) = distanceOfOrigo)
    inline std::vector<TEntityID> PlaneSearch(TGeometry distanceOfOrigo, TVector const& planeNormal, TGeometry tolerance) const noexcept
    {
      return this->m_tree.PlaneSearch(distanceOfOrigo, planeNormal, tolerance, this->m_geometryCollection);
    }

    // Hyperplane intersection using built-in plane
    inline std::vector<TEntityID> PlaneSearch(TPlane const& plane, TGeometry tolerance) const noexcept
    {
      return this->m_tree.PlaneSearch(AD::GetPlaneOrigoDistance(plane), AD::GetPlaneNormal(plane), tolerance, this->m_geometryCollection);
    }
  };


  // General OrthoTreeCore container for Box types
  template<typename OrthoTreeCore>
  class OrthoTreeContainerBox final : public OrthoTreeContainerBase<OrthoTreeCore>
  {
  public:
    using base = OrthoTreeContainerBase<OrthoTreeCore>;
    using AD = typename base::AD;

    using TGeometry = typename base::TGeometry;
    using TVector = typename base::TVector;
    using TBox = typename base::TBox;
    using TRay = typename base::TRay;
    using TPlane = typename base::TPlane;
    using TEntityID = typename OrthoTreeCore::TEntityID;
    using TEntity = typename OrthoTreeCore::TEntity;
    using TContainer = typename base::TContainer;

    using base::base; // inherits all constructors

  public: // Edit functions
    template<bool IS_PARALLEL_EXEC = false>
    static OrthoTreeContainerBox Create(
      std::span<TEntity const> const& geometryCollection,
      std::optional<depth_t> maxDepthID = std::nullopt,
      std::optional<TBox> boxSpace = std::nullopt,
      std::size_t maxElementNoInNode = OrthoTreeCore::DEFAULT_MAX_ELEMENT) noexcept
      requires(OrthoTreeCore::IS_CONTIGOUS_CONTAINER)
    {
      auto otc = OrthoTreeContainerBox();
      otc.m_geometryCollection = std::vector(geometryCollection.begin(), geometryCollection.end());
      OrthoTreeCore::template Create<IS_PARALLEL_EXEC>(otc.m_tree, otc.m_geometryCollection, maxDepthID, std::move(boxSpace), maxElementNoInNode);
      return otc;
    }

    template<bool IS_PARALLEL_EXEC = false>
    static OrthoTreeContainerBox Create(
      TContainer const& geometryCollection,
      std::optional<depth_t> maxDepthID = std::nullopt,
      std::optional<TBox> boxSpace = std::nullopt,
      std::size_t maxElementNoInNode = OrthoTreeCore::DEFAULT_MAX_ELEMENT) noexcept
    {
      auto otc = OrthoTreeContainerBox();
      otc.m_geometryCollection = geometryCollection;
      OrthoTreeCore::template Create<IS_PARALLEL_EXEC>(otc.m_tree, otc.m_geometryCollection, maxDepthID, std::move(boxSpace), maxElementNoInNode);
      return otc;
    }

    template<bool IS_PARALLEL_EXEC = false>
    static OrthoTreeContainerBox Create(
      TContainer&& geometryCollection,
      std::optional<depth_t> maxDepthID = std::nullopt,
      std::optional<TBox> boxSpace = std::nullopt,
      std::size_t maxElementNoInNode = OrthoTreeCore::DEFAULT_MAX_ELEMENT) noexcept
    {
      auto otc = OrthoTreeContainerBox();
      otc.m_geometryCollection = std::move(geometryCollection);
      OrthoTreeCore::template Create<IS_PARALLEL_EXEC>(otc.m_tree, otc.m_geometryCollection, maxDepthID, std::move(boxSpace), maxElementNoInNode);
      return otc;
    }

    template<bool IS_PARALLEL_EXEC = false>
    void Move(TVector const& moveVector) noexcept
    {
      this->m_tree.template Move<IS_PARALLEL_EXEC>(moveVector);
      EXEC_POL_DEF(ep); // GCC 11.3
      std::for_each(EXEC_POL_ADD(ep) this->m_geometryCollection.begin(), this->m_geometryCollection.end(), [&moveVector](auto& data) {
        auto box = detail::getValuePart(data);
        AD::MoveBox(box, moveVector);
        detail::setValuePart(data, box);
      });
    }


  public: // Search functions
    // Pick search
    inline std::vector<TEntityID> PickSearch(TVector const& pickPoint) const noexcept
    {
      return this->m_tree.PickSearch(pickPoint, this->m_geometryCollection);
    }

    // Range search
    template<bool isFullyContained = true>
    inline std::vector<TEntityID> RangeSearch(TBox const& range) const noexcept
    {
      return this->m_tree.template RangeSearch<isFullyContained>(range, this->m_geometryCollection);
    }

  public: // Collision detection
    // Collision detection between the contained elements
    template<bool IS_PARALLEL_EXEC = false>
    inline std::vector<std::pair<TEntityID, TEntityID>> CollisionDetection() const noexcept
    {
      return this->m_tree.template CollisionDetection<IS_PARALLEL_EXEC>(this->m_geometryCollection);
    }

    // Collision detection with another tree
    inline std::vector<std::pair<TEntityID, TEntityID>> CollisionDetection(OrthoTreeContainerBox const& otherTree) const noexcept
    {
      return this->m_tree.CollisionDetection(this->m_tree, this->m_geometryCollection, otherTree.m_tree, otherTree.m_geometryCollection);
    }

    // Collision detection between trees
    static inline std::vector<std::pair<TEntityID, TEntityID>> CollisionDetection(
      OrthoTreeContainerBox const& leftTree, OrthoTreeContainerBox const& rightTree) noexcept
    {
      return leftTree.CollisionDetection(rightTree);
    }


  public: // Ray intersection
    // Get all Box which is intersected by the ray in order
    inline std::vector<TEntityID> RayIntersectedAll(
      TVector const& rayBasePoint, TVector const& rayHeading, TGeometry tolerance, TGeometry maxDistance = std::numeric_limits<TGeometry>::max()) const noexcept
    {
      return this->m_tree.RayIntersectedAll(rayBasePoint, rayHeading, this->m_geometryCollection, tolerance, maxDistance);
    }

    // Get first Box which is intersected by the ray
    inline std::optional<TEntityID> RayIntersectedFirst(TVector const& rayBasePoint, TVector const& rayHeading, TGeometry tolerance) const noexcept
    {
      return this->m_tree.RayIntersectedFirst(rayBasePoint, rayHeading, this->m_geometryCollection, tolerance);
    }

  public: // Plane
    // Hyperplane intersection (Plane equation: dotProduct(planeNormal, point) = distanceOfOrigo)
    inline std::vector<TEntityID> PlaneIntersection(TGeometry distanceOfOrigo, TVector const& planeNormal, TGeometry tolerance) const noexcept
    {
      return this->m_tree.PlaneIntersection(distanceOfOrigo, planeNormal, tolerance, this->m_geometryCollection);
    }

    // Hyperplane intersection using built-in plane
    inline std::vector<TEntityID> PlaneIntersection(TPlane const& plane, TGeometry tolerance) const noexcept
    {
      return this->m_tree.PlaneIntersection(plane, tolerance, this->m_geometryCollection);
    }
  };

  template<dim_t DIMENSION_NO, typename TGeometry = BaseGeometryType>
  using VectorSpan = std::span<VectorND<DIMENSION_NO, TGeometry> const>;

  template<dim_t DIMENSION_NO, typename TGeometry = BaseGeometryType>
  using BoxSpan = std::span<BoundingBoxND<DIMENSION_NO, TGeometry> const>;

  template<dim_t DIMENSION_NO, typename TGeometry = BaseGeometryType>
  using VectorMap = std::unordered_map<index_t, VectorND<DIMENSION_NO, TGeometry> const>;

  template<dim_t DIMENSION_NO, typename TGeometry = BaseGeometryType>
  using BoxMap = std::unordered_map<index_t, BoundingBoxND<DIMENSION_NO, TGeometry> const>;


  template<dim_t DIMENSION_NO, typename TGeometry = BaseGeometryType, typename TContainer = VectorSpan<DIMENSION_NO, TGeometry>>
  using TreePointContainerND = OrthoTreeContainerPoint<TreePointND<DIMENSION_NO, TGeometry, TContainer>>;

  template<dim_t DIMENSION_NO, bool DO_SPLIT_PARENT_ENTITIES = true, typename TGeometry = BaseGeometryType, typename TContainer = BoxSpan<DIMENSION_NO, TGeometry>>
  using TreeBoxContainerND = OrthoTreeContainerBox<TreeBoxND<DIMENSION_NO, DO_SPLIT_PARENT_ENTITIES, TGeometry, TContainer>>;

  // Dualtree for points
  using DualtreePointC = TreePointContainerND<1, BaseGeometryType>;

  // Dualtree for bounding boxes
  template<bool DO_SPLIT_PARENT_ENTITIES = true>
  using DualtreeBoxCs = TreeBoxContainerND<1, DO_SPLIT_PARENT_ENTITIES, BaseGeometryType>;
  using DualtreeBoxC = TreeBoxContainerND<1, true, BaseGeometryType>;

  // Quadtree for points
  using QuadtreePointC = TreePointContainerND<2, BaseGeometryType>;

  // Quadtree for bounding boxes
  template<bool DO_SPLIT_PARENT_ENTITIES = true>
  using QuadtreeBoxCs = TreeBoxContainerND<2, DO_SPLIT_PARENT_ENTITIES, BaseGeometryType>;
  using QuadtreeBoxC = TreeBoxContainerND<2, true, BaseGeometryType>;

  // Octree for points
  using OctreePointC = TreePointContainerND<3, BaseGeometryType>;

  // Octree for bounding boxes
  template<bool DO_SPLIT_PARENT_ENTITIES = true>
  using OctreeBoxCs = TreeBoxContainerND<3, DO_SPLIT_PARENT_ENTITIES, BaseGeometryType>;
  using OctreeBoxC = TreeBoxContainerND<3, true, BaseGeometryType>;


  // std::unordered_map-based Dualtree for points
  using DualtreePointMapC = TreePointContainerND<1, BaseGeometryType, VectorMap<1>>;

  // std::unordered_map-based Dualtree for bounding boxes
  template<bool DO_SPLIT_PARENT_ENTITIES = true>
  using DualtreeBoxMapCs = TreeBoxContainerND<1, DO_SPLIT_PARENT_ENTITIES, BaseGeometryType, BoxMap<1>>;
  using DualtreeBoxMapC = TreeBoxContainerND<1, true, BaseGeometryType, BoxMap<1>>;

  // std::unordered_map-based Quadtree for points
  using QuadtreePointMapC = TreePointContainerND<2, BaseGeometryType, VectorMap<2>>;

  // std::unordered_map-based Quadtree for bounding boxes
  template<bool DO_SPLIT_PARENT_ENTITIES = true>
  using QuadtreeBoxMapCs = TreeBoxContainerND<2, DO_SPLIT_PARENT_ENTITIES, BaseGeometryType, BoxMap<2>>;
  using QuadtreeBoxMapC = TreeBoxContainerND<2, true, BaseGeometryType, BoxMap<2>>;

  // std::unordered_map-based Octree for points
  using OctreePointMapC = TreePointContainerND<3, BaseGeometryType, VectorMap<3>>;

  // std::unordered_map-based Octree for bounding boxes
  template<bool DO_SPLIT_PARENT_ENTITIES = true>
  using OctreeBoxMapCs = TreeBoxContainerND<3, DO_SPLIT_PARENT_ENTITIES, BaseGeometryType, BoxMap<3>>;
  using OctreeBoxMapC = TreeBoxContainerND<3, true, BaseGeometryType, BoxMap<3>>;

  // User-defined container-based Quadtree for points
  template<typename TContainer>
  using QuadtreePointUDMapC = TreePointContainerND<2, BaseGeometryType, TContainer>;

  // User-defined container-based Quadtree for bounding boxes
  template<typename TContainer, bool DO_SPLIT_PARENT_ENTITIES = true>
  using QuadtreeBoxUDMapCs = TreeBoxContainerND<2, DO_SPLIT_PARENT_ENTITIES, BaseGeometryType, TContainer>;
  template<typename TContainer>
  using QuadtreeBoxUDMapC = TreeBoxContainerND<2, true, BaseGeometryType, TContainer>;

  // User-defined container-based Octree for points
  template<typename TContainer>
  using OctreePointUDMapC = TreePointContainerND<3, BaseGeometryType, TContainer>;

  // User-defined container-based Octree for bounding boxes
  template<typename TContainer, bool DO_SPLIT_PARENT_ENTITIES = true>
  using OctreeBoxUDMapCs = TreeBoxContainerND<3, DO_SPLIT_PARENT_ENTITIES, BaseGeometryType, TContainer>;
  template<typename TContainer>
  using OctreeBoxUDMapC = TreeBoxContainerND<3, true, BaseGeometryType, TContainer>;
} // namespace OrthoTree
#endif // ORTHOTREE_CONTAINER_GUARD
