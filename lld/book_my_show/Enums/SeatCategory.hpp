#ifndef SEAT_CATEGORY_HPP
#define SEAT_CATEGORY_HPP

#include <string>

enum class SeatCategory {
    SILVER,
    GOLD,
    PLATINUM
};

inline std::string seatCategoryToString(SeatCategory category) {
    switch (category) {
        case SeatCategory::SILVER: return "Silver";
        case SeatCategory::GOLD: return "Gold";
        case SeatCategory::PLATINUM: return "Platinum";
        default: return "Unknown";
    }
}

inline double getSeatPrice(SeatCategory category) {
    switch (category) {
        case SeatCategory::SILVER: return 150.0;
        case SeatCategory::GOLD: return 250.0;
        case SeatCategory::PLATINUM: return 400.0;
        default: return 0.0;
    }
}

#endif // SEAT_CATEGORY_HPP
