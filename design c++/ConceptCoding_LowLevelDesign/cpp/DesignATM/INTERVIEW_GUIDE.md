# ATM System - LLD Interview Guide

A step-by-step framework for delivering the ATM system low-level design interview.

---

## 📁 Code Reference

| File | Purpose | Key Functions |
|------|---------|---------------|
| [ATM.hpp](ATM.hpp) | Singleton ATM context, manages state and notes | `getATMObject()`, `setCurrentState()`, `getCurrentState()` |
| [ATM.cpp](ATM.cpp) | All implementations (~170 lines) | State transitions, withdrawal processing |
| [States/ATMState.hpp](States/ATMState.hpp) | Abstract state interface | `insertCard()`, `authenticatePin()`, `selectOperation()`, `cashWithdrawal()`, `displayBalance()` |
| [States/IdleState.hpp](States/IdleState.hpp) | Initial state - waiting for card | `insertCard()` → transitions to HasCardState |
| [States/HasCardState.hpp](States/HasCardState.hpp) | Card inserted, awaiting PIN | `authenticatePin()` → transitions to SelectOperationState |
| [States/SelectOperationState.hpp](States/SelectOperationState.hpp) | Choose transaction type | `selectOperation()` → routes to CashWithdrawal/CheckBalance |
| [States/CashWithdrawalState.hpp](States/CashWithdrawalState.hpp) | Handle withdrawal | `cashWithdrawal()` → uses Chain of Responsibility |
| [States/CheckBalanceState.hpp](States/CheckBalanceState.hpp) | Display balance | `displayBalance()` |
| [Withdrawal/CashWithdrawProcessor.hpp](Withdrawal/CashWithdrawProcessor.hpp) | Chain of Responsibility base | `setNextProcessor()`, `withdraw()` |
| [Withdrawal/TwoThousandWithdrawProcessor.hpp](Withdrawal/TwoThousandWithdrawProcessor.hpp) | Process ₹2000 notes | `withdraw()` |
| [Withdrawal/FiveHundredWithdrawProcessor.hpp](Withdrawal/FiveHundredWithdrawProcessor.hpp) | Process ₹500 notes | `withdraw()` |
| [Withdrawal/OneHundredWithdrawProcessor.hpp](Withdrawal/OneHundredWithdrawProcessor.hpp) | Process ₹100 notes | `withdraw()` |
| [Card.hpp](Card.hpp) | Card entity with PIN validation | `isCorrectPinEntered()`, `getBankAccount()` |
| [User.hpp](User.hpp) | User entity | `getName()`, `getCard()` |
| [UserBankAccount.hpp](UserBankAccount.hpp) | Account with balance | `getBalance()`, `withdrawBalance()` |
| [TransactionType.hpp](TransactionType.hpp) | Transaction enum | `CASH_WITHDRAWAL`, `CHECK_BALANCE` |

---

## 🎯 Design Patterns Used

| Pattern | Implementation Location | Purpose |
|---------|------------------------|---------|
| **Singleton** | [ATM.hpp](ATM.hpp) - `getATMObject()`, private constructor | Single ATM instance |
| **State Pattern** | [States/](States/) - ATMState hierarchy (Idle→HasCard→SelectOp→CashWithdrawal/CheckBalance) | Manage ATM operation states |
| **Chain of Responsibility** | [Withdrawal/](Withdrawal/) - CashWithdrawProcessor hierarchy (2000→500→100) | Handle different note denominations |

---

## 1) Requirements (~5 minutes)

### Clarifying Questions to Ask
- "What operations should the ATM support? Withdrawal, balance check, deposit, transfer?"
- "Is this card-based authentication or cardless (using phone/biometrics)?"
- "Can one card be linked to multiple accounts?"
- "What denominations are available? Any constraints on dispensing?"
- "Should the system handle multiple ATMs or just one?"

### Requirements

```
Requirements:
1. User inserts card and authenticates with PIN.
2. ATM supports cash withdrawal and balance inquiry.
3. Cash is dispensed in available denominations (e.g., 2000, 500, 100).
4. Invalid operations should be rejected (wrong PIN, insufficient funds, invalid amount).
5. ATM tracks its own cash inventory and rejects withdrawals it cannot fulfill.
6. Card is retained/blocked after 3 wrong PIN attempts.

Out of Scope:
- Multi-bank network integration
- Card-to-card transfers
- Deposit functionality
- Admin interface for restocking
- Concurrent user sessions
```

---

## 2) Entities and Relationships (~3 minutes)

### Identify Entities

Scan the requirements and pull out meaningful nouns:

- **ATM** - The physical machine, manages state and cash inventory
- **Card** - Links to bank account, stores PIN
- **User** - The person using the ATM
- **UserBankAccount** - Stores balance, handles debit/credit
- **ATMState** - Represents current operational state
- **CashWithdrawProcessor** - Handles denomination-based dispensing

### Define Relationships

```
Entities:
- ATM (Singleton)
- Card
- User
- UserBankAccount
- ATMState (abstract, multiple implementations)
- CashWithdrawProcessor (abstract, chain of handlers)

Relationships:
- User -> Card (1:1)
- Card -> UserBankAccount (1:1)
- ATM -> ATMState (1:1, current state)
- ATM -> CashWithdrawProcessor (1:1, chain head)
- ATMState implementations: IdleState, HasCardState, SelectOperationState, 
                            CashWithdrawalState, CheckBalanceState
```

---

## 3) Class Design (~10-15 minutes)

### ATM (Singleton - Orchestrator)

| Requirement | State Needed |
|-------------|--------------|
| Single physical machine | Singleton instance |
| Track cash inventory | noOf2000Notes, noOf500Notes, noOf100Notes |
| Track total balance | atmBalance |
| Manage current state | currentState: ATMState* |

```
class ATM:
  - instance: ATM* (static)
  - atmBalance: int
  - noOf2000Notes: int
  - noOf500Notes: int
  - noOf100Notes: int
  - currentState: ATMState*

  + getInstance() -> ATM*
  + setCurrentState(state: ATMState*)
  + getCurrentState() -> ATMState*
  + getAtmBalance() -> int
  + deductNotes(denomination: int, count: int)
```

### Card

| Requirement | State Needed |
|-------------|--------------|
| PIN validation | pin: int |
| Link to account | bankAccount: UserBankAccount* |

```
class Card:
  - cardNumber: string
  - pin: int
  - bankAccount: UserBankAccount*

  + validatePIN(inputPin: int) -> bool
  + getBankAccount() -> UserBankAccount*
```

### UserBankAccount

```
class UserBankAccount:
  - accountNumber: string
  - balance: int

  + getBalance() -> int
  + withdraw(amount: int) -> bool
  + deposit(amount: int)
```

### ATMState (Abstract - State Pattern)

```
class ATMState:
  + insertCard(atm: ATM*, card: Card*)
  + authenticatePin(atm: ATM*, card: Card*, pin: int)
  + selectOperation(atm: ATM*, card: Card*, txnType: TransactionType)
  + cashWithdrawal(atm: ATM*, card: Card*, amount: int)
  + displayBalance(atm: ATM*, card: Card*)
  + returnCard()
  + exit(atm: ATM*)
```

### CashWithdrawProcessor (Abstract - Chain of Responsibility)

```
class CashWithdrawProcessor:
  - nextProcessor: CashWithdrawProcessor*

  + withdraw(atm: ATM*, amount: int)
```

---

## 4) Implementation (~10 minutes)

### Key Method: CashWithdrawalState.cashWithdrawal()

```
cashWithdrawal(atm, card, amount)
    // Validation
    if amount <= 0
        return error "Invalid amount"
    if amount % 100 != 0
        return error "Amount must be multiple of 100"
    if amount > atm.getAtmBalance()
        return error "ATM has insufficient funds"
    if amount > card.getBankAccount().getBalance()
        return error "Insufficient account balance"

    // Deduct from bank account
    card.getBankAccount().withdraw(amount)

    // Dispense cash using Chain of Responsibility
    cashProcessor.withdraw(atm, amount)

    // Return card and reset state
    atm.setCurrentState(new IdleState())
    return success
```

### Key Method: TwoThousandWithdrawProcessor.withdraw()

```
withdraw(atm, amount)
    requiredNotes = amount / 2000
    availableNotes = atm.getNoOf2000Notes()
    notesToDispense = min(requiredNotes, availableNotes)

    // Dispense notes
    atm.setNoOf2000Notes(availableNotes - notesToDispense)
    dispensed = notesToDispense * 2000
    remaining = amount - dispensed

    // Pass remainder to next processor
    if remaining > 0 and nextProcessor != null
        nextProcessor.withdraw(atm, remaining)
```

### Key Method: HasCardState.authenticatePin()

```
authenticatePin(atm, card, inputPin)
    if card.validatePIN(inputPin)
        atm.setCurrentState(new SelectOperationState())
    else
        wrongAttempts++
        if wrongAttempts >= 3
            retainCard()
            atm.setCurrentState(new IdleState())
        else
            return error "Wrong PIN. Attempts remaining: " + (3 - wrongAttempts)
```

### Verification: Walk Through a Scenario

```
Initial: ATM in IdleState, balance=50000, 2000-notes=10, 500-notes=20

1. insertCard(card) → State changes to HasCardState
2. authenticatePin(card, 1234) → PIN valid, State → SelectOperationState
3. selectOperation(CASH_WITHDRAWAL) → State → CashWithdrawalState
4. cashWithdrawal(card, 2500)
   - Validate: 2500 > 0 ✓, 2500 % 100 = 0 ✓
   - Account has funds ✓, ATM has funds ✓
   - Deduct 2500 from account
   - Chain: 2000-processor dispenses 1×2000, passes 500
   - Chain: 500-processor dispenses 1×500
   - State → IdleState
5. Final: ATM balance=47500, 2000-notes=9, 500-notes=19
```

---

## 5) Extensibility (~5 minutes)

### "How would you add a transfer operation?"

> "All operations flow through SelectOperationState. I'd add a new TransactionType.TRANSFER, create a TransferState that handles source/destination accounts, validates both, and performs atomic transfer. The state pattern makes this clean—just add a new state class."

### "How would you handle concurrent access?"

> "ATM is singleton, so I'd add a mutex. Each operation locks at the start and releases after. For distributed ATMs, I'd use database-level locking on account records."

```
class ATM:
  - mutex: std::mutex

  void performOperation()
      lock_guard<mutex> lock(mtx)
      // operation logic
```

### "How would you add new denominations?"

> "Chain of Responsibility makes this simple. Just create a new processor (e.g., TwoHundredWithdrawProcessor) and insert it into the chain at the appropriate position."

### "What if ATM runs out of one denomination?"

> "Each processor checks available notes. If insufficient, it dispenses what's available and passes the remainder. The chain naturally handles this."

---

## Design Patterns Summary

| Pattern | Where | Why |
|---------|-------|-----|
| **Singleton** | ATM | One physical machine per instance |
| **State** | ATMState hierarchy | Different behavior per operational state |
| **Chain of Responsibility** | CashWithdrawProcessor | Handle multiple denominations flexibly |

---

## State Transitions

| Current State | Action | Next State | Condition |
|--------------|--------|------------|-----------|
| Idle | insertCard | HasCard | Always |
| HasCard | authenticatePin | SelectOperation | PIN correct |
| HasCard | authenticatePin | Idle | PIN wrong 3x |
| SelectOperation | selectOp(CASH) | CashWithdrawal | - |
| SelectOperation | selectOp(BALANCE) | CheckBalance | - |
| CashWithdrawal | withdraw | Idle | After completion |
| CheckBalance | display | Idle | After display |
