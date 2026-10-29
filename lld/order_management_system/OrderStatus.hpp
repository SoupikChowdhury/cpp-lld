#pragma once

#include <string>

namespace OrderManagement {

/**
 * @brief Enum class representing order status
 */
enum class OrderStatus {
    PENDING,
    CONFIRMED,
    PROCESSING,
    SHIPPED,
    OUT_FOR_DELIVERY,
    DELIVERED,
    CANCELLED,
    RETURNED,
    REFUNDED
};

/**
 * @brief Utility to convert OrderStatus to string
 */
inline std::string orderStatusToString(OrderStatus status) {
    switch (status) {
        case OrderStatus::PENDING: return "Pending";
        case OrderStatus::CONFIRMED: return "Confirmed";
        case OrderStatus::PROCESSING: return "Processing";
        case OrderStatus::SHIPPED: return "Shipped";
        case OrderStatus::OUT_FOR_DELIVERY: return "Out for Delivery";
        case OrderStatus::DELIVERED: return "Delivered";
        case OrderStatus::CANCELLED: return "Cancelled";
        case OrderStatus::RETURNED: return "Returned";
        case OrderStatus::REFUNDED: return "Refunded";
        default: return "Unknown";
    }
}

} // namespace OrderManagement
