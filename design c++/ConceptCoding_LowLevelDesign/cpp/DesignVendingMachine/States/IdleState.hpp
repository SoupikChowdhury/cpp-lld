#pragma once
#include "State.hpp"
#include "../VendingMachine.hpp"
#include &lt;iostream&gt;
#include &lt;stdexcept&gt;

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
        std::cout &lt;&lt; "Currently Vending machine is in IdleState" &lt;&lt; std::endl;
    }

    explicit IdleState(VendingMachine&amp; machine) {
        std::cout &lt;&lt; "Currently Vending machine is in IdleState" &lt;&lt; std::endl;
        machine.clearCoinList();
    }

    void clickOnInsertCoinButton(VendingMachine&amp; machine) override;

    void clickOnStartProductSelectionButton(VendingMachine&amp; machine) override {
        throw std::runtime_error("First you need to click on insert coin button");
    }

    void insertCoin(VendingMachine&amp; machine, Coin coin) override {
        throw std::runtime_error("You cannot insert coin in idle state");
    }

    void chooseProduct(VendingMachine&amp; machine, int codeNumber) override {
        throw std::runtime_error("You cannot choose product in idle state");
    }

    int getChange(int returnChangeMoney) override {
        throw std::runtime_error("You cannot get change in idle state");
    }

    std::vector&lt;Coin&gt; refundFullMoney(VendingMachine&amp; machine) override {
        throw std::runtime_error("You cannot get refund in idle state");
    }

    std::shared_ptr&lt;Item&gt; dispenseProduct(VendingMachine&amp; machine, int codeNumber) override {
        throw std::runtime_error("Product cannot be dispensed in idle state");
    }

    void updateInventory(VendingMachine&amp; machine, std::shared_ptr&lt;Item&gt; item, int codeNumber) override {
        machine.getInventory()-&gt;addItem(item, codeNumber);
    }
};
