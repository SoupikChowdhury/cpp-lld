#pragma once
#include "Item.hpp"
#include &lt;memory&gt;

/**
 * Represents a slot/shelf in the vending machine
 */
class ItemShelf {
private:
    int code;
    std::shared_ptr&lt;Item&gt; item;
    bool soldOut;

public:
    ItemShelf() : code(0), item(nullptr), soldOut(true) {}

    int getCode() const { return code; }
    void setCode(int c) { code = c; }

    std::shared_ptr&lt;Item&gt; getItem() const { return item; }
    void setItem(std::shared_ptr&lt;Item&gt; i) { item = i; }

    bool isSoldOut() const { return soldOut; }
    void setSoldOut(bool s) { soldOut = s; }
};
