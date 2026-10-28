# Snake and Ladder Game - LLD Interview Guide

A step-by-step framework for delivering the Snake and Ladder game low-level design interview.

---

## 📁 Code Reference

| File | Purpose | Key Functions |
|------|---------|---------------|
| [Game.hpp](Game.hpp) | Game controller | `addPlayer()`, `playGame()`, `isGameOver()`, `getWinner()` |
| [Board.hpp](Board.hpp) | Game board with cells | `getFinalPosition()`, `addSnakeOrLadder()`, `initializeBoard()` |
| [Cell.hpp](Cell.hpp) | Individual cell | `getJump()`, `setJump()`, `hasJump()` |
| [Jump.hpp](Jump.hpp) | Snake or ladder | `getStartPosition()`, `getEndPosition()`, `isSnake()`, `isLadder()` |
| [Dice.hpp](Dice.hpp) | Random dice roll | `rollDice()` |
| [Player.hpp](Player.hpp) | Player entity | `moveTo()`, `getCurrentPosition()`, `getName()` |

---

## 🎯 Design Patterns Used

| Pattern | Implementation Location | Purpose |
|---------|------------------------|----------|
| **Composition** | Game contains Board, Dice, and Players | Object relationships |
| **Queue-based Turn Management** | [Game.hpp](Game.hpp) - `std::queue<Player>` for player turns | Rotate players fairly |

---

## 1) Requirements (~5 minutes)

### Clarifying Questions to Ask
- "What's the board size? Standard 100 or configurable?"
- "How many players should we support?"
- "Should snakes and ladders be randomly placed or configurable?"
- "What dice configuration? Single die, multiple dice?"
- "Is an exact roll needed to win, or can we exceed the final cell?"

### Requirements

```
Requirements:
1. Board has 100 cells (or configurable NxN).
2. Configurable number of snakes and ladders.
3. 2+ players take turns rolling dice and moving.
4. Landing on snake head moves player down to tail.
5. Landing on ladder bottom moves player up to top.
6. First player to reach exactly the final cell wins.
7. If dice roll exceeds the board, player stays in place.

Out of Scope:
- Graphics/UI rendering
- Network multiplayer
- Game save/load
- Power-ups and special cells
- Betting/gambling mechanics
- Roll again on 6
```

---

## 2) Entities and Relationships (~3 minutes)

### Identify Entities

Scan the requirements and pull out meaningful nouns:

- **Game** - Main controller, manages turns
- **Board** - Grid with snakes and ladders
- **Cell** - Single position, may have a Jump
- **Jump** - Snake or ladder (start → end)
- **Player** - Current position on board
- **Dice** - Random number generator

### Define Relationships

```
Entities:
- Game (orchestrator)
- Board
- Cell
- Jump (snake or ladder)
- Player
- Dice

Relationships:
- Game -> Board (1:1)
- Game -> Dice (1:1)
- Game -> Player (1:N, queue for turns)
- Board -> Cell (1:N)
- Cell -> Jump (1:0..1, optional)
```

---

## 3) Class Design (~10-15 minutes)

### Game (Orchestrator)

| Requirement | State Needed |
|-------------|--------------|
| Manage board | board |
| Manage dice | dice |
| Track players | players: queue |
| Track winner | winner |
| Game state | isGameOver |

```
class Game:
  - board: Board
  - dice: Dice
  - players: queue<Player>
  - winner: Player?
  - isGameOver: bool

  + addPlayer(player: Player)
  + playTurn() -> TurnResult
  + play() -> Player  // Play until winner
  + getWinner() -> Player?
```

### Board

| Requirement | State Needed |
|-------------|--------------|
| Store cells | cells: vector<Cell> |
| Track size | size: int |

```
class Board:
  - size: int
  - cells: vector<Cell>

  + getCell(position: int) -> Cell
  + getFinalPosition(position: int) -> int  // After any jumps
  + isWinningPosition(position: int) -> bool
  + getSize() -> int
```

### Cell

```
class Cell:
  - position: int
  - jump: Jump?  // Optional snake or ladder

  + getPosition() -> int
  + hasJump() -> bool
  + getJump() -> Jump?
  + getFinalPosition() -> int
      if hasJump()
          return jump.getEndPosition()
      return position
```

### Jump

```
class Jump:
  - startPosition: int
  - endPosition: int

  + getStartPosition() -> int
  + getEndPosition() -> int
  + isSnake() -> bool
      return endPosition < startPosition
  + isLadder() -> bool
      return endPosition > startPosition
```

### Player

```
class Player:
  - playerId: string
  - name: string
  - currentPosition: int  // 0 = not started

  + getName() -> string
  + getCurrentPosition() -> int
  + setPosition(position: int)
```

### Dice

```
class Dice:
  - numDice: int
  - minValue: int
  - maxValue: int
  - rng: RandomGenerator

  + roll() -> int
      total = 0
      for i = 1 to numDice:
          total += random(minValue, maxValue)
      return total
```

---

## 4) Implementation (~10 minutes)

### Key Method: Game.playTurn()

```
playTurn() -> TurnResult
    if isGameOver
        return TurnResult(gameOver = true, winner = winner)
    
    // Get current player
    currentPlayer = players.front()
    players.pop()
    
    // Roll dice
    diceValue = dice.roll()
    currentPosition = currentPlayer.getCurrentPosition()
    newPosition = currentPosition + diceValue
    
    // Check if move is valid
    if newPosition > board.getSize()
        // Cannot move, need exact roll
        players.push(currentPlayer)  // Back to queue
        return TurnResult(
            player = currentPlayer,
            diceValue = diceValue,
            moved = false,
            reason = "Need exact roll to win"
        )
    
    // Get final position (after any snakes/ladders)
    finalPosition = board.getFinalPosition(newPosition)
    currentPlayer.setPosition(finalPosition)
    
    // Check for win
    if board.isWinningPosition(finalPosition)
        isGameOver = true
        winner = currentPlayer
        return TurnResult(
            player = currentPlayer,
            diceValue = diceValue,
            from = currentPosition,
            to = finalPosition,
            won = true
        )
    
    // Add player back to queue
    players.push(currentPlayer)
    
    return TurnResult(
        player = currentPlayer,
        diceValue = diceValue,
        from = currentPosition,
        to = finalPosition
    )
```

### Key Method: Board.getFinalPosition()

```
getFinalPosition(position: int) -> int
    cell = cells[position]
    
    if not cell.hasJump()
        return position
    
    // Follow the jump (snake or ladder)
    jump = cell.getJump()
    finalPosition = jump.getEndPosition()
    
    // Recursively check for chain (ladder → snake → ladder)
    return getFinalPosition(finalPosition)
```

### Key Method: Board constructor (Random placement)

```
Board(size: int, numSnakes: int, numLadders: int)
    this.size = size
    cells = vector<Cell>(size + 1)  // 1-indexed
    
    usedPositions = set<int>()
    usedPositions.insert(1)     // No jump at start
    usedPositions.insert(size)  // No jump at end
    
    // Add snakes
    snakesAdded = 0
    while snakesAdded < numSnakes:
        start = random(2, size - 1)
        if usedPositions.contains(start)
            continue
        
        end = random(1, start - 1)
        if usedPositions.contains(end)
            continue
        
        usedPositions.insert(start)
        cells[start].setJump(new Jump(start, end))  // Snake goes down
        snakesAdded++
    
    // Add ladders (similar, but end > start)
    laddersAdded = 0
    while laddersAdded < numLadders:
        start = random(2, size - 1)
        if usedPositions.contains(start)
            continue
        
        end = random(start + 1, size)
        if usedPositions.contains(end)
            continue
        
        usedPositions.insert(start)
        cells[start].setJump(new Jump(start, end))  // Ladder goes up
        laddersAdded++
```

### Verification: Walk Through a Scenario

```
Setup: Board size = 20, Snakes: 18→5, 15→9, Ladders: 3→14, 6→16
Players: Alice, Bob (queue order)

Turn 1: Alice's turn
- Position: 0 (start)
- Rolls: 3
- New position: 0 + 3 = 3
- Cell 3 has ladder to 14
- Final position: 14
- Alice at 14

Turn 2: Bob's turn
- Position: 0
- Rolls: 6
- New position: 0 + 6 = 6
- Cell 6 has ladder to 16
- Final position: 16
- Bob at 16

Turn 3: Alice's turn
- Position: 14
- Rolls: 4
- New position: 14 + 4 = 18
- Cell 18 has snake to 5
- Final position: 5
- Alice at 5 (ouch!)

Turn 4: Bob's turn
- Position: 16
- Rolls: 5
- New position: 16 + 5 = 21 > 20
- Cannot move (need exact)
- Bob stays at 16

Turn 5: Alice's turn
- Position: 5
- Rolls: 6
- New position: 5 + 6 = 11
- No jump
- Alice at 11

... continues until someone reaches exactly 20 ...
```

---

## 5) Extensibility (~5 minutes)

### "How would you add 'roll again on 6' rule?"

> "Modify playTurn() to check dice value. If 6, don't push player back to queue at end—let them roll again. Add a counter to limit consecutive 6s (e.g., max 3)."

```
playTurn() -> TurnResult
    // ... existing logic ...
    
    diceValue = dice.roll()
    
    // ... move player ...
    
    // Check for roll again
    if diceValue == 6 and not won and consecutiveSixes < 3
        consecutiveSixes++
        // Don't add player back to queue
        return TurnResult(rollAgain = true, ...)
    
    consecutiveSixes = 0
    players.push(currentPlayer)
```

### "How would you support multiple dice?"

> "Dice class already supports numDice parameter. Roll each die and sum. For 'doubles' rule, track individual rolls and check if they match."

```
class Dice:
  + rollMultiple() -> vector<int>
      rolls = []
      for i = 1 to numDice:
          rolls.push(random(1, 6))
      return rolls
  
  + isDoubles(rolls) -> bool
      return rolls[0] == rolls[1]
```

### "How would you add power-ups (immunity, teleport)?"

> "Create PowerUp interface with apply() method. Add powerUp field to Cell. When landing on cell with power-up, apply effect to player."

```
class PowerUp:
  + apply(player: Player, game: Game)

class ImmunityPowerUp : PowerUp
  + apply(player, game)
      player.setImmune(true)  // Ignore next snake

class TeleportPowerUp : PowerUp
  + apply(player, game)
      destination = random(player.getPosition(), board.getSize())
      player.setPosition(destination)
```

### "How would you implement game save/load?"

> "Serialize game state (player positions, current turn, board configuration) to JSON. Store in file. On load, reconstruct Game object from saved state."

```
class GameState:
  - boardConfig: BoardConfig
  - playerPositions: map<string, int>
  - currentPlayerIndex: int
  
  + serialize() -> string
  + static deserialize(json: string) -> GameState
```

---

## Design Patterns Summary

| Pattern | Where | Why |
|---------|-------|-----|
| **Composition** | Game contains Board, Dice, Players | Build game from components |
| **Factory** | BoardFactory (extension) | Create different board configurations |
| **Strategy** | DiceStrategy (extension) | Different dice behaviors |
| **Observer** | GameObserver (extension) | Notify UI of moves |

---

## Time/Space Complexity

| Operation | Time | Space |
|-----------|------|-------|
| Initialize Board | O(S + L) | O(B) |
| Roll Dice | O(D) | O(1) |
| Play Turn | O(1)* | O(1) |
| Check Jump | O(1) | O(1) |
| Full Game | O(T) | O(B + P) |

Where:
- S = Snakes, L = Ladders, B = Board size
- D = Number of dice, P = Players
- T = Total turns (random, depends on dice rolls)

*O(1) assuming no chain of jumps; worst case O(J) for jump chains

---

## Game Flow

```
Initialize Game
      ↓
Add Players to Queue
      ↓
┌────────────────┐
│  Play Turn     │◄─────────┐
├────────────────┤          │
│ 1. Get player  │          │
│ 2. Roll dice   │          │
│ 3. Calculate   │          │
│    new pos     │          │
│ 4. Apply jumps │          │
│ 5. Check win   │          │
└────────────────┘          │
      │                     │
      ├── Not won ──────────┘
      │
      └── Won → Game Over → Announce Winner
```
