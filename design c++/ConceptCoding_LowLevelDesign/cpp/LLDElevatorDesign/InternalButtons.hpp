#pragma once

#include <set>
#include <iostream>

namespace ElevatorSystem {

class InternalButtons {
private:
    std::set<int> pressedFloors;
    int minFloor;
    int maxFloor;

public:
    InternalButtons(int minFloor, int maxFloor) 
        : minFloor(minFloor), maxFloor(maxFloor) {}

    bool pressButton(int floor) {
        if (floor >= minFloor && floor <= maxFloor) {
            pressedFloors.insert(floor);
            std::cout << "Internal button pressed for floor: " << floor << std::endl;
            return true;
        }
        return false;
    }

    void clearButton(int floor) {
        pressedFloors.erase(floor);
    }

    const std::set<int>& getPressedFloors() const {
        return pressedFloors;
    }

    bool hasRequests() const {
        return !pressedFloors.empty();
    }
};

} // namespace ElevatorSystem
