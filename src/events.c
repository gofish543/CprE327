#include "events.h"

EventManager* event_manager_initialize(Dungeon* dungeon) {
    EventManager* eventManager = malloc(sizeof(EventManager));

    eventManager->dungeon = dungeon;
    eventManager->currentTick = 0;
    eventManager->queue = malloc(sizeof(heap_t));

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

int32_t event_compare(const void* A, const void* B) {
    Event* eventA = (Event*) A;
    Event* eventB = (Event*) B;

    return eventA->tick - eventB->tick;
}

Event* event_initialize(EventManager* eventManager, u_int tick, u_char type, void* structure, int (* handler)(Event*)) {
    Event* event = malloc(sizeof(Event));

    event->tick = tick;
    event->handler = handler;
    event->type = type;
    event->structure = structure;

    heap_insert(eventManager->queue, event);

    return event;
}

Event* event_terminate(Event* event) {
    free(event);

    return null;
}

int event_increment_tick(EventManager* eventManager, u_int increment) {
    eventManager->currentTick += increment;

    return eventManager->currentTick;
}

Event* event_peek_next(EventManager* eventManager) {
    return (Event*) heap_peek_min(eventManager->queue);
}

int event_handle_next(EventManager* eventManager) {
    Event* nextEvent = (Event*) heap_remove_min(eventManager->queue);

    nextEvent->handler(nextEvent);

    // Fast forward the game tick to the event's if in the future
    if (nextEvent->tick > eventManager->currentTick) {
        eventManager->currentTick = nextEvent->tick;
    }

    event_terminate(nextEvent);

    return 0;
}