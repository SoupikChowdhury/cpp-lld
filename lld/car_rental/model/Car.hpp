#ifndef CAR_HPP
#define CAR_HPP

#include "Vehicle.hpp"

class Car : public Vehicle {
public:
    Car(const std::string& vehicleId, const std::string& brand,
        const std::string& model, const std::string& registrationNumber,
        double pricePerDay)
        : Vehicle(vehicleId, brand, model, registrationNumber, pricePerDay, VehicleType::CAR) {}

    std::string getDescription() const override {
        return "[CAR] " + Vehicle::getDescription();
    }
};

#endif // CAR_HPP
