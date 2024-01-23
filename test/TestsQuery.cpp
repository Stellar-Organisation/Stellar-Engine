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

TEST_CASE("Query")
{
    Engine::Core::World world;
    constexpr int hps = 10;

    struct Player : public Engine::Component
    {};

    SECTION("getAllEntities")
    {
        world.registerComponents<hp1, hp2, Player>();

        world.createEntity();
        world.createEntity();

        world.addComponentToEntity(0, hp1 {hps});
        world.addComponentToEntity(1, hp1 {hps});
        world.addComponentToEntity(0, hp2 {hps});
        world.addComponentToEntity(1, hp2 {hps});
        world.addComponentToEntity(0, Player {});

        auto query = world.query<hp1, hp2, Player>().getAllEntities();
        REQUIRE(query.size() == 1);
        REQUIRE(query[0] == 0);
    }

    SECTION("getAllEntities 2")
    {
        world.registerComponents<hp1, hp2, Player>();

        world.createEntity();
        world.createEntity();

        world.addComponentToEntity(0, hp1 {hps});
        world.addComponentToEntity(1, hp1 {hps});
        world.addComponentToEntity(0, hp2 {hps});
        world.addComponentToEntity(1, hp2 {hps});
        world.addComponentToEntity(0, Player {});
        world.addComponentToEntity(1, Player {});

        auto query = world.query<hp1, hp2, Player>().getAllEntities();
        REQUIRE(query.size() == 2);
        REQUIRE(query[0] == 0);
        REQUIRE(query[1] == 1);
    }

    SECTION("getAllEntities 3")
    {
        world.registerComponents<hp1, hp2, Player>();

        world.createEntity();
        world.createEntity();
        world.createEntity();

        world.addComponentToEntity(0, hp1 {hps});
        world.addComponentToEntity(1, hp1 {hps});
        world.addComponentToEntity(2, hp1 {hps});
        world.addComponentToEntity(0, hp2 {hps});
        world.addComponentToEntity(1, hp2 {hps});
        world.addComponentToEntity(2, hp2 {hps});
        world.addComponentToEntity(0, Player {});
        world.addComponentToEntity(2, Player {});

        auto query = world.query<hp1, hp2, Player>().getAllEntities();
        REQUIRE(query.size() == 2);
        REQUIRE(query[0] == 0);
        REQUIRE(query[1] == 2);
    }

    SECTION("getComponentsOfEntity")
    {
        world.registerComponents<hp1, hp2, Player>();

        world.createEntity();

        world.addComponentToEntity(0, hp1 {hps});
        world.addComponentToEntity(0, hp2 {hps});
        world.addComponentToEntity(0, Player {});

        auto query = world.query<hp1, hp2, Player>().getComponentsOfEntity(0);
        const hp1 &hp1Comp = std::get<0>(query);
        const hp2 &hp2Comp = std::get<1>(query);
        const Player &playerComp = std::get<2>(query);

        REQUIRE(hp1Comp.hp == hps);
        REQUIRE(hp2Comp.maxHp == hps);
        REQUIRE_NOTHROW(playerComp);
    }

    SECTION("getAll")
    {
        world.registerComponents<hp1, hp2, Player>();

        auto idx1 = world.createEntity();
        world.createEntity();
        auto idx2 = world.createEntity();

        world.addComponentToEntity(0, hp1 {hps});
        world.addComponentToEntity(1, hp1 {hps});
        world.addComponentToEntity(2, hp1 {hps});
        world.addComponentToEntity(0, hp2 {hps});
        world.addComponentToEntity(1, hp2 {hps});
        world.addComponentToEntity(2, hp2 {hps});
        world.addComponentToEntity(0, Player {});
        world.addComponentToEntity(2, Player {});

        auto query = world.query<hp1, hp2, Player>().getAll();
        REQUIRE(query.size() == 2);
        REQUIRE(std::get<0>(query[0]) == idx1);
        REQUIRE(std::get<0>(query[1]) == idx2);
    }
}
