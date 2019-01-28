#include "player.h"

Player* player_initialize(u_char x, u_char y, u_char floor) {
    Player* player = malloc(sizeof(Player));

    player->x = x;
    player->y = y;
    player->floor = floor;

    return player;
}

Player* player_terminate(Player* player) {
    free(player);

    return NULL;
}