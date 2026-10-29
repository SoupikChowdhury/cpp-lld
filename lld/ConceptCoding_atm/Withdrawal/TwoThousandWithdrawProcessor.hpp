#pragma once
#include "CashWithdrawProcessor.hpp"

/**
 * Handles 2000 rupee notes withdrawal
 */
class TwoThousandWithdrawProcessor : public CashWithdrawProcessor {
public:
    explicit TwoThousandWithdrawProcessor(std::shared_ptr<CashWithdrawProcessor> next = nullptr)
        : CashWithdrawProcessor(next) {}

    void withdraw(ATM* atm, int remainingAmount) override;
};
