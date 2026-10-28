#ifndef DOCTOR_REPOSITORY_HPP
#define DOCTOR_REPOSITORY_HPP

#include <unordered_map>
#include <vector>
#include <mutex>
#include "../model/Doctor.hpp"
#include "../enums/Specialty.hpp"

class DoctorRepository {
private:
    std::unordered_map<std::string, Doctor*> doctors;
    mutable std::mutex mutex_;

public:
    void addDoctor(Doctor* doctor) {
        std::lock_guard<std::mutex> lock(mutex_);
        doctors[doctor->getDoctorId()] = doctor;
    }

    Doctor* getDoctor(const std::string& doctorId) {
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = doctors.find(doctorId);
        return (it != doctors.end()) ? it->second : nullptr;
    }

    std::vector<Doctor*> getAllDoctors() const {
        std::lock_guard<std::mutex> lock(mutex_);
        std::vector<Doctor*> allDoctors;
        for (const auto& [id, doctor] : doctors) {
            allDoctors.push_back(doctor);
        }
        return allDoctors;
    }

    std::vector<Doctor*> getDoctorsBySpecialty(Specialty specialty) const {
        std::lock_guard<std::mutex> lock(mutex_);
        std::vector<Doctor*> result;
        for (const auto& [id, doctor] : doctors) {
            if (doctor->getSpecialty() == specialty) {
                result.push_back(doctor);
            }
        }
        return result;
    }

    int getDoctorCount() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return static_cast<int>(doctors.size());
    }
};

#endif // DOCTOR_REPOSITORY_HPP
