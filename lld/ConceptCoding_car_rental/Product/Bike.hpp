#pragma once

#include "Vehicle.hpp"

namespace CarRentalSystem {

enum class BikeType {
    STANDARD,
    SPORTS,
    CRUISER,
    SCOOTER
};

inline const char* bikeTypeToString(BikeType type) {
    switch (type) {
        case BikeType::STANDARD: return "STANDARD";
        case BikeType::SPORTS: return "SPORTS";
        case BikeType::CRUISER: return "CRUISER";
        case BikeType::SCOOTER: return "SCOOTER";
        default: return "UNKNOWN";
    }
}

class Bike : public Vehicle {
private:
    BikeType bikeType;
    int engineCC;
    bool hasHelmet;

public:
    Bike(const std::string& id, const std::string& plate, const std::string& brand,
         const std::string& model, int year, double rate, BikeType bikeType,
         int cc = 150, bool helmet = true)
        : Vehicle(id, plate, brand, model, year, rate, VehicleType::BIKE),
          bikeType(bikeType), engineCC(cc), hasHelmet(helmet) {}

    BikeType getBikeType() const { return bikeType; }
    int getEngineCC() const { return engineCC; }
    bool getHasHelmet() const { return hasHelmet; }

    std::string getVehicleInfo() const override {
        return Vehicle::getVehicleInfo() + " [" + bikeTypeToString(bikeType) + 
               ", " + std::to_string(engineCC) + "cc]";
    }

    void displayDetails() const override {
        Vehicle::displayDetails();
        std::cout << "Bike Type: " << bikeTypeToString(bikeType) << std::endl;
        std::cout << "Engine: " << engineCC << "cc" << std::endl;
        std::cout << "Helmet Included: " << (hasHelmet ? "Yes" : "No") << std::endl;
    }
};

} // namespace CarRentalSystem
