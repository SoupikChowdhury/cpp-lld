#pragma once
#include "IRobot.hpp"
#include "Sprites.hpp"
#include <iostream>
#include <memory>

/**
 * Concrete Flyweight - Humanoid Robot
 * 
 * Intrinsic state: type, body (sprites) - shared, immutable
 * Extrinsic state: x, y position - passed from outside
 */
class HumanoidRobot : public IRobot {
private:
    std::string type;                      // Intrinsic - shared
    std::shared_ptr<Sprites> body;        // Intrinsic - shared (heavy object)

public:
    HumanoidRobot(const std::string& t, std::shared_ptr<Sprites> b)
        : type(t), body(b) {
        std::cout << "Creating HumanoidRobot (expensive operation)" << std::endl;
    }

    void display(int x, int y) override {
        std::cout << "Humanoid Robot at (" << x << ", " << y << ")" << std::endl;
        // Use shared sprites to render at position
    }

    const std::string& getType() const { return type; }
};
