#include "player.h"

Player* player_initialize(Dungeon* dungeon, u_char x, u_char y, u_char floor) {
    Player* player = malloc(sizeof(Player));

    player->dot = floor_dot_initialize(x, y, type_player, PLAYER_HARDNESS, PLAYER_CHARACTER);
    player->floor = floor;
    player->dungeon = dungeon;
    player->standingOn = NULL;

    player_move_to(player, floor, x, y);

    return player;
}

Player* player_terminate(Player* player) {
    // Clean up the standing on pointer, as long as it is not a special object
    if (player->standingOn != NULL &&
        player->standingOn->type != type_staircase_up &&
        player->standingOn->type != type_staircase_down &&
        player->standingOn->type != type_monster &&
        player->standingOn->type != type_player) {
        player->standingOn = floor_dot_terminate(player->standingOn, true);
    }

    player->dot = floor_dot_terminate(player->dot, true);
    free(player);

    return NULL;
}

void player_free_standing_on(Player* player) {
    if (player->standingOn != NULL) {
        player->dungeon->floors[player->floor]->floorPlan[player->dot->y][player->dot->x] = player->standingOn;
        player->standingOn = NULL;
    }
}

void player_move_to(Player* player, u_char toFloor, u_char toX, u_char toY) {
    // Restore previous floor dot pointer
    player_free_standing_on(player);

    // Move the player to the new dot
    player->dot->x = toX;
    player->dot->y = toY;
    player->floor = toFloor;
    // Save the standing on dot
    player->standingOn = player->dungeon->floors[player->floor]->floorPlan[player->dot->y][player->dot->x];

    // Place the player on the floor plan position
    player->dungeon->floors[player->floor]->floorPlan[player->dot->y][player->dot->x] = player->dot;
    player->dungeon->currentFloor = toFloor;
}