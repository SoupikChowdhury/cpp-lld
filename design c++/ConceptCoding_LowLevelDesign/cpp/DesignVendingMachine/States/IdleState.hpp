#pragma once
#include "State.hpp"
#include "../VendingMachine.hpp"
#include <iostream>
#include <stdexcept>

// Forward declaration
class HasMoneyState;

/**
 * Idle State - Initial state of vending machine
 * 
 * In this state, machine waits for user to insert coins.
 * Only clickOnInsertCoinButton and updateInventory are valid.
 */
class IdleState : public State {
public:
    IdleState() {
        std::cout << "Currently Vending machine is in IdleState" << std::endl;
    }

    explicit IdleState(VendingMachine& machine) {
        std::cout << "Currently Vending machine is in IdleState" << std::endl;
        machine.clearCoinList();
    }

    void clickOnInsertCoinButton(VendingMachine& machine) override;

    void clickOnStartProductSelectionButton(VendingMachine& machine) override {
        throw std::runtime_error("First you need to click on insert coin button");
    }

    void insertCoin(VendingMachine& machine, Coin coin) override {
        throw std::runtime_error("You cannot insert coin in idle state");
    }

    void chooseProduct(VendingMachine& machine, int codeNumber) override {
        throw std::runtime_error("You cannot choose product in idle state");
    }

    int getChange(int returnChangeMoney) override {
        throw std::runtime_error("You cannot get change in idle state");
    }

    std::vector<Coin> refundFullMoney(VendingMachine& machine) override {
        throw std::runtime_error("You cannot get refund in idle state");
    }

    std::shared_ptr<Item> dispenseProduct(VendingMachine& machine, int codeNumber) override {
        throw std::runtime_error("Product cannot be dispensed in idle state");
    }

    void updateInventory(VendingMachine& machine, std::shared_ptr<Item> item, int codeNumber) override {
        machine.getInventory()->addItem(item, codeNumber);
    }
};
