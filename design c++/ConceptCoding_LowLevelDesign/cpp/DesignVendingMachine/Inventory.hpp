#pragma once
#include "ItemShelf.hpp"
#include &lt;vector&gt;
#include &lt;stdexcept&gt;

/**
 * Manages the inventory of items in the vending machine
 */
class Inventory {
private:
    std::vector&lt;ItemShelf&gt; inventory;

public:
    explicit Inventory(int itemCount) {
        inventory.resize(itemCount);
        initializeEmptyInventory();
    }

    std::vector&lt;ItemShelf&gt;&amp; getInventory() { return inventory; }

    void initializeEmptyInventory() {
        int startCode = 101;
        for (size_t i = 0; i &lt; inventory.size(); ++i) {
            inventory[i].setCode(startCode);
            inventory[i].setSoldOut(true);
            startCode++;
        }
    }

    void addItem(std::shared_ptr&lt;Item&gt; item, int codeNumber) {
        for (auto&amp; shelf : inventory) {
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

    std::shared_ptr&lt;Item&gt; getItem(int codeNumber) {
        for (auto&amp; shelf : inventory) {
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
        for (auto&amp; shelf : inventory) {
            if (shelf.getCode() == codeNumber) {
                shelf.setSoldOut(true);
                return;
            }
        }
    }
};
