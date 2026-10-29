#pragma once

#include "MatchType.hpp"
#include <iostream>

namespace Cricbuzz {

/**
 * @brief Concrete match type for One Day International (ODI) format
 */
class OneDayMatchType : public MatchType {
private:
    int drinkBreakInterval_ = 15; // Every 15 overs
    bool hasBattingPowerPlay_ = true;

public:
    OneDayMatchType()
        : MatchType(50, 10, "ODI") {}

    void initializeRules() override {
        std::cout << "[ODI] Initializing ODI rules:\n";
        std::cout << "  - 50 overs per side\n";
        std::cout << "  - Maximum 10 overs per bowler\n";
        std::cout << "  - Free hit after no-ball\n";
        std::cout << "  - Drinks break every " << drinkBreakInterval_ << " overs\n";
    }

    void setupFieldRestrictions() override {
        std::cout << "[ODI] Setting up field restrictions:\n";
        std::cout << "  - Power play 1 (overs 1-10): Mandatory, max 2 fielders outside circle\n";
        std::cout << "  - Power play 2 (overs 11-40): Max 4 fielders outside circle\n";
        std::cout << "  - Power play 3 (overs 41-50): Max 5 fielders outside circle\n";
    }

    void setupReviewSystem() override {
        std::cout << "[ODI] DRS setup:\n";
        std::cout << "  - 2 reviews per team per innings\n";
        std::cout << "  - Failed review is lost\n";
        std::cout << "  - Successful review retained\n";
    }

    int getFieldersOutsideCirclePowerPlay() const override {
        return 2;
    }

    int getFieldersOutsideCircleNormal() const override {
        return 4;  // During middle overs
    }

    int getDRSReviewsPerInnings() const override {
        return 2;
    }

    /**
     * @brief Get maximum overs a bowler can bowl
     */
    int getMaxOversPerBowler() const {
        return 10;
    }

    /**
     * @brief Check if in death overs (last 10)
     */
    bool isDeathOvers(int currentOver) const {
        return currentOver >= 40;
    }

    /**
     * @brief Get fielders allowed outside circle for given over
     */
    int getFieldersOutsideCircle(int overNumber) const {
        if (overNumber < 10) return 2;       // Power play 1
        else if (overNumber < 40) return 4;   // Middle overs
        else return 5;                        // Death overs
    }

    std::string getDescription() const override {
        return "One Day International (ODI) - " + std::to_string(oversPerInning_) + 
               " overs per side, balanced format with three power plays";
    }
};

} // namespace Cricbuzz
