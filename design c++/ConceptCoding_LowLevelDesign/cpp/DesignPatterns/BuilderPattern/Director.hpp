#pragma once
#include "StudentBuilder.hpp"
#include "EngineeringStudentBuilder.hpp"
#include "MBAStudentBuilder.hpp"
#include &lt;memory&gt;

/**
 * Director - Controls the building process.
 * Knows the recipe/order to build different types of students.
 */
class Director {
private:
    std::shared_ptr&lt;StudentBuilder&gt; studentBuilder;

public:
    explicit Director(std::shared_ptr&lt;StudentBuilder&gt; builder)
        : studentBuilder(builder) {}

    std::unique_ptr&lt;Student&gt; createStudent() {
        // Check type and build accordingly
        if (dynamic_cast&lt;EngineeringStudentBuilder*&gt;(studentBuilder.get())) {
            return createEngineeringStudent();
        } else if (dynamic_cast&lt;MBAStudentBuilder*&gt;(studentBuilder.get())) {
            return createMBAStudent();
        }
        return nullptr;
    }

private:
    std::unique_ptr&lt;Student&gt; createEngineeringStudent() {
        studentBuilder-&gt;setRollNumber(1)
                      .setAge(22)
                      .setName("John")
                      .setSubjects();
        return studentBuilder-&gt;build();
    }

    std::unique_ptr&lt;Student&gt; createMBAStudent() {
        studentBuilder-&gt;setRollNumber(2)
                      .setAge(24)
                      .setName("Jane")
                      .setFatherName("Father Name")
                      .setMotherName("Mother Name")
                      .setSubjects();
        return studentBuilder-&gt;build();
    }
};
