#pragma once

#include "Vehicle.hpp"

namespace CarRentalSystem {

enum class CarType {
    SEDAN,
    HATCHBACK,
    COUPE,
    CONVERTIBLE,
    SPORTS
};

inline const char* carTypeToString(CarType type) {
    switch (type) {
        case CarType::SEDAN: return "SEDAN";
        case CarType::HATCHBACK: return "HATCHBACK";
        case CarType::COUPE: return "COUPE";
        case CarType::CONVERTIBLE: return "CONVERTIBLE";
        case CarType::SPORTS: return "SPORTS";
        default: return "UNKNOWN";
    }
}

class Car : public Vehicle {
private:
    CarType carType;
    int numSeats;
    bool hasAC;
    std::string fuelType;

public:
    Car(const std::string& id, const std::string& plate, const std::string& brand,
        const std::string& model, int year, double rate, CarType carType,
        int seats = 5, bool ac = true, const std::string& fuel = "Petrol")
        : Vehicle(id, plate, brand, model, year, rate, VehicleType::CAR),
          carType(carType), numSeats(seats), hasAC(ac), fuelType(fuel) {}

    CarType getCarType() const { return carType; }
    int getNumSeats() const { return numSeats; }
    bool getHasAC() const { return hasAC; }
    const std::string& getFuelType() const { return fuelType; }

    std::string getVehicleInfo() const override {
        return Vehicle::getVehicleInfo() + " [" + carTypeToString(carType) + 
               ", " + std::to_string(numSeats) + " seats]";
    }

    void displayDetails() const override {
        Vehicle::displayDetails();
        std::cout << "Car Type: " << carTypeToString(carType) << std::endl;
        std::cout << "Seats: " << numSeats << std::endl;
        std::cout << "AC: " << (hasAC ? "Yes" : "No") << std::endl;
        std::cout << "Fuel: " << fuelType << std::endl;
    }
};

} // namespace CarRentalSystem
