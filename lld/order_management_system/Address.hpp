#pragma once

#include <string>
#include <cmath>

namespace OrderManagement {

/**
 * @brief Class representing an address
 */
class Address {
private:
    std::string street_;
    std::string city_;
    std::string state_;
    std::string zipCode_;
    std::string country_;
    double latitude_;   // For distance calculation
    double longitude_;  // For distance calculation

public:
    Address(const std::string& street, const std::string& city,
            const std::string& state, const std::string& zipCode,
            const std::string& country, double latitude = 0.0, double longitude = 0.0)
        : street_(street)
        , city_(city)
        , state_(state)
        , zipCode_(zipCode)
        , country_(country)
        , latitude_(latitude)
        , longitude_(longitude) {}

    // Getters
    std::string getStreet() const { return street_; }
    std::string getCity() const { return city_; }
    std::string getState() const { return state_; }
    std::string getZipCode() const { return zipCode_; }
    std::string getCountry() const { return country_; }
    double getLatitude() const { return latitude_; }
    double getLongitude() const { return longitude_; }

    // Setters
    void setCoordinates(double lat, double lon) {
        latitude_ = lat;
        longitude_ = lon;
    }

    std::string toString() const {
        return street_ + ", " + city_ + ", " + state_ + " " + zipCode_ + ", " + country_;
    }

    // Calculate distance between two addresses (simplified Euclidean distance)
    double distanceTo(const Address& other) const {
        double dx = latitude_ - other.latitude_;
        double dy = longitude_ - other.longitude_;
        return std::sqrt(dx * dx + dy * dy);
    }
};

} // namespace OrderManagement
