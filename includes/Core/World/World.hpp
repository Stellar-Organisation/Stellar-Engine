#ifndef WORLD_HPP_
#define WORLD_HPP_

#include <cstddef>
#include <functional>
#include <memory>
#include <typeindex>
#include <vector>
#include "Components/SparseArray.hpp"
#include "Exception.hpp"
#include "Systems/System.hpp"
#include "Systems/SystemsManager.hpp"
#include <boost/container/flat_map.hpp>

namespace Engine::Core {
    //------------------- WORLD - EXCEPTIONS -------------------//
    DEFINE_EXCEPTION(WorldException);
    DEFINE_EXCEPTION_FROM(WorldExceptionComponentAlreadyRegistered, WorldException);
    DEFINE_EXCEPTION_FROM(WorldExceptionComponentNotRegistered, WorldException);
    DEFINE_EXCEPTION_FROM(WorldExceptionSystemAlreadyRegistered, WorldException);
    DEFINE_EXCEPTION_FROM(WorldExceptionSystemNotRegistered, WorldException);

    //------------------- WORLD CLASS -------------------//
    /**
     * @brief The world class represent a scene in the game, it store all the entities, the components and the systems
     * It also provide a way to query the entities and run the systems
     */
    class World final
    {
        public:
            //------------------- WORLD ALIAS -------------------//
            //------------------- WORLD ALIAS - COMPONENTS -------------------//
            using container = std::unique_ptr<ISparseArray>;
            using containerMap = boost::container::flat_map<std::type_index, container>;

            //------------------- WORLD ALIAS - ENTITIES -------------------//
            using id = std::size_t;
            using idsContainer = std::vector<id>;

            //------------------- WORLD ALIAS - SYSTEMS -------------------//
            using SystemPtr = Systems::SystemsManager::SystemPtr;
            using SystemName = Systems::SystemsManager::SystemName;
            using SystemPair = Systems::SystemsManager::SystemPair;

        protected:
            //------------------- WORLD ATTRIBUTES -------------------//
            //------------------- WORLD ATTRIBUTES - COMPONENTS -------------------//
            containerMap _components;

            //------------------- WORLD ATTRIBUTES - ENTITIES -------------------//
            idsContainer _ids;
            id _nextId = 0;

            //------------------- WORLD ATTRIBUTES - SYSTEMS -------------------//
            Systems::SystemsManager _systems;

            //------------------- QUERY CLASS -------------------//
            /**
             * @brief The Query class is a class that allow to query the entities or the components of the world
             *
             * @tparam Components The differnent components to query, must inherit from Component
             */
            template<ComponentConcept... Components>
            class Query
            {
                private:
                    //------------------- QUERY ATTRIBUTES -------------------//
                    std::reference_wrapper<Core::World> _world;

                public:
                    //------------------- QUERY CONSTRUCTOR -------------------//
                    /**
                     * @brief Construct a new Query object
                     *
                     * @param world The world to query
                     */
                    explicit Query(Core::World &world);

                    //------------------- QUERY METHODS -------------------//
                    /**
                     * @brief Call a function for each entity that has all the components
                     *
                     * @param deltaTime The time between each frame
                     * @param func The function to call for each entity, must take the world, the deltaTime, the index
                     * of the entity and the components as parameters and return
                     */
                    void
                    forEach(double deltaTime,
                            std::function<void(World &world, double deltaTime, std::size_t idx, Components &...)> func);

                    /**
                     * @brief Get all the entities that have all the components in the query
                     *
                     * @return auto A vector of the entities that have all the components
                     */
                    auto getAllEntities();

                    /**
                     * @brief Get all the entities and their components that follow the query
                     *
                     * @return auto A vector of tuple of the entities that have all the components and their components
                     */
                    auto getAll();

                    /**
                     * @brief Get the Components Of Entity object
                     *
                     * @param idx The index of the entity
                     * @return auto A tuple of the components of the entity
                     */
                    auto getComponentsOfEntity(std::size_t idx);
            };

        public:
            //------------------- WORLD CONSTRUCTOR - DESTRUCTOR -------------------//
            /**
             * @brief Construct a new World object
             */
            World() = default;
            /**
             * @brief Destroy the World object
             */
            ~World() = default;

            //------------------- WORLD COPY - MOVE CONSTRUCTOR / OPERATOR -------------------//
            World(const World &other) = default;
            World &operator=(const World &other) = default;

            World(World &&other) noexcept = default;
            World &operator=(World &&other) noexcept = default;

            //------------------- WORLD METHODS - QUERY -------------------//
            /**
             * @brief Create a query object
             *
             * @tparam Components The components to query
             * @return Query<Components...> The query object
             */
            template<ComponentConcept... Components>
            Query<Components...> query();

            //------------------- WORLD METHODS - COMPONENTS -------------------//
            /**
             * @brief Register a component to the world, it means the entities will be able to have this component
             *
             * @tparam Component The component to register to the world, must inherit from Component
             * @throw WorldExceptionComponentAlreadyRegistered If the component is already registered
             * @return SparseArray<Component> & The SparseArray of the component registered
             */
            template<ComponentConcept Component>
            SparseArray<Component> &registerComponent();

            /**
             * @brief Register multiple components to the world
             *
             * @tparam Components The components to register to the world, must inherit from Component
             */
            template<ComponentConcept... Components>
            void registerComponents();

            /**
             * @brief Get the component of the given type
             *
             * @tparam Component The type of the component to get, must inherit from Component
             * @throw WorldExceptionComponentAlreadyRegistered If the component is already registered
             * @return SparseArray<Component> & The SparseArray of the component
             */
            template<ComponentConcept Component>
            SparseArray<Component> &getComponent();

            /**
             * @brief Get the component of the given type
             *
             * @tparam Component The type of the component to get, must inherit from Component
             * @throw WorldExceptionComponentAlreadyRegistered If the component is already registered
             * @return SparseArray<Component> const & The SparseArray of the component
             */
            template<ComponentConcept Component>
            SparseArray<Component> const &getComponent() const;

            /**
             * @brief Check if the entity has all the components
             *
             * @tparam Components The components to check, must inherit from Component
             * @param aIndex The index of the entity to check
             * @return bool True if the entity has all the components, false otherwise
             */
            template<ComponentConcept... Components>
            [[nodiscard]] bool hasComponents(std::size_t aIndex) const;

            /**
             * @brief Remove a component from the world
             *
             * @tparam Component The component to remove, must inherit from Component
             * @throw WorldExceptionComponentAlreadyRegistered If the component is already registered
             */
            template<ComponentConcept Component>
            void removeComponent();

            /**
             * @brief Add a component to an entity
             *
             * @tparam Component The type of the component to add, must inherit from Component (should be inferred)
             * @param aIndex The entity to add the component to
             * @param aComponent The component to add to the entity
             * @throw WorldExceptionComponentNotRegistered If the component is not registered
             * @return Component & The component added to the entity
             */
            template<ComponentConcept Component>
            Component &addComponentToEntity(std::size_t aIndex, Component &&aComponent);

            /**
             * @brief Emplace a component to an entity
             *
             * @tparam Component The type of the component to add, must inherit from Component (should be inferred)
             * @param aIndex The entity to add the component to
             * @param aArgs The arguments to construct the component
             * @throw WorldExceptionComponentNotRegistered If the component is not registered
             * @return Component & The component added to the entity
             */
            template<ComponentConcept Component, typename... Args>
            Component &emplaceComponentToEntity(std::size_t aIndex, Args &&...aArgs);

            /**
             * @brief Remove a component from an entity
             *
             * @tparam Component The type of the component to remove, must inherit from Component
             * @param aIndex The entity to remove the component from
             */
            template<ComponentConcept Component>
            void removeComponentFromEntity(std::size_t aIndex);

            //------------------- WORLD METHODS - ENTITIES -------------------//
            /**
             * @brief Kill an entity
             * @details The id of the entity will be reusable, the components of the entity will be reinitialized
             * @param aIndex The index of the entity to kill
             */
            void killEntity(std::size_t aIndex);

            /**
             * @brief Create an entity
             * @details The id of the entity will be unique, the components of the entity will be initialized
             * @return std::size_t The index of the entity created
             */
            std::size_t createEntity();

            /**
             * @brief Get the next entity id
             * @details The next entity id is the id that will be given to the next entity created by the world
             * @return std::size_t The current entity id
             */
            [[nodiscard]] std::size_t getNextEntityId() const;

            //------------------- WORLD METHODS - SYSTEMS -------------------//
            /**
             * @brief Add a system to the world
             *
             * @param aSystem The system to add to the world
             * @throw WorldExceptionSystemAlreadyRegistered If the system is already registered
             */
            void addSystem(SystemPair &aSystem);

            /**
             * @brief Add a system to the world
             *
             * @param aName The name of the system to add
             * @param aSystem The system to add to the world
             * @throw WorldExceptionSystemAlreadyRegistered If the system is already registered
             */
            void addSystem(SystemName &aName, SystemPtr &aSystem);

            /**
             * @brief Remove a system from the world
             *
             * @param aFuncName The name of the system to remove
             * @throw WorldExceptionSystemNotRegistered If the system is not registered
             */
            void removeSystem(std::string &aFuncName);

            /**
             * @brief Run all the systems of the world
             */
            void runSystems();
    };
} // namespace Engine::Core

#ifndef WORLD_QUERY_HPP_
    #include "World+Query.hpp"
#endif // !

#ifndef WORLD_COMPONENTS_HPP_
    #include "World+Components.hpp"
#endif // !

#endif /* !WORLD_HPP_ */
