#pragma once

#include <iterator>
#include <memory>
#include <queue>
#include <set>
#include <thread>
#include <chrono>
#include "ElevatorCar.hpp"

namespace ElevatorSystem {

class ElevatorController {
private:
    std::unique_ptr<ElevatorCar> elevatorCar;
    std::set<int> upRequests;
    std::set<int> downRequests;

public:
    ElevatorController(int id, int minFloor, int maxFloor)
        : elevatorCar(std::make_unique<ElevatorCar>(id, minFloor, maxFloor)) {}

    ElevatorCar* getElevatorCar() { return elevatorCar.get(); }

    void addRequest(int floor, Direction dir) {
        if (dir == Direction::UP) {
            upRequests.insert(floor);
        } else if (dir == Direction::DOWN) {
            downRequests.insert(floor);
        }
        std::cout << "Request added: Floor " << floor << ", Direction: " << directionToString(dir) << std::endl;
    }

    void addInternalRequest(int floor) {
        int currentFloor = elevatorCar->getCurrentFloor();
        if (floor > currentFloor) {
            upRequests.insert(floor);
        } else if (floor < currentFloor) {
            downRequests.insert(floor);
        }
        elevatorCar->pressInternalButton(floor);
    }

    bool hasRequests() const {
        return !upRequests.empty() || !downRequests.empty();
    }

    void processRequests() {
        while (hasRequests()) {
            int currentFloor = elevatorCar->getCurrentFloor();

            // LOOK: sweep up serving UP requests ahead, then down serving DOWN requests ahead
            if (hasUpRequestAtOrAbove(currentFloor)) {
                processUpRequests();
            } else if (hasDownRequestAtOrBelow(currentFloor)) {
                processDownRequests();
            } else if (!upRequests.empty()) {
                // Only UP requests below us: go down to the lowest one, then sweep up
                moveToFloor(*upRequests.begin());
            } else {
                // Only DOWN requests above us: go up to the highest one, then sweep down
                moveToFloor(*downRequests.rbegin());
            }
        }
        elevatorCar->stop();
        elevatorCar->setState(ElevatorState::IDLE);
    }

private:
    bool hasUpRequestAtOrAbove(int floor) const {
        return upRequests.lower_bound(floor) != upRequests.end();
    }

    bool hasDownRequestAtOrBelow(int floor) const {
        return downRequests.upper_bound(floor) != downRequests.begin();
    }

    void processUpRequests() {
        // Serve UP requests at or above the current floor, lowest first
        while (hasUpRequestAtOrAbove(elevatorCar->getCurrentFloor())) {
            int targetFloor = *upRequests.lower_bound(elevatorCar->getCurrentFloor());
            moveToFloor(targetFloor);
            upRequests.erase(targetFloor);
            elevatorCar->getInternalButtons()->clearButton(targetFloor);
            
            // Simulate stop at floor
            elevatorCar->stop();
            elevatorCar->openDoor();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            elevatorCar->closeDoor();
        }
    }

    void processDownRequests() {
        // Serve DOWN requests at or below the current floor, highest first
        while (hasDownRequestAtOrBelow(elevatorCar->getCurrentFloor())) {
            int targetFloor = *std::prev(downRequests.upper_bound(elevatorCar->getCurrentFloor()));
            moveToFloor(targetFloor);
            downRequests.erase(targetFloor);
            elevatorCar->getInternalButtons()->clearButton(targetFloor);
            
            // Simulate stop at floor
            elevatorCar->stop();
            elevatorCar->openDoor();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            elevatorCar->closeDoor();
        }
    }

    void moveToFloor(int targetFloor) {
        // Step towards the target from wherever the car is
        while (elevatorCar->getCurrentFloor() < targetFloor) {
            elevatorCar->moveUp();
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
        while (elevatorCar->getCurrentFloor() > targetFloor) {
            elevatorCar->moveDown();
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    }
};

} // namespace ElevatorSystem
