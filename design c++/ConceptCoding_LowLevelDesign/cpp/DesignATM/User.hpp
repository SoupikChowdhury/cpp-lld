#pragma once
#include "Card.hpp"
#include &lt;memory&gt;

/**
 * ATM User
 */
class User {
public:
    std::shared_ptr&lt;Card&gt; card;
    std::shared_ptr&lt;UserBankAccount&gt; bankAccount;

    std::shared_ptr&lt;Card&gt; getCard() const { return card; }
    void setCard(std::shared_ptr&lt;Card&gt; c) { card = c; }
};
