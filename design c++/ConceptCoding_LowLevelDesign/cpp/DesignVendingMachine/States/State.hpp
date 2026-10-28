#pragma once
#include "../Coin.hpp"
#include "../Item.hpp"
#include <vector>
#include <memory>

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

    virtual void clickOnInsertCoinButton(VendingMachine& machine) = 0;
    virtual void clickOnStartProductSelectionButton(VendingMachine& machine) = 0;
    virtual void insertCoin(VendingMachine& machine, Coin coin) = 0;
    virtual void chooseProduct(VendingMachine& machine, int codeNumber) = 0;
    virtual int getChange(int returnChangeMoney) = 0;
    virtual std::shared_ptr<Item> dispenseProduct(VendingMachine& machine, int codeNumber) = 0;
    virtual std::vector<Coin> refundFullMoney(VendingMachine& machine) = 0;
    virtual void updateInventory(VendingMachine& machine, std::shared_ptr<Item> item, int codeNumber) = 0;
};
