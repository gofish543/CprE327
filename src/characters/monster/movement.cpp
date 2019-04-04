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
void Monster::Move0(Monster* monster, u_char* x, u_char* y) {
    Floor* floor = monster->getFloor();

    if (monster->isTunneler()) {
        do {
            *x = monster->getX();
            *y = monster->getY();

            *x = (*x + u_char(Dice::RandomNumberBetween(-1, 1)));
            *y = (*y + u_char(Dice::RandomNumberBetween(-1, 1)));
        } while (floor->getTerrainAt(*x, *y)->isImmutable());
    } else {
        do {
            *x = monster->getX();
            *y = monster->getY();

            *x = (*x + u_char(Dice::RandomNumberBetween(-1, 1)));
            *y = (*y + u_char(Dice::RandomNumberBetween(-1, 1)));
        } while (!floor->getTerrainAt(*x, *y)->isWalkable() || floor->getTerrainAt(*x, *y)->isImmutable());
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
void Monster::Move1(Monster* monster, u_char* x, u_char* y) {
    // If the monster has line of sight, store previous location
    if (monster->hasLineOfSightTo(monster->getFloor()->getDungeon()->getPlayer())) {
        monster->setPlayerLastSpottedX(monster->getFloor()->getDungeon()->getPlayer()->getX());
        monster->setPlayerLastSpottedY(monster->getFloor()->getDungeon()->getPlayer()->getY());
    } else if ((monster->getPlayerLastSpottedY() == 0 && monster->getPlayerLastSpottedX() == 0) ||
               (monster->getX() == monster->getPlayerLastSpottedX() && monster->getY() == monster->getPlayerLastSpottedY())) {
        // No previous location found, just stay here
        // Previous player location is our location, stay here
        *x = monster->getX();
        *y = monster->getY();
        return;
    }

    Floor* floor = monster->getFloor();
    Player* player = floor->getDungeon()->getPlayer();

    u_char playerX = player->getX();
    u_char playerY = player->getY();

    // "Move" the player
    player->setX(monster->getPlayerLastSpottedX());
    player->setY(monster->getPlayerLastSpottedY());
    // Run dijstra for just the non tunneler view
    Monster::RunDijkstra(floor, -1, floor->nonTunnelerView);

    // Default stay right where they are
    u_char cheapestX = monster->getX();
    u_char cheapestY = monster->getY();
    // Position values
    u_char tempX;
    u_char tempY;
    // Record cheapest cost
    u_char cheapestCost = U_CHAR_MAX;
    for (tempY = monster->getY() - u_char(1); tempY <= monster->getY() + 1; tempY++) {
        for (tempX = monster->getX() - u_char(1); tempX <= monster->getX() + 1; tempX++) {
            if (floor->nonTunnelerView[tempY][tempX] <= cheapestCost) {
                cheapestX = tempX;
                cheapestY = tempY;
                cheapestCost = floor->nonTunnelerView[tempY][tempX];
            }
        }
    }

    // "Move" player back and rerun dijkstra
    player->setX(playerX);
    player->setY(playerY);
    Monster::RunDijkstra(floor, -1, floor->nonTunnelerView);

    if (floor->getTerrainAt(cheapestX, cheapestY)->isWalkable()) {
        *x = cheapestX;
        *y = cheapestY;
    } else {
        *x = monster->getX();
        *y = monster->getY();
    }
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
void Monster::Move2(Monster* monster, u_char* x, u_char* y) {
    Floor* floor = monster->getFloor();

    char deltaX = floor->getDungeon()->getPlayer()->getX() - monster->getX();
    char deltaY = floor->getDungeon()->getPlayer()->getY() - monster->getY();

    if (abs(deltaX) > abs(deltaY)) { // Move in the x direction?
        *x = u_char(monster->getX() + get_sign(deltaX));
        *y = u_char(monster->getY());
    } else if (abs(deltaX) < abs(deltaY)) { // Move in the y direction?
        *x = u_char(monster->getX());
        *y = u_char(monster->getY() + get_sign(deltaY));
    } else { // Move in a diagonal?
        *x = u_char(monster->getX() + get_sign(deltaX));
        *y = u_char(monster->getY() + get_sign(deltaY));
    }

    // Cant tunnel
    if (floor->getTerrainAt(*x, *y)->isRock() || floor->getTerrainAt(*x, *y)->isImmutable()) {
        *x = monster->getX();
        *y = monster->getY();
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
void Monster::Move3(Monster* monster, u_char* x, u_char* y) {
    Floor* floor = monster->getFloor();

    // Default stay right where they are
    u_char cheapestX = monster->getX();
    u_char cheapestY = monster->getY();
    // Position values
    u_char tempX;
    u_char tempY;
    // Record cheapest cost
    u_char cheapestCost = U_CHAR_MAX;
    for (tempY = monster->getY() - u_char(1); tempY <= monster->getY() + 1; tempY++) {
        for (tempX = monster->getX() - u_char(1); tempX <= monster->getX() + 1; tempX++) {
            if (floor->nonTunnelerView[tempY][tempX] <= cheapestCost) {
                cheapestX = tempX;
                cheapestY = tempY;
                cheapestCost = floor->nonTunnelerView[tempY][tempX];
            }
        }
    }

    if (floor->getTerrainAt(cheapestX, cheapestY)->isWalkable()) {
        *x = cheapestX;
        *y = cheapestY;
    } else {
        *x = monster->getX();
        *y = monster->getY();
    }
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
void Monster::Move4(Monster* monster, u_char* x, u_char* y) {
    // If the monster has line of sight, store previous location
    if (monster->hasLineOfSightTo(monster->getFloor()->getDungeon()->getPlayer())) {
        monster->setPlayerLastSpottedX(monster->getFloor()->getDungeon()->getPlayer()->getX());
        monster->setPlayerLastSpottedY(monster->getFloor()->getDungeon()->getPlayer()->getY());
    } else if ((monster->getPlayerLastSpottedY() == 0 && monster->getPlayerLastSpottedX() == 0) ||
               (monster->getX() == monster->getPlayerLastSpottedX() && monster->getY() == monster->getPlayerLastSpottedY())) {
        // No previous location found, just stay here
        // Previous player location is our location, stay here
        *x = monster->getX();
        *y = monster->getY();
        return;
    }

    Floor* floor = monster->getFloor();

    char deltaX = monster->getPlayerLastSpottedX() - monster->getX();
    char deltaY = monster->getPlayerLastSpottedY() - monster->getY();

    if (deltaY == 0) { // Move in the x direction
        *x = u_char(monster->getX() + get_sign(deltaX));
        *y = u_char(monster->getY());
    } else if (abs(deltaX) / double(abs(deltaY)) > 1.25) { // Move in the x direction?
        *x = u_char(monster->getX() + get_sign(deltaX));
        *y = u_char(monster->getY());
    } else if (abs(deltaX) / double(abs(deltaY)) < 0.75) { // Move in the y direction?
        *x = u_char(monster->getX());
        *y = u_char(monster->getY() + get_sign(deltaY));
    } else { // Move in a diagonal?
        *x = u_char(monster->getX() + get_sign(deltaX));
        *y = u_char(monster->getY() + get_sign(deltaY));
    }

    // Cant tunnel through immutable rock
    if (floor->getTerrainAt(*x, *y)->isImmutable()) {
        *x = monster->getX();
        *y = monster->getY();
    }
}

/*
 * MONSTER 5
 *      INTELLIGENT     = 1
 *      TELEPATHIC      = 0
 *      TUNNELER        = 1
 *      ERRATIC         = 0
 *
 * This monster is intelligent with tunneling and will move towards the player if it has line of sight or last spotted
 */
void Monster::Move5(Monster* monster, u_char* x, u_char* y) {
    // If the monster has line of sight, store previous location
    if (monster->hasLineOfSightTo(monster->getFloor()->getDungeon()->getPlayer())) {
        monster->setPlayerLastSpottedX(monster->getFloor()->getDungeon()->getPlayer()->getX());
        monster->setPlayerLastSpottedY(monster->getFloor()->getDungeon()->getPlayer()->getY());
    } else if ((monster->getPlayerLastSpottedY() == 0 && monster->getPlayerLastSpottedX() == 0) ||
               (monster->getX() == monster->getPlayerLastSpottedX() && monster->getY() == monster->getPlayerLastSpottedY())) {
        // No previous location found, just stay here
        // Previous player location is our location, stay here
        *x = monster->getX();
        *y = monster->getY();
        return;
    }

    Floor* floor = monster->getFloor();
    Player* player = floor->getDungeon()->getPlayer();

    u_char playerX = player->getX();
    u_char playerY = player->getY();

    // "Move" the player
    player->setX(monster->getPlayerLastSpottedX());
    player->setY(monster->getPlayerLastSpottedY());
    // Run dijstra for just the non tunneler view
    Monster::RunDijkstra(floor, 1, floor->tunnelerView);

    // Default stay right where they are
    u_char cheapestX = monster->getX();
    u_char cheapestY = monster->getY();
    // Position values
    u_char tempX;
    u_char tempY;
    // Record cheapest cost
    u_char cheapestCost = U_CHAR_MAX;
    for (tempY = monster->getY() - u_char(1); tempY <= monster->getY() + 1; tempY++) {
        for (tempX = monster->getX() - u_char(1); tempX <= monster->getX() + 1; tempX++) {
            if (floor->tunnelerView[tempY][tempX] <= cheapestCost) {
                cheapestX = tempX;
                cheapestY = tempY;
                cheapestCost = floor->tunnelerView[tempY][tempX];
            }
        }
    }

    // "Move" player back and rerun dijkstra
    player->setX(playerX);
    player->setY(playerY);
    Monster::RunDijkstra(floor, 1, floor->tunnelerView);

    if (floor->getTerrainAt(cheapestX, cheapestY)->isImmutable()) {
        *x = monster->getX();
        *y = monster->getY();
    } else {
        *x = cheapestX;
        *y = cheapestY;
    }
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
void Monster::Move6(Monster* monster, u_char* x, u_char* y) {
    Floor* floor = monster->getFloor();

    char deltaX = floor->getDungeon()->getPlayer()->getX() - monster->getX();
    char deltaY = floor->getDungeon()->getPlayer()->getY() - monster->getY();

    if (deltaY == 0) { // Move in the x direction
        *x = u_char(monster->getX() + get_sign(deltaX));
        *y = u_char(monster->getY());
    } else if (abs(deltaX) / double(abs(deltaY)) > 1.25) { // Move in the x direction?
        *x = u_char(monster->getX() + get_sign(deltaX));
        *y = u_char(monster->getY());
    } else if (abs(deltaX) / double(abs(deltaY)) < 0.75) { // Move in the y direction?
        *x = u_char(monster->getX());
        *y = u_char(monster->getY() + get_sign(deltaY));
    } else { // Move in a diagonal?
        *x = u_char(monster->getX() + get_sign(deltaX));
        *y = u_char(monster->getY() + get_sign(deltaY));
    }

    // Cant tunnel
    if (floor->getTerrainAt(*x, *y)->isImmutable()) {
        *x = monster->getX();
        *y = monster->getY();
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
void Monster::Move7(Monster* monster, u_char* x, u_char* y) {
    Floor* floor = monster->getFloor();

    // Default stay right where they are
    u_char cheapestX = monster->getX();
    u_char cheapestY = monster->getY();
    // Position values
    u_char tempX;
    u_char tempY;
    // Record cheapest cost
    u_char cheapestCost = U_CHAR_MAX;
    for (tempY = monster->getY() - u_char(1); tempY <= monster->getY() + 1; tempY++) {
        for (tempX = monster->getX() - u_char(1); tempX <= monster->getX() + 1; tempX++) {
            if (floor->tunnelerView[tempY][tempX] <= cheapestCost) {
                cheapestX = tempX;
                cheapestY = tempY;
                cheapestCost = floor->tunnelerView[tempY][tempX];
            }
        }
    }

    if (floor->getTerrainAt(cheapestX, cheapestY)->isImmutable()) {
        *x = monster->getX();
        *y = monster->getY();
    } else {
        *x = cheapestX;
        *y = cheapestY;
    }
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
void Monster::Move8(Monster* monster, u_char* x, u_char* y) {
    Floor* floor = monster->getFloor();
    auto random = u_char(Dice::RandomNumberBetween(0, 100));

    if (random < EVOLVE_CHANCE) {
        auto randomCharacteristic = u_char(Dice::RandomNumberBetween(1, 4));

        monster->setClassification(monster->getClassification() | randomCharacteristic);
        monster->setCharacter(convert_base10_to_char(monster->getClassification()));
//        monster->setLevel(
//                u_char(
//                        (monster->isIntelligent() ? MONSTER_INTELLIGENT_LEVEL : 0) +
//                        (monster->isTelepathic() ? MONSTER_TELEPATHIC_LEVEL : 0) +
//                        (monster->isTunneler() ? MONSTER_TUNNELER_LEVEL : 0) +
//                        (monster->isErratic() ? MONSTER_ERRATIC_LEVEL : 0) +
//                        1));

        std::string evolveFormat = "%s has evolved";
        floor->getDungeon()->prependText(&evolveFormat, monster->getName().c_str());

        *x = monster->getX();
        *y = monster->getY();
    } else if (random < TELEPORT_CHANCE) {
        u_char monsterX;
        u_char monsterY;
        u_char height;
        u_char width;
        u_char placementAttempts = 0;
        u_char roomIndex = 0;
        auto monsterRoom = u_char(Dice::RandomNumberBetween(0, floor->getRoomCount() - u_char(1)));

        do {
            // Select random spot inside the room
            monsterX = u_char(Dice::RandomNumberBetween(floor->getRoom(monsterRoom)->getStartingX(), floor->getRoom(monsterRoom)->getStartingX() + floor->getRoom(monsterRoom)->getWidth() - 1));
            monsterY = u_char(Dice::RandomNumberBetween(floor->getRoom(monsterRoom)->getStartingY(), floor->getRoom(monsterRoom)->getStartingY() + floor->getRoom(monsterRoom)->getHeight() - 1));

            placementAttempts++;
        } while (floor->getCharacterAt(monsterX, monsterY) != null && placementAttempts < 25);

        // If failed to find, just man handle it through
        if (placementAttempts >= 25) {
            for (roomIndex = 0; roomIndex < floor->getRoomCount(); roomIndex++) {
                // Start looping and find the next open spot
                for (height = floor->getRoom(monsterRoom)->getStartingY(); height < floor->getRoom(monsterRoom)->getStartingY() + floor->getRoom(monsterRoom)->getHeight(); height++) {
                    for (width = floor->getRoom(monsterRoom)->getStartingX(); width < floor->getRoom(monsterRoom)->getStartingX() + floor->getRoom(monsterRoom)->getWidth(); width++) {
                        if (floor->getCharacterAt(width, height) == null) {
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
        Monster::Move0(monster, x, y);
    } else if (random < OTHER_CHARACTERISTIC_CHANCE) {
        Monster::Move0(monster, x, y);
    } else {
        *x = monster->getX();
        *y = monster->getY();
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
void Monster::Move9(Monster* monster, u_char* x, u_char* y) {
    Floor* floor = monster->getFloor();
    auto random = u_char(Dice::RandomNumberBetween(0, 100));

    if (random < EVOLVE_CHANCE) {
        auto randomCharacteristic = u_char(Dice::RandomNumberBetween(1, 4));

        monster->setClassification(monster->getClassification() | randomCharacteristic);
        monster->setCharacter(convert_base10_to_char(monster->getClassification()));
//        monster->setLevel(
//                u_char(
//                        (monster->isIntelligent() ? MONSTER_INTELLIGENT_LEVEL : 0) +
//                        (monster->isTelepathic() ? MONSTER_TELEPATHIC_LEVEL : 0) +
//                        (monster->isTunneler() ? MONSTER_TUNNELER_LEVEL : 0) +
//                        (monster->isErratic() ? MONSTER_ERRATIC_LEVEL : 0) +
//                        1));

        std::string evolveFormat = "%s has evolved";
        floor->getDungeon()->prependText(&evolveFormat, monster->getName().c_str());
        *x = monster->getX();
        *y = monster->getY();
    } else if (random < TELEPORT_CHANCE) {
        u_char monsterX;
        u_char monsterY;
        u_char height;
        u_char width;
        u_char placementAttempts = 0;
        u_char roomIndex = 0;
        auto monsterRoom = u_char(Dice::RandomNumberBetween(0, floor->getRoomCount() - u_char(1)));

        do {
            // Select random spot inside the room
            monsterX = u_char(Dice::RandomNumberBetween(floor->getRoom(monsterRoom)->getStartingX(), floor->getRoom(monsterRoom)->getStartingX() + floor->getRoom(monsterRoom)->getWidth() - 1));
            monsterY = u_char(Dice::RandomNumberBetween(floor->getRoom(monsterRoom)->getStartingY(), floor->getRoom(monsterRoom)->getStartingY() + floor->getRoom(monsterRoom)->getHeight() - 1));

            placementAttempts++;
        } while (floor->getCharacterAt(monsterX, monsterY) != null && placementAttempts < 25);

        // If failed to find, just man handle it through
        if (placementAttempts >= 25) {
            for (roomIndex = 0; roomIndex < floor->getRoomCount(); roomIndex++) {
                // Start looping and find the next open spot
                for (height = floor->getRoom(monsterRoom)->getStartingY(); height < floor->getRoom(monsterRoom)->getStartingY() + floor->getRoom(monsterRoom)->getHeight(); height++) {
                    for (width = floor->getRoom(monsterRoom)->getStartingX(); width < floor->getRoom(monsterRoom)->getStartingX() + floor->getRoom(monsterRoom)->getWidth(); width++) {
                        if (floor->getCharacterAt(width, height) == null) {
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
        Monster::Move0(monster, x, y);
    } else if (random < OTHER_CHARACTERISTIC_CHANCE) {
        Monster::Move1(monster, x, y);
    } else {
        *x = monster->getX();
        *y = monster->getY();
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
void Monster::Move10(Monster* monster, u_char* x, u_char* y) {
    Floor* floor = monster->getFloor();
    auto random = u_char(Dice::RandomNumberBetween(0, 100));

    if (random < EVOLVE_CHANCE) {
        auto randomCharacteristic = u_char(Dice::RandomNumberBetween(1, 4));

        monster->setClassification(monster->getClassification() | randomCharacteristic);
        monster->setCharacter(convert_base10_to_char(monster->getClassification()));
//        monster->setLevel(
//                u_char(
//                        (monster->isIntelligent() ? MONSTER_INTELLIGENT_LEVEL : 0) +
//                        (monster->isTelepathic() ? MONSTER_TELEPATHIC_LEVEL : 0) +
//                        (monster->isTunneler() ? MONSTER_TUNNELER_LEVEL : 0) +
//                        (monster->isErratic() ? MONSTER_ERRATIC_LEVEL : 0) +
//                        1));

        std::string evolveFormat = "%s has evolved";
        floor->getDungeon()->prependText(&evolveFormat, monster->getName().c_str());
        *x = monster->getX();
        *y = monster->getY();
    } else if (random < TELEPORT_CHANCE) {
        u_char monsterX;
        u_char monsterY;
        u_char height;
        u_char width;
        u_char placementAttempts = 0;
        u_char roomIndex = 0;
        auto monsterRoom = u_char(Dice::RandomNumberBetween(0, floor->getRoomCount() - u_char(1)));

        do {
            // Select random spot inside the room
            monsterX = u_char(Dice::RandomNumberBetween(floor->getRoom(monsterRoom)->getStartingX(), floor->getRoom(monsterRoom)->getStartingX() + floor->getRoom(monsterRoom)->getWidth() - 1));
            monsterY = u_char(Dice::RandomNumberBetween(floor->getRoom(monsterRoom)->getStartingY(), floor->getRoom(monsterRoom)->getStartingY() + floor->getRoom(monsterRoom)->getHeight() - 1));

            placementAttempts++;
        } while (floor->getCharacterAt(monsterX, monsterY) != null && placementAttempts < 25);

        // If failed to find, just man handle it through
        if (placementAttempts >= 25) {
            for (roomIndex = 0; roomIndex < floor->getRoomCount(); roomIndex++) {
                // Start looping and find the next open spot
                for (height = floor->getRoom(monsterRoom)->getStartingY(); height < floor->getRoom(monsterRoom)->getStartingY() + floor->getRoom(monsterRoom)->getHeight(); height++) {
                    for (width = floor->getRoom(monsterRoom)->getStartingX(); width < floor->getRoom(monsterRoom)->getStartingX() + floor->getRoom(monsterRoom)->getWidth(); width++) {
                        if (floor->getCharacterAt(width, height) == null) {
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
        Monster::Move0(monster, x, y);
    } else if (random < OTHER_CHARACTERISTIC_CHANCE) {
        Monster::Move2(monster, x, y);
    } else {
        *x = monster->getX();
        *y = monster->getY();
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
void Monster::Move11(Monster* monster, u_char* x, u_char* y) {
    Floor* floor = monster->getFloor();
    auto random = u_char(Dice::RandomNumberBetween(0, 100));

    if (random < EVOLVE_CHANCE) {
        auto randomCharacteristic = u_char(Dice::RandomNumberBetween(1, 4));

        monster->setClassification(monster->getClassification() | randomCharacteristic);
        monster->setCharacter(convert_base10_to_char(monster->getClassification()));
//        monster->setLevel(
//                u_char(
//                        (monster->isIntelligent() ? MONSTER_INTELLIGENT_LEVEL : 0) +
//                        (monster->isTelepathic() ? MONSTER_TELEPATHIC_LEVEL : 0) +
//                        (monster->isTunneler() ? MONSTER_TUNNELER_LEVEL : 0) +
//                        (monster->isErratic() ? MONSTER_ERRATIC_LEVEL : 0) +
//                        1));

        std::string evolveFormat = "%s has evolved";
        floor->getDungeon()->prependText(&evolveFormat, monster->getName().c_str());

        *x = monster->getX();
        *y = monster->getY();
    } else if (random < TELEPORT_CHANCE) {
        u_char monsterX;
        u_char monsterY;
        u_char height;
        u_char width;
        u_char placementAttempts = 0;
        u_char roomIndex = 0;
        auto monsterRoom = u_char(Dice::RandomNumberBetween(0, floor->getRoomCount() - u_char(1)));

        do {
            // Select random spot inside the room
            monsterX = u_char(Dice::RandomNumberBetween(floor->getRoom(monsterRoom)->getStartingX(), floor->getRoom(monsterRoom)->getStartingX() + floor->getRoom(monsterRoom)->getWidth() - 1));
            monsterY = u_char(Dice::RandomNumberBetween(floor->getRoom(monsterRoom)->getStartingY(), floor->getRoom(monsterRoom)->getStartingY() + floor->getRoom(monsterRoom)->getHeight() - 1));

            placementAttempts++;
        } while (floor->getCharacterAt(monsterX, monsterY) != null && placementAttempts < 25);

        // If failed to find, just man handle it through
        if (placementAttempts >= 25) {
            for (roomIndex = 0; roomIndex < floor->getRoomCount(); roomIndex++) {
                // Start looping and find the next open spot
                for (height = floor->getRoom(monsterRoom)->getStartingY(); height < floor->getRoom(monsterRoom)->getStartingY() + floor->getRoom(monsterRoom)->getHeight(); height++) {
                    for (width = floor->getRoom(monsterRoom)->getStartingX(); width < floor->getRoom(monsterRoom)->getStartingX() + floor->getRoom(monsterRoom)->getWidth(); width++) {
                        if (floor->getCharacterAt(width, height) == null) {
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
        Monster::Move0(monster, x, y);
    } else if (random < OTHER_CHARACTERISTIC_CHANCE) {
        Monster::Move3(monster, x, y);
    } else {
        *x = monster->getX();
        *y = monster->getY();
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
void Monster::Move12(Monster* monster, u_char* x, u_char* y) {
    Floor* floor = monster->getFloor();
    auto random = u_char(Dice::RandomNumberBetween(0, 100));

    if (random < EVOLVE_CHANCE) {
        auto randomCharacteristic = u_char(Dice::RandomNumberBetween(1, 4));

        monster->setClassification(monster->getClassification() | randomCharacteristic);
        monster->setCharacter(convert_base10_to_char(monster->getClassification()));
//        monster->setLevel(
//                u_char(
//                        (monster->isIntelligent() ? MONSTER_INTELLIGENT_LEVEL : 0) +
//                        (monster->isTelepathic() ? MONSTER_TELEPATHIC_LEVEL : 0) +
//                        (monster->isTunneler() ? MONSTER_TUNNELER_LEVEL : 0) +
//                        (monster->isErratic() ? MONSTER_ERRATIC_LEVEL : 0) +
//                        1));

        std::string evolveFormat = "%s has evolved";
        floor->getDungeon()->prependText(&evolveFormat, monster->getName().c_str());

        *x = monster->getX();
        *y = monster->getY();
    } else if (random < TELEPORT_CHANCE) {
        u_char monsterX;
        u_char monsterY;
        u_char height;
        u_char width;
        u_char placementAttempts = 0;
        u_char roomIndex = 0;
        auto monsterRoom = u_char(Dice::RandomNumberBetween(0, floor->getRoomCount() - u_char(1)));

        do {
            // Select random spot inside the room
            monsterX = u_char(Dice::RandomNumberBetween(floor->getRoom(monsterRoom)->getStartingX(), floor->getRoom(monsterRoom)->getStartingX() + floor->getRoom(monsterRoom)->getWidth() - 1));
            monsterY = u_char(Dice::RandomNumberBetween(floor->getRoom(monsterRoom)->getStartingY(), floor->getRoom(monsterRoom)->getStartingY() + floor->getRoom(monsterRoom)->getHeight() - 1));

            placementAttempts++;
        } while (floor->getCharacterAt(monsterX, monsterY) != null && placementAttempts < 25);

        // If failed to find, just man handle it through
        if (placementAttempts >= 25) {
            for (roomIndex = 0; roomIndex < floor->getRoomCount(); roomIndex++) {
                // Start looping and find the next open spot
                for (height = floor->getRoom(monsterRoom)->getStartingY(); height < floor->getRoom(monsterRoom)->getStartingY() + floor->getRoom(monsterRoom)->getHeight(); height++) {
                    for (width = floor->getRoom(monsterRoom)->getStartingX(); width < floor->getRoom(monsterRoom)->getStartingX() + floor->getRoom(monsterRoom)->getWidth(); width++) {
                        if (floor->getCharacterAt(width, height) == null) {
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
        Monster::Move0(monster, x, y);
    } else if (random < OTHER_CHARACTERISTIC_CHANCE) {
        Monster::Move4(monster, x, y);
    } else {
        *x = monster->getX();
        *y = monster->getY();
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
void Monster::Move13(Monster* monster, u_char* x, u_char* y) {
    Floor* floor = monster->getFloor();
    auto random = u_char(Dice::RandomNumberBetween(0, 100));

    if (random < EVOLVE_CHANCE) {
        auto randomCharacteristic = u_char(Dice::RandomNumberBetween(1, 4));

        monster->setClassification(monster->getClassification() | randomCharacteristic);
        monster->setCharacter(convert_base10_to_char(monster->getClassification()));
//        monster->setLevel(
//                u_char(
//                        (monster->isIntelligent() ? MONSTER_INTELLIGENT_LEVEL : 0) +
//                        (monster->isTelepathic() ? MONSTER_TELEPATHIC_LEVEL : 0) +
//                        (monster->isTunneler() ? MONSTER_TUNNELER_LEVEL : 0) +
//                        (monster->isErratic() ? MONSTER_ERRATIC_LEVEL : 0) +
//                        1));

        std::string evolveFormat = "%s has evolved";
        floor->getDungeon()->prependText(&evolveFormat, monster->getName().c_str());

        *x = monster->getX();
        *y = monster->getY();
    } else if (random < TELEPORT_CHANCE) {
        u_char monsterX;
        u_char monsterY;
        u_char height;
        u_char width;
        u_char placementAttempts = 0;
        u_char roomIndex = 0;
        auto monsterRoom = u_char(Dice::RandomNumberBetween(0, floor->getRoomCount() - u_char(1)));

        do {
            // Select random spot inside the room
            monsterX = u_char(Dice::RandomNumberBetween(floor->getRoom(monsterRoom)->getStartingX(), floor->getRoom(monsterRoom)->getStartingX() + floor->getRoom(monsterRoom)->getWidth() - 1));
            monsterY = u_char(Dice::RandomNumberBetween(floor->getRoom(monsterRoom)->getStartingY(), floor->getRoom(monsterRoom)->getStartingY() + floor->getRoom(monsterRoom)->getHeight() - 1));

            placementAttempts++;
        } while (floor->getCharacterAt(monsterX, monsterY) != null && placementAttempts < 25);

        // If failed to find, just man handle it through
        if (placementAttempts >= 25) {
            for (roomIndex = 0; roomIndex < floor->getRoomCount(); roomIndex++) {
                // Start looping and find the next open spot
                for (height = floor->getRoom(monsterRoom)->getStartingY(); height < floor->getRoom(monsterRoom)->getStartingY() + floor->getRoom(monsterRoom)->getHeight(); height++) {
                    for (width = floor->getRoom(monsterRoom)->getStartingX(); width < floor->getRoom(monsterRoom)->getStartingX() + floor->getRoom(monsterRoom)->getWidth(); width++) {
                        if (floor->getCharacterAt(width, height) == null) {
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
        Monster::Move0(monster, x, y);
    } else if (random < OTHER_CHARACTERISTIC_CHANCE) {
        Monster::Move5(monster, x, y);
    } else {
        *x = monster->getX();
        *y = monster->getY();
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
void Monster::Move14(Monster* monster, u_char* x, u_char* y) {
    Floor* floor = monster->getFloor();
    auto random = u_char(Dice::RandomNumberBetween(0, 100));

    if (random < EVOLVE_CHANCE) {
        auto randomCharacteristic = u_char(Dice::RandomNumberBetween(1, 4));

        monster->setClassification(monster->getClassification() | randomCharacteristic);
        monster->setCharacter(convert_base10_to_char(monster->getClassification()));
//        monster->setLevel(
//                u_char(
//                        (monster->isIntelligent() ? MONSTER_INTELLIGENT_LEVEL : 0) +
//                        (monster->isTelepathic() ? MONSTER_TELEPATHIC_LEVEL : 0) +
//                        (monster->isTunneler() ? MONSTER_TUNNELER_LEVEL : 0) +
//                        (monster->isErratic() ? MONSTER_ERRATIC_LEVEL : 0) +
//                        1));

        std::string evolveFormat = "%s has evolved";
        floor->getDungeon()->prependText(&evolveFormat, monster->getName().c_str());

        *x = monster->getX();
        *y = monster->getY();
    } else if (random < TELEPORT_CHANCE) {
        u_char monsterX;
        u_char monsterY;
        u_char height;
        u_char width;
        u_char placementAttempts = 0;
        u_char roomIndex = 0;
        auto monsterRoom = u_char(Dice::RandomNumberBetween(0, floor->getRoomCount() - u_char(1)));

        do {
            // Select random spot inside the room
            monsterX = u_char(Dice::RandomNumberBetween(floor->getRoom(monsterRoom)->getStartingX(), floor->getRoom(monsterRoom)->getStartingX() + floor->getRoom(monsterRoom)->getWidth() - 1));
            monsterY = u_char(Dice::RandomNumberBetween(floor->getRoom(monsterRoom)->getStartingY(), floor->getRoom(monsterRoom)->getStartingY() + floor->getRoom(monsterRoom)->getHeight() - 1));

            placementAttempts++;
        } while (floor->getCharacterAt(monsterX, monsterY) != null && placementAttempts < 25);

        // If failed to find, just man handle it through
        if (placementAttempts >= 25) {
            for (roomIndex = 0; roomIndex < floor->getRoomCount(); roomIndex++) {
                // Start looping and find the next open spot
                for (height = floor->getRoom(monsterRoom)->getStartingY(); height < floor->getRoom(monsterRoom)->getStartingY() + floor->getRoom(monsterRoom)->getHeight(); height++) {
                    for (width = floor->getRoom(monsterRoom)->getStartingX(); width < floor->getRoom(monsterRoom)->getStartingX() + floor->getRoom(monsterRoom)->getWidth(); width++) {
                        if (floor->getCharacterAt(width, height) == null) {
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
        Monster::Move0(monster, x, y);
    } else if (random < OTHER_CHARACTERISTIC_CHANCE) {
        Monster::Move6(monster, x, y);
    } else {
        *x = monster->getX();
        *y = monster->getY();
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
void Monster::Move15(Monster* monster, u_char* x, u_char* y) {
    Floor* floor = monster->getFloor();
    auto random = u_char(Dice::RandomNumberBetween(0, 100));

    if (random < EVOLVE_CHANCE) {
        auto randomCharacteristic = u_char(Dice::RandomNumberBetween(1, 4));

        monster->setClassification(monster->getClassification() | randomCharacteristic);
        monster->setCharacter(convert_base10_to_char(monster->getClassification()));
//        monster->setLevel(
//                u_char(
//                        (monster->isIntelligent() ? MONSTER_INTELLIGENT_LEVEL : 0) +
//                        (monster->isTelepathic() ? MONSTER_TELEPATHIC_LEVEL : 0) +
//                        (monster->isTunneler() ? MONSTER_TUNNELER_LEVEL : 0) +
//                        (monster->isErratic() ? MONSTER_ERRATIC_LEVEL : 0) +
//                        1));

        std::string evolveFormat = "%s has evolved";
        floor->getDungeon()->prependText(&evolveFormat, monster->getName().c_str());

        *x = monster->getX();
        *y = monster->getY();
    } else if (random < TELEPORT_CHANCE) {
        u_char monsterX;
        u_char monsterY;
        u_char height;
        u_char width;
        u_char placementAttempts = 0;
        u_char roomIndex = 0;
        auto monsterRoom = u_char(Dice::RandomNumberBetween(0, floor->getRoomCount() - u_char(1)));

        do {
            // Select random spot inside the room
            monsterX = u_char(Dice::RandomNumberBetween(floor->getRoom(monsterRoom)->getStartingX(), floor->getRoom(monsterRoom)->getStartingX() + floor->getRoom(monsterRoom)->getWidth() - 1));
            monsterY = u_char(Dice::RandomNumberBetween(floor->getRoom(monsterRoom)->getStartingY(), floor->getRoom(monsterRoom)->getStartingY() + floor->getRoom(monsterRoom)->getHeight() - 1));

            placementAttempts++;
        } while (floor->getCharacterAt(monsterX, monsterY) != null && placementAttempts < 25);

        // If failed to find, just man handle it through
        if (placementAttempts >= 25) {
            for (roomIndex = 0; roomIndex < floor->getRoomCount(); roomIndex++) {
                // Start looping and find the next open spot
                for (height = floor->getRoom(monsterRoom)->getStartingY(); height < floor->getRoom(monsterRoom)->getStartingY() + floor->getRoom(monsterRoom)->getHeight(); height++) {
                    for (width = floor->getRoom(monsterRoom)->getStartingX(); width < floor->getRoom(monsterRoom)->getStartingX() + floor->getRoom(monsterRoom)->getWidth(); width++) {
                        if (floor->getCharacterAt(width, height) == null) {
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
        Monster::Move0(monster, x, y);
    } else if (random < OTHER_CHARACTERISTIC_CHANCE) {
        Monster::Move7(monster, x, y);
    } else {
        *x = monster->getX();
        *y = monster->getY();
    }
}