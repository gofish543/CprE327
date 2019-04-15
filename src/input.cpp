#include "input.h"

int getChar(const bool ncurses) {
    int userInput = 0;

    if (ncurses) {
        userInput = getch();

        switch (userInput) {
            case 10:
                userInput = KEY_ENTER;
                break;
            default:
                break;
        }
    } else {
        int character = 0;
        printf("Please enter your command, then click enter\n");
        while ((character = getchar()) != '\n' && character != EOF) {
            userInput += character;
        }

        switch (userInput) {
            case 183: // Up arrow
                userInput = KEY_UP;
                break;
            case 184: // Down arrow
                userInput = KEY_DOWN;
                break;
            case 185: // Right arrow
                userInput = KEY_RIGHT;
                break;
            case 186: // Left arrow
                userInput = KEY_LEFT;
                break;
            case 0: // Enter key
                userInput = KEY_ENTER;
                break;
            default:
                break;
        }
    }
    return userInput;
}