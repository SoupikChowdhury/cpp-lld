#pragma once
#include "ATMState.hpp"

class CashWithdrawalState : public ATMState {
public:
    CashWithdrawalState() {
        std::cout << "Please enter the withdrawal amount" << std::endl;
    }

    void cashWithdrawal(ATM* atm, std::shared_ptr<Card> card, int withdrawAmount) override;
    void exit(ATM* atm) override;
    void returnCard() override {
        std::cout << "Please collect your card" << std::endl;
    }
};
