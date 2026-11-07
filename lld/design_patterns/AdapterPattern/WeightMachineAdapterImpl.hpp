#pragma once
#include "WeightMachineAdapter.hpp"
#include "WeightMachine.hpp"
#include <memory>
#include <stdexcept>

/**
 * Adapter - Converts Pound interface to Kg interface.
 * 
 * This adapter wraps a WeightMachine (returns pounds)
 * and provides a getWeightInKg() method for the client.
 */
class WeightMachineAdapterImpl : public WeightMachineAdapter {
private:
    static constexpr double KG_PER_POUND = 0.45359237;

    std::unique_ptr<WeightMachine> weightMachine;

public:
    explicit WeightMachineAdapterImpl(std::unique_ptr<WeightMachine> wm)
        : weightMachine(std::move(wm)) {
        if (!weightMachine) {
            throw std::invalid_argument("The adapter needs a weight machine");
        }
    }

    double getWeightInKg() override {
        double weightInPound = weightMachine->getWeightInPound();
        // Convert pounds to kilograms
        double weightInKg = weightInPound * KG_PER_POUND;
        return weightInKg;
    }
};
