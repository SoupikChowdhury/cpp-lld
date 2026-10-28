#pragma once

#include <string>
#include <vector>
#include <memory>
#include "../User/User.hpp"
#include "../Expense/Expense.hpp"
#include "../Expense/ExpenseController.hpp"
#include "../Expense/ExpenseSplitType.hpp"
#include "../Expense/Split/Split.hpp"

/**
 * @brief Represents a group in Splitwise
 * 
 * A group contains members and their shared expenses.
 * Groups allow multiple users to track expenses together.
 */
class Group {
private:
    std::string groupId;
    std::string groupName;
    std::vector<std::shared_ptr<User>> groupMembers;
    std::vector<std::shared_ptr<Expense>> expenseList;
    ExpenseController expenseController;

public:
    Group() = default;

    /**
     * @brief Add a member to the group
     * @param member User to add to the group
     */
    void addMember(std::shared_ptr<User> member) {
        groupMembers.push_back(member);
    }

    /**
     * @brief Create an expense within this group
     * @param expenseId Unique identifier for the expense
     * @param description Description of the expense
     * @param expenseAmount Total amount of the expense
     * @param splitDetails How the expense is split among users
     * @param splitType Type of split (EQUAL, UNEQUAL, PERCENTAGE)
     * @param paidByUser User who paid for the expense
     * @return Shared pointer to the created Expense
     */
    std::shared_ptr<Expense> createExpense(
        const std::string& expenseId,
        const std::string& description,
        double expenseAmount,
        const std::vector<std::shared_ptr<Split>>& splitDetails,
        ExpenseSplitType splitType,
        std::shared_ptr<User> paidByUser) {
        
        auto expense = expenseController.createExpense(
            expenseId, description, expenseAmount, 
            splitDetails, splitType, paidByUser);
        
        expenseList.push_back(expense);
        return expense;
    }

    // Getters
    const std::string& getGroupId() const { return groupId; }
    const std::string& getGroupName() const { return groupName; }
    const std::vector<std::shared_ptr<User>>& getGroupMembers() const { return groupMembers; }
    const std::vector<std::shared_ptr<Expense>>& getExpenseList() const { return expenseList; }

    // Setters
    void setGroupId(const std::string& id) { groupId = id; }
    void setGroupName(const std::string& name) { groupName = name; }
};
