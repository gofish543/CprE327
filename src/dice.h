#ifndef DICE_H
#define DICE_H

#include <unistd.h>
#include <fcntl.h>
#include <random>
#include <sys/time.h>

#include <forward_declarations.h>
#include <exception.h>
#include <global.h>

namespace App {
    class Dice {
    public:
        Dice(int base, u_short rolls, u_short sides);
        explicit Dice(std::string* diceString);
        ~Dice();

        static int RandomNumberBetween(int min, int max);
        static u_int Hash3(u_int h1, u_int h2, u_int h3);

        u_int roll();

        /** GETTERS **/
        u_int getBase();
        u_short getRolls();
        u_short getSides();
        u_int getResult();
        /** GETTERS **/

        /** SETTERS **/
        Dice* setBase(int base);
        Dice* setRolls(u_short rolls);
        Dice* setSides(u_short sides);
        Dice* setResult(u_int result);
        /** SETTERS **/

    protected:

    private:
        u_int base;
        u_short rolls;
        u_short sides;

        int result;
    };
}

#endif