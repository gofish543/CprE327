#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <exception>

namespace Exception {
    class DungeonTextOutOfBounds : public std::exception {
        const char* what() const noexcept override {
            return "Dungeon text index out of bounds exception";
        }
    };

    class DungeonDeletingCurrentFloor : public std::exception {
        const char* what() const noexcept override {
            return "Attempted to delete the current floor";
        }
    };

    class FloorOutOfBounds : public std::exception {
        const char* what() const noexcept override {
            return "Floor index out of bounds exception";
        }
    };

    class DiceStringInvalidParse : public std::exception {
        const char* what() const noexcept override {
            return "Unknown Dice string parsed";
        }
    };
}

#endif