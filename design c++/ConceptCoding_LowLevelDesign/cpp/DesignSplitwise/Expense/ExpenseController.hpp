#pragma once

#include <string>
#include <vector>
#include <memory>
#include "Expense.hpp"
#include "ExpenseSplitType.hpp"
#include "SplitFactory.hpp"
#include "Split/Split.hpp"
#include "Split/ExpenseSplit.hpp"
#include "../BalanceSheetController.hpp"
#include "../User/User.hpp"

/**
 * @brief Controller for managing expenses
 * 
 * Handles creation and validation of expenses using the Strategy pattern
 * for different split types.
 */
class ExpenseController {
private:
    BalanceSheetController balanceSheetController;

public:
    ExpenseController() = default;

    /**
     * @brief Create a new expense with validation
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
        
        // Use Strategy Pattern: Get appropriate split validator
        auto expenseSplit = SplitFactory::getSplitObject(splitType);
        
        // Validate the split request
        expenseSplit->validateSplitRequest(splitDetails, expenseAmount);

        // Create the expense
        auto expense = std::make_shared<Expense>(
            expenseId, expenseAmount, description, 
            paidByUser, splitType, splitDetails);

        // Update balance sheets for all involved users
        balanceSheetController.updateUserExpenseBalanceSheet(
            paidByUser, splitDetails, expenseAmount);

        return expense;
    }
};
