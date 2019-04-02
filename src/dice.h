#ifndef DICE_H
#define DICE_H

#include "../include/forward_declarations.h"
#include "resource.h"
#include <string>
#include <fstream>

namespace App {
    class Dice {
    public:
        Dice(int base, u_short rolls, u_short sides);
        explicit Dice(std::string* diceString);
        ~Dice();

        static int RandomNumberBetween(int min, int max);

        int roll();

        /** GETTERS **/
        int getBase();
        u_short getRolls();
        u_short getSides();
        int getResult();
        /** GETTERS **/

        /** SETTERS **/
        Dice* setBase(int base);
        Dice* setRolls(u_short rolls);
        Dice* setSides(u_short sides);
        Dice* setResult(int result);
        /** SETTERS **/

    protected:

    private:
        int base;
        u_short rolls;
        u_short sides;

        int result;
    };
}

#endif