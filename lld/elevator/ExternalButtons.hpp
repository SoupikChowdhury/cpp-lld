#pragma once

#include <iostream>
#include "Direction.hpp"

namespace ElevatorSystem {

class ExternalButtons {
private:
    bool upButtonPressed;
    bool downButtonPressed;
    int floorNumber;

public:
    explicit ExternalButtons(int floor) 
        : upButtonPressed(false), downButtonPressed(false), floorNumber(floor) {}

    void pressUp() {
        upButtonPressed = true;
        std::cout << "External UP button pressed on floor " << floorNumber << std::endl;
    }

    void pressDown() {
        downButtonPressed = true;
        std::cout << "External DOWN button pressed on floor " << floorNumber << std::endl;
    }

    void resetUp() { upButtonPressed = false; }
    void resetDown() { downButtonPressed = false; }

    bool isUpPressed() const { return upButtonPressed; }
    bool isDownPressed() const { return downButtonPressed; }
    int getFloorNumber() const { return floorNumber; }
};

} // namespace ElevatorSystem
