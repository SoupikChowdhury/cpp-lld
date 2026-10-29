#pragma once
#include "Item.hpp"
#include <memory>

/**
 * Represents a slot/shelf in the vending machine
 */
class ItemShelf {
private:
    int code;
    std::shared_ptr<Item> item;
    bool soldOut;

public:
    ItemShelf() : code(0), item(nullptr), soldOut(true) {}

    int getCode() const { return code; }
    void setCode(int c) { code = c; }

    std::shared_ptr<Item> getItem() const { return item; }
    void setItem(std::shared_ptr<Item> i) { item = i; }

    bool isSoldOut() const { return soldOut; }
    void setSoldOut(bool s) { soldOut = s; }
};
