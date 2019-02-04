#include "settings.h"

Settings* settings_initialize(int argc, char* argv[]) {
    Settings* settings = malloc(sizeof(Settings));

    if (load_function_arguments(settings, argc, argv) != 0) {
        return NULL;
    }

    return settings;
}

Settings* settings_terminate(Settings* settings) {
    if (settings->doSave) {
        free(settings->savePath);
    }

    if (settings->doLoad) {
        free(settings->loadPath);
    }

    free(settings);

    return NULL;
}


void create_home_folders() {
    char* homePath = getenv("HOME");
    u_char folderCount = 1;
    char* folders[] = {
            DATA_PATH
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

int load_function_arguments(Settings* settings, int argc, char* argv[]) {
    settings->loadPath = NULL;
    settings->savePath = NULL;
    settings->doLoad = false;
    settings->doSave = false;
    settings->file_version = 0;

    char* homePath = getenv("HOME");
    char dotFolder[] = DATA_PATH;
    char resourcePath[] = RESOURCE_PATH;

    u_char index;
    for (index = 0; index < argc; index++) {
        if (strcmp(argv[index], "--save") == 0) {
            // Make sure there is a save path specified
            if (index + 1 == argc || strstr(argv[index + 1], "--")) {
                printf("Using --save requires a file as the target to save to\n");
                return 1;
            }

            // The file path we will be using
            char* filePathCopy = argv[index + 1];
            char* filePath = malloc((strlen(filePathCopy) * sizeof(char)) + 1);
            strcpy(filePath, filePathCopy);

            // Step 1) Check if the path specified is either a file or invalid path
            DIR* checkDirectory = opendir(filePath);
            if (checkDirectory != NULL) {
                printf("Using --save requires a valid file and file path. Potentially only a directory was provided or an invalid file path was presented\n");
                closedir(checkDirectory);
                free(filePath);
                return 1;
            }

            // Step 2) Parse out file and path from filePath
            char* fileName = basename(filePathCopy);
            char* fileDir = dirname(filePathCopy);

            // Step 3) If fileDir is '.' => then we have just a file name
            // Try to use the home directory, if that fails, use the program's base path
            if (strcmp(fileDir, ".") == 0) {
                free(filePath);

                // Using file name, check if home directory exists
                DIR* homeDirectory = opendir(homePath);
                if (homeDirectory == NULL) {
                    // Save path to internal game file
                    filePath = malloc((sizeof(char) * strlen(resourcePath)) + (sizeof(char) * strlen(dotFolder) + (sizeof(char) * strlen(argv[index + 1]))) + 1);
                    sprintf(filePath, "%s%s%s", resourcePath, dotFolder, fileName);
                } else {
                    // Home directory exists
                    closedir(homeDirectory);
                    // Create home hidden folders
                    create_home_folders();
                    // Save path to that file
                    filePath = malloc((sizeof(char) * strlen(homePath)) + (sizeof(char) * strlen(dotFolder) + (sizeof(char) * strlen(argv[index + 1]))) + 1);
                    sprintf(filePath, "%s%s%s", homePath, dotFolder, fileName);
                }
            }
            // Try to load the path to the file
            FILE* checkFile = fopen(filePath, "a");
            if (checkFile == NULL) {
                printf("Could not open file provided or create it\n");
                free(filePath);
                return 1;
            } else {
                fclose(checkFile);
            }

            settings->doSave = true;
            settings->savePath = filePath;
        } else if (strcmp(argv[index], "--load") == 0) {
            // Make sure there is a save path specified
            if (index + 1 == argc || strstr(argv[index + 1], "--")) {
                printf("Using --load requires a file as the target to load from\n");
                return 1;
            }

            // The file path we will be using
            char* filePathCopy = argv[index + 1];
            char* filePath = malloc((strlen(filePathCopy) * sizeof(char)) + 1);
            strcpy(filePath, filePathCopy);

            // Step 1) Check if the path specified is either a file or invalid path
            DIR* checkDirectory = opendir(filePath);
            if (checkDirectory != NULL) {
                printf("Using --load requires a valid file and file path. Potentially only a directory was provided or an invalid file path was presented\n");
                closedir(checkDirectory);
                free(filePath);
                return 1;
            }

            // Step 2) Parse out file and path from filePath
            char* fileName = basename(filePathCopy);
            char* fileDir = dirname(filePathCopy);

            // Step 3) If fileDir is '.' => then we have just a file name
            // Try to use the home directory, if that fails, use the program's base path
            if (strcmp(fileDir, ".") == 0) {
                free(filePath);

                // Using file name, check if home directory exists
                DIR* homeDirectory = opendir(homePath);
                if (homeDirectory == NULL) {
                    // Save path to internal game file
                    filePath = malloc((sizeof(char) * strlen(resourcePath)) + (sizeof(char) * strlen(dotFolder) + (sizeof(char) * strlen(argv[index + 1]))) + 1);
                    sprintf(filePath, "%s%s%s", resourcePath, dotFolder, fileName);
                } else {
                    // Home directory exists
                    closedir(homeDirectory);
                    // Create home hidden folders
                    create_home_folders();
                    // Save path to that file
                    filePath = malloc((sizeof(char) * strlen(homePath)) + (sizeof(char) * strlen(dotFolder) + (sizeof(char) * strlen(argv[index + 1]))) + 1);
                    sprintf(filePath, "%s%s%s", homePath, dotFolder, fileName);
                }
            }
            // Try to load the path to the file
            FILE* checkFile = fopen(filePath, "a");
            if (checkFile == NULL) {
                printf("Could not open file provided or create it\n");
                free(filePath);
                return 1;
            } else {
                fclose(checkFile);
            }

            settings->doLoad = true;
            settings->loadPath = filePath;
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