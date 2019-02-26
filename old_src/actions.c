#include "actions.h"

void actions_battle(Player* player, Monster* monster) {
    if (monster == null || player == null || !player->isAlive || !monster->isAlive) {
        print(player->dungeon->settings->doNCursesPrint, "Battle cannot occur.\n");
    }

    if (player_battle(player, monster)) {
        player->level += monster_slain(monster);
        player->monstersSlain++;

        dungeon_prepend_message(monster->dungeon, "You have slain the beast!");
    } else {
        player->isAlive = false;
    }
}