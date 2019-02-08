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

void monster_dijkstra(Floor* floor, u_char fromX, u_char fromY, u_char toX, u_char toY) {
    static corridor_path_y path[FLOOR_HEIGHT][FLOOR_WIDTH]
    static corridor_path_y* p;
    static uint32_t initialized = 0;

    heap_t heap;
    u_char width, height;

    if (!initialized) {
        for (height = 0; height < FLOOR_HEIGHT; height++) {
            for (width = 0; width < FLOOR_WIDTH; width++) {
                path[height][width].pos[dim_y] = height;
                path[height][width].pos[dim_x] = width;
            }
        }
        initialized = 1;
    }

    for (height = 0; height < FLOOR_HEIGHT; height++) {
        for (width = 0; width < FLOOR_WIDTH; width++) {
            path[height][width].cost = INT_MAX;
        }
    }

    path[from[dim_y]][from[dim_x]].cost = 0;

    heap_init(&heap, corridor_path_cmp, NULL);

    for (height = 0; height < FLOOR_HEIGHT; height++) {
        for (width = 0; width < FLOOR_WIDTH; width++) {
            if (floor->floorPlan[height][width] != type_border) {
                path[height][width].hn = heap_insert(&heap, &path[height][width]);
            } else {
                path[height][width].hn = NULL;
            }
        }
    }

    while ((p = heap_remove_min(&heap))) {
        p->hn = NULL;

        if ((p->pos[dim_y] == to[dim_y]) && p->pos[dim_x] == to[dim_x]) {
            for (width = to[dim_x], height = to[dim_y];
                 (width != from[dim_x]) || (height != from[dim_y]);
                 p = &path[height][width], width = p->from[dim_x], height = p->from[dim_y]) {
                if (mapxy(width, height) != ter_floor_room) {
                    mapxy(width, height) = ter_floor_hall;
                    hardnessxy(width, height) = 0;
                }
            }
            heap_delete(&heap);
            return;
        }

        if ((path[p->pos[dim_y] - 1][p->pos[dim_x]].hn) &&
            (path[p->pos[dim_y] - 1][p->pos[dim_x]].cost >
             p->cost + hardnesspair(p->pos))) {
            path[p->pos[dim_y] - 1][p->pos[dim_x]].cost =
                    p->cost + hardnesspair(p->pos);
            path[p->pos[dim_y] - 1][p->pos[dim_x]].from[dim_y] = p->pos[dim_y];
            path[p->pos[dim_y] - 1][p->pos[dim_x]].from[dim_x] = p->pos[dim_x];
            heap_decrease_key_no_replace(&heap, path[p->pos[dim_y] - 1]
            [p->pos[dim_x]].hn);
        }
        if ((path[p->pos[dim_y]][p->pos[dim_x] - 1].hn) &&
            (path[p->pos[dim_y]][p->pos[dim_x] - 1].cost >
             p->cost + hardnesspair(p->pos))) {
            path[p->pos[dim_y]][p->pos[dim_x] - 1].cost =
                    p->cost + hardnesspair(p->pos);
            path[p->pos[dim_y]][p->pos[dim_x] - 1].from[dim_y] = p->pos[dim_y];
            path[p->pos[dim_y]][p->pos[dim_x] - 1].from[dim_x] = p->pos[dim_x];
            heap_decrease_key_no_replace(&heap, path[p->pos[dim_y]]
            [p->pos[dim_x] - 1].hn);
        }
        if ((path[p->pos[dim_y]][p->pos[dim_x] + 1].hn) &&
            (path[p->pos[dim_y]][p->pos[dim_x] + 1].cost >
             p->cost + hardnesspair(p->pos))) {
            path[p->pos[dim_y]][p->pos[dim_x] + 1].cost =
                    p->cost + hardnesspair(p->pos);
            path[p->pos[dim_y]][p->pos[dim_x] + 1].from[dim_y] = p->pos[dim_y];
            path[p->pos[dim_y]][p->pos[dim_x] + 1].from[dim_x] = p->pos[dim_x];
            heap_decrease_key_no_replace(&heap, path[p->pos[dim_y]]
            [p->pos[dim_x] + 1].hn);
        }
        if ((path[p->pos[dim_y] + 1][p->pos[dim_x]].hn) &&
            (path[p->pos[dim_y] + 1][p->pos[dim_x]].cost >
             p->cost + hardnesspair(p->pos))) {
            path[p->pos[dim_y] + 1][p->pos[dim_x]].cost =
                    p->cost + hardnesspair(p->pos);
            path[p->pos[dim_y] + 1][p->pos[dim_x]].from[dim_y] = p->pos[dim_y];
            path[p->pos[dim_y] + 1][p->pos[dim_x]].from[dim_x] = p->pos[dim_x];
            heap_decrease_key_no_replace(&heap, path[p->pos[dim_y] + 1]
            [p->pos[dim_x]].hn);
        }
    }
}

void monsters_move(Floor* floor) {
    u_short index;
    for (index = 0; index < floor->monsterCount; index++) {
        Monster* monster = floor->monsters[index];

        if (monster->canTunnel) {
//            monster_move_tunneler(monster);
        } else {
//            monster_move_non_tunneler(floor, monster);
        }
        break;
    }
    printf("Moving all monsters\n");
}