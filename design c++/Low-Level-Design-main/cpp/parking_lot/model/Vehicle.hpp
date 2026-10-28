#ifndef VEHICLE_HPP
#define VEHICLE_HPP

#include <string>
#include "../enums/VehicleType.hpp"

class Vehicle {
protected:
    std::string licensePlate;

public:
    explicit Vehicle(const std::string& licensePlate) : licensePlate(licensePlate) {}
    virtual ~Vehicle() = default;

    std::string getLicensePlate() const { return licensePlate; }
    virtual VehicleType getType() const = 0;
};

#endif // VEHICLE_HPP
