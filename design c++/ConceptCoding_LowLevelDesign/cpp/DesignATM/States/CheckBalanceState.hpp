#pragma once
#include "ATMState.hpp"

class CheckBalanceState : public ATMState {
public:
    void displayBalance(ATM* atm, std::shared_ptr&lt;Card&gt; card) override;
    void exit(ATM* atm) override;
    void returnCard() override {
        std::cout &lt;&lt; "Please collect your card" &lt;&lt; std::endl;
    }
};
