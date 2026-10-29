// Doctor Appointment System - Main Entry Point
// Demonstrates Strategy, Repository patterns

#include <iostream>
#include <iomanip>
#include "model/Doctor.hpp"
#include "model/Patient.hpp"
#include "model/TimeSlot.hpp"
#include "service/AppointmentService.hpp"
#include "strategy/EarliestSlotStrategy.hpp"
#include "strategy/HighestRatedDoctorStrategy.hpp"
#include "strategy/CheapestSlotStrategy.hpp"

// Helper to create time slots
TimeSlot createSlot(int hoursFromNow, int durationMinutes) {
    time_t now = time(nullptr);
    time_t start = now + (hoursFromNow * 3600);
    return TimeSlot(start, durationMinutes);
}

int main() {
    std::cout << "=== Doctor Appointment System Demo ===" << std::endl;
    std::cout << std::endl;

    AppointmentService service;

    // Create doctors
    Doctor* drSmith = new Doctor("D001", "Dr. Smith", Specialty::CARDIOLOGY, 4.5, 150.0);
    Doctor* drJones = new Doctor("D002", "Dr. Jones", Specialty::CARDIOLOGY, 4.8, 200.0);
    Doctor* drWilson = new Doctor("D003", "Dr. Wilson", Specialty::DERMATOLOGY, 4.2, 100.0);
    Doctor* drBrown = new Doctor("D004", "Dr. Brown", Specialty::GENERAL, 4.0, 75.0);

    service.addDoctor(drSmith);
    service.addDoctor(drJones);
    service.addDoctor(drWilson);
    service.addDoctor(drBrown);

    std::cout << "--- Doctors Registered ---" << std::endl;
    std::cout << "Dr. Smith - Cardiology, Rating: 4.5, Fee: $150" << std::endl;
    std::cout << "Dr. Jones - Cardiology, Rating: 4.8, Fee: $200" << std::endl;
    std::cout << "Dr. Wilson - Dermatology, Rating: 4.2, Fee: $100" << std::endl;
    std::cout << "Dr. Brown - General, Rating: 4.0, Fee: $75" << std::endl;

    // Create time slots for doctors
    std::cout << std::endl << "--- Adding Time Slots ---" << std::endl;
    
    // Dr. Smith's slots (next 2 hours)
    service.addSlot(drSmith, createSlot(1, 30));  // 1 hour from now, 30 min
    service.addSlot(drSmith, createSlot(2, 30));  // 2 hours from now
    service.addSlot(drSmith, createSlot(3, 30));

    // Dr. Jones's slots
    service.addSlot(drJones, createSlot(1, 30));
    service.addSlot(drJones, createSlot(4, 30));

    // Dr. Wilson's slots
    service.addSlot(drWilson, createSlot(2, 30));
    service.addSlot(drWilson, createSlot(3, 30));

    // Dr. Brown's slots
    service.addSlot(drBrown, createSlot(1, 30));
    service.addSlot(drBrown, createSlot(2, 30));

    std::cout << "Time slots added for all doctors" << std::endl;

    // Create patients
    Patient alice("P001", "Alice Johnson", "alice@email.com", "555-0101", 35);
    Patient bob("P002", "Bob Williams", "bob@email.com", "555-0102", 45);
    Patient carol("P003", "Carol Davis", "carol@email.com", "555-0103", 28);
    Patient david("P004", "David Miller", "david@email.com", "555-0104", 52);

    // Search with Earliest Slot Strategy (default)
    std::cout << std::endl << "--- Searching Cardiology Slots (Earliest First) ---" << std::endl;
    std::cout << "Strategy: " << service.getActiveStrategy() << std::endl;
    
    auto cardioSlots = service.searchAvailableSlots(Specialty::CARDIOLOGY);
    std::cout << "Available slots:" << std::endl;
    for (auto* apt : cardioSlots) {
        std::cout << "  " << apt->getAppointmentId() << " | Dr. " 
                  << apt->getDoctor()->getName() << " | "
                  << apt->getSlot().toString() << " | $"
                  << apt->getDoctor()->getConsultationFee() << std::endl;
    }

    // Book first available slot for Alice
    std::cout << std::endl << "--- Booking Appointments ---" << std::endl;
    if (!cardioSlots.empty()) {
        service.bookAppointment(&alice, cardioSlots[0]->getAppointmentId());
    }

    // Switch to Highest Rated Doctor Strategy
    std::cout << std::endl << "--- Switching to Highest Rated Doctor Strategy ---" << std::endl;
    service.setRankingStrategy(std::make_unique<HighestRatedDoctorStrategy>());
    std::cout << "Strategy: " << service.getActiveStrategy() << std::endl;

    cardioSlots = service.searchAvailableSlots(Specialty::CARDIOLOGY);
    std::cout << "Available slots (sorted by rating):" << std::endl;
    for (auto* apt : cardioSlots) {
        std::cout << "  " << apt->getAppointmentId() << " | Dr. " 
                  << apt->getDoctor()->getName() 
                  << " (Rating: " << apt->getDoctor()->getRating() << ") | "
                  << apt->getSlot().toString() << std::endl;
    }

    // Bob books highest rated available
    if (!cardioSlots.empty()) {
        service.bookAppointment(&bob, cardioSlots[0]->getAppointmentId());
    }

    // Switch to Cheapest Strategy
    std::cout << std::endl << "--- Switching to Cheapest Slot Strategy ---" << std::endl;
    service.setRankingStrategy(std::make_unique<CheapestSlotStrategy>());
    std::cout << "Strategy: " << service.getActiveStrategy() << std::endl;

    auto generalSlots = service.searchAvailableSlots(Specialty::GENERAL);
    std::cout << "Available General Practice slots:" << std::endl;
    for (auto* apt : generalSlots) {
        std::cout << "  " << apt->getAppointmentId() << " | Dr. " 
                  << apt->getDoctor()->getName() << " | $"
                  << apt->getDoctor()->getConsultationFee() << std::endl;
    }

    // Carol books cheapest
    if (!generalSlots.empty()) {
        service.bookAppointment(&carol, generalSlots[0]->getAppointmentId());
    }

    // David wants Dr. Jones but no slots - add to waitlist
    std::cout << std::endl << "--- Waitlist Demonstration ---" << std::endl;
    auto jonesSlots = service.searchAvailableSlots("D002");
    if (jonesSlots.empty()) {
        std::cout << "No slots available for Dr. Jones" << std::endl;
    }
    service.addToWaitlist(&david, drJones);
    std::cout << "Waitlist size for Dr. Jones: " << service.getWaitlistSize("D002") << std::endl;

    // Cancel Bob's appointment (triggers waitlist)
    std::cout << std::endl << "--- Cancellation with Waitlist Processing ---" << std::endl;
    service.cancelAppointment("APT000004");  // Bob's appointment with Dr. Jones

    // Print doctor schedules
    service.printDoctorSchedule("D001");
    service.printDoctorSchedule("D002");

    // Complete an appointment
    std::cout << std::endl << "--- Completing Appointment ---" << std::endl;
    service.completeAppointment("APT000001");

    service.printDoctorSchedule("D001");

    // Cleanup
    delete drSmith;
    delete drJones;
    delete drWilson;
    delete drBrown;

    std::cout << std::endl << "=== Demo Complete ===" << std::endl;

    return 0;
}
