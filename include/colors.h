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
#define SHELL_TEXT_LIGHT_GREY        "\033[38;5;249m"
#define SHELL_TEXT_DARK_GREY         "\033[38;5;238m"
#define SHELL_TEXT_LIGHT_BLUE        "\033[38;5;33m"
#define SHELL_TEXT_LIGHT_GREEN       "\033[38;5;46m"
#define SHELL_TEXT_LIGHT_CYAN        "\033[38;5;87m"
#define SHELL_TEXT_LIGHT_RED         "\033[38;5;160m"
#define SHELL_TEXT_LIGHT_PURPLE      "\033[38;5;201m"
#define SHELL_TEXT_LIGHT_YELLOW      "\033[38;5;228m"
#define SHELL_TEXT_WHITE             "\033[38;5;255m"

#define SHELL_BG_BLACK              "\033[48;5;16m"
#define SHELL_BG_BLUE               "\033[48;5;18m"
#define SHELL_BG_GREEN              "\033[48;5;34m"
#define SHELL_BG_CYAN               "\033[48;5;51m"
#define SHELL_BG_RED                "\033[48;5;196m"
#define SHELL_BG_PURPLE             "\033[48;5;165m"
#define SHELL_BG_YELLOW             "\033[48;5;226m"
#define SHELL_BG_LIGHT_GREY         "\033[48;5;249m"
#define SHELL_BG_DARK_GREY          "\033[48;5;238m"
#define SHELL_BG_LIGHT_BLUE         "\033[48;5;33m"
#define SHELL_BG_LIGHT_GREEN        "\033[48;5;46m"
#define SHELL_BG_LIGHT_CYAN         "\033[48;5;87m"
#define SHELL_BG_LIGHT_RED          "\033[48;5;160m"
#define SHELL_BG_LIGHT_PURPLE       "\033[48;5;201m"
#define SHELL_BG_LIGHT_YELLOW       "\033[48;5;228m"
#define SHELL_BG_WHITE              "\033[48;5;255m"

#define SHELL_DEFAULT               "\033[0m"
#define SHELL_UNDERLINE             "\033[4m"
#define SHELL_BLINK                 "\033[5m"
#define SHELL_INVERSE               "\033[7m"
#define SHELL_CONCEALED             "\033[8m"

#define NCURSES_BLACK          0b00000000u
#define NCURSES_RED            0b00000001u
#define NCURSES_GREEN          0b00000010u
#define NCURSES_YELLOW         0b00000011u
#define NCURSES_BLUE           0b00000100u
#define NCURSES_MAGENTA        0b00000101u
#define NCURSES_CYAN           0b00000110u
#define NCURSES_WHITE          0b00000111u

#include <string>
#include <cstdio>
#include <ncurses.h>

std::string colorToShell(int index);
int colorToNcurses(int index);

void bgBlack(bool ncurses);
void textWhite(bool ncurses);
void reset(bool ncurses);

#endif