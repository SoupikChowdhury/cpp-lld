#ifndef PAYMENT_HPP
#define PAYMENT_HPP

#include <string>
#include <ctime>
#include <iomanip>
#include <sstream>

enum class PaymentStatus {
    PENDING,
    COMPLETED,
    FAILED,
    REFUNDED
};

enum class PaymentMethod {
    CREDIT_CARD,
    DEBIT_CARD,
    UPI,
    NET_BANKING,
    WALLET
};

inline std::string paymentStatusToString(PaymentStatus status) {
    switch (status) {
        case PaymentStatus::PENDING: return "Pending";
        case PaymentStatus::COMPLETED: return "Completed";
        case PaymentStatus::FAILED: return "Failed";
        case PaymentStatus::REFUNDED: return "Refunded";
        default: return "Unknown";
    }
}

inline std::string paymentMethodToString(PaymentMethod method) {
    switch (method) {
        case PaymentMethod::CREDIT_CARD: return "Credit Card";
        case PaymentMethod::DEBIT_CARD: return "Debit Card";
        case PaymentMethod::UPI: return "UPI";
        case PaymentMethod::NET_BANKING: return "Net Banking";
        case PaymentMethod::WALLET: return "Wallet";
        default: return "Unknown";
    }
}

class Payment {
private:
    int paymentId;
    double amount;
    PaymentStatus status;
    PaymentMethod method;
    std::string transactionId;
    std::time_t paymentTime;

public:
    Payment(int id, double amount, PaymentMethod method)
        : paymentId(id), amount(amount), status(PaymentStatus::PENDING), 
          method(method), transactionId(""), paymentTime(0) {}

    // Getters
    int getPaymentId() const { return paymentId; }
    double getAmount() const { return amount; }
    PaymentStatus getStatus() const { return status; }
    PaymentMethod getMethod() const { return method; }
    std::string getTransactionId() const { return transactionId; }
    std::time_t getPaymentTime() const { return paymentTime; }

    // Process payment
    bool processPayment() {
        // Simulate payment processing
        // In real implementation, this would connect to payment gateway
        transactionId = "TXN" + std::to_string(paymentId) + std::to_string(std::time(nullptr));
        paymentTime = std::time(nullptr);
        status = PaymentStatus::COMPLETED;
        return true;
    }

    // Refund payment
    bool refund() {
        if (status == PaymentStatus::COMPLETED) {
            status = PaymentStatus::REFUNDED;
            return true;
        }
        return false;
    }

    std::string getPaymentTimeFormatted() const {
        if (paymentTime == 0) return "N/A";
        std::tm* tm = std::localtime(&paymentTime);
        std::ostringstream oss;
        oss << std::put_time(tm, "%Y-%m-%d %H:%M:%S");
        return oss.str();
    }

    std::string toString() const {
        return "Payment[ID: " + std::to_string(paymentId) + 
               ", Amount: Rs." + std::to_string(static_cast<int>(amount)) +
               ", Method: " + paymentMethodToString(method) +
               ", Status: " + paymentStatusToString(status) +
               ", TxnID: " + transactionId + "]";
    }
};

#endif // PAYMENT_HPP
