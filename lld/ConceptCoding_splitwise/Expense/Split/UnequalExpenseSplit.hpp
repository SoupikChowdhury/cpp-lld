#pragma once

#include <vector>
#include <memory>
#include <cmath>
#include <stdexcept>
#include "ExpenseSplit.hpp"
#include "Split.hpp"

/**
 * @brief Concrete Strategy for unequal expense splitting
 * 
 * Validates that the sum of all splits equals the total expense amount.
 * Each participant can owe a different amount.
 */
class UnequalExpenseSplit : public ExpenseSplit {
public:
    /**
     * @brief Validate that all splits sum up to total amount
     * @param splitList List of splits for each user
     * @param totalAmount Total expense amount
     * @return true if sum of all splits equals totalAmount
     * @throws std::invalid_argument if splits don't sum to total
     */
    bool validateSplitRequest(
        const std::vector<std::shared_ptr<Split>>& splitList, 
        double totalAmount) override {
        
        if (splitList.empty()) {
            throw std::invalid_argument("Split list cannot be empty");
        }

        double sum = 0.0;
        for (const auto& split : splitList) {
            if (split->getAmountOwe() < 0) {
                throw std::invalid_argument("Split amount cannot be negative");
            }
            sum += split->getAmountOwe();
        }

        const double epsilon = 0.01; // Tolerance for floating point comparison
        if (std::abs(sum - totalAmount) > epsilon) {
            throw std::invalid_argument(
                "Invalid unequal split: Sum of splits (" + std::to_string(sum) + 
                ") doesn't match total amount (" + std::to_string(totalAmount) + ")");
        }
        return true;
    }
};
