#ifndef EXPENSE_HPP
#define EXPENSE_HPP

#include <string>
#include <vector>
#include "User.hpp"
#include "Split.hpp"

class Expense {
private:
    std::string id;
    std::string description;
    double amount;
    User* paidBy;
    std::vector<Split> splits;

public:
    Expense(const std::string& id, const std::string& description, 
            double amount, User* paidBy, const std::vector<Split>& splits)
        : id(id), description(description), amount(amount), 
          paidBy(paidBy), splits(splits) {}

    std::string getId() const { return id; }
    std::string getDescription() const { return description; }
    double getAmount() const { return amount; }
    User* getPaidBy() const { return paidBy; }
    const std::vector<Split>& getSplits() const { return splits; }
};

#endif // EXPENSE_HPP
