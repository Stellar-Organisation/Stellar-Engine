#include <cstddef>
#include <cstdio>
#include <functional>
#include <iostream>
#include <memory>
#include "Component.hpp"
#include "ECS.hpp"
#include <catch2/catch_test_macros.hpp>

struct testEvent : public Engine::Event::Event
{
    public:
        explicit testEvent(int aHp)
            : hp(aHp)
        {}

        int hp;
};

TEST_CASE("Events")
{
    Engine::Event::EventManager EventManager;

    SECTION("Register an event and push it")
    {
        EventManager.initEventHandler<testEvent>();

        EventManager.pushEvent(testEvent {10});

        REQUIRE(EventManager.getEvents<testEvent>().size() == 1);
        REQUIRE(EventManager.getEvents<testEvent>()[0].hp == 10);
    }

    SECTION("Register an event and push it and clear it")
    {
        EventManager.initEventHandler<testEvent>();
        EventManager.pushEvent(testEvent {10});
        EventManager.keepEventsAndClear();

        REQUIRE(EventManager.getEvents<testEvent>().size() == 0);
    }

    SECTION("Register an event and push it and clear it and push it again")
    {
        EventManager.initEventHandler<testEvent>();
        EventManager.pushEvent(testEvent {10});
        EventManager.keepEventsAndClear();
        EventManager.pushEvent(testEvent {20});

        REQUIRE(EventManager.getEvents<testEvent>().size() == 1);
        REQUIRE(EventManager.getEvents<testEvent>()[0].hp == 20);
    }

    SECTION("Register an event and push it then push another one and remove the first one")
    {
        EventManager.initEventHandler<testEvent>();
        EventManager.pushEvent(testEvent {10});
        EventManager.pushEvent(testEvent {20});
        EventManager.removeEvent<testEvent>(0);

        REQUIRE(EventManager.getEvents<testEvent>().size() == 1);
        REQUIRE(EventManager.getEvents<testEvent>()[0].hp == 20);
    }

    SECTION("Register an event and push it then push another one and remove the second one")
    {
        EventManager.initEventHandler<testEvent>();
        EventManager.pushEvent(testEvent {10});
        EventManager.pushEvent(testEvent {20});
        EventManager.removeEvent<testEvent>(1);

        REQUIRE(EventManager.getEvents<testEvent>().size() == 1);
        REQUIRE(EventManager.getEvents<testEvent>()[0].hp == 10);
    }
}
