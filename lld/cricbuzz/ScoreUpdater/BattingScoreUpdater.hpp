#pragma once

#include "ScoreUpdaterObserver.hpp"
#include <iostream>

namespace Cricbuzz {

/**
 * @brief Concrete observer that updates batting scores
 */
class BattingScoreUpdater : public ScoreUpdaterObserver {
public:
    void update(std::shared_ptr<BallDetails> ballDetails) override {
        auto striker = ballDetails->getStriker();
        if (!striker) return;

        auto* battingCard = striker->getBattingScoreCard();
        
        // Update runs scored
        int runs = ballDetails->getRunsScored();
        RunType runType = ballDetails->getRunType();
        
        // Only add runs if they are off the bat (not extras like byes, leg byes)
        if (runType != RunType::BYE && runType != RunType::LEG_BYE &&
            runType != RunType::WIDE) {
            
            // For no-ball, runs off the bat still count
            if (runType == RunType::NO_BALL) {
                battingCard->addRuns(runs > 1 ? runs - 1 : 0);  // Subtract 1 for no-ball extra
            } else {
                battingCard->addRuns(runs);
            }
        }
        
        // Increment balls faced for legal deliveries (not wides)
        if (ballDetails->isLegalDelivery() || 
            ballDetails->getBallType() == BallType::NO_BALL) {
            // Note: Wide balls don't count as balls faced
            if (ballDetails->getBallType() != BallType::WIDE) {
                battingCard->incrementBallsFaced();
            }
        }
        
        // Handle wicket
        if (ballDetails->isWicket()) {
            auto wicket = ballDetails->getWicket();
            if (wicket && wicket->getBatsmanOut() == striker) {
                battingCard->setOut(
                    wicketTypeToString(wicket->getWicketType()),
                    wicket->getBowler() ? wicket->getBowler()->getName() : ""
                );
            }
        }
        
        std::cout << "[BattingUpdater] " << striker->getName() 
                  << ": " << battingCard->getRunsScored() << " runs off "
                  << battingCard->getBallsFaced() << " balls\n";
    }

    std::string getName() const override {
        return "Batting Score Updater";
    }
};

} // namespace Cricbuzz
