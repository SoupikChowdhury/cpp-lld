#pragma once

#include <string>
#include <vector>
#include <memory>
#include "ExpenseSplitType.hpp"
#include "Split/Split.hpp"
#include "../User/User.hpp"

/**
 * @brief Represents an expense in the Splitwise system
 * 
 * Contains all information about an expense including who paid,
 * the amount, description, and how it's split among participants.
 */
class Expense {
private:
    std::string expenseId;
    std::string description;
    double expenseAmount;
    std::shared_ptr<User> paidByUser;
    ExpenseSplitType splitType;
    std::vector<std::shared_ptr<Split>> splitDetails;

public:
    Expense(const std::string& expenseId,
            double expenseAmount,
            const std::string& description,
            std::shared_ptr<User> paidByUser,
            ExpenseSplitType splitType,
            const std::vector<std::shared_ptr<Split>>& splitDetails)
        : expenseId(expenseId)
        , description(description)
        , expenseAmount(expenseAmount)
        , paidByUser(paidByUser)
        , splitType(splitType)
        , splitDetails(splitDetails) {}

    // Getters
    const std::string& getExpenseId() const { return expenseId; }
    const std::string& getDescription() const { return description; }
    double getExpenseAmount() const { return expenseAmount; }
    std::shared_ptr<User> getPaidByUser() const { return paidByUser; }
    ExpenseSplitType getSplitType() const { return splitType; }
    const std::vector<std::shared_ptr<Split>>& getSplitDetails() const { return splitDetails; }
};
