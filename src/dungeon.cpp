#include "dungeon.h"

Dungeon::Dungeon(int argc, char* argv[]) : textLines{new std::string, new std::string, new std::string} {
    u_char index;

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
        init_color(NCURSES_BG_GREY_LIGHT, 300, 300, 300);
        init_color(NCURSES_BG_GREY_DARK, 100, 100, 100);

        init_pair(NCURSES_BG_GREY_LIGHT | NCURSES_BLACK, NCURSES_BLACK, NCURSES_BG_GREY_LIGHT);
        init_pair(NCURSES_BG_GREY_LIGHT | NCURSES_RED, NCURSES_RED, NCURSES_BG_GREY_LIGHT);
        init_pair(NCURSES_BG_GREY_LIGHT | NCURSES_GREEN, NCURSES_GREEN, NCURSES_BG_GREY_LIGHT);
        init_pair(NCURSES_BG_GREY_LIGHT | NCURSES_YELLOW, NCURSES_YELLOW, NCURSES_BG_GREY_LIGHT);
        init_pair(NCURSES_BG_GREY_LIGHT | NCURSES_BLUE, NCURSES_BLUE, NCURSES_BG_GREY_LIGHT);
        init_pair(NCURSES_BG_GREY_LIGHT | NCURSES_MAGENTA, NCURSES_MAGENTA, NCURSES_BG_GREY_LIGHT);
        init_pair(NCURSES_BG_GREY_LIGHT | NCURSES_CYAN, NCURSES_CYAN, NCURSES_BG_GREY_LIGHT);
        init_pair(NCURSES_BG_GREY_LIGHT | NCURSES_WHITE, NCURSES_WHITE, NCURSES_BG_GREY_LIGHT);

        init_pair(NCURSES_BG_GREY_DARK | NCURSES_BLACK, NCURSES_BLACK, NCURSES_BG_GREY_DARK);
        init_pair(NCURSES_BG_GREY_DARK | NCURSES_RED, NCURSES_RED, NCURSES_BG_GREY_DARK);
        init_pair(NCURSES_BG_GREY_DARK | NCURSES_GREEN, NCURSES_GREEN, NCURSES_BG_GREY_DARK);
        init_pair(NCURSES_BG_GREY_DARK | NCURSES_YELLOW, NCURSES_YELLOW, NCURSES_BG_GREY_DARK);
        init_pair(NCURSES_BG_GREY_DARK | NCURSES_BLUE, NCURSES_BLUE, NCURSES_BG_GREY_DARK);
        init_pair(NCURSES_BG_GREY_DARK | NCURSES_MAGENTA, NCURSES_MAGENTA, NCURSES_BG_GREY_DARK);
        init_pair(NCURSES_BG_GREY_DARK | NCURSES_CYAN, NCURSES_CYAN, NCURSES_BG_GREY_DARK);
        init_pair(NCURSES_BG_GREY_DARK | NCURSES_WHITE, NCURSES_WHITE, NCURSES_BG_GREY_DARK);
    } else {
        this->window = null;
    }

    this->output = new Output(this);
    this->eventManager = new EventManager(this);
    this->boss = null;

    this->monsterTemplates = MonsterTemplate::GenerateTemplates(this->settings->getMonsterDesc());
    this->objectTemplates = ObjectTemplate::GenerateTemplates(this->settings->getObjectDesc());

    if (this->settings->doLoad()) {
        load_from_file(this);
    } else {
        this->player = new Player(null, 0, 0);

        u_short stairCount = Dice::RandomNumberBetween(FLOOR_STAIRS_MIN, FLOOR_STAIRS_MAX);

        for (index = 0; index < Dice::RandomNumberBetween(DUNGEON_FLOORS_MIN, DUNGEON_FLOORS_MAX); index++) {
            this->floors.push_back(
                    new Floor(
                            this,
                            index,
                            Dice::RandomNumberBetween(FLOOR_ROOMS_MIN, FLOOR_ROOMS_MAX),
                            stairCount,
                            stairCount,
                            Dice::RandomNumberBetween(FLOOR_MONSTERS_MIN, FLOOR_MONSTERS_MAX),
                            Dice::RandomNumberBetween(FLOOR_OBJECTS_MIN, FLOOR_OBJECTS_MAX)
                    )
            );
        }

        this->floor = this->getFloor(0);
    }

    // If after creation the boss was not created, we need to create it now
    for (index = 0; index < this->monsterTemplates.size(); index++) {
        if (this->monsterTemplates[index]->getAbilities() & MONSTER_BOSS) {
            // Put the boss on the final floor, first room
            this->boss = this->monsterTemplates[index]->generateMonster(this->floors.back(), this->floors.back()->getRoom(0)->randomXInside(), this->floors.back()->getRoom(0)->randomYInside());
            // Kill what ever was previously there
            if(this->floors.back()->getCharacterAt(this->boss->getX(), this->boss->getY())) {
                this->floors.back()->getCharacterAt(this->boss->getX(), this->boss->getY())->killCharacter();
            }

            // Place our boss there
            this->floors.back()->setCharacterAt(this->boss, this->boss->getX(), this->boss->getY());
        }
    }

    if (this->boss == null) {
        // No boss template was defined, and no boss was created... terminate
        throw Exception::NoBossCreated();
    }

    Monster::RunDijkstraOnFloor(this->floor);

    this->eventManager->addToQueue(new Event(0, event_type_player, this->player, Player::HandleEvent, Player::NextEventTick));
    for (index = 0; index < this->floor->getMonsterCount(); index++) {
        this->eventManager->addToQueue(
                new Event(1 + index, event_type_monster, this->floor->getMonster(index), Monster::HandleEvent, Monster::NextEventTick)
        );
    }
    this->eventManager->addToQueue(new Event(1 + index, event_type_monster, this->boss, Monster::HandleEvent, Monster::NextEventTick));

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

    for (index = 0; index < this->objectTemplates.size(); index++) {
        delete (this->objectTemplates[index]);
    }

    for (index = 0; index < this->monsterTemplates.size(); index++) {
        delete (this->monsterTemplates[index]);
    }

    delete (this->player);
    delete (this->boss);
    delete (this->eventManager);
    delete (this->output);
    delete (this->settings);

    for (index = 0; index < DUNGEON_TEXT_LINES; index++) {
        delete (this->textLines[index]);
    }
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
    char buffer[DUNGEON_FLOOR_WIDTH + 1];

    va_list args;
    va_start(args, format);
    vsnprintf(buffer, DUNGEON_FLOOR_WIDTH + 1, format->c_str(), args);
    va_end(args);

    return this->prependText(buffer);
}

std::string* Dungeon::appendText(const std::string* format, ...) {
    char buffer[DUNGEON_FLOOR_WIDTH + 1];

    va_list args;
    va_start(args, format);
    vsnprintf(buffer, DUNGEON_FLOOR_WIDTH + 1, format->c_str(), args);
    va_end(args);

    return this->appendText(buffer);
}

MonsterTemplate* Dungeon::randomMonsterTemplate() {
    // Verify at least 1 monster template is valid
    if (this->monsterTemplates.empty()) {
        throw Exception::MonsterTemplatesEmpty();
    }

    MonsterTemplate* monsterTemplate;
    do {
        monsterTemplate = this->monsterTemplates[Dice::RandomNumberBetween(0, this->monsterTemplates.size() - 1)];
    } while (monsterTemplate->getRarity() >= Dice::RandomNumberBetween(0, 100) || (monsterTemplate->getAbilities() & MONSTER_BOSS));

    return monsterTemplate;
}

ObjectTemplate* Dungeon::randomObjectTemplate() {
    // Verify at least 1 monster template is valid
    if (this->objectTemplates.empty()) {
        throw Exception::ObjectTemplatesEmpty();
    }

    ObjectTemplate* objectTemplate;
    do {
        objectTemplate = this->objectTemplates[Dice::RandomNumberBetween(0, this->objectTemplates.size() - 1)];
    } while (objectTemplate->getRarity() >= Dice::RandomNumberBetween(0, 100));

    return objectTemplate;
}

bool Dungeon::continueGame() {
    if (this->boss == null) {
        this->getOutput()->printError("No boss created. Terminating safely\n");
        return false;
    }
    if (this->eventManager->getQueue()->size == 0) {
        this->getOutput()->printError("Queue of events is empty. Terminating safely\n");
        return false;
    }
    return
            this->player->isAlive() &&
            !this->player->getRequestTerminate() &&
            this->boss->isAlive();
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

Monster* Dungeon::getBoss() {
    return this->boss;
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

Dungeon* Dungeon::removeMonsterTemplate(MonsterTemplate* monsterTemplate) {
    this->monsterTemplates.erase(std::remove(this->monsterTemplates.begin(), this->monsterTemplates.end(), monsterTemplate), this->monsterTemplates.end());
    delete (monsterTemplate);
    return this;
}

Dungeon* Dungeon::removeObjectTemplate(ObjectTemplate* objectTemplate) {
    this->objectTemplates.erase(std::remove(this->objectTemplates.begin(), this->objectTemplates.end(), objectTemplate), this->objectTemplates.end());
    delete (objectTemplate);
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

Dungeon* Dungeon::setBoss(Monster* boss) {
    this->boss = boss;

    return this;
}
/** SETTERS **/