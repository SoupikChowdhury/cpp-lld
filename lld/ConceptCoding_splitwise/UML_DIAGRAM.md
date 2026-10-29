# Splitwise - UML Class Diagram

## Class Diagram (Mermaid)

```mermaid
classDiagram
    direction TB

    %% Main Application
    class Splitwise {
        -UserController userController
        -GroupController groupController
        -BalanceSheetController balanceSheetController
        +setupUserAndGroup()
        +demo()
        +getUserController() UserController&
        +getGroupController() GroupController&
        +getBalanceSheetController() BalanceSheetController&
        -addUsersToSplitwiseApp()
    }

    %% User Classes
    class User {
        -string userId
        -string userName
        -shared_ptr~UserExpenseBalanceSheet~ userExpenseBalanceSheet
        +getUserId() string
        +getUserName() string
        +getUserExpenseBalanceSheet() shared_ptr~UserExpenseBalanceSheet~
    }

    class UserController {
        -vector~shared_ptr~User~~ userList
        +addUser(shared_ptr~User~)
        +getUser(string userId) shared_ptr~User~
        +getAllUsers() vector~shared_ptr~User~~
    }

    %% Balance Classes
    class Balance {
        -double amountOwe
        -double amountGetBack
        +getAmountOwe() double
        +getAmountGetBack() double
        +setAmountOwe(double)
        +setAmountGetBack(double)
    }

    class UserExpenseBalanceSheet {
        -map~string, shared_ptr~Balance~~ userVsBalance
        -double totalYourExpense
        -double totalPayment
        -double totalYouOwe
        -double totalYouGetBack
        +getUserVsBalance() map
        +getTotalYourExpense() double
        +getTotalPayment() double
        +getTotalYouOwe() double
        +getTotalYouGetBack() double
        +setTotalYourExpense(double)
        +setTotalPayment(double)
        +setTotalYouOwe(double)
        +setTotalYouGetBack(double)
    }

    class BalanceSheetController {
        +updateUserExpenseBalanceSheet(shared_ptr~User~, vector~Split~, double)
        +showBalanceSheetOfUser(shared_ptr~User~)
    }

    %% Expense Classes
    class ExpenseSplitType {
        <<enumeration>>
        EQUAL
        UNEQUAL
        PERCENTAGE
    }

    class Split {
        -shared_ptr~User~ user
        -double amountOwe
        +getUser() shared_ptr~User~
        +getAmountOwe() double
        +setUser(shared_ptr~User~)
        +setAmountOwe(double)
    }

    class ExpenseSplit {
        <<interface>>
        +validateSplitRequest(vector~Split~, double)* bool
    }

    class EqualExpenseSplit {
        +validateSplitRequest(vector~Split~, double) bool
    }

    class UnequalExpenseSplit {
        +validateSplitRequest(vector~Split~, double) bool
    }

    class PercentageExpenseSplit {
        +validateSplitRequest(vector~Split~, double) bool
    }

    class SplitFactory {
        +getSplitObject(ExpenseSplitType)$ unique_ptr~ExpenseSplit~
    }

    class Expense {
        -string expenseId
        -string description
        -double expenseAmount
        -shared_ptr~User~ paidByUser
        -ExpenseSplitType splitType
        -vector~shared_ptr~Split~~ splitDetails
        +getExpenseId() string
        +getDescription() string
        +getExpenseAmount() double
        +getPaidByUser() shared_ptr~User~
        +getSplitType() ExpenseSplitType
        +getSplitDetails() vector~Split~
    }

    class ExpenseController {
        -BalanceSheetController balanceSheetController
        +createExpense(...) shared_ptr~Expense~
    }

    %% Group Classes
    class Group {
        -string groupId
        -string groupName
        -vector~shared_ptr~User~~ groupMembers
        -vector~shared_ptr~Expense~~ expenseList
        -ExpenseController expenseController
        +addMember(shared_ptr~User~)
        +createExpense(...) shared_ptr~Expense~
        +getGroupId() string
        +getGroupName() string
        +getGroupMembers() vector
        +getExpenseList() vector
    }

    class GroupController {
        -vector~shared_ptr~Group~~ groupList
        +createNewGroup(string, string, shared_ptr~User~)
        +getGroup(string) shared_ptr~Group~
        +getAllGroups() vector~Group~
    }

    %% Relationships
    Splitwise --> UserController : uses
    Splitwise --> GroupController : uses
    Splitwise --> BalanceSheetController : uses

    UserController --> User : manages
    User --> UserExpenseBalanceSheet : has
    UserExpenseBalanceSheet --> Balance : contains

    GroupController --> Group : manages
    Group --> User : has members
    Group --> Expense : has
    Group --> ExpenseController : uses

    ExpenseController --> BalanceSheetController : uses
    ExpenseController --> SplitFactory : uses
    ExpenseController --> Expense : creates

    Expense --> User : paidBy
    Expense --> Split : contains
    Expense --> ExpenseSplitType : uses

    Split --> User : references

    SplitFactory --> ExpenseSplit : creates
    SplitFactory --> ExpenseSplitType : uses

    ExpenseSplit <|.. EqualExpenseSplit : implements
    ExpenseSplit <|.. UnequalExpenseSplit : implements
    ExpenseSplit <|.. PercentageExpenseSplit : implements

    BalanceSheetController --> User : updates
    BalanceSheetController --> Split : processes
```

## Component Overview

### Core Components

| Component | Responsibility |
|-----------|----------------|
| **Splitwise** | Main facade class that orchestrates the entire application |
| **User** | Represents a user with their balance sheet |
| **Group** | Container for members and shared expenses |
| **Expense** | Represents a single expense with split details |

### Controllers

| Controller | Responsibility |
|------------|----------------|
| **UserController** | CRUD operations for users |
| **GroupController** | CRUD operations for groups |
| **ExpenseController** | Creates and validates expenses |
| **BalanceSheetController** | Updates and displays balance sheets |

### Strategy Pattern Components

| Component | Role |
|-----------|------|
| **ExpenseSplit** | Strategy interface |
| **EqualExpenseSplit** | Concrete strategy for equal splits |
| **UnequalExpenseSplit** | Concrete strategy for unequal splits |
| **PercentageExpenseSplit** | Concrete strategy for percentage splits |
| **SplitFactory** | Factory to create strategy objects |

## Data Flow

```
User creates expense
        ↓
ExpenseController receives request
        ↓
SplitFactory creates appropriate ExpenseSplit strategy
        ↓
Strategy validates the split (Equal/Unequal/Percentage)
        ↓
Expense object is created
        ↓
BalanceSheetController updates all user balance sheets
        ↓
Users can view their updated balances
```
