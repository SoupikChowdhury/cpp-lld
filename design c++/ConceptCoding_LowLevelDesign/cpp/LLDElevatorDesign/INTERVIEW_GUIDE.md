# Elevator System - LLD Interview Guide

A step-by-step framework for delivering the elevator system low-level design interview.

---

## 📁 Code Reference

| File | Purpose | Key Functions |
|------|---------|---------------|
| [Building.hpp](Building.hpp) | Building facade | `requestElevator()`, `requestFloor()`, `processAllRequests()` |
| [Floor.hpp](Floor.hpp) | Individual floor | `getFloorNumber()`, `pressUpButton()`, `pressDownButton()` |
| [ElevatorController.hpp](ElevatorController.hpp) | Controls one elevator | `addRequest()`, `processRequests()`, `moveToFloor()`, `selectOptimalElevator()` |
| [ElevatorCar.hpp](ElevatorCar.hpp) | Physical elevator | `moveUp()`, `moveDown()`, `stop()`, `openDoor()`, `closeDoor()` |
| [ElevatorDisplay.hpp](ElevatorDisplay.hpp) | Display component | `showFloor()`, `showDirection()` |
| [ElevatorDoor.hpp](ElevatorDoor.hpp) | Door component | `open()`, `close()`, `isOpen()` |
| [ElevatorState.hpp](ElevatorState.hpp) | State enum | `IDLE`, `MOVING`, `STOPPED` |
| [Direction.hpp](Direction.hpp) | Direction enum | `UP`, `DOWN`, `NONE` |
| [InternalButtons.hpp](InternalButtons.hpp) | Inside elevator buttons | `pressButton()`, `getRequestedFloors()` |
| [ExternalButtons.hpp](ExternalButtons.hpp) | Floor call buttons | `pressUp()`, `pressDown()` |
| [InternalDispatcher.hpp](InternalDispatcher.hpp) | Handles internal requests | `dispatchRequest()` |
| [ExternalDispatcher.hpp](ExternalDispatcher.hpp) | Handles external requests (Singleton) | `dispatchRequest()`, `selectOptimalElevator()` - LOOK algorithm |

---

## 🎯 Design Patterns Used

| Pattern | Implementation Location | Purpose |
|---------|------------------------|----------|
| **Singleton** | [ExternalDispatcher.hpp](ExternalDispatcher.hpp), [InternalDispatcher.hpp](InternalDispatcher.hpp) | Central request dispatching |
| **Strategy Pattern** | [ExternalDispatcher.hpp](ExternalDispatcher.hpp) - LOOK algorithm for elevator selection | Pluggable scheduling algorithms |
| **Composition** | Building contains Floors and ElevatorControllers | Object relationships |
| **State Machine** | ElevatorCar uses ElevatorState (IDLE→MOVING→STOPPED) | Elevator state management |

---

## 1) Requirements (~5 minutes)

### Clarifying Questions to Ask
- "How many elevators and floors should the system support?"
- "Are there different elevator types (express, freight, VIP)?"
- "What scheduling algorithm should we use for dispatching?"
- "Should we handle emergency situations?"
- "Are there weight/capacity constraints?"

### Requirements

```
Requirements:
1. Building has N floors and M elevators.
2. Users can request elevators from any floor (external request with direction).
3. Users inside elevator can select destination floors (internal request).
4. System dispatches the optimal elevator for external requests.
5. Elevators move up/down, stop at requested floors, and open/close doors.
6. Display shows current floor and direction.

Out of Scope:
- VIP/Express elevator types
- Weight/capacity sensing
- Fire/emergency protocols
- Elevator maintenance mode
- Voice announcements
- Security access control
```

---

## 2) Entities and Relationships (~3 minutes)

### Identify Entities

Scan the requirements and pull out meaningful nouns:

- **Building** - Contains floors and elevators (Facade)
- **Floor** - Has up/down buttons
- **ElevatorCar** - Physical elevator with door, display, buttons
- **ElevatorController** - Controls one elevator, manages request queue
- **ExternalDispatcher** - Handles floor button requests (Singleton)
- **InternalDispatcher** - Handles requests from inside elevator (Singleton)
- **Request** - Floor + Direction

### Define Relationships

```
Entities:
- Building (Facade)
- Floor
- ElevatorCar
- ElevatorController
- ExternalDispatcher (Singleton)
- InternalDispatcher (Singleton)
- Request

Relationships:
- Building -> Floor (1:N)
- Building -> ElevatorController (1:M)
- ElevatorController -> ElevatorCar (1:1)
- ElevatorController -> Request (1:N, queue)
- Floor -> ExternalDispatcher (N:1)
- ElevatorCar buttons -> InternalDispatcher (N:1)
```

---

## 3) Class Design (~10-15 minutes)

### Building (Facade)

| Requirement | State Needed |
|-------------|--------------|
| Manage floors | floors: vector<Floor> |
| Manage elevators | controllers: vector<ElevatorController> |

```
class Building:
  - floors: vector<Floor>
  - controllers: vector<ElevatorController>
  - numFloors: int
  - numElevators: int

  + requestElevator(floor: int, direction: Direction)
  + requestFloor(elevatorId: int, floor: int)
  + processAllRequests()
```

### ElevatorCar

| Requirement | State Needed |
|-------------|--------------|
| Current position | currentFloor: int |
| Movement direction | direction: Direction |
| State | state: ElevatorState |
| Components | door, display, buttons |

```
class ElevatorCar:
  - elevatorId: int
  - currentFloor: int
  - direction: Direction (UP, DOWN, IDLE)
  - state: ElevatorState (MOVING, IDLE, STOPPED)
  - door: Door
  - display: Display

  + getCurrentFloor() -> int
  + getDirection() -> Direction
  + moveUp()
  + moveDown()
  + stop()
  + openDoor()
  + closeDoor()
  + updateDisplay()
```

### ElevatorController

| Requirement | State Needed |
|-------------|--------------|
| Control one elevator | elevatorCar |
| Manage requests | upRequests, downRequests (sorted) |

```
class ElevatorController:
  - elevatorCar: ElevatorCar
  - upRequests: set<int>  // floors to visit going up
  - downRequests: set<int>  // floors to visit going down

  + addRequest(floor: int, direction: Direction)
  + processRequests()
  + hasRequestsInDirection() -> bool
  + getCurrentFloor() -> int
  + getDirection() -> Direction
  + getPendingRequestCount() -> int
```

### ExternalDispatcher (Singleton)

```
class ExternalDispatcher:
  - instance: ExternalDispatcher* (static)
  - controllers: vector<ElevatorController*>

  + getInstance() -> ExternalDispatcher*
  + setControllers(controllers)
  + dispatchRequest(floor: int, direction: Direction)
  + findOptimalElevator(floor: int, direction: Direction) -> ElevatorController*
```

### InternalDispatcher (Singleton)

```
class InternalDispatcher:
  - instance: InternalDispatcher* (static)
  - controllers: map<int, ElevatorController*>

  + getInstance() -> InternalDispatcher*
  + dispatchRequest(elevatorId: int, floor: int)
```

### Floor

```
class Floor:
  - floorNumber: int
  - upButton: Button
  - downButton: Button

  + pressUpButton()
  + pressDownButton()
```

---

## 4) Implementation (~10 minutes)

### Key Method: ExternalDispatcher.findOptimalElevator() - LOOK Algorithm

```
findOptimalElevator(requestFloor: int, requestDirection: Direction) -> ElevatorController*
    bestController = null
    minScore = MAX_INT
    
    for controller in controllers:
        car = controller.getElevatorCar()
        currentFloor = car.getCurrentFloor()
        elevatorDirection = car.getDirection()
        
        score = MAX_INT
        
        // Case 1: Elevator moving towards request in same direction
        if elevatorDirection == requestDirection:
            if requestDirection == UP and currentFloor <= requestFloor:
                score = requestFloor - currentFloor
            else if requestDirection == DOWN and currentFloor >= requestFloor:
                score = currentFloor - requestFloor
        
        // Case 2: Elevator is idle
        else if elevatorDirection == IDLE:
            score = abs(currentFloor - requestFloor) + 1  // Slight penalty
        
        // Case 3: Elevator moving away - will need to return
        else:
            // Score based on remaining travel + return distance
            score = getRemainingTravel(controller) + 
                    getReturnDistance(controller, requestFloor)
        
        // Prefer elevator with fewer pending requests (tie-breaker)
        if score < minScore or 
           (score == minScore and controller.getPendingRequestCount() < bestController.getPendingRequestCount()):
            minScore = score
            bestController = controller
    
    return bestController
```

### Key Method: ElevatorController.processRequests()

```
processRequests() -> void
    while hasRequests():
        currentFloor = elevatorCar.getCurrentFloor()
        direction = elevatorCar.getDirection()
        
        // Check if we should stop at current floor
        if shouldStopAtFloor(currentFloor, direction):
            elevatorCar.stop()
            elevatorCar.openDoor()
            removeRequest(currentFloor, direction)
            wait(DOOR_OPEN_TIME)
            elevatorCar.closeDoor()
        
        // Determine next move
        if direction == UP:
            if hasUpRequests() and canContinueUp()
                elevatorCar.moveUp()
            else if hasDownRequests()
                elevatorCar.setDirection(DOWN)
            else
                elevatorCar.setDirection(IDLE)
        
        else if direction == DOWN:
            if hasDownRequests() and canContinueDown()
                elevatorCar.moveDown()
            else if hasUpRequests()
                elevatorCar.setDirection(UP)
            else
                elevatorCar.setDirection(IDLE)
        
        else  // IDLE
            if hasUpRequests()
                elevatorCar.setDirection(UP)
            else if hasDownRequests()
                elevatorCar.setDirection(DOWN)
```

### Key Method: ElevatorController.addRequest()

```
addRequest(floor: int, direction: Direction) -> void
    if direction == UP or 
       (direction == NONE and floor > elevatorCar.getCurrentFloor()):
        upRequests.insert(floor)
    else:
        downRequests.insert(floor)
    
    // Wake up idle elevator
    if elevatorCar.getDirection() == IDLE:
        if floor > elevatorCar.getCurrentFloor()
            elevatorCar.setDirection(UP)
        else
            elevatorCar.setDirection(DOWN)
```

### Verification: Walk Through a Scenario

```
Setup: Building with 10 floors, 2 elevators (E1, E2)
- E1 at floor 3, moving UP
- E2 at floor 7, IDLE

Scenario: User at floor 5 presses UP

1. ExternalDispatcher.dispatchRequest(5, UP)
   
2. findOptimalElevator(5, UP):
   - E1: at floor 3, moving UP, floor 5 is ahead
     - Score = 5 - 3 = 2
   - E2: at floor 7, IDLE
     - Score = |7 - 5| + 1 = 3
   - Winner: E1 (score 2)

3. E1.addRequest(5, UP)
   - upRequests = {5}

4. E1.processRequests():
   - Move from 3 → 4 → 5
   - shouldStopAtFloor(5, UP) = true
   - Stop, open door
   - User enters, presses 8
   
5. InternalDispatcher.dispatchRequest(E1, 8)
   - E1.addRequest(8, NONE) → upRequests = {8}

6. E1 continues: 5 → 6 → 7 → 8
   - Stop at 8, user exits
   - No more requests → E1 becomes IDLE
```

---

## 5) Extensibility (~5 minutes)

### "How would you add VIP/Express elevators?"

> "Create ElevatorType enum (NORMAL, EXPRESS, VIP). Express elevators only stop at certain floors (e.g., every 10th). ExternalDispatcher filters eligible elevators based on request floor."

```
class ExpressElevator : ElevatorCar
  + canStopAt(floor: int) -> bool
      return floor % 10 == 0 or floor == 0 or floor == maxFloor
```

### "How would you handle emergency situations?"

> "Add an emergency state. On activation: all elevators stop at nearest floor, open doors, disable new requests, announce evacuation. Implement with State pattern."

```
handleEmergency()
    for controller in controllers:
        controller.stopAtNearestFloor()
        controller.openDoor()
        controller.setEmergencyMode(true)
```

### "How would you add weight/capacity constraints?"

> "Add currentWeight sensor to ElevatorCar. In addRequest(), check if adding this request would overload. If overloaded, skip floor pickup and let another elevator handle it."

```
class ElevatorCar:
  - currentWeight: int
  - maxCapacity: int
  
  + isOverloaded() -> bool
      return currentWeight >= maxCapacity
```

### "How would you prevent request starvation?"

> "Add timestamp to requests. Implement aging—if a request waits beyond threshold, increase its priority. Dispatch that elevator even if not optimal."

---

## Design Patterns Summary

| Pattern | Where | Why |
|---------|-------|-----|
| **Singleton** | ExternalDispatcher, InternalDispatcher | Single dispatch point |
| **Strategy** | Elevator selection algorithm | Swappable algorithms (FCFS, SCAN, LOOK) |
| **State** | ElevatorState | Clean state transitions |
| **Facade** | Building | Simplified system interface |

---

## Elevator Scheduling Algorithms

| Algorithm | Description | Pros | Cons |
|-----------|-------------|------|------|
| **FCFS** | First Come First Served | Simple | Inefficient |
| **SCAN** | Go to end, then reverse | Fair | Unnecessary travel |
| **LOOK** | Reverse at last request | Efficient | Complex |
| **C-SCAN** | One direction only, return empty | Uniform wait | Return trip waste |

---

## Request Flow

```
External Request (Floor button)
          ↓
    ExternalDispatcher
          ↓
    Find optimal elevator (LOOK)
          ↓
    Add to ElevatorController queue
          ↓
    Process: Move → Stop → Door → Continue
          
Internal Request (Inside button)
          ↓
    InternalDispatcher
          ↓
    Route to specific ElevatorController
          ↓
    Add to appropriate queue (up/down)
```
