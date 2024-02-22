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
** File: System.hpp                                                            *
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

#ifndef SYSTEM_HPP_
#define SYSTEM_HPP_
#include <functional>
#include "Core/Clock.hpp"

namespace Engine::Core {
    class Scene;
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
            std::reference_wrapper<Core::Scene> _world;
            Clock _clock;

        public:
            //------------------- SYSTEM CONSTRUCTORS - DESTRUCTOR -------------------//
            /**
             * @brief Construct a new System object
             * @param aWorld A reference to the world in which the system is going to be used.
             */
            explicit System(Core::Scene &aWorld)
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
