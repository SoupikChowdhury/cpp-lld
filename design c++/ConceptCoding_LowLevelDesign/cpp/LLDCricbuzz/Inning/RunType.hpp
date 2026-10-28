#pragma once

#include <string>

namespace Cricbuzz {

/**
 * @brief Enum for types of runs
 */
enum class RunType {
    ZERO,           // Dot ball
    ONE,
    TWO,
    THREE,
    FOUR,           // Boundary
    SIX,            // Over the boundary
    BYE,
    LEG_BYE,
    WIDE,
    NO_BALL,
    OVERTHROW
};

/**
 * @brief Utility to convert RunType to string
 */
inline std::string runTypeToString(RunType type) {
    switch (type) {
        case RunType::ZERO: return "Dot";
        case RunType::ONE: return "1 Run";
        case RunType::TWO: return "2 Runs";
        case RunType::THREE: return "3 Runs";
        case RunType::FOUR: return "FOUR!";
        case RunType::SIX: return "SIX!";
        case RunType::BYE: return "Bye";
        case RunType::LEG_BYE: return "Leg Bye";
        case RunType::WIDE: return "Wide";
        case RunType::NO_BALL: return "No Ball";
        case RunType::OVERTHROW: return "Overthrow";
        default: return "Unknown";
    }
}

/**
 * @brief Get run value from RunType
 */
inline int getRunValue(RunType type) {
    switch (type) {
        case RunType::ZERO: return 0;
        case RunType::ONE: return 1;
        case RunType::TWO: return 2;
        case RunType::THREE: return 3;
        case RunType::FOUR: return 4;
        case RunType::SIX: return 6;
        case RunType::BYE: return 1;
        case RunType::LEG_BYE: return 1;
        case RunType::WIDE: return 1;
        case RunType::NO_BALL: return 1;
        case RunType::OVERTHROW: return 1;
        default: return 0;
    }
}

} // namespace Cricbuzz
