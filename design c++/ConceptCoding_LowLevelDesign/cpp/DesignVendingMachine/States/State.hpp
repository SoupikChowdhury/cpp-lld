#pragma once
#include "../Coin.hpp"
#include "../Item.hpp"
#include &lt;vector&gt;
#include &lt;memory&gt;

// Forward declaration
class VendingMachine;

/**
 * State Pattern - Abstract State Interface
 * 
 * Defines all operations that can be performed on the vending machine.
 * Each state implements these methods differently based on current state.
 */
class State {
public:
    virtual ~State() = default;

    virtual void clickOnInsertCoinButton(VendingMachine&amp; machine) = 0;
    virtual void clickOnStartProductSelectionButton(VendingMachine&amp; machine) = 0;
    virtual void insertCoin(VendingMachine&amp; machine, Coin coin) = 0;
    virtual void chooseProduct(VendingMachine&amp; machine, int codeNumber) = 0;
    virtual int getChange(int returnChangeMoney) = 0;
    virtual std::shared_ptr&lt;Item&gt; dispenseProduct(VendingMachine&amp; machine, int codeNumber) = 0;
    virtual std::vector&lt;Coin&gt; refundFullMoney(VendingMachine&amp; machine) = 0;
    virtual void updateInventory(VendingMachine&amp; machine, std::shared_ptr&lt;Item&gt; item, int codeNumber) = 0;
};
