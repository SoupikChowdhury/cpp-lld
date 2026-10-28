// Car Rental System - Main Entry Point
// Demonstrates Strategy, Factory, and Repository patterns

#include <iostream>
#include <iomanip>
#include <ctime>
#include "model/Branch.hpp"
#include "model/Car.hpp"
#include "model/Bike.hpp"
#include "model/SUV.hpp"
#include "model/LuxuryCar.hpp"
#include "model/Customer.hpp"
#include "service/BookingService.hpp"
#include "strategy/booking/CheapestVehicleStrategy.hpp"
#include "strategy/booking/LeastBookedStrategy.hpp"
#include "strategy/pricing/StandardPricing.hpp"
#include "strategy/pricing/WeekendPricing.hpp"
#include "strategy/pricing/DiscountPricing.hpp"

// Helper function to create a date
time_t createDate(int daysFromNow) {
    time_t now = time(nullptr);
    return now + (daysFromNow * 24 * 60 * 60);
}

int main() {
    std::cout << "=== Car Rental System Demo ===" << std::endl;
    std::cout << std::endl;

    // Create branches
    Branch downtown("BR001", "Downtown Branch", "123 Main St, Downtown");
    Branch airport("BR002", "Airport Branch", "Airport Terminal 1");

    // Create vehicles for downtown branch
    Car* car1 = new Car("V001", "Toyota", "Camry", "ABC-1234", 50.0);
    Car* car2 = new Car("V002", "Honda", "Accord", "DEF-5678", 55.0);
    SUV* suv1 = new SUV("V003", "Ford", "Explorer", "GHI-9012", 80.0);
    Bike* bike1 = new Bike("V004", "Harley", "Sportster", "JKL-3456", 30.0);
    
    downtown.addVehicle(car1);
    downtown.addVehicle(car2);
    downtown.addVehicle(suv1);
    downtown.addVehicle(bike1);

    // Create vehicles for airport branch
    LuxuryCar* luxury1 = new LuxuryCar("V005", "Mercedes", "S-Class", "MNO-7890", 200.0);
    Car* car3 = new Car("V006", "BMW", "3 Series", "PQR-1234", 100.0);
    SUV* suv2 = new SUV("V007", "Audi", "Q7", "STU-5678", 150.0);

    airport.addVehicle(luxury1);
    airport.addVehicle(car3);
    airport.addVehicle(suv2);

    // Create booking service and add branches
    BookingService bookingService;
    bookingService.addBranch(&downtown);
    bookingService.addBranch(&airport);

    // Create customers
    Customer john("C001", "John Doe", "john@email.com", "555-0101", "DL12345");
    Customer jane("C002", "Jane Smith", "jane@email.com", "555-0102", "DL67890");
    Customer bob("C003", "Bob Wilson", "bob@email.com", "555-0103", "DL11111");

    std::cout << "--- Branch Setup ---" << std::endl;
    std::cout << "Downtown Branch: " << downtown.getVehicleCount() << " vehicles" << std::endl;
    std::cout << "Airport Branch: " << airport.getVehicleCount() << " vehicles" << std::endl;

    std::cout << std::endl << "--- Available Vehicles at Downtown ---" << std::endl;
    for (Vehicle* v : downtown.getAvailableVehicles()) {
        std::cout << "  " << v->getDescription() << " - $" 
                  << std::fixed << std::setprecision(2) << v->getPricePerDay() << "/day" << std::endl;
    }

    std::cout << std::endl << "--- Booking with Cheapest Vehicle Strategy ---" << std::endl;
    std::cout << "Active Strategies: " << bookingService.getActiveStrategies() << std::endl;

    try {
        // John books a car (cheapest strategy will pick Toyota Camry at $50)
        time_t start1 = createDate(1);  // Tomorrow
        time_t end1 = createDate(3);    // 3 days from now
        
        Booking* booking1 = bookingService.createBooking(&john, "BR001", VehicleType::CAR, start1, end1);
        std::cout << "\nBooking 1 Created:" << std::endl;
        std::cout << "  ID: " << booking1->getBookingId() << std::endl;
        std::cout << "  Customer: " << booking1->getCustomer()->getName() << std::endl;
        std::cout << "  Vehicle: " << booking1->getVehicle()->getDescription() << std::endl;
        std::cout << "  Duration: " << booking1->getDurationDays() << " days" << std::endl;
        std::cout << "  Total: $" << std::fixed << std::setprecision(2) << booking1->getTotalAmount() << std::endl;
        std::cout << "  Status: " << bookingStatusToString(booking1->getStatus()) << std::endl;

    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }

    // Change to weekend pricing
    std::cout << std::endl << "--- Switching to Weekend Pricing (1.5x) ---" << std::endl;
    bookingService.setPricingStrategy(std::make_unique<WeekendPricing>(1.5));
    std::cout << "Active Strategies: " << bookingService.getActiveStrategies() << std::endl;

    try {
        // Jane books a car (only Honda Accord left at $55)
        time_t start2 = createDate(5);
        time_t end2 = createDate(7);
        
        Booking* booking2 = bookingService.createBooking(&jane, "BR001", VehicleType::CAR, start2, end2);
        std::cout << "\nBooking 2 Created:" << std::endl;
        std::cout << "  ID: " << booking2->getBookingId() << std::endl;
        std::cout << "  Customer: " << booking2->getCustomer()->getName() << std::endl;
        std::cout << "  Vehicle: " << booking2->getVehicle()->getDescription() << std::endl;
        std::cout << "  Duration: " << booking2->getDurationDays() << " days" << std::endl;
        std::cout << "  Total (with weekend pricing): $" << std::fixed << std::setprecision(2) << booking2->getTotalAmount() << std::endl;

    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }

    // Switch to discount pricing
    std::cout << std::endl << "--- Switching to Discount Pricing (20% off) ---" << std::endl;
    bookingService.setPricingStrategy(std::make_unique<DiscountPricing>(0.20));
    std::cout << "Active Strategies: " << bookingService.getActiveStrategies() << std::endl;

    try {
        // Bob books a luxury car at airport
        time_t start3 = createDate(10);
        time_t end3 = createDate(12);
        
        Booking* booking3 = bookingService.createBooking(&bob, "BR002", VehicleType::LUXURY, start3, end3);
        std::cout << "\nBooking 3 Created:" << std::endl;
        std::cout << "  ID: " << booking3->getBookingId() << std::endl;
        std::cout << "  Customer: " << booking3->getCustomer()->getName() << std::endl;
        std::cout << "  Vehicle: " << booking3->getVehicle()->getDescription() << std::endl;
        std::cout << "  Duration: " << booking3->getDurationDays() << " days" << std::endl;
        std::cout << "  Total (with 20% discount): $" << std::fixed << std::setprecision(2) << booking3->getTotalAmount() << std::endl;

    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }

    std::cout << std::endl << "--- All Bookings ---" << std::endl;
    for (Booking* b : bookingService.getAllBookings()) {
        std::cout << b->getBookingId() << " | " 
                  << b->getCustomer()->getName() << " | "
                  << b->getVehicle()->getDescription() << " | "
                  << "$" << std::fixed << std::setprecision(2) << b->getTotalAmount() << " | "
                  << bookingStatusToString(b->getStatus()) << std::endl;
    }

    // Complete a booking
    std::cout << std::endl << "--- Completing Booking BK000001 ---" << std::endl;
    bookingService.completeBooking("BK000001");
    Booking* completed = bookingService.getBooking("BK000001");
    std::cout << "Status: " << bookingStatusToString(completed->getStatus()) << std::endl;
    std::cout << "Vehicle " << completed->getVehicle()->getVehicleId() 
              << " available again: " << (completed->getVehicle()->isAvailable() ? "Yes" : "No") << std::endl;

    // Cleanup
    delete car1; delete car2; delete car3;
    delete suv1; delete suv2;
    delete bike1;
    delete luxury1;

    std::cout << std::endl << "=== Demo Complete ===" << std::endl;

    return 0;
}
