#ifndef PAYMENT_STRATEGY_HPP
#define PAYMENT_STRATEGY_HPP

class PaymentStrategy {
public:
    virtual ~PaymentStrategy() = default;
    virtual bool processPayment(double amount) = 0;
};

#endif // PAYMENT_STRATEGY_HPP
