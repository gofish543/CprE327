#include "movement.h"

void monster_move(Monster* monster, u_char visionTable[FLOOR_HEIGHT][FLOOR_WIDTH]) {
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
}

/*
 * MONSTER 0
 *      INTELLIGENT     = 0
 *      TELEPATHIC      = 0
 *      TUNNELER        = 0
 *      ERRATIC         = 0
 *
 * This monster is stupid, and will move in a random direction that it can move in
 */
void monster_move_0(Monster* monster, u_char* x, u_char* y) {
    Floor* floor = monster->character->floor;

    do {
        *x = monster->character->x;
        *y = monster->character->y;

        *x = (*x + random_number_between(-1, 1));
        *y = (*y + random_number_between(-1, 1));
    } while (!floor->terrains[*y][*x]->isWalkable);
}

/*
 * MONSTER 1
 *      INTELLIGENT     = 1
 *      TELEPATHIC      = 0
 *      TUNNELER        = 0
 *      ERRATIC         = 0
 *
 * This monster is intelligent and will move towards the player if it has line of sight, if not, it moves in a random direction
 */
void monster_move_1(Monster* monster, u_char* x, u_char* y) {
    printf("Moving 1\n");
    exit(1);

    if (monster_has_line_of_signt(monster, monster->character->floor->dungeon->player)) {

    } else {
        monster_move_0(monster, x, y);
    }
}
void monster_move_2(Monster* monster, u_char* x, u_char* y) {

}
void monster_move_3(Monster* monster, u_char* x, u_char* y) {

}
void monster_move_4(Monster* monster, u_char* x, u_char* y) {

}
void monster_move_5(Monster* monster, u_char* x, u_char* y) {

}
void monster_move_6(Monster* monster, u_char* x, u_char* y) {

}
void monster_move_7(Monster* monster, u_char* x, u_char* y) {

}
void monster_move_8(Monster* monster, u_char* x, u_char* y) {

}
void monster_move_9(Monster* monster, u_char* x, u_char* y) {

}
void monster_move_10(Monster* monster, u_char* x, u_char* y) {

}
void monster_move_11(Monster* monster, u_char* x, u_char* y) {

}

void monster_move_12(Monster* monster, u_char* x, u_char* y) {

}
void monster_move_13(Monster* monster, u_char* x, u_char* y) {

}
void monster_move_14(Monster* monster, u_char* x, u_char* y) {

}
void monster_move_15(Monster* monster, u_char* x, u_char* y) {

}