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
** File: GenericSystem.hpp                                                     *
** Project: Stellar-Engine                                                     *
** Created Date: We Feb 2024                                                   *
** Author: GlassAlo                                                            *
** Email: ofourpatat@gmail.com                                                 *
** -----                                                                       *
** Description: {Enter a description for the file}                             *
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

#ifndef GENERICSYSTEM_HPP_
#define GENERICSYSTEM_HPP_

#include <functional>
#include <utility>
#include "Core/Scene/Scene.hpp"
#include "System.hpp"

namespace Engine::Systems {
    template<typename Func, ComponentConcept... Components>
    class GenericSystem : public System
    {
        private:
            Func _updateFunc;

        public:
            GenericSystem(Core::Scene &aScene, Func aUpdateFunc)
                : System(aScene),
                  _updateFunc(aUpdateFunc)
            {}

            void update() override
            {
                double deltaTime = _clock.getElapsedTime();

                _world.get().template query<Components...>().forEach(deltaTime, _updateFunc);
            }
    };

    template<ComponentConcept... Components, typename Func>
    std::pair<std::string, std::unique_ptr<System>> createSystem(Core::Scene &aWorld, const std::string &aName,
                                                                 Func aUpdateFunc)
    {
        return std::pair<std::string, std::unique_ptr<System>>(
            std::make_pair(aName, std::make_unique<GenericSystem<Func, Components...>>(aWorld, aUpdateFunc)));
    }
} // namespace Engine::Systems

#endif /* !GENERICSYSTEM_HPP_ */
