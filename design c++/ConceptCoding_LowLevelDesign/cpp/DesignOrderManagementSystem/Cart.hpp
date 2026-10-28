#pragma once

#include <unordered_map>
#include <memory>
#include <vector>
#include "Product.hpp"

namespace OrderManagement {

/**
 * @brief Represents an item in the cart with product and quantity
 */
struct CartItem {
    std::shared_ptr<Product> product;
    int quantity;

    CartItem(std::shared_ptr<Product> prod, int qty)
        : product(prod), quantity(qty) {}

    double getTotalPrice() const {
        return product->getPrice() * quantity;
    }
};

/**
 * @brief Class representing a shopping cart
 */
class Cart {
private:
    std::string cartId_;
    std::string userId_;
    std::unordered_map<std::string, CartItem> items_; // productId -> CartItem

public:
    Cart(const std::string& cartId, const std::string& userId)
        : cartId_(cartId), userId_(userId) {}

    // Getters
    std::string getCartId() const { return cartId_; }
    std::string getUserId() const { return userId_; }

    /**
     * @brief Add product to cart
     */
    void addProduct(std::shared_ptr<Product> product, int quantity = 1) {
        std::string productId = product->getProductId();
        auto it = items_.find(productId);
        if (it != items_.end()) {
            it->second.quantity += quantity;
        } else {
            items_.emplace(productId, CartItem(product, quantity));
        }
    }

    /**
     * @brief Update product quantity
     */
    void updateQuantity(const std::string& productId, int quantity) {
        auto it = items_.find(productId);
        if (it != items_.end()) {
            if (quantity <= 0) {
                items_.erase(it);
            } else {
                it->second.quantity = quantity;
            }
        }
    }

    /**
     * @brief Remove product from cart
     */
    void removeProduct(const std::string& productId) {
        items_.erase(productId);
    }

    /**
     * @brief Get all cart items
     */
    std::vector<CartItem> getItems() const {
        std::vector<CartItem> result;
        for (const auto& pair : items_) {
            result.push_back(pair.second);
        }
        return result;
    }

    /**
     * @brief Get total cart value
     */
    double getTotalValue() const {
        double total = 0.0;
        for (const auto& pair : items_) {
            total += pair.second.getTotalPrice();
        }
        return total;
    }

    /**
     * @brief Check if cart is empty
     */
    bool isEmpty() const { return items_.empty(); }

    /**
     * @brief Clear the cart
     */
    void clear() { items_.clear(); }

    /**
     * @brief Get item count
     */
    int getItemCount() const {
        int count = 0;
        for (const auto& pair : items_) {
            count += pair.second.quantity;
        }
        return count;
    }

    std::string toString() const {
        std::string str = "Cart[" + cartId_ + "] for User[" + userId_ + "]\n";
        for (const auto& pair : items_) {
            const auto& item = pair.second;
            str += "  - " + item.product->getName() + " x" + 
                   std::to_string(item.quantity) + " = $" +
                   std::to_string(item.getTotalPrice()) + "\n";
        }
        str += "Total: $" + std::to_string(getTotalValue());
        return str;
    }
};

} // namespace OrderManagement
