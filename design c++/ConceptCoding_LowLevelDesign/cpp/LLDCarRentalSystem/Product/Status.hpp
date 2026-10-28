#pragma once

namespace CarRentalSystem {

enum class VehicleStatus {
    AVAILABLE,
    RENTED,
    UNDER_MAINTENANCE,
    RESERVED
};

inline const char* statusToString(VehicleStatus status) {
    switch (status) {
        case VehicleStatus::AVAILABLE: return "AVAILABLE";
        case VehicleStatus::RENTED: return "RENTED";
        case VehicleStatus::UNDER_MAINTENANCE: return "UNDER_MAINTENANCE";
        case VehicleStatus::RESERVED: return "RESERVED";
        default: return "UNKNOWN";
    }
}

} // namespace CarRentalSystem
