#include <curses.h>
#include "dungeon.h"

Dungeon::Dungeon(int argc, char* argv[]) {
    u_char index;

    for (index = 0; index < DUNGEON_TEXT_LINES; index++) {
        this->textLines[index] = "";
    }

    this->settings = new Settings(argc, argv);

    if (settings->doLoad()) {

    } else {
        this->floorCount = (u_char) random_number_between(DUNGEON_FLOORS_MIN, DUNGEON_FLOORS_MAX);
        auto stairCount = (u_short) random_number_between(FLOOR_STAIRS_MIN, FLOOR_STAIRS_MAX);

        for (index = 0; index < this->floorCount; index++) {
            this->floors.push_back(
                    new Floor(
                            this,
                            index,
                            (u_short) random_number_between(FLOOR_ROOMS_MIN, FLOOR_ROOMS_MAX),
                            stairCount,
                            stairCount,
                            (u_short) random_number_between(FLOOR_MONSTERS_MIN, FLOOR_MONSTERS_MAX)
                    )
            );
        }

        this->setCurrentFloor(this->getFloors().front());
    }

    if (this->settings->doNCursesPrint()) {
        this->window = initscr();
        raw();
        keypad(stdscr, true);
        noecho();
    } else {
        this->window = null;
    }
}

Dungeon::~Dungeon() {
    u_char index;

    for (index = 0; index < this->getFloorCount(); index++) {
        delete (this->floors.at(index));
    }

    if (this->settings->doNCursesPrint()) {
        endwin();
    }

    delete (this->settings);
}

std::string* Dungeon::appendText(std::string message, ...) {
    u_char index;

    for (index = 0; index < DUNGEON_TEXT_LINES - 1; index++) {
        this->textLines[index] = this->textLines[index + 1];
    }

    this->textLines[DUNGEON_TEXT_LINES - 1] = std::move(message);

    return this->textLines;
}

std::string* Dungeon::prependText(std::string message, ...) {
    u_char index;

    for (index = 1; index < DUNGEON_TEXT_LINES; index++) {
        this->textLines[index - 1] = this->textLines[index];
    }
    this->textLines[0] = std::move(message);

    return this->textLines;
}

WINDOW* Dungeon::getWindow() {
    return this->window;
}

u_char Dungeon::getFloorCount() {
    return this->floorCount;
}

Floor* Dungeon::getCurrentFloor() {
    return this->floor;
}

std::vector<Floor*> Dungeon::getFloors() {
    return this->floors;
}

Settings* Dungeon::getSettings() {
    return this->settings;
}

std::string* Dungeon::getTextLines() {
    return this->textLines;
}

std::string Dungeon::getText(u_char index) {
    return this->textLines[index];
}

Dungeon* Dungeon::setCurrentFloor(Floor* floor) {
    this->floor = floor;

    return this;
}