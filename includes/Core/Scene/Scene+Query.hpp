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
** File: Scene+Query.hpp                                                       *
** Project: Stellar-Engine                                                     *
** Created Date: We Feb 2024                                                   *
** Author: GlassAlo                                                            *
** Email: ofourpatat@gmail.com                                                 *
** -----                                                                       *
** Description: This file contains the implementation of the Query subclass    *
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

#ifndef SCENE_QUERY_HPP_
#define SCENE_QUERY_HPP_

#include <cstddef>
#include <functional>
#include <tuple>
#include <vector>
#include <boost/container/flat_map.hpp>

#ifndef SCENE_HPP_
    #include "Scene.hpp"
#endif // !SCENE_HPP_

namespace Engine::Core {

    template<ComponentConcept... Components>
    Scene::Query<Components...> Scene::query()
    {
        return Query<Components...>(*this);
    }

    template<ComponentConcept... Components>
    Scene::Query<Components...>::Query(Core::Scene &aScene)
        : _scene(aScene)
    {}

    template<ComponentConcept... Components>
    void Scene::Query<Components...>::forEach(
        double deltaTime,
        std::function<void(Scene &aScene, double aDeltaTime, std::size_t aIdx, Components &...)> aFunc)
    {
        for (std::size_t idx = 0; idx < _scene.get().getNextEntityId(); idx++) {
            if (_scene.get().template hasComponents<Components...>(idx)) {
                aFunc(_scene.get(), deltaTime, idx, _scene.get().template getComponent<Components>().get(idx)...);
            }
        }
    }

    template<ComponentConcept... Components>
    auto Scene::Query<Components...>::getAllEntities()
    {
        std::vector<std::size_t> entities;

        for (std::size_t idx = 0; idx < _scene.get().getNextEntityId(); idx++) {
            if (_scene.get().template hasComponents<Components...>(idx)) {
                entities.emplace_back(idx);
            }
        }
        return entities;
    }

    template<ComponentConcept... Components>
    auto Scene::Query<Components...>::getAll()
    {
        std::vector<std::tuple<std::size_t, Components &...>> entities;

        for (std::size_t idx = 0; idx < _scene.get().getNextEntityId(); idx++) {
            if (_scene.get().template hasComponents<Components...>(idx)) {
                entities.emplace_back(idx, _scene.get().template getComponent<Components>().get(idx)...);
            }
        }
        return entities;
    }

    template<ComponentConcept... Components>
    auto Scene::Query<Components...>::getComponentsOfEntity(std::size_t aIdx)
    {
        return std::make_tuple(_scene.get().template getComponent<Components>().get(aIdx)...);
    }
} // namespace Engine::Core

#endif /* !SCENEQUERY_HPP_ */
