#pragma once

#include "WarehouseSelectionStrategy.hpp"
#include <limits>

namespace OrderManagement {

/**
 * @brief Concrete strategy that selects the nearest warehouse
 * that can fulfill all items in the cart
 */
class NearestWarehouseSelectionStrategy : public WarehouseSelectionStrategy {
public:
    std::shared_ptr<Warehouse> selectWarehouse(
        const std::vector<std::shared_ptr<Warehouse>>& warehouses,
        const Cart& cart,
        const Address& deliveryAddress) override {
        
        std::shared_ptr<Warehouse> nearestWarehouse = nullptr;
        double minDistance = std::numeric_limits<double>::max();

        for (const auto& warehouse : warehouses) {
            // Check if warehouse can fulfill all cart items
            bool canFulfill = true;
            for (const auto& item : cart.getItems()) {
                if (!warehouse->hasProduct(item.product->getProductId(), item.quantity)) {
                    canFulfill = false;
                    break;
                }
            }

            if (canFulfill) {
                double distance = warehouse->distanceTo(deliveryAddress);
                if (distance < minDistance) {
                    minDistance = distance;
                    nearestWarehouse = warehouse;
                }
            }
        }

        return nearestWarehouse;
    }

    std::string getStrategyName() const override {
        return "Nearest Warehouse Selection Strategy";
    }
};

/**
 * @brief Strategy that selects warehouse with highest stock levels
 */
class HighestStockWarehouseSelectionStrategy : public WarehouseSelectionStrategy {
public:
    std::shared_ptr<Warehouse> selectWarehouse(
        const std::vector<std::shared_ptr<Warehouse>>& warehouses,
        const Cart& cart,
        const Address& deliveryAddress) override {
        
        std::shared_ptr<Warehouse> bestWarehouse = nullptr;
        int maxStock = -1;

        for (const auto& warehouse : warehouses) {
            bool canFulfill = true;
            int totalStock = 0;

            for (const auto& item : cart.getItems()) {
                std::string productId = item.product->getProductId();
                int available = warehouse->getInventory()->getAvailableQuantity(productId);
                
                if (available < item.quantity) {
                    canFulfill = false;
                    break;
                }
                totalStock += available;
            }

            if (canFulfill && totalStock > maxStock) {
                maxStock = totalStock;
                bestWarehouse = warehouse;
            }
        }

        return bestWarehouse;
    }

    std::string getStrategyName() const override {
        return "Highest Stock Warehouse Selection Strategy";
    }
};

} // namespace OrderManagement
