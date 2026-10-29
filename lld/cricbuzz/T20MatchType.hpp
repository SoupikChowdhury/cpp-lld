#pragma once

#include "MatchType.hpp"
#include <iostream>

namespace Cricbuzz {

/**
 * @brief Concrete match type for T20 format
 */
class T20MatchType : public MatchType {
private:
    bool hasStrategicTimeout_ = true;
    int strategicTimeoutDuration_ = 150; // 2.5 minutes in seconds

public:
    T20MatchType()
        : MatchType(20, 6, "T20") {}

    void initializeRules() override {
        std::cout << "[T20] Initializing T20 rules:\n";
        std::cout << "  - 20 overs per side\n";
        std::cout << "  - Maximum 4 overs per bowler\n";
        std::cout << "  - Free hit after no-ball\n";
        std::cout << "  - Strategic timeout available\n";
    }

    void setupFieldRestrictions() override {
        std::cout << "[T20] Setting up field restrictions:\n";
        std::cout << "  - Power play (overs 1-6): Max 2 fielders outside circle\n";
        std::cout << "  - Overs 7-20: Max 5 fielders outside circle\n";
    }

    void setupReviewSystem() override {
        std::cout << "[T20] DRS setup:\n";
        std::cout << "  - 2 reviews per team per innings\n";
        std::cout << "  - Review retained if decision overturned\n";
    }

    int getFieldersOutsideCirclePowerPlay() const override {
        return 2;
    }

    int getFieldersOutsideCircleNormal() const override {
        return 5;
    }

    int getDRSReviewsPerInnings() const override {
        return 2;
    }

    /**
     * @brief Get maximum overs a bowler can bowl
     */
    int getMaxOversPerBowler() const {
        return 4;
    }

    std::string getDescription() const override {
        return "Twenty20 (T20) - " + std::to_string(oversPerInning_) + 
               " overs per side, fast-paced, power-play in first 6 overs";
    }
};

} // namespace Cricbuzz
