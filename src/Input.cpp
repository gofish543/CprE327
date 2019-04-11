#include "Input.h"

int getChar(const bool ncurses) {
    int userInput = 0;

    if (ncurses) {
        userInput = getch();
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
            case 186:
                userInput = KEY_LEFT;
                break;
            default:
                break;
        }
    }
    return userInput;
}