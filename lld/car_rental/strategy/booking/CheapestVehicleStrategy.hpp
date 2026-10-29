#ifndef CHEAPEST_VEHICLE_STRATEGY_HPP
#define CHEAPEST_VEHICLE_STRATEGY_HPP

#include <algorithm>
#include "BookingStrategy.hpp"

class CheapestVehicleStrategy : public BookingStrategy {
public:
    Vehicle* selectVehicle(const std::vector<Vehicle*>& vehicles) override {
        if (vehicles.empty()) {
            return nullptr;
        }

        Vehicle* cheapest = vehicles[0];
        for (Vehicle* vehicle : vehicles) {
            if (vehicle->getPricePerDay() < cheapest->getPricePerDay()) {
                cheapest = vehicle;
            }
        }
        return cheapest;
    }

    std::string getStrategyName() const override {
        return "CheapestVehicleStrategy";
    }
};

#endif // CHEAPEST_VEHICLE_STRATEGY_HPP
