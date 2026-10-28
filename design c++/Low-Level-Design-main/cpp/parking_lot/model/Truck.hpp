#ifndef TRUCK_HPP
#define TRUCK_HPP

#include "Vehicle.hpp"

class Truck : public Vehicle {
public:
    explicit Truck(const std::string& licensePlate) : Vehicle(licensePlate) {}

    VehicleType getType() const override {
        return VehicleType::TRUCK;
    }
};

#endif // TRUCK_HPP
