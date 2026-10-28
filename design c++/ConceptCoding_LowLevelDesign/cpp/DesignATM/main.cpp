#include "ATM.hpp"
#include "ATM.cpp"
#include "User.hpp"
#include &lt;iostream&gt;

int main() {
    std::cout &lt;&lt; "=== ATM System Demo ===" &lt;&lt; std::endl;

    // Initialize ATM
    ATM* atm = ATM::getATMObject();
    atm-&gt;setAtmBalance(3500, 1, 2, 5);  // 1x2000 + 2x500 + 5x100 = 3500

    // Create user with bank account and card
    auto bankAccount = std::make_shared&lt;UserBankAccount&gt;();
    bankAccount-&gt;balance = 5000;

    auto card = std::make_shared&lt;Card&gt;();
    card-&gt;setBankAccount(bankAccount);

    User user;
    user.setCard(card);

    std::cout &lt;&lt; "\n--- Initial ATM Status ---" &lt;&lt; std::endl;
    atm-&gt;printCurrentATMStatus();

    std::cout &lt;&lt; "\n--- User inserts card ---" &lt;&lt; std::endl;
    atm-&gt;getCurrentATMState()-&gt;insertCard(atm, user.getCard());

    std::cout &lt;&lt; "\n--- User enters PIN ---" &lt;&lt; std::endl;
    atm-&gt;getCurrentATMState()-&gt;authenticatePin(atm, user.getCard(), 112211);

    std::cout &lt;&lt; "\n--- User selects Cash Withdrawal ---" &lt;&lt; std::endl;
    atm-&gt;getCurrentATMState()-&gt;selectOperation(atm, user.getCard(), TransactionType::CASH_WITHDRAWAL);

    std::cout &lt;&lt; "\n--- User withdraws 2700 ---" &lt;&lt; std::endl;
    atm-&gt;getCurrentATMState()-&gt;cashWithdrawal(atm, user.getCard(), 2700);

    std::cout &lt;&lt; "\n--- Final ATM Status ---" &lt;&lt; std::endl;
    atm-&gt;printCurrentATMStatus();

    std::cout &lt;&lt; "\n--- User Bank Balance: " &lt;&lt; bankAccount-&gt;balance &lt;&lt; " ---" &lt;&lt; std::endl;

    return 0;
}
