#ifndef PARKING_LOT_HPP
#define PARKING_LOT_HPP

#include <iostream>
#include <string>
#include <unordered_map>
#include <memory>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <random>
#include "../model/ParkingFloor.hpp"
#include "../model/Ticket.hpp"
#include "../model/Vehicle.hpp"
#include "../strategy/pricing/PricingStrategy.hpp"
#include "../strategy/pricing/TimeBasedPricing.hpp"
#include "../factory/PricingStrategyFactory.hpp"
#include "../factory/PaymentStrategyFactory.hpp"
#include "PaymentService.hpp"

class ParkingLot {
private:
    static ParkingLot* instance;
    std::unordered_map<std::string, std::shared_ptr<ParkingFloor>> floors;
    std::unordered_map<std::string, std::unique_ptr<Ticket>> activeTickets;
    std::unique_ptr<PricingStrategy> pricingStrategy;

    // Private constructor for Singleton
    ParkingLot() {
        pricingStrategy = std::make_unique<TimeBasedPricing>();
    }

    std::string generateTicketId() {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<> dis(10000, 99999);
        return "TKT-" + std::to_string(dis(gen));
    }

public:
    // Delete copy constructor and assignment operator
    ParkingLot(const ParkingLot&) = delete;
    ParkingLot& operator=(const ParkingLot&) = delete;

    static ParkingLot* getInstance() {
        if (instance == nullptr) {
            instance = new ParkingLot();
        }
        return instance;
    }

    void addFloor(std::shared_ptr<ParkingFloor> floor) {
        floors[floor->getId()] = floor;
    }

    void setPricingStrategy(std::unique_ptr<PricingStrategy> strategy) {
        pricingStrategy = std::move(strategy);
    }

    Ticket* parkVehicle(std::shared_ptr<Vehicle> vehicle, 
                        std::chrono::system_clock::time_point entryTime) {
        // Find available spot
        for (auto& [floorId, floor] : floors) {
            auto spot = floor->findAvailableSpot(vehicle->getType());
            if (spot) {
                std::string ticketId = generateTicketId();
                
                auto ticket = Ticket::Builder()
                    .setId(ticketId)
                    .setVehicle(vehicle)
                    .setSpot(spot)
                    .setEntryTime(entryTime)
                    .build();

                Ticket* ticketPtr = ticket.get();
                activeTickets[ticketId] = std::move(ticket);

                std::cout << "Vehicle " << vehicle->getLicensePlate() 
                          << " parked at spot " << spot->getId() 
                          << " on floor " << floorId << std::endl;
                std::cout << "Ticket ID: " << ticketId << std::endl;

                return ticketPtr;
            }
        }

        std::cout << "No available spot for vehicle type." << std::endl;
        return nullptr;
    }

    void unparkVehicle(const std::string& ticketId, 
                       std::chrono::system_clock::time_point exitTime,
                       PaymentMode paymentMode) {
        auto it = activeTickets.find(ticketId);
        if (it == activeTickets.end()) {
            std::cout << "Invalid ticket ID." << std::endl;
            return;
        }

        Ticket* ticket = it->second.get();
        ticket->setExitTime(exitTime);

        // Calculate duration in hours
        auto duration = std::chrono::duration_cast<std::chrono::hours>(
            exitTime - ticket->getEntryTime()
        ).count();
        
        // Minimum 1 hour
        if (duration < 1) duration = 1;

        // Calculate fee
        double fee = pricingStrategy->calculateFee(
            ticket->getVehicle()->getType(), 
            duration
        );
        ticket->setFee(fee);

        std::cout << "Duration: " << duration << " hour(s)" << std::endl;
        std::cout << "Parking fee: Rs. " << fee << std::endl;

        // Process payment
        auto paymentStrategy = PaymentStrategyFactory::get(paymentMode);
        PaymentService paymentService;
        
        if (paymentService.processPayment(fee, paymentStrategy.get())) {
            // Release spot
            ticket->getSpot()->release();
            
            std::cout << "Vehicle " << ticket->getVehicle()->getLicensePlate() 
                      << " has exited." << std::endl;
            
            // Remove ticket
            activeTickets.erase(it);
        }
    }

    const std::unordered_map<std::string, std::shared_ptr<ParkingFloor>>& getFloors() const {
        return floors;
    }

    size_t getActiveTicketsCount() const {
        return activeTickets.size();
    }
};

// Initialize static member
ParkingLot* ParkingLot::instance = nullptr;

#endif // PARKING_LOT_HPP
