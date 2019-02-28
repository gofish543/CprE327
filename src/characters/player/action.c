#include "action.h"

int player_action_monster_menu(Dungeon* dungeon) {
    u_short startIndex = 0;
    int character = 0;

    while (character != 27) {
        output_print_current_floor_monster_menu(dungeon, startIndex);

        character = getChar(dungeon->window, dungeon->settings->doNCursesPrint);

        switch (character) {
            case KEY_DOWN:
                if (startIndex < dungeon->floor->monsterCount) {
                    startIndex++;
                }
                break;
            case KEY_UP:
                if (startIndex > 0) {
                    startIndex--;
                }
                break;
        }
    }

    return 0;
}

int player_action_movement(Dungeon* dungeon, int command) {
    u_char x = dungeon->player->character->x;
    u_char y = dungeon->player->character->y;

    switch (command) {
        // Upper Left
        case '7':
        case 'y':
            y--;
            x--;
            break;
            // Up
        case '8':
        case 'k':
            y--;
            break;
            // Upper right
        case '9':
        case 'u':
            y--;
            x++;
            break;
            // Left
        case '4':
        case 'h':
            x--;
            break;
            // Right
        case '6':
        case 'l':
            x++;
            break;
            // Down left
        case '1':
        case 'b':
            y++;
            x--;
            break;
            // Down
        case '2':
        case 'j':
            y++;
            break;
        case '3':
        case 'n':
            // Down right
            y++;
            x++;
            break;
        case 'Q':
            dungeon->player->requestTerminate = true;
            // Take stairs down, if standing on
            // Show monster window
            // Rest for a turn
        case '5':
        case '.':
            break;
        default:
            return 1;
    }

    if (!dungeon->floor->terrains[y][x]->isWalkable) {
        return 1;
    }

    player_move_to(dungeon->player, x, y);

    return 0;
}

int player_action_staircase(Dungeon* dungeon, int command) {
    Floor* floor = dungeon->floor;
    Player* player = dungeon->player;
    Terrain* onTerrain = floor->terrains[player->character->y][player->character->x];
    // We are standing on a staircase
    if (onTerrain->staircase != null) {
        if ((command == STAIRCASE_DOWN_CHARACTER && onTerrain->staircase->isDown) ||
            (command == STAIRCASE_UP_CHARACTER && onTerrain->staircase->isUp)) {
            // Trying to take a down staircase, and standing on a down staircase
            // Trying to take an up staircase and standing on an up staircase
            player->takingStaircase = onTerrain->staircase;
            return 0;
        }
    }

    // Not on staircase, cant take a staircase
    // Or not on a staircase facing the right direction
    return 1;
}