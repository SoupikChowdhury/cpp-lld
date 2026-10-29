#include "Director.hpp"
#include <iostream>

/**
 * Builder Pattern Demo
 * 
 * Creates different types of Student objects using builders.
 */
int main() {
    // Create Engineering Student
    auto engBuilder = std::make_shared<EngineeringStudentBuilder>();
    Director engDirector(engBuilder);
    auto engineerStudent = engDirector.createStudent();

    // Create MBA Student
    auto mbaBuilder = std::make_shared<MBAStudentBuilder>();
    Director mbaDirector(mbaBuilder);
    auto mbaStudent = mbaDirector.createStudent();

    std::cout << "Engineering Student: " << engineerStudent->toString() << std::endl;
    std::cout << "MBA Student: " << mbaStudent->toString() << std::endl;

    // Builder can also be used without Director (Fluent interface)
    std::cout << "\n--- Using Builder Directly (Fluent) ---\n";
    auto customBuilder = std::make_unique<EngineeringStudentBuilder>();
    customBuilder->setRollNumber(100)
                  .setAge(20)
                  .setName("Custom Student")
                  .setSubjects();
    auto customStudent = customBuilder->build();
    std::cout << "Custom: " << customStudent->toString() << std::endl;

    return 0;
}
