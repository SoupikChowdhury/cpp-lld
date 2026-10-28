#pragma once
#include &lt;string&gt;
#include &lt;vector&gt;

/**
 * Builder Pattern - Product
 * 
 * The complex object that is being built.
 */
class Student {
public:
    int rollNumber = 0;
    int age = 0;
    std::string name;
    std::string fatherName;
    std::string motherName;
    std::vector&lt;std::string&gt; subjects;

    std::string toString() const {
        std::string subjectsStr;
        for (size_t i = 0; i &lt; subjects.size(); ++i) {
            subjectsStr += subjects[i];
            if (i &lt; subjects.size() - 1) subjectsStr += ", ";
        }
        return "Roll: " + std::to_string(rollNumber) +
               " Age: " + std::to_string(age) +
               " Name: " + name +
               " Father: " + fatherName +
               " Mother: " + motherName +
               " Subjects: [" + subjectsStr + "]";
    }
};
