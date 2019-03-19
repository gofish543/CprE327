#include "settings.h"

Settings::Settings(int argc, char* argv[]) {
    this->loadPath = "";
    this->savePath = "";
    this->fileVersion = 0;
    this->load = false;
    this->save = false;
    this->expandedPrint = false;
    this->setNumberOfMonsters = false;
    this->nCursesPrint = true;

    if (this->loadArguments(argc, argv)) {
        exit(1);
    }
}

int Settings::loadArguments(int argc, char* argv[]) {
    std::string homePath = getenv("HOME");
    std::string dotFolder = DATA_PATH;
    std::string resourcePath = RESOURCE_PATH;

    u_char index;
    for (index = 0; index < argc; index++) {
        if (strcmp(argv[index], "--save") == 0 || strcmp(argv[index], "-s") == 0) {
            // Make sure there is a save path specified
            if (index + 1 == argc || strstarts(argv[index + 1], "-")) {
                printf("Using --save (-s) requires a file as the target to save to\n");
                return 1;
            }

            // The file path we will be using
            std::string filePath(argv[index + 1]);

            // Parse out file and path from filePath
            std::string fileName = this->getFileName(filePath);

            // Check if the file path exists
            if (!this->fileExists(filePath)) {
                // Check if the name of the file is the same as the file path
                if (!homePath.empty() &&
                    fileName == filePath &&
                    !this->createFolder(homePath)) {
                    filePath = homePath.append("/") + fileName;
                } else {
                    // File doesn't exist and we cant use the home directory error out
                    std::cout << "Using --save (-s) requires a valid file and file path. Potentially only a directory was provided or an invalid file path was presented\n" << std::endl;

                    return 1;
                }
            }

            this->savePath = filePath;
            this->save = true;
        } else if (strcmp(argv[index], "--load") == 0 || strcmp(argv[index], "-l") == 0) {
            // Make sure there is a load path specified
            if (index + 1 == argc || strstarts(argv[index + 1], "-")) {
                printf("Using --load (l) requires a file as the target to load from\n");
                return 1;
            }
            // The file path we will be using
            std::string filePath(argv[index + 1]);

            // Parse out file and path from filePath
            std::string fileName = this->getFileName(filePath);

            // Check if the file path exists
            if (!this->fileExists(filePath)) {
                // File doesn't exist and we cant use the home directory error out
                std::cout << "Using --load (-s) requires a valid file and file path. Potentially only a directory was provided or an invalid file path was presented\n" << std::endl;

                return 1;
            }

            this->loadPath = filePath;
            this->load = true;
        } else if (strcmp(argv[index], "--file_version") == 0 || strcmp(argv[index], "-fv") == 0) {
            if (index + 1 == argc || strstarts(argv[index + 1], "-")) {
                printf("Using --file_version (-fv) requires a file version integer\n");
                return 1;
            }

            switch (argv[index + 1][0]) {
                case '1':
                    this->fileVersion = 1;
                    break;
                case '0':
                default:
                    this->fileVersion = 0;
                    break;
            }
        } else if (strcmp(argv[index], "--nummon") == 0 || strcmp(argv[index], "-nm") == 0) {
            if (index + 1 == argc || strstarts(argv[index + 1], "-") || strtol(argv[index + 1], null, 0) == 0 || errno == ERANGE) {
                printf("Using --nummon (-nm) requires a number of monsters which must be greater than 0\n");
                return 1;
            }

            this->setNumberOfMonsters = true;
            this->numberOfMonsters = (u_short) strtol(argv[index + 1], null, 0);
        } else if (strcmp(argv[index], "--expanded_print") == 0 || strcmp(argv[index], "-ep") == 0) {
            this->expandedPrint = true;
        } else if (strcmp(argv[index], "--no_ncurses") == 0 || strcmp(argv[index], "-nnc") == 0) {
            this->nCursesPrint = false;
        }
    }

    return 0;
}

std::string Settings::getFileName(const std::string& string) {
    // Find the last slash location
    size_t index = string.rfind(PATH_SEPARATOR, string.length());
    if (index != std::string::npos) {
        // Return the stuff after the last slash
        return string.substr(index + 1, string.length() - index);
    }

    // Nothing to split, we have the file name
    return string;
}

bool Settings::fileExists(const std::string& fileString) {
    struct stat item = {};
    if (stat(fileString.c_str(), &item) != -1) {
        // Item exists
        if (item.st_mode & S_IFREG) {
            // It's a file
            return true;
        }
    }

    return false;
}

int Settings::createFolder(const std::string& path) {
    mkdir(path.c_str(), 0755);

    return 0;
}

bool Settings::doExpandPrint() {
    return this->expandedPrint;
}

bool Settings::doNCursesPrint() {
    return this->nCursesPrint;
}

bool Settings::doNumberOfMonsters() {
    return this->setNumberOfMonsters;
}

bool Settings::doSave() {
    return this->save;
}

bool Settings::doLoad() {
    return this->load;
}

u_char Settings::getFileVersion() {
    return this->fileVersion;
}

std::string Settings::getSavePath() {
    return this->savePath;
}

std::string Settings::getLoadPath() {
    return this->loadPath;
}

u_short Settings::getNumberOfMonsters() {
    return this->numberOfMonsters;
}