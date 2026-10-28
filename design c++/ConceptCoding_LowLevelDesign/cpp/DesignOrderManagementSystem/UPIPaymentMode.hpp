#pragma once

#include "PaymentMode.hpp"
#include <iostream>
#include <regex>

namespace OrderManagement {

/**
 * @brief Concrete implementation of UPI payment mode
 */
class UPIPaymentMode : public PaymentMode {
private:
    std::string upiId_;
    std::string pin_;

public:
    UPIPaymentMode(const std::string& upiId, const std::string& pin)
        : upiId_(upiId), pin_(pin) {}

    bool processPayment(double amount) override {
        if (!validateDetails()) {
            std::cout << "UPI Payment Failed: Invalid UPI details\n";
            return false;
        }

        // Simulate UPI payment processing
        std::cout << "Processing UPI payment of $" << amount 
                  << " from " << upiId_ << "...\n";
        
        // Simulate successful payment
        std::cout << "UPI Payment Successful!\n";
        return true;
    }

    std::string getPaymentModeName() const override {
        return "UPI Payment";
    }

    bool validateDetails() const override {
        // Basic UPI ID format validation (name@bank)
        std::regex upiPattern(R"([a-zA-Z0-9.]+@[a-zA-Z]+)");
        return std::regex_match(upiId_, upiPattern) && pin_.length() >= 4;
    }

    bool refund(double amount) override {
        std::cout << "Initiating UPI refund of $" << amount 
                  << " to " << upiId_ << "...\n";
        std::cout << "UPI Refund Successful!\n";
        return true;
    }

    // Getters
    std::string getUpiId() const { return upiId_; }
};

} // namespace OrderManagement
