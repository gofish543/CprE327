#include "events.h"

EventManager::EventManager(Dungeon* dungeon) {
    this->dungeon = dungeon;

    this->tick = 0;
    this->queue = (heap_t*) malloc(sizeof(heap_t));

    heap_init(this->queue, Event::Compare, null);
}

EventManager::~EventManager() {
    while (this->queue->size != 0) {
        delete ((Event*) heap_remove_min(this->queue));
    }

    free(this->queue);
}

EventManager* EventManager::addToQueue(Event* event) {
    heap_insert(this->queue, event);

    this->incrementTick();

    return this;
}

Event* EventManager::peekNext() {
    return (Event*) heap_peek_min(this->queue);
}

int EventManager::handleNext() {
    auto nextEvent = (Event*) heap_remove_min(this->queue);

    nextEvent->handler(nextEvent);

    if (nextEvent->getTick() > this->tick) {
        this->tick = nextEvent->getTick();
    } else {
        this->incrementTick();
    }

    nextEvent->setTick(nextEvent->nextTick(nextEvent));

    if (nextEvent->getTick() == -1) {
        // Don't re-insert
        delete (nextEvent);
    } else {
        heap_insert(this->queue, nextEvent);
    }

    return 0;
}

EventManager* EventManager::incrementTick() {
    this->tick++;

    return this;
}

/** GETTERS **/
Dungeon* EventManager::getDungeon() {
    return this->dungeon;
}

int EventManager::getTick() {
    return this->tick;
}

heap_t* EventManager::getQueue() {
    return this->queue;
}
/** GETTERS **/

/** SETTERS **/
EventManager* EventManager::setDungeon(Dungeon* dungeon) {
    this->dungeon = dungeon;

    return this;
}

EventManager* EventManager::setTick(int tick) {
    this->tick = tick;

    return this;
}
/** SETTERS **/

/**-----------------EVENT-------------**/

Event::Event(int tick, enum EventType type, Character* character, int (* handler)(Event*), int (* nextTick)(Event*)) {
    this->tick = tick;
    this->type = type;

    this->character = character;

    this->handler = handler;
    this->nextTick = nextTick;
}

Event::~Event() = default;

int32_t Event::Compare(const void* A, const void* B) {
    auto eventA = (Event*) A;
    auto eventB = (Event*) B;

    return eventA->getTick() - eventB->getTick();
}

/** GETTERS **/
int Event::getTick() {
    return this->tick;
}

enum EventType Event::getType() {
    return this->type;
}
/** GETTERS **/

/** SETTERS **/
Event* Event::setTick(int tick) {
    this->tick = tick;

    return this;
}

Event* Event::setType(enum EventType type) {
    this->type = type;

    return this;
}
/** SETTERS **/
