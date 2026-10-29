#ifndef BOOKING_STATUS_HPP
#define BOOKING_STATUS_HPP

#include <string>

enum class BookingStatus {
    PENDING,
    CONFIRMED,
    CANCELLED,
    COMPLETED
};

inline std::string bookingStatusToString(BookingStatus status) {
    switch (status) {
        case BookingStatus::PENDING: return "PENDING";
        case BookingStatus::CONFIRMED: return "CONFIRMED";
        case BookingStatus::CANCELLED: return "CANCELLED";
        case BookingStatus::COMPLETED: return "COMPLETED";
        default: return "UNKNOWN";
    }
}

#endif // BOOKING_STATUS_HPP
