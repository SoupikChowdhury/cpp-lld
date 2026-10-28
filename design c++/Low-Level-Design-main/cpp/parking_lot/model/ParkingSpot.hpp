#ifndef PARKING_SPOT_HPP
#define PARKING_SPOT_HPP

#include <string>
#include <atomic>
#include "../enums/VehicleType.hpp"

class ParkingSpot {
private:
    std::string id;
    VehicleType allowedType;
    std::atomic<bool> occupied{false};

public:
    ParkingSpot(const std::string& id, VehicleType allowedType)
        : id(id), allowedType(allowedType) {}

    std::string getId() const { return id; }
    VehicleType getAllowedType() const { return allowedType; }
    bool isOccupied() const { return occupied.load(); }

    // Atomic operation for thread-safe occupation
    bool tryOccupy() {
        bool expected = false;
        return occupied.compare_exchange_strong(expected, true);
    }

    void release() {
        occupied.store(false);
    }
};

#endif // PARKING_SPOT_HPP
