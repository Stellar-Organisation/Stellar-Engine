#ifndef SYSTEMSMANAGER_HPP_
#define SYSTEMSMANAGER_HPP_

#include <memory>
#include <string>
#include "Exception.hpp"
#include <boost/container/flat_map.hpp>

namespace Engine::Systems {
    class System;

    //------------------- SYSTEMS MANAGER EXCEPTION -------------------//
    DEFINE_EXCEPTION(SystemsManagerException);
    DEFINE_EXCEPTION_FROM(SystemNotFound, SystemsManagerException);
    DEFINE_EXCEPTION_FROM(SystemAlreadyRegistered, SystemsManagerException);

    //------------------- SYSTEMS MANAGER CLASS -------------------//
    /**
     * @brief Class that manages all the systems in a world.
     */
    class SystemsManager
    {
        public:
            //------------------- SYSTEMS MANAGER ALIAS -------------------//
            using SystemName = std::string;
            using SystemPtr = std::unique_ptr<System>;
            using SystemPair = std::pair<SystemName, SystemPtr>;
            using SystemList = boost::container::flat_map<SystemName, SystemPtr>;

        protected:
            //------------------- SYSTEMS MANAGER ATTRIBUTES -------------------//
            SystemList _systems;

        public:
            //------------------- SYSTEMS MANAGER CONSTRUCTORS - DESTRUCTOR -------------------//
            SystemsManager() = default;
            ~SystemsManager() = default;

            //------------------- SYSTEMS MANAGER COPY - MOVE CONSTRUCTOR / OPERATOR -------------------//
            SystemsManager &operator=(const SystemsManager &) = default;
            SystemsManager &operator=(SystemsManager &&) = default;

            SystemsManager(const SystemsManager &) = default;
            SystemsManager(SystemsManager &&) = default;

            //------------------- SYSTEMS MANAGER PUBLIC METHODS -------------------//
            /**
             * @brief Add a system to the manager.
             * @param aName The name of the system, as a string.
             * @param aSystem The system to add, as a unique pointer.
             */
            void addSystem(SystemName &aName, SystemPtr &aSystem);

            /**
             * @brief Add a system to the manager.
             * @param aSystem The system to add, as a pair of name and unique pointer.
             */
            void addSystem(SystemPair &aSystem);

            /**
             * @brief Remove a system from the manager.
             * @param aName The name of the system to remove, as a string.
             */
            void removeSystem(SystemName &aName);

            /**
             * @brief Run all the systems registered in the manager.
             */
            void runSystems();
    };
} // namespace Engine::Systems

#endif /* !SYSTEMSMANAGER_HPP_ */
