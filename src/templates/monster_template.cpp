#include "monster_template.h"

MonsterTemplate::MonsterTemplate(std::string& templateString) {
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
        std::string buffer;
        std::stringstream templateStream(templateString);

        while (std::getline(templateStream, buffer, '\n')) {
            buffer = trim(buffer);

            if (!buffer.empty()) {
                if (strstarts(buffer, MONSTER_TEMPLATE_NAME)) {

                    this->name = buffer.substr(std::strlen(MONSTER_TEMPLATE_NAME));

                } else if (strstarts(buffer, MONSTER_TEMPLATE_DESCRIPTION)) {

                    while (std::getline(templateStream, buffer, '\n')) {
                        if (trim(buffer) == ".") {
                            break;
                        }
                        this->description += buffer;
                    }

                } else if (strstarts(buffer, MONSTER_TEMPLATE_COLOR)) {

                    for (auto const& color: split(trim(buffer.substr(std::strlen(MONSTER_TEMPLATE_COLOR))), ' ')) {
                        if (color == "RED") {
                            this->color = EFD_COLOR_RED;
                        } else if (color == "GREEN") {
                            this->color = EFD_COLOR_GREEN;
                        } else if (color == "BLUE") {
                            this->color = EFD_COLOR_BLUE;
                        } else if (color == "CYAN") {
                            this->color = EFD_COLOR_CYAN;
                        } else if (color == "YELLOW") {
                            this->color = EFD_COLOR_YELLOW;
                        } else if (color == "MAGENTA") {
                            this->color = EFD_COLOR_MAGENTA;
                        } else if (color == "WHITE") {
                            this->color = EFD_COLOR_WHITE;
                        } else if (color == "BLACK") {
                            this->color = EFD_COLOR_BLACK;
                        } else {
                            throw std::exception();
                        }
                        break;
                    }

                } else if (strstarts(buffer, MONSTER_TEMPLATE_SPEED)) {

                    try {
                        this->speed = new Dice(trim(buffer.substr(std::strlen(MONSTER_TEMPLATE_SPEED))));
                    } catch (Exception::DiceStringInvalidParse& exception) {
                        throw std::exception();
                    }

                } else if (strstarts(buffer, MONSTER_TEMPLATE_ABILITY)) {

                    for (auto const& ability: split(trim(buffer.substr(std::strlen(MONSTER_TEMPLATE_ABILITY))), ' ')) {
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
                        } else {
                            return;
                        }
                    }

                } else if (strstarts(buffer, MONSTER_TEMPLATE_HIT_POINTS)) {

                    try {
                        this->hitPoints = new Dice(trim(buffer.substr(std::strlen(MONSTER_TEMPLATE_HIT_POINTS))));
                    } catch (Exception::DiceStringInvalidParse& exception) {
                        throw std::exception();
                    }

                } else if (strstarts(buffer, MONSTER_TEMPLATE_DAMAGE)) {

                    try {
                        this->attackDamage = new Dice(trim(buffer.substr(std::strlen(MONSTER_TEMPLATE_DAMAGE))));
                    } catch (Exception::DiceStringInvalidParse& exception) {
                        throw std::exception();
                    }

                } else if (strstarts(buffer, MONSTER_TEMPLATE_SYMBOL)) {

                    this->symbol = trim(buffer.substr(std::strlen(MONSTER_TEMPLATE_SYMBOL))).at(0);

                } else if (strstarts(buffer, MONSTER_TEMPLATE_RARITY)) {

                    try {
                        this->rarity = std::stoi(trim(buffer.substr(std::strlen(MONSTER_TEMPLATE_RARITY))));
                    } catch (std::exception& exception) {
                        throw std::exception();
                    }
                }
            }
        }
    }
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

std::vector<MonsterTemplate*> MonsterTemplate::GenerateTemplates(std::ifstream* inputFile) {
    std::vector<MonsterTemplate*> monsterTemplates;

    std::string buffer;
    std::string templateBuffer;

    // Pull heading
    std::getline(*inputFile, buffer, '\n');
    if (trim(buffer) != MONSTER_TEMPLATE_HEADING) {
        printf(SHELL_TEXT_RED);
        printf("Invalid heading %s\n", buffer.c_str());
        printf(SHELL_DEFAULT "\n");
        exit(1);
    }

    while (std::getline(*inputFile, buffer, '\n')) {
        if (trim(buffer) == "BEGIN MONSTER") {
            templateBuffer = "";
        } else if (trim(buffer) == "END") {
            try {
                monsterTemplates.push_back(new MonsterTemplate(templateBuffer));
            } catch (std::exception& exception) {
            }
        } else {
            templateBuffer += buffer + "\n";
        }
    }

    return monsterTemplates;
}

Monster* MonsterTemplate::generateMonster(Floor* floor, u_char x, u_char y) {
    return new Monster(
            floor, x, y,
            &(this->name), &(this->description),
            this->color, this->speed->roll(),
            this->abilities, this->hitPoints->roll(),
            this->attackDamage->copy(),
            this->symbol, this->rarity
    );
}

/** GETTERS **/
std::string MonsterTemplate::getName() {
    return this->name;
}

std::string MonsterTemplate::getDescription() {
    return this->description;
}

u_short MonsterTemplate::getColor() {
    return this->color;
}

u_int MonsterTemplate::getSpeed() {
    return this->speed->roll();
}

u_short MonsterTemplate::getAbilities() {
    return this->abilities;
}

u_int MonsterTemplate::getHitPoints() {
    return this->hitPoints->roll();
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

/** SETTERS **/
/** SETTERS **/
