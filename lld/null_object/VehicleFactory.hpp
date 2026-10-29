#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <algorithm>
#include "Vehicle.hpp"
#include "Car.hpp"
#include "NullVehicle.hpp"

namespace NullObjectPattern {

/**
 * @brief Factory class for creating Vehicle objects
 * 
 * Demonstrates Null Object Pattern:
 * - Instead of returning nullptr for unknown vehicle types,
 *   returns a NullVehicle instance
 * - Client code doesn't need null checks
 */
class VehicleFactory {
private:
    // Cache of created vehicles (optional, for demo)
    std::unordered_map<std::string, std::shared_ptr<Vehicle>> vehicleCache_;

public:
    VehicleFactory() = default;

    /**
     * @brief Create a vehicle by type name
     * 
     * @param type The type of vehicle ("car", "motorcycle", "truck")
     * @return shared_ptr to Vehicle (never null!)
     * 
     * Key aspect of Null Object Pattern:
     * - Returns NullVehicle instead of nullptr for unknown types
     * - Guarantees non-null return value
     */
    std::shared_ptr<Vehicle> createVehicle(const std::string& type) {
        std::string lowerType = toLower(type);
        
        if (lowerType == "car") {
            return std::make_shared<Car>("Toyota", "Camry", 2024);
        }
        else if (lowerType == "motorcycle" || lowerType == "bike") {
            return std::make_shared<Motorcycle>("Honda", "CBR", 1000);
        }
        else if (lowerType == "truck") {
            return std::make_shared<Truck>("Volvo", 25.0);
        }
        else {
            // Instead of returning nullptr, return NullVehicle
            return NullVehicleSingleton::getInstance();
        }
    }

    /**
     * @brief Create a custom car
     */
    std::shared_ptr<Vehicle> createCar(const std::string& brand,
                                        const std::string& model,
                                        int year) {
        return std::make_shared<Car>(brand, model, year);
    }

    /**
     * @brief Get vehicle from cache or create new
     */
    std::shared_ptr<Vehicle> getOrCreate(const std::string& id,
                                          const std::string& type) {
        auto it = vehicleCache_.find(id);
        if (it != vehicleCache_.end()) {
            return it->second;
        }
        
        auto vehicle = createVehicle(type);
        if (!vehicle->isNull()) {
            vehicleCache_[id] = vehicle;
        }
        return vehicle;
    }

    /**
     * @brief Get vehicle by ID from cache
     * Returns NullVehicle if not found (not nullptr!)
     */
    std::shared_ptr<Vehicle> getVehicle(const std::string& id) {
        auto it = vehicleCache_.find(id);
        if (it != vehicleCache_.end()) {
            return it->second;
        }
        // Return Null Object instead of nullptr
        return NullVehicleSingleton::getInstance();
    }

    /**
     * @brief Traditional approach (AVOID) - returns nullptr
     * This is shown for comparison purposes only
     */
    Vehicle* getVehicleTraditional(const std::string& id) {
        auto it = vehicleCache_.find(id);
        if (it != vehicleCache_.end()) {
            return it->second.get();
        }
        return nullptr;  // BAD: Caller must check for null!
    }

private:
    std::string toLower(const std::string& str) {
        std::string result = str;
        std::transform(result.begin(), result.end(), result.begin(), ::tolower);
        return result;
    }
};

} // namespace NullObjectPattern
