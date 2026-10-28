#pragma once

#include <vector>
#include <memory>
#include <cmath>
#include <stdexcept>
#include "ExpenseSplit.hpp"
#include "Split.hpp"

/**
 * @brief Concrete Strategy for equal expense splitting
 * 
 * Validates that the expense is split equally among all participants.
 * Each participant should owe the same amount (total / number of participants).
 */
class EqualExpenseSplit : public ExpenseSplit {
public:
    /**
     * @brief Validate that all splits are equal
     * @param splitList List of splits for each user
     * @param totalAmount Total expense amount
     * @return true if all splits are equal to totalAmount/numberOfSplits
     * @throws std::invalid_argument if splits are not equal
     */
    bool validateSplitRequest(
        const std::vector<std::shared_ptr<Split>>& splitList, 
        double totalAmount) override {
        
        if (splitList.empty()) {
            throw std::invalid_argument("Split list cannot be empty");
        }

        double amountShouldBePresent = totalAmount / static_cast<double>(splitList.size());
        const double epsilon = 0.01; // Tolerance for floating point comparison

        for (const auto& split : splitList) {
            if (std::abs(split->getAmountOwe() - amountShouldBePresent) > epsilon) {
                throw std::invalid_argument(
                    "Invalid equal split: Each participant should owe " + 
                    std::to_string(amountShouldBePresent));
            }
        }
        return true;
    }
};
