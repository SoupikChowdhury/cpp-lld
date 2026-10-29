#ifndef VEHICLE_FACTORY_HPP
#define VEHICLE_FACTORY_HPP

#include <memory>
#include "../model/Vehicle.hpp"
#include "../model/Car.hpp"
#include "../model/Bike.hpp"
#include "../model/Truck.hpp"
#include "../enums/VehicleType.hpp"

class VehicleFactory {
public:
    static std::shared_ptr<Vehicle> create(VehicleType type, const std::string& licensePlate) {
        switch (type) {
            case VehicleType::CAR:
                return std::make_shared<Car>(licensePlate);
            case VehicleType::BIKE:
                return std::make_shared<Bike>(licensePlate);
            case VehicleType::TRUCK:
                return std::make_shared<Truck>(licensePlate);
            default:
                return std::make_shared<Car>(licensePlate);
        }
    }
};

#endif // VEHICLE_FACTORY_HPP
