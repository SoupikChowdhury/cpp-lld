#include "VendingMachine.hpp"
#include "VendingMachine.cpp"  // Include implementation
#include "States/State.hpp"
#include &lt;iostream&gt;

void fillUpInventory(VendingMachine&amp; vendingMachine) {
    auto&amp; slots = vendingMachine.getInventory()-&gt;getInventory();
    
    for (size_t i = 0; i &lt; slots.size(); ++i) {
        auto newItem = std::make_shared&lt;Item&gt;();
        
        if (i &lt; 3) {
            newItem-&gt;setType(ItemType::COKE);
            newItem-&gt;setPrice(12);
        } else if (i &lt; 5) {
            newItem-&gt;setType(ItemType::PEPSI);
            newItem-&gt;setPrice(9);
        } else if (i &lt; 7) {
            newItem-&gt;setType(ItemType::JUICE);
            newItem-&gt;setPrice(13);
        } else {
            newItem-&gt;setType(ItemType::SODA);
            newItem-&gt;setPrice(7);
        }
        
        slots[i].setItem(newItem);
        slots[i].setSoldOut(false);
    }
}

void displayInventory(VendingMachine&amp; vendingMachine) {
    auto&amp; slots = vendingMachine.getInventory()-&gt;getInventory();
    
    std::cout &lt;&lt; "\n=== Inventory ===" &lt;&lt; std::endl;
    for (const auto&amp; slot : slots) {
        std::cout &lt;&lt; "Code: " &lt;&lt; slot.getCode()
                  &lt;&lt; " | Item: " &lt;&lt; itemTypeToString(slot.getItem()-&gt;getType())
                  &lt;&lt; " | Price: " &lt;&lt; slot.getItem()-&gt;getPrice()
                  &lt;&lt; " | Available: " &lt;&lt; (slot.isSoldOut() ? "No" : "Yes")
                  &lt;&lt; std::endl;
    }
    std::cout &lt;&lt; std::endl;
}

int main() {
    try {
        VendingMachine vendingMachine;

        std::cout &lt;&lt; "=== Vending Machine Demo ===" &lt;&lt; std::endl;
        std::cout &lt;&lt; "\n--- Filling up inventory ---" &lt;&lt; std::endl;
        fillUpInventory(vendingMachine);
        displayInventory(vendingMachine);

        // User interaction flow
        std::cout &lt;&lt; "--- User clicks Insert Coin Button ---" &lt;&lt; std::endl;
        vendingMachine.getState()-&gt;clickOnInsertCoinButton(vendingMachine);

        std::cout &lt;&lt; "\n--- User inserts coins ---" &lt;&lt; std::endl;
        vendingMachine.getState()-&gt;insertCoin(vendingMachine, Coin::NICKEL);
        vendingMachine.getState()-&gt;insertCoin(vendingMachine, Coin::QUARTER);

        std::cout &lt;&lt; "\n--- User clicks Product Selection Button ---" &lt;&lt; std::endl;
        vendingMachine.getState()-&gt;clickOnStartProductSelectionButton(vendingMachine);

        std::cout &lt;&lt; "\n--- User selects product 102 (PEPSI - 9 cents) ---" &lt;&lt; std::endl;
        // User paid 30 cents (NICKEL=5 + QUARTER=25), PEPSI costs 9
        vendingMachine.getState()-&gt;chooseProduct(vendingMachine, 104);

        displayInventory(vendingMachine);

    } catch (const std::exception&amp; e) {
        std::cerr &lt;&lt; "Error: " &lt;&lt; e.what() &lt;&lt; std::endl;
    }

    return 0;
}
