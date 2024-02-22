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
        return _components.registerComponent<Component>(_nextId);
    }

    template<ComponentConcept... Component>
    auto Scene::registerComponents()
    {
        return std::tuple<SparseArray<Component> &...> {registerComponent<Component>()...};
    }

    template<ComponentConcept Component>
    SparseArray<Component> &Scene::getComponent()
    {
        return _components.getComponent<Component>();
    }

    template<ComponentConcept Component>
    SparseArray<Component> const &Scene::getComponent() const
    {
        return _components.getComponent<Component>();
    }

    template<ComponentConcept... Components>
    [[nodiscard]] bool Scene::hasComponents(std::size_t aIndex) const
    {
        return (_components.hasComponents<Components>(aIndex) && ...);
    }

    template<ComponentConcept Component>
    void Scene::removeComponent()
    {
        _components.removeComponent<Component>();
    }

    template<ComponentConcept Component>
    Component &Scene::addComponentToEntity(std::size_t aIndex, Component &&aComponent)
    {
        return _components.addComponentToEntity<Component>(aIndex, std::forward<Component>(aComponent));
    }

    template<ComponentConcept Component, typename... Args>
    Component &Scene::emplaceComponentToEntity(std::size_t aIndex, Args &&...aArgs)
    {
        return _components.emplaceComponentToEntity<Component>(aIndex, std::forward<Args>(aArgs)...);
    }

    template<ComponentConcept Component>
    void Scene::removeComponentFromEntity(std::size_t aIndex)
    {
        _components.removeComponentFromEntity<Component>(aIndex);
    }
} // namespace Engine::Core

#endif /* !WORLD_COMPONENTS_HPP_ */
