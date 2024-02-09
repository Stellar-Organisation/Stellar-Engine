/*
** EPITECH PROJECT, 2024
** Stellar-Engine
** File description:
** SystemsManager
*/

#include "SystemsManager.hpp"
#include "Systems/System.hpp"

namespace Engine::Systems {
    void SystemsManager::addSystem(SystemName &aName, SystemPtr &aSystem)
    {
        if (_systems.find(aName) != _systems.end()) {
            throw SystemAlreadyRegistered("System already registered");
        }
        _systems[aName] = std::move(aSystem);
    }

    void SystemsManager::addSystem(SystemPair &aSystem)
    {
        try {
            addSystem(aSystem.first, aSystem.second);
        } catch (const SystemAlreadyRegistered &e) {
            throw e;
        }
    }

    void SystemsManager::removeSystem(SystemName &aName)
    {
        if (_systems.find(aName) == _systems.end()) {
            throw SystemNotFound("System not found");
        }
        _systems.erase(aName);
    }

    void SystemsManager::runSystems()
    {
        for (auto &system : _systems) {
            if (system.second->_isActivated) {
                system.second->update();
            }
        }
    }
} // namespace Engine::Systems