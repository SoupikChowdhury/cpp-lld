#ifndef TIME_BASED_PRICING_HPP
#define TIME_BASED_PRICING_HPP

#include "PricingStrategy.hpp"

class TimeBasedPricing : public PricingStrategy {
public:
    double calculateFee(VehicleType type, long hours) const override {
        double baseRate = 0.0;
        
        switch (type) {
            case VehicleType::BIKE:
                baseRate = 10.0;  // Rs. 10 per hour
                break;
            case VehicleType::CAR:
                baseRate = 20.0;  // Rs. 20 per hour
                break;
            case VehicleType::TRUCK:
                baseRate = 30.0;  // Rs. 30 per hour
                break;
        }

        // Peak hours multiplier (simplified - in real scenario check time)
        double multiplier = 1.0;
        if (hours > 0 && hours <= 3) {
            multiplier = 1.0;
        } else if (hours > 3 && hours <= 6) {
            multiplier = 1.5;  // 50% more after 3 hours
        } else {
            multiplier = 2.0;  // Double after 6 hours
        }

        return baseRate * hours * multiplier;
    }
};

#endif // TIME_BASED_PRICING_HPP
