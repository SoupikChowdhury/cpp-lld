#ifndef APPOINTMENT_HPP
#define APPOINTMENT_HPP

#include <string>
#include <atomic>
#include "Doctor.hpp"
#include "Patient.hpp"
#include "TimeSlot.hpp"
#include "../enums/AppointmentStatus.hpp"

class Appointment {
private:
    std::string appointmentId;
    Doctor* doctor;
    Patient* patient;
    TimeSlot slot;
    std::atomic<AppointmentStatus> status;

public:
    Appointment(const std::string& appointmentId, Doctor* doctor, const TimeSlot& slot)
        : appointmentId(appointmentId), doctor(doctor), patient(nullptr),
          slot(slot), status(AppointmentStatus::AVAILABLE) {}

    // Getters
    std::string getAppointmentId() const { return appointmentId; }
    Doctor* getDoctor() const { return doctor; }
    Patient* getPatient() const { return patient; }
    TimeSlot getSlot() const { return slot; }
    AppointmentStatus getStatus() const { return status.load(); }

    bool isAvailable() const { 
        return status.load() == AppointmentStatus::AVAILABLE; 
    }

    // Atomic booking
    bool tryBook(Patient* p) {
        AppointmentStatus expected = AppointmentStatus::AVAILABLE;
        if (status.compare_exchange_strong(expected, AppointmentStatus::BOOKED)) {
            patient = p;
            return true;
        }
        return false;
    }

    void cancel() {
        status.store(AppointmentStatus::CANCELLED);
        patient = nullptr;
    }

    void complete() {
        status.store(AppointmentStatus::COMPLETED);
    }

    void makeAvailable() {
        status.store(AppointmentStatus::AVAILABLE);
        patient = nullptr;
    }
};

#endif // APPOINTMENT_HPP
