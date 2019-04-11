#ifndef OUTPUT_H
#define OUTPUT_H

#define OUTPUT_DEBUG_HARDNESS           0b0000001u
#define OUTPUT_DEBUG_TUNNELER           0b0000010u
#define OUTPUT_DEBUG_NON_TUNNELER       0b0000100u
#define OUTPUT_DEBUG_SHORTEST_PATH      0b0001000u
#define OUTPUT_DEBUG_MONSTER_TEMPLATES  0b0010000u
#define OUTPUT_DEBUG_OBJECT_TEMPLATES   0b0100000u
#define OUTPUT_DEBUG_TERMINATE          0b1000000u

#include <string>

#include <ncurses.h>
#include <cstdio>

#include <forward_declarations.h>
#include <colors.h>
#include <global.h>

namespace App {
    class Output {
    public:
        explicit Output(Dungeon* dungeon);
        ~Output();

        Output* print();
        Output* print(std::string* format, ...);
        Output* print(const char* format, ...);
        Output* print(u_int debugFunctions, Floor* floor = null);
        Output* printEndgame();
        Output* printMonsterMenu(u_short startIndex);

        /** DEBUG PRINTS **/
        Output* printHardness(Floor* floor);
        Output* printTunneler(Floor* floor);
        Output* printNonTunneler(Floor* floor);
        Output* printShortestPath(Floor* floor);
        Output* printMonsterTemplates();
        Output* printObjectTemplates();
        void printTerminate();
        Output* printError(std::string* format, ...);
        Output* printError(const char* format, ...);
        /** DEBUG PRINTS **/

        /** COLORED **/
        void setColor(u_int index);
        void resetColor(u_int index);
        /** COLORED **/

        /** GETTERS **/
        /** GETTERS **/

        /** SETTERS **/
        /** SETTERS **/
    protected:

    private:
        Dungeon* dungeon;
        bool doNCurses;
        bool dontNCurses;
        bool doExpanded;
        WINDOW* window;
    };
}

#include "Dungeon.h"

#endif