#ifndef MOVIE_CONTROLLER_HPP
#define MOVIE_CONTROLLER_HPP

#include <map>
#include <vector>
#include <memory>
#include <string>
#include <algorithm>
#include "Movie.hpp"
#include "Enums/City.hpp"

class MovieController {
private:
    // Map of City to list of movies
    std::map<City, std::vector<std::shared_ptr<Movie>>> cityMoviesMap;
    // All movies in the system
    std::vector<std::shared_ptr<Movie>> allMovies;

    // Singleton instance
    static std::shared_ptr<MovieController> instance;

    MovieController() = default;

public:
    // Delete copy constructor and assignment operator
    MovieController(const MovieController&) = delete;
    MovieController& operator=(const MovieController&) = delete;

    // Get singleton instance
    static std::shared_ptr<MovieController> getInstance() {
        if (!instance) {
            instance = std::shared_ptr<MovieController>(new MovieController());
        }
        return instance;
    }

    // Add movie to the system
    void addMovie(std::shared_ptr<Movie> movie) {
        allMovies.push_back(movie);
    }

    // Add movie to a specific city
    void addMovieToCity(std::shared_ptr<Movie> movie, City city) {
        // Check if movie already exists in this city
        auto& cityMovies = cityMoviesMap[city];
        for (const auto& m : cityMovies) {
            if (m->getMovieId() == movie->getMovieId()) {
                return; // Already exists
            }
        }
        cityMovies.push_back(movie);
    }

    // Remove movie from a city
    void removeMovieFromCity(int movieId, City city) {
        auto& cityMovies = cityMoviesMap[city];
        cityMovies.erase(
            std::remove_if(cityMovies.begin(), cityMovies.end(),
                [movieId](const std::shared_ptr<Movie>& movie) {
                    return movie->getMovieId() == movieId;
                }),
            cityMovies.end()
        );
    }

    // Get all movies in a city
    std::vector<std::shared_ptr<Movie>> getMoviesByCity(City city) const {
        auto it = cityMoviesMap.find(city);
        if (it != cityMoviesMap.end()) {
            return it->second;
        }
        return {};
    }

    // Get movie by ID
    std::shared_ptr<Movie> getMovieById(int movieId) const {
        for (const auto& movie : allMovies) {
            if (movie->getMovieId() == movieId) {
                return movie;
            }
        }
        return nullptr;
    }

    // Get movie by name
    std::shared_ptr<Movie> getMovieByName(const std::string& name) const {
        for (const auto& movie : allMovies) {
            if (movie->getMovieName() == name) {
                return movie;
            }
        }
        return nullptr;
    }

    // Search movies by name pattern
    std::vector<std::shared_ptr<Movie>> searchMovies(const std::string& pattern) const {
        std::vector<std::shared_ptr<Movie>> results;
        for (const auto& movie : allMovies) {
            if (movie->getMovieName().find(pattern) != std::string::npos) {
                results.push_back(movie);
            }
        }
        return results;
    }

    // Search movies by genre
    std::vector<std::shared_ptr<Movie>> getMoviesByGenre(const std::string& genre) const {
        std::vector<std::shared_ptr<Movie>> results;
        for (const auto& movie : allMovies) {
            if (movie->getGenre() == genre) {
                results.push_back(movie);
            }
        }
        return results;
    }

    // Get all movies
    const std::vector<std::shared_ptr<Movie>>& getAllMovies() const {
        return allMovies;
    }

    // Get cities where a movie is available
    std::vector<City> getCitiesForMovie(int movieId) const {
        std::vector<City> cities;
        for (const auto& [city, movies] : cityMoviesMap) {
            for (const auto& movie : movies) {
                if (movie->getMovieId() == movieId) {
                    cities.push_back(city);
                    break;
                }
            }
        }
        return cities;
    }
};

// Initialize singleton
inline std::shared_ptr<MovieController> MovieController::instance = nullptr;

#endif // MOVIE_CONTROLLER_HPP
