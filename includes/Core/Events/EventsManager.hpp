#ifndef EVENTMANAGER_HPP
#define EVENTMANAGER_HPP

#include <cstddef>
#include <iostream>
#include <memory>
#include <typeindex>
#include <utility>
#include <vector>
#include "Event.hpp"
#include "EventHandler.hpp"
#include "Exception.hpp"
#include <boost/container/flat_map.hpp>

namespace Engine::Event {
    DEFINE_EXCEPTION(EventManagerException);
    DEFINE_EXCEPTION_FROM(EventManagerExceptionNoHandler, EventManagerException);

    /**
     * @brief EventManager class is a singleton that manage all events
     *
     */
    class EventManager final
    {
        public:
            using eventHandler = std::unique_ptr<IEventHandler>;

        private:
            boost::container::flat_map<std::type_index, eventHandler> _eventsHandler;
            std::mutex _mutex;

        public:
            //------------------- DESTRUCTOR-------------------//
            /**
             * @brief Destroy the Event Manager object
             *
             */
            EventManager();
            ~EventManager();

            //-------------------OPERATORS-------------------//
            /**
             * @brief Copy assignment operator, delete because singleton.
             *
             * @param aOther The EventManager to copy.
             */
            EventManager(const EventManager &aOther) = delete;

            /**
             * @brief Move assignment operator, delete because singleton.
             *
             * @param aOther The EventManager to move.
             */
            EventManager(EventManager &&aOther) noexcept = delete;

            /**
             * @brief Copy assignment operator, delete because singleton.
             *
             * @param aOther The EventManager to copy.
             * @return EventManager& A reference to the EventManager.
             */
            EventManager &operator=(const EventManager &aOther) = delete;

            /**
             * @brief Move assignment operator, delete because singleton.
             *
             * @param aOther The EventManager to move.
             * @return EventManager& A reference to the EventManager.
             */
            EventManager &operator=(EventManager &&aOther) noexcept = delete;

            //-------------------METHODS-------------------//
            /**
             * @brief Get the Instance object (singleton)
             *
             * @return EventManager A reference to the EventManager.
             */
            static EventManager &getInstance();

            /**
             * @brief Push an event to the queue
             * @details Doesn't call the subscribers
             * @param aEvent The event to push.
             * @tparam Event The type of the event.
             */
            template<EventConcept Event>
            void pushEvent(const Event &aEvent)
            {
                std::lock_guard<std::mutex> lock(_mutex);

                try {
                    auto &handler = getHandler<Event>();

                    handler.pushEvent(aEvent);
                } catch (const EventManagerExceptionNoHandler &e) {
                    throw EventManagerExceptionNoHandler("Can't push event");
                }
            }

            /**
             * @brief Get all the events of a specific type
             * @tparam Event The type of the event.
             * @return std::vector<Event>& The list of events.
             */
            template<EventConcept Event>
            std::vector<Event> &getEvents()
            {
                std::lock_guard<std::mutex> lock(_mutex);

                try {
                    auto &handler = getHandler<Event>();

                    return handler.getEvents();
                } catch (const EventManagerExceptionNoHandler &e) {
                    throw EventManagerExceptionNoHandler("Can't get events");
                }
            }

            /**
             * @brief Clear all the events of the types that aren't in the list
             * @tparam EventList The list of events to keep.
             */
            template<EventConcept... EventList>
            void keepEventsAndClear()
            {
                std::lock_guard<std::mutex> lock(_mutex);
                std::vector<std::type_index> eventIndexList = {std::type_index(typeid(EventList))...};

                for (auto &handler : _eventsHandler) {
                    if (std::find(eventIndexList.begin(), eventIndexList.end(), handler.first)
                        == eventIndexList.end()) {
                        handler.second->clearEvents();
                    }
                }
            }

            /**
             * @brief Remove an event from the queue
             * @param aIndex The index of the event to remove.
             * @tparam Event The type of the event.
             *
             */
            template<EventConcept Event>
            void removeEvent(const std::size_t aIndex)
            {
                std::lock_guard<std::mutex> lock(_mutex);
                auto eventIndex = std::type_index(typeid(Event));

                if (_eventsHandler.find(eventIndex) == _eventsHandler.end()) {
                    return;
                }
                try {
                    auto &handler = getHandler<Event>();

                    handler.removeEvent(aIndex);
                } catch (const EventManagerExceptionNoHandler &e) {
                    throw EventManagerExceptionNoHandler("Can't remove event");
                }
            }

            /**
             * @brief Remove an event from the queue
             * @param aIndexes The indexes of the event to remove.
             * @tparam Event The type of the event.
             *
             */
            template<EventConcept Event>
            void removeEvent(std::vector<size_t> aIndexes)
            {
                std::lock_guard<std::mutex> lock(_mutex);
                auto eventIndex = std::type_index(typeid(Event));

                if (_eventsHandler.find(eventIndex) == _eventsHandler.end()) {
                    return;
                }
                try {
                    auto &handler = getHandler<Event>();

                    for (size_t i = 0; i < aIndexes.size(); i++) {
                        size_t idx = aIndexes[i] - i;

                        handler.removeEvent(idx);
                    }
                } catch (const EventManagerExceptionNoHandler &e) {
                    throw EventManagerExceptionNoHandler("Can't remove event");
                }
            }

            template<EventConcept Event>
            void initEventHandler()
            {
                std::lock_guard<std::mutex> lock(_mutex);
                auto eventTypeIndex = std::type_index(typeid(Event));

                if (_eventsHandler.find(eventTypeIndex) != _eventsHandler.end()) {
                    return;
                }
                _eventsHandler[eventTypeIndex] = std::make_unique<EventHandler<Event>>();
            }

            template<EventConcept... EventList>
            void initEventHandlers()
            {
                (initEventHandler<EventList>(), ...);
            }

        private:
            /**
             * @brief Get an Hander linked to an event
             *
             * @tparam Event The type of the event to get the handler
             * @return EventHandler<Event>& The handler of the event.
             */
            template<EventConcept Event>
            EventHandler<Event> &getHandler()
            {
                auto eventTypeIndex = std::type_index(typeid(Event));

                if (_eventsHandler.find(eventTypeIndex) == _eventsHandler.cend()) {
                    throw EventManagerExceptionNoHandler("There is no handler of this type");
                }
                auto &handler = _eventsHandler.at(eventTypeIndex);
                auto &component = static_cast<EventHandler<Event> &>(*handler);

                return component;
            }
    };
} // namespace Engine::Event

#endif // !