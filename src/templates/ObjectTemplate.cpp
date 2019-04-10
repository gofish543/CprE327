#include "ObjectTemplate.h"

ObjectTemplate::ObjectTemplate(std::string& templateString) {
    this->name = "";
    this->description = "";
    this->itemType = 0;
    this->color = 0;
    this->hitBonus = null;
    this->damageBonus = null;
    this->dodgeBonus = null;
    this->defenseBonus = null;
    this->weight = null;
    this->speedBonus = null;
    this->specialAttribute = null;
    this->value = null;
    this->isArtifact = false;
    this->rarity = 0;

    if (!templateString.empty()) {
        std::string buffer;
        std::stringstream templateStream(templateString);

        while (std::getline(templateStream, buffer, '\n')) {
            buffer = trim(buffer);

            if (!buffer.empty()) {
                if (strstarts(buffer, OBJECT_TEMPLATE_NAME)) {

                    this->name = buffer.substr(std::strlen(OBJECT_TEMPLATE_NAME));

                } else if (strstarts(buffer, OBJECT_TEMPLATE_DESCRIPTION)) {

                    while (std::getline(templateStream, buffer, '\n')) {
                        if (trim(buffer) == ".") {
                            break;
                        }
                        this->description += buffer;
                    }

                } else if (strstarts(buffer, OBJECT_TEMPLATE_TYPE)) {

                    buffer = trim(buffer.substr(std::strlen(OBJECT_TEMPLATE_TYPE)));

                    if (buffer == OBJECT_WEAPON_KEYWORD) {
                        this->itemType = OBJECT_WEAPON;
                    } else if (buffer == OBJECT_OFFHAND_KEYWORD) {
                        this->itemType = OBJECT_OFFHAND;
                    } else if (buffer == OBJECT_RANGED_KEYWORD) {
                        this->itemType = OBJECT_RANGED;
                    } else if (buffer == OBJECT_ARMOR_KEYWORD) {
                        this->itemType = OBJECT_ARMOR;
                    } else if (buffer == OBJECT_HELMET_KEYWORD) {
                        this->itemType = OBJECT_HELMET;
                    } else if (buffer == OBJECT_CLOAK_KEYWORD) {
                        this->itemType = OBJECT_CLOAK;
                    } else if (buffer == OBJECT_GLOVES_KEYWORD) {
                        this->itemType = OBJECT_GLOVES;
                    } else if (buffer == OBJECT_BOOTS_KEYWORD) {
                        this->itemType = OBJECT_BOOTS;
                    } else if (buffer == OBJECT_RING_KEYWORD) {
                        this->itemType = OBJECT_RING;
                    } else if (buffer == OBJECT_AMULET_KEYWORD) {
                        this->itemType = OBJECT_AMULET;
                    } else if (buffer == OBJECT_LIGHT_KEYWORD) {
                        this->itemType = OBJECT_LIGHT;
                    } else if (buffer == OBJECT_SCROLL_KEYWORD) {
                        this->itemType = OBJECT_SCROLL;
                    } else if (buffer == OBJECT_BOOK_KEYWORD) {
                        this->itemType = OBJECT_BOOK;
                    } else if (buffer == OBJECT_FLASK_KEYWORD) {
                        this->itemType = OBJECT_FLASK;
                    } else if (buffer == OBJECT_GOLD_KEYWORD) {
                        this->itemType = OBJECT_GOLD;
                    } else if (buffer == OBJECT_AMMUNITION_KEYWORD) {
                        this->itemType = OBJECT_AMMUNITION;
                    } else if (buffer == OBJECT_FOOD_KEYWORD) {
                        this->itemType = OBJECT_FOOD;
                    } else if (buffer == OBJECT_WAND_KEYWORD) {
                        this->itemType = OBJECT_WAND;
                    } else if (buffer == OBJECT_CONTAINER_KEYWORD) {
                        this->itemType = OBJECT_CONTAINER;
                    } else {
                        throw std::exception();
                    }

                } else if (strstarts(buffer, OBJECT_TEMPLATE_COLOR)) {

                    for (auto const& color: split(trim(buffer.substr(std::strlen(OBJECT_TEMPLATE_COLOR))), ' ')) {
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

                } else if (strstarts(buffer, OBJECT_TEMPLATE_HIT_BONUS)) {

                    try {
                        this->hitBonus = new Dice(trim(buffer.substr(std::strlen(OBJECT_TEMPLATE_HIT_BONUS))));
                    } catch (Exception::DiceStringInvalidParse& exception) {
                        throw std::exception();
                    }

                } else if (strstarts(buffer, OBJECT_TEMPLATE_DAMAGE_BONUS)) {

                    try {
                        this->damageBonus = new Dice(trim(buffer.substr(std::strlen(OBJECT_TEMPLATE_DAMAGE_BONUS))));
                    } catch (Exception::DiceStringInvalidParse& exception) {
                        throw std::exception();
                    }

                } else if (strstarts(buffer, OBJECT_TEMPLATE_DODGE_BONUS)) {

                    try {
                        this->dodgeBonus = new Dice(trim(buffer.substr(std::strlen(OBJECT_TEMPLATE_DODGE_BONUS))));
                    } catch (Exception::DiceStringInvalidParse& exception) {
                        throw std::exception();
                    }

                } else if (strstarts(buffer, OBJECT_TEMPLATE_DEFENSE_BONUS)) {

                    try {
                        this->defenseBonus = new Dice(trim(buffer.substr(std::strlen(OBJECT_TEMPLATE_DEFENSE_BONUS))));
                    } catch (Exception::DiceStringInvalidParse& exception) {
                        throw std::exception();
                    }

                } else if (strstarts(buffer, OBJECT_TEMPLATE_WEIGHT)) {

                    try {
                        this->weight = new Dice(trim(buffer.substr(std::strlen(OBJECT_TEMPLATE_WEIGHT))));
                    } catch (Exception::DiceStringInvalidParse& exception) {
                        throw std::exception();
                    }

                } else if (strstarts(buffer, OBJECT_TEMPLATE_SPEED_BONUS)) {

                    try {
                        this->speedBonus = new Dice(trim(buffer.substr(std::strlen(OBJECT_TEMPLATE_SPEED_BONUS))));
                    } catch (Exception::DiceStringInvalidParse& exception) {
                        throw std::exception();
                    }

                } else if (strstarts(buffer, OBJECT_TEMPLATE_SPECIAL_ATTRIBUTE)) {

                    try {
                        this->specialAttribute = new Dice(trim(buffer.substr(std::strlen(OBJECT_TEMPLATE_SPECIAL_ATTRIBUTE))));
                    } catch (Exception::DiceStringInvalidParse& exception) {
                        throw std::exception();
                    }

                } else if (strstarts(buffer, OBJECT_TEMPLATE_VALUE)) {

                    try {
                        this->value = new Dice(trim(buffer.substr(std::strlen(OBJECT_TEMPLATE_VALUE))));
                    } catch (Exception::DiceStringInvalidParse& exception) {
                        throw std::exception();
                    }

                } else if (strstarts(buffer, OBJECT_TEMPLATE_ARTIFACT_STATUS)) {
                    buffer = trim(buffer.substr(std::strlen(OBJECT_TEMPLATE_ARTIFACT_STATUS)));

                    if (buffer == "TRUE") {
                        this->isArtifact = true;
                    } else if (buffer == "FALSE") {
                        this->isArtifact = false;
                    } else {
                        throw std::exception();
                    }

                } else if (strstarts(buffer, OBJECT_TEMPLATE_RARITY)) {

                    try {
                        this->rarity = std::stoi(trim(buffer.substr(std::strlen(OBJECT_TEMPLATE_RARITY))));
                    } catch (std::exception& exception) {
                        throw std::exception();
                    }
                }
            }
        }
    }
}

ObjectTemplate::~ObjectTemplate() {
    if (this->hitBonus != null) {
        delete (this->hitBonus);
    }
    if (this->damageBonus != null) {
        delete (this->damageBonus);
    }
    if (this->dodgeBonus != null) {
        delete (this->dodgeBonus);
    }
    if (this->defenseBonus != null) {
        delete (this->defenseBonus);
    }
    if (this->weight != null) {
        delete (this->weight);
    }
    if (this->speedBonus != null) {
        delete (this->speedBonus);
    }
    if (this->specialAttribute != null) {
        delete (this->specialAttribute);
    }
    if (this->value != null) {
        delete (this->value);
    }
}

Object* ObjectTemplate::generateObject(Floor* floor, u_char x, u_char y) {
    return new Object(floor, x, y, &this->name, &this->description, this->itemType, this->color, this->hitBonus->roll(), this->damageBonus->roll(), this->dodgeBonus->roll(), this->defenseBonus->roll(), this->weight->roll(), this->speedBonus->roll(), this->specialAttribute->roll(), this->value->roll(), this->isArtifact, this->rarity);
}

std::vector<ObjectTemplate*> ObjectTemplate::GenerateTemplates(std::ifstream* inputFile) {
    std::vector<ObjectTemplate*> objectTemplates;

    std::string buffer;
    std::string templateBuffer;
    // Pull heading
    std::getline(*inputFile, buffer, '\n');

    if (trim(buffer) != OBJECT_TEMPLATE_HEADING) {
        printf(SHELL_TEXT_RED);
        printf("Invalid heading %s\n", buffer.c_str());
        printf(SHELL_DEFAULT "\n");
        exit(1);
    }

    while (std::getline(*inputFile, buffer, '\n')) {
        if (trim(buffer) == "BEGIN OBJECT") {
        } else if (trim(buffer) == "END") {
            try {
                objectTemplates.push_back(new ObjectTemplate(templateBuffer));
            } catch (std::exception& exception) {
            }
        } else {
            templateBuffer += buffer + "\n";
        }
    }

    return objectTemplates;
}

/** GETTERS **/
std::string ObjectTemplate::getName() {
    return this->name;
}

std::string ObjectTemplate::getDescription() {
    return this->description;
}

u_int ObjectTemplate::getItemType() {
    return this->itemType;
}

u_short ObjectTemplate::getColor() {
    return this->color;
}

u_short ObjectTemplate::getHitBonus() {
    return this->hitBonus->roll();
}

u_short ObjectTemplate::getDamageBonus() {
    return this->damageBonus->roll();
}

u_short ObjectTemplate::getDodgeBonus() {
    return this->dodgeBonus->roll();
}

u_short ObjectTemplate::getDefenseBonus() {
    return this->defenseBonus->roll();
}

u_short ObjectTemplate::getWeight() {
    return this->weight->roll();
}

short ObjectTemplate::getSpeedBonus() {
    return this->speedBonus->roll();
}

u_short ObjectTemplate::getSpecialAttribute() {
    return this->specialAttribute->roll();
}

u_short ObjectTemplate::getValue() {
    return this->value->roll();
}

bool ObjectTemplate::getIsArtifact() {
    return this->isArtifact;
}

u_char ObjectTemplate::getRarity() {
    return this->rarity;
}
/** GETTERS **/

/** SETTERS **/
/** SETTERS **/
