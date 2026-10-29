#pragma once

#include <vector>
#include <memory>
#include <string>
#include "BallDetails.hpp"

namespace Cricbuzz {

/**
 * @brief Class representing details of a single over
 */
class OverDetails {
private:
    int overNumber_;
    std::shared_ptr<Player> bowler_;
    std::vector<std::shared_ptr<BallDetails>> balls_;
    int runsInOver_ = 0;
    int wicketsInOver_ = 0;
    bool isMaiden_ = false;
    bool isComplete_ = false;

public:
    OverDetails(int overNumber, std::shared_ptr<Player> bowler)
        : overNumber_(overNumber), bowler_(bowler) {}

    // Getters
    int getOverNumber() const { return overNumber_; }
    std::shared_ptr<Player> getBowler() const { return bowler_; }
    const std::vector<std::shared_ptr<BallDetails>>& getBalls() const { return balls_; }
    int getRunsInOver() const { return runsInOver_; }
    int getWicketsInOver() const { return wicketsInOver_; }
    bool isMaiden() const { return isMaiden_; }
    bool isComplete() const { return isComplete_; }

    /**
     * @brief Get legal ball count
     */
    int getLegalBallCount() const {
        int count = 0;
        for (const auto& ball : balls_) {
            if (ball->isLegalDelivery()) {
                count++;
            }
        }
        return count;
    }

    /**
     * @brief Add a ball to this over
     */
    void addBall(std::shared_ptr<BallDetails> ball) {
        balls_.push_back(ball);
        runsInOver_ += ball->getRunsScored();
        
        if (ball->isWicket()) {
            wicketsInOver_++;
        }
        
        if (getLegalBallCount() >= 6) {
            isComplete_ = true;
            isMaiden_ = (runsInOver_ == 0 && wicketsInOver_ == 0);
        }
    }

    /**
     * @brief Get over summary (e.g., "W 1 . 4 2 .")
     */
    std::string getOverSummary() const {
        std::string summary = "";
        for (const auto& ball : balls_) {
            if (ball->isWicket()) {
                summary += "W ";
            } else if (ball->getRunsScored() == 0 && ball->isLegalDelivery()) {
                summary += ". ";
            } else if (ball->getBallType() == BallType::WIDE) {
                summary += "Wd ";
            } else if (ball->getBallType() == BallType::NO_BALL) {
                summary += "Nb ";
            } else {
                summary += std::to_string(ball->getRunsScored()) + " ";
            }
        }
        return summary;
    }

    std::string toString() const {
        std::string str = "Over " + std::to_string(overNumber_ + 1);
        str += " (" + bowler_->getName() + "): ";
        str += getOverSummary();
        str += "| " + std::to_string(runsInOver_) + " runs";
        if (wicketsInOver_ > 0) {
            str += ", " + std::to_string(wicketsInOver_) + " wicket(s)";
        }
        if (isMaiden_) {
            str += " [MAIDEN]";
        }
        return str;
    }
};

} // namespace Cricbuzz
