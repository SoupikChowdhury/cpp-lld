#pragma once
#include "StudentBuilder.hpp"

/**
 * Concrete Builder for Engineering Students.
 * Sets engineering-specific subjects.
 */
class EngineeringStudentBuilder : public StudentBuilder {
public:
    StudentBuilder&amp; setSubjects() override {
        subjects.clear();
        subjects.push_back("DSA");
        subjects.push_back("OS");
        subjects.push_back("Computer Architecture");
        return *this;
    }
};
