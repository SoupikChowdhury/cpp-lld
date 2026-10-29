#pragma once

#include <string>

namespace Cricbuzz {

/**
 * @brief Class representing batting statistics for a player in an inning
 */
class BattingScoreCard {
private:
    int runsScored_ = 0;
    int ballsFaced_ = 0;
    int fours_ = 0;
    int sixes_ = 0;
    bool isOut_ = false;
    std::string dismissalType_;
    std::string bowlerName_;  // Bowler who dismissed the batsman

public:
    BattingScoreCard() = default;

    // Getters
    int getRunsScored() const { return runsScored_; }
    int getBallsFaced() const { return ballsFaced_; }
    int getFours() const { return fours_; }
    int getSixes() const { return sixes_; }
    bool isOut() const { return isOut_; }
    std::string getDismissalType() const { return dismissalType_; }

    // Setters/Updaters
    void addRuns(int runs) { 
        runsScored_ += runs;
        if (runs == 4) fours_++;
        else if (runs == 6) sixes_++;
    }

    void incrementBallsFaced() { ballsFaced_++; }

    void setOut(const std::string& dismissalType, const std::string& bowlerName = "") {
        isOut_ = true;
        dismissalType_ = dismissalType;
        bowlerName_ = bowlerName;
    }

    /**
     * @brief Calculate strike rate
     */
    double getStrikeRate() const {
        if (ballsFaced_ == 0) return 0.0;
        return (static_cast<double>(runsScored_) / ballsFaced_) * 100.0;
    }

    std::string toString() const {
        std::string str = std::to_string(runsScored_);
        if (isOut_) {
            str += " (" + std::to_string(ballsFaced_) + "b)";
            str += " - " + dismissalType_;
        } else {
            str += "* (" + std::to_string(ballsFaced_) + "b)";
        }
        str += " | SR: " + std::to_string(static_cast<int>(getStrikeRate()));
        str += " | 4s: " + std::to_string(fours_) + ", 6s: " + std::to_string(sixes_);
        return str;
    }
};

} // namespace Cricbuzz
