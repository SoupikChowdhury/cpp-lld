#pragma once

#include <unordered_map>
#include <memory>
#include <vector>
#include <stdexcept>
#include "Product.hpp"

namespace OrderManagement {

/**
 * @brief Class representing inventory with product stock management
 */
class Inventory {
private:
    std::unordered_map<std::string, std::shared_ptr<Product>> products_;
    std::unordered_map<std::string, int> stock_; // productId -> quantity

public:
    Inventory() = default;

    /**
     * @brief Add product to inventory
     */
    void addProduct(std::shared_ptr<Product> product, int quantity) {
        std::string productId = product->getProductId();
        products_[productId] = product;
        stock_[productId] += quantity;
    }

    /**
     * @brief Remove product from inventory
     */
    bool removeProduct(const std::string& productId, int quantity) {
        auto stockIt = stock_.find(productId);
        if (stockIt == stock_.end() || stockIt->second < quantity) {
            return false;
        }
        stockIt->second -= quantity;
        if (stockIt->second == 0) {
            stock_.erase(stockIt);
            products_.erase(productId);
        }
        return true;
    }

    /**
     * @brief Check if product is available in required quantity
     */
    bool isAvailable(const std::string& productId, int quantity = 1) const {
        auto it = stock_.find(productId);
        return it != stock_.end() && it->second >= quantity;
    }

    /**
     * @brief Get available quantity of a product
     */
    int getAvailableQuantity(const std::string& productId) const {
        auto it = stock_.find(productId);
        return (it != stock_.end()) ? it->second : 0;
    }

    /**
     * @brief Get product by ID
     */
    std::shared_ptr<Product> getProduct(const std::string& productId) const {
        auto it = products_.find(productId);
        if (it != products_.end()) {
            return it->second;
        }
        return nullptr;
    }

    /**
     * @brief Get all products in inventory
     */
    std::vector<std::shared_ptr<Product>> getAllProducts() const {
        std::vector<std::shared_ptr<Product>> result;
        for (const auto& pair : products_) {
            result.push_back(pair.second);
        }
        return result;
    }

    /**
     * @brief Restock a product
     */
    void restock(const std::string& productId, int quantity) {
        if (products_.find(productId) != products_.end()) {
            stock_[productId] += quantity;
        }
    }

    /**
     * @brief Get inventory status
     */
    std::string toString() const {
        std::string str = "Inventory Status:\n";
        for (const auto& pair : products_) {
            str += "  " + pair.second->getName() + ": " +
                   std::to_string(getAvailableQuantity(pair.first)) + " units\n";
        }
        return str;
    }
};

} // namespace OrderManagement
