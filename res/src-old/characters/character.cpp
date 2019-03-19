#include "character.h"

Character* character_initialize(Floor* floor, Monster* monster, Player* player, u_char x, u_char y) {
    Character* character = (Character*) malloc(sizeof(Character));

    character->floor = floor;
    character->monster = monster;
    character->player = player;

    character->x = x;
    character->y = y;

    if (player != null) {
        character->character = PLAYER_CHARACTER;
    } else {
        character->character = convert_base10_to_char(monster->classification);
    }

    return character;
}

Character* character_terminate(Character* character) {
    free(character);

    return null;
}