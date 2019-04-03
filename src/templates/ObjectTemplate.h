#ifndef OBJECT_TEMPLATE_H
#define OBJECT_TEMPLATE_H

#define OBJECT_TEMPLATE_HEADING "RLG327 OBJECT DESCRIPTION 1"


namespace App {
    class ObjectTemplate {
    public:
        explicit ObjectTemplate(std::string& templateString);
        ~ObjectTemplate();

        static std::vector<ObjectTemplate*> GenerateTemplates(std::ifstream* inputFile);

//        Object* generateObjects();

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
