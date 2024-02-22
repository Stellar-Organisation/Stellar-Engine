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
** File: Scene+Systems.cpp                                                     *
** Project: Stellar-Engine                                                     *
** Created Date: We Feb 2024                                                   *
** Author: GlassAlo                                                            *
** Email: ofourpatat@gmail.com                                                 *
** -----                                                                       *
** Description: This file contains the implementation of the Scene with the    *
**              systems                                                       *
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

#include <spdlog/spdlog.h>
#include "Scene/Scene.hpp"

namespace Engine::Core {
    void Scene::runSystems()
    {
        _systems.runSystems();
    }

    void Scene::addSystem(SystemName &aName, SystemPtr &aSystem)
    {
        try {
            _systems.addSystem(aName, aSystem);
        } catch (const Systems::SystemsManagerException &e) {
            spdlog::error("Error while adding system: {}", e.what());
        }
    }

    void Scene::addSystem(SystemPair &aSystem)
    {
        addSystem(aSystem.first, aSystem.second);
    }

    void Scene::removeSystem(std::string &aFuncName)
    {
        try {
            _systems.removeSystem(aFuncName);
        } catch (const Systems::SystemsManagerException &e) {
            spdlog::error("Error while removing system: {}", e.what());
        }
    }
} // namespace Engine::Core
