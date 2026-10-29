#pragma once

#include <string>
#include <vector>
#include <iostream>

namespace CarRentalSystem {

class User {
private:
    std::string userId;
    std::string name;
    std::string email;
    std::string phone;
    std::string drivingLicense;
    std::string address;
    bool isActive;

public:
    User(const std::string& id, const std::string& name, const std::string& email,
         const std::string& phone, const std::string& license, const std::string& address = "")
        : userId(id), name(name), email(email), phone(phone),
          drivingLicense(license), address(address), isActive(true) {}

    // Getters
    const std::string& getUserId() const { return userId; }
    const std::string& getName() const { return name; }
    const std::string& getEmail() const { return email; }
    const std::string& getPhone() const { return phone; }
    const std::string& getDrivingLicense() const { return drivingLicense; }
    const std::string& getAddress() const { return address; }
    bool getIsActive() const { return isActive; }

    // Setters
    void setName(const std::string& n) { name = n; }
    void setEmail(const std::string& e) { email = e; }
    void setPhone(const std::string& p) { phone = p; }
    void setAddress(const std::string& a) { address = a; }
    void setActive(bool active) { isActive = active; }

    void displayInfo() const {
        std::cout << "User: " << name << " (ID: " << userId << ")" << std::endl;
        std::cout << "Email: " << email << std::endl;
        std::cout << "Phone: " << phone << std::endl;
        std::cout << "License: " << drivingLicense << std::endl;
    }
};

} // namespace CarRentalSystem
