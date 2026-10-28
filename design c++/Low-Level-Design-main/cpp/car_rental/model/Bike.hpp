#ifndef BIKE_HPP
#define BIKE_HPP

#include "Vehicle.hpp"

class Bike : public Vehicle {
public:
    Bike(const std::string& vehicleId, const std::string& brand,
         const std::string& model, const std::string& registrationNumber,
         double pricePerDay)
        : Vehicle(vehicleId, brand, model, registrationNumber, pricePerDay, VehicleType::BIKE) {}

    std::string getDescription() const override {
        return "[BIKE] " + Vehicle::getDescription();
    }
};

#endif // BIKE_HPP
