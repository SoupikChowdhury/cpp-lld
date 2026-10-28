#pragma once
#include "StudentBuilder.hpp"
#include "EngineeringStudentBuilder.hpp"
#include "MBAStudentBuilder.hpp"
#include <memory>

/**
 * Director - Controls the building process.
 * Knows the recipe/order to build different types of students.
 */
class Director {
private:
    std::shared_ptr<StudentBuilder> studentBuilder;

public:
    explicit Director(std::shared_ptr<StudentBuilder> builder)
        : studentBuilder(builder) {}

    std::unique_ptr<Student> createStudent() {
        // Check type and build accordingly
        if (dynamic_cast<EngineeringStudentBuilder*>(studentBuilder.get())) {
            return createEngineeringStudent();
        } else if (dynamic_cast<MBAStudentBuilder*>(studentBuilder.get())) {
            return createMBAStudent();
        }
        return nullptr;
    }

private:
    std::unique_ptr<Student> createEngineeringStudent() {
        studentBuilder->setRollNumber(1)
                      .setAge(22)
                      .setName("John")
                      .setSubjects();
        return studentBuilder->build();
    }

    std::unique_ptr<Student> createMBAStudent() {
        studentBuilder->setRollNumber(2)
                      .setAge(24)
                      .setName("Jane")
                      .setFatherName("Father Name")
                      .setMotherName("Mother Name")
                      .setSubjects();
        return studentBuilder->build();
    }
};
