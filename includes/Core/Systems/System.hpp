#ifndef SYSTEM_HPP_
#define SYSTEM_HPP_
#include <functional>
#include "Core/Clock.hpp"

namespace Engine::Core {
    class World;
}

namespace Engine::Systems {
    //------------------- SYSTEM CLASS -------------------//
    /**
     * @brief The System class is the base class for all systems in the engine.
     * @details It's an abstract class that contains the basic structure of a system.
     * @details A system is a class that contains the logic of the game. It's used to update the game state.
     */
    class System
    {
        public:
            //------------------- SYSTEM PUBLIC ATTRIBUTES -------------------//
            bool _isActivated = true;

        protected:
            //------------------- SYSTEM ATTRIBUTES -------------------//
            std::reference_wrapper<Core::World> _world;
            Clock _clock;

        public:
            //------------------- SYSTEM CONSTRUCTORS - DESTRUCTOR -------------------//
            /**
             * @brief Construct a new System object
             * @param aWorld A reference to the world in which the system is going to be used.
             */
            explicit System(Core::World &aWorld)
                : _world(aWorld)
            {}
            /**
             * @brief Destroy the System object
             */
            virtual ~System() = default;

            //------------------- SYSTEM PUBLIC MEMBER FUNCTIONS -------------------//
            /**
             * @brief Update the system.
             */
            virtual void update() = 0;

            //------------------- SYSTEM COPY - MOVE CONSTRUCTOR / OPERATOR -------------------//
            System &operator=(const System &) = default;
            System &operator=(System &&) = default;

            System(const System &) = default;
            System(System &&) = default;
    };
} // namespace Engine::Systems

#endif /* !SYSTEM_HPP_ */
