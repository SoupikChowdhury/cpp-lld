#pragma once
#include "WeightMachineAdapter.hpp"
#include "WeightMachine.hpp"
#include <memory>

/**
 * Adapter - Converts Pound interface to Kg interface.
 * 
 * This adapter wraps a WeightMachine (returns pounds)
 * and provides a getWeightInKg() method for the client.
 */
class WeightMachineAdapterImpl : public WeightMachineAdapter {
private:
    std::unique_ptr<WeightMachine> weightMachine;

public:
    explicit WeightMachineAdapterImpl(std::unique_ptr<WeightMachine> wm)
        : weightMachine(std::move(wm)) {}

    double getWeightInKg() override {
        double weightInPound = weightMachine->getWeightInPound();
        // Convert pounds to kilograms
        double weightInKg = weightInPound * 0.45;
        return weightInKg;
    }
};
