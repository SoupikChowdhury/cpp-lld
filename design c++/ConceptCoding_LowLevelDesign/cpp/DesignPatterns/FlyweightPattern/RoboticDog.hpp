#pragma once
#include "IRobot.hpp"
#include "Sprites.hpp"
#include &lt;iostream&gt;
#include &lt;memory&gt;

/**
 * Concrete Flyweight - Robotic Dog
 * 
 * Intrinsic state: type, body (sprites) - shared, immutable
 * Extrinsic state: x, y position - passed from outside
 */
class RoboticDog : public IRobot {
private:
    std::string type;                      // Intrinsic - shared
    std::shared_ptr&lt;Sprites&gt; body;        // Intrinsic - shared (heavy object)

public:
    RoboticDog(const std::string&amp; t, std::shared_ptr&lt;Sprites&gt; b)
        : type(t), body(b) {
        std::cout &lt;&lt; "Creating RoboticDog (expensive operation)" &lt;&lt; std::endl;
    }

    void display(int x, int y) override {
        std::cout &lt;&lt; "Robotic Dog at (" &lt;&lt; x &lt;&lt; ", " &lt;&lt; y &lt;&lt; ")" &lt;&lt; std::endl;
        // Use shared sprites to render at position
    }

    const std::string&amp; getType() const { return type; }
};
