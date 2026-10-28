#pragma once

#include <string>
#include <memory>
#include "Person.hpp"
#include "PlayerType.hpp"
#include "Score/BattingScoreCard.hpp"
#include "Score/BowlingScoreCard.hpp"

namespace Cricbuzz {

/**
 * @brief Class containing additional player details
 */
class PlayerDetails {
private:
    std::string jerseyNumber_;
    std::string battingStyle_;  // Right-handed, Left-handed
    std::string bowlingStyle_;  // Fast, Medium, Off-spin, Leg-spin
    int totalMatches_ = 0;
    int totalRuns_ = 0;
    int totalWickets_ = 0;

public:
    PlayerDetails(const std::string& jerseyNumber = "",
                  const std::string& battingStyle = "Right-handed",
                  const std::string& bowlingStyle = "")
        : jerseyNumber_(jerseyNumber)
        , battingStyle_(battingStyle)
        , bowlingStyle_(bowlingStyle) {}

    // Getters
    std::string getJerseyNumber() const { return jerseyNumber_; }
    std::string getBattingStyle() const { return battingStyle_; }
    std::string getBowlingStyle() const { return bowlingStyle_; }
    int getTotalMatches() const { return totalMatches_; }
    int getTotalRuns() const { return totalRuns_; }
    int getTotalWickets() const { return totalWickets_; }

    // Updaters
    void addMatch() { totalMatches_++; }
    void addRuns(int runs) { totalRuns_ += runs; }
    void addWickets(int wickets) { totalWickets_ += wickets; }
};

/**
 * @brief Class representing a cricket player
 */
class Player : public Person {
private:
    std::string playerId_;
    PlayerType playerType_;
    PlayerDetails details_;
    
    // Current match score cards
    std::unique_ptr<BattingScoreCard> battingScoreCard_;
    std::unique_ptr<BowlingScoreCard> bowlingScoreCard_;

public:
    Player(const std::string& playerId, const std::string& name, int age,
           const std::string& country, PlayerType playerType,
           const PlayerDetails& details = PlayerDetails())
        : Person(name, age, country)
        , playerId_(playerId)
        , playerType_(playerType)
        , details_(details)
        , battingScoreCard_(std::make_unique<BattingScoreCard>())
        , bowlingScoreCard_(std::make_unique<BowlingScoreCard>()) {}

    // Getters
    std::string getPlayerId() const { return playerId_; }
    PlayerType getPlayerType() const { return playerType_; }
    PlayerDetails& getDetails() { return details_; }
    const PlayerDetails& getDetails() const { return details_; }
    
    BattingScoreCard* getBattingScoreCard() { return battingScoreCard_.get(); }
    const BattingScoreCard* getBattingScoreCard() const { return battingScoreCard_.get(); }
    
    BowlingScoreCard* getBowlingScoreCard() { return bowlingScoreCard_.get(); }
    const BowlingScoreCard* getBowlingScoreCard() const { return bowlingScoreCard_.get(); }

    /**
     * @brief Reset score cards for new innings
     */
    void resetScoreCards() {
        battingScoreCard_ = std::make_unique<BattingScoreCard>();
        bowlingScoreCard_ = std::make_unique<BowlingScoreCard>();
    }

    std::string toString() const override {
        std::string str = Person::toString();
        str += " | " + playerTypeToString(playerType_);
        str += " | Jersey: " + details_.getJerseyNumber();
        return str;
    }

    std::string getScoreSummary() const {
        std::string str = name_ + ":\n";
        str += "  Batting: " + battingScoreCard_->toString() + "\n";
        str += "  Bowling: " + bowlingScoreCard_->toString();
        return str;
    }
};

} // namespace Cricbuzz
