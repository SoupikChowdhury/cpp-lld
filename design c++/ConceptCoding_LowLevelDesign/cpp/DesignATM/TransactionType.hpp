#pragma once

/**
 * Transaction types supported by ATM
 */
enum class TransactionType {
    CASH_WITHDRAWAL,
    BALANCE_CHECK
};

inline void showAllTransactionTypes() {
    std::cout &lt;&lt; "1. CASH_WITHDRAWAL" &lt;&lt; std::endl;
    std::cout &lt;&lt; "2. BALANCE_CHECK" &lt;&lt; std::endl;
}
