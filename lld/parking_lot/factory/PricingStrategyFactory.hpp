#ifndef PRICING_STRATEGY_FACTORY_HPP
#define PRICING_STRATEGY_FACTORY_HPP

#include <memory>
#include "../enums/PricingStrategyType.hpp"
#include "../strategy/pricing/PricingStrategy.hpp"
#include "../strategy/pricing/TimeBasedPricing.hpp"
#include "../strategy/pricing/EventBasedPricing.hpp"

class PricingStrategyFactory {
public:
    static std::unique_ptr<PricingStrategy> get(PricingStrategyType type) {
        switch (type) {
            case PricingStrategyType::TIME_BASED:
                return std::make_unique<TimeBasedPricing>();
            case PricingStrategyType::EVENT_BASED:
                return std::make_unique<EventBasedPricing>();
            default:
                return std::make_unique<TimeBasedPricing>();
        }
    }
};

#endif // PRICING_STRATEGY_FACTORY_HPP
