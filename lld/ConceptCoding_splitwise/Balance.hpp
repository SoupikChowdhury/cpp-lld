#pragma once

/**
 * @brief Represents the balance between two users
 * 
 * Tracks how much one user owes to or should get back from another user.
 */
class Balance {
private:
    double amountOwe;      // Amount this user owes to another
    double amountGetBack;  // Amount this user should get back from another

public:
    Balance() : amountOwe(0.0), amountGetBack(0.0) {}

    // Getters
    double getAmountOwe() const { return amountOwe; }
    double getAmountGetBack() const { return amountGetBack; }

    // Setters
    void setAmountOwe(double amount) { amountOwe = amount; }
    void setAmountGetBack(double amount) { amountGetBack = amount; }
};
