#ifndef AUTHENTICATED_STATE_HPP
#define AUTHENTICATED_STATE_HPP

#include <iostream>
#include <memory>
#include "ATMState.hpp"
#include "../service/ATMMachine.hpp"

class DispenseCashState;
class IdleState;

class AuthenticatedState : public ATMState {
private:
    ATMMachine* atmMachine;

public:
    explicit AuthenticatedState(ATMMachine* machine) : atmMachine(machine) {}

    void insertCard(std::shared_ptr<Card> card) override {
        std::cout << "Card already inserted." << std::endl;
    }

    void enterPin(const std::string& pin) override {
        std::cout << "Already authenticated." << std::endl;
    }

    void selectOption(const std::string& option) override;

    void dispenseCash(int amount) override {
        std::cout << "Select an option first." << std::endl;
    }

    void ejectCard() override;

    ATMStatus getStatus() const override {
        return ATMStatus::AUTHENTICATED;
    }
};

#endif // AUTHENTICATED_STATE_HPP
