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
** File: TestsApp.cpp                                                          *
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

#include <cstddef>
#include <cstdio>
#include <functional>
#include <iostream>
#include <memory>
#include "Component.hpp"
#include "ECS.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("App", "[App]")
{
    Engine::App app;
    auto world = std::make_unique<Engine::Core::Scene>();

    SECTION("Add a world")
    {
        app.addWorld(0, std::move(world));
        REQUIRE(app[0] != nullptr);
    }
    SECTION("Add a world with a key already used")
    {
        app.addWorld(0, std::move(world));
        REQUIRE_THROWS_AS(app.addWorld(0, std::move(world)), Engine::AppExceptionKeyAlreadyExists);
    }
    SECTION("Get a world with a key not used")
    {
        REQUIRE_THROWS_AS(app[0], Engine::AppExceptionKeyNotFound);
    }
    SECTION("Get a world with a key used")
    {
        app.addWorld(0, std::move(world));
        REQUIRE_NOTHROW(app[0]);
    }
    SECTION("Get a world with a key used and check if it's the same")
    {
        app.addWorld(0, std::move(world));
        REQUIRE(app[0] == app[0]);
    }
    SECTION("Get a world with a key used and check if it's not the same")
    {
        app.addWorld(0, std::move(world));
        app.addWorld(1, std::move(world));
        REQUIRE(app[0] != app[1]);
    }
    SECTION("Get a world with a key used and check if it's not the same after a move")
    {
        app.addWorld(0, std::move(world));
        auto tmp = std::move(app[0]);
        REQUIRE(app[0] != tmp);
    }
}
