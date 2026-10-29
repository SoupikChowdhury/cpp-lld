#ifndef SCREEN_HPP
#define SCREEN_HPP

#include <vector>
#include <memory>
#include <string>
#include "Seat.hpp"
#include "Enums/SeatCategory.hpp"

class Screen {
private:
    int screenId;
    std::string screenName;
    std::vector<std::shared_ptr<Seat>> seats;

public:
    Screen(int id, const std::string& name = "")
        : screenId(id), screenName(name.empty() ? "Screen " + std::to_string(id) : name) {}

    // Getters
    int getScreenId() const { return screenId; }
    std::string getScreenName() const { return screenName; }
    const std::vector<std::shared_ptr<Seat>>& getSeats() const { return seats; }

    // Add a single seat
    void addSeat(std::shared_ptr<Seat> seat) {
        seats.push_back(seat);
    }

    // Create seats with rows and seat numbers
    void createSeats(int silverRows, int goldRows, int platinumRows, int seatsPerRow) {
        int seatId = 1;
        int currentRow = 1;

        // Create SILVER seats (front rows)
        for (int row = 0; row < silverRows; ++row) {
            for (int seatNum = 1; seatNum <= seatsPerRow; ++seatNum) {
                seats.push_back(std::make_shared<Seat>(
                    seatId++, currentRow, seatNum, SeatCategory::SILVER));
            }
            currentRow++;
        }

        // Create GOLD seats (middle rows)
        for (int row = 0; row < goldRows; ++row) {
            for (int seatNum = 1; seatNum <= seatsPerRow; ++seatNum) {
                seats.push_back(std::make_shared<Seat>(
                    seatId++, currentRow, seatNum, SeatCategory::GOLD));
            }
            currentRow++;
        }

        // Create PLATINUM seats (back rows)
        for (int row = 0; row < platinumRows; ++row) {
            for (int seatNum = 1; seatNum <= seatsPerRow; ++seatNum) {
                seats.push_back(std::make_shared<Seat>(
                    seatId++, currentRow, seatNum, SeatCategory::PLATINUM));
            }
            currentRow++;
        }
    }

    int getTotalSeats() const {
        return static_cast<int>(seats.size());
    }

    std::vector<std::shared_ptr<Seat>> getSeatsByCategory(SeatCategory category) const {
        std::vector<std::shared_ptr<Seat>> result;
        for (const auto& seat : seats) {
            if (seat->getSeatCategory() == category) {
                result.push_back(seat);
            }
        }
        return result;
    }

    std::string toString() const {
        return "Screen[ID: " + std::to_string(screenId) + 
               ", Name: " + screenName +
               ", Total Seats: " + std::to_string(getTotalSeats()) + "]";
    }
};

#endif // SCREEN_HPP
