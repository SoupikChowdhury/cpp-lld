#ifndef UPI_PAYMENT_HPP
#define UPI_PAYMENT_HPP

#include <iostream>
#include "PaymentStrategy.hpp"

class UPIPayment : public PaymentStrategy {
public:
    bool processPayment(double amount) override {
        std::cout << "Processing UPI payment of Rs. " << amount << std::endl;
        std::cout << "UPI payment successful!" << std::endl;
        return true;
    }
};

#endif // UPI_PAYMENT_HPP
