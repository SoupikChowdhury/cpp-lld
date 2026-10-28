#include "WeightMachineAdapterImpl.hpp"
#include "WeightMachineForBabies.hpp"
#include &lt;iostream&gt;
#include &lt;memory&gt;

/**
 * Adapter Pattern Demo
 * 
 * Client wants weight in Kg, but machine returns Pounds.
 * Adapter converts the interface.
 */
int main() {
    // Create adapter with the adaptee (WeightMachineForBabies)
    auto adapter = std::make_unique&lt;WeightMachineAdapterImpl&gt;(
        std::make_unique&lt;WeightMachineForBabies&gt;()
    );

    // Client uses Kg interface
    std::cout &lt;&lt; "Weight in Kg: " &lt;&lt; adapter-&gt;getWeightInKg() &lt;&lt; std::endl;

    return 0;
}
