#ifndef CHEAPEST_SLOT_STRATEGY_HPP
#define CHEAPEST_SLOT_STRATEGY_HPP

#include <algorithm>
#include "SlotRankingStrategy.hpp"

class CheapestSlotStrategy : public SlotRankingStrategy {
public:
    std::vector<Appointment*> rankSlots(std::vector<Appointment*>& slots) override {
        std::vector<Appointment*> ranked = slots;
        
        std::sort(ranked.begin(), ranked.end(), 
            [](Appointment* a, Appointment* b) {
                // Sort by consultation fee ascending, then by start time
                if (a->getDoctor()->getConsultationFee() != b->getDoctor()->getConsultationFee()) {
                    return a->getDoctor()->getConsultationFee() < b->getDoctor()->getConsultationFee();
                }
                return a->getSlot().getStartTime() < b->getSlot().getStartTime();
            });
        
        return ranked;
    }

    std::string getStrategyName() const override {
        return "CheapestSlotStrategy";
    }
};

#endif // CHEAPEST_SLOT_STRATEGY_HPP
