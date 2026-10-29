/*
 * LLD Elevator Design System
 * 
 * This implementation demonstrates a complete elevator system with:
 * - Multiple elevators in a building
 * - External dispatching (from floor buttons)
 * - Internal dispatching (from inside elevator)
 * - LOOK algorithm for optimal elevator selection
 * 
 * Design Patterns Used:
 * - Singleton: InternalDispatcher, ExternalDispatcher
 * - Strategy: Elevator selection algorithm
 * - State: Elevator states (IDLE, MOVING, STOPPED)
 */

#include <iostream>
#include <thread>
#include <chrono>
#include "Building.hpp"

using namespace ElevatorSystem;

void simulateBasicUsage() {
    std::cout << "\n========================================" << std::endl;
    std::cout << "  ELEVATOR SYSTEM SIMULATION" << std::endl;
    std::cout << "========================================\n" << std::endl;

    // Create a building with 10 floors and 3 elevators
    Building building("Tech Tower", 10, 3);
    
    // Show initial status
    building.showAllElevatorStatus();

    // Simulate: Person on floor 5 wants to go UP
    std::cout << "--- Scenario 1: Person on floor 5 wants to go UP ---" << std::endl;
    building.requestElevator(5, Direction::UP);
    
    // Simulate: Person on floor 2 wants to go DOWN
    std::cout << "\n--- Scenario 2: Person on floor 2 wants to go DOWN ---" << std::endl;
    building.requestElevator(2, Direction::DOWN);
    
    // Simulate: Person on floor 8 wants to go DOWN
    std::cout << "\n--- Scenario 3: Person on floor 8 wants to go DOWN ---" << std::endl;
    building.requestElevator(8, Direction::DOWN);

    // Process all requests
    std::cout << "\n--- Processing all elevator requests ---" << std::endl;
    building.processAllRequests();

    // Show final status
    building.showAllElevatorStatus();
}

void simulateInternalRequests() {
    std::cout << "\n========================================" << std::endl;
    std::cout << "  INTERNAL REQUEST SIMULATION" << std::endl;
    std::cout << "========================================\n" << std::endl;

    Building building("Office Complex", 15, 2);
    
    building.showAllElevatorStatus();

    // Person enters elevator 0 and presses floor 7
    std::cout << "--- Person in Elevator 0 requests floor 7 ---" << std::endl;
    building.requestFloor(0, 7);

    // Person enters elevator 1 and presses floor 10
    std::cout << "\n--- Person in Elevator 1 requests floor 10 ---" << std::endl;
    building.requestFloor(1, 10);

    // External request: Floor 3 going UP
    std::cout << "\n--- External request: Floor 3 going UP ---" << std::endl;
    building.requestElevator(3, Direction::UP);

    // Process all requests
    std::cout << "\n--- Processing all elevator requests ---" << std::endl;
    building.processAllRequests();

    building.showAllElevatorStatus();
}

void demonstrateElevatorSelection() {
    std::cout << "\n========================================" << std::endl;
    std::cout << "  OPTIMAL ELEVATOR SELECTION DEMO" << std::endl;
    std::cout << "========================================\n" << std::endl;

    Building building("Smart Building", 20, 4);

    // Move elevators to different floors first
    auto* controller0 = building.getElevatorController(0);
    auto* controller1 = building.getElevatorController(1);
    auto* controller2 = building.getElevatorController(2);
    auto* controller3 = building.getElevatorController(3);

    // Manually set positions for demonstration
    controller0->getElevatorCar()->setCurrentFloor(0);
    controller1->getElevatorCar()->setCurrentFloor(5);
    controller2->getElevatorCar()->setCurrentFloor(10);
    controller3->getElevatorCar()->setCurrentFloor(15);

    building.showAllElevatorStatus();

    // Request from floor 6 should pick elevator at floor 5
    std::cout << "Request from floor 6 (UP) - Should pick Elevator 1 (at floor 5)" << std::endl;
    building.requestElevator(6, Direction::UP);

    // Request from floor 12 should pick elevator at floor 10
    std::cout << "\nRequest from floor 12 (DOWN) - Should pick Elevator 2 (at floor 10)" << std::endl;
    building.requestElevator(12, Direction::DOWN);

    std::cout << "\n--- Processing requests ---" << std::endl;
    building.processAllRequests();

    building.showAllElevatorStatus();
}

int main() {
    try {
        // Run simulations
        simulateBasicUsage();
        
        std::cout << "\n\n";
        simulateInternalRequests();
        
        std::cout << "\n\n";
        demonstrateElevatorSelection();

        std::cout << "\n========================================" << std::endl;
        std::cout << "  ALL SIMULATIONS COMPLETED!" << std::endl;
        std::cout << "========================================" << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
