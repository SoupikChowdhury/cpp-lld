#ifndef SPLIT_STRATEGY_FACTORY_HPP
#define SPLIT_STRATEGY_FACTORY_HPP

#include <memory>
#include "../enums/SplitType.hpp"
#include "../strategy/SplitStrategy.hpp"
#include "../strategy/EqualSplitStrategy.hpp"
#include "../strategy/PercentageSplitStrategy.hpp"

class SplitStrategyFactory {
public:
    static std::unique_ptr<SplitStrategy> get(SplitType type) {
        switch (type) {
            case SplitType::EQUAL:
                return std::make_unique<EqualSplitStrategy>();
            case SplitType::PERCENTAGE:
                return std::make_unique<PercentageSplitStrategy>();
            default:
                return std::make_unique<EqualSplitStrategy>();
        }
    }
};

#endif // SPLIT_STRATEGY_FACTORY_HPP
