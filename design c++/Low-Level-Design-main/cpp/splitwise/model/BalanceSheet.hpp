#ifndef BALANCE_SHEET_HPP
#define BALANCE_SHEET_HPP

#include <unordered_map>
#include <string>

class BalanceSheet {
private:
    double totalPaid = 0.0;
    double totalExpense = 0.0;
    std::unordered_map<std::string, double> balances;  // userId -> balance

public:
    void addPaid(double amount) { totalPaid += amount; }
    void addExpense(double amount) { totalExpense += amount; }

    double getTotalPaid() const { return totalPaid; }
    double getTotalExpense() const { return totalExpense; }
    double getNetBalance() const { return totalPaid - totalExpense; }

    void updateBalance(const std::string& userId, double amount) {
        balances[userId] += amount;
    }

    double getBalanceWith(const std::string& userId) const {
        auto it = balances.find(userId);
        return (it != balances.end()) ? it->second : 0.0;
    }

    const std::unordered_map<std::string, double>& getBalances() const {
        return balances;
    }
};

#endif // BALANCE_SHEET_HPP
