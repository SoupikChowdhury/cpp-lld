#pragma once

#include <memory>
#include <string>
#include <sstream>
#include <iomanip>
#include <chrono>
#include "Order.hpp"

namespace OrderManagement {

/**
 * @brief Class representing an invoice for an order
 */
class Invoice {
private:
    std::string invoiceId_;
    std::shared_ptr<Order> order_;
    double subtotal_;
    double taxRate_;
    double taxAmount_;
    double shippingCost_;
    double discount_;
    double totalAmount_;
    std::string generatedTime_;

    std::string getCurrentTimestamp() {
        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
        return ss.str();
    }

public:
    Invoice(const std::string& invoiceId, std::shared_ptr<Order> order,
            double taxRate = 0.08, double shippingCost = 0.0, double discount = 0.0)
        : invoiceId_(invoiceId)
        , order_(order)
        , taxRate_(taxRate)
        , shippingCost_(shippingCost)
        , discount_(discount)
        , generatedTime_(getCurrentTimestamp()) {
        
        calculateTotals();
    }

    void calculateTotals() {
        subtotal_ = order_->getTotalAmount();
        taxAmount_ = subtotal_ * taxRate_;
        totalAmount_ = subtotal_ + taxAmount_ + shippingCost_ - discount_;
    }

    // Getters
    std::string getInvoiceId() const { return invoiceId_; }
    std::shared_ptr<Order> getOrder() const { return order_; }
    double getSubtotal() const { return subtotal_; }
    double getTaxAmount() const { return taxAmount_; }
    double getShippingCost() const { return shippingCost_; }
    double getDiscount() const { return discount_; }
    double getTotalAmount() const { return totalAmount_; }

    // Setters
    void setDiscount(double discount) {
        discount_ = discount;
        calculateTotals();
    }

    void setShippingCost(double cost) {
        shippingCost_ = cost;
        calculateTotals();
    }

    std::string generateInvoice() const {
        std::stringstream ss;
        ss << std::fixed << std::setprecision(2);
        
        ss << "╔══════════════════════════════════════════════════════╗\n";
        ss << "║                    INVOICE                            ║\n";
        ss << "╠══════════════════════════════════════════════════════╣\n";
        ss << "║ Invoice ID: " << std::setw(40) << std::left << invoiceId_ << " ║\n";
        ss << "║ Order ID:   " << std::setw(40) << order_->getOrderId() << " ║\n";
        ss << "║ Date:       " << std::setw(40) << generatedTime_ << " ║\n";
        ss << "╠══════════════════════════════════════════════════════╣\n";
        ss << "║ CUSTOMER DETAILS                                      ║\n";
        ss << "║ Name:    " << std::setw(43) << order_->getUser()->getName() << " ║\n";
        ss << "║ Email:   " << std::setw(43) << order_->getUser()->getEmail() << " ║\n";
        ss << "║ Address: " << std::setw(43) << order_->getDeliveryAddress()->getCity() << " ║\n";
        ss << "╠══════════════════════════════════════════════════════╣\n";
        ss << "║ ORDER ITEMS                                           ║\n";
        ss << "╠──────────────────────────────────────────────────────╣\n";
        
        for (const auto& item : order_->getItems()) {
            std::string itemLine = item.product->getName() + " x" + 
                                   std::to_string(item.quantity);
            ss << "║ " << std::setw(35) << std::left << itemLine;
            ss << "$" << std::setw(16) << std::right << item.getTotalPrice() << " ║\n";
        }
        
        ss << "╠══════════════════════════════════════════════════════╣\n";
        ss << "║ " << std::setw(35) << std::left << "Subtotal:";
        ss << "$" << std::setw(16) << std::right << subtotal_ << " ║\n";
        
        ss << "║ " << std::setw(35) << std::left << ("Tax (" + std::to_string(int(taxRate_ * 100)) + "%):");
        ss << "$" << std::setw(16) << std::right << taxAmount_ << " ║\n";
        
        ss << "║ " << std::setw(35) << std::left << "Shipping:";
        ss << "$" << std::setw(16) << std::right << shippingCost_ << " ║\n";
        
        if (discount_ > 0) {
            ss << "║ " << std::setw(35) << std::left << "Discount:";
            ss << "-$" << std::setw(15) << std::right << discount_ << " ║\n";
        }
        
        ss << "╠══════════════════════════════════════════════════════╣\n";
        ss << "║ " << std::setw(35) << std::left << "TOTAL:";
        ss << "$" << std::setw(16) << std::right << totalAmount_ << " ║\n";
        ss << "╚══════════════════════════════════════════════════════╝\n";
        
        return ss.str();
    }
};

} // namespace OrderManagement
