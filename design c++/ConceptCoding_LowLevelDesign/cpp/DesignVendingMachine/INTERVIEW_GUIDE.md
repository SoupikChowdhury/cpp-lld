# Vending Machine - LLD Interview Guide

A step-by-step framework for delivering the vending machine low-level design interview.

---

## 📁 Code Reference

| File | Purpose | Key Functions |
|------|---------|---------------|
| [VendingMachine.hpp](VendingMachine.hpp) | Context class managing state | `setCurrentState()`, `getCurrentState()`, `getInventory()` |
| [VendingMachine.cpp](VendingMachine.cpp) | All state implementations | State transition logic, coin handling |
| [States/State.hpp](States/State.hpp) | Abstract state interface | `clickOnInsertCoinButton()`, `insertCoin()`, `chooseProduct()`, `dispenseProduct()`, `refundFullMoney()` |
| [States/IdleState.hpp](States/IdleState.hpp) | Initial state | `clickOnInsertCoinButton()` → transitions to HasMoneyState |
| [States/HasMoneyState.hpp](States/HasMoneyState.hpp) | Money inserted | `insertCoin()`, `chooseProduct()` → transitions to SelectionState |
| [States/SelectionState.hpp](States/SelectionState.hpp) | Product selection | `chooseProduct()` → validates payment, transitions to DispenseState |
| [States/DispenseState.hpp](States/DispenseState.hpp) | Dispense product | `dispenseProduct()` → returns to IdleState |
| [Inventory.hpp](Inventory.hpp) | Manages item shelves | `addItem()`, `getItem()`, `updateInventory()` |
| [ItemShelf.hpp](ItemShelf.hpp) | Individual shelf slot | `getItem()`, `setItem()`, `isSoldOut()` |
| [Item.hpp](Item.hpp) | Product item | `getType()`, `getPrice()` |
| [ItemType.hpp](ItemType.hpp) | Item type enum | `COKE`, `PEPSI`, `JUICE`, `SODA` |
| [Coin.hpp](Coin.hpp) | Coin denomination enum | `PENNY`, `NICKEL`, `DIME`, `QUARTER` |

---

## 🎯 Design Patterns Used

| Pattern | Implementation Location | Purpose |
|---------|------------------------|----------|
| **State Pattern** | [States/](States/) - State hierarchy (Idle→HasMoney→Selection→Dispense→Idle) | Manage vending machine operation states |
| **Composition** | VendingMachine contains Inventory, Inventory contains ItemShelf | Object relationships |

---

## 1) Requirements (~5 minutes)

### Clarifying Questions to Ask
- "What payment methods? Coins only, or cards and digital payments too?"
- "Is there one item per slot or multiple items of the same type per slot?"
- "Should we handle refunds if the user cancels mid-transaction?"
- "Are there admin operations like restocking or viewing sales?"

### Requirements

```
Requirements:
1. Machine displays available items with prices.
2. User can insert coins of various denominations.
3. User selects a product by entering its code.
4. If payment is sufficient, product is dispensed and change returned.
5. If payment is insufficient, all coins are refunded.
6. User can cancel and get a refund at any point before dispensing.
7. Machine tracks inventory and marks items as sold out.

Out of Scope:
- Credit/debit card payments
- Admin restocking interface
- Multiple vending machines
- Sales analytics and reporting
- Networked inventory management
```

---

## 2) Entities and Relationships (~3 minutes)

### Identify Entities

Scan the requirements and pull out meaningful nouns:

- **VendingMachine** - The context, manages state and inventory
- **Item** - Product with type and price
- **ItemShelf** - Slot containing items
- **Inventory** - Manages all slots
- **Coin** - Payment denomination
- **State** - Current operational state of the machine

### Define Relationships

```
Entities:
- VendingMachine (context)
- Inventory
- ItemShelf
- Item
- Coin (enum)
- State (abstract, multiple implementations)

Relationships:
- VendingMachine -> Inventory (1:1)
- VendingMachine -> State (1:1, current state)
- VendingMachine -> Coin (1:N, inserted coins)
- Inventory -> ItemShelf (1:N)
- ItemShelf -> Item (1:1)
- State implementations: IdleState, HasMoneyState, SelectionState, DispenseState
```

---

## 3) Class Design (~10-15 minutes)

### VendingMachine (Context)

| Requirement | State Needed |
|-------------|--------------|
| Track coins inserted | coinList: vector<Coin> |
| Track current state | currentState: State* |
| Manage inventory | inventory: Inventory* |

```
class VendingMachine:
  - currentState: State*
  - inventory: Inventory*
  - coinList: vector<Coin>

  + setState(state: State*)
  + getState() -> State*
  + getInventory() -> Inventory*
  + getCoinList() -> vector<Coin>
  + addCoin(coin: Coin)
  + clearCoins()
  + getTotalInserted() -> int
```

### State (Abstract - State Pattern)

```
class State:
  + pressInsertCoinButton(machine: VendingMachine*)
  + insertCoin(machine: VendingMachine*, coin: Coin)
  + pressSelectProductButton(machine: VendingMachine*)
  + chooseProduct(machine: VendingMachine*, code: int)
  + dispenseProduct(machine: VendingMachine*, code: int)
  + getChange(machine: VendingMachine*, amount: int) -> vector<Coin>
  + cancelOrRefund(machine: VendingMachine*)
```

### Inventory

| Requirement | State Needed |
|-------------|--------------|
| Store all shelves | shelves: vector<ItemShelf> |

```
class Inventory:
  - shelves: vector<ItemShelf>

  + addItem(item: Item*, code: int)
  + getItem(code: int) -> Item*
  + isItemAvailable(code: int) -> bool
  + updateSoldOutStatus(code: int)
```

### ItemShelf

```
class ItemShelf:
  - code: int
  - item: Item*
  - soldOut: bool

  + getCode() -> int
  + getItem() -> Item*
  + isSoldOut() -> bool
  + setSoldOut(status: bool)
```

### Item

```
class Item:
  - type: ItemType
  - price: int

  + getType() -> ItemType
  + getPrice() -> int
```

### Coin (Enum)

```
enum Coin:
  PENNY = 1
  NICKEL = 5
  DIME = 10
  QUARTER = 25
```

---

## 4) Implementation (~10 minutes)

### Key Method: SelectionState.chooseProduct()

```
chooseProduct(machine, code) -> void
    // Get item from inventory
    item = machine.getInventory().getItem(code)
    
    if item == null or machine.getInventory().isSoldOut(code)
        throw "Item not available"
    
    // Check payment
    totalInserted = machine.getTotalInserted()
    price = item.getPrice()
    
    if totalInserted < price
        // Refund all coins
        refundCoins = machine.getCoinList()
        machine.clearCoins()
        machine.setState(new IdleState())
        return refundCoins  // "Insufficient payment, refunding"
    
    // Calculate and return change
    change = totalInserted - price
    if change > 0
        changeCoins = getChange(machine, change)
        // Return changeCoins to user
    
    // Move to dispense state
    machine.setState(new DispenseState(code))
```

### Key Method: DispenseState.dispenseProduct()

```
dispenseProduct(machine, code) -> Item
    // Get item
    item = machine.getInventory().getItem(code)
    
    // Update inventory
    machine.getInventory().updateSoldOutStatus(code)
    
    // Clear inserted coins (payment complete)
    machine.clearCoins()
    
    // Return to idle state
    machine.setState(new IdleState())
    
    return item  // Dispensed!
```

### Key Method: getChange() - Greedy Algorithm

```
getChange(machine, amount) -> vector<Coin>
    result = []
    remaining = amount
    
    // Greedy: use largest denominations first
    denominations = [QUARTER, DIME, NICKEL, PENNY]  // 25, 10, 5, 1
    
    for coin in denominations:
        while remaining >= coin.value
            result.append(coin)
            remaining -= coin.value
    
    return result
```

### Key Method: HasMoneyState.cancelOrRefund()

```
cancelOrRefund(machine) -> vector<Coin>
    // Return all inserted coins
    refund = machine.getCoinList()
    machine.clearCoins()
    
    // Return to idle state
    machine.setState(new IdleState())
    
    return refund
```

### Verification: Walk Through a Scenario

```
Initial: Machine in IdleState, Item "COKE" at code 101, price 75 cents

1. pressInsertCoinButton() → State changes to HasMoneyState

2. insertCoin(QUARTER) → coinList = [25], total = 25
3. insertCoin(QUARTER) → coinList = [25, 25], total = 50
4. insertCoin(QUARTER) → coinList = [25, 25, 25], total = 75

5. pressSelectProductButton() → State changes to SelectionState

6. chooseProduct(101)
   - Get item: COKE, price = 75
   - totalInserted = 75, price = 75 → Sufficient!
   - change = 0, no change needed
   - State → DispenseState(101)

7. dispenseProduct(101)
   - Get COKE item
   - Mark shelf 101 as soldOut
   - Clear coins
   - State → IdleState
   - Return COKE to user

Final: Machine back in IdleState, shelf 101 sold out
```

---

## 5) Extensibility (~5 minutes)

### "How would you add card payment?"

> "I'd use the Strategy pattern for payment methods. Create a PaymentStrategy interface with CoinPayment and CardPayment implementations. The machine delegates payment processing to the selected strategy."

```
class PaymentStrategy:
  + processPayment(amount: int) -> bool
  + refund(amount: int)

class CoinPayment : PaymentStrategy
class CardPayment : PaymentStrategy
```

### "How would you handle concurrent access?"

> "Add a mutex to the VendingMachine. Lock at the start of any state transition and unlock after. This prevents two users from interacting with the same machine simultaneously."

```
class VendingMachine:
  - mutex: std::mutex
  
  + insertCoin(coin)
      lock_guard<mutex> lock(mtx)
      currentState.insertCoin(this, coin)
```

### "How would you add multiple items per slot?"

> "Change ItemShelf to track quantity instead of just soldOut boolean. Decrement on purchase, mark soldOut when quantity reaches 0."

```
class ItemShelf:
  - quantity: int  // Instead of bool soldOut
  
  + decrementQuantity()
  + isSoldOut() -> quantity == 0
```

### "How would you handle machine maintenance?"

> "Add a MaintenanceState that can only be entered by admin authentication. In this state, restocking and price changes are allowed. Normal user operations throw an error."

---

## Design Patterns Summary

| Pattern | Where | Why |
|---------|-------|-----|
| **State** | VendingMachine states | Different behavior per operational state |
| **Singleton** | VendingMachine (optional) | One machine instance |
| **Strategy** | Payment methods (extension) | Different payment algorithms |
| **Factory** | Item creation (optional) | Decouple item creation |

---

## State Transitions

| Current State | Action | Next State | Condition |
|--------------|--------|------------|-----------|
| Idle | insertCoinButton | HasMoney | Always |
| Idle | updateInventory | Idle | Admin only |
| HasMoney | insertCoin | HasMoney | - |
| HasMoney | selectButton | Selection | - |
| HasMoney | cancel | Idle | Refund coins |
| Selection | chooseProduct | Dispense | Sufficient payment |
| Selection | chooseProduct | Idle | Insufficient (refund) |
| Dispense | dispense | Idle | Auto-transition |

---

## Edge Cases

1. **Insufficient payment** - Refund all coins, return to Idle
2. **Item sold out** - Show error, allow reselection
3. **Exact change** - No change returned
4. **Cancel mid-transaction** - Refund all coins, reset
5. **Invalid product code** - Show error, stay in Selection state
