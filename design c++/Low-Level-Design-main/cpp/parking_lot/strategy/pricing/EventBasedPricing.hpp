#ifndef EVENT_BASED_PRICING_HPP
#define EVENT_BASED_PRICING_HPP

#include "PricingStrategy.hpp"

class EventBasedPricing : public PricingStrategy {
public:
    double calculateFee(VehicleType type, long hours) const override {
        // Flat rate for events
        switch (type) {
            case VehicleType::BIKE:
                return 50.0;   // Rs. 50 flat for bikes
            case VehicleType::CAR:
                return 100.0;  // Rs. 100 flat for cars
            case VehicleType::TRUCK:
                return 200.0;  // Rs. 200 flat for trucks
            default:
                return 100.0;
        }
    }
};

#endif // EVENT_BASED_PRICING_HPP
