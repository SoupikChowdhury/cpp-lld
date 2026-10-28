#pragma once
#include <iostream>

/**
 * Transaction types supported by ATM
 */
enum class TransactionType {
    CASH_WITHDRAWAL,
    BALANCE_CHECK
};

inline void showAllTransactionTypes() {
    std::cout << "1. CASH_WITHDRAWAL" << std::endl;
    std::cout << "2. BALANCE_CHECK" << std::endl;
}
