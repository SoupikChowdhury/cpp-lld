#ifndef SLOT_RANKING_STRATEGY_HPP
#define SLOT_RANKING_STRATEGY_HPP

#include <vector>
#include "../model/Appointment.hpp"

class SlotRankingStrategy {
public:
    virtual ~SlotRankingStrategy() = default;
    virtual std::vector<Appointment*> rankSlots(std::vector<Appointment*>& slots) = 0;
    virtual std::string getStrategyName() const = 0;
};

#endif // SLOT_RANKING_STRATEGY_HPP
