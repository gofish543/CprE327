#ifndef ROOM_H
#define ROOM_H

#define ROOM_CELL 'x'

#define MIN_WIDTH 4
#define MIN_HEIGHT 3
#define MAX_WIDTH 7
#define MAX_HEIGHT 6

typedef struct {
    short width;
    short height;

    short startX;
    short startY;
} ROOM;

#endif
