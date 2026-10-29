#pragma once
#include "State.hpp"
#include "../VendingMachine.hpp"
#include <iostream>
#include <stdexcept>

/**
 * Selection State - User is selecting a product
 * 
 * In this state, user chooses a product. Machine validates payment.
 */
class SelectionState : public State {
public:
    SelectionState() {
        std::cout << "Currently Vending machine is in SelectionState" << std::endl;
    }

    void clickOnInsertCoinButton(VendingMachine& machine) override {
        throw std::runtime_error("You cannot click on insert coin button in Selection state");
    }

    void clickOnStartProductSelectionButton(VendingMachine& machine) override {
        // Already in selection mode
        return;
    }

    void insertCoin(VendingMachine& machine, Coin coin) override {
        throw std::runtime_error("You cannot insert coin in selection state");
    }

    void chooseProduct(VendingMachine& machine, int codeNumber) override;

    int getChange(int returnExtraMoney) override {
        std::cout << "Returned the change in the Coin Dispense Tray: " << returnExtraMoney << std::endl;
        return returnExtraMoney;
    }

    std::vector<Coin> refundFullMoney(VendingMachine& machine) override;

    std::shared_ptr<Item> dispenseProduct(VendingMachine& machine, int codeNumber) override {
        throw std::runtime_error("Product cannot be dispensed in Selection state");
    }

    void updateInventory(VendingMachine& machine, std::shared_ptr<Item> item, int codeNumber) override {
        throw std::runtime_error("Inventory cannot be updated in Selection state");
    }
};
