#pragma once

#include <string>
#include <ctime>
#include <iostream>
#include "PaymentMode.hpp"

namespace CarRentalSystem {

enum class PaymentStatus {
    PENDING,
    COMPLETED,
    FAILED,
    REFUNDED
};

inline const char* paymentStatusToString(PaymentStatus status) {
    switch (status) {
        case PaymentStatus::PENDING: return "PENDING";
        case PaymentStatus::COMPLETED: return "COMPLETED";
        case PaymentStatus::FAILED: return "FAILED";
        case PaymentStatus::REFUNDED: return "REFUNDED";
        default: return "UNKNOWN";
    }
}

class Payment {
private:
    std::string paymentId;
    std::string reservationId;
    double amount;
    PaymentMode mode;
    PaymentStatus status;
    time_t paidAt;
    std::string transactionId;

public:
    Payment(const std::string& id, const std::string& reservationId, 
            double amount, PaymentMode mode)
        : paymentId(id), reservationId(reservationId), amount(amount),
          mode(mode), status(PaymentStatus::PENDING), paidAt(0) {}

    // Getters
    const std::string& getPaymentId() const { return paymentId; }
    const std::string& getReservationId() const { return reservationId; }
    double getAmount() const { return amount; }
    PaymentMode getMode() const { return mode; }
    PaymentStatus getStatus() const { return status; }
    time_t getPaidAt() const { return paidAt; }
    const std::string& getTransactionId() const { return transactionId; }

    bool processPayment() {
        // Simulate payment processing
        std::cout << "Processing payment of $" << amount 
                  << " via " << paymentModeToString(mode) << "..." << std::endl;
        
        // Simulate success (in real system, call payment gateway)
        status = PaymentStatus::COMPLETED;
        paidAt = std::time(nullptr);
        transactionId = "TXN" + std::to_string(paidAt);
        
        std::cout << "Payment successful! Transaction ID: " << transactionId << std::endl;
        return true;
    }

    bool refund() {
        if (status == PaymentStatus::COMPLETED) {
            status = PaymentStatus::REFUNDED;
            std::cout << "Refund processed for $" << amount << std::endl;
            return true;
        }
        return false;
    }

    void displayPayment() const {
        std::cout << "\n=== Payment Details ===" << std::endl;
        std::cout << "Payment ID: " << paymentId << std::endl;
        std::cout << "Reservation: " << reservationId << std::endl;
        std::cout << "Amount: $" << amount << std::endl;
        std::cout << "Mode: " << paymentModeToString(mode) << std::endl;
        std::cout << "Status: " << paymentStatusToString(status) << std::endl;
        if (!transactionId.empty()) {
            std::cout << "Transaction ID: " << transactionId << std::endl;
        }
    }
};

} // namespace CarRentalSystem
