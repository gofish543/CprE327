#include "dungeon.h"

Dungeon::Dungeon(int argc, char* argv[]) {
    u_char index;

    this->window = null;
    this->settings = new Settings(argc, argv);

    if (this->settings->doNCursesPrint()) {
        this->window = initscr();
        raw();
        keypad(stdscr, true);
        noecho();
        curs_set(false);
        use_default_colors();
        start_color();

        init_color(NCURSES_BLACK, 0, 0, 0);
        init_color(NCURSES_RED, 1000, 0, 0);
        init_color(NCURSES_GREEN, 0, 1000, 0);
        init_color(NCURSES_YELLOW, 1000, 1000, 0);
        init_color(NCURSES_BLUE, 0, 0, 1000);
        init_color(NCURSES_MAGENTA, 1000, 0, 1000);
        init_color(NCURSES_CYAN, 0, 1000, 1000);
        init_color(NCURSES_WHITE, 1000, 1000, 1000);

        init_pair(NCURSES_BLACK, NCURSES_BLACK, NCURSES_BLACK);
        init_pair(NCURSES_RED, NCURSES_RED, NCURSES_BLACK);
        init_pair(NCURSES_GREEN, NCURSES_GREEN, NCURSES_BLACK);
        init_pair(NCURSES_YELLOW, NCURSES_YELLOW, NCURSES_BLACK);
        init_pair(NCURSES_BLUE, NCURSES_BLUE, NCURSES_BLACK);
        init_pair(NCURSES_MAGENTA, NCURSES_MAGENTA, NCURSES_BLACK);
        init_pair(NCURSES_CYAN, NCURSES_CYAN, NCURSES_BLACK);
        init_pair(NCURSES_WHITE, NCURSES_WHITE, NCURSES_BLACK);
    }

    this->output = new Output(this);
    this->eventManager = new EventManager(this);

    this->monsterTemplates = MonsterTemplate::GenerateTemplates(this->settings->getMonsterDesc());
//    this->objectTemplates = ObjectTemplate::GenerateTemplates(this->settings->getObjectDesc());

    this->player = new Player(null, 0, 0);

    this->floor = null;

    for (index = 0; index < DUNGEON_TEXT_LINES; index++) {
        this->textLines[index] = new std::string();
    }

    if (this->settings->doLoad()) {
        load_from_file(this);
    } else {
        auto stairCount = u_short(Dice::RandomNumberBetween(FLOOR_STAIRS_MIN, FLOOR_STAIRS_MAX));

        for (index = 0; index < u_char(Dice::RandomNumberBetween(DUNGEON_FLOORS_MIN, DUNGEON_FLOORS_MAX)); index++) {
            this->floors.push_back(
                    new Floor(
                            this,
                            index,
                            u_short(Dice::RandomNumberBetween(FLOOR_ROOMS_MIN, FLOOR_ROOMS_MAX)),
                            stairCount,
                            stairCount,
                            u_short(Dice::RandomNumberBetween(FLOOR_MONSTERS_MIN, FLOOR_MONSTERS_MAX))
                    )
            );
        }

        this->floor = this->getFloor(0);
    }

    Monster::RunDijkstraOnFloor(this->floor);

    this->eventManager->addToQueue(new Event(0, event_type_player, this->player, Player::HandleEvent, Player::NextEventTick));
    for (index = 0; index < this->floor->getMonsterCount(); index++) {
        this->eventManager
                ->addToQueue(new Event(1 + index, event_type_monster, this->floor->getMonster(index), Monster::HandleEvent, Monster::NextEventTick));
    }

    this->player->updateVisibility();
}

Dungeon::~Dungeon() {
    u_char index;

    if (this->settings->doNCursesPrint()) {
        endwin();
    }

    if (this->getSettings()->doSave()) {
        // Save the game state
        save_to_file(this);
    }

    for (index = 0; index < this->floors.size(); index++) {
        delete (this->floors[index]);
    }

    for (index = 0; index < DUNGEON_TEXT_LINES; index++) {
        delete (this->textLines[index]);
    }

//    for (index = 0; index < this->objectTemplates.size(); index++) {
//        delete (this->objectTemplates[index]);
//    }

    for (index = 0; index < this->monsterTemplates.size(); index++) {
        delete (this->monsterTemplates[index]);
    }

    delete (this->player);
    delete (this->eventManager);
    delete (this->output);
    delete (this->settings);
}

std::string* Dungeon::prependText(const std::string& text) {
    u_char index;
    for (index = DUNGEON_TEXT_LINES - 1; index > 0; index--) {
        this->textLines[index]->assign(*(this->textLines[index - 1]));
    }

    this->textLines[0]->assign(text);

    return this->textLines[0];
}

std::string* Dungeon::appendText(const std::string& text) {
    u_char index;
    for (index = DUNGEON_TEXT_LINES - 1; index > 0; index--) {
        this->textLines[index - 1]->assign(*(this->textLines[index]));
    }

    this->textLines[DUNGEON_TEXT_LINES - 1]->assign(text);

    return this->textLines[DUNGEON_TEXT_LINES - 1];
}

std::string* Dungeon::prependText(const std::string* format, ...) {
    u_char index;
    char buffer[DUNGEON_FLOOR_WIDTH + 1];
    va_list args;
    va_start(args, format);

    for (index = DUNGEON_TEXT_LINES - 1; index > 0; index--) {
        this->textLines[index]->assign(*(this->textLines[index - 1]));
    }

    vsnprintf(buffer, DUNGEON_FLOOR_WIDTH + 1, format->c_str(), args);
    this->textLines[0]->assign(buffer, DUNGEON_FLOOR_WIDTH);

    va_end(args);

    return this->textLines[0];
}

std::string* Dungeon::appendText(const std::string* format, ...) {
    u_char index;
    char buffer[DUNGEON_FLOOR_WIDTH + 1];

    for (index = DUNGEON_TEXT_LINES - 1; index > 0; index--) {
        this->textLines[index - 1]->assign(*(this->textLines[index]));
    }

    va_list args;
    va_start(args, format);

    vsnprintf(buffer, DUNGEON_FLOOR_WIDTH, format->c_str(), args);

    va_end(args);

    this->textLines[DUNGEON_TEXT_LINES - 1]->assign(buffer, DUNGEON_FLOOR_WIDTH);

    return this->textLines[DUNGEON_TEXT_LINES - 1];
}

MonsterTemplate* Dungeon::randomMonsterTemplate() {
    MonsterTemplate* monsterTemplate;
    do {
        monsterTemplate = this->monsterTemplates[Dice::RandomNumberBetween(0, this->monsterTemplates.size() - 1)];
    } while (!monsterTemplate->isValid());

    return monsterTemplate;
}

/** GETTERS **/
Floor* Dungeon::getFloor(u_char index) {
    if (index < this->floors.size()) {
        return this->floors[index];
    } else {
        throw Exception::FloorOutOfBounds();
    }
}

Floor* Dungeon::getCurrentFloor() {
    return this->floor;
}

u_char Dungeon::getFloorCount() {
    return this->floors.size();
}

std::vector<MonsterTemplate*> Dungeon::getMonsterTemplates() {
    return this->monsterTemplates;
}

std::vector<ObjectTemplate*> Dungeon::getObjectTemplates() {
    return this->objectTemplates;
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

Output* Dungeon::getOutput() {
    return this->output;
}

std::string Dungeon::getText(u_char index) {
    if (index < DUNGEON_TEXT_LINES) {
        return *this->textLines[index];
    } else {
        throw Exception::DungeonTextOutOfBounds();
    }
}
/** GETTERS **/

/** SETTERS **/
Dungeon* Dungeon::addFloor(Floor* floor, u_char index) {
    if (index == U_CHAR_MAX) {
        this->floors.push_back(floor);
    } else if (index < this->floors.size()) {
        if (this->floors[index]) {
            if (this->floors[index] == this->floor) {
                throw Exception::DungeonDeletingCurrentFloor();
            }

            delete (this->floors[index]);
        }

        this->floors[index] = floor;
    } else {
        throw Exception::FloorOutOfBounds();
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
/** SETTERS **/