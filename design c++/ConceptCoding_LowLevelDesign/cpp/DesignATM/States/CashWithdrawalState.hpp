#pragma once
#include "ATMState.hpp"

class CashWithdrawalState : public ATMState {
public:
    CashWithdrawalState() {
        std::cout &lt;&lt; "Please enter the withdrawal amount" &lt;&lt; std::endl;
    }

    void cashWithdrawal(ATM* atm, std::shared_ptr&lt;Card&gt; card, int withdrawAmount) override;
    void exit(ATM* atm) override;
    void returnCard() override {
        std::cout &lt;&lt; "Please collect your card" &lt;&lt; std::endl;
    }
};
