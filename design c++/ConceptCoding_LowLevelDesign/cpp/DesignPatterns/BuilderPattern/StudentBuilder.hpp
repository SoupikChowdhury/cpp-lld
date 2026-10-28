#pragma once
#include "Student.hpp"
#include &lt;memory&gt;

/**
 * Builder Pattern - Abstract Builder
 * 
 * Defines all steps to build a Student object.
 */
class StudentBuilder {
public:
    int rollNumber = 0;
    int age = 0;
    std::string name;
    std::string fatherName;
    std::string motherName;
    std::vector&lt;std::string&gt; subjects;

    virtual ~StudentBuilder() = default;

    StudentBuilder&amp; setRollNumber(int roll) {
        rollNumber = roll;
        return *this;
    }

    StudentBuilder&amp; setAge(int a) {
        age = a;
        return *this;
    }

    StudentBuilder&amp; setName(const std::string&amp; n) {
        name = n;
        return *this;
    }

    StudentBuilder&amp; setFatherName(const std::string&amp; fn) {
        fatherName = fn;
        return *this;
    }

    StudentBuilder&amp; setMotherName(const std::string&amp; mn) {
        motherName = mn;
        return *this;
    }

    // Abstract method - concrete builders implement this
    virtual StudentBuilder&amp; setSubjects() = 0;

    std::unique_ptr&lt;Student&gt; build() {
        auto student = std::make_unique&lt;Student&gt;();
        student-&gt;rollNumber = rollNumber;
        student-&gt;age = age;
        student-&gt;name = name;
        student-&gt;fatherName = fatherName;
        student-&gt;motherName = motherName;
        student-&gt;subjects = subjects;
        return student;
    }
};
