#pragma once
#include "Student.hpp"
#include <memory>
#include <stdexcept>

/**
 * Builder Pattern - Abstract Builder
 *
 * Defines all steps to build a Student object.
 */
class StudentBuilder {
protected:
    int rollNumber = 0;
    int age = 0;
    std::string name;
    std::string fatherName;
    std::string motherName;
    std::vector<std::string> subjects;

public:
    virtual ~StudentBuilder() = default;

    StudentBuilder& setRollNumber(int roll) {
        rollNumber = roll;
        return *this;
    }

    StudentBuilder& setAge(int a) {
        age = a;
        return *this;
    }

    StudentBuilder& setName(const std::string& n) {
        name = n;
        return *this;
    }

    StudentBuilder& setFatherName(const std::string& fn) {
        fatherName = fn;
        return *this;
    }

    StudentBuilder& setMotherName(const std::string& mn) {
        motherName = mn;
        return *this;
    }

    // Abstract method - concrete builders implement this
    virtual StudentBuilder& setSubjects() = 0;

    std::unique_ptr<Student> build() const {
        if (name.empty()) {
            throw std::invalid_argument("A student needs a name");
        }
        if (subjects.empty()) {
            throw std::invalid_argument("A student needs subjects: call setSubjects");
        }

        // Not make_unique: it couldn't reach Student's private constructor
        std::unique_ptr<Student> student(new Student());
        student->rollNumber = rollNumber;
        student->age = age;
        student->name = name;
        student->fatherName = fatherName;
        student->motherName = motherName;
        student->subjects = subjects;
        return student;
    }
};
