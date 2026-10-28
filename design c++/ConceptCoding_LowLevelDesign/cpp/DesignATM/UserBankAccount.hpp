#pragma once

/**
 * User's bank account
 */
class UserBankAccount {
public:
    int balance = 0;

    void withdrawBalance(int amount) {
        balance -= amount;
    }
};
