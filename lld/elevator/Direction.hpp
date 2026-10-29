#pragma once

namespace ElevatorSystem {

enum class Direction {
    UP,
    DOWN,
    NONE
};

inline const char* directionToString(Direction dir) {
    switch (dir) {
        case Direction::UP: return "UP";
        case Direction::DOWN: return "DOWN";
        case Direction::NONE: return "NONE";
        default: return "UNKNOWN";
    }
}

} // namespace ElevatorSystem
