#pragma once
#include "States/State.hpp"
#include "Coin.hpp"
#include "Inventory.hpp"
#include <vector>
#include <memory>

// Forward declarations for state implementations
class IdleState;
class HasMoneyState;
class SelectionState;
class DispenseState;

/**
 * Vending Machine - Context class for State Pattern
 * 
 * Maintains current state and delegates operations to the current state.
 */
class VendingMachine {
private:
    std::shared_ptr<State> currentState;
    std::unique_ptr<Inventory> inventory;
    std::vector<Coin> coinList;

public:
    VendingMachine();  // Implementation in VendingMachine.cpp

    std::shared_ptr<State> getState() const { return currentState; }
    void setState(std::shared_ptr<State> state) { currentState = state; }

    Inventory* getInventory() { return inventory.get(); }

    std::vector<Coin>& getCoinList() { return coinList; }
    void setCoinList(const std::vector<Coin>& coins) { coinList = coins; }
    void clearCoinList() { coinList.clear(); }
    void addCoin(Coin coin) { coinList.push_back(coin); }
};
