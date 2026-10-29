#pragma once
#include "ATMState.hpp"

class HasCardState : public ATMState {
public:
    HasCardState() {
        std::cout << "Enter your card PIN number" << std::endl;
    }

    void authenticatePin(ATM* atm, std::shared_ptr<Card> card, int pin) override;
    void exit(ATM* atm) override;
    void returnCard() override {
        std::cout << "Please collect your card" << std::endl;
    }
};
