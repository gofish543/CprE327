#include "movement.h"

int monster_move(Monster* monster, u_char visionTable[FLOOR_HEIGHT][FLOOR_WIDTH]) {
    // Default stay right where they are
    u_char cheapestX = monster->character->x;
    u_char cheapestY = monster->character->y;
    // Position values
    u_char x;
    u_char y;
    // Record cheapest cost
    u_char cheapestCost = U_CHAR_MAX;
    for (y = monster->character->y - 1; y <= monster->character->y + 1; y++) {
        for (x = monster->character->x - 1; x <= monster->character->x + 1; x++) {
            if (visionTable[y][x] <= cheapestCost) {
                cheapestX = x;
                cheapestY = y;
                cheapestCost = visionTable[y][x];
            }
        }
    }

    // Decrement the hardness of the rock, if it is 1, move there
    monster_move_to(monster, cheapestX, cheapestY);

    return 0;
}

bool monster_move_0(Monster* monster, u_char* x, u_char* y) {
    return false;
//    // If moving to the same spot, just exit
//    if (toX == monster->character->x && toY == monster->character->y) {
//        return 0;
//    }
//
//    // Check out the target point
//    if (floor->characters[toY][toX] != null) {
//        if (floor->characters[toY][toX]->player != null) {
//            // Player battle happening
//        } else if (floor->characters[toY][toX]->monster != null) {
//            // Monster stepping on top of another monster, BATTLE TO THE DEATH!
//            action_monster_vs_monster(monster, floor->characters[toY][toX]->monster)
//        }
//    } else if (floor->terrains[monster->character->y][monster->character->x]->hardness > MONSTER_HARDNESS_PER_TURN) {
//        // Take out the hardness, the monster is "tunneling"
//        floor->terrains[monster->character->y][monster->character->x]->hardness -= MONSTER_HARDNESS_PER_TURN;
//
//        monster_run_dijkstra_on_floor(floor);
//    } else {
//        // Tunneler monsters leave corridors behind if tunneling
//        if (monster_is_tunneler(monster->classification) && floor->terrains[monster->character->y][monster->character->x]->isRock) {
//            floor->terrains[monster->character->y][monster->character->x]->isRock = false;
//            floor->terrains[monster->character->y][monster->character->x]->isWalkable = true;
//            floor->terrains[monster->character->y][monster->character->x]->hardness = CORRIDOR_HARDNESS;
//            floor->terrains[monster->character->y][monster->character->x]->character = CORRIDOR_CHARACTER;
//        }
//        // Remove where they were previously standing
//        floor->characters[monster->character->y][monster->character->x] = null;
//
//        // Update the character's x and y
//        monster->character->x = toX;
//        monster->character->y = toY;
//
//        // Simply move the player there
//        floor->characters[monster->character->y][monster->character->x] = monster->character;
//    }
}
bool monster_move_1(Monster* monster, u_char* x, u_char* y) {

}
bool monster_move_2(Monster* monster, u_char* x, u_char* y) {

}
bool monster_move_3(Monster* monster, u_char* x, u_char* y) {

}
bool monster_move_4(Monster* monster, u_char* x, u_char* y) {

}
bool monster_move_5(Monster* monster, u_char* x, u_char* y) {

}
bool monster_move_6(Monster* monster, u_char* x, u_char* y) {

}
bool monster_move_7(Monster* monster, u_char* x, u_char* y) {

}
bool monster_move_8(Monster* monster, u_char* x, u_char* y) {

}
bool monster_move_9(Monster* monster, u_char* x, u_char* y) {

}
bool monster_move_10(Monster* monster, u_char* x, u_char* y) {

}
bool monster_move_11(Monster* monster, u_char* x, u_char* y) {

}

bool monster_move_12(Monster* monster, u_char* x, u_char* y) {

}
bool monster_move_13(Monster* monster, u_char* x, u_char* y) {

}
bool monster_move_14(Monster* monster, u_char* x, u_char* y) {

}
bool monster_move_15(Monster* monster, u_char* x, u_char* y) {

}