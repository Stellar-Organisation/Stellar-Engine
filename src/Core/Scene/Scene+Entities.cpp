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
** File: Scene+Entities.cpp                                                    *
** Project: Stellar-Engine                                                     *
** Created Date: We Feb 2024                                                   *
** Author: GlassAlo                                                            *
** Email: ofourpatat@gmail.com                                                 *
** -----                                                                       *
** Description: This file contains the implementation of the Scene with the    *
**              entities                                                       *
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

#include <algorithm>
#include <cstddef>
#include <spdlog/spdlog.h>
#include "Scene/Scene.hpp"

namespace Engine::Core {
    std::size_t Scene::createEntity()
    {
        std::size_t newIdx = 0;

        if (_ids.empty()) {
            newIdx = _nextId;
            _nextId++;
        } else {
            const auto smallestIdx = std::min_element(_ids.begin(), _ids.end());

            _ids.erase(smallestIdx);
            newIdx = *smallestIdx;
        }
        spdlog::debug("Creating entity {}", newIdx);
        _components.init(newIdx);
        return newIdx;
    }

    void Scene::killEntity(std::size_t aIndex)
    {
        spdlog::debug("Killing entity {}", aIndex);
        _ids.push_back(aIndex);

        _components.erase(aIndex);
    }

    std::size_t Scene::getNextEntityId() const
    {
        return _nextId;
    }
} // namespace Engine::Core
