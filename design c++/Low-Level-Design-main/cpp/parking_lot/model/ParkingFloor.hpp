#ifndef PARKING_FLOOR_HPP
#define PARKING_FLOOR_HPP

#include <string>
#include <unordered_map>
#include <memory>
#include "ParkingSpot.hpp"
#include "../enums/VehicleType.hpp"

class ParkingFloor {
private:
    std::string id;
    std::unordered_map<std::string, std::shared_ptr<ParkingSpot>> spots;

public:
    explicit ParkingFloor(const std::string& id) : id(id) {}

    std::string getId() const { return id; }

    void addSpot(std::shared_ptr<ParkingSpot> spot) {
        spots[spot->getId()] = spot;
    }

    std::shared_ptr<ParkingSpot> findAvailableSpot(VehicleType vehicleType) {
        for (auto& [spotId, spot] : spots) {
            if (spot->getAllowedType() == vehicleType && spot->tryOccupy()) {
                return spot;
            }
        }
        return nullptr;
    }

    const std::unordered_map<std::string, std::shared_ptr<ParkingSpot>>& getSpots() const {
        return spots;
    }
};

#endif // PARKING_FLOOR_HPP
