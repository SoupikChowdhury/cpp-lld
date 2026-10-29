#pragma once

#include <string>

namespace CarRentalSystem {

enum class VehicleType {
    CAR,
    BIKE,
    SUV,
    VAN,
    TRUCK
};

inline const char* vehicleTypeToString(VehicleType type) {
    switch (type) {
        case VehicleType::CAR: return "CAR";
        case VehicleType::BIKE: return "BIKE";
        case VehicleType::SUV: return "SUV";
        case VehicleType::VAN: return "VAN";
        case VehicleType::TRUCK: return "TRUCK";
        default: return "UNKNOWN";
    }
}

} // namespace CarRentalSystem
