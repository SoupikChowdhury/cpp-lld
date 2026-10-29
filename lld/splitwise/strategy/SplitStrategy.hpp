#ifndef SPLIT_STRATEGY_HPP
#define SPLIT_STRATEGY_HPP

#include <vector>
#include <unordered_map>
#include "../model/User.hpp"
#include "../model/Split.hpp"

class SplitStrategy {
public:
    virtual ~SplitStrategy() = default;
    virtual std::vector<Split> split(double totalAmount, 
                                      const std::vector<User*>& participants,
                                      const std::unordered_map<std::string, double>& metadata) = 0;
};

#endif // SPLIT_STRATEGY_HPP
