#include "BookMyShow.hpp"
#include <iostream>
#include <iomanip>

// Initialize static members
std::shared_ptr<BookMyShow> BookMyShow::instance = nullptr;
int BookMyShow::nextShowId = 1;

BookMyShow::BookMyShow() {
    movieController = MovieController::getInstance();
    theatreController = TheatreController::getInstance();
}

std::shared_ptr<BookMyShow> BookMyShow::getInstance() {
    if (!instance) {
        instance = std::shared_ptr<BookMyShow>(new BookMyShow());
    }
    return instance;
}

void BookMyShow::initialize() {
    // Create some sample movies
    auto movie1 = std::make_shared<Movie>(1, "Avengers: Endgame", 180, "Action", "English", 8.5);
    auto movie2 = std::make_shared<Movie>(2, "The Dark Knight", 152, "Action", "English", 9.0);
    auto movie3 = std::make_shared<Movie>(3, "Inception", 148, "Sci-Fi", "English", 8.8);
    auto movie4 = std::make_shared<Movie>(4, "3 Idiots", 170, "Comedy", "Hindi", 8.4);

    // Add movies to the system
    movieController->addMovie(movie1);
    movieController->addMovie(movie2);
    movieController->addMovie(movie3);
    movieController->addMovie(movie4);

    // Add movies to cities
    movieController->addMovieToCity(movie1, City::BANGALORE);
    movieController->addMovieToCity(movie2, City::BANGALORE);
    movieController->addMovieToCity(movie3, City::BANGALORE);
    movieController->addMovieToCity(movie1, City::MUMBAI);
    movieController->addMovieToCity(movie4, City::MUMBAI);
    movieController->addMovieToCity(movie2, City::DELHI);

    // Create theatres in Bangalore
    auto pvr1 = std::make_shared<Theatre>(1, "PVR Forum Mall", "Koramangala", City::BANGALORE);
    auto screen1 = createScreen(1, "Screen 1", 3, 3, 2, 10);
    auto screen2 = createScreen(2, "Screen 2", 2, 4, 2, 8);
    pvr1->addScreen(screen1);
    pvr1->addScreen(screen2);

    auto inox1 = std::make_shared<Theatre>(2, "INOX Garuda Mall", "MG Road", City::BANGALORE);
    auto screen3 = createScreen(3, "Audi 1", 3, 3, 2, 12);
    inox1->addScreen(screen3);

    // Create theatres in Mumbai
    auto pvr2 = std::make_shared<Theatre>(3, "PVR Phoenix", "Lower Parel", City::MUMBAI);
    auto screen4 = createScreen(4, "Screen 1", 3, 4, 3, 10);
    pvr2->addScreen(screen4);

    // Add theatres to the system
    theatreController->addTheatre(pvr1);
    theatreController->addTheatre(inox1);
    theatreController->addTheatre(pvr2);

    // Create shows with different times
    std::time_t now = std::time(nullptr);
    std::time_t morning = now + 2 * 3600;    // 2 hours from now
    std::time_t afternoon = now + 5 * 3600;  // 5 hours from now
    std::time_t evening = now + 8 * 3600;    // 8 hours from now

    // Add shows to PVR Forum (Bangalore)
    addShowToTheatre(movie1, screen1, morning, 1);
    addShowToTheatre(movie1, screen1, evening, 1);
    addShowToTheatre(movie2, screen2, afternoon, 1);

    // Add shows to INOX (Bangalore)
    addShowToTheatre(movie3, screen3, morning, 2);
    addShowToTheatre(movie2, screen3, evening, 2);

    // Add shows to PVR Phoenix (Mumbai)
    addShowToTheatre(movie1, screen4, morning, 3);
    addShowToTheatre(movie4, screen4, evening, 3);
}

// ==================== Movie Operations ====================

std::vector<std::shared_ptr<Movie>> BookMyShow::getMoviesInCity(City city) const {
    return movieController->getMoviesByCity(city);
}

std::vector<std::shared_ptr<Movie>> BookMyShow::searchMovies(const std::string& name) const {
    return movieController->searchMovies(name);
}

std::shared_ptr<Movie> BookMyShow::getMovieById(int movieId) const {
    return movieController->getMovieById(movieId);
}

void BookMyShow::addMovie(std::shared_ptr<Movie> movie, City city) {
    movieController->addMovie(movie);
    movieController->addMovieToCity(movie, city);
}

// ==================== Theatre Operations ====================

std::vector<std::shared_ptr<Theatre>> BookMyShow::getTheatresInCity(City city) const {
    return theatreController->getTheatresByCity(city);
}

std::shared_ptr<Theatre> BookMyShow::getTheatreById(int theatreId) const {
    return theatreController->getTheatreById(theatreId);
}

void BookMyShow::addTheatre(std::shared_ptr<Theatre> theatre) {
    theatreController->addTheatre(theatre);
}

// ==================== Show Operations ====================

std::map<std::shared_ptr<Theatre>, std::vector<std::shared_ptr<Show>>> 
BookMyShow::getShowsForMovie(const std::string& movieName, City city) const {
    return theatreController->getShowsForMovie(movieName, city);
}

std::map<std::shared_ptr<Theatre>, std::vector<std::shared_ptr<Show>>> 
BookMyShow::getShowsForMovieById(int movieId, City city) const {
    return theatreController->getShowsForMovie(movieId, city);
}

void BookMyShow::addShowToTheatre(std::shared_ptr<Movie> movie, std::shared_ptr<Screen> screen,
                                   std::time_t startTime, int theatreId) {
    auto show = std::make_shared<Show>(nextShowId++, movie, screen, startTime);
    theatreController->addShowToTheatre(show, theatreId);
}

// ==================== Booking Operations ====================

std::vector<std::shared_ptr<Seat>> BookMyShow::getAvailableSeats(std::shared_ptr<Show> show) const {
    return show->getAvailableSeats();
}

std::vector<std::shared_ptr<Seat>> BookMyShow::getAvailableSeatsByCategory(
    std::shared_ptr<Show> show, SeatCategory category) const {
    return show->getAvailableSeatsByCategory(category);
}

std::shared_ptr<Booking> BookMyShow::createBooking(
    std::shared_ptr<Show> show,
    const std::vector<std::shared_ptr<Seat>>& seats,
    const std::string& customerName,
    const std::string& customerPhone,
    PaymentMethod paymentMethod) {
    
    // Check if seats are available
    if (!show->areSeatsAvailable(seats)) {
        std::cout << "Error: One or more selected seats are not available.\n";
        return nullptr;
    }

    // Create booking
    auto booking = std::make_shared<Booking>(show, seats, customerName, customerPhone);
    
    // Process payment
    if (booking->makePayment(paymentMethod)) {
        allBookings.push_back(booking);
        return booking;
    }

    std::cout << "Error: Payment failed or seats were booked by another user.\n";
    return nullptr;
}

bool BookMyShow::cancelBooking(int bookingId) {
    auto booking = getBookingById(bookingId);
    if (booking) {
        return booking->cancelBooking();
    }
    return false;
}

std::shared_ptr<Booking> BookMyShow::getBookingById(int bookingId) const {
    for (const auto& booking : allBookings) {
        if (booking->getBookingId() == bookingId) {
            return booking;
        }
    }
    return nullptr;
}

const std::vector<std::shared_ptr<Booking>>& BookMyShow::getAllBookings() const {
    return allBookings;
}

// ==================== Utility Operations ====================

std::shared_ptr<Screen> BookMyShow::createScreen(int screenId, const std::string& name,
                                                  int silverRows, int goldRows, 
                                                  int platinumRows, int seatsPerRow) {
    auto screen = std::make_shared<Screen>(screenId, name);
    screen->createSeats(silverRows, goldRows, platinumRows, seatsPerRow);
    return screen;
}

void BookMyShow::printMoviesInCity(City city) const {
    std::cout << "\n=== Movies in " << cityToString(city) << " ===\n";
    auto movies = getMoviesInCity(city);
    if (movies.empty()) {
        std::cout << "No movies available.\n";
        return;
    }
    for (const auto& movie : movies) {
        std::cout << "  - " << movie->getMovieName() 
                  << " (" << movie->getLanguage() << ", " 
                  << movie->getDurationInMinutes() << " mins)\n";
    }
}

void BookMyShow::printShowsForMovie(const std::string& movieName, City city) const {
    std::cout << "\n=== Shows for '" << movieName << "' in " << cityToString(city) << " ===\n";
    auto theatreShows = getShowsForMovie(movieName, city);
    
    if (theatreShows.empty()) {
        std::cout << "No shows available.\n";
        return;
    }

    for (const auto& [theatre, shows] : theatreShows) {
        std::cout << "\n" << theatre->getTheatreName() << " (" << theatre->getAddress() << ")\n";
        for (const auto& show : shows) {
            std::cout << "  - " << show->getStartTimeFormatted() 
                      << " | " << show->getScreen()->getScreenName()
                      << " | Available: " << show->getAvailableSeatCount() << " seats\n";
        }
    }
}
