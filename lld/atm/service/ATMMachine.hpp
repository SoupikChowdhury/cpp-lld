#ifndef ATM_MACHINE_HPP
#define ATM_MACHINE_HPP

#include <memory>
#include <string>
#include <stdexcept>
#include "../model/ATM.hpp"
#include "../model/Card.hpp"
#include "../state/ATMState.hpp"
#include "../repository/ATMRepository.hpp"

// Forward declarations for states
class IdleState;
class CardInsertedState;
class AuthenticatedState;
class DispenseCashState;

class ATMMachine {
private:
    std::shared_ptr<ATM> atm;
    std::unique_ptr<ATMState> state;
    std::shared_ptr<ATMRepository> atmRepository;
    std::shared_ptr<Card> currentCard;

public:
    ATMMachine(const std::string& atmId, std::shared_ptr<ATMRepository> atmRepository);

    std::shared_ptr<ATM> getAtm() const { return atm; }
    std::shared_ptr<Card> getCurrentCard() const { return currentCard; }
    void setCurrentCard(std::shared_ptr<Card> card) { currentCard = card; }

    void insertCard(std::shared_ptr<Card> card) {
        state->insertCard(card);
    }

    void enterPin(const std::string& pin) {
        state->enterPin(pin);
    }

    void selectOption(const std::string& option) {
        state->selectOption(option);
    }

    void dispenseCash(int amount) {
        state->dispenseCash(amount);
    }

    void ejectCard() {
        state->ejectCard();
    }

    void setState(std::unique_ptr<ATMState> newState) {
        state = std::move(newState);
        atm->setStatus(state->getStatus());
    }
};

#endif // ATM_MACHINE_HPP
