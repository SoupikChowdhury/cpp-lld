#pragma once

#include <vector>
#include <memory>
#include <algorithm>
#include <limits>
#include <cmath>
#include "ElevatorController.hpp"

namespace ElevatorSystem {

// Singleton Pattern for External Dispatcher
// Uses Strategy Pattern for elevator selection algorithm
class ExternalDispatcher {
private:
    std::vector<ElevatorController*> controllers;

    ExternalDispatcher() = default;

    // Find optimal elevator using LOOK algorithm
    ElevatorController* findOptimalElevator(int floor, Direction dir) {
        ElevatorController* bestController = nullptr;
        int minDistance = std::numeric_limits<int>::max();

        for (auto* controller : controllers) {
            auto* car = controller->getElevatorCar();
            int currentFloor = car->getCurrentFloor();
            int distance = std::abs(currentFloor - floor);

            // Prefer idle elevators
            if (car->getState() == ElevatorState::IDLE) {
                if (distance < minDistance) {
                    minDistance = distance;
                    bestController = controller;
                }
            }
            // Or elevators moving towards the request
            else if (car->getDirection() == dir) {
                if (dir == Direction::UP && currentFloor <= floor) {
                    if (distance < minDistance) {
                        minDistance = distance;
                        bestController = controller;
                    }
                } else if (dir == Direction::DOWN && currentFloor >= floor) {
                    if (distance < minDistance) {
                        minDistance = distance;
                        bestController = controller;
                    }
                }
            }
        }

        // If no optimal found, just pick the closest
        if (!bestController && !controllers.empty()) {
            for (auto* controller : controllers) {
                int distance = std::abs(controller->getElevatorCar()->getCurrentFloor() - floor);
                if (distance < minDistance) {
                    minDistance = distance;
                    bestController = controller;
                }
            }
        }

        return bestController;
    }

public:
    // Delete copy constructor and assignment operator
    ExternalDispatcher(const ExternalDispatcher&) = delete;
    ExternalDispatcher& operator=(const ExternalDispatcher&) = delete;

    static ExternalDispatcher& getInstance() {
        static ExternalDispatcher instance;
        return instance;
    }

    void addController(ElevatorController* controller) {
        controllers.push_back(controller);
    }

    void removeController(ElevatorController* controller) {
        controllers.erase(
            std::remove(controllers.begin(), controllers.end(), controller),
            controllers.end()
        );
    }

    // Dispatch external request to optimal elevator
    void dispatchRequest(int floor, Direction dir) {
        ElevatorController* optimal = findOptimalElevator(floor, dir);
        if (optimal) {
            optimal->addRequest(floor, dir);
            std::cout << "External request from floor " << floor << " (" << directionToString(dir)
                      << ") dispatched to Elevator " << optimal->getElevatorCar()->getId() << std::endl;
        } else {
            std::cout << "No elevator available!" << std::endl;
        }
    }

    void clearControllers() {
        controllers.clear();
    }
};

} // namespace ElevatorSystem
