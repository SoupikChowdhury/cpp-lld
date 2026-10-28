#ifndef BOOKING_HPP
#define BOOKING_HPP

#include <string>
#include <ctime>
#include <stdexcept>
#include "Customer.hpp"
#include "Vehicle.hpp"
#include "Branch.hpp"
#include "../enums/BookingStatus.hpp"

class Booking {
private:
    std::string bookingId;
    Customer* customer;
    Vehicle* vehicle;
    Branch* branch;
    time_t startDate;
    time_t endDate;
    double totalAmount;
    BookingStatus status;

public:
    Booking(const std::string& bookingId, Customer* customer, 
            Vehicle* vehicle, Branch* branch,
            time_t startDate, time_t endDate, double totalAmount)
        : bookingId(bookingId), customer(customer), vehicle(vehicle),
          branch(branch), startDate(startDate), endDate(endDate),
          totalAmount(totalAmount), status(BookingStatus::PENDING) {}

    // Getters
    std::string getBookingId() const { return bookingId; }
    Customer* getCustomer() const { return customer; }
    Vehicle* getVehicle() const { return vehicle; }
    Branch* getBranch() const { return branch; }
    time_t getStartDate() const { return startDate; }
    time_t getEndDate() const { return endDate; }
    double getTotalAmount() const { return totalAmount; }
    BookingStatus getStatus() const { return status; }

    int getDurationDays() const {
        return static_cast<int>(difftime(endDate, startDate) / (60 * 60 * 24)) + 1;
    }

    void confirm() {
        if (status != BookingStatus::PENDING) {
            throw std::runtime_error("Cannot confirm booking in " + 
                                     bookingStatusToString(status) + " state");
        }
        status = BookingStatus::CONFIRMED;
    }

    void cancel() {
        if (status == BookingStatus::COMPLETED) {
            throw std::runtime_error("Cannot cancel a completed booking");
        }
        status = BookingStatus::CANCELLED;
        if (vehicle) {
            vehicle->release();
        }
    }

    void complete() {
        if (status != BookingStatus::CONFIRMED) {
            throw std::runtime_error("Cannot complete booking in " + 
                                     bookingStatusToString(status) + " state");
        }
        status = BookingStatus::COMPLETED;
        if (vehicle) {
            vehicle->release();
        }
    }
};

#endif // BOOKING_HPP
