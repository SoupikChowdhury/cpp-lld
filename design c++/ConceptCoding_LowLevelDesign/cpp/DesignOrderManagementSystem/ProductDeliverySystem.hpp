#pragma once

#include <memory>
#include <vector>
#include <iostream>
#include "Product.hpp"
#include "User.hpp"
#include "UserController.hpp"
#include "Cart.hpp"
#include "Warehouse.hpp"
#include "WarehouseController.hpp"
#include "Order.hpp"
#include "OrderController.hpp"
#include "Invoice.hpp"
#include "UPIPaymentMode.hpp"
#include "CardPaymentMode.hpp"

namespace OrderManagement {

/**
 * @brief Main facade class for the Product Delivery System
 * Provides a simplified interface to the entire order management system
 */
class ProductDeliverySystem {
private:
    UserController& userController_;
    WarehouseController& warehouseController_;
    OrderController& orderController_;
    std::unordered_map<std::string, std::shared_ptr<Cart>> userCarts_; // userId -> Cart
    int cartIdCounter_ = 0;
    int invoiceIdCounter_ = 0;

    ProductDeliverySystem()
        : userController_(UserController::getInstance())
        , warehouseController_(WarehouseController::getInstance())
        , orderController_(OrderController::getInstance()) {}

public:
    // Singleton instance
    static ProductDeliverySystem& getInstance() {
        static ProductDeliverySystem instance;
        return instance;
    }

    // Delete copy constructor and assignment operator
    ProductDeliverySystem(const ProductDeliverySystem&) = delete;
    ProductDeliverySystem& operator=(const ProductDeliverySystem&) = delete;

    // ============ User Management ============

    std::shared_ptr<User> registerUser(const std::string& name,
                                        const std::string& email,
                                        const std::string& phone,
                                        std::shared_ptr<Address> address = nullptr) {
        return userController_.registerUser(name, email, phone, address);
    }

    std::shared_ptr<User> getUser(const std::string& userId) {
        return userController_.getUser(userId);
    }

    // ============ Warehouse Management ============

    std::shared_ptr<Warehouse> addWarehouse(const std::string& name,
                                            std::shared_ptr<Address> address) {
        return warehouseController_.addWarehouse(name, address);
    }

    void addProductToWarehouse(const std::string& warehouseId,
                               std::shared_ptr<Product> product,
                               int quantity) {
        auto warehouse = warehouseController_.getWarehouse(warehouseId);
        if (warehouse) {
            warehouse->addProduct(product, quantity);
        }
    }

    void setWarehouseSelectionStrategy(std::unique_ptr<WarehouseSelectionStrategy> strategy) {
        warehouseController_.setSelectionStrategy(std::move(strategy));
    }

    // ============ Cart Management ============

    std::shared_ptr<Cart> getOrCreateCart(const std::string& userId) {
        auto it = userCarts_.find(userId);
        if (it != userCarts_.end()) {
            return it->second;
        }
        
        std::string cartId = "CART" + std::to_string(++cartIdCounter_);
        auto cart = std::make_shared<Cart>(cartId, userId);
        userCarts_[userId] = cart;
        return cart;
    }

    void addToCart(const std::string& userId, std::shared_ptr<Product> product, int quantity = 1) {
        auto cart = getOrCreateCart(userId);
        cart->addProduct(product, quantity);
    }

    void removeFromCart(const std::string& userId, const std::string& productId) {
        auto it = userCarts_.find(userId);
        if (it != userCarts_.end()) {
            it->second->removeProduct(productId);
        }
    }

    void clearCart(const std::string& userId) {
        auto it = userCarts_.find(userId);
        if (it != userCarts_.end()) {
            it->second->clear();
        }
    }

    // ============ Order Management ============

    /**
     * @brief Place an order from user's cart
     */
    std::shared_ptr<Order> placeOrder(const std::string& userId,
                                       std::shared_ptr<Address> deliveryAddress = nullptr) {
        auto user = userController_.getUser(userId);
        auto cartIt = userCarts_.find(userId);
        
        if (cartIt == userCarts_.end() || cartIt->second->isEmpty()) {
            throw std::runtime_error("Cart is empty");
        }

        auto& cart = cartIt->second;
        
        // Use user's address if delivery address not provided
        if (!deliveryAddress) {
            deliveryAddress = user->getAddress();
        }
        
        if (!deliveryAddress) {
            throw std::runtime_error("Delivery address not provided");
        }

        auto order = orderController_.createOrder(user, *cart, deliveryAddress);
        cart->clear(); // Clear cart after order is created
        
        return order;
    }

    /**
     * @brief Pay for an order with UPI
     */
    bool payWithUPI(const std::string& orderId, const std::string& upiId, 
                    const std::string& pin) {
        auto order = orderController_.getOrder(orderId);
        if (!order) {
            throw std::runtime_error("Order not found");
        }

        auto paymentMode = std::make_unique<UPIPaymentMode>(upiId, pin);
        return orderController_.processPayment(order, std::move(paymentMode));
    }

    /**
     * @brief Pay for an order with Card
     */
    bool payWithCard(const std::string& orderId, const std::string& cardNumber,
                     const std::string& cardHolderName, const std::string& expiryDate,
                     const std::string& cvv, CardType cardType = CardType::CREDIT) {
        auto order = orderController_.getOrder(orderId);
        if (!order) {
            throw std::runtime_error("Order not found");
        }

        auto paymentMode = std::make_unique<CardPaymentMode>(
            cardNumber, cardHolderName, expiryDate, cvv, cardType);
        return orderController_.processPayment(order, std::move(paymentMode));
    }

    /**
     * @brief Get order details
     */
    std::shared_ptr<Order> getOrder(const std::string& orderId) {
        return orderController_.getOrder(orderId);
    }

    /**
     * @brief Get all orders for a user
     */
    std::vector<std::shared_ptr<Order>> getUserOrders(const std::string& userId) {
        return orderController_.getOrdersByUser(userId);
    }

    /**
     * @brief Cancel an order
     */
    bool cancelOrder(const std::string& orderId) {
        return orderController_.cancelOrder(orderId);
    }

    /**
     * @brief Update order status
     */
    void updateOrderStatus(const std::string& orderId, OrderStatus status) {
        orderController_.updateOrderStatus(orderId, status);
    }

    // ============ Invoice Generation ============

    /**
     * @brief Generate invoice for an order
     */
    std::unique_ptr<Invoice> generateInvoice(const std::string& orderId,
                                              double taxRate = 0.08,
                                              double shippingCost = 5.99,
                                              double discount = 0.0) {
        auto order = orderController_.getOrder(orderId);
        if (!order) {
            throw std::runtime_error("Order not found");
        }

        std::string invoiceId = "INV" + std::to_string(++invoiceIdCounter_);
        return std::make_unique<Invoice>(invoiceId, order, taxRate, shippingCost, discount);
    }

    // ============ System Management ============

    /**
     * @brief Reset the entire system (for testing)
     */
    void reset() {
        userController_.clear();
        warehouseController_.clear();
        orderController_.clear();
        userCarts_.clear();
        cartIdCounter_ = 0;
        invoiceIdCounter_ = 0;
    }

    /**
     * @brief Print system status
     */
    void printSystemStatus() {
        std::cout << "\n========== SYSTEM STATUS ==========\n\n";
        
        std::cout << "Users: " << userController_.getAllUsers().size() << "\n";
        std::cout << "Warehouses: " << warehouseController_.getAllWarehouses().size() << "\n";
        std::cout << "Active Carts: " << userCarts_.size() << "\n";
        std::cout << "Total Orders: " << orderController_.getAllOrders().size() << "\n";
        std::cout << "Warehouse Strategy: " << warehouseController_.getCurrentStrategyName() << "\n";
        
        std::cout << "\n====================================\n";
    }
};

} // namespace OrderManagement
