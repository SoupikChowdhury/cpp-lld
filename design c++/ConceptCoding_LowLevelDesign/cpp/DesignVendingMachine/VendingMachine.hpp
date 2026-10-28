#pragma once
#include "State.hpp"
#include "../Coin.hpp"
#include "../Inventory.hpp"
#include &lt;vector&gt;
#include &lt;memory&gt;

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
    std::shared_ptr&lt;State&gt; currentState;
    std::unique_ptr&lt;Inventory&gt; inventory;
    std::vector&lt;Coin&gt; coinList;

public:
    VendingMachine();  // Implementation in VendingMachine.cpp

    std::shared_ptr&lt;State&gt; getState() const { return currentState; }
    void setState(std::shared_ptr&lt;State&gt; state) { currentState = state; }

    Inventory* getInventory() { return inventory.get(); }

    std::vector&lt;Coin&gt;&amp; getCoinList() { return coinList; }
    void setCoinList(const std::vector&lt;Coin&gt;&amp; coins) { coinList = coins; }
    void clearCoinList() { coinList.clear(); }
    void addCoin(Coin coin) { coinList.push_back(coin); }
};
