#include "input.h"

int getChar(WINDOW* window, const bool ncurses) {
    int userInput = 0;

    if (ncurses) {
        userInput = getch();
    } else {
        int character = 0;
        print(window, ncurses, "Please enter your command, then click enter\n");
        while ((character = getchar()) != '\n' && character != EOF) {
            userInput += character;
        }

        switch (userInput) {
            case 183: // Up arrow
                userInput = KEY_UP;
                break;
            case 184: // Down key
                userInput = KEY_DOWN;
        }
    }
    return userInput;
}