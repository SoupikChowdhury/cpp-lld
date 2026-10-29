#pragma once
#include "ATMState.hpp"

class CheckBalanceState : public ATMState {
public:
    void displayBalance(ATM* atm, std::shared_ptr<Card> card) override;
    void exit(ATM* atm) override;
    void returnCard() override {
        std::cout << "Please collect your card" << std::endl;
    }
};
