#ifndef THEATRE_HPP
#define THEATRE_HPP

#include <vector>
#include <memory>
#include <string>
#include <algorithm>
#include "Screen.hpp"
#include "Show.hpp"
#include "Movie.hpp"
#include "Enums/City.hpp"

class Theatre {
private:
    int theatreId;
    std::string theatreName;
    std::string address;
    City city;
    std::vector<std::shared_ptr<Screen>> screens;
    std::vector<std::shared_ptr<Show>> shows;

public:
    Theatre(int id, const std::string& name, const std::string& addr, City city)
        : theatreId(id), theatreName(name), address(addr), city(city) {}

    // Getters
    int getTheatreId() const { return theatreId; }
    std::string getTheatreName() const { return theatreName; }
    std::string getAddress() const { return address; }
    City getCity() const { return city; }
    const std::vector<std::shared_ptr<Screen>>& getScreens() const { return screens; }
    const std::vector<std::shared_ptr<Show>>& getShows() const { return shows; }

    // Setters
    void setTheatreName(const std::string& name) { theatreName = name; }
    void setAddress(const std::string& addr) { address = addr; }

    // Add screen
    void addScreen(std::shared_ptr<Screen> screen) {
        screens.push_back(screen);
    }

    // Add show
    void addShow(std::shared_ptr<Show> show) {
        shows.push_back(show);
    }

    // Remove show
    void removeShow(int showId) {
        shows.erase(
            std::remove_if(shows.begin(), shows.end(),
                [showId](const std::shared_ptr<Show>& show) {
                    return show->getShowId() == showId;
                }),
            shows.end()
        );
    }

    // Get shows for a specific movie
    std::vector<std::shared_ptr<Show>> getShowsForMovie(int movieId) const {
        std::vector<std::shared_ptr<Show>> result;
        for (const auto& show : shows) {
            if (show->getMovie()->getMovieId() == movieId) {
                result.push_back(show);
            }
        }
        return result;
    }

    // Get shows for a specific movie by name
    std::vector<std::shared_ptr<Show>> getShowsForMovie(const std::string& movieName) const {
        std::vector<std::shared_ptr<Show>> result;
        for (const auto& show : shows) {
            if (show->getMovie()->getMovieName() == movieName) {
                result.push_back(show);
            }
        }
        return result;
    }

    // Get all movies currently showing
    std::vector<std::shared_ptr<Movie>> getCurrentMovies() const {
        std::vector<std::shared_ptr<Movie>> movies;
        std::set<int> movieIds;
        for (const auto& show : shows) {
            if (movieIds.find(show->getMovie()->getMovieId()) == movieIds.end()) {
                movies.push_back(show->getMovie());
                movieIds.insert(show->getMovie()->getMovieId());
            }
        }
        return movies;
    }

    std::string toString() const {
        return "Theatre[ID: " + std::to_string(theatreId) + 
               ", Name: " + theatreName +
               ", City: " + cityToString(city) +
               ", Screens: " + std::to_string(screens.size()) +
               ", Shows: " + std::to_string(shows.size()) + "]";
    }
};

#endif // THEATRE_HPP
