#ifndef CARD_HPP
#define CARD_HPP

#include <string>
#include <memory>
#include "Account.hpp"

class Card {
private:
    std::string cardNumber;
    std::string pin;
    std::shared_ptr<Account> account;

public:
    Card(const std::string& cardNumber, const std::string& pin, 
         std::shared_ptr<Account> account)
        : cardNumber(cardNumber), pin(pin), account(account) {}

    std::string getCardNumber() const { return cardNumber; }
    std::string getPin() const { return pin; }
    std::shared_ptr<Account> getAccount() const { return account; }
};

#endif // CARD_HPP
