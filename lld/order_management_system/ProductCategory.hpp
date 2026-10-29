#pragma once

#include <string>

namespace OrderManagement {

/**
 * @brief Enum class representing product categories
 */
enum class ProductCategory {
    ELECTRONICS,
    FASHION,
    GROCERY,
    FURNITURE,
    SPORTS,
    BOOKS,
    HOME_APPLIANCES
};

/**
 * @brief Utility to convert ProductCategory to string
 */
inline std::string productCategoryToString(ProductCategory category) {
    switch (category) {
        case ProductCategory::ELECTRONICS: return "Electronics";
        case ProductCategory::FASHION: return "Fashion";
        case ProductCategory::GROCERY: return "Grocery";
        case ProductCategory::FURNITURE: return "Furniture";
        case ProductCategory::SPORTS: return "Sports";
        case ProductCategory::BOOKS: return "Books";
        case ProductCategory::HOME_APPLIANCES: return "Home Appliances";
        default: return "Unknown";
    }
}

} // namespace OrderManagement
