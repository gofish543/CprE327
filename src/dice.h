#ifndef DICE_H
#define DICE_H

#include <forward_declarations.h>
#include <exception.h>
#include <global.h>

#include <string>
#include <fstream>
#include <unistd.h>
#include <fcntl.h>
#include <random>
#include <sys/time.h>

namespace App {
    class Dice {
    public:
        Dice(int base, u_short rolls, u_short sides);
        explicit Dice(std::string* diceString);
        ~Dice();

        static int RandomNumberBetween(int min, int max);
        static u_int Hash3(unsigned int h1, unsigned int h2, unsigned int h3);

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