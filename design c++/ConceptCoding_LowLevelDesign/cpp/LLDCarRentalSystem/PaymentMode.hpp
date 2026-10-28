#pragma once

namespace CarRentalSystem {

enum class PaymentMode {
    CASH,
    CREDIT_CARD,
    DEBIT_CARD,
    UPI,
    NET_BANKING
};

inline const char* paymentModeToString(PaymentMode mode) {
    switch (mode) {
        case PaymentMode::CASH: return "CASH";
        case PaymentMode::CREDIT_CARD: return "CREDIT_CARD";
        case PaymentMode::DEBIT_CARD: return "DEBIT_CARD";
        case PaymentMode::UPI: return "UPI";
        case PaymentMode::NET_BANKING: return "NET_BANKING";
        default: return "UNKNOWN";
    }
}

} // namespace CarRentalSystem
