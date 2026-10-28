#pragma once
#include &lt;memory&gt;
#include &lt;iostream&gt;

// Forward declaration
class ATM;

/**
 * Chain of Responsibility - Abstract Withdrawal Processor
 * 
 * Each processor handles specific denomination and passes
 * remaining to next processor in chain.
 */
class CashWithdrawProcessor {
protected:
    std::shared_ptr&lt;CashWithdrawProcessor&gt; nextProcessor;

public:
    explicit CashWithdrawProcessor(std::shared_ptr&lt;CashWithdrawProcessor&gt; next = nullptr)
        : nextProcessor(next) {}

    virtual ~CashWithdrawProcessor() = default;

    virtual void withdraw(ATM* atm, int remainingAmount);
};
