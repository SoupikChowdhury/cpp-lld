#ifndef PERCENTAGE_SPLIT_STRATEGY_HPP
#define PERCENTAGE_SPLIT_STRATEGY_HPP

#include "SplitStrategy.hpp"

class PercentageSplitStrategy : public SplitStrategy {
public:
    std::vector<Split> split(double totalAmount, 
                              const std::vector<User*>& participants,
                              const std::unordered_map<std::string, double>& metadata) override {
        std::vector<Split> splits;
        
        for (User* user : participants) {
            auto it = metadata.find(user->getId());
            if (it != metadata.end()) {
                double percentage = it->second;
                double amount = (totalAmount * percentage) / 100.0;
                splits.emplace_back(user, amount);
            }
        }
        
        return splits;
    }
};

#endif // PERCENTAGE_SPLIT_STRATEGY_HPP
