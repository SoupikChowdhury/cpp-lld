#pragma once

#include <unordered_map>
#include <string>
#include <memory>
#include "Balance.hpp"

/**
 * @brief Tracks a user's expense balance sheet
 * 
 * Maintains the balance information for a user including:
 * - Individual balances with other users
 * - Total expenses, payments, amounts owed and amounts to get back
 */
class UserExpenseBalanceSheet {
private:
    std::unordered_map<std::string, std::shared_ptr<Balance>> userVsBalance;
    double totalYourExpense;
    double totalPayment;
    double totalYouOwe;
    double totalYouGetBack;

public:
    UserExpenseBalanceSheet() 
        : totalYourExpense(0.0)
        , totalPayment(0.0)
        , totalYouOwe(0.0)
        , totalYouGetBack(0.0) {}

    // Getters
    std::unordered_map<std::string, std::shared_ptr<Balance>>& getUserVsBalance() { 
        return userVsBalance; 
    }
    
    const std::unordered_map<std::string, std::shared_ptr<Balance>>& getUserVsBalance() const { 
        return userVsBalance; 
    }

    double getTotalYourExpense() const { return totalYourExpense; }
    double getTotalPayment() const { return totalPayment; }
    double getTotalYouOwe() const { return totalYouOwe; }
    double getTotalYouGetBack() const { return totalYouGetBack; }

    // Setters
    void setTotalYourExpense(double amount) { totalYourExpense = amount; }
    void setTotalPayment(double amount) { totalPayment = amount; }
    void setTotalYouOwe(double amount) { totalYouOwe = amount; }
    void setTotalYouGetBack(double amount) { totalYouGetBack = amount; }
};
