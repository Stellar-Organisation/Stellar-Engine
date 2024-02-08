
#ifndef WORLD_QUERY_HPP_
#define WORLD_QUERY_HPP_

#include <cstddef>
#include <functional>
#include <tuple>
#include <vector>
#include "Systems/System.hpp"
#include <boost/container/flat_map.hpp>

#ifndef WORLD_HPP_
    #include "World.hpp"
#endif // !WORLD_HPP

namespace Engine::Core {

    template<ComponentConcept... Components>
    World::Query<Components...> World::query()
    {
        return Query<Components...>(*this);
    }

    template<ComponentConcept... Components>
    World::Query<Components...>::Query(Core::World &world)
        : _world(world)
    {}

    template<ComponentConcept... Components>
    void World::Query<Components...>::forEach(
        double deltaTime, std::function<void(World &world, double deltaTime, std::size_t idx, Components &...)> func)
    {
        for (std::size_t idx = 0; idx < _world.get().getNextEntityId(); idx++) {
            if (_world.get().template hasComponents<Components...>(idx)) {
                func(_world.get(), deltaTime, idx, _world.get().template getComponent<Components>().get(idx)...);
            }
        }
    }

    template<ComponentConcept... Components>
    auto World::Query<Components...>::getAllEntities()
    {
        std::vector<std::size_t> entities;

        for (std::size_t idx = 0; idx < _world.get().getNextEntityId(); idx++) {
            if (_world.get().template hasComponents<Components...>(idx)) {
                entities.emplace_back(idx);
            }
        }
        return entities;
    }

    template<ComponentConcept... Components>
    auto World::Query<Components...>::getAll()
    {
        std::vector<std::tuple<std::size_t, Components &...>> entities;

        for (std::size_t idx = 0; idx < _world.get().getNextEntityId(); idx++) {
            if (_world.get().template hasComponents<Components...>(idx)) {
                entities.emplace_back(idx, _world.get().template getComponent<Components>().get(idx)...);
            }
        }
        return entities;
    }

    template<ComponentConcept... Components>
    auto World::Query<Components...>::getComponentsOfEntity(std::size_t idx)
    {
        return std::make_tuple(_world.get().template getComponent<Components>().get(idx)...);
    }
} // namespace Engine::Core

#endif /* !WORLD_QUERY_HPP_ */
