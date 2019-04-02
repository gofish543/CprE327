#ifndef MONSTER_TEMPLATE_H
#define MONSTER_TEMPLATE_H

#define MONSTER_TEMPLATE_HEADING "RLG327 MONSTER DESCRIPTION 1"

#include "../../include/forward_declarations.h"
#include "../characters/monster.h"
#include "../dice.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

namespace App {
    class MonsterTemplate {
    public:
        explicit MonsterTemplate(std::string &templateString);
        ~MonsterTemplate();

        static std::vector<MonsterTemplate*> GenerateTemplates(std::ifstream* inputFile);

        Monster* generateMonster();

        /** GETTERS **/
        bool isValid();
        std::string getName();
        std::string getDescription();
        u_char getColor();
        u_int getSpeed();
        u_short getAbilities();
        u_int getHitPoints();
        u_int getAttackDamage();
        u_char getSymbol();
        u_char getRarity();
        /** GETTERS **/
    protected:
    private:
        bool validTemplate;

        std::string name;
        std::string description;
        u_char color;
        Dice* speed;
        u_short abilities;
        Dice* hitPoints;
        Dice* attackDamage;
        u_char symbol;
        u_char rarity;
    };
}
#endif
