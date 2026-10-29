#ifndef WEEKEND_PRICING_HPP
#define WEEKEND_PRICING_HPP

#include "PricingStrategy.hpp"

class WeekendPricing : public PricingStrategy {
private:
    double multiplier;

public:
    explicit WeekendPricing(double multiplier = 1.5) : multiplier(multiplier) {}

    double calculatePrice(Vehicle* vehicle, int days) override {
        return vehicle->getPricePerDay() * days * multiplier;
    }

    std::string getStrategyName() const override {
        return "WeekendPricing (x" + std::to_string(multiplier) + ")";
    }
};

#endif // WEEKEND_PRICING_HPP
