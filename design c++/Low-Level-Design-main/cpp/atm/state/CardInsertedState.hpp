#ifndef CARD_INSERTED_STATE_HPP
#define CARD_INSERTED_STATE_HPP

#include <iostream>
#include <memory>
#include "ATMState.hpp"
#include "../service/ATMMachine.hpp"

class AuthenticatedState;
class IdleState;

class CardInsertedState : public ATMState {
private:
    ATMMachine* atmMachine;

public:
    explicit CardInsertedState(ATMMachine* machine) : atmMachine(machine) {}

    void insertCard(std::shared_ptr<Card> card) override {
        std::cout << "Card already inserted." << std::endl;
    }

    void enterPin(const std::string& pin) override;

    void selectOption(const std::string& option) override {
        std::cout << "Enter PIN first." << std::endl;
    }

    void dispenseCash(int amount) override {
        std::cout << "Enter PIN before dispensing." << std::endl;
    }

    void ejectCard() override;

    ATMStatus getStatus() const override {
        return ATMStatus::CARD_INSERTED;
    }
};

#endif // CARD_INSERTED_STATE_HPP
