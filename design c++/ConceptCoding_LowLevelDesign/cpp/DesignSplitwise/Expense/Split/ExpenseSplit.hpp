#pragma once

#include <vector>
#include <memory>
#include "Split.hpp"

/**
 * @brief Strategy interface for expense split validation
 * 
 * This is the Strategy Pattern interface. Different split types
 * (Equal, Unequal, Percentage) implement this interface with
 * their own validation logic.
 */
class ExpenseSplit {
public:
    virtual ~ExpenseSplit() = default;

    /**
     * @brief Validate that the split request is valid
     * @param splitList List of splits for each user
     * @param totalAmount Total expense amount
     * @return true if validation passes, false otherwise
     */
    virtual bool validateSplitRequest(
        const std::vector<std::shared_ptr<Split>>& splitList, 
        double totalAmount) = 0;
};
