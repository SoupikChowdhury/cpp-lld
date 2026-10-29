#ifndef LEAST_BOOKED_STRATEGY_HPP
#define LEAST_BOOKED_STRATEGY_HPP

#include <unordered_map>
#include "BookingStrategy.hpp"

class LeastBookedStrategy : public BookingStrategy {
private:
    // Track booking counts per vehicle
    std::unordered_map<std::string, int>& bookingCounts;

public:
    explicit LeastBookedStrategy(std::unordered_map<std::string, int>& counts)
        : bookingCounts(counts) {}

    Vehicle* selectVehicle(const std::vector<Vehicle*>& vehicles) override {
        if (vehicles.empty()) {
            return nullptr;
        }

        Vehicle* leastBooked = vehicles[0];
        int minBookings = getBookingCount(leastBooked->getVehicleId());

        for (Vehicle* vehicle : vehicles) {
            int count = getBookingCount(vehicle->getVehicleId());
            if (count < minBookings) {
                minBookings = count;
                leastBooked = vehicle;
            }
        }
        return leastBooked;
    }

    std::string getStrategyName() const override {
        return "LeastBookedStrategy";
    }

private:
    int getBookingCount(const std::string& vehicleId) {
        auto it = bookingCounts.find(vehicleId);
        return (it != bookingCounts.end()) ? it->second : 0;
    }
};

#endif // LEAST_BOOKED_STRATEGY_HPP
