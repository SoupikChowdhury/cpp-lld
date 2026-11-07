#pragma once

/**
 * Adapter Pattern - Target Interface
 *
 * The interface that the client expects to work with (weight in Kg).
 */
class WeightMachineAdapter {
public:
    virtual ~WeightMachineAdapter() = default;
    virtual double getWeightInKg() = 0;
};
