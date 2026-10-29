#ifndef DISCOUNT_PRICING_HPP
#define DISCOUNT_PRICING_HPP

#include "PricingStrategy.hpp"

class DiscountPricing : public PricingStrategy {
private:
    double discountPercentage;  // e.g., 0.20 for 20% discount

public:
    explicit DiscountPricing(double discountPercentage) 
        : discountPercentage(discountPercentage) {}

    double calculatePrice(Vehicle* vehicle, int days) override {
        double basePrice = vehicle->getPricePerDay() * days;
        return basePrice * (1.0 - discountPercentage);
    }

    std::string getStrategyName() const override {
        return "DiscountPricing (" + std::to_string(static_cast<int>(discountPercentage * 100)) + "% off)";
    }
};

#endif // DISCOUNT_PRICING_HPP
