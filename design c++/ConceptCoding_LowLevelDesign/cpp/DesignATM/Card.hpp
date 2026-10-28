#pragma once
#include "UserBankAccount.hpp"
#include &lt;memory&gt;

/**
 * ATM Card - Links to user's bank account
 */
class Card {
private:
    int cardNumber = 0;
    int cvv = 0;
    int expiryDate = 0;
    std::string holderName;
    static const int PIN_NUMBER = 112211;
    std::shared_ptr&lt;UserBankAccount&gt; bankAccount;

public:
    bool isCorrectPINEntered(int pin) const {
        return pin == PIN_NUMBER;
    }

    int getBankBalance() const {
        return bankAccount ? bankAccount-&gt;balance : 0;
    }

    void deductBankBalance(int amount) {
        if (bankAccount) {
            bankAccount-&gt;withdrawBalance(amount);
        }
    }

    void setBankAccount(std::shared_ptr&lt;UserBankAccount&gt; account) {
        bankAccount = account;
    }
};
