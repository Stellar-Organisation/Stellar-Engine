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
**                                                                             *
** File: Component.hpp                                                         *
** Project: Stellar-Engine                                                     *
** Created Date: Sa Feb 2024                                                   *
** Author: GlassAlo                                                            *
** Email: ofourpatat@gmail.com                                                 *
** -----                                                                       *
** Description: The file contains the base class for all components,           *
**              and a concept for components.                                  *
** -----                                                                       *
** Last Modified: Sat Feb 17 2024                                              *
** Modified By: GlassAlo                                                       *
** -----                                                                       *
** Copyright (c) 2024 Stellar-Organisation                                     *
** -----                                                                       *
** HISTORY:                                                                    *
** Date      	By	Comments                                                   *
** ----------	---	---------------------------------------------------------  *
*/

#ifndef COMPONENT_HPP_
#define COMPONENT_HPP_
#include <type_traits>

namespace Engine {
    struct Component
    {
        public:
            Component() = default;
            virtual ~Component() = default;

            Component(const Component &) = default;
            Component &operator=(const Component &) = default;

            Component(Component &&) noexcept = default;
            Component &operator=(Component &&) noexcept = default;
    };

    template<typename T>
    concept ComponentConcept = std::is_base_of_v<Component, T>;
} // namespace Engine
#endif /* !COMPONENT_HPP_ */
