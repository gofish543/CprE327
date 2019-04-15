#ifndef OBJECT_H
#define OBJECT_H

#define OBJECT_WEAPON_KEYWORD       "WEAPON"
#define OBJECT_OFFHAND_KEYWORD      "OFFHAND"
#define OBJECT_RANGED_KEYWORD       "RANGED"
#define OBJECT_ARMOR_KEYWORD        "ARMOR"
#define OBJECT_HELMET_KEYWORD       "HELMET"
#define OBJECT_CLOAK_KEYWORD        "CLOAK"
#define OBJECT_GLOVES_KEYWORD       "GLOVES"
#define OBJECT_BOOTS_KEYWORD        "BOOTS"
#define OBJECT_RING_KEYWORD         "RING"
#define OBJECT_AMULET_KEYWORD       "AMULET"
#define OBJECT_LIGHT_KEYWORD        "LIGHT"
#define OBJECT_SCROLL_KEYWORD       "SCROLL"
#define OBJECT_BOOK_KEYWORD         "BOOK"
#define OBJECT_FLASK_KEYWORD        "FLASK"
#define OBJECT_GOLD_KEYWORD         "GOLD"
#define OBJECT_AMMUNITION_KEYWORD   "AMMUNITION"
#define OBJECT_FOOD_KEYWORD         "FOOD"
#define OBJECT_WAND_KEYWORD         "WAND"
#define OBJECT_CONTAINER_KEYWORD    "CONTAINER"

#define OBJECT_WEAPON       0b0000000000000000001u
#define OBJECT_OFFHAND      0b0000000000000000010u
#define OBJECT_RANGED       0b0000000000000000100u
#define OBJECT_ARMOR        0b0000000000000001000u
#define OBJECT_HELMET       0b0000000000000010000u
#define OBJECT_CLOAK        0b0000000000000100000u
#define OBJECT_GLOVES       0b0000000000001000000u
#define OBJECT_BOOTS        0b0000000000010000000u
#define OBJECT_RING         0b0000000000100000000u
#define OBJECT_AMULET       0b0000000001000000000u
#define OBJECT_LIGHT        0b0000000010000000000u
#define OBJECT_SCROLL       0b0000000100000000000u
#define OBJECT_BOOK         0b0000001000000000000u
#define OBJECT_FLASK        0b0000010000000000000u
#define OBJECT_GOLD         0b0000100000000000000u
#define OBJECT_AMMUNITION   0b0001000000000000000u
#define OBJECT_FOOD         0b0010000000000000000u
#define OBJECT_WAND         0b0100000000000000000u
#define OBJECT_CONTAINER    0b1000000000000000000u

#include <string>

#include <forward_declarations.h>
#include <character_listings.h>
#include <global.h>

namespace App {
    class Object {
    public:
        Object(Floor* floor, u_char x, u_char y, std::string* name, std::string* description, u_int itemType, u_short color, Dice* hitBonus, Dice* damageBonus, Dice* dodgeBonus, Dice* defenseBonus, Dice* weight, Dice* speedBonus, Dice* specialAttribute, Dice* value, bool isArtifact, u_char rarity);
        ~Object();

        u_char getCharacter();

        /** GETTERS **/
        Floor* getFloor();
        u_char getX();
        u_char getY();
        const std::string& getName();
        const std::string& getDescription();
        u_int getItemType();
        std::string getTypeString();
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
        /** SETTERS **/

        /** TYPE CONVERSION **/
        bool isWeapon();
        bool isOffHand();
        bool isRanged();
        bool isArmor();
        bool isHelmet();
        bool isCloak();
        bool isGloves();
        bool isBoots();
        bool isRing();
        bool isAmulet();
        bool isLight();
        bool isScroll();
        bool isBook();
        bool isFlask();
        bool isGold();
        bool isAmmunition();
        bool isFood();
        bool isWand();
        bool isContainer();
        /** TYPE CONVERSION **/
    protected:

    private:
        Floor* floor;
        u_char x;
        u_char y;

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

#endif
