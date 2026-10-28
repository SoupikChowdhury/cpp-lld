#pragma once

#include <unordered_map>
#include <memory>
#include <vector>
#include "Order.hpp"
#include "WarehouseController.hpp"

namespace OrderManagement {

/**
 * @brief Controller class for managing orders
 * Singleton pattern for centralized order management
 */
class OrderController {
private:
    std::unordered_map<std::string, std::shared_ptr<Order>> orders_;
    std::unordered_map<std::string, std::vector<std::string>> userOrders_; // userId -> orderIds
    int orderIdCounter_ = 0;
    int paymentIdCounter_ = 0;

    OrderController() = default;

public:
    // Singleton instance
    static OrderController& getInstance() {
        static OrderController instance;
        return instance;
    }

    // Delete copy constructor and assignment operator
    OrderController(const OrderController&) = delete;
    OrderController& operator=(const OrderController&) = delete;

    /**
     * @brief Create a new order from cart
     */
    std::shared_ptr<Order> createOrder(std::shared_ptr<User> user,
                                       const Cart& cart,
                                       std::shared_ptr<Address> deliveryAddress) {
        if (cart.isEmpty()) {
            throw std::runtime_error("Cannot create order from empty cart");
        }

        std::string orderId = "ORD" + std::to_string(++orderIdCounter_);
        auto order = std::make_shared<Order>(
            orderId, user, cart.getItems(), cart.getTotalValue(), deliveryAddress);

        // Select warehouse for fulfillment
        auto& warehouseController = WarehouseController::getInstance();
        auto warehouse = warehouseController.selectWarehouse(cart, *deliveryAddress);
        
        if (!warehouse) {
            throw std::runtime_error("No warehouse available to fulfill the order");
        }
        
        order->setFulfillingWarehouse(warehouse);
        
        orders_[orderId] = order;
        userOrders_[user->getUserId()].push_back(orderId);
        
        return order;
    }

    /**
     * @brief Process payment for an order
     */
    bool processPayment(std::shared_ptr<Order> order, 
                        std::unique_ptr<PaymentMode> paymentMode) {
        std::string paymentId = "PAY" + std::to_string(++paymentIdCounter_);
        auto payment = std::make_shared<Payment>(
            paymentId, order->getOrderId(), order->getTotalAmount(), std::move(paymentMode));

        if (payment->processPayment()) {
            order->setPayment(payment);
            order->confirm();
            
            // Deduct items from warehouse inventory
            auto warehouse = order->getFulfillingWarehouse();
            if (warehouse) {
                for (const auto& item : order->getItems()) {
                    warehouse->removeProduct(item.product->getProductId(), item.quantity);
                }
            }
            
            return true;
        }
        
        return false;
    }

    /**
     * @brief Get order by ID
     */
    std::shared_ptr<Order> getOrder(const std::string& orderId) const {
        auto it = orders_.find(orderId);
        if (it != orders_.end()) {
            return it->second;
        }
        return nullptr;
    }

    /**
     * @brief Get all orders for a user
     */
    std::vector<std::shared_ptr<Order>> getOrdersByUser(const std::string& userId) const {
        std::vector<std::shared_ptr<Order>> result;
        auto it = userOrders_.find(userId);
        if (it != userOrders_.end()) {
            for (const auto& orderId : it->second) {
                auto order = getOrder(orderId);
                if (order) {
                    result.push_back(order);
                }
            }
        }
        return result;
    }

    /**
     * @brief Get all orders
     */
    std::vector<std::shared_ptr<Order>> getAllOrders() const {
        std::vector<std::shared_ptr<Order>> result;
        for (const auto& pair : orders_) {
            result.push_back(pair.second);
        }
        return result;
    }

    /**
     * @brief Update order status
     */
    void updateOrderStatus(const std::string& orderId, OrderStatus status) {
        auto order = getOrder(orderId);
        if (order) {
            order->setStatus(status);
        }
    }

    /**
     * @brief Cancel an order
     */
    bool cancelOrder(const std::string& orderId) {
        auto order = getOrder(orderId);
        if (order && order->cancel()) {
            // Refund payment if already paid
            auto payment = order->getPayment();
            if (payment && payment->getStatus() == PaymentStatus::COMPLETED) {
                payment->refund();
            }
            
            // Return items to warehouse inventory
            auto warehouse = order->getFulfillingWarehouse();
            if (warehouse) {
                for (const auto& item : order->getItems()) {
                    warehouse->addProduct(item.product, item.quantity);
                }
            }
            
            return true;
        }
        return false;
    }

    /**
     * @brief Clear all orders (for testing)
     */
    void clear() {
        orders_.clear();
        userOrders_.clear();
        orderIdCounter_ = 0;
        paymentIdCounter_ = 0;
    }
};

} // namespace OrderManagement
