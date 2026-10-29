#pragma once

/**
 * Flyweight Pattern - Flyweight Interface
 */
class IRobot {
public:
    virtual ~IRobot() = default;
    // Extrinsic state (position) is passed as parameter
    virtual void display(int x, int y) = 0;
};
