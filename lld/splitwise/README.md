# Splitwise - Low Level Design (C++)

## Design Patterns Used
- **Strategy Pattern**: SplitStrategy interface with EqualSplitStrategy, PercentageSplitStrategy
- **Factory Pattern**: SplitStrategyFactory creates appropriate split strategy
- **Repository Pattern**: GroupRepository for data persistence abstraction

## Class Diagram

```mermaid
classDiagram
    class SplitType {
        <<enumeration>>
        EQUAL
        PERCENTAGE
    }
    
    class User {
        -string id
        -string name
        +getId() string
        +getName() string
    }
    
    class Split {
        -User* user
        -double amount
        +getUser() User*
        +getAmount() double
    }
    
    class Expense {
        -string id
        -string description
        -double amount
        -User* paidBy
        -vector~Split~ splits
        +getAmount() double
        +getPaidBy() User*
        +getSplits() vector~Split~
    }
    
    class BalanceSheet {
        -double totalPaid
        -double totalExpense
        -map~string, double~ balances
        +updateBalance(string userId, double amount)
        +getBalance(string userId) double
    }
    
    class Group {
        -string id
        -string name
        -vector~User*~ members
        -vector~Expense*~ expenses
        -map~string, BalanceSheet~ balanceSheets
        +addMember(User* user)
        +addExpense(Expense* expense)
        +getBalanceSheet(string userId) BalanceSheet
    }
    
    class SplitStrategy {
        <<interface>>
        +split(double amount, vector~User*~ participants, map~string,double~ meta)* vector~Split~
    }
    
    class EqualSplitStrategy {
        +split(double amount, vector~User*~ participants, map~string,double~ meta) vector~Split~
    }
    
    class PercentageSplitStrategy {
        +split(double amount, vector~User*~ participants, map~string,double~ meta) vector~Split~
    }
    
    class GroupService {
        -GroupRepository* repo
        +createGroup(string name, vector~User*~ members) string
        +addMember(string groupId, User* user)
        +addExpense(string groupId, ExpenseRequest req)
        +simplifyDebts(string groupId)
        +printBalances(string groupId)
    }
    
    class DebtSimplificationService {
        +simplifyDebts(Group* group)
    }
    
    class SplitStrategyFactory {
        +get(SplitType type)$ SplitStrategy*
    }

    SplitStrategy <|.. EqualSplitStrategy
    SplitStrategy <|.. PercentageSplitStrategy
    
    Group --> User
    Group --> Expense
    Group --> BalanceSheet
    
    Expense --> User
    Expense --> Split
    
    Split --> User
    
    GroupService --> SplitStrategy
    GroupService --> DebtSimplificationService
    
    SplitStrategyFactory ..> SplitStrategy : creates
```

## Expense Split Flow

```mermaid
flowchart TD
    A[Add Expense] --> B{Split Type?}
    B -->|EQUAL| C[EqualSplitStrategy]
    B -->|PERCENTAGE| D[PercentageSplitStrategy]
    C --> E[Divide Equally]
    D --> F[Apply Percentages]
    E --> G[Create Splits]
    F --> G
    G --> H[Update Payer Balance Sheet]
    H --> I[Update Each Participant Balance Sheet]
    I --> J[Calculate Net Balances]
```

## Debt Simplification Algorithm

```mermaid
flowchart TD
    A[Start Simplification] --> B[Calculate Net Balances]
    B --> C[Separate Creditors & Debtors]
    C --> D[Sort by Amount]
    D --> E{Any Debts Left?}
    E -->|Yes| F[Match Largest Creditor with Largest Debtor]
    F --> G[Settle Minimum of Both]
    G --> H[Update Balances]
    H --> I[Record Transaction]
    I --> E
    E -->|No| J[Return Simplified Transactions]
```

## Sequence Diagram

```mermaid
sequenceDiagram
    participant User
    participant GroupService
    participant SplitStrategyFactory
    participant SplitStrategy
    participant Group
    participant BalanceSheet
    
    User->>GroupService: addExpense(groupId, request)
    GroupService->>SplitStrategyFactory: get(splitType)
    SplitStrategyFactory-->>GroupService: strategy
    GroupService->>SplitStrategy: split(amount, participants, meta)
    SplitStrategy-->>GroupService: splits[]
    GroupService->>Group: addExpense(expense)
    loop For each split
        GroupService->>BalanceSheet: updateBalance(userId, amount)
    end
    GroupService-->>User: expense added
```

## Files Structure
```
splitwise/
├── enums/
│   └── SplitType.hpp
├── model/
│   ├── User.hpp
│   ├── Split.hpp
│   ├── Expense.hpp
│   ├── BalanceSheet.hpp
│   └── Group.hpp
├── strategy/
│   ├── SplitStrategy.hpp
│   ├── EqualSplitStrategy.hpp
│   └── PercentageSplitStrategy.hpp
├── factory/
│   └── SplitStrategyFactory.hpp
├── repository/
│   └── GroupRepository.hpp
├── service/
│   ├── GroupService.hpp
│   └── DebtSimplificationService.hpp
└── main.cpp
```
