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

#pragma once

//////////////////////////////////////////////////////////////////////////
// Container type to make easier the OrthoTree usage
//////////////////////////////////////////////////////////////////////////


#include "adapters/general.h"
#include "core/types.h"
#include "detail/common.h"


#include <utility>


namespace OrthoTree
{
  template<typename TOrthoTreeCore>
  class OrthoTreeContainer
  {
  public:
    using CONFIG = typename TOrthoTreeCore::CONFIG;

    using GA = typename TOrthoTreeCore::GA;
    using TScalar = typename GA::Scalar;
    using TFloatScalar = typename GA::FloatScalar;
    using TVector = typename GA::Vector;
    using TBox = typename GA::Box;
    using TRay = typename GA::Ray;
    using TPlane = typename GA::Plane;

    using EA = typename TOrthoTreeCore::EA;
    using Entity = typename EA::Entity;
    using EntityID = typename EA::EntityID;
    using EntityContainer = EA::EntityContainer;

  protected:
    TOrthoTreeCore m_tree;
    EntityContainer m_entities;

  public: // Constructors
    constexpr explicit OrthoTreeContainer() noexcept = default;

    // Constructor for any contiguous container with runtime parallel parameter
    template<typename TExecMode = SeqExec>
    explicit OrthoTreeContainer(
      std::span<Entity const> const& geometryCollection,
      std::optional<depth_t> maxDepthID = std::nullopt,
      std::optional<TBox> boxSpace = std::nullopt,
      std::size_t maxElementNoInNode = CONFIG::DEFAULT_TARGET_ELEMENT_NUM_IN_NODES,
      TExecMode execMode = {}) noexcept
      requires(!EA::IS_ENTITY_KEYED)
    : m_entities(geometryCollection.begin(), geometryCollection.end())
    {
#ifndef __cpp_lib_execution
      static_assert(!std::is_same_v<TExecMode, ExecutionTags::Parallel>, "Parallel creation is based on execution policies. __cpp_lib_execution is required.");
#endif
      TOrthoTreeCore::Create(m_tree, m_entities, maxDepthID, std::move(boxSpace), maxElementNoInNode, execMode);
    }

    // Constructor for any copyable container with runtime parallel parameter
    template<typename TExecMode = SeqExec>
    explicit OrthoTreeContainer(
      EntityContainer const& geometryCollection,
      std::optional<depth_t> maxDepthID = std::nullopt,
      std::optional<TBox> boxSpace = std::nullopt,
      std::size_t maxElementNoInNode = CONFIG::DEFAULT_TARGET_ELEMENT_NUM_IN_NODES,
      TExecMode execMode = {}) noexcept
    : m_entities(geometryCollection)
    {
#ifndef __cpp_lib_execution
      static_assert(!std::is_same_v<TExecMode, ExecutionTags::Parallel>, "Parallel creation is based on execution policies. __cpp_lib_execution is required.");
#endif
      TOrthoTreeCore::Create(m_tree, m_entities, maxDepthID, std::move(boxSpace), maxElementNoInNode, execMode);
    }

    // Constructor for any movable container with runtime parallel parameter
    template<typename TExecMode = SeqExec>
    explicit OrthoTreeContainer(
      EntityContainer&& geometryCollection,
      std::optional<depth_t> maxDepthID = std::nullopt,
      std::optional<TBox> boxSpace = std::nullopt,
      std::size_t maxElementNoInNode = CONFIG::DEFAULT_TARGET_ELEMENT_NUM_IN_NODES,
      TExecMode execMode = {}) noexcept
    : m_entities(std::move(geometryCollection))
    {
#ifndef __cpp_lib_execution
      static_assert(!std::is_same_v<TExecMode, ExecutionTags::Parallel>, "Parallel creation is based on execution policies. __cpp_lib_execution is required.");
#endif
      TOrthoTreeCore::Create(m_tree, m_entities, maxDepthID, std::move(boxSpace), maxElementNoInNode, execMode);
    }

    // Constructor for any contiguous container with compile-time parallel parameter
    template<typename TExecMode>
    explicit OrthoTreeContainer(
      TExecMode execMode,
      std::span<Entity const> const& geometryCollection,
      std::optional<depth_t> maxDepthID = std::nullopt,
      std::optional<TBox> boxSpace = std::nullopt,
      std::size_t maxElementNoInNode = CONFIG::DEFAULT_TARGET_ELEMENT_NUM_IN_NODES) noexcept
      requires(!EA::IS_ENTITY_KEYED)
    : m_entities(geometryCollection.begin(), geometryCollection.end())
    {
#ifdef __cpp_lib_execution
#else
      static_assert(!std::is_same_v<TExecMode, ExecutionTags::Parallel>, "Parallel creation is based on execution policies. __cpp_lib_execution is required.");
#endif
      TOrthoTreeCore::Create(m_tree, m_entities, maxDepthID, std::move(boxSpace), maxElementNoInNode, execMode);
    }

    // Constructor for any copyable container compile-time parallel parameter
    template<typename TExecMode>
    explicit OrthoTreeContainer(
      TExecMode execMode,
      EntityContainer const& geometryCollection,
      std::optional<depth_t> maxDepthID = std::nullopt,
      std::optional<TBox> boxSpace = std::nullopt,
      std::size_t maxElementNoInNode = CONFIG::DEFAULT_TARGET_ELEMENT_NUM_IN_NODES) noexcept
    : m_entities(geometryCollection)
    {
#ifdef __cpp_lib_execution
#else
      static_assert(!std::is_same_v<TExecMode, ExecutionTags::Parallel>, "Parallel creation is based on execution policies. __cpp_lib_execution is required.");
#endif
      TOrthoTreeCore::Create(m_tree, m_entities, maxDepthID, std::move(boxSpace), maxElementNoInNode, execMode);
    }

    // Constructor for any movable container with compile-time parallel parameter
    template<typename TExecMode>
    explicit OrthoTreeContainer(
      TExecMode execMode,
      EntityContainer&& geometryCollection,
      std::optional<depth_t> maxDepthID = std::nullopt,
      std::optional<TBox> boxSpace = std::nullopt,
      std::size_t maxElementNoInNode = CONFIG::DEFAULT_TARGET_ELEMENT_NUM_IN_NODES) noexcept
    : m_entities(std::move(geometryCollection))
    {
#ifdef __cpp_lib_execution
#else
      static_assert(!std::is_same_v<TExecMode, ExecutionTags::Parallel>, "Parallel creation is based on execution policies. __cpp_lib_execution is required.");
#endif
      TOrthoTreeCore::Create(m_tree, m_entities, maxDepthID, std::move(boxSpace), maxElementNoInNode, execMode);
    }

    template<typename TExecMode = SeqExec>
    static OrthoTreeContainer Create(
      std::span<Entity const> const& entities,
      depth_t maxDepthID = 0,
      std::optional<TBox> boxSpace = std::nullopt,
      std::size_t maxElementNoInNode = CONFIG::DEFAULT_TARGET_ELEMENT_NUM_IN_NODES,
      TExecMode execMode = {}) noexcept
      requires(!EA::IS_ENTITY_KEYED)
    {
      auto otc = OrthoTreeContainer();
      otc.m_entities = std::vector(entities.begin(), entities.end());
      TOrthoTreeCore::Create(otc.m_tree, otc.m_entities, maxDepthID, std::move(boxSpace), maxElementNoInNode, execMode);
      return otc;
    }

    template<typename TExecMode = SeqExec>
    static OrthoTreeContainer Create(
      EntityContainer const& entities,
      depth_t maxDepthID = 0,
      std::optional<TBox> boxSpace = std::nullopt,
      std::size_t maxElementNoInNode = CONFIG::DEFAULT_TARGET_ELEMENT_NUM_IN_NODES,
      TExecMode execMode = {}) noexcept
    {
      auto otc = OrthoTreeContainer();
      otc.m_entities = entities;
      TOrthoTreeCore::Create(otc.m_tree, otc.m_entities, maxDepthID, std::move(boxSpace), maxElementNoInNode, execMode);
      return otc;
    }

    template<typename TExecMode = SeqExec>
    static OrthoTreeContainer Create(
      EntityContainer&& entities,
      depth_t maxDepthID = 0,
      std::optional<TBox> boxSpace = std::nullopt,
      std::size_t maxElementNoInNode = CONFIG::DEFAULT_TARGET_ELEMENT_NUM_IN_NODES,
      TExecMode execMode = {}) noexcept
    {
      auto otc = OrthoTreeContainer();
      otc.m_entities = std::move(entities);
      TOrthoTreeCore::Create(otc.m_tree, otc.m_entities, maxDepthID, std::move(boxSpace), maxElementNoInNode, execMode);
      return otc;
    }

  public: // Member functions
    constexpr TOrthoTreeCore const& GetCore() const noexcept { return m_tree; }
    constexpr EntityContainer const& GetData() const noexcept { return m_entities; }

    constexpr void Init(TBox const& boxSpace, depth_t maxDepthID, std::size_t maxElementNoInNode = TOrthoTreeCore::DEFAULT_MAX_ELEMENT) noexcept
    {
      m_tree.Init(boxSpace, maxDepthID, maxElementNoInNode);
    }

    // Get entity by ID
    constexpr Entity const& Get(EntityID entityID) const noexcept { return EA::GetEntity(m_entities, entityID); }

    // Add entity with tree rebalancing
    template<typename TEntity = Entity>
    constexpr std::optional<EntityID> Add(TEntity&& newEntity, InsertionMode insertionMode = InsertionMode::Balanced) noexcept
    {
      auto const newEntityID = EA::Insert(m_entities, std::forward<decltype(newEntity)>(newEntity));
      bool isInserted = false;
      switch (insertionMode)
      {
      case InsertionMode::Balanced: isInserted = m_tree.Insert(newEntityID, EA::GetGeometry(m_entities, newEntityID), m_entities); break;

      case InsertionMode::LowestLeaf:
      case InsertionMode::ExistingLeaf:
        isInserted = m_tree.InsertIntoLeaf(newEntityID, EA::GetGeometry(m_entities, newEntityID), insertionMode);
        break;
      }

      if (!isInserted)
      {
        EA::Erase(m_entities, newEntityID);
        return std::nullopt;
      }

      return newEntityID;
    }

    // Add entity with tree rebalancing if it is not yet added
    template<typename TEntity = Entity>
    constexpr std::optional<EntityID> AddUnique(
      TEntity&& newEntity, TFloatScalar tolerance = GA::BASE_TOLERANCE, InsertionMode insertionMode = InsertionMode::Balanced) noexcept
    {
      auto const newEntityID = EA::Insert(m_entities, std::forward<decltype(newEntity)>(newEntity));
      if (!m_tree.InsertUnique(newEntityID, EA::GetGeometry(m_entities, newEntityID), m_entities, tolerance, insertionMode))
      {
        EA::Erase(m_entities, newEntityID);
        return std::nullopt;
      }

      return newEntityID;
    }

    // Replace entity to the changedEntity
    template<typename TEntity = Entity>
    constexpr bool Update(EntityID entityID, TEntity&& changedEntity, InsertionMode insertionMode = InsertionMode::Balanced) noexcept
      requires(!EA::IS_ENTITY_KEYED)
    {
      auto oldEntity = EA::Exchange(m_entities, entityID, std::forward<decltype(changedEntity)>(changedEntity));
      bool isInserted = false;
      switch (insertionMode)
      {
      case InsertionMode::Balanced:
        isInserted = m_tree.Update(entityID, EA::GetGeometry(oldEntity), EA::GetGeometry(m_entities, entityID), m_entities);
        break;

      case InsertionMode::ExistingLeaf:
      case InsertionMode::LowestLeaf:
        isInserted = m_tree.Update(entityID, EA::GetGeometry(oldEntity), EA::GetGeometry(m_entities, entityID), insertionMode);
      }

      // restore the original state
      if (!isInserted)
        EA::Exchange(m_entities, entityID, std::move(oldEntity));

      return isInserted;
    }

    // Replace entity to the changedEntity
    template<typename TEntity = Entity>
    constexpr bool Update(TEntity&& changedEntity, InsertionMode insertionMode = InsertionMode::Balanced) noexcept
      requires(EA::IS_ENTITY_KEYED)
    {
      auto const entityID = EA::GetEntityID(changedEntity);
      auto oldEntity = EA::Exchange(m_entities, entityID, std::forward<decltype(changedEntity)>(changedEntity));

      bool isInserted = false;
      switch (insertionMode)
      {
      case InsertionMode::Balanced:
        isInserted = m_tree.Update(entityID, EA::GetGeometry(oldEntity), EA::GetGeometry(m_entities, entityID), m_entities);
        break;

      case InsertionMode::ExistingLeaf:
      case InsertionMode::LowestLeaf:
        isInserted = m_tree.Update(entityID, EA::GetGeometry(oldEntity), EA::GetGeometry(m_entities, entityID), insertionMode);
      }

      // restore the original state
      if (!isInserted)
        EA::Exchange(m_entities, entityID, std::move(oldEntity));

      return isInserted;
    }

    // Erase entity by ID
    bool Erase(EntityID entityID) noexcept
    {
      if (!EA::Contains(m_entities, entityID))
        return false;

      if (!m_tree.Erase(entityID, EA::GetGeometry(m_entities, entityID)))
        return false;

      EA::Erase(m_entities, entityID);
      return true;
    }

    // Clear the tree: remove all node and entity.
    void Clear() noexcept
    {
      m_tree.Clear();
      EA::Clear(m_entities);
    }

    // Reset the tree: Same as clear but also reset the handled domain
    void Reset() noexcept
    {
      m_tree.Reset();
      EA::Clear(m_entities);
    }

    template<typename TExecMode = SeqExec>
    void Move(TVector const& moveVector, TExecMode execMode = SEQ_EXEC) noexcept
    {
      m_tree.Move(moveVector, execMode);

      EXEC_POL_DEF(ep); // GCC 11.3
      std::for_each(EXEC_POL_ADD(ep) m_entities.begin(), m_entities.end(), [&moveVector](EA::Entity& entity) {
        if constexpr (EA::GEOMETRY_TYPE == GeometryType::Point)
        {
          EA::SetGeometry(entity, GA::Add(EA::GetGeometry(entity), moveVector));
        }
        else
        {
          auto box = EA::GetGeometry(entity);
          GA::MoveBox(box, moveVector);
          EA::SetGeometry(entity, box);
        }
      });
    }

    // Collect all entity ID in breadth-first traverse order
    std::vector<EntityID> GetEntitiesBreadthFirst() const noexcept { return m_tree.GetEntitiesBreadthFirst(); }

    // Collect all entity ID in depth-first traverse order
    std::vector<EntityID> GetEntitiesDepthFirst() const noexcept { return m_tree.GetEntitiesDepthFirst(); }

    // Visit entities in breadth first order
    template<typename TProcedure>
    constexpr void TraverseEntitiesBreadthFirst(TProcedure&& procedure) const noexcept
    {
      m_tree.TraverseEntitiesBreadthFirst(std::forward<TProcedure>(procedure));
    }

    // Visit entities in depth first order
    template<typename TProcedure>
    constexpr void TraverseEntitiesDepthFirst(TProcedure&& procedure) const noexcept
    {
      m_tree.TraverseEntitiesDepthFirst(std::forward<TProcedure>(procedure));
    }

    // Visit entities in priority order
    template<typename TProcedure, typename TPriorityCalculator>
    constexpr void TraverseEntitiesByPriority(TProcedure&& procedure, TPriorityCalculator&& priorityCalculator) const noexcept
    {
      m_tree.TraverseEntitiesByPriority(std::forward<TProcedure>(procedure), std::forward<TPriorityCalculator>(priorityCalculator));
    }

    // Pick search
    //
    // Accepted tester signatures (same as OrthoTreeQueryBase::PickSearch):
    // * bool(EntityID)
    // * bool(EntityID, TVector&)
    // * bool(Entity)
    // * bool(Entity, TVector&)
    template<typename TTester = std::monostate>
    std::vector<EntityID> PickSearch(TVector const& pickPoint, TFloatScalar tolerance = GA::BASE_TOLERANCE, TTester&& tester = {}) const noexcept
    {
      return m_tree.PickSearch(pickPoint, m_entities, tolerance, std::forward<TTester>(tester));
    }

    // Range search
    //
    // Accepted tester signatures (same as OrthoTreeQueryBase::RangeSearch):
    // * bool(EntityID)
    // * bool(EntityID, TVector&)
    // * bool(Entity)
    // * bool(Entity, TVector&)
    template<bool isFullyContained = true, typename TTester = std::monostate>
    std::vector<EntityID> RangeSearch(TBox const& range, TFloatScalar tolerance = GA::BASE_TOLERANCE, TTester&& tester = {}) const noexcept
    {
      return m_tree.template RangeSearch<isFullyContained>(range, m_entities, tolerance, std::forward<TTester>(tester));
    }

    // Hyperplane segmentation, get all elements in positive side (Plane equation: dotProduct(planeNormal, point) = distanceOfOrigo)
    //
    // Accepted tester signatures (same as OrthoTreeQueryBase::PlanePositiveSegmentation):
    // * bool(EntityID)
    // * bool(EntityID, TScalar, TVector)
    // * bool(Entity)
    // * bool(Entity, TScalar, TVector)
    template<typename TTester = std::monostate>
    std::vector<EntityID> PlanePositiveSegmentation(
      TScalar distanceOfOrigo, TVector const& planeNormal, TFloatScalar tolerance = GA::BASE_TOLERANCE, TTester&& tester = {}) const noexcept
    {
      return m_tree.PlanePositiveSegmentation(distanceOfOrigo, planeNormal, m_entities, tolerance, std::forward<TTester>(tester));
    }

    // Hyperplane segmentation, get all elements in positive side (Plane equation: dotProduct(planeNormal, point) = distanceOfOrigo)
    //
    // Accepted tester signatures (same as OrthoTreeQueryBase::PlanePositiveSegmentation):
    // * bool(EntityID)
    // * bool(EntityID, TScalar, TVector)
    // * bool(Entity)
    // * bool(Entity, TScalar, TVector)
    template<typename TTester = std::monostate>
    std::vector<EntityID> PlanePositiveSegmentation(TPlane const& plane, TFloatScalar tolerance = GA::BASE_TOLERANCE, TTester&& tester = {}) const noexcept
    {
      return m_tree.PlanePositiveSegmentation(plane, m_entities, tolerance, std::forward<TTester>(tester));
    }

    // Hyperplane segmentation, get all elements in positive side (Plane equation: dotProduct(planeNormal, point) = distanceOfOrigo)
    //
    // Accepted tester signatures (same as OrthoTreeQueryBase::FrustumCulling):
    // * bool(EntityID)
    // * bool(EntityID, TScalar, TVector)
    // * bool(Entity)
    // * bool(Entity, TScalar, TVector)
    template<typename TTester = std::monostate>
    std::vector<EntityID> FrustumCulling(
      std::span<TPlane const> const& boundaryPlanes, TFloatScalar tolerance = GA::BASE_TOLERANCE, TTester&& tester = {}) const noexcept
    {
      return m_tree.FrustumCulling(boundaryPlanes, m_entities, tolerance, std::forward<TTester>(tester));
    }

    using FrustumCondition = TOrthoTreeCore::FrustumCondition;
    using RangeCondition = TOrthoTreeCore::RangeCondition;
    using PlaneIntersectionCondition = TOrthoTreeCore::PlaneIntersectionCondition;
    using EntityIDCondition = TOrthoTreeCore::EntityIDCondition;
    using EntityCondition = TOrthoTreeCore::EntityCondition;

    // Complex query with multiple conditions. The conditions are combined with logical AND by default, but can be switched to OR by template parameter.
    // See `FrustumCondition`/ `RangeCondition` / `PlaneIntersectionCondition`/ `EntityIDCondition`/ `EntityCondition` for the accepted condition signatures.
    template<bool IS_LOGICAL_OR_FILTERING = false>
    std::vector<EntityID> Query(auto const& conditions, TFloatScalar tolerance = GA::BASE_TOLERANCE) const noexcept
    {
      return m_tree.template Query<IS_LOGICAL_OR_FILTERING>(conditions, m_entities, tolerance);
    }

    // K Nearest Neighbor
    //
    // Accepted entityDistanceFn signatures (same as OrthoTreeQueryBase::GetNearestNeighbors):
    // * bool(EntityID)
    // * bool(EntityID, TVector)
    // * bool(Entity)
    // * bool(Entity, TVector)
    // * std::optional<TScalar>(EntityID)
    // * std::optional<TScalar>(EntityID, TVector)
    // * std::optional<TScalar>(Entity)
    // * std::optional<TScalar>(Entity, TVector)
    template<bool SHOULD_SORT_ENTITIES_BY_DISTANCE = true, typename TTester = std::monostate>
    std::vector<EntityID> GetNearestNeighbors(
      TVector const& pt,
      std::size_t k,
      TScalar maxDistanceWithin = std::numeric_limits<TScalar>::max(),
      TFloatScalar tolerance = GA::BASE_TOLERANCE,
      TTester&& entityDistanceFn = {}) const noexcept
    {
      return m_tree.template GetNearestNeighbors<SHOULD_SORT_ENTITIES_BY_DISTANCE>(
        pt, k, maxDistanceWithin, m_entities, tolerance, std::forward<TTester>(entityDistanceFn));
    }

  public: // Collision detection
    using FCollisionDetector = typename TOrthoTreeCore::FCollisionDetector;

    // Collision detection between the contained elements
    template<typename TExecMode = SeqExec>
    std::vector<std::pair<EntityID, EntityID>> CollisionDetection(
      TFloatScalar tolerance = GA::BASE_TOLERANCE,
      std::optional<FCollisionDetector> const& collisionDetector = std::nullopt,
      TExecMode execMode = {}) const noexcept
    {
      if (collisionDetector)
        return m_tree.CollisionDetection(m_entities, FCollisionDetector(*collisionDetector), tolerance, execMode);
      else
        return m_tree.template CollisionDetection<TExecMode>(m_entities, tolerance);
    }

    // Collision detection with another tree
    std::vector<std::pair<EntityID, EntityID>> CollisionDetection(
      OrthoTreeContainer const& otherTree,
      TFloatScalar tolerance = GA::BASE_TOLERANCE,
      std::optional<FCollisionDetector> const& collisionDetector = std::nullopt) const noexcept
    {
      return m_tree.CollisionDetection(m_tree, m_entities, otherTree.m_tree, otherTree.m_entities, tolerance, collisionDetector);
    }

    // Collision detection between trees
    static std::vector<std::pair<EntityID, EntityID>> CollisionDetection(
      OrthoTreeContainer const& leftTree,
      OrthoTreeContainer const& rightTree,
      TFloatScalar tolerance = GA::BASE_TOLERANCE,
      std::optional<FCollisionDetector> const& collisionDetector = std::nullopt) noexcept
    {
      return leftTree.CollisionDetection(rightTree, tolerance, collisionDetector);
    }


  public: // Ray intersection
    // Get all entities that are intersected by the ray in order
    //
    // Accepted entity ray-hit tester signatures (same as OrthoTreeQueryBase::RayIntersectedAll/First):
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
      TFloatScalar tolerance = GA::BASE_TOLERANCE,
      TFloatScalar toleranceIncrement = {},
      TScalar maxDistance = std::numeric_limits<TScalar>::max(),
      TEntityRayHitTester&& entityHitTester = {}) const noexcept
    {
      return m_tree.template RayIntersectedAll<SHOULD_SORT_ENTITIES_BY_DISTANCE>(
        rayBasePoint, rayHeading, m_entities, tolerance, toleranceIncrement, maxDistance, std::forward<TEntityRayHitTester>(entityHitTester));
    }

    // Get all entities that are intersected by the ray in order
    //
    // Accepted entity ray-hit tester signatures (same as OrthoTreeQueryBase::RayIntersectedAll/First):
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
      TRay const& ray,
      TFloatScalar tolerance = GA::BASE_TOLERANCE,
      TFloatScalar toleranceIncrement = {},
      TScalar maxDistance = std::numeric_limits<TScalar>::max(),
      TEntityRayHitTester&& entityHitTester = {}) const noexcept
    {
      return m_tree.template RayIntersectedAll<SHOULD_SORT_ENTITIES_BY_DISTANCE>(
        ray, m_entities, tolerance, toleranceIncrement, maxDistance, std::forward<TEntityRayHitTester>(entityHitTester));
    }

    // Get first entities that hit by the ray
    //
    // Accepted entity ray-hit tester signatures (same as OrthoTreeQueryBase::RayIntersectedAll/First):
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
      TFloatScalar tolerance = GA::BASE_TOLERANCE,
      TFloatScalar toleranceIncrement = {},
      TScalar maxDistance = std::numeric_limits<TScalar>::max(),
      TEntityRayHitTester&& entityHitTester = {}) const noexcept
    {
      return m_tree.RayIntersectedFirst(
        rayBasePoint, rayHeading, m_entities, tolerance, toleranceIncrement, maxDistance, std::forward<TEntityRayHitTester>(entityHitTester));
    }

    // Get first entities that hit by the ray
    //
    // Accepted entity ray-hit tester signatures (same as OrthoTreeQueryBase::RayIntersectedAll/First):
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
      TRay const& ray,
      TFloatScalar tolerance = GA::BASE_TOLERANCE,
      TFloatScalar toleranceIncrement = {},
      TScalar maxDistance = std::numeric_limits<TScalar>::max(),
      TEntityRayHitTester&& entityHitTester = {}) const noexcept
    {
      return m_tree.RayIntersectedFirst(ray, m_entities, tolerance, toleranceIncrement, maxDistance, std::forward<TEntityRayHitTester>(entityHitTester));
    }

  public: // Plane
    // Hyperplane intersection (Plane equation: dotProduct(planeNormal, point) = distanceOfOrigo)
    //
    // Accepted tester signatures (same as OrthoTreeQueryBase::PlaneSearch):
    // * bool(EntityID)
    // * bool(EntityID, TScalar, TVector)
    // * bool(Entity)
    // * bool(Entity, TScalar, TVector)
    template<typename TTester = std::monostate>
    std::vector<EntityID> PlaneSearch(
      TScalar distanceOfOrigo, TVector const& planeNormal, TFloatScalar tolerance = GA::BASE_TOLERANCE, TTester&& tester = {}) const noexcept
    {
      return m_tree.PlaneSearch(distanceOfOrigo, planeNormal, m_entities, tolerance, std::forward<TTester>(tester));
    }

    // Hyperplane intersection using built-in plane
    //
    // Accepted tester signatures (same as OrthoTreeQueryBase::PlaneSearch):
    // * bool(EntityID)
    // * bool(EntityID, TScalar, TVector)
    // * bool(Entity)
    // * bool(Entity, TScalar, TVector)
    template<typename TTester = std::monostate>
    std::vector<EntityID> PlaneSearch(TPlane const& plane, TFloatScalar tolerance = GA::BASE_TOLERANCE, TTester&& tester = {}) const noexcept
    {
      return m_tree.PlaneSearch(plane, m_entities, tolerance, std::forward<TTester>(tester));
    }

    // Hyperplane intersection using built-in plane
    //
    // Accepted tester signatures (same as OrthoTreeQueryBase::PlaneSearch):
    // * bool(EntityID)
    // * bool(EntityID, TScalar, TVector)
    // * bool(Entity)
    // * bool(Entity, TScalar, TVector)
    template<typename TTester = std::monostate>
    std::vector<EntityID> PlaneIntersection(
      TScalar distanceOfOrigo, TVector const& planeNormal, TFloatScalar tolerance = GA::BASE_TOLERANCE, TTester&& tester = {}) const noexcept
    {
      return m_tree.PlaneSearch(distanceOfOrigo, planeNormal, m_entities, tolerance, std::forward<TTester>(tester));
    }

    // Hyperplane intersection using built-in plane
    //
    // Accepted tester signatures (same as OrthoTreeQueryBase::PlaneSearch):
    // * bool(EntityID)
    // * bool(EntityID, TScalar, TVector)
    // * bool(Entity)
    // * bool(Entity, TScalar, TVector)
    template<typename TTester = std::monostate>
    std::vector<EntityID> PlaneIntersection(TPlane const& plane, TFloatScalar tolerance = GA::BASE_TOLERANCE, TTester&& tester = {}) const noexcept
    {
      return m_tree.PlaneSearch(plane, m_entities, tolerance, std::forward<TTester>(tester));
    }
  };

  template<dim_t DIMENSION_NO, typename TScalar = BaseGeometryType, bool IS_CONTIOGUOS_CONTAINER = true>
  using TreePointContainerND = OrthoTreeContainer<TreePointND<DIMENSION_NO, TScalar, IS_CONTIOGUOS_CONTAINER>>;

  template<dim_t DIMENSION_NO, bool IS_LOOSE_TREE = true, typename TScalar = BaseGeometryType, bool IS_CONTIOGUOS_CONTAINER = true>
  using TreeBoxContainerND = OrthoTreeContainer<TreeBoxND<DIMENSION_NO, IS_LOOSE_TREE, TScalar, IS_CONTIOGUOS_CONTAINER>>;

  template<dim_t DIMENSION_NO, typename TScalar, typename TEntityContainer>
  using TreePointContainerNDUD = OrthoTreeContainer<TreePointNDUD<DIMENSION_NO, TScalar, TEntityContainer>>;

  template<dim_t DIMENSION_NO, bool IS_LOOSE_TREE, typename TScalar, typename TEntityContainer>
  using TreeBoxContainerNDUD = OrthoTreeContainer<TreeBoxNDUD<DIMENSION_NO, IS_LOOSE_TREE, TScalar, TEntityContainer>>;

  // Dualtree for points
  using DualtreePointC = TreePointContainerND<1, BaseGeometryType>;

  // Dualtree for bounding boxes
  template<bool IS_LOOSE_TREE = true>
  using DualtreeBoxCs = TreeBoxContainerND<1, IS_LOOSE_TREE, BaseGeometryType>;
  using DualtreeBoxC = TreeBoxContainerND<1, true, BaseGeometryType>;

  // Quadtree for points
  using QuadtreePointC = TreePointContainerND<2, BaseGeometryType>;

  // Quadtree for bounding boxes
  template<bool IS_LOOSE_TREE = true>
  using QuadtreeBoxCs = TreeBoxContainerND<2, IS_LOOSE_TREE, BaseGeometryType>;
  using QuadtreeBoxC = TreeBoxContainerND<2, true, BaseGeometryType>;

  // Octree for points
  using OctreePointC = TreePointContainerND<3, BaseGeometryType>;

  // Octree for bounding boxes
  template<bool IS_LOOSE_TREE = true>
  using OctreeBoxCs = TreeBoxContainerND<3, IS_LOOSE_TREE, BaseGeometryType>;
  using OctreeBoxC = TreeBoxContainerND<3, true, BaseGeometryType>;


  // std::unordered_map-based Dualtree for points
  using DualtreePointMapC = TreePointContainerND<1, BaseGeometryType, false>;

  // std::unordered_map-based Dualtree for bounding boxes
  template<bool IS_LOOSE_TREE = true>
  using DualtreeBoxMapCs = TreeBoxContainerND<1, IS_LOOSE_TREE, BaseGeometryType, false>;
  using DualtreeBoxMapC = TreeBoxContainerND<1, true, BaseGeometryType, false>;

  // std::unordered_map-based Quadtree for points
  using QuadtreePointMapC = TreePointContainerND<2, BaseGeometryType, false>;

  // std::unordered_map-based Quadtree for bounding boxes
  template<bool IS_LOOSE_TREE = true>
  using QuadtreeBoxMapCs = TreeBoxContainerND<2, IS_LOOSE_TREE, BaseGeometryType, false>;
  using QuadtreeBoxMapC = TreeBoxContainerND<2, true, BaseGeometryType, false>;

  // std::unordered_map-based Octree for points
  using OctreePointMapC = TreePointContainerND<3, BaseGeometryType, false>;

  // std::unordered_map-based Octree for bounding boxes
  template<bool IS_LOOSE_TREE = true>
  using OctreeBoxMapCs = TreeBoxContainerND<3, IS_LOOSE_TREE, BaseGeometryType, false>;
  using OctreeBoxMapC = TreeBoxContainerND<3, true, BaseGeometryType, false>;

  // User-defined container-based Quadtree for points
  template<typename EntityContainer>
  using QuadtreePointUDMapC = TreePointContainerNDUD<2, BaseGeometryType, EntityContainer>;

  // User-defined container-based Quadtree for bounding boxes
  template<typename EntityContainer, bool IS_LOOSE_TREE = true>
  using QuadtreeBoxUDMapCs = TreeBoxContainerNDUD<2, IS_LOOSE_TREE, BaseGeometryType, EntityContainer>;
  template<typename EntityContainer>
  using QuadtreeBoxUDMapC = TreeBoxContainerNDUD<2, true, BaseGeometryType, EntityContainer>;

  // User-defined container-based Octree for points
  template<typename EntityContainer>
  using OctreePointUDMapC = TreePointContainerNDUD<3, BaseGeometryType, EntityContainer>;

  // User-defined container-based Octree for bounding boxes
  template<typename EntityContainer, bool IS_LOOSE_TREE = true>
  using OctreeBoxUDMapCs = TreeBoxContainerNDUD<3, IS_LOOSE_TREE, BaseGeometryType, EntityContainer>;
  template<typename EntityContainer>
  using OctreeBoxUDMapC = TreeBoxContainerNDUD<3, true, BaseGeometryType, EntityContainer>;
} // namespace OrthoTree
