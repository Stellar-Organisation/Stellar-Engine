/*
**    _____ _       _ _                   ______             _                 *
**   / ____| |     | | |                 |  ____|           (_)                *
**  | (___ | |_ ___| | | __ _ _ __ ______| |__   _ __   __ _ _ _ __   ___      *
**   \___ \| __/ _ \ | |/ _` | '__|______|  __| | '_ \ / _` | | '_ \ / _ \     *
**   ____) | ||  __/ | | (_| | |         | |____| | | | (_| | | | | |  __/     *
**  |_____/ \__\___|_|_|\__,_|_|         |______|_| |_|\__, |_|_| |_|\___|     *
**                                                      __/ |                  *
**                                                     |___/                   *
**                                                                             *
*
** File: Scene.hpp                                                             *
** Project: Stellar-Engine                                                     *
** Created Date: Sa Feb 2024                                                   *
** Author: GlassAlo                                                            *
** Email: ofourpatat@gmail.com                                                 *
** -----                                                                       *
** Description: This file contains the definition of Scene                     *
** -----                                                                       *
** Last Modified: Thu Feb 22 2024                                              *
** Modified By: GlassAlo                                                       *
** -----                                                                       *
** Copyright (c) 2024 Stellar-Organisation                                     *
** -----                                                                       *
** HISTORY:                                                                    *
** Date      	By	Comments                                                   *
** ----------	---	---------------------------------------------------------  *
*/

#ifndef SCENE_HPP_
#define SCENE_HPP_

#include <cstddef>
#include <functional>
#include <memory>
#include <typeindex>
#include <vector>
#include "Components/SparseArraysManager.hpp"
#include "Exception.hpp"
#include "Systems/SystemsManager.hpp"
#include <boost/container/flat_map.hpp>

namespace Engine::Core {
    //------------------- SCENE - EXCEPTIONS -------------------//
    DEFINE_EXCEPTION(SceneException);
    DEFINE_EXCEPTION_FROM(SceneExceptionComponentAlreadyRegistered, SceneException);
    DEFINE_EXCEPTION_FROM(SceneExceptionComponentNotRegistered, SceneException);
    DEFINE_EXCEPTION_FROM(SceneExceptionSystemAlreadyRegistered, SceneException);
    DEFINE_EXCEPTION_FROM(SceneExceptionSystemNotRegistered, SceneException);

    //------------------- SCENE CLASS -------------------//
    /**
     * @brief The scene class stores all the entities, the components and the systems
     * It also provides a way to query the entities and run the systems
     */
    class Scene final
    {
        public:
            //------------------- SCENE ALIAS -------------------//
            //------------------- SCENE ALIAS - COMPONENTS -------------------//
            using container = Components::SparseArraysManager::container;
            using containerMap = Components::SparseArraysManager::containerMap;

            //------------------- SCENE ALIAS - ENTITIES -------------------//
            using id = std::size_t;
            using idsContainer = std::vector<id>;

            //------------------- SCENE ALIAS - SYSTEMS -------------------//
            using SystemPtr = Systems::SystemsManager::SystemPtr;
            using SystemName = Systems::SystemsManager::SystemName;
            using SystemPair = Systems::SystemsManager::SystemPair;

        protected:
            //------------------- SCENE ATTRIBUTES -------------------//
            //------------------- SCENE ATTRIBUTES - COMPONENTS -------------------//
            Components::SparseArraysManager _components;

            //------------------- SCENE ATTRIBUTES - ENTITIES -------------------//
            idsContainer _ids;
            id _nextId = 0;

            //------------------- SCENE ATTRIBUTES - SYSTEMS -------------------//
            Systems::SystemsManager _systems;

            //------------------- QUERY CLASS -------------------//
            /**
             * @brief The Query class is a class that allow to query the entities or the components of the scene
             *
             * @tparam Components The differnent components to query, must inherit from Component
             */
            template<ComponentConcept... Components>
            class Query
            {
                private:
                    //------------------- QUERY ATTRIBUTES -------------------//
                    std::reference_wrapper<Core::Scene> _scene;

                public:
                    //------------------- QUERY CONSTRUCTOR -------------------//
                    /**
                     * @brief Construct a new Query object
                     *
                     * @param aScene The scene to query
                     */
                    explicit Query(Core::Scene &aScene);

                    //------------------- QUERY METHODS -------------------//
                    /**
                     * @brief Call a function for each entity that has all the components
                     *
                     * @param deltaTime The time between each frame
                     * @param func The function to call for each entity, must take the scene, the deltaTime, the index
                     * of the entity and the components as parameters and return
                     */
                    void forEach(
                        double aDeltaTime,
                        std::function<void(Scene &aScene, double aDeltaTime, std::size_t aIdx, Components &...)> aFunc);

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
                    auto getComponentsOfEntity(std::size_t aIdx);
            };

        public:
            //------------------- SCENE CONSTRUCTOR - DESTRUCTOR -------------------//
            /**
             * @brief Construct a new Scene object
             */
            Scene() = default;
            /**
             * @brief Destroy the Scene object
             */
            ~Scene() = default;

            //------------------- SCENE COPY - MOVE CONSTRUCTOR / OPERATOR -------------------//
            Scene(const Scene &aOther) = default;
            Scene &operator=(const Scene &aOther) = default;

            Scene(Scene &&aOther) noexcept = default;
            Scene &operator=(Scene &&aOther) noexcept = default;

            //------------------- SCENE METHODS - QUERY -------------------//
            /**
             * @brief Create a query object
             *
             * @tparam Components The components to query
             * @return Query<Components...> The query object
             */
            template<ComponentConcept... Components>
            Query<Components...> query();

            //------------------- SCENE METHODS - COMPONENTS -------------------//
            /**
             * @brief Register a component to the scene, it means the entities will be able to have this component
             *
             * @tparam Component The component to register to the scene, must inherit from Component
             * @throw SceneExceptionComponentAlreadyRegistered If the component is already registered
             * @return SparseArray<Component> & The SparseArray of the component registered
             */
            template<ComponentConcept Component>
            SparseArray<Component> &registerComponent();

            /**
             * @brief Register multiple components to the scene
             *
             * @tparam Components The components to register to the scene, must inherit from Component
             */
            template<ComponentConcept... Components>
            auto registerComponents();

            /**
             * @brief Get the component of the given type
             *
             * @tparam Component The type of the component to get, must inherit from Component
             * @throw SceneExceptionComponentAlreadyRegistered If the component is already registered
             * @return SparseArray<Component> & The SparseArray of the component
             */
            template<ComponentConcept Component>
            SparseArray<Component> &getComponent();

            /**
             * @brief Get the component of the given type
             *
             * @tparam Component The type of the component to get, must inherit from Component
             * @throw SceneExceptionComponentAlreadyRegistered If the component is already registered
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
             * @brief Remove a component from the scene
             *
             * @tparam Component The component to remove, must inherit from Component
             * @throw SceneExceptionComponentAlreadyRegistered If the component is already registered
             */
            template<ComponentConcept Component>
            void removeComponent();

            /**
             * @brief Add a component to an entity
             *
             * @tparam Component The type of the component to add, must inherit from Component (should be inferred)
             * @param aIndex The entity to add the component to
             * @param aComponent The component to add to the entity
             * @throw SceneExceptionComponentNotRegistered If the component is not registered
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
             * @throw SceneExceptionComponentNotRegistered If the component is not registered
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

            //------------------- Scene METHODS - ENTITIES -------------------//
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
             * @details The next entity id is the id that will be given to the next entity created by the scene
             * @return std::size_t The current entity id
             */
            [[nodiscard]] std::size_t getNextEntityId() const;

            //------------------- SCENE METHODS - SYSTEMS -------------------//
            /**
             * @brief Add a system to the scene
             *
             * @param aSystem The system to add to the scene
             * @throw SceneExceptionSystemAlreadyRegistered If the system is already registered
             */
            void addSystem(SystemPair &aSystem);

            /**
             * @brief Add a system to the scene
             *
             * @param aName The name of the system to add
             * @param aSystem The system to add to the scene
             * @throw SceneExceptionSystemAlreadyRegistered If the system is already registered
             */
            void addSystem(SystemName &aName, SystemPtr &aSystem);

            /**
             * @brief Remove a system from the scene
             *
             * @param aFuncName The name of the system to remove
             * @throw SceneExceptionSystemNotRegistered If the system is not registered
             */
            void removeSystem(std::string &aFuncName);

            /**
             * @brief Run all the systems of the scene
             */
            void runSystems();
    };
} // namespace Engine::Core

#ifndef SCENE_QUERY_HPP_
    #include "Scene+Query.hpp"
#endif // !

#ifndef SCENE_COMPONENTS_HPP_
    #include "Scene+Components.hpp"
#endif // !

#endif /* !SCENE_HPP_ */
