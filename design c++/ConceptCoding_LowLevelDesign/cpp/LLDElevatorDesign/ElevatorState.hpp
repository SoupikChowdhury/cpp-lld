#pragma once

namespace ElevatorSystem {

enum class ElevatorState {
    MOVING,
    IDLE,
    STOPPED
};

inline const char* stateToString(ElevatorState state) {
    switch (state) {
        case ElevatorState::MOVING: return "MOVING";
        case ElevatorState::IDLE: return "IDLE";
        case ElevatorState::STOPPED: return "STOPPED";
        default: return "UNKNOWN";
    }
}

} // namespace ElevatorSystem
