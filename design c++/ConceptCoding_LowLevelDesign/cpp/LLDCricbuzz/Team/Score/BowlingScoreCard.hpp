#pragma once

#include <string>

namespace Cricbuzz {

/**
 * @brief Class representing bowling statistics for a player in an inning
 */
class BowlingScoreCard {
private:
    int oversBowled_ = 0;
    int ballsBowled_ = 0;  // Balls in current over
    int runsConceded_ = 0;
    int wicketsTaken_ = 0;
    int maidens_ = 0;
    int noBalls_ = 0;
    int wides_ = 0;
    int currentOverRuns_ = 0; // Runs in current over (for maiden tracking)

public:
    BowlingScoreCard() = default;

    // Getters
    int getOversBowled() const { return oversBowled_; }
    int getBallsBowled() const { return ballsBowled_; }
    int getRunsConceded() const { return runsConceded_; }
    int getWicketsTaken() const { return wicketsTaken_; }
    int getMaidens() const { return maidens_; }
    int getNoBalls() const { return noBalls_; }
    int getWides() const { return wides_; }

    // Updaters
    void addRuns(int runs) {
        runsConceded_ += runs;
        currentOverRuns_ += runs;
    }

    void incrementBalls() {
        ballsBowled_++;
        if (ballsBowled_ == 6) {
            oversBowled_++;
            if (currentOverRuns_ == 0) {
                maidens_++;
            }
            ballsBowled_ = 0;
            currentOverRuns_ = 0;
        }
    }

    void addWicket() { wicketsTaken_++; }
    
    void addNoBall() { 
        noBalls_++; 
        runsConceded_++;
    }
    
    void addWide() { 
        wides_++; 
        runsConceded_++;
    }

    /**
     * @brief Get overs in format X.Y
     */
    std::string getOversString() const {
        return std::to_string(oversBowled_) + "." + std::to_string(ballsBowled_);
    }

    /**
     * @brief Calculate economy rate
     */
    double getEconomyRate() const {
        int totalBalls = oversBowled_ * 6 + ballsBowled_;
        if (totalBalls == 0) return 0.0;
        return (static_cast<double>(runsConceded_) / totalBalls) * 6.0;
    }

    /**
     * @brief Get bowling average
     */
    double getBowlingAverage() const {
        if (wicketsTaken_ == 0) return 0.0;
        return static_cast<double>(runsConceded_) / wicketsTaken_;
    }

    std::string toString() const {
        std::string str = getOversString() + " - ";
        str += std::to_string(maidens_) + " - ";
        str += std::to_string(runsConceded_) + " - ";
        str += std::to_string(wicketsTaken_);
        str += " | Econ: " + std::to_string(getEconomyRate()).substr(0, 4);
        return str;
    }
};

} // namespace Cricbuzz
