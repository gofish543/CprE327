#include "player.h"

Player* player_initialize(Floor* floor, u_char x, u_char y) {
    Player* player =(Player*) malloc(sizeof(Player));

    player->speed = PLAYER_SPEED;
    player->isAlive = true;
    player->requestTerminate = false;
    player->takingStaircase = null;

    player->daysSurvived = 0;
    player->monstersSlain = 0;
    player->level = 0;

    player->character = character_initialize(floor, null, player, x, y);

    return player;
}

Player* player_terminate(Player* player) {
    player->character = character_terminate(player->character);
    free(player);

    return null;
}

int player_next_tick(Event* event) {
    Player* player = (Player*) event->structure;

    if (player->isAlive) {
        return player_next_action(player);
    } else {
        return -1;
    }
}

int player_event(Event* event) {
    int move;
    Player* player = (Player*) event->structure;
    Dungeon* dungeon = player->character->floor->dungeon;

    INPUT:
    move = getChar(dungeon->window, dungeon->settings->doNCursesPrint);

    if (move == 'm') {
        // Handle monster menu
        player_action_monster_menu(dungeon);
        output_print_current_floor(dungeon);

        goto INPUT;
    } else if (move == '<' || move == '>') {
        // Take the staircase if standing on one
        if (player_action_staircase(dungeon, move)) {
            goto INPUT;
        }
    } else {
        // Handle the key press
        if (player_action_movement(dungeon, move)) {
            goto INPUT;
        }
    }
    return 0;
}

int player_move_to(Player* player, u_char toX, u_char toY) {
    Floor* floor = player->character->floor;

    // If moving to the same spot, just exit
    if (toX == player->character->x && toY == player->character->y) {
        return 0;
    }

    // Check out the target point
    if (floor->characters[toY][toX] != null) {
        if (floor->characters[toY][toX]->monster != null) {
            // Monster battle happening
            action_player_vs_monster(player, floor->characters[toY][toX]->monster);

            // If the player didn't win, get out game is over
            if (!player->isAlive) {
                return 0;
            }
        }
    }
    // By default we move the player to the requested spot

    // Remove where they were previously standing
    floor->characters[player->character->y][player->character->x] = null;

    // Update the character's x and y
    player->character->x = toX;
    player->character->y = toY;

    // Simply move the player there
    floor->characters[player->character->y][player->character->x] = player->character;

    // Player moved, rerun monster vision
    monster_run_dijkstra_on_floor(floor);

    return 0;
}

// Return false on lost fight
// Return true on won fight
bool player_battle(Player* player, Monster* monster) {
    if (monster_alive_count(monster->character->floor->dungeon) == 1) {
        return false;
    } else {
        return true;
    }
}

void player_slain(Player* player) {
    player->isAlive = false;
}