#pragma once

#include <string>
#include <ctime>
#include <memory>
#include <iostream>
#include "Product/Vehicle.hpp"
#include "User.hpp"
#include "Location.hpp"
#include "ReservationStatus.hpp"
#include "ReservationType.hpp"

namespace CarRentalSystem {

class Reservation {
private:
    std::string reservationId;
    std::shared_ptr<User> user;
    std::shared_ptr<Vehicle> vehicle;
    std::shared_ptr<Location> pickupLocation;
    std::shared_ptr<Location> dropoffLocation;
    time_t pickupTime;
    time_t dropoffTime;
    ReservationType reservationType;
    ReservationStatus status;
    time_t createdAt;
    int duration;  // In units based on reservation type

public:
    Reservation(const std::string& id, std::shared_ptr<User> user,
                std::shared_ptr<Vehicle> vehicle, std::shared_ptr<Location> pickup,
                std::shared_ptr<Location> dropoff, time_t pickupTime, int duration,
                ReservationType type)
        : reservationId(id), user(user), vehicle(vehicle),
          pickupLocation(pickup), dropoffLocation(dropoff),
          pickupTime(pickupTime), reservationType(type),
          status(ReservationStatus::PENDING), duration(duration) {
        
        createdAt = std::time(nullptr);
        calculateDropoffTime();
    }

    void calculateDropoffTime() {
        int seconds = 0;
        switch (reservationType) {
            case ReservationType::HOURLY:
                seconds = duration * 3600;
                break;
            case ReservationType::DAILY:
                seconds = duration * 86400;
                break;
            case ReservationType::WEEKLY:
                seconds = duration * 604800;
                break;
            case ReservationType::MONTHLY:
                seconds = duration * 2592000; // 30 days
                break;
        }
        dropoffTime = pickupTime + seconds;
    }

    // Getters
    const std::string& getReservationId() const { return reservationId; }
    std::shared_ptr<User> getUser() const { return user; }
    std::shared_ptr<Vehicle> getVehicle() const { return vehicle; }
    std::shared_ptr<Location> getPickupLocation() const { return pickupLocation; }
    std::shared_ptr<Location> getDropoffLocation() const { return dropoffLocation; }
    time_t getPickupTime() const { return pickupTime; }
    time_t getDropoffTime() const { return dropoffTime; }
    ReservationType getReservationType() const { return reservationType; }
    ReservationStatus getStatus() const { return status; }
    int getDuration() const { return duration; }

    // Status updates
    void confirm() { 
        status = ReservationStatus::CONFIRMED;
        vehicle->setStatus(VehicleStatus::RESERVED);
    }
    
    void activate() { 
        status = ReservationStatus::ACTIVE;
        vehicle->setStatus(VehicleStatus::RENTED);
    }
    
    void complete() { 
        status = ReservationStatus::COMPLETED;
        vehicle->setStatus(VehicleStatus::AVAILABLE);
    }
    
    void cancel() { 
        status = ReservationStatus::CANCELLED;
        vehicle->setStatus(VehicleStatus::AVAILABLE);
    }

    double calculateBaseAmount() const {
        double dailyRate = vehicle->getDailyRate();
        double multiplier = getTypeMultiplier(reservationType);
        return dailyRate * multiplier * duration;
    }

    void displayReservation() const {
        std::cout << "\n=== Reservation Details ===" << std::endl;
        std::cout << "Reservation ID: " << reservationId << std::endl;
        std::cout << "Customer: " << user->getName() << std::endl;
        std::cout << "Vehicle: " << vehicle->getVehicleInfo() << std::endl;
        std::cout << "Pickup: " << pickupLocation->getName() << std::endl;
        std::cout << "Dropoff: " << dropoffLocation->getName() << std::endl;
        std::cout << "Duration: " << duration << " " << reservationTypeToString(reservationType) << std::endl;
        std::cout << "Status: " << reservationStatusToString(status) << std::endl;
        std::cout << "Base Amount: $" << calculateBaseAmount() << std::endl;
    }
};

} // namespace CarRentalSystem
