#ifndef SETTINGS_H
#define SETTINGS_H

namespace App {
    class Settings;
}

#define PATH_SEPARATOR '/'
#define RESOURCE_PATH "./res"
#define DATA_PATH "/.rlg327/"
#define FILE_HEADING "RLG327-S2019"

#include "resource.h"
#include <iostream>
#include <cerrno>
#include <cstdio>
#include <filesystem>
#include <cstring>
#include <dirent.h>
#include <libgen.h>
#include <sys/stat.h>
#include <sys/types.h>

namespace App {
    class Settings {
    private:
        bool expandedPrint;
        bool nCursesPrint;
        bool setNumberOfMonsters;
        bool save;
        bool load;

        u_char fileVersion;
        std::string savePath;
        std::string loadPath;

        u_short numberOfMonsters;

    protected:
        int loadArguments(int argc, char* argv[]);
        std::string getFileName(const std::string &string);
        bool fileExists(const std::string &fileString);
        int createFolder(const std::string &path);

    public:
        Settings(int argc, char* argv[]);

        bool doExpandPrint();
        bool doNCursesPrint();
        bool doNumberOfMonsters();
        bool doSave();
        bool doLoad();

        u_char getFileVersion();
        std::string getSavePath();
        std::string getLoadPath();
        u_short getNumberOfMonsters();

    };
}

using App::Settings;

#endif