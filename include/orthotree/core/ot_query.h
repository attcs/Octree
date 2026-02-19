#pragma once

namespace OrthoTree
{

  // TraverseControl is the result type of node-visitor functions' procedure.
  enum class TraverseControl
  {
    Terminate,    // Terminates the traverse
    SkipChildren, // Skips children nodes
    Continue      // Continues the traverse
  };

  // OrthoTree: Non-owning Base container which spatially organize data ids in N dimension space into a hash-table by Morton Z order.
  template<typename T>
  concept OrthoTreeCoreView = requires(
    const T ct,
    typename T::NodeIDCR nodeID,
    typename T::NodeValue nodeValue,
    typename T::EA::Geometry const& geometry,
    typename T::EA::EntityContainerView entities,
    typename T::EntityID entityID) {
    typename T::NodeID;
    typename T::NodeIDCR;
    typename T::NodeValue;
    typename T::EA;
    typename T::GA;
    typename T::CONFIG;
    typename T::IGM;

    { T::GetRootNodeID() } -> std::convertible_to<typename T::NodeID>;
    { ct.GetRootNodeValue() } -> std::convertible_to<typename T::NodeValue>;
    { ct.GetNodeValue(nodeID) } -> std::convertible_to<typename T::NodeValue>;

    { ct.GetNodeChildren(nodeValue) };
    { ct.GetNodeEntities(nodeValue) };

    { ct.GetNodeBox(nodeValue) };
    { ct.GetNodeMinPoint(nodeValue) };
    { ct.GetNodeSize(nodeValue) };

    { ct.IsNodeEntitiesEmpty(nodeValue) } -> std::convertible_to<bool>;

    { ct.GetNodeCount() } -> std::convertible_to<std::size_t>;
  };

  template<typename TOrthoTreeCore>
  class OrthoTreeQueryBase : public TOrthoTreeCore
  {
  public:
    using Core = TOrthoTreeCore;
    ORTHOTREE_DEPENDENT_TYPES(Core);

  public:
#ifdef ORTHOTREE_PUBLIC_NODE_INTERFACE
    static_assert(OrthoTreeCoreView<TOrthoTreeCore>);
#endif
    static_assert(GeometryAdapterConcept<GA, GA::DIMENSION_NO, TVector, TBox, TRay, TPlane, TScalar, TFloatScalar>);
    static_assert(0 < GA::DIMENSION_NO && GA::DIMENSION_NO < 64);

    static_assert(std::is_trivially_copyable_v<EntityID>, "Only trivially copyable EntityID types are supported!");
    static_assert(CONFIG::IS_HOMOGENEOUS_GEOMETRY, "Mixed geometry types are not supported yet!");
    static_assert(EA::GEOMETRY_TYPE == GeometryType::Point || EA::GEOMETRY_TYPE == GeometryType::Box, "Entity geometry type is not supported!");
    static_assert(CONFIG::MAX_ALLOWED_DEPTH_ID <= MAX_DEPTH_ID, "MAX_ALLOWED_DEPTH_ID of Configuration is too large.");
    static_assert(CONFIG::LOOSE_FACTOR >= 1.0 && CONFIG::LOOSE_FACTOR <= 2.0, "Wrong loose factor for Loose trees.");

    using TOrthoTreeCore::TOrthoTreeCore;

    using NodeID = typename Core::NodeID;
    using NodeIDCR = typename Core::NodeIDCR;
    using NodeValue = Core::NodeValue;

  protected: // Aid struct to partitioning and distance ordering
    template<typename T>
    using DimArray = std::array<T, GA::DIMENSION_NO>;

  public:
    // Insert entity into the tree, if there is no entity within the same location by tolerance.
    bool InsertUnique(EA::Geometry const& entityGeometry, EntityContainerView entities, TFloatScalar tolerance = GA::BASE_TOLERANCE, bool doInsertToLeaf = false)
      requires(EA::REQUIRES_CONTIGUOUS_ENTITY_IDS)
    {
      auto const nearestEntityList = GetNearestNeighbors(entityGeometry, 1, 0.0, entities, tolerance);
      if (!nearestEntityList.empty())
        return false;

      auto const entityID = EntityID(entities.size());
      return doInsertToLeaf ? Core::InsertIntoLeaf(entityID, entityGeometry, true) : Core::Insert(entityID, entityGeometry, entities);
    }

    // Insert entity into the tree, if there is no entity within the same location by tolerance.
    bool InsertUnique(
      EntityID entityID,
      EA::Geometry const& entityGeometry,
      EntityContainerView entities,
      TFloatScalar tolerance = GA::BASE_TOLERANCE,
      bool doInsertToLeaf = false)
      requires(!EA::REQUIRES_CONTIGUOUS_ENTITY_IDS)
    {
      auto const nearestEntityList = GetNearestNeighbors(entityGeometry, 1, 0.0, entities, tolerance);
      if (!nearestEntityList.empty())
        return false;

      return doInsertToLeaf ? Core::InsertIntoLeaf(entityID, entityGeometry, true) : Core::Insert(entityID, entityGeometry, entities);
    }


  public:
    constexpr TBox GetNodeBox(NodeValue nodeValue) const noexcept
    {
      auto const box = Core::GetNodeBox(nodeValue);
      TBox outBox = {};
      detail::static_for<GA::DIMENSION_NO>([&](auto dimensionID) {
        GA::SetBoxMinC(outBox, dimensionID, box.Min[dimensionID]);
        GA::SetBoxMaxC(outBox, dimensionID, box.Max[dimensionID]);
      });

      return outBox;
    }


    // Node interface
#ifdef ORTHOTREE_PUBLIC_NODE_INTERFACE
  public:
#else
  protected:
#endif

    // Visit nodes in breadth first order
    constexpr void TraverseNodesBreadthFirst(auto&& procedure, NodeValue rootNodeValue) const noexcept
    {
      auto queue = std::queue<NodeValue>();
      queue.push(rootNodeValue);
      while (!queue.empty())
      {
        auto const nodeValue = queue.front();
        queue.pop();

        auto const traverseControl = procedure(nodeValue);
        switch (traverseControl)
        {
        case TraverseControl::Terminate: return;
        case TraverseControl::SkipChildren: continue;
        case TraverseControl::Continue:
          for (NodeIDCR childNodeID : Core::GetNodeChildren(nodeValue))
            queue.push(Core::GetNodeValue(childNodeID));

          break;
        }
      }
    }

    // Visit nodes in depth first order
    constexpr void TraverseNodesDepthFirst(auto&& procedure, NodeValue pRootValue) const noexcept
    {
      auto nodeStack = std::vector<NodeValue>{};
      nodeStack.reserve(64);
      nodeStack.push_back(pRootValue);

      while (!nodeStack.empty())
      {
        auto const currentNodeValue = nodeStack.back();
        nodeStack.pop_back();

        auto const control = procedure(currentNodeValue);
        switch (control)
        {
        case TraverseControl::Terminate: return;
        case TraverseControl::SkipChildren: continue;
        case TraverseControl::Continue: {
          for (auto const& childNodeID : Core::GetNodeChildren(currentNodeValue))
          {
            nodeStack.push_back(Core::GetNodeValue(childNodeID));
          }
          break;
        }
        }
      }
    }

    // Visit nodes in priority order
    constexpr void TraverseNodesByPriority(auto&& procedure, auto&& priorityCalculator, NodeValue rootNodeValue) const noexcept
    {
      using TPriorityResult = std::invoke_result_t<decltype(priorityCalculator), NodeValue>;
      using TPriority = std::conditional_t<detail::IsStdOptionalV<TPriorityResult>, typename TPriorityResult::value_type, TPriorityResult>;

      auto constexpr GetValue = [](TPriorityResult const& pr) noexcept -> TPriority {
        if constexpr (detail::IsStdOptionalV<TPriorityResult>)
          return *pr;
        else
          return pr;
      };

      struct PrioritizedNode
      {
        NodeValue nodeValue;
        TPriority priority;

        constexpr auto operator<=>(PrioritizedNode const& other) const noexcept { return priority <=> other.priority; }
      };

      auto nodePriority = priorityCalculator(rootNodeValue);
      if constexpr (detail::IsStdOptionalV<TPriorityResult>)
      {
        if (!nodePriority)
          return;
      }

      auto nodesToProceed = std::priority_queue<PrioritizedNode, std::vector<PrioritizedNode>, std::greater<PrioritizedNode>>();
      nodesToProceed.push({ rootNodeValue, GetValue(nodePriority) });
      while (!nodesToProceed.empty())
      {
        auto const [nodeValue, priority] = nodesToProceed.top();
        nodesToProceed.pop();

        auto const control = procedure(nodeValue, priority);
        switch (control)
        {
        case TraverseControl::Terminate: return;
        case TraverseControl::SkipChildren: continue;
        case TraverseControl::Continue: break;
        }

        for (NodeIDCR childNodeID : Core::GetNodeChildren(nodeValue))
        {
          auto const& childNodeValue = Core::GetNodeValue(childNodeID);
          auto childNodePriority = priorityCalculator(childNodeValue);

          if constexpr (detail::IsStdOptionalV<TPriorityResult>)
          {
            if (!childNodePriority)
              continue;
          }

          nodesToProceed.push({ childNodeValue, GetValue(childNodePriority) });
        }
      }
    }

  public:
    // Visit nodes in breadth first order
    constexpr void TraverseNodesBreadthFirst(auto&& procedure) const noexcept
    {
      TraverseNodesBreadthFirst(std::forward<decltype(procedure)>(procedure), Core::GetRootNodeValue());
    }

    // Visit nodes in depth first order
    constexpr void TraverseNodesDepthFirst(auto&& procedure) const noexcept
    {
      TraverseNodesDepthFirst(std::forward<decltype(procedure)>(procedure), Core::GetRootNodeValue());
    }

    // Visit nodes in priority order
    constexpr void TraverseNodesByPriority(auto&& procedure, auto&& priorityCalculator) const noexcept
    {
      TraverseNodesByPriority(
        std::forward<decltype(procedure)>(procedure), std::forward<decltype(priorityCalculator)>(priorityCalculator), Core::GetRootNodeValue());
    }

    // Visit entities in breadth first order
    constexpr void TraverseEntitiesBreadthFirst(auto&& procedure) const noexcept
    {
      TraverseNodesBreadthFirst([&](auto const nodeValue) { return procedure(GetNodeEntities(nodeValue), GetNodeBox(nodeValue)); });
    }

    // Visit entities in depth first order
    constexpr void TraverseEntitiesDepthFirst(auto&& procedure) const noexcept
    {
      TraverseNodesDepthFirst([&](auto const nodeValue) { return procedure(GetNodeEntities(nodeValue), GetNodeBox(nodeValue)); });
    }

    // Visit entities in priority order
    constexpr void TraverseEntitiesByPriority(auto&& procedure, auto&& priorityCalculator) const noexcept
    {
      TraverseNodesByPriority(
        [&](auto const nodeValue, auto const& priority) { return procedure(GetNodeEntities(nodeValue), GetNodeBox(nodeValue), priority); },
        [&](auto const nodeValue) { return priorityCalculator(GetNodeBox(nodeValue)); });
    }

    // Collect all item id, traversing the tree in breadth-first search order
    std::vector<EntityID> GetEntitiesBreadthFirst(NodeIDCR rootNodeID = Core::GetRootNodeID(), bool shouldSortInsideNodes = false) const noexcept
    {
      auto entityIDs = std::vector<EntityID>();
      TraverseNodesBreadthFirst(
        [&](auto const nodeValue) {
          auto const& entities = Core::GetNodeEntities(nodeValue);
          auto const entityIDsSize = entityIDs.size();
          entityIDs.insert(entityIDs.end(), entities.begin(), entities.end());
          if (shouldSortInsideNodes)
            std::sort(entityIDs.begin() + entityIDsSize, entityIDs.end());

          return TraverseControl::Continue;
        },
        Core::GetNodeValue(rootNodeID));
      return entityIDs;
    }

  private:
    void CollectAllEntitiesInDFSRecursive(NodeValue parentNode, std::vector<EntityID>& foundEntities, bool shouldSortInsideNodes) const noexcept
    {
      auto const& entities = Core::GetNodeEntities(parentNode);
      auto const entityIDsSize = foundEntities.size();
      foundEntities.insert(foundEntities.end(), entities.begin(), entities.end());
      if (shouldSortInsideNodes)
        std::sort(foundEntities.begin() + entityIDsSize, foundEntities.end());

      for (NodeIDCR childKey : Core::GetNodeChildren(parentNode))
        CollectAllEntitiesInDFSRecursive(Core::GetNodeValue(childKey), foundEntities, shouldSortInsideNodes);
    }

  public:
    // Collect all entity id, traversing the tree in depth-first search pre-order
    std::vector<EntityID> GetEntitiesDepthFirst(NodeID parentKey = Core::GetRootNodeID(), bool shouldSortInsideNodes = false) const noexcept
    {
      auto entityIDs = std::vector<EntityID>{};
      CollectAllEntitiesInDFSRecursive(Core::GetNodeValue(parentKey), entityIDs, shouldSortInsideNodes);
      return entityIDs;
    }


    template<typename TTester, typename... TAgainst>
    static constexpr auto TestEntity(TTester const& tester, EntityID entityID, EntityContainerView entities, [[maybe_unused]] TAgainst&&... against) noexcept
    {
      if constexpr (std::is_same_v<TTester, std::monostate>)
      {
        return true;
      }
      else if constexpr (sizeof...(against) > 0 && std::is_invocable_v<TTester, EntityID, TAgainst...>)
      {
        return tester(entityID, std::forward<TAgainst>(against)...);
      }
      else if constexpr (sizeof...(against) > 0 && std::is_invocable_v<TTester, typename EA::Entity, TAgainst...>)
      {
        return tester(EA::GetEntity(entities, entityID), std::forward<TAgainst>(against)...);
      }
      else if constexpr (sizeof...(against) == 1 && std::is_invocable_v<TTester, TAgainst..., EntityID>)
      {
        return tester(std::forward<TAgainst>(against)..., entityID);
      }
      else if constexpr (sizeof...(against) == 1 && std::is_invocable_v<TTester, TAgainst..., typename EA::Entity>)
      {
        return tester(std::forward<TAgainst>(against)..., EA::GetEntity(entities, entityID));
      }
      else if constexpr (std::is_invocable_v<TTester, EntityID>)
      {
        return tester(entityID);
      }
      else if constexpr (std::is_invocable_v<TTester, typename EA::Entity>)
      {
        return tester(EA::GetEntity(entities, entityID));
      }
      else
      {
        static_assert(sizeof(TTester) == 0, "Unsupported tester signature");
      }
    }

  public: // Search functions
    // Pick search
    //
    // Accepted tester signatures:
    // * bool(EntityID) / bool(EntityID, TVector&)
    // * bool(Entity) / bool(Entity, TVector&)
    template<typename TTester = std::monostate>
    std::vector<EntityID> PickSearch(
      TVector const& pickPoint, EntityContainerView entities, TFloatScalar tolerance = GA::BASE_TOLERANCE, TTester&& tester = {}) const noexcept
    {
      auto foundEntitiyIDs = std::vector<EntityID>();
      if (!IGM::DoesBoxContainPointAD(Core::GetNodeBox(Core::GetRootNodeValue()), pickPoint, tolerance))
        return foundEntitiyIDs;

      foundEntitiyIDs.reserve(100);

      TraverseNodesDepthFirst([&](auto const nodeValue) {
        if (!IGM::DoesBoxContainPointAD(Core::GetNodeMinPoint(nodeValue), Core::GetNodeSize(nodeValue), pickPoint, tolerance))
          return TraverseControl::SkipChildren;

        auto const pointTest = [&](auto const entityID) {
          if constexpr (EA::GEOMETRY_TYPE == GeometryType::Point)
          {
            return GA::ArePointsEqual(EA::GetGeometry(entities, entityID), pickPoint, tolerance);
          }
          else if constexpr (EA::GEOMETRY_TYPE == GeometryType::Box)
          {
            return GA::DoesBoxContainPoint(EA::GetGeometry(entities, entityID), pickPoint, tolerance);
          }
          else
          {
            static_assert(false, "Unsupported geometry type!");
          }
          ORTHOTREE_UNREACHABLE();
        };

        std::ranges::copy_if(Core::GetNodeEntities(nodeValue), std::back_inserter(foundEntitiyIDs), [&](auto const entityID) {
          if (!pointTest(entityID))
            return false;

          return TestEntity(tester, entityID, entities, pickPoint);
        });

        return TraverseControl::Continue;
      });

      return foundEntitiyIDs;
    }

  public:
    // Range search
    //
    // Accepted tester signatures:
    // * bool(EntityID)
    // * bool(EntityID, TVector&)
    // * bool(Entity)
    // * bool(Entity, TVector&)
    template<bool DO_RANGE_MUST_FULLY_CONTAIN = true, typename TTester = std::monostate>
    std::vector<EntityID> RangeSearch(
      TBox const& range, EntityContainerView entities, TFloatScalar tolerance = GA::BASE_TOLERANCE, TTester&& tester = {}) const noexcept
    {
      auto foundEntities = std::vector<EntityID>{};

      auto const entityNo = entities.size();
      if (IGM::DoesRangeContainBoxAD(range, Core::GetNodeBox(Core::GetRootNodeValue()), tolerance))
      {
        foundEntities.reserve(entityNo);

        if constexpr (EA::REQUIRES_CONTIGUOUS_ENTITY_IDS)
        {
          foundEntities.resize(entityNo);
          std::iota(foundEntities.begin(), foundEntities.end(), 0);
        }
        else
        {
          std::transform(entities.begin(), entities.end(), std::back_inserter(foundEntities), [&entities](auto const& item) {
            return EA::GetEntityID(entities, item);
          });
        }
        return foundEntities;
      }

      // If the range has zero volume, it could stuck at any node comparison with point/side touch. It is eliminated to work node bounding box independently.
      const auto rangeVolume = IGM::GetVolumeAD(range);
      if (rangeVolume <= 0.0)
      {
        return foundEntities;
      }

      auto const boxTest = [&](auto const entityID) {
        if constexpr (EA::GEOMETRY_TYPE == GeometryType::Point)
        {
          return GA::DoesBoxContainPoint(range, EA::GetGeometry(entities, entityID), tolerance);
        }
        else if constexpr (EA::GEOMETRY_TYPE == GeometryType::Box)
        {
          if constexpr (DO_RANGE_MUST_FULLY_CONTAIN)
            return GA::AreBoxesOverlapped(range, EA::GetGeometry(entities, entityID), DO_RANGE_MUST_FULLY_CONTAIN, false, tolerance);
          else
            return GA::AreBoxesOverlappedStrict(range, EA::GetGeometry(entities, entityID), tolerance);
        }
        else
        {
          static_assert(false, "Unsupported geometry type!");
        }
        ORTHOTREE_UNREACHABLE();
      };

      auto const searchRangeMinPoint = IGM::GetBoxMinPointAD(range);
      auto const searchRangeSize = IGM::GetBoxSizeAD(range);
      TraverseNodesDepthFirst([&](auto const nodeValue) {
        auto const& nodeMinPoint = Core::GetNodeMinPoint(nodeValue);
        auto const& nodeSize = Core::GetNodeSize(nodeValue);
        if (!IGM::AreBoxesOverlappingByMinPoint(searchRangeMinPoint, searchRangeSize, nodeMinPoint, nodeSize, tolerance))
          return TraverseControl::SkipChildren;

        if (IGM::DoesRangeContainBoxAD(range, nodeMinPoint, nodeSize, tolerance))
        {
          TraverseNodesDepthFirst(
            [&](auto const childNodeValue) {
              if constexpr (std::is_same_v<TTester, std::monostate>)
              {
                std::ranges::copy(Core::GetNodeEntities(childNodeValue), std::back_inserter(foundEntities));
              }
              else
              {
                std::ranges::copy_if(Core::GetNodeEntities(childNodeValue), std::back_inserter(foundEntities), [&](auto const entityID) {
                  return TestEntity(tester, entityID, entities, range);
                });
              }
              return TraverseControl::Continue;
            },
            nodeValue);

          return TraverseControl::SkipChildren;
        }
        else
        {
          std::ranges::copy_if(Core::GetNodeEntities(nodeValue), std::back_inserter(foundEntities), [&](auto const entityID) {
            if (!boxTest(entityID))
              return false;

            return TestEntity(tester, entityID, entities, range);
          });
        }

        return TraverseControl::Continue;
      });

      return foundEntities;
    }

  private:
    static constexpr PlaneRelation GetEntityPlaneRelation(
      EA::Geometry const& entityGeometry, TScalar distanceOfOrigo, TVector const& planeNormal, TFloatScalar tolerance = GA::BASE_TOLERANCE) noexcept
    {
      if constexpr (EA::GEOMETRY_TYPE == GeometryType::Point)
      {
        return GA::GetPointPlaneRelation(entityGeometry, distanceOfOrigo, planeNormal, tolerance);
      }
      else if constexpr (EA::GEOMETRY_TYPE == GeometryType::Box)
      {
        return IGM::GetBoxPlaneRelationAD(IGM::GetBoxMinPointAD(entityGeometry), IGM::GetBoxSizeAD(entityGeometry), distanceOfOrigo, planeNormal, tolerance);
      }
      else
      {
        static_assert(false, "Unsupported geometry type for kNN!");
      }
      ORTHOTREE_UNREACHABLE();
    }

  public:
    // Hyperplane intersection
    //
    // Accepted tester signatures:
    // * bool(EntityID)
    // * bool(EntityID, TScalar, TVector)
    // * bool(Entity)
    // * bool(Entity, TScalar, TVector)
    template<typename TTester = std::monostate>
    std::vector<EntityID> PlaneSearch(
      TScalar distanceOfOrigo,
      TVector const& planeNormal,
      EntityContainerView entities,
      TFloatScalar tolerance = GA::BASE_TOLERANCE,
      TTester&& tester = {}) const noexcept
    {
      assert(GA::IsNormalizedVector(planeNormal));

      auto results = std::vector<EntityID>{};
      TraverseNodesDepthFirst([&](auto const nodeValue) {
        if (IGM::GetBoxPlaneRelationAD(Core::GetNodeMinPoint(nodeValue), Core::GetNodeSize(nodeValue), distanceOfOrigo, planeNormal, tolerance) != PlaneRelation::Hit)
          return TraverseControl::SkipChildren;

        for (auto const entityID : Core::GetNodeEntities(nodeValue))
          if (GetEntityPlaneRelation(EA::GetGeometry(entities, entityID), distanceOfOrigo, planeNormal, tolerance) == PlaneRelation::Hit)
            if (TestEntity(tester, entityID, entities, distanceOfOrigo, planeNormal))
              results.emplace_back(entityID);

        return TraverseControl::Continue;
      });

      return results;
    }

    // Hyperplane intersection using built-in plane
    //
    // Accepted tester signatures:
    // * bool(EntityID)
    // * bool(EntityID, TScalar, TVector)
    // * bool(Entity)
    // * bool(Entity, TScalar, TVector)
    template<typename TTester = std::monostate>
    std::vector<EntityID> PlaneSearch(
      TPlane const& plane, EntityContainerView entities, TFloatScalar tolerance = GA::BASE_TOLERANCE, TTester&& tester = {}) const noexcept
    {
      return PlaneSearch(GA::GetPlaneOrigoDistance(plane), GA::GetPlaneNormal(plane), entities, tolerance, std::forward<TTester>(tester));
    }

    // Hyperplane intersection using built-in plane (same as PlaneSearch())
    //
    // Accepted tester signatures:
    // * bool(EntityID)
    // * bool(EntityID, TScalar, TVector)
    // * bool(Entity)
    // * bool(Entity, TScalar, TVector)
    template<typename TTester = std::monostate>
    std::vector<EntityID> PlaneIntersection(
      TScalar distanceOfOrigo,
      TVector const& planeNormal,
      EntityContainerView entities,
      TFloatScalar tolerance = GA::BASE_TOLERANCE,
      TTester&& tester = {}) const noexcept
    {
      return PlaneSearch(distanceOfOrigo, planeNormal, entities, tolerance, std::forward<TTester>(tester));
    }

    // Hyperplane intersection using built-in plane (same as PlaneSearch())
    //
    // Accepted tester signatures:
    // * bool(EntityID)
    // * bool(EntityID, TScalar, TVector)
    // * bool(Entity)
    // * bool(Entity, TScalar, TVector)
    template<typename TTester = std::monostate>
    std::vector<EntityID> PlaneIntersection(
      TPlane const& plane, EntityContainerView entities, TFloatScalar tolerance = GA::BASE_TOLERANCE, TTester&& tester = {}) const noexcept
    {
      return PlaneSearch(plane, entities, tolerance, std::forward<TTester>(tester));
    }

    // Hyperplane segmentation, get all elements in positive side (Plane equation: dotProduct(planeNormal, point) = distanceOfOrigo)
    //
    // Accepted tester signatures:
    // * bool(EntityID)
    // * bool(EntityID, TScalar, TVector)
    // * bool(Entity)
    // * bool(Entity, TScalar, TVector)
    template<typename TTester = std::monostate>
    std::vector<EntityID> PlanePositiveSegmentation(
      TScalar distanceOfOrigo,
      TVector const& planeNormal,
      EntityContainerView entities,
      TFloatScalar tolerance = GA::BASE_TOLERANCE,
      TTester&& tester = {}) const noexcept
    {
      assert(GA::IsNormalizedVector(planeNormal));

      auto results = std::vector<EntityID>{};
      TraverseNodesDepthFirst([&](auto const nodeValue) {
        auto const nodeRelation =
          IGM::GetBoxPlaneRelationAD(Core::GetNodeMinPoint(nodeValue), Core::GetNodeSize(nodeValue), distanceOfOrigo, planeNormal, tolerance);

        switch (nodeRelation)
        {
        case PlaneRelation::Negative: return TraverseControl::SkipChildren;
        case PlaneRelation::Positive:
          TraverseNodesDepthFirst(
            [&](auto const childNodeValue) {
              if constexpr (std::is_same_v<std::monostate, TTester>)
              {
                std::ranges::copy(Core::GetNodeEntities(childNodeValue), std::back_inserter(results));
              }
              else
              {
                std::ranges::copy_if(Core::GetNodeEntities(childNodeValue), std::back_inserter(results), [&](auto const entityID) {
                  return TestEntity(tester, entityID, entities, distanceOfOrigo, planeNormal);
                });
              }
              return TraverseControl::Continue;
            },
            nodeValue);
          return TraverseControl::SkipChildren;

        case PlaneRelation::Hit:
          for (auto const entityID : Core::GetNodeEntities(nodeValue))
          {
            auto const entityRelation = GetEntityPlaneRelation(EA::GetGeometry(entities, entityID), distanceOfOrigo, planeNormal, tolerance);
            if (entityRelation == PlaneRelation::Negative)
              continue;

            if (TestEntity(tester, entityID, entities, distanceOfOrigo, planeNormal))
              results.emplace_back(entityID);
          }
          return TraverseControl::Continue;
        }

        ORTHOTREE_UNREACHABLE();
      });

      return results;
    }

    // Hyperplane segmentation, get all elements in positive side (Plane equation: dotProduct(planeNormal, point) = distanceOfOrigo)
    //
    // Accepted tester signatures:
    // * bool(EntityID)
    // * bool(EntityID, TScalar, TVector)
    // * bool(Entity)
    // * bool(Entity, TScalar, TVector)
    template<typename TTester = std::monostate>
    std::vector<EntityID> PlanePositiveSegmentation(
      TPlane const& plane, EntityContainerView entities, TFloatScalar tolerance = GA::BASE_TOLERANCE, TTester&& tester = {}) const noexcept
    {
      return PlanePositiveSegmentation(GA::GetPlaneOrigoDistance(plane), GA::GetPlaneNormal(plane), entities, tolerance, std::forward<TTester>(tester));
    }

    // Get all entities which relation is positive or intersected by the given space boundary planes
    //
    // Accepted tester signatures:
    // * bool(EntityID)
    // * bool(EntityID, TPlane)
    // * bool(Entity)
    // * bool(Entity, TPlane)
    template<typename TTester = std::monostate>
    std::vector<EntityID> FrustumCulling(
      std::span<TPlane const> const& boundaryPlanes, EntityContainerView entities, TFloatScalar tolerance = GA::BASE_TOLERANCE, TTester&& tester = {}) const noexcept
    {
      auto results = std::vector<EntityID>{};
      if (boundaryPlanes.empty())
        return results;

      assert(std::all_of(boundaryPlanes.begin(), boundaryPlanes.end(), [](auto const& plane) -> bool {
        return GA::IsNormalizedVector(GA::GetPlaneNormal(plane));
      }));

      auto const selector = [&](auto const nodeValue) -> bool {
        auto const& nodeMinPoint = Core::GetNodeMinPoint(nodeValue);
        auto const& nodeSize = Core::GetNodeSize(nodeValue);

        for (auto const& plane : boundaryPlanes)
        {
          auto const relation =
            IGM::GetBoxPlaneRelationAD(nodeMinPoint, nodeSize, GA::GetPlaneOrigoDistance(plane), GA::GetPlaneNormal(plane), tolerance);
          switch (relation)
          {
          case PlaneRelation::Hit: return true;
          case PlaneRelation::Negative: return false;
          case PlaneRelation::Positive: break;
          }
        }
        return true;
      };

      auto const procedure = [&](auto const nodeValue) {
        if (!selector(nodeValue))
          return TraverseControl::SkipChildren;

        for (auto const entityID : Core::GetNodeEntities(nodeValue))
        {
          bool isAnyOnNegativeSide = false;
          for (auto const& plane : boundaryPlanes)
          {
            auto const relation =
              GetEntityPlaneRelation(EA::GetGeometry(entities, entityID), GA::GetPlaneOrigoDistance(plane), GA::GetPlaneNormal(plane), tolerance);
            auto const isOnPositiveSideOrHit = [&](auto const planeRelation) {
              switch (planeRelation)
              {
              case PlaneRelation::Hit: return TestEntity(tester, entityID, entities, plane);
              case PlaneRelation::Negative: return false;
              case PlaneRelation::Positive: return true;
              }
              return false;
            }(relation);

            isAnyOnNegativeSide |= !isOnPositiveSideOrHit;
            if (isAnyOnNegativeSide)
              break;
          }

          if (!isAnyOnNegativeSide)
            results.emplace_back(entityID);
        }

        return TraverseControl::Continue;
      };

      TraverseNodesDepthFirst(procedure);

      return results;
    }

    struct FrustumCondition
    {
      std::vector<std::pair<TPlane, bool>> boundaryPlanes; // pair<plane, isCrossingAllowed>
    };

    struct RangeCondition
    {
      TBox range;
      bool isFullyContainRequired = false;
    };

    struct PlaneIntersectionCondition
    {
      TPlane plane;
      TFloatScalar tolerance = GA::BASE_TOLERANCE;
    };
    using EntityIDCondition = std::function<bool(EntityID)>;
    using EntityCondition = std::function<bool(typename EA::Entity)>;
    using QueryCondition = std::variant<FrustumCondition, RangeCondition, PlaneIntersectionCondition, EntityIDCondition, EntityCondition>;

    // Complex query with multiple conditions. The conditions are combined with logical AND by default, but can be switched to OR by template parameter.
    template<bool IS_LOGICAL_OR_FILTERING = false>
    constexpr std::vector<EntityID> Query(auto const& conditions, EntityContainerView entities, TFloatScalar tolerance = GA::BASE_TOLERANCE) const noexcept
    {
      auto results = std::vector<EntityID>{};

      TraverseNodesDepthFirst([&](auto const nodeValue) {
        // Node selection

        auto const& nodeMinPoint = Core::GetNodeMinPoint(nodeValue);
        auto const& nodeSize = Core::GetNodeSize(nodeValue);

        bool isNodePassed = false;
        for (auto const& condition : conditions)
        {
          auto const isNodePassedCondition = detail::VisitVariant(
            condition,
            [&](FrustumCondition const& frustumCondition) {
              for (auto const& [plane, isCrossingAllowed] : frustumCondition.boundaryPlanes)
              {
                auto const relation =
                  IGM::GetBoxPlaneRelationAD(nodeMinPoint, nodeSize, GA::GetPlaneOrigoDistance(plane), GA::GetPlaneNormal(plane), tolerance);
                switch (relation)
                {
                case PlaneRelation::Hit: return true;
                case PlaneRelation::Negative: return false;
                case PlaneRelation::Positive: break;
                }
              }
              return true;
            },
            [&](RangeCondition const& rangeCondition) {
              auto const& [range, shouldFullyContained] = rangeCondition;

              auto const searchRangeMinPoint = IGM::GetBoxMinPointAD(range);
              auto const searchRangeSize = IGM::GetBoxSizeAD(range);

              return IGM::AreBoxesOverlappingByMinPoint(searchRangeMinPoint, searchRangeSize, nodeMinPoint, nodeSize, tolerance);
            },
            [&](PlaneIntersectionCondition const& planeCondition) {
              auto const& [plane, planeTolerance] = planeCondition;

              return IGM::GetBoxPlaneRelationAD(nodeMinPoint, nodeSize, GA::GetPlaneOrigoDistance(plane), GA::GetPlaneNormal(plane), planeTolerance) ==
                     PlaneRelation::Hit;
            },
            [&](EntityIDCondition const&) { return true; },
            [&](EntityCondition const&) { return true; });

          if constexpr (IS_LOGICAL_OR_FILTERING)
          {
            isNodePassed |= isNodePassedCondition;
            if (isNodePassed)
              break;
          }
          else
          {
            if (!isNodePassedCondition)
              return TraverseControl::SkipChildren;
          }
        }

        if constexpr (IS_LOGICAL_OR_FILTERING)
        {
          if (!isNodePassed)
            return TraverseControl::SkipChildren;
        }


        // Entity selection

        for (auto const entityID : Core::GetNodeEntities(nodeValue))
        {
          bool isPassed = !IS_LOGICAL_OR_FILTERING;
          for (auto const& condition : conditions)
          {
            auto const isConditionPassed = detail::VisitVariant(
              condition,
              [&](FrustumCondition const& frustumCondition) {
                for (auto const& [plane, isCrossingAllowed] : frustumCondition.boundaryPlanes)
                {
                  auto relation =
                    GetEntityPlaneRelation(EA::GetGeometry(entities, entityID), GA::GetPlaneOrigoDistance(plane), GA::GetPlaneNormal(plane), tolerance);
                  switch (relation)
                  {
                  case PlaneRelation::Hit:
                    if (!isCrossingAllowed)
                      return false;
                    break;
                  case PlaneRelation::Negative: return false;
                  case PlaneRelation::Positive: break;
                  }
                }
                return true;
              },
              [&](RangeCondition const& rangeCondition) {
                auto const& [range, shouldFullyContained] = rangeCondition;

                if constexpr (EA::GEOMETRY_TYPE == GeometryType::Point)
                {
                  return GA::DoesBoxContainPoint(range, EA::GetGeometry(entities, entityID), tolerance);
                }
                else if constexpr (EA::GEOMETRY_TYPE == GeometryType::Box)
                {
                  if (shouldFullyContained)
                    return GA::AreBoxesOverlapped(range, EA::GetGeometry(entities, entityID), shouldFullyContained, false, tolerance);
                  else
                    return GA::AreBoxesOverlappedStrict(range, EA::GetGeometry(entities, entityID), tolerance);
                }
                else
                {
                  static_assert(false, "Unsupported geometry type!");
                  return false;
                }
              },
              [&](PlaneIntersectionCondition const& planeCondition) {
                auto const& [plane, planeTolerance] = planeCondition;

                return GetEntityPlaneRelation(EA::GetGeometry(entities, entityID), GA::GetPlaneOrigoDistance(plane), GA::GetPlaneNormal(plane), planeTolerance) ==
                       PlaneRelation::Hit;
              },
              [&](EntityIDCondition const& entityIDTester) { return entityIDTester(entityID); },
              [&](EntityCondition const& entityTester) { return entityTester(EA::GetEntity(entities, entityID)); });

            if constexpr (IS_LOGICAL_OR_FILTERING)
            {
              isPassed |= isConditionPassed;
              if (isPassed)
                break;
            }
            else
            {
              if (!isConditionPassed)
              {
                isPassed = false;
                break;
              }
            }
          }

          if (isPassed)
            results.emplace_back(entityID);
        }

        return TraverseControl::Continue;
      });

      return results;
    }

  public: // K Nearest Neighbor
    // Get the precise distance between the entity and kNN's search point. Floating-point return value is required.
    using EntityDistanceFn = std::function<TFloatScalar(TVector const&, EntityID)>;

  private: // K Nearest Neighbor helpers
    static constexpr TFloatScalar GetValueWithToleranceUpper(TFloatScalar value, TFloatScalar tolerance = GA::BASE_TOLERANCE) noexcept
    {
      return std::fmax(tolerance, value * (TFloatScalar(1.0) + tolerance));
    }

    static constexpr TFloatScalar GetValueWithToleranceLower(TFloatScalar value, TFloatScalar tolerance = GA::BASE_TOLERANCE) noexcept
    {
      return value == 0 ? -tolerance : (value * (TFloatScalar(1.0) - tolerance));
    }

    // TODO: remove? lower is not used
    struct FarthestDistance
    {
      TFloatScalar lower;
      TFloatScalar upper;
    };

    struct EntityDistance
    {
      EntityID entityID;
      TFloatScalar distance;

      constexpr auto operator<=>(EntityDistance const& other) const noexcept { return distance <=> other.distance; }
      constexpr bool operator==(EntityDistance const& other) const noexcept { return distance == other.distance; }
    };

    struct EntityDistances
    {
      std::vector<EntityDistance> optimistic;
      std::vector<EntityDistance> pessimistic;
    };

    template<typename TEntityDistanceFn = std::monostate>
    static void AddEntityDistance(
      std::size_t neighborCount,
      TVector const& searchPoint,
      TEntityDistanceFn const& entityDistanceFn,
      auto const& nodeEntityIDs,
      EntityContainerView entities,
      TFloatScalar tolerance,
      EntityDistances& neighborEntities,
      FarthestDistance& farthestEntityDistance) noexcept
    {
      auto constexpr pessimisticCmp = [](auto const& lhs, auto const& rhs) {
        return lhs.pessimisticDistance < rhs.pessimisticDistance;
      };

      auto neighborEntitiesPessimisticMaxHeap = std::vector<TFloatScalar>{};
      for (auto const entityID : nodeEntityIDs)
      {
        typename GA::PointBoxMinMaxDistance pbd;
        if constexpr (EA::GEOMETRY_TYPE == GeometryType::Point)
        {
          const auto distance = GA::Distance(searchPoint, EA::GetGeometry(entities, entityID));
          pbd = { distance, distance };
        }
        else if constexpr (EA::GEOMETRY_TYPE == GeometryType::Box)
        {
          pbd = GA::MinMaxDistance(searchPoint, EA::GetGeometry(entities, entityID), tolerance);
        }
        else
        {
          static_assert(false, "Unsupported geometry type for kNN!");
        }

        if (pbd.min >= farthestEntityDistance.upper)
          continue;

        if constexpr (!std::is_same_v<TEntityDistanceFn, std::monostate>)
        {
          auto const entityDistanceResult = TestEntity(entityDistanceFn, entityID, entities, searchPoint);
          if (!entityDistanceResult)
            continue;

          if constexpr (detail::IsStdOptionalV<decltype(entityDistanceResult)>)
          {
            pbd.min = *entityDistanceResult;
            if (pbd.min >= farthestEntityDistance.upper)
              continue;

            pbd.minMax = pbd.min;
          }
        }

        auto const shouldHeapify = neighborEntities.optimistic.size() == neighborCount - 1;
        neighborEntities.optimistic.push_back({ entityID, pbd.min });
        neighborEntities.pessimistic.push_back({ entityID, pbd.minMax });
        if (neighborEntities.pessimistic.size() < neighborCount)
          continue;

        if (shouldHeapify)
        {
          std::ranges::make_heap(neighborEntities.optimistic);
          std::ranges::make_heap(neighborEntities.pessimistic);

          farthestEntityDistance.upper = GetValueWithToleranceUpper(neighborEntities.pessimistic[0].distance, tolerance);
          if constexpr (EA ::GEOMETRY_TYPE == GeometryType::Point)
            farthestEntityDistance.lower = farthestEntityDistance.upper;
          else
            farthestEntityDistance.lower = GetValueWithToleranceLower(neighborEntities.pessimistic[0].distance, tolerance);
        }
        else
        {
          std::ranges::push_heap(neighborEntities.optimistic);

          std::ranges::push_heap(neighborEntities.pessimistic);
          std::ranges::pop_heap(neighborEntities.pessimistic);
          neighborEntities.pessimistic.pop_back();

          auto nthFarthestEntityDistanceLimit = GetValueWithToleranceUpper(neighborEntities.pessimistic.front().distance, tolerance);

          if (nthFarthestEntityDistanceLimit < farthestEntityDistance.upper)
          {
            farthestEntityDistance.upper = nthFarthestEntityDistanceLimit;
            if constexpr (EA::GEOMETRY_TYPE == GeometryType::Point)
              farthestEntityDistance.lower = farthestEntityDistance.upper;
            else if constexpr (EA::GEOMETRY_TYPE == GeometryType::Box)
              farthestEntityDistance.lower = GetValueWithToleranceLower(neighborEntities.pessimistic.front().distance, tolerance);
            else
              static_assert(false);

            bool pessimisticRemoved = false;
            while (neighborEntities.optimistic.front().distance > farthestEntityDistance.upper)
            {
              auto const removedEntityID = neighborEntities.optimistic.front().entityID;
              auto const it =
                std::ranges::find_if(neighborEntities.pessimistic, [removedEntityID](auto const& ed) { return ed.entityID == removedEntityID; });
              if (it != neighborEntities.pessimistic.end())
              {
                neighborEntities.pessimistic.erase(it);
                pessimisticRemoved = true;
              }
              std::ranges::pop_heap(neighborEntities.optimistic);
              neighborEntities.optimistic.pop_back();
            }

            if (pessimisticRemoved)
              std::ranges::make_heap(neighborEntities.pessimistic);
          }
        }
      }
    }

    template<bool SHOULD_SORT_ENTITIES_BY_DISTANCE = true>
    static constexpr std::vector<EntityID> ConvertEntityDistanceToList(std::vector<EntityDistance>& neighborEntities, std::size_t neighborNo) noexcept
    {
      auto entityIDs = std::vector<EntityID>();
      if (neighborEntities.empty())
        return entityIDs;

      if (neighborEntities.size() < neighborNo)
      {
        if constexpr (SHOULD_SORT_ENTITIES_BY_DISTANCE)
        {
          std::sort(neighborEntities.begin(), neighborEntities.end());
        }
      }
      else
      {
        std::sort_heap(neighborEntities.begin(), neighborEntities.end());
      }

      auto const entityNo = neighborEntities.size();
      entityIDs.resize(entityNo);
      for (std::size_t i = 0; i < entityNo; ++i)
        entityIDs[i] = neighborEntities[i].entityID;

      return entityIDs;
    }

    constexpr IGM::Geometry GetNodeWallDistance(TVector const& searchPoint, NodeValue nodeValue, bool isInsideConsideredAsZero) const noexcept
    {
      return IGM::GetBoxWallDistanceAD(searchPoint, Core::GetNodeMinPoint(nodeValue), Core::GetNodeSize(nodeValue), isInsideConsideredAsZero);
    }

  public:
    // Get K Nearest Neighbor sorted by distance (point distance should be less than maxDistanceWithin, it is used as a Tolerance check). It may
    // results more element than neighborCount, if those are in equal distance (point-like) or possible hit (box-like).
    //
    // Accepted entityDistanceFn signatures:
    // * bool(EntityID)
    // * bool(EntityID, TVector)
    // * bool(Entity)
    // * bool(Entity, TVector)
    // * std::optional<TScalar>(EntityID)
    // * std::optional<TScalar>(EntityID, TVector)
    // * std::optional<TScalar>(Entity)
    // * std::optional<TScalar>(Entity, TVector)
    template<bool SHOULD_SORT_ENTITIES_BY_DISTANCE = true, typename TEntityDistanceFn = std::monostate>
    std::vector<EntityID> GetNearestNeighbors(
      TVector const& searchPoint,
      std::size_t neighborCount,
      TScalar maxDistanceWithin,
      EntityContainerView entities,
      TFloatScalar tolerance = GA::BASE_TOLERANCE,
      TEntityDistanceFn&& entityDistanceFn = {}) const noexcept
    {
      assert(neighborCount > 0 && "At least one neighbor must be requested!");
      if (neighborCount == 0)
        return {};

      auto neighborEntities = EntityDistances{};
      neighborEntities.optimistic.reserve(neighborCount + 4);
      neighborEntities.pessimistic.reserve(neighborCount);

      // farthestEntityDistance already contains the numerical tolerance
      auto farthestEntityDistance =
        FarthestDistance{ {},
                          maxDistanceWithin == std::numeric_limits<TScalar>::max() ? std::numeric_limits<TScalar>::max()
                                                                                   : GetValueWithToleranceUpper(maxDistanceWithin, tolerance) };
      TraverseNodesByPriority(
        [&](auto const nodeValue, TFloatScalar nodeDistance) -> TraverseControl {
          if (nodeDistance >= farthestEntityDistance.upper)
            return TraverseControl::Terminate;

          AddEntityDistance(
            neighborCount, searchPoint, entityDistanceFn, Core::GetNodeEntities(nodeValue), entities, tolerance, neighborEntities, farthestEntityDistance);

          return TraverseControl::Continue;
        },
        [&](auto const nodeValue) -> std::optional<TFloatScalar> {
          auto wallDistance = GetNodeWallDistance(searchPoint, nodeValue, true);
          if (wallDistance >= farthestEntityDistance.upper)
            return std::nullopt;

          return wallDistance;
        });

      return ConvertEntityDistanceToList<SHOULD_SORT_ENTITIES_BY_DISTANCE>(neighborEntities.optimistic, neighborCount);
    }

    // Get K Nearest Neighbor sorted by distance
    //
    // Accepted entityDistanceFn signatures:
    // * bool(EntityID)
    // * bool(EntityID, TVector)
    // * bool(Entity)
    // * bool(Entity, TVector)
    // * std::optional<TScalar>(EntityID)
    // * std::optional<TScalar>(EntityID, TVector)
    // * std::optional<TScalar>(Entity)
    // * std::optional<TScalar>(Entity, TVector)
    template<bool SHOULD_SORT_ENTITIES_BY_DISTANCE = true, typename TEntityDistanceFn = std::monostate>
    std::vector<EntityID> GetNearestNeighbors(
      TVector const& searchPoint,
      std::size_t neighborNo,
      EntityContainerView entities,
      TFloatScalar tolerance = GA::BASE_TOLERANCE,
      TEntityDistanceFn&& entityDistanceFn = {}) const noexcept
    {
      return GetNearestNeighbors<SHOULD_SORT_ENTITIES_BY_DISTANCE>(
        searchPoint, neighborNo, std::numeric_limits<TScalar>::max(), entities, tolerance, std::forward<TEntityDistanceFn>(entityDistanceFn));
    }


  private:
    struct SweepAndPruneDatabase
    {
      constexpr SweepAndPruneDatabase(EntityContainerView entities, auto const& entityIDs) noexcept
      : m_sortedEntityIDs(entityIDs.begin(), entityIDs.end())
      {
        std::sort(m_sortedEntityIDs.begin(), m_sortedEntityIDs.end(), [&](auto const entityIDL, auto const entityIDR) {
          if constexpr (EA::GEOMETRY_TYPE == GeometryType::Point)
          {
            return GA::GetPointC(EA::GetGeometry(entities, entityIDL), 0) < GA::GetPointC(EA::GetGeometry(entities, entityIDR), 0);
          }
          else if constexpr (EA::GEOMETRY_TYPE == GeometryType::Box)
          {
            return GA::GetBoxMinC(EA::GetGeometry(entities, entityIDL), 0) < GA::GetBoxMinC(EA::GetGeometry(entities, entityIDR), 0);
          }
          else
          {
            static_assert(false, "Unsupported geometry type for collision detection!");
          }
          ORTHOTREE_UNREACHABLE();
        });
      }

      constexpr std::vector<EntityID> const& GetEntities() const noexcept { return m_sortedEntityIDs; }

    private:
      std::vector<EntityID> m_sortedEntityIDs;
    };

  public:
    // Client-defined Collision detector based on indexes. AABB intersection is executed independently from this checker.
    using FCollisionDetector = std::function<bool(EntityID, EntityID)>;

  private:
    // Collision detection: Returns all overlapping entities from the source trees.
    static void CollisionDetection(
      OrthoTreeQueryBase const& leftTree,
      EntityContainerView leftEntities,
      NodeValue leftNodeValue,
      OrthoTreeQueryBase const& rightTree,
      EntityContainerView rightEntities,
      NodeValue rightNodeValue,
      std::vector<std::pair<EntityID, EntityID>>& collidedEntities,
      TFloatScalar tolerance,
      std::optional<FCollisionDetector> const& collisionDetector) noexcept
    {
      struct NodeValueAndStatus
      {
        NodeValue nodeValue;
        bool IsTraversed;
      };

      enum : bool
      {
        Left,
        Right
      };

      auto const isSameTree = &leftTree == &rightTree;

      auto const trees = std::array{ &leftTree, &rightTree };

      auto entitiesInOrderCache = std::array<std::unordered_map<NodeValue, SweepAndPruneDatabase>, 2>{};
      auto const GetOrCreateEntitiesInOrder = [&](bool side, NodeValue nodeValue, EntityContainerView entities) -> std::vector<EntityID> const& {
        if (isSameTree)
          side = Left;

        auto itKeyAndSPD = entitiesInOrderCache[side].find(nodeValue);
        if (itKeyAndSPD == entitiesInOrderCache[side].end())
        {
          bool isInserted = false;
          std::tie(itKeyAndSPD, isInserted) =
            entitiesInOrderCache[side].emplace(nodeValue, SweepAndPruneDatabase(entities, trees[side]->GetNodeEntities(nodeValue)));
        }

        return itKeyAndSPD->second.GetEntities();
      };

      auto nodePairToProceed = std::queue<std::array<NodeValueAndStatus, 2>>{};
      nodePairToProceed.push(
        {
          NodeValueAndStatus{  leftNodeValue, false },
          NodeValueAndStatus{ rightNodeValue, false }
      });
      while (!nodePairToProceed.empty())
      {
        auto const parentNodePair = nodePairToProceed.front();
        nodePairToProceed.pop();

        // Check the current ascendant content

        auto const& leftEntitiesInOrder = GetOrCreateEntitiesInOrder(Left, parentNodePair[Left].nodeValue, leftEntities);
        auto const& rightEntitiesInOrder = GetOrCreateEntitiesInOrder(Right, parentNodePair[Right].nodeValue, rightEntities);

        auto const rightEntityNo = rightEntitiesInOrder.size();
        std::size_t iRightEntityBegin = 0;
        for (auto const leftEntityID : leftEntitiesInOrder)
        {
          auto const& leftEntityGeometry = EA::GetGeometry(leftEntities, leftEntityID);
          for (; iRightEntityBegin < rightEntityNo; ++iRightEntityBegin)
          {
            auto const& rightEntityGeometry = EA::GetGeometry(rightEntities, rightEntitiesInOrder[iRightEntityBegin]);
            if constexpr (EA::GEOMETRY_TYPE == GeometryType::Point)
            {
              if (GA::GetPointC(rightEntityGeometry, 0) >= GA::GetPointC(leftEntityGeometry, 0))
                break; // sweep and prune optimization
            }
            else if constexpr (EA::GEOMETRY_TYPE == GeometryType::Box)
            {
              if (GA::GetBoxMaxC(rightEntityGeometry, 0) >= GA::GetBoxMinC(leftEntityGeometry, 0))
                break; // sweep and prune optimization
            }
            else
            {
              static_assert(false, "Unsupported geometry type for collision detection!");
            }
          }

          for (std::size_t iRightEntity = iRightEntityBegin; iRightEntity < rightEntityNo; ++iRightEntity)
          {
            auto const rightEntityID = rightEntitiesInOrder[iRightEntity];

            auto const& rightEntityGeometry = EA::GetGeometry(rightEntities, rightEntityID);
            if constexpr (EA::GEOMETRY_TYPE == GeometryType::Point)
            {
              if (GA::GetPointC(leftEntityGeometry, 0) < GA::GetPointC(rightEntityGeometry, 0))
                break; // sweep and prune optimization

              if (GA::ArePointEqual(leftEntityGeometry, rightEntityGeometry, tolerance))
                if (!collisionDetector || (*collisionDetector)(leftEntityID, rightEntityID))
                  collidedEntities.emplace_back(leftEntityID, rightEntityID);
            }
            else if constexpr (EA::GEOMETRY_TYPE == GeometryType::Box)
            {
              if (GA::GetBoxMaxC(leftEntityGeometry, 0) < GA::GetBoxMinC(rightEntityGeometry, 0))
                break; // sweep and prune optimization

              if (GA::AreBoxesOverlapped(leftEntityGeometry, rightEntityGeometry, false, false, tolerance))
                if (!collisionDetector || (*collisionDetector)(leftEntityID, rightEntityID))
                  collidedEntities.emplace_back(leftEntityID, rightEntityID);
            }
            else
            {
              static_assert(false, "Unsupported geometry type for collision detection!");
            }
          }
        }

        // Collect children
        auto childNodes = std::array<std::vector<NodeValueAndStatus>, 2>{};
        for (auto const sideID : { Left, Right })
        {
          auto const& [nodeValue, isTraversed] = parentNodePair[sideID];
          if (isTraversed)
            continue;

          auto const& childIDs = trees[sideID]->Core::GetNodeChildren(nodeValue);
          childNodes[sideID].resize(childIDs.size());
          std::transform(childIDs.begin(), childIDs.end(), childNodes[sideID].begin(), [&](NodeIDCR childNodeID) -> NodeValueAndStatus {
            return { trees[sideID]->Core::GetNodeValue(childNodeID), false };
          });
        }

        // Stop condition
        if (childNodes[Left].empty() && childNodes[Right].empty())
          continue;

        // Add parent if it has any element
        for (auto const sideID : { Left, Right })
          if (!trees[sideID]->IsNodeEntitiesEmpty(parentNodePair[sideID].nodeValue))
            childNodes[sideID].push_back({ parentNodePair[sideID].nodeValue, true });


        // Cartesian product of childNodes left and right
        for (auto const& leftChildNode : childNodes[Left])
          for (auto const& rightChildNode : childNodes[Right])
            if (!(leftChildNode.nodeValue == parentNodePair[Left].nodeValue && rightChildNode.nodeValue == parentNodePair[Right].nodeValue))
              if (IGM::AreBoxesOverlappingByMinPoint(
                    trees[Left]->GetNodeMinPoint(leftChildNode.nodeValue),
                    trees[Left]->GetNodeSize(leftChildNode.nodeValue),
                    trees[Right]->GetNodeMinPoint(rightChildNode.nodeValue),
                    trees[Right]->GetNodeSize(rightChildNode.nodeValue),
                    tolerance))
                nodePairToProceed.emplace(std::array{ leftChildNode, rightChildNode });
      }
    }

  public:
    // Collision detection: Returns all overlapping entities from the source trees.
    static std::vector<std::pair<EntityID, EntityID>> CollisionDetection(
      OrthoTreeQueryBase const& leftTree,
      EntityContainerView leftEntities,
      OrthoTreeQueryBase const& rightTree,
      EntityContainerView rightEntities,
      TFloatScalar tolerance = GA::BASE_TOLERANCE,
      std::optional<FCollisionDetector> const& collisionDetector = std::nullopt) noexcept
    {
      std::vector<std::pair<EntityID, EntityID>> collidedEntities;
      collidedEntities.reserve(leftEntities.size() / 10);

      CollisionDetection(
        leftTree,
        leftEntities,
        leftTree.GetNodeValue(Core::GetRootNodeID()),
        rightTree,
        rightEntities,
        rightTree.GetNodeValue(Core::GetRootNodeID()),
        collidedEntities,
        tolerance,
        collisionDetector);

      return collidedEntities;
    }


    // Collision detection: Returns all overlapping boxes from the source trees.
    std::vector<std::pair<EntityID, EntityID>> CollisionDetection(
      EntityContainerView const& boxes,
      OrthoTreeQueryBase const& otherTree,
      EntityContainerView const& otherBoxes,
      TFloatScalar tolerance = GA::BASE_TOLERANCE,
      std::optional<FCollisionDetector> const& collisionDetector = std::nullopt) const noexcept
    {
      return CollisionDetection(*this, boxes, otherTree, otherBoxes, tolerance, collisionDetector);
    }

  private:
    struct NodeCollisionContext
    {
      NodeValue nodeValue;
      IGM::Box Box;
      std::vector<EntityID> EntityIDs;
    };

    constexpr void FillNodeCollisionContext(NodeValue nodeValue, NodeCollisionContext& nodeContext, auto const& comparator) const noexcept
    {
      auto const& nodeEntities = Core::GetNodeEntities(nodeValue);

      nodeContext.nodeValue = nodeValue;
      nodeContext.EntityIDs.clear();
      nodeContext.EntityIDs.assign(nodeEntities.begin(), nodeEntities.end());
      std::ranges::sort(nodeContext.EntityIDs, comparator);
      nodeContext.Box = Core::GetNodeBox(nodeValue);
    }

    constexpr void InsertCollidedEntitiesInsideNode(
      EntityContainerView entities,
      NodeCollisionContext const& context,
      std::vector<std::pair<EntityID, EntityID>>& collidedEntities,
      TFloatScalar tolerance,
      std::optional<FCollisionDetector> const& collisionDetector) const noexcept
    {
      auto const& entityIDs = context.EntityIDs;
      auto const entityNo = entityIDs.size();
      for (std::size_t i = 0; i < entityNo; ++i)
      {
        auto const entityIDI = entityIDs[i];
        auto const& entityBoxI = EA::GetGeometry(entities, entityIDI);

        for (std::size_t j = i + 1; j < entityNo; ++j)
        {
          auto const entityIDJ = entityIDs[j];
          auto const& entityBoxJ = EA::GetGeometry(entities, entityIDJ);

          if constexpr (EA::GEOMETRY_TYPE == GeometryType::Point)
          {
            if (GA::GetPointC(entityBoxI, 0) < GA::GetPointC(entityBoxJ, 0))
              break; // sweep and prune optimization

            if (GA::ArePointsEqual(entityBoxI, entityBoxJ, tolerance))
              if (!collisionDetector || (*collisionDetector)(entityIDI, entityIDJ))
                collidedEntities.emplace_back(entityIDI, entityIDJ);
          }
          else if constexpr (EA::GEOMETRY_TYPE == GeometryType::Box)
          {
            if (GA::GetBoxMaxC(entityBoxI, 0) < GA::GetBoxMinC(entityBoxJ, 0))
              break; // sweep and prune optimization

            if (GA::AreBoxesOverlappedStrict(entityBoxI, entityBoxJ, tolerance))
              if (!collisionDetector || (*collisionDetector)(entityIDI, entityIDJ))
                collidedEntities.emplace_back(entityIDI, entityIDJ);
          }
          else
          {
            static_assert(false, "Unsupported geometry type for collision detection!");
          }
        }
      }
    }

    constexpr void InsertCollidedEntitiesWithParents(
      EntityContainerView entities,
      std::vector<NodeCollisionContext> const& nodeContextStack,
      std::vector<std::pair<EntityID, EntityID>>& collidedEntities,
      TFloatScalar tolerance,
      std::optional<FCollisionDetector> const& collisionDetector) const noexcept
    {
      auto const& nodeContext = nodeContextStack.back();
      auto const& nodeMinPoint = nodeContext.Box.Min;
      auto const& nodeSize = Core::GetNodeSize(nodeContext.nodeValue);
      auto const& entityIDs = nodeContext.EntityIDs;

      auto const entityNo = entityIDs.size();
      auto const depthNo = nodeContextStack.size();
      for (depth_t parentDepthID = 0; parentDepthID < depthNo - 1; ++parentDepthID)
      {
        auto const& [pParentNodeValue, parentBox, parenEntityIDs] = nodeContextStack[parentDepthID];

        auto iEntityBegin = std::size_t{};
        for (auto const parenEntityID : parenEntityIDs)
        {
          auto const& parentEntityGeometry = EA::GetGeometry(entities, parenEntityID);
          if constexpr (EA::GEOMETRY_TYPE == GeometryType::Point)
          {
            if (GA::GetPointC(parentEntityGeometry, 0) > nodeContext.Box.Max[0])
              break;
          }
          else if constexpr (EA::GEOMETRY_TYPE == GeometryType::Box)
          {
            if (GA::GetBoxMinC(parentEntityGeometry, 0) > nodeContext.Box.Max[0])
              break;
          }
          else
          {
            static_assert(false, "Unsupported geometry type for collision detection!");
          }

          auto const parentEntityMinPoint = IGM::GetBoxMinPointAD(parentEntityGeometry);
          auto const parentEntitySize = IGM::GetBoxSizeAD(parentEntityGeometry);
          if (!IGM::AreBoxesOverlappingByMinPoint(nodeMinPoint, nodeSize, parentEntityMinPoint, parentEntitySize, tolerance))
            continue;

          for (; iEntityBegin < entityNo; ++iEntityBegin)
          {
            if constexpr (EA::GEOMETRY_TYPE == GeometryType::Point)
            {
              if (GA::GetPointC(EA::GetGeometry(entities, entityIDs[iEntityBegin]), 0) >= GA::GetPointC(parentEntityGeometry, 0))
                break; // sweep and prune optimization
            }
            else if constexpr (EA::GEOMETRY_TYPE == GeometryType::Box)
            {
              if (GA::GetBoxMaxC(EA::GetGeometry(entities, entityIDs[iEntityBegin]), 0) >= GA::GetBoxMinC(parentEntityGeometry, 0))
                break; // sweep and prune optimization
            }
            else
            {
              static_assert(false, "Unsupported geometry type for collision detection!");
            }
          }

          for (std::size_t iEntity = iEntityBegin; iEntity < entityNo; ++iEntity)
          {
            auto const entityID = entityIDs[iEntity];
            auto const& entityGeometry = EA::GetGeometry(entities, entityID);

            if constexpr (EA::GEOMETRY_TYPE == GeometryType::Point)
            {
              if (GA::GetPointC(parentEntityGeometry, 0) < GA::GetPointC(entityGeometry, 0))
                break; // sweep and prune optimization

              if (GA::ArePointsEqual(parentEntityGeometry, entityGeometry, tolerance))
                if (!collisionDetector || (*collisionDetector)(entityID, parenEntityID))
                  collidedEntities.emplace_back(entityID, parenEntityID);
            }
            else if constexpr (EA::GEOMETRY_TYPE == GeometryType::Box)
            {
              if (GA::GetBoxMaxC(parentEntityGeometry, 0) < GA::GetBoxMinC(entityGeometry, 0))
                break; // sweep and prune optimization

              if (GA::AreBoxesOverlappedStrict(entityGeometry, parentEntityGeometry, tolerance))
                if (!collisionDetector || (*collisionDetector)(entityID, parenEntityID))
                  collidedEntities.emplace_back(entityID, parenEntityID);
            }
            else
            {
              static_assert(false, "Unsupported geometry type for collision detection!");
            }
          }
        }
      }
    }

    void InsertCollidedEntitiesInSubtree(
      EntityContainerView entities,
      auto const& comparator,
      NodeValue nodeValue,
      std::vector<NodeCollisionContext>& nodeContextStack,
      std::vector<std::pair<EntityID, EntityID>>& collidedEntities,
      TFloatScalar tolerance,
      std::optional<FCollisionDetector> const& collisionDetector) const noexcept
    {
      auto& nodeContext = nodeContextStack.emplace_back();
      FillNodeCollisionContext(nodeValue, nodeContext, comparator);
      InsertCollidedEntitiesInsideNode(entities, nodeContext, collidedEntities, tolerance, collisionDetector);
      InsertCollidedEntitiesWithParents(entities, nodeContextStack, collidedEntities, tolerance, collisionDetector);

      for (NodeIDCR childKey : Core::GetNodeChildren(nodeValue))
        InsertCollidedEntitiesInSubtree(entities, comparator, Core::GetNodeValue(childKey), nodeContextStack, collidedEntities, tolerance, collisionDetector);

      // Pairwise sub-tree collision detection for loose octree
      if constexpr (CONFIG::LOOSE_FACTOR > 1.0)
      {
        auto const& childNodeIDs = Core::GetNodeChildren(nodeValue);
        for (auto it1 = childNodeIDs.begin(); it1 != childNodeIDs.end(); ++it1)
        {
          for (auto it2 = it1 + 1; it2 != childNodeIDs.end(); ++it2)
          {
            CollisionDetection(
              *this, entities, Core::GetNodeValue(*it1), *this, entities, Core::GetNodeValue(*it2), collidedEntities, tolerance, collisionDetector);
          }
        }
      }

      nodeContextStack.pop_back();
    }

    // Collision detection between the stored entities from bottom to top logic
    template<bool IS_PARALLEL_EXEC = false>
    std::vector<std::pair<EntityID, EntityID>> CollectCollidedEntities(
      EntityContainerView entities,
      TFloatScalar tolerance = EA::GEOMETRY_TYPE == GeometryType::Point ? GA::BASE_TOLERANCE : 0,
      std::optional<FCollisionDetector> const& collisionDetector = std::nullopt) const noexcept
    {
      auto const comparator = [&entities](EntityID entityID1, EntityID entityID2) {
        if constexpr (EA::GEOMETRY_TYPE == GeometryType::Point)
        {
          auto const x1 = GA::GetPointC(EA::GetGeometry(entities, entityID1), 0);
          auto const x2 = GA::GetPointC(EA::GetGeometry(entities, entityID2), 0);
          return x1 < x2 || (x1 == x2 && entityID1 < entityID2);
        }
        else if constexpr (EA::GEOMETRY_TYPE == GeometryType::Box)
        {
          auto const x1 = GA::GetBoxMinC(EA::GetGeometry(entities, entityID1), 0);
          auto const x2 = GA::GetBoxMinC(EA::GetGeometry(entities, entityID2), 0);
          return x1 < x2 || (x1 == x2 && entityID1 < entityID2);
        }
        else
        {
          static_assert(false, "Unsupported geometry type for collision detection!");
        }
      };

      auto const entityNo = entities.size();
      auto collidedEntities = std::vector<std::pair<EntityID, EntityID>>{};
      collidedEntities.reserve(std::max<std::size_t>(100, entityNo / 10));

      // non-parallel execution
      if constexpr (!IS_PARALLEL_EXEC)
      {
        auto nodeContextStack = std::vector<NodeCollisionContext>{};
        nodeContextStack.reserve(Core::GetDepthNo());
        InsertCollidedEntitiesInSubtree(
          entities, comparator, Core::GetNodeValue(Core::GetRootNodeID()), nodeContextStack, collidedEntities, tolerance, collisionDetector);

        return collidedEntities;
      }
      else
      {
        // maybe parallel execution

        auto const threadNum = uint32_t(std::thread::hardware_concurrency());
        auto const isSingleThreadMoreEffective = Core::GetNodeCount() < threadNum * 3;
        if (isSingleThreadMoreEffective)
        {
          auto nodeContextStack = std::vector<NodeCollisionContext>();
          nodeContextStack.reserve(Core::GetDepthNo());
          InsertCollidedEntitiesInSubtree(
            entities, comparator, Core::GetNodeValue(Core::GetRootNodeID()), nodeContextStack, collidedEntities, tolerance, collisionDetector);

          return collidedEntities;
        }

        constexpr uint32_t INVALID_INDEX = std::numeric_limits<uint32_t>::max();
        struct NodeData
        {
          NodeValue nodeValue;
          uint32_t parentID = INVALID_INDEX;
          uint32_t contextID = INVALID_INDEX;
        };

        auto nodeQueue = std::vector<NodeData>{};
        nodeQueue.reserve(threadNum * 2);
        nodeQueue.push_back(NodeData{ Core::GetNodeValue(Core::GetRootNodeID()), INVALID_INDEX, INVALID_INDEX });

        auto nodeContextMap = std::vector<NodeCollisionContext>{};

        uint32_t nodeQueueNum = 1;
        uint32_t childNodeAddedParentNum = 0;
        for (uint32_t i = 0; 0 < nodeQueueNum && nodeQueueNum < threadNum - 2; --nodeQueueNum, ++i)
        {
          ++childNodeAddedParentNum;
          auto& [nodeValue, parentID, contextID] = nodeQueue[i];
          contextID = static_cast<uint32_t>(nodeContextMap.size());
          auto& nodeContext = nodeContextMap.emplace_back();
          FillNodeCollisionContext(nodeValue, nodeContext, comparator);

          for (NodeIDCR childNodeID : Core::GetNodeChildren(nodeValue))
          {
            nodeQueue.push_back(NodeData{ Core::GetNodeValue(childNodeID), i, INVALID_INDEX });
            ++nodeQueueNum;
          }
        }

        if (nodeQueueNum < threadNum)
        {
          auto nodeContextStack = std::vector<NodeCollisionContext>();
          nodeContextStack.reserve(Core::GetDepthNo());
          InsertCollidedEntitiesInSubtree(
            entities, comparator, Core::GetNodeValue(Core::GetRootNodeID()), nodeContextStack, collidedEntities, tolerance, collisionDetector);

          return collidedEntities;
        }

        // parallel execution

        struct TaskContext
        {
          NodeValue nodeValue;
          NodeValue nodeValueSecondary;
          uint32_t parentID;
          uint32_t contextID;
          std::vector<std::pair<EntityID, EntityID>> collidedEntities;
        };

        auto const nodeQueueAllNo = nodeQueue.size();
        auto const nodeQueueBegin = nodeQueueAllNo - nodeQueueNum;
        auto taskContexts = std::vector<TaskContext>(nodeQueueNum);
        for (std::size_t taskID = 0; taskID < nodeQueueNum; ++taskID)
        {
          auto const& [nodeValue, parentID, contextID] = nodeQueue[nodeQueueBegin + taskID];
          taskContexts[taskID].nodeValue = nodeValue;
          taskContexts[taskID].parentID = parentID;
          taskContexts[taskID].contextID = contextID;
        }

        EXEC_POL_DEF(epcd); // GCC 11.3
        std::for_each(EXEC_POL_ADD(epcd) taskContexts.begin(), taskContexts.end(), [&](auto& taskContext) {
          auto const& [nodeValue, nvs_, parentID, contextID, ce_] = taskContext;

          auto nodeContextStack = std::vector<NodeCollisionContext>();
          nodeContextStack.reserve(Core::GetDepthNo());

          auto parentID_ = parentID;
          while (parentID_ != INVALID_INDEX)
          {
            auto const& parentNodeData = nodeQueue[parentID_];
            nodeContextStack.push_back(nodeContextMap[parentNodeData.contextID]);
            parentID_ = parentNodeData.parentID;
          }

          std::ranges::reverse(nodeContextStack);
          InsertCollidedEntitiesInSubtree(entities, comparator, nodeValue, nodeContextStack, taskContext.collidedEntities, tolerance, collisionDetector);
        });

        if constexpr (CONFIG::LOOSE_FACTOR > 1.0)
        {
          for (std::size_t i = 0; i < childNodeAddedParentNum; ++i)
          {
            auto const& childNodeIDs = Core::GetNodeChildren(nodeQueue[i].nodeValue);
            for (auto it1 = childNodeIDs.begin(); it1 != childNodeIDs.end(); ++it1)
            {
              for (auto it2 = it1 + 1; it2 != childNodeIDs.end(); ++it2)
              {
                taskContexts.push_back({ .nodeValue = Core::GetNodeValue(*it1), .nodeValueSecondary = Core::GetNodeValue(*it2) });
              }
            }
          }

          EXEC_POL_DEF(epst); // GCC 11.3
          std::for_each(EXEC_POL_ADD(epst) taskContexts.begin() + nodeQueueNum, taskContexts.end(), [&](auto& taskContext) {
            CollisionDetection(
              *this, entities, taskContext.nodeValue, *this, entities, taskContext.nodeValueSecondary, taskContext.collidedEntities, tolerance, collisionDetector);
          });
        }
        auto collidedEntitiesInParents = std::vector<std::pair<EntityID, EntityID>>{};
        auto nodeContextStack = std::vector<NodeCollisionContext>();
        auto usedContextsStack = std::vector<NodeCollisionContext*>{};
        std::for_each(nodeQueue.begin(), nodeQueue.end() - nodeQueueNum, [&](auto const& nodeData) {
          {
            usedContextsStack.emplace_back(&nodeContextMap[nodeData.contextID]);
            auto parentID = nodeData.parentID;
            while (parentID != INVALID_INDEX)
            {
              auto const& parentData = nodeQueue[parentID];
              usedContextsStack.emplace_back(&nodeContextMap[parentData.contextID]);
              parentID = parentData.parentID;
            }

            for (auto it = usedContextsStack.rbegin(); it != usedContextsStack.rend(); ++it)
              nodeContextStack.emplace_back(std::move(*(*it)));
          }

          InsertCollidedEntitiesInsideNode(entities, nodeContextStack.back(), collidedEntitiesInParents, tolerance, collisionDetector);
          InsertCollidedEntitiesWithParents(entities, nodeContextStack, collidedEntitiesInParents, tolerance, collisionDetector);

          {
            auto i = 0;
            for (auto it = usedContextsStack.rbegin(); it != usedContextsStack.rend(); ++it, ++i)
              *(*it) = std::move(nodeContextStack[i]);
            usedContextsStack.clear();
            nodeContextStack.clear();
          }
        });

        auto const collisionNo =
          std::transform_reduce(taskContexts.begin(), taskContexts.end(), collidedEntitiesInParents.size(), std::plus{}, [](auto const& taskContext) {
            return taskContext.collidedEntities.size();
          });

        collidedEntities.reserve(collisionNo);
        collidedEntities.insert(collidedEntities.end(), collidedEntitiesInParents.begin(), collidedEntitiesInParents.end());
        for (auto const& taskContext : taskContexts)
          collidedEntities.insert(collidedEntities.end(), taskContext.collidedEntities.begin(), taskContext.collidedEntities.end());

        return collidedEntities;
      }
    }

  public:
    // Collision detection between the stored entities from bottom to top logic
    template<bool IS_PARALLEL_EXEC = false>
    std::vector<std::pair<EntityID, EntityID>> CollisionDetection(EntityContainerView entities, TFloatScalar tolerance = GA::BASE_TOLERANCE) const noexcept
    {
      return CollectCollidedEntities<IS_PARALLEL_EXEC>(entities, tolerance, std::nullopt);
    }

    // Collision detection between the stored entities from bottom to top logic
    template<bool IS_PARALLEL_EXEC = false>
    std::vector<std::pair<EntityID, EntityID>> CollisionDetection(
      EntityContainerView entities, FCollisionDetector&& collisionDetector, TFloatScalar tolerance = GA::BASE_TOLERANCE) const noexcept
    {
      return CollectCollidedEntities<IS_PARALLEL_EXEC>(entities, tolerance, collisionDetector);
    }

  public:
    // Get all entities that are intersected by the ray in order
    //
    // Accepted entityDistanceFn signatures:
    // * bool(EntityID)
    // * bool(EntityID, TVector rayBasePoint, TVector rayHeading)
    // * bool(Entity)
    // * bool(Entity, TVector rayBasePoint, TVector rayHeading)
    // * std::optional<TFloatScalar>(EntityID)
    // * std::optional<TFloatScalar>(EntityID, TVector rayBasePoint, TVector rayHeading)
    // * std::optional<TFloatScalar>(Entity)
    // * std::optional<TFloatScalar>(Entity, TVector rayBasePoint, TVector rayHeading)
    template<bool SHOULD_SORT_ENTITIES_BY_DISTANCE = true, typename TEntityRayHitTester = std::monostate>
    std::vector<EntityID> RayIntersectedAll(
      TVector const& rayBasePoint,
      TVector const& rayHeading,
      EntityContainerView entities,
      TFloatScalar tolerance = GA::BASE_TOLERANCE,
      TFloatScalar toleranceIncrement = {},
      TScalar maxExaminationDistance = std::numeric_limits<TScalar>::max(),
      TEntityRayHitTester&& entityRayHitTester = {}) const noexcept
    {
      assert(
        toleranceIncrement <= std::max<TFloatScalar>(tolerance * 10, TFloatScalar(0.1)) &&
        "toleranceIncrement seems to be large. Probably maxExaminationDistance is used.");

      const auto rayHitTester = IGM::RayHitTester::Make(rayBasePoint, rayHeading, tolerance, toleranceIncrement);
      if (!rayHitTester)
        return {};

      struct ItemDistance
      {
        IGM_Geometry Distance;
        auto operator<=>(ItemDistance const& rhs) const = default;
      };

      struct EntityDistance : ItemDistance
      {
        EntityID EntityID;
        auto operator<=>(EntityDistance const& rhs) const { return ItemDistance::operator<=>(rhs); }
      };

      using EntityDistanceContainer = std::conditional_t<SHOULD_SORT_ENTITIES_BY_DISTANCE, std::vector<EntityDistance>, std::vector<EntityID>>;

      auto foundEntities = EntityDistanceContainer{};
      foundEntities.reserve(20);

      TraverseNodesDepthFirst([&](auto const nodeValue) {
        auto const nodeHit = rayHitTester->Hit(Core::GetNodeMinPoint(nodeValue), Core::GetNodeSize(nodeValue));
        if (!nodeHit)
          return TraverseControl::SkipChildren;

        for (auto const entityID : Core::GetNodeEntities(nodeValue))
        {
          auto const entityDistance = rayHitTester->Hit(EA::GetGeometry(entities, entityID));
          if (!entityDistance)
            continue;

          if (!((maxExaminationDistance == 0 || entityDistance->enterDistance <= maxExaminationDistance)))
            continue;

          auto closestEntityDistance = entityDistance->enterDistance;
          if constexpr (!std::is_same_v<std::monostate, TEntityRayHitTester>)
          {
            auto result = TestEntity(entityRayHitTester, entityID, entities, rayBasePoint, rayHeading);
            if (!result)
              continue;

            if constexpr (SHOULD_SORT_ENTITIES_BY_DISTANCE && detail::IsStdOptionalV<decltype(result)>)
            {
              closestEntityDistance = *result;
            }
          }

          if constexpr (SHOULD_SORT_ENTITIES_BY_DISTANCE)
            detail::insert(foundEntities, EntityDistance{ { closestEntityDistance }, entityID });
          else
            detail::insert(foundEntities, entityID);
        }

        return TraverseControl::Continue;
      });

      if constexpr (!SHOULD_SORT_ENTITIES_BY_DISTANCE)
      {
        return foundEntities;
      }
      else
      {
        std::ranges::sort(foundEntities);

        auto foundEntityIDs = std::vector<EntityID>(foundEntities.size());
        std::ranges::transform(foundEntities, foundEntityIDs.begin(), [](auto const& entityDistance) { return entityDistance.EntityID; });
        return foundEntityIDs;
      }
    }

    // Get all box which is intersected by the ray in order
    std::vector<EntityID> RayIntersectedAll(
      TRay const& ray,
      EntityContainerView entities,
      TFloatScalar tolerance = GA::BASE_TOLERANCE,
      TFloatScalar toleranceIncrement = {},
      TScalar maxExaminationDistance = std::numeric_limits<TScalar>::max()) const noexcept
    {
      return RayIntersectedAll(GA::GetRayOrigin(ray), GA::GetRayDirection(ray), entities, tolerance, toleranceIncrement, maxExaminationDistance);
    }

    // Get first entities that hit by the ray
    //
    // Accepted entityDistanceFn signatures:
    // * bool(EntityID)
    // * bool(EntityID, TVector rayBasePoint, TVector rayHeading)
    // * bool(Entity)
    // * bool(Entity, TVector rayBasePoint, TVector rayHeading)
    // * std::optional<TFloatScalar>(EntityID)
    // * std::optional<TFloatScalar>(EntityID, TVector rayBasePoint, TVector rayHeading)
    // * std::optional<TFloatScalar>(Entity)
    // * std::optional<TFloatScalar>(Entity, TVector rayBasePoint, TVector rayHeading)
    template<typename TEntityRayHitTester = std::monostate>
    std::vector<EntityID> RayIntersectedFirst(
      TVector const& rayBasePoint,
      TVector const& rayHeading,
      EntityContainerView entities,
      TFloatScalar tolerance = GA::BASE_TOLERANCE,
      TFloatScalar toleranceIncrement = {},
      TScalar maxDistance = std::numeric_limits<TScalar>::max(),
      TEntityRayHitTester&& entityRayHitTester = {}) const noexcept
    {
      const auto rayHitTester = IGM::RayHitTester::Make(rayBasePoint, rayHeading, tolerance, toleranceIncrement);
      if (!rayHitTester)
        return {};

      struct Candidate
      {
        EntityID entityID;
        TFloatScalar enterDistance;

        constexpr auto operator<=>(Candidate const& other) const noexcept { return enterDistance <=> other.enterDistance; }
      };

      const auto appliedTolerance = tolerance == 0 ? std::numeric_limits<TFloatScalar>::epsilon() : tolerance;

      // max-heap by enterDistance (largest enterDistance at top)
      auto maxDistanceHeap = std::priority_queue<Candidate, std::vector<Candidate>>{};
      auto maxExaminationDistance = TFloatScalar(maxDistance);
      TraverseNodesByPriority(
        [&, rayHitTester = *rayHitTester](const auto nodeValue, TFloatScalar nodeEnterDistance) {
          if (nodeEnterDistance > maxExaminationDistance)
            return TraverseControl::Terminate;

          for (auto const entityID : Core::GetNodeEntities(nodeValue))
          {
            auto pickDomain = rayHitTester.Hit(EA::GetGeometry(entities, entityID));
            if (!pickDomain)
              continue;

            if (pickDomain->enterDistance > maxExaminationDistance)
              continue;

            // furthest possible hit distance
            auto& [closestHitDistance, furthestPossibleHitDistance] = *pickDomain;
            if constexpr (!std::is_same_v<std::monostate, TEntityRayHitTester>)
            {
              auto result = TestEntity(entityRayHitTester, entityID, entities, rayBasePoint, rayHeading);
              if (!result)
                continue;

              if constexpr (detail::IsStdOptionalV<decltype(result)>)
              {
                assert(*result <= furthestPossibleHitDistance && "entityRayHitTester returned out of box result.");
                assert(*result >= closestHitDistance && "entityRayHitTester returned out of box result.");
                closestHitDistance = furthestPossibleHitDistance = *result;
              }
            }

            // push candidate
            maxDistanceHeap.push(Candidate{ entityID, closestHitDistance });

            // update bestExitDistance if this entity's exit is closer
            if (furthestPossibleHitDistance >= maxExaminationDistance)
              continue;

            maxExaminationDistance = (TScalar(1) + toleranceIncrement) * furthestPossibleHitDistance + appliedTolerance;

            // drop any candidates whose enterDistance > new maxExaminationDistance
            while (!maxDistanceHeap.empty() && maxDistanceHeap.top().enterDistance > maxExaminationDistance)
              maxDistanceHeap.pop();
          }

          return TraverseControl::Continue;
        },
        [&, rayHitTester = *rayHitTester](auto const nodeValue) -> std::optional<TFloatScalar> {
          auto result = rayHitTester.Hit(Core::GetNodeMinPoint(nodeValue), Core::GetNodeSize(nodeValue));
          if (!result)
            return std::nullopt;

          return result->enterDistance;
        });

      std::vector<EntityID> resulEntityIDs;
      resulEntityIDs.reserve(maxDistanceHeap.size());
      for (; !maxDistanceHeap.empty(); maxDistanceHeap.pop())
        resulEntityIDs.push_back(maxDistanceHeap.top().entityID);

      return resulEntityIDs;
    }

    // Get first entities that hit by the ray
    //
    // Accepted entityDistanceFn signatures:
    // * bool(EntityID)
    // * bool(EntityID, TVector)
    // * bool(Entity)
    // * bool(Entity, TVector)
    // * std::optional<TScalar>(EntityID)
    // * std::optional<TScalar>(EntityID, TVector)
    // * std::optional<TScalar>(Entity)
    // * std::optional<TScalar>(Entity, TVector)
    template<typename TEntityRayHitTester = std::monostate>
    std::vector<EntityID> RayIntersectedFirst(
      TRay const& ray,
      EntityContainerView entities,
      TFloatScalar tolerance = GA::BASE_TOLERANCE,
      TFloatScalar toleranceIncrement = {},
      TScalar maxDistance = std::numeric_limits<TScalar>::max(),
      TEntityRayHitTester&& entityHitTester = {}) const noexcept
    {
      return RayIntersectedFirst(
        GA::GetRayOrigin(ray), GA::GetRayDirection(ray), entities, tolerance, toleranceIncrement, maxDistance, std::forward<TEntityRayHitTester>(entityHitTester));
    }
  };

} // namespace OrthoTree
