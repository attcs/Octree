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
    using TContainer = typename std::conditional_t<std::is_same_v<typename OrthoTreeCore::TContainer, std::span<TEntity const>>, std::vector<TEntity>, typename OrthoTreeCore::TContainer>;

  protected:
    OrthoTreeCore m_tree;
    TContainer m_geometryCollection;

  public: // Constructors
    OrthoTreeContainerBase() noexcept = default;

    OrthoTreeContainerBase(
      std::span<TEntity const> const& geometryCollection,
      std::optional<depth_t> maxDepthNo = std::nullopt,
      std::optional<TBox> const& boxSpace = std::nullopt,
      std::size_t maxElementNoInNode = OrthoTreeCore::DEFAULT_MAX_ELEMENT,
      bool isParallelCreation = false) noexcept
      requires(OrthoTreeCore::IS_CONTIGOUS_CONTAINER)
    : m_geometryCollection(geometryCollection.begin(), geometryCollection.end())
    {
      if (isParallelCreation)
        OrthoTreeCore::template Create<std::execution::parallel_unsequenced_policy>(m_tree, m_geometryCollection, maxDepthNo, boxSpace, maxElementNoInNode);
      else
        OrthoTreeCore::Create(m_tree, m_geometryCollection, maxDepthNo, boxSpace, maxElementNoInNode);
    }
    
    OrthoTreeContainerBase(
      TContainer const& geometryCollection,
      std::optional<depth_t> maxDepthNo = std::nullopt,
      std::optional<TBox> const& boxSpace = std::nullopt,
      std::size_t maxElementNoInNode = OrthoTreeCore::DEFAULT_MAX_ELEMENT,
      bool isParallelCreation = false) noexcept
    : m_geometryCollection(geometryCollection)
    {
      if (isParallelCreation)
        OrthoTreeCore::template Create<std::execution::parallel_unsequenced_policy>(m_tree, m_geometryCollection, maxDepthNo, boxSpace, maxElementNoInNode);
      else
        OrthoTreeCore::Create(m_tree, m_geometryCollection, maxDepthNo, boxSpace, maxElementNoInNode);
    }
    
    OrthoTreeContainerBase(
      TContainer&& geometryCollection,
      std::optional<depth_t> maxDepthNo = std::nullopt,
      std::optional<TBox> const& boxSpace = std::nullopt,
      std::size_t maxElementNoInNode = OrthoTreeCore::DEFAULT_MAX_ELEMENT,
      bool isParallelCreation = false) noexcept
    : m_geometryCollection(std::move(geometryCollection))
    {
      if (isParallelCreation)
        OrthoTreeCore::template Create<std::execution::parallel_unsequenced_policy>(m_tree, geometryCollection, maxDepthNo, boxSpace, maxElementNoInNode);
      else
        OrthoTreeCore::Create(m_tree, geometryCollection, maxDepthNo, boxSpace, maxElementNoInNode);
    }
    

  public: // Member functions
    constexpr OrthoTreeCore const& GetCore() const noexcept { return m_tree; }
    constexpr TContainer const& GetData() const noexcept { return m_geometryCollection; }

    constexpr void Init(TBox const& boxSpace, depth_t maxDepthNo, std::size_t maxElementNoInNode = OrthoTreeCore::DEFAULT_MAX_ELEMENT) noexcept
    {
      m_tree.Init(boxSpace, maxDepthNo, maxElementNoInNode);
    }

    TEntity const& Get(TEntityID entityID) const noexcept { return detail::at(m_geometryCollection, entityID); }

    bool Add(TEntity const& data, bool doInsertToLeaf = false) noexcept
      requires(OrthoTreeCore::IS_CONTIGOUS_CONTAINER)
    {
      autoc entityID = TEntityID(m_geometryCollection.size());
      if (m_tree.Insert(entityID, data, doInsertToLeaf))
      {
        m_geometryCollection.emplace_back(data);
        return true;
      }

      return false;
    }

    template<bool CHECK_ID_FOR_CONTAINMENT = false>
    bool Add(TEntityID entityID, TEntity const& data, bool doInsertToLeaf = false) noexcept
    {
      if constexpr (CHECK_ID_FOR_CONTAINMENT)
      {
        if (m_geometryCollection.contains(entityID))
          return false;
      }

      if (m_tree.Insert(entityID, data, doInsertToLeaf))
      {
        m_geometryCollection.emplace(entityID, data);
        return true;
      }

      return false;
    }

    bool Update(TEntityID entityID, TEntity const& newData, bool doInsertToLeaf = false) noexcept
    {
      if (m_tree.Update(entityID, detail::at(m_geometryCollection, entityID), newData, doInsertToLeaf))
      {
        detail::set(m_geometryCollection, entityID, newData);
        return true;
      }

      return false;
    }

    bool Erase(TEntityID entityID) noexcept 
    {
      if (m_geometryCollection.size() <= entityID)
        return false;

      if (!m_tree.Erase(entityID, detail::at(m_geometryCollection, entityID)))
        return false;

      if constexpr (OrthoTreeCore::IS_CONTIGOUS_CONTAINER)
        m_geometryCollection.erase(std::next(m_geometryCollection.begin(), entityID));
      else
        m_geometryCollection.erase(entityID);

      return true;
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

    inline std::vector<TEntityID> CollectAllIdInBFS(OrthoTreeCore::MortonNodeIDCR parentKey = OrthoTreeCore::GetRootKey()) const noexcept
    {
      return m_tree.CollectAllIdInBFS(parentKey);
    }

    inline std::vector<TEntityID> CollectAllIdInDFS(OrthoTreeCore::MortonNodeIDCR parentKey = OrthoTreeCore::GetRootKey()) const noexcept
    {
      return m_tree.CollectAllIdInDFS(parentKey);
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
    template<typename TExecutionPolicy = std::execution::unsequenced_policy>
    static OrthoTreeContainerPoint Create(
      std::span<TEntity const> const& geometryCollectionSpan,
      depth_t maxDepthNo = 0,
      std::optional<TBox> const& boxSpace = std::nullopt,
      std::size_t maxElementNoInNode = OrthoTreeCore::DEFAULT_MAX_ELEMENT) noexcept
      requires(OrthoTreeCore::IS_CONTIGOUS_CONTAINER)
    {
      auto otc = OrthoTreeContainerPoint();
      otc.m_geometryCollection = std::vector(geometryCollectionSpan.begin(), geometryCollectionSpan.end());
      OrthoTreeCore::template Create<TExecutionPolicy>(otc.m_tree, otc.m_geometryCollection, maxDepthNo, boxSpace, maxElementNoInNode);
      return otc;
    }
    
    template<typename TExecutionPolicy = std::execution::unsequenced_policy>
    static OrthoTreeContainerPoint Create(
      TContainer const& geometryCollection,
      depth_t maxDepthNo = 0,
      std::optional<TBox> const& boxSpace = std::nullopt,
      std::size_t maxElementNoInNode = OrthoTreeCore::DEFAULT_MAX_ELEMENT) noexcept
    {
      auto otc = OrthoTreeContainerPoint();
      otc.m_geometryCollection = geometryCollection;
      OrthoTreeCore::template Create<TExecutionPolicy>(otc.m_tree, geometryCollection, maxDepthNo, boxSpace, maxElementNoInNode);
      return otc;
    }
    
    template<typename TExecutionPolicy = std::execution::unsequenced_policy>
    static OrthoTreeContainerPoint Create(
      TContainer&& geometryCollection,
      depth_t maxDepthNo = 0,
      std::optional<TBox> const& boxSpace = std::nullopt,
      std::size_t maxElementNoInNode = OrthoTreeCore::DEFAULT_MAX_ELEMENT) noexcept
    {
      auto otc = OrthoTreeContainerPoint();
      otc.m_geometryCollection = std::move(geometryCollection);
      OrthoTreeCore::template Create<TExecutionPolicy>(otc.m_tree, geometryCollection, maxDepthNo, boxSpace, maxElementNoInNode);
      return otc;
    }

    template<typename TExecutionPolicy = std::execution::unsequenced_policy>
    void Move(TVector const& vMove) noexcept
    {
      this->m_tree.template Move<TExecutionPolicy>(vMove);
      auto ep = TExecutionPolicy{}; // GCC 11.3
      std::for_each(ep, this->m_geometryCollection.begin(), this->m_geometryCollection.end(), [&vMove](auto& data) { 
        detail::setValuePart(data, AD::Add(detail::getValuePart(data), vMove));
      });
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
    template<typename TExecutionPolicy = std::execution::unsequenced_policy>
    static OrthoTreeContainerBox Create(
      std::span<TEntity const> const& geometryCollection,
      std::optional<depth_t> maxDepthNo = std::nullopt,
      std::optional<TBox> const& boxSpace = std::nullopt,
      std::size_t maxElementNoInNode = OrthoTreeCore::DEFAULT_MAX_ELEMENT) noexcept
      requires(OrthoTreeCore::IS_CONTIGOUS_CONTAINER)
    {
      auto otc = OrthoTreeContainerBox();
      otc.m_geometryCollection = std::vector(geometryCollection.begin(), geometryCollection.end());
      OrthoTreeCore::template Create<TExecutionPolicy>(otc.m_tree, otc.m_geometryCollection, maxDepthNo, boxSpace, maxElementNoInNode);
      return otc;
    }

    template<typename TExecutionPolicy = std::execution::unsequenced_policy>
    static OrthoTreeContainerBox Create(
      TContainer const& geometryCollection,
      std::optional<depth_t> maxDepthNo = std::nullopt,
      std::optional<TBox> const& boxSpace = std::nullopt,
      std::size_t maxElementNoInNode = OrthoTreeCore::DEFAULT_MAX_ELEMENT) noexcept
    {
      auto otc = OrthoTreeContainerBox();
      otc.m_geometryCollection = geometryCollection;
      OrthoTreeCore::template Create<TExecutionPolicy>(otc.m_tree, otc.m_geometryCollection, maxDepthNo, boxSpace, maxElementNoInNode);
      return otc;
    }

    template<typename TExecutionPolicy = std::execution::unsequenced_policy>
    static OrthoTreeContainerBox Create(
      TContainer&& geometryCollection,
      std::optional<depth_t> maxDepthNo = std::nullopt,
      std::optional<TBox> const& boxSpace = std::nullopt,
      std::size_t maxElementNoInNode = OrthoTreeCore::DEFAULT_MAX_ELEMENT) noexcept
    {
      auto otc = OrthoTreeContainerBox();
      otc.m_geometryCollection = std::move(geometryCollection);
      OrthoTreeCore::template Create<TExecutionPolicy>(otc.m_tree, otc.m_geometryCollection, maxDepthNo, boxSpace, maxElementNoInNode);
      return otc;
    }

    template<typename TExecutionPolicy = std::execution::unsequenced_policy>
    void Move(TVector const& moveVector) noexcept
    {
      this->m_tree.template Move<TExecutionPolicy>(moveVector);
      auto ep = TExecutionPolicy{}; // GCC 11.3
      std::for_each(ep, this->m_geometryCollection.begin(), this->m_geometryCollection.end(), [&moveVector](auto& data) {
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
    template<typename TExecutionPolicy = std::execution::unsequenced_policy>
    inline std::vector<std::pair<TEntityID, TEntityID>> CollisionDetection() const noexcept
    {
      return this->m_tree.template CollisionDetection<TExecutionPolicy>(this->m_geometryCollection);
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

  template<dim_t DIMENSION_NO, uint32_t SPLIT_DEPTH_INCREASEMENT = 2, typename TGeometry = BaseGeometryType, typename TContainer = BoxSpan<DIMENSION_NO, TGeometry>>
  using TreeBoxContainerND = OrthoTreeContainerBox<TreeBoxND<DIMENSION_NO, SPLIT_DEPTH_INCREASEMENT, TGeometry, TContainer>>;

  // Dualtree for points
  using DualtreePointC = TreePointContainerND<1, BaseGeometryType>;

  // Dualtree for bounding boxes
  template<uint32_t SPLIT_DEPTH_INCREASEMENT = 2>
  using DualtreeBoxCs = TreeBoxContainerND<1, SPLIT_DEPTH_INCREASEMENT, BaseGeometryType>;
  using DualtreeBoxC = TreeBoxContainerND<1, 2, BaseGeometryType>;

  // Quadtree for points
  using QuadtreePointC = TreePointContainerND<2, BaseGeometryType>;

  // Quadtree for bounding boxes
  template<uint32_t SPLIT_DEPTH_INCREASEMENT = 2>
  using QuadtreeBoxCs = TreeBoxContainerND<2, SPLIT_DEPTH_INCREASEMENT, BaseGeometryType>;
  using QuadtreeBoxC = TreeBoxContainerND<2, 2, BaseGeometryType>;

  // Octree for points
  using OctreePointC = TreePointContainerND<3, BaseGeometryType>;

  // Octree for bounding boxes
  template<uint32_t SPLIT_DEPTH_INCREASEMENT = 2>
  using OctreeBoxCs = TreeBoxContainerND<3, SPLIT_DEPTH_INCREASEMENT, BaseGeometryType>;
  using OctreeBoxC = TreeBoxContainerND<3, 2, BaseGeometryType>;


  // std::unordered_map-based Dualtree for points
  using DualtreePointMapC = TreePointContainerND<1, BaseGeometryType, VectorMap<1>>;

  // std::unordered_map-based Dualtree for bounding boxes
  template<uint32_t SPLIT_DEPTH_INCREASEMENT = 2>
  using DualtreeBoxMapCs = TreeBoxContainerND<1, SPLIT_DEPTH_INCREASEMENT, BaseGeometryType, BoxMap<1>>;
  using DualtreeBoxMapC = TreeBoxContainerND<1, 2, BaseGeometryType, BoxMap<1>>;

  // std::unordered_map-based Quadtree for points
  using QuadtreePointMapC = TreePointContainerND<2, BaseGeometryType, VectorMap<2>>;

  // std::unordered_map-based Quadtree for bounding boxes
  template<uint32_t SPLIT_DEPTH_INCREASEMENT = 2>
  using QuadtreeBoxMapCs = TreeBoxContainerND<2, SPLIT_DEPTH_INCREASEMENT, BaseGeometryType, BoxMap<2>>;
  using QuadtreeBoxMapC = TreeBoxContainerND<2, 2, BaseGeometryType, BoxMap<2>>;

  // std::unordered_map-based Octree for points
  using OctreePointMapC = TreePointContainerND<3, BaseGeometryType, VectorMap<3>>;

  // std::unordered_map-based Octree for bounding boxes
  template<uint32_t SPLIT_DEPTH_INCREASEMENT = 2>
  using OctreeBoxMapCs = TreeBoxContainerND<3, SPLIT_DEPTH_INCREASEMENT, BaseGeometryType, BoxMap<3>>;
  using OctreeBoxMapC = TreeBoxContainerND<3, 2, BaseGeometryType, BoxMap<3>>;

  // User-defined container-based Quadtree for points
  template<typename TContainer>
  using QuadtreePointUDMapC = TreePointContainerND<2, BaseGeometryType, TContainer>;

  // User-defined container-based Quadtree for bounding boxes
  template<typename TContainer, uint32_t SPLIT_DEPTH_INCREASEMENT = 2>
  using QuadtreeBoxUDMapCs = TreeBoxContainerND<2, SPLIT_DEPTH_INCREASEMENT, BaseGeometryType, TContainer>;
  template<typename TContainer>
  using QuadtreeBoxUDMapC = TreeBoxContainerND<2, 2, BaseGeometryType, TContainer>;

  // User-defined container-based Octree for points
  template<typename TContainer>
  using OctreePointUDMapC = TreePointContainerND<3, BaseGeometryType, TContainer>;

  // User-defined container-based Octree for bounding boxes
  template<typename TContainer, uint32_t SPLIT_DEPTH_INCREASEMENT = 2>
  using OctreeBoxUDMapCs = TreeBoxContainerND<3, SPLIT_DEPTH_INCREASEMENT, BaseGeometryType, TContainer>;
  template<typename TContainer>
  using OctreeBoxUDMapC = TreeBoxContainerND<3, 2, BaseGeometryType, TContainer>;
} // namespace OrthoTree
#endif // ORTHOTREE_CONTAINER_GUARD
