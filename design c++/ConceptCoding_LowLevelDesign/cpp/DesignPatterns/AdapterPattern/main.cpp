#include "WeightMachineAdapterImpl.hpp"
#include "WeightMachineForBabies.hpp"
#include <iostream>
#include <memory>

/**
 * Adapter Pattern Demo
 * 
 * Client wants weight in Kg, but machine returns Pounds.
 * Adapter converts the interface.
 */
int main() {
    // Create adapter with the adaptee (WeightMachineForBabies)
    auto adapter = std::make_unique<WeightMachineAdapterImpl>(
        std::make_unique<WeightMachineForBabies>()
    );

    // Client uses Kg interface
    std::cout << "Weight in Kg: " << adapter->getWeightInKg() << std::endl;

    return 0;
}
