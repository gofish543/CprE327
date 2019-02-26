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

/*
 * MONSTER 0
 * This monster is stupid, and will move in a random direction that it can move in
 */
bool monster_move_0(Monster* monster, u_char* x, u_char* y) {
    Floor* floor = monster->character->floor;

    do {
        *x = monster->character->x;
        *y = monster->character->y;

        *x = (*x + random_number_between(-1, 1));
        *y = (*y + random_number_between(-1, 1));
    } while (!floor->terrains[*y][*x]->isWalkable);

    // Now a few things could happen
    // 0) The monster just moved on themselves
    // 1) The monster just fell on the player
    // 2) The monster just fell on another monster
    // 3) The monster just fell on open land
    if(*x == monster->character->x && *y == monster->character->y) { // 0) The monster just moved onto themselves
        return false;
    }
    else if (floor->characters[*y][*x] != null) {
        if (floor->characters[*y][*x]->player != null) {         // 1) The monster fell on the player
            // Let the battle happen
            return !action_player_vs_monster(floor->characters[*y][*x]->player, monster);
        } else if (floor->characters[*y][*x]->monster != null) {// 2) The monster fell on another monster
            // Let the battle happen
            return action_monster_vs_monster(monster, floor->characters[*y][*x]->monster) == 1;
        }
    }

    return true; // The monster is making a valid move
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