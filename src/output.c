#include "output.h"

void output_print_current_floor(Dungeon* dungeon) {
    output_print_floor(dungeon, dungeon->currentFloor);
}

void output_print_current_floor_hardness(Dungeon* dungeon) {
    output_print_floor_hardness(dungeon, dungeon->currentFloor);
}

void output_print_current_floor_tunneler_view(Dungeon* dungeon) {
    output_print_floor_tunneler_view(dungeon, dungeon->currentFloor);
}

void output_print_current_floor_non_tunneler_view(Dungeon* dungeon) {
    output_print_floor_non_tunneler_view(dungeon, dungeon->currentFloor);
}

void output_print_all_floors(Dungeon* dungeon) {
    u_char index;
    for (index = 0; index < dungeon->floorCount; index++) {
        output_print_floor(dungeon, index);
    }
}

void output_print_floor(Dungeon* dungeon, u_char floor) {
    u_char width;
    u_char height;

    if (dungeon->settings->expandedPrint) {
        printf("  ");
        for (width = 0; width < dungeon->floors[floor]->width; width++) {
            printf("%3d", width);
        }
        printf("\n");

        for (height = 0; height < dungeon->floors[floor]->height; height++) {
            printf("%2d ", height);
            for (width = 0; width < dungeon->floors[floor]->width; width++) {
                printf(" %c ", dungeon->floors[floor]->floorPlan[height][width]->character);
            }
            printf("\n");
        }
    } else {
        for (height = 0; height < dungeon->floors[floor]->height; height++) {
            for (width = 0; width < dungeon->floors[floor]->width; width++) {
                printf("%c", dungeon->floors[floor]->floorPlan[height][width]->character);
            }
            printf("\n");
        }
    }
}

void output_print_floor_hardness(Dungeon* dungeon, u_char floor) {
    u_char width;
    u_char height;

    if (dungeon->settings->expandedPrint) {
        printf("   ");
        for (width = 0; width < dungeon->floors[floor]->width; width++) {
            printf("%3d", width);
        }
        printf("\n");

        for (height = 0; height < dungeon->floors[floor]->height; height++) {
            printf("%2d ", height);
            for (width = 0; width < dungeon->floors[floor]->width; width++) {
                printf("%3d", dungeon->floors[floor]->floorPlan[height][width]->hardness);
            }
            printf("\n");
        }
    } else {
        for (height = 0; height < dungeon->floors[floor]->height; height++) {
            for (width = 0; width < dungeon->floors[floor]->width; width++) {
                printf("%d", dungeon->floors[floor]->floorPlan[height][width]->hardness % 10);
            }
            printf("\n");
        }
    }
}

void output_print_floor_tunneler_view(Dungeon* dungeon, u_char floor) {
    u_char width;
    u_char height;

    if (dungeon->settings->expandedPrint) {
        printf("   ");
        for (width = 0; width < dungeon->floors[floor]->width; width++) {
            printf("%3d", width);
        }
        printf("\n");

        for (height = 0; height < dungeon->floors[floor]->height; height++) {
            printf("%2d ", height);
            for (width = 0; width < dungeon->floors[floor]->width; width++) {
                if (dungeon->floors[floor]->floorPlan[height][width]->type == type_border) {
                    printf(" %c ", dungeon->floors[floor]->floorPlan[height][width]->character);
                } else {
                    printf("%3d", dungeon->floors[floor]->tunnelerCost[height][width]);
                }
            }
            printf("\n");
        }
    } else {
        for (height = 0; height < dungeon->floors[floor]->height; height++) {
            for (width = 0; width < dungeon->floors[floor]->width; width++) {
                if (dungeon->floors[floor]->floorPlan[height][width]->type == type_border) {
                    printf("%c", dungeon->floors[floor]->floorPlan[height][width]->character);
                } else {
                    printf("%d", dungeon->floors[floor]->tunnelerCost[height][width] % 10);
                }
            }
            printf("\n");
        }
    }
}

void output_print_floor_non_tunneler_view(Dungeon* dungeon, u_char floor) {
    u_char width;
    u_char height;

    if (dungeon->settings->expandedPrint) {
        printf("   ");
        for (width = 0; width < dungeon->floors[floor]->width; width++) {
            printf("%3d", width);
        }
        printf("\n");

        for (height = 0; height < dungeon->floors[floor]->height; height++) {
            printf("%2d ", height);
            for (width = 0; width < dungeon->floors[floor]->width; width++) {
                if (dungeon->floors[floor]->floorPlan[height][width]->type == type_border) {
                    printf(" %c ", dungeon->floors[floor]->floorPlan[height][width]->character);
                } else if (dungeon->floors[floor]->nonTunnelerCost[height][width] == BORDER_HARDNESS) {
                    printf("   ");
                } else {
                    printf("%3d", dungeon->floors[floor]->nonTunnelerCost[height][width]);
                }
            }
            printf("\n");
        }
    } else {
        for (height = 0; height < dungeon->floors[floor]->height; height++) {
            for (width = 0; width < dungeon->floors[floor]->width; width++) {
                if (dungeon->floors[floor]->floorPlan[height][width]->type == type_border) {
                    printf("%c", dungeon->floors[floor]->floorPlan[height][width]->character);
                } else if (dungeon->floors[floor]->nonTunnelerCost[height][width] == BORDER_HARDNESS) {
                    printf(" ");
                } else {
                    printf("%d", dungeon->floors[floor]->nonTunnelerCost[height][width] % 10);
                }
            }
            printf("\n");
        }
    }
}