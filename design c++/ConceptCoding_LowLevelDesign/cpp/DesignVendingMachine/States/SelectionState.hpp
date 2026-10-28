#pragma once
#include "State.hpp"
#include "../VendingMachine.hpp"
#include &lt;iostream&gt;
#include &lt;stdexcept&gt;

/**
 * Selection State - User is selecting a product
 * 
 * In this state, user chooses a product. Machine validates payment.
 */
class SelectionState : public State {
public:
    SelectionState() {
        std::cout &lt;&lt; "Currently Vending machine is in SelectionState" &lt;&lt; std::endl;
    }

    void clickOnInsertCoinButton(VendingMachine&amp; machine) override {
        throw std::runtime_error("You cannot click on insert coin button in Selection state");
    }

    void clickOnStartProductSelectionButton(VendingMachine&amp; machine) override {
        // Already in selection mode
        return;
    }

    void insertCoin(VendingMachine&amp; machine, Coin coin) override {
        throw std::runtime_error("You cannot insert coin in selection state");
    }

    void chooseProduct(VendingMachine&amp; machine, int codeNumber) override;

    int getChange(int returnExtraMoney) override {
        std::cout &lt;&lt; "Returned the change in the Coin Dispense Tray: " &lt;&lt; returnExtraMoney &lt;&lt; std::endl;
        return returnExtraMoney;
    }

    std::vector&lt;Coin&gt; refundFullMoney(VendingMachine&amp; machine) override;

    std::shared_ptr&lt;Item&gt; dispenseProduct(VendingMachine&amp; machine, int codeNumber) override {
        throw std::runtime_error("Product cannot be dispensed in Selection state");
    }

    void updateInventory(VendingMachine&amp; machine, std::shared_ptr&lt;Item&gt; item, int codeNumber) override {
        throw std::runtime_error("Inventory cannot be updated in Selection state");
    }
};
