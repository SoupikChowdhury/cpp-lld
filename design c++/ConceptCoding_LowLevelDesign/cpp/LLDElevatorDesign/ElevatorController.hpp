#pragma once

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
            Direction currentDir = elevatorCar->getDirection();
            int currentFloor = elevatorCar->getCurrentFloor();

            // Process based on current direction
            if (currentDir == Direction::UP || currentDir == Direction::NONE) {
                if (!upRequests.empty()) {
                    processUpRequests();
                } else if (!downRequests.empty()) {
                    processDownRequests();
                }
            } else {
                if (!downRequests.empty()) {
                    processDownRequests();
                } else if (!upRequests.empty()) {
                    processUpRequests();
                }
            }
        }
        elevatorCar->stop();
        elevatorCar->setState(ElevatorState::IDLE);
    }

private:
    void processUpRequests() {
        while (!upRequests.empty()) {
            int targetFloor = *upRequests.begin();
            moveToFloor(targetFloor, Direction::UP);
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
        while (!downRequests.empty()) {
            int targetFloor = *downRequests.rbegin();
            moveToFloor(targetFloor, Direction::DOWN);
            downRequests.erase(targetFloor);
            elevatorCar->getInternalButtons()->clearButton(targetFloor);
            
            // Simulate stop at floor
            elevatorCar->stop();
            elevatorCar->openDoor();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            elevatorCar->closeDoor();
        }
    }

    void moveToFloor(int targetFloor, Direction dir) {
        elevatorCar->setDirection(dir);
        elevatorCar->setState(ElevatorState::MOVING);

        int currentFloor = elevatorCar->getCurrentFloor();
        while (currentFloor != targetFloor) {
            if (dir == Direction::UP) {
                elevatorCar->moveUp();
            } else {
                elevatorCar->moveDown();
            }
            currentFloor = elevatorCar->getCurrentFloor();
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    }
};

} // namespace ElevatorSystem
