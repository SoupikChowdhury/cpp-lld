#pragma once

#include "Vehicle.hpp"
#include <iostream>

namespace NullObjectPattern {

/**
 * @brief Null Object implementation of Vehicle
 * 
 * This class represents a "null" or "missing" vehicle. Instead of returning
 * nullptr when a vehicle is not found, we return an instance of NullVehicle.
 * 
 * Benefits:
 * 1. No null pointer checks needed in client code
 * 2. Methods can be called safely without crashing
 * 3. Provides sensible default/neutral behavior
 * 4. Follows "Tell, Don't Ask" principle
 */
class NullVehicle : public Vehicle {
public:
    NullVehicle() = default;

    std::string getType() const override {
        return "No Vehicle";
    }

    int getNumberOfWheels() const override {
        return 0;  // No wheels - neutral behavior
    }

    void start() const override {
        // Do nothing - silent operation
        // Could also log: "No vehicle to start"
        std::cout << "[NullVehicle] No vehicle available to start.\n";
    }

    void stop() const override {
        // Do nothing - silent operation
        std::cout << "[NullVehicle] No vehicle available to stop.\n";
    }

    double getFuelCapacity() const override {
        return 0.0;  // No fuel capacity
    }

    int getSeatingCapacity() const override {
        return 0;  // No seating
    }

    bool isNull() const override {
        return true;  // This IS the null object
    }

    std::string toString() const override {
        return "NullVehicle: No vehicle available";
    }
};

/**
 * @brief Singleton Null Vehicle for efficiency
 * 
 * Since NullVehicle is stateless, we can share a single instance
 */
class NullVehicleSingleton {
public:
    static std::shared_ptr<Vehicle> getInstance() {
        static std::shared_ptr<Vehicle> instance = std::make_shared<NullVehicle>();
        return instance;
    }
    
private:
    NullVehicleSingleton() = default;
};

} // namespace NullObjectPattern
