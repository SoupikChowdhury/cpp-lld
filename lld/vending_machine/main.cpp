#include "VendingMachine.hpp"
#include "States/State.hpp"
#include <iostream>

void fillUpInventory(VendingMachine& vendingMachine) {
    auto& slots = vendingMachine.getInventory()->getInventory();
    
    for (size_t i = 0; i < slots.size(); ++i) {
        auto newItem = std::make_shared<Item>();
        
        if (i < 3) {
            newItem->setType(ItemType::COKE);
            newItem->setPrice(12);
        } else if (i < 5) {
            newItem->setType(ItemType::PEPSI);
            newItem->setPrice(9);
        } else if (i < 7) {
            newItem->setType(ItemType::JUICE);
            newItem->setPrice(13);
        } else {
            newItem->setType(ItemType::SODA);
            newItem->setPrice(7);
        }
        
        slots[i].setItem(newItem);
        slots[i].setSoldOut(false);
    }
}

void displayInventory(VendingMachine& vendingMachine) {
    auto& slots = vendingMachine.getInventory()->getInventory();
    
    std::cout << "\n=== Inventory ===" << std::endl;
    for (const auto& slot : slots) {
        std::cout << "Code: " << slot.getCode()
                  << " | Item: " << itemTypeToString(slot.getItem()->getType())
                  << " | Price: " << slot.getItem()->getPrice()
                  << " | Available: " << (slot.isSoldOut() ? "No" : "Yes")
                  << std::endl;
    }
    std::cout << std::endl;
}

int main() {
    try {
        VendingMachine vendingMachine;

        std::cout << "=== Vending Machine Demo ===" << std::endl;
        std::cout << "\n--- Filling up inventory ---" << std::endl;
        fillUpInventory(vendingMachine);
        displayInventory(vendingMachine);

        // User interaction flow
        std::cout << "--- User clicks Insert Coin Button ---" << std::endl;
        vendingMachine.getState()->clickOnInsertCoinButton(vendingMachine);

        std::cout << "\n--- User inserts coins ---" << std::endl;
        vendingMachine.getState()->insertCoin(vendingMachine, Coin::NICKEL);
        vendingMachine.getState()->insertCoin(vendingMachine, Coin::QUARTER);

        std::cout << "\n--- User clicks Product Selection Button ---" << std::endl;
        vendingMachine.getState()->clickOnStartProductSelectionButton(vendingMachine);

        std::cout << "\n--- User selects product 102 (PEPSI - 9 cents) ---" << std::endl;
        // User paid 30 cents (NICKEL=5 + QUARTER=25), PEPSI costs 9
        vendingMachine.getState()->chooseProduct(vendingMachine, 104);

        displayInventory(vendingMachine);

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
