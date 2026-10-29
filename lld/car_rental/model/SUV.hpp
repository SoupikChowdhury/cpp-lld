#ifndef SUV_HPP
#define SUV_HPP

#include "Vehicle.hpp"

class SUV : public Vehicle {
public:
    SUV(const std::string& vehicleId, const std::string& brand,
        const std::string& model, const std::string& registrationNumber,
        double pricePerDay)
        : Vehicle(vehicleId, brand, model, registrationNumber, pricePerDay, VehicleType::SUV) {}

    std::string getDescription() const override {
        return "[SUV] " + Vehicle::getDescription();
    }
};

#endif // SUV_HPP
