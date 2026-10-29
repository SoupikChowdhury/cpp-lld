#pragma once

#include "PaymentMode.hpp"
#include <iostream>

namespace OrderManagement {

/**
 * @brief Enum for card types
 */
enum class CardType {
    CREDIT,
    DEBIT
};

/**
 * @brief Concrete implementation of Card payment mode
 */
class CardPaymentMode : public PaymentMode {
private:
    std::string cardNumber_;
    std::string cardHolderName_;
    std::string expiryDate_;  // MM/YY format
    std::string cvv_;
    CardType cardType_;

public:
    CardPaymentMode(const std::string& cardNumber,
                    const std::string& cardHolderName,
                    const std::string& expiryDate,
                    const std::string& cvv,
                    CardType cardType = CardType::CREDIT)
        : cardNumber_(cardNumber)
        , cardHolderName_(cardHolderName)
        , expiryDate_(expiryDate)
        , cvv_(cvv)
        , cardType_(cardType) {}

    bool processPayment(double amount) override {
        if (!validateDetails()) {
            std::cout << "Card Payment Failed: Invalid card details\n";
            return false;
        }

        std::string cardTypeName = (cardType_ == CardType::CREDIT) ? "Credit" : "Debit";
        std::cout << "Processing " << cardTypeName << " Card payment of $" << amount 
                  << " using card ending in " << getMaskedCardNumber() << "...\n";
        
        // Simulate payment gateway processing
        std::cout << "Card Payment Successful!\n";
        return true;
    }

    std::string getPaymentModeName() const override {
        return (cardType_ == CardType::CREDIT) ? "Credit Card Payment" : "Debit Card Payment";
    }

    bool validateDetails() const override {
        // Basic validation
        bool validCardNumber = cardNumber_.length() >= 13 && cardNumber_.length() <= 19;
        bool validCvv = cvv_.length() >= 3 && cvv_.length() <= 4;
        bool validExpiry = expiryDate_.length() == 5; // MM/YY
        bool validName = !cardHolderName_.empty();

        return validCardNumber && validCvv && validExpiry && validName;
    }

    bool refund(double amount) override {
        std::cout << "Initiating Card refund of $" << amount 
                  << " to card ending in " << getMaskedCardNumber() << "...\n";
        std::cout << "Card Refund Successful!\n";
        return true;
    }

    /**
     * @brief Get masked card number for display
     */
    std::string getMaskedCardNumber() const {
        if (cardNumber_.length() < 4) return "****";
        return "****" + cardNumber_.substr(cardNumber_.length() - 4);
    }

    // Getters
    std::string getCardHolderName() const { return cardHolderName_; }
    CardType getCardType() const { return cardType_; }
};

} // namespace OrderManagement
