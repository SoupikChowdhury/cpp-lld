#pragma once

#include <memory>
#include "../Inning/BallDetails.hpp"
#include "../Team/PlayerDetails.hpp"

namespace Cricbuzz {

/**
 * @brief Abstract observer interface for score updates
 * Implements Observer Design Pattern
 */
class ScoreUpdaterObserver {
public:
    virtual ~ScoreUpdaterObserver() = default;

    /**
     * @brief Called when a ball is bowled and score needs updating
     */
    virtual void update(std::shared_ptr<BallDetails> ballDetails) = 0;

    /**
     * @brief Get observer name
     */
    virtual std::string getName() const = 0;
};

} // namespace Cricbuzz
