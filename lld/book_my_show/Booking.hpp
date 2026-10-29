#ifndef BOOKING_HPP
#define BOOKING_HPP

#include <memory>
#include <vector>
#include <string>
#include <ctime>
#include <iomanip>
#include <sstream>
#include "Show.hpp"
#include "Seat.hpp"
#include "Payment.hpp"

enum class BookingStatus {
    PENDING,
    CONFIRMED,
    CANCELLED,
    EXPIRED
};

inline std::string bookingStatusToString(BookingStatus status) {
    switch (status) {
        case BookingStatus::PENDING: return "Pending";
        case BookingStatus::CONFIRMED: return "Confirmed";
        case BookingStatus::CANCELLED: return "Cancelled";
        case BookingStatus::EXPIRED: return "Expired";
        default: return "Unknown";
    }
}

class Booking {
private:
    int bookingId;
    std::shared_ptr<Show> show;
    std::vector<std::shared_ptr<Seat>> bookedSeats;
    std::shared_ptr<Payment> payment;
    BookingStatus status;
    std::time_t bookingTime;
    std::string customerName;
    std::string customerPhone;
    std::string customerEmail;

    static int nextBookingId;

public:
    Booking(std::shared_ptr<Show> show, const std::vector<std::shared_ptr<Seat>>& seats,
            const std::string& name, const std::string& phone, const std::string& email = "")
        : bookingId(nextBookingId++), show(show), bookedSeats(seats),
          status(BookingStatus::PENDING), bookingTime(std::time(nullptr)),
          customerName(name), customerPhone(phone), customerEmail(email) {}

    // Getters
    int getBookingId() const { return bookingId; }
    std::shared_ptr<Show> getShow() const { return show; }
    const std::vector<std::shared_ptr<Seat>>& getBookedSeats() const { return bookedSeats; }
    std::shared_ptr<Payment> getPayment() const { return payment; }
    BookingStatus getStatus() const { return status; }
    std::time_t getBookingTime() const { return bookingTime; }
    std::string getCustomerName() const { return customerName; }
    std::string getCustomerPhone() const { return customerPhone; }
    std::string getCustomerEmail() const { return customerEmail; }

    // Calculate total amount
    double calculateTotalAmount() const {
        double total = 0.0;
        for (const auto& seat : bookedSeats) {
            total += seat->getPrice();
        }
        return total;
    }

    // Make payment
    bool makePayment(PaymentMethod method) {
        double amount = calculateTotalAmount();
        payment = std::make_shared<Payment>(bookingId, amount, method);
        
        if (payment->processPayment()) {
            // Book seats in the show
            if (show->bookSeats(bookedSeats)) {
                status = BookingStatus::CONFIRMED;
                return true;
            } else {
                payment->refund();
                status = BookingStatus::CANCELLED;
                return false;
            }
        }
        return false;
    }

    // Cancel booking
    bool cancelBooking() {
        if (status == BookingStatus::CONFIRMED) {
            show->cancelSeats(bookedSeats);
            if (payment) {
                payment->refund();
            }
            status = BookingStatus::CANCELLED;
            return true;
        }
        return false;
    }

    std::string getBookingTimeFormatted() const {
        std::tm* tm = std::localtime(&bookingTime);
        std::ostringstream oss;
        oss << std::put_time(tm, "%Y-%m-%d %H:%M:%S");
        return oss.str();
    }

    std::string generateTicket() const {
        std::ostringstream oss;
        oss << "\n========================================\n";
        oss << "          MOVIE TICKET                  \n";
        oss << "========================================\n";
        oss << "Booking ID: BMS" << std::setfill('0') << std::setw(6) << bookingId << "\n";
        oss << "----------------------------------------\n";
        oss << "Movie: " << show->getMovie()->getMovieName() << "\n";
        oss << "Show Time: " << show->getStartTimeFormatted() << "\n";
        oss << "Screen: " << show->getScreen()->getScreenName() << "\n";
        oss << "----------------------------------------\n";
        oss << "Seats: ";
        for (size_t i = 0; i < bookedSeats.size(); ++i) {
            oss << bookedSeats[i]->getSeatLabel();
            if (i < bookedSeats.size() - 1) oss << ", ";
        }
        oss << "\n";
        oss << "----------------------------------------\n";
        oss << "Customer: " << customerName << "\n";
        oss << "Phone: " << customerPhone << "\n";
        oss << "----------------------------------------\n";
        oss << "Total Amount: Rs." << static_cast<int>(calculateTotalAmount()) << "\n";
        oss << "Payment Status: " << (payment ? paymentStatusToString(payment->getStatus()) : "N/A") << "\n";
        oss << "Booking Status: " << bookingStatusToString(status) << "\n";
        oss << "========================================\n";
        return oss.str();
    }

    std::string toString() const {
        std::string seatLabels;
        for (size_t i = 0; i < bookedSeats.size(); ++i) {
            seatLabels += bookedSeats[i]->getSeatLabel();
            if (i < bookedSeats.size() - 1) seatLabels += ", ";
        }
        return "Booking[ID: " + std::to_string(bookingId) + 
               ", Movie: " + show->getMovie()->getMovieName() +
               ", Seats: " + seatLabels +
               ", Status: " + bookingStatusToString(status) + "]";
    }
};

// Initialize static member
inline int Booking::nextBookingId = 1000;

#endif // BOOKING_HPP
