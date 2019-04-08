#ifndef MONSTER_TEMPLATE_H
#define MONSTER_TEMPLATE_H

#define MONSTER_TEMPLATE_HEADING "RLG327 MONSTER DESCRIPTION 1"

#define MONSTER_TEMPLATE_NAME_VALID     0b000000001u
#define MONSTER_TEMPLATE_DESC_VALID     0b000000010u
#define MONSTER_TEMPLATE_COLOR_VALID    0b000000100u
#define MONSTER_TEMPLATE_SPEED_VALID     0b000001000u
#define MONSTER_TEMPLATE_ABILITIES_VALID     0b000010000u
#define MONSTER_TEMPLATE_HIT_POINTS_VALID     0b000100000u
#define MONSTER_TEMPLATE_ATTACK_DAMAGE_VALID     0b001000000u
#define MONSTER_TEMPLATE_SYMBOL_VALID     0b010000000u
#define MONSTER_TEMPLATE_RARITY_VALID     0b100000000u
#define MONSTER_TEMPLATE_IS_VALID       0b111111111u

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

#include <forward_declarations.h>

#include "../characters/monster.h"
#include "../dice.h"

namespace App {
    class MonsterTemplate {
    public:
        explicit MonsterTemplate(std::string& templateString);
        ~MonsterTemplate();

        static std::vector<MonsterTemplate*> GenerateTemplates(std::ifstream* inputFile);

        Monster* generateMonster(Floor* floor, u_char x, u_char y);

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
