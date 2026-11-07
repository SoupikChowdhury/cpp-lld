#pragma once
#include <string>
#include <vector>

class StudentBuilder;

/**
 * Builder Pattern - Product
 *
 * The complex object that is being built. Only StudentBuilder can create one,
 * so a Student is always valid, and it can't change after it's built.
 */
class Student {
private:
    int rollNumber = 0;
    int age = 0;
    std::string name;
    std::string fatherName;
    std::string motherName;
    std::vector<std::string> subjects;

    Student() = default;
    friend class StudentBuilder;

public:
    int getRollNumber() const { return rollNumber; }
    int getAge() const { return age; }
    const std::string& getName() const { return name; }
    const std::string& getFatherName() const { return fatherName; }
    const std::string& getMotherName() const { return motherName; }
    const std::vector<std::string>& getSubjects() const { return subjects; }

    std::string toString() const {
        std::string subjectsStr;
        for (size_t i = 0; i < subjects.size(); ++i) {
            subjectsStr += subjects[i];
            if (i < subjects.size() - 1) subjectsStr += ", ";
        }
        return "Roll: " + std::to_string(rollNumber) +
               " Age: " + std::to_string(age) +
               " Name: " + name +
               " Father: " + fatherName +
               " Mother: " + motherName +
               " Subjects: [" + subjectsStr + "]";
    }
};
