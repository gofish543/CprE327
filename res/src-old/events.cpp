#include "events.h"

EventManager* event_manager_initialize(Dungeon* dungeon) {
    EventManager* eventManager =(EventManager*) malloc(sizeof(EventManager));

    eventManager->dungeon = dungeon;
    eventManager->tick = 0;
    eventManager->queue =(heap_t*) malloc(sizeof(heap_t));

    heap_init(eventManager->queue, event_compare, null);

    return eventManager;
}

EventManager* event_manager_terminate(EventManager* eventManager) {
    while (eventManager->queue->size != 0) {
        event_terminate((Event*) heap_remove_min(eventManager->queue));
    }
    free(eventManager->queue);
    free(eventManager);

    return null;
}

Event* event_initialize(EventManager* eventManager, u_int tick, enum EventType type, void* structure, int (* handler)(Event*), int (* nextTick)(Event*)) {
    Event* event = (Event*) malloc(sizeof(Event));

    event->tick = tick;
    event->nextTick = nextTick;
    event->handler = handler;
    event->type = type;
    event->structure = structure;

    heap_insert(eventManager->queue, event);

    eventManager->tick++;

    return event;
}

Event* event_terminate(Event* event) {
    free(event);

    return null;
}

int32_t event_compare(const void* A, const void* B) {
    Event* eventA = (Event*) A;
    Event* eventB = (Event*) B;

    return eventA->tick - eventB->tick;
}

Event* event_peek_next(EventManager* eventManager) {
    return (Event*) heap_peek_min(eventManager->queue);
}

int event_handle_next(EventManager* eventManager) {
    Event* nextEvent = (Event*) heap_remove_min(eventManager->queue);

    nextEvent->handler(nextEvent);

    if (nextEvent->tick > eventManager->tick) {
        eventManager->tick = nextEvent->tick;
    } else {
        eventManager->tick++;
    }

    nextEvent->tick = nextEvent->nextTick(nextEvent);

    if (nextEvent->tick == (u_int) -1) {
        // Don't re-insert
        event_terminate(nextEvent);
    } else {
        heap_insert(eventManager->queue, nextEvent);
    }

    return 0;
}