#ifndef CITY_HPP
#define CITY_HPP

#include <string>

enum class City {
    BANGALORE,
    DELHI,
    MUMBAI,
    CHENNAI,
    HYDERABAD,
    KOLKATA,
    PUNE
};

inline std::string cityToString(City city) {
    switch (city) {
        case City::BANGALORE: return "Bangalore";
        case City::DELHI: return "Delhi";
        case City::MUMBAI: return "Mumbai";
        case City::CHENNAI: return "Chennai";
        case City::HYDERABAD: return "Hyderabad";
        case City::KOLKATA: return "Kolkata";
        case City::PUNE: return "Pune";
        default: return "Unknown";
    }
}

#endif // CITY_HPP
