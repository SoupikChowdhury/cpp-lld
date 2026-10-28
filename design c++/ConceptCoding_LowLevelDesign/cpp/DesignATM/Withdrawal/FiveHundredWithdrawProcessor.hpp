#pragma once
#include "CashWithdrawProcessor.hpp"

/**
 * Handles 500 rupee notes withdrawal
 */
class FiveHundredWithdrawProcessor : public CashWithdrawProcessor {
public:
    explicit FiveHundredWithdrawProcessor(std::shared_ptr&lt;CashWithdrawProcessor&gt; next = nullptr)
        : CashWithdrawProcessor(next) {}

    void withdraw(ATM* atm, int remainingAmount) override;
};
