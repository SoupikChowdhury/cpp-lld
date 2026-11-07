#pragma once
#include "StudentBuilder.hpp"
#include <memory>

/**
 * Director - Controls the building process.
 *
 * Knows the recipes: which steps to run, and in what order. It only sees the
 * StudentBuilder interface, so any new builder works with every recipe.
 */
class Director {
private:
    std::shared_ptr<StudentBuilder> studentBuilder;

public:
    explicit Director(std::shared_ptr<StudentBuilder> builder)
        : studentBuilder(builder) {}

    std::unique_ptr<Student> createBasicStudent() {
        studentBuilder->setRollNumber(1)
                      .setAge(22)
                      .setName("John")
                      .setSubjects();
        return studentBuilder->build();
    }

    std::unique_ptr<Student> createStudentWithParents() {
        studentBuilder->setRollNumber(2)
                      .setAge(24)
                      .setName("Jane")
                      .setFatherName("Father Name")
                      .setMotherName("Mother Name")
                      .setSubjects();
        return studentBuilder->build();
    }
};
