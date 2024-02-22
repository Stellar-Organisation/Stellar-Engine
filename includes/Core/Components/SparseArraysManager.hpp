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
** File: SparseArraysManager.hpp                                               *
** Project: Stellar-Engine                                                     *
** Created Date: Sa Feb 2024                                                   *
** Author: GlassAlo                                                            *
** Email: ofourpatat@gmail.com                                                 *
** -----                                                                       *
** Description: This file contains the SparseArraysManager, it's a class that  *
**              manages the sparse arrays and so the components                *
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

#ifndef SPARSEARRAYSMANAGER_HPP_
#define SPARSEARRAYSMANAGER_HPP_

#include <cstddef>
#include <memory>
#include <typeindex>
#include "Exception.hpp"
#include "SparseArray.hpp"
#include "boost/container/flat_map.hpp"

namespace Engine::Components {
    //------------------- SPARSE ARRAYS MANAGER - EXCEPTIONS -------------------//
    DEFINE_EXCEPTION(SparseArraysManagerException);
    DEFINE_EXCEPTION_FROM(SparseArraysManagerExceptionNotFound, SparseArraysManagerException);
    DEFINE_EXCEPTION_FROM(SparseArraysManagerExceptionAlreadyRegistered, SparseArraysManagerException);

    //------------------- SPARSE ARRAYS MANAGER CLASS -------------------//
    /**
     * @brief Class that manages all the sparse arrays in a scene.
     */
    class SparseArraysManager
    {
        public:
            //------------------- SPARSE ARRAYS MANAGER ALIAS -------------------//
            using container = std::unique_ptr<Core::ISparseArray>;
            using containerMap = boost::container::flat_map<std::type_index, container>;

        protected:
            //------------------- SPARSE ARRAYS MANAGER ATTRIBUTES -------------------//
            containerMap _sparseArrays;

        public:
            //------------------- SPARSE ARRAYS MANAGER CONSTRUCTORS - DESTRUCTOR -------------------//
            /**
             * @brief Construct a new Sparse Arrays Manager object
             */
            SparseArraysManager() = default;
            /**
             * @brief Destroy the Sparse Arrays Manager object
             */
            ~SparseArraysManager() = default;

            //------------------- SPARSE ARRAYS MANAGER COPY - MOVE CONSTRUCTOR / OPERATOR -------------------//
            SparseArraysManager &operator=(const SparseArraysManager &) = default;
            SparseArraysManager &operator=(SparseArraysManager &&) = default;

            SparseArraysManager(const SparseArraysManager &) = default;
            SparseArraysManager(SparseArraysManager &&) = default;

            //------------------- SPARSE ARRAYS MANAGER METHODS - REGISTER -------------------//
            /**
             * @brief Register a component to the scene, it means the entities will be able to have this component
             *
             * @tparam Component The component to register to the scene, must inherit from Component
             * @throw SparseArraysManagerExceptionAlreadyRegistered If the component is already registered
             * @return SparseArray<Component> & The SparseArray of the component registered
             */
            template<ComponentConcept Component>
            Core::SparseArray<Component> &registerComponent(std::size_t aNextEntityId)
            {
                auto typeIndex = std::type_index(typeid(Component));

                if (_sparseArrays.find(typeIndex) != _sparseArrays.end()) {
                    throw SparseArraysManagerExceptionAlreadyRegistered("Component already registered");
                }
                _sparseArrays[typeIndex] = std::make_unique<Core::SparseArray<Component>>();

                for (std::size_t idx = 0; idx < aNextEntityId; idx++) {
                    _sparseArrays[typeIndex]->init(idx);
                }

                return static_cast<Core::SparseArray<Component> &>(*_sparseArrays[typeIndex]);
            }

            //------------------- SPARSE ARRAYS MANAGER METHODS - INIT -------------------//
            /**
             * @brief Initialize the sparse arrays
             *
             * @param aNextEntityId The next entity id to initialize the sparse arrays
             */
            void init(std::size_t aNextEntityId)
            {
                for (const auto &component : _sparseArrays) {
                    component.second->init(aNextEntityId);
                }
            }

            //------------------- SPARSE ARRAYS MANAGER METHODS - ERASE -------------------//
            /**
             * @brief Erase an entity from the sparse arrays
             *
             * @param aIndex The index of the entity to erase
             */
            void erase(std::size_t aIndex)
            {
                for (const auto &component : _sparseArrays) {
                    component.second->erase(aIndex);
                }
            }
            //------------------- SPARSE ARRAYS MANAGER METHODS - GET -------------------//
            /**
             * @brief Get the component of the given type
             *
             * @tparam Component The type of the component to get, must inherit from Component
             * @throw SparseArraysManagerExceptionAlreadyRegistered If the component is already registered
             * @return SparseArray<Component> & The SparseArray of the component
             */
            template<ComponentConcept Component>
            Core::SparseArray<Component> &getComponent()
            {
                auto typeIndex = std::type_index(typeid(Component));

                if (_sparseArrays.find(typeIndex) == _sparseArrays.end()) {
                    throw SparseArraysManagerExceptionNotFound("Component not registered");
                }
                return static_cast<Core::SparseArray<Component> &>(*_sparseArrays[typeIndex]);
            }

            /**
             * @brief Get the component of the given type
             *
             * @tparam Component The type of the component to get, must inherit from Component
             * @throw SparseArraysManagerExceptionAlreadyRegistered If the component is already registered
             * @return SparseArray<Component> const & The SparseArray of the component
             */
            template<ComponentConcept Component>
            Core::SparseArray<Component> const &getComponent() const
            {
                auto typeIndex = std::type_index(typeid(Component));

                if (_sparseArrays.find(typeIndex) == _sparseArrays.end()) {
                    throw SparseArraysManagerExceptionNotFound("Component not registered");
                }
                return static_cast<Core::SparseArray<Component> const &>(*_sparseArrays.at(typeIndex));
            }

            //------------------- SPARSE ARRAYS MANAGER METHODS - HAS -------------------//
            /**
             * @brief Check if the entity has all the components
             *
             * @tparam Components The components to check, must inherit from Component
             * @param aIndex The index of the entity to check
             * @return bool True if the entity has all the components, false otherwise
             */
            template<ComponentConcept... Components>
            [[nodiscard]] bool hasComponents(std::size_t aIndex) const
            {
                return (... && getComponent<Components>().has(aIndex));
            }

            //------------------- SPARSE ARRAYS MANAGER METHODS - REMOVE -------------------//
            /**
             * @brief Remove a component from the scene
             *
             * @tparam Component The component to remove, must inherit from Component
             * @throw SparseArraysManagerExceptionAlreadyRegistered If the component is already registered
             */
            template<ComponentConcept Component>
            void removeComponent()
            {
                auto typeIndex = std::type_index(typeid(Component));

                if (_sparseArrays.find(typeIndex) == _sparseArrays.end()) {
                    throw SparseArraysManagerExceptionNotFound("Component not registered");
                }
                _sparseArrays.erase(typeIndex);
            }

            /**
             * @brief Remove a component from an entity
             *
             * @tparam Component The type of the component to remove, must inherit from Component
             * @param aIndex The entity to remove the component from
             */
            template<ComponentConcept Component>
            void removeComponentFromEntity(std::size_t aIndex)
            {
                auto &component = getComponent<Component>();

                component.erase(aIndex);
            }

            //------------------- SPARSE ARRAYS MANAGER METHODS - ADD -------------------//
            /**
             * @brief Add a component to an entity
             *
             * @tparam Component The type of the component to add, must inherit from Component (should be inferred)
             * @param aIndex The entity to add the component to
             * @param aComponent The component to add to the entity
             * @throw SparseArraysManagerExceptionNotFound If the component is not registered
             * @return Component & The component added to the entity
             */
            template<ComponentConcept Component>
            Component &addComponentToEntity(std::size_t aIndex, Component &&aComponent)
            {
                auto &component = getComponent<Component>();

                component.set(aIndex, std::forward<Component>(aComponent));
                return component.get(aIndex);
            }

            /**
             * @brief Emplace a component to an entity
             *
             * @tparam Component The type of the component to add, must inherit from Component (should be inferred)
             * @param aIndex The entity to add the component to
             * @param aArgs The arguments to construct the component
             * @throw SparseArraysManagerExceptionNotFound If the component is not registered
             * @return Component & The component added to the entity
             */
            template<ComponentConcept Component, typename... Args>
            Component &emplaceComponentToEntity(std::size_t aIndex, Args &&...aArgs)
            {
                auto &component = getComponent<Component>();

                component.emplace(aIndex, std::forward<Args>(aArgs)...);
                return component.get(aIndex);
            }
    };

} // namespace Engine::Components

#endif /* !SPARSEARRAYSMANAGER_HPP_ */
