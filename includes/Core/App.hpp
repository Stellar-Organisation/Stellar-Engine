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
** File: App.hpp                                                               *
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

#ifndef APP_HPP_
#define APP_HPP_

#include <memory>
#include "Core/Events/Event.hpp"
#include "Exception.hpp"
#include "Scene/Scene.hpp"
#include <boost/container/flat_map.hpp>

namespace Engine {

    DEFINE_EXCEPTION(AppException);
    DEFINE_EXCEPTION_FROM(AppExceptionOutOfRange, AppException);
    DEFINE_EXCEPTION_FROM(AppExceptionKeyNotFound, AppException);
    DEFINE_EXCEPTION_FROM(AppExceptionKeyAlreadyExists, AppException);

    template<typename T> // must not be a component or a system or an event
    concept KeyConcept = !ComponentConcept<T> && !Event::EventConcept<T>;

    template<KeyConcept Key = std::size_t>
    class App
    {
        public:
            using scene = std::unique_ptr<Core::Scene>;
            using scenes = boost::container::flat_map<Key, scene>;

        private:
            scenes _scenes;
            Key _currentScenes;

        public:
#pragma region constructors / destructors
            App() = default;
            ~App() = default;

            App(const App &other) = delete;
            App &operator=(const App &other) = delete;

            App(App &&other) noexcept = delete;
            App &operator=(App &&other) noexcept = delete;
#pragma endregion constructors / destructors

#pragma region operators
            scene &operator[](const Key &aKey)
            {
                if (_scenes.find(aKey) == _scenes.end()) {
                    throw AppExceptionKeyNotFound("The key doesn't exist");
                }
                return _scenes[aKey];
            }
#pragma endregion operators

#pragma region methods
            /**
             * @brief Add a level to the app
             *
             * @param key The key of the world
             * @param aWorld The world to add
             * @throw AppExceptionKeyAlreadyExists If the key already exists
             */
            scene &addWorld(const Key &aKey, scene &&aWorld)
            {
                if (_scenes.find(aKey) != _scenes.end()) {
                    throw AppExceptionKeyAlreadyExists("The key already exists");
                }
                _scenes[aKey] = std::move(aWorld);
                return _scenes[aKey];
            }

            /**
             * @brief Add a level to the app
             *
             * @param key The key of the world
             * @throw AppExceptionKeyAlreadyExists If the key already exists
             */
            scene &addWorld(const Key &aKey)
            {
                if (_scenes.find(aKey) != _scenes.end()) {
                    throw AppExceptionKeyAlreadyExists("The key already exists");
                }
                _scenes[aKey] = std::make_unique<Core::Scene>();
                return _scenes[aKey];
            }

            /**
             * @brief Remove the world at the given key
             *
             * @param key The key of the world to remove
             * @throw AppExceptionKeyNotFound If the key doesn't exist
             */
            void removeWorld(const Key &aKey)
            {
                if (_scenes.find(aKey) == _scenes.end()) {
                    throw AppExceptionKeyNotFound("The key doesn't exist");
                }
                _scenes.erase(aKey);
            }

            /**
             * @brief Get the current world
             *
             * @throw AppExceptionKeyNotFound If the key doesn't exist
             * @return The current world
             */
            [[nodiscard]] const scene &getCurrentWorld() const
            {
                if (_scenes.find(_currentScenes) == _scenes.end()) {
                    throw AppExceptionKeyNotFound("The key doesn't exist");
                }
                return _scenes.at(_currentScenes);
            }

            /**
             * @brief Get the current world
             * @throw AppExceptionKeyNotFound If the key doesn't exist
             * @return The current world
             */
            [[nodiscard]] scene &getCurrentWorld()
            {
                if (_scenes.find(_currentScenes) == _scenes.end()) {
                    throw AppExceptionKeyNotFound("The key doesn't exist");
                }
                return _scenes.at(_currentScenes);
            }

            /**
             * @brief Set the current world
             * @throw AppExceptionKeyNotFound If the key doesn't exist
             * @param index The index of the world to set as current
             */
            void setCurrentWorld(const Key &key)
            {
                if (_scenes.find(key) == _scenes.end()) {
                    throw AppExceptionKeyNotFound("The key doesn't exist");
                }
                _currentScenes = key;
            }
#pragma endregion methods
    };
} // namespace Engine

#endif /* !APP_HPP_ */
