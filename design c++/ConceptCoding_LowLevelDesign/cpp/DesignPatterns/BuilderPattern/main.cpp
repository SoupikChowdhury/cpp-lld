#include "Director.hpp"
#include &lt;iostream&gt;

/**
 * Builder Pattern Demo
 * 
 * Creates different types of Student objects using builders.
 */
int main() {
    // Create Engineering Student
    auto engBuilder = std::make_shared&lt;EngineeringStudentBuilder&gt;();
    Director engDirector(engBuilder);
    auto engineerStudent = engDirector.createStudent();

    // Create MBA Student
    auto mbaBuilder = std::make_shared&lt;MBAStudentBuilder&gt;();
    Director mbaDirector(mbaBuilder);
    auto mbaStudent = mbaDirector.createStudent();

    std::cout &lt;&lt; "Engineering Student: " &lt;&lt; engineerStudent-&gt;toString() &lt;&lt; std::endl;
    std::cout &lt;&lt; "MBA Student: " &lt;&lt; mbaStudent-&gt;toString() &lt;&lt; std::endl;

    // Builder can also be used without Director (Fluent interface)
    std::cout &lt;&lt; "\n--- Using Builder Directly (Fluent) ---\n";
    auto customBuilder = std::make_unique&lt;EngineeringStudentBuilder&gt;();
    customBuilder-&gt;setRollNumber(100)
                  .setAge(20)
                  .setName("Custom Student")
                  .setSubjects();
    auto customStudent = customBuilder-&gt;build();
    std::cout &lt;&lt; "Custom: " &lt;&lt; customStudent-&gt;toString() &lt;&lt; std::endl;

    return 0;
}
