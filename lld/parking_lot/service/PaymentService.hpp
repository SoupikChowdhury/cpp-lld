#ifndef PAYMENT_SERVICE_HPP
#define PAYMENT_SERVICE_HPP

#include "../strategy/payment/PaymentStrategy.hpp"

class PaymentService {
public:
    bool processPayment(double amount, PaymentStrategy* strategy) {
        if (strategy == nullptr) {
            return false;
        }
        return strategy->processPayment(amount);
    }
};

#endif // PAYMENT_SERVICE_HPP
