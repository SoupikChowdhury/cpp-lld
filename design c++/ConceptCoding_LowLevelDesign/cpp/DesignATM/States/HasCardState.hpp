#pragma once
#include "ATMState.hpp"

class HasCardState : public ATMState {
public:
    HasCardState() {
        std::cout &lt;&lt; "Enter your card PIN number" &lt;&lt; std::endl;
    }

    void authenticatePin(ATM* atm, std::shared_ptr&lt;Card&gt; card, int pin) override;
    void exit(ATM* atm) override;
    void returnCard() override {
        std::cout &lt;&lt; "Please collect your card" &lt;&lt; std::endl;
    }
};
