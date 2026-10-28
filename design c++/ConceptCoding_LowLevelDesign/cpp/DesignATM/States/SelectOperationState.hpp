#pragma once
#include "ATMState.hpp"

class SelectOperationState : public ATMState {
public:
    SelectOperationState() {
        std::cout &lt;&lt; "Please select the operation:" &lt;&lt; std::endl;
        std::cout &lt;&lt; "1. CASH_WITHDRAWAL" &lt;&lt; std::endl;
        std::cout &lt;&lt; "2. BALANCE_CHECK" &lt;&lt; std::endl;
    }

    void selectOperation(ATM* atm, std::shared_ptr&lt;Card&gt; card, TransactionType txnType) override;
    void exit(ATM* atm) override;
    void returnCard() override {
        std::cout &lt;&lt; "Please collect your card" &lt;&lt; std::endl;
    }
};
