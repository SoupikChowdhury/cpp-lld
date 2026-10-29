#pragma once

#include <vector>
#include <memory>
#include "Floor.hpp"
#include "ElevatorController.hpp"
#include "InternalDispatcher.hpp"
#include "ExternalDispatcher.hpp"

namespace ElevatorSystem {

class Building {
private:
    std::string name;
    std::vector<std::unique_ptr<Floor>> floors;
    std::vector<std::unique_ptr<ElevatorController>> elevatorControllers;
    int minFloor;
    int maxFloor;

public:
    Building(const std::string& name, int numFloors, int numElevators)
        : name(name), minFloor(0), maxFloor(numFloors - 1) {
        
        // Initialize floors
        for (int i = 0; i < numFloors; ++i) {
            floors.push_back(std::make_unique<Floor>(i));
        }

        // Initialize elevators and register with dispatchers
        for (int i = 0; i < numElevators; ++i) {
            auto controller = std::make_unique<ElevatorController>(i, minFloor, maxFloor);
            
            // Register with dispatchers
            InternalDispatcher::getInstance().addController(controller.get());
            ExternalDispatcher::getInstance().addController(controller.get());
            
            elevatorControllers.push_back(std::move(controller));
        }

        std::cout << "Building '" << name << "' created with " << numFloors 
                  << " floors and " << numElevators << " elevators." << std::endl;
    }

    ~Building() {
        // Cleanup dispatchers
        InternalDispatcher::getInstance().clearControllers();
        ExternalDispatcher::getInstance().clearControllers();
    }

    // External request from a floor
    void requestElevator(int floorNumber, Direction dir) {
        if (floorNumber >= minFloor && floorNumber <= maxFloor) {
            Floor* floor = floors[floorNumber].get();
            
            if (dir == Direction::UP) {
                floor->pressUpButton();
            } else {
                floor->pressDownButton();
            }

            ExternalDispatcher::getInstance().dispatchRequest(floorNumber, dir);
        }
    }

    // Internal request from inside elevator
    void requestFloor(int elevatorId, int floorNumber) {
        InternalDispatcher::getInstance().dispatchRequest(elevatorId, floorNumber);
    }

    // Process all elevator requests
    void processAllRequests() {
        for (auto& controller : elevatorControllers) {
            if (controller->hasRequests()) {
                controller->processRequests();
            }
        }
    }

    // Get elevator status
    void showAllElevatorStatus() const {
        std::cout << "\n=== " << name << " Elevator Status ===" << std::endl;
        for (const auto& controller : elevatorControllers) {
            controller->getElevatorCar()->showStatus();
        }
        std::cout << "================================\n" << std::endl;
    }

    Floor* getFloor(int number) {
        if (number >= minFloor && number <= maxFloor) {
            return floors[number].get();
        }
        return nullptr;
    }

    ElevatorController* getElevatorController(int id) {
        if (id >= 0 && id < static_cast<int>(elevatorControllers.size())) {
            return elevatorControllers[id].get();
        }
        return nullptr;
    }

    int getNumFloors() const { return static_cast<int>(floors.size()); }
    int getNumElevators() const { return static_cast<int>(elevatorControllers.size()); }
    const std::string& getName() const { return name; }
};

} // namespace ElevatorSystem
