#pragma once

#include <memory>
#include <string>
#include "WicketType.hpp"

namespace Cricbuzz {

// Forward declaration
class Player;

/**
 * @brief Class representing a wicket in cricket
 */
class Wicket {
private:
    WicketType wicketType_;
    std::shared_ptr<Player> batsmanOut_;
    std::shared_ptr<Player> bowler_;
    std::shared_ptr<Player> fielder_;  // For caught/run out
    int overNumber_;
    int ballNumber_;
    int teamScore_;  // Score when wicket fell

public:
    Wicket(WicketType type, std::shared_ptr<Player> batsman,
           std::shared_ptr<Player> bowler, int overNumber, int ballNumber, int teamScore,
           std::shared_ptr<Player> fielder = nullptr)
        : wicketType_(type)
        , batsmanOut_(batsman)
        , bowler_(bowler)
        , fielder_(fielder)
        , overNumber_(overNumber)
        , ballNumber_(ballNumber)
        , teamScore_(teamScore) {}

    // Getters
    WicketType getWicketType() const { return wicketType_; }
    std::shared_ptr<Player> getBatsmanOut() const { return batsmanOut_; }
    std::shared_ptr<Player> getBowler() const { return bowler_; }
    std::shared_ptr<Player> getFielder() const { return fielder_; }
    int getOverNumber() const { return overNumber_; }
    int getBallNumber() const { return ballNumber_; }
    int getTeamScore() const { return teamScore_; }

    std::string getOverString() const {
        return std::to_string(overNumber_) + "." + std::to_string(ballNumber_);
    }

    std::string toString() const;  // Defined after Player is complete
};

} // namespace Cricbuzz
