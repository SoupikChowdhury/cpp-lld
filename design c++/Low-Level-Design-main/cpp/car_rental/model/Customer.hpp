#ifndef CUSTOMER_HPP
#define CUSTOMER_HPP

#include <string>

class Customer {
private:
    std::string customerId;
    std::string name;
    std::string email;
    std::string phoneNumber;
    std::string licenseNumber;

public:
    Customer(const std::string& customerId, const std::string& name,
             const std::string& email, const std::string& phoneNumber,
             const std::string& licenseNumber)
        : customerId(customerId), name(name), email(email),
          phoneNumber(phoneNumber), licenseNumber(licenseNumber) {}

    // Getters
    std::string getCustomerId() const { return customerId; }
    std::string getName() const { return name; }
    std::string getEmail() const { return email; }
    std::string getPhoneNumber() const { return phoneNumber; }
    std::string getLicenseNumber() const { return licenseNumber; }
};

#endif // CUSTOMER_HPP
