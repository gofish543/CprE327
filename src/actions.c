#include "actions.h"

void action_player_vs_monster(Player* player, Monster* monster) {
    if (monster == null || player == null || !player->isAlive || !monster->isAlive) {
        print(player->character->floor->dungeon->window, player->character->floor->dungeon->settings->doNCursesPrint, "Battle cannot occur.\n");
    }

    if (player_battle(player, monster)) {
        u_int expGained = monster_slain(monster);
        player->level += expGained;
        player->monstersSlain++;

        dungeon_prepend_message(player->character->floor->dungeon, "You have slain the beast! You have gained %d experience", expGained);
    } else {
        player_slain(player);
    }
}

void action_monster_vs_monster(Monster* monsterA, Monster* monsterB) {
    if (monsterA == null || monsterB == null || !monsterA->isAlive || !monsterB->isAlive) {
        print(monsterA->character->floor->dungeon->window, monsterA->character->floor->dungeon->settings->doNCursesPrint, "Battle cannot occur.\n");
    }

    u_char result = monsters_battle(monsterA, monsterB);
    if(result > 0) {
        // Monster A won
        monster_slain(monsterB);
    }
    else if(result < 0) {
        // Monster B win
        monster_slain(monsterA);
    }
}