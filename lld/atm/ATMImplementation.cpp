// ATM Machine - State Implementations
// This file contains the implementation of state transition methods

#include "state/IdleState.hpp"
#include "state/CardInsertedState.hpp"
#include "state/AuthenticatedState.hpp"
#include "state/DispenseCashState.hpp"
#include "service/ATMMachine.hpp"
#include "factory/ATMStateFactory.hpp"

// ATMMachine constructor
ATMMachine::ATMMachine(const std::string& atmId, std::shared_ptr<ATMRepository> atmRepository)
    : atmRepository(atmRepository), currentCard(nullptr) {
    atm = atmRepository->getById(atmId);
    if (!atm) {
        throw std::runtime_error("ATM not found");
    }
    state = ATMStateFactory::getState(atm->getStatus(), this);
}

// IdleState implementations
void IdleState::insertCard(std::shared_ptr<Card> card) {
    atmMachine->setCurrentCard(card);
    std::cout << "Card inserted." << std::endl;
    atmMachine->setState(std::make_unique<CardInsertedState>(atmMachine));
}

// CardInsertedState implementations
void CardInsertedState::enterPin(const std::string& pin) {
    if (atmMachine->getCurrentCard()->getPin() == pin) {
        std::cout << "PIN correct. Authenticated." << std::endl;
        atmMachine->setState(std::make_unique<AuthenticatedState>(atmMachine));
    } else {
        std::cout << "Invalid PIN." << std::endl;
    }
}

void CardInsertedState::ejectCard() {
    atmMachine->setCurrentCard(nullptr);
    std::cout << "Card ejected." << std::endl;
    atmMachine->setState(std::make_unique<IdleState>(atmMachine));
}

// AuthenticatedState implementations
void AuthenticatedState::selectOption(const std::string& option) {
    std::cout << "Option selected: Withdrawal." << std::endl;
    atmMachine->setState(std::make_unique<DispenseCashState>(atmMachine));
}

void AuthenticatedState::ejectCard() {
    atmMachine->setCurrentCard(nullptr);
    std::cout << "Card ejected." << std::endl;
    atmMachine->setState(std::make_unique<IdleState>(atmMachine));
}

// DispenseCashState implementations
void DispenseCashState::dispenseCash(int amount) {
    double atmBalance = atmMachine->getAtm()->getCashAvailable();
    double accountBalance = atmMachine->getCurrentCard()->getAccount()->getBalance();

    if (amount > atmBalance) {
        std::cout << "ATM has insufficient cash. Cannot dispense " << amount << std::endl;
        ejectCard();
        return;
    }

    if (amount > accountBalance) {
        std::cout << "Insufficient account balance." << std::endl;
        ejectCard();
        return;
    }

    // Check if note combination is possible
    if (chain->canDispense(atmMachine->getAtm().get(), amount)) {
        int amountToDispense = amount;
        chain->dispense(atmMachine->getAtm().get(), amountToDispense);

        // Deduct from account balance
        atmMachine->getCurrentCard()->getAccount()->setBalance(accountBalance - amount);
        std::cout << "Cash dispensed successfully. New balance: " 
                  << atmMachine->getCurrentCard()->getAccount()->getBalance() << std::endl;
        ejectCard();
    } else {
        std::cout << "Cannot dispense exact amount with available notes." << std::endl;
        ejectCard();
    }
}

void DispenseCashState::ejectCard() {
    atmMachine->setCurrentCard(nullptr);
    std::cout << "Card ejected." << std::endl;
    atmMachine->setState(std::make_unique<IdleState>(atmMachine));
}
