#ifndef BRANCH_HPP
#define BRANCH_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <mutex>
#include <algorithm>
#include "Vehicle.hpp"
#include "../enums/VehicleType.hpp"

class Branch {
private:
    std::string branchId;
    std::string name;
    std::string address;
    std::unordered_map<std::string, Vehicle*> vehicles;
    mutable std::mutex mutex_;

public:
    Branch(const std::string& branchId, const std::string& name, 
           const std::string& address)
        : branchId(branchId), name(name), address(address) {}

    // Getters
    std::string getBranchId() const { return branchId; }
    std::string getName() const { return name; }
    std::string getAddress() const { return address; }

    void addVehicle(Vehicle* vehicle) {
        std::lock_guard<std::mutex> lock(mutex_);
        vehicles[vehicle->getVehicleId()] = vehicle;
    }

    void removeVehicle(const std::string& vehicleId) {
        std::lock_guard<std::mutex> lock(mutex_);
        vehicles.erase(vehicleId);
    }

    Vehicle* getVehicle(const std::string& vehicleId) {
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = vehicles.find(vehicleId);
        return (it != vehicles.end()) ? it->second : nullptr;
    }

    std::vector<Vehicle*> getAvailableVehicles() const {
        std::lock_guard<std::mutex> lock(mutex_);
        std::vector<Vehicle*> available;
        for (const auto& [id, vehicle] : vehicles) {
            if (vehicle->isAvailable()) {
                available.push_back(vehicle);
            }
        }
        return available;
    }

    std::vector<Vehicle*> getAvailableVehicles(VehicleType type) const {
        std::lock_guard<std::mutex> lock(mutex_);
        std::vector<Vehicle*> available;
        for (const auto& [id, vehicle] : vehicles) {
            if (vehicle->isAvailable() && vehicle->getType() == type) {
                available.push_back(vehicle);
            }
        }
        return available;
    }

    std::vector<Vehicle*> getAllVehicles() const {
        std::lock_guard<std::mutex> lock(mutex_);
        std::vector<Vehicle*> allVehicles;
        for (const auto& [id, vehicle] : vehicles) {
            allVehicles.push_back(vehicle);
        }
        return allVehicles;
    }

    int getVehicleCount() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return static_cast<int>(vehicles.size());
    }
};

#endif // BRANCH_HPP
