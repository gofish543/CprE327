#include "dice.h"

Dice::Dice(int base, int rolls, int sides) {
    this->base = base;
    this->rolls = rolls;
    this->sides = sides;

    this->result = 0;
}

Dice::Dice(std::string diceString) {
    // String format is <base> + <rolls> d <sides>
    std::string firstDelimiter = "+";
    std::string secondDelimiter = "d";

    long unsigned int indexOfPlus = diceString.find(firstDelimiter);
    long unsigned int indexOfD = diceString.find(secondDelimiter);

    if (indexOfPlus == std::string::npos || indexOfD == std::string::npos) {
        throw Exception::DiceStringInvalidParse();
    }

    long int base = std::stoi(diceString.substr(0, indexOfPlus));
    long int rolls = std::stoi(diceString.substr(indexOfPlus + 1, indexOfD));
    long int sides = std::stoi(diceString.substr(indexOfD + 1));

    if (std::abs(base) > U_INT_MAX ||
        std::abs(rolls) > U_INT_MAX ||
        std::abs(sides) > U_INT_MAX) {
        throw Exception::DiceStringInvalidParse();
    }

    this->base = int(base);
    this->rolls = short(rolls);
    this->sides = short(sides);

    this->result = 0;
}

Dice* Dice::copy() {
    return new Dice(this->base, this->rolls, this->sides);
}

Dice::~Dice() = default;

int Dice::RandomNumberBetween(int min, int max) {
    static std::random_device randomDevice;
    static std::default_random_engine randomEngine(randomDevice());
    static bool seedSet = false;

    if (!seedSet) {
        struct timeval time = {};
        gettimeofday(&time, null);

        size_t buffer[1];
        u_int defaultSeed;
        int randomFile = open("/dev/random", O_RDWR);

        if (randomFile == 0 || read(randomFile, buffer, sizeof(size_t)) != sizeof(size_t)) {
            defaultSeed = Dice::Hash3(time.tv_sec, time.tv_usec, getpid());
        } else {
            defaultSeed = buffer[0];
        }

        randomEngine.seed(defaultSeed);

        seedSet = true;
    }

    return randomEngine() % ((max + 1) - min) + min;
}

u_int Dice::Hash3(u_int h1, u_int h2, u_int h3) {
    return (((h1 * 2654435789U) + h2) * 2654435789U) + h3;
}

int Dice::roll() {
    this->result = base;

    for (int index = 0; index < rolls; index++) {
        if (sides == 0) {
            continue;
        } else {
            this->result += Dice::RandomNumberBetween(1, sides);
        }
    }

    return this->result;
}

std::string Dice::toString() {
    return std::to_string(this->base) + "+" + std::to_string(this->rolls) + "d" + std::to_string(this->sides);
}

/** GETTERS **/
int Dice::getBase() {
    return this->base;
}

int Dice::getRolls() {
    return this->rolls;
}

int Dice::getSides() {
    return this->sides;
}

int Dice::getResult() {
    return this->result;
}

/** SETTERS **/
Dice* Dice::setBase(int base) {
    this->base = base;

    return this;
}

Dice* Dice::setRolls(int rolls) {
    this->rolls = rolls;

    return this;
}

Dice* Dice::setSides(int sides) {
    this->sides = sides;

    return this;
}
Dice* Dice::setResult(int result) {
    this->result = result;

    return this;
}
/** SETTERS **/