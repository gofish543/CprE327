#ifndef COLORS_H
#define COLORS_H

#define EFD_COLOR_BLACK          0u
#define EFD_COLOR_RED            1u
#define EFD_COLOR_GREEN          2u
#define EFD_COLOR_YELLOW         3u
#define EFD_COLOR_BLUE           4u
#define EFD_COLOR_MAGENTA        5u
#define EFD_COLOR_CYAN           6u
#define EFD_COLOR_WHITE          7u

#define SHELL_TEXT_BLACK             "\033[38;5;16m"
#define SHELL_TEXT_BLUE              "\033[38;5;18m"
#define SHELL_TEXT_GREEN             "\033[38;5;34m"
#define SHELL_TEXT_CYAN              "\033[38;5;51m"
#define SHELL_TEXT_RED               "\033[38;5;196m"
#define SHELL_TEXT_PURPLE            "\033[38;5;165m"
#define SHELL_TEXT_YELLOW            "\033[38;5;226m"
#define SHELL_TEXT_WHITE             "\033[38;5;255m"
#define SHELL_BG_BLACK              "\033[48;5;16m"
#define SHELL_DEFAULT               "\033[0m"

#define NCURSES_BLACK          0b00000000u
#define NCURSES_RED            0b00000001u
#define NCURSES_GREEN          0b00000010u
#define NCURSES_YELLOW         0b00000011u
#define NCURSES_BLUE           0b00000100u
#define NCURSES_MAGENTA        0b00000101u
#define NCURSES_CYAN           0b00000110u
#define NCURSES_WHITE          0b00000111u

#endif