# ATM System LLD - UML Class Diagram

```
┌─────────────────────────────────────────────────────────────────────────────┐
│                            ATM SYSTEM                                        │
│             Patterns: State + Singleton + Chain of Responsibility            │
└─────────────────────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────────────────────┐
│                              ENTITIES                                        │
└─────────────────────────────────────────────────────────────────────────────┘

┌────────────────┐      ┌────────────────┐      ┌────────────────────┐
│ UserBankAccount│◄─────│      Card      │◄─────│       User         │
├────────────────┤ has  ├────────────────┤ has  ├────────────────────┤
│ - balance: int │      │ - cardNumber   │      │ - card: Card*      │
├────────────────┤      │ - cvv          │      │ - bankAccount      │
│ + withdraw()   │      │ - PIN_NUMBER   │      ├────────────────────┤
└────────────────┘      │ - bankAccount* │      │ + getCard()        │
                        ├────────────────┤      │ + setCard()        │
                        │ + isCorrectPIN()│     └────────────────────┘
                        │ + getBankBalance│
                        │ + deductBalance│
                        └────────────────┘

┌─────────────────────────────────────────────────────────────────────────────┐
│                          SINGLETON + STATE PATTERN                           │
└─────────────────────────────────────────────────────────────────────────────┘

                    ┌─────────────────────────────────┐
                    │        ATM (Singleton)          │
                    ├─────────────────────────────────┤
                    │ - instance: ATM* (static)       │
                    │ - currentState: ATMState*       │
                    │ - atmBalance: int               │
                    │ - noOfTwoThousandNotes: int     │
                    │ - noOfFiveHundredNotes: int     │
                    │ - noOfOneHundredNotes: int      │
                    ├─────────────────────────────────┤
                    │ + getATMObject(): ATM* (static) │
                    │ + getCurrentATMState()          │
                    │ + setCurrentATMState()          │
                    │ + deductATMBalance()            │
                    │ + deductXXXNotes()              │
                    │ + printCurrentATMStatus()       │
                    └─────────────────────────────────┘
                                    │
                                    │ has
                                    ▼
                    ┌─────────────────────────────────┐
                    │      ATMState (Abstract)        │
                    ├─────────────────────────────────┤
                    │ + insertCard()                  │
                    │ + authenticatePin()             │
                    │ + selectOperation()             │
                    │ + cashWithdrawal()              │
                    │ + displayBalance()              │
                    │ + returnCard()                  │
                    │ + exit()                        │
                    └─────────────────────────────────┘
                                    ▲
                                    │ extends
        ┌───────────────┬───────────┼───────────┬─────────────────┐
        │               │           │           │                 │
┌───────┴─────┐ ┌───────┴─────┐ ┌───┴───────┐ ┌─┴─────────────┐ ┌─┴───────────┐
│ IdleState   │ │HasCardState │ │SelectOp   │ │CashWithdrawal │ │CheckBalance │
├─────────────┤ ├─────────────┤ │   State   │ │    State      │ │   State     │
│+ insertCard │ │+ authPin    │ ├───────────┤ ├───────────────┤ ├─────────────┤
│  →HasCard   │ │  →SelectOp  │ │+ selectOp │ │+ cashWithdraw │ │+ displayBal │
└─────────────┘ │+ exit→Idle  │ │  →Cash/Bal│ │+ exit→Idle    │ │+ exit→Idle  │
                └─────────────┘ │+ exit→Idle│ └───────────────┘ └─────────────┘
                                └───────────┘

┌─────────────────────────────────────────────────────────────────────────────┐
│                      CHAIN OF RESPONSIBILITY                                 │
└─────────────────────────────────────────────────────────────────────────────┘

              ┌────────────────────────────────┐
              │   CashWithdrawProcessor        │◄──────────────┐
              │      (Abstract Handler)        │               │
              ├────────────────────────────────┤               │ next
              │ - nextProcessor: Processor*    │───────────────┘
              ├────────────────────────────────┤
              │ + withdraw(atm, amount)        │
              └────────────────────────────────┘
                              ▲
                              │ extends
          ┌───────────────────┼───────────────────┐
          │                   │                   │
┌─────────┴───────┐  ┌────────┴────────┐  ┌───────┴───────┐
│TwoThousandProc  │  │FiveHundredProc  │  │OneHundredProc │
├─────────────────┤  ├─────────────────┤  ├───────────────┤
│ + withdraw()    │  │ + withdraw()    │  │ + withdraw()  │
│ // handle 2000s │  │ // handle 500s  │  │ // handle 100s│
│ // pass rest    │  │ // pass rest    │  │               │
└─────────────────┘  └─────────────────┘  └───────────────┘


┌─────────────────────────────────────────────────────────────────────────────┐
│                         STATE TRANSITION DIAGRAM                             │
└─────────────────────────────────────────────────────────────────────────────┘

                               insertCard
    ┌─────────┐  ────────────────────────────►  ┌────────────┐
    │  IDLE   │                                  │  HAS_CARD  │
    └─────────┘  ◄────────────────────────────  └────────────┘
         ▲              exit/wrong PIN                 │
         │                                             │ authenticatePin
         │                                             ▼
         │                                      ┌────────────────┐
         │ exit                                 │ SELECT_OPERATION│
         │◄─────────────────────────────────────└────────────────┘
         │                                             │
         │                        ┌────────────────────┴────────────────────┐
         │                        │                                         │
         │              selectOperation                          selectOperation
         │              (CASH_WITHDRAWAL)                        (BALANCE_CHECK)
         │                        │                                         │
         │                        ▼                                         ▼
         │               ┌─────────────────┐                      ┌─────────────────┐
         │               │ CASH_WITHDRAWAL │                      │  CHECK_BALANCE  │
         │               └─────────────────┘                      └─────────────────┘
         │                        │                                         │
         └────────────────────────┴─────────────────────────────────────────┘
                                    after operation
```

## Design Patterns Summary

| Pattern | Component | Purpose |
|---------|-----------|---------|
| **Singleton** | ATM | Single ATM instance |
| **State** | ATMState hierarchy | State-based behavior |
| **Chain of Responsibility** | CashWithdrawProcessor | Denomination selection |
