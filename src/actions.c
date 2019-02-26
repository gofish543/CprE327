#include "actions.h"

bool action_player_vs_monster(Player* player, Monster* monster) {
    if (monster == null || player == null || !player->isAlive || !monster->isAlive) {
        print(player->character->floor->dungeon->window, player->character->floor->dungeon->settings->doNCursesPrint, "Battle cannot occur.\n");
    }

    bool result = player_battle(player, monster);
    if (result) {
        u_int expGained = monster_slain(monster);
        player->level += expGained;
        player->monstersSlain++;

        dungeon_prepend_message(player->character->floor->dungeon, "You have slain the beast! You have gained %d experience", expGained);

        // Remove the slain monster from the map
        monster->character->floor->characters[monster->character->y][monster->character->x] = null;
    } else {
        player_slain(player);

        // Remove the slain player from the map
        player->character->floor->characters[player->character->y][player->character->x] = null;
    }

    return result;
}

int action_monster_vs_monster(Monster* monsterA, Monster* monsterB) {
    if (monsterA == null || monsterB == null || !monsterA->isAlive || !monsterB->isAlive) {
        print(monsterA->character->floor->dungeon->window, monsterA->character->floor->dungeon->settings->doNCursesPrint, "Battle cannot occur.\n");
    }

    u_char result = monsters_battle(monsterA, monsterB);
    if (result > 0) {
        // Monster A won
        monster_slain(monsterB);
        monsterB->character->floor->characters[monsterB->character->y][monsterB->character->x] = null;
        dungeon_prepend_message(monsterA->character->floor->dungeon, "Monster %c and %c just battled. Monster %c won", monsterA->character->character, monsterB->character->character, monsterA->character->character);

    } else if (result < 0) {
        // Monster B win
        monster_slain(monsterA);
        monsterA->character->floor->characters[monsterA->character->y][monsterA->character->x] = null;
        dungeon_prepend_message(monsterA->character->floor->dungeon, "Monster %c and %c just battled. Monster %c won", monsterA->character->character, monsterB->character->character, monsterB->character->character);
    }

    return result;
}