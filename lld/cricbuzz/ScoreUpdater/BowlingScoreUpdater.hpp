#pragma once

#include "ScoreUpdaterObserver.hpp"
#include <iostream>

namespace Cricbuzz {

/**
 * @brief Concrete observer that updates bowling scores
 */
class BowlingScoreUpdater : public ScoreUpdaterObserver {
public:
    void update(std::shared_ptr<BallDetails> ballDetails) override {
        auto bowler = ballDetails->getBowler();
        if (!bowler) return;

        auto* bowlingCard = bowler->getBowlingScoreCard();
        BallType ballType = ballDetails->getBallType();
        int runs = ballDetails->getRunsScored();
        RunType runType = ballDetails->getRunType();
        
        // Handle extras
        if (ballType == BallType::WIDE) {
            bowlingCard->addWide();
            // Additional runs on wide
            if (runs > 1) {
                bowlingCard->addRuns(runs - 1);
            }
        } else if (ballType == BallType::NO_BALL) {
            bowlingCard->addNoBall();
            // Runs scored off no-ball
            if (runs > 1) {
                bowlingCard->addRuns(runs - 1);
            }
        } else {
            // Regular runs conceded (excluding byes and leg byes)
            if (runType != RunType::BYE && runType != RunType::LEG_BYE) {
                bowlingCard->addRuns(runs);
            }
        }
        
        // Increment balls for legal deliveries
        if (ballDetails->isLegalDelivery()) {
            bowlingCard->incrementBalls();
        }
        
        // Handle wicket (excluding run outs which don't credit the bowler)
        if (ballDetails->isWicket()) {
            auto wicket = ballDetails->getWicket();
            if (wicket && wicket->getWicketType() != WicketType::RUN_OUT) {
                bowlingCard->addWicket();
            }
        }
        
        std::cout << "[BowlingUpdater] " << bowler->getName() 
                  << ": " << bowlingCard->getOversString() << " overs, "
                  << bowlingCard->getRunsConceded() << " runs, "
                  << bowlingCard->getWicketsTaken() << " wickets\n";
    }

    std::string getName() const override {
        return "Bowling Score Updater";
    }
};

} // namespace Cricbuzz
