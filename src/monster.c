#include "monster.h"

Monster* monster_initialize(Dungeon* dungeon, u_char x, u_char y, u_char floor, bool canTunnel) {
    Monster* monster = malloc(sizeof(Monster));

    monster->dot = floor_dot_initialize(x, y, type_monster, MONSTER_HARDNESS, MONSTER_CHARACTER);
    monster->floor = floor;
    monster->dungeon = dungeon;
    monster->canTunnel = canTunnel;
    monster->standingOn = NULL;

    monster_move_to(monster, x, y);

    return monster;
}

Monster* monster_terminate(Monster* monster) {
    // Clean up the standing on pointer as long as it is not a special object
    if (monster->standingOn != NULL &&
        monster->standingOn->type != type_staircase_up &&
        monster->standingOn->type != type_staircase_down &&
        monster->standingOn->type != type_monster &&
        monster->standingOn->type != type_player) {
        monster->standingOn = floor_dot_terminate(monster->standingOn, true);
    }
    monster->dot = floor_dot_terminate(monster->dot, true);
    free(monster);

    return NULL;
}

void monster_move_to(Monster* monster, u_char toX, u_char toY) {
    Dungeon* dungeon = monster->dungeon;
    // Restore previous floor dot pointer
    if (monster->standingOn != NULL) {
        dungeon->floors[monster->floor]->floorPlan[monster->dot->y][monster->dot->x] = monster->standingOn;
    }

    // Move the monster to the new dot
    monster->dot->x = toX;
    monster->dot->y = toY;

    // Save the standing on dot
    monster->standingOn = dungeon->floors[monster->floor]->floorPlan[monster->dot->y][monster->dot->x];

    // Place the player on the floor plan position
    dungeon->floors[monster->floor]->floorPlan[monster->dot->y][monster->dot->x] = monster->dot;
}

int32_t monster_path_cmp(const void* key, const void* with) {
    MonsterCost* keyMonster = (MonsterCost*) key;
    MonsterCost* withMonster = (MonsterCost*) with;

    if (keyMonster->isTunnel) {
        return keyMonster->floor->tunnelerCost[keyMonster->y][keyMonster->x] - withMonster->floor->tunnelerCost[withMonster->y][withMonster->x];
    } else {
        return keyMonster->floor->nonTunnelerCost[keyMonster->y][keyMonster->x] - withMonster->floor->nonTunnelerCost[withMonster->y][withMonster->x];
    }
}

void monster_dijkstra_tunneler(Floor* floor) {
    MonsterCost monsterCost[FLOOR_HEIGHT][FLOOR_WIDTH];

    u_char fromX = floor->dungeon->player->dot->x;
    u_char fromY = floor->dungeon->player->dot->y;

    heap_t heap;
    u_char width, height;

    for (height = 0; height < FLOOR_HEIGHT; height++) {
        for (width = 0; width < FLOOR_WIDTH; width++) {
            monsterCost[height][width].y = height;
            monsterCost[height][width].x = width;
            monsterCost[height][width].cost = 1 + (floor->floorPlan[height][width]->hardness / 85);
            monsterCost[height][width].floor = floor;
            monsterCost[height][width].isTunnel = true;

            floor->tunnelerCost[height][width] = U_CHAR_MAX;
        }
    }

    monsterCost[fromY][fromX].cost = 0;

    heap_init(&heap, monster_path_cmp, NULL);
    heap_insert(&heap, &monsterCost[fromY][fromX]);

    MonsterCost* item;
    while ((item = heap_remove_min(&heap))) {
        if (floor->floorPlan[item->y][item->x]->type != type_border) {
            // Top Left
            MonsterCost* topLeft = &(monsterCost[item->y - 1][item->x - 1]);
            if (floor->tunnelerCost[topLeft->y][topLeft->x] >= item->cost + topLeft->cost) {
                if (floor->floorPlan[topLeft->y][topLeft->x]->type != type_border) {
                    topLeft->cost += item->cost;
                    floor->tunnelerCost[topLeft->y][topLeft->x] = topLeft->cost;
                    heap_insert(&heap, topLeft);
                }
            }
            // Top Center
            MonsterCost* topCenter = &monsterCost[item->y - 1][item->x];
            if (floor->tunnelerCost[topCenter->y][topCenter->x] >= item->cost + topCenter->cost) {
                if (floor->floorPlan[topLeft->y][topLeft->x]->type != type_border) {
                    topCenter->cost += item->cost;
                    floor->tunnelerCost[topCenter->y][topCenter->x] = topCenter->cost;
                    heap_insert(&heap, topCenter);
                }
            }
            // Top Right
            MonsterCost* topRight = &monsterCost[item->y - 1][item->x + 1];
            if (floor->tunnelerCost[topRight->y][topRight->x] >= item->cost + topRight->cost) {
                if (floor->floorPlan[topRight->y][topRight->x]->type != type_border) {
                    topRight->cost += item->cost;
                    floor->tunnelerCost[topRight->y][topRight->x] = topRight->cost;
                    heap_insert(&heap, topRight);
                }
            }
            // Left Center
            MonsterCost* leftCenter = &monsterCost[item->y][item->x - 1];
            if (floor->tunnelerCost[leftCenter->y][leftCenter->x] >= item->cost + leftCenter->cost) {
                if (floor->floorPlan[leftCenter->y][leftCenter->x]->type != type_border) {
                    leftCenter->cost += item->cost;
                    floor->tunnelerCost[leftCenter->y][leftCenter->x] = leftCenter->cost;
                    heap_insert(&heap, leftCenter);
                }
            }
            // Right Center
            MonsterCost* rightCenter = &monsterCost[item->y][item->x + 1];
            if (floor->tunnelerCost[rightCenter->y][rightCenter->x] >= item->cost + rightCenter->cost) {
                if (floor->floorPlan[rightCenter->y][rightCenter->x]->type != type_border) {
                    rightCenter->cost += item->cost;
                    floor->tunnelerCost[rightCenter->y][rightCenter->x] = rightCenter->cost;
                    heap_insert(&heap, rightCenter);
                }
            }
            // Bot Left
            MonsterCost* bottomLeft = &monsterCost[item->y + 1][item->x - 1];
            if (floor->tunnelerCost[bottomLeft->y][bottomLeft->x] >= item->cost + bottomLeft->cost) {
                if (floor->floorPlan[bottomLeft->y][bottomLeft->x]->type != type_border) {
                    bottomLeft->cost += item->cost;
                    floor->tunnelerCost[bottomLeft->y][bottomLeft->x] = bottomLeft->cost;
                    heap_insert(&heap, bottomLeft);
                }
            }
            // Bot Center
            MonsterCost* bottomCenter = &monsterCost[item->y + 1][item->x];
            if (floor->tunnelerCost[bottomCenter->y][bottomCenter->x] >= item->cost + bottomCenter->cost) {
                if (floor->floorPlan[bottomCenter->y][bottomCenter->x]->type != type_border) {
                    bottomCenter->cost += item->cost;
                    floor->tunnelerCost[bottomCenter->y][bottomCenter->x] = bottomCenter->cost;
                    heap_insert(&heap, bottomCenter);
                }
            }
            // Bot Right
            MonsterCost* bottomRight = &monsterCost[item->y + 1][item->x + 1];
            if (floor->tunnelerCost[bottomRight->y][bottomRight->x] >= item->cost + bottomRight->cost) {
                if (floor->floorPlan[bottomRight->y][bottomRight->x]->type != type_border) {
                    bottomRight->cost += item->cost;
                    floor->tunnelerCost[bottomRight->y][bottomRight->x] = bottomRight->cost;
                    heap_insert(&heap, bottomRight);
                }
            }
        }
    }
    floor->nonTunnelerCost[fromY][fromX] = U_CHAR_MIN;
}

void monster_dijkstra_non_tunneler(Floor* floor) {
    MonsterCost monsterCost[FLOOR_HEIGHT][FLOOR_WIDTH];

    u_char fromX = floor->dungeon->player->dot->x;
    u_char fromY = floor->dungeon->player->dot->y;

    heap_t heap;
    u_char width, height;

    for (height = 0; height < FLOOR_HEIGHT; height++) {
        for (width = 0; width < FLOOR_WIDTH; width++) {
            monsterCost[height][width].y = height;
            monsterCost[height][width].x = width;
            monsterCost[height][width].cost = 1;
            monsterCost[height][width].floor = floor;
            monsterCost[height][width].isTunnel = false;

            floor->nonTunnelerCost[height][width] = U_CHAR_MAX;
        }
    }

    monsterCost[fromY][fromX].cost = 0;

    heap_init(&heap, monster_path_cmp, NULL);
    heap_insert(&heap, &monsterCost[fromY][fromX]);

    MonsterCost* item;
    while ((item = heap_remove_min(&heap))) {
        if (floor->floorPlan[item->y][item->x]->type != type_border &&
            floor->floorPlan[item->y][item->x]->type != type_rock) {
            // Top Left
            MonsterCost* topLeft = &(monsterCost[item->y - 1][item->x - 1]);
            if (floor->nonTunnelerCost[topLeft->y][topLeft->x] >= item->cost + topLeft->cost) {
                if (floor->floorPlan[topLeft->y][topLeft->x]->type != type_border && floor->floorPlan[topLeft->y][topLeft->x]->type != type_rock) {
                    topLeft->cost += item->cost;
                    floor->nonTunnelerCost[topLeft->y][topLeft->x] = topLeft->cost;
                    heap_insert(&heap, topLeft);
                }
            }
            // Top Center
            MonsterCost* topCenter = &monsterCost[item->y - 1][item->x];
            if (floor->nonTunnelerCost[topCenter->y][topCenter->x] >= item->cost + topCenter->cost) {
                if (floor->floorPlan[topCenter->y][topCenter->x]->type != type_border && floor->floorPlan[topCenter->y][topCenter->x]->type != type_rock) {
                    topCenter->cost += item->cost;
                    floor->nonTunnelerCost[topCenter->y][topCenter->x] = topCenter->cost;
                    heap_insert(&heap, topCenter);
                }
            }
            // Top Right
            MonsterCost* topRight = &monsterCost[item->y - 1][item->x + 1];
            if (floor->nonTunnelerCost[topRight->y][topRight->x] >= item->cost + topRight->cost) {
                if (floor->floorPlan[topRight->y][topRight->x]->type != type_border && floor->floorPlan[topRight->y][topRight->x]->type != type_rock) {
                    topRight->cost += item->cost;
                    floor->nonTunnelerCost[topRight->y][topRight->x] = topRight->cost;
                    heap_insert(&heap, topRight);
                }
            }
            // Left Center
            MonsterCost* leftCenter = &monsterCost[item->y][item->x - 1];
            if (floor->nonTunnelerCost[leftCenter->y][leftCenter->x] >= item->cost + leftCenter->cost) {
                if (floor->floorPlan[leftCenter->y][leftCenter->x]->type != type_border && floor->floorPlan[leftCenter->y][leftCenter->x]->type != type_rock) {
                    leftCenter->cost += item->cost;
                    floor->nonTunnelerCost[leftCenter->y][leftCenter->x] = leftCenter->cost;
                    heap_insert(&heap, leftCenter);
                }
            }
            // Right Center
            MonsterCost* rightCenter = &monsterCost[item->y][item->x + 1];
            if (floor->nonTunnelerCost[rightCenter->y][rightCenter->x] >= item->cost + rightCenter->cost) {
                if (floor->floorPlan[rightCenter->y][rightCenter->x]->type != type_border && floor->floorPlan[rightCenter->y][rightCenter->x]->type != type_rock) {
                    rightCenter->cost += item->cost;
                    floor->nonTunnelerCost[rightCenter->y][rightCenter->x] = rightCenter->cost;
                    heap_insert(&heap, rightCenter);
                }
            }
            // Bot Left
            MonsterCost* bottomLeft = &monsterCost[item->y + 1][item->x - 1];
            if (floor->nonTunnelerCost[bottomLeft->y][bottomLeft->x] >= item->cost + bottomLeft->cost) {
                if (floor->floorPlan[bottomLeft->y][bottomLeft->x]->type != type_border && floor->floorPlan[bottomLeft->y][bottomLeft->x]->type != type_rock) {
                    bottomLeft->cost += item->cost;
                    floor->nonTunnelerCost[bottomLeft->y][bottomLeft->x] = bottomLeft->cost;
                    heap_insert(&heap, bottomLeft);
                }
            }
            // Bot Center
            MonsterCost* bottomCenter = &monsterCost[item->y + 1][item->x];
            if (floor->nonTunnelerCost[bottomCenter->y][bottomCenter->x] >= item->cost + bottomCenter->cost) {
                if (floor->floorPlan[bottomCenter->y][bottomCenter->x]->type != type_border && floor->floorPlan[bottomCenter->y][bottomCenter->x]->type != type_rock) {
                    bottomCenter->cost += item->cost;
                    floor->nonTunnelerCost[bottomCenter->y][bottomCenter->x] = bottomCenter->cost;
                    heap_insert(&heap, bottomCenter);
                }
            }
            // Bot Right
            MonsterCost* bottomRight = &monsterCost[item->y + 1][item->x + 1];
            if (floor->nonTunnelerCost[bottomRight->y][bottomRight->x] >= item->cost + bottomRight->cost) {
                if (floor->floorPlan[bottomRight->y][bottomRight->x]->type != type_border && floor->floorPlan[bottomRight->y][bottomRight->x]->type != type_rock) {
                    bottomRight->cost += item->cost;
                    floor->nonTunnelerCost[bottomRight->y][bottomRight->x] = bottomRight->cost;
                    heap_insert(&heap, bottomRight);
                }
            }
        }
    }
    floor->nonTunnelerCost[fromY][fromX] = U_CHAR_MIN;
}

void monsters_move(Floor* floor) {
    u_short index;
    for (index = 0; index < floor->monsterCount; index++) {
        Monster* monster = floor->monsters[index];
        // Default stay right where they are
        u_char cheapestX = monster->dot->x;
        u_char cheapestY = monster->dot->y;
        // Record cheapest cost
        u_char cheapestCost = U_CHAR_MAX;
        if (monster->canTunnel) {
            // If top left is the cheapest, move there
            if (floor->tunnelerCost[monster->dot->y - 1][monster->dot->x - 1] <= cheapestCost) {
                cheapestX = monster->dot->x - 1;
                cheapestY = monster->dot->y - 1;
                cheapestCost = floor->tunnelerCost[monster->dot->y - 1][monster->dot->x - 1];
            }
            // If top center is the cheapest, move there
            if (floor->tunnelerCost[monster->dot->y - 1][monster->dot->x] <= cheapestCost) {
                cheapestX = monster->dot->x;
                cheapestY = monster->dot->y - 1;
                cheapestCost = floor->tunnelerCost[monster->dot->y - 1][monster->dot->x];
            }
            // If top right is the cheapest, move there
            if (floor->tunnelerCost[monster->dot->y - 1][monster->dot->x + 1] <= cheapestCost) {
                cheapestX = monster->dot->x + 1;
                cheapestY = monster->dot->y - 1;
                cheapestCost = floor->tunnelerCost[monster->dot->y - 1][monster->dot->x + 1];
            }
            // If center left is the cheapest, move there
            if (floor->tunnelerCost[monster->dot->y][monster->dot->x - 1] <= cheapestCost) {
                cheapestX = monster->dot->x - 1;
                cheapestY = monster->dot->y;
                cheapestCost = floor->tunnelerCost[monster->dot->y][monster->dot->x - 1];
            }
            // If center right is the cheapest, move there
            if (floor->tunnelerCost[monster->dot->y][monster->dot->x + 1] <= cheapestCost) {
                cheapestX = monster->dot->x + 1;
                cheapestY = monster->dot->y;
                cheapestCost = floor->tunnelerCost[monster->dot->y][monster->dot->x + 1];
            }
            // If bottom left is the cheapest, move there
            if (floor->tunnelerCost[monster->dot->y + 1][monster->dot->x - 1] <= cheapestCost) {
                cheapestX = monster->dot->x - 1;
                cheapestY = monster->dot->y + 1;
                cheapestCost = floor->tunnelerCost[monster->dot->y + 1][monster->dot->x - 1];
            }
            // If bottom center is the cheapest, move there
            if (floor->tunnelerCost[monster->dot->y + 1][monster->dot->x] <= cheapestCost) {
                cheapestX = monster->dot->x;
                cheapestY = monster->dot->y + 1;
                cheapestCost = floor->tunnelerCost[monster->dot->y + 1][monster->dot->x];
            }
            // If bottom right center is the cheapest, move there
            if (floor->tunnelerCost[monster->dot->y + 1][monster->dot->x + 1] <= cheapestCost) {
                cheapestX = monster->dot->x + 1;
                cheapestY = monster->dot->y + 1;
                cheapestCost = floor->tunnelerCost[monster->dot->y + 1][monster->dot->x + 1];
            }
        } else {
            // If top left is the cheapest, move there
            if (floor->nonTunnelerCost[monster->dot->y - 1][monster->dot->x - 1] <= cheapestCost) {
                cheapestX = monster->dot->x - 1;
                cheapestY = monster->dot->y - 1;
                cheapestCost = floor->nonTunnelerCost[monster->dot->y - 1][monster->dot->x - 1];
            }
            // If top center is the cheapest, move there
            if (floor->nonTunnelerCost[monster->dot->y - 1][monster->dot->x] <= cheapestCost) {
                cheapestX = monster->dot->x;
                cheapestY = monster->dot->y - 1;
                cheapestCost = floor->nonTunnelerCost[monster->dot->y - 1][monster->dot->x];
            }
            // If top right is the cheapest, move there
            if (floor->nonTunnelerCost[monster->dot->y - 1][monster->dot->x + 1] <= cheapestCost) {
                cheapestX = monster->dot->x + 1;
                cheapestY = monster->dot->y - 1;
                cheapestCost = floor->nonTunnelerCost[monster->dot->y - 1][monster->dot->x + 1];
            }
            // If center left is the cheapest, move there
            if (floor->nonTunnelerCost[monster->dot->y][monster->dot->x - 1] <= cheapestCost) {
                cheapestX = monster->dot->x - 1;
                cheapestY = monster->dot->y;
                cheapestCost = floor->nonTunnelerCost[monster->dot->y][monster->dot->x - 1];
            }
            // If center right is the cheapest, move there
            if (floor->nonTunnelerCost[monster->dot->y][monster->dot->x + 1] <= cheapestCost) {
                cheapestX = monster->dot->x + 1;
                cheapestY = monster->dot->y;
                cheapestCost = floor->nonTunnelerCost[monster->dot->y][monster->dot->x + 1];
            }
                // If bottom left is the cheapest, move there
            else if (floor->nonTunnelerCost[monster->dot->y + 1][monster->dot->x - 1] <= cheapestCost) {
                cheapestX = monster->dot->x - 1;
                cheapestY = monster->dot->y + 1;
                cheapestCost = floor->nonTunnelerCost[monster->dot->y + 1][monster->dot->x - 1];
            }
            // If bottom center is the cheapest, move there
            if (floor->nonTunnelerCost[monster->dot->y + 1][monster->dot->x] <= cheapestCost) {
                cheapestX = monster->dot->x;
                cheapestY = monster->dot->y + 1;
                cheapestCost = floor->nonTunnelerCost[monster->dot->y + 1][monster->dot->x];
            }
            // If bottom right center is the cheapest, move there
            if (floor->nonTunnelerCost[monster->dot->y + 1][monster->dot->x + 1] <= cheapestCost) {
                cheapestX = monster->dot->x + 1;
                cheapestY = monster->dot->y + 1;
                cheapestCost = floor->nonTunnelerCost[monster->dot->y + 1][monster->dot->x + 1];
            }
        }
        // Before moving, make sure that the space is open
        if (floor->floorPlan[cheapestY][cheapestX]->type == type_player) {
            // Handle it
        } else if (floor->floorPlan[cheapestY][cheapestX]->type == type_monster) {
            // Dont move there
            continue;
        } else {
            monster_move_to(monster, cheapestX, cheapestY);
        }
    }
}