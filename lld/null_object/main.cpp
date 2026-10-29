#include <iostream>
#include <memory>
#include <vector>
#include "VehicleFactory.hpp"

using namespace NullObjectPattern;

/**
 * @brief Process a vehicle - demonstrates Null Object Pattern benefit
 * 
 * Notice: NO null checks needed! The code works regardless of whether
 * the vehicle is real or null.
 */
void processVehicle(std::shared_ptr<Vehicle> vehicle) {
    std::cout << "\n--- Processing Vehicle ---\n";
    std::cout << vehicle->toString() << "\n";
    std::cout << "Type: " << vehicle->getType() << "\n";
    std::cout << "Wheels: " << vehicle->getNumberOfWheels() << "\n";
    std::cout << "Fuel Capacity: " << vehicle->getFuelCapacity() << "L\n";
    
    vehicle->start();
    vehicle->stop();
    
    if (vehicle->isNull()) {
        std::cout << "Note: This was a null vehicle\n";
    }
}

/**
 * @brief Traditional approach with null checks - VERBOSE and ERROR-PRONE
 */
void processVehicleTraditional(Vehicle* vehicle) {
    std::cout << "\n--- Processing Vehicle (Traditional) ---\n";
    
    // Must check for null everywhere!
    if (vehicle != nullptr) {
        std::cout << vehicle->toString() << "\n";
        vehicle->start();
        vehicle->stop();
    } else {
        std::cout << "Error: Vehicle is null!\n";
        // Handle null case - duplicated logic
    }
}

/**
 * @brief Calculate total fuel capacity - works with real and null vehicles
 */
double calculateTotalFuelCapacity(const std::vector<std::shared_ptr<Vehicle>>& vehicles) {
    double total = 0.0;
    for (const auto& vehicle : vehicles) {
        // No null check needed - NullVehicle returns 0
        total += vehicle->getFuelCapacity();
    }
    return total;
}

int main() {
    std::cout << "╔═══════════════════════════════════════════════════════════╗\n";
    std::cout << "║           NULL OBJECT PATTERN DEMONSTRATION               ║\n";
    std::cout << "╚═══════════════════════════════════════════════════════════╝\n";

    VehicleFactory factory;

    // ==================== Create Valid Vehicles ====================
    std::cout << "\n=== Creating Valid Vehicles ===\n";
    
    auto car = factory.createVehicle("car");
    auto motorcycle = factory.createVehicle("motorcycle");
    auto truck = factory.createVehicle("truck");
    
    processVehicle(car);
    processVehicle(motorcycle);
    processVehicle(truck);

    // ==================== Create Invalid/Unknown Vehicle ====================
    std::cout << "\n=== Creating Unknown Vehicle Type ===\n";
    
    auto unknownVehicle = factory.createVehicle("spaceship");
    
    // Notice: No crash! No null check needed!
    processVehicle(unknownVehicle);

    // ==================== Demonstrate Collection Processing ====================
    std::cout << "\n=== Collection Processing ===\n";
    
    std::vector<std::shared_ptr<Vehicle>> fleet = {
        factory.createVehicle("car"),
        factory.createVehicle("car"),
        factory.createVehicle("invalid"),  // NullVehicle
        factory.createVehicle("truck"),
        factory.createVehicle("???"),      // NullVehicle
        factory.createVehicle("bike")
    };
    
    std::cout << "Fleet size: " << fleet.size() << "\n";
    std::cout << "Total fuel capacity: " << calculateTotalFuelCapacity(fleet) << "L\n";
    
    // Count real vs null vehicles
    int realCount = 0, nullCount = 0;
    for (const auto& v : fleet) {
        if (v->isNull()) nullCount++;
        else realCount++;
    }
    std::cout << "Real vehicles: " << realCount << ", Null vehicles: " << nullCount << "\n";

    // ==================== Cache Demonstration ====================
    std::cout << "\n=== Cache with Null Object ===\n";
    
    // Add some vehicles to cache
    factory.getOrCreate("V001", "car");
    factory.getOrCreate("V002", "truck");
    
    // Get existing vehicle
    auto cachedVehicle = factory.getVehicle("V001");
    std::cout << "Cached V001: " << cachedVehicle->getType() << "\n";
    
    // Get non-existing vehicle - returns NullVehicle, not nullptr!
    auto missingVehicle = factory.getVehicle("V999");
    std::cout << "Missing V999: " << missingVehicle->getType() << "\n";
    std::cout << "Is null? " << (missingVehicle->isNull() ? "Yes" : "No") << "\n";
    
    // Safe to call methods on missing vehicle!
    missingVehicle->start();

    // ==================== Comparison with Traditional Approach ====================
    std::cout << "\n=== Comparison: Null Object vs Traditional ===\n";
    
    std::cout << "\nWith Null Object Pattern:\n";
    std::cout << "------------------------\n";
    auto v1 = factory.getVehicle("nonexistent");
    v1->start();  // Safe! No crash!
    
    std::cout << "\nTraditional Approach (Dangerous):\n";
    std::cout << "---------------------------------\n";
    Vehicle* v2 = factory.getVehicleTraditional("nonexistent");
    if (v2 != nullptr) {
        v2->start();
    } else {
        std::cout << "Error: Must handle null pointer!\n";
    }

    // ==================== Custom Car Creation ====================
    std::cout << "\n=== Custom Car Creation ===\n";
    
    auto customCar = factory.createCar("Tesla", "Model S", 2025);
    processVehicle(customCar);

    std::cout << "\n╔═══════════════════════════════════════════════════════════╗\n";
    std::cout << "║           DEMONSTRATION COMPLETE                          ║\n";
    std::cout << "╚═══════════════════════════════════════════════════════════╝\n";

    std::cout << R"(
Key Takeaways:
1. NullVehicle provides sensible defaults (0 wheels, 0 fuel, etc.)
2. Methods can be called without null checks
3. Collections can be processed uniformly
4. Code is cleaner and less error-prone
5. Follows "Tell, Don't Ask" principle
    )";

    return 0;
}
