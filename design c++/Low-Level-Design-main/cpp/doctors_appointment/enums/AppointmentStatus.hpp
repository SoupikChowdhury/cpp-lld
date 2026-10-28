#ifndef APPOINTMENT_STATUS_HPP
#define APPOINTMENT_STATUS_HPP

#include <string>

enum class AppointmentStatus {
    AVAILABLE,
    BOOKED,
    CANCELLED,
    COMPLETED
};

inline std::string appointmentStatusToString(AppointmentStatus status) {
    switch (status) {
        case AppointmentStatus::AVAILABLE: return "AVAILABLE";
        case AppointmentStatus::BOOKED: return "BOOKED";
        case AppointmentStatus::CANCELLED: return "CANCELLED";
        case AppointmentStatus::COMPLETED: return "COMPLETED";
        default: return "UNKNOWN";
    }
}

#endif // APPOINTMENT_STATUS_HPP
