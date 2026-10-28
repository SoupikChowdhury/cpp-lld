#pragma once

/**
 * Adapter Pattern - Target Interface
 * 
 * The existing interface that returns weight in Pounds.
 * This is the "incompatible" interface we need to adapt.
 */
class WeightMachine {
public:
    virtual ~WeightMachine() = default;
    virtual double getWeightInPound() = 0;
};
