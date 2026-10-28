/*
 * LLD Car Rental System
 * 
 * This implementation demonstrates a complete car rental system with:
 * - Multiple stores across locations
 * - Vehicle inventory management (Cars, Bikes)
 * - Reservation system with different rental types
 * - Billing with taxes, insurance, penalties, discounts
 * - Payment processing
 * 
 * Design Patterns Used:
 * - Singleton: VehicleRentalSystem
 * - Factory: Can be extended for vehicle creation
 * - Strategy: Different pricing strategies based on rental type
 */

#include <iostream>
#include <memory>
#include "VehicleRentalSystem.hpp"

using namespace CarRentalSystem;

void setupTestData() {
    auto& system = VehicleRentalSystem::getInstance();

    // Create locations
    auto loc1 = std::make_shared<Location>("LOC1", "Downtown Store", 
        "123 Main St", "New York", "NY", "10001", 40.7128, -74.0060);
    auto loc2 = std::make_shared<Location>("LOC2", "Airport Store",
        "456 Airport Rd", "New York", "NY", "10002", 40.6413, -73.7781);

    // Create stores
    auto store1 = std::make_shared<Store>("S1", "Downtown Car Rental", loc1);
    auto store2 = std::make_shared<Store>("S2", "Airport Car Rental", loc2);

    // Add cars to store 1
    store1->addVehicle(std::make_shared<Car>(
        "C1", "ABC-1234", "Toyota", "Camry", 2023, 50.0, CarType::SEDAN, 5, true, "Petrol"
    ));
    store1->addVehicle(std::make_shared<Car>(
        "C2", "XYZ-5678", "Honda", "Accord", 2022, 55.0, CarType::SEDAN, 5, true, "Hybrid"
    ));
    store1->addVehicle(std::make_shared<Car>(
        "C3", "DEF-9012", "BMW", "M3", 2024, 150.0, CarType::SPORTS, 4, true, "Petrol"
    ));

    // Add bikes to store 1
    store1->addVehicle(std::make_shared<Bike>(
        "B1", "BIKE-001", "Honda", "CBR500R", 2023, 30.0, BikeType::SPORTS, 500, true
    ));

    // Add cars to store 2
    store2->addVehicle(std::make_shared<Car>(
        "C4", "AIR-1111", "Ford", "Explorer", 2023, 80.0, CarType::SEDAN, 7, true, "Petrol"
    ));
    store2->addVehicle(std::make_shared<Car>(
        "C5", "AIR-2222", "Chevrolet", "Tahoe", 2024, 100.0, CarType::SEDAN, 7, true, "Diesel"
    ));

    // Register stores
    system.addStore(store1);
    system.addStore(store2);

    // Register users
    auto user1 = std::make_shared<User>(
        "U1", "John Doe", "john@email.com", "555-1234", "DL-123456789"
    );
    auto user2 = std::make_shared<User>(
        "U2", "Jane Smith", "jane@email.com", "555-5678", "DL-987654321"
    );

    system.registerUser(user1);
    system.registerUser(user2);
}

void demonstrateRentalFlow() {
    std::cout << "\n========================================" << std::endl;
    std::cout << "   CAR RENTAL SYSTEM DEMONSTRATION" << std::endl;
    std::cout << "========================================\n" << std::endl;

    auto& system = VehicleRentalSystem::getInstance();

    // Display system status
    system.displaySystemStatus();

    // Display store info
    auto stores = system.getAllStores();
    for (const auto& store : stores) {
        store->displayStoreInfo();
        store->getInventory()->displayInventory();
    }

    // Search for available cars
    std::cout << "\n--- Searching for available CARs ---" << std::endl;
    auto availableCars = system.searchAvailableVehicles(VehicleType::CAR);
    for (const auto& [store, vehicle] : availableCars) {
        std::cout << "Store: " << store->getName() << " -> " << vehicle->getVehicleInfo() << std::endl;
    }

    // Make a reservation
    std::cout << "\n--- Making a Reservation ---" << std::endl;
    auto dropoffLoc = std::make_shared<Location>("LOC2", "Airport Store",
        "456 Airport Rd", "New York", "NY", "10002");
    
    time_t pickupTime = std::time(nullptr) + 86400; // Tomorrow
    
    auto reservation = system.makeReservation(
        "S1",           // Store ID
        "U1",           // User ID
        "C1",           // Vehicle ID
        dropoffLoc,     // Dropoff location
        pickupTime,     // Pickup time
        3,              // Duration
        ReservationType::DAILY
    );

    if (reservation) {
        reservation->displayReservation();

        // Activate the reservation (pickup)
        std::cout << "\n--- Picking up the vehicle ---" << std::endl;
        reservation->activate();

        // Generate and display bill
        std::cout << "\n--- Generating Bill ---" << std::endl;
        auto bill = system.generateBill(reservation);
        bill->displayBill();

        // Process payment
        std::cout << "\n--- Processing Payment ---" << std::endl;
        auto payment = system.processPayment(bill, PaymentMode::CREDIT_CARD);
        if (payment) {
            payment->displayPayment();
        }

        // Complete the rental
        std::cout << "\n--- Completing Rental ---" << std::endl;
        system.completeRental("S1", reservation->getReservationId(), 250, 0, 0);
    }

    // Final status
    system.displaySystemStatus();
}

void demonstrateLateReturn() {
    std::cout << "\n========================================" << std::endl;
    std::cout << "   LATE RETURN WITH PENALTY DEMO" << std::endl;
    std::cout << "========================================\n" << std::endl;

    auto& system = VehicleRentalSystem::getInstance();

    // Make another reservation
    auto dropoffLoc = std::make_shared<Location>("LOC1", "Downtown Store",
        "123 Main St", "New York", "NY", "10001");
    
    auto reservation = system.makeReservation(
        "S2", "U2", "C4", dropoffLoc,
        std::time(nullptr), 2, ReservationType::DAILY
    );

    if (reservation) {
        reservation->activate();

        // Complete with late return and damage
        std::cout << "\n--- Vehicle returned LATE with DAMAGE ---" << std::endl;
        system.completeRental("S2", reservation->getReservationId(), 
            180,    // km driven
            5,      // 5 hours late
            50.0    // $50 damage
        );
    }
}

void demonstrateBikeRental() {
    std::cout << "\n========================================" << std::endl;
    std::cout << "      BIKE RENTAL DEMONSTRATION" << std::endl;
    std::cout << "========================================\n" << std::endl;

    auto& system = VehicleRentalSystem::getInstance();

    // Search for bikes
    std::cout << "--- Searching for available BIKEs ---" << std::endl;
    auto availableBikes = system.searchAvailableVehicles(VehicleType::BIKE);
    for (const auto& [store, vehicle] : availableBikes) {
        std::cout << "Store: " << store->getName() << " -> " << vehicle->getVehicleInfo() << std::endl;
        vehicle->displayDetails();
    }

    // Make hourly bike rental
    auto dropoffLoc = std::make_shared<Location>("LOC1", "Downtown Store",
        "123 Main St", "New York", "NY", "10001");
    
    auto reservation = system.makeReservation(
        "S1", "U1", "B1", dropoffLoc,
        std::time(nullptr), 4, ReservationType::HOURLY
    );

    if (reservation) {
        reservation->displayReservation();
        reservation->activate();

        auto bill = system.generateBill(reservation);
        bill->applyPercentageDiscount(10); // 10% discount
        bill->displayBill();
    }
}

int main() {
    try {
        // Setup test data
        setupTestData();

        // Run demonstrations
        demonstrateRentalFlow();
        demonstrateLateReturn();
        demonstrateBikeRental();

        std::cout << "\n========================================" << std::endl;
        std::cout << "   ALL DEMONSTRATIONS COMPLETED!" << std::endl;
        std::cout << "========================================" << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
