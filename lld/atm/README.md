# ATM Machine - Low Level Design (C++)

## Design Patterns Used
- **State Pattern**: ATM transitions between different states (Idle, CardInserted, Authenticated, DispenseCash)
- **Chain of Responsibility**: Cash dispensing through denomination-specific dispensers
- **Factory Pattern**: ATMStateFactory creates appropriate state objects
- **Repository Pattern**: ATMRepository provides data access abstraction

## Class Diagram

```mermaid
classDiagram
    class ATMState {
        <<interface>>
        +insertCard(Card* card)*
        +enterPin(string pin)*
        +selectOption(string option)*
        +dispenseCash(int amount)*
        +ejectCard()*
        +getStatus()* ATMStatus
    }
    
    class IdleState {
        -ATMMachine* atmMachine
        +insertCard(Card* card)
        +enterPin(string pin)
        +selectOption(string option)
        +dispenseCash(int amount)
        +ejectCard()
        +getStatus() ATMStatus
    }
    
    class CardInsertedState {
        -ATMMachine* atmMachine
        +insertCard(Card* card)
        +enterPin(string pin)
        +selectOption(string option)
        +dispenseCash(int amount)
        +ejectCard()
        +getStatus() ATMStatus
    }
    
    class AuthenticatedState {
        -ATMMachine* atmMachine
        +insertCard(Card* card)
        +enterPin(string pin)
        +selectOption(string option)
        +dispenseCash(int amount)
        +ejectCard()
        +getStatus() ATMStatus
    }
    
    class DispenseCashState {
        -ATMMachine* atmMachine
        -CashDispenser* chain
        +insertCard(Card* card)
        +enterPin(string pin)
        +selectOption(string option)
        +dispenseCash(int amount)
        +ejectCard()
        +getStatus() ATMStatus
    }
    
    class CashDispenser {
        <<interface>>
        +setNext(CashDispenser* next)*
        +canDispense(ATM* atm, int amount)* bool
        +dispense(ATM* atm, int amount)*
    }
    
    class TwoThousandDispenser {
        -CashDispenser* nextDispenser
        +setNext(CashDispenser* next)
        +canDispense(ATM* atm, int amount) bool
        +dispense(ATM* atm, int amount)
    }
    
    class FiveHundredDispenser {
        -CashDispenser* nextDispenser
        +setNext(CashDispenser* next)
        +canDispense(ATM* atm, int amount) bool
        +dispense(ATM* atm, int amount)
    }
    
    class OneHundredDispenser {
        -CashDispenser* nextDispenser
        +setNext(CashDispenser* next)
        +canDispense(ATM* atm, int amount) bool
        +dispense(ATM* atm, int amount)
    }
    
    class ATM {
        -string id
        -int twoThousandNotes
        -int fiveHundredNotes
        -int oneHundredNotes
        -ATMStatus status
        +getId() string
        +getCashAvailable() double
        +setCashAvailable(double amount)
    }
    
    class Account {
        -string id
        -double balance
        +getId() string
        +getBalance() double
        +setBalance(double amount)
    }
    
    class Card {
        -string cardNumber
        -string pin
        -Account* account
        +getCardNumber() string
        +getPin() string
        +getAccount() Account*
    }
    
    class ATMMachine {
        -ATM* atm
        -ATMState* state
        -ATMRepository* atmRepository
        -Card* currentCard
        +insertCard(Card* card)
        +enterPin(string pin)
        +selectOption(string option)
        +dispenseCash(int amount)
        +ejectCard()
        +setState(ATMState* state)
    }
    
    class ATMRepository {
        -map~string, ATM*~ atms
        +save(ATM* atm)
        +getById(string id) ATM*
        +updateATMStatusById(string id, ATMStatus status)
    }
    
    class ATMStateFactory {
        +getState(ATMStatus status, ATMMachine* machine)$ ATMState*
    }

    ATMState <|.. IdleState
    ATMState <|.. CardInsertedState
    ATMState <|.. AuthenticatedState
    ATMState <|.. DispenseCashState
    
    CashDispenser <|.. TwoThousandDispenser
    CashDispenser <|.. FiveHundredDispenser
    CashDispenser <|.. OneHundredDispenser
    
    TwoThousandDispenser --> FiveHundredDispenser : next
    FiveHundredDispenser --> OneHundredDispenser : next
    
    ATMMachine --> ATM
    ATMMachine --> ATMState
    ATMMachine --> ATMRepository
    ATMMachine --> Card
    
    Card --> Account
    
    DispenseCashState --> CashDispenser
```

## State Diagram

```mermaid
stateDiagram-v2
    [*] --> Idle
    Idle --> CardInserted : insertCard()
    CardInserted --> Authenticated : enterPin(valid)
    CardInserted --> CardInserted : enterPin(invalid)
    CardInserted --> Idle : ejectCard()
    Authenticated --> DispenseCash : selectOption(withdraw)
    Authenticated --> Idle : ejectCard()
    DispenseCash --> Idle : dispenseCash()/ejectCard()
```

## Sequence Diagram

```mermaid
sequenceDiagram
    participant User
    participant ATMMachine
    participant State
    participant CashDispenser
    participant ATM
    
    User->>ATMMachine: insertCard(card)
    ATMMachine->>State: insertCard(card)
    State->>ATMMachine: setState(CardInsertedState)
    
    User->>ATMMachine: enterPin("1234")
    ATMMachine->>State: enterPin("1234")
    State->>State: validate PIN
    State->>ATMMachine: setState(AuthenticatedState)
    
    User->>ATMMachine: selectOption("WITHDRAW")
    ATMMachine->>State: selectOption("WITHDRAW")
    State->>ATMMachine: setState(DispenseCashState)
    
    User->>ATMMachine: dispenseCash(1500)
    ATMMachine->>State: dispenseCash(1500)
    State->>CashDispenser: canDispense(atm, 1500)
    CashDispenser-->>State: true
    State->>CashDispenser: dispense(atm, 1500)
    CashDispenser->>ATM: dispense notes
    State->>ATMMachine: setState(IdleState)
```

## Files Structure
```
atm/
├── enums/
│   └── ATMStatus.hpp
├── model/
│   ├── Account.hpp
│   ├── ATM.hpp
│   └── Card.hpp
├── cor/
│   ├── CashDispenser.hpp
│   ├── TwoThousandDispenser.hpp
│   ├── FiveHundredDispenser.hpp
│   ├── OneHundredDispenser.hpp
│   └── CashDispenserChainBuilder.hpp
├── state/
│   ├── ATMState.hpp
│   ├── IdleState.hpp
│   ├── CardInsertedState.hpp
│   ├── AuthenticatedState.hpp
│   └── DispenseCashState.hpp
├── factory/
│   └── ATMStateFactory.hpp
├── repository/
│   └── ATMRepository.hpp
├── service/
│   └── ATMMachine.hpp
└── main.cpp
```
