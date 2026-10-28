#ifndef APPOINTMENT_SERVICE_HPP
#define APPOINTMENT_SERVICE_HPP

#include <memory>
#include <iostream>
#include <queue>
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
    std::unordered_map<std::string, std::queue<WaitlistEntry*>> waitlists;
    std::vector<WaitlistEntry*> allWaitlistEntries;  // For cleanup
    int waitlistCounter = 0;

    void processWaitlist(const std::string& doctorId, Appointment* availableSlot) {
        auto it = waitlists.find(doctorId);
        if (it != waitlists.end() && !it->second.empty()) {
            WaitlistEntry* entry = it->second.front();
            it->second.pop();
            
            if (availableSlot->tryBook(entry->getPatient())) {
                std::cout << "  [Waitlist] Auto-booked " << entry->getPatient()->getName()
                          << " for slot " << availableSlot->getSlot().toString()
                          << " with Dr. " << availableSlot->getDoctor()->getName() << std::endl;
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
        return appointmentRepo.createAppointment(doctor, slot);
    }

    // Strategy management
    void setRankingStrategy(std::unique_ptr<SlotRankingStrategy> strategy) {
        rankingStrategy = std::move(strategy);
    }

    std::string getActiveStrategy() const {
        return rankingStrategy->getStrategyName();
    }

    // Search available slots
    std::vector<Appointment*> searchAvailableSlots(Specialty specialty) {
        std::vector<Doctor*> doctors = doctorRepo.getDoctorsBySpecialty(specialty);
        std::vector<Appointment*> allAvailable;

        for (Doctor* doctor : doctors) {
            std::vector<Appointment*> doctorSlots = appointmentRepo.getAvailableSlots(doctor->getDoctorId());
            allAvailable.insert(allAvailable.end(), doctorSlots.begin(), doctorSlots.end());
        }

        return rankingStrategy->rankSlots(allAvailable);
    }

    std::vector<Appointment*> searchAvailableSlots(const std::string& doctorId) {
        std::vector<Appointment*> slots = appointmentRepo.getAvailableSlots(doctorId);
        return rankingStrategy->rankSlots(slots);
    }

    // Booking
    bool bookAppointment(Patient* patient, const std::string& appointmentId) {
        Appointment* appointment = appointmentRepo.getAppointment(appointmentId);
        if (appointment == nullptr) {
            std::cout << "  [Error] Appointment not found: " << appointmentId << std::endl;
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
    void cancelAppointment(const std::string& appointmentId) {
        Appointment* appointment = appointmentRepo.getAppointment(appointmentId);
        if (appointment == nullptr) return;

        Patient* patient = appointment->getPatient();
        Doctor* doctor = appointment->getDoctor();
        
        appointment->cancel();
        appointment->makeAvailable();  // Make available for rebooking
        
        std::cout << "  [Cancelled] Appointment " << appointmentId;
        if (patient) std::cout << " for " << patient->getName();
        std::cout << std::endl;

        // Process waitlist
        processWaitlist(doctor->getDoctorId(), appointment);
    }

    // Complete appointment
    void completeAppointment(const std::string& appointmentId) {
        Appointment* appointment = appointmentRepo.getAppointment(appointmentId);
        if (appointment != nullptr) {
            appointment->complete();
            std::cout << "  [Completed] Appointment " << appointmentId << std::endl;
        }
    }

    // Waitlist management
    void addToWaitlist(Patient* patient, Doctor* doctor) {
        std::string waitlistId = "WL" + std::to_string(++waitlistCounter);
        WaitlistEntry* entry = new WaitlistEntry(waitlistId, patient, doctor);
        allWaitlistEntries.push_back(entry);
        waitlists[doctor->getDoctorId()].push(entry);
        
        std::cout << "  [Waitlist] " << patient->getName() 
                  << " added to waitlist for Dr. " << doctor->getName() << std::endl;
    }

    int getWaitlistSize(const std::string& doctorId) const {
        auto it = waitlists.find(doctorId);
        return (it != waitlists.end()) ? static_cast<int>(it->second.size()) : 0;
    }

    // Status
    void printDoctorSchedule(const std::string& doctorId) const {
        Doctor* doctor = const_cast<DoctorRepository&>(doctorRepo).getDoctor(doctorId);
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
