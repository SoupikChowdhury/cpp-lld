#ifndef SHOW_HPP
#define SHOW_HPP

#include <memory>
#include <vector>
#include <set>
#include <string>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <mutex>
#include <algorithm>
#include "Movie.hpp"
#include "Screen.hpp"
#include "Seat.hpp"

class Show {
private:
    int showId;
    std::shared_ptr<Movie> movie;
    std::shared_ptr<Screen> screen;
    std::time_t startTime;
    std::set<int> bookedSeatIds;  // Set of booked seat IDs
    mutable std::mutex bookingMutex;  // For thread-safe booking

public:
    Show(int id, std::shared_ptr<Movie> movie, std::shared_ptr<Screen> screen, std::time_t startTime)
        : showId(id), movie(movie), screen(screen), startTime(startTime) {}

    // Getters
    int getShowId() const { return showId; }
    std::shared_ptr<Movie> getMovie() const { return movie; }
    std::shared_ptr<Screen> getScreen() const { return screen; }
    std::time_t getStartTime() const { return startTime; }

    // Get show time as formatted string
    std::string getStartTimeFormatted() const {
        std::tm* tm = std::localtime(&startTime);
        std::ostringstream oss;
        oss << std::put_time(tm, "%Y-%m-%d %H:%M");
        return oss.str();
    }

    // Check if specific seats are available
    bool areSeatsAvailable(const std::vector<std::shared_ptr<Seat>>& requestedSeats) const {
        std::lock_guard<std::mutex> lock(bookingMutex);
        for (const auto& seat : requestedSeats) {
            if (bookedSeatIds.find(seat->getSeatId()) != bookedSeatIds.end()) {
                return false;
            }
        }
        return true;
    }

    // Book seats (returns true if successful)
    bool bookSeats(const std::vector<std::shared_ptr<Seat>>& seatsToBook) {
        std::lock_guard<std::mutex> lock(bookingMutex);
        
        // First verify all seats are available
        for (const auto& seat : seatsToBook) {
            if (bookedSeatIds.find(seat->getSeatId()) != bookedSeatIds.end()) {
                return false;  // Seat already booked
            }
        }

        // Book all seats
        for (const auto& seat : seatsToBook) {
            bookedSeatIds.insert(seat->getSeatId());
        }
        return true;
    }

    // Cancel seat booking
    void cancelSeats(const std::vector<std::shared_ptr<Seat>>& seatsToCancel) {
        std::lock_guard<std::mutex> lock(bookingMutex);
        for (const auto& seat : seatsToCancel) {
            bookedSeatIds.erase(seat->getSeatId());
        }
    }

    // Get available seats
    std::vector<std::shared_ptr<Seat>> getAvailableSeats() const {
        std::lock_guard<std::mutex> lock(bookingMutex);
        std::vector<std::shared_ptr<Seat>> availableSeats;
        for (const auto& seat : screen->getSeats()) {
            if (bookedSeatIds.find(seat->getSeatId()) == bookedSeatIds.end()) {
                availableSeats.push_back(seat);
            }
        }
        return availableSeats;
    }

    // Get available seats by category
    std::vector<std::shared_ptr<Seat>> getAvailableSeatsByCategory(SeatCategory category) const {
        std::lock_guard<std::mutex> lock(bookingMutex);
        std::vector<std::shared_ptr<Seat>> availableSeats;
        for (const auto& seat : screen->getSeats()) {
            if (seat->getSeatCategory() == category && 
                bookedSeatIds.find(seat->getSeatId()) == bookedSeatIds.end()) {
                availableSeats.push_back(seat);
            }
        }
        return availableSeats;
    }

    int getAvailableSeatCount() const {
        std::lock_guard<std::mutex> lock(bookingMutex);
        return screen->getTotalSeats() - static_cast<int>(bookedSeatIds.size());
    }

    int getBookedSeatCount() const {
        std::lock_guard<std::mutex> lock(bookingMutex);
        return static_cast<int>(bookedSeatIds.size());
    }

    std::string toString() const {
        return "Show[ID: " + std::to_string(showId) + 
               ", Movie: " + movie->getMovieName() +
               ", Screen: " + screen->getScreenName() +
               ", Time: " + getStartTimeFormatted() +
               ", Available: " + std::to_string(getAvailableSeatCount()) + "/" + 
               std::to_string(screen->getTotalSeats()) + "]";
    }
};

#endif // SHOW_HPP
