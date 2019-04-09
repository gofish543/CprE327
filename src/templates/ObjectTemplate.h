#ifndef OBJECT_TEMPLATE_H
#define OBJECT_TEMPLATE_H

#define OBJECT_TEMPLATE_HEADING "RLG327 OBJECT DESCRIPTION 1"

#define OBJECT_TEMPLATE_NAME_VALID                  0b00000000000001u
#define OBJECT_TEMPLATE_DESC_VALID                  0b00000000000010u
#define OBJECT_TEMPLATE_TYPE_VALID                  0b00000000000100u
#define OBJECT_TEMPLATE_COLOR_VALID                 0b00000000001000u
#define OBJECT_TEMPLATE_HIT_BONUS_VALID             0b00000000010000u
#define OBJECT_TEMPLATE_DAMAGE_BONUS_VALID          0b00000000100000u
#define OBJECT_TEMPLATE_DODGE_BONUS_VALID           0b00000001000000u
#define OBJECT_TEMPLATE_DEFENSE_BONUS_VALID         0b00000010000000u
#define OBJECT_TEMPLATE_WEIGHT_VALID                0b00000100000000u
#define OBJECT_TEMPLATE_SPEED_BONUS_VALID           0b00001000000000u
#define OBJECT_TEMPLATE_SPECIAL_ATTRIBUTE_VALID     0b00010000000000u
#define OBJECT_TEMPLATE_VALUE_VALID                 0b00100000000000u
#define OBJECT_TEMPLATE_ARTIFACT_STATUS_VALID       0b01000000000000u
#define OBJECT_TEMPLATE_RARITY_VALID                0b10000000000000u
#define OBJECT_TEMPLATE_IS_VALID                    0b11111111111111u

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

#include <forward_declarations.h>
#include <colors.h>
#include <global.h>

#include "../resource.h"

namespace App {
    class ObjectTemplate {
    public:
        explicit ObjectTemplate(std::string& templateString);
        ~ObjectTemplate();

        static std::vector<ObjectTemplate*> GenerateTemplates(std::ifstream* inputFile);

        Object* generateObject(Floor* floor, u_char x, u_char y);

        /** GETTERS **/
        bool isValid();
        std::string getName();
        std::string getDescription();
        u_int getItemType();
        u_short getColor();
        u_short getHitBonus();
        u_short getDamageBonus();
        u_short getDodgeBonus();
        u_short getDefenseBonus();
        u_short getWeight();
        short getSpeedBonus();
        u_short getSpecialAttribute();
        u_short getValue();
        bool getIsArtifact();
        u_char getRarity();
        /** GETTERS **/

        /** SETTERS **/
        ObjectTemplate* setIsValid(bool isValid);
        /** SETTERS **/
    protected:

    private:
        bool validTemplate;

        std::string name;
        std::string description;
        u_int itemType;
        u_short color;
        Dice* hitBonus;
        Dice* damageBonus;
        Dice* dodgeBonus;
        Dice* defenseBonus;
        Dice* weight;
        Dice* speedBonus;
        Dice* specialAttribute;
        Dice* value;
        bool isArtifact;
        u_char rarity;
    };
}

#include "../dice.h"
#include "../objects/Object.h"

#endif
