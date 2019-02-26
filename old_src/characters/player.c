#include "player.h"

Player* player_initialize(Character* character) {
    Player* player = malloc(sizeof(Player));

    player->speed = PLAYER_SPEED;
    player->isAlive = true;

    player->daysSurvived = 0;
    player->monstersSlain = 0;
    player->level = 0;

    player->character = character;

    return player;
}

Player* player_terminate(Player* player) {
    // Clean up the standing on pointer, as long as it is not a special object
    free(player);

    return null;
}

int player_event(Event* event) {
    u_char move;
    u_char x;
    u_char y;
    Player* player = (Player*) event->structure;
    Dungeon* dungeon = player->character->floor->dungeon;
    FloorDot* targetDot;
    Character* targetCharacter;
    do {
        print(dungeon->settings->doNCursesPrint, "Please enter your movement direction\n");
        move = getchar();

        x = player->dot->x;
        y = player->dot->y;

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
                break;
        }
        targetDot = player->character->floor->floorPlan[y][x];
        targetCharacter = player->character->floor->characters[y][x];
    } while (targetDot->type != type_walkable);


    if (targetCharacter != null && targetCharacter->monster != null) {
        // Battle the monster
        actions_battle(player, targetCharacter->monster);
        if(player->isAlive) {
            player_move_to(player, x, y);
            monster_run_dijkstra_on_floor(player->character->floor);
        }
    } else if (targetDot->type == type_staircase_down) {
        // Move down a staircase
    } else if (targetDot->type == type_staircase_up) {
        // Move up a staircase
    } else {
        // Simply move over a dot
        player_move_to(player, x, y);
        monster_run_dijkstra_on_floor(player->character->floor);
    }

    if (targetCharacter == null && targetDot->type == type_staircase_down) {
        Handle it
    } else if (targetCharacter == null)
        if (targetDot->type == type_monster) {
            Monster* monster = monster_get_at(dungeon->floors[dungeon->currentFloor], x, y);

            char message[FLOOR_WIDTH + 1] = "";
            sprintf(message, "You have attacked monster %d!", monster->classification);
            dungeon_prepend_message(monster->dungeon, message);

            actions_battle(player, monster);
            if (player->isAlive) {
                player_move_to(player, dungeon->currentFloor, x, y);
                monster_run_dijkstra_on_floor(dungeon->floors[dungeon->currentFloor]);
            }
        } else {
            player_move_to(player, dungeon->currentFloor, x, y);
            monster_run_dijkstra_on_floor(dungeon->floors[dungeon->currentFloor]);
        }

    event_initialize(dungeon->eventManager, dungeon->eventManager->currentTick + (1000 / PLAYER_SPEED), type_player, player, player_event);

    return 0;
}

int player_move_to(Player* player, u_char toX, u_char toY) {
    player->character->floor->characters[player->character->y][player->character->x] = null;

    if (player->character->floor->characters[toY][toX] != null) {
        // A battle is to occur
        actions_battle(player, player->character->floor->characters[toY][toX]->monster);
    }

    // If the player lived, move to the next dot
    if (player->isAlive) {
        // Move the player to the new dot
        player->character->x = toX;
        player->character->y = toY;

        player->character->floor->characters[toY][toX] = player->character;
    }

    return 0;
}

bool player_battle(Player* player, Monster* monster) {
    return true;
}