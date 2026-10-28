# Low Level Design (LLD) - C++ Implementation

## 📚 Complete LLD Interview Preparation Guide

This repository contains C++ implementations of common Low Level Design problems with:
- ✅ Complete C++ source code (C++17)
- ✅ UML Class Diagrams (ASCII art)
- ✅ Interview Preparation Guides (Bottom-up approach)
- ✅ Design Pattern explanations

---

## 📁 Directory Structure

```
cpp/
├── DesignPatterns/
│   ├── AdapterPattern/          # Convert interface to another
│   ├── BuilderPattern/          # Step-by-step construction
│   ├── ChainOfResponsibility/   # Pass request along chain
│   ├── CompositePattern/        # Tree structures (files/folders)
│   └── FlyweightPattern/        # Share objects to save memory
│
├── DesignVendingMachine/        # State Pattern
├── DesignATM/                   # State + Singleton + Chain of Responsibility
├── DesignBookMyShow/            # Movie ticket booking
├── DesignSplitwise/             # Expense sharing (Strategy Pattern)
├── DesignOrderManagementSystem/ # E-commerce order flow
│
├── LLDElevatorDesign/           # Elevator scheduling
├── LLDCarRentalSystem/          # Vehicle rental
├── LLDCricbuzz/                 # Cricket score (Observer Pattern)
├── LLDSnakeLadder/              # Board game
├── LLDTicTacToe/                # Board game
│
├── DesignDataStructures/        # Custom HashMap
└── HandleNullObject/            # Null Object Pattern
```

---

## 🎯 Design Patterns Used

| Pattern | Systems | Key Concept |
|---------|---------|-------------|
| **State** | VendingMachine, ATM | Object behavior changes with state |
| **Singleton** | ATM, Controllers | Single instance |
| **Chain of Responsibility** | ATM (cash dispense), Logging | Pass request along chain |
| **Strategy** | Splitwise, Order Management | Interchangeable algorithms |
| **Observer** | Cricbuzz | Event notification |
| **Factory** | VehicleFactory, SplitFactory | Object creation |
| **Composite** | FileSystem | Tree structure |
| **Flyweight** | Robots, Characters | Share heavy objects |
| **Adapter** | WeightMachine | Interface conversion |
| **Builder** | Student | Step-by-step construction |
| **Null Object** | NullVehicle | Avoid null checks |
| **Facade** | BookMyShow, OrderSystem | Simplified interface |

---

## 📖 How to Use for Interview Preparation

### Bottom-Up Approach (Recommended)

1. **Start with Entities** (5 min)
   - Identify core entities (User, Item, Order)
   - Define their attributes and relationships

2. **Add Behaviors** (5 min)
   - What operations does each entity support?
   - Who owns what behavior?

3. **Identify Patterns** (5 min)
   - Does behavior change with state? → **State Pattern**
   - Multiple ways to do same thing? → **Strategy Pattern**
   - Notify multiple objects? → **Observer Pattern**

4. **Connect Components** (5 min)
   - Controllers, Services, Facades
   - Define APIs and interactions

### For Each System, Study:

```
📁 SystemName/
├── *.hpp, *.cpp      # Source code
├── UML_DIAGRAM.md    # Class relationships
└── INTERVIEW_GUIDE.md # How to explain in interview
```

---

## 🚀 Quick Start

### Compile and Run (Windows with g++)

```powershell
cd "cpp\DesignVendingMachine"
g++ -std=c++17 -o vending.exe main.cpp VendingMachine.cpp
.\vending.exe
```

### Compile (Linux/Mac)

```bash
cd cpp/DesignVendingMachine
g++ -std=c++17 -o vending main.cpp VendingMachine.cpp
./vending
```

---

## 🎤 Interview Cheat Sheet

### 1. Vending Machine
**Patterns:** State Pattern
**Key Classes:** VendingMachine, State, IdleState, HasMoneyState, SelectionState, DispenseState
**Flow:** Insert Coin → Select Product → Validate Payment → Dispense

### 2. ATM Machine
**Patterns:** State + Singleton + Chain of Responsibility
**Key Classes:** ATM, ATMState, CashWithdrawProcessor
**Flow:** Insert Card → Enter PIN → Select Operation → Execute → Return Card

### 3. BookMyShow
**Patterns:** Facade, Singleton Controllers
**Key Classes:** Movie, Theatre, Show, Seat, Booking
**Flow:** Select City → Select Movie → Select Show → Select Seats → Pay → Confirm

### 4. Splitwise
**Patterns:** Strategy (for split types)
**Key Classes:** User, Group, Expense, Split, ExpenseSplit
**Flow:** Create Group → Add Members → Add Expense → Calculate Splits → Update Balances

### 5. Elevator System
**Patterns:** State, Observer, Strategy
**Key Classes:** Building, Floor, ElevatorCar, ElevatorController, Dispatcher
**Algorithm:** LOOK algorithm for scheduling

### 6. Car Rental
**Patterns:** Factory, Strategy
**Key Classes:** Vehicle, Store, Reservation, Bill
**Flow:** Search Store → Select Vehicle → Reserve → Pay → Return

---

## 📊 Complexity Summary

| System | # Classes | Patterns | Difficulty |
|--------|-----------|----------|------------|
| Vending Machine | 10 | State | ⭐⭐ |
| ATM | 15 | State, CoR, Singleton | ⭐⭐⭐ |
| BookMyShow | 12 | Facade | ⭐⭐⭐ |
| Splitwise | 16 | Strategy, Factory | ⭐⭐⭐ |
| Elevator | 15 | State, Observer | ⭐⭐⭐⭐ |
| Car Rental | 18 | Factory, Strategy | ⭐⭐⭐ |
| Cricbuzz | 20 | Observer, Template | ⭐⭐⭐⭐ |
| Order Management | 20 | Strategy, Facade | ⭐⭐⭐⭐ |
| Snake &amp; Ladder | 6 | - | ⭐⭐ |
| TicTacToe | 8 | - | ⭐⭐ |

---

## 💡 Tips for LLD Interviews

1. **Always clarify requirements** before coding
2. **Start with core entities** - identify nouns in problem
3. **Think about relationships** - has-a, is-a
4. **Identify patterns** - don't force them
5. **Consider extensibility** - what might change?
6. **Discuss trade-offs** - why this design?
7. **Draw diagrams** - boxes and arrows help
8. **Write clean code** - names matter!

---

## 📝 License

Educational purposes only. Use for interview preparation.

---

**Happy Coding! 🎉**
