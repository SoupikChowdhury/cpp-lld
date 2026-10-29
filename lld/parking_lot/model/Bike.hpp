#ifndef BIKE_HPP
#define BIKE_HPP

#include "Vehicle.hpp"

class Bike : public Vehicle {
public:
    explicit Bike(const std::string& licensePlate) : Vehicle(licensePlate) {}

    VehicleType getType() const override {
        return VehicleType::BIKE;
    }
};

#endif // BIKE_HPP
