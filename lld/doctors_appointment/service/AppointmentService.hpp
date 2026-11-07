#ifndef APPOINTMENT_SERVICE_HPP
#define APPOINTMENT_SERVICE_HPP

#include <algorithm>
#include <ctime>
#include <deque>
#include <memory>
#include <iostream>
#include <mutex>
#include "../repository/DoctorRepository.hpp"
#include "../repository/AppointmentRepository.hpp"
#include "../strategy/SlotRankingStrategy.hpp"
#include "../strategy/EarliestSlotStrategy.hpp"
#include "../model/WaitlistEntry.hpp"
#include "../model/Patient.hpp"

class AppointmentService {
private:
    DoctorRepository doctorRepo;
    AppointmentRepository appointmentRepo;
    std::unique_ptr<SlotRankingStrategy> rankingStrategy;

    // Waitlist per doctor
    std::unordered_map<std::string, std::deque<WaitlistEntry*>> waitlists;
    std::vector<WaitlistEntry*> allWaitlistEntries;  // For cleanup
    int waitlistCounter = 0;
    mutable std::mutex mutex_;  // Makes each use case one step, such as cancel + waitlist

    static bool hasStarted(const TimeSlot& slot) {
        return slot.getStartTime() <= time(nullptr);
    }

    static std::vector<Appointment*> upcoming(const std::vector<Appointment*>& slots) {
        std::vector<Appointment*> result;
        for (Appointment* apt : slots) {
            if (!hasStarted(apt->getSlot())) {
                result.push_back(apt);
            }
        }
        return result;
    }

    bool patientHasClash(Patient* patient, const TimeSlot& slot) const {
        for (Appointment* apt : appointmentRepo.getAllAppointments()) {
            if (apt->getStatus() == AppointmentStatus::BOOKED && apt->getPatient() == patient &&
                apt->getSlot().overlaps(slot)) {
                return true;
            }
        }
        return false;
    }

    // Offer a free slot to the first waiting patient who can take it; the others keep their place
    void processWaitlist(const std::string& doctorId, Appointment* availableSlot) {
        auto it = waitlists.find(doctorId);
        if (it == waitlists.end() || hasStarted(availableSlot->getSlot())) return;

        std::deque<WaitlistEntry*>& waiting = it->second;
        for (auto entry = waiting.begin(); entry != waiting.end(); ++entry) {
            Patient* patient = (*entry)->getPatient();
            if (!patientHasClash(patient, availableSlot->getSlot()) && availableSlot->tryBook(patient)) {
                waiting.erase(entry);
                std::cout << "  [Waitlist] Auto-booked " << patient->getName()
                          << " for slot " << availableSlot->getSlot().toString()
                          << " with Dr. " << availableSlot->getDoctor()->getName() << std::endl;
                return;
            }
        }
    }

public:
    AppointmentService() {
        rankingStrategy = std::make_unique<EarliestSlotStrategy>();
    }

    ~AppointmentService() {
        for (auto* entry : allWaitlistEntries) {
            delete entry;
        }
    }

    // Doctor management
    void addDoctor(Doctor* doctor) {
        doctorRepo.addDoctor(doctor);
    }

    Doctor* getDoctor(const std::string& doctorId) {
        return doctorRepo.getDoctor(doctorId);
    }

    std::vector<Doctor*> getDoctorsBySpecialty(Specialty specialty) {
        return doctorRepo.getDoctorsBySpecialty(specialty);
    }

    // Slot management
    Appointment* addSlot(Doctor* doctor, const TimeSlot& slot) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (doctorRepo.getDoctor(doctor->getDoctorId()) == nullptr) {
            std::cout << "  [Failed] Unknown doctor: " << doctor->getName() << std::endl;
            return nullptr;
        }
        if (slot.getEndTime() <= slot.getStartTime()) {
            std::cout << "  [Failed] A slot must end after it starts" << std::endl;
            return nullptr;
        }
        for (Appointment* existing : appointmentRepo.getAppointmentsForDoctor(doctor->getDoctorId())) {
            if (existing->getSlot().overlaps(slot)) {
                std::cout << "  [Failed] Slot overlaps " << existing->getAppointmentId()
                          << " for Dr. " << doctor->getName() << std::endl;
                return nullptr;
            }
        }

        Appointment* appointment = appointmentRepo.createAppointment(doctor, slot);
        processWaitlist(doctor->getDoctorId(), appointment);
        return appointment;
    }

    // Strategy management
    void setRankingStrategy(std::unique_ptr<SlotRankingStrategy> strategy) {
        std::lock_guard<std::mutex> lock(mutex_);
        rankingStrategy = std::move(strategy);
    }

    std::string getActiveStrategy() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return rankingStrategy->getStrategyName();
    }

    // Search available slots
    std::vector<Appointment*> searchAvailableSlots(Specialty specialty) {
        std::lock_guard<std::mutex> lock(mutex_);
        std::vector<Doctor*> doctors = doctorRepo.getDoctorsBySpecialty(specialty);
        std::vector<Appointment*> allAvailable;

        for (Doctor* doctor : doctors) {
            std::vector<Appointment*> doctorSlots = upcoming(appointmentRepo.getAvailableSlots(doctor->getDoctorId()));
            allAvailable.insert(allAvailable.end(), doctorSlots.begin(), doctorSlots.end());
        }

        return rankingStrategy->rankSlots(allAvailable);
    }

    std::vector<Appointment*> searchAvailableSlots(const std::string& doctorId) {
        std::lock_guard<std::mutex> lock(mutex_);
        std::vector<Appointment*> slots = upcoming(appointmentRepo.getAvailableSlots(doctorId));
        return rankingStrategy->rankSlots(slots);
    }

    // Booking
    bool bookAppointment(Patient* patient, const std::string& appointmentId) {
        std::lock_guard<std::mutex> lock(mutex_);
        Appointment* appointment = appointmentRepo.getAppointment(appointmentId);
        if (appointment == nullptr) {
            std::cout << "  [Error] Appointment not found: " << appointmentId << std::endl;
            return false;
        }
        if (hasStarted(appointment->getSlot())) {
            std::cout << "  [Failed] Slot has already started" << std::endl;
            return false;
        }
        if (patientHasClash(patient, appointment->getSlot())) {
            std::cout << "  [Failed] " << patient->getName()
                      << " already has an appointment at that time" << std::endl;
            return false;
        }

        if (appointment->tryBook(patient)) {
            std::cout << "  [Booked] " << patient->getName() << " booked "
                      << appointment->getSlot().toString()
                      << " with Dr. " << appointment->getDoctor()->getName() << std::endl;
            return true;
        } else {
            std::cout << "  [Failed] Slot already taken" << std::endl;
            return false;
        }
    }

    // Cancellation
    bool cancelAppointment(const std::string& appointmentId) {
        std::lock_guard<std::mutex> lock(mutex_);
        Appointment* appointment = appointmentRepo.getAppointment(appointmentId);
        if (appointment == nullptr) return false;

        Patient* patient = appointment->getPatient();
        Doctor* doctor = appointment->getDoctor();

        if (!appointment->cancel()) {
            std::cout << "  [Failed] Only a booked appointment can be cancelled: "
                      << appointmentId << std::endl;
            return false;
        }
        appointment->makeAvailable();  // Make available for rebooking

        std::cout << "  [Cancelled] Appointment " << appointmentId;
        if (patient) std::cout << " for " << patient->getName();
        std::cout << std::endl;

        // Process waitlist
        processWaitlist(doctor->getDoctorId(), appointment);
        return true;
    }

    // Complete appointment
    bool completeAppointment(const std::string& appointmentId) {
        std::lock_guard<std::mutex> lock(mutex_);
        Appointment* appointment = appointmentRepo.getAppointment(appointmentId);
        if (appointment == nullptr) return false;

        if (!appointment->complete()) {
            std::cout << "  [Failed] Only a booked appointment can be completed: "
                      << appointmentId << std::endl;
            return false;
        }
        std::cout << "  [Completed] Appointment " << appointmentId << std::endl;
        return true;
    }

    // Waitlist management
    bool addToWaitlist(Patient* patient, Doctor* doctor) {
        std::lock_guard<std::mutex> lock(mutex_);
        const std::string doctorId = doctor->getDoctorId();
        if (doctorRepo.getDoctor(doctorId) == nullptr) {
            std::cout << "  [Failed] Unknown doctor: " << doctor->getName() << std::endl;
            return false;
        }
        if (!upcoming(appointmentRepo.getAvailableSlots(doctorId)).empty()) {
            std::cout << "  [Failed] Dr. " << doctor->getName()
                      << " has free slots: book one instead" << std::endl;
            return false;
        }

        std::deque<WaitlistEntry*>& waiting = waitlists[doctorId];
        bool alreadyWaiting = std::any_of(waiting.begin(), waiting.end(), [patient](WaitlistEntry* entry) {
            return entry->getPatient() == patient;
        });
        if (alreadyWaiting) {
            std::cout << "  [Failed] " << patient->getName() << " is already on the waitlist" << std::endl;
            return false;
        }

        std::string waitlistId = "WL" + std::to_string(++waitlistCounter);
        WaitlistEntry* entry = new WaitlistEntry(waitlistId, patient, doctor);
        allWaitlistEntries.push_back(entry);
        waiting.push_back(entry);

        std::cout << "  [Waitlist] " << patient->getName()
                  << " added to waitlist for Dr. " << doctor->getName() << std::endl;
        return true;
    }

    int getWaitlistSize(const std::string& doctorId) const {
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = waitlists.find(doctorId);
        return (it != waitlists.end()) ? static_cast<int>(it->second.size()) : 0;
    }

    // Status
    void printDoctorSchedule(const std::string& doctorId) const {
        std::lock_guard<std::mutex> lock(mutex_);
        Doctor* doctor = doctorRepo.getDoctor(doctorId);
        if (!doctor) return;

        std::cout << "\n--- Schedule for Dr. " << doctor->getName()
                  << " (" << specialtyToString(doctor->getSpecialty()) << ") ---" << std::endl;
        std::cout << "Rating: " << doctor->getRating()
                  << " | Fee: $" << doctor->getConsultationFee() << std::endl;

        for (Appointment* apt : appointmentRepo.getAppointmentsForDoctor(doctorId)) {
            std::cout << "  " << apt->getAppointmentId() << " | "
                      << apt->getSlot().toString() << " | "
                      << appointmentStatusToString(apt->getStatus());
            if (apt->getPatient()) {
                std::cout << " | " << apt->getPatient()->getName();
            }
            std::cout << std::endl;
        }
    }
};

#endif // APPOINTMENT_SERVICE_HPP
