#ifndef IDLE_STATE_HPP
#define IDLE_STATE_HPP

#include <iostream>
#include <memory>
#include "ATMState.hpp"
#include "../service/ATMMachine.hpp"

class CardInsertedState;

class IdleState : public ATMState {
private:
    ATMMachine* atmMachine;

public:
    explicit IdleState(ATMMachine* machine) : atmMachine(machine) {}

    void insertCard(std::shared_ptr<Card> card) override;

    void enterPin(const std::string& pin) override {
        std::cout << "No card inserted." << std::endl;
    }

    void selectOption(const std::string& option) override {
        std::cout << "No card inserted." << std::endl;
    }

    void dispenseCash(int amount) override {
        std::cout << "No card inserted." << std::endl;
    }

    void ejectCard() override {
        std::cout << "No card to eject." << std::endl;
    }

    ATMStatus getStatus() const override {
        return ATMStatus::IDLE;
    }
};

#endif // IDLE_STATE_HPP
