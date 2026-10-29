#pragma once
#include "State.hpp"
#include "../VendingMachine.hpp"
#include <iostream>
#include <stdexcept>

/**
 * Dispense State - Product is being dispensed
 * 
 * This state handles product dispensing and returns to idle.
 */
class DispenseState : public State {
public:
    DispenseState(VendingMachine& machine, int codeNumber) {
        std::cout << "Currently Vending machine is in DispenseState" << std::endl;
        dispenseProduct(machine, codeNumber);
    }

    void clickOnInsertCoinButton(VendingMachine& machine) override {
        throw std::runtime_error("Insert coin button cannot be clicked in Dispense state");
    }

    void clickOnStartProductSelectionButton(VendingMachine& machine) override {
        throw std::runtime_error("Product selection button cannot be clicked in Dispense state");
    }

    void insertCoin(VendingMachine& machine, Coin coin) override {
        throw std::runtime_error("Coin cannot be inserted in Dispense state");
    }

    void chooseProduct(VendingMachine& machine, int codeNumber) override {
        throw std::runtime_error("Product cannot be chosen in Dispense state");
    }

    int getChange(int returnChangeMoney) override {
        throw std::runtime_error("Change cannot be returned in Dispense state");
    }

    std::vector<Coin> refundFullMoney(VendingMachine& machine) override {
        throw std::runtime_error("Refund cannot happen in Dispense state");
    }

    std::shared_ptr<Item> dispenseProduct(VendingMachine& machine, int codeNumber) override;

    void updateInventory(VendingMachine& machine, std::shared_ptr<Item> item, int codeNumber) override {
        throw std::runtime_error("Inventory cannot be updated in Dispense state");
    }
};
