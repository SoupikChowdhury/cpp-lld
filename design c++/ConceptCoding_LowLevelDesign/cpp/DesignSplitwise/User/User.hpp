#pragma once

#include <string>
#include <memory>
#include "../UserExpenseBalanceSheet.hpp"

/**
 * @brief Represents a user in the Splitwise system
 * 
 * Each user has a unique ID, name, and their own expense balance sheet
 * to track all expenses and balances with other users.
 */
class User {
private:
    std::string userId;
    std::string userName;
    std::shared_ptr<UserExpenseBalanceSheet> userExpenseBalanceSheet;

public:
    User(const std::string& id, const std::string& name)
        : userId(id)
        , userName(name)
        , userExpenseBalanceSheet(std::make_shared<UserExpenseBalanceSheet>()) {}

    // Getters
    const std::string& getUserId() const { return userId; }
    const std::string& getUserName() const { return userName; }
    
    std::shared_ptr<UserExpenseBalanceSheet> getUserExpenseBalanceSheet() {
        return userExpenseBalanceSheet;
    }
    
    std::shared_ptr<const UserExpenseBalanceSheet> getUserExpenseBalanceSheet() const {
        return userExpenseBalanceSheet;
    }
};
