#pragma once
#include "StudentBuilder.hpp"

/**
 * Concrete Builder for MBA Students.
 * Sets MBA-specific subjects.
 */
class MBAStudentBuilder : public StudentBuilder {
public:
    StudentBuilder& setSubjects() override {
        subjects.clear();
        subjects.push_back("Micro Economics");
        subjects.push_back("Business Studies");
        subjects.push_back("Operations Management");
        return *this;
    }
};
