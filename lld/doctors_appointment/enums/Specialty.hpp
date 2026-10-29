#ifndef SPECIALTY_HPP
#define SPECIALTY_HPP

#include <string>

enum class Specialty {
    GENERAL,
    CARDIOLOGY,
    DERMATOLOGY,
    ORTHOPEDICS,
    PEDIATRICS,
    NEUROLOGY
};

inline std::string specialtyToString(Specialty specialty) {
    switch (specialty) {
        case Specialty::GENERAL: return "GENERAL";
        case Specialty::CARDIOLOGY: return "CARDIOLOGY";
        case Specialty::DERMATOLOGY: return "DERMATOLOGY";
        case Specialty::ORTHOPEDICS: return "ORTHOPEDICS";
        case Specialty::PEDIATRICS: return "PEDIATRICS";
        case Specialty::NEUROLOGY: return "NEUROLOGY";
        default: return "UNKNOWN";
    }
}

#endif // SPECIALTY_HPP
