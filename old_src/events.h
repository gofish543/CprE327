#ifndef EVENTS_H
#define EVENTS_H

struct EventManager;
typedef struct EventManager EventManager;

struct Event;
typedef struct Event Event;

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "dungeon.h"
#include "resource.h"
#include "vendor/heap.h"

enum EventType {
    event_type_player,
    event_type_monster,
};

struct EventManager {
    Dungeon* dungeon;
    u_int currentTick;
    heap_t* queue;
};

struct Event {
    u_int tick;
    EventType type;
    void* structure;
    int (* handler)(Event*);
};

EventManager* event_manager_initialize(Dungeon* dungeon);
EventManager* event_manager_terminate(EventManager* eventManager);

Event* event_initialize(EventManager* eventManager, u_int tick, u_char type, void* structure, int (* handler)(Event*));
Event* event_terminate(Event* event);

int32_t event_compare(const void* A, const void* B);

Event* event_peek_next(EventManager* eventManager);
int event_handle_next(EventManager* eventManager);

#endif
