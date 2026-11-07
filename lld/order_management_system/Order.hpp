#pragma once

#include <memory>
#include <vector>
#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>
#include "User.hpp"
#include "Cart.hpp"
#include "OrderStatus.hpp"
#include "Warehouse.hpp"
#include "Payment.hpp"

namespace OrderManagement {

/**
 * @brief One line of an order, with the price the customer agreed to.
 * A later price change must not alter an order already placed.
 */
struct OrderItem {
    std::shared_ptr<Product> product;
    int quantity;
    double unitPrice;

    OrderItem(std::shared_ptr<Product> prod, int qty, double price)
        : product(prod), quantity(qty), unitPrice(price) {}

    double getTotalPrice() const {
        return unitPrice * quantity;
    }
};

/**
 * @brief Class representing an order
 */
class Order {
private:
    std::string orderId_;
    std::shared_ptr<User> user_;
    std::vector<OrderItem> items_;
    double totalAmount_;  // Sum of the items
    double amountDue_;    // What payment charges: the invoice total once there is one
    OrderStatus status_;
    std::shared_ptr<Address> deliveryAddress_;
    std::shared_ptr<Warehouse> fulfillingWarehouse_;
    std::shared_ptr<Payment> payment_;
    std::string orderTime_;
    std::string estimatedDeliveryTime_;

    std::string getCurrentTimestamp() {
        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
        return ss.str();
    }

public:
    Order(const std::string& orderId, std::shared_ptr<User> user,
          const std::vector<CartItem>& items, double totalAmount,
          std::shared_ptr<Address> deliveryAddress)
        : orderId_(orderId)
        , user_(user)
        , totalAmount_(totalAmount)
        , amountDue_(totalAmount)
        , status_(OrderStatus::PENDING)
        , deliveryAddress_(deliveryAddress)
        , orderTime_(getCurrentTimestamp()) {
        for (const auto& item : items) {
            items_.emplace_back(item.product, item.quantity, item.product->getPrice());
        }
    }

    // Getters
    std::string getOrderId() const { return orderId_; }
    std::shared_ptr<User> getUser() const { return user_; }
    const std::vector<OrderItem>& getItems() const { return items_; }
    double getTotalAmount() const { return totalAmount_; }
    double getAmountDue() const { return amountDue_; }
    OrderStatus getStatus() const { return status_; }
    std::shared_ptr<Address> getDeliveryAddress() const { return deliveryAddress_; }
    std::shared_ptr<Warehouse> getFulfillingWarehouse() const { return fulfillingWarehouse_; }
    std::shared_ptr<Payment> getPayment() const { return payment_; }
    std::string getOrderTime() const { return orderTime_; }

    // Setters
    void setAmountDue(double amount) { amountDue_ = amount; }
    void setFulfillingWarehouse(std::shared_ptr<Warehouse> warehouse) {
        fulfillingWarehouse_ = warehouse;
    }
    void setPayment(std::shared_ptr<Payment> payment) { payment_ = payment; }
    void setEstimatedDeliveryTime(const std::string& time) { 
        estimatedDeliveryTime_ = time; 
    }

    /**
     * @brief Confirm the order after payment
     */
    void confirm() {
        status_ = OrderStatus::CONFIRMED;
    }

    /**
     * @brief Check a fulfilment step: forward only, and only once paid.
     * PENDING, CONFIRMED and CANCELLED come from placing, paying and cancelling.
     */
    bool canMoveTo(OrderStatus next) const {
        switch (next) {
            case OrderStatus::PENDING:
            case OrderStatus::CONFIRMED:
            case OrderStatus::CANCELLED:
                return false;
            case OrderStatus::RETURNED:
                return status_ == OrderStatus::DELIVERED;
            case OrderStatus::REFUNDED:
                return status_ == OrderStatus::RETURNED;
            default:
                return status_ >= OrderStatus::CONFIRMED && status_ < OrderStatus::DELIVERED &&
                       next > status_;
        }
    }

    /**
     * @brief Apply a fulfilment step
     * @return false if the step isn't allowed from the current status
     */
    bool moveTo(OrderStatus next) {
        if (!canMoveTo(next)) {
            return false;
        }
        status_ = next;
        return true;
    }

    /**
     * @brief Cancel the order
     */
    bool cancel() {
        if (status_ == OrderStatus::PENDING || status_ == OrderStatus::CONFIRMED) {
            status_ = OrderStatus::CANCELLED;
            return true;
        }
        return false; // Cannot cancel shipped or delivered orders
    }

    std::string toString() const {
        std::string str = "=== Order[" + orderId_ + "] ===\n";
        str += "Customer: " + user_->getName() + "\n";
        str += "Order Time: " + orderTime_ + "\n";
        str += "Status: " + orderStatusToString(status_) + "\n";
        str += "Delivery Address: " + deliveryAddress_->toString() + "\n";
        
        str += "\nItems:\n";
        for (const auto& item : items_) {
            str += "  - " + item.product->getName() + " x" + 
                   std::to_string(item.quantity) + " = $" +
                   std::to_string(item.getTotalPrice()) + "\n";
        }
        
        str += "\nTotal Amount: $" + std::to_string(totalAmount_) + "\n";
        
        if (fulfillingWarehouse_) {
            str += "Shipping from: " + fulfillingWarehouse_->getName() + "\n";
        }
        
        if (!estimatedDeliveryTime_.empty()) {
            str += "Estimated Delivery: " + estimatedDeliveryTime_ + "\n";
        }
        
        return str;
    }
};

} // namespace OrderManagement
