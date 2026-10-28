#pragma once

#include <string>
#include <memory>
#include <vector>
#include <unordered_map>
#include "Location.hpp"
#include "VehicleInventoryManagement.hpp"
#include "Reservation.hpp"

namespace CarRentalSystem {

class Store {
private:
    std::string storeId;
    std::string name;
    std::shared_ptr<Location> location;
    std::unique_ptr<VehicleInventoryManagement> inventory;
    std::unordered_map<std::string, std::shared_ptr<Reservation>> reservations;

public:
    Store(const std::string& id, const std::string& name, std::shared_ptr<Location> location)
        : storeId(id), name(name), location(location),
          inventory(std::make_unique<VehicleInventoryManagement>()) {}

    // Getters
    const std::string& getStoreId() const { return storeId; }
    const std::string& getName() const { return name; }
    std::shared_ptr<Location> getLocation() const { return location; }
    VehicleInventoryManagement* getInventory() { return inventory.get(); }

    // Vehicle management
    void addVehicle(std::shared_ptr<Vehicle> vehicle) {
        inventory->addVehicle(vehicle);
    }

    std::vector<std::shared_ptr<Vehicle>> getAvailableVehicles() {
        return inventory->getAvailableVehicles();
    }

    std::vector<std::shared_ptr<Vehicle>> getAvailableVehiclesByType(VehicleType type) {
        return inventory->getAvailableVehiclesByType(type);
    }

    // Reservation management
    std::shared_ptr<Reservation> createReservation(
        const std::string& reservationId,
        std::shared_ptr<User> user,
        std::shared_ptr<Vehicle> vehicle,
        std::shared_ptr<Location> dropoffLocation,
        time_t pickupTime,
        int duration,
        ReservationType type
    ) {
        auto reservation = std::make_shared<Reservation>(
            reservationId, user, vehicle, location, dropoffLocation,
            pickupTime, duration, type
        );
        reservations[reservationId] = reservation;
        return reservation;
    }

    std::shared_ptr<Reservation> getReservation(const std::string& reservationId) {
        auto it = reservations.find(reservationId);
        return (it != reservations.end()) ? it->second : nullptr;
    }

    std::vector<std::shared_ptr<Reservation>> getAllReservations() {
        std::vector<std::shared_ptr<Reservation>> result;
        for (const auto& [id, reservation] : reservations) {
            result.push_back(reservation);
        }
        return result;
    }

    void displayStoreInfo() const {
        std::cout << "\n=== Store Information ===" << std::endl;
        std::cout << "Store: " << name << " (ID: " << storeId << ")" << std::endl;
        std::cout << "Location: " << location->getFullAddress() << std::endl;
        std::cout << "Vehicles: " << inventory->getTotalVehicleCount() << " total, "
                  << inventory->getAvailableVehicleCount() << " available" << std::endl;
        std::cout << "Reservations: " << reservations.size() << std::endl;
    }
};

} // namespace CarRentalSystem
