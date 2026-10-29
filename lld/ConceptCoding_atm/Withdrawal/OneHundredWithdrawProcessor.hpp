#pragma once
#include "CashWithdrawProcessor.hpp"

/**
 * Handles 100 rupee notes withdrawal
 */
class OneHundredWithdrawProcessor : public CashWithdrawProcessor {
public:
    explicit OneHundredWithdrawProcessor(std::shared_ptr<CashWithdrawProcessor> next = nullptr)
        : CashWithdrawProcessor(next) {}

    void withdraw(ATM* atm, int remainingAmount) override;
};
