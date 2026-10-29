#pragma once

#include <iostream>

namespace ElevatorSystem {

enum class DoorState {
    OPEN,
    CLOSED
};

class ElevatorDoor {
private:
    DoorState state;

public:
    ElevatorDoor() : state(DoorState::CLOSED) {}

    void open() {
        std::cout << "Door opening..." << std::endl;
        state = DoorState::OPEN;
    }

    void close() {
        std::cout << "Door closing..." << std::endl;
        state = DoorState::CLOSED;
    }

    bool isOpen() const { return state == DoorState::OPEN; }
    bool isClosed() const { return state == DoorState::CLOSED; }

    DoorState getState() const { return state; }
};

} // namespace ElevatorSystem
