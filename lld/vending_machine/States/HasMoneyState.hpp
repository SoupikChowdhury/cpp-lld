#pragma once
#include "State.hpp"
#include "../VendingMachine.hpp"
#include <iostream>
#include <stdexcept>

// Forward declaration
class SelectionState;

/**
 * HasMoney State - User has inserted coins
 * 
 * In this state, user can insert more coins or proceed to selection.
 */
class HasMoneyState : public State {
public:
    HasMoneyState() {
        std::cout << "Currently Vending machine is in HasMoneyState" << std::endl;
    }

    void clickOnInsertCoinButton(VendingMachine& machine) override {
        // Already in money insertion mode
        return;
    }

    void clickOnStartProductSelectionButton(VendingMachine& machine) override;

    void insertCoin(VendingMachine& machine, Coin coin) override {
        std::cout << "Accepted the coin: " << getCoinValue(coin) << std::endl;
        machine.addCoin(coin);
    }

    void chooseProduct(VendingMachine& machine, int codeNumber) override {
        throw std::runtime_error("You need to click on start product selection button first");
    }

    int getChange(int returnChangeMoney) override {
        throw std::runtime_error("You cannot get change in hasMoney state");
    }

    std::shared_ptr<Item> dispenseProduct(VendingMachine& machine, int codeNumber) override {
        throw std::runtime_error("Product cannot be dispensed in hasMoney state");
    }

    std::vector<Coin> refundFullMoney(VendingMachine& machine) override;

    void updateInventory(VendingMachine& machine, std::shared_ptr<Item> item, int codeNumber) override {
        throw std::runtime_error("You cannot update inventory in hasMoney state");
    }
};
