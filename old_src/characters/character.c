#include "character.h"

Character* character_initialize(Floor* floor, Monster* monster, Player* player, u_char x, u_char y) {
    Character* character = malloc(sizeof(Character));

    character->floor = floor;
    character->monster = monster;
    character->player = player;

    character->x = x;
    character->y = y;

    return character;
}

Character* character_terminate(Character* character) {
    free(character);

    return null;
}