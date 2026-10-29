#pragma once

#include <string>
#include "ProductCategory.hpp"

namespace OrderManagement {

/**
 * @brief Class representing a product in the system
 */
class Product {
private:
    std::string productId_;
    std::string name_;
    std::string description_;
    double price_;
    ProductCategory category_;

public:
    Product(const std::string& productId, const std::string& name,
            const std::string& description, double price, ProductCategory category)
        : productId_(productId)
        , name_(name)
        , description_(description)
        , price_(price)
        , category_(category) {}

    // Getters
    std::string getProductId() const { return productId_; }
    std::string getName() const { return name_; }
    std::string getDescription() const { return description_; }
    double getPrice() const { return price_; }
    ProductCategory getCategory() const { return category_; }

    // Setters
    void setPrice(double price) { price_ = price; }
    void setDescription(const std::string& description) { description_ = description; }

    std::string toString() const {
        return "Product[" + productId_ + "] " + name_ + 
               " - $" + std::to_string(price_) + 
               " (" + productCategoryToString(category_) + ")";
    }
};

} // namespace OrderManagement
