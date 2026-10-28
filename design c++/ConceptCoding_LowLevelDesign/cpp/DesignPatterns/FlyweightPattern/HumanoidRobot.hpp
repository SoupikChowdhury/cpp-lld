#pragma once
#include "IRobot.hpp"
#include "Sprites.hpp"
#include &lt;iostream&gt;
#include &lt;memory&gt;

/**
 * Concrete Flyweight - Humanoid Robot
 * 
 * Intrinsic state: type, body (sprites) - shared, immutable
 * Extrinsic state: x, y position - passed from outside
 */
class HumanoidRobot : public IRobot {
private:
    std::string type;                      // Intrinsic - shared
    std::shared_ptr&lt;Sprites&gt; body;        // Intrinsic - shared (heavy object)

public:
    HumanoidRobot(const std::string&amp; t, std::shared_ptr&lt;Sprites&gt; b)
        : type(t), body(b) {
        std::cout &lt;&lt; "Creating HumanoidRobot (expensive operation)" &lt;&lt; std::endl;
    }

    void display(int x, int y) override {
        std::cout &lt;&lt; "Humanoid Robot at (" &lt;&lt; x &lt;&lt; ", " &lt;&lt; y &lt;&lt; ")" &lt;&lt; std::endl;
        // Use shared sprites to render at position
    }

    const std::string&amp; getType() const { return type; }
};
