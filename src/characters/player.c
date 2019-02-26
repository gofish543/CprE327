#include "player.h"

Player* player_initialize(Floor* floor, u_char x, u_char y) {
    Player* player = malloc(sizeof(Player));

    player->speed = PLAYER_SPEED;
    player->isAlive = true;

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
    return 0;
    u_char move;
    u_char x;
    u_char y;
    Player* player = (Player*) event->structure;
    Dungeon* dungeon = player->character->floor->dungeon;
    Floor* floor = player->character->floor;
    do {
        if (dungeon->settings->doNCursesPrint) {
            move = getch();
        } else {
            print(dungeon->window, dungeon->settings->doNCursesPrint, "Please enter your movement direction\n");
            move = getchar();
        }

        x = player->character->x;
        y = player->character->y;

        switch (move) {
            case 'q':
                y--;
                x--;
                break;
            case 'w':
                y--;
                break;
            case 'e':
                y--;
                x++;
                break;
            case 'a':
                x--;
                break;
            case 'd':
                x++;
                break;
            case 'z':
                y++;
                x--;
                break;
            case 'x':
                y++;
                break;
            case 'c':
                y++;
                x++;
                break;
            case 's':
            default:
                player->isAlive = false;
                break;
        }

        // Empty out the buffer for non-ncurse runs
        if (!dungeon->settings->doNCursesPrint) {
            while ((move = getchar()) != '\n' && move != EOF);
        }
    } while (!floor->terrains[y][x]->isWalkable);

    player_move_to(player, x, y);

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
        } else {
            return 0;
        }
    } else if (floor->terrains[toY][toX]->staircase != null) {
        // A staircase was hit, handle it
        return 0;
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