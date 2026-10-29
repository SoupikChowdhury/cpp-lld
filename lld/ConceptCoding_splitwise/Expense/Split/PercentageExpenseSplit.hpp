#pragma once

#include <vector>
#include <memory>
#include <cmath>
#include <stdexcept>
#include "ExpenseSplit.hpp"
#include "Split.hpp"

/**
 * @brief Concrete Strategy for percentage-based expense splitting
 * 
 * Validates that the percentage splits are valid and sum to 100%.
 * Note: In this implementation, amountOwe represents the percentage (0-100).
 */
class PercentageExpenseSplit : public ExpenseSplit {
public:
    /**
     * @brief Validate that all percentage splits sum to 100%
     * @param splitList List of splits (amountOwe represents percentage)
     * @param totalAmount Total expense amount (used for reference)
     * @return true if percentages sum to 100
     * @throws std::invalid_argument if percentages don't sum to 100
     */
    bool validateSplitRequest(
        const std::vector<std::shared_ptr<Split>>& splitList, 
        double totalAmount) override {
        
        if (splitList.empty()) {
            throw std::invalid_argument("Split list cannot be empty");
        }

        double totalPercentage = 0.0;
        for (const auto& split : splitList) {
            double percentage = (split->getAmountOwe() / totalAmount) * 100.0;
            if (percentage < 0 || percentage > 100) {
                throw std::invalid_argument("Percentage must be between 0 and 100");
            }
            totalPercentage += percentage;
        }

        const double epsilon = 0.01; // Tolerance for floating point comparison
        if (std::abs(totalPercentage - 100.0) > epsilon) {
            throw std::invalid_argument(
                "Invalid percentage split: Total percentage must equal 100%, got " + 
                std::to_string(totalPercentage) + "%");
        }
        return true;
    }
};
