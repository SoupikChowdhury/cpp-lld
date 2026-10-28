#pragma once

/**
 * Vending Machine - Coin Denominations
 */
enum class Coin {
    PENNY = 1,
    NICKEL = 5,
    DIME = 10,
    QUARTER = 25
};

inline int getCoinValue(Coin coin) {
    return static_cast&lt;int&gt;(coin);
}
