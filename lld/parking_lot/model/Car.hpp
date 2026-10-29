#ifndef CAR_HPP
#define CAR_HPP

#include "Vehicle.hpp"

class Car : public Vehicle {
public:
    explicit Car(const std::string& licensePlate) : Vehicle(licensePlate) {}

    VehicleType getType() const override {
        return VehicleType::CAR;
    }
};

#endif // CAR_HPP
