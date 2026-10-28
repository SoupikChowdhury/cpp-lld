#ifndef DOCTOR_HPP
#define DOCTOR_HPP

#include <string>
#include "../enums/Specialty.hpp"

class Doctor {
private:
    std::string doctorId;
    std::string name;
    Specialty specialty;
    double rating;  // 1.0 - 5.0
    double consultationFee;

public:
    Doctor(const std::string& doctorId, const std::string& name,
           Specialty specialty, double rating, double consultationFee)
        : doctorId(doctorId), name(name), specialty(specialty),
          rating(rating), consultationFee(consultationFee) {}

    // Getters
    std::string getDoctorId() const { return doctorId; }
    std::string getName() const { return name; }
    Specialty getSpecialty() const { return specialty; }
    double getRating() const { return rating; }
    double getConsultationFee() const { return consultationFee; }

    // Setters
    void setRating(double newRating) { 
        rating = std::min(5.0, std::max(1.0, newRating)); 
    }
};

#endif // DOCTOR_HPP
