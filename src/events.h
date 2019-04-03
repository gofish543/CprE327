#ifndef EVENTS_H
#define EVENTS_H

#include "dungeon.h"
#include "vendor/heap.h"

enum EventType {
    event_type_player,
    event_type_monster,
};

namespace App {
    class EventManager {
    public:
        explicit EventManager(Dungeon* dungeon);
        ~EventManager();

        EventManager* addToQueue(Event* event);
        Event* peekNext();
        int handleNext();

        EventManager* incrementTick();

        /** GETTERS **/
        Dungeon* getDungeon();
        int getTick();
        heap_t* getQueue();
        /** GETTERS **/

        /** SETTERS **/
        EventManager* setTick(int tick);
        /** SETTERS **/
    protected:

    private:
        Dungeon* dungeon;
        int tick;
        heap_t* queue;
    };

    class Event {
    public:
        Event(int tick, enum EventType type, Character* character, int(* handler)(Event*), int (* nextTick)(Event*));
        ~Event();

        static int32_t Compare(const void* A, const void* B);

        /** GETTERS **/
        int getTick();
        enum EventType getType();
        /** GETTERS **/

        /** SETTERS **/
        Event* setTick(int tick);
        /** SETTERS **/

        Character* character;
        int (* handler)(Event*);
        int (* nextTick)(Event*);
    protected:

    private:
        int tick;
        enum EventType type;
    };
}

#endif
