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
            dungeon_prepend_message(dungeon, "Moving upper left");
            y--;
            x--;
            break;
            // Up
        case '8':
        case 'k':
            dungeon_prepend_message(dungeon, "Moving up");
            y--;
            break;
            // Upper right
        case '9':
        case 'u':
            dungeon_prepend_message(dungeon, "Moving upper right");
            y--;
            x++;
            break;
            // Left
        case '4':
        case 'h':
            dungeon_prepend_message(dungeon, "Moving left");
            x--;
            break;
            // Right
        case '6':
        case 'l':
            dungeon_prepend_message(dungeon, "Moving right");
            x++;
            break;
            // Down left
        case '1':
        case 'b':
            dungeon_prepend_message(dungeon, "Moving down left");
            y++;
            x--;
            break;
            // Down
        case '2':
        case 'j':
            dungeon_prepend_message(dungeon, "Moving down");
            y++;
            break;
            // Down right
        case '3':
        case 'n':
            dungeon_prepend_message(dungeon, "Moving down right");
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
            dungeon_prepend_message(dungeon, "A day of rest...");
            break;
        default:
            dungeon_prepend_message(dungeon, "Invalid key entered");
            output(dungeon, output_print_current_floor);
            return 1;
    }

    if (!dungeon->floor->terrains[y][x]->isWalkable) {
        dungeon_prepend_message(dungeon, "Terrain is not walkable, invalid key entered");
        output(dungeon, output_print_current_floor);
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
            dungeon_prepend_message(dungeon, "Moving %s a staircase", (onTerrain->staircase->isUp ? "Up" : "Down"));
            return 0;
        }
    }

    dungeon_prepend_message(dungeon, "Not standing on a valid staircase");
    output(dungeon, output_print_current_floor);

    // Not on staircase, cant take a staircase
    // Or not on a staircase facing the right direction
    return 1;
}