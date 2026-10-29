#pragma once

#include <vector>
#include <memory>
#include <algorithm>
#include "ElevatorController.hpp"

namespace ElevatorSystem {

// Singleton Pattern for Internal Dispatcher
class InternalDispatcher {
private:
    std::vector<ElevatorController*> controllers;

    InternalDispatcher() = default;

public:
    // Delete copy constructor and assignment operator
    InternalDispatcher(const InternalDispatcher&) = delete;
    InternalDispatcher& operator=(const InternalDispatcher&) = delete;

    static InternalDispatcher& getInstance() {
        static InternalDispatcher instance;
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

    // Dispatch internal request to the specific elevator
    void dispatchRequest(int elevatorId, int floor) {
        for (auto* controller : controllers) {
            if (controller->getElevatorCar()->getId() == elevatorId) {
                controller->addInternalRequest(floor);
                std::cout << "Internal request dispatched to Elevator " << elevatorId 
                          << " for floor " << floor << std::endl;
                return;
            }
        }
        std::cout << "Elevator " << elevatorId << " not found!" << std::endl;
    }

    void clearControllers() {
        controllers.clear();
    }
};

} // namespace ElevatorSystem
