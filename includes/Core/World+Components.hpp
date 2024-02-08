#ifndef WORLD_COMPONENTS_HPP_
#define WORLD_COMPONENTS_HPP_

#include "Components/SparseArray.hpp"

#ifndef WORLD_HPP_
    #include "World.hpp"
#endif // !WORLD_HPP

namespace Engine::Core {
    template<ComponentConcept Component>
    SparseArray<Component> &World::registerComponent()
    {
        auto typeIndex = std::type_index(typeid(Component));

        if (_components.find(typeIndex) != _components.end()) {
            throw WorldExceptionComponentAlreadyRegistered("Component already registered");
        }
        _components[typeIndex] = std::make_unique<SparseArray<Component>>();

        for (std::size_t idx = 0; idx < _nextId; idx++) {
            _components[typeIndex]->init(idx);
        }

        return static_cast<SparseArray<Component> &>(*_components[typeIndex]);
    }

    template<ComponentConcept... Component>
    void World::registerComponents()
    {
        (registerComponent<Component>(), ...);
    }

    template<ComponentConcept Component>
    SparseArray<Component> &World::getComponent()
    {
        auto typeIndex = std::type_index(typeid(Component));

        if (_components.find(typeIndex) == _components.end()) {
            throw WorldExceptionComponentNotRegistered("Component not registered");
        }
        return static_cast<SparseArray<Component> &>(*_components[typeIndex]);
    }

    template<ComponentConcept Component>
    SparseArray<Component> const &World::getComponent() const
    {
        auto typeIndex = std::type_index(typeid(Component));

        if (_components.find(typeIndex) == _components.end()) {
            throw WorldExceptionComponentNotRegistered("Component not registered");
        }
        return static_cast<SparseArray<Component> const &>(*_components.at(typeIndex));
    }

    template<ComponentConcept... Components>
    [[nodiscard]] bool World::hasComponents(std::size_t aIndex) const
    {
        return (... && getComponent<Components>().has(aIndex));
    }

    template<ComponentConcept Component>
    void World::removeComponent()
    {
        auto typeIndex = std::type_index(typeid(Component));

        if (_components.find(typeIndex) == _components.end()) {
            throw WorldExceptionComponentNotRegistered("Component not registered");
        }
        _components.erase(typeIndex);
    }

    template<ComponentConcept Component>
    Component &World::addComponentToEntity(std::size_t aIndex, Component &&aComponent)
    {
        try {
            auto &component = getComponent<Component>();

            component.set(aIndex, std::forward<Component>(aComponent));
            return component.get(aIndex);
        } catch (WorldExceptionComponentNotRegistered &e) {
            throw WorldExceptionComponentNotRegistered("Component not registered");
        }
    }

    template<ComponentConcept Component, typename... Args>
    Component &World::emplaceComponentToEntity(std::size_t aIndex, Args &&...aArgs)
    {
        try {
            auto &component = getComponent<Component>();

            component.emplace(aIndex, std::forward<Args>(aArgs)...);
            return component.get(aIndex);
        } catch (WorldExceptionComponentNotRegistered &e) {
            throw WorldExceptionComponentNotRegistered("Component not registered");
        }
    }

    template<ComponentConcept Component>
    void World::removeComponentFromEntity(std::size_t aIndex)
    {
        try {
            auto &component = getComponent<Component>();

            component.erase(aIndex);
        } catch (WorldExceptionComponentNotRegistered &e) {
            throw WorldExceptionComponentNotRegistered("Component not registered");
        }
    }
} // namespace Engine::Core

#endif /* !WORLD_COMPONENTS_HPP_ */
