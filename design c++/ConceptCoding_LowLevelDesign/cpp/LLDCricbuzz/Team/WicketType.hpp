#pragma once

#include <string>

namespace Cricbuzz {

/**
 * @brief Enum for types of wickets
 */
enum class WicketType {
    BOWLED,
    CAUGHT,
    CAUGHT_BEHIND,
    LBW,
    RUN_OUT,
    STUMPED,
    HIT_WICKET,
    CAUGHT_AND_BOWLED,
    RETIRED_HURT,
    TIMED_OUT,
    HANDLED_BALL,
    OBSTRUCTING_FIELD
};

/**
 * @brief Utility to convert WicketType to string
 */
inline std::string wicketTypeToString(WicketType type) {
    switch (type) {
        case WicketType::BOWLED: return "Bowled";
        case WicketType::CAUGHT: return "Caught";
        case WicketType::CAUGHT_BEHIND: return "Caught Behind";
        case WicketType::LBW: return "LBW";
        case WicketType::RUN_OUT: return "Run Out";
        case WicketType::STUMPED: return "Stumped";
        case WicketType::HIT_WICKET: return "Hit Wicket";
        case WicketType::CAUGHT_AND_BOWLED: return "Caught & Bowled";
        case WicketType::RETIRED_HURT: return "Retired Hurt";
        case WicketType::TIMED_OUT: return "Timed Out";
        case WicketType::HANDLED_BALL: return "Handled Ball";
        case WicketType::OBSTRUCTING_FIELD: return "Obstructing Field";
        default: return "Unknown";
    }
}

} // namespace Cricbuzz
