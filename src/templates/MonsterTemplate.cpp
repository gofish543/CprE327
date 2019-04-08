#include "MonsterTemplate.h"

MonsterTemplate::MonsterTemplate(std::string& templateString) {
    u_short isValid = 0;

    this->name = "";
    this->description = "";
    this->color = 0;
    this->speed = null;
    this->abilities = 0;
    this->attackDamage = null;
    this->hitPoints = null;
    this->symbol = 0;
    this->rarity = 0;

    if (!templateString.empty()) {
        std::stringstream templateStream(templateString);
        std::string buffer;
        std::string untrimmedBuffer;
        while (std::getline(templateStream, buffer, '\n')) {
            buffer = trim(buffer);
            if (!buffer.empty()) {
                if (strstarts(buffer, "NAME")) {
                    buffer = buffer.substr(4);
                    buffer = trim(buffer);

                    this->name = buffer;
                    isValid |= 0b1u;
                } else if (strstarts(buffer, "DESC")) {
                    buffer = buffer.substr(4);
                    buffer = trim(buffer);

                    if (!buffer.empty()) {
                        this->description += buffer;
                    }

                    while (std::getline(templateStream, buffer, '\n')) {
                        untrimmedBuffer = buffer;
                        trim(buffer);
                        if (buffer == ".") {
                            break;
                        }

                        this->description += untrimmedBuffer + "\n";
                    }

                    isValid |= 0b10u;
                } else if (strstarts(buffer, "COLOR")) {
                    buffer = buffer.substr(5);

                    std::vector<std::string> colors = split(trim(buffer), ' ');
                    for (auto const& color: colors) {
                        if (color == "RED") {
                            this->color |= NCURSES_TEXT_RED;
                        } else if (color == "GREEN") {
                            this->color |= NCURSES_TEXT_GREEN;
                        } else if (color == "BLUE") {
                            this->color |= NCURSES_TEXT_BLUE;
                        } else if (color == "CYAN") {
                            this->color |= NCURSES_TEXT_CYAN;
                        } else if (color == "YELLOW") {
                            this->color |= NCURSES_TEXT_YELLOW;
                        } else if (color == "MAGENTA") {
                            this->color |= NCURSES_TEXT_MAGENTA;
                        } else if (color == "WHITE") {
                            this->color |= NCURSES_TEXT_WHITE;
                        } else if (color == "BLACK") {
                            this->color |= NCURSES_TEXT_BLACK;
                        }
                    }

                    isValid |= 0b100u;
                } else if (strstarts(buffer, "SPEED")) {
                    buffer = buffer.substr(5);
                    buffer = trim(buffer);
                    this->speed = new Dice(&buffer);

                    if (this->speed->getResult() == 0) {
                        isValid |= 0b1000u;
                    }
                } else if (strstarts(buffer, "ABIL")) {
                    buffer = buffer.substr(4);
                    buffer = trim(buffer);

                    std::vector<std::string> abilities = split(buffer, ' ');

                    for (auto const& ability: abilities) {
                        if (ability == "SMART") {
                            this->abilities |= MONSTER_INTELLIGENT;
                        } else if (ability == "TELE") {
                            this->abilities |= MONSTER_TELEPATHIC;
                        } else if (ability == "TUNNEL") {
                            this->abilities |= MONSTER_TUNNELER;
                        } else if (ability == "ERRATIC") {
                            this->abilities |= MONSTER_ERRATIC;
                        } else if (ability == "PASS") {
                            this->abilities |= MONSTER_PASS;
                        } else if (ability == "PICKUP") {
                            this->abilities |= MONSTER_PICKUP;
                        } else if (ability == "DESTROY") {
                            this->abilities |= MONSTER_DESTROY;
                        } else if (ability == "UNIQ") {
                            this->abilities |= MONSTER_UNIQ;
                        } else if (ability == "BOSS") {
                            this->abilities |= MONSTER_BOSS;
                        }
                    }

                    isValid |= 0b10000u;
                } else if (strstarts(buffer, "HP")) {
                    buffer = buffer.substr(2);
                    buffer = trim(buffer);

                    this->hitPoints = new Dice(&buffer);

                    if (this->hitPoints->getResult() == 0) {
                        isValid |= 0b100000u;
                    }
                } else if (strstarts(buffer, "DAM")) {
                    buffer = buffer.substr(3);
                    buffer = trim(buffer);

                    this->attackDamage = new Dice(&buffer);

                    if (this->attackDamage->getResult() == 0) {
                        isValid |= 0b1000000u;
                    }
                } else if (strstarts(buffer, "SYMB")) {
                    buffer = buffer.substr(4);
                    buffer = trim(buffer);

                    this->symbol = buffer.at(0);
                    isValid |= 0b10000000u;
                } else if (strstarts(buffer, "RRTY")) {
                    buffer = buffer.substr(4);
                    buffer = trim(buffer);

                    try {
                        this->rarity = std::stoi(buffer);
                        isValid |= 0b10000000u;
                    } catch (std::exception& exception) {

                    }
                }
            }
        }
    }

    this->validTemplate = isValid & 0b111111111u;
}

MonsterTemplate::~MonsterTemplate() {
    if (this->speed != null) {
        delete (this->speed);
    }
    if (this->hitPoints != null) {
        delete (this->hitPoints);
    }
    if (this->attackDamage != null) {
        delete (this->attackDamage);
    }
}

Monster* MonsterTemplate::generateMonster(Floor* floor, u_char x, u_char y) {
    return new Monster(floor, x, y, &(this->name), &(this->description), this->color, this->speed->roll(), this->abilities, this->hitPoints->roll(), this->attackDamage->roll(), this->symbol, this->rarity);
}

std::vector<MonsterTemplate*> MonsterTemplate::GenerateTemplates(std::ifstream* inputFile) {
    std::vector<MonsterTemplate*> monsterTemplates;

    // Pull heading
    std::string heading;

    do {
        std::getline(*inputFile, heading, '\n');
        std::cout << heading << std::endl;

        printf("Getting line %s\n", heading.c_str());

        if (!trim(heading).empty()) {
            break;
        }
    } while (true);

    if (trim(heading) != MONSTER_TEMPLATE_HEADING) {
        printf("Invalid heading %s\n", heading.c_str());
        exit(1);
    }

    std::string buffer;
    std::string unTrimmedBuffer;
    std::string templateBuffer;
    MonsterTemplate* monsterTemplate;
    while (std::getline(*inputFile, buffer, '\n')) {
        unTrimmedBuffer = buffer;
        trim(buffer);
        if (buffer == "BEGIN MONSTER") {
            templateBuffer = "";
        } else if (buffer == "END") {
            monsterTemplate = new MonsterTemplate(templateBuffer);
            if (monsterTemplate->isValid()) {
                monsterTemplates.push_back(monsterTemplate);
            } else {
                delete (monsterTemplate);
            }
        } else {
            templateBuffer += (unTrimmedBuffer + "\n");
        }
    }

    return monsterTemplates;
}

/** GETTERS **/
bool MonsterTemplate::isValid() {
    return this->validTemplate;
}

std::string MonsterTemplate::getName() {
    return this->name;
}

std::string MonsterTemplate::getDescription() {
    return this->description;
}

u_char MonsterTemplate::getColor() {
    return this->color;
}

u_int MonsterTemplate::getSpeed() {
    return this->attackDamage->roll();
}

u_short MonsterTemplate::getAbilities() {
    return this->abilities;
}

u_int MonsterTemplate::getHitPoints() {
    return this->attackDamage->roll();
}

u_int MonsterTemplate::getAttackDamage() {
    return this->attackDamage->roll();
}

u_char MonsterTemplate::getSymbol() {
    return this->symbol;
}

u_char MonsterTemplate::getRarity() {
    return this->rarity;
}
/** GETTERS **/
