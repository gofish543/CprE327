#include <colors.h>

std::string colorToShell(int index) {
    static const std::string shellColors[] = {
            SHELL_TEXT_BLACK,
            SHELL_TEXT_RED,
            SHELL_TEXT_GREEN,
            SHELL_TEXT_YELLOW,
            SHELL_TEXT_BLUE,
            SHELL_TEXT_PURPLE,
            SHELL_TEXT_CYAN,
            SHELL_TEXT_WHITE,
    };

    return shellColors[index];
}
int colorToNcurses(int index) {
    static const int ncursesColors[] = {
            NCURSES_BLACK,
            NCURSES_RED,
            NCURSES_GREEN,
            NCURSES_YELLOW,
            NCURSES_BLUE,
            NCURSES_MAGENTA,
            NCURSES_CYAN,
            NCURSES_WHITE
    };

    return ncursesColors[index];
}

void bgBlack(bool ncurses) {
    if (ncurses) {

    } else {
        printf(SHELL_BG_BLACK);
    }
}

void textWhite(bool ncurses) {
    if (ncurses) {

    } else {
        printf(SHELL_TEXT_WHITE);
    }
}

void reset(bool ncurses) {
    if (ncurses) {

    } else {
        printf(SHELL_DEFAULT);
    }
}