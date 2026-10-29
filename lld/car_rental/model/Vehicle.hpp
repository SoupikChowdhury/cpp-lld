#ifndef VEHICLE_HPP
#define VEHICLE_HPP

#include <string>
#include <atomic>
#include "../enums/VehicleType.hpp"

class Vehicle {
protected:
    std::string vehicleId;
    std::string brand;
    std::string model;
    std::string registrationNumber;
    double pricePerDay;
    std::atomic<bool> available;
    VehicleType type;

public:
    Vehicle(const std::string& vehicleId, const std::string& brand, 
            const std::string& model, const std::string& registrationNumber,
            double pricePerDay, VehicleType type)
        : vehicleId(vehicleId), brand(brand), model(model),
          registrationNumber(registrationNumber), pricePerDay(pricePerDay),
          available(true), type(type) {}

    virtual ~Vehicle() = default;

    // Getters
    std::string getVehicleId() const { return vehicleId; }
    std::string getBrand() const { return brand; }
    std::string getModel() const { return model; }
    std::string getRegistrationNumber() const { return registrationNumber; }
    double getPricePerDay() const { return pricePerDay; }
    bool isAvailable() const { return available.load(); }
    VehicleType getType() const { return type; }

    // Atomic availability operations
    void setAvailable(bool value) { available.store(value); }
    
    bool tryReserve() {
        bool expected = true;
        return available.compare_exchange_strong(expected, false);
    }

    void release() { available.store(true); }

    virtual std::string getDescription() const {
        return brand + " " + model + " (" + registrationNumber + ")";
    }
};

#endif // VEHICLE_HPP
