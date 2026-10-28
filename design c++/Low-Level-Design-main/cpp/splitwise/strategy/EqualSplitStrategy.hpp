#ifndef EQUAL_SPLIT_STRATEGY_HPP
#define EQUAL_SPLIT_STRATEGY_HPP

#include "SplitStrategy.hpp"

class EqualSplitStrategy : public SplitStrategy {
public:
    std::vector<Split> split(double totalAmount, 
                              const std::vector<User*>& participants,
                              const std::unordered_map<std::string, double>& metadata) override {
        std::vector<Split> splits;
        double perPerson = totalAmount / participants.size();
        
        for (User* user : participants) {
            splits.emplace_back(user, perPerson);
        }
        
        return splits;
    }
};

#endif // EQUAL_SPLIT_STRATEGY_HPP
