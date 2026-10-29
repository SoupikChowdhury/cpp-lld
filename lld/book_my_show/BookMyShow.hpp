#ifndef BOOK_MY_SHOW_HPP
#define BOOK_MY_SHOW_HPP

#include <memory>
#include <vector>
#include <map>
#include <string>
#include "MovieController.hpp"
#include "TheatreController.hpp"
#include "Booking.hpp"
#include "Movie.hpp"
#include "Theatre.hpp"
#include "Show.hpp"
#include "Seat.hpp"
#include "Screen.hpp"
#include "Payment.hpp"
#include "Enums/City.hpp"
#include "Enums/SeatCategory.hpp"

class BookMyShow {
private:
    std::shared_ptr<MovieController> movieController;
    std::shared_ptr<TheatreController> theatreController;
    std::vector<std::shared_ptr<Booking>> allBookings;
    
    // Static show ID counter
    static int nextShowId;

    // Private constructor for singleton
    BookMyShow();

    // Singleton instance
    static std::shared_ptr<BookMyShow> instance;

public:
    // Delete copy constructor and assignment operator
    BookMyShow(const BookMyShow&) = delete;
    BookMyShow& operator=(const BookMyShow&) = delete;

    // Get singleton instance
    static std::shared_ptr<BookMyShow> getInstance();

    // Initialize the system with sample data
    void initialize();

    // ==================== Movie Operations ====================
    
    // Get all movies in a city
    std::vector<std::shared_ptr<Movie>> getMoviesInCity(City city) const;
    
    // Search movies by name
    std::vector<std::shared_ptr<Movie>> searchMovies(const std::string& name) const;
    
    // Get movie by ID
    std::shared_ptr<Movie> getMovieById(int movieId) const;
    
    // Add a new movie
    void addMovie(std::shared_ptr<Movie> movie, City city);

    // ==================== Theatre Operations ====================
    
    // Get all theatres in a city
    std::vector<std::shared_ptr<Theatre>> getTheatresInCity(City city) const;
    
    // Get theatre by ID
    std::shared_ptr<Theatre> getTheatreById(int theatreId) const;
    
    // Add a new theatre
    void addTheatre(std::shared_ptr<Theatre> theatre);

    // ==================== Show Operations ====================
    
    // Get shows for a movie in a city
    std::map<std::shared_ptr<Theatre>, std::vector<std::shared_ptr<Show>>> 
    getShowsForMovie(const std::string& movieName, City city) const;
    
    // Get shows for a movie by ID in a city
    std::map<std::shared_ptr<Theatre>, std::vector<std::shared_ptr<Show>>> 
    getShowsForMovieById(int movieId, City city) const;
    
    // Add a show to a theatre
    void addShowToTheatre(std::shared_ptr<Movie> movie, std::shared_ptr<Screen> screen,
                          std::time_t startTime, int theatreId);

    // ==================== Booking Operations ====================
    
    // Get available seats for a show
    std::vector<std::shared_ptr<Seat>> getAvailableSeats(std::shared_ptr<Show> show) const;
    
    // Get available seats by category
    std::vector<std::shared_ptr<Seat>> getAvailableSeatsByCategory(
        std::shared_ptr<Show> show, SeatCategory category) const;
    
    // Create a booking
    std::shared_ptr<Booking> createBooking(
        std::shared_ptr<Show> show,
        const std::vector<std::shared_ptr<Seat>>& seats,
        const std::string& customerName,
        const std::string& customerPhone,
        PaymentMethod paymentMethod);
    
    // Cancel a booking
    bool cancelBooking(int bookingId);
    
    // Get booking by ID
    std::shared_ptr<Booking> getBookingById(int bookingId) const;
    
    // Get all bookings
    const std::vector<std::shared_ptr<Booking>>& getAllBookings() const;

    // ==================== Utility Operations ====================
    
    // Create a screen with seats
    std::shared_ptr<Screen> createScreen(int screenId, const std::string& name,
                                          int silverRows, int goldRows, 
                                          int platinumRows, int seatsPerRow);
    
    // Print all movies in a city
    void printMoviesInCity(City city) const;
    
    // Print shows for a movie in a city
    void printShowsForMovie(const std::string& movieName, City city) const;
};

#endif // BOOK_MY_SHOW_HPP
