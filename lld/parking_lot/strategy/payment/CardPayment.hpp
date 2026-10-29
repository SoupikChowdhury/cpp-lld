#ifndef CARD_PAYMENT_HPP
#define CARD_PAYMENT_HPP

#include <iostream>
#include "PaymentStrategy.hpp"

class CardPayment : public PaymentStrategy {
public:
    bool processPayment(double amount) override {
        std::cout << "Processing card payment of Rs. " << amount << std::endl;
        std::cout << "Card payment successful!" << std::endl;
        return true;
    }
};

#endif // CARD_PAYMENT_HPP
