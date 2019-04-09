#include "MonsterTemplate.h"

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

    u_short isValid = 0;

    if (!templateString.empty()) {
        std::stringstream templateStream(templateString);
        std::string buffer;
        std::string untrimmedBuffer;

        while (std::getline(templateStream, buffer, '\n')) {
            if (!trim(buffer).empty()) {
                if (strstarts(buffer, "NAME")) {

                    buffer = buffer.substr(4);
                    this->name = trim(buffer);
                    isValid += MONSTER_TEMPLATE_NAME_VALID;

                } else if (strstarts(buffer, "DESC")) {

                    while (std::getline(templateStream, buffer, '\n')) {
                        untrimmedBuffer = buffer;
                        trim(buffer);
                        if (buffer == ".") {
                            break;
                        }
                        this->description += untrimmedBuffer;
                    }
                    isValid += MONSTER_TEMPLATE_DESC_VALID;

                } else if (strstarts(buffer, "COLOR")) {

                    buffer = buffer.substr(5);
                    for (auto const& color: split(trim(buffer), ' ')) {
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
                            continue;
                        }
                        break;
                    }
                    isValid += MONSTER_TEMPLATE_COLOR_VALID;

                } else if (strstarts(buffer, "SPEED")) {

                    try {
                        buffer = buffer.substr(5);
                        this->speed = new Dice(&trim(buffer));
                        isValid += MONSTER_TEMPLATE_SPEED_VALID;
                    } catch (Exception::DiceStringInvalidParse& exception) {
                        this->validTemplate = false;
                        return;
                    }

                } else if (strstarts(buffer, "ABIL")) {

                    buffer = buffer.substr(4);
                    for (auto const& ability: split(trim(buffer), ' ')) {
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
                    isValid += MONSTER_TEMPLATE_ABILITIES_VALID;

                } else if (strstarts(buffer, "HP")) {

                    try {
                        buffer = buffer.substr(2);
                        this->hitPoints = new Dice(&trim(buffer));
                        isValid += MONSTER_TEMPLATE_HIT_POINTS_VALID;
                    } catch (Exception::DiceStringInvalidParse& exception) {
                        this->validTemplate = false;
                        return;
                    }

                } else if (strstarts(buffer, "DAM")) {

                    try {
                        buffer = buffer.substr(3);
                        this->attackDamage = new Dice(&trim(buffer));
                        isValid += MONSTER_TEMPLATE_ATTACK_DAMAGE_VALID;
                    } catch (Exception::DiceStringInvalidParse& exception) {
                        this->validTemplate = false;
                        return;
                    }

                } else if (strstarts(buffer, "SYMB")) {

                    buffer = buffer.substr(4);
                    this->symbol = trim(buffer).at(0);
                    isValid += MONSTER_TEMPLATE_SYMBOL_VALID;

                } else if (strstarts(buffer, "RRTY")) {

                    try {
                        buffer = buffer.substr(4);
                        this->rarity = std::stoi(trim(buffer));
                        isValid += MONSTER_TEMPLATE_RARITY_VALID;
                    } catch (std::exception& exception) {
                        this->validTemplate = false;
                        return;
                    }
                }
            }
        }
    }

    this->validTemplate = isValid == MONSTER_TEMPLATE_IS_VALID;
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
    std::getline(*inputFile, heading, '\n');

    std::string buffer;
    std::string unTrimmedBuffer;
    std::string templateBuffer;

    MonsterTemplate* monsterTemplate;

    if (trim(heading) != MONSTER_TEMPLATE_HEADING) {
        printf(SHELL_TEXT_RED);
        printf("Invalid heading %s\n", heading.c_str());
        printf(SHELL_DEFAULT "\n");
        exit(1);
    }

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
