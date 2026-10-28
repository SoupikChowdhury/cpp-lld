#pragma once
#include "ItemType.hpp"

/**
 * Represents a product in the vending machine
 */
class Item {
private:
    ItemType type;
    int price;

public:
    Item() : type(ItemType::COKE), price(0) {}

    ItemType getType() const { return type; }
    void setType(ItemType t) { type = t; }

    int getPrice() const { return price; }
    void setPrice(int p) { price = p; }
};
