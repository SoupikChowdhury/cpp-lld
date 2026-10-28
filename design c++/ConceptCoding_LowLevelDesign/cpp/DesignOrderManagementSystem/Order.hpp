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
 * @brief Class representing an order
 */
class Order {
private:
    std::string orderId_;
    std::shared_ptr<User> user_;
    std::vector<CartItem> items_;
    double totalAmount_;
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
        , items_(items)
        , totalAmount_(totalAmount)
        , status_(OrderStatus::PENDING)
        , deliveryAddress_(deliveryAddress)
        , orderTime_(getCurrentTimestamp()) {}

    // Getters
    std::string getOrderId() const { return orderId_; }
    std::shared_ptr<User> getUser() const { return user_; }
    const std::vector<CartItem>& getItems() const { return items_; }
    double getTotalAmount() const { return totalAmount_; }
    OrderStatus getStatus() const { return status_; }
    std::shared_ptr<Address> getDeliveryAddress() const { return deliveryAddress_; }
    std::shared_ptr<Warehouse> getFulfillingWarehouse() const { return fulfillingWarehouse_; }
    std::shared_ptr<Payment> getPayment() const { return payment_; }
    std::string getOrderTime() const { return orderTime_; }

    // Setters
    void setStatus(OrderStatus status) { status_ = status; }
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
     * @brief Mark order as shipped
     */
    void ship() {
        status_ = OrderStatus::SHIPPED;
    }

    /**
     * @brief Mark order as delivered
     */
    void deliver() {
        status_ = OrderStatus::DELIVERED;
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
