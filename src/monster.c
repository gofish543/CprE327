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