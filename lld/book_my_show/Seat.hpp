#ifndef SEAT_HPP
#define SEAT_HPP

#include <string>
#include <memory>
#include "Enums/SeatCategory.hpp"

class Seat {
private:
    int seatId;
    int rowNumber;
    int seatNumber;
    SeatCategory seatCategory;

public:
    Seat(int id, int row, int seatNum, SeatCategory category)
        : seatId(id), rowNumber(row), seatNumber(seatNum), seatCategory(category) {}

    // Getters
    int getSeatId() const { return seatId; }
    int getRowNumber() const { return rowNumber; }
    int getSeatNumber() const { return seatNumber; }
    SeatCategory getSeatCategory() const { return seatCategory; }

    // Setters
    void setSeatCategory(SeatCategory category) { seatCategory = category; }

    double getPrice() const {
        return getSeatPrice(seatCategory);
    }

    std::string getSeatLabel() const {
        char rowChar = 'A' + rowNumber - 1;
        return std::string(1, rowChar) + std::to_string(seatNumber);
    }

    std::string toString() const {
        return "Seat[" + getSeatLabel() + 
               ", Category: " + seatCategoryToString(seatCategory) +
               ", Price: Rs." + std::to_string(static_cast<int>(getPrice())) + "]";
    }

    bool operator==(const Seat& other) const {
        return seatId == other.seatId;
    }
};

#endif // SEAT_HPP
