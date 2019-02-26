#include "monster.h"

Monster* monster_initialize(Character* character, u_char type, u_char speed) {
    Monster* monster = malloc(sizeof(Monster));

    monster->classification = type;
    monster->speed = speed;
    monster->isAlive = true;
    monster->standingOn = null;
    monster->character = character;

    if (monster_is_tunneler(type)) {
        monster->visionTable = &(character->floor->tunnelerCost[0][0]);
    } else {
        monster->visionTable = &(character->floor->nonTunnelerCost[0][0]);
    }

    return monster;
}

Monster* monster_terminate(Monster* monster) {
    free(monster);

    return null;
}

int monster_event(Event* event) {
    Monster* monster = (Monster*) event->structure;

    // Only move alive monsters
    if (monster->isAlive) {
        monster_move(monster->dungeon->floors[monster->floor], monster);
    }

    // If monster is still alive after move
    if (monster->isAlive) {
        event_initialize(monster->character->floor->dungeon->eventManager, monster_next_tick(monster), type_monster, monster, event_type_monster);
    }

    return 0;
}

int monster_slain(Monster* monster) {
    // Get all monsters on the current floor
    monster->isAlive = false;

    return monster->classification;
}

int monster_count(Dungeon* dungeon) {
    u_char index;
    u_short monsterIndex;
    u_int total;
    for (index = 0, total = 0; index < dungeon->floorCount; index++) {
        for (monsterIndex = 0; monsterIndex < dungeon->floors[index]->monsterCount; monsterIndex++) {
            if (dungeon->floors[index]->monsters[monsterIndex]->isAlive) {
                total++;
            }
        }
    }

    return total;
}

int32_t monster_dijkstra_compare(const void* A, const void* B) {
    MonsterCost* monsterA = (MonsterCost*) A;
    MonsterCost* monsterB = (MonsterCost*) B;

    return monsterA->vision[monsterA->y][monsterA->x] - monsterB->vision[monsterB->y][monsterB->x];
}

int monster_run_dijkstra_on_floor(Floor* floor) {
    monster_run_dijkstra(floor, &(floor->tunnelerCost[0][0]));
    monster_run_dijkstra(floor, &(floor->nonTunnelerCost[0][0]));

    return 0;
}

void monster_run_dijkstra(Floor* floor, u_char** costChart) {
    MonsterCost** monsterCost;
    heap_t heap;
    u_char width;
    u_char height;

    u_char playerX = floor->dungeon->player->character->x;
    u_char playerY = floor->dungeon->player->character->y;

    heap_init(&heap, monster_dijkstra_compare, null);

    monsterCost = malloc(sizeof(MonsterCost*) * (FLOOR_HEIGHT * FLOOR_WIDTH));
    for (height = 0; height < FLOOR_HEIGHT; height++) {
        for (width = 0; width < FLOOR_WIDTH; width++) {
            monsterCost[height][width] = malloc(sizeof(MonsterCost));
            monsterCost[height][width]->y = height;
            monsterCost[height][width]->x = width;
            monsterCost[height][width]->cost = 1 + (floor->floorPlan[height][width]->hardness / 85);
            monsterCost[height][width]->floor = floor;

            costChart[height][width] = U_CHAR_MAX;
        }
    }
    monsterCost[playerY][playerX]->cost = 0;

    heap_insert(&heap, monsterCost[playerY][playerX]);

    MonsterCost* item;
    while ((item = heap_remove_min(&heap))) {
        if (floor->floorPlan[item->y][item->x]->type != type_border) {
            for (height = item->y - 1; height <= item->y + 1; height++) {
                for (width = item->x - 1; width <= item->x + 1; width++) {
                    if (costChart[height][width] >= item->cost + monsterCost[height][width]->cost && floor->floorPlan[height][width]->type != type_border) {
                        monsterCost[height][width]->cost += item->cost;
                        costChart[height][width] = monsterCost[height][width]->cost;
                        heap_insert(&heap, monsterCost[height][width]);
                    }
                }
            }
        }
    }
    costChart[playerY][playerX] = U_CHAR_MIN;

    for (height = 0; height < FLOOR_HEIGHT; height++) {
        for (width = 0; width < FLOOR_WIDTH; width++) {
            free(monsterCost[height][width]);
        }
    }
    free(monsterCost);
}

int monsters_move(Floor* floor) {
    u_short index;
    for (index = 0; index < floor->monsterCount; index++) {
        if (floor->monsters[index]->isAlive) {
            monster_move(floor, floor->monsters[index]);
        }
    }

    return 0;
}

int monster_move(Monster* monster) {
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
            if (monster->visionTable[y][x] <= cheapestCost) {
                cheapestX = x;
                cheapestY = y;
                cheapestCost = monster->visionTable[y][x];
            }
        }
    }

    // Before moving, make sure that the space is open
    if (monster->character->floor->floorPlan[cheapestY][cheapestX]->type == type_player) {
        // Handle it
        dungeon_prepend_message(monster->dungeon, "Monster %d attacked you!", monster->classification);

        actions_battle(monster->dungeon->player, monster);

        return 0;
    } else if (monster->floor->floorPlan[cheapestY][cheapestX]->type == type_monster) {
        // Dont move there
        return 0;
    } else {
        // Decrement the hardness of the rock, if it is 1, move there
        monster_move_to(monster, cheapestX, cheapestY);
    }

    return 0;
}

int monster_move_to(Monster* monster, u_char toX, u_char toY) {
    // Move the monster to the new dot
    monster->character->x = toX;
    monster->character->y = toY;

    return 0;
}