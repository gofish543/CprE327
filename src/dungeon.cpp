#include "dungeon.h"

Dungeon::Dungeon(int argc, char* argv[]) {
    u_char index;

    for (index = 0; index < DUNGEON_TEXT_LINES; index++) {
        this->textLines[index] = "";
    }

    this->settings = new Settings(argc, argv);
    this->player = null;

    if (settings->doLoad()) {
        load_from_file(this);
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
        this->setCurrentFloor(this->getFloors().at(0));
    }

    Monster::RunDijkstraOnFloor(this->getCurrentFloor());

    if (this->settings->doNCursesPrint()) {
        this->window = initscr();
        raw();
        keypad(stdscr, true);
        noecho();
    } else {
        this->window = null;
    }

    this->eventManager = new EventManager(this);

    this->eventManager->addToQueue(new Event(0, event_type_player, this->getPlayer(), Player::HandleEvent, Player::NextEventTick));
    for (index = 0; index < this->getCurrentFloor()->getMonsterCount(); index++) {
        this->eventManager->addToQueue(new Event(1 + index, event_type_monster, this->getCurrentFloor()->getMonster(index), Monster::HandleEvent, Monster::NextEventTick));
    }

    this->getPlayer()->updateVisibility();

}

Dungeon::~Dungeon() {
    u_char index;

    if (this->getSettings()->doSave()) {
        // Save the game state
        save_to_file(this);
    }

    for (index = 0; index < this->floorCount; index++) {
        delete (this->floors.at(index));
    }

    if (this->settings->doNCursesPrint()) {
        endwin();
    }

    delete (this->eventManager);
    delete (this->settings);
}

std::string* Dungeon::prependText(std::string message, ...) {
    u_char index;

    for (index = 1; index < DUNGEON_TEXT_LINES; index++) {
        this->textLines[index - 1] = std::move(this->textLines[index]);
    }
    this->textLines[0] = std::move(message);

    return this->textLines;
}

std::string* Dungeon::appendText(std::string message, ...) {
    u_char index;

    for (index = 0; index < DUNGEON_TEXT_LINES - 1; index++) {
        this->textLines[index] = std::move(this->textLines[index + 1]);
    }

    this->textLines[DUNGEON_TEXT_LINES - 1] = std::move(message);

    return this->textLines;
}

/** GETTERS **/
Floor* Dungeon::getFloor(u_char index) {
    if (index < this->getFloorCount()) {
        return this->floors[index];
    } else {
        throw "Floor out of bounds exception";
    }
}
std::vector<Floor*> Dungeon::getFloors() {
    return this->floors;
}

Floor* Dungeon::getCurrentFloor() {
    return this->floor;
}

Settings* Dungeon::getSettings() {
    return this->settings;
}

EventManager* Dungeon::getEventManager() {
    return this->eventManager;
}

Player* Dungeon::getPlayer() {
    return this->player;
}

WINDOW* Dungeon::getWindow() {
    return this->window;
}

u_char Dungeon::getFloorCount() {
    return this->floorCount;
}

std::string* Dungeon::getTextLines() {
    return this->textLines;
}

std::string Dungeon::getText(u_char index) {
    return this->textLines[index];
}
/** GETTERS **/

/** SETTERS **/
Dungeon* Dungeon::setFloor(Floor* floor, u_char index) {
    if (index == UCHAR_MAX) {
        this->floors.push_back(floor);
    } else if (index < this->getFloorCount()) {
        this->floors[index] = floor;
    } else {
        throw "Floor out of bounds exception";
    }

    return this;
}

Dungeon* Dungeon::setCurrentFloor(Floor* floor) {
    this->floor = floor;

    return this;
}

Dungeon* Dungeon::setSettings(Settings* settings) {
    this->settings = settings;

    return this;
}

Dungeon* Dungeon::setEventManager(EventManager* eventManager) {
    this->eventManager = eventManager;

    return this;
}

Dungeon* Dungeon::setPlayer(Player* player) {
    this->player = player;

    return this;
}

Dungeon* Dungeon::setWindow(WINDOW* window) {
    this->window = window;

    return this;
}

Dungeon* Dungeon::setFloorCount(u_char floorCount) {
    this->floorCount = floorCount;

    return this;
}
/** SETTERS **/