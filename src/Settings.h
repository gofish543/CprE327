#ifndef SETTINGS_H
#define SETTINGS_H

#define SETTINGS_RES_FOLDER "./res/"
#define SETTINGS_HOME_FOLDER "/.rlg327/"

#define SETTINGS_MONSTER_DESC "monster_desc.txt"
#define SETTINGS_OBJECT_DESC "object_desc.txt"

#include <fstream>
#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <forward_declarations.h>
#include <global.h>
#include <colors.h>
#include <exception.h>

#include "Resource.h"

namespace App {
    class Settings {

    public:
        Settings(int argc, char* argv[]);
        ~Settings();

        /** GETTERS **/
        bool doExpandPrint();
        bool doNCursesPrint();
        bool doNumberOfMonsters();
        bool doSave();
        bool doLoad();
        bool doFogOfWar();
        u_char getFileVersion();
        std::string* getSavePath();
        std::string* getLoadPath();
        u_short getNumberOfMonsters();
        std::ifstream* getMonsterDesc();
        std::ifstream* getObjectDesc();
        /** GETTERS **/

        /** SETTERS **/
        Settings* setExpandedPrint(bool expandedPrint);
        Settings* setNCursesPrint(bool nCursesPrint);
        Settings* setCanNumberOfMonsters(bool canNumberOfMonsters);
        Settings* setSave(bool save);
        Settings* setLoad(bool load);
        Settings* setFogOfWar(bool fogOfWar);
        Settings* setFileVersion(u_char fileVersion);
        Settings* setSavePath(std::string& savePath);
        Settings* setLoadPath(std::string& loadPath);
        Settings* setNumberOfMonsters(u_short numberOfMonsters);
        Settings* setMonsterDesc(std::string& path);
        Settings* setObjectDesc(std::string& path);
        /** SETTERS **/

    protected:
        Settings* loadArguments(int argc, char* argv[]);
        Settings* loadFiles();
        std::string getFileName(const std::string& string);
        bool fileExists(const std::string& fileString);
        int createFolder(const std::string& path);

    private:
        bool expandedPrint;
        bool nCursesPrint;
        bool canNumberOfMonsters;
        bool save;
        bool load;
        bool fogOfWar;

        u_char fileVersion;
        std::string* savePath;
        std::string* loadPath;

        u_short numberOfMonsters;

        std::ifstream* monsterDesc;
        std::ifstream* objectDesc;
    };
}

#endif