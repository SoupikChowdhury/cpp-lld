#pragma once
#include "../TransactionType.hpp"
#include "../Card.hpp"
#include <memory>
#include <iostream>

// Forward declaration
class ATM;

/**
 * State Pattern - Abstract ATM State
 */
class ATMState {
public:
    virtual ~ATMState() = default;

    virtual void insertCard(ATM* atm, std::shared_ptr<Card> card) {
        std::cout << "OOPS!! Something went wrong" << std::endl;
    }

    virtual void authenticatePin(ATM* atm, std::shared_ptr<Card> card, int pin) {
        std::cout << "OOPS!! Something went wrong" << std::endl;
    }

    virtual void selectOperation(ATM* atm, std::shared_ptr<Card> card, TransactionType txnType) {
        std::cout << "OOPS!! Something went wrong" << std::endl;
    }

    virtual void cashWithdrawal(ATM* atm, std::shared_ptr<Card> card, int withdrawAmount) {
        std::cout << "OOPS!! Something went wrong" << std::endl;
    }

    virtual void displayBalance(ATM* atm, std::shared_ptr<Card> card) {
        std::cout << "OOPS!! Something went wrong" << std::endl;
    }

    virtual void returnCard() {
        std::cout << "OOPS!! Something went wrong" << std::endl;
    }

    virtual void exit(ATM* atm) {
        std::cout << "OOPS!! Something went wrong" << std::endl;
    }
};
