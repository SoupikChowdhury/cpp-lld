// Parking Lot System - Main Entry Point
// Demonstrates Singleton, Factory, Strategy, and Builder patterns

#include <iostream>
#include <chrono>
#include <thread>
#include "model/ParkingSpot.hpp"
#include "model/ParkingFloor.hpp"
#include "model/EntryGate.hpp"
#include "model/ExitGate.hpp"
#include "factory/VehicleFactory.hpp"
#include "factory/PricingStrategyFactory.hpp"
#include "service/ParkingLot.hpp"

int main() {
    std::cout << "=== Parking Lot System Demo ===" << std::endl;
    std::cout << std::endl;

    // Get singleton instance
    ParkingLot* lot = ParkingLot::getInstance();

    // Set pricing strategy (Event-based for this demo)
    lot->setPricingStrategy(PricingStrategyFactory::get(PricingStrategyType::EVENT_BASED));

    // Create parking floors
    auto floor1 = std::make_shared<ParkingFloor>("Floor1");
    floor1->addSpot(std::make_shared<ParkingSpot>("F1S1", VehicleType::BIKE));
    floor1->addSpot(std::make_shared<ParkingSpot>("F1S2", VehicleType::CAR));
    floor1->addSpot(std::make_shared<ParkingSpot>("F1S3", VehicleType::CAR));

    auto floor2 = std::make_shared<ParkingFloor>("Floor2");
    floor2->addSpot(std::make_shared<ParkingSpot>("F2S1", VehicleType::CAR));
    floor2->addSpot(std::make_shared<ParkingSpot>("F2S2", VehicleType::TRUCK));

    lot->addFloor(floor1);
    lot->addFloor(floor2);

    // Create gates
    EntryGate entryGate("EG1");
    ExitGate exitGate("XG1");

    // Create vehicles using factory
    auto car1 = VehicleFactory::create(VehicleType::CAR, "KA-01-AB-1234");
    auto bike1 = VehicleFactory::create(VehicleType::BIKE, "KA-01-CD-5678");
    auto truck1 = VehicleFactory::create(VehicleType::TRUCK, "KA-01-EF-9012");

    std::cout << "--- Vehicle Entry ---" << std::endl;
    
    // Get current time as entry time
    auto entryTime1 = std::chrono::system_clock::now();
    
    // Park vehicles
    std::cout << std::endl << "Parking Car:" << std::endl;
    Ticket* ticket1 = entryGate.parkVehicle(car1, entryTime1);

    std::cout << std::endl << "Parking Bike:" << std::endl;
    Ticket* ticket2 = entryGate.parkVehicle(bike1, entryTime1);

    std::cout << std::endl << "Parking Truck:" << std::endl;
    Ticket* ticket3 = entryGate.parkVehicle(truck1, entryTime1);

    std::cout << std::endl << "Active tickets: " << lot->getActiveTicketsCount() << std::endl;

    // Simulate some time passing (using exit time 2 hours later)
    auto exitTime = entryTime1 + std::chrono::hours(2);

    std::cout << std::endl << "--- Vehicle Exit ---" << std::endl;
    
    if (ticket1) {
        std::cout << std::endl << "Exiting Car (Card Payment):" << std::endl;
        exitGate.unparkVehicle(ticket1->getId(), exitTime, PaymentMode::CARD);
    }

    if (ticket2) {
        std::cout << std::endl << "Exiting Bike (Cash Payment):" << std::endl;
        exitGate.unparkVehicle(ticket2->getId(), exitTime, PaymentMode::CASH);
    }

    if (ticket3) {
        std::cout << std::endl << "Exiting Truck (UPI Payment):" << std::endl;
        exitGate.unparkVehicle(ticket3->getId(), exitTime, PaymentMode::UPI);
    }

    std::cout << std::endl << "Active tickets remaining: " << lot->getActiveTicketsCount() << std::endl;

    std::cout << std::endl << "=== Demo Complete ===" << std::endl;

    return 0;
}
