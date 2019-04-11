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
        explicit Dice(std::string diceString);
        Dice(int base, int rolls, int sides);
        ~Dice();

        static int RandomNumberBetween(int min, int max);
        static u_int Hash3(u_int h1, u_int h2, u_int h3);

        int roll();

        /** GETTERS **/
        int getBase();
        int getRolls();
        int getSides();
        int getResult();
        /** GETTERS **/

        /** SETTERS **/
        Dice* setBase(int base);
        Dice* setRolls(int rolls);
        Dice* setSides(int sides);
        Dice* setResult(int result);
        /** SETTERS **/

    protected:

    private:
        int base;
        int rolls;
        int sides;

        int result;
    };
}

#endif