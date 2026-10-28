#ifndef ACCOUNT_HPP
#define ACCOUNT_HPP

#include <string>

class Account {
private:
    std::string id;
    double balance;

public:
    Account(const std::string& id, double balance) 
        : id(id), balance(balance) {}

    std::string getId() const { return id; }
    double getBalance() const { return balance; }
    void setBalance(double amount) { balance = amount; }
};

#endif // ACCOUNT_HPP
