#pragma once

#include <string>

namespace Cricbuzz {

/**
 * @brief Enum for types of balls
 */
enum class BallType {
    NORMAL,
    WIDE,
    NO_BALL,
    DEAD_BALL,
    FREE_HIT
};

/**
 * @brief Utility to convert BallType to string
 */
inline std::string ballTypeToString(BallType type) {
    switch (type) {
        case BallType::NORMAL: return "Normal";
        case BallType::WIDE: return "Wide";
        case BallType::NO_BALL: return "No Ball";
        case BallType::DEAD_BALL: return "Dead Ball";
        case BallType::FREE_HIT: return "Free Hit";
        default: return "Unknown";
    }
}

} // namespace Cricbuzz
