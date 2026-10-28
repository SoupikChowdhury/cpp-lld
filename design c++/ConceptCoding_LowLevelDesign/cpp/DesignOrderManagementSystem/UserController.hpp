#pragma once

#include <unordered_map>
#include <memory>
#include <vector>
#include <stdexcept>
#include "User.hpp"

namespace OrderManagement {

/**
 * @brief Controller class for managing users
 * Singleton pattern for centralized user management
 */
class UserController {
private:
    std::unordered_map<std::string, std::shared_ptr<User>> users_;
    int userIdCounter_ = 0;

    UserController() = default;

public:
    // Singleton instance
    static UserController& getInstance() {
        static UserController instance;
        return instance;
    }

    // Delete copy constructor and assignment operator
    UserController(const UserController&) = delete;
    UserController& operator=(const UserController&) = delete;

    /**
     * @brief Register a new user
     */
    std::shared_ptr<User> registerUser(const std::string& name,
                                       const std::string& email,
                                       const std::string& phone,
                                       std::shared_ptr<Address> address = nullptr) {
        std::string userId = "U" + std::to_string(++userIdCounter_);
        auto user = std::make_shared<User>(userId, name, email, phone, address);
        users_[userId] = user;
        return user;
    }

    /**
     * @brief Get user by ID
     */
    std::shared_ptr<User> getUser(const std::string& userId) const {
        auto it = users_.find(userId);
        if (it != users_.end()) {
            return it->second;
        }
        throw std::runtime_error("User not found: " + userId);
    }

    /**
     * @brief Get all users
     */
    std::vector<std::shared_ptr<User>> getAllUsers() const {
        std::vector<std::shared_ptr<User>> result;
        for (const auto& pair : users_) {
            result.push_back(pair.second);
        }
        return result;
    }

    /**
     * @brief Update user address
     */
    void updateUserAddress(const std::string& userId, std::shared_ptr<Address> address) {
        auto user = getUser(userId);
        user->setAddress(address);
    }

    /**
     * @brief Remove a user
     */
    bool removeUser(const std::string& userId) {
        return users_.erase(userId) > 0;
    }

    /**
     * @brief Clear all users (for testing)
     */
    void clear() {
        users_.clear();
        userIdCounter_ = 0;
    }
};

} // namespace OrderManagement
