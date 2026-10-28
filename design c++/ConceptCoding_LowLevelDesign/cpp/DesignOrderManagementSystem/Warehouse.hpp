#pragma once

#include <string>
#include <memory>
#include "Address.hpp"
#include "Inventory.hpp"

namespace OrderManagement {

/**
 * @brief Class representing a warehouse with inventory and location
 */
class Warehouse {
private:
    std::string warehouseId_;
    std::string name_;
    std::shared_ptr<Address> address_;
    std::unique_ptr<Inventory> inventory_;

public:
    Warehouse(const std::string& warehouseId, const std::string& name,
              std::shared_ptr<Address> address)
        : warehouseId_(warehouseId)
        , name_(name)
        , address_(address)
        , inventory_(std::make_unique<Inventory>()) {}

    // Getters
    std::string getWarehouseId() const { return warehouseId_; }
    std::string getName() const { return name_; }
    std::shared_ptr<Address> getAddress() const { return address_; }
    Inventory* getInventory() { return inventory_.get(); }
    const Inventory* getInventory() const { return inventory_.get(); }

    /**
     * @brief Add product to warehouse inventory
     */
    void addProduct(std::shared_ptr<Product> product, int quantity) {
        inventory_->addProduct(product, quantity);
    }

    /**
     * @brief Check if product is available
     */
    bool hasProduct(const std::string& productId, int quantity = 1) const {
        return inventory_->isAvailable(productId, quantity);
    }

    /**
     * @brief Remove product from inventory (for order fulfillment)
     */
    bool removeProduct(const std::string& productId, int quantity) {
        return inventory_->removeProduct(productId, quantity);
    }

    /**
     * @brief Calculate distance to a given address
     */
    double distanceTo(const Address& deliveryAddress) const {
        return address_->distanceTo(deliveryAddress);
    }

    std::string toString() const {
        std::string str = "Warehouse[" + warehouseId_ + "] " + name_ + "\n";
        str += "  Location: " + address_->toString() + "\n";
        str += "  " + inventory_->toString();
        return str;
    }
};

} // namespace OrderManagement
