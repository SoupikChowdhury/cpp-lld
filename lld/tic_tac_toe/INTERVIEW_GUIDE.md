# Tic Tac Toe - LLD Interview Guide

A step-by-step framework for delivering the Tic Tac Toe game low-level design interview.

---

## 📁 Code Reference

| File | Purpose | Key Functions |
|------|---------|---------------|
| [TicTacToeGame.hpp](TicTacToeGame.hpp) | Game controller | `initializePlayers()`, `startGame()`, `isWinner()`, `checkWin()` |
| [main.cpp](main.cpp) | Entry point | Game initialization and execution |
| [Model/Board.hpp](Model/Board.hpp) | Game board with grid | `placePiece()`, `checkWin()`, `isFull()`, `getRow()`, `getColumn()`, `getMainDiagonal()`, `getAntiDiagonal()` |
| [Model/Player.hpp](Model/Player.hpp) | Player entity | `getName()`, `getPieceType()`, `getSymbol()` |
| [Model/PlayingPiece.hpp](Model/PlayingPiece.hpp) | Abstract base for pieces | `getPieceType()`, `getSymbol()` |
| [Model/PlayingPieceX.hpp](Model/PlayingPieceX.hpp) | Concrete X piece | `getSymbol()` returns 'X' |
| [Model/PlayingPieceO.hpp](Model/PlayingPieceO.hpp) | Concrete O piece | `getSymbol()` returns 'O' |
| [Model/PieceType.hpp](Model/PieceType.hpp) | Piece type enum | `X`, `O` |

---

## 🎯 Design Patterns Used

| Pattern | Implementation Location | Purpose |
|---------|------------------------|----------|
| **Polymorphism** | [Model/PlayingPiece.hpp](Model/PlayingPiece.hpp) base with PlayingPieceX, PlayingPieceO | Different piece types |
| **Composition** | Game contains Board and Players | Object relationships |
| **Queue-based Turn Management** | [TicTacToeGame.hpp](TicTacToeGame.hpp) - `std::deque<Player>` | Rotate players |

---

## 1) Requirements (~5 minutes)

### Clarifying Questions to Ask
- "Is this a standard 3x3 board or should it support NxN?"
- "How many players? Standard 2 or more?"
- "What defines a win? N in a row, or different rules?"
- "Should we handle draw detection?"
- "Do we need undo/redo functionality?"

### Requirements

```
Requirements:
1. Two players alternate placing X and O on a 3x3 grid.
2. A player wins by completing a row, column, or diagonal.
3. The game ends in a draw if all nine cells are filled with no winner.
4. Invalid moves should be rejected (placing on an occupied cell, acting after game over).
5. The system should provide a way to query current game state and reset the game.

Out of Scope:
- UI/rendering layer
- AI opponent or move suggestions
- Networked multiplayer
- Variable board sizes (NxN grids)
- Undo/redo functionality
```

---

## 2) Entities and Relationships (~3 minutes)

### Identify Entities

Scan the requirements and pull out meaningful nouns:

- **Game** - Orchestrator, manages game flow and state
- **Board** - Maintains the grid and cell states
- **Player** - Represents each participant
- **PlayingPiece** - X or O marker

### Define Relationships

```
Entities:
- Game (orchestrator)
- Board
- Player
- PlayingPiece (abstract: PlayingPieceX, PlayingPieceO)

Relationships:
- Game -> Board (1:1)
- Game -> Player (1:2)
- Player -> PlayingPiece (1:1)
- Board contains grid of PieceType
```

---

## 3) Class Design (~10-15 minutes)

### Game (Orchestrator)

| Requirement | State Needed |
|-------------|--------------|
| Track the board | board: Board |
| Track both players | playerX, playerO: Player |
| Track whose turn | currentPlayer: Player |
| Track game outcome | state: GameState, winner: Player? |

```
class Game:
  - board: Board
  - playerX: Player
  - playerO: Player
  - currentPlayer: Player
  - state: GameState (IN_PROGRESS, WON, DRAW)
  - winner: Player? (null if no winner)

  + makeMove(player: Player, row: int, col: int) -> bool
  + getCurrentPlayer() -> Player
  + getGameState() -> GameState
  + getWinner() -> Player?
  + getBoard() -> Board
  + resetGame()
```

### Board

| Requirement | State Needed |
|-------------|--------------|
| Store the grid | grid: 2D array of PieceType |
| Track board size | size: int |

```
class Board:
  - size: int
  - grid: vector<vector<PieceType>>

  + placePiece(row: int, col: int, piece: PieceType) -> bool
  + isCellEmpty(row: int, col: int) -> bool
  + isFull() -> bool
  + getRow(row: int) -> vector<PieceType>
  + getColumn(col: int) -> vector<PieceType>
  + getMainDiagonal() -> vector<PieceType>
  + getAntiDiagonal() -> vector<PieceType>
  + print()
```

### Player

```
class Player:
  - name: string
  - piece: PlayingPiece

  + getName() -> string
  + getPieceType() -> PieceType
  + getSymbol() -> char
```

### PlayingPiece (Abstract)

```
class PlayingPiece:
  - pieceType: PieceType

  + getSymbol() -> char
  + getPieceType() -> PieceType

class PlayingPieceX : PlayingPiece
  + getSymbol() -> 'X'

class PlayingPieceO : PlayingPiece
  + getSymbol() -> 'O'
```

---

## 4) Implementation (~10 minutes)

### Key Method: Game.makeMove()

```
makeMove(player, row, col) -> bool
    // Validation
    if state != IN_PROGRESS
        return false  // Game already over
    if player != currentPlayer
        return false  // Not your turn
    if not board.isCellEmpty(row, col)
        return false  // Cell occupied

    // Place the piece
    board.placePiece(row, col, player.getPieceType())

    // Check for win
    if checkWinner(row, col, player.getPieceType())
        state = WON
        winner = player
    else if board.isFull()
        state = DRAW
    else
        // Switch turns
        currentPlayer = (player == playerX) ? playerO : playerX

    return true
```

### Key Method: Game.checkWinner() - O(N) approach

```
checkWinner(row, col, piece) -> bool
    size = board.getSize()

    // Check row - O(N)
    if isWinningLine(board.getRow(row), piece)
        return true

    // Check column - O(N)
    if isWinningLine(board.getColumn(col), piece)
        return true

    // Check main diagonal (if on it) - O(N)
    if row == col
        if isWinningLine(board.getMainDiagonal(), piece)
            return true

    // Check anti-diagonal (if on it) - O(N)
    if row + col == size - 1
        if isWinningLine(board.getAntiDiagonal(), piece)
            return true

    return false

isWinningLine(line, piece) -> bool
    for cell in line:
        if cell != piece
            return false
    return true
```

### Key Method: Board.placePiece()

```
placePiece(row, col, piece) -> bool
    if row < 0 or row >= size or col < 0 or col >= size
        return false
    if grid[row][col] != EMPTY
        return false
    
    grid[row][col] = piece
    return true
```

### Verification: Walk Through a Scenario

```
Initial: 3x3 empty board, currentPlayer = X

1. makeMove(X, 0, 0) → Place X at (0,0), currentPlayer = O
   Board: [X, _, _]
          [_, _, _]
          [_, _, _]

2. makeMove(O, 1, 1) → Place O at (1,1), currentPlayer = X
   Board: [X, _, _]
          [_, O, _]
          [_, _, _]

3. makeMove(X, 0, 1) → Place X at (0,1), currentPlayer = O
4. makeMove(O, 2, 2) → Place O at (2,2), currentPlayer = X
5. makeMove(X, 0, 2) → Place X at (0,2)
   Board: [X, X, X]  ← Row 0 complete!
          [_, O, _]
          [_, _, O]
   
   checkWinner(0, 2, X) → Row 0 all X → true
   state = WON, winner = X

6. makeMove(O, 2, 0) → Returns false (game over)
```

---

## 5) Extensibility (~5 minutes)

### "How would you add undo functionality?"

> "All state changes happen in makeMove(). I'd introduce a Command pattern with a move history stack. Each move records the previous state. Undo pops the stack and reverts."

```
class Move:
  - row, col: int
  - piece: PieceType
  - player: Player

class Game:
  - undoStack: stack<Move>
  - redoStack: stack<Move>

  + undo()
      last = undoStack.pop()
      board.removePiece(last.row, last.col)
      redoStack.push(last)
      // Switch player back
```

### "How would you extend to NxN board?"

> "Board already has a size parameter. The win check logic works for any N. Just make size configurable in the constructor and adjust the UI printing."

### "How would you add an AI opponent?"

> "Create an AIPlayer subclass that implements getNextMove() using Minimax algorithm. The game checks if currentPlayer is AI and auto-triggers the move."

```
class AIPlayer : Player
  + getNextMove(board: Board) -> pair<int, int>
      return minimax(board, piece, isMaximizing)
```

### "How would you support a 3-player game?"

> "Add a third PieceType (Z), third player, and use a queue instead of toggle for turn management. Win condition remains the same—any player getting N in a row wins."

---

## Design Patterns Summary

| Pattern | Where | Why |
|---------|-------|-----|
| **Factory** | PlayingPieceFactory | Centralize piece creation |
| **Strategy** | WinStrategy (extension) | Different win conditions (Connect 4) |
| **Command** | Move history (extension) | Undo/redo functionality |

---

## Time/Space Complexity

| Operation | Time | Space |
|-----------|------|-------|
| Make Move | O(N) | O(1) |
| Check Win | O(N) | O(N) |
| Initialize | O(N²) | O(N²) |
| Display Board | O(N²) | O(1) |

Where N = board size

---

## Edge Cases

1. **Invalid cell** - Out of bounds coordinates
2. **Occupied cell** - Cell already has piece
3. **Move after game over** - Reject move
4. **Draw detection** - Board full, no winner
5. **Win on last move** - Both win and draw possible
