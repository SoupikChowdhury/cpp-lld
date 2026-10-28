#pragma once

#include <string>

namespace CarRentalSystem {

class Location {
private:
    std::string locationId;
    std::string name;
    std::string address;
    std::string city;
    std::string state;
    std::string zipCode;
    double latitude;
    double longitude;

public:
    Location(const std::string& id, const std::string& name, const std::string& address,
             const std::string& city, const std::string& state, const std::string& zip,
             double lat = 0.0, double lng = 0.0)
        : locationId(id), name(name), address(address), city(city), 
          state(state), zipCode(zip), latitude(lat), longitude(lng) {}

    // Getters
    const std::string& getLocationId() const { return locationId; }
    const std::string& getName() const { return name; }
    const std::string& getAddress() const { return address; }
    const std::string& getCity() const { return city; }
    const std::string& getState() const { return state; }
    const std::string& getZipCode() const { return zipCode; }
    double getLatitude() const { return latitude; }
    double getLongitude() const { return longitude; }

    std::string getFullAddress() const {
        return address + ", " + city + ", " + state + " " + zipCode;
    }

    // Calculate distance to another location (simplified Euclidean)
    double distanceTo(const Location& other) const {
        double dx = latitude - other.latitude;
        double dy = longitude - other.longitude;
        return std::sqrt(dx * dx + dy * dy) * 111; // Approximate km
    }
};

} // namespace CarRentalSystem
