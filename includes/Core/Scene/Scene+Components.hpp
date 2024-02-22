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
** File: Scene+Components.hpp                                                  *
** Project: Stellar-Engine                                                     *
** Created Date: Sa Feb 2024                                                   *
** Author: GlassAlo                                                            *
** Email: ofourpatat@gmail.com                                                 *
** -----                                                                       *
** Description: This file contains the implementation of the Scene with the    *
**              components                                                     *
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

#ifndef SCENE_COMPONENTS_HPP_
#define SCENE_COMPONENTS_HPP_

#include "Components/SparseArray.hpp"

#ifndef SCENE_HPP_
    #include "Scene.hpp"
#endif // !SCENE_HPP_

namespace Engine::Core {
    template<ComponentConcept Component>
    SparseArray<Component> &Scene::registerComponent()
    {
        auto typeIndex = std::type_index(typeid(Component));

        if (_components.find(typeIndex) != _components.end()) {
            throw SceneExceptionComponentAlreadyRegistered("Component already registered");
        }
        _components[typeIndex] = std::make_unique<SparseArray<Component>>();

        for (std::size_t idx = 0; idx < _nextId; idx++) {
            _components[typeIndex]->init(idx);
        }

        return static_cast<SparseArray<Component> &>(*_components[typeIndex]);
    }

    template<ComponentConcept... Component>
    void Scene::registerComponents()
    {
        (registerComponent<Component>(), ...);
    }

    template<ComponentConcept Component>
    SparseArray<Component> &Scene::getComponent()
    {
        auto typeIndex = std::type_index(typeid(Component));

        if (_components.find(typeIndex) == _components.end()) {
            throw SceneExceptionComponentNotRegistered("Component not registered");
        }
        return static_cast<SparseArray<Component> &>(*_components[typeIndex]);
    }

    template<ComponentConcept Component>
    SparseArray<Component> const &Scene::getComponent() const
    {
        auto typeIndex = std::type_index(typeid(Component));

        if (_components.find(typeIndex) == _components.end()) {
            throw SceneExceptionComponentNotRegistered("Component not registered");
        }
        return static_cast<SparseArray<Component> const &>(*_components.at(typeIndex));
    }

    template<ComponentConcept... Components>
    [[nodiscard]] bool Scene::hasComponents(std::size_t aIndex) const
    {
        return (... && getComponent<Components>().has(aIndex));
    }

    template<ComponentConcept Component>
    void Scene::removeComponent()
    {
        auto typeIndex = std::type_index(typeid(Component));

        if (_components.find(typeIndex) == _components.end()) {
            throw SceneExceptionComponentNotRegistered("Component not registered");
        }
        _components.erase(typeIndex);
    }

    template<ComponentConcept Component>
    Component &Scene::addComponentToEntity(std::size_t aIndex, Component &&aComponent)
    {
        auto &component = getComponent<Component>();

        component.set(aIndex, std::forward<Component>(aComponent));
        return component.get(aIndex);
    }

    template<ComponentConcept Component, typename... Args>
    Component &Scene::emplaceComponentToEntity(std::size_t aIndex, Args &&...aArgs)
    {
        auto &component = getComponent<Component>();

        component.emplace(aIndex, std::forward<Args>(aArgs)...);
        return component.get(aIndex);
    }

    template<ComponentConcept Component>
    void Scene::removeComponentFromEntity(std::size_t aIndex)
    {
        auto &component = getComponent<Component>();

        component.erase(aIndex);
    }
} // namespace Engine::Core

#endif /* !WORLD_COMPONENTS_HPP_ */
