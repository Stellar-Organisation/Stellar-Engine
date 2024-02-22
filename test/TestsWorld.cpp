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
** File: TestsWorld.cpp                                                        *
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

struct hp1 : public Engine::Component
{
    public:
        explicit hp1(int aHp)
            : hp(aHp)
        {}

        int hp;
};

struct hp2 : public Engine::Component
{
    public:
        explicit hp2(int aHp)
            : maxHp(aHp)
        {}
        int maxHp;
};

template<typename... Components>
class MySystemClass : public Engine::Systems::System
{
    public:
        explicit MySystemClass(Engine::Core::Scene &world)
            : Engine::Systems::System(world)
        {}

        MySystemClass(const MySystemClass &) = default;
        MySystemClass(MySystemClass &&) = default;

        MySystemClass &operator=(const MySystemClass &) = default;
        MySystemClass &operator=(MySystemClass &&) = default;

        void update() override
        {
            _world.get().template query<Components...>().forEach(
                _clock.getElapsedTime(), [this](Engine::Core::Scene & /*world*/, double /*deltaTime*/, std::size_t idx,
                                                Components &...components) {
                    this->updateSystem(_world.get(), _clock.getElapsedTime(), idx, components...);
                });
            _clock.restart();
        }

    private:
        void updateSystem(Engine::Core::Scene & /*world*/, double /*deltaTime*/, std::size_t /*idx*/, hp1 &hp1Comp,
                          hp2 &hp2Comp)
        {
            hp1Comp.hp--;
            hp2Comp.maxHp -= 2;
        }
};

TEST_CASE("World", "[World]")
{
    Engine::Core::Scene world;

    SECTION("Create an entity")
    {
        auto entity = world.createEntity();
        REQUIRE(entity == 0);
    }
    SECTION("Create an entity and check if it's not the same")
    {
        auto entity = world.createEntity();
        REQUIRE(entity != world.createEntity());
    }
    SECTION("Create an entity and kill it")
    {
        auto entity = world.createEntity();
        world.killEntity(entity);
        REQUIRE(entity == world.createEntity());
    }

    SECTION("Register a component and create an entity")
    {
        world.registerComponent<hp1>();
        auto entity = world.createEntity();
        auto hp1Comp = world.addComponentToEntity(entity, hp1 {10});
        REQUIRE(hp1Comp.hp == 10);
    }

    SECTION("Register a component and create an entity, add the component then kill the entity")
    {
        world.registerComponent<hp1>();
        auto entity = world.createEntity();
        auto hp1Comp = world.addComponentToEntity(entity, hp1 {10});
        REQUIRE(hp1Comp.hp == 10);
        world.killEntity(entity);
        REQUIRE_THROWS_AS(world.query<hp1>().getComponentsOfEntity(entity), Engine::Core::SparseArrayExceptionEmpty);
    }

    SECTION("Register a component after having created an entity")
    {
        auto entity = world.createEntity();
        world.registerComponent<hp1>();
        auto hp1Comp = world.addComponentToEntity(entity, hp1 {10});
        REQUIRE(hp1Comp.hp == 10);
    }

    SECTION("Run a system")
    {
        constexpr int hps = 10;
        auto MySystem = Engine::Systems::createSystem<hp1, hp2>(
            world, "MySystem",
            [](Engine::Core::Scene & /*world*/, double /*deltaTime*/, std::size_t /*idx*/, hp1 &cop1, hp2 &cop2) {
                cop1.hp--;
                cop2.maxHp -= 2;
            });

        world.registerComponents<hp1, hp2>();
        world.addSystem(MySystem);

        auto entity = world.createEntity();
        auto entity2 = world.createEntity();
        auto entity3 = world.createEntity();
        auto &hp1Comp = world.addComponentToEntity(entity, hp1 {hps});
        auto &hp2Comp = world.addComponentToEntity(entity, hp2 {hps});
        auto &hp1Comp2 = world.addComponentToEntity(entity3, hp1 {hps});
        auto &hp2Comp2 = world.addComponentToEntity(entity3, hp2 {hps});
        auto &hp2Comp3 = world.addComponentToEntity(entity2, hp2 {hps});

        world.runSystems();
        REQUIRE(hp1Comp.hp == hps - 1);
        REQUIRE(hp2Comp.maxHp == hps - 2);
        REQUIRE(hp1Comp2.hp == hps - 1);
        REQUIRE(hp2Comp2.maxHp == hps - 2);
        REQUIRE(hp2Comp3.maxHp == hps);
        auto secondSystem = std::make_pair<std::string, std::unique_ptr<Engine::Systems::System>>(
            "MySystemClass", std::make_unique<MySystemClass<hp1, hp2>>(world));
        world.addSystem(secondSystem);
        world.runSystems();
        REQUIRE(hp1Comp.hp == hps - 3);
        REQUIRE(hp2Comp.maxHp == hps - 6);
        REQUIRE(hp1Comp2.hp == hps - 3);
        REQUIRE(hp2Comp2.maxHp == hps - 6);
        REQUIRE(hp2Comp3.maxHp == hps);
        world.removeSystem(MySystem.first);
        world.runSystems();
        REQUIRE(hp1Comp.hp == hps - 4);
        REQUIRE(hp2Comp.maxHp == hps - 8);
        REQUIRE(hp1Comp2.hp == hps - 4);
        REQUIRE(hp2Comp2.maxHp == hps - 8);
        REQUIRE(hp2Comp3.maxHp == hps);
    }
}
