#include "staircase.h"

Staircase* staircase_initialize(u_char x, u_char y, u_char fromFloor, u_char toFloor) {
    Staircase* staircase = malloc(sizeof(Staircase));

    staircase->x = x;
    staircase->y = y;

    staircase->fromFloor = fromFloor;
    staircase->toFloor = toFloor;

    if (fromFloor < toFloor) {
        staircase->isDown = false;
        staircase->isUp = true;
    } else if (fromFloor > toFloor) {
        staircase->isDown = true;
        staircase->isUp = false;
    } else {
        staircase->isDown = false;
        staircase->isUp = false;
    }

    return staircase;
}

Staircase* staircase_terminate(Staircase* staircase) {
    free(staircase);

    return NULL;
}

Dungeon* staircase_take(Dungeon* dungeon, Staircase* staircase) {
    // Find the target staircase index

    // Wipe the current queue of events

    // Create a new event manager for the floor upcoming floor

    // Set dungeon floor and player floor pointers

    // Place the character on the upper floor's staircase
    // Or place at a random location

    // Move monster's on that floor to random locations not in the same room as the user

    // Update monster mappings for that floor

    // Add player to the new queue

    // Add monsters to the new queue

    return dungeon;
}