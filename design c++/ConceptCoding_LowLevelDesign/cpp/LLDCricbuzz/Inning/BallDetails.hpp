#pragma once

#include <memory>
#include <string>
#include "BallType.hpp"
#include "RunType.hpp"
#include "../Team/PlayerDetails.hpp"
#include "../Team/Wicket.hpp"

namespace Cricbuzz {

/**
 * @brief Class representing details of a single ball delivery
 */
class BallDetails {
private:
    int ballNumber_;       // Ball number in the over (1-6)
    BallType ballType_;
    int runsScored_;
    RunType runType_;
    std::shared_ptr<Player> bowler_;
    std::shared_ptr<Player> striker_;
    std::shared_ptr<Player> nonStriker_;
    std::shared_ptr<Wicket> wicket_;  // Null if no wicket
    bool isLegalDelivery_;
    std::string commentary_;

public:
    BallDetails(int ballNumber, BallType ballType, int runs, RunType runType,
                std::shared_ptr<Player> bowler, std::shared_ptr<Player> striker,
                std::shared_ptr<Player> nonStriker)
        : ballNumber_(ballNumber)
        , ballType_(ballType)
        , runsScored_(runs)
        , runType_(runType)
        , bowler_(bowler)
        , striker_(striker)
        , nonStriker_(nonStriker)
        , wicket_(nullptr)
        , isLegalDelivery_(ballType == BallType::NORMAL || ballType == BallType::FREE_HIT) {}

    // Getters
    int getBallNumber() const { return ballNumber_; }
    BallType getBallType() const { return ballType_; }
    int getRunsScored() const { return runsScored_; }
    RunType getRunType() const { return runType_; }
    std::shared_ptr<Player> getBowler() const { return bowler_; }
    std::shared_ptr<Player> getStriker() const { return striker_; }
    std::shared_ptr<Player> getNonStriker() const { return nonStriker_; }
    std::shared_ptr<Wicket> getWicket() const { return wicket_; }
    bool isLegalDelivery() const { return isLegalDelivery_; }
    std::string getCommentary() const { return commentary_; }

    // Setters
    void setWicket(std::shared_ptr<Wicket> wicket) { wicket_ = wicket; }
    void setCommentary(const std::string& commentary) { commentary_ = commentary; }

    /**
     * @brief Check if wicket fell on this ball
     */
    bool isWicket() const { return wicket_ != nullptr; }

    std::string toString() const {
        std::string str = std::to_string(ballNumber_) + ". ";
        
        if (ballType_ != BallType::NORMAL) {
            str += ballTypeToString(ballType_) + " - ";
        }
        
        if (wicket_) {
            str += "WICKET! ";
        }
        
        str += runTypeToString(runType_);
        
        if (!commentary_.empty()) {
            str += " | " + commentary_;
        }
        
        return str;
    }
};

} // namespace Cricbuzz
