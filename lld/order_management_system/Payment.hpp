#pragma once

#include <memory>
#include <iostream>
#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>
#include "PaymentMode.hpp"

namespace OrderManagement {

/**
 * @brief Enum for payment status
 */
enum class PaymentStatus {
    PENDING,
    COMPLETED,
    FAILED,
    REFUNDED
};

inline std::string paymentStatusToString(PaymentStatus status) {
    switch (status) {
        case PaymentStatus::PENDING: return "Pending";
        case PaymentStatus::COMPLETED: return "Completed";
        case PaymentStatus::FAILED: return "Failed";
        case PaymentStatus::REFUNDED: return "Refunded";
        default: return "Unknown";
    }
}

/**
 * @brief Class representing a payment transaction
 */
class Payment {
private:
    std::string paymentId_;
    std::string orderId_;
    double amount_;
    std::unique_ptr<PaymentMode> paymentMode_;
    PaymentStatus status_;
    std::string transactionTime_;

    std::string getCurrentTimestamp() {
        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
        return ss.str();
    }

public:
    Payment(const std::string& paymentId, const std::string& orderId,
            double amount, std::unique_ptr<PaymentMode> paymentMode)
        : paymentId_(paymentId)
        , orderId_(orderId)
        , amount_(amount)
        , paymentMode_(std::move(paymentMode))
        , status_(PaymentStatus::PENDING)
        , transactionTime_("") {}

    // Getters
    std::string getPaymentId() const { return paymentId_; }
    std::string getOrderId() const { return orderId_; }
    double getAmount() const { return amount_; }
    PaymentStatus getStatus() const { return status_; }
    std::string getTransactionTime() const { return transactionTime_; }

    /**
     * @brief Process the payment
     */
    bool processPayment() {
        if (!paymentMode_) {
            std::cout << "No payment mode set!\n";
            status_ = PaymentStatus::FAILED;
            return false;
        }

        if (paymentMode_->processPayment(amount_)) {
            status_ = PaymentStatus::COMPLETED;
            transactionTime_ = getCurrentTimestamp();
            return true;
        }

        status_ = PaymentStatus::FAILED;
        return false;
    }

    /**
     * @brief Refund the payment
     */
    bool refund() {
        if (status_ != PaymentStatus::COMPLETED) {
            std::cout << "Cannot refund: Payment not completed\n";
            return false;
        }

        if (paymentMode_->refund(amount_)) {
            status_ = PaymentStatus::REFUNDED;
            return true;
        }
        return false;
    }

    std::string toString() const {
        std::string str = "Payment[" + paymentId_ + "] for Order[" + orderId_ + "]\n";
        str += "  Amount: $" + std::to_string(amount_) + "\n";
        str += "  Mode: " + (paymentMode_ ? paymentMode_->getPaymentModeName() : "None") + "\n";
        str += "  Status: " + paymentStatusToString(status_) + "\n";
        if (!transactionTime_.empty()) {
            str += "  Transaction Time: " + transactionTime_;
        }
        return str;
    }
};

} // namespace OrderManagement
