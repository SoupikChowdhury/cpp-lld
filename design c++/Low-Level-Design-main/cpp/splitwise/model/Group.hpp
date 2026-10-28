#ifndef GROUP_HPP
#define GROUP_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include "User.hpp"
#include "Expense.hpp"
#include "BalanceSheet.hpp"

class Group {
private:
    std::string id;
    std::string name;
    std::vector<User*> members;
    std::vector<Expense*> expenses;
    std::unordered_map<std::string, BalanceSheet> balanceSheets;  // userId -> sheet

public:
    Group(const std::string& id, const std::string& name)
        : id(id), name(name) {}

    std::string getId() const { return id; }
    std::string getName() const { return name; }
    const std::vector<User*>& getMembers() const { return members; }
    const std::vector<Expense*>& getExpenses() const { return expenses; }

    void addMember(User* user) {
        members.push_back(user);
        balanceSheets[user->getId()] = BalanceSheet();
    }

    void addExpense(Expense* expense) {
        expenses.push_back(expense);
    }

    BalanceSheet& getBalanceSheet(const std::string& userId) {
        return balanceSheets[userId];
    }

    std::unordered_map<std::string, BalanceSheet>& getAllBalanceSheets() {
        return balanceSheets;
    }
};

#endif // GROUP_HPP
