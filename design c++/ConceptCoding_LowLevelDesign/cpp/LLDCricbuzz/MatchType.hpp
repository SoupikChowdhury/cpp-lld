#pragma once

#include <string>
#include <memory>
#include "Team/Team.hpp"

namespace Cricbuzz {

/**
 * @brief Abstract base class for match types
 * Implements Template Method Design Pattern
 */
class MatchType {
protected:
    int oversPerInning_;
    int powerPlayOvers_;
    std::string matchTypeName_;

public:
    MatchType(int overs, int powerPlay, const std::string& name)
        : oversPerInning_(overs)
        , powerPlayOvers_(powerPlay)
        , matchTypeName_(name) {}

    virtual ~MatchType() = default;

    // Getters
    int getOversPerInning() const { return oversPerInning_; }
    int getPowerPlayOvers() const { return powerPlayOvers_; }
    std::string getMatchTypeName() const { return matchTypeName_; }

    /**
     * @brief Template method for match setup
     */
    void setupMatch() {
        initializeRules();
        setupFieldRestrictions();
        setupReviewSystem();
    }

    // Abstract methods to be implemented by concrete types
    virtual void initializeRules() = 0;
    virtual void setupFieldRestrictions() = 0;
    virtual void setupReviewSystem() = 0;

    /**
     * @brief Get maximum fielders outside circle during power play
     */
    virtual int getFieldersOutsideCirclePowerPlay() const = 0;

    /**
     * @brief Get maximum fielders outside circle during non-power play
     */
    virtual int getFieldersOutsideCircleNormal() const = 0;

    /**
     * @brief Get number of DRS reviews per team per innings
     */
    virtual int getDRSReviewsPerInnings() const = 0;

    /**
     * @brief Check if follow-on is applicable
     */
    virtual bool isFollowOnApplicable() const { return false; }

    /**
     * @brief Get match description
     */
    virtual std::string getDescription() const = 0;
};

} // namespace Cricbuzz
