# Splitwise - LLD Interview Guide

A step-by-step framework for delivering the expense sharing application low-level design interview.

---

## 📁 Code Reference

| File | Purpose | Key Functions |
|------|---------|---------------|
| [Splitwise.hpp](Splitwise.hpp) | Main facade class | `addUser()`, `addGroup()`, `createExpense()`, `demo()` |
| [BalanceSheetController.hpp](BalanceSheetController.hpp) | Manages user balances | `updateUserExpenseBalanceSheet()`, `showBalanceSheetOfUser()` |
| [Balance.hpp](Balance.hpp) | Individual balance record | `getAmount()`, `setAmount()` |
| [UserExpenseBalanceSheet.hpp](UserExpenseBalanceSheet.hpp) | User's expense sheet | `getTotalYouOwe()`, `getTotalYouGetBack()`, `getBalances()` |
| [User/User.hpp](User/User.hpp) | User entity | `getName()`, `getId()`, `getBalanceSheet()` |
| [User/UserController.hpp](User/UserController.hpp) | User management | `addUser()`, `getUser()` |
| [Group/Group.hpp](Group/Group.hpp) | Expense group | `addMember()`, `createExpense()`, `getMembers()` |
| [Group/GroupController.hpp](Group/GroupController.hpp) | Group management | `addGroup()`, `getGroup()` |
| [Expense/Expense.hpp](Expense/Expense.hpp) | Expense record | `getAmount()`, `getPaidBy()`, `getSplits()` |
| [Expense/ExpenseController.hpp](Expense/ExpenseController.hpp) | Expense management | `createExpense()` |
| [Expense/ExpenseSplitType.hpp](Expense/ExpenseSplitType.hpp) | Split type enum | `EQUAL`, `UNEQUAL`, `PERCENTAGE` |
| [Expense/SplitFactory.hpp](Expense/SplitFactory.hpp) | Factory for split strategies | `getSplitStrategy()` |
| [Expense/Split/ExpenseSplit.hpp](Expense/Split/ExpenseSplit.hpp) | Strategy interface | `validateSplit()` |
| [Expense/Split/EqualExpenseSplit.hpp](Expense/Split/EqualExpenseSplit.hpp) | Equal split strategy | `validateSplit()` - validates equal division |
| [Expense/Split/UnequalExpenseSplit.hpp](Expense/Split/UnequalExpenseSplit.hpp) | Unequal split strategy | `validateSplit()` - validates custom amounts |
| [Expense/Split/PercentageExpenseSplit.hpp](Expense/Split/PercentageExpenseSplit.hpp) | Percentage split strategy | `validateSplit()` - validates percentages sum to 100 |
| [Expense/Split/Split.hpp](Expense/Split/Split.hpp) | Individual split record | `getUser()`, `getAmount()` |

---

## 🎯 Design Patterns Used

| Pattern | Implementation Location | Purpose |
|---------|------------------------|----------|
| **Strategy Pattern** | [Expense/Split/](Expense/Split/) - ExpenseSplit interface with Equal/Unequal/Percentage | Different split validation algorithms |
| **Factory Pattern** | [Expense/SplitFactory.hpp](Expense/SplitFactory.hpp) - `getSplitStrategy()` | Create appropriate split strategy |
| **Facade Pattern** | [Splitwise.hpp](Splitwise.hpp) | Unified interface for expense operations |

---

## 1) Requirements (~5 minutes)

### Clarifying Questions to Ask
- "What types of expense splits should we support? Equal, exact amounts, percentages?"
- "Should users be part of groups, or can expenses be between any users?"
- "Do we need to track payment settlements, or just balances?"
- "Should we simplify debts (minimize transactions)?"

### Requirements

```
Requirements:
1. Users can create accounts and be identified uniquely.
2. Users can create groups and add members.
3. Users can add expenses with different split types (equal, unequal, percentage).
4. System tracks who owes whom and how much.
5. Users can view their overall balance sheet.
6. Split amounts must be validated (sum matches total).

Out of Scope:
- User authentication
- Payment/settlement processing
- Debt simplification algorithm
- Expense categories and receipts
- Currency conversion
- Push notifications
```

---

## 2) Entities and Relationships (~3 minutes)

### Identify Entities

Scan the requirements and pull out meaningful nouns:

- **User** - Person with balance sheet
- **Group** - Collection of users sharing expenses
- **Expense** - Amount paid by one user, split among others
- **Split** - Individual user's share of an expense
- **ExpenseSplit** - Strategy for validating splits
- **UserExpenseBalanceSheet** - Tracks user's overall balances

### Define Relationships

```
Entities:
- User
- Group
- Expense
- Split
- ExpenseSplit (abstract strategy)
- UserExpenseBalanceSheet
- Controllers (UserController, GroupController, BalanceSheetController)

Relationships:
- User -> UserExpenseBalanceSheet (1:1)
- Group -> User (N:M)
- Group -> Expense (1:N)
- Expense -> Split (1:N)
- Expense -> ExpenseSplit (1:1, strategy)
- ExpenseSplit implementations: EqualExpenseSplit, UnequalExpenseSplit, PercentageExpenseSplit
```

---

## 3) Class Design (~10-15 minutes)

### User

| Requirement | State Needed |
|-------------|--------------|
| Unique identity | userId, name |
| Track balances | balanceSheet |

```
class User:
  - userId: string
  - name: string
  - email: string
  - balanceSheet: UserExpenseBalanceSheet

  + getUserId() -> string
  + getBalanceSheet() -> UserExpenseBalanceSheet
```

### UserExpenseBalanceSheet

| Requirement | State Needed |
|-------------|--------------|
| Total paid | totalPayment |
| Total owed to others | totalYouOwe |
| Total others owe you | totalYouGetBack |
| Per-user balances | userVsBalance: map |

```
class UserExpenseBalanceSheet:
  - totalPayment: double
  - totalYouOwe: double
  - totalYouGetBack: double
  - userVsBalance: map<string, Balance>

  + getTotalPayment() -> double
  + getTotalYouOwe() -> double
  + getTotalYouGetBack() -> double
  + getBalanceWithUser(userId: string) -> Balance
  + updateBalance(userId: string, amount: double, youOwe: bool)
```

### Expense

```
class Expense:
  - expenseId: string
  - description: string
  - totalAmount: double
  - paidByUser: User
  - splitType: ExpenseSplitType
  - splits: vector<Split>

  + getTotalAmount() -> double
  + getPaidByUser() -> User
  + getSplits() -> vector<Split>
```

### Split

```
class Split:
  - user: User
  - amountOwe: double

  + getUser() -> User
  + getAmountOwe() -> double
  + setAmountOwe(amount: double)
```

### ExpenseSplit (Abstract - Strategy Pattern)

```
class ExpenseSplit:
  + validateSplitRequest(splits: vector<Split>, totalAmount: double) -> bool

class EqualExpenseSplit : ExpenseSplit
class UnequalExpenseSplit : ExpenseSplit
class PercentageExpenseSplit : ExpenseSplit
```

### Group

```
class Group:
  - groupId: string
  - groupName: string
  - members: vector<User>
  - expenseController: ExpenseController

  + addMember(user: User)
  + createExpense(description, amount, paidBy, splitType, splits) -> Expense
  + getExpenses() -> vector<Expense>
```

---

## 4) Implementation (~10 minutes)

### Key Method: EqualExpenseSplit.validateSplitRequest()

```
validateSplitRequest(splits, totalAmount) -> bool
    expectedAmount = totalAmount / splits.size()
    
    for split in splits:
        if split.getAmountOwe() != expectedAmount
            throw "Invalid equal split: each should owe " + expectedAmount
    
    return true
```

### Key Method: UnequalExpenseSplit.validateSplitRequest()

```
validateSplitRequest(splits, totalAmount) -> bool
    sum = 0
    for split in splits:
        sum += split.getAmountOwe()
    
    if sum != totalAmount
        throw "Split amounts don't add up to total: " + sum + " vs " + totalAmount
    
    return true
```

### Key Method: PercentageExpenseSplit.validateSplitRequest()

```
validateSplitRequest(splits, totalAmount) -> bool
    percentageSum = 0
    for split in splits:
        percentageSum += split.getPercentage()
    
    if percentageSum != 100
        throw "Percentages must sum to 100, got: " + percentageSum
    
    // Convert percentages to amounts
    for split in splits:
        amount = (split.getPercentage() / 100) * totalAmount
        split.setAmountOwe(amount)
    
    return true
```

### Key Method: BalanceSheetController.updateBalances()

```
updateBalances(paidByUser, splits, totalAmount) -> void
    // Update payer's sheet
    payerSheet = paidByUser.getBalanceSheet()
    payerSheet.addToTotalPayment(totalAmount)
    
    for split in splits:
        owingUser = split.getUser()
        amountOwed = split.getAmountOwe()
        
        if owingUser == paidByUser
            continue  // Skip self
        
        // Payer gets back money from this user
        payerSheet.updateBalance(owingUser.getId(), amountOwed, youGetBack=true)
        payerSheet.addToTotalYouGetBack(amountOwed)
        
        // Owing user owes money to payer
        owingSheet = owingUser.getBalanceSheet()
        owingSheet.updateBalance(paidByUser.getId(), amountOwed, youOwe=true)
        owingSheet.addToTotalYouOwe(amountOwed)
        owingSheet.addToTotalExpense(amountOwed)
```

### Key Method: ExpenseController.createExpense()

```
createExpense(description, amount, paidBy, splitType, splits) -> Expense
    // Get appropriate strategy from factory
    expenseSplit = SplitFactory.getSplitObject(splitType)
    
    // Validate splits
    expenseSplit.validateSplitRequest(splits, amount)
    
    // Create expense
    expense = new Expense(description, amount, paidBy, splitType, splits)
    
    // Update balance sheets
    BalanceSheetController.updateBalances(paidBy, splits, amount)
    
    return expense
```

### Verification: Walk Through a Scenario

```
Setup: Users A, B, C in a group

Expense 1: A pays $300 for dinner, split equally
- Each owes: $300 / 3 = $100
- A paid $300, owes self $100, gets back $200 (from B and C)
- B owes A $100
- C owes A $100

Balance sheets after:
- A: totalPayment=300, totalYouGetBack=200, totalYouOwe=0
  - balance[B] = +100 (B owes A)
  - balance[C] = +100 (C owes A)
- B: totalPayment=0, totalYouOwe=100
  - balance[A] = -100 (B owes A)
- C: totalPayment=0, totalYouOwe=100
  - balance[A] = -100 (C owes A)

Expense 2: B pays $150 for taxi, split equally
- Each owes: $150 / 3 = $50
- B paid $150, gets back $100 (from A and C)
- A owes B $50
- C owes B $50

Balance sheets after:
- A: totalYouOwe=50, totalYouGetBack=200
  - balance[B] = +100 - 50 = +50 (net: B owes A $50)
- B: totalPayment=150, totalYouGetBack=100, totalYouOwe=100
  - balance[A] = -100 + 50 = -50 (net: B owes A $50)
  - balance[C] = +50 (C owes B)
```

---

## 5) Extensibility (~5 minutes)

### "How would you add a new split type (e.g., by shares)?"

> "Strategy pattern makes this simple. Create a SharesExpenseSplit class implementing ExpenseSplit. Add new enum value to ExpenseSplitType. Add case to SplitFactory. No existing code changes needed."

```
class SharesExpenseSplit : ExpenseSplit
    validateSplitRequest(splits, totalAmount)
        totalShares = sum of all shares
        for split in splits:
            split.amountOwe = (split.shares / totalShares) * totalAmount
```

### "How would you implement debt simplification?"

> "Calculate net balance for each user. Users with positive balance receive money, negative balance pay. Use a greedy algorithm to pair largest creditor with largest debtor, settle the minimum of their amounts, repeat until all balanced."

### "How would you handle concurrent expense additions?"

> "Add a mutex at the group level. Lock when creating expenses to prevent race conditions in balance updates."

```
class Group:
  - mutex: std::mutex
  
  + createExpense(...)
      lock_guard<mutex> lock(mtx)
      // expense creation logic
```

### "How would you add expense notifications?"

> "Use the Observer pattern. BalanceSheetController notifies registered observers when balances change. NotificationService implements the observer interface and sends alerts."

---

## Design Patterns Summary

| Pattern | Where | Why |
|---------|-------|-----|
| **Strategy** | ExpenseSplit hierarchy | Different validation per split type |
| **Factory** | SplitFactory | Create appropriate strategy at runtime |
| **Facade** | Splitwise class | Simplified interface to subsystem |
| **Singleton** | Controllers | Central management |
| **Observer** | Notifications (extension) | Balance update alerts |

---

## Split Type Validation Summary

| Split Type | Validation Rule |
|------------|-----------------|
| Equal | Each split = total / count |
| Unequal | Sum of splits = total |
| Percentage | Percentages sum to 100%, then calculate amounts |
| Shares | Calculate proportional amounts based on shares |
