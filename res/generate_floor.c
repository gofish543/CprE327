#include "generate_floor.h"

void generate_floor(GAME *game) {
    generate_empty_text(game);

    generate_empty_map(game);
    generate_borders(game);
}

void generate_empty_text(GAME *game) {
    int width;
    int height;

    for (height = 0; height < TEXT_HEIGHT; height++) {
        for (width = 0; width < TEXT_WIDTH; width++) {
            game->text[height][width] = WHITE_SPACE;
        }
    }
}

void generate_empty_map(GAME *game) {
    int width;
    int height;

    for (height = 0; height < FLOOR_HEIGHT; height++) {
        for (width = 0; width < FLOOR_WIDTH; width++) {
            game->map[height][width] = WHITE_SPACE;
        }
    }
}

void generate_borders(GAME *game) {
    int width;
    int height;

    for (height = 0; height < FLOOR_HEIGHT; height++) {
        for (width = 0; width < FLOOR_WIDTH; width++) {
            if ((width == 0 && height == 0) || (width == FLOOR_WIDTH - 1 && height == FLOOR_HEIGHT - 1) ||
                (width == FLOOR_WIDTH - 1 && height == 0) || (width == 0 && height == FLOOR_HEIGHT - 1)) {
                game->map[height][width] = BOUNDARY_CORNER;
            } else if (height == 0 || height == FLOOR_HEIGHT - 1) {
                game->map[height][width] = BOUNDARY_TOP_BOT;
            } else if (width == 0 || width == FLOOR_WIDTH - 1) {
                game->map[height][width] = BOUNDARY_LEFT_RIGHT;
            }
        }
    }
}