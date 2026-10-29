#pragma once

#include <string>
#include <iostream>
#include "VehicleType.hpp"
#include "Status.hpp"

namespace CarRentalSystem {

class Vehicle {
protected:
    std::string vehicleId;
    std::string licensePlate;
    std::string brand;
    std::string model;
    int year;
    double dailyRate;
    VehicleType type;
    VehicleStatus status;
    int kmDriven;

public:
    Vehicle(const std::string& id, const std::string& plate, const std::string& brand,
            const std::string& model, int year, double rate, VehicleType type)
        : vehicleId(id), licensePlate(plate), brand(brand), model(model),
          year(year), dailyRate(rate), type(type), 
          status(VehicleStatus::AVAILABLE), kmDriven(0) {}

    virtual ~Vehicle() = default;

    // Getters
    const std::string& getVehicleId() const { return vehicleId; }
    const std::string& getLicensePlate() const { return licensePlate; }
    const std::string& getBrand() const { return brand; }
    const std::string& getModel() const { return model; }
    int getYear() const { return year; }
    double getDailyRate() const { return dailyRate; }
    VehicleType getType() const { return type; }
    VehicleStatus getStatus() const { return status; }
    int getKmDriven() const { return kmDriven; }

    // Setters
    void setStatus(VehicleStatus s) { status = s; }
    void setDailyRate(double rate) { dailyRate = rate; }
    void addKmDriven(int km) { kmDriven += km; }

    // Virtual method for polymorphism
    virtual std::string getVehicleInfo() const {
        return brand + " " + model + " (" + std::to_string(year) + ") - " + 
               vehicleTypeToString(type) + " - $" + std::to_string(dailyRate) + "/day";
    }

    virtual void displayDetails() const {
        std::cout << "Vehicle ID: " << vehicleId << std::endl;
        std::cout << "License: " << licensePlate << std::endl;
        std::cout << "Brand: " << brand << " " << model << " (" << year << ")" << std::endl;
        std::cout << "Type: " << vehicleTypeToString(type) << std::endl;
        std::cout << "Daily Rate: $" << dailyRate << std::endl;
        std::cout << "Status: " << statusToString(status) << std::endl;
        std::cout << "KM Driven: " << kmDriven << std::endl;
    }
};

} // namespace CarRentalSystem
