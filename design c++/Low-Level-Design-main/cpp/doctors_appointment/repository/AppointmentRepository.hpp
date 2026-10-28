#ifndef APPOINTMENT_REPOSITORY_HPP
#define APPOINTMENT_REPOSITORY_HPP

#include <unordered_map>
#include <vector>
#include <mutex>
#include <sstream>
#include <iomanip>
#include "../model/Appointment.hpp"

class AppointmentRepository {
private:
    std::unordered_map<std::string, Appointment*> appointments;
    std::unordered_map<std::string, std::vector<Appointment*>> doctorAppointments;
    mutable std::mutex mutex_;
    int appointmentCounter = 0;

    std::string generateAppointmentId() {
        std::stringstream ss;
        ss << "APT" << std::setfill('0') << std::setw(6) << (++appointmentCounter);
        return ss.str();
    }

public:
    ~AppointmentRepository() {
        for (auto& [id, appointment] : appointments) {
            delete appointment;
        }
    }

    Appointment* createAppointment(Doctor* doctor, const TimeSlot& slot) {
        std::lock_guard<std::mutex> lock(mutex_);
        std::string appointmentId = generateAppointmentId();
        Appointment* appointment = new Appointment(appointmentId, doctor, slot);
        appointments[appointmentId] = appointment;
        doctorAppointments[doctor->getDoctorId()].push_back(appointment);
        return appointment;
    }

    Appointment* getAppointment(const std::string& appointmentId) {
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = appointments.find(appointmentId);
        return (it != appointments.end()) ? it->second : nullptr;
    }

    std::vector<Appointment*> getAppointmentsForDoctor(const std::string& doctorId) const {
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = doctorAppointments.find(doctorId);
        if (it != doctorAppointments.end()) {
            return it->second;
        }
        return {};
    }

    std::vector<Appointment*> getAvailableSlots(const std::string& doctorId) const {
        std::lock_guard<std::mutex> lock(mutex_);
        std::vector<Appointment*> available;
        auto it = doctorAppointments.find(doctorId);
        if (it != doctorAppointments.end()) {
            for (Appointment* apt : it->second) {
                if (apt->isAvailable()) {
                    available.push_back(apt);
                }
            }
        }
        return available;
    }

    std::vector<Appointment*> getAllAvailableSlots() const {
        std::lock_guard<std::mutex> lock(mutex_);
        std::vector<Appointment*> available;
        for (const auto& [id, apt] : appointments) {
            if (apt->isAvailable()) {
                available.push_back(apt);
            }
        }
        return available;
    }

    std::vector<Appointment*> getAllAppointments() const {
        std::lock_guard<std::mutex> lock(mutex_);
        std::vector<Appointment*> allApts;
        for (const auto& [id, apt] : appointments) {
            allApts.push_back(apt);
        }
        return allApts;
    }
};

#endif // APPOINTMENT_REPOSITORY_HPP
