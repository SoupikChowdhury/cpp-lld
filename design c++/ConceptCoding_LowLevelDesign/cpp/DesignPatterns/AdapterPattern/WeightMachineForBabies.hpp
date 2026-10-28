#pragma once
#include "WeightMachine.hpp"

/**
 * Concrete implementation of WeightMachine.
 * Returns weight in Pounds.
 */
class WeightMachineForBabies : public WeightMachine {
public:
    double getWeightInPound() override {
        return 28.0; // Returns weight in pounds
    }
};
