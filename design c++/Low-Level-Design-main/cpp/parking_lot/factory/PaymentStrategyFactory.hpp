#ifndef PAYMENT_STRATEGY_FACTORY_HPP
#define PAYMENT_STRATEGY_FACTORY_HPP

#include <memory>
#include "../enums/PaymentMode.hpp"
#include "../strategy/payment/PaymentStrategy.hpp"
#include "../strategy/payment/CashPayment.hpp"
#include "../strategy/payment/CardPayment.hpp"
#include "../strategy/payment/UPIPayment.hpp"

class PaymentStrategyFactory {
public:
    static std::unique_ptr<PaymentStrategy> get(PaymentMode mode) {
        switch (mode) {
            case PaymentMode::CASH:
                return std::make_unique<CashPayment>();
            case PaymentMode::CARD:
                return std::make_unique<CardPayment>();
            case PaymentMode::UPI:
                return std::make_unique<UPIPayment>();
            default:
                return std::make_unique<CashPayment>();
        }
    }
};

#endif // PAYMENT_STRATEGY_FACTORY_HPP
