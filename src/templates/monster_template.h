#ifndef MONSTER_TEMPLATE_H
#define MONSTER_TEMPLATE_H

#define MONSTER_TEMPLATE_HEADING "RLG327 MONSTER DESCRIPTION 1"

#define MONSTER_TEMPLATE_NAME           "NAME"
#define MONSTER_TEMPLATE_DESCRIPTION    "DESC"
#define MONSTER_TEMPLATE_COLOR          "COLOR"
#define MONSTER_TEMPLATE_SPEED          "SPEED"
#define MONSTER_TEMPLATE_ABILITY        "ABIL"
#define MONSTER_TEMPLATE_HIT_POINTS     "HP"
#define MONSTER_TEMPLATE_DAMAGE         "DAM"
#define MONSTER_TEMPLATE_SYMBOL         "SYMB"
#define MONSTER_TEMPLATE_RARITY         "RRTY"

#include <string>
#include <vector>
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>

#include <forward_declarations.h>
#include <global.h>

namespace App {
    class MonsterTemplate {
    public:
        explicit MonsterTemplate(std::string& templateString);
        ~MonsterTemplate();

        static std::vector<MonsterTemplate*> GenerateTemplates(std::ifstream* inputFile);

        Monster* generateMonster(Floor* floor, u_char x, u_char y);

        /** GETTERS **/
        std::string getName();
        std::string getDescription();
        u_short getColor();
        u_int getSpeed();
        u_short getAbilities();
        u_int getHitPoints();
        u_int getAttackDamage();
        u_char getSymbol();
        u_char getRarity();
        /** GETTERS **/

        /** SETTERS **/
        /** SETTERS **/
    protected:

    private:
        std::string name;
        std::string description;
        u_short color;
        Dice* speed;
        u_short abilities;
        Dice* hitPoints;
        Dice* attackDamage;
        u_char symbol;
        u_char rarity;
    };
}

#include "../characters/monster.h"
#include "../dice.h"

#endif
