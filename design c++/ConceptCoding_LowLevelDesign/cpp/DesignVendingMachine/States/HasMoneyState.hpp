#pragma once
#include "State.hpp"
#include "../VendingMachine.hpp"
#include &lt;iostream&gt;
#include &lt;stdexcept&gt;

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
        std::cout &lt;&lt; "Currently Vending machine is in HasMoneyState" &lt;&lt; std::endl;
    }

    void clickOnInsertCoinButton(VendingMachine&amp; machine) override {
        // Already in money insertion mode
        return;
    }

    void clickOnStartProductSelectionButton(VendingMachine&amp; machine) override;

    void insertCoin(VendingMachine&amp; machine, Coin coin) override {
        std::cout &lt;&lt; "Accepted the coin: " &lt;&lt; getCoinValue(coin) &lt;&lt; std::endl;
        machine.addCoin(coin);
    }

    void chooseProduct(VendingMachine&amp; machine, int codeNumber) override {
        throw std::runtime_error("You need to click on start product selection button first");
    }

    int getChange(int returnChangeMoney) override {
        throw std::runtime_error("You cannot get change in hasMoney state");
    }

    std::shared_ptr&lt;Item&gt; dispenseProduct(VendingMachine&amp; machine, int codeNumber) override {
        throw std::runtime_error("Product cannot be dispensed in hasMoney state");
    }

    std::vector&lt;Coin&gt; refundFullMoney(VendingMachine&amp; machine) override;

    void updateInventory(VendingMachine&amp; machine, std::shared_ptr&lt;Item&gt; item, int codeNumber) override {
        throw std::runtime_error("You cannot update inventory in hasMoney state");
    }
};
