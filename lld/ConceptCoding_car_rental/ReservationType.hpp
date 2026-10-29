#pragma once

namespace CarRentalSystem {

enum class ReservationType {
    HOURLY,
    DAILY,
    WEEKLY,
    MONTHLY
};

inline const char* reservationTypeToString(ReservationType type) {
    switch (type) {
        case ReservationType::HOURLY: return "HOURLY";
        case ReservationType::DAILY: return "DAILY";
        case ReservationType::WEEKLY: return "WEEKLY";
        case ReservationType::MONTHLY: return "MONTHLY";
        default: return "UNKNOWN";
    }
}

inline double getTypeMultiplier(ReservationType type) {
    switch (type) {
        case ReservationType::HOURLY: return 1.0 / 24.0;  // Per hour rate
        case ReservationType::DAILY: return 1.0;
        case ReservationType::WEEKLY: return 6.5;  // Discount for weekly
        case ReservationType::MONTHLY: return 25.0;  // Discount for monthly
        default: return 1.0;
    }
}

} // namespace CarRentalSystem
