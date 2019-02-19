#ifndef SETTINGS_H
#define SETTINGS_H

struct Settings;
typedef struct Settings Settings;

#include <stdbool.h>
#include <libgen.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "resource.h"

#define RESOURCE_PATH "./res"
#define DATA_PATH "/.rlg327/"
#define FILE_HEADING "RLG327-S2019"

struct Settings {
    bool expandedPrint;

    bool doSave;
    bool doLoad;

    bool doNumberOfMonsters;

    char* savePath;
    char* loadPath;

    u_char file_version;

    u_int numberOfMonsters;
};

Settings* settings_initialize(int argc, char* argv[]);
Settings* settings_terminate(Settings* settings);

int create_home_folders();
int settings_load_arguments(Settings* settings, int argc, char* argv[]);

#endif