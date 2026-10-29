#ifndef STANDARD_PRICING_HPP
#define STANDARD_PRICING_HPP

#include "PricingStrategy.hpp"

class StandardPricing : public PricingStrategy {
public:
    double calculatePrice(Vehicle* vehicle, int days) override {
        return vehicle->getPricePerDay() * days;
    }

    std::string getStrategyName() const override {
        return "StandardPricing";
    }
};

#endif // STANDARD_PRICING_HPP
