#ifndef EARLIEST_SLOT_STRATEGY_HPP
#define EARLIEST_SLOT_STRATEGY_HPP

#include <algorithm>
#include "SlotRankingStrategy.hpp"

class EarliestSlotStrategy : public SlotRankingStrategy {
public:
    std::vector<Appointment*> rankSlots(std::vector<Appointment*>& slots) override {
        std::vector<Appointment*> ranked = slots;
        
        std::sort(ranked.begin(), ranked.end(), 
            [](Appointment* a, Appointment* b) {
                return a->getSlot().getStartTime() < b->getSlot().getStartTime();
            });
        
        return ranked;
    }

    std::string getStrategyName() const override {
        return "EarliestSlotStrategy";
    }
};

#endif // EARLIEST_SLOT_STRATEGY_HPP
