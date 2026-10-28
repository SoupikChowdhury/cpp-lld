#ifndef SPLIT_HPP
#define SPLIT_HPP

#include "User.hpp"

class Split {
private:
    User* user;
    double amount;

public:
    Split(User* user, double amount) : user(user), amount(amount) {}

    User* getUser() const { return user; }
    double getAmount() const { return amount; }
};

#endif // SPLIT_HPP
