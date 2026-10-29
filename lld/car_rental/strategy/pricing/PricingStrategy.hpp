#ifndef PRICING_STRATEGY_HPP
#define PRICING_STRATEGY_HPP

#include "../../model/Vehicle.hpp"

class PricingStrategy {
public:
    virtual ~PricingStrategy() = default;
    virtual double calculatePrice(Vehicle* vehicle, int days) = 0;
    virtual std::string getStrategyName() const = 0;
};

#endif // PRICING_STRATEGY_HPP
