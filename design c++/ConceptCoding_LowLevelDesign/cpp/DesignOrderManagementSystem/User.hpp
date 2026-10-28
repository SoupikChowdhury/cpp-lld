#pragma once

#include <string>
#include <memory>
#include "Address.hpp"

namespace OrderManagement {

/**
 * @brief Class representing a user/customer in the system
 */
class User {
private:
    std::string userId_;
    std::string name_;
    std::string email_;
    std::string phone_;
    std::shared_ptr<Address> address_;

public:
    User(const std::string& userId, const std::string& name,
         const std::string& email, const std::string& phone,
         std::shared_ptr<Address> address = nullptr)
        : userId_(userId)
        , name_(name)
        , email_(email)
        , phone_(phone)
        , address_(address) {}

    // Getters
    std::string getUserId() const { return userId_; }
    std::string getName() const { return name_; }
    std::string getEmail() const { return email_; }
    std::string getPhone() const { return phone_; }
    std::shared_ptr<Address> getAddress() const { return address_; }

    // Setters
    void setAddress(std::shared_ptr<Address> address) { address_ = address; }
    void setPhone(const std::string& phone) { phone_ = phone; }
    void setEmail(const std::string& email) { email_ = email; }

    std::string toString() const {
        std::string str = "User[" + userId_ + "] " + name_ + " (" + email_ + ")";
        if (address_) {
            str += "\n  Address: " + address_->toString();
        }
        return str;
    }
};

} // namespace OrderManagement
