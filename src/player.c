#include "player.h"

Player* player_initialize(Dungeon* dungeon, u_char x, u_char y, u_char floor) {
    Player* player = malloc(sizeof(Player));

    player->dot = floor_dot_initialize(x, y, type_player, PLAYER_HARDNESS, PLAYER_CHARACTER);
    player->floor = floor;
    player->dungeon = dungeon;
    player->standingOn = null;

    player_move_to(player, floor, x, y);

    return player;
}

Player* player_terminate(Player* player) {
    // Clean up the standing on pointer, as long as it is not a special object
    player->dot = floor_dot_terminate(player->dot);
    free(player);

    return null;
}

int player_free(Player* player) {
    if (player->standingOn != null) {
        player->dungeon->floors[player->floor]->floorPlan[player->dot->y][player->dot->x] = player->standingOn;
        player->standingOn = null;
    }

    return 0;
}

int player_move_to(Player* player, u_char toFloor, u_char toX, u_char toY) {
    // Restore previous floor dot pointer
    player_free(player);

    // Move the player to the new dot
    player->dot->x = toX;
    player->dot->y = toY;
    player->floor = toFloor;

    // Save the standing on dot
    player->standingOn = player->dungeon->floors[player->floor]->floorPlan[player->dot->y][player->dot->x];

    // Place the player on the floor plan position
    player->dungeon->floors[player->floor]->floorPlan[player->dot->y][player->dot->x] = player->dot;
    player->dungeon->currentFloor = toFloor;

    return 0;
}