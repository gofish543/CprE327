#ifndef COLORS_H
#define COLORS_H

#define EFD_COLOR_BLACK         0u
#define EFD_COLOR_RED           1u
#define EFD_COLOR_GREEN         2u
#define EFD_COLOR_YELLOW        3u
#define EFD_COLOR_BLUE          4u
#define EFD_COLOR_MAGENTA       5u
#define EFD_COLOR_CYAN          6u
#define EFD_COLOR_WHITE         7u
#define EFD_COLOR_GREY_LIGHT    8u
#define EFD_COLOR_GREY_DARK     9u

#define SHELL_TEXT_BLACK        "\033[38;5;0m"
#define SHELL_TEXT_BLUE         "\033[38;5;18m"
#define SHELL_TEXT_GREEN        "\033[38;5;34m"
#define SHELL_TEXT_CYAN         "\033[38;5;51m"
#define SHELL_TEXT_RED          "\033[38;5;196m"
#define SHELL_TEXT_PURPLE       "\033[38;5;165m"
#define SHELL_TEXT_YELLOW       "\033[38;5;226m"
#define SHELL_TEXT_WHITE        "\033[38;5;255m"
#define SHELL_BG_GREY_LIGHT     "\033[48;5;240m"
#define SHELL_BG_GREY_DARK      "\033[48;5;233m"
#define SHELL_DEFAULT           "\033[0m"

#define NCURSES_BLACK           0b00000000u // 0
#define NCURSES_RED             0b00000001u // 1
#define NCURSES_GREEN           0b00000010u // 2
#define NCURSES_YELLOW          0b00000011u // 3
#define NCURSES_BLUE            0b00000100u // 4
#define NCURSES_MAGENTA         0b00000101u // 5
#define NCURSES_CYAN            0b00000110u // 6
#define NCURSES_WHITE           0b00000111u // 7

#define NCURSES_BG_GREY_LIGHT   0b01000000u // Light grey bit
#define NCURSES_BG_GREY_DARK    0b10000000u // Dark grey bit

#endif