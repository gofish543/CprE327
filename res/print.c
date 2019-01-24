#include "print.h"

void print_screen(GAME *game) {
    print_floor(game);
    print_text(game);
}

void print_floor(GAME *game) {
    int width;
    int height;
    for (height = 0; height < FLOOR_HEIGHT; height++) {
        for (width = 0; width < FLOOR_WIDTH; width++) {
            printf("%c", game->map[height][width]);
        }
        printf("\n");
    }
}

void print_text(GAME *game) {
    int width;
    int height;
    for (height = 0; height < TEXT_HEIGHT; height++) {
        for (width = 0; width < TEXT_WIDTH; width++) {
            printf("%c", game->map[height][width]);
        }
        printf("\n");
    }
}

