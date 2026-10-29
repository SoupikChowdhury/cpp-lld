#include <iostream>
#include <vector>
#include <memory>
#include "BookMyShow.hpp"

void demonstrateBookingFlow() {
    std::cout << "\n";
    std::cout << "╔══════════════════════════════════════════════════════════════╗\n";
    std::cout << "║           BookMyShow - Movie Ticket Booking System           ║\n";
    std::cout << "╚══════════════════════════════════════════════════════════════╝\n";

    // Get BookMyShow instance
    auto bookMyShow = BookMyShow::getInstance();
    
    // Initialize with sample data
    bookMyShow->initialize();

    // ==================== Step 1: Browse Movies ====================
    std::cout << "\n========== STEP 1: Browse Movies in Bangalore ==========\n";
    bookMyShow->printMoviesInCity(City::BANGALORE);

    // ==================== Step 2: Select Movie and View Shows ====================
    std::cout << "\n========== STEP 2: View Shows for 'Avengers: Endgame' ==========\n";
    bookMyShow->printShowsForMovie("Avengers: Endgame", City::BANGALORE);

    // ==================== Step 3: Get Shows and Select One ====================
    std::cout << "\n========== STEP 3: Select a Show and View Available Seats ==========\n";
    
    auto theatreShows = bookMyShow->getShowsForMovie("Avengers: Endgame", City::BANGALORE);
    
    if (theatreShows.empty()) {
        std::cout << "No shows available.\n";
        return;
    }

    // Get first theatre and its first show
    auto firstTheatre = theatreShows.begin()->first;
    auto firstShow = theatreShows.begin()->second.front();

    std::cout << "Selected Show: " << firstShow->toString() << "\n";
    std::cout << "Theatre: " << firstTheatre->getTheatreName() << "\n";

    // ==================== Step 4: View Available Seats ====================
    std::cout << "\n========== STEP 4: Available Seats by Category ==========\n";
    
    auto silverSeats = bookMyShow->getAvailableSeatsByCategory(firstShow, SeatCategory::SILVER);
    auto goldSeats = bookMyShow->getAvailableSeatsByCategory(firstShow, SeatCategory::GOLD);
    auto platinumSeats = bookMyShow->getAvailableSeatsByCategory(firstShow, SeatCategory::PLATINUM);

    std::cout << "Silver Seats (Rs." << getSeatPrice(SeatCategory::SILVER) << "): " 
              << silverSeats.size() << " available\n";
    std::cout << "Gold Seats (Rs." << getSeatPrice(SeatCategory::GOLD) << "): " 
              << goldSeats.size() << " available\n";
    std::cout << "Platinum Seats (Rs." << getSeatPrice(SeatCategory::PLATINUM) << "): " 
              << platinumSeats.size() << " available\n";

    // Print first few seats of each category
    std::cout << "\nSample Silver Seats: ";
    for (size_t i = 0; i < std::min(size_t(5), silverSeats.size()); ++i) {
        std::cout << silverSeats[i]->getSeatLabel() << " ";
    }
    std::cout << "\n";

    std::cout << "Sample Gold Seats: ";
    for (size_t i = 0; i < std::min(size_t(5), goldSeats.size()); ++i) {
        std::cout << goldSeats[i]->getSeatLabel() << " ";
    }
    std::cout << "\n";

    std::cout << "Sample Platinum Seats: ";
    for (size_t i = 0; i < std::min(size_t(5), platinumSeats.size()); ++i) {
        std::cout << platinumSeats[i]->getSeatLabel() << " ";
    }
    std::cout << "\n";

    // ==================== Step 5: Book Seats ====================
    std::cout << "\n========== STEP 5: Book 3 Gold Seats ==========\n";
    
    // Select 3 gold seats
    std::vector<std::shared_ptr<Seat>> selectedSeats;
    for (size_t i = 0; i < 3 && i < goldSeats.size(); ++i) {
        selectedSeats.push_back(goldSeats[i]);
    }

    std::cout << "Selected Seats: ";
    for (const auto& seat : selectedSeats) {
        std::cout << seat->getSeatLabel() << " ";
    }
    std::cout << "\n";

    // Create booking
    auto booking1 = bookMyShow->createBooking(
        firstShow, selectedSeats, 
        "John Doe", "9876543210", 
        PaymentMethod::UPI
    );

    if (booking1) {
        std::cout << "\n✓ Booking Successful!\n";
        std::cout << booking1->generateTicket();
    }

    // ==================== Step 6: Try Another Booking ====================
    std::cout << "\n========== STEP 6: Book 2 Platinum Seats ==========\n";
    
    std::vector<std::shared_ptr<Seat>> moreSeats;
    for (size_t i = 0; i < 2 && i < platinumSeats.size(); ++i) {
        moreSeats.push_back(platinumSeats[i]);
    }

    auto booking2 = bookMyShow->createBooking(
        firstShow, moreSeats,
        "Jane Smith", "8765432109",
        PaymentMethod::CREDIT_CARD
    );

    if (booking2) {
        std::cout << "\n✓ Booking Successful!\n";
        std::cout << booking2->generateTicket();
    }

    // ==================== Step 7: Check Updated Availability ====================
    std::cout << "\n========== STEP 7: Updated Seat Availability ==========\n";
    
    goldSeats = bookMyShow->getAvailableSeatsByCategory(firstShow, SeatCategory::GOLD);
    platinumSeats = bookMyShow->getAvailableSeatsByCategory(firstShow, SeatCategory::PLATINUM);

    std::cout << "Gold Seats Now Available: " << goldSeats.size() << "\n";
    std::cout << "Platinum Seats Now Available: " << platinumSeats.size() << "\n";
    std::cout << "Total Available: " << firstShow->getAvailableSeatCount() << " seats\n";

    // ==================== Step 8: Cancel a Booking ====================
    std::cout << "\n========== STEP 8: Cancel First Booking ==========\n";
    
    if (booking1 && bookMyShow->cancelBooking(booking1->getBookingId())) {
        std::cout << "✓ Booking " << booking1->getBookingId() << " cancelled successfully!\n";
        std::cout << "Refund Status: " << paymentStatusToString(booking1->getPayment()->getStatus()) << "\n";
    }

    // ==================== Step 9: Verify Seats Released ====================
    std::cout << "\n========== STEP 9: Verify Seats Released ==========\n";
    
    goldSeats = bookMyShow->getAvailableSeatsByCategory(firstShow, SeatCategory::GOLD);
    std::cout << "Gold Seats Now Available: " << goldSeats.size() << " (3 seats released)\n";
    std::cout << "Total Available: " << firstShow->getAvailableSeatCount() << " seats\n";

    // ==================== Step 10: Browse Other Cities ====================
    std::cout << "\n========== STEP 10: Browse Movies in Mumbai ==========\n";
    bookMyShow->printMoviesInCity(City::MUMBAI);
    bookMyShow->printShowsForMovie("Avengers: Endgame", City::MUMBAI);

    // ==================== Summary ====================
    std::cout << "\n========== BOOKING SUMMARY ==========\n";
    const auto& allBookings = bookMyShow->getAllBookings();
    std::cout << "Total Bookings in System: " << allBookings.size() << "\n";
    for (const auto& booking : allBookings) {
        std::cout << "  - " << booking->toString() << "\n";
    }

    std::cout << "\n✓ Demo completed successfully!\n";
}

int main() {
    try {
        demonstrateBookingFlow();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
