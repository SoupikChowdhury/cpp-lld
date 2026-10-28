#include "RoboticFactory.hpp"
#include <iostream>

/**
 * Flyweight Pattern Demo
 * 
 * Creates many robot instances but shares underlying heavy objects.
 * Without flyweight: 1000 robots = 1000 sprite objects (10GB memory)
 * With flyweight: 1000 robots = 2 sprite objects (20MB memory)
 */
int main() {
    std::cout << "=== Flyweight Pattern Demo ===" << std::endl;
    std::cout << std::endl;

    // Create humanoid robots at different positions
    auto humanoid1 = RoboticFactory::createRobot("HUMANOID");
    humanoid1->display(1, 2);

    auto humanoid2 = RoboticFactory::createRobot("HUMANOID");  // Cached!
    humanoid2->display(10, 30);

    auto humanoid3 = RoboticFactory::createRobot("HUMANOID");  // Cached!
    humanoid3->display(100, 200);

    std::cout << std::endl;

    // Create robotic dogs at different positions
    auto dog1 = RoboticFactory::createRobot("ROBOTICDOG");
    dog1->display(2, 9);

    auto dog2 = RoboticFactory::createRobot("ROBOTICDOG");  // Cached!
    dog2->display(11, 19);

    std::cout << std::endl;

    // Verify caching
    std::cout << "Total robot types in cache: " << RoboticFactory::getCacheSize() << std::endl;
    std::cout << "Total robot instances created: 5" << std::endl;
    std::cout << "Heavy objects created: " << RoboticFactory::getCacheSize() << std::endl;

    // Same object reference check
    std::cout << "\nhumanoid1 == humanoid2: " 
              << (humanoid1.get() == humanoid2.get() ? "true" : "false") << std::endl;

    return 0;
}
