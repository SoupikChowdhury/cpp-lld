#pragma once
#include "Card.hpp"
#include <memory>

/**
 * ATM User
 */
class User {
public:
    std::shared_ptr<Card> card;
    std::shared_ptr<UserBankAccount> bankAccount;

    std::shared_ptr<Card> getCard() const { return card; }
    void setCard(std::shared_ptr<Card> c) { card = c; }
};
