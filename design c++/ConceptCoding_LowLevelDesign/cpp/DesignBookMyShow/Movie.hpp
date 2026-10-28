#ifndef MOVIE_HPP
#define MOVIE_HPP

#include <string>
#include <memory>

class Movie {
private:
    int movieId;
    std::string movieName;
    int durationInMinutes;
    std::string genre;
    std::string language;
    double rating;

public:
    Movie(int id, const std::string& name, int duration)
        : movieId(id), movieName(name), durationInMinutes(duration),
          genre(""), language("English"), rating(0.0) {}

    Movie(int id, const std::string& name, int duration, 
          const std::string& genre, const std::string& language, double rating)
        : movieId(id), movieName(name), durationInMinutes(duration),
          genre(genre), language(language), rating(rating) {}

    // Getters
    int getMovieId() const { return movieId; }
    std::string getMovieName() const { return movieName; }
    int getDurationInMinutes() const { return durationInMinutes; }
    std::string getGenre() const { return genre; }
    std::string getLanguage() const { return language; }
    double getRating() const { return rating; }

    // Setters
    void setMovieName(const std::string& name) { movieName = name; }
    void setDurationInMinutes(int duration) { durationInMinutes = duration; }
    void setGenre(const std::string& g) { genre = g; }
    void setLanguage(const std::string& lang) { language = lang; }
    void setRating(double r) { rating = r; }

    std::string toString() const {
        return "Movie[ID: " + std::to_string(movieId) + 
               ", Name: " + movieName + 
               ", Duration: " + std::to_string(durationInMinutes) + " mins" +
               ", Genre: " + genre +
               ", Language: " + language +
               ", Rating: " + std::to_string(rating) + "]";
    }
};

#endif // MOVIE_HPP
