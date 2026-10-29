#pragma once

/**
 * @brief Enum representing different types of expense splits
 * 
 * EQUAL: Expense is split equally among all participants
 * UNEQUAL: Expense is split with specific amounts for each participant
 * PERCENTAGE: Expense is split based on percentage contribution
 */
enum class ExpenseSplitType {
    EQUAL,
    UNEQUAL,
    PERCENTAGE
};
