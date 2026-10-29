#pragma once

namespace CarRentalSystem {

enum class ReservationStatus {
    PENDING,
    CONFIRMED,
    ACTIVE,
    COMPLETED,
    CANCELLED
};

inline const char* reservationStatusToString(ReservationStatus status) {
    switch (status) {
        case ReservationStatus::PENDING: return "PENDING";
        case ReservationStatus::CONFIRMED: return "CONFIRMED";
        case ReservationStatus::ACTIVE: return "ACTIVE";
        case ReservationStatus::COMPLETED: return "COMPLETED";
        case ReservationStatus::CANCELLED: return "CANCELLED";
        default: return "UNKNOWN";
    }
}

} // namespace CarRentalSystem
