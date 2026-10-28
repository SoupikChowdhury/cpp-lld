#ifndef BOOKING_STRATEGY_HPP
#define BOOKING_STRATEGY_HPP

#include <vector>
#include "../../model/Vehicle.hpp"

class BookingStrategy {
public:
    virtual ~BookingStrategy() = default;
    virtual Vehicle* selectVehicle(const std::vector<Vehicle*>& vehicles) = 0;
    virtual std::string getStrategyName() const = 0;
};

#endif // BOOKING_STRATEGY_HPP
