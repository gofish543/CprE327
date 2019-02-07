#include "player.h"

Player* player_initialize(Dungeon* dungeon, u_char x, u_char y, u_char floor) {
    Player* player = malloc(sizeof(Player));

    player->dot = floor_dot_initialize(x, y, type_player, PLAYER_HARDNESS, PLAYER_CHARACTER);
    player->floor = floor;
    player->dungeon = dungeon;
    player->standingOn = NULL;

    move_to(player, floor, x, y);

    return player;
}

Player* player_terminate(Player* player) {
    // Clean up the standing on pointer
    if (player->standingOn != NULL) {
        free(player->standingOn);
    }
    player->dot = floor_dot_terminate(player->dot);
    free(player);

    return NULL;
}

void move_to(Player* player, u_char toFloor, u_char toX, u_char toY) {
    Dungeon* dungeon = player->dungeon;
    // Restore previous floor dot pointer
    if (player->standingOn != NULL) {
        dungeon->floors[player->floor]->floorPlan[player->dot->y][player->dot->x] = player->standingOn;
    }

    // Move the player to the new dot
    player->dot->x = toX;
    player->dot->y = toY;
    player->floor = toFloor;
    // Save the standing on dot
    player->standingOn = dungeon->floors[player->floor]->floorPlan[player->dot->y][player->dot->x];

    // Place the player on the floor plan position
    dungeon->floors[player->floor]->floorPlan[player->dot->y][player->dot->x] = player->dot;
    dungeon->currentFloor = toFloor;
}