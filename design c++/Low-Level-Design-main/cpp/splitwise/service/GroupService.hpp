#ifndef GROUP_SERVICE_HPP
#define GROUP_SERVICE_HPP

#include <iostream>
#include <string>
#include <vector>
#include <random>
#include "../model/Group.hpp"
#include "../model/Expense.hpp"
#include "../repository/GroupRepository.hpp"
#include "../factory/SplitStrategyFactory.hpp"
#include "DebtSimplificationService.hpp"

class GroupService {
private:
    GroupRepository& repository;
    DebtSimplificationService simplificationService;
    int expenseCounter = 0;

    std::string generateId() {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<> dis(1000, 9999);
        return "G" + std::to_string(dis(gen));
    }

public:
    explicit GroupService(GroupRepository& repo) : repository(repo) {}

    std::string createGroup(const std::string& name, const std::vector<User*>& members) {
        std::string groupId = generateId();
        auto group = std::make_unique<Group>(groupId, name);
        
        for (User* user : members) {
            group->addMember(user);
        }
        
        repository.save(std::move(group));
        std::cout << "Group '" << name << "' created with ID: " << groupId << std::endl;
        return groupId;
    }

    void addMember(const std::string& groupId, User* user) {
        Group* group = repository.findById(groupId);
        if (group) {
            group->addMember(user);
            std::cout << "Added " << user->getName() << " to group." << std::endl;
        }
    }

    void addExpense(const std::string& groupId, const std::string& description,
                    double amount, User* paidBy, const std::vector<User*>& participants,
                    SplitType splitType, 
                    const std::unordered_map<std::string, double>& metadata = {}) {
        
        Group* group = repository.findById(groupId);
        if (!group) {
            std::cout << "Group not found!" << std::endl;
            return;
        }

        // Get split strategy and calculate splits
        auto strategy = SplitStrategyFactory::get(splitType);
        std::vector<Split> splits = strategy->split(amount, participants, metadata);

        // Create expense
        std::string expenseId = "E" + std::to_string(++expenseCounter);
        auto* expense = new Expense(expenseId, description, amount, paidBy, splits);
        group->addExpense(expense);

        // Update balance sheets
        BalanceSheet& payerSheet = group->getBalanceSheet(paidBy->getId());
        payerSheet.addPaid(amount);

        for (const Split& split : splits) {
            BalanceSheet& participantSheet = group->getBalanceSheet(split.getUser()->getId());
            participantSheet.addExpense(split.getAmount());

            // Update inter-user balances
            if (split.getUser()->getId() != paidBy->getId()) {
                payerSheet.updateBalance(split.getUser()->getId(), split.getAmount());
                participantSheet.updateBalance(paidBy->getId(), -split.getAmount());
            }
        }

        std::cout << "Added expense: " << description << " - Rs. " << amount 
                  << " (paid by " << paidBy->getName() << ")" << std::endl;
    }

    void printBalances(const std::string& groupId) {
        Group* group = repository.findById(groupId);
        if (!group) return;

        std::cout << "\n=== Balances for Group: " << group->getName() << " ===" << std::endl;
        
        for (User* user : group->getMembers()) {
            BalanceSheet& sheet = group->getBalanceSheet(user->getId());
            std::cout << user->getName() << ":" << std::endl;
            std::cout << "  Total Paid: Rs. " << sheet.getTotalPaid() << std::endl;
            std::cout << "  Total Expense: Rs. " << sheet.getTotalExpense() << std::endl;
            std::cout << "  Net Balance: Rs. " << sheet.getNetBalance() << std::endl;
            
            for (auto& [otherId, balance] : sheet.getBalances()) {
                if (balance > 0) {
                    std::cout << "  -> Owes from " << otherId << ": Rs. " << balance << std::endl;
                } else if (balance < 0) {
                    std::cout << "  -> Owes to " << otherId << ": Rs. " << -balance << std::endl;
                }
            }
        }
    }

    void simplifyDebts(const std::string& groupId) {
        Group* group = repository.findById(groupId);
        if (group) {
            simplificationService.simplifyDebts(group);
        }
    }
};

#endif // GROUP_SERVICE_HPP
