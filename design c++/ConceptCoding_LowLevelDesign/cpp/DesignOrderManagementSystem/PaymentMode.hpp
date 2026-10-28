#pragma once

#include <string>

namespace OrderManagement {

/**
 * @brief Abstract interface for payment modes
 * Implements Strategy Design Pattern for payment
 */
class PaymentMode {
public:
    virtual ~PaymentMode() = default;

    /**
     * @brief Process payment for the given amount
     * @param amount Amount to be paid
     * @return true if payment successful, false otherwise
     */
    virtual bool processPayment(double amount) = 0;

    /**
     * @brief Get payment mode name
     */
    virtual std::string getPaymentModeName() const = 0;

    /**
     * @brief Validate payment details before processing
     */
    virtual bool validateDetails() const = 0;

    /**
     * @brief Refund the payment
     */
    virtual bool refund(double amount) = 0;
};

} // namespace OrderManagement
