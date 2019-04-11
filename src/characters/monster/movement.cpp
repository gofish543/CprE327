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

            *x = *x + Dice::RandomNumberBetween(-1, 1);
            *y = *y + Dice::RandomNumberBetween(-1, 1);
        } while (floor->getTerrainAt(*x, *y)->isImmutable());
    } else {
        do {
            *x = monster->getX();
            *y = monster->getY();

            *x = *x + Dice::RandomNumberBetween(-1, 1);
            *y = *y + Dice::RandomNumberBetween(-1, 1);
        } while (!floor->getTerrainAt(*x, *y)->isWalkable());
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
    Floor* floor = monster->getFloor();
    Player* player = floor->getDungeon()->getPlayer();

    u_char playerX = player->getX();
    u_char playerY = player->getY();

    // Default stay right where they are
    u_char cheapestX = monster->getX();
    u_char cheapestY = monster->getY();
    // Position values
    u_char tempX;
    u_char tempY;

    // Record cheapest cost
    u_char cheapestCost = U_CHAR_MAX;

    // If the monster has line of sight, store previous location
    if (monster->hasLineOfSightTo(player->getX(), player->getY())) {
        monster->setPlayerLastSpottedX(playerX)->setPlayerLastSpottedY(playerY);
    } else if ((monster->getPlayerLastSpottedY() == 0 && monster->getPlayerLastSpottedX() == 0) ||
               (monster->getX() == monster->getPlayerLastSpottedX() && monster->getY() == monster->getPlayerLastSpottedY())) {
        // No previous location found, just stay here
        // Previous player location is our location, stay here
        *x = monster->getX();
        *y = monster->getY();
        return;
    }

    // "Move" the player to where the monster thought they were and rerun dijkstra
    player->setX(monster->getPlayerLastSpottedX())->setY(monster->getPlayerLastSpottedY());
    Monster::RunDijkstra(floor, MONSTER_DIJKSTRA_TYPE_NON_TUNNELER, floor->nonTunnelerView);

    for (tempY = monster->getY() - 1; tempY <= monster->getY() + 1; tempY++) {
        for (tempX = monster->getX() - 1; tempX <= monster->getX() + 1; tempX++) {
            if (floor->getNonTunnelerViewAt(tempX, tempY) < cheapestCost && floor->getTerrainAt(tempX, tempY)->isWalkable()) {
                cheapestX = tempX;
                cheapestY = tempY;
                cheapestCost = floor->getNonTunnelerViewAt(tempX, tempY);
            }
        }
    }

    // "Move" player back and rerun dijkstra
    player->setX(playerX)->setY(playerY);
    Monster::RunDijkstra(floor, MONSTER_DIJKSTRA_TYPE_NON_TUNNELER, floor->nonTunnelerView);

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
void Monster::Move2(Monster* monster, u_char* x, u_char* y) {
    Floor* floor = monster->getFloor();

    char deltaX = floor->getDungeon()->getPlayer()->getX() - monster->getX();
    char deltaY = floor->getDungeon()->getPlayer()->getY() - monster->getY();

    if (abs(deltaX) > abs(deltaY)) { // Move in the x direction?
        *x = monster->getX() + get_sign(deltaX);
        *y = monster->getY();
    } else if (abs(deltaX) < abs(deltaY)) { // Move in the y direction?
        *x = monster->getX();
        *y = monster->getY() + get_sign(deltaY);
    } else { // Move in a diagonal?
        *x = monster->getX() + get_sign(deltaX);
        *y = monster->getY() + get_sign(deltaY);
    }

    // Cant tunnel, revert movement back
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

    // Record cheapest cost
    u_char cheapestCost = U_CHAR_MAX;

    // Position values
    u_char tempX;
    u_char tempY;

    for (tempY = monster->getY() - 1; tempY <= monster->getY() + 1; tempY++) {
        for (tempX = monster->getX() - 1; tempX <= monster->getX() + 1; tempX++) {
            if (floor->getNonTunnelerViewAt(tempX, tempY) < cheapestCost) {
                cheapestX = tempX;
                cheapestY = tempY;
                cheapestCost = floor->getNonTunnelerViewAt(tempX, tempY);
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
    Floor* floor = monster->getFloor();
    Player* player = floor->getDungeon()->getPlayer();
    char deltaX;
    char deltaY;

    // If the monster has line of sight, store previous location
    if (monster->hasLineOfSightTo(player->getX(), player->getY())) {
        monster->setPlayerLastSpottedX(player->getX())->setPlayerLastSpottedY(player->getY());
    } else if ((monster->getPlayerLastSpottedY() == 0 && monster->getPlayerLastSpottedX() == 0) ||
               (monster->getX() == monster->getPlayerLastSpottedX() && monster->getY() == monster->getPlayerLastSpottedY())) {
        // No previous location found, just stay here
        // Previous player location is our location, stay here
        *x = monster->getX();
        *y = monster->getY();
        return;
    }

    deltaX = monster->getPlayerLastSpottedX() - monster->getX();
    deltaY = monster->getPlayerLastSpottedY() - monster->getY();

    if (deltaY == 0) { // Move in the x direction
        *x = monster->getX() + get_sign(deltaX);
        *y = monster->getY();
    } else if (abs(deltaX) / double(abs(deltaY)) > 1.25) { // Move in the x direction?
        *x = monster->getX() + get_sign(deltaX);
        *y = monster->getY();
    } else if (abs(deltaX) / double(abs(deltaY)) < 0.75) { // Move in the y direction?
        *x = monster->getX();
        *y = monster->getY() + get_sign(deltaY);
    } else { // Move in a diagonal?
        *x = monster->getX() + get_sign(deltaX);
        *y = monster->getY() + get_sign(deltaY);
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
    Floor* floor = monster->getFloor();
    Player* player = floor->getDungeon()->getPlayer();

    u_char playerX = player->getX();
    u_char playerY = player->getY();

    // Default stay right where they are
    u_char cheapestX = monster->getX();
    u_char cheapestY = monster->getY();

    // Position values
    u_char tempX;
    u_char tempY;

    // Record cheapest cost
    u_char cheapestCost = U_CHAR_MAX;

    // If the monster has line of sight, store previous location
    if (monster->hasLineOfSightTo(player->getX(), player->getY())) {
        monster->setPlayerLastSpottedX(player->getX());
        monster->setPlayerLastSpottedY(player->getY());
    } else if ((monster->getPlayerLastSpottedY() == 0 && monster->getPlayerLastSpottedX() == 0) ||
               (monster->getX() == monster->getPlayerLastSpottedX() && monster->getY() == monster->getPlayerLastSpottedY())) {
        // No previous location found, just stay here
        // Previous player location is our location, stay here
        *x = monster->getX();
        *y = monster->getY();
        return;
    }


    // "Move" the player and run dijkstra for just the non tunneler view
    player->setX(monster->getPlayerLastSpottedX())->setY(monster->getPlayerLastSpottedY());
    Monster::RunDijkstra(floor, MONSTER_DIJKSTRA_TYPE_TUNNELER, floor->tunnelerView);

    for (tempY = monster->getY() - 1; tempY <= monster->getY() + 1; tempY++) {
        for (tempX = monster->getX() - 1; tempX <= monster->getX() + 1; tempX++) {
            if (floor->getTunnelerViewAt(tempX, tempY) < cheapestCost) {
                cheapestX = tempX;
                cheapestY = tempY;
                cheapestCost = floor->getTunnelerViewAt(tempX, tempY);
            }
        }
    }

    // "Move" player back and rerun dijkstra
    player->setX(playerX)->setY(playerY);
    Monster::RunDijkstra(floor, MONSTER_DIJKSTRA_TYPE_TUNNELER, floor->tunnelerView);

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
        *x = monster->getX() + get_sign(deltaX);
        *y = monster->getY();
    } else if (abs(deltaX) / double(abs(deltaY)) > 1.25) { // Move in the x direction?
        *x = monster->getX() + get_sign(deltaX);
        *y = monster->getY();
    } else if (abs(deltaX) / double(abs(deltaY)) < 0.75) { // Move in the y direction?
        *x = monster->getX();
        *y = monster->getY() + get_sign(deltaY);
    } else { // Move in a diagonal?
        *x = monster->getX() + get_sign(deltaX);
        *y = monster->getY() + get_sign(deltaY);
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

    for (tempY = monster->getY() - 1; tempY <= monster->getY() + 1; tempY++) {
        for (tempX = monster->getX() - 1; tempX <= monster->getX() + 1; tempX++) {
            if (floor->getTunnelerViewAt(tempX, tempY) <= cheapestCost) {
                cheapestX = tempX;
                cheapestY = tempY;
                cheapestCost = floor->getTunnelerViewAt(tempX, tempY);
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
 * 5% Chance to teleport to a random location
 */
void Monster::Move8(Monster* monster, u_char* x, u_char* y) {
    Floor* floor = monster->getFloor();
    auto random = u_char(Dice::RandomNumberBetween(0, 100));

    if (random < TELEPORT_CHANCE) {
        Room* room = floor->getRoom(Dice::RandomNumberBetween(0, floor->getRoomCount() - 1));

        *x = room->randomXInside();
        *y = room->randomYInside();
    } else if (random < RANDOM_MOVE_CHANCE) {
        Monster::Move0(monster, x, y);
    } else if (random < OTHER_CHARACTERISTIC_CHANCE) {
        void (* monsterMovement[])(Monster*, u_char*, u_char*) = {
                Monster::Move0, Monster::Move1,
                Monster::Move2, Monster::Move3,
                Monster::Move4, Monster::Move5,
                Monster::Move6, Monster::Move7,
        };
        monsterMovement[monster->getNonErraticMovementValue()](monster, x, y);
    } else {
        *x = monster->getX();
        *y = monster->getY();
    }
}