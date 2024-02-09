#include <spdlog/spdlog.h>
#include "World/World.hpp"

namespace Engine::Core {
    void World::runSystems()
    {
        _systems.runSystems();
    }

    void World::addSystem(SystemName &aName, SystemPtr &aSystem)
    {
        try {
            _systems.addSystem(aName, aSystem);
        } catch (const Systems::SystemsManagerException &e) {
            spdlog::error("Error while adding system: {}", e.what());
        }
    }

    void World::addSystem(SystemPair &aSystem)
    {
        addSystem(aSystem.first, aSystem.second);
    }

    void World::removeSystem(std::string &aFuncName)
    {
        try {
            _systems.removeSystem(aFuncName);
        } catch (const Systems::SystemsManagerException &e) {
            spdlog::error("Error while removing system: {}", e.what());
        }
    }
} // namespace Engine::Core
