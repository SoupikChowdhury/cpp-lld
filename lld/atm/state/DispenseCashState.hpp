#ifndef DISPENSE_CASH_STATE_HPP
#define DISPENSE_CASH_STATE_HPP

#include <iostream>
#include <memory>
#include "ATMState.hpp"
#include "../service/ATMMachine.hpp"
#include "../cor/CashDispenser.hpp"
#include "../cor/CashDispenserChainBuilder.hpp"

class IdleState;

class DispenseCashState : public ATMState {
private:
    ATMMachine* atmMachine;
    CashDispenser* chain;

public:
    explicit DispenseCashState(ATMMachine* machine) 
        : atmMachine(machine), chain(CashDispenserChainBuilder::buildChain()) {}

    ~DispenseCashState() {
        // In production, properly clean up the chain
    }

    void insertCard(std::shared_ptr<Card> card) override {
        std::cout << "Transaction in progress. Cannot insert another card." << std::endl;
    }

    void enterPin(const std::string& pin) override {
        std::cout << "Already authenticated." << std::endl;
    }

    void selectOption(const std::string& option) override {
        std::cout << "Option already selected." << std::endl;
    }

    void dispenseCash(int amount) override;

    void ejectCard() override;

    ATMStatus getStatus() const override {
        return ATMStatus::DISPENSE_CASH;
    }
};

#endif // DISPENSE_CASH_STATE_HPP
