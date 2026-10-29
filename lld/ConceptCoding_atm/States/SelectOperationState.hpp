#pragma once
#include "ATMState.hpp"

class SelectOperationState : public ATMState {
public:
    SelectOperationState() {
        std::cout << "Please select the operation:" << std::endl;
        std::cout << "1. CASH_WITHDRAWAL" << std::endl;
        std::cout << "2. BALANCE_CHECK" << std::endl;
    }

    void selectOperation(ATM* atm, std::shared_ptr<Card> card, TransactionType txnType) override;
    void exit(ATM* atm) override;
    void returnCard() override {
        std::cout << "Please collect your card" << std::endl;
    }
};
