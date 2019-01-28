#ifndef SETTINGS_H
#define SETTINGS_H

#include <stdbool.h>
#include "resource.h"

#define DATA_PATH "/.rlg327/"
#define FILE_HEADING "RLG327-S2019"

typedef struct {
    bool doSave;
    bool doLoad;

    char* savePath;
    char* loadPath;

    u_char file_version;

} Settings;

#endif