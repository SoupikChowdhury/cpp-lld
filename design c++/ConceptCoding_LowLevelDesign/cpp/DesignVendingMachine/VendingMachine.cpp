#include "States/IdleState.hpp"
#include "States/HasMoneyState.hpp"
#include "States/SelectionState.hpp"
#include "States/DispenseState.hpp"
#include "VendingMachine.hpp"

// VendingMachine constructor
VendingMachine::VendingMachine() {
    currentState = std::make_shared&lt;IdleState&gt;();
    inventory = std::make_unique&lt;Inventory&gt;(10);
}

// IdleState transitions
void IdleState::clickOnInsertCoinButton(VendingMachine&amp; machine) {
    machine.setState(std::make_shared&lt;HasMoneyState&gt;());
}

// HasMoneyState transitions
void HasMoneyState::clickOnStartProductSelectionButton(VendingMachine&amp; machine) {
    machine.setState(std::make_shared&lt;SelectionState&gt;());
}

std::vector&lt;Coin&gt; HasMoneyState::refundFullMoney(VendingMachine&amp; machine) {
    std::cout &lt;&lt; "Returned the full amount back in the Coin Dispense Tray" &lt;&lt; std::endl;
    auto coins = machine.getCoinList();
    machine.setState(std::make_shared&lt;IdleState&gt;(machine));
    return coins;
}

// SelectionState logic
void SelectionState::chooseProduct(VendingMachine&amp; machine, int codeNumber) {
    // Get the item
    auto item = machine.getInventory()-&gt;getItem(codeNumber);

    // Calculate total paid amount
    int paidByUser = 0;
    for (const auto&amp; coin : machine.getCoinList()) {
        paidByUser += getCoinValue(coin);
    }

    // Compare with item price
    if (paidByUser &lt; item-&gt;getPrice()) {
        std::cout &lt;&lt; "Insufficient Amount. Product price: " &lt;&lt; item-&gt;getPrice() 
                  &lt;&lt; ", You paid: " &lt;&lt; paidByUser &lt;&lt; std::endl;
        refundFullMoney(machine);
        throw std::runtime_error("Insufficient amount");
    } else {
        if (paidByUser &gt; item-&gt;getPrice()) {
            getChange(paidByUser - item-&gt;getPrice());
        }
        machine.setState(std::make_shared&lt;DispenseState&gt;(machine, codeNumber));
    }
}

std::vector&lt;Coin&gt; SelectionState::refundFullMoney(VendingMachine&amp; machine) {
    std::cout &lt;&lt; "Returned the full amount back in the Coin Dispense Tray" &lt;&lt; std::endl;
    auto coins = machine.getCoinList();
    machine.setState(std::make_shared&lt;IdleState&gt;(machine));
    return coins;
}

// DispenseState logic
std::shared_ptr&lt;Item&gt; DispenseState::dispenseProduct(VendingMachine&amp; machine, int codeNumber) {
    std::cout &lt;&lt; "Product has been dispensed" &lt;&lt; std::endl;
    auto item = machine.getInventory()-&gt;getItem(codeNumber);
    machine.getInventory()-&gt;updateSoldOutItem(codeNumber);
    machine.setState(std::make_shared&lt;IdleState&gt;(machine));
    return item;
}
