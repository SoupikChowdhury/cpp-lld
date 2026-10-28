#ifndef DEBT_SIMPLIFICATION_SERVICE_HPP
#define DEBT_SIMPLIFICATION_SERVICE_HPP

#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include "../model/Group.hpp"

class DebtSimplificationService {
public:
    void simplifyDebts(Group* group) {
        std::cout << "\n=== Simplified Debts ===" << std::endl;
        
        // Calculate net balance for each user
        std::vector<std::pair<std::string, double>> balances;
        
        for (User* user : group->getMembers()) {
            BalanceSheet& sheet = group->getBalanceSheet(user->getId());
            double netBalance = sheet.getNetBalance();
            if (std::abs(netBalance) > 0.01) {  // Skip zero balances
                balances.push_back({user->getName(), netBalance});
            }
        }

        // Separate into creditors (positive) and debtors (negative)
        auto cmp = [](std::pair<std::string, double>& a, std::pair<std::string, double>& b) {
            return std::abs(a.second) < std::abs(b.second);
        };

        std::priority_queue<std::pair<std::string, double>, 
                           std::vector<std::pair<std::string, double>>,
                           decltype(cmp)> creditors(cmp), debtors(cmp);

        for (auto& [name, balance] : balances) {
            if (balance > 0) {
                creditors.push({name, balance});
            } else if (balance < 0) {
                debtors.push({name, -balance});  // Store as positive for easier calculation
            }
        }

        // Match creditors with debtors
        while (!creditors.empty() && !debtors.empty()) {
            auto creditor = creditors.top(); creditors.pop();
            auto debtor = debtors.top(); debtors.pop();

            double amount = std::min(creditor.second, debtor.second);
            
            std::cout << debtor.first << " pays " << creditor.first 
                      << ": Rs. " << amount << std::endl;

            creditor.second -= amount;
            debtor.second -= amount;

            if (creditor.second > 0.01) {
                creditors.push(creditor);
            }
            if (debtor.second > 0.01) {
                debtors.push(debtor);
            }
        }
    }
};

#endif // DEBT_SIMPLIFICATION_SERVICE_HPP
