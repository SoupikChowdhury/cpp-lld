#pragma once

#include <vector>
#include <memory>
#include <unordered_map>
#include <algorithm>
#include "Product/Vehicle.hpp"
#include "Product/VehicleType.hpp"
#include "Product/Status.hpp"

namespace CarRentalSystem {

class VehicleInventoryManagement {
private:
    std::unordered_map<std::string, std::shared_ptr<Vehicle>> vehicles;
    std::unordered_map<VehicleType, std::vector<std::string>> vehiclesByType;

public:
    VehicleInventoryManagement() = default;

    void addVehicle(std::shared_ptr<Vehicle> vehicle) {
        vehicles[vehicle->getVehicleId()] = vehicle;
        vehiclesByType[vehicle->getType()].push_back(vehicle->getVehicleId());
        std::cout << "Vehicle added: " << vehicle->getVehicleInfo() << std::endl;
    }

    void removeVehicle(const std::string& vehicleId) {
        auto it = vehicles.find(vehicleId);
        if (it != vehicles.end()) {
            VehicleType type = it->second->getType();
            auto& typeVehicles = vehiclesByType[type];
            typeVehicles.erase(
                std::remove(typeVehicles.begin(), typeVehicles.end(), vehicleId),
                typeVehicles.end()
            );
            vehicles.erase(it);
        }
    }

    std::shared_ptr<Vehicle> getVehicle(const std::string& vehicleId) {
        auto it = vehicles.find(vehicleId);
        return (it != vehicles.end()) ? it->second : nullptr;
    }

    std::vector<std::shared_ptr<Vehicle>> getAvailableVehicles() {
        std::vector<std::shared_ptr<Vehicle>> available;
        for (const auto& [id, vehicle] : vehicles) {
            if (vehicle->getStatus() == VehicleStatus::AVAILABLE) {
                available.push_back(vehicle);
            }
        }
        return available;
    }

    std::vector<std::shared_ptr<Vehicle>> getAvailableVehiclesByType(VehicleType type) {
        std::vector<std::shared_ptr<Vehicle>> available;
        auto it = vehiclesByType.find(type);
        if (it != vehiclesByType.end()) {
            for (const auto& id : it->second) {
                auto vehicle = vehicles[id];
                if (vehicle->getStatus() == VehicleStatus::AVAILABLE) {
                    available.push_back(vehicle);
                }
            }
        }
        return available;
    }

    bool updateVehicleStatus(const std::string& vehicleId, VehicleStatus status) {
        auto vehicle = getVehicle(vehicleId);
        if (vehicle) {
            vehicle->setStatus(status);
            return true;
        }
        return false;
    }

    int getTotalVehicleCount() const {
        return static_cast<int>(vehicles.size());
    }

    int getAvailableVehicleCount() const {
        int count = 0;
        for (const auto& [id, vehicle] : vehicles) {
            if (vehicle->getStatus() == VehicleStatus::AVAILABLE) {
                count++;
            }
        }
        return count;
    }

    void displayInventory() const {
        std::cout << "\n=== Vehicle Inventory ===" << std::endl;
        std::cout << "Total: " << getTotalVehicleCount() << std::endl;
        for (const auto& [id, vehicle] : vehicles) {
            std::cout << "- " << vehicle->getVehicleInfo() 
                      << " [" << statusToString(vehicle->getStatus()) << "]" << std::endl;
        }
    }
};

} // namespace CarRentalSystem
