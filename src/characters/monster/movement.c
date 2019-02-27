#include "movement.h"

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

    if (monster_is_tunneler(monster)) {
        do {
            *x = monster->character->x;
            *y = monster->character->y;

            *x = (*x + random_number_between(-1, 1));
            *y = (*y + random_number_between(-1, 1));
        } while (floor->terrains[*y][*x]->isImmutable);
    } else {
        do {
            *x = monster->character->x;
            *y = monster->character->y;

            *x = (*x + random_number_between(-1, 1));
            *y = (*y + random_number_between(-1, 1));
        } while (!floor->terrains[*y][*x]->isWalkable || floor->terrains[*y][*x]->isImmutable);
    }
}

/*
 * MONSTER 1
 *      INTELLIGENT     = 1
 *      TELEPATHIC      = 0
 *      TUNNELER        = 0
 *      ERRATIC         = 0
 *
 * This monster is intelligent and will move towards the player if it has line of sight
 */
void monster_move_1(Monster* monster, u_char* x, u_char* y) {
    // If the monster has line of sight, store previous location
    if (monster_has_line_of_sight(monster)) {
        monster->playerLastSpottedX = monster->character->floor->dungeon->player->character->x;
        monster->playerLastSpottedY = monster->character->floor->dungeon->player->character->y;
    } else if ((monster->playerLastSpottedY == 0 && monster->playerLastSpottedX == 0) ||
               (monster->character->x == monster->playerLastSpottedX && monster->character->y == monster->playerLastSpottedY)) {
        // No previous location found, just stay here
        // Previous player location is our location, stay here
        *x = monster->character->x;
        *y = monster->character->y;
        return;
    }

    Floor* floor = monster->character->floor;
    Player* player = floor->dungeon->player;

    u_char playerX = player->character->x;
    u_char playerY = player->character->y;

    // "Move" the player
    player->character->x = monster->playerLastSpottedX;
    player->character->y = monster->playerLastSpottedY;
    // Run dijstra for just the non tunneler view
    monster_run_dijkstra(floor, -1, floor->nonTunnelerView);

    // Default stay right where they are
    u_char cheapestX = monster->character->x;
    u_char cheapestY = monster->character->y;
    // Position values
    u_char tempX;
    u_char tempY;
    // Record cheapest cost
    u_char cheapestCost = U_CHAR_MAX;
    for (tempY = monster->character->y - 1; tempY <= monster->character->y + 1; tempY++) {
        for (tempX = monster->character->x - 1; tempX <= monster->character->x + 1; tempX++) {
            if (floor->nonTunnelerView[tempY][tempX] <= cheapestCost) {
                cheapestX = tempX;
                cheapestY = tempY;
                cheapestCost = floor->nonTunnelerView[tempY][tempX];
            }
        }
    }

    // "Move" player back and rerun dijkstra
    player->character->x = playerX;
    player->character->y = playerY;
    monster_run_dijkstra(floor, -1, floor->nonTunnelerView);

    *x = cheapestX;
    *y = cheapestY;
}

/*
 * MONSTER 2
 *      INTELLIGENT     = 0
 *      TELEPATHIC      = 1
 *      TUNNELER        = 0
 *      ERRATIC         = 0
 *
 * This monster is telepathic and will move towards the player, but cannot go through rock, and isn't intelligent
 * Since it isn't intelligent, move towards the player in a direct line
 */
void monster_move_2(Monster* monster, u_char* x, u_char* y) {
    Floor* floor = monster->character->floor;

    char deltaX = floor->dungeon->player->character->x - monster->character->x;
    char deltaY = floor->dungeon->player->character->y - monster->character->y;

    if (abs(deltaX) > abs(deltaY)) { // Move in the x direction?
        *x = monster->character->x + get_sign(deltaX);
        *y = monster->character->y;
    } else if (abs(deltaX) < abs(deltaY)) { // Move in the y direction?
        *x = monster->character->x;
        *y = monster->character->y + get_sign(deltaY);
    } else { // Move in a diagonal?
        *x = monster->character->x + get_sign(deltaX);
        *y = monster->character->y + get_sign(deltaY);
    }

    // Cant tunnel
    if (floor->terrains[*y][*x]->isRock || floor->terrains[*y][*x]->isImmutable) {
        *x = monster->character->x;
        *y = monster->character->y;
    }
}

/*
 * MONSTER 3
 *      INTELLIGENT     = 1
 *      TELEPATHIC      = 1
 *      TUNNELER        = 0
 *      ERRATIC         = 0
 *
 * This monster is intelligent and telepathic. It will move towards the player using the non tunneler path
 */
void monster_move_3(Monster* monster, u_char* x, u_char* y) {
    Floor* floor = monster->character->floor;

    // Default stay right where they are
    u_char cheapestX = monster->character->x;
    u_char cheapestY = monster->character->y;
    // Position values
    u_char tempX;
    u_char tempY;
    // Record cheapest cost
    u_char cheapestCost = U_CHAR_MAX;
    for (tempY = monster->character->y - 1; tempY <= monster->character->y + 1; tempY++) {
        for (tempX = monster->character->x - 1; tempX <= monster->character->x + 1; tempX++) {
            if (floor->nonTunnelerView[tempY][tempX] <= cheapestCost) {
                cheapestX = tempX;
                cheapestY = tempY;
                cheapestCost = floor->nonTunnelerView[tempY][tempX];
            }
        }
    }

    *x = cheapestX;
    *y = cheapestY;
}

/*
 * MONSTER 4
 *      INTELLIGENT     = 0
 *      TELEPATHIC      = 0
 *      TUNNELER        = 1
 *      ERRATIC         = 0
 *
 * This monster is a tunneler, but is not intelligent or telepathic.
 * As a result the monster will move in a direct line towards line of sight or last spotted
 */
void monster_move_4(Monster* monster, u_char* x, u_char* y) {
    // If the monster has line of sight, store previous location
    if (monster_has_line_of_sight(monster)) {
        monster->playerLastSpottedX = monster->character->floor->dungeon->player->character->x;
        monster->playerLastSpottedY = monster->character->floor->dungeon->player->character->y;
    } else if ((monster->playerLastSpottedY == 0 && monster->playerLastSpottedX == 0) ||
               (monster->character->x == monster->playerLastSpottedX && monster->character->y == monster->playerLastSpottedY)) {
        // No previous location found, just stay here
        // Previous player location is our location, stay here
        *x = monster->character->x;
        *y = monster->character->y;
        return;
    }

    Floor* floor = monster->character->floor;

    char deltaX = monster->playerLastSpottedX - monster->character->x;
    char deltaY = monster->playerLastSpottedY - monster->character->y;

    if (deltaY == 0) { // Move in the x direction
        *x = monster->character->x + get_sign(deltaX);
        *y = monster->character->y;
    } else if (abs(deltaX) / abs(deltaY) > 1.25) { // Move in the x direction?
        *x = monster->character->x + get_sign(deltaX);
        *y = monster->character->y;
    } else if (abs(deltaX) / abs(deltaY) < 0.75) { // Move in the y direction?
        *x = monster->character->x;
        *y = monster->character->y + get_sign(deltaY);
    } else { // Move in a diagonal?
        *x = monster->character->x + get_sign(deltaX);
        *y = monster->character->y + get_sign(deltaY);
    }

    // Cant tunnel through immutable rock
    if (floor->terrains[*y][*x]->isImmutable) {
        *x = monster->character->x;
        *y = monster->character->y;
    }
}

/*
 * MONSTER 5
 *      INTELLIGENT     = 1
 *      TELEPATHIC      = 0
 *      TUNNELER        = 1
 *      ERRATIC         = 0
 *
 * This monster is intelligent with tunneling and will move towards the player if it has line of sight
 */
void monster_move_5(Monster* monster, u_char* x, u_char* y) {
// If the monster has line of sight, store previous location
    if (monster_has_line_of_sight(monster)) {
        monster->playerLastSpottedX = monster->character->floor->dungeon->player->character->x;
        monster->playerLastSpottedY = monster->character->floor->dungeon->player->character->y;
    } else if ((monster->playerLastSpottedY == 0 && monster->playerLastSpottedX == 0) ||
               (monster->character->x == monster->playerLastSpottedX && monster->character->y == monster->playerLastSpottedY)) {
        // No previous location found, just stay here
        // Previous player location is our location, stay here
        *x = monster->character->x;
        *y = monster->character->y;
        return;
    }

    Floor* floor = monster->character->floor;
    Player* player = floor->dungeon->player;

    u_char playerX = player->character->x;
    u_char playerY = player->character->y;

    // "Move" the player
    player->character->x = monster->playerLastSpottedX;
    player->character->y = monster->playerLastSpottedY;
    // Run dijstra for just the non tunneler view
    monster_run_dijkstra(floor, 1, floor->tunnelerView);

    // Default stay right where they are
    u_char cheapestX = monster->character->x;
    u_char cheapestY = monster->character->y;
    // Position values
    u_char tempX;
    u_char tempY;
    // Record cheapest cost
    u_char cheapestCost = U_CHAR_MAX;
    for (tempY = monster->character->y - 1; tempY <= monster->character->y + 1; tempY++) {
        for (tempX = monster->character->x - 1; tempX <= monster->character->x + 1; tempX++) {
            if (floor->tunnelerView[tempY][tempX] <= cheapestCost) {
                cheapestX = tempX;
                cheapestY = tempY;
                cheapestCost = floor->tunnelerView[tempY][tempX];
            }
        }
    }

    // "Move" player back and rerun dijkstra
    player->character->x = playerX;
    player->character->y = playerY;
    monster_run_dijkstra(floor, 1, floor->tunnelerView);

    *x = cheapestX;
    *y = cheapestY;
}

/*
 * MONSTER 6
 *      INTELLIGENT     = 0
 *      TELEPATHIC      = 1
 *      TUNNELER        = 1
 *      ERRATIC         = 0
 *
 * This monster is telepathic and a tunneler. It will attempt to go straight towards the player.
 */
void monster_move_6(Monster* monster, u_char* x, u_char* y) {
    Floor* floor = monster->character->floor;

    char deltaX = floor->dungeon->player->character->x - monster->character->x;
    char deltaY = floor->dungeon->player->character->y - monster->character->y;

    if (deltaY == 0) { // Move in the x direction
        *x = monster->character->x + get_sign(deltaX);
        *y = monster->character->y;
    } else if (abs(deltaX) / abs(deltaY) > 1.25) { // Move in the x direction?
        *x = monster->character->x + get_sign(deltaX);
        *y = monster->character->y;
    } else if (abs(deltaX) / abs(deltaY) < 0.75) { // Move in the y direction?
        *x = monster->character->x;
        *y = monster->character->y + get_sign(deltaY);
    } else { // Move in a diagonal?
        *x = monster->character->x + get_sign(deltaX);
        *y = monster->character->y + get_sign(deltaY);
    }

    // Cant tunnel
    if (floor->terrains[*y][*x]->isImmutable) {
        *x = monster->character->x;
        *y = monster->character->y;
    }
}

/*
 * MONSTER 7
 *      INTELLIGENT     = 1
 *      TELEPATHIC      = 1
 *      TUNNELER        = 1
 *      ERRATIC         = 0
 *
 * This monster is intelligent, telepathic, and smart. It will seek out the player using the tunneler path
 */
void monster_move_7(Monster* monster, u_char* x, u_char* y) {
    Floor* floor = monster->character->floor;

    // Default stay right where they are
    u_char cheapestX = monster->character->x;
    u_char cheapestY = monster->character->y;
    // Position values
    u_char tempX;
    u_char tempY;
    // Record cheapest cost
    u_char cheapestCost = U_CHAR_MAX;
    for (tempY = monster->character->y - 1; tempY <= monster->character->y + 1; tempY++) {
        for (tempX = monster->character->x - 1; tempX <= monster->character->x + 1; tempX++) {
            if (floor->tunnelerView[tempY][tempX] <= cheapestCost) {
                cheapestX = tempX;
                cheapestY = tempY;
                cheapestCost = floor->tunnelerView[tempY][tempX];
            }
        }
    }

    *x = cheapestX;
    *y = cheapestY;
}

/*
 * MONSTER 8
 *      INTELLIGENT     = 0
 *      TELEPATHIC      = 0
 *      TUNNELER        = 0
 *      ERRATIC         = 1
 *
 * This monster is erratic. There will be probability involved here
 * 50% To move via other characteristics
 * 45% Chance to move to a neighboring cell
 * 3% Chance to teleport to a random location
 * 2% Chance evolve another characteristic, but not move
 */
void monster_move_8(Monster* monster, u_char* x, u_char* y) {
    Floor* floor = monster->character->floor;
    u_char random = random_number_between(0, 100);

    if (random < EVOLVE_CHANCE) {
        u_char randomCharacteristic = random_number_between(1, 4);

        monster->classification |= randomCharacteristic;
        monster->character->character = convert_base10_to_char(monster->classification);
        monster->level =
                (monster_is_intelligent(monster) ? MONSTER_INTELLIGENT_LEVEL : 0) +
                (monster_is_telepathic(monster) ? MONSTER_TELEPATHIC_LEVEL : 0) +
                (monster_is_tunneler(monster) ? MONSTER_TUNNELER_LEVEL : 0) +
                (monster_is_erratic(monster) ? MONSTER_ERRATIC_LEVEL : 0) +
                1;

        dungeon_prepend_message(monster->character->floor->dungeon, "A monster has evolved");

        *x = monster->character->x;
        *y = monster->character->y;
    } else if (random < TELEPORT_CHANCE) {
        u_char monsterX;
        u_char monsterY;
        u_char height;
        u_char width;
        u_char placementAttempts = 0;
        u_char roomIndex = 0;
        u_char monsterRoom = random_number_between(0, floor->roomCount - 1);

        do {
            // Select random spot inside the room
            monsterX = random_number_between(floor->rooms[monsterRoom]->startX, floor->rooms[monsterRoom]->startX + floor->rooms[monsterRoom]->width - 1);
            monsterY = random_number_between(floor->rooms[monsterRoom]->startY, floor->rooms[monsterRoom]->startY + floor->rooms[monsterRoom]->height - 1);

            placementAttempts++;
        } while (floor->characters[monsterY][monsterX] != null && placementAttempts < 25);

        // If failed to find, just man handle it through
        if (placementAttempts >= 25) {
            for (roomIndex = 0; roomIndex < floor->roomCount; roomIndex++) {
                // Start looping and find the next open spot
                for (height = floor->rooms[roomIndex]->startY; height < floor->rooms[roomIndex]->startY + floor->rooms[roomIndex]->height; height++) {
                    for (width = floor->rooms[roomIndex]->startX; width < floor->rooms[roomIndex]->startX + floor->rooms[roomIndex]->width; width++) {
                        if (floor->characters[height][width] == null) {
                            monsterX = width;
                            monsterY = height;
                        }
                    }
                }
            }
        }

        *x = monsterX;
        *y = monsterY;
    } else if (random < RANDOM_MOVE_CHANCE) {
        monster_move_0(monster, x, y);
    } else if (random < OTHER_CHARACTERISTIC_CHANCE) {
        monster_move_0(monster, x, y);
    } else {
        *x = monster->character->x;
        *y = monster->character->y;
    }
}

/*
 * MONSTER 9
 *      INTELLIGENT     = 1
 *      TELEPATHIC      = 0
 *      TUNNELER        = 0
 *      ERRATIC         = 1
 *
 * This monster is erratic. There will be probability involved here
 * 50% To move via other characteristics
 * 45% Chance to move to a neighboring cell
 * 3% Chance to teleport to a random location
 * 2% Chance evolve another characteristic, but not move
 */
void monster_move_9(Monster* monster, u_char* x, u_char* y) {
    Floor* floor = monster->character->floor;
    u_char random = random_number_between(0, 100);

    if (random < EVOLVE_CHANCE) {
        u_char randomCharacteristic = random_number_between(1, 4);

        monster->classification |= randomCharacteristic;
        monster->character->character = convert_base10_to_char(monster->classification);
        monster->level =
                (monster_is_intelligent(monster) ? MONSTER_INTELLIGENT_LEVEL : 0) +
                (monster_is_telepathic(monster) ? MONSTER_TELEPATHIC_LEVEL : 0) +
                (monster_is_tunneler(monster) ? MONSTER_TUNNELER_LEVEL : 0) +
                (monster_is_erratic(monster) ? MONSTER_ERRATIC_LEVEL : 0) +
                1;

        dungeon_prepend_message(monster->character->floor->dungeon, "A monster has evolved");

        *x = monster->character->x;
        *y = monster->character->y;
    } else if (random < TELEPORT_CHANCE) {
        u_char monsterX;
        u_char monsterY;
        u_char height;
        u_char width;
        u_char placementAttempts = 0;
        u_char roomIndex = 0;
        u_char monsterRoom = random_number_between(0, floor->roomCount - 1);

        do {
            // Select random spot inside the room
            monsterX = random_number_between(floor->rooms[monsterRoom]->startX, floor->rooms[monsterRoom]->startX + floor->rooms[monsterRoom]->width - 1);
            monsterY = random_number_between(floor->rooms[monsterRoom]->startY, floor->rooms[monsterRoom]->startY + floor->rooms[monsterRoom]->height - 1);

            placementAttempts++;
        } while (floor->characters[monsterY][monsterX] != null && placementAttempts < 25);

        // If failed to find, just man handle it through
        if (placementAttempts >= 25) {
            for (roomIndex = 0; roomIndex < floor->roomCount; roomIndex++) {
                // Start looping and find the next open spot
                for (height = floor->rooms[roomIndex]->startY; height < floor->rooms[roomIndex]->startY + floor->rooms[roomIndex]->height; height++) {
                    for (width = floor->rooms[roomIndex]->startX; width < floor->rooms[roomIndex]->startX + floor->rooms[roomIndex]->width; width++) {
                        if (floor->characters[height][width] == null) {
                            monsterX = width;
                            monsterY = height;
                        }
                    }
                }
            }
        }

        *x = monsterX;
        *y = monsterY;
    } else if (random < RANDOM_MOVE_CHANCE) {
        monster_move_0(monster, x, y);
    } else if (random < OTHER_CHARACTERISTIC_CHANCE) {
        monster_move_1(monster, x, y);
    } else {
        *x = monster->character->x;
        *y = monster->character->y;
    }
}

/*
 * MONSTER 10
 *      INTELLIGENT     = 0
 *      TELEPATHIC      = 1
 *      TUNNELER        = 0
 *      ERRATIC         = 1
 *
 * This monster is erratic. There will be probability involved here
 * 50% To move via other characteristics
 * 45% Chance to move to a neighboring cell
 * 3% Chance to teleport to a random location
 * 2% Chance evolve another characteristic, but not move
 */
void monster_move_10(Monster* monster, u_char* x, u_char* y) {
    Floor* floor = monster->character->floor;
    u_char random = random_number_between(0, 100);

    if (random < EVOLVE_CHANCE) {
        u_char randomCharacteristic = random_number_between(1, 4);

        monster->classification |= randomCharacteristic;
        monster->character->character = convert_base10_to_char(monster->classification);
        monster->level =
                (monster_is_intelligent(monster) ? MONSTER_INTELLIGENT_LEVEL : 0) +
                (monster_is_telepathic(monster) ? MONSTER_TELEPATHIC_LEVEL : 0) +
                (monster_is_tunneler(monster) ? MONSTER_TUNNELER_LEVEL : 0) +
                (monster_is_erratic(monster) ? MONSTER_ERRATIC_LEVEL : 0) +
                1;

        dungeon_prepend_message(monster->character->floor->dungeon, "A monster has evolved");

        *x = monster->character->x;
        *y = monster->character->y;
    } else if (random < TELEPORT_CHANCE) {
        u_char monsterX;
        u_char monsterY;
        u_char height;
        u_char width;
        u_char placementAttempts = 0;
        u_char roomIndex = 0;
        u_char monsterRoom = random_number_between(0, floor->roomCount - 1);

        do {
            // Select random spot inside the room
            monsterX = random_number_between(floor->rooms[monsterRoom]->startX, floor->rooms[monsterRoom]->startX + floor->rooms[monsterRoom]->width - 1);
            monsterY = random_number_between(floor->rooms[monsterRoom]->startY, floor->rooms[monsterRoom]->startY + floor->rooms[monsterRoom]->height - 1);

            placementAttempts++;
        } while (floor->characters[monsterY][monsterX] != null && placementAttempts < 25);

        // If failed to find, just man handle it through
        if (placementAttempts >= 25) {
            for (roomIndex = 0; roomIndex < floor->roomCount; roomIndex++) {
                // Start looping and find the next open spot
                for (height = floor->rooms[roomIndex]->startY; height < floor->rooms[roomIndex]->startY + floor->rooms[roomIndex]->height; height++) {
                    for (width = floor->rooms[roomIndex]->startX; width < floor->rooms[roomIndex]->startX + floor->rooms[roomIndex]->width; width++) {
                        if (floor->characters[height][width] == null) {
                            monsterX = width;
                            monsterY = height;
                        }
                    }
                }
            }
        }

        *x = monsterX;
        *y = monsterY;
    } else if (random < RANDOM_MOVE_CHANCE) {
        monster_move_0(monster, x, y);
    } else if (random < OTHER_CHARACTERISTIC_CHANCE) {
        monster_move_2(monster, x, y);
    } else {
        *x = monster->character->x;
        *y = monster->character->y;
    }
}

/*
 * MONSTER 11
 *      INTELLIGENT     = 1
 *      TELEPATHIC      = 1
 *      TUNNELER        = 0
 *      ERRATIC         = 1
 *
 * This monster is erratic. There will be probability involved here
 * 50% To move via other characteristics
 * 45% Chance to move to a neighboring cell
 * 3% Chance to teleport to a random location
 * 2% Chance evolve another characteristic, but not move
 */
void monster_move_11(Monster* monster, u_char* x, u_char* y) {
    Floor* floor = monster->character->floor;
    u_char random = random_number_between(0, 100);

    if (random < EVOLVE_CHANCE) {
        u_char randomCharacteristic = random_number_between(1, 4);

        monster->classification |= randomCharacteristic;
        monster->character->character = convert_base10_to_char(monster->classification);
        monster->level =
                (monster_is_intelligent(monster) ? MONSTER_INTELLIGENT_LEVEL : 0) +
                (monster_is_telepathic(monster) ? MONSTER_TELEPATHIC_LEVEL : 0) +
                (monster_is_tunneler(monster) ? MONSTER_TUNNELER_LEVEL : 0) +
                (monster_is_erratic(monster) ? MONSTER_ERRATIC_LEVEL : 0) +
                1;

        dungeon_prepend_message(monster->character->floor->dungeon, "A monster has evolved");

        *x = monster->character->x;
        *y = monster->character->y;
    } else if (random < TELEPORT_CHANCE) {
        u_char monsterX;
        u_char monsterY;
        u_char height;
        u_char width;
        u_char placementAttempts = 0;
        u_char roomIndex = 0;
        u_char monsterRoom = random_number_between(0, floor->roomCount - 1);

        do {
            // Select random spot inside the room
            monsterX = random_number_between(floor->rooms[monsterRoom]->startX, floor->rooms[monsterRoom]->startX + floor->rooms[monsterRoom]->width - 1);
            monsterY = random_number_between(floor->rooms[monsterRoom]->startY, floor->rooms[monsterRoom]->startY + floor->rooms[monsterRoom]->height - 1);

            placementAttempts++;
        } while (floor->characters[monsterY][monsterX] != null && placementAttempts < 25);

        // If failed to find, just man handle it through
        if (placementAttempts >= 25) {
            for (roomIndex = 0; roomIndex < floor->roomCount; roomIndex++) {
                // Start looping and find the next open spot
                for (height = floor->rooms[roomIndex]->startY; height < floor->rooms[roomIndex]->startY + floor->rooms[roomIndex]->height; height++) {
                    for (width = floor->rooms[roomIndex]->startX; width < floor->rooms[roomIndex]->startX + floor->rooms[roomIndex]->width; width++) {
                        if (floor->characters[height][width] == null) {
                            monsterX = width;
                            monsterY = height;
                        }
                    }
                }
            }
        }

        *x = monsterX;
        *y = monsterY;
    } else if (random < RANDOM_MOVE_CHANCE) {
        monster_move_0(monster, x, y);
    } else if (random < OTHER_CHARACTERISTIC_CHANCE) {
        monster_move_3(monster, x, y);
    } else {
        *x = monster->character->x;
        *y = monster->character->y;
    }
}

/*
 * MONSTER 12
 *      INTELLIGENT     = 0
 *      TELEPATHIC      = 0
 *      TUNNELER        = 1
 *      ERRATIC         = 1
 *
 * This monster is erratic. There will be probability involved here
 * 50% To move via other characteristics
 * 45% Chance to move to a neighboring cell
 * 3% Chance to teleport to a random location
 * 2% Chance evolve another characteristic, but not move
 */
void monster_move_12(Monster* monster, u_char* x, u_char* y) {
    Floor* floor = monster->character->floor;
    u_char random = random_number_between(0, 100);

    if (random < EVOLVE_CHANCE) {
        u_char randomCharacteristic = random_number_between(1, 4);

        monster->classification |= randomCharacteristic;
        monster->character->character = convert_base10_to_char(monster->classification);
        monster->level =
                (monster_is_intelligent(monster) ? MONSTER_INTELLIGENT_LEVEL : 0) +
                (monster_is_telepathic(monster) ? MONSTER_TELEPATHIC_LEVEL : 0) +
                (monster_is_tunneler(monster) ? MONSTER_TUNNELER_LEVEL : 0) +
                (monster_is_erratic(monster) ? MONSTER_ERRATIC_LEVEL : 0) +
                1;

        dungeon_prepend_message(monster->character->floor->dungeon, "A monster has evolved");

        *x = monster->character->x;
        *y = monster->character->y;
    } else if (random < TELEPORT_CHANCE) {
        u_char monsterX;
        u_char monsterY;
        u_char height;
        u_char width;
        u_char placementAttempts = 0;
        u_char roomIndex = 0;
        u_char monsterRoom = random_number_between(0, floor->roomCount - 1);

        do {
            // Select random spot inside the room
            monsterX = random_number_between(floor->rooms[monsterRoom]->startX, floor->rooms[monsterRoom]->startX + floor->rooms[monsterRoom]->width - 1);
            monsterY = random_number_between(floor->rooms[monsterRoom]->startY, floor->rooms[monsterRoom]->startY + floor->rooms[monsterRoom]->height - 1);

            placementAttempts++;
        } while (floor->characters[monsterY][monsterX] != null && placementAttempts < 25);

        // If failed to find, just man handle it through
        if (placementAttempts >= 25) {
            for (roomIndex = 0; roomIndex < floor->roomCount; roomIndex++) {
                // Start looping and find the next open spot
                for (height = floor->rooms[roomIndex]->startY; height < floor->rooms[roomIndex]->startY + floor->rooms[roomIndex]->height; height++) {
                    for (width = floor->rooms[roomIndex]->startX; width < floor->rooms[roomIndex]->startX + floor->rooms[roomIndex]->width; width++) {
                        if (floor->characters[height][width] == null) {
                            monsterX = width;
                            monsterY = height;
                        }
                    }
                }
            }
        }

        *x = monsterX;
        *y = monsterY;
    } else if (random < RANDOM_MOVE_CHANCE) {
        monster_move_0(monster, x, y);
    } else if (random < OTHER_CHARACTERISTIC_CHANCE) {
        monster_move_4(monster, x, y);
    } else {
        *x = monster->character->x;
        *y = monster->character->y;
    }
}

/*
 * MONSTER 13
 *      INTELLIGENT     = 1
 *      TELEPATHIC      = 0
 *      TUNNELER        = 1
 *      ERRATIC         = 1
 *
 * This monster is erratic. There will be probability involved here
 * 50% To move via other characteristics
 * 45% Chance to move to a neighboring cell
 * 3% Chance to teleport to a random location
 * 2% Chance evolve another characteristic, but not move
 */
void monster_move_13(Monster* monster, u_char* x, u_char* y) {
    Floor* floor = monster->character->floor;
    u_char random = random_number_between(0, 100);

    if (random < EVOLVE_CHANCE) {
        u_char randomCharacteristic = random_number_between(1, 4);

        monster->classification |= randomCharacteristic;
        monster->character->character = convert_base10_to_char(monster->classification);
        monster->level =
                (monster_is_intelligent(monster) ? MONSTER_INTELLIGENT_LEVEL : 0) +
                (monster_is_telepathic(monster) ? MONSTER_TELEPATHIC_LEVEL : 0) +
                (monster_is_tunneler(monster) ? MONSTER_TUNNELER_LEVEL : 0) +
                (monster_is_erratic(monster) ? MONSTER_ERRATIC_LEVEL : 0) +
                1;

        dungeon_prepend_message(monster->character->floor->dungeon, "A monster has evolved");

        *x = monster->character->x;
        *y = monster->character->y;
    } else if (random < TELEPORT_CHANCE) {
        u_char monsterX;
        u_char monsterY;
        u_char height;
        u_char width;
        u_char placementAttempts = 0;
        u_char roomIndex = 0;
        u_char monsterRoom = random_number_between(0, floor->roomCount - 1);

        do {
            // Select random spot inside the room
            monsterX = random_number_between(floor->rooms[monsterRoom]->startX, floor->rooms[monsterRoom]->startX + floor->rooms[monsterRoom]->width - 1);
            monsterY = random_number_between(floor->rooms[monsterRoom]->startY, floor->rooms[monsterRoom]->startY + floor->rooms[monsterRoom]->height - 1);

            placementAttempts++;
        } while (floor->characters[monsterY][monsterX] != null && placementAttempts < 25);

        // If failed to find, just man handle it through
        if (placementAttempts >= 25) {
            for (roomIndex = 0; roomIndex < floor->roomCount; roomIndex++) {
                // Start looping and find the next open spot
                for (height = floor->rooms[roomIndex]->startY; height < floor->rooms[roomIndex]->startY + floor->rooms[roomIndex]->height; height++) {
                    for (width = floor->rooms[roomIndex]->startX; width < floor->rooms[roomIndex]->startX + floor->rooms[roomIndex]->width; width++) {
                        if (floor->characters[height][width] == null) {
                            monsterX = width;
                            monsterY = height;
                        }
                    }
                }
            }
        }

        *x = monsterX;
        *y = monsterY;
    } else if (random < RANDOM_MOVE_CHANCE) {
        monster_move_0(monster, x, y);
    } else if (random < OTHER_CHARACTERISTIC_CHANCE) {
        monster_move_5(monster, x, y);
    } else {
        *x = monster->character->x;
        *y = monster->character->y;
    }
}

/*
 * MONSTER 14
 *      INTELLIGENT     = 0
 *      TELEPATHIC      = 1
 *      TUNNELER        = 1
 *      ERRATIC         = 1
 *
 * This monster is erratic. There will be probability involved here
 * 50% To move via other characteristics
 * 45% Chance to move to a neighboring cell
 * 3% Chance to teleport to a random location
 * 2% Chance evolve another characteristic, but not move
 */
void monster_move_14(Monster* monster, u_char* x, u_char* y) {
    Floor* floor = monster->character->floor;
    u_char random = random_number_between(0, 100);

    if (random < EVOLVE_CHANCE) {
        u_char randomCharacteristic = random_number_between(1, 4);

        monster->classification |= randomCharacteristic;
        monster->character->character = convert_base10_to_char(monster->classification);
        monster->level =
                (monster_is_intelligent(monster) ? MONSTER_INTELLIGENT_LEVEL : 0) +
                (monster_is_telepathic(monster) ? MONSTER_TELEPATHIC_LEVEL : 0) +
                (monster_is_tunneler(monster) ? MONSTER_TUNNELER_LEVEL : 0) +
                (monster_is_erratic(monster) ? MONSTER_ERRATIC_LEVEL : 0) +
                1;

        dungeon_prepend_message(monster->character->floor->dungeon, "A monster has evolved");

        *x = monster->character->x;
        *y = monster->character->y;
    } else if (random < TELEPORT_CHANCE) {
        u_char monsterX;
        u_char monsterY;
        u_char height;
        u_char width;
        u_char placementAttempts = 0;
        u_char roomIndex = 0;
        u_char monsterRoom = random_number_between(0, floor->roomCount - 1);

        do {
            // Select random spot inside the room
            monsterX = random_number_between(floor->rooms[monsterRoom]->startX, floor->rooms[monsterRoom]->startX + floor->rooms[monsterRoom]->width - 1);
            monsterY = random_number_between(floor->rooms[monsterRoom]->startY, floor->rooms[monsterRoom]->startY + floor->rooms[monsterRoom]->height - 1);

            placementAttempts++;
        } while (floor->characters[monsterY][monsterX] != null && placementAttempts < 25);

        // If failed to find, just man handle it through
        if (placementAttempts >= 25) {
            for (roomIndex = 0; roomIndex < floor->roomCount; roomIndex++) {
                // Start looping and find the next open spot
                for (height = floor->rooms[roomIndex]->startY; height < floor->rooms[roomIndex]->startY + floor->rooms[roomIndex]->height; height++) {
                    for (width = floor->rooms[roomIndex]->startX; width < floor->rooms[roomIndex]->startX + floor->rooms[roomIndex]->width; width++) {
                        if (floor->characters[height][width] == null) {
                            monsterX = width;
                            monsterY = height;
                        }
                    }
                }
            }
        }

        *x = monsterX;
        *y = monsterY;
    } else if (random < RANDOM_MOVE_CHANCE) {
        monster_move_0(monster, x, y);
    } else if (random < OTHER_CHARACTERISTIC_CHANCE) {
        monster_move_6(monster, x, y);
    } else {
        *x = monster->character->x;
        *y = monster->character->y;
    }
}

/*
 * MONSTER 15
 *      INTELLIGENT     = 1
 *      TELEPATHIC      = 1
 *      TUNNELER        = 1
 *      ERRATIC         = 1
 *
 * This monster is erratic. There will be probability involved here
 * 50% To move via other characteristics
 * 45% Chance to move to a neighboring cell
 * 3% Chance to teleport to a random location
 * 2% Chance evolve another characteristic, but not move
 */
void monster_move_15(Monster* monster, u_char* x, u_char* y) {
    Floor* floor = monster->character->floor;
    u_char random = random_number_between(0, 100);

    if (random < EVOLVE_CHANCE) {
        u_char randomCharacteristic = random_number_between(1, 4);

        monster->classification |= randomCharacteristic;
        monster->character->character = convert_base10_to_char(monster->classification);
        monster->level =
                (monster_is_intelligent(monster) ? MONSTER_INTELLIGENT_LEVEL : 0) +
                (monster_is_telepathic(monster) ? MONSTER_TELEPATHIC_LEVEL : 0) +
                (monster_is_tunneler(monster) ? MONSTER_TUNNELER_LEVEL : 0) +
                (monster_is_erratic(monster) ? MONSTER_ERRATIC_LEVEL : 0) +
                1;

        dungeon_prepend_message(monster->character->floor->dungeon, "A monster has evolved");

        *x = monster->character->x;
        *y = monster->character->y;
    } else if (random < TELEPORT_CHANCE) {
        u_char monsterX;
        u_char monsterY;
        u_char height;
        u_char width;
        u_char placementAttempts = 0;
        u_char roomIndex = 0;
        u_char monsterRoom = random_number_between(0, floor->roomCount - 1);

        do {
            // Select random spot inside the room
            monsterX = random_number_between(floor->rooms[monsterRoom]->startX, floor->rooms[monsterRoom]->startX + floor->rooms[monsterRoom]->width - 1);
            monsterY = random_number_between(floor->rooms[monsterRoom]->startY, floor->rooms[monsterRoom]->startY + floor->rooms[monsterRoom]->height - 1);

            placementAttempts++;
        } while (floor->characters[monsterY][monsterX] != null && placementAttempts < 25);

        // If failed to find, just man handle it through
        if (placementAttempts >= 25) {
            for (roomIndex = 0; roomIndex < floor->roomCount; roomIndex++) {
                // Start looping and find the next open spot
                for (height = floor->rooms[roomIndex]->startY; height < floor->rooms[roomIndex]->startY + floor->rooms[roomIndex]->height; height++) {
                    for (width = floor->rooms[roomIndex]->startX; width < floor->rooms[roomIndex]->startX + floor->rooms[roomIndex]->width; width++) {
                        if (floor->characters[height][width] == null) {
                            monsterX = width;
                            monsterY = height;
                        }
                    }
                }
            }
        }

        *x = monsterX;
        *y = monsterY;
    } else if (random < RANDOM_MOVE_CHANCE) {
        monster_move_0(monster, x, y);
    } else if (random < OTHER_CHARACTERISTIC_CHANCE) {
        monster_move_7(monster, x, y);
    } else {
        *x = monster->character->x;
        *y = monster->character->y;
    }
}