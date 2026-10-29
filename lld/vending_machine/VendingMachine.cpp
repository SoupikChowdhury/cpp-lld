#include "States/IdleState.hpp"
#include "States/HasMoneyState.hpp"
#include "States/SelectionState.hpp"
#include "States/DispenseState.hpp"
#include "VendingMachine.hpp"

// VendingMachine constructor
VendingMachine::VendingMachine() {
    currentState = std::make_shared<IdleState>();
    inventory = std::make_unique<Inventory>(10);
}

// IdleState transitions
void IdleState::clickOnInsertCoinButton(VendingMachine& machine) {
    machine.setState(std::make_shared<HasMoneyState>());
}

// HasMoneyState transitions
void HasMoneyState::clickOnStartProductSelectionButton(VendingMachine& machine) {
    machine.setState(std::make_shared<SelectionState>());
}

std::vector<Coin> HasMoneyState::refundFullMoney(VendingMachine& machine) {
    std::cout << "Returned the full amount back in the Coin Dispense Tray" << std::endl;
    auto coins = machine.getCoinList();
    machine.setState(std::make_shared<IdleState>(machine));
    return coins;
}

// SelectionState logic
void SelectionState::chooseProduct(VendingMachine& machine, int codeNumber) {
    // Get the item
    auto item = machine.getInventory()->getItem(codeNumber);

    // Calculate total paid amount
    int paidByUser = 0;
    for (const auto& coin : machine.getCoinList()) {
        paidByUser += getCoinValue(coin);
    }

    // Compare with item price
    if (paidByUser < item->getPrice()) {
        std::cout << "Insufficient Amount. Product price: " << item->getPrice() 
                  << ", You paid: " << paidByUser << std::endl;
        refundFullMoney(machine);
        throw std::runtime_error("Insufficient amount");
    } else {
        if (paidByUser > item->getPrice()) {
            getChange(paidByUser - item->getPrice());
        }
        machine.setState(std::make_shared<DispenseState>(machine, codeNumber));
    }
}

std::vector<Coin> SelectionState::refundFullMoney(VendingMachine& machine) {
    std::cout << "Returned the full amount back in the Coin Dispense Tray" << std::endl;
    auto coins = machine.getCoinList();
    machine.setState(std::make_shared<IdleState>(machine));
    return coins;
}

// DispenseState logic
std::shared_ptr<Item> DispenseState::dispenseProduct(VendingMachine& machine, int codeNumber) {
    std::cout << "Product has been dispensed" << std::endl;
    auto item = machine.getInventory()->getItem(codeNumber);
    machine.getInventory()->updateSoldOutItem(codeNumber);
    machine.setState(std::make_shared<IdleState>(machine));
    return item;
}
