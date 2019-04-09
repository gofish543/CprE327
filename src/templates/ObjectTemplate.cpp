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
                    isValid += OBJECT_TEMPLATE_NAME_VALID;

                } else if (strstarts(buffer, "DESC")) {

                    while (std::getline(templateStream, buffer, '\n')) {
                        untrimmedBuffer = buffer;
                        trim(buffer);
                        if (buffer == ".") {
                            break;
                        }
                        this->description += untrimmedBuffer;
                    }
                    isValid += OBJECT_TEMPLATE_DESC_VALID;

                } else if (strstarts(buffer, "TYPE")) {

                    buffer = buffer.substr(4);
                    trim(buffer);

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
                        this->validTemplate = false;
                        return;
                    }
                    isValid += OBJECT_TEMPLATE_TYPE_VALID;

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
                    isValid += OBJECT_TEMPLATE_COLOR_VALID;

                } else if (strstarts(buffer, "HIT")) {

                    try {
                        buffer = buffer.substr(3);
                        this->hitBonus = new Dice(&trim(buffer));
                        isValid += OBJECT_TEMPLATE_HIT_BONUS_VALID;
                    } catch (Exception::DiceStringInvalidParse& exception) {
                        this->validTemplate = false;
                        return;
                    }

                } else if (strstarts(buffer, "DAM")) {

                    try {
                        buffer = buffer.substr(3);
                        this->damageBonus = new Dice(&trim(buffer));
                        isValid += OBJECT_TEMPLATE_DAMAGE_BONUS_VALID;
                    } catch (Exception::DiceStringInvalidParse& exception) {
                        this->validTemplate = false;
                        return;
                    }

                } else if (strstarts(buffer, "DODGE")) {

                    try {
                        buffer = buffer.substr(5);
                        this->dodgeBonus = new Dice(&trim(buffer));
                        isValid += OBJECT_TEMPLATE_DODGE_BONUS_VALID;
                    } catch (Exception::DiceStringInvalidParse& exception) {
                        this->validTemplate = false;
                        return;
                    }

                } else if (strstarts(buffer, "DEF")) {

                    try {
                        buffer = buffer.substr(3);
                        this->defenseBonus = new Dice(&trim(buffer));
                        isValid += OBJECT_TEMPLATE_DEFENSE_BONUS_VALID;
                    } catch (Exception::DiceStringInvalidParse& exception) {
                        this->validTemplate = false;
                        return;
                    }

                } else if (strstarts(buffer, "WEIGHT")) {

                    try {
                        buffer = buffer.substr(6);
                        this->weight = new Dice(&trim(buffer));
                        isValid += OBJECT_TEMPLATE_WEIGHT_VALID;
                    } catch (Exception::DiceStringInvalidParse& exception) {
                        this->validTemplate = false;
                        return;
                    }

                } else if (strstarts(buffer, "SPEED")) {

                    try {
                        buffer = buffer.substr(5);
                        this->speedBonus = new Dice(&trim(buffer));
                        isValid += OBJECT_TEMPLATE_SPEED_BONUS_VALID;
                    } catch (Exception::DiceStringInvalidParse& exception) {
                        this->validTemplate = false;
                        return;
                    }

                } else if (strstarts(buffer, "ATTR")) {

                    try {
                        buffer = buffer.substr(4);
                        this->specialAttribute = new Dice(&trim(buffer));
                        isValid += OBJECT_TEMPLATE_SPECIAL_ATTRIBUTE_VALID;
                    } catch (Exception::DiceStringInvalidParse& exception) {
                        this->validTemplate = false;
                        return;
                    }

                } else if (strstarts(buffer, "VAL")) {

                    try {
                        buffer = buffer.substr(3);
                        this->value = new Dice(&trim(buffer));
                        isValid += OBJECT_TEMPLATE_VALUE_VALID;
                    } catch (Exception::DiceStringInvalidParse& exception) {
                        this->validTemplate = false;
                        return;
                    }

                } else if (strstarts(buffer, "ART")) {
                    buffer = buffer.substr(3);
                    trim(buffer);

                    if (buffer == "TRUE") {
                        this->isArtifact = true;
                    } else if (buffer == "FALSE") {
                        this->isArtifact = false;
                    } else {
                        this->validTemplate = false;
                        return;
                    }

                    isValid += OBJECT_TEMPLATE_ARTIFACT_STATUS_VALID;

                } else if (strstarts(buffer, "RRTY")) {

                    try {
                        buffer = buffer.substr(4);
                        this->rarity = std::stoi(trim(buffer));
                        isValid += OBJECT_TEMPLATE_RARITY_VALID;
                    } catch (std::exception& exception) {
                        this->validTemplate = false;
                        return;
                    }
                }
            }
        }
    }

    this->validTemplate = isValid == OBJECT_TEMPLATE_IS_VALID;
}

ObjectTemplate::~ObjectTemplate() {
    if (this->hitBonus) {
        delete (this->hitBonus);
    }
    if (this->damageBonus) {
        delete (this->damageBonus);
    }
    if (this->dodgeBonus) {
        delete (this->dodgeBonus);
    }
    if (this->defenseBonus) {
        delete (this->defenseBonus);
    }
    if (this->weight) {
        delete (this->weight);
    }
    if (this->speedBonus) {
        delete (this->speedBonus);
    }
    if (this->specialAttribute) {
        delete (this->specialAttribute);
    }
    if (this->value) {
        delete (this->value);
    }
}

Object* ObjectTemplate::generateObject(Floor* floor, u_char x, u_char y) {
    return null;
//    return new Object();
}

std::vector<ObjectTemplate*> ObjectTemplate::GenerateTemplates(std::ifstream* inputFile) {
    std::vector<ObjectTemplate*> objectTemplates;

    // Pull heading
    std::string heading;
    std::getline(*inputFile, heading, '\n');

    std::string buffer;
    std::string unTrimmedBuffer;
    std::string templateBuffer;

    ObjectTemplate* objectTemplate;

    if (trim(heading) != OBJECT_TEMPLATE_HEADING) {
        printf(SHELL_TEXT_RED);
        printf("Invalid heading %s\n", heading.c_str());
        printf(SHELL_DEFAULT
               "\n");
        exit(1);
    }

    while (std::getline(*inputFile, buffer, '\n')) {
        unTrimmedBuffer = buffer;
        trim(buffer);
        if (buffer == "BEGIN OBJECT") {
            templateBuffer = "";
        } else if (buffer == "END") {
            objectTemplate = new ObjectTemplate(templateBuffer);
            if (objectTemplate->isValid()) {
                objectTemplates.push_back(objectTemplate);
            } else {
                delete (objectTemplate);
            }
        } else {
            templateBuffer += (unTrimmedBuffer + "\n");
        }
    }

    return objectTemplates;
}

/** GETTERS **/
bool ObjectTemplate::isValid() {
    return this->validTemplate;
}

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
ObjectTemplate* ObjectTemplate::setIsValid(bool isValid) {
    this->validTemplate = isValid;

    return this;
}
/** SETTERS **/
