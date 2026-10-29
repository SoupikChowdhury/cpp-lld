#ifndef BOOKING_SERVICE_HPP
#define BOOKING_SERVICE_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <ctime>
#include <sstream>
#include <iomanip>
#include "../model/Branch.hpp"
#include "../model/Booking.hpp"
#include "../model/Customer.hpp"
#include "../strategy/booking/BookingStrategy.hpp"
#include "../strategy/booking/CheapestVehicleStrategy.hpp"
#include "../strategy/pricing/PricingStrategy.hpp"
#include "../strategy/pricing/StandardPricing.hpp"

class BookingService {
private:
    std::unordered_map<std::string, Branch*> branches;
    std::unordered_map<std::string, Booking*> bookings;
    std::unordered_map<std::string, int> vehicleBookingCounts;
    
    std::unique_ptr<BookingStrategy> bookingStrategy;
    std::unique_ptr<PricingStrategy> pricingStrategy;
    
    mutable std::mutex mutex_;
    int bookingCounter = 0;

    std::string generateBookingId() {
        std::stringstream ss;
        ss << "BK" << std::setfill('0') << std::setw(6) << (++bookingCounter);
        return ss.str();
    }

public:
    BookingService() {
        bookingStrategy = std::make_unique<CheapestVehicleStrategy>();
        pricingStrategy = std::make_unique<StandardPricing>();
    }

    ~BookingService() {
        for (auto& [id, booking] : bookings) {
            delete booking;
        }
    }

    // Branch management
    void addBranch(Branch* branch) {
        std::lock_guard<std::mutex> lock(mutex_);
        branches[branch->getBranchId()] = branch;
    }

    Branch* getBranch(const std::string& branchId) {
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = branches.find(branchId);
        return (it != branches.end()) ? it->second : nullptr;
    }

    // Strategy setters
    void setBookingStrategy(std::unique_ptr<BookingStrategy> strategy) {
        std::lock_guard<std::mutex> lock(mutex_);
        bookingStrategy = std::move(strategy);
    }

    void setPricingStrategy(std::unique_ptr<PricingStrategy> strategy) {
        std::lock_guard<std::mutex> lock(mutex_);
        pricingStrategy = std::move(strategy);
    }

    // Core booking operations
    Booking* createBooking(Customer* customer, const std::string& branchId,
                           VehicleType vehicleType, time_t startDate, time_t endDate) {
        std::lock_guard<std::mutex> lock(mutex_);

        Branch* branch = nullptr;
        auto it = branches.find(branchId);
        if (it == branches.end()) {
            throw std::runtime_error("Branch not found: " + branchId);
        }
        branch = it->second;

        // Get available vehicles
        std::vector<Vehicle*> available = branch->getAvailableVehicles(vehicleType);
        if (available.empty()) {
            throw std::runtime_error("No vehicles available of type: " + 
                                     vehicleTypeToString(vehicleType));
        }

        // Select vehicle using strategy
        Vehicle* selectedVehicle = bookingStrategy->selectVehicle(available);
        if (!selectedVehicle) {
            throw std::runtime_error("Failed to select vehicle");
        }

        // Try to reserve atomically
        if (!selectedVehicle->tryReserve()) {
            throw std::runtime_error("Vehicle already booked");
        }

        // Calculate price
        int days = static_cast<int>(difftime(endDate, startDate) / (60 * 60 * 24)) + 1;
        double totalAmount = pricingStrategy->calculatePrice(selectedVehicle, days);

        // Create booking
        std::string bookingId = generateBookingId();
        Booking* booking = new Booking(bookingId, customer, selectedVehicle, 
                                        branch, startDate, endDate, totalAmount);
        booking->confirm();
        
        bookings[bookingId] = booking;
        vehicleBookingCounts[selectedVehicle->getVehicleId()]++;

        return booking;
    }

    void cancelBooking(const std::string& bookingId) {
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = bookings.find(bookingId);
        if (it == bookings.end()) {
            throw std::runtime_error("Booking not found: " + bookingId);
        }
        it->second->cancel();
    }

    void completeBooking(const std::string& bookingId) {
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = bookings.find(bookingId);
        if (it == bookings.end()) {
            throw std::runtime_error("Booking not found: " + bookingId);
        }
        it->second->complete();
    }

    Booking* getBooking(const std::string& bookingId) {
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = bookings.find(bookingId);
        return (it != bookings.end()) ? it->second : nullptr;
    }

    std::vector<Booking*> getAllBookings() const {
        std::lock_guard<std::mutex> lock(mutex_);
        std::vector<Booking*> result;
        for (const auto& [id, booking] : bookings) {
            result.push_back(booking);
        }
        return result;
    }

    std::unordered_map<std::string, int>& getVehicleBookingCounts() {
        return vehicleBookingCounts;
    }

    std::string getActiveStrategies() const {
        return "Booking: " + bookingStrategy->getStrategyName() + 
               ", Pricing: " + pricingStrategy->getStrategyName();
    }
};

#endif // BOOKING_SERVICE_HPP
