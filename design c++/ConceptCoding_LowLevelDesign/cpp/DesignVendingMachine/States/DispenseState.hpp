#pragma once
#include "State.hpp"
#include "../VendingMachine.hpp"
#include &lt;iostream&gt;
#include &lt;stdexcept&gt;

/**
 * Dispense State - Product is being dispensed
 * 
 * This state handles product dispensing and returns to idle.
 */
class DispenseState : public State {
public:
    DispenseState(VendingMachine&amp; machine, int codeNumber) {
        std::cout &lt;&lt; "Currently Vending machine is in DispenseState" &lt;&lt; std::endl;
        dispenseProduct(machine, codeNumber);
    }

    void clickOnInsertCoinButton(VendingMachine&amp; machine) override {
        throw std::runtime_error("Insert coin button cannot be clicked in Dispense state");
    }

    void clickOnStartProductSelectionButton(VendingMachine&amp; machine) override {
        throw std::runtime_error("Product selection button cannot be clicked in Dispense state");
    }

    void insertCoin(VendingMachine&amp; machine, Coin coin) override {
        throw std::runtime_error("Coin cannot be inserted in Dispense state");
    }

    void chooseProduct(VendingMachine&amp; machine, int codeNumber) override {
        throw std::runtime_error("Product cannot be chosen in Dispense state");
    }

    int getChange(int returnChangeMoney) override {
        throw std::runtime_error("Change cannot be returned in Dispense state");
    }

    std::vector&lt;Coin&gt; refundFullMoney(VendingMachine&amp; machine) override {
        throw std::runtime_error("Refund cannot happen in Dispense state");
    }

    std::shared_ptr&lt;Item&gt; dispenseProduct(VendingMachine&amp; machine, int codeNumber) override;

    void updateInventory(VendingMachine&amp; machine, std::shared_ptr&lt;Item&gt; item, int codeNumber) override {
        throw std::runtime_error("Inventory cannot be updated in Dispense state");
    }
};
