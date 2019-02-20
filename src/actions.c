#include "actions.h"

void actions_battle(Player* player, Monster* monster) {
    if (monster == null || player == null) {
        printf("Battle cannot occur\n");
    }

    player->level += monster_slain(monster);
    player->monstersSlain++;

    char message[FLOOR_WIDTH + 1] = "";
    sprintf(message, "You have slain the beast!");
    dungeon_prepend_message(monster->dungeon, message);
}