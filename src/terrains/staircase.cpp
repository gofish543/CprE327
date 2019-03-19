#include "staircase.h"

Staircase::Staircase(Floor* floor, u_short id, u_char x, u_char y, char type) : Terrain(floor, id, x, y) {
    this->isWalkable = true;
    this->isRock = false;
    this->isImmutable = false;

    this->hardness = STAIRCASE_HARDNESS;
    this->type = type;
    if (this->type == STAIRCASE_TYPE_DOWN) {
        this->character = STAIRCASE_DOWN_CHARACTER;
    } else if (this->type == STAIRCASE_TYPE_UP) {
        this->character = STAIRCASE_UP_CHARACTER;
    } else {
        this->character = UNKNOWN_CHARACTER;
    }
}

//Dungeon* staircase_take(Dungeon* dungeon, Staircase* staircase) {
//    // Find the target staircase index
//    u_short staircaseIndex;
//    if (staircase->isUp) {
//        for (staircaseIndex = 0; staircaseIndex < dungeon->floor->stairUpCount; staircaseIndex++) {
//            if (dungeon->floor->upStairs[staircaseIndex] == staircase) {
//                break;
//            }
//        }
//    } else {
//        for (staircaseIndex = 0; staircaseIndex < dungeon->floor->stairDownCount; staircaseIndex++) {
//            if (dungeon->floor->downStairs[staircaseIndex] == staircase) {
//                break;
//            }
//        }
//    }
//
//    // Wipe the current queue of events
//    event_manager_terminate(dungeon->eventManager);
//
//    // Create a new event manager for the floor upcoming floor
//    dungeon->eventManager = event_manager_initialize(dungeon);
//
//    // Update character map
//    dungeon->floor->characters[dungeon->player->character->y][dungeon->player->character->x] = null;
//
//    // Set dungeon floor and player floor pointers
//    dungeon->floor = dungeon->floors[staircase->toFloor];
//    dungeon->player->character->floor = dungeon->floor;
//
//    // Place the character on the upper floor's staircase
//    // If up staircase and there is a down staircase to step on
//    if (staircase->isUp && staircaseIndex < dungeon->floor->stairDownCount) {
//        dungeon->player->character->x = dungeon->floor->downStairs[staircaseIndex]->x;
//        dungeon->player->character->y = dungeon->floor->downStairs[staircaseIndex]->y;
//    }
//        // If down staircase and there is an up staircase to step on
//    else if (staircase->isDown && staircaseIndex < dungeon->floor->stairUpCount) {
//        dungeon->player->character->x = dungeon->floor->upStairs[staircaseIndex]->x;
//        dungeon->player->character->y = dungeon->floor->upStairs[staircaseIndex]->y;
//    }
//        // Else random location
//    else {
//        u_char playerX;
//        u_char playerY;
//        u_short room = random_number_between(0, dungeon->floor->roomCount - 1);
//        do {
//            playerX = random_number_between(dungeon->floor->rooms[room]->startX, dungeon->floor->rooms[room]->startX + dungeon->floor->rooms[room]->width - 1);
//            playerY = random_number_between(dungeon->floor->rooms[room]->startY, dungeon->floor->rooms[room]->startY + dungeon->floor->rooms[room]->height - 1);
//        } while (dungeon->floor->terrains[playerY][playerX]->staircase != null);
//
//        if (dungeon->floor->characters[playerY][playerX] != null) {
//            dungeon->floor->characters[playerY][playerX]->monster->isAlive = false;
//        }
//
//        dungeon->player->character->x = playerX;
//        dungeon->player->character->y = playerY;
//    }
//
//    dungeon->floor->characters[dungeon->player->character->y][dungeon->player->character->x] = dungeon->player->character;
//
//    // Move monster's on that floor to random locations not in the same room as the user
//    Room* playerRoom = dungeon->floor->terrains[dungeon->player->character->y][dungeon->player->character->x]->room;
//
//    u_char height = 0;
//    u_char width = 0;
//
//    for (height = playerRoom->startY; height < playerRoom->startY + playerRoom->height; height++) {
//        for (width = playerRoom->startX; width < playerRoom->startX + playerRoom->width; width++) {
//            if (dungeon->floor->characters[height][width] != null && dungeon->floor->characters[height][width]->monster != null) {
//                // Move that monster somewhere else or kill it
//                Room* randomRoomPlacement;
//                bool placed = false;
//                u_char monsterHeight;
//                u_char monsterWidth;
//                do {
//                    randomRoomPlacement = dungeon->floor->rooms[random_number_between(0, dungeon->floor->roomCount - 1)];
//                } while (randomRoomPlacement == playerRoom);
//
//                for (monsterHeight = randomRoomPlacement->startY; monsterHeight < randomRoomPlacement->startY + randomRoomPlacement->height && !placed; monsterHeight++) {
//                    for (monsterWidth = randomRoomPlacement->startX; monsterWidth < randomRoomPlacement->startX + randomRoomPlacement->width && !placed; monsterWidth++) {
//                        if (dungeon->floor->characters[monsterHeight][monsterWidth] == null) {
//                            dungeon->floor->characters[monsterHeight][monsterWidth] = dungeon->floor->characters[height][width];
//
//                            dungeon->floor->characters[monsterHeight][monsterWidth]->y = monsterHeight;
//                            dungeon->floor->characters[monsterHeight][monsterWidth]->x = monsterWidth;
//                            placed = true;
//                        }
//                    }
//                }
//
//                // If not placed, just kill the monster
//                if (!placed) {
//                    dungeon->floor->characters[height][width]->monster->isAlive = false;
//                }
//
//                dungeon->floor->characters[height][width] = null;
//            }
//        }
//    }
//
//    // Update monster mappings for that floor
//    monster_run_dijkstra_on_floor(dungeon->floor);
//
//    // Add player to the new queue
//    event_initialize(dungeon->eventManager, 0, event_type_player, dungeon->player, player_event, player_next_tick);
//
//    u_short index = 0;
//    // Add monsters to the new queue
//    for (index = 0; index < dungeon->floor->monsterCount; index++) {
//        if (dungeon->floor->monsters[index]->isAlive) {
//            event_initialize(dungeon->eventManager, dungeon->eventManager->tick, event_type_monster, dungeon->floor->monsters[index], monster_event, monster_next_tick);
//        }
//    }
//
//    return dungeon;
//}

Floor* Staircase::getTargetFloor() {
    return this->floor->getDungeon()->getFloors().at(
            this->floor->getDungeon()->getCurrentFloor()->getFloorNumber() + this->type
    );
}

bool Staircase::isUp() {
    return this->type == STAIRCASE_TYPE_UP;
}

bool Staircase::isDown() {
    return this->type == STAIRCASE_TYPE_DOWN;
}