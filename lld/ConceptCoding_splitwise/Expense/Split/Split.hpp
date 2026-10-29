#pragma once

#include <memory>
#include "../../User/User.hpp"

/**
 * @brief Represents a split portion of an expense for a single user
 * 
 * Contains the user involved in the split and the amount they owe.
 */
class Split {
private:
    std::shared_ptr<User> user;
    double amountOwe;

public:
    Split(std::shared_ptr<User> user, double amountOwe)
        : user(user)
        , amountOwe(amountOwe) {}

    // Getters
    std::shared_ptr<User> getUser() const { return user; }
    double getAmountOwe() const { return amountOwe; }

    // Setters
    void setUser(std::shared_ptr<User> user) { this->user = user; }
    void setAmountOwe(double amount) { amountOwe = amount; }
};
