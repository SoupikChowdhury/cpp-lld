#ifndef WAITLIST_ENTRY_HPP
#define WAITLIST_ENTRY_HPP

#include <string>
#include <ctime>
#include "Patient.hpp"
#include "Doctor.hpp"

class WaitlistEntry {
private:
    std::string waitlistId;
    Patient* patient;
    Doctor* doctor;
    time_t requestedTime;

public:
    WaitlistEntry(const std::string& waitlistId, Patient* patient, 
                  Doctor* doctor)
        : waitlistId(waitlistId), patient(patient), doctor(doctor) {
        requestedTime = time(nullptr);
    }

    // Getters
    std::string getWaitlistId() const { return waitlistId; }
    Patient* getPatient() const { return patient; }
    Doctor* getDoctor() const { return doctor; }
    time_t getRequestedTime() const { return requestedTime; }
};

#endif // WAITLIST_ENTRY_HPP
