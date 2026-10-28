#ifndef CASH_DISPENSER_CHAIN_BUILDER_HPP
#define CASH_DISPENSER_CHAIN_BUILDER_HPP

#include "CashDispenser.hpp"
#include "TwoThousandDispenser.hpp"
#include "FiveHundredDispenser.hpp"
#include "OneHundredDispenser.hpp"

class CashDispenserChainBuilder {
public:
    static CashDispenser* buildChain() {
        CashDispenser* twoThousand = new TwoThousandDispenser();
        CashDispenser* fiveHundred = new FiveHundredDispenser();
        CashDispenser* oneHundred = new OneHundredDispenser();

        twoThousand->setNext(fiveHundred);
        fiveHundred->setNext(oneHundred);

        return twoThousand;
    }
};

#endif // CASH_DISPENSER_CHAIN_BUILDER_HPP
