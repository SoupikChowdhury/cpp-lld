#ifndef PAYMENT_MODE_HPP
#define PAYMENT_MODE_HPP

#include <string>

enum class PaymentMode {
    CASH,
    CARD,
    UPI
};

inline std::string paymentModeToString(PaymentMode mode) {
    switch (mode) {
        case PaymentMode::CASH: return "CASH";
        case PaymentMode::CARD: return "CARD";
        case PaymentMode::UPI: return "UPI";
        default: return "UNKNOWN";
    }
}

#endif // PAYMENT_MODE_HPP
