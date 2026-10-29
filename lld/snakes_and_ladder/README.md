# Snakes and Ladder Game - Low Level Design (C++)

## Design Patterns Used
- **Factory Pattern**: ObstacleFactory creates Snake or Ladder objects
- **Template Method**: Obstacle abstract class with getDestination() as template
- **Composition**: Board contains Cells, Cells contain optional Obstacles

## Class Diagram

```mermaid
classDiagram
    class ObstacleType {
        <<enumeration>>
        SNAKE
        LADDER
    }
    
    class Obstacle {
        <<abstract>>
        #int start
        #int end
        +getStart() int
        +getEnd() int
        +getDestination()* int
        +getType()* ObstacleType
    }
    
    class Snake {
        +getDestination() int
        +getType() ObstacleType
    }
    
    class Ladder {
        +getDestination() int
        +getType() ObstacleType
    }
    
    class Cell {
        -int position
        -Obstacle* obstacle
        +getPosition() int
        +setObstacle(Obstacle* obs)
        +getObstacle() Obstacle*
        +hasObstacle() bool
    }
    
    class Board {
        -int size
        -vector~vector~Cell~~ grid
        +getSize() int
        +getCell(int position) Cell*
        +placeObstacle(Obstacle* obstacle)
        +display()
    }
    
    class Player {
        -string name
        -int position
        +getName() string
        +getPosition() int
        +setPosition(int pos)
    }
    
    class Dice {
        -int numDice
        -random_device rd
        +roll() int
    }
    
    class GameService {
        -Board* board
        -Dice* dice
        -queue~Player*~ players
        +addPlayer(Player* player)
        +startGame()
        -playTurn(Player* player)
        -calculateNewPosition(int current, int roll) int
    }
    
    class ObstacleFactory {
        +create(ObstacleType type, int start, int end)$ Obstacle*
    }

    Obstacle <|-- Snake
    Obstacle <|-- Ladder
    
    Cell --> Obstacle
    Board --> Cell
    
    GameService --> Board
    GameService --> Dice
    GameService --> Player
    
    ObstacleFactory ..> Obstacle : creates
```

## Game Flow

```mermaid
flowchart TD
    A[Start Game] --> B[Initialize Board]
    B --> C[Place Snakes & Ladders]
    C --> D[Add Players to Queue]
    D --> E{Game Over?}
    E -->|No| F[Get Next Player]
    F --> G[Roll Dice]
    G --> H[Calculate New Position]
    H --> I{Position > Board Size?}
    I -->|Yes| J[Stay at Current Position]
    I -->|No| K[Move to New Position]
    K --> L{Has Obstacle?}
    L -->|Snake| M[Move Down to End]
    L -->|Ladder| N[Move Up to End]
    L -->|No| O[Stay at Position]
    M --> P[Update Player Position]
    N --> P
    O --> P
    J --> P
    P --> Q{Player Won?}
    Q -->|Yes| R[Declare Winner]
    Q -->|No| S[Add Player Back to Queue]
    S --> E
    R --> T[End Game]
    E -->|Yes| T
```

## Sequence Diagram

```mermaid
sequenceDiagram
    participant Main
    participant Game as GameService
    participant Board
    participant Dice
    participant Player
    participant Cell
    participant Obstacle
    
    Main->>Game: startGame()
    loop Until Winner
        Game->>Game: getNextPlayer()
        Game->>Dice: roll()
        Dice-->>Game: diceValue
        Game->>Game: calculateNewPosition(current, diceValue)
        Game->>Board: getCell(newPosition)
        Board-->>Game: cell
        Game->>Cell: hasObstacle()
        alt Has Obstacle
            Cell-->>Game: true
            Game->>Cell: getObstacle()
            Cell-->>Game: obstacle
            Game->>Obstacle: getDestination()
            Obstacle-->>Game: finalPosition
        else No Obstacle
            Cell-->>Game: false
        end
        Game->>Player: setPosition(finalPosition)
        alt Position == BoardSize
            Game-->>Main: Winner Found!
        else Not Won
            Game->>Game: addPlayerBackToQueue()
        end
    end
```

## Board Layout

```mermaid
graph TD
    subgraph "Snakes & Ladder Board (10x10)"
        direction TB
        R1[100 99 98 97 96 95 94 93 92 91]
        R2[81 82 83 84 85 86 87 88 89 90]
        R3[80 79 78 77 76 75 74 73 72 71]
        R4[61 62 63 64 65 66 67 68 69 70]
        R5[60 59 58 57 56 55 54 53 52 51]
        R6[41 42 43 44 45 46 47 48 49 50]
        R7[40 39 38 37 36 35 34 33 32 31]
        R8[21 22 23 24 25 26 27 28 29 30]
        R9[20 19 18 17 16 15 14 13 12 11]
        R10[1  2  3  4  5  6  7  8  9  10]
    end
    
    style R1 fill:#90EE90
    style R10 fill:#FFB6C1
```

## Files Structure
```
snakes_and_ladder/
├── enums/
│   └── ObstacleType.hpp
├── model/
│   ├── Obstacle.hpp
│   ├── Snake.hpp
│   ├── Ladder.hpp
│   ├── Cell.hpp
│   ├── Board.hpp
│   ├── Player.hpp
│   └── Dice.hpp
├── factory/
│   └── ObstacleFactory.hpp
├── service/
│   └── GameService.hpp
└── main.cpp
```
