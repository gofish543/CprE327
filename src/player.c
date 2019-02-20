#include "player.h"

Player* player_initialize(Dungeon* dungeon, u_char x, u_char y, u_char floor) {
    Player* player = malloc(sizeof(Player));

    player->dot = floor_dot_initialize(x, y, type_player, PLAYER_HARDNESS, PLAYER_CHARACTER);
    player->floor = floor;
    player->dungeon = dungeon;
    player->standingOn = null;
    player->speed = PLAYER_SPEED;
    player->isAlive = true;

    player->daysSurvived = 0;
    player->tilesExplored = 0;
    player->monstersSlain = 0;
    player->level = 0;

    player_move_to(player, floor, x, y);

    return player;
}

Player* player_terminate(Player* player) {
    // Clean up the standing on pointer, as long as it is not a special object
    player->dot = floor_dot_terminate(player->dot);
    free(player);

    return null;
}

int player_free(Player* player) {
    if (player->standingOn != null) {
        player->dungeon->floors[player->floor]->floorPlan[player->dot->y][player->dot->x] = player->standingOn;
        player->standingOn = null;
    }

    return 0;
}

int player_move_to(Player* player, u_char toFloor, u_char toX, u_char toY) {
    // Restore previous floor dot pointer
    player_free(player);

    // Move the player to the new dot
    player->dot->x = toX;
    player->dot->y = toY;
    player->floor = toFloor;

    // Save the standing on dot
    player->standingOn = player->dungeon->floors[player->floor]->floorPlan[player->dot->y][player->dot->x];

    // Place the player on the floor plan position
    player->dungeon->floors[player->floor]->floorPlan[player->dot->y][player->dot->x] = player->dot;
    player->dungeon->currentFloor = toFloor;

    return 0;
}

int player_event(Event* event) {
    u_char move;
    u_char x;
    u_char y;
    FloorDot* targetDot = null;
    Player* player = (Player*) event->structure;
    Dungeon* dungeon = player->dungeon;
    do {
        printf("Please enter your movement direction\n");
        move = getchar();

        x = player->dot->x;
        y = player->dot->y;

        switch (move) {
            case 'w':
                y--;
                break;
            case 'a':
                x--;
                break;
            case 's':
                y++;
                break;
            case 'd':
                x++;
                break;
            default:
                targetDot = player->dot;
                continue;
        }

        targetDot = dungeon->floors[dungeon->currentFloor]->floorPlan[y][x];

        while ((move = getchar()) != '\n' && move != EOF);
    } while (targetDot == null || (targetDot->type == type_border || targetDot->type == type_rock));

    if (targetDot->type == type_player) {
        // Take no action
    }
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