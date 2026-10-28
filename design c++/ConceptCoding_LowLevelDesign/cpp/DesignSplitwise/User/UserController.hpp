#pragma once

#include <vector>
#include <memory>
#include <string>
#include "User.hpp"

/**
 * @brief Controller for managing users in the Splitwise system
 * 
 * Provides functionality to add users, retrieve users by ID,
 * and get all users in the system.
 */
class UserController {
private:
    std::vector<std::shared_ptr<User>> userList;

public:
    UserController() = default;

    /**
     * @brief Add a new user to the system
     * @param user Shared pointer to the user to add
     */
    void addUser(std::shared_ptr<User> user) {
        userList.push_back(user);
    }

    /**
     * @brief Get a user by their ID
     * @param userId The unique identifier of the user
     * @return Shared pointer to the user, or nullptr if not found
     */
    std::shared_ptr<User> getUser(const std::string& userId) {
        for (auto& user : userList) {
            if (user->getUserId() == userId) {
                return user;
            }
        }
        return nullptr;
    }

    /**
     * @brief Get all users in the system
     * @return Reference to the vector of all users
     */
    const std::vector<std::shared_ptr<User>>& getAllUsers() const {
        return userList;
    }
};
