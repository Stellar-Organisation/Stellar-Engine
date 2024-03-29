#ifndef WORLD_HPP_
#define WORLD_HPP_

#include <cstddef>
#include <functional>
#include <memory>
#include <tuple>
#include <typeindex>
#include <utility>
#include <vector>
#include "Components/SparseArray.hpp"
#include "Core/Components/Component.hpp"
#include "Exception.hpp"
#include "Systems/System.hpp"
#include <boost/container/flat_map.hpp>

namespace Engine::Core {
    DEFINE_EXCEPTION(WorldException);
    DEFINE_EXCEPTION_FROM(WorldExceptionComponentAlreadyRegistered, WorldException);
    DEFINE_EXCEPTION_FROM(WorldExceptionComponentNotRegistered, WorldException);
    DEFINE_EXCEPTION_FROM(WorldExceptionSystemAlreadyRegistered, WorldException);
    DEFINE_EXCEPTION_FROM(WorldExceptionSystemNotRegistered, WorldException);

    /**
     * @brief The world class represents a level, a scene
     * @details it contains the entities, components and systems used in the scene
     *
     */
    class World
    {
        public:
            using id = std::size_t;
            using container = std::unique_ptr<ISparseArray>;
            using containerMap = boost::container::flat_map<std::type_index, container>;
            using idsContainer = std::vector<id>;
            using systemFunc = std::unique_ptr<System>;
            using newSystemFunc = std::pair<std::string, std::unique_ptr<System>>;
            using systems = boost::container::flat_map<std::string, systemFunc>;

        protected:
            containerMap _components;
            idsContainer _ids;
            id _nextId = 0;
            systems _systems;

            template<ComponentConcept... Components>
            class Query
            {
                private:
                    std::reference_wrapper<Core::World> _world;

                public:
                    explicit Query(Core::World &world)
                        : _world(world)
                    {}

                    void
                    forEach(double deltaTime,
                            std::function<void(World &world, double deltaTime, std::size_t idx, Components &...)> func)
                    {
                        for (std::size_t idx = 0; idx < _world.get().getCurrentId(); idx++) {
                            if (_world.get().template hasComponents<Components...>(idx)) {
                                func(_world.get(), deltaTime, idx,
                                     _world.get().template getComponent<Components>().get(idx)...);
                            }
                        }
                    }

                    auto getAllEntities()
                    {
                        std::vector<std::size_t> entities;

                        for (std::size_t idx = 0; idx < _world.get().getCurrentId(); idx++) {
                            if (_world.get().template hasComponents<Components...>(idx)) {
                                entities.emplace_back(idx);
                            }
                        }
                        return entities;
                    }

                    auto getAll()
                    {
                        std::vector<std::tuple<std::size_t, Components &...>> entities;

                        for (std::size_t idx = 0; idx < _world.get().getCurrentId(); idx++) {
                            if (_world.get().template hasComponents<Components...>(idx)) {
                                entities.emplace_back(idx,
                                                      _world.get().template getComponent<Components>().get(idx)...);
                            }
                        }
                        return entities;
                    }

                    auto getComponentsOfEntity(std::size_t idx)
                    {
                        return std::make_tuple(_world.get().template getComponent<Components>().get(idx)...);
                    }
            };

        public:
#pragma region constructors / destructors
            World() = default;
            ~World() = default;

            World(const World &other) = default;
            World &operator=(const World &other) = default;

            World(World &&other) noexcept = default;
            World &operator=(World &&other) noexcept = default;
#pragma endregion constructors / destructors

#pragma region methods

            template<ComponentConcept... Components>
            Query<Components...> query()
            {
                return Query<Components...>(*this);
            }

            /**
             * @brief Add a component to the World, all components should be added before any entity is created
             *
             * @tparam Component Type of the component
             * @return SparseArray<Component>& Reference to the component SparseArray
             */
            template<ComponentConcept Component>
            SparseArray<Component> &registerComponent()
            {
                auto typeIndex = std::type_index(typeid(Component));

                if (_components.find(typeIndex) != _components.end()) {
                    throw WorldExceptionComponentAlreadyRegistered("Component already registered");
                }
                _components[typeIndex] = std::make_unique<SparseArray<Component>>();

                for (std::size_t idx = 0; idx < _nextId; idx++) {
                    _components[typeIndex]->init(idx);
                }

                return static_cast<SparseArray<Component> &>(*_components[typeIndex]);
            }

            /**
             * @brief Add multiple components to the World
             *
             * @tparam Components The components to add
             */
            template<ComponentConcept... Components>
            void registerComponents()
            {
                (registerComponent<Components>(), ...);
            }

            /**
             * @brief Get the Component object
             *
             * @tparam Component The type of the component
             * @return SparseArray<Component>& the SparseArray of the component
             */
            template<ComponentConcept Component>
            SparseArray<Component> &getComponent()
            {
                auto typeIndex = std::type_index(typeid(Component));

                if (_components.find(typeIndex) == _components.end()) {
                    throw WorldExceptionComponentNotRegistered("Component not registered");
                }
                return static_cast<SparseArray<Component> &>(*_components[typeIndex]);
            }

            /**
             * @brief Get the Component object
             *
             * @tparam Component The type of the component
             * @return SparseArray<Component>& the SparseArray of the component
             */
            template<ComponentConcept Component>
            SparseArray<Component> const &getComponent() const
            {
                auto typeIndex = std::type_index(typeid(Component));

                if (_components.find(typeIndex) == _components.end()) {
                    throw WorldExceptionComponentNotRegistered("Component not registered");
                }
                return static_cast<SparseArray<Component> const &>(*_components.at(typeIndex));
            }

            /**
             * @brief Check if the entity has all the components
             *
             * @tparam Components The components to check
             * @param aIndex The index of the entity
             * @return true if the entity has all the components
             * @return false if the entity doesn't have all the components
             */
            template<ComponentConcept... Components>
            [[nodiscard]] bool hasComponents(std::size_t aIndex) const
            {
                return (... && getComponent<Components>().has(aIndex));
            }

            /**
             * @brief Remove a component
             *
             * @tparam Component The type of the component
             */
            template<ComponentConcept Component>
            void removeComponent()
            {
                auto typeIndex = std::type_index(typeid(Component));

                if (_components.find(typeIndex) == _components.end()) {
                    throw WorldExceptionComponentNotRegistered("Component not registered");
                }
                _components.erase(typeIndex);
            }

            /**
             * @brief Add a component to an entity
             *
             * @tparam Component The type of the component to add
             * @param aIndex The index of the entity
             * @param aComponent The component to add
             * @return Component& The component added
             */
            template<ComponentConcept Component>
            Component &addComponentToEntity(std::size_t aIndex, Component &&aComponent)
            {
                try {
                    auto &component = getComponent<Component>();

                    component.set(aIndex, std::forward<Component>(aComponent));
                    return component.get(aIndex);
                } catch (WorldExceptionComponentNotRegistered &e) {
                    throw WorldExceptionComponentNotRegistered("Component not registered");
                }
            }

            /**
             * @brief Build and add a component to an entity
             *
             * @tparam Component The type of the component to add
             * @tparam Args The types of the arguments to pass to the component constructor (infered)
             * @param aIndex The index of the entity
             * @param aArgs The arguments to pass to the component constructor
             * @return Component& The component added
             */
            template<ComponentConcept Component, typename... Args>
            Component &emplaceComponentToEntity(std::size_t aIndex, Args &&...aArgs)
            {
                try {
                    auto &component = getComponent<Component>();

                    component.emplace(aIndex, std::forward<Args>(aArgs)...);
                    return component.get(aIndex);
                } catch (WorldExceptionComponentNotRegistered &e) {
                    throw WorldExceptionComponentNotRegistered("Component not registered");
                }
            }

            /**
             * @brief Remove a component from an entity
             *
             * @tparam Component The type of the component to remove
             * @param aIndex The index of the entity
             */
            template<ComponentConcept Component>
            void removeComponentFromEntity(std::size_t aIndex)
            {
                try {
                    auto &component = getComponent<Component>();

                    component.erase(aIndex);
                } catch (WorldExceptionComponentNotRegistered &e) {
                    throw WorldExceptionComponentNotRegistered("Component not registered");
                }
            }

            /**
             * @brief Kill an entity
             * @details Call erase from each component on the entity, then add the id as a free id
             * @param aIndex The index of the entity to kill
             */
            void killEntity(std::size_t aIndex);

            /**
             * @brief Create an entity
             * @details Get the next free id, call init from each component on the entity, then return the id
             * @return std::size_t The id of the entity
             */
            std::size_t createEntity();

            /**
             * @brief Add a system to the world
             *
             * @tparam Components The components the system will use
             * @tparam Function The type of the system (infered)
             * @param aFunction The system to add
             */
            void addSystem(newSystemFunc &aSystem)
            {
                if (_systems.find(aSystem.first) != _systems.end()) {
                    throw WorldExceptionSystemAlreadyRegistered("System already registered");
                }

                _systems[aSystem.first] = std::move(aSystem.second);
            }

            /**
             * @brief Remove a system from the world
             *
             * @tparam Function The type of the system (infered)
             * @param aFunction The system to remove
             */
            void removeSystem(std::string &aFuncName)
            {
                if (_systems.find(aFuncName) == _systems.end()) {
                    throw WorldExceptionSystemNotRegistered("System not registered");
                }

                _systems.erase(aFuncName);
            }

            /**
             * @brief Run all the systems once
             *
             */
            void runSystems();

            /**
             * @brief Get the Current Id object
             *
             * @return std::size_t The current id
             */
            [[nodiscard]] std::size_t getCurrentId() const;

        protected:
#pragma endregion methods
    };
} // namespace Engine::Core

#endif /* !WORLD_HPP_ */
