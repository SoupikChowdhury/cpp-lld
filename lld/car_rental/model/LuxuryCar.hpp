#ifndef LUXURY_CAR_HPP
#define LUXURY_CAR_HPP

#include "Vehicle.hpp"

class LuxuryCar : public Vehicle {
public:
    LuxuryCar(const std::string& vehicleId, const std::string& brand,
              const std::string& model, const std::string& registrationNumber,
              double pricePerDay)
        : Vehicle(vehicleId, brand, model, registrationNumber, pricePerDay, VehicleType::LUXURY) {}

    std::string getDescription() const override {
        return "[LUXURY] " + Vehicle::getDescription();
    }
};

#endif // LUXURY_CAR_HPP
