#ifndef SETTINGS_H
#define SETTINGS_H

#include <stdbool.h>
#include <libgen.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "resource.h"

struct Settings;
typedef struct Settings Settings;

#define RESOURCE_PATH "./res"
#define DATA_PATH "/.rlg327/"
#define FILE_HEADING "RLG327-S2019"

struct Settings{
    bool doSave;
    bool doLoad;

    char* savePath;
    char* loadPath;

    u_char file_version;
};

Settings* settings_initialize(int argc, char* argv[]);
Settings* settings_terminate(Settings* settings);

void create_home_folders();
int load_function_arguments(Settings* settings, int argc, char* argv[]);

#endif