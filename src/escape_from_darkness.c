#include "escape_from_darkness.h"


int main(int argc, char* argv[]) {
    Dungeon* dungeon = NULL;

    int status;

    status = initialize(&dungeon, argc, argv);
    if (status != 0) {
        printf("An error has occurred initializing the application, exiting\n");
        return status;
    }

    u_char index;
    for (index = 0; index < dungeon->floorCount; index++) {
        dungeon->currentFloor = index;
        print_current_floor(dungeon);
    }
    dungeon->currentFloor = 0;

    status = terminate(&dungeon);
    if (status != 0) {
        printf("An error has occurred terminating the application, exiting\n");
        return status;
    }

    return 0;
}

int initialize(Dungeon** dungeon, int argc, char* argv[]) {
    createApplicationFolders();

    Settings* settings = malloc(sizeof(Settings));
    int parseFunctionParams = loadFunctionArguments(settings, argc, argv);
    if (parseFunctionParams != 0) {
        free(settings);
        return parseFunctionParams;
    }

    *dungeon = dungeon_initialize(settings);

    if (*dungeon == NULL) {
        return 1;
    }


    return 0;
}

int terminate(Dungeon** dungeon) {
    *dungeon = dungeon_terminate(*dungeon);

    return 0;
}

void createApplicationFolders() {
    char* homePath = getenv("HOME");
    u_char folderCount = 1;
    char* folders[] = {
            "/.rlg327/"
    };

    char* tempCreation;
    u_char index;
    for (index = 0; index < folderCount; index++) {
        tempCreation = malloc((sizeof(char) * strlen(homePath)) + (sizeof(char) * strlen(folders[index])) + 1);

        // Load the folder path
        sprintf(tempCreation, "%s%s", homePath, folders[index]);
        mkdir(tempCreation, 0755);

        free(tempCreation);
    }
}

int loadFunctionArguments(Settings* settings, int argc, char* argv[]) {
    settings->loadPath = NULL;
    settings->savePath = NULL;
    settings->doLoad = false;
    settings->doSave = false;
    settings->file_version = 0;

    char* homePath = getenv("HOME");
    char dotFolder[] = DATA_PATH;

    u_char index;
    for (index = 0; index < argc; index++) {
        if (strcmp(argv[index], "--save") == 0) {
            if (index + 1 == argc || strstr(argv[index + 1], "--")) {
                printf("Using --save requires a file name as the target to save to\n");
                return 1;
            }

            settings->doSave = true;

            settings->savePath = malloc((sizeof(char) * strlen(homePath)) + (sizeof(char) * strlen(dotFolder) + (sizeof(char) * strlen(argv[index + 1]))) + 1);
            sprintf(settings->savePath, "%s%s%s", homePath, dotFolder, argv[index + 1]);
        } else if (strcmp(argv[index], "--load") == 0) {
            if (index + 1 == argc || strstr(argv[index + 1], "--")) {
                printf("Using --load requires a file name as the target file to load from\n");
                return 1;
            }

            settings->doLoad = true;
            settings->loadPath = malloc((sizeof(char) * strlen(homePath)) + (sizeof(char) * strlen(dotFolder) + (sizeof(char) * strlen(argv[index + 1]))) + 1);
            sprintf(settings->loadPath, "%s%s%s", homePath, dotFolder, argv[index + 1]);
        } else if (strcmp(argv[index], "--file_version") == 0) {
            if (index + 1 == argc || strstr(argv[index + 1], "--")) {
                printf("Using --file_version requires a file version integer\n");
                return 1;
            }

            switch (argv[index + 1][0]) {
                case '1':
                    settings->file_version = 1;
                    break;
                case '0':
                default:
                    settings->file_version = 0;
                    break;
            }
        }
    }

    return 0;
}