#pragma once
#include <string>

/**
 * Item types available in the vending machine
 */
enum class ItemType {
    COKE,
    PEPSI,
    JUICE,
    SODA
};

inline std::string itemTypeToString(ItemType type) {
    switch (type) {
        case ItemType::COKE: return "COKE";
        case ItemType::PEPSI: return "PEPSI";
        case ItemType::JUICE: return "JUICE";
        case ItemType::SODA: return "SODA";
        default: return "UNKNOWN";
    }
}
