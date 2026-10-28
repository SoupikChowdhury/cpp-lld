#pragma once

#include <unordered_map>
#include <memory>
#include <vector>
#include "Warehouse.hpp"
#include "WarehouseSelectionStrategy.hpp"
#include "NearestWarehouseSelectionStrategy.hpp"

namespace OrderManagement {

/**
 * @brief Controller class for managing warehouses
 * Singleton pattern for centralized warehouse management
 */
class WarehouseController {
private:
    std::unordered_map<std::string, std::shared_ptr<Warehouse>> warehouses_;
    std::unique_ptr<WarehouseSelectionStrategy> selectionStrategy_;
    int warehouseIdCounter_ = 0;

    WarehouseController() 
        : selectionStrategy_(std::make_unique<NearestWarehouseSelectionStrategy>()) {}

public:
    // Singleton instance
    static WarehouseController& getInstance() {
        static WarehouseController instance;
        return instance;
    }

    // Delete copy constructor and assignment operator
    WarehouseController(const WarehouseController&) = delete;
    WarehouseController& operator=(const WarehouseController&) = delete;

    /**
     * @brief Set warehouse selection strategy
     */
    void setSelectionStrategy(std::unique_ptr<WarehouseSelectionStrategy> strategy) {
        selectionStrategy_ = std::move(strategy);
    }

    /**
     * @brief Add a new warehouse
     */
    std::shared_ptr<Warehouse> addWarehouse(const std::string& name,
                                            std::shared_ptr<Address> address) {
        std::string warehouseId = "W" + std::to_string(++warehouseIdCounter_);
        auto warehouse = std::make_shared<Warehouse>(warehouseId, name, address);
        warehouses_[warehouseId] = warehouse;
        return warehouse;
    }

    /**
     * @brief Get warehouse by ID
     */
    std::shared_ptr<Warehouse> getWarehouse(const std::string& warehouseId) const {
        auto it = warehouses_.find(warehouseId);
        if (it != warehouses_.end()) {
            return it->second;
        }
        return nullptr;
    }

    /**
     * @brief Get all warehouses
     */
    std::vector<std::shared_ptr<Warehouse>> getAllWarehouses() const {
        std::vector<std::shared_ptr<Warehouse>> result;
        for (const auto& pair : warehouses_) {
            result.push_back(pair.second);
        }
        return result;
    }

    /**
     * @brief Select warehouse for order fulfillment
     */
    std::shared_ptr<Warehouse> selectWarehouse(const Cart& cart, 
                                                const Address& deliveryAddress) {
        return selectionStrategy_->selectWarehouse(
            getAllWarehouses(), cart, deliveryAddress);
    }

    /**
     * @brief Remove a warehouse
     */
    bool removeWarehouse(const std::string& warehouseId) {
        return warehouses_.erase(warehouseId) > 0;
    }

    /**
     * @brief Clear all warehouses (for testing)
     */
    void clear() {
        warehouses_.clear();
        warehouseIdCounter_ = 0;
    }

    /**
     * @brief Get current strategy name
     */
    std::string getCurrentStrategyName() const {
        return selectionStrategy_ ? selectionStrategy_->getStrategyName() : "None";
    }
};

} // namespace OrderManagement
