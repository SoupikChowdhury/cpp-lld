# Elevator System - UML Diagram

## Class Diagram

```
┌─────────────────────────────────────────────────────────────────────────────────────────────────┐
│                                    ELEVATOR SYSTEM UML                                           │
└─────────────────────────────────────────────────────────────────────────────────────────────────┘

                                    ┌─────────────────┐
                                    │   «enumeration» │
                                    │    Direction    │
                                    ├─────────────────┤
                                    │ UP              │
                                    │ DOWN            │
                                    │ NONE            │
                                    └─────────────────┘

                                    ┌─────────────────┐
                                    │   «enumeration» │
                                    │  ElevatorState  │
                                    ├─────────────────┤
                                    │ MOVING          │
                                    │ IDLE            │
                                    │ STOPPED         │
                                    └─────────────────┘

┌──────────────────────────────────────────────────────────────────────────────────────────────────┐
│                                         BUILDING                                                  │
├──────────────────────────────────────────────────────────────────────────────────────────────────┤
│ - name: string                                                                                    │
│ - floors: vector<unique_ptr<Floor>>                                                              │
│ - elevatorControllers: vector<unique_ptr<ElevatorController>>                                    │
│ - minFloor: int                                                                                   │
│ - maxFloor: int                                                                                   │
├──────────────────────────────────────────────────────────────────────────────────────────────────┤
│ + Building(name: string, numFloors: int, numElevators: int)                                      │
│ + requestElevator(floorNumber: int, dir: Direction): void                                        │
│ + requestFloor(elevatorId: int, floorNumber: int): void                                          │
│ + processAllRequests(): void                                                                      │
│ + showAllElevatorStatus(): void                                                                   │
│ + getFloor(number: int): Floor*                                                                   │
│ + getElevatorController(id: int): ElevatorController*                                            │
└──────────────────────────────────────────────────────────────────────────────────────────────────┘
                    │                                               │
                    │ 1..*                                          │ 1..*
                    ▼                                               ▼
    ┌───────────────────────────┐                 ┌─────────────────────────────────┐
    │          FLOOR            │                 │      ELEVATOR CONTROLLER        │
    ├───────────────────────────┤                 ├─────────────────────────────────┤
    │ - floorNumber: int        │                 │ - elevatorCar: unique_ptr       │
    │ - externalButtons: ptr    │                 │ - upRequests: set<int>          │
    ├───────────────────────────┤                 │ - downRequests: set<int>        │
    │ + getFloorNumber(): int   │                 ├─────────────────────────────────┤
    │ + pressUpButton(): void   │                 │ + addRequest(floor, dir): void  │
    │ + pressDownButton(): void │                 │ + addInternalRequest(floor)     │
    │ + resetButtons(): void    │                 │ + processRequests(): void       │
    └───────────────────────────┘                 │ + hasRequests(): bool           │
              │                                   └─────────────────────────────────┘
              │ 1                                               │
              ▼                                                 │ 1
    ┌───────────────────────────┐                               ▼
    │    EXTERNAL BUTTONS       │                 ┌─────────────────────────────────┐
    ├───────────────────────────┤                 │         ELEVATOR CAR            │
    │ - upButtonPressed: bool   │                 ├─────────────────────────────────┤
    │ - downButtonPressed: bool │                 │ - id: int                       │
    │ - floorNumber: int        │                 │ - currentFloor: int             │
    ├───────────────────────────┤                 │ - direction: Direction          │
    │ + pressUp(): void         │                 │ - state: ElevatorState          │
    │ + pressDown(): void       │                 │ - display: unique_ptr           │
    │ + resetUp(): void         │                 │ - door: unique_ptr              │
    │ + resetDown(): void       │                 │ - internalButtons: unique_ptr   │
    │ + isUpPressed(): bool     │                 ├─────────────────────────────────┤
    │ + isDownPressed(): bool   │                 │ + moveUp(): void                │
    └───────────────────────────┘                 │ + moveDown(): void              │
                                                  │ + stop(): void                  │
                                                  │ + openDoor(): void              │
                                                  │ + closeDoor(): void             │
                                                  │ + showStatus(): void            │
                                                  └─────────────────────────────────┘
                                                          │ 1     │ 1     │ 1
                              ┌────────────────────────────┘       │       └────────────────────┐
                              ▼                                    ▼                            ▼
                ┌─────────────────────────┐         ┌─────────────────────┐      ┌──────────────────────┐
                │    ELEVATOR DISPLAY     │         │    ELEVATOR DOOR    │      │   INTERNAL BUTTONS   │
                ├─────────────────────────┤         ├─────────────────────┤      ├──────────────────────┤
                │ - currentFloor: int     │         │ - state: DoorState  │      │ - pressedFloors: set │
                │ - direction: Direction  │         ├─────────────────────┤      │ - minFloor: int      │
                ├─────────────────────────┤         │ + open(): void      │      │ - maxFloor: int      │
                │ + setDisplay(): void    │         │ + close(): void     │      ├──────────────────────┤
                │ + showDisplay(): void   │         │ + isOpen(): bool    │      │ + pressButton(): bool│
                └─────────────────────────┘         │ + isClosed(): bool  │      │ + clearButton(): void│
                                                    └─────────────────────┘      │ + hasRequests(): bool│
                                                                                 └──────────────────────┘

                    ┌─────────────────────────────────────────────────────────────────┐
                    │                      DISPATCHER SYSTEM                          │
                    └─────────────────────────────────────────────────────────────────┘

    ┌─────────────────────────────────────┐           ┌─────────────────────────────────────┐
    │   EXTERNAL DISPATCHER «Singleton»   │           │   INTERNAL DISPATCHER «Singleton»   │
    ├─────────────────────────────────────┤           ├─────────────────────────────────────┤
    │ - controllers: vector<Controller*>  │           │ - controllers: vector<Controller*>  │
    │ - instance: static                  │           │ - instance: static                  │
    ├─────────────────────────────────────┤           ├─────────────────────────────────────┤
    │ + getInstance(): ExternalDispatcher&│           │ + getInstance(): InternalDispatcher&│
    │ + addController(ctrl): void         │           │ + addController(ctrl): void         │
    │ + dispatchRequest(floor, dir): void │           │ + dispatchRequest(id, floor): void  │
    │ - findOptimalElevator(): Controller*│           │ + clearControllers(): void          │
    └─────────────────────────────────────┘           └─────────────────────────────────────┘
```

## Sequence Diagram - External Request

```
┌───────┐     ┌──────────┐     ┌──────────────────┐     ┌───────────────────┐     ┌─────────────┐
│ User  │     │  Floor   │     │ExternalDispatcher│     │ElevatorController │     │ ElevatorCar │
└───┬───┘     └────┬─────┘     └────────┬─────────┘     └─────────┬─────────┘     └──────┬──────┘
    │              │                    │                         │                      │
    │ pressUpBtn() │                    │                         │                      │
    │─────────────>│                    │                         │                      │
    │              │                    │                         │                      │
    │              │ dispatchRequest()  │                         │                      │
    │              │───────────────────>│                         │                      │
    │              │                    │                         │                      │
    │              │                    │ findOptimalElevator()   │                      │
    │              │                    │──────────┐              │                      │
    │              │                    │          │              │                      │
    │              │                    │<─────────┘              │                      │
    │              │                    │                         │                      │
    │              │                    │ addRequest(floor, dir)  │                      │
    │              │                    │────────────────────────>│                      │
    │              │                    │                         │                      │
    │              │                    │                         │ moveToFloor()        │
    │              │                    │                         │─────────────────────>│
    │              │                    │                         │                      │
    │              │                    │                         │    openDoor()        │
    │              │                    │                         │─────────────────────>│
    │              │                    │                         │                      │
```

## Component Diagram

```
┌─────────────────────────────────────────────────────────────────────────────────────┐
│                              ELEVATOR SYSTEM COMPONENTS                              │
└─────────────────────────────────────────────────────────────────────────────────────┘

     ┌─────────────────────────────────────────────────────────────────────┐
     │                           «component»                                │
     │                            BUILDING                                  │
     │  ┌──────────────────┐  ┌──────────────────┐  ┌──────────────────┐   │
     │  │    Floor Mgmt    │  │  Elevator Mgmt   │  │ Request Handler  │   │
     │  └──────────────────┘  └──────────────────┘  └──────────────────┘   │
     └─────────────────────────────────────────────────────────────────────┘
                     │                    │                    │
                     ▼                    ▼                    ▼
     ┌────────────────────┐   ┌────────────────────┐   ┌────────────────────┐
     │   «component»      │   │   «component»      │   │   «component»      │
     │      FLOOR         │   │  ELEVATOR CAR      │   │   DISPATCHERS      │
     │  ┌──────────────┐  │   │  ┌──────────────┐  │   │  ┌──────────────┐  │
     │  │ ExtButtons   │  │   │  │   Display    │  │   │  │  External    │  │
     │  └──────────────┘  │   │  ├──────────────┤  │   │  ├──────────────┤  │
     │                    │   │  │    Door      │  │   │  │  Internal    │  │
     │                    │   │  ├──────────────┤  │   │  └──────────────┘  │
     │                    │   │  │ IntButtons   │  │   │                    │
     │                    │   │  └──────────────┘  │   │                    │
     └────────────────────┘   └────────────────────┘   └────────────────────┘
```

## State Diagram - Elevator

```
                                    ┌─────────────────┐
                                    │                 │
                                    │      IDLE       │◄─────────────────────────┐
                                    │                 │                          │
                                    └────────┬────────┘                          │
                                             │                                   │
                                   request received                              │
                                             │                                   │
                                             ▼                                   │
                                    ┌─────────────────┐                          │
                          ┌────────>│                 │────────┐                 │
                          │         │     MOVING      │        │                 │
                          │         │                 │        │                 │
                          │         └────────┬────────┘        │                 │
                          │                  │                 │                 │
                more      │        reached   │         more    │                 │
              requests    │         floor    │       requests  │                 │
                          │                  │                 │                 │
                          │                  ▼                 │                 │
                          │         ┌─────────────────┐        │                 │
                          │         │                 │        │                 │
                          └─────────│     STOPPED     │<───────┘                 │
                                    │                 │                          │
                                    └────────┬────────┘                          │
                                             │                                   │
                                      no more requests                           │
                                             │                                   │
                                             └───────────────────────────────────┘
```

## Design Patterns Used

| Pattern | Where Used | Purpose |
|---------|------------|---------|
| **Singleton** | ExternalDispatcher, InternalDispatcher | Ensure single point of control for request dispatching |
| **Strategy** | Elevator selection algorithm | Allow different algorithms for selecting optimal elevator |
| **State** | ElevatorState enum and state transitions | Manage elevator behavior based on current state |
| **Observer** | Display updates | Notify display when floor/direction changes |
| **Facade** | Building class | Provide simplified interface to complex subsystem |
