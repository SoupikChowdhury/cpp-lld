#pragma once

#include <iostream>
#include "Direction.hpp"

namespace ElevatorSystem {

class ElevatorDisplay {
private:
    int currentFloor;
    Direction direction;

public:
    ElevatorDisplay() : currentFloor(0), direction(Direction::NONE) {}

    void setDisplay(int floor, Direction dir) {
        currentFloor = floor;
        direction = dir;
    }

    void showDisplay() const {
        std::cout << "Display: Floor " << currentFloor 
                  << " | Direction: " << directionToString(direction) << std::endl;
    }

    int getCurrentFloor() const { return currentFloor; }
    Direction getDirection() const { return direction; }
};

} // namespace ElevatorSystem
