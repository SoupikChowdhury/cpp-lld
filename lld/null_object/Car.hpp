#pragma once

#include "Vehicle.hpp"
#include <iostream>

namespace NullObjectPattern {

/**
 * @brief Concrete implementation of Vehicle for a Car
 */
class Car : public Vehicle {
private:
    std::string brand_;
    std::string model_;
    int year_;
    double fuelCapacity_;
    int seatingCapacity_;

public:
    Car(const std::string& brand = "Generic",
        const std::string& model = "Sedan",
        int year = 2024,
        double fuelCapacity = 50.0,
        int seatingCapacity = 5)
        : brand_(brand)
        , model_(model)
        , year_(year)
        , fuelCapacity_(fuelCapacity)
        , seatingCapacity_(seatingCapacity) {}

    std::string getType() const override {
        return "Car";
    }

    int getNumberOfWheels() const override {
        return 4;
    }

    void start() const override {
        std::cout << "[" << brand_ << " " << model_ << "] Engine started! Vroom vroom!\n";
    }

    void stop() const override {
        std::cout << "[" << brand_ << " " << model_ << "] Engine stopped.\n";
    }

    double getFuelCapacity() const override {
        return fuelCapacity_;
    }

    int getSeatingCapacity() const override {
        return seatingCapacity_;
    }

    bool isNull() const override {
        return false;  // This is a real object
    }

    std::string toString() const override {
        return "Car: " + brand_ + " " + model_ + " (" + std::to_string(year_) + ")" +
               "\n  - Wheels: " + std::to_string(getNumberOfWheels()) +
               "\n  - Fuel Capacity: " + std::to_string(fuelCapacity_) + "L" +
               "\n  - Seating: " + std::to_string(seatingCapacity_) + " persons";
    }

    // Car-specific methods
    std::string getBrand() const { return brand_; }
    std::string getModel() const { return model_; }
    int getYear() const { return year_; }
};

/**
 * @brief Concrete implementation for a Motorcycle
 */
class Motorcycle : public Vehicle {
private:
    std::string brand_;
    std::string model_;
    int engineCC_;

public:
    Motorcycle(const std::string& brand = "Generic",
               const std::string& model = "Sport",
               int engineCC = 600)
        : brand_(brand)
        , model_(model)
        , engineCC_(engineCC) {}

    std::string getType() const override {
        return "Motorcycle";
    }

    int getNumberOfWheels() const override {
        return 2;
    }

    void start() const override {
        std::cout << "[" << brand_ << " " << model_ << "] Motorcycle revving up!\n";
    }

    void stop() const override {
        std::cout << "[" << brand_ << " " << model_ << "] Motorcycle engine off.\n";
    }

    double getFuelCapacity() const override {
        return 15.0;
    }

    int getSeatingCapacity() const override {
        return 2;
    }

    bool isNull() const override {
        return false;
    }

    std::string toString() const override {
        return "Motorcycle: " + brand_ + " " + model_ + " (" + std::to_string(engineCC_) + "cc)";
    }
};

/**
 * @brief Concrete implementation for a Truck
 */
class Truck : public Vehicle {
private:
    std::string brand_;
    double cargoCapacity_;  // in tons

public:
    Truck(const std::string& brand = "Generic",
          double cargoCapacity = 10.0)
        : brand_(brand)
        , cargoCapacity_(cargoCapacity) {}

    std::string getType() const override {
        return "Truck";
    }

    int getNumberOfWheels() const override {
        return 6;
    }

    void start() const override {
        std::cout << "[" << brand_ << " Truck] Heavy engine starting!\n";
    }

    void stop() const override {
        std::cout << "[" << brand_ << " Truck] Engine stopped.\n";
    }

    double getFuelCapacity() const override {
        return 200.0;
    }

    int getSeatingCapacity() const override {
        return 2;
    }

    bool isNull() const override {
        return false;
    }

    std::string toString() const override {
        return "Truck: " + brand_ + " (Cargo: " + std::to_string(cargoCapacity_) + " tons)";
    }
};

} // namespace NullObjectPattern
