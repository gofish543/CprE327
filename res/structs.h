#ifndef STRUCTS_H
#define STRUCTS_H

#define FLOOR_WIDTH 80
#define FLOOR_HEIGHT 21
#define TEXT_WIDTH 80
#define TEXT_HEIGHT 3

#define BOUNDARY_TOP_BOT '-'
#define BOUNDARY_LEFT_RIGHT '|'
#define BOUNDARY_CORNER '+'
#define WHITE_SPACE ' '

typedef struct {
    char map[FLOOR_HEIGHT][FLOOR_WIDTH];
    char text[TEXT_HEIGHT][TEXT_WIDTH];
} GAME;

#endif
