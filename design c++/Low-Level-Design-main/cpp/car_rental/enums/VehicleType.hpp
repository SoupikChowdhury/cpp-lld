#ifndef VEHICLE_TYPE_HPP
#define VEHICLE_TYPE_HPP

#include <string>

enum class VehicleType {
    CAR,
    BIKE,
    SUV,
    LUXURY
};

inline std::string vehicleTypeToString(VehicleType type) {
    switch (type) {
        case VehicleType::CAR: return "CAR";
        case VehicleType::BIKE: return "BIKE";
        case VehicleType::SUV: return "SUV";
        case VehicleType::LUXURY: return "LUXURY";
        default: return "UNKNOWN";
    }
}

#endif // VEHICLE_TYPE_HPP
