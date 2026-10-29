#pragma once

#include <memory>
#include <stdexcept>
#include "ExpenseSplitType.hpp"
#include "Split/ExpenseSplit.hpp"
#include "Split/EqualExpenseSplit.hpp"
#include "Split/UnequalExpenseSplit.hpp"
#include "Split/PercentageExpenseSplit.hpp"

/**
 * @brief Factory for creating ExpenseSplit strategy objects
 * 
 * Uses the Factory Pattern to create appropriate ExpenseSplit
 * implementations based on the split type.
 */
class SplitFactory {
public:
    /**
     * @brief Create an ExpenseSplit strategy based on the split type
     * @param splitType The type of expense split
     * @return Unique pointer to the appropriate ExpenseSplit implementation
     * @throws std::invalid_argument if splitType is unknown
     */
    static std::unique_ptr<ExpenseSplit> getSplitObject(ExpenseSplitType splitType) {
        switch (splitType) {
            case ExpenseSplitType::EQUAL:
                return std::make_unique<EqualExpenseSplit>();
            case ExpenseSplitType::UNEQUAL:
                return std::make_unique<UnequalExpenseSplit>();
            case ExpenseSplitType::PERCENTAGE:
                return std::make_unique<PercentageExpenseSplit>();
            default:
                throw std::invalid_argument("Unknown expense split type");
        }
    }
};
