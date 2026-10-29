#ifndef THEATRE_CONTROLLER_HPP
#define THEATRE_CONTROLLER_HPP

#include <map>
#include <vector>
#include <memory>
#include <algorithm>
#include "Theatre.hpp"
#include "Show.hpp"
#include "Movie.hpp"
#include "Enums/City.hpp"

class TheatreController {
private:
    // Map of City to list of theatres
    std::map<City, std::vector<std::shared_ptr<Theatre>>> cityTheatresMap;
    // All theatres in the system
    std::vector<std::shared_ptr<Theatre>> allTheatres;

    // Singleton instance
    static std::shared_ptr<TheatreController> instance;

    TheatreController() = default;

public:
    // Delete copy constructor and assignment operator
    TheatreController(const TheatreController&) = delete;
    TheatreController& operator=(const TheatreController&) = delete;

    // Get singleton instance
    static std::shared_ptr<TheatreController> getInstance() {
        if (!instance) {
            instance = std::shared_ptr<TheatreController>(new TheatreController());
        }
        return instance;
    }

    // Add theatre to the system
    void addTheatre(std::shared_ptr<Theatre> theatre) {
        allTheatres.push_back(theatre);
        cityTheatresMap[theatre->getCity()].push_back(theatre);
    }

    // Remove theatre from the system
    void removeTheatre(int theatreId) {
        // Remove from city map
        for (auto& [city, theatres] : cityTheatresMap) {
            theatres.erase(
                std::remove_if(theatres.begin(), theatres.end(),
                    [theatreId](const std::shared_ptr<Theatre>& theatre) {
                        return theatre->getTheatreId() == theatreId;
                    }),
                theatres.end()
            );
        }
        // Remove from all theatres
        allTheatres.erase(
            std::remove_if(allTheatres.begin(), allTheatres.end(),
                [theatreId](const std::shared_ptr<Theatre>& theatre) {
                    return theatre->getTheatreId() == theatreId;
                }),
            allTheatres.end()
        );
    }

    // Get all theatres in a city
    std::vector<std::shared_ptr<Theatre>> getTheatresByCity(City city) const {
        auto it = cityTheatresMap.find(city);
        if (it != cityTheatresMap.end()) {
            return it->second;
        }
        return {};
    }

    // Get theatre by ID
    std::shared_ptr<Theatre> getTheatreById(int theatreId) const {
        for (const auto& theatre : allTheatres) {
            if (theatre->getTheatreId() == theatreId) {
                return theatre;
            }
        }
        return nullptr;
    }

    // Get all theatres showing a specific movie in a city
    std::map<std::shared_ptr<Theatre>, std::vector<std::shared_ptr<Show>>> 
    getShowsForMovie(int movieId, City city) const {
        std::map<std::shared_ptr<Theatre>, std::vector<std::shared_ptr<Show>>> result;
        
        auto theatres = getTheatresByCity(city);
        for (const auto& theatre : theatres) {
            auto shows = theatre->getShowsForMovie(movieId);
            if (!shows.empty()) {
                result[theatre] = shows;
            }
        }
        return result;
    }

    // Get all shows for a movie by name in a city
    std::map<std::shared_ptr<Theatre>, std::vector<std::shared_ptr<Show>>> 
    getShowsForMovie(const std::string& movieName, City city) const {
        std::map<std::shared_ptr<Theatre>, std::vector<std::shared_ptr<Show>>> result;
        
        auto theatres = getTheatresByCity(city);
        for (const auto& theatre : theatres) {
            auto shows = theatre->getShowsForMovie(movieName);
            if (!shows.empty()) {
                result[theatre] = shows;
            }
        }
        return result;
    }

    // Get all shows in a city
    std::vector<std::shared_ptr<Show>> getAllShowsInCity(City city) const {
        std::vector<std::shared_ptr<Show>> allShows;
        auto theatres = getTheatresByCity(city);
        for (const auto& theatre : theatres) {
            const auto& shows = theatre->getShows();
            allShows.insert(allShows.end(), shows.begin(), shows.end());
        }
        return allShows;
    }

    // Get all theatres
    const std::vector<std::shared_ptr<Theatre>>& getAllTheatres() const {
        return allTheatres;
    }

    // Add show to a theatre
    void addShowToTheatre(std::shared_ptr<Show> show, int theatreId) {
        auto theatre = getTheatreById(theatreId);
        if (theatre) {
            theatre->addShow(show);
        }
    }

    // Remove show from a theatre
    void removeShowFromTheatre(int showId, int theatreId) {
        auto theatre = getTheatreById(theatreId);
        if (theatre) {
            theatre->removeShow(showId);
        }
    }
};

// Initialize singleton
inline std::shared_ptr<TheatreController> TheatreController::instance = nullptr;

#endif // THEATRE_CONTROLLER_HPP
