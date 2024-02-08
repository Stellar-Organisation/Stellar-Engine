#include "World/World.hpp"

namespace Engine::Core {
    void World::runSystems()
    {
        for (auto &system : _systems) {
            system.second->update();
        }
    }

    void World::addSystem(newSystemFunc &aSystem)
    {
        if (_systems.find(aSystem.first) != _systems.end()) {
            throw WorldExceptionSystemAlreadyRegistered("System already registered");
        }

        _systems[aSystem.first] = std::move(aSystem.second);
    }

    void World::removeSystem(std::string &aFuncName)
    {
        if (_systems.find(aFuncName) == _systems.end()) {
            throw WorldExceptionSystemNotRegistered("System not registered");
        }

        _systems.erase(aFuncName);
    }
} // namespace Engine::Core
