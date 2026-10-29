#pragma once

#include <memory>
#include <vector>
#include "Warehouse.hpp"
#include "Cart.hpp"

namespace OrderManagement {

/**
 * @brief Abstract strategy interface for warehouse selection
 * Implements Strategy Design Pattern
 */
class WarehouseSelectionStrategy {
public:
    virtual ~WarehouseSelectionStrategy() = default;

    /**
     * @brief Select the best warehouse for fulfilling the order
     * @param warehouses List of available warehouses
     * @param cart Cart containing items to fulfill
     * @param deliveryAddress Delivery address for distance calculation
     * @return Selected warehouse or nullptr if no suitable warehouse found
     */
    virtual std::shared_ptr<Warehouse> selectWarehouse(
        const std::vector<std::shared_ptr<Warehouse>>& warehouses,
        const Cart& cart,
        const Address& deliveryAddress) = 0;

    /**
     * @brief Get strategy name
     */
    virtual std::string getStrategyName() const = 0;
};

} // namespace OrderManagement
