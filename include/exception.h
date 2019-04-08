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

    class MonsterOutOfBounds : public std::exception {
        const char* what() const noexcept override {
            return "Monster index out of bounds exception";
        }
    };

    class StaircaseOutOfBounds : public std::exception {
        const char* what() const noexcept override {
            return "Staircase index out of bounds exception";
        }
    };

    class RoomOutOfBounds : public std::exception {
        const char* what() const noexcept override {
            return "Room index out of bounds exception";
        }
    };

    class RoomEmptySpaceInvalid : public std::exception {
        const char* what() const noexcept override {
            return "Cannot accurately place rooms with the given settings, more than 60% of the dungeon would be rooms";
        }
    };

    class DijkstraKeyInvalid : public std::exception {
        const char* what() const noexcept override {
            return "Invalid dijkstra key passed";
        }
    };

    class DiceStringInvalidParse : public std::exception {
        const char* what() const noexcept override {
            return "Unknown Dice string parsed";
        }
    };

    class FileFailedToOpen : public std::exception {
        const char* what() const noexcept override {
            return "Failed to open file";
        }
    };
}

#endif