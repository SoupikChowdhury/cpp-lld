#pragma once
#include "ItemShelf.hpp"
#include <vector>
#include <stdexcept>

/**
 * Manages the inventory of items in the vending machine
 */
class Inventory {
private:
    std::vector<ItemShelf> inventory;

public:
    explicit Inventory(int itemCount) {
        inventory.resize(itemCount);
        initializeEmptyInventory();
    }

    std::vector<ItemShelf>& getInventory() { return inventory; }

    void initializeEmptyInventory() {
        int startCode = 101;
        for (size_t i = 0; i < inventory.size(); ++i) {
            inventory[i].setCode(startCode);
            inventory[i].setSoldOut(true);
            startCode++;
        }
    }

    void addItem(std::shared_ptr<Item> item, int codeNumber) {
        for (auto& shelf : inventory) {
            if (shelf.getCode() == codeNumber) {
                if (shelf.isSoldOut()) {
                    shelf.setItem(item);
                    shelf.setSoldOut(false);
                    return;
                } else {
                    throw std::runtime_error("Item already present at this location");
                }
            }
        }
        throw std::runtime_error("Invalid code number");
    }

    std::shared_ptr<Item> getItem(int codeNumber) {
        for (auto& shelf : inventory) {
            if (shelf.getCode() == codeNumber) {
                if (shelf.isSoldOut()) {
                    throw std::runtime_error("Item already sold out");
                } else {
                    return shelf.getItem();
                }
            }
        }
        throw std::runtime_error("Invalid code");
    }

    void updateSoldOutItem(int codeNumber) {
        for (auto& shelf : inventory) {
            if (shelf.getCode() == codeNumber) {
                shelf.setSoldOut(true);
                return;
            }
        }
    }
};
