#include "Settings.h"

Settings::Settings(int argc, char* argv[]) {
    this->loadPath = new std::string("");
    this->savePath = new std::string("");
    this->fileVersion = 0;
    this->numberOfMonsters = 0;
    this->load = false;
    this->save = false;
    this->expandedPrint = false;
    this->canNumberOfMonsters = false;
    this->nCursesPrint = true;
    this->fogOfWar = true;

    this->monsterDesc = new std::ifstream();
    this->objectDesc = new std::ifstream();

    this->loadArguments(argc, argv)->loadFiles();
}

Settings::~Settings() {
    if (this->objectDesc->is_open()) {
        this->objectDesc->close();
    }
    delete (this->objectDesc);

    if (this->monsterDesc->is_open()) {
        this->monsterDesc->close();
    }
    delete (this->monsterDesc);

    delete (this->savePath);
    delete (this->loadPath);
}

Settings* Settings::loadArguments(int argc, char* argv[]) {
    u_char index;
    for (index = 0; index < argc; index++) {
        if (strcmp(argv[index], "--save") == 0 || strcmp(argv[index], "-s") == 0) {
            // Make sure there is a save path specified
            if (index + 1 == argc || strstarts(argv[index + 1], "-")) {
                printf(SHELL_TEXT_RED "Using --save (-s-) requires a file as the target to save to \n");
                printf(SHELL_DEFAULT "\n");
                exit(1);
            }

        } else if (strcmp(argv[index], "--load") == 0 || strcmp(argv[index], "-l") == 0) {
            // Make sure there is a load path specified
            if (index + 1 == argc || strstarts(argv[index + 1], "-")) {
                printf(SHELL_TEXT_RED "Using --load (l) requires a file as the target to load from\n");
                printf(SHELL_DEFAULT "\n");
                exit(1);
            }

        } else if (strcmp(argv[index], "--file_version") == 0 || strcmp(argv[index], "-fv") == 0) {
            if (index + 1 == argc || strstarts(argv[index + 1], "-")) {
                printf("Using --file_version (-fv) requires a file version integer\n");
                printf(SHELL_TEXT_RED "Using --file_version (-fv) requires a file version integer");
                printf(SHELL_DEFAULT "\n");
                exit(1);
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
                printf(SHELL_TEXT_RED "Using --nummon (-nm) requires a number of monsters which must be greater than 0\n");
                printf(SHELL_DEFAULT "\n");
                exit(1);
            }

            this->canNumberOfMonsters = true;
            this->numberOfMonsters = (u_short) strtol(argv[index + 1], null, 0);
        } else if (strcmp(argv[index], "--expanded_print") == 0 || strcmp(argv[index], "-ep") == 0) {
            this->expandedPrint = true;
        } else if (strcmp(argv[index], "--no_ncurses") == 0 || strcmp(argv[index], "-nnc") == 0) {
            this->nCursesPrint = false;
        }
    }

    return this;
}

Settings* Settings::loadFiles() {
    struct stat buffer = {};

    std::string resPath;
    std::string homePath;

    // Attempt to load in monster descriptions
    resPath = std::string(SETTINGS_RES_FOLDER) + std::string(SETTINGS_MONSTER_DESC);
    homePath = std::string(getenv("HOME")) + std::string(SETTINGS_HOME_FOLDER) + std::string(SETTINGS_MONSTER_DESC);

    if (stat(resPath.c_str(), &buffer) == 0) {
        this->monsterDesc->open(resPath);
    } else if (stat(homePath.c_str(), &buffer) == 0) {
        this->monsterDesc->open(homePath);
    } else {
        printf(SHELL_TEXT_RED "Failed to open %s and %s\n", resPath.c_str(), homePath.c_str());
        printf(SHELL_DEFAULT "\n");
        exit(1);
    }

    // Attempt to load in object descriptions
    resPath = std::string(SETTINGS_RES_FOLDER) + std::string(SETTINGS_OBJECT_DESC);
    homePath = std::string(getenv("HOME")) + std::string(SETTINGS_HOME_FOLDER) + std::string(SETTINGS_OBJECT_DESC);

    if (stat(resPath.c_str(), &buffer) == 0) {
        this->objectDesc->open(resPath);
    } else if (stat(homePath.c_str(), &buffer) == 0) {
        this->objectDesc->open(homePath);
    } else {
        printf(SHELL_TEXT_RED "Failed to open %s and %s\n", resPath.c_str(), homePath.c_str());
        printf(SHELL_DEFAULT "\n");
        exit(1);
    }

    return this;
}
//
//std::string Settings::getFileName(const std::string& string) {
//    // Find the last slash location
//    size_t index = string.rfind(PATH_SEPARATOR, string.length());
//    if (index != std::string::npos) {
//        // Return the stuff after the last slash
//        return string.substr(index + 1, string.length() - index);
//    }
//
//    // Nothing to split, we have the file name
//    return string;
//}
//
//bool Settings::fileExists(const std::string& fileString) {
//    struct stat item = {};
//    if (stat(fileString.c_str(), &item) != -1) {
//        // Item exists
//        if (item.st_mode & S_IFREG) {
//            // It's a file
//            return true;
//        }
//    }
//
//    return false;
//}
//
//int Settings::createFolder(const std::string& path) {
//    mkdir(path.c_str(), 0755);
//
//    return 0;
//}

/** GETTERS **/
bool Settings::doExpandPrint() {
    return this->expandedPrint;
}

bool Settings::doNCursesPrint() {
    return this->nCursesPrint;
}

bool Settings::doNumberOfMonsters() {
    return this->canNumberOfMonsters;
}

bool Settings::doSave() {
    return this->save;
}

bool Settings::doLoad() {
    return this->load;
}

bool Settings::doFogOfWar() {
    return this->fogOfWar;
}

u_char Settings::getFileVersion() {
    return this->fileVersion;
}

std::string* Settings::getSavePath() {
    return this->savePath;
}

std::string* Settings::getLoadPath() {
    return this->loadPath;
}

u_short Settings::getNumberOfMonsters() {
    return this->numberOfMonsters;
}

std::ifstream* Settings::getMonsterDesc() {
    return this->monsterDesc;
}

std::ifstream* Settings::getObjectDesc() {
    return this->objectDesc;
}
/** GETTERS **/

/** SETTERS **/
Settings* Settings::setExpandedPrint(bool expandedPrint) {
    this->expandedPrint = expandedPrint;

    return this;
}

Settings* Settings::setNCursesPrint(bool nCursesPrint) {
    this->nCursesPrint = nCursesPrint;

    return this;
}

Settings* Settings::setCanNumberOfMonsters(bool canNumberOfMonsters) {
    this->canNumberOfMonsters = canNumberOfMonsters;

    return this;
}

Settings* Settings::setSave(bool save) {
    this->save = save;

    return this;
}

Settings* Settings::setLoad(bool load) {
    this->load = load;

    return this;
}

Settings* Settings::setFogOfWar(bool fogOfWar) {
    this->fogOfWar = fogOfWar;

    return this;
}

Settings* Settings::setFileVersion(u_char fileVersion) {
    this->fileVersion = fileVersion;

    return this;
}

Settings* Settings::setSavePath(std::string& savePath) {
    this->savePath->assign(savePath);

    return this;
}

Settings* Settings::setLoadPath(std::string& loadPath) {
    this->loadPath->assign(loadPath);

    return this;
}

Settings* Settings::setNumberOfMonsters(u_short numberOfMonsters) {
    this->numberOfMonsters = numberOfMonsters;

    return this;
}

Settings* Settings::setMonsterDesc(std::string& path) {
    // Close previous stream
    this->monsterDesc->close();

    this->monsterDesc->open(path);
    if (!this->monsterDesc->is_open()) {
        throw Exception::FileFailedToOpen();
    }

    return this;
}

Settings* Settings::setObjectDesc(std::string& path) {
    // Close previous stream
    this->objectDesc->close();

    // If the new stream is closed, open it
    this->objectDesc->open(path);
    if (!objectDesc->is_open()) {
        throw Exception::FileFailedToOpen();
    }

    return this;
}
/** SETTERS **/