#pragma once

#include <vector>
#include <memory>
#include <string>
#include "PlayerDetails.hpp"

namespace Cricbuzz {

/**
 * @brief Class representing a cricket team
 */
class Team {
private:
    std::string teamId_;
    std::string name_;
    std::string country_;
    std::vector<std::shared_ptr<Player>> players_;
    std::shared_ptr<Player> captain_;
    std::shared_ptr<Player> wicketKeeper_;
    int battingOrder_[11] = {0};

public:
    Team(const std::string& teamId, const std::string& name, const std::string& country)
        : teamId_(teamId), name_(name), country_(country) {}

    // Getters
    std::string getTeamId() const { return teamId_; }
    std::string getName() const { return name_; }
    std::string getCountry() const { return country_; }
    const std::vector<std::shared_ptr<Player>>& getPlayers() const { return players_; }
    std::shared_ptr<Player> getCaptain() const { return captain_; }
    std::shared_ptr<Player> getWicketKeeper() const { return wicketKeeper_; }

    /**
     * @brief Add player to team
     */
    void addPlayer(std::shared_ptr<Player> player) {
        if (players_.size() < 11) {
            players_.push_back(player);
        }
    }

    /**
     * @brief Set team captain
     */
    void setCaptain(std::shared_ptr<Player> captain) {
        captain_ = captain;
    }

    /**
     * @brief Set wicket keeper
     */
    void setWicketKeeper(std::shared_ptr<Player> keeper) {
        wicketKeeper_ = keeper;
    }

    /**
     * @brief Get player by index (batting order)
     */
    std::shared_ptr<Player> getPlayer(int index) const {
        if (index >= 0 && index < static_cast<int>(players_.size())) {
            return players_[index];
        }
        return nullptr;
    }

    /**
     * @brief Get player by ID
     */
    std::shared_ptr<Player> getPlayerById(const std::string& playerId) const {
        for (const auto& player : players_) {
            if (player->getPlayerId() == playerId) {
                return player;
            }
        }
        return nullptr;
    }

    /**
     * @brief Get team size
     */
    size_t getTeamSize() const { return players_.size(); }

    /**
     * @brief Reset all player score cards for new match
     */
    void resetAllScoreCards() {
        for (auto& player : players_) {
            player->resetScoreCards();
        }
    }

    std::string toString() const {
        std::string str = "Team: " + name_ + " (" + country_ + ")\n";
        str += "Captain: " + (captain_ ? captain_->getName() : "N/A") + "\n";
        str += "Players:\n";
        for (size_t i = 0; i < players_.size(); ++i) {
            str += "  " + std::to_string(i + 1) + ". " + players_[i]->toString() + "\n";
        }
        return str;
    }

    std::string getScoreBoard() const {
        std::string str = "=== " + name_ + " Batting ===\n";
        for (const auto& player : players_) {
            auto* batting = player->getBattingScoreCard();
            if (batting->getBallsFaced() > 0 || batting->isOut()) {
                str += player->getName() + ": " + batting->toString() + "\n";
            }
        }
        return str;
    }

    std::string getBowlingFigures() const {
        std::string str = "=== " + name_ + " Bowling ===\n";
        for (const auto& player : players_) {
            auto* bowling = player->getBowlingScoreCard();
            if (bowling->getOversBowled() > 0 || bowling->getBallsBowled() > 0) {
                str += player->getName() + ": " + bowling->toString() + "\n";
            }
        }
        return str;
    }
};

} // namespace Cricbuzz
