#ifndef PRICING_STRATEGY_HPP
#define PRICING_STRATEGY_HPP

#include "../../enums/VehicleType.hpp"

class PricingStrategy {
public:
    virtual ~PricingStrategy() = default;
    virtual double calculateFee(VehicleType type, long hours) const = 0;
};

#endif // PRICING_STRATEGY_HPP
