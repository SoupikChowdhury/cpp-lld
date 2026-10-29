#ifndef CASH_PAYMENT_HPP
#define CASH_PAYMENT_HPP

#include <iostream>
#include "PaymentStrategy.hpp"

class CashPayment : public PaymentStrategy {
public:
    bool processPayment(double amount) override {
        std::cout << "Processing cash payment of Rs. " << amount << std::endl;
        std::cout << "Cash payment successful!" << std::endl;
        return true;
    }
};

#endif // CASH_PAYMENT_HPP
