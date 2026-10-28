# Snake and Ladder Game - UML Diagram

## Class Diagram

```
┌─────────────────────────────────────────────────────────────────────────────────────────────────┐
│                                 SNAKE AND LADDER GAME UML                                        │
└─────────────────────────────────────────────────────────────────────────────────────────────────┘

                                    ┌─────────────────┐
                                    │  «enumeration»  │
                                    │    JumpType     │
                                    ├─────────────────┤
                                    │ SNAKE           │
                                    │ LADDER          │
                                    └─────────────────┘

┌──────────────────────────────────────────────────────────────────────────────────────────────────┐
│                                           GAME                                                    │
├──────────────────────────────────────────────────────────────────────────────────────────────────┤
│ - board: unique_ptr<Board>                                                                        │
│ - dice: unique_ptr<Dice>                                                                          │
│ - players: queue<shared_ptr<Player>>                                                              │
│ - winner: shared_ptr<Player>                                                                      │
│ - gameOver: bool                                                                                  │
│ - turnCount: int                                                                                  │
├──────────────────────────────────────────────────────────────────────────────────────────────────┤
│ + Game(boardSize, numSnakes, numLadders, numDice)                                                │
│ + Game(boardSize, snakes, ladders, numDice)                                                      │
│ + addPlayer(id: string, name: string): void                                                      │
│ + playTurn(): shared_ptr<Player>                                                                 │
│ + play(): shared_ptr<Player>                                                                     │
│ + playWithLimit(maxTurns: int): shared_ptr<Player>                                               │
│ + isGameOver(): bool                                                                              │
│ + getWinner(): shared_ptr<Player>                                                                │
│ + displayPlayerPositions(): void                                                                  │
└──────────────────────────────────────────────────────────────────────────────────────────────────┘
            │                    │                    │
            │ 1                  │ 1                  │ 2..*
            ▼                    ▼                    ▼
┌─────────────────────┐  ┌─────────────────────┐  ┌─────────────────────┐
│       BOARD         │  │        DICE         │  │       PLAYER        │
├─────────────────────┤  ├─────────────────────┤  ├─────────────────────┤
│ - size: int         │  │ - numberOfDice: int │  │ - id: string        │
│ - cells: vector     │  │ - minValue: int     │  │ - name: string      │
│ - numSnakes: int    │  │ - maxValue: int     │  │ - currentPosition   │
│ - numLadders: int   │  │ - rng: mt19937      │  ├─────────────────────┤
├─────────────────────┤  │ - dist: uniform_int │  │ + getId(): string   │
│ + getSize(): int    │  ├─────────────────────┤  │ + getName(): string │
│ + getCell(pos): Cell│  │ + rollDice(): int   │  │ + getCurrentPos()   │
│ + getFinalPos(): int│  │ + getMaxPossible()  │  │ + moveTo(pos): void │
│ + isWinningPos()    │  │ + getMinPossible()  │  │ + reset(): void     │
│ + displayBoard()    │  └─────────────────────┘  └─────────────────────┘
│ - initializeSnakes  │
│   AndLadders()      │
└─────────────────────┘
            │
            │ 1..*
            ▼
┌─────────────────────────┐
│          CELL           │
├─────────────────────────┤
│ - position: int         │
│ - jump: unique_ptr<Jump>│
├─────────────────────────┤
│ + getPosition(): int    │
│ + hasJump(): bool       │
│ + setJump(Jump): void   │
│ + removeJump(): void    │
│ + getJump(): Jump*      │
│ + getFinalPosition()    │
│ + hasSnake(): bool      │
│ + hasLadder(): bool     │
└─────────────────────────┘
            │
            │ 0..1
            ▼
┌─────────────────────────┐
│          JUMP           │
├─────────────────────────┤
│ - startPosition: int    │
│ - endPosition: int      │
│ - type: JumpType        │
├─────────────────────────┤
│ + getStartPosition()    │
│ + getEndPosition()      │
│ + getType(): JumpType   │
│ + isSnake(): bool       │
│ + isLadder(): bool      │
│ + getJumpDistance(): int│
│ + getTypeName(): string │
└─────────────────────────┘
```

## Sequence Diagram - Game Play

```
┌──────┐     ┌────────┐     ┌────────┐     ┌────────┐     ┌────────┐
│ main │     │  Game  │     │  Dice  │     │ Board  │     │ Player │
└──┬───┘     └───┬────┘     └───┬────┘     └───┬────┘     └───┬────┘
   │             │              │              │              │
   │ play()      │              │              │              │
   │────────────>│              │              │              │
   │             │              │              │              │
   │             │ loop [!gameOver]            │              │
   │             │═════════════════════════════════════════════
   │             │              │              │              │
   │             │ rollDice()   │              │              │
   │             │─────────────>│              │              │
   │             │<─────────────│              │              │
   │             │ diceValue    │              │              │
   │             │              │              │              │
   │             │ getFinalPosition(pos)       │              │
   │             │────────────────────────────>│              │
   │             │<────────────────────────────│              │
   │             │ finalPos (after snake/ladder)              │
   │             │              │              │              │
   │             │              │              │  moveTo(pos) │
   │             │──────────────────────────────────────────>│
   │             │              │              │              │
   │             │ isWinningPosition(pos)      │              │
   │             │────────────────────────────>│              │
   │             │<────────────────────────────│              │
   │             │              │              │              │
   │             │═════════════════════════════════════════════
   │             │              │              │              │
   │<────────────│              │              │              │
   │  winner     │              │              │              │
```

## Component Diagram

```
┌─────────────────────────────────────────────────────────────────────────────────────┐
│                           SNAKE AND LADDER COMPONENTS                                │
└─────────────────────────────────────────────────────────────────────────────────────┘

                    ┌─────────────────────────────────────────┐
                    │              «component»                 │
                    │                 GAME                     │
                    │  ┌─────────────────────────────────────┐│
                    │  │         Game Controller             ││
                    │  │  - Turn management                  ││
                    │  │  - Win condition check              ││
                    │  │  - Player rotation                  ││
                    │  └─────────────────────────────────────┘│
                    └─────────────────────────────────────────┘
                         │              │              │
                         ▼              ▼              ▼
          ┌──────────────────┐ ┌──────────────────┐ ┌──────────────────┐
          │   «component»    │ │   «component»    │ │   «component»    │
          │      BOARD       │ │      DICE        │ │     PLAYER       │
          │ ┌──────────────┐ │ │ ┌──────────────┐ │ │ ┌──────────────┐ │
          │ │    Cells     │ │ │ │  Random Gen  │ │ │ │   Position   │ │
          │ ├──────────────┤ │ │ └──────────────┘ │ │ │   Tracking   │ │
          │ │    Jumps     │ │ │                  │ │ └──────────────┘ │
          │ │(Snakes/Ladders│ │                  │ │                  │
          │ └──────────────┘ │ │                  │ │                  │
          └──────────────────┘ └──────────────────┘ └──────────────────┘
```

## State Diagram - Game State

```
                    ┌─────────────────┐
                    │                 │
                    │    CREATED      │
                    │                 │
                    └────────┬────────┘
                             │
                    addPlayers()
                             │
                             ▼
                    ┌─────────────────┐
                    │                 │
                    │     READY       │
                    │                 │
                    └────────┬────────┘
                             │
                       play()│
                             ▼
                    ┌─────────────────┐
        ┌──────────>│                 │<──────────┐
        │           │   IN_PROGRESS   │           │
        │           │                 │           │
        │           └────────┬────────┘           │
        │                    │                    │
  playTurn()          player wins           playTurn()
  (no winner)                │              (no winner)
        │                    ▼                    │
        │           ┌─────────────────┐           │
        └───────────│                 │───────────┘
                    │    GAME_OVER    │
                    │                 │
                    └─────────────────┘
```

## State Diagram - Player Turn

```
                    ┌─────────────────┐
                    │                 │
                    │   WAITING_TURN  │◄──────────────────────────────┐
                    │                 │                               │
                    └────────┬────────┘                               │
                             │                                        │
                       turnStart                                      │
                             │                                        │
                             ▼                                        │
                    ┌─────────────────┐                               │
                    │                 │                               │
                    │  ROLLING_DICE   │                               │
                    │                 │                               │
                    └────────┬────────┘                               │
                             │                                        │
                      rollDice()                                      │
                             │                                        │
                             ▼                                        │
                    ┌─────────────────┐                               │
                    │                 │                               │
                    │     MOVING      │                               │
                    │                 │                               │
                    └────────┬────────┘                               │
                             │                                        │
           ┌─────────────────┼─────────────────┐                      │
           │                 │                 │                      │
     on ladder          on snake          normal move                 │
           │                 │                 │                      │
           ▼                 ▼                 ▼                      │
    ┌────────────┐   ┌────────────┐    ┌────────────┐                │
    │CLIMBING_UP │   │SLIDING_DOWN│    │   LANDED   │                │
    └─────┬──────┘   └─────┬──────┘    └──────┬─────┘                │
          │                │                  │                       │
          └────────────────┴──────────────────┘                       │
                           │                                          │
          ┌────────────────┼────────────────┐                         │
          │                                 │                         │
    reached 100                    not reached 100                    │
          │                                 │                         │
          ▼                                 └─────────────────────────┘
    ┌─────────────────┐
    │                 │
    │     WINNER      │
    │                 │
    └─────────────────┘
```

## Design Patterns Used

| Pattern | Where Used | Purpose |
|---------|------------|---------|
| **Composition** | Board contains Cells, Cells contain Jumps | Build complex structures from simpler objects |
| **Strategy** | Dice can have different rolling strategies | Allow different dice configurations |
| **State** | Game state management | Manage game flow states |
| **Factory** | Board initialization (random/predefined) | Create boards with different configurations |
| **Iterator** | Player queue rotation | Cycle through players for turns |

## Object Relationships

```
Game ──────────┬── has-a ──────> Board
               │
               ├── has-a ──────> Dice
               │
               └── has-many ───> Players


Board ── has-many ──> Cell


Cell ── has-a ──> Jump (optional)


Jump ── is-a ──> Snake OR Ladder (based on start/end positions)
```
