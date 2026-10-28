#include "RoboticFactory.hpp"
#include &lt;iostream&gt;

/**
 * Flyweight Pattern Demo
 * 
 * Creates many robot instances but shares underlying heavy objects.
 * Without flyweight: 1000 robots = 1000 sprite objects (10GB memory)
 * With flyweight: 1000 robots = 2 sprite objects (20MB memory)
 */
int main() {
    std::cout &lt;&lt; "=== Flyweight Pattern Demo ===" &lt;&lt; std::endl;
    std::cout &lt;&lt; std::endl;

    // Create humanoid robots at different positions
    auto humanoid1 = RoboticFactory::createRobot("HUMANOID");
    humanoid1-&gt;display(1, 2);

    auto humanoid2 = RoboticFactory::createRobot("HUMANOID");  // Cached!
    humanoid2-&gt;display(10, 30);

    auto humanoid3 = RoboticFactory::createRobot("HUMANOID");  // Cached!
    humanoid3-&gt;display(100, 200);

    std::cout &lt;&lt; std::endl;

    // Create robotic dogs at different positions
    auto dog1 = RoboticFactory::createRobot("ROBOTICDOG");
    dog1-&gt;display(2, 9);

    auto dog2 = RoboticFactory::createRobot("ROBOTICDOG");  // Cached!
    dog2-&gt;display(11, 19);

    std::cout &lt;&lt; std::endl;

    // Verify caching
    std::cout &lt;&lt; "Total robot types in cache: " &lt;&lt; RoboticFactory::getCacheSize() &lt;&lt; std::endl;
    std::cout &lt;&lt; "Total robot instances created: 5" &lt;&lt; std::endl;
    std::cout &lt;&lt; "Heavy objects created: " &lt;&lt; RoboticFactory::getCacheSize() &lt;&lt; std::endl;

    // Same object reference check
    std::cout &lt;&lt; "\nhumanoid1 == humanoid2: " 
              &lt;&lt; (humanoid1.get() == humanoid2.get() ? "true" : "false") &lt;&lt; std::endl;

    return 0;
}
